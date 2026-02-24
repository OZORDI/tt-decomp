#pragma once

// CRT initialization functions
// Assembly addresses: 0x820C0038 - 0x820C0374

#ifdef __cplusplus
extern "C" {
#endif

// xe_main_thread_init_0038 @ 0x820C0038 (size: 0x88)
// Thread/locale initialization stub
void xe_main_thread_init_0038(void);

// _locale_register @ 0x820C02D0 (size: 0xA4)
// Locale/heap registration with address range checking
void _locale_register(void* addr, int type);

// ref_0BA0_h @ 0x820C0BA0 (size: 0x4C)
// Reverse iteration helper - calls function pointer on array elements
void ref_0BA0_h(void* base, int stride, int count, void (*func)(void*));

#ifdef __cplusplus
}
#endif
