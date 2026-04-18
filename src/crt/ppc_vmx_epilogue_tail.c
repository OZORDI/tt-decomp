/**
 * PPC VMX Register-Restore Helper Belt — Tail (__restvmx_85 .. __restvmx_127)
 * Rockstar Presents Table Tennis — FLIRT batch_09 (agent F10)
 *
 * These 43 addresses (0x82569824 .. 0x82569974, 8-byte strides) are NOT
 * independent functions.  They are the tail entries of the Xbox 360 XCC
 * __restvmx register-restore belt — a single shared epilogue routine that
 * each call site enters at a different offset depending on how many VMX
 * registers (v14..v127) it needs to reload from the caller's stack frame.
 *
 * Disassembly pattern at 0x82569824 (__restvmx_85):
 *     li   r11,-688
 *     lvx128 v85,r11,r12
 *     li   r11,-672
 *     lvx128 v86,r11,r12
 *     ...
 *     li   r11,-16
 *     lvx128 v127,r11,r12
 *     blr
 *
 * Each subsequent 8-byte row is a fall-through stepping stone (li + lvx128)
 * that continues into the next register slot.  pass5_final emits the full
 * epilogue under __imp____restvmx_85 and chains later entry points via
 * __attribute__((alias)) so that all 43 symbols resolve to the same native
 * call site without duplicating the body.
 *
 * The recomp already handles the guest-visible effect (restoring v85..v127
 * from the caller stack), so this translation unit exists solely to document
 * the disposition.  There is nothing to decompile — translating the PPC body
 * into C++ game code would be meaningless because the helper mutates only
 * simulated VMX registers on the PPC ctx, which the recomp already preserves
 * across function calls.
 *
 * Addresses in this batch (all dispositioned as PPC-helper tail, no action):
 *   0x82569824 __restvmx_85     0x82569894 __restvmx_99     0x82569904 __restvmx_113
 *   0x8256982c __restvmx_86     0x8256989c __restvmx_100    0x8256990c __restvmx_114
 *   0x82569834 __restvmx_87     0x825698a4 __restvmx_101    0x82569914 __restvmx_115
 *   0x8256983c __restvmx_88     0x825698ac __restvmx_102    0x8256991c __restvmx_116
 *   0x82569844 __restvmx_89     0x825698b4 __restvmx_103    0x82569924 __restvmx_117
 *   0x8256984c __restvmx_90     0x825698bc __restvmx_104    0x8256992c __restvmx_118
 *   0x82569854 __restvmx_91     0x825698c4 __restvmx_105    0x82569934 __restvmx_119
 *   0x8256985c __restvmx_92     0x825698cc __restvmx_106    0x8256993c __restvmx_120
 *   0x82569864 __restvmx_93     0x825698d4 __restvmx_107    0x82569944 __restvmx_121
 *   0x8256986c __restvmx_94     0x825698dc __restvmx_108    0x8256994c __restvmx_122
 *   0x82569874 __restvmx_95     0x825698e4 __restvmx_109    0x82569954 __restvmx_123
 *   0x8256987c __restvmx_96     0x825698ec __restvmx_110    0x8256995c __restvmx_124
 *   0x82569884 __restvmx_97     0x825698f4 __restvmx_111    0x82569964 __restvmx_125
 *   0x8256988c __restvmx_98     0x825698fc __restvmx_112    0x8256996c __restvmx_126
 *                                                           0x82569974 __restvmx_127
 *
 * This file is intentionally empty — the disposition is the comment block
 * itself.  Do not add emulation here; see recomp/.../tt-decomp_recomp.31.cpp
 * for the authoritative translation.
 */
