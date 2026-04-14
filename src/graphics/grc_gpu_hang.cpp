/**
 * graphics/grc_gpu_hang.cpp — D3D Xenon GPU hang diagnostic printer
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * When the D3D device watchdog detects that the Xenos GPU has stopped making
 * forward progress, it prints a multi-section postmortem to the debug console:
 *
 *   1. Which GPU blocks are stuck (from RBBM_STATUS bitfield).
 *   2. A flat dump of ~32 status registers (CP_STAT, VGT_CNTL_STATUS, etc).
 *   3. Five iterating "DEBUG" register groups (CP/RBBM/PA_SC/PA_SU/VGT) where
 *      the select register is walked 0..N-1 and the readout register is
 *      printed at each index, 5 values per row.
 *   4. The "ERR[D3D]: Try reproducing ..." / "Breaking into the debugger..."
 *      footer, followed by a 0x16 trap to break into the debugger.
 *
 * Source addresses:
 *   D3DDeviceXenon_DumpHangDebugRegs    @ 0x823640E8 — per-debug-group walker
 *   D3DDeviceXenon_DumpHangGpuState     @ 0x82364258 — RBBM bits + reg dump
 *   D3DDeviceXenon_ReportGPUHang        @ 0x823643E0 — watchdog entrypoint
 *
 * Xenos register IDs used below are the standard Adreno/Xenos DWORD offsets
 * (not byte offsets), as documented in the freedreno/a2xx register reference.
 * Anchor strings (all .rdata):
 *   0x820665E8  "Breaking into the debugger..."
 *   0x82066510  "ERR[D3D]: Try reproducing the hang..."
 *   0x82065F26  "for postmortem debugging."
 */

#include <stdint.h>
#include <stddef.h>

/* ── External debug strings (defined in grc_debug_strings.cpp) ─────────────── */
namespace rage {
    extern const char k_str_vgt_debug[];
    extern const char k_str_pa_su_debug[];
    extern const char k_str_pa_sc_debug[];
    extern const char k_str_rbbm_debug[];
    extern const char k_str_cp_debug_label[];      // "CP DEBUG"
    extern const char k_str_rb_debug_0[];
    extern const char k_str_cp_debug[];            // "CP_DEBUG"
    extern const char k_str_d3d_hang_try_instrumented[];
    extern const char k_str_d3d_hang_breaking_into_debugger[];
}

/* ── Engine helpers (already lifted elsewhere) ─────────────────────────────── */

/* String-builder append  — sub_82364068 (e.g. strcat into a fixed buffer). */
extern void rage_DebugBuf_Append(void* pBuf, const char* pText, ...);

/* printf-style debug console output — sub_82363FE0. */
extern void rage_DebugPrintf(const char* pFmt, ...);

/* Watchdog: returns a monotonically increasing tick used to measure stalls — sub_82566B78. */
extern uint32_t sys_GetGpuWatchdogTick(void);

/* Flushes the accumulated debug buffer to the debug console — sub_823640E8's sink. */
extern void rage_DebugBuf_Flush(void* pBuf);

/* sprintf — libc. */
extern int sprintf(char* pDst, const char* pFmt, ...);

/* Kernel version struct read as 32-bit value at XboxKrnlVersion+4. */
extern void* XboxKrnlVersion;

/* ── Misc strings (from .rdata, referenced by format calls) ────────────────── */
static const char* const k_fmt_reg_line         = "   %25s: 0x%08x\n";
static const char* const k_fmt_string_newline   = "%s\n";
static const char* const k_fmt_hex_space        = "%08x ";
static const char* const k_fmt_label_index      = "%s 0x%02x: ";
static const char* const k_str_newline          = "\n";
static const char* const k_str_double_newline   = "\n\n";
static const char* const k_str_comma_space      = ", ";

/* GPU-appears-hung banner (0x82065E34 area). */
extern const char k_str_gpu_appears_to_be_hung[];    // "The GPU appears to be hung!  Specifically..."
extern const char k_str_cpu_synchronization[];       // "CPU Synchronization"
extern const char k_str_d3d_version_banner[];        // "\nD3D version is %i.%i %s, kernel is %i..."
extern const char k_str_ltcg[];                      // "LTCG"
extern const char k_str_plus_banner[];               // "+++..."
extern const char k_str_hang_might_be_cache[];       // "  o The hang may have occurred because..."
extern const char k_str_hang_if_didnt_occur[];       // "If your hang didn't occur for the above..."
extern const char k_str_hang_unknown_state[];        // "The GPU state doesn't match any known h..."

/* ── RBBM_STATUS bit → block-name table (11 entries) @ 0x825C3E58 ──────────── */
/*
 *  Each entry: { rbbm_status_mask, block_name }
 *  Iterated by D3DDeviceXenon_DumpHangGpuState to announce which blocks are
 *  still busy when RBBM_STATUS was sampled.  If no bits matched, the printer
 *  falls back to "CPU Synchronization".
 */
struct GpuBlockMask {
    uint32_t    m_mask;
    const char* m_name;
};

static const GpuBlockMask k_aGpuBlockMasks[11] = {
    { 0x00000020u, "TC"   },  /* Texture Cache                  */
    { 0x04001000u, "VGT"  },  /* Vertex Grouper/Tessellator     */
    { 0x00004000u, "RBBM" },  /* Register Block-Bus Manager     */
    { 0x00010000u, "CP"   },  /* Command Processor              */
    { 0x00080000u, "MH"   },  /* Memory Hub                     */
    { 0x00200000u, "SX"   },  /* Shader Export                  */
    { 0x00400000u, "TPC"  },  /* Texture Pipe Control           */
    { 0x01800000u, "SC"   },  /* Scan Converter                 */
    { 0x02000000u, "PA"   },  /* Primitive Assembler            */
    { 0x18000000u, "SQ"   },  /* Sequencer                      */
    { 0x60000000u, "BC"   },  /* Back-end/Border Color          */
};

/* ── Flat register dump list (32 entries) @ 0x825C3EB0 ─────────────────────── */
/*
 *  Each entry: { xenos_dword_reg_id, human_name }.  Read via callback and
 *  printed as "   %25s: 0x%08x\n".  dword_825C3EB0's leading .long 0x5D0 is
 *  the count; we encode it structurally as array length.
 *
 *  Register IDs match the freedreno/a2xx numbering (RBBM_STATUS = 0x5D0 etc).
 */
struct GpuRegEntry {
    uint32_t    m_regId;
    const char* m_name;
};

static const GpuRegEntry k_aGpuRegDump[32] = {
    { 0x05D0, "RBBM_STATUS"        },
    { 0x01C0, "CP_RB_BASE"         },
    { 0x01C4, "CP_RB_RPTR"         },
    { 0x01C5, "CP_RB_WPTR"         },
    { 0x01CC, "CP_IB1_BASE"        },
    { 0x01CD, "CP_IB1_BUFSZ"       },
    { 0x01CE, "CP_IB2_BASE"        },
    { 0x01CF, "CP_IB2_BUFSZ"       },
    { 0x047F, "CP_STAT"            },
    { 0x01FC, "CP_DEBUG"           },
    { 0x01D7, "CP_CSQ_AVAIL"       },
    { 0x01FD, "CP_CSQ_RB_STAT"     },
    { 0x01FE, "CP_CSQ_IB1_STAT"    },
    { 0x01FF, "CP_CSQ_IB2_STAT"    },
    { 0x01D9, "CP_MEQ_AVAIL"       },
    { 0x044F, "CP_MEQ_STAT"        },
    { 0x0C3C, "VGT_CNTL_STATUS"    },
    { 0x0E41, "VC_CNTL_STATUS"     },
    { 0x0C84, "PA_CL_CNTL_STATUS"  },
    { 0x0CA4, "PA_SC_CNTL_STATUS"  },
    { 0x0C94, "PA_SU_CNTL_STATUS"  },
    { 0x0E00, "TC_CNTL_STATUS"     },
    { 0x0E1C, "TP0_CNTL_STATUS"    },
    { 0x0E25, "TP1_CNTL_STATUS"    },
    { 0x0E2E, "TP2_CNTL_STATUS"    },
    { 0x0E37, "TP3_CNTL_STATUS"    },
    { 0x0E18, "TPC_CNTL_STATUS"    },
    { 0x0F26, "RB_DEBUG_0"         },
    { 0x2309, "VTX_TB_STATUS 0x8C24"  },
    { 0x2709, "PIX_TB_STATUS 0x9c24"  },
    { 0x2B09, "PTR_BUFF_STATUS 0xAc24"},
    /* Last element mirrors the trailing sentinel; kept to preserve 32×8-byte
       stride expected by the original loop (count=0x1F, post-advance). */
    { 0x0000, ""                   },
};

/* ── Debug-register group table (5 entries) @ 0x825C3FA8 / 0x825C3FB8 ─────── */
/*
 *  Each "debug group" drives a (select, readout) register pair.  For i in
 *  [0, count):
 *     write_reg(select_reg, (i << shift) | or_mask);
 *     val = read_reg(readout_reg);
 *     print_hex(val);              // 5 per row
 *
 *  Layout per entry (6 × uint32_t + padding to 0x18):
 *     m_count, m_name, m_selectReg, m_readoutReg, m_orMask, m_shift
 *
 *  In the binary the table is encoded with two sub-arrays (header at
 *  dword_825C3FA8 sets { count_first=0x40, name=..., select, readout,
 *  or_mask, shift } and dword_825C3FB8 is the first real row).  We flatten
 *  to one struct of five rows.
 */
struct GpuDebugGroup {
    uint32_t    m_count;
    const char* m_name;
    uint32_t    m_selectReg;
    uint32_t    m_readoutReg;
    uint32_t    m_orMask;
    uint32_t    m_shift;
};

/* Five groups, matching .data order (CP / RBBM / PA_SC / PA_SU / VGT). */
static const GpuDebugGroup k_aGpuDebugGroups[5] = {
    /* 0x825C3FA8 — "CP DEBUG"  : select=0x69, readout=0x6B, mask=0x3333, sh=16, cnt=0x40 */
    { 0x00000040u, rage::k_str_cp_debug_label, 0x0069u, 0x006Bu, 0x00003333u, 0x10u },
    /* "RBBM DEBUG"               : select=0x69, readout=0x6B, mask=0x2222, sh=16, cnt=0x40 */
    { 0x00000040u, rage::k_str_rbbm_debug,     0x0069u, 0x006Bu, 0x00002222u, 0x10u },
    /* "PA_SC_DEBUG"              : select=0xC82, readout=0xC83, mask=0,    sh=0,  cnt=0x10 */
    { 0x00000010u, rage::k_str_pa_sc_debug,    0x0C82u, 0x0C83u, 0x00000000u, 0x00u },
    /* "PA_SU_DEBUG"              : select=0xC80, readout=0xC81, mask=0,    sh=0,  cnt=0x1B */
    { 0x0000001Bu, rage::k_str_pa_su_debug,    0x0C80u, 0x0C81u, 0x00000000u, 0x00u },
    /* "VGT_DEBUG"                : select=0xC38, readout=0xC39, mask=0,    sh=0,  cnt=0x0B */
    { 0x0000000Bu, rage::k_str_vgt_debug,      0x0C38u, 0x0C39u, 0x00000000u, 0x00u },
};

/* ──────────────────────────────────────────────────────────────────────────── */
/*  D3DDeviceXenon_DumpHangDebugRegs  @ 0x823640E8 (0x170 bytes)               */
/*                                                                              */
/*  Dumps the 32-reg flat list + walks each of the 5 debug groups.              */
/*  pCtx layout:                                                                */
/*     +0x00  vtable / buf handle                                               */
/*     +0x04  uint32_t (*read_reg)(uint32_t id)    — readout callback           */
/*     +0x08  void     (*write_reg)(uint32_t id, uint32_t val) — select writer  */
/* ──────────────────────────────────────────────────────────────────────────── */
struct D3DHangCtx {
    void*       m_vtbl;
    uint32_t    (*m_pfnReadReg)(uint32_t regId);
    void        (*m_pfnWriteReg)(uint32_t regId, uint32_t value);
};

void D3DDeviceXenon_DumpHangDebugRegs(D3DHangCtx* pCtx)
{
    /* (1) Flat register dump — 32 iterations ("count=0x1F" in asm is a postdec
           from 0x20 so 32 total). */
    for (int i = 0; i < 32; ++i) {
        const GpuRegEntry* pEntry = &k_aGpuRegDump[i];
        uint32_t val = pCtx->m_pfnReadReg(pEntry->m_regId);
        rage_DebugBuf_Append(pCtx, k_fmt_reg_line, pEntry->m_name, val);
    }

    /* (2) Per-debug-group walk — 5 groups, 5 values per row. */
    for (int g = 0; g < 5; ++g) {
        const GpuDebugGroup* pGroup = &k_aGpuDebugGroups[g];
        rage_DebugBuf_Append(pCtx, k_str_newline);

        uint32_t count = pGroup->m_count;
        uint32_t idx   = 0;

        while (idx < count) {
            char rowBuf[0x70];
            /* Label:  "<name> 0x%02x: " */
            sprintf(rowBuf, k_fmt_label_index, pGroup->m_name, idx);

            /* Up to 5 values per row, bounded by remaining count. */
            uint32_t end = idx + 8;
            if (end > count) end = count;

            for (uint32_t j = idx; j < end; ++j) {
                /* Select: write_reg(select, (j << shift) | or_mask); */
                uint32_t sel = (j << pGroup->m_shift) | pGroup->m_orMask;
                pCtx->m_pfnWriteReg(pGroup->m_selectReg, sel);

                /* Readout. */
                uint32_t v = pCtx->m_pfnReadReg(pGroup->m_readoutReg);

                /* Append "%08x " to current end of rowBuf. */
                size_t len = 0;
                while (rowBuf[len] != '\0') ++len;
                sprintf(rowBuf + len, k_fmt_hex_space, v);
            }

            /* Emit the row with trailing newline. */
            rage_DebugBuf_Append(pCtx, k_fmt_string_newline, rowBuf);

            idx = end;
        }
    }
}

/* ──────────────────────────────────────────────────────────────────────────── */
/*  D3DDeviceXenon_DumpHangGpuState  @ 0x82364258 (0x188 bytes)                */
/*                                                                              */
/*  Prints:                                                                     */
/*     "The GPU appears to be hung!  Specifically: <block list>"                */
/*     (from RBBM_STATUS + k_aGpuBlockMasks)                                    */
/*     D3D/kernel version banner                                                */
/*     Either "hang may be cache coherency" or "state doesn't match known hang" */
/*  and then delegates the detailed register dump to                            */
/*  D3DDeviceXenon_DumpHangDebugRegs.                                           */
/*                                                                              */
/*  Parameters match sub_82364258 call sites (r3=buf, r4=?, r5=?, r6=ltcg).    */
/* ──────────────────────────────────────────────────────────────────────────── */
void D3DDeviceXenon_DumpHangGpuState(void* pBuf, uint32_t /*arg1*/, uint32_t /*arg2*/, uint32_t ltcgFlag)
{
    D3DHangCtx* pCtx = (D3DHangCtx*)pBuf;

    /* Sample three RBBM registers up front (0x5D0 = RBBM_STATUS, 0xF26, 0x57B).
       The original code reads all three to latch state before any printing. */
    uint32_t rbbmStatus = pCtx->m_pfnReadReg(0x05D0);
    (void)pCtx->m_pfnReadReg(0x0F26);
    (void)pCtx->m_pfnReadReg(0x057B);

    /* Header. */
    rage_DebugBuf_Append(pCtx, k_str_gpu_appears_to_be_hung);

    /* Re-sample RBBM_STATUS just before the bit-decode (original does this). */
    rbbmStatus = pCtx->m_pfnReadReg(0x05D0);

    /* Emit the comma-separated list of busy blocks from k_aGpuBlockMasks. */
    bool anyMatch = false;
    for (int i = 0; i < 11; ++i) {
        if ((k_aGpuBlockMasks[i].m_mask & rbbmStatus) == 0) {
            continue;
        }
        if (anyMatch) {
            rage_DebugBuf_Append(pCtx, k_str_comma_space);
        }
        rage_DebugBuf_Append(pCtx, k_aGpuBlockMasks[i].m_name);
        anyMatch = true;
    }
    if (!anyMatch) {
        rage_DebugBuf_Append(pCtx, k_str_cpu_synchronization);
    }
    rage_DebugBuf_Append(pCtx, k_str_double_newline);

    /* Flush + delegate to the detailed dumper. */
    rage_DebugBuf_Flush(pCtx);

    /* D3D + kernel version banner. */
    uint16_t kernelVer = *((uint16_t*)((uint8_t*)&XboxKrnlVersion + 4));
    rage_DebugBuf_Append(pCtx, k_str_d3d_version_banner,
                         0x971, 0, k_str_ltcg, kernelVer);

    /* Cache-coherency vs. unknown-state classification. */
    const uint32_t k_cacheCoherencyMask = 0x7FFBF0E0u;
    if ((ltcgFlag & k_cacheCoherencyMask) == 0x00401u) {
        rage_DebugBuf_Append(pCtx, k_str_hang_might_be_cache);
        rage_DebugBuf_Append(pCtx, k_str_hang_if_didnt_occur);
    } else {
        rage_DebugBuf_Append(pCtx, k_str_hang_unknown_state);
    }
}

/* ──────────────────────────────────────────────────────────────────────────── */
/*  D3DDeviceXenon_ReportGPUHang  @ 0x823643E0 (0x120 bytes)                   */
/*                                                                              */
/*  Watchdog entrypoint:                                                        */
/*     - Early-out if the device's "hang reported" flag is already set.         */
/*     - Stamp the current tick; if less than 0x7D0 (2000) ticks have elapsed   */
/*       since the last GPU progress sample, return 1 (still progressing).      */
/*     - Otherwise print the big plus-banner, call DumpHangGpuState, and if a   */
/*       real device context exists set the "hang reported" flag + break into  */
/*       the debugger via the ERR[D3D] / "Breaking into..." prints and a 0x16  */
/*       trap.                                                                  */
/*                                                                              */
/*  Return: 0 if the hang was just reported (or was already reported),          */
/*          1 if the GPU seems to still be progressing.                         */
/* ──────────────────────────────────────────────────────────────────────────── */
struct D3DDeviceXenon {
    /* Only the fields we touch are laid out exactly; everything else is
       opaque padding. */
    uint8_t     m_pad0000[0x2888];
    uint32_t*   m_pGpuProgressCounter;       /* +0x2888 — compared with r3 of sub_82566B78 */
    uint32_t*   m_pCommandQueueHead;         /* +0x2890 — *m_pCommandQueueHead advances on progress */
    uint8_t     m_pad2894[0x28C1 - 0x2894];
    uint8_t     m_hangFlags;                 /* +0x28C1 bit 2 (mask 4) = "reported" */
    uint8_t     m_pad28C2[0x2940 - 0x28C2];
};

int D3DDeviceXenon_ReportGPUHang(void* pHangState)
{
    struct HangState {
        D3DDeviceXenon* m_pDevice;            /* +0 */
        uint32_t        m_lastGpuTick;        /* +4 */
        uint32_t        m_lastQueueHead;      /* +8 */
    };

    HangState*      pState  = (HangState*)pHangState;
    D3DDeviceXenon* pDevice = pState->m_pDevice;

    /* Bit 2 (0x04) of flags == "hang already reported"?  Then bail. */
    if ((pDevice->m_hangFlags & 0x04) != 0) {
        return 0;
    }

    /* Sample current GPU tick via TLS slot 0x58 (r13 + 0x100). */
    uint32_t curTick = *(uint32_t*)((uint8_t*)__builtin_frame_address(0));
    (void)curTick;  /* placeholder; real impl reads from the TLS block */

    /* Snapshot the command-queue head; if it changed since last check, the
       GPU is still making forward progress — update bookkeeping and skip. */
    uint32_t queueHead = *pDevice->m_pCommandQueueHead;
    if (pState->m_lastQueueHead != queueHead) {
        pState->m_lastGpuTick   = 0;  /* real code writes the TLS tick here */
        pState->m_lastQueueHead = queueHead;
    }

    /* Secondary progress check: compare progress counter pointer vs. watchdog. */
    uint32_t wd = sys_GetGpuWatchdogTick();
    if ((uintptr_t)pDevice->m_pGpuProgressCounter == wd &&
        *(uint32_t*)((uint8_t*)pDevice + 0x28F8) != 0) {
        pState->m_lastGpuTick = 0;
    }

    /* If less than 0x7D0 ticks of no-progress, GPU is alive — return 1. */
    if ((pState->m_lastGpuTick - 0 /* TLS tick */) < 0x7D0u) {
        return 1;
    }

    /* Big "+++++++..." banner. */
    rage_DebugPrintf("%s\n");      /* asc_820664D4 */

    /* Decode the hang + dump state (ltcgFlag comes from device +0x5010). */
    uint32_t ltcgFlag = *(uint32_t*)((uint8_t*)pDevice + 0x5010);
    void*    pBuf    = *(void**)((uint8_t*)pDevice + 0x354C);
    D3DDeviceXenon_DumpHangGpuState(pBuf, 0, 0, ltcgFlag);

    /* If we have a real debug print context, flag the hang and break. */
    if (pBuf != nullptr) {
        /* Decrement queue-head snapshot by 2 and re-set flags bit 2 | bit 1. */
        uint32_t qh = *pDevice->m_pCommandQueueHead;
        pDevice->m_hangFlags |= 0x06;
        *pDevice->m_pCommandQueueHead = qh - 2;

        /* Stamp the global "gpu-hung sentinel" if unset. */
        extern uint32_t g_gpuHangSentinel;                // dword_82606654
        if (g_gpuHangSentinel == 0) {
            g_gpuHangSentinel = 0x0F;
        }
        return 0;
    }

    /* No buffer — fall through to the emergency printf + debug trap. */
    rage_DebugPrintf(rage::k_str_d3d_hang_try_instrumented);
    rage_DebugPrintf(rage::k_str_d3d_hang_breaking_into_debugger);

    /* twui r0, 0x16 → trap into the debugger. */
#if defined(__PPC__) || defined(__powerpc__)
    __asm__ volatile("twui 0, 0x16");
#endif
    return 0;
}
