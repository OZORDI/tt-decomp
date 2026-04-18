/**
 * FLIRT batch_09 tail — Xam profile wrappers + false-FLIRT ledger
 * Rockstar Presents Table Tennis — agent F10
 *
 * Addresses in this batch that fall outside the __restvmx belt (see
 * ppc_vmx_epilogue_tail.c) were FLIRT-matched to D3DXShader / XGRAPHICS /
 * XUser names.  Per the project FLIRT-trust rule, the names are
 * authoritative only for Xam/Xbox-API addresses with a real underlying
 * Xbox API import (XamUser*).  Everything else in this tail turns out
 * to be either tiny game-code leaves or false matches on in-function
 * constants / .rodata-adjacent bytes.  Dispositions:
 *
 * -----------------------------------------------------------------------
 * 0x8256a708  FLIRT: ?InsertCopy@Compiler@D3DXShader@@...
 *             RESOLVES: game_A708_h  (0x24 bytes, aliased to xam_A730)
 *
 *   PPC body is a 2-instruction leaf:
 *       bl  0x82481b08   ; tail-call rage_1B08(ctx, base)
 *       li  r3, 0        ; return 0
 *       blr
 *
 *   This is NOT the D3DX shader-compiler insert-copy routine — it is a
 *   tiny rage-side helper that defers to rage_1B08 and zeroes the return
 *   value.  The FLIRT match is a false positive on an unrelated leaf
 *   prologue.  Body already lives in pass5_final under __imp__game_A708_h;
 *   no separate lift is required.  FLIRT name dropped.
 *
 * -----------------------------------------------------------------------
 * 0x8256d770  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_87
 * 0x8256d7e4  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_88
 * 0x8256d828  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_89
 * 0x8256dfec  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_90
 * 0x8256e1b8  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_91
 * 0x8258fdf0  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_92
 * 0x8259cb00  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_93
 * 0x8259cb98  FLIRT: ?IsAlloc@IRInst@XGRAPHICS@@...UBA?B_NXZ_94
 *
 *   resolve_address() reports these as byte offsets inside larger
 *   containers — either embedded in xam_D760_sp (a data/sp table) or deep
 *   inside xam_XamUserWriteProfileSettings_68EC at +0x9504 / +0x16214 /
 *   +0x162ac.  The 2-byte string "8`" returned by resolve is the literal
 *   bytes 0x38 0x60 — the PPC encoding of "li r3, 0", i.e. a false FLIRT
 *   match on any 'return 0' leaf pattern that appears inside a larger
 *   function.  These are not standalone functions; they have no PPC entry
 *   point in pass5_final.  No lift required.
 *
 * -----------------------------------------------------------------------
 * 0x8257b7a0  FLIRT: XUserReadProfileSettings
 *             RESOLVES: xam_B7A0 (100 bytes, real function)
 *
 *   Game-side thunk that validates a precondition via xam_B738, then
 *   delegates to the real import XamUserReadProfileSettings with a
 *   fixed bOverlapped = 0.  The FLIRT name is reasonable (it's a
 *   compiler-emitted forwarder into the XAM import) but the actual
 *   implementation is the imported XamUserReadProfileSettings stub that
 *   already lives in the xam/ subsystem.  No separate host hook needed;
 *   pass5_final already chains the call correctly.
 *
 * 0x8257b808  FLIRT: XUserReadProfileSettings_0
 *             RESOLVES: xam_B808 (108 bytes, real function)
 *
 *   Eight-argument variant of the above — same precondition check via
 *   xam_B738 then XamUserReadProfileSettings.  The stack slot at +244
 *   is forwarded as the overlapped pointer.  Same disposition: already
 *   handled by the existing XamUserReadProfileSettings stub.
 *
 * 0x8257b878  FLIRT: XUserWriteProfileSettings
 *             RESOLVES: xam_B878 (72 bytes, real function)
 *
 *   XamUserGetXUID(userIndex=3, &xuid) then
 *   XamUserWriteProfileSettings(0, xuid, settingCount, settings, overlapped).
 *   Thin forwarder into the two XAM imports.  Already routed correctly.
 *
 *   For all three XUser* entries, behavior resolves through the existing
 *   XamUser* imports (see xam/xam_stubs.c and xam/xe_buffers.c).  The
 *   default success-path stub (return 0, zero-init output buffer) in
 *   those files covers the host-side effect; no new hook is added here.
 * -----------------------------------------------------------------------
 *
 * This file is documentation-only.  Nothing here participates in link.
 */
