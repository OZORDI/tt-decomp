# Native CRT Hooks — Integration Guide

## What This Is

The Xbox 360 ships the C runtime compiled *into* the game binary. Without
hooks, functions like `memcpy`, `strlen`, and `sprintf` run as translated
PowerPC through the recompiler. They're correct, but they're paying the full
recomp dispatch and memory-indirection penalty on every call.

Native CRT hooks transparently redirect those call sites to the host
platform's own libc. On a modern x86-64 host, `memcpy` becomes an AVX2
vector copy; `strlen` becomes a `pcmpeqb`+`bsf` scan — orders of magnitude
faster than the translated scalar Xenon loop.

## Files

```
src/crt/
  crt_hooks.h      — address constants, CrtHookEntry type, registration API
  crt_hooks.cpp    — PPC_FUNC_IMPL wrappers + g_crt_hook_table[] + crt_hooks_register()
```

## What Gets Hooked

| Function           | Guest Address  | Replaced With        |
|--------------------|----------------|----------------------|
| `strlen`           | `0x820C0000`   | native `strlen`      |
| `strcmp`           | `0x82432970`   | native `strcmp`      |
| `strncpy`          | `0x82430120`   | native `strncpy`     |
| `memset`           | `0x8242FED0`   | native `memset`      |
| `memset_aligned`   | `0x8242FEF0`   | native `memset`      |
| `memmove`          | `0x8242FF70`   | native `memmove`     |
| `memmove_backward` | `0x8242FFAc`   | native `memmove`     |
| `memcpy`           | `0x82434100`   | native `memcpy`      |
| `sprintf`          | `0x825863BC`   | native `sprintf`     |
| `_snprintf`        | `0x820C0B40`   | native `snprintf`    |
| `_vsnprintf`       | `0x825863CC`   | **fallback** (TODO)  |

## What Is Intentionally NOT Hooked

**`sysMemAllocator_Allocate` / `Free` / `rage_free_00C0`** — the RAGE heap
owns its own virtual address ranges, stores a raw pointer at `user_ptr[-4]`
for bookkeeping, and uses `atSingleton_Find()` ownership checks to prevent
cross-heap frees. Routing through native `malloc`/`free` would bypass all
of that and corrupt the engine's memory invariants. Leave these alone.

## Host Integration

### 1. What the Host Needs to Provide

The hooks depend on two things from `ppc_recomp.h`:

```cpp
// Pointer type for recomp functions
typedef void (*PPC_FUNC_PTR)(PPCRegContext& ctx, uint8_t* base);

// Register a native function to replace the translated one at guest_addr.
// Returns true if the address was known and the hook was installed.
bool recomp_register_hook(uint32_t guest_addr, PPC_FUNC_PTR native_impl);

// For hooks that fall back to translated code (e.g. vsnprintf):
#define PPC_HOOK_FALLBACK()  /* invoke the translated version or return */

// Host memory base — all guest pointers are offsets into this.
extern uint8_t* g_mem_base;
```

### 2. Call Site

In your host init, after the dispatch table is built but before any guest
code runs:

```cpp
#include "crt/crt_hooks.h"

int main() {
    recomp_init();        // builds the function dispatch table
    
    int n = crt_hooks_register();
    printf("[host] installed %d native CRT hooks\n", n);
    
    recomp_run();         // now runs game code with hooks active
}
```

### 3. Verbose Logging

Build with `-DCRT_HOOKS_VERBOSE` to log each installed hook:

```
[crt_hooks] hooked strlen               @ 0x820C0000
[crt_hooks] hooked strcmp               @ 0x82432970
...
```

## Calling Convention Notes

All hooks follow PPC register calling convention:

```
Integer args:  r3 = arg1, r4 = arg2, r5 = arg3, r6 = arg4, ...
Integer return: r3
Float args:    f1 = arg1, f2 = arg2, ...
Float return:  f1
```

Guest pointers are 32-bit guest virtual addresses. Convert with:
```cpp
void* host = GUEST_PTR(ctx.r3);          // guest addr → host ptr
ctx.r3.u32 = HOST_TO_GUEST(host_ptr);    // host ptr → guest addr
```

## Known Limitations

### sprintf / snprintf — Integer Args Only

The PPC variadic ABI saves integer args in r5–r10 (6 slots) and float args
in f1–f8 with a parallel stack save area. The current sprintf hook
reconstructs up to 6 integer arguments — which covers virtually all RAGE
log/debug/HUD formatting calls.

Format strings needing more than 6 integer args, or any `%f`/`%g` float
conversions, will produce incorrect output with the hook. Add a float-arg
reconstruction path if needed:

```cpp
// Detecting float args requires scanning the format string:
for (const char* p = fmt; *p; ++p) {
    if (*p == '%') {
        // skip flags/width/precision...
        if (*p == 'f' || *p == 'g' || *p == 'e')
            // next arg is float — read from ctx.f1, f2, ...
    }
}
```

### _vsnprintf — Not Hooked

PPC `va_list` is a struct on the guest stack (gpr_count, fpr_count,
overflow_arg_area, gpr_save_area, fpr_save_area). Building a native `va_list`
from it is platform-specific and complex. The hook currently falls back to
the translated version. A full implementation is tracked as TODO.

## Performance Impact (Expected)

On an x86-64 host copying 16KB buffers:

| Function     | Translated PPC | Native Hook | Speedup |
|--------------|---------------|-------------|---------|
| `memcpy`     | ~4,200 ns     | ~180 ns     | ~23×    |
| `memset`     | ~3,100 ns     | ~90 ns      | ~34×    |
| `strlen`     | ~280 ns       | ~12 ns      | ~23×    |

(Estimates based on Xenon scalar loop vs. AVX2. Actual numbers vary.)

Given that `memcpy` and `memset` are called millions of times per frame
across physics, animation, network, and renderer subsystems, this is
one of the highest-leverage single changes in the entire decomp project.
