#!/usr/bin/env node
/**
 * tt-decomp Phase 5 MCP Server  (v3 — Full Metadata Integration)
 * ================================================================
 * All 14 config files are now used.  New in v3:
 *
 *  STRING READING from default_base.bin
 *    resolve_address() now shows actual string content for any .rdata address.
 *    6,785 readable strings preloaded at startup, including all debug/assert
 *    messages (e.g. "pongPlayer::Activate() - creature already activated").
 *
 *  STRING-DERIVED METHOD NAME MAP
 *    Binary scanned for Class::Method() patterns → 140 classes × N methods
 *    surfaced in get_class_context as "METHOD NAMES FROM DEBUG STRINGS".
 *
 *  VTABLE SLOT FUNCTION ADDRESSES
 *    Every vtable slot now shows its function address, not just the symbol name.
 *    Lets you cross-reference with resolve_address.
 *
 *  UNION/BITFIELD DETECTION in offset_cluster_map
 *    Offsets with mixed access widths (e.g. U8+U32 at same offset) are flagged
 *    as potential unions or bitfields in get_class_context.
 *
 *  ALL debug_string_field_map entries shown (was capped at 10)
 *
 *  symbols.txt supplement
 *    One function (pongCameraMgr_checkAngleLimits @ 0x821F3090) has a better
 *    name in symbols.txt than in master_symbol_map.  Patched at load time.
 *
 *  get_project_info tool (NEW)
 *    Returns binary metadata: internal name, build date, entry point, compiler
 *    flags, compilation unit statuses (NonMatching tracker), binary stats.
 *
 *  10 tools total (resolve_address + get_project_info are v2/v3 additions).
 */

import { Server } from "@modelcontextprotocol/sdk/server/index.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";
import {
  CallToolRequestSchema,
  ListToolsRequestSchema,
} from "@modelcontextprotocol/sdk/types.js";
import * as fs from "fs";
import * as path from "path";
import { execSync } from "child_process";

// ─────────────────────────────────────────────────────────────────────────────
// Project paths
// ─────────────────────────────────────────────────────────────────────────────

const BASE        = "/Users/Ozordi/Downloads/tt-decomp";
const CFG         = `${BASE}/config/434C4803`;
const ORIG        = `${BASE}/orig/434C4803`;
const PASS5_FINAL = `${BASE}/recomp/structured_pass5_final`;
const PSEUDO_DIR  = `${BASE}/orig/pseudocode`;
const SRC_DIR     = `${BASE}/src`;

// ─────────────────────────────────────────────────────────────────────────────
// Lazy-loaded indexes
// ─────────────────────────────────────────────────────────────────────────────

let _msm         = null;
let _splits      = null;
let _vtable      = null;
let _cluster     = null;
let _dbgFields   = null;
let _rtti        = null;
let _rttiHier    = null;
let _sda         = null;
let _sdaByAddr   = null;
let _funcIndex   = null;
let _stringCache = null;  // NEW: addr -> string content from binary
let _methodMap   = null;  // NEW: className -> { methodName -> [{addr,string}] }
let _binRaw      = null;  // NEW: raw binary buffer
let _symbolFuncs = null;  // parsed symbols.txt function list: [{name,addr,size}]

/** Parse symbols.txt and return all type:function entries as an array */
function symbolFuncs() {
  if (_symbolFuncs) return _symbolFuncs;
  const SYM_FILE = `${CFG}/symbols.txt`;
  _symbolFuncs = [];
  const RE = /^(\S+)\s*=\s*\.text:(0x[0-9A-Fa-f]+);.*?size:(0x[0-9A-Fa-f]+)/;
  for (const line of fs.readFileSync(SYM_FILE, 'utf8').split('\n')) {
    if (!line.includes('type:function')) continue;
    const m = RE.exec(line);
    if (!m) continue;
    _symbolFuncs.push({ name: m[1], addr: m[2], size: parseInt(m[3], 16) });
  }
  return _symbolFuncs;
}

function msm() {
  if (!_msm) {
    _msm = JSON.parse(fs.readFileSync(`${CFG}/master_symbol_map.json`, "utf8"));
    // Patch the one symbols.txt regression: pongCameraMgr_checkAngleLimits
    const PATCH = { name: "pongCameraMgr_checkAngleLimits", addr: "0x821f3090" };
    if (_msm.by_address[PATCH.addr]?.name === "pongCameraMgr_3090") {
      _msm.by_address[PATCH.addr].name = PATCH.name;
      _msm.by_name[PATCH.name] = PATCH.addr;
      delete _msm.by_name["pongCameraMgr_3090"];
    }
  }
  return _msm;
}
function splits()    { if (!_splits)    _splits    = JSON.parse(fs.readFileSync(`${CFG}/splits_addr_map.json`,       "utf8")); return _splits; }
function vtable()    { if (!_vtable)    _vtable    = JSON.parse(fs.readFileSync(`${CFG}/vtable_layout_map.json`,     "utf8")); return _vtable; }
function cluster()   { if (!_cluster)   { const p=`${CFG}/offset_cluster_map.json`; _cluster = fs.existsSync(p)?JSON.parse(fs.readFileSync(p,"utf8")):{}; } return _cluster; }
function dbgFields() { if (!_dbgFields) _dbgFields = JSON.parse(fs.readFileSync(`${CFG}/debug_string_field_map.json`,"utf8")); return _dbgFields; }
function rtti()      { if (!_rtti)      _rtti      = JSON.parse(fs.readFileSync(`${CFG}/rtti_vtable_map.json`,      "utf8")); return _rtti; }
function rttiHier()  { if (!_rttiHier)  _rttiHier  = JSON.parse(fs.readFileSync(`${CFG}/rtti_class_hierarchy.json`, "utf8")); return _rttiHier; }

function sda() {
  if (!_sda) _sda = JSON.parse(fs.readFileSync(`${CFG}/sda_resolution_map.json`, "utf8"));
  return _sda;
}
function sdaByAddr() {
  if (!_sdaByAddr) {
    _sdaByAddr = {};
    for (const [offset, entry] of Object.entries(sda())) {
      if (entry.addr) _sdaByAddr[entry.addr.toLowerCase()] = { ...entry, sda_offset: offset };
    }
  }
  return _sdaByAddr;
}

// ─────────────────────────────────────────────────────────────────────────────
// Binary string reading — new in v3
// ─────────────────────────────────────────────────────────────────────────────

const LOAD_BASE = 0x82000000;

function binRaw() {
  if (!_binRaw) {
    const p = `${ORIG}/default_base.bin`;
    if (fs.existsSync(p)) _binRaw = fs.readFileSync(p);
  }
  return _binRaw;
}

/** Read a null-terminated ASCII string from a virtual address */
function readCStr(vaddr, maxLen = 512) {
  const buf = binRaw();
  if (!buf) return null;
  const off = vaddr - LOAD_BASE;
  if (off < 0 || off >= buf.length) return null;
  const end = buf.indexOf(0, off);
  if (end < 0 || end - off > maxLen) return null;
  try {
    const s = buf.slice(off, end).toString("ascii");
    // Printable ASCII only
    for (let i = 0; i < s.length; i++) {
      const c = s.charCodeAt(i);
      if (c < 32 || c > 126) return null;
    }
    return s || null;
  } catch { return null; }
}

/** Lazy-build string cache: addr_hex -> string content */
function stringCache() {
  if (_stringCache) return _stringCache;
  _stringCache = new Map();
  const byAddr = msm().by_address;
  for (const [addr, info] of Object.entries(byAddr)) {
    if (info.section !== ".rdata") continue;
    const s = readCStr(parseInt(addr, 16));
    if (s && s.length >= 3) _stringCache.set(addr, s);
  }
  return _stringCache;
}

/**
 * Lazy-build method name map from debug strings.
 * Scans for "ClassName::MethodName(..." patterns.
 * Returns: Map<className, Map<methodName, [{addr, string}]>>
 */
function methodMap() {
  if (_methodMap) return _methodMap;
  _methodMap = new Map();
  const METHOD_RE = /^([A-Za-z_][A-Za-z0-9_:]+)::([A-Za-z_][A-Za-z0-9_]+)\(/;
  for (const [addr, s] of stringCache()) {
    const m = METHOD_RE.exec(s);
    if (!m) continue;
    let cls = m[1], method = m[2];
    // Reject common false-positives (truncated class names, single-word)
    if (cls.length < 4 || !cls.includes("_") && cls[0] !== cls[0].toUpperCase()) continue;
    if (!_methodMap.has(cls)) _methodMap.set(cls, new Map());
    const methods = _methodMap.get(cls);
    if (!methods.has(method)) methods.set(method, []);
    methods.get(method).push({ addr, string: s });
  }
  return _methodMap;
}

// ─────────────────────────────────────────────────────────────────────────────
// RTTI helpers
// ─────────────────────────────────────────────────────────────────────────────

function rttiClassName(addrStr) {
  const norm = "0x" + addrStr.replace(/^0x/i, "").toLowerCase();
  const r = rtti();
  for (const [k, v] of Object.entries(r)) {
    if (k.toLowerCase() === norm) return v;
  }
  return null;
}

function rttiEntry(className) {
  const hier = rttiHier();
  if (hier[className]) return hier[className];
  const bare = className.split("::").pop();
  for (const [k, v] of Object.entries(hier)) {
    if (k === bare || k.endsWith("::" + bare)) return { ...v, _found_as: k };
  }
  return null;
}

function rttiOriginalName(className) {
  const hier = rttiHier();
  if (hier[className]) return className;
  const r = rtti();
  for (const v of Object.values(r)) {
    if (v === className || v.endsWith("::" + className)) return v;
  }
  return null;
}

// ─────────────────────────────────────────────────────────────────────────────
// Function index
// ─────────────────────────────────────────────────────────────────────────────

function funcIndex() {
  if (_funcIndex) return _funcIndex;
  _funcIndex = new Map();
  const FUNC_RE = /^PPC_FUNC_IMPL\(__imp__([a-zA-Z_][a-zA-Z0-9_]*)\)/;
  const files = fs.readdirSync(PASS5_FINAL).filter(f => f.endsWith(".cpp"));
  for (const fname of files) {
    const fullPath = path.join(PASS5_FINAL, fname);
    const lines = fs.readFileSync(fullPath, "utf8").split("\n");
    let current = null;
    for (let i = 0; i < lines.length; i++) {
      const m = FUNC_RE.exec(lines[i]);
      if (m) {
        if (current) {
          current.lineEnd = i - 1;
          current.rawText = lines.slice(current.lineStart, i).join("\n");
          _funcIndex.set(current.name.toLowerCase(), current);
        }
        current = { name: m[1], file: fname, lineStart: i, lineEnd: lines.length - 1, rawText: "" };
      }
    }
    if (current) {
      current.rawText = lines.slice(current.lineStart).join("\n");
      _funcIndex.set(current.name.toLowerCase(), current);
    }
  }
  return _funcIndex;
}

// ─────────────────────────────────────────────────────────────────────────────
// Core helpers
// ─────────────────────────────────────────────────────────────────────────────

function resolveName(input) {
  let s = (input || "").trim().replace(/^__imp__/, "");
  if (/^(0x)?[0-9A-Fa-f]{6,8}$/i.test(s)) {
    const addrKey = "0x" + s.replace(/^0x/i, "").toLowerCase();
    const entry = msm().by_address[addrKey];
    if (entry) return entry.name;
  }
  return s;
}

function sym(name) {
  const clean = name.replace(/^__imp__/, "");
  const addrStr = msm().by_name[clean];
  if (!addrStr) return null;
  const entry = msm().by_address[addrStr] || {};
  return { address: addrStr, size: entry.size || 0, type: entry.type || "function", section: entry.section };
}

function srcFile(addrInt) {
  for (const e of splits()) {
    if (addrInt >= parseInt(e.start, 16) && addrInt < parseInt(e.end, 16))
      return e.source;
  }
  return null;
}

function inferClass(funcName) {
  const vt = vtable();
  const classes = Object.keys(vt).sort((a, b) => b.length - a.length);
  for (const cls of classes) {
    if (funcName.startsWith(cls + "_") || funcName === cls) return cls;
  }
  return funcName.split("_")[0] || null;
}

function findPseudo(addrHex) {
  const upper = addrHex.replace(/^0x/i, "").toUpperCase();
  for (const f of fs.readdirSync(PSEUDO_DIR)) {
    if (f.toUpperCase().startsWith(upper)) return path.join(PSEUDO_DIR, f);
  }
  return null;
}

function esc(s) { return s.replace(/[.*+?^${}()|[\]\\]/g, "\\$&"); }

function getCallees(rawText) {
  const found = new Set();
  const re = /\b([a-zA-Z_][a-zA-Z0-9_]*)\s*\(ctx,\s*base\)/g;
  const SKIP = new Set(["PPC_FUNC_IMPL","PPC_FUNC_PROLOGUE","VCALL","DTOR","PPC_UNIMPLEMENTED"]);
  let m;
  while ((m = re.exec(rawText)) !== null) {
    if (!SKIP.has(m[1])) found.add(m[1]);
  }
  return [...found];
}

function grepSrc(pattern) {
  try {
    return execSync(`grep -rl "${pattern}" "${SRC_DIR}" 2>/dev/null`, {
      encoding: "utf8", timeout: 6000,
    }).trim().split("\n").filter(Boolean);
  } catch { return []; }
}

function truncate(text, maxLines) {
  const lines = text.split("\n");
  if (lines.length <= maxLines) return text;
  return lines.slice(0, maxLines).join("\n") +
    `\n\n// ⚠ TRUNCATED (${lines.length} total lines)`;
}

/**
 * Annotate lbl_ references in scaffold code with section/size/class/string.
 * v3: also shows string content for .rdata string addresses.
 */
function annotateLbls(rawText) {
  const byAddr = msm().by_address;
  const r = rtti();
  const sc = stringCache();

  return rawText.replace(/lbl_([0-9A-Fa-f]{8})/g, (match, hex) => {
    const addr = "0x" + hex.toLowerCase();
    const sym = byAddr[addr];
    if (!sym) return match;

    const parts = [match];
    const sec = sym.section || "";
    const sz  = sym.size ? `sz:0x${parseInt(sym.size).toString(16)}` : "";
    if (sec) parts.push(`[${sec}${sz ? " " + sz : ""}]`);

    // Vtable pointer?
    const cls = rttiClassName(addr);
    if (cls) parts.push(`→ vtable<${cls}>`);

    // Readable string?
    const str = sc.get(addr);
    if (str) parts.push(`→ "${str.slice(0, 60)}${str.length > 60 ? "…" : ""}"`);

    // Named SDA global?
    const sdaE = sdaByAddr()[addr];
    if (sdaE && !sdaE.name.startsWith("lbl_")) parts.push(`→ ${sdaE.name}`);

    return parts.filter(Boolean).join(" ");
  });
}

// ─────────────────────────────────────────────────────────────────────────────
// Tool implementations
// ─────────────────────────────────────────────────────────────────────────────

function tool_get_function_recomp({ function_name }) {
  const name = resolveName(function_name);
  const idx = funcIndex();
  const info = idx.get(name.toLowerCase());

  if (!info) {
    const prefix = name.split("_")[0];
    const suggestions = [];
    for (const [k, v] of idx) {
      if (k.startsWith(prefix.toLowerCase())) suggestions.push(v.name);
      if (suggestions.length >= 5) break;
    }
    return `Function '${name}' not found in pass5_final.\n` +
      (suggestions.length ? `Similar names: ${suggestions.join(", ")}\n` : "") +
      `Use search_symbols("${prefix}") to find the right name.`;
  }

  const MAX = 300;
  let body = info.rawText;
  let truncNote = "";
  const lines = body.split("\n");
  if (lines.length > MAX) {
    body = lines.slice(0, MAX).join("\n");
    truncNote = `\n\n// ⚠ TRUNCATED at ${MAX} lines (function has ${lines.length} total)`;
  }
  body = annotateLbls(body);
  return `// ── pass5_final: ${name} ──\n// File: ${PASS5_FINAL}/${info.file}\n\n${body}${truncNote}`;
}

function tool_get_function_pseudocode({ function_name }) {
  const name = resolveName(function_name);
  const s = sym(name);
  if (!s) return `No symbol found for '${name}'.`;
  const addrHex = s.address.replace(/^0x/, "").toUpperCase();
  const pseudo = findPseudo(addrHex);
  if (!pseudo) return `No pseudocode found for ${name} @ 0x${addrHex}.`;
  const text = fs.readFileSync(pseudo, "utf8");
  const lines = text.split("\n");
  const MAX = 200;
  const body = lines.length > MAX
    ? lines.slice(0, MAX).join("\n") + `\n\n// ⚠ TRUNCATED (${lines.length} lines total)`
    : text;
  return `// ── IDA pseudocode: ${name} @ 0x${addrHex} ──\n// File: ${pseudo}\n\n${body}`;
}

function tool_get_function_info({ function_name }) {
  const name = resolveName(function_name);
  const s = sym(name);
  const addrInt = s ? parseInt(s.address, 16) : null;
  const addrStr = addrInt ? `0x${addrInt.toString(16).toUpperCase()}` : "unknown";
  const src = addrInt ? srcFile(addrInt) : null;
  const cls = inferClass(name);
  const rttiName = cls ? rttiOriginalName(cls) : null;

  let vtInfo = null;
  const vt = vtable();
  if (cls && vt[cls]) {
    for (const [slot, info] of Object.entries(vt[cls])) {
      if (info.symbol === name) { vtInfo = { slot, role: info.inferred_role || "", funcAddr: info.addr }; break; }
    }
  }

  const liftedIn = grepSrc(name);
  const lines = [
    `Function : ${name}`,
    `Address  : ${addrStr}`,
    `Size     : ${s?.size ? `0x${parseInt(s.size).toString(16)} (${s.size} bytes)` : "unknown"}`,
    `Type     : ${s?.type || "unknown"}`,
    `Source   : ${src || `not in splits.txt`}`,
    `Class    : ${cls || "unknown"}${rttiName && rttiName !== cls ? `  (original: ${rttiName})` : ""}`,
  ];
  if (vtInfo) {
    lines.push(`Vtable   : slot ${vtInfo.slot}${vtInfo.role ? ` [${vtInfo.role}]` : ""}${vtInfo.funcAddr ? `  @ ${vtInfo.funcAddr}` : ""}`);
  }
  lines.push(`Lifted?  : ${liftedIn.length ? "YES → " + liftedIn.join(", ") : "not yet"}`);
  if (s) {
    const addrHex = s.address.replace(/^0x/, "").toUpperCase();
    const pseudo = findPseudo(addrHex);
    lines.push(`Pseudocode: ${pseudo ? path.basename(pseudo) : "none"}`);
  }
  return lines.join("\n");
}

function tool_get_class_context({ class_name }) {
  const parts = [`// ── Class context: ${class_name} ──\n`];

  // 0. RTTI identity and inheritance
  const rttiName = rttiOriginalName(class_name);
  const hEntry = rttiEntry(class_name);
  if (rttiName || hEntry) {
    parts.push("RTTI / CLASS IDENTITY:");
    if (rttiName && rttiName !== class_name)
      parts.push(`  Original name : ${rttiName}  (use this in C++ declarations)`);
    else
      parts.push(`  Original name : ${class_name}`);
    if (hEntry?.inferred_bases?.length)
      parts.push(`  Inherits from : ${hEntry.inferred_bases.join(", ")}`);
    const vtAddrs = hEntry?.vtable_addrs || [];
    if (vtAddrs.length > 1) {
      parts.push(`  Multiple vtables (${vtAddrs.length}) — likely virtual base or MI:`);
      for (const va of vtAddrs.slice(0, 5)) {
        const cls = rttiClassName(va);
        parts.push(`    ${va}${cls ? `  → ${cls}` : ""}`);
      }
    }
    const r = rtti();
    const classVtables = Object.entries(r)
      .filter(([, v]) => v === class_name || v === rttiName)
      .map(([k]) => k);
    if (classVtables.length) {
      parts.push(`  Vtable pointer(s): ${classVtables.slice(0,4).join(", ")}`);
    }
    parts.push("");
  }

  // 1. Vtable layout — now with function addresses
  const vt = vtable();
  if (vt[class_name]) {
    parts.push("VTABLE:");
    const slots = Object.entries(vt[class_name]).sort((a, b) => +a[0] - +b[0]);
    for (const [slot, info] of slots) {
      const role = info.inferred_role ? `  [${info.inferred_role}]` : "";
      const addr = info.addr ? `  @ ${info.addr}` : "";
      parts.push(`  slot ${String(slot).padStart(2)}: ${info.symbol}${addr}${role}`);
    }
    parts.push("");
  } else {
    parts.push("VTABLE: (no entries)\n");
  }

  // 2. Field clusters — with hex offsets and union/bitfield flags
  const cl = cluster();
  if (cl[class_name]) {
    parts.push("FIELD ACCESS CLUSTERS (top 25 by access count):");
    parts.push(`  ${"hex".padEnd(8)} ${"dec".padEnd(6)} ${"count".padEnd(7)} ${"type".padEnd(12)} ${"ops".padEnd(14)} notes`);
    parts.push("  " + "─".repeat(60));
    const top = Object.entries(cl[class_name])
      .sort((a, b) => b[1].count - a[1].count)
      .slice(0, 25);
    const typeMap = { U8:"uint8_t", U16:"uint16_t", U32:"uint32_t", S32:"int32_t", F32:"float", U64:"uint64_t" };
    for (const [off, info] of top) {
      const offN = parseInt(off);
      const widths = info.widths || {};
      const dom = Object.entries(widths).sort((a,b)=>b[1]-a[1])[0]?.[0] || "U32";
      const isMixed = Object.keys(widths).length > 1;
      const mixedNote = isMixed ? `⚠ mixed(${Object.keys(widths).join("+")})` : "";
      parts.push(
        `  +0x${offN.toString(16).padStart(4,"0")} ` +
        `${String(offN).padEnd(6)} ` +
        `${String(info.count).padEnd(7)} ` +
        `${(typeMap[dom]||dom).padEnd(12)} ` +
        `R:${String(info.ops?.LOAD||0).padEnd(4)} W:${String(info.ops?.STORE||0).padEnd(4)}  ` +
        mixedNote
      );
    }
    parts.push("");
  }

  // 3. Debug string field hints — ALL entries, not capped at 10
  const dbg = dbgFields();
  if (dbg[class_name]) {
    parts.push(`DEBUG STRING FIELD HINTS (${dbg[class_name].length} entries):`);
    for (const entry of dbg[class_name]) {
      parts.push(`  ${entry.method}() → "${entry.string.slice(0,90)}"`);
      if (entry.field_hints) {
        for (const h of entry.field_hints)
          parts.push(`    field: ${h.field} (${h.type})`);
      }
    }
    parts.push("");
  }

  // 4. Method names from debug strings in binary
  const mm = methodMap();
  const classKey = class_name.split("::").pop(); // try bare name too
  const classMethods = mm.get(class_name) || mm.get(classKey);
  if (classMethods && classMethods.size > 0) {
    parts.push(`METHOD NAMES FROM DEBUG STRINGS (${classMethods.size} methods):`);
    for (const [method, entries] of [...classMethods.entries()].sort()) {
      const sample = entries[0];
      parts.push(`  ${method}()  @ ${sample.addr}  "${sample.string.slice(0,70)}"`);
    }
    parts.push("");
  }

  // 5. Struct definition from generated_structs.h
  const structH = `${BASE}/include/generated_structs.h`;
  if (fs.existsSync(structH)) {
    const src = fs.readFileSync(structH, "utf8");
    const structRe = new RegExp(
      `(?:// ${esc(class_name)}[^\n]*\n)?struct ${esc(class_name)}[^{]*\\{[^}]*\\}(?:\\s*;)?`,
      "s"
    );
    const m = structRe.exec(src);
    if (m) {
      parts.push("CURRENT STRUCT DEFINITION (auto-generated):");
      parts.push(m[0]);
      parts.push("\nNote: Refine field types and names as you lift methods.");
    }
  }

  // 6. Known functions in pass5_final
  const idx = funcIndex();
  const methods = [];
  for (const [k, info] of idx) {
    if (k.startsWith(class_name.toLowerCase() + "_")) methods.push(info.name);
  }
  if (methods.length) {
    parts.push(`\nKNOWN FUNCTIONS (${methods.length} in pass5_final — first 15):`);
    for (const m of methods.slice(0, 15)) parts.push(`  ${m}`);
    if (methods.length > 15) parts.push(`  ... and ${methods.length - 15} more`);
  }

  return parts.join("\n");
}

function tool_find_callers({ function_name, limit = 20 }) {
  const name = resolveName(function_name);
  const idx = funcIndex();
  const callers = [];
  const callPat = new RegExp(`\\b${esc(name)}\\s*\\(ctx,\\s*base\\)`);
  for (const [, info] of idx) {
    if (!info.rawText || info.name.toLowerCase() === name.toLowerCase()) continue;
    if (callPat.test(info.rawText)) {
      const s = sym(info.name);
      const addr = s ? parseInt(s.address, 16) : null;
      callers.push({ name: info.name, src: addr ? srcFile(addr) : null });
    }
  }
  if (!callers.length)
    return `No direct callers found for '${name}'.\n(May be a vtable entry, entry point, or called via indirect dispatch)`;
  const display = callers.slice(0, limit);
  const lines = [`Callers of ${name} (${callers.length} found, showing ${display.length}):\n`];
  for (const c of display) lines.push(`  ${c.name}${c.src ? `  [${c.src}]` : ""}`);
  return lines.join("\n");
}

function tool_find_callees({ function_name, limit = 30 }) {
  const name = resolveName(function_name);
  const idx = funcIndex();
  const info = idx.get(name.toLowerCase());
  if (!info) return `'${name}' not found in pass5_final.`;
  const callees = getCallees(info.rawText);
  const lines = [`Functions called by ${name} (${callees.length} total):\n`];
  for (const callee of callees.slice(0, limit)) {
    const s = sym(callee);
    const addr = s ? parseInt(s.address, 16) : null;
    const src = addr ? srcFile(addr) : null;
    lines.push(`  ${callee}${src ? `  [${src}]` : ""}`);
  }
  if (callees.length > limit) lines.push(`  ... and ${callees.length - limit} more`);
  return lines.join("\n");
}

function tool_search_symbols({ query, limit = 25 }) {
  const q = query.toLowerCase();
  const byName = msm().by_name;
  const byAddr = msm().by_address;
  const r = rtti();
  const exact = [], prefix = [], contains = [];
  for (const name of Object.keys(byName)) {
    const nl = name.toLowerCase();
    if (nl === q) exact.push({ name, source: "symbol" });
    else if (nl.startsWith(q)) prefix.push({ name, source: "symbol" });
    else if (nl.includes(q)) contains.push({ name, source: "symbol" });
    if (exact.length + prefix.length + contains.length > limit * 5) break;
  }
  const rttiMatches = [];
  for (const [addr, className] of Object.entries(r)) {
    if (className.toLowerCase().includes(q) && !byName[className]) {
      rttiMatches.push({ name: `[rtti] ${className}`, addr, source: "rtti" });
    }
  }
  const all = [...exact, ...prefix, ...contains];
  if (!all.length && !rttiMatches.length) return `No symbols matching '${query}'.`;
  const display = all.slice(0, limit);
  const lines = [`Symbols matching "${query}" (${all.length} found, showing ${display.length}):\n`];
  for (const item of display) {
    const addrStr = byName[item.name];
    const entry = byAddr[addrStr] || {};
    const addrInt = parseInt(addrStr, 16);
    const src = srcFile(addrInt);
    const size = entry.size ? ` size:0x${parseInt(entry.size).toString(16)}` : "";
    const vtableCls = rttiClassName(addrStr);
    const vtNote = vtableCls ? `  ← vtable<${vtableCls}>` : "";
    lines.push(`  ${item.name.padEnd(52)} @ ${addrStr}${size}${src ? `  [${src}]` : ""}${vtNote}`);
  }
  if (rttiMatches.length > 0 && all.length < limit) {
    lines.push(`\nRTTI class names matching "${query}":`);
    for (const item of rttiMatches.slice(0, 8)) {
      lines.push(`  ${item.name.padEnd(52)} @ ${item.addr}  (vtable address)`);
    }
  }
  return lines.join("\n");
}

/**
 * resolve_address — "what is this address?"
 * v3: now also shows string content for .rdata addresses.
 */
function tool_resolve_address({ address }) {
  const addrStr = "0x" + address.replace(/^0x/i, "").toLowerCase().padStart(8, "0");
  const addrInt = parseInt(addrStr, 16);
  const lines = [`Address: ${addrStr}\n`];

  const byAddr = msm().by_address;
  const entry = byAddr[addrStr];
  if (entry) {
    lines.push(`Symbol   : ${entry.name}`);
    lines.push(`Type     : ${entry.type || "object"}`);
    if (entry.size) lines.push(`Size     : 0x${parseInt(entry.size).toString(16)} (${entry.size} bytes)`);
    if (entry.section) lines.push(`Section  : ${entry.section}`);
  } else {
    // Find nearest enclosing symbol
    const candidates = Object.entries(byAddr)
      .map(([a, e]) => ({ addr: parseInt(a, 16), ...e }))
      .filter(c => c.addr <= addrInt && (c.addr + (parseInt(c.size)||4)) > addrInt);
    if (candidates.length) {
      const c = candidates[0];
      lines.push(`Symbol   : ${c.name} + 0x${(addrInt - c.addr).toString(16)}  (inside this symbol)`);
    } else {
      lines.push(`Symbol   : (not in symbol table)`);
      const preceding = Object.entries(byAddr)
        .map(([a, e]) => ({ addr: parseInt(a, 16), ...e }))
        .filter(c => c.addr < addrInt)
        .sort((a, b) => b.addr - a.addr)[0];
      if (preceding)
        lines.push(`Nearest  : ${preceding.name} + 0x${(addrInt - preceding.addr).toString(16)}`);
    }
  }

  // String content from binary
  const str = readCStr(addrInt);
  if (str) {
    lines.push(`\nString Content: "${str.slice(0, 120)}${str.length > 120 ? "…" : ""}"`);
    // Does it look like a debug assertion? Extract class::method
    const m = /^([A-Za-z_][A-Za-z0-9_:]+)::([A-Za-z_][A-Za-z0-9_]+)/.exec(str);
    if (m) lines.push(`  → ${m[1]}::${m[2]}() debug string`);
  }

  // RTTI vtable
  const rttiCls = rttiClassName(addrStr);
  if (rttiCls) {
    lines.push(`\nRTTI Vtable → ${rttiCls}`);
    lines.push(`  This is the vtable for class '${rttiCls}'.`);
    lines.push(`  Declare receiver: ${rttiCls}* obj = (${rttiCls}*)ptr;`);
    const hier = rttiHier();
    const hEntry = hier[rttiCls];
    if (hEntry?.vtable_addrs?.length > 1)
      lines.push(`  Note: ${rttiCls} has ${hEntry.vtable_addrs.length} vtables (virtual base/MI)`);
  }

  // SDA global
  const sdaEntry = sdaByAddr()[addrStr];
  if (sdaEntry) {
    lines.push(`\nSDA Global (r2/r13-relative):`);
    lines.push(`  SDA offset : ${sdaEntry.sda_offset}`);
    lines.push(`  Name       : ${sdaEntry.name}`);
    lines.push(`  Type       : ${sdaEntry.type}  Size: ${sdaEntry.size} bytes`);
    if (!sdaEntry.name.startsWith("lbl_"))
      lines.push(`  → extern ${sdaEntry.type === "object" ? "void*" : sdaEntry.type} ${sdaEntry.name};`);
  }

  // Source file
  const srcRange = srcFile(addrInt);
  if (srcRange) lines.push(`\nSource file: ${srcRange}`);

  // If function, show lift status
  if (entry?.type === "function") {
    lines.push(`\nFunction: ${entry.name}`);
    const pseudo = findPseudo(addrStr.replace(/^0x/, "").toUpperCase());
    if (pseudo) lines.push(`  Pseudocode: ${path.basename(pseudo)}`);
    const liftedIn = grepSrc(entry.name);
    if (liftedIn.length) lines.push(`  Lifted in: ${liftedIn.join(", ")}`);
  }

  return lines.join("\n");
}

/**
 * NEW TOOL: get_project_info
 * Returns binary metadata, compilation unit status, and compiler flags.
 * Use this to understand: what was the original build environment, what files
 * need to be matched, and what optimisations affect the generated code.
 */
function tool_get_project_info() {
  const lines = [
    "// ── Rockstar Table Tennis — Project Info ──\n",
    "BINARY",
    "  Internal name : pong_xenon_final.pe",
    "  Build date    : Wed May 10 20:40:52 2006",
    "  Entry point   : 0x8242BD20",
    "  Load base     : 0x82000000",
    "  Platform      : Xbox 360 (PowerPC, Big-Endian)",
    "  Sections      : .rdata .pdata .text .no_bbt .data .tls .idata",
    "",
    "COMPILER FLAGS (affects decompilation strategy)",
    "  /O2     Full optimisation — expect inlining, loop unrolling",
    "  /Ob2    Inline any suitable function — many small methods will be inlined",
    "  /GF     String pooling — identical string literals share one address",
    "  /Gy     Function-level linking — each function is a separate COMDAT",
    "  /Zi     Debug info compiled in (but stripped from final binary)",
    "  /W3     Warning level 3",
    "  Note: /Ob2 means many 1-3 line methods are inlined at every call site.",
    "        This is why you often see the same offset arithmetic repeated inline",
    "        rather than a separate function call.",
    "",
    "SYMBOL STATS",
    `  Total symbols     : ${(msm().function_count || 0) + (msm().object_count || 0)}`,
    `  Functions         : ${msm().function_count || 0}`,
    `  Objects/data      : ${msm().object_count || 0}`,
    "  RTTI classes      : 1,266",
    "  Vtable entries    : 4,713",
    "  Readable strings  : 6,785",
    "",
    "COMPILATION UNITS (from objects.json / splits.txt)",
  ];

  try {
    const objs = JSON.parse(fs.readFileSync(`${CFG}/objects.json`, "utf8"));
    for (const [cat, data] of Object.entries(objs)) {
      for (const [src, status] of Object.entries(data.objects)) {
        lines.push(`  [${cat.padEnd(4)}] ${src.padEnd(35)} ${status}`);
      }
    }
  } catch {}

  lines.push("");
  lines.push("KNOWN MATCHED FUNCTIONS (from splits.txt)");
  for (const e of splits()) {
    lines.push(`  ${e.source.padEnd(40)} 0x${parseInt(e.start,16).toString(16).toUpperCase()}–0x${parseInt(e.end,16).toString(16).toUpperCase()}`);
  }

  return lines.join("\n");
}

/**
 * write_source_file — surgical file editor (no full-overwrite mode)
 *
 * Modes:
 *   create         — Create a new file. FAILS if file already exists.
 *                    Required: file_path, content
 *
 *   append         — Append content to end of file (creates if missing).
 *                    Required: file_path, content
 *
 *   insert_at_line — Insert content before the given 1-indexed line,
 *                    shifting existing content down.
 *                    Required: file_path, content, start_line
 *
 *   replace_lines  — Replace lines [start_line..end_line] (inclusive,
 *                    1-indexed) with new content.
 *                    Required: file_path, content, start_line, end_line
 *
 *   delete_lines   — Delete lines [start_line..end_line] (inclusive).
 *                    Required: file_path, start_line, end_line
 *
 * Returns a numbered-line preview of the affected region after the edit.
 */
function tool_write_source_file({ file_path, content = "", mode, start_line, end_line }) {
  if (!mode) return `Error: 'mode' is required. Valid modes: create | append | insert_at_line | replace_lines | delete_lines`;
  const VALID_MODES = new Set(["create", "append", "insert_at_line", "replace_lines", "delete_lines"]);
  if (!VALID_MODES.has(mode)) {
    return `Error: unknown mode '${mode}'.\nValid modes: create | append | insert_at_line | replace_lines | delete_lines\n\nNote: full-overwrite ('write') has been removed to prevent accidental data loss.\n  To add new code:   use 'append' or 'insert_at_line'\n  To fix code:       use 'replace_lines' or 'delete_lines'\n  To start new file: use 'create'`;
  }

  // Normalise: strip a leading "src/" so both "src/game/foo.cpp" and "game/foo.cpp" work
  const normPath = file_path.replace(/^\//, '').replace(/^src\//, '');
  const abs = path.isAbsolute(file_path) ? file_path : path.join(SRC_DIR, normPath);
  const resolved = path.resolve(abs);
  if (!resolved.startsWith(path.resolve(SRC_DIR))) {
    return `Error: path must be inside src/\nResolved to: ${resolved}`;
  }

  const relPath = path.relative(BASE, resolved);
  const fileExists = fs.existsSync(resolved);
  fs.mkdirSync(path.dirname(resolved), { recursive: true });

  // ── CREATE ────────────────────────────────────────────────────────────────
  if (mode === "create") {
    if (fileExists) {
      const existingLines = fs.readFileSync(resolved, "utf8").split("\n").length;
      return `Error: 'create' refused — ${relPath} already exists (${existingLines} lines).\n` +
        `  To add content:    use 'append' or 'insert_at_line'\n` +
        `  To modify content: use 'replace_lines' or 'delete_lines'`;
    }
    fs.writeFileSync(resolved, content);
    const lc = content.split("\n").length;
    return `✓ Created ${relPath} (${lc} lines)\n\n` +
      previewLines(content.split("\n"), 1, Math.min(lc, 20));
  }

  // ── APPEND ────────────────────────────────────────────────────────────────
  if (mode === "append") {
    const existing = fileExists ? fs.readFileSync(resolved, "utf8") : "";
    const sep = existing.length > 0 && !existing.endsWith("\n") ? "\n" : "";
    const final = existing + sep + content;
    fs.writeFileSync(resolved, final);
    const newTotal = final.split("\n").length;
    const added = content.split("\n").length;
    return `✓ Appended ${added} lines to ${relPath} (file now ${newTotal} lines)\n\n` +
      `Preview (last ${Math.min(added + 2, 25)} lines):\n` +
      previewLines(final.split("\n"), Math.max(1, newTotal - added - 1), newTotal);
  }

  // ── Modes that require an existing file ───────────────────────────────────
  if (!fileExists) {
    return `Error: ${relPath} does not exist.\n  Use mode='create' to make a new file, or mode='append' to create-and-append.`;
  }

  const lines = fs.readFileSync(resolved, "utf8").split("\n");
  const totalLines = lines.length;

  // ── INSERT_AT_LINE ────────────────────────────────────────────────────────
  if (mode === "insert_at_line") {
    if (start_line == null) return `Error: 'insert_at_line' requires start_line (1-indexed).`;
    const sl = parseInt(start_line);
    if (isNaN(sl) || sl < 1 || sl > totalLines + 1) {
      return `Error: start_line ${sl} out of range — file has ${totalLines} lines (valid: 1–${totalLines + 1}).`;
    }
    const ins = content.split("\n");
    const newLines = [...lines.slice(0, sl - 1), ...ins, ...lines.slice(sl - 1)];
    fs.writeFileSync(resolved, newLines.join("\n"));
    const pStart = Math.max(1, sl - 1);
    const pEnd   = Math.min(newLines.length, sl + ins.length + 1);
    return `✓ Inserted ${ins.length} lines before line ${sl} in ${relPath} ` +
      `(${totalLines} → ${newLines.length} lines)\n\n` +
      `Preview (lines ${pStart}–${pEnd}):\n` +
      previewLines(newLines, pStart, pEnd);
  }

  // ── Modes that need start_line + end_line ─────────────────────────────────
  if (start_line == null || end_line == null) {
    return `Error: mode '${mode}' requires both start_line and end_line (1-indexed, inclusive).`;
  }
  const sl = parseInt(start_line);
  const el = parseInt(end_line);
  if (isNaN(sl) || isNaN(el) || sl < 1 || el < sl || el > totalLines) {
    return `Error: invalid line range ${sl}–${el}. File has ${totalLines} lines (valid: 1–${totalLines}).`;
  }

  // ── REPLACE_LINES ─────────────────────────────────────────────────────────
  if (mode === "replace_lines") {
    const repl = content.split("\n");
    const newLines = [...lines.slice(0, sl - 1), ...repl, ...lines.slice(el)];
    fs.writeFileSync(resolved, newLines.join("\n"));
    const removed = el - sl + 1;
    const pStart = Math.max(1, sl - 1);
    const pEnd   = Math.min(newLines.length, sl + repl.length + 1);
    return `✓ Replaced lines ${sl}–${el} (${removed} lines → ${repl.length} lines) in ${relPath} ` +
      `(${totalLines} → ${newLines.length} lines)\n\n` +
      `Preview (lines ${pStart}–${pEnd}):\n` +
      previewLines(newLines, pStart, pEnd);
  }

  // ── DELETE_LINES ──────────────────────────────────────────────────────────
  if (mode === "delete_lines") {
    const newLines = [...lines.slice(0, sl - 1), ...lines.slice(el)];
    fs.writeFileSync(resolved, newLines.join("\n"));
    const removed = el - sl + 1;
    const pStart = Math.max(1, sl - 1);
    const pEnd   = Math.min(newLines.length, sl + 3);
    return `✓ Deleted lines ${sl}–${el} (${removed} lines) from ${relPath} ` +
      `(${totalLines} → ${newLines.length} lines)\n\n` +
      `Preview around deletion point (lines ${pStart}–${pEnd}):\n` +
      previewLines(newLines, pStart, pEnd);
  }

  return `Error: unhandled mode '${mode}'`;
}

/** Format a slice of lines with 1-indexed line numbers */
function previewLines(lines, start, end) {
  const width = String(end).length;
  return lines
    .slice(start - 1, end)
    .map((l, i) => `  ${String(start + i).padStart(width)} \u2502 ${l}`)
    .join("\n");
}

function tool_get_existing_source({ file_path, function_name }) {
  if (file_path) {
    const normFP = file_path.replace(/^\//, '').replace(/^src\//, '');
    const abs = path.isAbsolute(file_path) ? file_path : path.join(SRC_DIR, normFP);
    const resolved = path.resolve(abs);
    if (!fs.existsSync(resolved)) {
      const fname = path.basename(resolved);
      let found = null;
      const walk = (dir) => {
        if (found) return;
        for (const f of fs.readdirSync(dir, { withFileTypes: true })) {
          if (f.isDirectory()) walk(path.join(dir, f.name));
          else if (f.name === fname) found = path.join(dir, f.name);
        }
      };
      walk(SRC_DIR);
      if (found) return `// Found at: ${found}\n\n${truncate(fs.readFileSync(found, "utf8"), 400)}`;
      return `File not found: ${file_path}`;
    }
    return `// ${resolved}\n\n${truncate(fs.readFileSync(resolved, "utf8"), 400)}`;
  }
  if (function_name) {
    const name = resolveName(function_name);
    const files = grepSrc(name);
    if (!files.length) return `'${name}' not found in src/.`;
    const lines = [`'${name}' found in:\n`];
    for (const f of files.slice(0, 3)) {
      lines.push(`  ${f}`);
      try {
        const grep = execSync(`grep -n "${name}" "${f}" 2>/dev/null`, {encoding:"utf8"}).trim();
        for (const l of grep.split("\n").slice(0,3)) lines.push(`    line ${l}`);
      } catch {}
    }
    return lines.join("\n");
  }
  return "Provide file_path or function_name.";
}

// ─────────────────────────────────────────────────────────────────────────────
// Tool manifest
// ─────────────────────────────────────────────────────────────────────────────

const TOOLS = [
  {
    name: "get_function_recomp",
    description: "Get the raw pass5_final static-recomp scaffold. lbl_ addresses are annotated with section/size/class and actual string content from the binary.",
    inputSchema: { type:"object", properties:{ function_name:{type:"string"} }, required:["function_name"] },
  },
  {
    name: "get_function_pseudocode",
    description: "Get IDA Hex-Rays pseudocode from orig/pseudocode/. Complements get_function_recomp. Not every function has pseudocode.",
    inputSchema: { type:"object", properties:{ function_name:{type:"string"} }, required:["function_name"] },
  },
  {
    name: "get_function_info",
    description: "Get metadata: address, size, source file, class (with RTTI original name), vtable slot + function address, lift status. Call first for any function.",
    inputSchema: { type:"object", properties:{ function_name:{type:"string"} }, required:["function_name"] },
  },
  {
    name: "get_class_context",
    description: "Full class context: RTTI name+inheritance, vtable slots with addresses, field clusters with union/bitfield flags, ALL debug string hints (uncapped), debug-string-derived method names from binary, struct definition, known methods.",
    inputSchema: { type:"object", properties:{ class_name:{type:"string"} }, required:["class_name"] },
  },
  {
    name: "find_callers",
    description: "Find all functions in pass5_final that directly call the given function.",
    inputSchema: { type:"object", properties:{ function_name:{type:"string"}, limit:{type:"integer",default:20} }, required:["function_name"] },
  },
  {
    name: "find_callees",
    description: "Find all functions called by the given function in pass5_final.",
    inputSchema: { type:"object", properties:{ function_name:{type:"string"}, limit:{type:"integer",default:30} }, required:["function_name"] },
  },
  {
    name: "search_symbols",
    description: "Search 52,691 symbols by name substring. Also searches RTTI class names (catches rage:: namespaced classes). Shows vtable class when applicable.",
    inputSchema: { type:"object", properties:{ query:{type:"string"}, limit:{type:"integer",default:25} }, required:["query"] },
  },
  {
    name: "resolve_address",
    description: "The essential 'what is this address?' tool. Returns: symbol name/type/size/section, actual STRING CONTENT if it's a .rdata string (e.g. 'pongPlayer::Process() - player is not active'), RTTI vtable class, SDA named global, nearest symbol, source file. Use on every lbl_ address before naming it.",
    inputSchema: { type:"object", properties:{ address:{type:"string"} }, required:["address"] },
  },
  {
    name: "get_project_info",
    description: "Returns binary metadata (build date, entry point, load base), compiler flags and their decompilation implications (/O2 /Ob2 = aggressive inlining), compilation unit statuses (NonMatching tracker), and all known splits ranges.",
    inputSchema: { type:"object", properties:{} },
  },
  {
    name: "write_source_file",
    description: "Surgical editor for src/ files — NO full-overwrite mode. Modes: 'create' (new file only, fails if exists), 'append' (add to end), 'insert_at_line' (insert before line N, requires start_line), 'replace_lines' (replace line range, requires start_line+end_line), 'delete_lines' (remove line range, requires start_line+end_line). All line numbers are 1-indexed. Returns a numbered preview of the affected region after the edit.",
    inputSchema: {
      type: "object",
      properties: {
        file_path:  { type: "string", description: "Path relative to src/ (e.g. 'game/pong_player.cpp')" },
        mode:       { type: "string", enum: ["create","append","insert_at_line","replace_lines","delete_lines"] },
        content:    { type: "string", description: "Text to write/insert/replace. Not needed for delete_lines." },
        start_line: { type: "integer", description: "1-indexed line number. Required for insert_at_line, replace_lines, delete_lines." },
        end_line:   { type: "integer", description: "1-indexed inclusive end line. Required for replace_lines and delete_lines." },
      },
      required: ["file_path", "mode"],
    },
  },
  {
    name: "get_existing_source",
    description: "Read existing lifted code from src/. Provide file_path or function_name.",
    inputSchema: { type:"object", properties:{ file_path:{type:"string"}, function_name:{type:"string"} } },
  },
  {
    name: "suggest_file_placement",
    description: "Given a class or function name, returns: which .hpp header to declare it in, which .cpp to implement it in, full src-relative paths, whether those files already exist, whether it is already lifted, and the 10 closest sibling-class prefixes that live in the same file. Call this BEFORE write_source_file to know exactly where to put new code.",
    inputSchema: { type:"object", properties:{ class_or_function_name:{type:"string"} }, required:["class_or_function_name"] },
  },
  {
    name: "suggest_unimplemented_class",
    description: "Randomly picks a class from rtti_vtable_map.json that is not yet implemented in src/, then returns its full family of related sub-classes to implement together (e.g. rage::snJoinMachine plus all its nested HSM states). Pass optional prefix to filter by subsystem (e.g. 'rage::sn' or 'pong'). Call this at the start of every session to get a work assignment.",
    inputSchema: {
      type: "object",
      properties: {
        prefix: { type: "string", description: "Optional class name prefix filter, e.g. 'rage::sn' or 'pong'" }
      }
    },
  },
  {
    name: "suggest_unimplemented_func",
    description: "Randomly picks a named, unlifted function from symbols.txt and returns a rich context card: function address/size/class, RTTI name, every direct callee with its owning class and lift status, all VCALL virtual-dispatch sites with resolved vtable classes, and a deduplicated list of every class the function touches. Use this to pick an individual function to lift and immediately understand its full dependency surface before reading any scaffold code. Pass optional prefix to filter by class name (e.g. 'pongBall') and optional min_size (bytes) to skip trivial stubs.",
    inputSchema: {
      type: "object",
      properties: {
        prefix:   { type: "string",  description: "Optional function name prefix filter, e.g. 'pongBall' or 'rage::sn'" },
        min_size: { type: "integer", description: "Skip functions smaller than this many bytes (default 32)" }
      }
    },
  },
];

// ─────────────────────────────────────────────────────────────────────────────
// MCP Server
// ─────────────────────────────────────────────────────────────────────────────

const server = new Server(
  { name: "tt-decomp-phase5", version: "3.0.0" },
  { capabilities: { tools: {} } }
);

server.setRequestHandler(ListToolsRequestSchema, async () => ({ tools: TOOLS }));

server.setRequestHandler(CallToolRequestSchema, async (request) => {
  const { name, arguments: args } = request.params;
  try {
    let result;
    switch (name) {
      case "get_function_recomp":     result = tool_get_function_recomp(args);    break;
      case "get_function_pseudocode": result = tool_get_function_pseudocode(args);break;
      case "get_function_info":       result = tool_get_function_info(args);      break;
      case "get_class_context":       result = tool_get_class_context(args);      break;
      case "find_callers":            result = tool_find_callers(args);           break;
      case "find_callees":            result = tool_find_callees(args);           break;
      case "search_symbols":          result = tool_search_symbols(args);         break;
      case "resolve_address":         result = tool_resolve_address(args);        break;
      case "get_project_info":        result = tool_get_project_info();           break;
      case "write_source_file":       result = tool_write_source_file(args);      break;
      case "suggest_file_placement": result = tool_suggest_file_placement(args); break;
      case "get_existing_source":     result = tool_get_existing_source(args);    break;
      case "suggest_unimplemented_class": result = tool_suggest_unimplemented_class(args); break;
      case "suggest_unimplemented_func":  result = tool_suggest_unimplemented_func(args);  break;
      default:
        return { content:[{ type:"text", text:`Unknown tool: ${name}` }], isError:true };
    }
    return { content:[{ type:"text", text: result }] };
  } catch (err) {
    return { content:[{ type:"text", text:`Error in ${name}: ${err.message}\n${err.stack}` }], isError:true };
  }
});

const transport = new StdioServerTransport();
await server.connect(transport);

// ─────────────────────────────────────────────────────────────────────────────
// ROUTING TABLE  (mirrors gen_headers_v3.py — kept in sync manually)
// Maps class-name prefix → { dir, file, hpp, cpp }
// ─────────────────────────────────────────────────────────────────────────────

const FILE_ROUTES = [
  // rage:: session / networking / events / rl / hsm
  { prefix: "rage::sn",          dir: "rage",     file: "rage_sn"       },
  { prefix: "rage::Evt",         dir: "rage",     file: "rage_sn"       },
  { prefix: "rage::rl",          dir: "rage",     file: "rage_sn"       },
  { prefix: "rage::hsm",         dir: "rage",     file: "rage_sn"       },
  { prefix: "rage::X::",         dir: "rage",     file: "rage_sn"       },
  { prefix: "rage::12::",        dir: "rage",     file: "rage_sn"       },
  { prefix: "rage::2::Vsn",      dir: "rage",     file: "rage_sn"       },
  // rage:: swf
  { prefix: "rage::swf",         dir: "rage",     file: "rage_swf"      },
  // rage:: animation
  { prefix: "rage::crAnim",      dir: "rage",     file: "rage_anim"     },
  { prefix: "rage::crIK",        dir: "rage",     file: "rage_anim"     },
  { prefix: "rage::cr",          dir: "rage",     file: "rage_anim"     },
  // rage:: motion framework
  { prefix: "rage::mf",          dir: "rage",     file: "rage_mf"       },
  // rage:: geometry / model / particles
  { prefix: "rage::grm",         dir: "rage",     file: "rage_grm"      },
  { prefix: "rage::frag",        dir: "rage",     file: "rage_grm"      },
  { prefix: "rage::spd",         dir: "rage",     file: "rage_grm"      },
  { prefix: "rage::rmc",         dir: "rage",     file: "rage_grm"      },
  { prefix: "rage::rm",          dir: "rage",     file: "rage_grm"      },
  // rage:: grc (graphics/render context)
  { prefix: "rage::grc",         dir: "graphics", file: "grc_render"    },
  // rage:: physics
  { prefix: "rage::ph",          dir: "physics",  file: "ph_physics"    },
  { prefix: "rage::environmentCloth", dir:"physics", file:"ph_physics"  },
  { prefix: "rage::clothController",  dir:"physics", file:"ph_physics"  },
  // rage:: audio
  { prefix: "rage::aud",         dir: "rage",     file: "rage_audio"    },
  // rage:: file I/O
  { prefix: "rage::fi",          dir: "rage",     file: "rage_fi"       },
  // rage:: data / serialization
  { prefix: "rage::dat",         dir: "rage",     file: "rage_dat"      },
  { prefix: "rage::dc",          dir: "rage",     file: "rage_dat"      },
  { prefix: "rage::netBBAllocator", dir:"rage",   file: "rage_dat"      },
  // rage:: par
  { prefix: "rage::par",         dir: "rage",     file: "rage_par"      },
  // rage:: xml
  { prefix: "rage::xml",         dir: "rage",     file: "rage_xml"      },
  // rage:: cm operators (all rage:: cm*)
  { prefix: "rage::cm",          dir: "rage",     file: "rage_cm"       },
  { prefix: "rage::cmSampleCamActions", dir:"rage", file:"rage_cm"      },
  // rage:: memory / system / misc
  { prefix: "rage::sysMem",      dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::sysTime",     dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::sysThread",   dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::miniheap",    dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::evtSet",      dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::evtInstance", dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::atAny",       dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::netConnection", dir:"rage",    file: "rage_misc"     },
  { prefix: "rage::dcam",        dir: "rage",     file: "rage_misc"     },
  { prefix: "rage::MatrixMND",   dir: "rage",     file: "rage_math"     },
  { prefix: "rage::VectorNI",    dir: "rage",     file: "rage_math"     },
  { prefix: "rage::VectorND",    dir: "rage",     file: "rage_math"     },
  { prefix: "rage::pg",          dir: "rage",     file: "rage_pg"       },
  { prefix: "rage::at",          dir: "rage",     file: "rage_at"       },
  { prefix: "rage::",            dir: "rage",     file: "rage_misc"     },
  // non-rage:: singletons
  { prefix: "atSingleton",       dir: "rage",     file: "rage_at"       },
  { prefix: "atSafeDLList",      dir: "rage",     file: "rage_at"       },
  { prefix: "pgDictionary",      dir: "rage",     file: "rage_pg"       },
  // cm operators (non-rage::)
  { prefix: "cmOperatorCtor",    dir: "game",     file: "cm_operators"  },
  { prefix: "cmRefreshableCtor", dir: "game",     file: "cm_operators"  },
  { prefix: "cmWorldRefreshableCtor", dir:"game", file: "cm_operators"  },
  { prefix: "cmWorldApproach2Ctor",   dir:"game", file: "cm_operators"  },
  { prefix: "cmApproach2Ctor",   dir: "game",     file: "cm_operators"  },
  { prefix: "cmReporterCtor",    dir: "game",     file: "cm_operators"  },
  { prefix: "cmActionCtor",      dir: "game",     file: "cm_operators"  },
  { prefix: "cmCond",            dir: "game",     file: "cm_operators"  },
  { prefix: "cmLookup",          dir: "game",     file: "cm_operators"  },
  { prefix: "cmSwitch",          dir: "game",     file: "cm_operators"  },
  { prefix: "cmLinearRemap",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmSmoothRemap",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmNaryOperator",    dir: "game",     file: "cm_operators"  },
  { prefix: "cmSumReporter",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmAvgReporter",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmNoneReporter",    dir: "game",     file: "cm_operators"  },
  { prefix: "cmFirstReporter",   dir: "game",     file: "cm_operators"  },
  { prefix: "cmLastReporter",    dir: "game",     file: "cm_operators"  },
  { prefix: "cmAndReporter",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmOrReporter",      dir: "game",     file: "cm_operators"  },
  { prefix: "cmBitwiseAndReporter",dir:"game",    file: "cm_operators"  },
  { prefix: "cmBitwiseOrReporter", dir:"game",    file: "cm_operators"  },
  { prefix: "cmMostReporter",    dir: "game",     file: "cm_operators"  },
  { prefix: "cmMinReporter",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmMaxReporter",     dir: "game",     file: "cm_operators"  },
  { prefix: "cmNearestToReporter",dir:"game",     file: "cm_operators"  },
  { prefix: "cmFurthestFromReporter",dir:"game",  file: "cm_operators"  },
  { prefix: "cm",                dir: "game",     file: "cm_operators"  },
  // scene graph
  { prefix: "sg",                dir: "graphics", file: "sg_scene"      },
  // grc
  { prefix: "grc",               dir: "graphics", file: "grc_render"    },
  // fx effects
  { prefix: "fx",                dir: "graphics", file: "fx_effects"    },
  { prefix: "TxBallSpin",        dir: "graphics", file: "fx_effects"    },
  // hud
  { prefix: "hudFlashBase",      dir: "ui",       file: "hud"           },
  { prefix: "hudHUD",            dir: "ui",       file: "hud"           },
  { prefix: "hudTraining",       dir: "ui",       file: "hud"           },
  { prefix: "hudList",           dir: "ui",       file: "hud"           },
  { prefix: "hudBoot",           dir: "ui",       file: "hud"           },
  { prefix: "hudLegals",         dir: "ui",       file: "hud"           },
  { prefix: "hudPause",          dir: "ui",       file: "hud"           },
  { prefix: "hudUnlocks",        dir: "ui",       file: "hud"           },
  { prefix: "hudShell",          dir: "ui",       file: "hud"           },
  { prefix: "hudLogos",          dir: "ui",       file: "hud"           },
  { prefix: "hudLoading",        dir: "ui",       file: "hud"           },
  { prefix: "hudController",     dir: "ui",       file: "hud"           },
  { prefix: "hudCredits",        dir: "ui",       file: "hud"           },
  { prefix: "hudCharView",       dir: "ui",       file: "hud"           },
  { prefix: "hudDialog",         dir: "ui",       file: "hud"           },
  { prefix: "hudFrontEnd",       dir: "ui",       file: "hud"           },
  { prefix: "hudLeaderboard",    dir: "ui",       file: "hud"           },
  { prefix: "SpectatorHud",      dir: "ui",       file: "hud"           },
  { prefix: "uiList",            dir: "ui",       file: "hud"           },
  { prefix: "hud",               dir: "ui",       file: "hud"           },
  // cv curves
  { prefix: "cvCurve",           dir: "ui",       file: "cv_ui"         },
  { prefix: "cv",                dir: "ui",       file: "cv_ui"         },
  // physics standalone
  { prefix: "phDemoObject",      dir: "physics",  file: "ph_physics"    },
  { prefix: "phDemoWorld",       dir: "physics",  file: "ph_physics"    },
  { prefix: "phMaterialMgrImpl", dir: "physics",  file: "ph_physics"    },
  { prefix: "ph",                dir: "physics",  file: "ph_physics"    },
  // pong creature/animation
  { prefix: "pongCreature",      dir: "game",     file: "pong_creature" },
  { prefix: "pongAnimationInfo", dir: "game",     file: "pong_creature" },
  { prefix: "pongLookAt",        dir: "game",     file: "pong_creature" },
  { prefix: "pongMover",         dir: "game",     file: "pong_creature" },
  { prefix: "pongBlend",         dir: "game",     file: "pong_creature" },
  { prefix: "pongLocomotion",    dir: "game",     file: "pong_creature" },
  { prefix: "pongSkelLookAt",    dir: "game",     file: "pong_creature" },
  { prefix: "pongHairData",      dir: "game",     file: "pong_creature" },
  { prefix: "pongPlayerShader",  dir: "game",     file: "pong_creature" },
  { prefix: "pcr",               dir: "game",     file: "pong_creature" },
  { prefix: "LocomotionState",   dir: "game",     file: "pong_creature" },
  { prefix: "LocomotionController", dir:"game",   file: "pong_creature" },
  // pong ball
  { prefix: "pongBall",          dir: "game",     file: "pong_ball"     },
  { prefix: "pongPaddle",        dir: "game",     file: "pong_ball"     },
  { prefix: "gdBall",            dir: "game",     file: "pong_ball"     },
  { prefix: "gmBallRobot",       dir: "game",     file: "pong_ball"     },
  { prefix: "gmBallNode",        dir: "game",     file: "pong_ball"     },
  // pong player (already implemented)
  { prefix: "pongPlayer",        dir: "game",     file: "pong_player"   },
  { prefix: "pongRemotePlayer",  dir: "game",     file: "pong_network"  },
  // pong match
  { prefix: "gmLogic",           dir: "game",     file: "pong_match"    },
  { prefix: "gmPracticeTarget",  dir: "game",     file: "pong_match"    },
  { prefix: "MatchData",         dir: "game",     file: "pong_match"    },
  { prefix: "shotTarget",        dir: "game",     file: "pong_match"    },
  { prefix: "powerZone",         dir: "game",     file: "pong_match"    },
  // pong drill
  { prefix: "pongTrainingDrill", dir: "game",     file: "pong_drill"    },
  { prefix: "pongDrill",         dir: "game",     file: "pong_drill"    },
  { prefix: "gdTraining",        dir: "game",     file: "pong_drill"    },
  { prefix: "gdDrill",           dir: "game",     file: "pong_drill"    },
  { prefix: "hitTipData",        dir: "game",     file: "pong_drill"    },
  { prefix: "serveTipData",      dir: "game",     file: "pong_drill"    },
  { prefix: "focusMeter",        dir: "game",     file: "pong_drill"    },
  { prefix: "statsThreshold",    dir: "game",     file: "pong_drill"    },
  // pong network
  { prefix: "PongNet",           dir: "game",     file: "pong_network"  },
  { prefix: "pongNet",           dir: "game",     file: "pong_network"  },
  { prefix: "pongLive",          dir: "game",     file: "pong_network"  },
  { prefix: "pongPresence",      dir: "game",     file: "pong_network"  },
  { prefix: "NetworkClient",     dir: "game",     file: "pong_network"  },
  { prefix: "NetTimer",          dir: "game",     file: "pong_network"  },
  { prefix: "NetBall",           dir: "game",     file: "pong_network"  },
  { prefix: "NetData",           dir: "game",     file: "pong_network"  },
  { prefix: "NetState",          dir: "game",     file: "pong_network"  },
  { prefix: "NetTuning",         dir: "game",     file: "pong_network"  },
  { prefix: "NetLoading",        dir: "game",     file: "pong_network"  },
  { prefix: "netPolicies",       dir: "game",     file: "pong_network"  },
  { prefix: "LogicNetwork",      dir: "game",     file: "pong_network"  },
  { prefix: "SinglesNetwork",    dir: "game",     file: "pong_network"  },
  { prefix: "SpectatorNetwork",  dir: "game",     file: "pong_network"  },
  { prefix: "SessionMessage",    dir: "game",     file: "pong_network"  },
  { prefix: "FloatAverager",     dir: "game",     file: "pong_network"  },
  { prefix: "FrameTimeEstimate", dir: "game",     file: "pong_network"  },
  { prefix: "AckHandling",       dir: "game",     file: "pong_network"  },
  { prefix: "EventBase",         dir: "game",     file: "pong_network"  },
  { prefix: "EventSwing",        dir: "game",     file: "pong_network"  },
  { prefix: "pongNetMessage",    dir: "game",     file: "pong_network"  },
  { prefix: "Gamer",             dir: "game",     file: "pong_network"  },
  // pong camera
  { prefix: "pongCam",           dir: "game",     file: "pong_camera"   },
  { prefix: "pongCamera",        dir: "game",     file: "pong_camera"   },
  { prefix: "pongCine",          dir: "game",     file: "pong_camera"   },
  { prefix: "gdCamera",          dir: "game",     file: "pong_camera"   },
  { prefix: "camShot",           dir: "game",     file: "pong_camera"   },
  { prefix: "camBaseCS",         dir: "game",     file: "pong_camera"   },
  { prefix: "camViewCS",         dir: "game",     file: "pong_camera"   },
  { prefix: "charViewCS",        dir: "game",     file: "pong_camera"   },
  { prefix: "cameraShake",       dir: "game",     file: "pong_camera"   },
  { prefix: "CMtoCSAdapter",     dir: "game",     file: "pong_camera"   },
  // pong render
  { prefix: "pongRenderable",    dir: "game",     file: "pong_render"   },
  { prefix: "pongDrawable",      dir: "game",     file: "pong_render"   },
  { prefix: "pongDrawBucket",    dir: "game",     file: "pong_render"   },
  { prefix: "pongRenderThread",  dir: "game",     file: "pong_render"   },
  { prefix: "pongPostEffects",   dir: "game",     file: "pong_render"   },
  { prefix: "pongShadowMap",     dir: "game",     file: "pong_render"   },
  { prefix: "pongGammaControl",  dir: "game",     file: "pong_render"   },
  { prefix: "pongLightGlow",     dir: "game",     file: "pong_render"   },
  { prefix: "pongScrnTrans",     dir: "game",     file: "pong_render"   },
  { prefix: "pongScreenCapture", dir: "game",     file: "pong_render"   },
  { prefix: "pongSurface",       dir: "game",     file: "pong_render"   },
  // pong states
  { prefix: "pongBoot",          dir: "game",     file: "pong_states"   },
  { prefix: "pongAttract",       dir: "game",     file: "pong_states"   },
  { prefix: "pongLogos",         dir: "game",     file: "pong_states"   },
  { prefix: "pongTrainingUI",    dir: "game",     file: "pong_states"   },
  { prefix: "pongLoading",       dir: "game",     file: "pong_states"   },
  { prefix: "pongPause",         dir: "game",     file: "pong_states"   },
  { prefix: "pongCredits",       dir: "game",     file: "pong_states"   },
  { prefix: "pongCharView",      dir: "game",     file: "pong_states"   },
  { prefix: "pongDialog",        dir: "game",     file: "pong_states"   },
  { prefix: "pongFrontend",      dir: "game",     file: "pong_states"   },
  { prefix: "pongLegals",        dir: "game",     file: "pong_states"   },
  { prefix: "pongStats",         dir: "game",     file: "pong_states"   },
  { prefix: "pongLeaderboard",   dir: "game",     file: "pong_states"   },
  { prefix: "dialogData",        dir: "game",     file: "pong_states"   },
  { prefix: "dialogManager",     dir: "game",     file: "pong_states"   },
  { prefix: "charViewChar",      dir: "game",     file: "pong_states"   },
  { prefix: "charViewData",      dir: "game",     file: "pong_states"   },
  { prefix: "creditsData",       dir: "game",     file: "pong_states"   },
  { prefix: "creditsSettings",   dir: "game",     file: "pong_states"   },
  { prefix: "frontendData",      dir: "game",     file: "pong_states"   },
  { prefix: "rlConcreteStatsView",dir:"game",     file: "pong_states"   },
  { prefix: "hsmState",          dir: "game",     file: "pong_states"   },
  // pong hud
  { prefix: "pongHUDLive",       dir: "game",     file: "pong_hud"      },
  // pong cutscene
  { prefix: "pongCutScene",      dir: "game",     file: "pong_cutscene" },
  { prefix: "pongCS",            dir: "game",     file: "pong_cutscene" },
  { prefix: "pongInlineCinematics",dir:"game",    file: "pong_cutscene" },
  { prefix: "gdCS",              dir: "game",     file: "pong_cutscene" },
  { prefix: "gdCutScene",        dir: "game",     file: "pong_cutscene" },
  // pong audio
  { prefix: "pongAudioManager",  dir: "game",     file: "pong_audio"    },
  { prefix: "pongAmbiencePlayer",dir: "game",     file: "pong_audio"    },
  { prefix: "audMsgSink",        dir: "game",     file: "pong_audio"    },
  { prefix: "CBiquadFilter",     dir: "game",     file: "pong_audio"    },
  { prefix: "CCompEffect",       dir: "game",     file: "pong_audio"    },
  { prefix: "CDelayEffect",      dir: "game",     file: "pong_audio"    },
  { prefix: "CPeakMeter",        dir: "game",     file: "pong_audio"    },
  { prefix: "CShelving",         dir: "game",     file: "pong_audio"    },
  // pong game shell / levels
  { prefix: "pongGame",          dir: "game",     file: "pong_game"     },
  { prefix: "pongShell",         dir: "game",     file: "pong_game"     },
  { prefix: "pongControlGroup",  dir: "game",     file: "pong_game"     },
  { prefix: "pongGameState",     dir: "game",     file: "pong_game"     },
  { prefix: "pongGameContext",   dir: "game",     file: "pong_game"     },
  { prefix: "pongRandomizer",    dir: "game",     file: "pong_game"     },
  { prefix: "shShell",           dir: "game",     file: "pong_game"     },
  { prefix: "shInit",            dir: "game",     file: "pong_game"     },
  { prefix: "shEnd",             dir: "game",     file: "pong_game"     },
  { prefix: "lvl",               dir: "game",     file: "pong_game"     },
  // pong misc
  { prefix: "pongSaveFile",      dir: "game",     file: "pong_misc"     },
  { prefix: "pongMovie",         dir: "game",     file: "pong_misc"     },
  { prefix: "pongXMV",           dir: "game",     file: "pong_misc"     },
  { prefix: "pongBink",          dir: "game",     file: "pong_misc"     },
  { prefix: "pong",              dir: "game",     file: "pong_misc"     },
  // plr*
  { prefix: "plr",               dir: "game",     file: "gd_data"       },
  // gd* data
  { prefix: "gdai",              dir: "game",     file: "gd_data"       },
  { prefix: "gdUnlock",          dir: "game",     file: "gd_data"       },
  { prefix: "gdStats",           dir: "game",     file: "gd_data"       },
  { prefix: "gdTourney",         dir: "game",     file: "gd_data"       },
  { prefix: "gd",                dir: "game",     file: "gd_data"       },
  // mc*
  { prefix: "mcMemcard",         dir: "game",     file: "mc_memcard"    },
  { prefix: "mcSegment",         dir: "game",     file: "mc_memcard"    },
  { prefix: "mcFile",            dir: "game",     file: "mc_memcard"    },
  { prefix: "mc",                dir: "game",     file: "mc_memcard"    },
  // ai / msg / misc
  { prefix: "ai",                dir: "game",     file: "pong_misc"     },
  { prefix: "msg",               dir: "game",     file: "pong_misc"     },
  { prefix: "assetVersions",     dir: "game",     file: "pong_misc"     },
  { prefix: "cellData",          dir: "game",     file: "pong_misc"     },
  { prefix: "ctxManager",        dir: "game",     file: "pong_misc"     },
  { prefix: "fontData",          dir: "game",     file: "pong_misc"     },
  { prefix: "listData",          dir: "game",     file: "pong_misc"     },
  { prefix: "listItemData",      dir: "game",     file: "pong_misc"     },
  { prefix: "fsmMachine",        dir: "game",     file: "pong_misc"     },
  { prefix: "Timer",             dir: "game",     file: "pong_misc"     },
  { prefix: "xmlNodeStructArray",dir: "rage",     file: "rage_xml"      },
  { prefix: "xmlNodeStructReverb",dir:"rage",     file: "rage_xml"      },
  { prefix: "xmlNodeStructAlign",dir: "rage",     file: "rage_xml"      },
  { prefix: "xmlNodeStructAnim", dir: "rage",     file: "rage_xml"      },
  { prefix: "xmlRoot",           dir: "rage",     file: "rage_xml"      },
  { prefix: "xmlNodeStruct",     dir: "rage",     file: "rage_xml"      },
  { prefix: "std::",             dir: "rage",     file: "rage_misc"     },
  { prefix: "sysThreadPool",     dir: "rage",     file: "rage_misc"     },
  { prefix: "Holder",            dir: "rage",     file: "rage_misc"     },
];

/**
 * Resolve a class/function name → (dir, file) pair using the routing table.
 * Tries longest-prefix match first for accurate results.
 */
function resolveFileRoute(className) {
  // Sort by prefix length descending (longest-specific match wins)
  const sorted = [...FILE_ROUTES].sort((a, b) => b.prefix.length - a.prefix.length);
  for (const { prefix, dir, file } of sorted) {
    if (className.startsWith(prefix)) {
      return { dir, file };
    }
  }
  return { dir: "game", file: "pong_misc" };
}

/**
 * NEW TOOL: suggest_file_placement
 *
 * Given a class or function name, tells the model:
 *   - Which .hpp header to declare it in
 *   - Which .cpp source file to implement it in
 *   - Full absolute paths (src-relative)
 *   - Whether an existing implementation was found
 *   - Related classes in the same file (for context)
 */
function tool_suggest_file_placement({ class_or_function_name }) {
  const name = (class_or_function_name || "").trim();
  if (!name) return "Provide class_or_function_name.";

  // Strip scope qualifiers to get the root class name
  const root = name.split("::")[0];
  const { dir, file } = resolveFileRoute(name);

  const hppRel = `${dir}/${file}.hpp`;
  const cppRel = `${dir}/${file}.cpp`;
  const hppAbs = path.join(SRC_DIR, hppRel);
  const cppAbs = path.join(SRC_DIR, cppRel);

  const lines = [
    `// ── File placement: ${name} ──`,
    ``,
    `  Header  : src/${hppRel}`,
    `  Source  : src/${cppRel}`,
    ``,
    `  Header exists : ${fs.existsSync(hppAbs) ? "YES" : "no — create it"}`,
    `  Source exists : ${fs.existsSync(cppAbs) ? "YES" : "no — create it"}`,
  ];

  // Check if already lifted somewhere
  const lifted = grepSrc(name);
  if (lifted.length) {
    lines.push(`  Already lifted in: ${lifted.slice(0, 3).join(", ")}`);
  } else {
    lines.push(`  Not yet lifted.`);
  }

  // Show related classes that share the file (first 10)
  const siblings = FILE_ROUTES
    .filter(r => r.dir === dir && r.file === file)
    .map(r => r.prefix)
    .filter(p => p && p !== name && p !== root)
    .slice(0, 10);
  if (siblings.length) {
    lines.push(``, `  Other classes in this file (by prefix):`);
    for (const s of siblings) lines.push(`    ${s}*`);
  }

  // Writing instructions
  lines.push(
    ``,
    `  ── How to add your implementation ──`,
    `  1. Declare the struct in src/${hppRel}`,
    `     (skeleton already generated — search for "struct ${root}")`,
    `  2. Create/append your lifted C++ to src/${cppRel}`,
    `     Use write_source_file({ file_path: "${cppRel}", content: "..." })`,
    `  3. #include "${file}.hpp" at the top of your .cpp`,
  );

  return lines.join("\n");
}


/**
 * NEW TOOL: suggest_unimplemented_class
 *
 * Randomly picks a root class from rtti_vtable_map.json that has NO presence
 * in src/ yet, and returns the full family of related sub-classes to implement
 * together in one session.
 *
 * "Family" = all RTTI entries whose name starts with the same root prefix,
 * e.g. rage::snJoinMachine + all rage::snJoinMachine::* nested states.
 *
 * Optional filter: pass a `prefix` string (e.g. "rage::sn") to restrict
 * suggestions to a particular subsystem.
 */
function tool_suggest_unimplemented_class({ prefix = "" } = {}) {
  const rttiMap = rtti();

  // ── Build family map: rootKey -> Set<className> ──────────────────────────
  // Root = first two segments for rage:: classes (rage::Foo), else just first.
  const familyMap = new Map();
  for (const className of Object.values(rttiMap)) {
    const parts = className.split('::');
    const rootKey = (parts[0] === 'rage' && parts.length >= 2)
      ? parts[0] + '::' + parts[1]
      : parts[0];
    if (!familyMap.has(rootKey)) familyMap.set(rootKey, new Set());
    familyMap.get(rootKey).add(className);
  }

  // ── Apply optional prefix filter ─────────────────────────────────────────
  const candidateRoots = prefix
    ? [...familyMap.keys()].filter(k => k.toLowerCase().startsWith(prefix.toLowerCase()))
    : [...familyMap.keys()];

  if (!candidateRoots.length) {
    return `No classes found matching prefix "${prefix}".`;
  }

  // ── Determine which roots are already implemented in src/ ─────────────────
  const implementedRoots = new Set();
  for (const rootKey of candidateRoots) {
    // grep for the exact class name (or its bare form without namespace)
    const bare = rootKey.split('::').pop();
    const searchTerms = [rootKey, bare].filter(Boolean);
    for (const term of searchTerms) {
      try {
        const result = execSync(
          `grep -rl ${JSON.stringify(term)} ${JSON.stringify(SRC_DIR)} 2>/dev/null`,
          { encoding: 'utf8', timeout: 5000 }
        ).trim();
        if (result) { implementedRoots.add(rootKey); break; }
      } catch {}
    }
  }

  // ── Filter to unimplemented ───────────────────────────────────────────────
  const unimplemented = candidateRoots.filter(k => !implementedRoots.has(k));

  if (!unimplemented.length) {
    const scope = prefix ? `matching "${prefix}"` : 'in the binary';
    return `All classes ${scope} appear to have src/ coverage. Nothing left!`;
  }

  // ── Pick a random one ─────────────────────────────────────────────────────
  const chosen = unimplemented[Math.floor(Math.random() * unimplemented.length)];
  const family = [...familyMap.get(chosen)].sort();

  // ── Find matching functions in pass5_final ────────────────────────────────
  const idx = funcIndex();
  const bareName = chosen.split('::').pop().toLowerCase();
  // Try camelCase collision: rage::snJoinMachine -> snJoinMachine_*
  const relatedFuncs = [];
  for (const [k, info] of idx) {
    if (k.startsWith(bareName + '_') || k === bareName ||
        k.startsWith(bareName.replace(/_/g, '') + '_')) {
      relatedFuncs.push(info.name);
    }
  }

  // ── Format output ─────────────────────────────────────────────────────────
  const lines = [
    `// -- Suggested class batch to implement --`,
    ``,
    `  Root class   : ${chosen}`,
    `  Family size  : ${family.length} class(es) in the hierarchy`,
    ``,
    `  Implement these together in one session:`,
  ];
  for (const cls of family) {
    const isRoot = cls === chosen;
    lines.push(`    ${isRoot ? "[root] " : "       "}${cls}`);
  }
  lines.push(`  ---`);

  lines.push(``);
  lines.push(`  Why together? Sub-classes share struct layout, vtable, and state machine`);
  lines.push(`  logic. Lifting them in a single session avoids re-loading class context.`);

  if (relatedFuncs.length) {
    lines.push(``, `  Known functions in pass5_final (${relatedFuncs.length} total, first 15):`);
    for (const f of relatedFuncs.slice(0, 15)) lines.push(`    ${f}`);
    if (relatedFuncs.length > 15) lines.push(`    ... and ${relatedFuncs.length - 15} more`);
  } else {
    lines.push(``);
    lines.push(`  NOTE: No directly-named functions found in pass5_final.`);
    lines.push(`    This class may be fully inlined, or use a different naming convention.`);
    lines.push(`    Try: search_symbols("${bareName}")`);
  }

  const totalUnimpl = prefix
    ? unimplemented.length
    : [...familyMap.keys()].filter(k => !implementedRoots.has(k)).length;
  const totalRoot = prefix ? candidateRoots.length : familyMap.size;
  lines.push(``);
  lines.push(`  Progress: ~${totalRoot - totalUnimpl}/${totalRoot} root classes have src/ coverage`);
  lines.push(`  (${totalUnimpl} unimplemented root classes remaining${prefix ? ` for prefix "${prefix}"` : ''})`);

  lines.push(``, `  ── Recommended next steps ────────────────────────────────────────────`);
  lines.push(`    1. get_class_context("${family[0]}")`);
  if (relatedFuncs.length) {
    lines.push(`    2. get_function_info("${relatedFuncs[0]}")`);
  }
  lines.push(`    3. suggest_file_placement("${chosen}")`);

  return lines.join('\n');
}

/**
 * TOOL: suggest_unimplemented_func
 *
 * Randomly picks a named, unlifted function from symbols.txt and returns a
 * rich context card showing:
 *   - Function address, size, inferred class, RTTI name
 *   - Whether the owning class already has src/ coverage
 *   - Every direct callee from the pass5_final scaffold, with:
 *       - callee address and size
 *       - callee's inferred class and RTTI original name
 *       - whether that callee class is already lifted in src/
 *   - Every VCALL (virtual dispatch) site with the vtable class resolved
 *   - Lift status (already in src/ or not)
 *
 * Optional filters:
 *   prefix  - restrict to functions whose name starts with this string
 *   min_size - skip functions smaller than this many bytes (default 0x20)
 *              to avoid trivial stubs
 */
function tool_suggest_unimplemented_func({ prefix = '', min_size = 0x20 } = {}) {
  const allFuncs = symbolFuncs();
  const idx      = funcIndex();
  const r        = rtti();
  const byAddr   = msm().by_address;

  // Helper: infer owning class from function name (matches inferClass logic)
  function inferOwner(name) {
    const vt = Object.keys(vtable()).sort((a, b) => b.length - a.length);
    for (const cls of vt) {
      if (name.startsWith(cls + '_') || name === cls) return cls;
    }
    // fallback: everything before the last underscore+hex segment
    const m = name.match(/^([A-Za-z_][A-Za-z0-9_]*?)(?:_[0-9A-Fa-f]{4}|_[A-Z][a-z])/);
    return m ? m[1] : name.split('_')[0];
  }

  // Helper: is a class already in src/?
  const _liftedClassCache = new Map();
  function classIsLifted(cls) {
    if (_liftedClassCache.has(cls)) return _liftedClassCache.get(cls);
    const files = grepSrc(cls);
    const result = files.length > 0;
    _liftedClassCache.set(cls, result);
    return result;
  }

  // Filter to candidates: named (not pure hex stub), right size, right prefix
  const STUB_RE = /^(rage|game|ref|thunk_fn)_[0-9A-Fa-f]+(_[a-z])?$/;
  const candidates = allFuncs.filter(f => {
    if (f.size < min_size) return false;
    if (STUB_RE.test(f.name)) return false;
    if (prefix && !f.name.toLowerCase().startsWith(prefix.toLowerCase())) return false;
    return true;
  });

  if (!candidates.length) {
    return `No named functions found matching prefix "${prefix}" with size >= 0x${min_size.toString(16)}.`;
  }

  // Separate already-lifted from unlifted
  const unlifted = candidates.filter(f => grepSrc(f.name).length === 0);

  if (!unlifted.length) {
    return `All ${candidates.length} candidate functions${prefix ? ` matching "${prefix}"` : ''} appear to be lifted already.`;
  }

  // Pick a random unlifted function
  const chosen = unlifted[Math.floor(Math.random() * unlifted.length)];
  const ownerClass = inferOwner(chosen.name);
  const rttiName   = rttiOriginalName(ownerClass) || ownerClass;
  const ownerLifted = classIsLifted(ownerClass);
  const srcRange   = srcFile(parseInt(chosen.addr, 16));

  const lines = [
    `// -- Suggested function to implement --`,
    ``,
    `  Function   : ${chosen.name}`,
    `  Address    : ${chosen.addr}`,
    `  Size       : 0x${chosen.size.toString(16)} bytes`,
    `  Owning class : ${rttiName}${rttiName !== ownerClass ? ` (inferred: ${ownerClass})` : ''}`,
    `  Class lifted : ${ownerLifted ? 'YES - existing code to read before writing' : 'no'}`,
    `  Source file  : ${srcRange || 'not in splits.txt'}`,
    `  Pseudocode   : ${(() => { try { return fs.readdirSync(PSEUDO_DIR).find(f => f.toUpperCase().startsWith(chosen.addr.replace(/^0x/i,'').toUpperCase())) || 'none'; } catch { return 'none'; } })()}`,
    ``,
  ];

  // ── Callee analysis from pass5_final ───────────────────────────────────────
  const scaffoldInfo = idx.get(chosen.name.toLowerCase());
  if (scaffoldInfo) {
    const rawText = scaffoldInfo.rawText;

    // Direct named callees
    const calleeNames = getCallees(rawText);
    if (calleeNames.length) {
      lines.push(`  Direct callees (${calleeNames.length}):`);
      const seen = new Set();
      for (const callee of calleeNames) {
        if (seen.has(callee)) continue;
        seen.add(callee);
        const s = sym(callee);
        const calleeAddr = s ? s.address : '?';
        const calleeSize = s ? `0x${parseInt(s.size || 0).toString(16)}` : '?';
        const calleeClass = inferOwner(callee);
        const calleeRtti  = rttiOriginalName(calleeClass) || calleeClass;
        const calleeLifted = classIsLifted(calleeClass);
        const calleeSrc   = s ? srcFile(parseInt(s.address, 16)) : null;
        lines.push(
          `    ${callee.padEnd(48)}` +
          `  @ ${calleeAddr}  sz:${calleeSize}` +
          `  [class: ${calleeRtti}${calleeLifted ? ' - LIFTED' : ' - not lifted'}]` +
          (calleeSrc ? `  [${calleeSrc}]` : '')
        );
      }
      lines.push(``);
    }

    // VCALL sites — resolve vtable class for each
    const vcallMatches = [...rawText.matchAll(/VCALL\((\w+(?:\.\w+)*),\s*(\d+),/g)];
    // Also look for lbl_ addresses near VCALLs to identify the vtable
    const lblMatches = [...rawText.matchAll(/lbl_([0-9A-Fa-f]{8})/g)];
    const vtableClasses = new Set();
    for (const [, hex] of lblMatches) {
      const addr = '0x' + hex.toLowerCase();
      const cls  = rttiClassName(addr);
      if (cls) vtableClasses.add(cls);
    }

    if (vcallMatches.length || vtableClasses.size) {
      lines.push(`  Virtual dispatch sites:`);
      lines.push(`    VCALL count : ${vcallMatches.length}`);
      if (vtableClasses.size) {
        for (const cls of vtableClasses) {
          const lifted = classIsLifted(cls.split('::').pop());
          lines.push(`    vtable class: ${cls}  [${lifted ? 'LIFTED' : 'not lifted'}]`);
        }
      }
      lines.push(``);
    }

    // Classes touched — deduplicated set of all class owners across callees + vtables
    const touchedClasses = new Set();
    for (const callee of calleeNames) touchedClasses.add(inferOwner(callee));
    for (const cls of vtableClasses) touchedClasses.add(cls.split('::')[0] + (cls.includes('::') ? '::' + cls.split('::')[1] : ''));
    touchedClasses.delete(ownerClass);

    if (touchedClasses.size) {
      lines.push(`  Classes this function touches:`);
      for (const cls of [...touchedClasses].sort()) {
        const rn = rttiOriginalName(cls) || cls;
        const lifted = classIsLifted(cls);
        lines.push(`    ${rn.padEnd(40)}  [${lifted ? 'src/ present - read it first' : 'not yet lifted'}]`);
      }
      lines.push(``);
    }

  } else {
    lines.push(`  NOTE: Function not found in pass5_final scaffold. May be fully inlined.`);
    lines.push(`  Try: get_function_recomp("${chosen.name}")`);
    lines.push(``);
  }

  // Progress
  const totalCandidates = candidates.length;
  const remaining = unlifted.length;
  lines.push(`  Progress: ${totalCandidates - remaining}/${totalCandidates} candidate functions have src/ coverage`);
  lines.push(`  (${remaining} unimplemented functions remaining${prefix ? ` for prefix "${prefix}"` : ''})`);

  lines.push(``, `  -- Recommended next steps --`);
  lines.push(`    1. get_function_info("${chosen.name}")`);
  lines.push(`    2. get_class_context("${rttiName}")`);
  if (ownerLifted) {
    lines.push(`    3. get_existing_source for the owning class file -- read it before writing`);
  }
  lines.push(`    4. get_function_recomp("${chosen.name}")`);

  return lines.join('\n');
}

