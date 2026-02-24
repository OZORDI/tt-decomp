#!/usr/bin/env node
/**
 * tt-decomp Phase 5 MCP Server
 * ==============================
 * Gives an LLM all the context it needs to lift one function at a time
 * from the static-recomp scaffold (pass5_final) into clean, idiomatic C++
 * in the src/ tree, matching what Rockstar developers originally wrote.
 *
 * 9 focused tools — quality over quantity.
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

const BASE         = "/Users/Ozordi/Downloads/tt-decomp";
const CFG          = `${BASE}/config/434C4803`;
const PASS5_FINAL  = `${BASE}/recomp/structured_pass5_final`;
const PSEUDO_DIR   = `${BASE}/orig/pseudocode`;
const SRC_DIR      = `${BASE}/src`;

// ─────────────────────────────────────────────────────────────────────────────
// Lazy-loaded indexes
// ─────────────────────────────────────────────────────────────────────────────

let _msm         = null;  // master_symbol_map: { by_name: {name->addr}, by_address: {addr->{name,type,size}} }
let _splits      = null;  // splits_addr_map: [{start, end, source}]
let _vtable      = null;  // vtable_layout_map: {className -> {slot -> {symbol, inferred_role}}}
let _cluster     = null;  // offset_cluster_map: {className -> {offset -> {count, widths, ops}}}
let _dbgFields   = null;  // debug_string_field_map: {className -> [{method, string, field_hints}]}
let _funcIndex   = null;  // Map: funcNameLower -> {name, file, lineStart, lineEnd, rawText}

function msm() {
  if (!_msm) _msm = JSON.parse(fs.readFileSync(`${CFG}/master_symbol_map.json`, "utf8"));
  return _msm;
}
function splits() {
  if (!_splits) _splits = JSON.parse(fs.readFileSync(`${CFG}/splits_addr_map.json`, "utf8"));
  return _splits;
}
function vtable() {
  if (!_vtable) _vtable = JSON.parse(fs.readFileSync(`${CFG}/vtable_layout_map.json`, "utf8"));
  return _vtable;
}
function cluster() {
  if (!_cluster) {
    const p = `${CFG}/offset_cluster_map.json`;
    _cluster = fs.existsSync(p) ? JSON.parse(fs.readFileSync(p, "utf8")) : {};
  }
  return _cluster;
}
function dbgFields() {
  if (!_dbgFields) _dbgFields = JSON.parse(fs.readFileSync(`${CFG}/debug_string_field_map.json`, "utf8"));
  return _dbgFields;
}

// ─────────────────────────────────────────────────────────────────────────────
// Function index: scan pass5_final once, index every function
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
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

/** Resolve a name or 0xADDR → canonical function name */
function resolveName(input) {
  let s = (input || "").trim().replace(/^__imp__/, "");

  // Looks like an address?
  if (/^(0x)?[0-9A-Fa-f]{6,8}$/i.test(s)) {
    const addrKey = "0x" + s.replace(/^0x/i, "").toLowerCase();
    const entry = msm().by_address[addrKey];
    if (entry) return entry.name;
  }
  return s;
}

/** Look up a function symbol → {address, size, type} or null */
function sym(name) {
  const clean = name.replace(/^__imp__/, "");
  const addrStr = msm().by_name[clean];
  if (!addrStr) return null;
  const entry = msm().by_address[addrStr] || {};
  return { address: addrStr, size: entry.size || 0, type: entry.type || "function" };
}

/** Get source file from splits for an address integer */
function srcFile(addrInt) {
  for (const e of splits()) {
    if (addrInt >= parseInt(e.start, 16) && addrInt < parseInt(e.end, 16))
      return e.source;
  }
  return null;
}

/** Infer class name from function name */
function inferClass(funcName) {
  const vt = vtable();
  const classes = Object.keys(vt).sort((a, b) => b.length - a.length);
  for (const cls of classes) {
    if (funcName.startsWith(cls + "_") || funcName === cls) return cls;
  }
  return funcName.split("_")[0] || null;
}

/** Find IDA pseudocode file for an address hex string like "820CD598" */
function findPseudo(addrHex) {
  const upper = addrHex.replace(/^0x/i, "").toUpperCase();
  for (const f of fs.readdirSync(PSEUDO_DIR)) {
    if (f.toUpperCase().startsWith(upper)) return path.join(PSEUDO_DIR, f);
  }
  return null;
}

function esc(s) { return s.replace(/[.*+?^${}()|[\]\\]/g, "\\$&"); }

/** Extract callee names from a function's raw recomp text */
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

/** Grep src/ for a function name, returns matching file paths */
function grepSrc(pattern) {
  try {
    return execSync(`grep -rl "${pattern}" "${SRC_DIR}" 2>/dev/null`, {
      encoding: "utf8", timeout: 6000,
    }).trim().split("\n").filter(Boolean);
  } catch { return []; }
}

// ─────────────────────────────────────────────────────────────────────────────
// Tool implementations
// ─────────────────────────────────────────────────────────────────────────────

function tool_get_function_recomp({ function_name }) {
  const name = resolveName(function_name);
  const idx = funcIndex();
  const info = idx.get(name.toLowerCase());

  if (!info) {
    // Give a useful suggestion
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

  const lines = info.rawText.split("\n");
  const MAX = 300;
  let body = info.rawText;
  let truncNote = "";
  if (lines.length > MAX) {
    body = lines.slice(0, MAX).join("\n");
    truncNote = `\n\n// ⚠ TRUNCATED at ${MAX} lines (function has ${lines.length} total)\n// Use multiple calls or request a specific line range if needed`;
  }

  return `// ── pass5_final: ${name} ──\n// File: ${PASS5_FINAL}/${info.file}\n\n${body}${truncNote}`;
}

function tool_get_function_pseudocode({ function_name }) {
  const name = resolveName(function_name);
  const s = sym(name);
  if (!s) return `No symbol found for '${name}'.`;

  const addrHex = s.address.replace(/^0x/, "").toUpperCase();
  const pseudo = findPseudo(addrHex);
  if (!pseudo) {
    return `No pseudocode found for ${name} @ 0x${addrHex}.\n` +
           `(Checked ${PSEUDO_DIR} for files starting with ${addrHex})`;
  }

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

  // Vtable slot?
  let vtInfo = null;
  const vt = vtable();
  if (cls && vt[cls]) {
    for (const [slot, info] of Object.entries(vt[cls])) {
      if (info.symbol === name) { vtInfo = { slot, role: info.inferred_role || "" }; break; }
    }
  }

  // Already lifted?
  const liftedIn = grepSrc(name);

  const lines = [
    `Function : ${name}`,
    `Address  : ${addrStr}`,
    `Size     : ${s?.size ? `0x${parseInt(s.size).toString(16)} (${s.size} bytes)` : "unknown"}`,
    `Type     : ${s?.type || "unknown"}`,
    `Source   : ${src || `not in splits.txt`}`,
    `Class    : ${cls || "unknown"}`,
  ];
  if (vtInfo) lines.push(`Vtable   : slot ${vtInfo.slot}${vtInfo.role ? ` [${vtInfo.role}]` : ""}`);
  lines.push(`Lifted?  : ${liftedIn.length ? "YES → " + liftedIn.join(", ") : "not yet"}`);

  // Pseudo exists?
  if (s) {
    const addrHex = s.address.replace(/^0x/, "").toUpperCase();
    const pseudo = findPseudo(addrHex);
    lines.push(`Pseudocode: ${pseudo ? path.basename(pseudo) : "none"}`);
  }

  return lines.join("\n");
}

function tool_get_class_context({ class_name }) {
  const parts = [`// ── Class context: ${class_name} ──\n`];

  // 1. Vtable
  const vt = vtable();
  if (vt[class_name]) {
    parts.push("VTABLE:");
    const slots = Object.entries(vt[class_name]).sort((a, b) => +a[0] - +b[0]);
    for (const [slot, info] of slots) {
      const role = info.inferred_role ? `  [${info.inferred_role}]` : "";
      parts.push(`  slot ${String(slot).padStart(2)}: ${info.symbol}${role}`);
    }
    parts.push("");
  } else {
    parts.push("VTABLE: (no entries)\n");
  }

  // 2. Field cluster
  const cl = cluster();
  if (cl[class_name]) {
    parts.push("FIELD ACCESS CLUSTERS (top 20 by access count):");
    parts.push(`  ${"hex".padEnd(8)} ${"dec".padEnd(6)} ${"count".padEnd(7)} ${"type".padEnd(12)} ops`);
    parts.push("  " + "─".repeat(48));
    const top = Object.entries(cl[class_name])
      .sort((a, b) => b[1].count - a[1].count)
      .slice(0, 20);
    const typeMap = { U8:"uint8_t", U16:"uint16_t", U32:"uint32_t", S32:"int32_t", F32:"float", U64:"uint64_t" };
    for (const [off, info] of top) {
      const offN = parseInt(off);
      const dom = Object.entries(info.widths).sort((a,b)=>b[1]-a[1])[0]?.[0] || "U32";
      parts.push(
        `  +0x${offN.toString(16).padStart(4,"0")} ` +
        `${String(offN).padEnd(6)} ` +
        `${String(info.count).padEnd(7)} ` +
        `${(typeMap[dom]||dom).padEnd(12)} ` +
        `R:${info.ops?.LOAD||0} W:${info.ops?.STORE||0}`
      );
    }
    parts.push("");
  }

  // 3. Debug string hints
  const dbg = dbgFields();
  if (dbg[class_name]) {
    parts.push("DEBUG STRING FIELD HINTS:");
    for (const entry of dbg[class_name].slice(0, 10)) {
      parts.push(`  ${entry.method}() → "${entry.string.slice(0,90)}"`);
      if (entry.field_hints) {
        for (const h of entry.field_hints)
          parts.push(`    field: ${h.field} (${h.type})`);
      }
    }
    parts.push("");
  }

  // 4. Current struct definition from generated_structs.h
  const structH = `${BASE}/include/generated_structs.h`;
  if (fs.existsSync(structH)) {
    const src = fs.readFileSync(structH, "utf8");
    // Find the struct block for this class
    const structRe = new RegExp(
      `(?:// ${esc(class_name)}[^\n]*\n)?struct ${esc(class_name)}[^{]*\\{[^}]*\\}(?:\\s*;)?`,
      "s"
    );
    const m = structRe.exec(src);
    if (m) {
      parts.push("CURRENT STRUCT DEFINITION (auto-generated from clustering):");
      parts.push(m[0]);
      parts.push("\nNote: Refine field types and names as you lift methods.");
    }
  }

  // 5. Methods in pass5_final for this class
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

  // Pattern: either direct call or VCALL
  const callPat = new RegExp(`\\b${esc(name)}\\s*\\(ctx,\\s*base\\)`);

  for (const [, info] of idx) {
    if (!info.rawText || info.name.toLowerCase() === name.toLowerCase()) continue;
    if (callPat.test(info.rawText)) {
      const s = sym(info.name);
      const addr = s ? parseInt(s.address, 16) : null;
      callers.push({ name: info.name, src: addr ? srcFile(addr) : null });
    }
  }

  if (!callers.length) {
    return `No direct callers found for '${name}'.\n(May be a vtable entry, entry point, or called via indirect dispatch)`;
  }

  const display = callers.slice(0, limit);
  const lines = [`Callers of ${name} (${callers.length} found, showing ${display.length}):\n`];
  for (const c of display) {
    lines.push(`  ${c.name}${c.src ? `  [${c.src}]` : ""}`);
  }
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
    const cls = inferClass(callee);
    lines.push(`  ${callee}${src ? `  [${src}]` : ""}${cls && cls !== callee.split("_")[0] ? "" : ""}`);
  }
  if (callees.length > limit) lines.push(`  ... and ${callees.length - limit} more`);
  return lines.join("\n");
}

function tool_search_symbols({ query, limit = 25 }) {
  const q = query.toLowerCase();
  const byName = msm().by_name;
  const byAddr = msm().by_address;

  const exact = [], prefix = [], contains = [];

  for (const name of Object.keys(byName)) {
    const nl = name.toLowerCase();
    if (nl === q) exact.push(name);
    else if (nl.startsWith(q)) prefix.push(name);
    else if (nl.includes(q)) contains.push(name);
    if (exact.length + prefix.length + contains.length > limit * 5) break;
  }

  const all = [...exact, ...prefix, ...contains];
  if (!all.length) return `No symbols matching '${query}'.`;

  const display = all.slice(0, limit);
  const lines = [`Symbols matching "${query}" (${all.length} found, showing ${display.length}):\n`];
  for (const name of display) {
    const addrStr = byName[name];
    const entry = byAddr[addrStr] || {};
    const addrInt = parseInt(addrStr, 16);
    const src = srcFile(addrInt);
    const size = entry.size ? ` size:0x${parseInt(entry.size).toString(16)}` : "";
    lines.push(`  ${name.padEnd(52)} @ ${addrStr}${size}${src ? `  [${src}]` : ""}`);
  }
  return lines.join("\n");
}

function tool_write_source_file({ file_path, content, mode = "write" }) {
  // Resolve and validate path
  const abs = path.isAbsolute(file_path) ? file_path : path.join(SRC_DIR, file_path);
  const resolved = path.resolve(abs);
  if (!resolved.startsWith(path.resolve(SRC_DIR))) {
    return `Error: path must be inside ${SRC_DIR}\nGot: ${resolved}`;
  }

  fs.mkdirSync(path.dirname(resolved), { recursive: true });

  if (mode === "append") {
    fs.appendFileSync(resolved, "\n" + content);
  } else {
    fs.writeFileSync(resolved, content);
  }

  const lineCount = content.split("\n").length;
  const relPath = path.relative(BASE, resolved);
  return `✓ ${mode === "append" ? "Appended" : "Wrote"} ${lineCount} lines to ${relPath}`;
}

function tool_get_existing_source({ file_path, function_name }) {
  if (file_path) {
    const abs = path.isAbsolute(file_path) ? file_path : path.join(SRC_DIR, file_path);
    const resolved = path.resolve(abs);

    if (!fs.existsSync(resolved)) {
      // Try filename-only search
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
      if (found) {
        const content = fs.readFileSync(found, "utf8");
        return `// Found at: ${found}\n\n${truncate(content, 400)}`;
      }
      return `File not found: ${file_path}`;
    }

    const content = fs.readFileSync(resolved, "utf8");
    return `// ${resolved}\n\n${truncate(content, 400)}`;
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

function truncate(text, maxLines) {
  const lines = text.split("\n");
  if (lines.length <= maxLines) return text;
  return lines.slice(0, maxLines).join("\n") +
    `\n\n// ⚠ TRUNCATED (${lines.length} total lines)`;
}

// ─────────────────────────────────────────────────────────────────────────────
// Tool manifest
// ─────────────────────────────────────────────────────────────────────────────

const TOOLS = [
  {
    name: "get_function_recomp",
    description: "Get the raw pass5_final static-recomp scaffold for a function. This is the mechanical translation of PowerPC assembly — your starting material for Phase 5 lifting. Accepts function name (pongPlayer_D598_g) or hex address (0x820CD598).",
    inputSchema: { type: "object", properties: { function_name: { type: "string" } }, required: ["function_name"] },
  },
  {
    name: "get_function_pseudocode",
    description: "Get IDA Hex-Rays pseudocode from orig/pseudocode/ for a function. Gives cleaner typed C with variable names — complement this with get_function_recomp for the full picture. Not every function has pseudocode.",
    inputSchema: { type: "object", properties: { function_name: { type: "string" } }, required: ["function_name"] },
  },
  {
    name: "get_function_info",
    description: "Get metadata: address, size, source file (from splits.txt), class, vtable slot, and whether it's already been lifted. Call this first before starting on any function.",
    inputSchema: { type: "object", properties: { function_name: { type: "string" } }, required: ["function_name"] },
  },
  {
    name: "get_class_context",
    description: "Get full class context: vtable layout, top-accessed struct offsets with C types (from frequency clustering), debug string field hints, auto-generated struct definition, and all known methods. Call before lifting any method of a class you haven't seen before.",
    inputSchema: { type: "object", properties: { class_name: { type: "string" } }, required: ["class_name"] },
  },
  {
    name: "find_callers",
    description: "Find all functions in pass5_final that directly call the given function. Reveals usage context and call-site invariants.",
    inputSchema: { type: "object", properties: { function_name: { type: "string" }, limit: { type: "integer", default: 20 } }, required: ["function_name"] },
  },
  {
    name: "find_callees",
    description: "Find all functions called by the given function in pass5_final. Maps the dependency graph downward — what subsystems this function depends on.",
    inputSchema: { type: "object", properties: { function_name: { type: "string" }, limit: { type: "integer", default: 30 } }, required: ["function_name"] },
  },
  {
    name: "search_symbols",
    description: "Search the master symbol table (52,691 entries) by name substring. Returns address, size, type, and source file. Use this to discover related functions, look up global variables, or verify an address.",
    inputSchema: { type: "object", properties: { query: { type: "string" }, limit: { type: "integer", default: 25 } }, required: ["query"] },
  },
  {
    name: "write_source_file",
    description: "Write clean lifted C++ to src/. file_path is relative to src/ (e.g. 'game/pong_player.cpp'). mode='write' (default) overwrites; mode='append' adds to existing file. This is the primary output action.",
    inputSchema: {
      type: "object",
      properties: {
        file_path: { type: "string" },
        content: { type: "string" },
        mode: { type: "string", enum: ["write", "append"], default: "write" },
      },
      required: ["file_path", "content"],
    },
  },
  {
    name: "get_existing_source",
    description: "Read existing lifted code from src/. Provide file_path (e.g. 'game/pong_player.cpp') to read a file, or function_name to search all of src/ for that function. Use before writing to avoid duplication.",
    inputSchema: {
      type: "object",
      properties: {
        file_path: { type: "string" },
        function_name: { type: "string" },
      },
    },
  },
];

// ─────────────────────────────────────────────────────────────────────────────
// MCP Server
// ─────────────────────────────────────────────────────────────────────────────

const server = new Server(
  { name: "tt-decomp-phase5", version: "1.0.0" },
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
      case "write_source_file":       result = tool_write_source_file(args);      break;
      case "get_existing_source":     result = tool_get_existing_source(args);    break;
      default:
        return { content: [{ type: "text", text: `Unknown tool: ${name}` }], isError: true };
    }
    return { content: [{ type: "text", text: result }] };
  } catch (err) {
    return { content: [{ type: "text", text: `Error in ${name}: ${err.message}\n${err.stack}` }], isError: true };
  }
});

const transport = new StdioServerTransport();
await server.connect(transport);
