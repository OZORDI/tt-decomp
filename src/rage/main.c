/**
 * rage/main.c — RAGE engine entry dispatch
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Implements rage_main_6970, the top-level handoff from CRT startup into the
 * engine runtime. This routine owns:
 *   - startup argument canonicalization
 *   - one-time network probe/validation
 *   - pre/post run vtable dispatch on the global SDA allocator object
 *   - call-depth guarded network teardown
 */

#include "rage/rage_allocator.h"
#include <stddef.h>
#include <stdint.h>

/* External phase-3 dependencies */
void rage_setup_args_67F8(void* argOut0, void* argOut1);  /* @ 0x821867F8 */
void netHardware_1C60_v12(void* statusOut);               /* @ 0x82481C60 */
int util_1AF8(uint32_t type, void* statusOut);            /* @ 0x82481AF8 */
void nop_8240E6D0(const char* fmt, ...);                  /* @ 0x8240E6D0 */
void rage_1B08(void);                                     /* @ 0x82481B08 */
void rage_app_init_6418(void* argBuf0, void* argBuf1);   /* @ 0x82186418 */
void rage_get_exe_name_6628(void* outToken);              /* @ 0x82186628 */
int rage_run_6780(void);                                  /* @ 0x82186780 */

/* SDA + data references resolved from lis/addi pairs in rage_main_6970 */
extern rageAllocator* g_pAllocator;       /* SDA[0] + 4 */
extern int32_t lbl_82601E50;              /* @ 0x82601E50 call-depth guard */
extern const char lbl_8205AD90[];         /* @ 0x8205AD90 */
extern const char lbl_8205ADC4[];         /* @ 0x8205ADC4 */
extern const void* lbl_820385D4;          /* @ 0x820385D4 */
extern const void* lbl_820385E8;          /* @ 0x820385E8 */
extern const void* lbl_820385F8;          /* @ 0x820385F8 */

typedef struct RageMainGate {
    uint32_t m_pad0;        /* +0x00 */
    uint8_t* m_pStateByte;  /* +0x04 */
    uint8_t  m_pad1[12];    /* +0x08 */
} RageMainGate;

extern RageMainGate lbl_825CAE40;         /* @ 0x825CAE40 */
extern RageMainGate lbl_825CAE68;         /* @ 0x825CAE68 */
extern RageMainGate lbl_825CAE7C;         /* @ 0x825CAE7C */
extern RageMainGate lbl_825CAE54;         /* @ 0x825CAE54 */

typedef struct RageMainDispatchVtable {
    void* m_slots[17];  /* slots 0..16 */
} RageMainDispatchVtable;

typedef struct RageMainDispatchObject {
    RageMainDispatchVtable* vtable;
} RageMainDispatchObject;

typedef struct NetProbeStatus {
    uint8_t m_type;      /* +0 */
    uint8_t m_enabled;   /* +1 */
    uint8_t m_pad[11];   /* +2..+12 */
} NetProbeStatus;

static RageMainDispatchObject* rage_get_dispatch_obj(void)
{
    return (RageMainDispatchObject*)g_pAllocator;
}

static void rage_dispatch_slot7(RageMainDispatchObject* self, const void* arg)
{
    ((void (*)(RageMainDispatchObject*, const void*))self->vtable->m_slots[7])(self, arg);
}

static void rage_dispatch_slot13(RageMainDispatchObject* self)
{
    ((void (*)(RageMainDispatchObject*))self->vtable->m_slots[13])(self);
}

static void rage_dispatch_slot14(RageMainDispatchObject* self, const void* key, const void* value)
{
    ((void (*)(RageMainDispatchObject*, const void*, const void*))self->vtable->m_slots[14])(self, key, value);
}

static void rage_dispatch_slot15(RageMainDispatchObject* self, const void* arg, uint8_t active)
{
    ((void (*)(RageMainDispatchObject*, const void*, uint8_t))self->vtable->m_slots[15])(self, arg, active);
}

static void rage_dispatch_slot16(RageMainDispatchObject* self)
{
    ((void (*)(RageMainDispatchObject*))self->vtable->m_slots[16])(self);
}

/**
 * rage_main_6970 @ 0x82186970 | size: 0x278
 */
int rage_main_6970(void* rawArgs0, void* rawArgs1)
{
    void* arg0 = rawArgs0;
    void* arg1 = rawArgs1;
    rage_setup_args_67F8(&arg0, &arg1);

    int32_t callDepth = ++lbl_82601E50;
    if (callDepth <= 1) {
        NetProbeStatus probeStatus = {0};
        probeStatus.m_type = 13;
        probeStatus.m_enabled = 1;
        netHardware_1C60_v12(&probeStatus);

        uint16_t netStatusWord = 0;
        if (util_1AF8(2, &netStatusWord) != 0) {
            nop_8240E6D0(lbl_8205AD90);
        }

        uint8_t netStatusType = (uint8_t)netStatusWord;
        uint8_t netStatusResult = (uint8_t)(netStatusWord >> 8);
        if ((netStatusType != 2u) || (netStatusResult != 0u)) {
            rage_1B08();
            nop_8240E6D0(lbl_8205ADC4, netStatusWord);
        }
    }

    rage_app_init_6418(arg0, arg1);

    if (lbl_825CAE40.m_pStateByte != NULL) {
        uintptr_t exeNameToken = 0;
        rage_get_exe_name_6628(&exeNameToken);
        rage_dispatch_slot7(rage_get_dispatch_obj(), (const void*)exeNameToken);
    }

    RageMainDispatchObject* dispatchObj = rage_get_dispatch_obj();
    rage_dispatch_slot13(dispatchObj);

    const void* slot15Arg = &lbl_820385D4;
    uint8_t hasGate68 = 0;
    if (lbl_825CAE68.m_pStateByte != NULL) {
        if (lbl_825CAE68.m_pStateByte[0] != 0u) {
            slot15Arg = lbl_825CAE68.m_pStateByte;
        }
        hasGate68 = 1;
    }

    if (hasGate68 != 0u) {
        uint8_t gate7CActive = (lbl_825CAE7C.m_pStateByte != NULL) ? 1u : 0u;
        rage_dispatch_slot15(rage_get_dispatch_obj(), slot15Arg, gate7CActive);
    }

    int exitCode = rage_run_6780();

    if (lbl_825CAE68.m_pStateByte != NULL) {
        rage_dispatch_slot16(rage_get_dispatch_obj());
    }

    const void* slot14Arg = &lbl_820385E8;
    uint8_t hasGate54 = 0;
    if (lbl_825CAE54.m_pStateByte != NULL) {
        if (lbl_825CAE54.m_pStateByte[0] != 0u) {
            slot14Arg = lbl_825CAE54.m_pStateByte;
        }
        hasGate54 = 1;
    }

    if (hasGate54 == 0u) {
        slot14Arg = NULL;
    }

    rage_dispatch_slot14(rage_get_dispatch_obj(), &lbl_820385F8, slot14Arg);

    int32_t remainingDepth = --lbl_82601E50;
    if (remainingDepth == 0) {
        rage_1B08();
    }

    return exitCode;
}
