/**
 * ppc_helpers.c — PPC compiler-emitted register save/restore helpers
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * The Xenon XCC compiler emits "fall-through belts" at the end of .text
 * that function prologues/epilogues branch into to spill and reload
 * callee-saved registers:
 *
 *     __savevmx_NN   at 0x82569450..0x825696E8  (save  v14..v31 and v64..v127)
 *     __restvmx_NN   at 0x825696E8..0x82569980  (restore matching banks)
 *     __savegprlr_NN at 0x8242F85C..0x8242F8B0  (save  r14..r31 + LR)
 *     __restgprlr_NN at 0x8242F8B0..0x8242F904  (restore r14..r31 + LR)
 *     __savefpr_NN   at 0x824365E0..0x8243662C  (save  f14..f31)
 *     __restfpr_NN   at 0x8243662C..0x82436678  (restore f14..f31)
 *
 * The FLIRT signatures fire on every individual bank entry (e.g.
 * `__savefpr_14`, `__savefpr_15`, ... `__restfpr_31`, `__restgprlr_23`,
 * ... `__restgprlr_31`). Those addresses are NOT independent functions —
 * they are fall-through entry points INSIDE the shared save/rest blob.
 * A call site for "spill r23..r31 + LR" branches into __restgprlr_23
 * (0x8242F8D4) and executes the `lwz r23..r31; mtlr r0; blr` tail.
 *
 * Every Nth routine in the belt is 4–8 bytes — a single
 * `stvx`/`lvx`/`std`/`ld` against `r1` or `r12` plus a `blr`. The
 * compiler branches into the belt at the specific entry that covers the
 * frame's spill count and lets control fall through the remaining
 * stores/loads until it hits `blr`.
 *
 * RECOMP BEHAVIOUR
 * ----------------
 * The pass5 static recompiler already preserves the full non-volatile
 * register file — r14..r31, LR, f14..f31, v14..v127 — through its
 * PPCRegContext, because every callee operates on the same `ctx` block.
 * The spill/restore belts therefore have NO observable effect on
 * guest-visible state in the recompiled build.
 *
 * Worse than useless: pass5 emits each `__savegprlr_NN`/`__restgprlr_NN`
 * alias as its own PPC_FUNC_IMPL (see
 * `recomp/structured_pass5_final/tt-decomp_recomp.24.cpp` line 60105+),
 * and each body stores/loads uninitialised host locals (`uint32_t
 * var_r14 = 0; PPC_STORE_U64(... var_r14)`), which silently zeroes
 * ctx.r14..r31 and ctx.lr at every spill/reload call-site. Registering
 * each interior entry against a shared no-op is therefore both a
 * performance win AND a correctness fix.
 *
 * We install two canonical host no-ops:
 *   - ppc_helper_vmx_noop    covers __savevmx_NN   / __restvmx_NN
 *   - ppc_helper_gprlr_noop  covers __savegprlr_NN / __restgprlr_NN
 *
 * See:
 *   - crt_hooks.h   for the full CRT_HOOK_ADDR_*vmx_NN / *gprlr_NN lists.
 *   - crt_hooks.cpp g_crt_hook_table[] for the registrations.
 */

#include <stdint.h>
#include "crt/ppc_recomp.h"   /* PPC_FUNC_IMPL, ppc_context_t */

/**
 * ppc_helper_vmx_noop — canonical no-op for every __savevmx_NN / __restvmx_NN
 *                       entry point in the 0x82569450..0x82569980 belt.
 *
 * The PPC body on the guest side would be:
 *     stvx vN, r0, r12    ;  (save)
 *   or
 *     lvx  vN, r0, r12    ;  (restore)
 *     blr                 ;  return to caller
 *
 * In the static recomp, ctx.vN survives the call automatically — there
 * is no guest-visible register bank sitting in a stack frame we need to
 * service. So the hook just returns.
 *
 * The current ppc_recomp.h in this tree has PPC_FUNC_IMPL expanding to an
 * empty token, so we spell out the two-argument signature directly. Once
 * the real recomp runtime is wired in, this becomes `PPC_FUNC_IMPL(...)`.
 */
void ppc_helper_vmx_noop(ppc_context_t* ctx, uint8_t* base) {
    (void)ctx;
    (void)base;
}

/**
 * ppc_helper_gprlr_noop — canonical no-op for every __savegprlr_NN /
 *                         __restgprlr_NN entry in the
 *                         0x8242F860..0x8242F904 fall-through belt.
 *
 * PPC body on the guest side for __savegprlr at 0x8242F860 (80 bytes):
 *     std  r14, -152(r1)
 *     std  r15, -144(r1)
 *     ...
 *     std  r31,  -16(r1)
 *     stw  r12,   -8(r1)
 *     blr
 *
 * And for __restgprlr at 0x8242F8B0 (84 bytes) the symmetric `ld`
 * sequence plus `mtlr r0` and `blr`. Because the pass5 translation
 * zeroes r14..r31 through uninitialised `var_rN` locals, registering
 * each interior entry against this shared no-op is a correctness fix
 * (not just a perf optimisation): it short-circuits the buggy
 * round-trip and preserves the ctx-carried non-volatile values.
 */
void ppc_helper_gprlr_noop(ppc_context_t* ctx, uint8_t* base) {
    (void)ctx;
    (void)base;
}

/*
 * ─────────────────────────────────────────────────────────────────────────
 * FLIRT batch-06 / batch-07 — GPR/FPR save+restore belt disposition
 * ─────────────────────────────────────────────────────────────────────────
 *
 *  __restgprlr_NN  (9 entries, 0x8242F8D4 .. 0x8242F8F4)
 *    0x8242F8D4  __restgprlr_23   (lwz r23,-0x24(r1); ... ; mtlr r0; blr)
 *    0x8242F8D8  __restgprlr_24
 *    0x8242F8DC  __restgprlr_25
 *    0x8242F8E0  __restgprlr_26
 *    0x8242F8E4  __restgprlr_27
 *    0x8242F8E8  __restgprlr_28
 *    0x8242F8EC  __restgprlr_29
 *    0x8242F8F0  __restgprlr_30
 *    0x8242F8F4  __restgprlr_31
 *
 *  __savefpr_NN   (11 entries, 0x824365E0 .. 0x82436624)
 *    0x824365E0  __savefpr_14   (stfd f14,-0x90(r1))
 *    0x824365E4  __savefpr_15
 *    0x824365E8  __savefpr_16
 *    0x824365EC  __savefpr_17
 *    0x824365F0  __savefpr_18
 *    0x824365F4  __savefpr_19
 *    0x824365F8  __savefpr_20
 *    0x824365FC  __savefpr_21
 *    0x82436600  __savefpr_22
 *    0x82436604  __savefpr_23
 *    0x82436624  __savefpr_31
 *
 *  __restfpr_NN   (11 entries, 0x8243662C .. 0x82436670)
 *    0x8243662C  __restfpr_14   (lfd f14,-0x90(r1))
 *    0x82436630  __restfpr_15
 *    0x82436634  __restfpr_16
 *    0x82436638  __restfpr_17
 *    0x8243663C  __restfpr_18
 *    0x82436640  __restfpr_19
 *    0x82436644  __restfpr_20
 *    0x82436648  __restfpr_21
 *    0x8243664C  __restfpr_22
 *    0x82436650  __restfpr_23
 *    0x82436670  __restfpr_31
 *
 * Disposition (all 31): no-action PPC helpers. Unlike the VMX belt, the
 * static recompiler does NOT produce a distinct `__imp____restgprlr` /
 * `__imp____savefpr` PPC_FUNC_IMPL — a search of pass5_final turns up
 * zero matches. The recomp weaves the equivalent "reload r23..r31 / LR"
 * step directly into each translated caller's epilogue. So these
 * addresses are never *targets* of a guest call in the recompiled build,
 * and registering a hook for any of them would be a dead entry.
 *
 * We document them here (not stubbed) to record that batch-06 triaged
 * them and confirmed no host action is required. The parent symbols
 * `__savegprlr` (0x8242F85C), `__restgprlr` (0x8242F8B0), `__savefpr`
 * (0x824365E0), `__restfpr` (0x8243662C) are similarly handled.
 */
