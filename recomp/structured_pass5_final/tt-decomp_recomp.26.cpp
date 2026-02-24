#include "tt-decomp_init.h"

__attribute__((alias("__imp__snNotifyJoin_vfn_63"))) PPC_WEAK_FUNC(snNotifyJoin_vfn_63);
PPC_FUNC_IMPL(__imp__snNotifyJoin_vfn_63) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,24
	ctx.r3.s64 = ctx.r3.s64 + 24;
	// b 0x8244c1a8
	rage_C1A8(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_06D8_2h"))) PPC_WEAK_FUNC(msgMsgSink_06D8_2h);
PPC_FUNC_IMPL(__imp__msgMsgSink_06D8_2h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// lis r8,-32256
	// addi r30,r31,12
	var_r30 = (uint32_t)(var_r31 + 12);
	// addi r11,r11,17600
	ctx.r11.s64 = ctx.r11.s64 + 17600;
	// addi r10,r10,17592
	ctx.r10.s64 = ctx.r10.s64 + 17592;
	// addi r9,r9,17588
	ctx.r9.s64 = ctx.r9.s64 + 17588;
	// addi r8,r8,17684
	ctx.r8.s64 = ctx.r8.s64 + 17684;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// stw r8,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r8.u32);
	// lwz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 4)/* msgMsgSink::flags@+0x4 */;
	// cmplw cr6,r11,r30
	// beq cr6,0x8245075c
	if (ctx.r11.u32 != var_r30) {
		// lwz r10,8(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 8);
		// cmplw cr6,r10,r30
		// beq cr6,0x8245075c
		if (ctx.r10.u32 == var_r30) goto loc_8245075C;
		// rotlwi r9,r10,0
		ctx.r9.u64 = ctx.r10.u32;
		// rotlwi r8,r11,0
		ctx.r8.u64 = ctx.r11.u32;
		// stw r9,8(r8)
		PPC_STORE_U32(ctx.r8.u32 + 8, ctx.r9.u32);
		// lwz r7,8(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 8);
		// lwz r6,4(r30)
		ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 4)/* msgMsgSink::flags@+0x4 */;
		// stw r6,4(r7)
		PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r6.u32);
		// stw r30,4(r30)
		PPC_STORE_U32(var_r30 + 4,/* msgMsgSink::flags@+0x4 */ var_r30);
		// stw r30,8(r30)
		PPC_STORE_U32(var_r30 + 8, var_r30);
	}
loc_8245075C:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244f8b8
	msgMsgSink_F8B8_w(ctx, base);
	// li r4,6
	ctx.r4.s64 = 6;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244f828
	atSingleton_F828_fw(ctx, base);
	// addi r3,r31,28
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 28;
	// lwz r5,204(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 204);
	// cmplwi cr6,r5,0
	// beq cr6,0x824507a4
while (!ctx.cr6.eq) {
	loc_82450780:
		// lwz r11,204(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 204);
		// lwz r4,0(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// lwz r10,4(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r4,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r4.u32);
		// stw r11,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
		// rotlwi r11,r10,0
		ctx.r11.u64 = ctx.r10.u32;
		// cmplwi cr6,r11,0
		ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
		// stw r10,204(r3)
		PPC_STORE_U32(ctx.r3.u32 + 204, ctx.r10.u32);
		// bne cr6,0x82450780
}
loc_824507A4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,212(r3)
	PPC_STORE_U32(ctx.r3.u32 + 212, ctx.r11.u32);
	// stw r11,208(r3)
	PPC_STORE_U32(ctx.r3.u32 + 208, ctx.r11.u32);
	// bl 0x8244ea00
	msgMsgSink_EA00(ctx, base);
	// lis r11,-32256
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_07D8_w"))) PPC_WEAK_FUNC(msgMsgSink_07D8_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_07D8_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x824506d8
	msgMsgSink_06D8_2h(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82450818
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32861
		ctx.r4.u64 = ctx.r4.u64 | 32861;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82450818:
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_0830_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_0830_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_0830_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82450518
	snNotifyGamerAdded_0518_fw(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r10,r11,17696
	ctx.r10.s64 = ctx.r11.s64 + 17696;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r9,r11,17592
	ctx.r9.s64 = ctx.r11.s64 + 17592;
	// lis r11,-32256
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// addi r8,r11,17588
	ctx.r8.s64 = ctx.r11.s64 + 17588;
	// lis r11,-32256
	// stw r9,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r9.u32);
	// addi r7,r11,17688
	ctx.r7.s64 = ctx.r11.s64 + 17688;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r8,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
	// stw r7,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r7.u32);
	// stw r11,252(r31)
	PPC_STORE_U32(var_r31 + 252, ctx.r11.u32);
	// stw r11,256(r31)
	PPC_STORE_U32(var_r31 + 256, ctx.r11.u32);
	// stw r11,260(r31)
	PPC_STORE_U32(var_r31 + 260, ctx.r11.u32);
	// stw r11,264(r31)
	PPC_STORE_U32(var_r31 + 264, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_148"))) PPC_WEAK_FUNC(msgMsgSink_vfn_148);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_148) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-12
	ctx.r3.s64 = ctx.r3.s64 + -12;
	// b 0x824511b0
	msgMsgSink_11B0_w(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4520_11"))) PPC_WEAK_FUNC(game_vt4520_11);
PPC_FUNC_IMPL(__imp__game_vt4520_11) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x8244f450
	game_F450_h(ctx, base);
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244f490
	msgMsgSink_F490_p39(ctx, base);
	// clrlwi r11,r29,24
	ctx.r11.u64 = var_r29 & 0xFF;
	// clrlwi r9,r3,16
	ctx.r9.u64 = ctx.r3.u32 & 0xFFFF;
	// addi r11,r11,2
	ctx.r11.s64 = ctx.r11.s64 + 2;
	// clrlwi r10,r28,16
	ctx.r10.u64 = var_r28 & 0xFFFF;
	// rlwinm r8,r11,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r8
	ctx.r11.u64 = ctx.r11.u64 + ctx.r8.u64;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + var_r30;
	return;
}

__attribute__((alias("__imp__game_vt4520_16"))) PPC_WEAK_FUNC(game_vt4520_16);
PPC_FUNC_IMPL(__imp__game_vt4520_16) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,260(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 260);
	// blr
	return;
}

__attribute__((alias("__imp__game_0928_h"))) PPC_WEAK_FUNC(game_0928_h);
PPC_FUNC_IMPL(__imp__game_0928_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lwz r11,26(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 26);
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__game_0988_h"))) PPC_WEAK_FUNC(game_0988_h);
PPC_FUNC_IMPL(__imp__game_0988_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lwz r11,30(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 30);
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_20"))) PPC_WEAK_FUNC(game_vt4520_20);
PPC_FUNC_IMPL(__imp__game_vt4520_20) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lwz r11,50(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 50);
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_17"))) PPC_WEAK_FUNC(game_vt4520_17);
PPC_FUNC_IMPL(__imp__game_vt4520_17) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lwz r11,54(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 54);
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_19"))) PPC_WEAK_FUNC(game_vt4520_19);
PPC_FUNC_IMPL(__imp__game_vt4520_19) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// clrlwi r11,r30,16
	ctx.r11.u64 = var_r30 & 0xFFFF;
	// lwz r10,58(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 58);
	// rlwinm r9,r11,1,0,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r7,r11,r9
	ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
	// rlwinm r11,r7,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// add r3,r11,r29
	ctx.r3.u64 = ctx.r11.u64 + var_r29;
	return;
}

__attribute__((alias("__imp__game_vt4520_13"))) PPC_WEAK_FUNC(game_vt4520_13);
PPC_FUNC_IMPL(__imp__game_vt4520_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,76(r11)
	// bctrl
	VCALL(ctx.r3.u32, 19, ctx, base);  // vtable slot 19 (byte +76)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,48(r9)
  // [ph4a] slot load collapsed
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bctrl
	VCALL(var_r31, 12, ctx, base);  // pattern-B slot 12 (byte +48)
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_15"))) PPC_WEAK_FUNC(game_vt4520_15);
PPC_FUNC_IMPL(__imp__game_vt4520_15) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,-1
	// li r3,1
	ctx.r3.s64 = 1;
	// bnelr cr6
	if (ctx.r4.s32 != -1) return;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_14"))) PPC_WEAK_FUNC(game_vt4520_14);
PPC_FUNC_IMPL(__imp__game_vt4520_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,68(r11)
	// bctrl
	VCALL(ctx.r3.u32, 17, ctx, base);  // vtable slot 17 (byte +68)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lhz r5,15(r3)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 15);
	// lis r7,-21846
	// rotlwi r4,r5,1
	ctx.r4.u64 = __builtin_rotateleft32(ctx.r5.u32, 1);
	// ori r6,r7,43691
	ctx.r6.u64 = ctx.r7.u64 | 43691;
	// subf r3,r4,r30
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 - ctx.r4.s64;
	// subf r11,r29,r3
	ctx.r11.s64 = ctx.r3.s64 - (int64_t)(int32_t)var_r29;
	// mulhwu r10,r11,r6
	ctx.r10.u64 = (uint64_t(ctx.r11.u32) * uint64_t(ctx.r6.u32)) >> 32;
	// rlwinm r9,r10,30,2,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 30) & 0x3FFFFFFF;
	// clrlwi r3,r9,16
	ctx.r3.u64 = ctx.r9.u32 & 0xFFFF;
	return;
}

__attribute__((alias("__imp__msgMsgSink_0BE0_w"))) PPC_WEAK_FUNC(msgMsgSink_0BE0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_0BE0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// lis r8,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,17696
	ctx.r11.s64 = ctx.r11.s64 + 17696;
	// addi r10,r10,17592
	ctx.r10.s64 = ctx.r10.s64 + 17592;
	// addi r9,r9,17588
	ctx.r9.s64 = ctx.r9.s64 + 17588;
	// addi r8,r8,17688
	ctx.r8.s64 = ctx.r8.s64 + 17688;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// stw r8,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r8.u32);
	// bl 0x8244f8b8
	msgMsgSink_F8B8_w(ctx, base);
	// lwz r11,252(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 252);
	// cmpwi cr6,r11,0
	// beq cr6,0x82450c58
	if (ctx.r11.s32 != 0) {
		// lwz r10,0(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r30,256(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 256));
		// lwz r9,64(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 64);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_82450C58:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x824506d8
	msgMsgSink_06D8_2h(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_0C78_w"))) PPC_WEAK_FUNC(msgMsgSink_0C78_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_0C78_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// stw r6,252(r31)
	PPC_STORE_U32(var_r31 + 252, ctx.r6.u32);
	// stw r7,256(r31)
	PPC_STORE_U32(var_r31 + 256, ctx.r7.u32);
	// stw r5,264(r31)
	PPC_STORE_U32(var_r31 + 264, ctx.r5.u32);
	// stw r4,260(r31)
	PPC_STORE_U32(var_r31 + 260, ctx.r4.u32);
	// lwz r10,64(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 64);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lbz r9,17(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 17);
	// cmplwi cr6,r9,0
	// beq cr6,0x82450d44
	if (ctx.r9.u32 != 0) {
		// lwz r8,0(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,64(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 64);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r7.u32);
		// lbz r11,17(r3)
		ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 17);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// rotlwi r3,r11,2
		ctx.r3.u64 = __builtin_rotateleft32(ctx.r11.u32, 2);
		// ori r4,r4,1
		ctx.r4.u64 = ctx.r4.u64 | 1;
		// bl 0x820c01b8
		rage_01B8(ctx, base);
		// cmplwi cr6,r3,0
		// stw r3,244(r31)
		PPC_STORE_U32(var_r31 + 244, ctx.r3.u32);
		// beq cr6,0x82450d54
		if (ctx.r3.u32 == 0) {
			// lis r3,-32761
			// ori r3,r3,14
			ctx.r3.u64 = ctx.r3.u64 | 14;
			return;
		}
		// lwz r6,0(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,64(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 64);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		// lbz r4,17(r3)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r3.u32 + 17);
		// cmplwi cr6,r4,0
		// beq cr6,0x82450d44
		if (ctx.r4.u32 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x824502c0
			msgMsgSink_02C0_w(ctx, base);
			return;
		}
		// li r29,0
		var_r29 = 0;
		// mr r11,r29
		ctx.r11.u64 = var_r29;
	loc_82450D10:
		// lwz r10,244(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 244);
		// rlwinm r9,r11,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
		// addi r30,r11,1
		var_r30 = (uint32_t)(ctx.r11.s64 + 1);  // addr:0x82000001
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stwx r29,r9,r10
		PPC_STORE_U32(ctx.r9.u32 + ctx.r10.u32, var_r29);
		// lwz r6,64(r7)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
		// lbz r5,17(r3)
		ctx.r5.u64 = PPC_LOAD_U8(ctx.r3.u32 + 17);
		// clrlwi r11,r30,16
		ctx.r11.u64 = var_r30 & 0xFFFF;
		// cmplw cr6,r11,r5
		// blt cr6,0x82450d10
		if (ctx.r11.u32 < ctx.r5.u32) goto loc_82450D10;
	}
loc_82450D44:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x824502c0
	msgMsgSink_02C0_w(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4520_6"))) PPC_WEAK_FUNC(game_vt4520_6);
PPC_FUNC_IMPL(__imp__game_vt4520_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r29,0(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 0));
	// addi r11,r29,1
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 1;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// lwz r9,64(r10)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r28,4(r31)
	var_r28 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4));
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbzx r7,r11,r29
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + var_r29);
	// stb r8,9(r31)
	PPC_STORE_U8(var_r31 + 9, ctx.r8.u8);
	// stb r7,8(r31)
	PPC_STORE_U8(var_r31 + 8, ctx.r7.u8);
	// lwz r5,64(r6)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// add r3,r3,r28
	ctx.r3.u64 = ctx.r3.u64 + var_r28;
	return;
}

__attribute__((alias("__imp__game_vt4520_7"))) PPC_WEAK_FUNC(game_vt4520_7);
PPC_FUNC_IMPL(__imp__game_vt4520_7) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lwz r11,0(r30)
  // [ph4a] vtable load collapsed
	// lwz r29,4(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4));
	// lwz r10,64(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r30, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lwzx r9,r3,r29
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + var_r29);
	// clrlwi r11,r9,27
	ctx.r11.u64 = ctx.r9.u32 & 0x1F;
	// cmplwi cr6,r11,20
	// bgt cr6,0x82451094
	if (!(ctx.r11.u32 > 20)) {
		// lis r12,-32187
		// addi r12,r12,3616
		ctx.r12.s64 = ctx.r12.s64 + 3616;
		// rlwinm r0,r11,2,0,29
		ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
		// lwzx r0,r12,r0
		ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
		// mtctr r0
		ctx.ctr.u64 = ctx.r0.u64;
		// bctr
		switch (ctx.r11.u64) {
		case 0:
		goto loc_82450E74;
		case 1:
		goto loc_82450E84;
		case 2:
		goto loc_82450EA4;
		case 3:
		goto loc_82450EC4;
		case 4:
		goto loc_82450E94;
		case 5:
		goto loc_82450EB4;
		case 6:
		goto loc_82450F08;
		case 7:
		goto loc_82451018;
		case 8:
		goto loc_82451028;
		case 9:
		goto loc_82451038;
		case 10:
		goto loc_82450FA4;
		case 11:
		goto loc_82450FB4;
		case 12:
		goto loc_82450FFC;
		case 13:
		goto loc_82451008;
		case 14:
		goto loc_82451048;
		case 15:
		goto loc_82451058;
		case 16:
		goto loc_82451068;
		case 17:
		goto loc_82451078;
		case 18:
		goto loc_82451088;
		case 19:
		goto loc_82450F4C;
		case 20:
		goto loc_82450F5C;
		default:
		__builtin_trap(); // Switch case out of range
		}
		loc_82450E74:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r8,r11,8
		ctx.r8.s64 = ctx.r11.s64 + 8;
		// stw r8,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
	} else {
		loc_82450E84:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r7,r11,11
		ctx.r7.s64 = ctx.r11.s64 + 11;
		// stw r7,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r7.u32);
		loc_82450E94:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r6,r11,18
		ctx.r6.s64 = ctx.r11.s64 + 18;
		// stw r6,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r6.u32);
		loc_82450EA4:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r5,r11,15
		ctx.r5.s64 = ctx.r11.s64 + 15;
		// stw r5,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r5.u32);
		loc_82450EB4:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r4,r11,22
		ctx.r4.s64 = ctx.r11.s64 + 22;
		// stw r4,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r4.u32);
		loc_82450EC4:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r29,r11,8
		var_r29 = (uint32_t)(ctx.r11.s64 + 8);  // addr:0x82000008
		// stw r29,4(r31)
		PPC_STORE_U32(var_r31 + 4, var_r29);
		// lwz r10,64(r11)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
		// add r9,r3,r29
		ctx.r9.u64 = ctx.r3.u64 + var_r29;
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lhz r11,2(r9)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r9.u32 + 2);
		// rotlwi r9,r11,2
		ctx.r9.u64 = __builtin_rotateleft32(ctx.r11.u32, 2);
		// add r11,r11,r9
		ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// addi r8,r11,8
		ctx.r8.s64 = ctx.r11.s64 + 8;
		// stw r8,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
		loc_82450F08:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r29,r11,15
		var_r29 = (uint32_t)(ctx.r11.s64 + 15);  // addr:0x8200000f
		// stw r29,4(r31)
		PPC_STORE_U32(var_r31 + 4, var_r29);
		// lwz r6,64(r7)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
		// add r5,r3,r29
		ctx.r5.u64 = ctx.r3.u64 + var_r29;
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lhz r11,2(r5)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r5.u32 + 2);
		// rotlwi r9,r11,2
		ctx.r9.u64 = __builtin_rotateleft32(ctx.r11.u32, 2);
		// add r11,r11,r9
		ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// addi r4,r11,8
		ctx.r4.s64 = ctx.r11.s64 + 8;
		// stw r4,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r4.u32);
		loc_82450F4C:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r3,r11,11
		ctx.r3.s64 = ctx.r11.s64 + 11;
		// stw r3,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r3.u32);
		loc_82450F5C:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r29,r11,7
		var_r29 = (uint32_t)(ctx.r11.s64 + 7);  // addr:0x82000007
		// stw r29,4(r31)
		PPC_STORE_U32(var_r31 + 4, var_r29);
		// lwz r10,64(r11)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
		// add r9,r3,r29
		ctx.r9.u64 = ctx.r3.u64 + var_r29;
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lhz r11,2(r9)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r9.u32 + 2);
		// rotlwi r9,r11,1
		ctx.r9.u64 = __builtin_rotateleft32(ctx.r11.u32, 1);
		// add r8,r11,r9
		ctx.r8.u64 = ctx.r11.u64 + ctx.r9.u64;
		// rlwinm r11,r8,1,0,30
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 1) & 0xFFFFFFFE;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// addi r7,r11,8
		ctx.r7.s64 = ctx.r11.s64 + 8;
		// stw r7,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r7.u32);
		loc_82450FA4:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r6,r11,45
		ctx.r6.s64 = ctx.r11.s64 + 45;
		// stw r6,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r6.u32);
		loc_82450FB4:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r29,r11,41
		var_r29 = (uint32_t)(ctx.r11.s64 + 41);  // addr:0x82000029
		// stw r29,4(r31)
		PPC_STORE_U32(var_r31 + 4, var_r29);
		// lwz r4,64(r5)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
		// add r3,r3,r29
		ctx.r3.u64 = ctx.r3.u64 + var_r29;
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lhz r11,2(r3)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 2);
		// rotlwi r9,r11,1
		ctx.r9.u64 = __builtin_rotateleft32(ctx.r11.u32, 1);
		// add r11,r11,r9
		ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
		// rlwinm r11,r11,1,0,30
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// addi r10,r11,8
		ctx.r10.s64 = ctx.r11.s64 + 8;
		// stw r10,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
		loc_82450FFC:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r9,r11,42
		ctx.r9.s64 = ctx.r11.s64 + 42;
		// b 0x82451090
		goto loc_82451090;
		loc_82451008:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r8,r11,18
		ctx.r8.s64 = ctx.r11.s64 + 18;
		// stw r8,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
		loc_82451018:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r7,r11,22
		ctx.r7.s64 = ctx.r11.s64 + 22;
		// stw r7,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r7.u32);
		loc_82451028:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r6,r11,22
		ctx.r6.s64 = ctx.r11.s64 + 22;
		// stw r6,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r6.u32);
		loc_82451038:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r5,r11,11
		ctx.r5.s64 = ctx.r11.s64 + 11;
		// stw r5,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r5.u32);
		loc_82451048:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r4,r11,13
		ctx.r4.s64 = ctx.r11.s64 + 13;
		// stw r4,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r4.u32);
		loc_82451058:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r3,r11,22
		ctx.r3.s64 = ctx.r11.s64 + 22;
		// stw r3,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r3.u32);
		loc_82451068:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r11,r11,26
		ctx.r11.s64 = ctx.r11.s64 + 26;
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		loc_82451078:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r10,r11,26
		ctx.r10.s64 = ctx.r11.s64 + 26;
		// stw r10,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
		loc_82451088:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// addi r9,r11,15
		ctx.r9.s64 = ctx.r11.s64 + 15;
		loc_82451090:
		// stw r9,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r9.u32);
	}
loc_82451094:
	// lbz r11,9(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 9);
	// lbz r8,8(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 8);
	// addi r7,r11,1
	ctx.r7.s64 = ctx.r11.s64 + 1;
	// clrlwi r11,r7,24
	ctx.r11.u64 = ctx.r7.u32 & 0xFF;
	// mr r6,r11
	ctx.r6.u64 = ctx.r11.u64;
	// cmplw cr6,r6,r8
	// stb r11,9(r31)
	PPC_STORE_U8(var_r31 + 9, ctx.r11.u8);
	// bne cr6,0x824510c0
	if (ctx.r6.u32 != ctx.r8.u32) goto loc_824510C0;
	// li r3,0
	ctx.r3.s64 = 0;
	return;
loc_824510C0:
	// lwz r5,0(r30)
  // [ph4a] vtable load collapsed
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r31,4(r31)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4));
	// lwz r4,64(r5)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r30, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + var_r31;
	return;
}

__attribute__((alias("__imp__game_vt4520_18"))) PPC_WEAK_FUNC(game_vt4520_18);
PPC_FUNC_IMPL(__imp__game_vt4520_18) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// lhz r9,9(r3)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r3.u32 + 9);
	// clrlwi r30,r30,16
	var_r30 = (uint32_t)(var_r30 & 0xFFFF);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplw cr6,r30,r9
	// bge cr6,0x82451138
	if (var_r30 < ctx.r9.u32) {
		// bl 0x82450928
		game_0928_h(ctx, base);
		// rlwinm r11,r30,2,0,29
		ctx.r11.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 2) & 0xFFFFFFFC;
		// add r11,r30,r11
		ctx.r11.u64 = var_r30 + ctx.r11.u64;
		// add r3,r11,r3
		ctx.r3.u64 = ctx.r11.u64 + ctx.r3.u64;
		return;
	}
loc_82451138:
	// bl 0x82450988
	game_0988_h(ctx, base);
	// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r7,64(r8)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// lhz r6,9(r3)
	ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 9);
	// subf r11,r6,r30
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 - ctx.r6.s64;
	// rlwinm r5,r11,4,0,27
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// subf r11,r11,r5
	ctx.r11.s64 = ctx.r5.s64 - ctx.r11.s64;
	// add r3,r11,r29
	ctx.r3.u64 = ctx.r11.u64 + var_r29;
	return;
}

__attribute__((alias("__imp__game_vt4520_8"))) PPC_WEAK_FUNC(game_vt4520_8);
PPC_FUNC_IMPL(__imp__game_vt4520_8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lwz r10,64(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // pattern-B slot 16 (byte +64)
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_11B0_w"))) PPC_WEAK_FUNC(msgMsgSink_11B0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_11B0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x82450be0
	msgMsgSink_0BE0_w(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824511f0
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32793
		ctx.r4.u64 = ctx.r4.u64 | 32793;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824511F0:
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_1208_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_1208_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_1208_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82457d50
	snNotifyGamerAdded_7D50_fw(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r10,r11,17784
	ctx.r10.s64 = ctx.r11.s64 + 17784;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r9,r11,20456
	ctx.r9.s64 = ctx.r11.s64 + 20456;
	// lis r11,-32256
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// addi r8,r11,17780
	ctx.r8.s64 = ctx.r11.s64 + 17780;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r9,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r9.u32);
	// stw r8,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
	// stw r11,256(r31)
	PPC_STORE_U32(var_r31 + 256, ctx.r11.u32);
	// stw r11,260(r31)
	PPC_STORE_U32(var_r31 + 260, ctx.r11.u32);
	// stw r11,264(r31)
	PPC_STORE_U32(var_r31 + 264, ctx.r11.u32);
	// stw r11,268(r31)
	PPC_STORE_U32(var_r31 + 268, ctx.r11.u32);
	// stw r11,272(r31)
	PPC_STORE_U32(var_r31 + 272, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_21"))) PPC_WEAK_FUNC(game_vt4520_21);
PPC_FUNC_IMPL(__imp__game_vt4520_21) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-8
	ctx.r3.s64 = ctx.r3.s64 + -8;
	// b 0x82451450
	msgMsgSink_1450(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4520_26"))) PPC_WEAK_FUNC(game_vt4520_26);
PPC_FUNC_IMPL(__imp__game_vt4520_26) {
	PPC_FUNC_PROLOGUE();
	// clrlwi r11,r4,16
	ctx.r11.u64 = ctx.r4.u32 & 0xFFFF;
	// lwz r10,268(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 268);
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// rlwinm r11,r11,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_33"))) PPC_WEAK_FUNC(game_vt4520_33);
PPC_FUNC_IMPL(__imp__game_vt4520_33) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,280(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 280);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_32"))) PPC_WEAK_FUNC(game_vt4520_32);
PPC_FUNC_IMPL(__imp__game_vt4520_32) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,280(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 280);
	// addi r3,r11,88
	ctx.r3.s64 = ctx.r11.s64 + 88;
	// blr
	return;
}

__attribute__((alias("__imp__ph_12C0_h"))) PPC_WEAK_FUNC(ph_12C0_h);
PPC_FUNC_IMPL(__imp__ph_12C0_h) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// stw r6,256(r3)
	PPC_STORE_U32(ctx.r3.u32 + 256, ctx.r6.u32);
	// stw r7,260(r3)
	PPC_STORE_U32(ctx.r3.u32 + 260, ctx.r7.u32);
	// stw r5,264(r3)
	PPC_STORE_U32(ctx.r3.u32 + 264, ctx.r5.u32);
	// stw r4,276(r3)
	PPC_STORE_U32(ctx.r3.u32 + 276, ctx.r4.u32);
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// add r11,r11,r4
	ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
	// stw r11,280(r3)
	PPC_STORE_U32(ctx.r3.u32 + 280, ctx.r11.u32);
	// lwz r11,40(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 40);
	// add r10,r11,r4
	ctx.r10.u64 = ctx.r11.u64 + ctx.r4.u64;
	// stw r10,272(r3)
	PPC_STORE_U32(ctx.r3.u32 + 272, ctx.r10.u32);
	// lwz r11,16(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// add r9,r11,r4
	ctx.r9.u64 = ctx.r11.u64 + ctx.r4.u64;
	// stw r9,268(r3)
	PPC_STORE_U32(ctx.r3.u32 + 268, ctx.r9.u32);
	// bl 0x82456080
	msgMsgSink_6080_p33(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_36"))) PPC_WEAK_FUNC(game_vt4520_36);
PPC_FUNC_IMPL(__imp__game_vt4520_36) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// lwz r11,272(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 272);
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// add r9,r11,r10
	ctx.r9.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stw r9,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r9.u32);
	// lwz r8,12(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// stw r8,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
	// lwz r7,12(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// stw r7,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r7.u32);
	// lwz r6,4(r3)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// clrlwi r5,r6,31
	ctx.r5.u64 = ctx.r6.u32 & 0x1;
	// cmplwi cr6,r5,1
	// bne cr6,0x82451380
	if (ctx.r5.u32 == 1) {
		// addi r4,r31,12
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
		// bl 0x82455c70
		game_5C70_h(ctx, base);
		// b 0x824513b4
	} else {
	loc_82451380:
		// lwz r4,16(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// stw r4,12(r31)
		PPC_STORE_U32(var_r31 + 12, ctx.r4.u32);
		// lwz r11,16(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// cmplwi cr6,r11,0
		// bne cr6,0x824513ac
		if (ctx.r11.u32 == 0) {
			// lwz r10,20(r3)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
			// cmplwi cr6,r10,0
			// bne cr6,0x824513ac
			if (ctx.r10.u32 != 0) goto loc_824513AC;
			// lwz r9,12(r3)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
			// stw r9,16(r31)
			PPC_STORE_U32(var_r31 + 16, ctx.r9.u32);
			// b 0x824513b4
		} else {
		loc_824513AC:
			// lwz r8,20(r3)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
			// stw r8,16(r31)
			PPC_STORE_U32(var_r31 + 16, ctx.r8.u32);
		}
	}
loc_824513B4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_79"))) PPC_WEAK_FUNC(msgMsgSink_vfn_79);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_79) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,224(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 224);
	// cmplwi cr6,r11,0
	// beqlr cr6
	if (ctx.r11.u32 == 0) return;
loc_824513DC:
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplw cr6,r10,r4
	// beq cr6,0x824513f8
	if (ctx.r10.u32 == ctx.r4.u32) {
		// cmplwi cr6,r11,0
		// beqlr cr6
		if (ctx.r11.u32 == 0) return;
		// mr r4,r11
		ctx.r4.u64 = ctx.r11.u64;
		// addi r3,r3,20
		ctx.r3.s64 = ctx.r3.s64 + 20;
		// b 0x8244ef40
		ph_EF40(ctx, base);
		return;
	}
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplwi cr6,r11,0
	// bne cr6,0x824513dc
	if (ctx.r11.u32 != 0) goto loc_824513DC;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_27"))) PPC_WEAK_FUNC(game_vt4520_27);
PPC_FUNC_IMPL(__imp__game_vt4520_27) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,280(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 280);
	// addi r3,r11,8
	ctx.r3.s64 = ctx.r11.s64 + 8;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_29"))) PPC_WEAK_FUNC(game_vt4520_29);
PPC_FUNC_IMPL(__imp__game_vt4520_29) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,276(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 276);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_30"))) PPC_WEAK_FUNC(game_vt4520_30);
PPC_FUNC_IMPL(__imp__game_vt4520_30) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,276(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 276);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_28"))) PPC_WEAK_FUNC(game_vt4520_28);
PPC_FUNC_IMPL(__imp__game_vt4520_28) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,280(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 280);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1450"))) PPC_WEAK_FUNC(msgMsgSink_1450);
PPC_FUNC_IMPL(__imp__msgMsgSink_1450) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// addi r11,r11,17784
	ctx.r11.s64 = ctx.r11.s64 + 17784;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// lwz r8,256(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 256);
	// addi r9,r9,17780
	ctx.r9.s64 = ctx.r9.s64 + 17780;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// cmpwi cr6,r8,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// beq cr6,0x824514a8
	if (ctx.r8.s32 != 0) {
		// lwz r4,260(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 260);
		// lwz r3,276(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 276);
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_824514A8:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82457da8
	msgMsgSink_7DA8_p33(ctx, base);
	// clrlwi r7,r30,31
	ctx.r7.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r7,0
	// beq cr6,0x824514d0
	if (ctx.r7.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32796
		ctx.r4.u64 = ctx.r4.u64 | 32796;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824514D0:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_78"))) PPC_WEAK_FUNC(msgMsgSink_vfn_78);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_78) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,20
	ctx.r3.s64 = ctx.r3.s64 + 20;
	// b 0x8244c1a8
	rage_C1A8(ctx, base);
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_14F0_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_14F0_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_14F0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824585a0
	snNotifyGamerAdded_85A0_fw(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r10,r11,17848
	ctx.r10.s64 = ctx.r11.s64 + 17848;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r9,r11,20456
	ctx.r9.s64 = ctx.r11.s64 + 20456;
	// lis r11,-32256
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// li r10,2048
	ctx.r10.s64 = 2048;
	// addi r8,r11,17844
	ctx.r8.s64 = ctx.r11.s64 + 17844;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r9,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r9.u32);
	// stw r10,316(r31)
	PPC_STORE_U32(var_r31 + 316, ctx.r10.u32);
	// stw r8,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
	// stw r11,304(r31)
	PPC_STORE_U32(var_r31 + 304, ctx.r11.u32);
	// stw r11,308(r31)
	PPC_STORE_U32(var_r31 + 308, ctx.r11.u32);
	// stw r11,312(r31)
	PPC_STORE_U32(var_r31 + 312, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_37"))) PPC_WEAK_FUNC(game_vt4520_37);
PPC_FUNC_IMPL(__imp__game_vt4520_37) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-8
	ctx.r3.s64 = ctx.r3.s64 + -8;
	// b 0x82451930
	msgMsgSink_1930(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4520_43"))) PPC_WEAK_FUNC(game_vt4520_43);
PPC_FUNC_IMPL(__imp__game_vt4520_43) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,308(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 308);
	// addi r3,r11,8
	ctx.r3.s64 = ctx.r11.s64 + 8;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_45"))) PPC_WEAK_FUNC(game_vt4520_45);
PPC_FUNC_IMPL(__imp__game_vt4520_45) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,304(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 304);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_46"))) PPC_WEAK_FUNC(game_vt4520_46);
PPC_FUNC_IMPL(__imp__game_vt4520_46) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,304(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 304);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_44"))) PPC_WEAK_FUNC(game_vt4520_44);
PPC_FUNC_IMPL(__imp__game_vt4520_44) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,308(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 308);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_48"))) PPC_WEAK_FUNC(game_vt4520_48);
PPC_FUNC_IMPL(__imp__game_vt4520_48) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,308(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 308);
	// addi r3,r11,88
	ctx.r3.s64 = ctx.r11.s64 + 88;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_54"))) PPC_WEAK_FUNC(game_vt4520_54);
PPC_FUNC_IMPL(__imp__game_vt4520_54) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,308(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 308);
	// lwz r3,80(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_49"))) PPC_WEAK_FUNC(game_vt4520_49);
PPC_FUNC_IMPL(__imp__game_vt4520_49) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,308(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 308);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// blr
	return;
}

__attribute__((alias("__imp__rage_15D8"))) PPC_WEAK_FUNC(rage_15D8);
PPC_FUNC_IMPL(__imp__rage_15D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// ori r4,r4,1
	ctx.r4.u64 = ctx.r4.u64 | 1;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r10,256(r31)
	PPC_STORE_U32(var_r31 + 256, ctx.r10.u32);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r9,260(r31)
	PPC_STORE_U32(var_r31 + 260, ctx.r9.u32);
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r8,264(r31)
	PPC_STORE_U32(var_r31 + 264, ctx.r8.u32);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r7,268(r31)
	PPC_STORE_U32(var_r31 + 268, ctx.r7.u32);
	// lwz r3,316(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 316);
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x82451654
	if (ctx.r4.u32 != 0) {
		// addi r6,r31,272
		ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 272;
		// lwz r5,316(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 316);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r4,304(r31)
		PPC_STORE_U32(var_r31 + 304, ctx.r4.u32);
		// bl 0x824581d8
		game_81D8(ctx, base);
		// blr
		return;
	}
loc_82451654:
	// lis r3,-32761
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_57"))) PPC_WEAK_FUNC(game_vt4520_57);
PPC_FUNC_IMPL(__imp__game_vt4520_57) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,304(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 304);
	// lwz r8,316(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 316);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// add r7,r11,r10
	ctx.r7.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stw r7,308(r31)
	PPC_STORE_U32(var_r31 + 308, ctx.r7.u32);
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// add r4,r10,r9
	ctx.r4.u64 = ctx.r10.u64 + ctx.r9.u64;
	// cmplw cr6,r4,r8
	// ble cr6,0x82451720
	if (ctx.r4.u32 > ctx.r8.u32) {
		// bl 0x82457ef8
		msgMsgSink_7EF8_g(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// lwz r3,304(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 304);
		// ori r4,r4,1
		ctx.r4.u64 = ctx.r4.u64 | 1;
		// stw r11,316(r31)
		PPC_STORE_U32(var_r31 + 316, ctx.r11.u32);
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// lwz r3,316(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 316);
		// ori r4,r4,1
		ctx.r4.u64 = ctx.r4.u64 | 1;
		// bl 0x820c01b8
		rage_01B8(ctx, base);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// li r11,0
		ctx.r11.s64 = 0;
		// cmplwi cr6,r4,0
		// stw r4,304(r31)
		PPC_STORE_U32(var_r31 + 304, ctx.r4.u32);
		// stw r11,308(r31)
		PPC_STORE_U32(var_r31 + 308, ctx.r11.u32);
		// stw r11,312(r31)
		PPC_STORE_U32(var_r31 + 312, ctx.r11.u32);
		// beq cr6,0x8245173c
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// addi r6,r31,272
		ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 272;
		// stw r11,280(r31)
		PPC_STORE_U32(var_r31 + 280, ctx.r11.u32);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,316(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 316);
		// stw r11,284(r31)
		PPC_STORE_U32(var_r31 + 284, ctx.r11.u32);
		// bl 0x824581d8
		game_81D8(ctx, base);
		// blr
		return;
	}
loc_82451720:
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// add r6,r10,r11
	ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r6,308(r31)
	PPC_STORE_U32(var_r31 + 308, ctx.r6.u32);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// add r5,r10,r11
	ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r5,312(r31)
	PPC_STORE_U32(var_r31 + 312, ctx.r5.u32);
	// bl 0x82456080
	msgMsgSink_6080_p33(ctx, base);
loc_8245173C:
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_52"))) PPC_WEAK_FUNC(game_vt4520_52);
PPC_FUNC_IMPL(__imp__game_vt4520_52) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// lwz r9,304(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 304);
	// addi r6,r31,88
	ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 88;
	// lwz r5,112(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 112);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r4,120(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 120);
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r8,40(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 40);
	// lwz r9,8(r6)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r6.u32 + 8);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// add r11,r9,r8
	ctx.r11.u64 = ctx.r9.u64 + ctx.r8.u64;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stw r11,96(r31)
	PPC_STORE_U32(var_r31 + 96, ctx.r11.u32);
	// stw r7,100(r31)
	PPC_STORE_U32(var_r31 + 100, ctx.r7.u32);
	// bl 0x824581d8
	game_81D8(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_53"))) PPC_WEAK_FUNC(game_vt4520_53);
PPC_FUNC_IMPL(__imp__game_vt4520_53) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// mr r5,r6
	ctx.r5.u64 = ctx.r6.u64;
	// li r6,0
	ctx.r6.s64 = 0;
	// lwz r3,256(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 256);
	// lwz r11,20(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r11,188(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 188);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__game_vt4520_58"))) PPC_WEAK_FUNC(game_vt4520_58);
PPC_FUNC_IMPL(__imp__game_vt4520_58) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// li r31,0
	var_r31 = 0;
	// lwz r11,304(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 304);
	// cmplwi cr6,r11,0
	// stw r31,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
	// beq cr6,0x82451844
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// addi r5,r3,272
		ctx.r5.s64 = ctx.r3.s64 + 272;
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r9,60(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 15, ctx, base);  // pattern-B slot 15 (byte +60)
		// cmpwi cr6,r3,0
		// li r3,1
		ctx.r3.s64 = 1;
		// beq cr6,0x82451848
		if (ctx.r3.s32 == 0) {
			// blr
			return;
		}
	}
loc_82451844:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
loc_82451848:
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_42"))) PPC_WEAK_FUNC(game_vt4520_42);
PPC_FUNC_IMPL(__imp__game_vt4520_42) {
	PPC_FUNC_PROLOGUE();
	// clrlwi r11,r4,16
	ctx.r11.u64 = ctx.r4.u32 & 0xFFFF;
	// lwz r10,312(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 312);
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// rlwinm r11,r11,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1880"))) PPC_WEAK_FUNC(msgMsgSink_1880);
PPC_FUNC_IMPL(__imp__msgMsgSink_1880) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,17848
	ctx.r11.s64 = ctx.r11.s64 + 17848;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,17844
	ctx.r9.s64 = ctx.r9.s64 + 17844;
	// li r30,0
	var_r30 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
loc_824518C0:
	// lwz r11,304(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 304);
	// stw r30,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
	// cmplwi cr6,r11,0
	// beq cr6,0x824518f8
	if (ctx.r11.u32 == 0) goto loc_824518F8;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// addi r5,r31,272
	ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 272;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r9,60(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 60);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
	// cmpwi cr6,r3,0
	// beq cr6,0x824518c0
	if (ctx.r3.s32 == 0) goto loc_824518C0;
loc_824518F8:
	// lwz r3,304(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 304);
	// cmplwi cr6,r3,0
	// beq cr6,0x82451910
	if (ctx.r3.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,1
		ctx.r4.u64 = ctx.r4.u64 | 1;
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_82451910:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82457fb8
	msgMsgSink_7FB8_p33(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1930"))) PPC_WEAK_FUNC(msgMsgSink_1930);
PPC_FUNC_IMPL(__imp__msgMsgSink_1930) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x82451880
	msgMsgSink_1880(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82451970
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32797
		ctx.r4.u64 = ctx.r4.u64 | 32797;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82451970:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1988_p39"))) PPC_WEAK_FUNC(msgMsgSink_1988_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_1988_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lbz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 0);
	// addi r11,r10,-1
	ctx.r11.s64 = ctx.r10.s64 + -1;
	// cmplwi cr6,r11,13
	// bgt cr6,0x82451a40
	if (ctx.r11.u32 > 13) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
	// lis r12,-32187
	// addi r12,r12,6596
	ctx.r12.s64 = ctx.r12.s64 + 6596;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (ctx.r11.u64) {
	case 0:
		goto loc_824519FC;
	case 1:
		goto loc_824519FC;
	case 2:
		goto loc_824519FC;
	case 3:
		goto loc_824519FC;
	case 4:
		goto loc_824519FC;
	case 5:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	case 6:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	case 7:
		goto loc_824519FC;
	case 8:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	case 9:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	case 10:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	case 11:
		goto loc_82451A90;
	case 12:
		goto loc_82451A90;
	case 13:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_824519FC:
	// lwz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 6);
	// cmplwi cr6,r11,0
	// bne cr6,0x82451a40
	if (ctx.r11.u32 != 0) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
	// lwz r3,2(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 2);
	// cmplwi cr6,r3,0
	// beq cr6,0x82451a58
	if (ctx.r3.u32 == 0) goto loc_82451A58;
	// lwz r10,10(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 10);
	// cmplwi cr6,r10,0
	// bne cr6,0x82451a40
	if (ctx.r10.u32 != 0) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
	// lhz r9,14(r31)
	ctx.r9.u64 = PPC_LOAD_U16(var_r31 + 14);
	// cmplwi cr6,r9,65535
	// beq cr6,0x82451af0
	if (ctx.r9.u32 == 65535) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// bl 0x8244f7c8
	util_F7C8(ctx, base);
	// lhz r8,14(r31)
	ctx.r8.u64 = PPC_LOAD_U16(var_r31 + 14);
	// clrlwi r7,r3,16
	ctx.r7.u64 = ctx.r3.u32 & 0xFFFF;
	// cmplw cr6,r8,r7
	// blt cr6,0x82451af0
	if (ctx.r8.u32 < ctx.r7.u32) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
loc_82451A40:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
loc_82451A58:
	// lwz r6,10(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 10);
	// cmplwi cr6,r6,0
	// beq cr6,0x82451af0
	if (ctx.r6.u32 == 0) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// cmplwi cr6,r10,1
	// bne cr6,0x82451af0
	if (ctx.r10.u32 != 1) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// rotlwi r3,r6,0
	ctx.r3.u64 = ctx.r6.u32;
	// lwz r4,64(r5)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// clrlwi r3,r3,30
	ctx.r3.u64 = ctx.r3.u32 & 0x3;
	// cmplwi cr6,r3,0
	// bne cr6,0x82451af0
	if (ctx.r3.u32 != 0) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// b 0x82451a40
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
loc_82451A90:
	// lwz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 6);
	// cmplwi cr6,r11,0
	// beq cr6,0x82451ab8
	if (ctx.r11.u32 == 0) goto loc_82451AB8;
	// lwz r10,2(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 2);
	// cmplwi cr6,r10,0
	// bne cr6,0x82451a40
	if (ctx.r10.u32 != 0) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
	// lwz r9,10(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 10);
	// cmplwi cr6,r9,0
	// beq cr6,0x82451af0
	if (ctx.r9.u32 == 0) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// b 0x82451a40
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
loc_82451AB8:
	// lwz r3,2(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 2);
	// cmplwi cr6,r3,0
	// beq cr6,0x82451af0
	if (ctx.r3.u32 == 0) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// lwz r8,10(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 10);
	// cmplwi cr6,r8,0
	// bne cr6,0x82451a40
	if (ctx.r8.u32 != 0) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
	// lhz r7,14(r31)
	ctx.r7.u64 = PPC_LOAD_U16(var_r31 + 14);
	// cmplwi cr6,r7,65535
	// beq cr6,0x82451af0
	if (ctx.r7.u32 == 65535) {
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// bl 0x8244f7c8
	util_F7C8(ctx, base);
	// lhz r6,14(r31)
	ctx.r6.u64 = PPC_LOAD_U16(var_r31 + 14);
	// clrlwi r5,r3,16
	ctx.r5.u64 = ctx.r3.u32 & 0xFFFF;
	// cmplw cr6,r6,r5
	// bge cr6,0x82451a40
	if (ctx.r6.u32 >= ctx.r5.u32) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
loc_82451AF0:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4520_59"))) PPC_WEAK_FUNC(game_vt4520_59);
PPC_FUNC_IMPL(__imp__game_vt4520_59) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x82451b44
	if (ctx.r10.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32799
		ctx.r4.u64 = ctx.r4.u64 | 32799;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82451B44:
	// blr
	return;
}

__attribute__((alias("__imp__ph_1B58_h"))) PPC_WEAK_FUNC(ph_1B58_h);
PPC_FUNC_IMPL(__imp__ph_1B58_h) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32256
	// stw r3,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r3.u32);
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// stw r3,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r3.u32);
	// addi r9,r9,17932
	ctx.r9.s64 = ctx.r9.s64 + 17932;
	// addi r10,r3,16
	ctx.r10.s64 = ctx.r3.s64 + 16;
	// li r8,20
	ctx.r8.s64 = 20;
	// stw r9,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r9.u32);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
loc_82451B7C:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stb r9,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r9.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x82451b7c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82451B7C;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1B98_g"))) PPC_WEAK_FUNC(msgMsgSink_1B98_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_1B98_g) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// li r9,40
	ctx.r9.s64 = 40;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_82451BA8:
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bdnz 0x82451ba8
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82451BA8;
	// blr
	return;
}

__attribute__((alias("__imp__game_1BB8_h"))) PPC_WEAK_FUNC(game_1BB8_h);
PPC_FUNC_IMPL(__imp__game_1BB8_h) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r10,40
	ctx.r10.s64 = 40;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82451BC4:
	// lbz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r4.u32 + 0);
	// addi r4,r4,1
	ctx.r4.s64 = ctx.r4.s64 + 1;
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bdnz 0x82451bc4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82451BC4;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1BE0_g"))) PPC_WEAK_FUNC(msgMsgSink_1BE0_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_1BE0_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
	// li r3,2
	ctx.r3.s64 = 2;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451c00
	if (ctx.r11.u32 >= 1) {
		// bnelr cr6
		if (!ctx.cr6.eq) return;
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
loc_82451C00:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1C08_g"))) PPC_WEAK_FUNC(msgMsgSink_1C08_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_1C08_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
	// li r10,2
	ctx.r10.s64 = 2;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451c28
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451c2c
		if (!ctx.cr6.eq) {
			// cmplwi cr6,r10,1
			// li r3,0
			ctx.r3.s64 = 0;
			// bgelr cr6
			if (ctx.r10.u32 >= 1) return;
			// li r3,1
			ctx.r3.s64 = 1;
			// blr
			return;
		}
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x82451c2c
	} else {
	loc_82451C28:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_82451C2C:
	// cmplwi cr6,r10,1
	// li r3,0
	ctx.r3.s64 = 0;
	// bgelr cr6
	if (ctx.r10.u32 >= 1) return;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1C40_wrh"))) PPC_WEAK_FUNC(msgMsgSink_1C40_wrh);
PPC_FUNC_IMPL(__imp__msgMsgSink_1C40_wrh) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32761
	// li r9,2
	ctx.r9.s64 = 2;
	// ori r3,r10,87
	ctx.r3.u64 = ctx.r10.u64 | 87;
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// clrlwi r10,r8,31
	ctx.r10.u64 = ctx.r8.u32 & 0x1;
	// cmplwi cr6,r10,1
	// blt cr6,0x82451c6c
	if (ctx.r10.u32 >= 1) {
		// bne cr6,0x82451c70
		if (!ctx.cr6.eq) goto loc_82451C70;
		// li r9,1
		ctx.r9.s64 = 1;
		// b 0x82451c70
	} else {
	loc_82451C6C:
		// li r9,0
		ctx.r9.s64 = 0;
	}
loc_82451C70:
	// cmplwi cr6,r9,1
	// blt cr6,0x82451ce4
	if (ctx.r9.u32 >= 1) {
		// bnelr cr6
		if (!ctx.cr6.eq) return;
		// rlwinm r9,r8,31,29,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 31) & 0x7;
		// cmplwi cr6,r9,6
		// bgtlr cr6
		if (ctx.r9.u32 > 6) return;
		// li r3,0
		ctx.r3.s64 = 0;
		// cmplwi cr6,r9,0
		// beqlr cr6
		if (ctx.r9.u32 == 0) return;
		// addi r11,r11,20
		ctx.r11.s64 = ctx.r11.s64 + 20;
		// addi r10,r4,16
		ctx.r10.s64 = ctx.r4.s64 + 16;
	loc_82451C9C:
		// lwz r8,-4(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + -4);
		// addi r9,r9,-1
		ctx.r9.s64 = ctx.r9.s64 + -1;
		// cmplwi cr6,r9,0
		ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
		// stw r8,-4(r10)
		PPC_STORE_U32(ctx.r10.u32 + -4, ctx.r8.u32);
		// lwz r7,0(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// rlwinm r6,r7,28,4,31
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
		// stw r6,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r6.u32);
		// lwz r5,0(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// rlwinm r4,r5,30,30,31
		ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 30) & 0x3;
		// stb r4,4(r10)
		PPC_STORE_U8(ctx.r10.u32 + 4, ctx.r4.u8);
		// lwz r8,0(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,8
		ctx.r11.s64 = ctx.r11.s64 + 8;
		// clrlwi r8,r8,30
		ctx.r8.u64 = ctx.r8.u32 & 0x3;
		// addi r7,r8,1
		ctx.r7.s64 = ctx.r8.s64 + 1;
		// stb r7,5(r10)
		PPC_STORE_U8(ctx.r10.u32 + 5, ctx.r7.u8);
		// addi r10,r10,12
		ctx.r10.s64 = ctx.r10.s64 + 12;
		// bne cr6,0x82451c9c
		if (!ctx.cr6.eq) goto loc_82451C9C;
		// blr
		return;
	}
loc_82451CE4:
	// lwz r5,20(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplwi cr6,r5,0
	// bne cr6,0x82451d04
	if (ctx.r5.u32 == 0) {
		// li r3,0
		ctx.r3.s64 = 0;
		// stw r3,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r3.u32);
		// lwz r11,12(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// stw r11,16(r4)
		PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r11.u32);
		// blr
		return;
	}
loc_82451D04:
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r10,12(r4)
	PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r10.u32);
	// lwz r9,20(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// stw r9,16(r4)
	PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1D20_g"))) PPC_WEAK_FUNC(msgMsgSink_1D20_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_1D20_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
	// rlwinm r3,r11,31,29,31
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1D30_g"))) PPC_WEAK_FUNC(msgMsgSink_1D30_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_1D30_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
	// li r10,2
	ctx.r10.s64 = 2;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451d50
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451d54
		if (!ctx.cr6.eq) goto loc_82451D54;
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x82451d54
	} else {
	loc_82451D50:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_82451D54:
	// cmplwi cr6,r10,1
	// blt cr6,0x82451d74
	if (ctx.r10.u32 >= 1) {
		// beq cr6,0x82451d68
		if (!(ctx.cr6.eq)) {
			// li r3,0
			ctx.r3.s64 = 0;
			// blr
			return;
		}
	loc_82451D68:
		// lwz r10,16(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// rlwinm r3,r10,29,3,20
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFF800;
		// blr
		return;
	}
loc_82451D74:
	// lwz r3,16(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1D80_g"))) PPC_WEAK_FUNC(msgMsgSink_1D80_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_1D80_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
	// li r10,2
	ctx.r10.s64 = 2;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451da0
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451da4
		if (!ctx.cr6.eq) goto loc_82451DA4;
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x82451da4
	} else {
	loc_82451DA0:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_82451DA4:
	// cmplwi cr6,r10,1
	// blt cr6,0x82451dc8
	if (ctx.r10.u32 >= 1) {
		// bne cr6,0x82451dd4
		if (!ctx.cr6.eq) {
			// lwz r3,12(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
			// blr
			return;
		}
		// lwz r10,16(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// rlwinm r11,r10,29,3,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF;
		// addi r9,r11,-1
		ctx.r9.s64 = ctx.r11.s64 + -1;
		// rlwinm r11,r9,0,0,20
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFF800;
		// addi r11,r11,2048
		ctx.r11.s64 = ctx.r11.s64 + 2048;
		// b 0x82451dcc
	} else {
	loc_82451DC8:
		// lwz r11,20(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	}
loc_82451DCC:
	// cmplwi cr6,r11,0
	// bne cr6,0x82451ddc
	if (ctx.r11.u32 == 0) {
	loc_82451DD4:
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// blr
		return;
	}
loc_82451DDC:
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// blr
	return;
}

__attribute__((alias("__imp__game_1DE8_h"))) PPC_WEAK_FUNC(game_1DE8_h);
PPC_FUNC_IMPL(__imp__game_1DE8_h) {
	PPC_FUNC_PROLOGUE();
	// clrldi r11,r4,32
	ctx.r11.u64 = ctx.r4.u64 & 0xFFFFFFFF;
	// lwz r10,4(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// mulli r9,r11,1000
	ctx.r9.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(1000));
	// rotldi r11,r9,1
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u64, 1);
	// rldicl r8,r10,60,37
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u64, 60) & 0x7FFFFFF;
	// addi r7,r11,-1
	ctx.r7.s64 = ctx.r11.s64 + -1;
	// divd r6,r9,r8
	ctx.r6.s64 = ctx.r8.s64 ? ctx.r9.s64 / ctx.r8.s64 : 0;
	// andc r5,r8,r7
	ctx.r5.u64 = ctx.r8.u64 & ~ctx.r7.u64;
	// tdllei r8,0
	if (ctx.r8.s64 == 0ll || ctx.r8.u64 < 0ull) __builtin_trap();
	// rotlwi r3,r6,0
	ctx.r3.u64 = ctx.r6.u32;
	// tdlgei r5,-1
	if (ctx.r5.s64 == -1ll || ctx.r5.u64 > 18446744073709551615ull) __builtin_trap();
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_1E18_2h"))) PPC_WEAK_FUNC(atSingleton_1E18_2h);
PPC_FUNC_IMPL(__imp__atSingleton_1E18_2h) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
	// lis r9,6815
	ctx.r9.s64 = 446627840;
	// lis r7,1572
	ctx.r7.s64 = 103022592;
	// rlwinm r11,r11,28,5,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 28) & 0x7FFFFFF;
	// clrldi r10,r4,32
	ctx.r10.u64 = ctx.r4.u64 & 0xFFFFFFFF;
	// clrldi r5,r11,32
	ctx.r5.u64 = ctx.r11.u64 & 0xFFFFFFFF;
	// ori r8,r9,48759
	ctx.r8.u64 = ctx.r9.u64 | 48759;
	// ori r6,r7,56623
	ctx.r6.u64 = ctx.r7.u64 | 56623;
	// mulld r11,r5,r10
	ctx.r11.s64 = static_cast<int64_t>(ctx.r5.u64 * ctx.r10.u64);
	// rldimi r8,r6,32,0
	ctx.r8.u64 = (__builtin_rotateleft64(ctx.r6.u64, 32) & 0xFFFFFFFF00000000) | (ctx.r8.u64 & 0xFFFFFFFF);
	// mulhdu r10,r11,r8
	ctx.r10.u64 = static_cast<uint64_t>((static_cast<__uint128_t>(ctx.r11.u64) * static_cast<__uint128_t>(ctx.r8.u64)) >> 64);
	// subf r4,r10,r11
	ctx.r4.s64 = ctx.r11.s64 - ctx.r10.s64;
	// rldicl r11,r4,63,1
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u64, 63) & 0x7FFFFFFFFFFFFFFF;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rldicl r11,r3,55,9
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u64, 55) & 0x7FFFFFFFFFFFFF;
	// rotlwi r3,r11,0
	ctx.r3.u64 = ctx.r11.u32;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_1E60_sp"))) PPC_WEAK_FUNC(atSingleton_1E60_sp);
PPC_FUNC_IMPL(__imp__atSingleton_1E60_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r9,4(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
	// li r10,2
	ctx.r10.s64 = 2;
	// clrlwi r11,r9,31
	ctx.r11.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451e80
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451e84
		if (!ctx.cr6.eq) goto loc_82451E84;
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x82451e84
	} else {
	loc_82451E80:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_82451E84:
	// cmplwi cr6,r10,1
	// blt cr6,0x82451ea4
	if (ctx.r10.u32 >= 1) {
		// beq cr6,0x82451e98
		if (!(ctx.cr6.eq)) {
			// li r11,0
			ctx.r11.s64 = 0;
			// b 0x82451ea8
			goto loc_82451EA8;
		}
	loc_82451E98:
		// lwz r11,16(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// rlwinm r11,r11,29,3,20
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFF800;
		// b 0x82451ea8
	} else {
	loc_82451EA4:
		// lwz r11,16(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	}
loc_82451EA8:
	// rlwinm r10,r9,0,0,0
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x80000000;
	// cmplwi cr6,r10,0
	// li r10,8
	ctx.r10.s64 = 8;
	// beq cr6,0x82451ebc
	if (ctx.r10.u32 != 0) {
		// li r10,16
		ctx.r10.s64 = 16;
	}
loc_82451EBC:
	// rlwinm r9,r9,31,29,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7;
	// rlwinm r8,r10,29,3,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF;
	// mullw r7,r9,r8
	ctx.r7.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r8.s32);
	// divwu r3,r11,r7
	ctx.r3.u32 = ctx.r7.u32 ? ctx.r11.u32 / ctx.r7.u32 : 0;
	// twllei r7,0
	if (ctx.r7.s32 == 0 || ctx.r7.u32 < 0u) __builtin_trap();
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_1ED8_2h"))) PPC_WEAK_FUNC(msgMsgSink_1ED8_2h);
PPC_FUNC_IMPL(__imp__msgMsgSink_1ED8_2h) {
	PPC_FUNC_PROLOGUE();
	// lwz r9,4(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
	// li r10,2
	ctx.r10.s64 = 2;
	// clrlwi r11,r9,31
	ctx.r11.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451ef8
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451efc
		if (!ctx.cr6.eq) goto loc_82451EFC;
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x82451efc
	} else {
	loc_82451EF8:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_82451EFC:
	// cmplwi cr6,r10,1
	// blt cr6,0x82451f20
	if (ctx.r10.u32 >= 1) {
		// bne cr6,0x82451f2c
		if (!ctx.cr6.eq) goto loc_82451F2C;
		// lwz r11,16(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// rlwinm r11,r11,29,3,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
		// addi r10,r11,-1
		ctx.r10.s64 = ctx.r11.s64 + -1;
		// rlwinm r11,r10,0,0,20
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFF800;
		// addi r11,r11,2048
		ctx.r11.s64 = ctx.r11.s64 + 2048;
		// b 0x82451f24
	} else {
	loc_82451F20:
		// lwz r11,20(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	}
loc_82451F24:
	// cmplwi cr6,r11,0
	// bne cr6,0x82451f30
	if (ctx.r11.u32 == 0) {
	loc_82451F2C:
		// lwz r11,12(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	}
loc_82451F30:
	// rlwinm r8,r9,0,0,0
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x80000000;
	// li r10,8
	ctx.r10.s64 = 8;
	// cmplwi cr6,r8,0
	// beq cr6,0x82451f44
	if (ctx.r8.u32 != 0) {
		// li r10,16
		ctx.r10.s64 = 16;
	}
loc_82451F44:
	// rlwinm r7,r9,31,29,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7;
	// rlwinm r6,r10,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF;
	// mullw r5,r7,r6
	ctx.r5.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r6.s32);
	// divwu r3,r11,r5
	ctx.r3.u32 = ctx.r5.u32 ? ctx.r11.u32 / ctx.r5.u32 : 0;
	// twllei r5,0
	if (ctx.r5.s32 == 0 || ctx.r5.u32 < 0u) __builtin_trap();
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_1F60_wrh"))) PPC_WEAK_FUNC(atSingleton_1F60_wrh);
PPC_FUNC_IMPL(__imp__atSingleton_1F60_wrh) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,4(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
	// li r9,2
	ctx.r9.s64 = 2;
	// clrlwi r11,r10,31
	ctx.r11.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451f80
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451f84
		if (!ctx.cr6.eq) goto loc_82451F84;
		// li r9,1
		ctx.r9.s64 = 1;
		// b 0x82451f84
	} else {
	loc_82451F80:
		// li r9,0
		ctx.r9.s64 = 0;
	}
loc_82451F84:
	// cmplwi cr6,r9,1
	// blt cr6,0x82451f94
	if (ctx.r9.u32 >= 1) {
		// li r3,0
		ctx.r3.s64 = 0;
		// b 0x82451fb8
	} else {
	loc_82451F94:
		// rlwinm r11,r10,0,0,0
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x80000000;
		// cmplwi cr6,r11,0
		// li r11,8
		ctx.r11.s64 = 8;
		// beq cr6,0x82451fa8
		if (ctx.r11.u32 != 0) {
			// li r11,16
			ctx.r11.s64 = 16;
		}
	loc_82451FA8:
		// rlwinm r10,r10,31,29,31
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7;
		// rlwinm r9,r11,29,3,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
		// mullw r8,r10,r9
		ctx.r8.s64 = int64_t(ctx.r10.s32) * int64_t(ctx.r9.s32);
		// mullw r3,r8,r4
		ctx.r3.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r4.s32);
	}
loc_82451FB8:
	// cmplwi cr6,r5,0
	// beqlr cr6
	if (ctx.r5.u32 == 0) return;
	// li r7,0
	ctx.r7.s64 = 0;
	// stw r7,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r7.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_1FD0_p42"))) PPC_WEAK_FUNC(atSingleton_1FD0_p42);
PPC_FUNC_IMPL(__imp__atSingleton_1FD0_p42) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,4(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
	// li r9,2
	ctx.r9.s64 = 2;
	// clrlwi r11,r10,31
	ctx.r11.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82451ff0
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82451ff4
		if (!ctx.cr6.eq) goto loc_82451FF4;
		// li r9,1
		ctx.r9.s64 = 1;
		// b 0x82451ff4
	} else {
	loc_82451FF0:
		// li r9,0
		ctx.r9.s64 = 0;
	}
loc_82451FF4:
	// cmplwi cr6,r9,1
	// blt cr6,0x82452014
	if (ctx.r9.u32 >= 1) {
		// beq cr6,0x82452008
		if (!(ctx.cr6.eq)) {
			// li r3,0
			ctx.r3.s64 = 0;
			// blr
			return;
		}
	loc_82452008:
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// rlwinm r3,r11,28,4,31
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 28) & 0xFFFFFFF;
		// blr
		return;
	}
loc_82452014:
	// rlwinm r8,r10,0,0,0
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x80000000;
	// lwz r9,12(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// li r11,8
	ctx.r11.s64 = 8;
	// cmplwi cr6,r8,0
	// beq cr6,0x8245202c
	if (ctx.r8.u32 != 0) {
		// li r11,16
		ctx.r11.s64 = 16;
	}
loc_8245202C:
	// rlwinm r7,r10,31,29,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7;
	// rlwinm r6,r11,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
	// mullw r5,r7,r6
	ctx.r5.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r6.s32);
	// divwu r3,r9,r5
	ctx.r3.u32 = ctx.r5.u32 ? ctx.r9.u32 / ctx.r5.u32 : 0;
	// twllei r5,0
	if (ctx.r5.s32 == 0 || ctx.r5.u32 < 0u) __builtin_trap();
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_2048_2hr"))) PPC_WEAK_FUNC(atSingleton_2048_2hr);
PPC_FUNC_IMPL(__imp__atSingleton_2048_2hr) {
	PPC_FUNC_PROLOGUE();
	// lwz r9,4(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
	// li r10,2
	ctx.r10.s64 = 2;
	// clrlwi r11,r9,31
	ctx.r11.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82452068
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x8245206c
		if (!ctx.cr6.eq) goto loc_8245206C;
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x8245206c
	} else {
	loc_82452068:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_8245206C:
	// cmplwi cr6,r10,1
	// blt cr6,0x824520ec
	if (ctx.r10.u32 >= 1) {
		// beq cr6,0x82452080
		if (!(ctx.cr6.eq)) {
			// li r3,0
			ctx.r3.s64 = 0;
			// blr
			return;
		}
	loc_82452080:
		// li r10,2
		ctx.r10.s64 = 2;
		// cmplwi cr6,r11,1
		// blt cr6,0x82452098
		if (ctx.r11.u32 >= 1) {
			// bne cr6,0x8245209c
			if (!ctx.cr6.eq) goto loc_8245209C;
			// li r10,1
			ctx.r10.s64 = 1;
			// b 0x8245209c
		} else {
		loc_82452098:
			// li r10,0
			ctx.r10.s64 = 0;
		}
	loc_8245209C:
		// cmplwi cr6,r10,1
		// blt cr6,0x824520bc
		if (ctx.r10.u32 >= 1) {
			// beq cr6,0x824520b0
			if (!(ctx.cr6.eq)) {
				// li r11,0
				ctx.r11.s64 = 0;
				// b 0x824520c0
				goto loc_824520C0;
			}
		loc_824520B0:
			// lwz r11,16(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
			// rlwinm r11,r11,29,3,20
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFF800;
			// b 0x824520c0
		} else {
		loc_824520BC:
			// lwz r11,16(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		}
	loc_824520C0:
		// rlwinm r10,r9,0,0,0
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x80000000;
		// cmplwi cr6,r10,0
		// li r10,8
		ctx.r10.s64 = 8;
		// beq cr6,0x824520d4
		if (ctx.r10.u32 != 0) {
			// li r10,16
			ctx.r10.s64 = 16;
		}
	loc_824520D4:
		// rlwinm r9,r9,31,29,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7;
		// rlwinm r8,r10,29,3,31
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF;
		// mullw r7,r9,r8
		ctx.r7.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r8.s32);
		// divwu r3,r11,r7
		ctx.r3.u32 = ctx.r7.u32 ? ctx.r11.u32 / ctx.r7.u32 : 0;
		// twllei r7,0
		if (ctx.r7.s32 == 0 || ctx.r7.u32 < 0u) __builtin_trap();
		// blr
		return;
	}
loc_824520EC:
	// b 0x82451e60
	atSingleton_1E60_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_20F0"))) PPC_WEAK_FUNC(msgMsgSink_20F0);
PPC_FUNC_IMPL(__imp__msgMsgSink_20F0) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// mr r8,r3
	ctx.r8.u64 = ctx.r3.u64;
	// li r10,2
	ctx.r10.s64 = 2;
	// lwz r4,4(r8)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r8.u32 + 4);
	// clrlwi r11,r4,31
	ctx.r11.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r11,1
	// blt cr6,0x82452120
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x82452124
		if (!ctx.cr6.eq) goto loc_82452124;
		// li r10,1
		ctx.r10.s64 = 1;
		// b 0x82452124
	} else {
	loc_82452120:
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_82452124:
	// cmplwi cr6,r10,1
	// blt cr6,0x82452238
	if (ctx.r10.u32 >= 1) {
		// beq cr6,0x82452144
		if (!(ctx.cr6.eq)) {
			// li r3,0
			ctx.r3.s64 = 0;
			// blr
			return;
		}
	loc_82452144:
		// li r10,2
		ctx.r10.s64 = 2;
		// cmplwi cr6,r11,1
		// blt cr6,0x8245215c
		if (ctx.r11.u32 >= 1) {
			// bne cr6,0x82452160
			if (!ctx.cr6.eq) goto loc_82452160;
			// li r10,1
			ctx.r10.s64 = 1;
			// b 0x82452160
		} else {
		loc_8245215C:
			// li r10,0
			ctx.r10.s64 = 0;
		}
	loc_82452160:
		// cmplwi cr6,r10,1
		// blt cr6,0x82452180
		if (ctx.r10.u32 >= 1) {
			// beq cr6,0x82452174
			if (!(ctx.cr6.eq)) {
				// li r5,0
				ctx.r5.s64 = 0;
				// b 0x82452184
				goto loc_82452184;
			}
		loc_82452174:
			// lwz r11,16(r8)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
			// rlwinm r5,r11,29,3,20
			ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFF800;
			// b 0x82452184
		} else {
		loc_82452180:
			// lwz r5,16(r8)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		}
	loc_82452184:
		// mr r3,r8
		ctx.r3.u64 = ctx.r8.u64;
		// bl 0x82451d80
		msgMsgSink_1D80_g(ctx, base);
		// rlwinm r6,r4,1,31,31
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 1) & 0x1;
		// li r10,8
		ctx.r10.s64 = 8;
		// cmplwi cr6,r6,0
		// beq cr6,0x824521a0
		if (ctx.r6.u32 != 0) {
			// li r10,16
			ctx.r10.s64 = 16;
		}
	loc_824521A0:
		// lwz r11,4(r8)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r8.u32 + 4);
		// rlwinm r7,r10,29,3,31
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF;
		// li r9,2
		ctx.r9.s64 = 2;
		// clrlwi r10,r11,31
		ctx.r10.u64 = ctx.r11.u32 & 0x1;
		// rlwinm r11,r11,31,29,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7;
		// cmplwi cr6,r10,1
		// mullw r7,r11,r7
		ctx.r7.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r7.s32);
		// blt cr6,0x824521cc
		if (ctx.r10.u32 >= 1) {
			// bne cr6,0x824521d0
			if (!ctx.cr6.eq) goto loc_824521D0;
			// li r9,1
			ctx.r9.s64 = 1;
			// b 0x824521d0
		} else {
		loc_824521CC:
			// li r9,0
			ctx.r9.s64 = 0;
		}
	loc_824521D0:
		// cmplwi cr6,r9,1
		// blt cr6,0x824521f0
		if (ctx.r9.u32 >= 1) {
			// beq cr6,0x824521e4
			if (!(ctx.cr6.eq)) {
				// li r10,0
				ctx.r10.s64 = 0;
				// b 0x824521f4
				goto loc_824521F4;
			}
		loc_824521E4:
			// lwz r10,16(r8)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
			// rlwinm r10,r10,29,3,20
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFF800;
			// b 0x824521f4
		} else {
		loc_824521F0:
			// lwz r10,16(r8)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		}
	loc_824521F4:
		// cmplwi cr6,r6,0
		// li r11,8
		ctx.r11.s64 = 8;
		// beq cr6,0x82452204
		if (ctx.r6.u32 != 0) {
			// li r11,16
			ctx.r11.s64 = 16;
		}
	loc_82452204:
		// rlwinm r9,r4,31,29,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 31) & 0x7;
		// rlwinm r8,r11,29,3,31
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
		// add r6,r3,r5
		ctx.r6.u64 = ctx.r3.u64 + ctx.r5.u64;
		// mullw r5,r9,r8
		ctx.r5.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r8.s32);
		// divwu r4,r10,r5
		ctx.r4.u32 = ctx.r5.u32 ? ctx.r10.u32 / ctx.r5.u32 : 0;
		// divwu r3,r6,r7
		ctx.r3.u32 = ctx.r7.u32 ? ctx.r6.u32 / ctx.r7.u32 : 0;
		// twllei r5,0
		if (ctx.r5.s32 == 0 || ctx.r5.u32 < 0u) __builtin_trap();
		// twllei r7,0
		if (ctx.r7.s32 == 0 || ctx.r7.u32 < 0u) __builtin_trap();
		// subf r3,r4,r3
		ctx.r3.s64 = ctx.r3.s64 - ctx.r4.s64;
		// blr
		return;
	}
loc_82452238:
	// mr r3,r8
	ctx.r3.u64 = ctx.r8.u64;
	// bl 0x82451ed8
	msgMsgSink_1ED8_2h(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_2250_h"))) PPC_WEAK_FUNC(game_2250_h);
PPC_FUNC_IMPL(__imp__game_2250_h) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r11.u32);
	// stw r11,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r11.u32);
	// stw r11,32(r3)
	PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r11.u32);
	// stw r11,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r11.u32);
	// lbz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 40);
	// rlwinm r10,r11,0,28,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// stb r10,40(r3)
	PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r10.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_2278_w"))) PPC_WEAK_FUNC(atSingleton_2278_w);
PPC_FUNC_IMPL(__imp__atSingleton_2278_w) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,16056(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);  /* glob:lbl_82003EB8 @ 0x82003eb8 */
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// blt cr6,0x82452294
	if (ctx.f1.f64 >= ctx.f0.f64) {
		// bso cr6,0x82452294
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x82452288, "bso");
		// li r3,2400
		ctx.r3.s64 = 2400;
		// blr
		return;
	}
loc_82452294:
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lfs f0,-25416(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25416);  /* glob:lbl_82079CB8 @ 0x82079cb8 */
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// bgt cr6,0x824522b0
	if (ctx.f1.f64 <= ctx.f0.f64) {
		// bso cr6,0x824522b0
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x824522A4, "bso");
		// li r3,-2400
		// blr
		return;
	}
loc_824522B0:
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lfs f0,-25396(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25396);  /* glob:lbl_82079CCC @ 0x82079ccc */
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f1,f0
	ctx.f0.f64 = double(float(ctx.f1.f64 * ctx.f0.f64));
	// fctiwz f13,f0
	ctx.f13.s64 = (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfd f13,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f13.u64);
	// lhz r3,-10(r1)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r1.u32 + -10);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_22D0_g"))) PPC_WEAK_FUNC(atSingleton_22D0_g);
PPC_FUNC_IMPL(__imp__atSingleton_22D0_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_29"))) PPC_WEAK_FUNC(atSingleton_vfn_29);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_29) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 40);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,128
	// beq cr6,0x8245232c
	if (ctx.r10.u32 != 128) {
		// clrlwi r9,r11,24
		ctx.r9.u64 = ctx.r11.u32 & 0xFF;
		// ori r11,r9,128
		ctx.r11.u64 = ctx.r9.u64 | 128;
		// rlwinm r8,r11,0,28,28
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
		// cmplwi cr6,r8,8
		// stb r11,40(r31)
		PPC_STORE_U8(var_r31 + 40, ctx.r11.u8);
		// beq cr6,0x8245232c
		if (ctx.r8.u32 == 8) goto loc_8245232C;
		// lwz r11,44(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lbz r10,45(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 45);
		// addi r7,r10,1
		ctx.r7.s64 = ctx.r10.s64 + 1;
		// stb r7,45(r11)
		PPC_STORE_U8(ctx.r11.u32 + 45, ctx.r7.u8);
	}
loc_8245232C:
	// lwz r5,0(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,16(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lbz r10,40(r31)
	ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 40);
	// rlwinm r9,r10,0,28,28
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r9,8
	// beq cr6,0x82452364
	if (ctx.r9.u32 != 8) {
		// lwz r11,44(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lbz r10,44(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 44);
		// addi r8,r10,255
		ctx.r8.s64 = ctx.r10.s64 + 255;
		// stb r8,44(r11)
		PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r8.u8);
	}
loc_82452364:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_2378_g"))) PPC_WEAK_FUNC(atSingleton_2378_g);
PPC_FUNC_IMPL(__imp__atSingleton_2378_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 40);
	// rlwinm r9,r11,0,28,28
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r9,8
	// bne cr6,0x824523bc
	if (ctx.r9.u32 == 8) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r3,44(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 44);
		// bl 0x82459850
		atSingleton_9850(ctx, base);
		// blr
		return;
	}
loc_824523BC:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// ori r7,r8,64
	ctx.r7.u64 = ctx.r8.u64 | 64;
	// stb r7,40(r31)
	PPC_STORE_U8(var_r31 + 40, ctx.r7.u8);
	// lbz r10,45(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 45);
	// addi r6,r10,255
	ctx.r6.s64 = ctx.r10.s64 + 255;
	// stb r6,45(r11)
	PPC_STORE_U8(ctx.r11.u32 + 45, ctx.r6.u8);
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lwz r3,44(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 44);
	// stw r4,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r4.u32);
	// bl 0x82458ce8
	atSingleton_8CE8(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_2400_gen"))) PPC_WEAK_FUNC(atSingleton_2400_gen);
PPC_FUNC_IMPL(__imp__atSingleton_2400_gen) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=144, savegprlr_28
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lis r10,-32256
	// lwz r11,44(r29)
	ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 44);
	// lfs f31,18992(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 18992);
	var_f31 = double(temp.f32);
	// lwz r28,16(r11)
	var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + 16));
	// lbz r11,6(r30)
	ctx.r11.u64 = PPC_LOAD_U8(var_r30 + 6);
	// rlwinm r9,r11,0,25,25
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x40;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245255c
	if (ctx.r9.u32 != 0) {
		// rlwinm r7,r11,0,27,27
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10;
		// cmplwi cr6,r7,0
		// bne cr6,0x82452454
		if (ctx.r7.u32 == 0) {
			// lbz r6,40(r29)
			ctx.r6.u64 = PPC_LOAD_U8(var_r29 + 40);
			// rlwinm r5,r6,0,27,27
			ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0x10;
			// cmplwi cr6,r5,16
			// beq cr6,0x8245255c
			if (ctx.r5.u32 == 16) goto loc_8245255C;
		}
	loc_82452454:
		// lhz r4,2(r30)
		ctx.r4.u64 = PPC_LOAD_U16(var_r30 + 2);
		// lhz r3,0(r30)
		ctx.r3.u64 = PPC_LOAD_U16(var_r30 + 0);
		// extsh r5,r4
		ctx.r5.s64 = ctx.r4.s16;
		// extsh r4,r3
		ctx.r4.s64 = ctx.r3.s16;
		// lwz r3,12(r29)
		ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 12);
		// bl 0x82447ce0
		atSingleton_7CE0_w(ctx, base);
		// lbz r11,6(r30)
		ctx.r11.u64 = PPC_LOAD_U8(var_r30 + 6);
		// extsh r31,r3
		var_r31 = (uint32_t)(ctx.r3.s16);
		// rlwinm r11,r11,30,30,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 30) & 0x3;
		// cmpwi cr6,r11,1
		// beq cr6,0x824524f4
		if (ctx.r11.s32 != 1) {
			// cmpwi cr6,r11,2
			// beq cr6,0x824524dc
			if (ctx.r11.s32 != 2) {
				// cmpwi cr6,r11,3
				// bne cr6,0x82452508
				if (ctx.r11.s32 != 3) goto loc_82452508;
				// lfs f1,44(r28)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(var_r28 + 44);
				ctx.f1.f64 = double(temp.f32);
				// lwz r3,12(r29)
				ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 12);
				// bl 0x82452278
				atSingleton_2278_w(ctx, base);
				// extsh r7,r3
				ctx.r7.s64 = ctx.r3.s16;
				// extsh r9,r31
				ctx.r9.s64 = (int16_t)var_r31;
				// std r7,80(r1)
				PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
				// std r9,88(r1)
				PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r9.u64);
				// lfd f0,88(r1)
				ctx.fpscr.disableFlushMode();
				ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
				// fcfid f13,f0
				ctx.f13.f64 = double(ctx.f0.s64);
				// frsp f12,f13
				ctx.f12.f64 = double(float(ctx.f13.f64));
				// lfd f11,80(r1)
				ctx.f11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
				// fcfid f10,f11
				ctx.f10.f64 = double(ctx.f11.s64);
				// frsp f9,f10
				ctx.f9.f64 = double(float(ctx.f10.f64));
				// fmuls f8,f9,f12
				ctx.f8.f64 = double(float(ctx.f9.f64 * ctx.f12.f64));
				// fmuls f7,f8,f31
				ctx.f7.f64 = double(float(ctx.f8.f64 * var_f31));
				// fctiwz f6,f7
				ctx.f6.s64 = (ctx.f7.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f7.f64));
				// stfd f6,88(r1)
				PPC_STORE_U64(ctx.r1.u32 + 88, ctx.f6.u64);
				// lhz r31,94(r1)
				var_r31 = (uint32_t)(PPC_LOAD_U16(ctx.r1.u32 + 94));
				// b 0x82452508
				goto loc_82452508;
			}
		loc_824524DC:
			// lfs f1,44(r28)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r28 + 44);
			ctx.f1.f64 = double(temp.f32);
			// lwz r3,12(r29)
			ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 12);
			// bl 0x82452278
			atSingleton_2278_w(ctx, base);
			// subf r4,r31,r3
			ctx.r4.s64 = ctx.r3.s64 - (int64_t)(int32_t)var_r31;
			// extsh r31,r4
			var_r31 = (uint32_t)(ctx.r4.s16);
			// b 0x82452508
		} else {
		loc_824524F4:
			// lfs f1,44(r28)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r28 + 44);
			ctx.f1.f64 = double(temp.f32);
			// lwz r3,12(r29)
			ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 12);
			// bl 0x82452278
			atSingleton_2278_w(ctx, base);
			// add r3,r3,r31
			ctx.r3.u64 = ctx.r3.u64 + var_r31;
			// extsh r31,r3
			var_r31 = (uint32_t)(ctx.r3.s16);
		}
	loc_82452508:
		// extsh r11,r31
		ctx.r11.s64 = (int16_t)var_r31;
		// cmpwi cr6,r11,2400
		// blt cr6,0x82452520
		if (ctx.r11.s32 >= 2400) {
			// lis r11,-32256
			ctx.r11.s64 = -2113929216;
			// lfs f1,16056(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);  /* glob:lbl_82003EB8 @ 0x82003eb8 */
			ctx.f1.f64 = double(temp.f32);
			// b 0x82452554
		} else {
		loc_82452520:
			// cmpwi cr6,r11,-2400
			// bgt cr6,0x82452534
			if (ctx.r11.s32 <= -2400) {
				// lis r11,-32248
				ctx.r11.s64 = -2113404928;
				// lfs f1,-25416(r11)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25416);  /* glob:lbl_82079CB8 @ 0x82079cb8 */
				ctx.f1.f64 = double(temp.f32);
				// b 0x82452554
			} else {
			loc_82452534:
				// extsw r11,r11
				ctx.r11.s64 = ctx.r11.s32;
				// std r11,88(r1)
				PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r11.u64);
				// lfd f5,88(r1)
				ctx.fpscr.disableFlushMode();
				ctx.f5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
				// fcfid f4,f5
				ctx.f4.f64 = double(ctx.f5.s64);
				// lis r11,-32256
				ctx.r11.s64 = -2113929216;
				// lfs f0,18960(r11)
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18960);  /* glob:lbl_82004A10 @ 0x82004a10 */
				ctx.f0.f64 = double(temp.f32);
				// frsp f3,f4
				ctx.f3.f64 = double(float(ctx.f4.f64));
				// fmuls f1,f3,f0
				ctx.f1.f64 = double(float(ctx.f3.f64 * ctx.f0.f64));
			}
		}
	loc_82452554:
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// bl 0x8245a490
		atSingleton_A490_g(ctx, base);
	}
loc_8245255C:
	// lbz r11,6(r30)
	ctx.r11.u64 = PPC_LOAD_U8(var_r30 + 6);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// beq cr6,0x824526d0
	if (ctx.r10.u32 != 0) {
		// rlwinm r8,r11,0,26,26
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r8,0
		// bne cr6,0x82452588
		if (ctx.r8.u32 == 0) {
			// lbz r7,40(r29)
			ctx.r7.u64 = PPC_LOAD_U8(var_r29 + 40);
			// rlwinm r6,r7,0,27,27
			ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x10;
			// cmplwi cr6,r6,16
			// beq cr6,0x824526d0
			if (ctx.r6.u32 == 16) {
				// lbz r7,40(r29)
				ctx.r7.u64 = PPC_LOAD_U8(var_r29 + 40);
				// ori r6,r7,16
				ctx.r6.u64 = ctx.r7.u64 | 16;
				// stb r6,40(r29)
				PPC_STORE_U8(var_r29 + 40, ctx.r6.u8);
				return;
			}
		}
	loc_82452588:
		// lbz r5,5(r30)
		ctx.r5.u64 = PPC_LOAD_U8(var_r30 + 5);
		// lbz r4,4(r30)
		ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 4);
		// lwz r3,12(r29)
		ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 12);
		// bl 0x82447ce0
		atSingleton_7CE0_w(ctx, base);
		// lis r11,-32256
		// lbz r5,6(r30)
		ctx.r5.u64 = PPC_LOAD_U8(var_r30 + 6);
		// rlwinm r3,r3,2,22,29
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0x3FC;
		// addi r10,r11,17936
		ctx.r10.s64 = ctx.r11.s64 + 17936;
		// clrlwi r11,r5,30
		ctx.r11.u64 = ctx.r5.u32 & 0x3;
		// cmpwi cr6,r11,1
		// lwzx r31,r3,r10
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + ctx.r10.u32));
		// stw r31,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r31);
		// beq cr6,0x82452664
		if (ctx.r11.s32 != 1) {
			// cmpwi cr6,r11,2
			// beq cr6,0x82452634
			if (ctx.r11.s32 != 2) {
				// cmpwi cr6,r11,3
				// bne cr6,0x8245268c
				if (ctx.r11.s32 != 3) goto loc_8245268C;
				// lfs f1,40(r28)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(var_r28 + 40);
				ctx.f1.f64 = double(temp.f32);
				// bl 0x82432450
				jumptable_2450(ctx, base);
				// extsw r9,r31
				ctx.r9.s64 = (int32_t)var_r31;
				// addi r10,r1,80
				ctx.r10.s64 = ctx.r1.s64 + 80;
				// lis r11,-32256
				// addi r8,r1,88
				ctx.r8.s64 = ctx.r1.s64 + 88;
				// std r9,80(r1)
				PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r9.u64);
				// lfd f0,18968(r11)
				ctx.fpscr.disableFlushMode();
				ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18968);
				// fmul f2,f1,f0
				ctx.f2.f64 = ctx.f1.f64 * ctx.f0.f64;
				// fctiwz f13,f2
				ctx.f13.s64 = (ctx.f2.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f2.f64));
				// lfd f1,80(r1)
				ctx.f1.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
				// stfiwx f13,0,r10
				PPC_STORE_U32(ctx.r10.u32, ctx.f13.u32);
				// fcfid f0,f1
				ctx.f0.f64 = double(ctx.f1.s64);
				// frsp f12,f0
				ctx.f12.f64 = double(float(ctx.f0.f64));
				// lwz r7,80(r1)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				// extsw r6,r7
				ctx.r6.s64 = ctx.r7.s32;
				// std r6,80(r1)
				PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r6.u64);
				// lfd f11,80(r1)
				ctx.f11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
				// fcfid f10,f11
				ctx.f10.f64 = double(ctx.f11.s64);
				// frsp f9,f10
				ctx.f9.f64 = double(float(ctx.f10.f64));
				// fmuls f8,f9,f12
				ctx.f8.f64 = double(float(ctx.f9.f64 * ctx.f12.f64));
				// fmuls f7,f8,f31
				ctx.f7.f64 = double(float(ctx.f8.f64 * var_f31));
				// fctiwz f6,f7
				ctx.f6.s64 = (ctx.f7.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f7.f64));
				// stfiwx f6,0,r8
				PPC_STORE_U32(ctx.r8.u32, ctx.f6.u32);
				// lwz r31,88(r1)
				var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 88));
				// b 0x8245268c
				goto loc_8245268C;
			}
		loc_82452634:
			// lfs f1,40(r28)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r28 + 40);
			ctx.f1.f64 = double(temp.f32);
			// bl 0x82432450
			jumptable_2450(ctx, base);
			// lis r11,-32256
			// addi r5,r1,80
			ctx.r5.s64 = ctx.r1.s64 + 80;
			// neg r4,r31
			ctx.r4.s64 = static_cast<int64_t>(-var_r31);
			// lfd f0,18984(r11)
			ctx.fpscr.disableFlushMode();
			ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18984);
			// fmul f5,f1,f0
			ctx.f5.f64 = ctx.f1.f64 * ctx.f0.f64;
			// fctiwz f4,f5
			ctx.f4.s64 = (ctx.f5.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f5.f64));
			// stfiwx f4,0,r5
			PPC_STORE_U32(ctx.r5.u32, ctx.f4.u32);
			// lwz r3,80(r1)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// subf r31,r3,r4
			var_r31 = (uint32_t)(ctx.r4.s64 - ctx.r3.s64);
			// b 0x8245268c
		} else {
		loc_82452664:
			// lfs f1,40(r28)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r28 + 40);
			ctx.f1.f64 = double(temp.f32);
			// bl 0x82432450
			jumptable_2450(ctx, base);
			// lis r11,-32256
			// addi r10,r1,80
			ctx.r10.s64 = ctx.r1.s64 + 80;
			// lfd f0,18984(r11)
			ctx.fpscr.disableFlushMode();
			ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18984);
			// fmul f3,f1,f0
			ctx.f3.f64 = ctx.f1.f64 * ctx.f0.f64;
			// fctiwz f2,f3
			ctx.f2.s64 = (ctx.f3.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f3.f64));
			// stfiwx f2,0,r10
			PPC_STORE_U32(ctx.r10.u32, ctx.f2.u32);
			// lwz r9,80(r1)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// subf r31,r9,r31
			var_r31 = (uint32_t)((int64_t)(int32_t)var_r31 - ctx.r9.s64);
		}
	loc_8245268C:
		// mr r11,r31
		ctx.r11.u64 = var_r31;
		// cmpwi cr6,r31,6400
		// ble cr6,0x8245269c
		if ((int32_t)var_r31 > 6400) {
			// li r11,6400
			ctx.r11.s64 = 6400;
		}
	loc_8245269C:
		// extsw r8,r11
		ctx.r8.s64 = ctx.r11.s32;
		// lis r11,-32248
		// std r8,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r8.u64);
		// lfd f0,88(r1)
		ctx.fpscr.disableFlushMode();
		ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// fcfid f13,f0
		ctx.f13.f64 = double(ctx.f0.s64);
		// lfd f1,-25752(r11)
		ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -25752);
		// lis r11,-32256
		ctx.r11.s64 = -2113929216;
		// lfd f0,18976(r11)
		ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18976);  /* glob:lbl_82004A20 @ 0x82004a20 */
		// fmul f2,f13,f0
		ctx.f2.f64 = ctx.f13.f64 * ctx.f0.f64;
		// bl 0x82431308
		atSingleton_1308_g(ctx, base);
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// frsp f1,f1
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = double(float(ctx.f1.f64));
		// bl 0x8245a408
		atSingleton_A408_g(ctx, base);
	}
loc_824526D0:
	// lbz r7,40(r29)
	ctx.r7.u64 = PPC_LOAD_U8(var_r29 + 40);
	// ori r6,r7,16
	ctx.r6.u64 = ctx.r7.u64 | 16;
	// stb r6,40(r29)
	PPC_STORE_U8(var_r29 + 40, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__jumptable_26E8"))) PPC_WEAK_FUNC(jumptable_26E8);
PPC_FUNC_IMPL(__imp__jumptable_26E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// stw r3,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r3.u32);
	// stw r3,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r3.u32);
	// cmpwi cr6,r8,0
	// addi r11,r11,18996
	ctx.r11.s64 = ctx.r11.s64 + 18996;
	// stw r4,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r4.u32);
	// stw r6,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r6.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stw r7,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r7.u32);
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// bne cr6,0x8245271c
	if (ctx.r8.s32 == 0) {
		// mr r10,r11
		ctx.r10.u64 = ctx.r11.u64;
	}
loc_8245271C:
	// rlwinm r10,r10,3,28,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0x8;
	// stw r5,44(r3)
	PPC_STORE_U32(ctx.r3.u32 + 44, ctx.r5.u32);
	// stw r11,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r11.u32);
	// stw r11,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r11.u32);
	// stw r11,32(r3)
	PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r11.u32);
	// stw r11,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r11.u32);
	// stb r10,40(r3)
	PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r10.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_5_2740_1"))) PPC_WEAK_FUNC(atSingleton_vfn_5_2740_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_5_2740_1) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x82452750
	if (ctx.r4.s32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_82452750:
	// lbz r10,40(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 40);
	// rlwimi r10,r11,5,26,26
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 5) & 0x20) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFDF);
	// stb r10,40(r3)
	PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r10.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_3_2760_1"))) PPC_WEAK_FUNC(atSingleton_vfn_3_2760_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_3_2760_1) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r4,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r4.u32);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_0"))) PPC_WEAK_FUNC(game_vt4A34_0);
PPC_FUNC_IMPL(__imp__game_vt4A34_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x824527ac
	if (ctx.r10.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32862
		ctx.r4.u64 = ctx.r4.u64 | 32862;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824527AC:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_27C0"))) PPC_WEAK_FUNC(atSingleton_27C0);
PPC_FUNC_IMPL(__imp__atSingleton_27C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r10,4(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// stw r11,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
	// lwz r9,8(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r8,4(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// stw r8,4(r9)
	PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r8.u32);
	// lwz r7,0(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// stw r31,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ var_r31);
	// stw r31,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r31);
	// lwz r6,16(r7)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
	// lbz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 40);
	// rlwinm r4,r11,0,28,28
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r4,8
	// bne cr6,0x82452838
	if (ctx.r4.u32 == 8) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r3,44(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 44);
		// bl 0x82459850
		atSingleton_9850(ctx, base);
		// blr
		return;
	}
loc_82452838:
	// rlwinm r3,r11,0,0,24
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r3,128
	// bne cr6,0x82452854
	if (ctx.r3.u32 == 128) {
		// lwz r11,44(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lbz r10,45(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 45);
		// addi r10,r10,255
		ctx.r10.s64 = ctx.r10.s64 + 255;
		// stb r10,45(r11)
		PPC_STORE_U8(ctx.r11.u32 + 45, ctx.r10.u8);
	}
loc_82452854:
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lbz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 44);
	// addi r8,r10,255
	ctx.r8.s64 = ctx.r10.s64 + 255;
	// stb r8,44(r11)
	PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r8.u8);
	// lwz r3,44(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 44);
	// bl 0x82458ce8
	atSingleton_8CE8(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_2880_h"))) PPC_WEAK_FUNC(game_2880_h);
PPC_FUNC_IMPL(__imp__game_2880_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r3,r10,27,8,31
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0xFFFFFF;
	// rlwinm r9,r3,0,7,7
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0x1000000;
	// cmplwi cr6,r9,0
	// beqlr cr6
	if (ctx.r9.u32 == 0) return;
	// clrlwi r8,r3,8
	ctx.r8.u64 = ctx.r3.u32 & 0xFFFFFF;
	// neg r3,r8
	ctx.r3.s64 = static_cast<int64_t>(-ctx.r8.u64);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_28A8_g_28A8_1"))) PPC_WEAK_FUNC(atSingleton_28A8_g_28A8_1);
PPC_FUNC_IMPL(__imp__atSingleton_28A8_g_28A8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r11,0(r9)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// rlwinm r10,r11,0,2,2
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20000000;
	// cmplwi cr6,r10,0
	// beq cr6,0x82452924
	if (ctx.r10.u32 != 0) {
		// lbz r9,40(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 40);
		// rlwinm r8,r9,0,28,28
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x8;
		// cmplwi cr6,r8,8
		// bne cr6,0x824528e4
		if (ctx.r8.u32 == 8) {
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// b 0x824528ec
		} else {
		loc_824528E4:
			// lwz r7,44(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 44);
			// lwz r3,28(r7)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 28);
		}
	loc_824528EC:
		// lwz r9,16(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r6,0(r9)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// rlwinm r11,r6,27,8,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0xFFFFFF;
		// rlwinm r5,r11,0,7,7
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x1000000;
		// cmplwi cr6,r5,0
		// beq cr6,0x8245290c
		if (ctx.r5.u32 != 0) {
			// clrlwi r4,r11,8
			ctx.r4.u64 = ctx.r11.u32 & 0xFFFFFF;
			// neg r11,r4
			ctx.r11.s64 = static_cast<int64_t>(-ctx.r4.u64);
		}
	loc_8245290C:
		// lwz r10,32(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 32);
		// li r30,0
		var_r30 = 0;
		// subf r11,r10,r11
		ctx.r11.s64 = ctx.r11.s64 - ctx.r10.s64;
		// add r8,r11,r3
		ctx.r8.u64 = ctx.r11.u64 + ctx.r3.u64;
		// stw r8,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r8.u32);
		// b 0x824529b0
	} else {
	loc_82452924:
		// rlwinm r7,r11,0,1,1
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x40000000;
		// cmplwi cr6,r7,0
		// beq cr6,0x82452948
		if (ctx.r7.u32 != 0) {
			// lwz r6,36(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lwz r4,32(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 32);
			// lwz r5,20(r6)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 20);
			// subf r3,r4,r5
			ctx.r3.s64 = ctx.r5.s64 - ctx.r4.s64;
			// stw r3,20(r31)
			PPC_STORE_U32(var_r31 + 20, ctx.r3.u32);
			// b 0x82452980
		} else {
		loc_82452948:
			// lwz r11,36(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lbz r10,40(r11)
			ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 40);
			// rlwinm r8,r10,0,25,25
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x40;
			// cmplwi cr6,r8,64
			// bne cr6,0x82452970
			if (ctx.r8.u32 == 64) {
				// lwz r7,24(r11)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
				// lwz r6,32(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 32);
				// subf r5,r6,r7
				ctx.r5.s64 = ctx.r7.s64 - ctx.r6.s64;
				// stw r5,20(r31)
				PPC_STORE_U32(var_r31 + 20, ctx.r5.u32);
				// b 0x82452980
			} else {
			loc_82452970:
				// lwz r4,28(r11)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
				// lwz r3,32(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
				// subf r11,r3,r4
				ctx.r11.s64 = ctx.r4.s64 - ctx.r3.s64;
				// stw r11,20(r31)
				PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
			}
		}
	loc_82452980:
		// li r30,0
		var_r30 = 0;
		// lwz r10,20(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 20);
		// stw r30,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r30);
		// lwz r8,0(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// rlwinm r11,r8,27,8,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0xFFFFFF;
		// rlwinm r7,r11,0,7,7
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x1000000;
		// cmplwi cr6,r7,0
		// beq cr6,0x824529a8
		if (ctx.r7.u32 != 0) {
			// clrlwi r6,r11,8
			ctx.r6.u64 = ctx.r11.u32 & 0xFFFFFF;
			// neg r11,r6
			ctx.r11.s64 = static_cast<int64_t>(-ctx.r6.u64);
		}
	loc_824529A8:
		// add r5,r10,r11
		ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
		// stw r5,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r5.u32);
	}
loc_824529B0:
	// stw r30,32(r31)
	PPC_STORE_U32(var_r31 + 32, var_r30);
	// lhz r5,4(r9)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r9.u32 + 4);
	// cmplwi cr6,r5,0
	// beq cr6,0x824529e4
	if (ctx.r5.u32 != 0) {
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r29,20(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// bl 0x82447c78
		jumptable_7C78(ctx, base);
		// lwz r4,32(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 32);
		// stw r30,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r30);
		// subf r11,r4,r29
		ctx.r11.s64 = (int64_t)(int32_t)var_r29 - ctx.r4.s64;
		// add r3,r11,r3
		ctx.r3.u64 = ctx.r11.u64 + ctx.r3.u64;
		// stw r3,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r3.u32);
	}
loc_824529E4:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lis r9,32767
	ctx.r9.s64 = 2147418112;
	// ori r11,r9,65535
	ctx.r11.u64 = ctx.r9.u64 | 65535;
	// cmpw cr6,r3,r11
	// beq cr6,0x82452a10
	if (ctx.r3.s32 != ctx.r11.s32) {
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// add r11,r11,r3
		ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
	}
loc_82452A10:
	// lwz r8,44(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 44);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// stw r11,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
	// lwz r7,16(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
	// lwz r3,20(r7)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 20);
	// bl 0x824545d8
	atSingleton_45D8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__phBoundCapsule_2A30_g"))) PPC_WEAK_FUNC(phBoundCapsule_2A30_g);
PPC_FUNC_IMPL(__imp__phBoundCapsule_2A30_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=144, savegprlr_29
	// lis r11,-32256
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r29,0
	var_r29 = 0;
	// lfs f30,15784(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	var_f30 = double(temp.f32);
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 20);
	// fmr f31,f30
	var_f31 = var_f30;
	// addi r10,r11,5
	ctx.r10.s64 = ctx.r11.s64 + 5;
	// lbz r11,2(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// addi r8,r11,-1
	ctx.r8.s64 = ctx.r11.s64 + -1;
	// stw r9,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	// lfs f0,80(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// bgt cr6,0x82452a90
	if (ctx.f1.f64 <= ctx.f0.f64) {
		// bso cr6,0x82452a90
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x82452A7C, "bso");
		// lwz r8,4(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
		// stw r8,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r8.u32);
		// lfs f31,80(r1)
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		var_f31 = double(temp.f32);
		// b 0x82452c28
	} else {
	loc_82452A90:
		// rlwinm r9,r11,3,0,28
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
		// add r11,r11,r9
		ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// lwz r7,-9(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9);
		// stw r7,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r7.u32);
		// lfs f13,80(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		ctx.f13.f64 = double(temp.f32);
		// fcmpu cr6,f1,f13
		// blt cr6,0x82452ac8
		if (ctx.f1.f64 >= ctx.f13.f64) {
			// bso cr6,0x82452ac8
			// UNIMPLEMENTED: bso
			PPC_UNIMPLEMENTED(0x82452AB0, "bso");
			// lwz r6,-5(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + -5);
			// li r29,1
			var_r29 = 1;
			// stw r6,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r6.u32);
			// lfs f31,80(r1)
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			var_f31 = double(temp.f32);
			// b 0x82452c28
		} else {
		loc_82452AC8:
			// li r11,0
			ctx.r11.s64 = 0;
			// cmpwi cr6,r8,0
			// ble cr6,0x82452c28
			if (ctx.r8.s32 <= 0) goto loc_82452C28;
			// mr r9,r10
			ctx.r9.u64 = ctx.r10.u64;
		loc_82452AD8:
			// lwz r5,0(r9)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
			// stw r5,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r5.u32);
			// lfs f12,80(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f12.f64 = double(temp.f32);
			// fcmpu cr6,f1,f12
			// blt cr6,0x82452b08
			if (ctx.f1.f64 >= ctx.f12.f64) {
				// bso cr6,0x82452b08
				// UNIMPLEMENTED: bso
				PPC_UNIMPLEMENTED(0x82452AEC, "bso");
				// lwz r4,9(r9)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r9.u32 + 9);
				// stw r4,88(r1)
				PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r4.u32);
				// lfs f11,88(r1)
				temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
				ctx.f11.f64 = double(temp.f32);
				// fcmpu cr6,f1,f11
				// bso cr6,0x82452b08
				// UNIMPLEMENTED: bso
				PPC_UNIMPLEMENTED(0x82452B00, "bso");
				// ble cr6,0x82452b1c
				if (ctx.f1.f64 <= ctx.f11.f64) goto loc_82452B1C;
			}
		loc_82452B08:
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r9,r9,9
			ctx.r9.s64 = ctx.r9.s64 + 9;
			// cmpw cr6,r11,r8
			// blt cr6,0x82452ad8
			if (ctx.r11.s32 < ctx.r8.s32) goto loc_82452AD8;
			// b 0x82452c28
			goto loc_82452C28;
		loc_82452B1C:
			// rlwinm r9,r11,3,0,28
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
			// add r11,r11,r9
			ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
			// add r31,r11,r10
			var_r31 = (uint32_t)(ctx.r11.u64 + ctx.r10.u64);
			// lwz r10,9(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 9);
			// lwz r3,0(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* phBoundCapsule::vtable@+0x0 */;
			// lbz r11,8(r31)
			ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 8);
			// cmplwi cr6,r11,1
			// stw r10,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
			// stw r3,88(r1)
			PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r3.u32);
			// lfs f0,88(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			ctx.f0.f64 = double(temp.f32);
			// fsubs f9,f1,f0
			ctx.f9.f64 = double(float(ctx.f1.f64 - ctx.f0.f64));
			// lfs f10,80(r1)
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f10.f64 = double(temp.f32);
			// fsubs f8,f10,f0
			ctx.f8.f64 = double(float(ctx.f10.f64 - ctx.f0.f64));
			// fdivs f0,f9,f8
			ctx.f0.f64 = double(float(ctx.f9.f64 / ctx.f8.f64));
			// bne cr6,0x82452bb8
			if (ctx.r11.u32 == 1) {
				// lwz r8,4(r31)
				ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 4)/* phBoundCapsule::flags@+0x4 */;
				// lwz r9,13(r31)
				ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 13);
				// stw r8,80(r1)
				PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r8.u32);
				// stw r9,88(r1)
				PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r9.u32);
				// lfs f7,88(r1)
				temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
				ctx.f7.f64 = double(temp.f32);
				// lfs f6,80(r1)
				temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				ctx.f6.f64 = double(temp.f32);
				// fcmpu cr6,f7,f6
				// blt cr6,0x82452b98
				if (ctx.f7.f64 >= ctx.f6.f64) {
					// bso cr6,0x82452b98
					// UNIMPLEMENTED: bso
					PPC_UNIMPLEMENTED(0x82452B78, "bso");
					// lis r11,-32256
					ctx.r11.s64 = -2113929216;
					// lfs f31,15788(r11)
					temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);  /* glob:lbl_82003DAC @ 0x82003dac */
					var_f31 = double(temp.f32);
					// fnmsubs f1,f0,f0,f31
					ctx.f1.f64 = double(float(-(ctx.f0.f64 * ctx.f0.f64 - var_f31)));
					// bl 0x824301d0
					phBoundCapsule_01D0_g(ctx, base);
					// frsp f5,f1
					ctx.fpscr.disableFlushMode();
					ctx.f5.f64 = double(float(ctx.f1.f64));
					// fsubs f0,f31,f5
					ctx.f0.f64 = double(float(var_f31 - ctx.f5.f64));
					// b 0x82452c08
					goto loc_82452C08;
				}
			loc_82452B98:
				// lis r11,-32256
				ctx.r11.s64 = -2113929216;
				// lfs f31,15788(r11)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);  /* glob:lbl_82003DAC @ 0x82003dac */
				var_f31 = double(temp.f32);
				// fsubs f0,f31,f0
				ctx.f0.f64 = double(float(var_f31 - ctx.f0.f64));
				// fnmsubs f1,f0,f0,f31
				ctx.f1.f64 = double(float(-(ctx.f0.f64 * ctx.f0.f64 - var_f31)));
				// bl 0x824301d0
				phBoundCapsule_01D0_g(ctx, base);
				// frsp f4,f1
				ctx.fpscr.disableFlushMode();
				ctx.f4.f64 = double(float(ctx.f1.f64));
				// fsubs f0,f31,f4
				ctx.f0.f64 = double(float(var_f31 - ctx.f4.f64));
				// b 0x82452c08
			} else {
			loc_82452BB8:
				// cmplwi cr6,r11,2
				// bne cr6,0x82452c08
				if (ctx.r11.u32 != 2) goto loc_82452C08;
				// lwz r6,4(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 4)/* phBoundCapsule::flags@+0x4 */;
				// lwz r7,13(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 13);
				// stw r6,80(r1)
				PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r6.u32);
				// stw r7,88(r1)
				PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r7.u32);
				// lfs f3,88(r1)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
				ctx.f3.f64 = double(temp.f32);
				// lfs f2,80(r1)
				temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				ctx.f2.f64 = double(temp.f32);
				// fcmpu cr6,f3,f2
				// blt cr6,0x82452bf4
				if (ctx.f3.f64 >= ctx.f2.f64) {
					// bso cr6,0x82452bf4
					// UNIMPLEMENTED: bso
					PPC_UNIMPLEMENTED(0x82452BE0, "bso");
					// lis r11,-32256
					ctx.r11.s64 = -2113929216;
					// lfs f13,15788(r11)
					temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);  /* glob:lbl_82003DAC @ 0x82003dac */
					ctx.f13.f64 = double(temp.f32);
					// fsubs f0,f13,f0
					ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
					// b 0x82452bfc
				} else {
				loc_82452BF4:
					// lis r11,-32256
					ctx.r11.s64 = -2113929216;
					// lfs f13,15788(r11)
					ctx.fpscr.disableFlushMode();
					temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);  /* glob:lbl_82003DAC @ 0x82003dac */
					ctx.f13.f64 = double(temp.f32);
				}
			loc_82452BFC:
				// fnmsubs f1,f0,f0,f13
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = double(float(-(ctx.f0.f64 * ctx.f0.f64 - ctx.f13.f64)));
				// bl 0x824301d0
				phBoundCapsule_01D0_g(ctx, base);
				// frsp f0,f1
				ctx.fpscr.disableFlushMode();
				ctx.f0.f64 = double(float(ctx.f1.f64));
			}
		loc_82452C08:
			// lwz r4,13(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 13);
			// lwz r5,4(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 4)/* phBoundCapsule::flags@+0x4 */;
			// stw r4,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r4.u32);
			// stw r5,88(r1)
			PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r5.u32);
			// lfs f13,88(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			ctx.f13.f64 = double(temp.f32);
			// lfs f1,80(r1)
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f1.f64 = double(temp.f32);
			// fsubs f12,f1,f13
			ctx.f12.f64 = double(float(ctx.f1.f64 - ctx.f13.f64));
			// fmadds f31,f12,f0,f13
			var_f31 = double(float(ctx.f12.f64 * ctx.f0.f64 + ctx.f13.f64));
		}
	}
loc_82452C28:
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// lhz r11,3(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 3);
	// cmplwi cr6,r11,1
	// blt cr6,0x82452ce4
	if (ctx.r11.u32 >= 1) {
		// beq cr6,0x82452c88
		if (!(ctx.cr6.eq)) {
			// cmplwi cr6,r11,3
			// bge cr6,0x82452d2c
			if (ctx.r11.u32 >= 3) goto loc_82452D2C;
			// addi r11,r1,88
			ctx.r11.s64 = ctx.r1.s64 + 88;
			// fctiwz f11,f31
			ctx.fpscr.disableFlushMode();
			ctx.f11.s64 = (var_f31 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&var_f31));
			// stfiwx f11,0,r11
			PPC_STORE_U32(ctx.r11.u32, ctx.f11.u32);
			// lwz r11,88(r1)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			// cmpwi cr6,r11,6400
			// ble cr6,0x82452c60
			if (ctx.r11.s32 > 6400) {
				// li r11,6400
				ctx.r11.s64 = 6400;
			}
		loc_82452C60:
			// extsw r10,r11
			ctx.r10.s64 = ctx.r11.s32;
			// lis r11,-32248
			// std r10,88(r1)
			PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r10.u64);
			// lfd f10,88(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f10.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
			// fcfid f9,f10
			ctx.f9.f64 = double(ctx.f10.s64);
			// lfd f1,-25752(r11)
			ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -25752);
			// lis r11,-32256
			ctx.r11.s64 = -2113929216;
			// lfd f0,18976(r11)
			ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18976);  /* glob:lbl_82004A20 @ 0x82004a20 */
			// fmul f2,f9,f0
			ctx.f2.f64 = ctx.f9.f64 * ctx.f0.f64;
			// b 0x82452d24
			goto loc_82452D24;
		}
	loc_82452C88:
		// fctiwz f8,f31
		ctx.fpscr.disableFlushMode();
		ctx.f8.s64 = (var_f31 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&var_f31));
		// stfd f8,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.f8.u64);
		// lhz r11,94(r1)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r1.u32 + 94);
		// extsh r11,r11
		ctx.r11.s64 = ctx.r11.s16;
		// cmpwi cr6,r11,2400
		// blt cr6,0x82452cac
		if (ctx.r11.s32 >= 2400) {
			// lis r11,-32256
			ctx.r11.s64 = -2113929216;
			// lfs f31,16056(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);  /* glob:lbl_82003EB8 @ 0x82003eb8 */
			var_f31 = double(temp.f32);
			// b 0x82452d2c
			goto loc_82452D2C;
		}
	loc_82452CAC:
		// cmpwi cr6,r11,-2400
		// bgt cr6,0x82452cc0
		if (ctx.r11.s32 <= -2400) {
			// lis r11,-32248
			ctx.r11.s64 = -2113404928;
			// lfs f31,-25416(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25416);  /* glob:lbl_82079CB8 @ 0x82079cb8 */
			var_f31 = double(temp.f32);
			// b 0x82452d2c
			goto loc_82452D2C;
		}
	loc_82452CC0:
		// extsw r9,r11
		ctx.r9.s64 = ctx.r11.s32;
		// lis r11,-32256
		// std r9,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r9.u64);
		// lfd f7,88(r1)
		ctx.fpscr.disableFlushMode();
		ctx.f7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// fcfid f6,f7
		ctx.f6.f64 = double(ctx.f7.s64);
		// lfs f0,18960(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18960);  /* glob:0x82084a10 */
		ctx.f0.f64 = double(temp.f32);
		// frsp f5,f6
		ctx.f5.f64 = double(float(ctx.f6.f64));
		// fmuls f31,f5,f0
		var_f31 = double(float(ctx.f5.f64 * ctx.f0.f64));
		// b 0x82452d2c
	} else {
	loc_82452CE4:
		// addi r8,r1,88
		ctx.r8.s64 = ctx.r1.s64 + 88;
		// fctiwz f4,f31
		ctx.fpscr.disableFlushMode();
		ctx.f4.s64 = (var_f31 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&var_f31));
		// stfiwx f4,0,r8
		PPC_STORE_U32(ctx.r8.u32, ctx.f4.u32);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// cmpwi cr6,r11,6400
		// ble cr6,0x82452d00
		if (ctx.r11.s32 > 6400) {
			// li r11,6400
			ctx.r11.s64 = 6400;
		}
	loc_82452D00:
		// extsw r7,r11
		ctx.r7.s64 = ctx.r11.s32;
		// lis r11,-32248
		// std r7,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r7.u64);
		// lfd f3,88(r1)
		ctx.fpscr.disableFlushMode();
		ctx.f3.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// fcfid f2,f3
		ctx.f2.f64 = double(ctx.f3.s64);
		// lfd f1,-25752(r11)
		ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -25752);
		// lis r11,-32256
		ctx.r11.s64 = -2113929216;
		// lfd f0,18976(r11)
		ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18976);  /* glob:lbl_82004A20 @ 0x82004a20 */
		// fmul f2,f2,f0
		ctx.f2.f64 = ctx.f2.f64 * ctx.f0.f64;
	loc_82452D24:
		// bl 0x82431308
		atSingleton_1308_g(ctx, base);
		// frsp f31,f1
		ctx.fpscr.disableFlushMode();
		var_f31 = double(float(ctx.f1.f64));
	}
loc_82452D2C:
	// lwz r6,16(r30)
	ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lwz r11,32(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 32);
	// cmplwi cr6,r11,0
	// lwz r3,124(r6)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 124);
	// beq cr6,0x82452e00
	if (ctx.r11.u32 != 0) {
		// lwz r5,20(r30)
		ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 20);
		// lhz r10,3(r5)
		ctx.r10.u64 = PPC_LOAD_U16(ctx.r5.u32 + 3);
		// cmplwi cr6,r10,1
		// blt cr6,0x82452dd0
		if (ctx.r10.u32 >= 1) {
			// beq cr6,0x82452da8
			if (!(ctx.cr6.eq)) {
				// cmplwi cr6,r10,3
				// bge cr6,0x82452ec4
				if (ctx.r10.u32 >= 3) {
					// li r6,1
					ctx.r6.s64 = 1;
					// stfs f31,36(r30)
					ctx.fpscr.disableFlushMode();
					temp.f32 = float(var_f31);
					PPC_STORE_U32(var_r30 + 36, temp.u32);
					// stw r6,40(r30)
					PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
					return;
				}
				// lwz r11,44(r4)
				// bctrl
				VCALL(ctx.r3.u32, 11, ctx, base);  // vtable slot 11 (byte +44)
				// lhz r10,21(r3)
				ctx.r10.u64 = PPC_LOAD_U16(ctx.r3.u32 + 21);
				// cmplwi cr6,r10,0
				// beq cr6,0x82452ec4
				if (ctx.r10.u32 == 0) {
					// li r6,1
					ctx.r6.s64 = 1;
					// stfs f31,36(r30)
					ctx.fpscr.disableFlushMode();
					temp.f32 = float(var_f31);
					PPC_STORE_U32(var_r30 + 36, temp.u32);
					// stw r6,40(r30)
					PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
					return;
				}
				// lwz r3,32(r30)
				ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 32);
				// lwz r9,40(r30)
				ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 40);
				// cmpwi cr6,r9,0
				// lfs f13,116(r3)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 116);
				ctx.f13.f64 = double(temp.f32);
				// beq cr6,0x82452d9c
				if (ctx.r9.s32 != 0) {
					// lfs f0,36(r30)
					temp.u32 = PPC_LOAD_U32(var_r30 + 36);
					ctx.f0.f64 = double(temp.f32);
					// fcmpu cr6,f0,f30
					// beq cr6,0x82452d9c
					if (ctx.f0.f64 == var_f30) goto loc_82452D9C;
					// fdivs f13,f13,f0
					ctx.f13.f64 = double(float(ctx.f13.f64 / ctx.f0.f64));
				}
			loc_82452D9C:
				// fmuls f1,f13,f31
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = double(float(ctx.f13.f64 * var_f31));
				// bl 0x82454048
				phBoundCapsule_4048_fw(ctx, base);
				// b 0x82452ec4
				// li r6,1
				ctx.r6.s64 = 1;
				// stfs f31,36(r30)
				ctx.fpscr.disableFlushMode();
				temp.f32 = float(var_f31);
				PPC_STORE_U32(var_r30 + 36, temp.u32);
				// stw r6,40(r30)
				PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
				return;
			}
		loc_82452DA8:
			// lwz r8,40(r30)
			ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 40);
			// lfs f0,120(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 120);
			ctx.f0.f64 = double(temp.f32);
			// cmpwi cr6,r8,0
			// beq cr6,0x82452dc0
			if (ctx.r8.s32 != 0) {
				// lfs f1,36(r30)
				temp.u32 = PPC_LOAD_U32(var_r30 + 36);
				ctx.f1.f64 = double(temp.f32);
				// fsubs f0,f0,f1
				ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f1.f64));
			}
		loc_82452DC0:
			// mr r3,r11
			ctx.r3.u64 = ctx.r11.u64;
			// fadds f1,f0,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = double(float(ctx.f0.f64 + var_f31));
			// bl 0x824540c0
			game_40C0_h(ctx, base);
			// b 0x82452ec4
			// li r6,1
			ctx.r6.s64 = 1;
			// stfs f31,36(r30)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(var_f31);
			PPC_STORE_U32(var_r30 + 36, temp.u32);
			// stw r6,40(r30)
			PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
			return;
		}
	loc_82452DD0:
		// lwz r7,40(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 40);
		// lfs f13,112(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 112);
		ctx.f13.f64 = double(temp.f32);
		// cmpwi cr6,r7,0
		// beq cr6,0x82452df0
		if (ctx.r7.s32 != 0) {
			// lfs f0,36(r30)
			temp.u32 = PPC_LOAD_U32(var_r30 + 36);
			ctx.f0.f64 = double(temp.f32);
			// fcmpu cr6,f0,f30
			// beq cr6,0x82452df0
			if (ctx.f0.f64 == var_f30) goto loc_82452DF0;
			// fdivs f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 / ctx.f0.f64));
		}
	loc_82452DF0:
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// fmuls f1,f13,f31
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = double(float(ctx.f13.f64 * var_f31));
		// bl 0x82454020
		game_4020_h(ctx, base);
		// b 0x82452ec4
	} else {
	loc_82452E00:
		// lwz r11,28(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
		// cmplwi cr6,r11,0
		// beq cr6,0x82452ea8
		if (ctx.r11.u32 != 0) {
			// lwz r6,20(r30)
			ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 20);
			// lhz r10,3(r6)
			ctx.r10.u64 = PPC_LOAD_U16(ctx.r6.u32 + 3);
			// cmplwi cr6,r10,1
			// blt cr6,0x82452e48
			if (ctx.r10.u32 >= 1) {
				// bne cr6,0x82452ec4
				if (!ctx.cr6.eq) {
					// li r6,1
					ctx.r6.s64 = 1;
					// stfs f31,36(r30)
					ctx.fpscr.disableFlushMode();
					temp.f32 = float(var_f31);
					PPC_STORE_U32(var_r30 + 36, temp.u32);
					// stw r6,40(r30)
					PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
					return;
				}
				// lwz r5,40(r30)
				ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 40);
				// lfs f0,60(r11)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 60);
				ctx.f0.f64 = double(temp.f32);
				// cmpwi cr6,r5,0
				// beq cr6,0x82452e38
				if (ctx.r5.s32 != 0) {
					// lfs f13,36(r30)
					temp.u32 = PPC_LOAD_U32(var_r30 + 36);
					ctx.f13.f64 = double(temp.f32);
					// fsubs f0,f0,f13
					ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
				}
			loc_82452E38:
				// mr r3,r11
				ctx.r3.u64 = ctx.r11.u64;
				// fadds f1,f0,f31
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = double(float(ctx.f0.f64 + var_f31));
				// bl 0x82459ec8
				atSingleton_9EC8_h(ctx, base);
				// b 0x82452ec4
				// li r6,1
				ctx.r6.s64 = 1;
				// stfs f31,36(r30)
				ctx.fpscr.disableFlushMode();
				temp.f32 = float(var_f31);
				PPC_STORE_U32(var_r30 + 36, temp.u32);
				// stw r6,40(r30)
				PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
				return;
			}
		loc_82452E48:
			// lwz r4,40(r30)
			ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 40);
			// lfs f13,56(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 56);
			ctx.f13.f64 = double(temp.f32);
			// cmpwi cr6,r4,0
			// beq cr6,0x82452e68
			if (ctx.r4.s32 != 0) {
				// lfs f0,36(r30)
				temp.u32 = PPC_LOAD_U32(var_r30 + 36);
				ctx.f0.f64 = double(temp.f32);
				// fcmpu cr6,f0,f30
				// beq cr6,0x82452e68
				if (ctx.f0.f64 == var_f30) goto loc_82452E68;
				// fdivs f13,f13,f0
				ctx.f13.f64 = double(float(ctx.f13.f64 / ctx.f0.f64));
			}
		loc_82452E68:
			// mr r3,r11
			ctx.r3.u64 = ctx.r11.u64;
			// fmuls f1,f13,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = double(float(ctx.f13.f64 * var_f31));
			// bl 0x82459e40
			game_9E40(ctx, base);
			// cmpwi cr6,r29,0
			// beq cr6,0x82452ec4
			if ((int32_t)var_r29 == 0) {
				// li r6,1
				ctx.r6.s64 = 1;
				// stfs f31,36(r30)
				ctx.fpscr.disableFlushMode();
				temp.f32 = float(var_f31);
				PPC_STORE_U32(var_r30 + 36, temp.u32);
				// stw r6,40(r30)
				PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
				return;
			}
			// lwz r3,20(r30)
			ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
			// lhz r11,0(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 0);
			// cmplwi cr6,r11,2
			// bne cr6,0x82452e9c
			if (ctx.r11.u32 == 2) {
				// li r4,1
				ctx.r4.s64 = 1;
				// lwz r3,28(r30)
				ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 28);
				// bl 0x8245a150
				phBoundCapsule_A150(ctx, base);
				// b 0x82452ec4
				// li r6,1
				ctx.r6.s64 = 1;
				// stfs f31,36(r30)
				ctx.fpscr.disableFlushMode();
				temp.f32 = float(var_f31);
				PPC_STORE_U32(var_r30 + 36, temp.u32);
				// stw r6,40(r30)
				PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
				return;
			}
		loc_82452E9C:
			// li r10,0
			ctx.r10.s64 = 0;
			// stw r10,44(r30)
			PPC_STORE_U32(var_r30 + 44, ctx.r10.u32);
			// b 0x82452ec4
		} else {
		loc_82452EA8:
			// lwz r9,20(r30)
			ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 20);
			// fmr f1,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = var_f31;
			// lhz r8,3(r9)
			ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 3);
			// addis r7,r8,1
			ctx.r7.s64 = ctx.r8.s64 + 65536;
			// addi r7,r7,-3
			ctx.r7.s64 = ctx.r7.s64 + -3;
			// clrlwi r4,r7,16
			ctx.r4.u64 = ctx.r7.u32 & 0xFFFF;
			// bl 0x8244c928
			phBoundCapsule_C928(ctx, base);
		}
	}
loc_82452EC4:
	// li r6,1
	ctx.r6.s64 = 1;
	// stfs f31,36(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r30 + 36, temp.u32);
	// stw r6,40(r30)
	PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
	return;
}

__attribute__((alias("__imp__rage_2EE0"))) PPC_WEAK_FUNC(rage_2EE0);
PPC_FUNC_IMPL(__imp__rage_2EE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lhz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,1
	// beq cr6,0x82452f28
	if (ctx.r11.u32 != 1) {
		// cmplwi cr6,r11,2
		// beq cr6,0x82452f28
		if (ctx.r11.u32 == 2) goto loc_82452F28;
		// lwz r10,0(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* rage_GameObject::vtable@+0x0 */;
		// lwz r9,24(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 24);
		// lwz r8,0(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
		// lfs f1,20(r9)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 20);
		ctx.f1.f64 = double(temp.f32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
		// b 0x82453058
	} else {
	loc_82452F28:
		// cmpwi cr6,r11,1
		// beq cr6,0x82452fc4
		if (ctx.r11.s32 != 1) {
			// cmpwi cr6,r11,2
			// bne cr6,0x8245305c
			if (ctx.r11.s32 != 2) {
				// blr
				return;
			}
			// lwz r7,28(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 28);
			// lwz r11,24(r7)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 24);
			// lwz r3,36(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
			// cmplwi cr6,r3,0
			// beq cr6,0x8245305c
			if (ctx.r3.u32 == 0) {
				// blr
				return;
			}
			// lwz r5,112(r6)
			// bctrl
			VCALL(ctx.r3.u32, 28, ctx, base);  // vtable slot 28 (byte +112)
			// cmpwi cr6,r3,0
			// beq cr6,0x8245305c
			if (ctx.r3.s32 == 0) {
				// blr
				return;
			}
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// lwz r4,28(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 28);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// lwz r11,24(r4)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
			// lwz r3,36(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
			// cmplwi cr6,r3,0
			// beq cr6,0x82452f94
			if (ctx.r3.u32 != 0) {
				// lwz r10,112(r11)
				// bctrl
				VCALL(ctx.r3.u32, 28, ctx, base);  // vtable slot 28 (byte +112)
				// b 0x82452f98
			} else {
			loc_82452F94:
				// li r3,0
				ctx.r3.s64 = 0;
			}
		loc_82452F98:
			// subf r9,r3,r30
			ctx.r9.s64 = (int64_t)(int32_t)var_r30 - ctx.r3.s64;
			// lwz r8,0(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* rage_GameObject::vtable@+0x0 */;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// extsw r7,r9
			ctx.r7.s64 = ctx.r9.s32;
			// lwz r6,0(r8)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r8.u32 + 0);
			// std r7,80(r1)
			PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
			// lfd f0,80(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
			// fcfid f13,f0
			ctx.f13.f64 = double(ctx.f0.s64);
			// frsp f1,f13
			ctx.f1.f64 = double(float(ctx.f13.f64));
			// mtctr r6
			ctx.ctr.u64 = ctx.r6.u64;
			// b 0x82453058
		} else {
		loc_82452FC4:
			// lwz r5,44(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 44);
			// cmpwi cr6,r5,0
			// beq cr6,0x8245305c
			if (ctx.r5.s32 == 0) {
				// blr
				return;
			}
			// lwz r4,28(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 28);
			// lwz r11,24(r4)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
			// lwz r3,36(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
			// cmplwi cr6,r3,0
			// beq cr6,0x8245305c
			if (ctx.r3.u32 == 0) {
				// blr
				return;
			}
			// lwz r10,108(r11)
			// bctrl
			VCALL(ctx.r3.u32, 27, ctx, base);  // vtable slot 27 (byte +108)
			// cmpwi cr6,r3,0
			// beq cr6,0x8245305c
			if (ctx.r3.s32 == 0) {
				// blr
				return;
			}
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// lwz r9,28(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 28);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// lwz r11,24(r9)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r9.u32 + 24);
			// lwz r3,36(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
			// cmplwi cr6,r3,0
			// beq cr6,0x8245302c
			if (ctx.r3.u32 != 0) {
				// lwz r7,108(r8)
				// bctrl
				VCALL(ctx.r3.u32, 27, ctx, base);  // vtable slot 27 (byte +108)
				// b 0x82453030
			} else {
			loc_8245302C:
				// li r3,0
				ctx.r3.s64 = 0;
			}
		loc_82453030:
			// subf r6,r3,r30
			ctx.r6.s64 = (int64_t)(int32_t)var_r30 - ctx.r3.s64;
			// lwz r5,0(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* rage_GameObject::vtable@+0x0 */;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// extsw r4,r6
			ctx.r4.s64 = ctx.r6.s32;
			// lwz r11,0(r5)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
			// std r4,80(r1)
			PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r4.u64);
			// lfd f12,80(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f12.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
			// fcfid f11,f12
			ctx.f11.f64 = double(ctx.f12.s64);
			// frsp f1,f11
			ctx.f1.f64 = double(float(ctx.f11.f64));
			// mtctr r11
			ctx.ctr.u64 = ctx.r11.u64;
		}
	}
loc_82453058:
	// bctrl
	ctx.lr = 0x8245305C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_8245305C:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_3078_p39"))) PPC_WEAK_FUNC(atSingleton_3078_p39);
PPC_FUNC_IMPL(__imp__atSingleton_3078_p39) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,20(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// lhz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,1
	// beq cr6,0x82453090
	if (ctx.r11.u32 != 1) {
		// cmplwi cr6,r11,2
		// bnelr cr6
		if (ctx.r11.u32 != 2) return;
	}
loc_82453090:
	// b 0x82452ee0
	rage_2EE0(ctx, base);
	return;
}

__attribute__((alias("__imp__sub_82453094"))) PPC_WEAK_FUNC(sub_82453094);
PPC_FUNC_IMPL(__imp__sub_82453094) {
	PPC_FUNC_PROLOGUE();
	// blr
	return;
}

__attribute__((alias("__imp__rage_3098"))) PPC_WEAK_FUNC(rage_3098);
PPC_FUNC_IMPL(__imp__rage_3098) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32256
	// addi r11,r3,4
	ctx.r11.s64 = ctx.r3.s64 + 4;
	// li r6,1
	ctx.r6.s64 = 1;
	// lfs f0,15784(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15784);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	// stw r11,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
	// addi r9,r10,15816
	ctx.r9.s64 = ctx.r10.s64 + 15816;
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// addi r8,r10,19020
	ctx.r8.s64 = ctx.r10.s64 + 19020;
	// lis r10,-32256
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// stfs f0,36(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 36, temp.u32);
	// addi r7,r10,19016
	ctx.r7.s64 = ctx.r10.s64 + 19016;
	// stw r6,44(r3)
	PPC_STORE_U32(ctx.r3.u32 + 44, ctx.r6.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r8,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r8.u32);
	// stw r7,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r7.u32);
	// stw r10,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r10.u32);
	// stw r10,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r10.u32);
	// stw r10,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r10.u32);
	// stw r10,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r10.u32);
	// stw r10,32(r3)
	PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r10.u32);
	// stw r10,40(r3)
	PPC_STORE_U32(ctx.r3.u32 + 40, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_5"))) PPC_WEAK_FUNC(game_vt4A34_5);
PPC_FUNC_IMPL(__imp__game_vt4A34_5) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-4
	ctx.r3.s64 = ctx.r3.s64 + -4;
	// b 0x8244bc40
	msgMsgSink_BC40(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_3108_p39"))) PPC_WEAK_FUNC(atSingleton_3108_p39);
PPC_FUNC_IMPL(__imp__atSingleton_3108_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// addi r11,r11,19020
	ctx.r11.s64 = ctx.r11.s64 + 19020;
	// addi r10,r10,19016
	ctx.r10.s64 = ctx.r10.s64 + 19016;
	// lwz r9,24(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r9,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ ctx.r10.u32);
	// beq cr6,0x82453178
	if (ctx.r9.u32 != 0) {
		// lwz r4,228(r9)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r9.u32 + 228);
		// cmplwi cr6,r4,0
		// beq cr6,0x82453178
		if (ctx.r4.u32 == 0) {
			// lis r11,-32256
			// addi r11,r11,15816
			ctx.r11.s64 = ctx.r11.s64 + 15816;
			// stw r11,4(r31)
			PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ ctx.r11.u32);
			// blr
			return;
		}
	loc_8245314C:
		// lwz r11,0(r4)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
		// cmplw cr6,r11,r31
		// beq cr6,0x82453168
		if (ctx.r11.u32 != var_r31) {
			// lwz r4,4(r4)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
			// cmplwi cr6,r4,0
			// bne cr6,0x8245314c
			if (ctx.r4.u32 != 0) goto loc_8245314C;
			// b 0x82453178
		} else {
		loc_82453168:
			// cmplwi cr6,r4,0
			// beq cr6,0x82453178
			if (ctx.r4.u32 == 0) {
				// lis r11,-32256
				// addi r11,r11,15816
				ctx.r11.s64 = ctx.r11.s64 + 15816;
				// stw r11,4(r31)
				PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ ctx.r11.u32);
				// blr
				return;
			}
			// addi r3,r9,24
			ctx.r3.s64 = ctx.r9.s64 + 24;
			// bl 0x8244ef40
			ph_EF40(ctx, base);
		}
	}
loc_82453178:
	// lis r11,-32256
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__rage_3198"))) PPC_WEAK_FUNC(rage_3198);
PPC_FUNC_IMPL(__imp__rage_3198) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r29,0
	var_r29 = 0;
	// stw r4,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r4.u32);
	// stw r6,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r6.u32);
	// stw r5,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r5.u32);
	// lhz r4,0(r6)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r6.u32 + 0);
	// bl 0x82448d10
	msgMsgSink_8D10_g(ctx, base);
	// lbz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// rlwinm r10,r11,0,29,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r10,0
	// beq cr6,0x8245321c
	if (ctx.r10.u32 != 0) {
		// lwz r9,20(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
		// addi r30,r31,24
		var_r30 = (uint32_t)(var_r31 + 24);
		// lwz r8,32(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 32);
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// lhz r4,0(r9)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r9.u32 + 0);
		// lwz r3,48(r8)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 48);
		// bl 0x8244ec28
		msgMsgSink_EC28_g(ctx, base);
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// cmpwi cr6,r29,0
		// blt cr6,0x82453210
		if ((int32_t)var_r29 >= 0) {
		loc_82453200:
			// lwz r11,0(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 0)/* rage_GameObject::vtable@+0x0 */;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// addi r3,r11,24
			ctx.r3.s64 = ctx.r11.s64 + 24;
			// bl 0x8244c1a8
			rage_C1A8(ctx, base);
		}
	loc_82453210:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
loc_8245321C:
	// lwz r7,20(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r3,16(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lhz r4,0(r7)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r7.u32 + 0);
	// bl 0x82448c60
	ph_8C60(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// cmplwi cr6,r3,0
	// stw r3,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r3.u32);
	// bne cr6,0x82453200
	if (ctx.r3.u32 != 0) goto loc_82453200;
	// lis r3,-30009
	// ori r3,r3,10
	ctx.r3.u64 = ctx.r3.u64 | 10;
	return;
}

__attribute__((alias("__imp__rage_3250"))) PPC_WEAK_FUNC(rage_3250);
PPC_FUNC_IMPL(__imp__rage_3250) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r29,0
	var_r29 = 0;
	// stw r4,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r4.u32);
	// stw r6,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r6.u32);
	// stw r5,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r5.u32);
	// lhz r4,0(r6)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r6.u32 + 0);
	// bl 0x82448d10
	msgMsgSink_8D10_g(ctx, base);
	// lbz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// rlwinm r10,r11,0,29,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r10,0
	// beq cr6,0x824532d8
	if (ctx.r10.u32 != 0) {
		// lwz r9,28(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 28);
		// addi r30,r31,24
		var_r30 = (uint32_t)(var_r31 + 24);
		// lwz r8,20(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// lwz r7,20(r9)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
		// lhz r4,0(r8)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r8.u32 + 0);
		// lwz r3,48(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 48);
		// bl 0x8244ec28
		msgMsgSink_EC28_g(ctx, base);
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// cmpwi cr6,r29,0
		// blt cr6,0x824532cc
		if ((int32_t)var_r29 >= 0) {
		loc_824532BC:
			// lwz r11,0(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 0)/* rage_GameObject::vtable@+0x0 */;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// addi r3,r11,24
			ctx.r3.s64 = ctx.r11.s64 + 24;
			// bl 0x8244c1a8
			rage_C1A8(ctx, base);
		}
	loc_824532CC:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
loc_824532D8:
	// lwz r6,20(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r3,16(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lhz r4,0(r6)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r6.u32 + 0);
	// bl 0x82448c60
	ph_8C60(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// cmplwi cr6,r3,0
	// stw r3,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r3.u32);
	// bne cr6,0x824532bc
	if (ctx.r3.u32 != 0) goto loc_824532BC;
	// lis r3,-30009
	// ori r3,r3,10
	ctx.r3.u64 = ctx.r3.u64 | 10;
	return;
}

__attribute__((alias("__imp__msgMsgSink_3308_w"))) PPC_WEAK_FUNC(msgMsgSink_3308_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_3308_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r30,0
	var_r30 = 0;
	// stw r4,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r4.u32);
	// stw r5,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r5.u32);
	// lhz r4,0(r5)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r5.u32 + 0);
	// bl 0x82448d10
	msgMsgSink_8D10_g(ctx, base);
	// lbz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// rlwinm r10,r11,0,29,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r10,0
	// bne cr6,0x82453370
	if (ctx.r10.u32 == 0) {
		// lwz r9,20(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lhz r4,0(r9)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r9.u32 + 0);
		// bl 0x82448c60
		ph_8C60(ctx, base);
		// cmplwi cr6,r3,0
		// stw r3,24(r31)
		PPC_STORE_U32(var_r31 + 24, ctx.r3.u32);
		// bne cr6,0x82453370
		if (ctx.r3.u32 != 0) goto loc_82453370;
		// lis r3,-30009
		// ori r3,r3,10
		ctx.r3.u64 = ctx.r3.u64 | 10;
		// b 0x82453384
	} else {
	loc_82453370:
		// lwz r11,24(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,24
		ctx.r3.s64 = ctx.r11.s64 + 24;
		// bl 0x8244c1a8
		rage_C1A8(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_82453384:
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_8"))) PPC_WEAK_FUNC(game_vt4A34_8);
PPC_FUNC_IMPL(__imp__game_vt4A34_8) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// b 0x82460e20
	game_0E20(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_9"))) PPC_WEAK_FUNC(game_vt4A34_9);
PPC_FUNC_IMPL(__imp__game_vt4A34_9) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// b 0x82460e98
	phInst_0E98_p28(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_11"))) PPC_WEAK_FUNC(game_vt4A34_11);
PPC_FUNC_IMPL(__imp__game_vt4A34_11) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x82460c08
	xam_0C08_g(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_12"))) PPC_WEAK_FUNC(game_vt4A34_12);
PPC_FUNC_IMPL(__imp__game_vt4A34_12) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x824533d0
	if (ctx.r4.s32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_824533D0:
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82460c58
	phInst_0C58(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_13"))) PPC_WEAK_FUNC(game_vt4A34_13);
PPC_FUNC_IMPL(__imp__game_vt4A34_13) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// b 0x82460cb0
	phInst_0CB0_p28(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_19"))) PPC_WEAK_FUNC(game_vt4A34_19);
PPC_FUNC_IMPL(__imp__game_vt4A34_19) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x82460dc8
	phInst_0DC8_p28(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_33F8_wrh"))) PPC_WEAK_FUNC(msgMsgSink_33F8_wrh);
PPC_FUNC_IMPL(__imp__msgMsgSink_33F8_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=160, savegprlr_27
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// addi r31,r29,4
	var_r31 = (uint32_t)(var_r29 + 4);
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r27,r6
	var_r27 = ctx.r6.u32;
	// mr r28,r7
	var_r28 = ctx.r7.u32;
	// bl 0x82461278
	game_1278(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x824534a0
	if (ctx.r3.s32 >= 0) {
		// cmplwi cr6,r28,0
		// beq cr6,0x824534a0
		if (var_r28 == 0) {
			return;
		}
		// lwz r3,0(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r30,0
		var_r30 = 0;
		// stw r30,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, var_r30);
		// stw r30,100(r1)
		PPC_STORE_U32(ctx.r1.u32 + 100, var_r30);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// li r9,2
		ctx.r9.s64 = 2;
		// addi r8,r1,96
		ctx.r8.s64 = ctx.r1.s64 + 96;
		// lwz r11,0(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// stw r3,104(r1)
		PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r3.u32);
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// stw r30,108(r1)
		PPC_STORE_U32(ctx.r1.u32 + 108, var_r30);
		// stb r9,80(r1)
		PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r9.u8);
		// stw r8,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r8.u32);
		// bl 0x824609f0
		phInst_09F0_p28(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x824534a0
		if (ctx.r3.s32 < 0) {
			return;
		}
		// lwz r7,0(r29)
		ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// mr r4,r27
		ctx.r4.u64 = var_r27;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lwz r6,44(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 44);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
	}
loc_824534A0:
	return;
}

__attribute__((alias("__imp__game_vt4A34_7"))) PPC_WEAK_FUNC(game_vt4A34_7);
PPC_FUNC_IMPL(__imp__game_vt4A34_7) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// bl 0x82460aa0
	phInst_0AA0_v12(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x824534e0
	if (ctx.r3.s32 >= 0) {
		// lbz r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
		// not r10,r11
		ctx.r10.u64 = ~ctx.r11.u64;
		// clrlwi r9,r10,31
		ctx.r9.u64 = ctx.r10.u32 & 0x1;
		// stw r9,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r9.u32);
	}
loc_824534E0:
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_10"))) PPC_WEAK_FUNC(game_vt4A34_10);
PPC_FUNC_IMPL(__imp__game_vt4A34_10) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=176, manual
	// lwz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// addi r11,r4,12
	ctx.r11.s64 = ctx.r4.s64 + 12;
	// lwz r9,4(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
	// stw r10,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
	// addi r10,r1,92
	ctx.r10.s64 = ctx.r1.s64 + 92;
	// stw r9,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// li r9,18
	ctx.r9.s64 = 18;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_82453524:
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// stw r8,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r8.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x82453524
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82453524;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// cmplwi cr6,r11,255
	// bne cr6,0x82453550
	if (ctx.r11.u32 == 255) {
		// li r7,-1
		ctx.r7.s64 = -1;
		// stw r7,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r7.u32);
		// b 0x82453554
	} else {
	loc_82453550:
		// stw r11,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
	}
loc_82453554:
	// lwz r6,84(r4)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r4.u32 + 84);
	// cmpwi cr6,r5,0
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r6,164(r1)
	PPC_STORE_U32(ctx.r1.u32 + 164, ctx.r6.u32);
	// bne cr6,0x8245356c
	if (ctx.r5.s32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_8245356C:
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// bl 0x82460b50
	xam_0B50_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_15"))) PPC_WEAK_FUNC(game_vt4A34_15);
PPC_FUNC_IMPL(__imp__game_vt4A34_15) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=112, manual
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// li r10,1
	ctx.r10.s64 = 1;
	// addi r9,r1,88
	ctx.r9.s64 = ctx.r1.s64 + 88;
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stb r10,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r10.u8);
	// stw r9,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// stw r8,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r8.u32);
	// bl 0x824609f0
	phInst_09F0_p28(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_17"))) PPC_WEAK_FUNC(game_vt4A34_17);
PPC_FUNC_IMPL(__imp__game_vt4A34_17) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=112, manual
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// li r10,1
	ctx.r10.s64 = 1;
	// addi r9,r1,88
	ctx.r9.s64 = ctx.r1.s64 + 88;
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stb r10,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r10.u8);
	// stw r9,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// stb r8,88(r1)
	PPC_STORE_U8(ctx.r1.u32 + 88, ctx.r8.u8);
	// bl 0x82460a48
	phBoundCapsule_0A48(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_14"))) PPC_WEAK_FUNC(game_vt4A34_14);
PPC_FUNC_IMPL(__imp__game_vt4A34_14) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// b 0x824609f0
	phInst_09F0_p28(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_16"))) PPC_WEAK_FUNC(game_vt4A34_16);
PPC_FUNC_IMPL(__imp__game_vt4A34_16) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// b 0x82460a48
	phBoundCapsule_0A48(ctx, base);
	return;
}

__attribute__((alias("__imp__game_vt4A34_18"))) PPC_WEAK_FUNC(game_vt4A34_18);
PPC_FUNC_IMPL(__imp__game_vt4A34_18) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// FRAME: size=144, manual
	// li r11,0
	ctx.r11.s64 = 0;
	// lfs f0,0(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// li r10,2
	ctx.r10.s64 = 2;
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// addi r9,r1,112
	ctx.r9.s64 = ctx.r1.s64 + 112;
	// stfs f0,116(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// addi r8,r1,80
	ctx.r8.s64 = ctx.r1.s64 + 80;
	// stfs f0,124(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 124, temp.u32);
	// addi r7,r1,88
	ctx.r7.s64 = ctx.r1.s64 + 88;
	// stb r11,112(r1)
	PPC_STORE_U8(ctx.r1.u32 + 112, ctx.r11.u8);
	// li r11,1
	ctx.r11.s64 = 1;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// stb r10,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r10.u8);
	// stw r9,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// stw r8,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r8.u32);
	// stw r7,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r7.u32);
	// stb r11,120(r1)
	PPC_STORE_U8(ctx.r1.u32 + 120, ctx.r11.u8);
	// stb r11,88(r1)
	PPC_STORE_U8(ctx.r1.u32 + 88, ctx.r11.u8);
	// stb r11,96(r1)
	PPC_STORE_U8(ctx.r1.u32 + 96, ctx.r11.u8);
	// bl 0x82460a48
	phBoundCapsule_0A48(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt4A34_20"))) PPC_WEAK_FUNC(game_vt4A34_20);
PPC_FUNC_IMPL(__imp__game_vt4A34_20) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r11,r11,19024
	ctx.r11.s64 = ctx.r11.s64 + 19024;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x824536d8
	if (ctx.r3.u32 != 0) {
		// bl 0x824608b8
		phInst_08B8(ctx, base);
	}
loc_824536D8:
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824536f8
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32810
		ctx.r4.u64 = ctx.r4.u64 | 32810;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824536F8:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_3710_w"))) PPC_WEAK_FUNC(msgMsgSink_3710_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_3710_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r25 = 0;
	// FRAME: size=144, savegprlr_25
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// ori r4,r4,32810
	ctx.r4.u64 = ctx.r4.u64 | 32810;
	// li r3,8
	ctx.r3.s64 = 8;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r27,r6
	var_r27 = ctx.r6.u32;
	// mr r26,r7
	var_r26 = ctx.r7.u32;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r25,0
	var_r25 = 0;
	// cmplwi cr6,r31,0
	// beq cr6,0x8245378c
	if (var_r31 != 0) {
		// lis r11,-32256
		// stw r25,4(r31)
		PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ var_r25);
		// mr r7,r27
		ctx.r7.u64 = var_r27;
		// addi r11,r11,19024
		ctx.r11.s64 = ctx.r11.s64 + 19024;
		// mr r6,r28
		ctx.r6.u64 = var_r28;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
		// bl 0x824533f8
		msgMsgSink_33F8_wrh(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x824537a4
		if ((int32_t)var_r30 < 0) goto loc_824537A4;
		// stw r31,0(r26)
		PPC_STORE_U32(var_r26 + 0, var_r31);
		return;
	}
loc_8245378C:
	// lis r30,-32761
	var_r30 = (uint32_t)(-2147024896);
	// stw r25,0(r26)
	PPC_STORE_U32(var_r26 + 0, var_r25);
	// ori r30,r30,14
	var_r30 = (uint32_t)(var_r30 | 14);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
loc_824537A4:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,52(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// stw r25,0(r26)
	PPC_STORE_U32(var_r26 + 0, var_r25);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_37D0_w"))) PPC_WEAK_FUNC(msgMsgSink_37D0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_37D0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// ori r4,r4,32811
	ctx.r4.u64 = ctx.r4.u64 | 32811;
	// li r3,8
	ctx.r3.s64 = 8;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r29,0
	var_r29 = 0;
	// cmplwi cr6,r31,0
	// beq cr6,0x82453838
	if (var_r31 != 0) {
		// lis r11,-32256
		// addi r4,r31,4
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,19080
		ctx.r11.s64 = ctx.r11.s64 + 19080;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r29,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r29);
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
		// bl 0x824612d8
		game_12D8(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x82453850
		if ((int32_t)var_r30 < 0) goto loc_82453850;
		// stw r31,0(r28)
		PPC_STORE_U32(var_r28 + 0, var_r31);
		return;
	}
loc_82453838:
	// lis r30,-32761
	var_r30 = (uint32_t)(-2147024896);
	// stw r29,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r29);
	// ori r30,r30,14
	var_r30 = (uint32_t)(var_r30 | 14);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
loc_82453850:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// stw r29,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r29);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__game_vt4A34_21"))) PPC_WEAK_FUNC(game_vt4A34_21);
PPC_FUNC_IMPL(__imp__game_vt4A34_21) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r23 = 0;
	PPCRegister temp{};
	// FRAME: size=224, savegprlr_23
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lis r11,-32256
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r7,r1,80
	ctx.r7.s64 = ctx.r1.s64 + 80;
	// li r6,1
	ctx.r6.s64 = 1;
	// lfs f13,84(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 84);
	ctx.f13.f64 = double(temp.f32);
	// li r5,2
	ctx.r5.s64 = 2;
	// lfs f0,18992(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18992);
	ctx.f0.f64 = double(temp.f32);
	// li r4,0
	ctx.r4.s64 = 0;
	// fmuls f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// stfs f12,80(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// bl 0x82460980
	game_0980(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x82453990
	if (ctx.r3.s32 >= 0) {
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lfs f11,52(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 52);
		ctx.f11.f64 = double(temp.f32);
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lfs f10,56(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 56);
		ctx.f10.f64 = double(temp.f32);
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lfs f9,60(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 60);
		ctx.f9.f64 = double(temp.f32);
		// lwz r9,12(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 12);
		// lfs f8,64(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 64);
		ctx.f8.f64 = double(temp.f32);
		// lwz r8,16(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lfs f7,68(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 68);
		ctx.f7.f64 = double(temp.f32);
		// lwz r30,20(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
		// lfs f6,72(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 72);
		ctx.f6.f64 = double(temp.f32);
		// lwz r29,24(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 24));
		// lfs f5,76(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 76);
		ctx.f5.f64 = double(temp.f32);
		// lwz r28,28(r31)
		var_r28 = (uint32_t)(PPC_LOAD_U32(var_r31 + 28));
		// lfs f4,80(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 80);
		ctx.f4.f64 = double(temp.f32);
		// lwz r27,32(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// addi r7,r1,80
		ctx.r7.s64 = ctx.r1.s64 + 80;
		// lwz r26,36(r31)
		var_r26 = (uint32_t)(PPC_LOAD_U32(var_r31 + 36));
		// li r6,3
		ctx.r6.s64 = 3;
		// lwz r25,40(r31)
		var_r25 = (uint32_t)(PPC_LOAD_U32(var_r31 + 40));
		// li r5,0
		ctx.r5.s64 = 0;
		// lwz r24,44(r31)
		var_r24 = (uint32_t)(PPC_LOAD_U32(var_r31 + 44));
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r23,48(r31)
		var_r23 = (uint32_t)(PPC_LOAD_U32(var_r31 + 48));
		// stfs f11,112(r1)
		temp.f32 = float(ctx.f11.f64);
		PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
		// stb r11,100(r1)
		PPC_STORE_U8(ctx.r1.u32 + 100, ctx.r11.u8);
		// addi r11,r1,96
		ctx.r11.s64 = ctx.r1.s64 + 96;
		// stb r10,102(r1)
		PPC_STORE_U8(ctx.r1.u32 + 102, ctx.r10.u8);
		// li r10,48
		ctx.r10.s64 = 48;
		// lwz r31,0(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 0));
		// stfs f10,116(r1)
		temp.f32 = float(ctx.f10.f64);
		PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
		// stfs f9,120(r1)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
		// stb r9,103(r1)
		PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r9.u8);
		// stfs f8,124(r1)
		temp.f32 = float(ctx.f8.f64);
		PPC_STORE_U32(ctx.r1.u32 + 124, temp.u32);
		// stb r8,104(r1)
		PPC_STORE_U8(ctx.r1.u32 + 104, ctx.r8.u8);
		// stfs f7,128(r1)
		temp.f32 = float(ctx.f7.f64);
		PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
		// stb r30,105(r1)
		PPC_STORE_U8(ctx.r1.u32 + 105, (uint8_t)var_r30);
		// stfs f6,132(r1)
		temp.f32 = float(ctx.f6.f64);
		PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
		// stb r29,106(r1)
		PPC_STORE_U8(ctx.r1.u32 + 106, (uint8_t)var_r29);
		// stfs f5,136(r1)
		temp.f32 = float(ctx.f5.f64);
		PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
		// stw r31,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, var_r31);
		// stfs f4,140(r1)
		temp.f32 = float(ctx.f4.f64);
		PPC_STORE_U32(ctx.r1.u32 + 140, temp.u32);
		// stb r28,107(r1)
		PPC_STORE_U8(ctx.r1.u32 + 107, (uint8_t)var_r28);
		// stb r27,108(r1)
		PPC_STORE_U8(ctx.r1.u32 + 108, (uint8_t)var_r27);
		// stb r26,109(r1)
		PPC_STORE_U8(ctx.r1.u32 + 109, (uint8_t)var_r26);
		// stb r25,110(r1)
		PPC_STORE_U8(ctx.r1.u32 + 110, (uint8_t)var_r25);
		// stb r24,111(r1)
		PPC_STORE_U8(ctx.r1.u32 + 111, (uint8_t)var_r24);
		// stb r23,101(r1)
		PPC_STORE_U8(ctx.r1.u32 + 101, (uint8_t)var_r23);
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// stw r10,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r10.u32);
		// bl 0x82460980
		game_0980(ctx, base);
	}
loc_82453990:
	return;
}

__attribute__((alias("__imp__game_vt4A34_23"))) PPC_WEAK_FUNC(game_vt4A34_23);
PPC_FUNC_IMPL(__imp__game_vt4A34_23) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r11,r11,19080
	ctx.r11.s64 = ctx.r11.s64 + 19080;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x824539d0
	if (ctx.r3.u32 != 0) {
		// bl 0x824608b8
		phInst_08B8(ctx, base);
	}
loc_824539D0:
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824539f0
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32811
		ctx.r4.u64 = ctx.r4.u64 | 32811;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824539F0:
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_3A08_2h"))) PPC_WEAK_FUNC(snNotifyGamerAdded_3A08_2h);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_3A08_2h) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r10,r11,19092
	ctx.r10.s64 = ctx.r11.s64 + 19092;
	// lis r11,-32256
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r10.u32);
	// lfs f0,15788(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);  /* glob:lbl_82003DAC @ 0x82003dac */
	ctx.f0.f64 = double(temp.f32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stfs f0,4(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// stfs f0,20(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// sth r11,8(r3)
	PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
	// stw r11,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r11.u32);
	// stw r11,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_3A38_g"))) PPC_WEAK_FUNC(msgMsgSink_3A38_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3A38_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// addi r11,r11,19092
	ctx.r11.s64 = ctx.r11.s64 + 19092;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_3A48"))) PPC_WEAK_FUNC(jumptable_3A48);
PPC_FUNC_IMPL(__imp__jumptable_3A48) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// cmpwi cr6,r4,1
	// stw r4,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r4.u32);
	// beq cr6,0x82453a78
	if (ctx.r4.s32 != 1) {
		// cmpwi cr6,r4,2
		// bnelr cr6
		if (ctx.r4.s32 != 2) return;
		// lis r11,-32256
		// stw r6,12(r3)
		PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r6.u32);
		// sth r5,8(r3)
		PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r5.u16);
		// lfs f0,15784(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,20(r3)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
		// stfs f0,4(r3)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
		// blr
		return;
	}
loc_82453A78:
	// lis r11,-32256
	// stw r6,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r6.u32);
	// sth r5,8(r3)
	PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r5.u16);
	// lfs f0,15788(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,20(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// stfs f0,4(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__phBoundCapsule_3A98_g"))) PPC_WEAK_FUNC(phBoundCapsule_3A98_g);
PPC_FUNC_IMPL(__imp__phBoundCapsule_3A98_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 12);
	// cmpwi cr6,r11,1
	// beq cr6,0x82453b0c
	if (ctx.r11.s32 != 1) {
		// cmpwi cr6,r11,2
		// beq cr6,0x82453adc
		if (ctx.r11.s32 != 2) {
			// lfs f0,20(r31)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r31 + 20);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,4(r31)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(var_r31 + 4,/* phBoundCapsule::flags@+0x4 */ temp.u32);
			// blr
			return;
		}
	loc_82453ADC:
		// lis r11,-32256
		// lfs f13,20(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 20);
		ctx.f13.f64 = double(temp.f32);
		// lfd f0,19104(r11)
		ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 19104);
		// fmul f1,f13,f0
		ctx.f1.f64 = ctx.f13.f64 * ctx.f0.f64;
		// bl 0x824301d8
		phBoundCapsule_01D8_g(ctx, base);
		// frsp f12,f1
		ctx.fpscr.disableFlushMode();
		ctx.f12.f64 = double(float(ctx.f1.f64));
		// stfs f12,4(r31)
		temp.f32 = float(ctx.f12.f64);
		PPC_STORE_U32(var_r31 + 4,/* phBoundCapsule::flags@+0x4 */ temp.u32);
		// blr
		return;
	}
loc_82453B0C:
	// lis r11,-32248
	// lfs f2,20(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 20);
	ctx.f2.f64 = double(temp.f32);
	// lfd f1,-25752(r11)
	ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -25752);
	// bl 0x82431308
	atSingleton_1308_g(ctx, base);
	// frsp f11,f1
	ctx.fpscr.disableFlushMode();
	ctx.f11.f64 = double(float(ctx.f1.f64));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,19100(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 19100);  /* glob:lbl_82004A9C @ 0x82004a9c */
	ctx.f0.f64 = double(temp.f32);
	// fmuls f10,f11,f0
	ctx.f10.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// stfs f10,4(r31)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(var_r31 + 4,/* phBoundCapsule::flags@+0x4 */ temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_3B48_g"))) PPC_WEAK_FUNC(msgMsgSink_3B48_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3B48_g) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// FRAME: size=96, manual
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// cmpwi cr6,r11,2
	// bne cr6,0x82453bec
	if (ctx.r11.s32 == 2) {
		// lhz r11,8(r3)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 8);
		// lfs f13,20(r3)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		ctx.f13.f64 = double(temp.f32);
		// extsw r10,r11
		ctx.r10.s64 = ctx.r11.s32;
		// lis r11,-32256
		// std r10,80(r1)
		PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r10.u64);
		// lfs f0,19112(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 19112);
		ctx.f0.f64 = double(temp.f32);
		// lis r11,-32256
		// lfd f12,80(r1)
		ctx.f12.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// fcfid f11,f12
		ctx.f11.f64 = double(ctx.f12.s64);
		// frsp f10,f11
		ctx.f10.f64 = double(float(ctx.f11.f64));
		// fdivs f9,f0,f10
		ctx.f9.f64 = double(float(ctx.f0.f64 / ctx.f10.f64));
		// fadds f0,f9,f13
		ctx.f0.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
		// lfs f13,15788(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
		ctx.f13.f64 = double(temp.f32);
		// stfs f0,20(r3)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
		// fcmpu cr6,f0,f13
		// blt cr6,0x82453bd4
		if (ctx.f0.f64 < ctx.f13.f64) {
			// bl 0x82453a98
			phBoundCapsule_3A98_g(ctx, base);
			// li r3,1
			ctx.r3.s64 = 1;
			// blr
			return;
		}
		// bso cr6,0x82453bd4
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x82453BA0, "bso");
		// lwz r9,0(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r8,0
		ctx.r8.s64 = 0;
		// stfs f13,4(r3)
		temp.f32 = float(ctx.f13.f64);
		PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ temp.u32);
		// lwz r7,4(r9)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
		// stw r8,16(r3)
		PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r8.u32);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r7.u32);
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	loc_82453BD4:
		// bl 0x82453a98
		phBoundCapsule_3A98_g(ctx, base);
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
loc_82453BEC:
	// cmpwi cr6,r11,1
	// bne cr6,0x82453c68
	if (ctx.r11.s32 != 1) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
	// lhz r6,8(r3)
	ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 8);
	// lis r11,-32256
	// lfs f8,20(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f8.f64 = double(temp.f32);
	// extsw r5,r6
	ctx.r5.s64 = ctx.r6.s32;
	// lfs f0,19112(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 19112);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	// std r5,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r5.u64);
	// lfs f13,15784(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	ctx.f13.f64 = double(temp.f32);
	// lfd f7,80(r1)
	ctx.f7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f6,f7
	ctx.f6.f64 = double(ctx.f7.s64);
	// frsp f5,f6
	ctx.f5.f64 = double(float(ctx.f6.f64));
	// fdivs f4,f0,f5
	ctx.f4.f64 = double(float(ctx.f0.f64 / ctx.f5.f64));
	// fsubs f0,f8,f4
	ctx.f0.f64 = double(float(ctx.f8.f64 - ctx.f4.f64));
	// stfs f0,20(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// fcmpu cr6,f0,f13
	// bgt cr6,0x82453bd4
	if (ctx.f0.f64 > ctx.f13.f64) {
		// bl 0x82453a98
		phBoundCapsule_3A98_g(ctx, base);
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
	// bso cr6,0x82453bd4
	// UNIMPLEMENTED: bso
	PPC_UNIMPLEMENTED(0x82453C34, "bso");
	// lwz r4,0(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r11,0
	ctx.r11.s64 = 0;
	// stfs f13,4(r3)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ temp.u32);
	// lwz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// stw r11,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// li r3,1
	ctx.r3.s64 = 1;
	// blr
	return;
}

__attribute__((alias("__imp__RtlLeaveCriticalSection_3C80_2hr"))) PPC_WEAK_FUNC(RtlLeaveCriticalSection_3C80_2hr);
PPC_FUNC_IMPL(__imp__RtlLeaveCriticalSection_3C80_2hr) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x824527c0
	atSingleton_27C0(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_3C88_g"))) PPC_WEAK_FUNC(msgMsgSink_3C88_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3C88_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// li r29,0
	var_r29 = 0;
	// lwz r11,80(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 80);
	// cmpwi cr6,r11,4
	// bgt cr6,0x82453d64
	if (ctx.r11.s32 <= 4) {
		// lbz r10,52(r30)
		ctx.r10.u64 = PPC_LOAD_U8(var_r30 + 52);
		// li r11,1
		ctx.r11.s64 = 1;
		// rlwinm r8,r10,0,26,26
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r8,32
		// beq cr6,0x82453cc4
		if (ctx.r8.u32 != 32) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_82453CC4:
		// cmpw cr6,r28,r11
		// beq cr6,0x82453d64
		if ((int32_t)var_r28 == ctx.r11.s32) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			return;
		}
		// cmpwi cr6,r28,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bne cr6,0x82453cdc
		if ((int32_t)var_r28 == 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_82453CDC:
		// rlwimi r10,r11,5,26,26
		ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 5) & 0x20) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFDF);
		// clrlwi r11,r10,24
		ctx.r11.u64 = ctx.r10.u32 & 0xFF;
		// rlwinm r6,r11,0,26,26
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r6,32
		// stb r11,52(r30)
		PPC_STORE_U8(var_r30 + 52, ctx.r11.u8);
		// bne cr6,0x82453d00
		if (ctx.r6.u32 == 32) {
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// stw r3,108(r30)
			PPC_STORE_U32(var_r30 + 108, ctx.r3.u32);
			// b 0x82453d38
		} else {
		loc_82453D00:
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// lwz r11,60(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 60);
			// lwz r5,108(r30)
			ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 108);
			// addi r9,r30,56
			ctx.r9.s64 = (int64_t)(int32_t)var_r30 + 56;
			// subf r7,r5,r3
			ctx.r7.s64 = ctx.r3.s64 - ctx.r5.s64;
			// cmplw cr6,r11,r9
			// beq cr6,0x82453d38
		while (!ctx.cr6.eq) {
			loc_82453D1C:
				// addi r10,r11,20
				ctx.r10.s64 = ctx.r11.s64 + 20;
				// lwz r11,4(r11)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
				// cmplw cr6,r11,r9
				ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r9.u32, ctx.xer);
				// lwz r8,0(r10)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
				// add r4,r8,r7
				ctx.r4.u64 = ctx.r8.u64 + ctx.r7.u64;
				// stw r4,0(r10)
				PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r4.u32);
				// bne cr6,0x82453d1c
		}
		}
	loc_82453D38:
		// lwz r31,40(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 40));
		// addi r30,r30,36
		var_r30 = (uint32_t)(var_r30 + 36);
	loc_82453D40:
		// cmplw cr6,r31,r30
		// beq cr6,0x82453d64
		if (var_r31 == var_r30) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			return;
		}
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459d70
		game_9D70_h(ctx, base);
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmpwi cr6,r29,0
		// bge cr6,0x82453d40
		if ((int32_t)var_r29 >= 0) goto loc_82453D40;
	}
loc_82453D64:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__msgMsgSink_3D70_p39"))) PPC_WEAK_FUNC(msgMsgSink_3D70_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_3D70_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r29,r3,36
	var_r29 = (uint32_t)(ctx.r3.s64 + 36);
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// cmplw cr6,r31,r29
	// beq cr6,0x82453db0
while (var_r31 != var_r29) {
	loc_82453D90:
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459b98
		atSingleton_9B98_p39(ctx, base);
		// cmpwi cr6,r3,0
		// bne cr6,0x82453dbc
		if (ctx.r3.s32 != 0) {
			// li r3,1
			ctx.r3.s64 = 1;
			return;
		}
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmplw cr6,r31,r29
		// bne cr6,0x82453d90
}
loc_82453DB0:
	// li r3,0
	ctx.r3.s64 = 0;
	return;
}

__attribute__((alias("__imp__msgMsgSink_3DC8_g"))) PPC_WEAK_FUNC(msgMsgSink_3DC8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3DC8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// cmplw cr6,r31,r30
	// beq cr6,0x82453e00
while (var_r31 != var_r30) {
	loc_82453DEC:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459c10
		game_9C10_h(ctx, base);
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmplw cr6,r31,r30
		// bne cr6,0x82453dec
}
loc_82453E00:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_3E18_2hr"))) PPC_WEAK_FUNC(atSingleton_3E18_2hr);
PPC_FUNC_IMPL(__imp__atSingleton_3E18_2hr) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// cmplw cr6,r31,r30
	// beq cr6,0x82453e50
while (var_r31 != var_r30) {
	loc_82453E3C:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459c90
		atSingleton_9C90_h(ctx, base);
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */);
		// cmplw cr6,r31,r30
		// bne cr6,0x82453e3c
}
loc_82453E50:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_3E68_g"))) PPC_WEAK_FUNC(msgMsgSink_3E68_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3E68_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// cmplw cr6,r31,r30
	// beq cr6,0x82453ea0
while (var_r31 != var_r30) {
	loc_82453E88:
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459cf8
		game_9CF8_h(ctx, base);
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmplw cr6,r31,r30
		// bne cr6,0x82453e88
}
loc_82453EA0:
	return;
}

__attribute__((alias("__imp__msgMsgSink_3EA8_g"))) PPC_WEAK_FUNC(msgMsgSink_3EA8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3EA8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// cmplw cr6,r31,r30
	// beq cr6,0x82453ee0
while (var_r31 != var_r30) {
	loc_82453EC8:
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459d10
		game_9D10_h(ctx, base);
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmplw cr6,r31,r30
		// bne cr6,0x82453ec8
}
loc_82453EE0:
	return;
}

__attribute__((alias("__imp__msgMsgSink_3EE8_g"))) PPC_WEAK_FUNC(msgMsgSink_3EE8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_3EE8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// cmplw cr6,r31,r30
	// beq cr6,0x82453f20
while (var_r31 != var_r30) {
	loc_82453F08:
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459d28
		game_9D28_h(ctx, base);
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmplw cr6,r31,r30
		// bne cr6,0x82453f08
}
loc_82453F20:
	return;
}

__attribute__((alias("__imp__game_3F28_h"))) PPC_WEAK_FUNC(game_3F28_h);
PPC_FUNC_IMPL(__imp__game_3F28_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// cmplw cr6,r31,r30
	// beq cr6,0x82453f60
while (var_r31 != var_r30) {
	loc_82453F48:
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459d40
		game_9D40_h(ctx, base);
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4));
		// cmplw cr6,r31,r30
		// bne cr6,0x82453f48
}
loc_82453F60:
	return;
}

__attribute__((alias("__imp__game_3F68_h"))) PPC_WEAK_FUNC(game_3F68_h);
PPC_FUNC_IMPL(__imp__game_3F68_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lwz r31,40(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 40));
	// addi r30,r3,36
	var_r30 = (uint32_t)(ctx.r3.s64 + 36);
	// cmplw cr6,r31,r30
	// beq cr6,0x82453fa8
while (var_r31 != var_r30) {
	loc_82453F8C:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82459d58
		game_9D58_h(ctx, base);
		// cmpwi cr6,r3,0
		// bne cr6,0x82453fc4
		if (ctx.r3.s32 != 0) {
			// li r3,1
			ctx.r3.s64 = 1;
			// b 0x82453fac
			// blr
			return;
		}
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4));
		// cmplw cr6,r31,r30
		// bne cr6,0x82453f8c
}
loc_82453FA8:
	// li r3,0
	ctx.r3.s64 = 0;
loc_82453FAC:
	// blr
	return;
}

__attribute__((alias("__imp__game_3FD0_h"))) PPC_WEAK_FUNC(game_3FD0_h);
PPC_FUNC_IMPL(__imp__game_3FD0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,48(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
	// lwz r10,116(r11)
	// bctrl
	VCALL(ctx.r3.u32, 29, ctx, base);  // vtable slot 29 (byte +116)
	// cmpwi cr6,r3,0
	// bne cr6,0x8245400c
	if (ctx.r3.s32 == 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r3,32(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
		// bl 0x82448a48
		game_8A48_h(ctx, base);
	}
loc_8245400C:
	// blr
	return;
}

__attribute__((alias("__imp__game_4020_h"))) PPC_WEAK_FUNC(game_4020_h);
PPC_FUNC_IMPL(__imp__game_4020_h) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,15784(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);  /* glob:lbl_82003DA8 @ 0x82003da8 */
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// blt cr6,0x8245403c
	if (ctx.f1.f64 >= ctx.f0.f64) {
		// bso cr6,0x8245403c
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x82454030, "bso");
		// stfs f1,112(r3)
		temp.f32 = float(ctx.f1.f64);
		PPC_STORE_U32(ctx.r3.u32 + 112, temp.u32);
		// b 0x82454040
	} else {
	loc_8245403C:
		// stfs f0,112(r3)
		ctx.fpscr.disableFlushMode();
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r3.u32 + 112, temp.u32);
	}
loc_82454040:
	// b 0x82453dc8
	msgMsgSink_3DC8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__phBoundCapsule_4048_fw"))) PPC_WEAK_FUNC(phBoundCapsule_4048_fw);
PPC_FUNC_IMPL(__imp__phBoundCapsule_4048_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,28(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r10,0,27,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r9,16
	// bne cr6,0x824540b4
	if (ctx.r9.u32 == 16) {
		// lis r11,-32256
		ctx.r11.s64 = -2113929216;
		// lfs f0,15784(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);  /* glob:lbl_82003DA8 @ 0x82003da8 */
		ctx.f0.f64 = double(temp.f32);
		// fcmpu cr6,f1,f0
		// blt cr6,0x82454088
		if (ctx.f1.f64 >= ctx.f0.f64) {
			// bso cr6,0x82454088
			// UNIMPLEMENTED: bso
			PPC_UNIMPLEMENTED(0x8245407C, "bso");
			// stfs f1,116(r30)
			temp.f32 = float(ctx.f1.f64);
			PPC_STORE_U32(var_r30 + 116, temp.u32);
			// b 0x8245408c
		} else {
		loc_82454088:
			// stfs f0,116(r30)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(var_r30 + 116, temp.u32);
		}
	loc_8245408C:
		// lwz r31,40(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 40));
		// addi r29,r30,36
		var_r29 = (uint32_t)(var_r30 + 36);
		// cmplw cr6,r31,r29
		// beq cr6,0x824540b4
	while (var_r31 != var_r29) {
		loc_8245409C:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lfs f1,116(r30)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r30 + 116);
			ctx.f1.f64 = double(temp.f32);
			// bl 0x82459c78
			phBoundCapsule_9C78_2hr(ctx, base);
			// lwz r31,4(r31)
			var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* phBoundCapsule::flags@+0x4 */);
			// cmplw cr6,r31,r29
			// bne cr6,0x8245409c
	}
	}
loc_824540B4:
	return;
}

__attribute__((alias("__imp__game_40C0_h"))) PPC_WEAK_FUNC(game_40C0_h);
PPC_FUNC_IMPL(__imp__game_40C0_h) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// stfs f1,120(r3)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r3.u32 + 120, temp.u32);
	// b 0x82453e18
	atSingleton_3E18_2hr(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_40C8_p39"))) PPC_WEAK_FUNC(msgMsgSink_40C8_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_40C8_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // msgMsgSink::vfn_4 (unnamed)  // vtable slot 4 (byte +16)
	// lwz r9,28(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 28);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// lbz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// rlwinm r7,r8,0,28,28
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r7,8
	// bne cr6,0x82454128
	if (ctx.r7.u32 == 8) {
		// lwz r6,0(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,16(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		// lbz r4,2(r3)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r3.u32 + 2);
		// rotlwi r11,r4,2
		ctx.r11.u64 = __builtin_rotateleft32(ctx.r4.u32, 2);
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// b 0x8245412c
	} else {
	loc_82454128:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_8245412C:
	// rlwinm r10,r30,2,22,29
	ctx.r10.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 2) & 0x3FC;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// add r3,r11,r29
	ctx.r3.u64 = ctx.r11.u64 + var_r29;
	// lwz r3,3(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 3);
	return;
}

__attribute__((alias("__imp__atSingleton_4148_g"))) PPC_WEAK_FUNC(atSingleton_4148_g);
PPC_FUNC_IMPL(__imp__atSingleton_4148_g) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lfs f0,120(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 120);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,92(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 92);
	ctx.f13.f64 = double(temp.f32);
	// fadds f1,f0,f13
	ctx.f1.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_4158_2hr"))) PPC_WEAK_FUNC(msgMsgSink_4158_2hr);
PPC_FUNC_IMPL(__imp__msgMsgSink_4158_2hr) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lwz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplwi cr6,r11,0
	// beq cr6,0x824541ac
	if (ctx.r11.u32 != 0) {
		// lwz r11,24(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
		// lwz r10,36(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// cmplwi cr6,r10,0
		// beq cr6,0x82454198
		if (ctx.r10.u32 != 0) {
			// rotlwi r3,r10,0
			ctx.r3.u64 = ctx.r10.u32;
			// lwz r8,48(r9)
			// bctrl
			VCALL(ctx.r3.u32, 12, ctx, base);  // vtable slot 12 (byte +48)
		}
	loc_82454198:
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// li r3,1
		ctx.r3.s64 = 1;
		// cmplwi cr6,r7,255
		// beq cr6,0x824541ac
		if (ctx.r7.u32 == 255) {
			// blr
			return;
		}
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_824541AC:
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_41C0_h"))) PPC_WEAK_FUNC(jumptable_41C0_h);
PPC_FUNC_IMPL(__imp__jumptable_41C0_h) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lwz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplwi cr6,r11,0
	// beq cr6,0x82454214
	if (ctx.r11.u32 != 0) {
		// lwz r11,24(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
		// lwz r10,36(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// cmplwi cr6,r10,0
		// beq cr6,0x82454200
		if (ctx.r10.u32 != 0) {
			// rotlwi r3,r10,0
			ctx.r3.u64 = ctx.r10.u32;
			// lwz r8,48(r9)
			// bctrl
			VCALL(ctx.r3.u32, 12, ctx, base);  // vtable slot 12 (byte +48)
		}
	loc_82454200:
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// li r3,1
		ctx.r3.s64 = 1;
		// cmplwi cr6,r7,0
		// bne cr6,0x82454214
		if (ctx.r7.u32 != 0) {
			// blr
			return;
		}
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_82454214:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_4228_p39"))) PPC_WEAK_FUNC(msgMsgSink_4228_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_4228_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// lwz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmplwi cr6,r11,0
	// beq cr6,0x824542b4
	if (ctx.r11.u32 != 0) {
		// lwz r11,24(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
		// li r3,0
		ctx.r3.s64 = 0;
		// lwz r10,36(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// cmplwi cr6,r10,0
		// beq cr6,0x82454278
		if (ctx.r10.u32 != 0) {
			// rotlwi r3,r10,0
			ctx.r3.u64 = ctx.r10.u32;
			// lwz r8,48(r9)
			// bctrl
			VCALL(ctx.r3.u32, 12, ctx, base);  // vtable slot 12 (byte +48)
		}
	loc_82454278:
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x824542b4
		if (ctx.r7.u32 == 0) goto loc_824542B4;
		// lwz r6,40(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 40);
		// li r3,0
		ctx.r3.s64 = 0;
		// lwz r11,24(r6)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r6.u32 + 24);
		// lwz r11,36(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// cmplwi cr6,r11,0
		// beq cr6,0x824542b8
		if (ctx.r11.u32 == 0) {
			// blr
			return;
		}
		// lwz r5,0(r11)
  // [ph4a] vtable load collapsed
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r11.u32, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// b 0x824542b8
	} else {
	loc_824542B4:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_824542B8:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_42D0_v12"))) PPC_WEAK_FUNC(msgMsgSink_42D0_v12);
PPC_FUNC_IMPL(__imp__msgMsgSink_42D0_v12) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// cmplwi cr6,r11,0
	// beqlr cr6
	if (ctx.r11.u32 == 0) return;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r3,36(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__atSingleton_4300_g"))) PPC_WEAK_FUNC(atSingleton_4300_g);
PPC_FUNC_IMPL(__imp__atSingleton_4300_g) {
	PPC_FUNC_PROLOGUE();
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lwz r3,48(r4)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 48);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// lwz r10,88(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__rage_4318"))) PPC_WEAK_FUNC(rage_4318);
PPC_FUNC_IMPL(__imp__rage_4318) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// lwz r9,28(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 28);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lbz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// rlwinm r7,r8,0,28,28
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r7,8
	// bne cr6,0x82454384
	if (ctx.r7.u32 == 8) {
		// lwz r6,0(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* rage_GameObject::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,16(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		// lbz r4,2(r3)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r3.u32 + 2);
		// rotlwi r11,r4,2
		ctx.r11.u64 = __builtin_rotateleft32(ctx.r4.u32, 2);
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// add r3,r11,r30
		ctx.r3.u64 = ctx.r11.u64 + var_r30;
		// lbz r3,2(r3)
		ctx.r3.u64 = PPC_LOAD_U8(ctx.r3.u32 + 2);
		// b 0x82454390
	} else {
	loc_82454384:
		// li r11,0
		ctx.r11.s64 = 0;
		// add r11,r11,r30
		ctx.r11.u64 = ctx.r11.u64 + var_r30;
		// lbz r3,2(r11)
		ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
	}
loc_82454390:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_43A8_g"))) PPC_WEAK_FUNC(msgMsgSink_43A8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_43A8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20144
	ctx.r11.s64 = ctx.r11.s64 + 20144;
	// lis r10,-32256
	// addi r28,r31,4
	var_r28 = (uint32_t)(var_r31 + 4);
	// addi r10,r10,20200
	ctx.r10.s64 = ctx.r10.s64 + 20200;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// lwz r11,84(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
	// stw r10,0(r28)
	PPC_STORE_U32(var_r28 + 0, ctx.r10.u32);
	// cmpwi cr6,r11,0
	// beq cr6,0x824543f8
	if (ctx.r11.s32 != 0) {
		// li r10,0
		ctx.r10.s64 = 0;
		// lwz r9,28(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 28);
		// lwz r3,32(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
		// stw r10,84(r31)
		PPC_STORE_U32(var_r31 + 84, ctx.r10.u32);
		// lhz r4,1(r9)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r9.u32 + 1);
		// bl 0x82448b90
		msgMsgSink_8B90_g(ctx, base);
	}
loc_824543F8:
	// lwz r3,40(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 40);
	// addi r29,r31,36
	var_r29 = (uint32_t)(var_r31 + 36);
	// cmplw cr6,r3,r29
	// beq cr6,0x82454448
while (var_r30 != var_r29) {
	loc_82454408:
		// lwz r30,4(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */);
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r8,8(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// stw r8,8(r30)
		PPC_STORE_U32(var_r30 + 8, ctx.r8.u32);
		// lwz r7,8(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// lwz r6,4(r3)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
		// stw r6,4(r7)
		PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r6.u32);
		// stw r3,4(r3)
		PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r3.u32);
		// stw r3,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r3.u32);
		// lwz r11,0(r5)
		// bctrl
		DTOR(ctx.r3.u32, ctx, base);  // vtable slot 0 (destructor)
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// cmplw cr6,r30,r29
		// bne cr6,0x82454408
}
loc_82454448:
	// lwz r11,96(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 96);
	// cmplwi cr6,r11,0
	// beq cr6,0x82454484
	if (ctx.r11.u32 != 0) {
		// addi r3,r11,-4
		ctx.r3.s64 = ctx.r11.s64 + -4;
		// lwz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// cmpwi cr6,r10,0
		// beq cr6,0x82454480
		if (ctx.r10.s32 != 0) {
			// addi r3,r11,4
			ctx.r3.s64 = ctx.r11.s64 + 4;
			// li r4,3
			ctx.r4.s64 = 3;
			// lwz r8,0(r9)
			// bctrl
			DTOR(ctx.r3.u32, ctx, base);  // vtable slot 0 (destructor)
			// b 0x82454484
		} else {
		loc_82454480:
			// bl 0x820c0120
			thunk_rage_free(ctx, base);
		}
	}
loc_82454484:
	// lbz r7,52(r31)
	ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 52);
	// rlwinm r6,r7,0,27,27
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r6,16
	// bne cr6,0x824544ac
	if (ctx.r6.u32 == 16) {
		// lwz r3,48(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r11,84(r5)
		// bctrl
		VCALL(ctx.r3.u32, 21, ctx, base);  // vtable slot 21 (byte +84)
	}
loc_824544AC:
	// lwz r11,48(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 48);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32256
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// stw r11,68(r31)
	PPC_STORE_U32(var_r31 + 68, ctx.r11.u32);
	// stw r11,56(r31)
	PPC_STORE_U32(var_r31 + 56, ctx.r11.u32);
	// stw r11,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x82453a38
	msgMsgSink_3A38_g(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_0_44F0_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_0_44F0_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_0_44F0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmplwi cr6,r31,0
	// beq cr6,0x8245453c
	if (var_r31 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,4(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	}
loc_8245453C:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_4550_g"))) PPC_WEAK_FUNC(msgMsgSink_4550_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_4550_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,80(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 80);
	// cmpwi cr6,r11,6
	// bne cr6,0x8245459c
	if (ctx.r11.s32 == 6) {
		// bl 0x82453f68
		game_3F68_h(ctx, base);
		// cmpwi cr6,r3,0
		// bne cr6,0x8245459c
		if (ctx.r3.s32 != 0) goto loc_8245459C;
		// lwz r10,0(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r9,4(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
		// b 0x824545c0
	} else {
	loc_8245459C:
		// addi r30,r31,36
		var_r30 = (uint32_t)(var_r31 + 36);
		// lwz r31,40(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 40));
		// cmplw cr6,r31,r30
		// beq cr6,0x824545c0
	while (var_r31 != var_r30) {
		loc_824545AC:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8245a650
			game_A650_h(ctx, base);
			// lwz r31,4(r31)
			var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
			// cmplw cr6,r31,r30
			// bne cr6,0x824545ac
	}
	}
loc_824545C0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_45D8_g"))) PPC_WEAK_FUNC(atSingleton_45D8_g);
PPC_FUNC_IMPL(__imp__atSingleton_45D8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// lwz r8,32(r30)
	ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 32);
	// lwz r9,20(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r7,24(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 24);
	// cmpw cr6,r9,r7
	// bgt cr6,0x82454638
	if (ctx.r9.s32 <= ctx.r7.s32) {
		// lwz r6,0(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,4(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 4);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		// b 0x8245467c
	} else {
	loc_82454638:
		// lwz r11,60(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 60);
		// addi r10,r30,56
		ctx.r10.s64 = (int64_t)(int32_t)var_r30 + 56;
		// cmplw cr6,r11,r10
		// beq cr6,0x82454660
	while (ctx.r11.u32 != ctx.r10.u32) {
		loc_82454648:
			// lwz r4,20(r11)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// cmpw cr6,r4,r9
			// bgt cr6,0x82454660
			if (ctx.r4.s32 > ctx.r9.s32) goto loc_82454660;
			// lwz r11,4(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
			// cmplw cr6,r11,r10
			// bne cr6,0x82454648
	}
	loc_82454660:
		// lwz r11,8(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// lwz r3,4(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// stw r3,4(r31)
		PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ ctx.r3.u32);
		// lwz r10,4(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r31,8(r10)
		PPC_STORE_U32(ctx.r10.u32 + 8, var_r31);
		// stw r31,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	}
loc_8245467C:
	// blr
	return;
}

__attribute__((alias("__imp__game_4698_h"))) PPC_WEAK_FUNC(game_4698_h);
PPC_FUNC_IMPL(__imp__game_4698_h) {
	PPC_FUNC_PROLOGUE();
	// addi r11,r3,68
	ctx.r11.s64 = ctx.r3.s64 + 68;
	// stw r11,4(r4)
	PPC_STORE_U32(ctx.r4.u32 + 4, ctx.r11.u32);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r10,8(r4)
	PPC_STORE_U32(ctx.r4.u32 + 8, ctx.r10.u32);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r4,4(r9)
	PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r4.u32);
	// stw r4,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r4.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_46B8_g"))) PPC_WEAK_FUNC(msgMsgSink_46B8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_46B8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r4,3
	// beq cr6,0x824547d0
	if (ctx.r4.s32 != 3) {
		// cmpwi cr6,r4,4
		// beq cr6,0x82454760
		if (ctx.r4.s32 != 4) {
			// cmpwi cr6,r4,6
			// bne cr6,0x82454820
			if (ctx.r4.s32 != 6) goto loc_82454820;
			// lwz r11,80(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 80);
			// cmpwi cr6,r11,6
			// bge cr6,0x82454820
			if (ctx.r11.s32 >= 6) goto loc_82454820;
			// li r10,6
			ctx.r10.s64 = 6;
			// lwz r9,84(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 84);
			// cmpwi cr6,r9,0
			// stw r10,80(r31)
			PPC_STORE_U32(var_r31 + 80, ctx.r10.u32);
			// beq cr6,0x8245471c
			if (ctx.r9.s32 != 0) {
				// li r8,0
				ctx.r8.s64 = 0;
				// lwz r7,28(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 28);
				// lwz r3,32(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
				// stw r8,84(r31)
				PPC_STORE_U32(var_r31 + 84, ctx.r8.u32);
				// lhz r4,1(r7)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r7.u32 + 1);
				// bl 0x82448b90
				msgMsgSink_8B90_g(ctx, base);
			}
		loc_8245471C:
			// lwz r3,48(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
			// lwz r5,124(r6)
			// bctrl
			VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
			// cmpwi cr6,r3,0
			// bne cr6,0x82454824
			if (ctx.r3.s32 != 0) {
				// blr
				return;
			}
			// lwz r3,48(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
			// lwz r11,108(r4)
			// bctrl
			VCALL(ctx.r3.u32, 27, ctx, base);  // vtable slot 27 (byte +108)
			// blr
			return;
		}
	loc_82454760:
		// lwz r10,80(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 80);
		// cmpwi cr6,r10,4
		// bge cr6,0x82454820
		if (ctx.r10.s32 >= 4) goto loc_82454820;
		// li r9,4
		ctx.r9.s64 = 4;
		// lwz r7,28(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 28);
		// li r8,1
		ctx.r8.s64 = 1;
		// lwz r3,32(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
		// stw r9,80(r31)
		PPC_STORE_U32(var_r31 + 80, ctx.r9.u32);
		// stw r8,84(r31)
		PPC_STORE_U32(var_r31 + 84, ctx.r8.u32);
		// lhz r4,1(r7)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r7.u32 + 1);
		// bl 0x82448b78
		msgMsgSink_8B78_g(ctx, base);
		// lwz r3,48(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
		// lwz r5,124(r6)
		// bctrl
		VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
		// cmpwi cr6,r3,0
		// bne cr6,0x82454824
		if (ctx.r3.s32 != 0) {
			// blr
			return;
		}
		// lwz r3,48(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
		// lwz r11,112(r4)
		// bctrl
		VCALL(ctx.r3.u32, 28, ctx, base);  // vtable slot 28 (byte +112)
		// blr
		return;
	}
loc_824547D0:
	// lwz r10,80(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 80);
	// cmpwi cr6,r10,3
	// bge cr6,0x82454820
	if (ctx.r10.s32 < 3) {
		// li r9,3
		ctx.r9.s64 = 3;
		// lwz r3,48(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
		// stw r9,80(r31)
		PPC_STORE_U32(var_r31 + 80, ctx.r9.u32);
		// lwz r7,124(r8)
		// bctrl
		VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
		// cmpwi cr6,r3,0
		// bne cr6,0x82454824
		if (ctx.r3.s32 != 0) {
			// blr
			return;
		}
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r3,48(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
		// bl 0x8244dc40
		msgMsgSink_DC40_g(ctx, base);
		// blr
		return;
	}
loc_82454820:
	// stw r4,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r4.u32);
loc_82454824:
	// blr
	return;
}

__attribute__((alias("__imp__rage_4838"))) PPC_WEAK_FUNC(rage_4838);
PPC_FUNC_IMPL(__imp__rage_4838) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// lwz r9,28(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 28);
	// addi r31,r3,2
	var_r31 = (uint32_t)(ctx.r3.s64 + 2);
	// lbz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// rlwinm r7,r8,0,30,30
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r7,2
	// bne cr6,0x82454888
	if (ctx.r7.u32 == 2) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82454318
		rage_4318(ctx, base);
		// rlwinm r11,r3,2,22,29
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0x3FC;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// addi r31,r11,1
		var_r31 = (uint32_t)(ctx.r11.s64 + 1);  // addr:0x82000001
	}
loc_82454888:
	// clrlwi r9,r29,24
	ctx.r9.u64 = var_r29 & 0xFF;
	// cmplwi cr6,r9,0
	// beq cr6,0x824548b8
	if (ctx.r9.u32 != 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	loc_82454898:
		// lbz r5,0(r31)
		ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 0);
		// addi r6,r11,1
		ctx.r6.s64 = ctx.r11.s64 + 1;
		// rotlwi r10,r5,2
		ctx.r10.u64 = __builtin_rotateleft32(ctx.r5.u32, 2);
		// clrlwi r11,r6,24
		ctx.r11.u64 = ctx.r6.u32 & 0xFF;
		// add r10,r10,r31
		ctx.r10.u64 = ctx.r10.u64 + var_r31;
		// cmplw cr6,r11,r9
		// addi r31,r10,1
		var_r31 = (uint32_t)(ctx.r10.s64 + 1);  // addr:0x82000001
		// blt cr6,0x82454898
		if (ctx.r11.u32 < ctx.r9.u32) goto loc_82454898;
	}
loc_824548B8:
	// lbz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 0);
	return;
}

__attribute__((alias("__imp__msgMsgSink_48C8_p39"))) PPC_WEAK_FUNC(msgMsgSink_48C8_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_48C8_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // msgMsgSink::vfn_4 (unnamed)  // vtable slot 4 (byte +16)
	// lwz r9,28(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 28);
	// addi r31,r3,2
	var_r31 = (uint32_t)(ctx.r3.s64 + 2);
	// lbz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// rlwinm r7,r8,0,30,30
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r7,2
	// bne cr6,0x8245491c
	if (ctx.r7.u32 == 2) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82454318
		rage_4318(ctx, base);
		// rlwinm r11,r3,2,22,29
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0x3FC;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// addi r31,r11,1
		var_r31 = (uint32_t)(ctx.r11.s64 + 1);  // addr:0x82000001
	}
loc_8245491C:
	// clrlwi r9,r29,24
	ctx.r9.u64 = var_r29 & 0xFF;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245494c
	if (ctx.r9.u32 != 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	loc_8245492C:
		// lbz r5,0(r31)
		ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 0);
		// addi r6,r11,1
		ctx.r6.s64 = ctx.r11.s64 + 1;
		// rotlwi r10,r5,2
		ctx.r10.u64 = __builtin_rotateleft32(ctx.r5.u32, 2);
		// clrlwi r11,r6,24
		ctx.r11.u64 = ctx.r6.u32 & 0xFF;
		// add r10,r10,r31
		ctx.r10.u64 = ctx.r10.u64 + var_r31;
		// cmplw cr6,r11,r9
		// addi r31,r10,1
		var_r31 = (uint32_t)(ctx.r10.s64 + 1);  // addr:0x82000001
		// blt cr6,0x8245492c
		if (ctx.r11.u32 < ctx.r9.u32) goto loc_8245492C;
	}
loc_8245494C:
	// rlwinm r11,r28,2,22,29
	ctx.r11.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0x3FC;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + var_r31;
	// lwz r3,1(r4)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 1);
	return;
}

__attribute__((alias("__imp__rage_4960"))) PPC_WEAK_FUNC(rage_4960);
PPC_FUNC_IMPL(__imp__rage_4960) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r27,0
	var_r27 = 0;
	// lwz r11,28(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 28);
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r10,0,30,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r9,2
	// bne cr6,0x82454aa0
	if (ctx.r9.u32 == 2) {
		// bl 0x82454318
		rage_4318(ctx, base);
		// clrlwi r26,r3,24
		var_r26 = (uint32_t)(ctx.r3.u32 & 0xFF);
		// cmplwi cr6,r26,0
		// bne cr6,0x824549a4
		if (var_r26 == 0) {
			// li r3,4
			ctx.r3.s64 = 4;
			// bl 0x820dec88
			xe_EC88(ctx, base);
			// b 0x824549e0
		} else {
		loc_824549A4:
			// lis r8,1365
			ctx.r8.s64 = 89456640;
			// ori r7,r8,21845
			ctx.r7.u64 = ctx.r8.u64 | 21845;
			// cmplw cr6,r26,r7
			// bgt cr6,0x824549d0
			if (var_r26 <= ctx.r7.u32) {
				// rlwinm r11,r26,1,0,30
				ctx.r11.u64 = __builtin_rotateleft64(var_r26 | (var_r26 << 32), 1) & 0xFFFFFFFE;
				// li r6,-5
				// add r5,r26,r11
				ctx.r5.u64 = var_r26 + ctx.r11.u64;
				// rlwinm r11,r5,4,0,27
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 4) & 0xFFFFFFF0;
				// cmplw cr6,r11,r6
				// addi r3,r11,4
				ctx.r3.s64 = ctx.r11.s64 + 4;
				// ble cr6,0x824549d4
				if (ctx.r11.u32 <= ctx.r6.u32) goto loc_824549D4;
			}
		loc_824549D0:
			// li r3,-1
		loc_824549D4:
			// lis r4,8324
			ctx.r4.s64 = 545521664;
			// ori r4,r4,83
			ctx.r4.u64 = ctx.r4.u64 | 83;
			// bl 0x820c01b8
			rage_01B8(ctx, base);
		}
	loc_824549E0:
		// cmplwi cr6,r3,0
		// beq cr6,0x82454a20
		if (ctx.r3.u32 != 0) {
			// addi r28,r3,4
			var_r28 = (uint32_t)(ctx.r3.s64 + 4);
			// stw r26,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* rage_GameObject::vtable@+0x0 */ var_r26);
			// addi r30,r26,-1
			var_r30 = (uint32_t)(var_r26 + -1);
			// mr r29,r28
			var_r29 = (uint32_t)(var_r28);
			// cmpwi cr6,r30,0
			// blt cr6,0x82454a18
		while ((int32_t)var_r30 >= 0) {
			loc_82454A00:
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x82453098
				rage_3098(ctx, base);
				// addi r30,r30,-1
				var_r30 = (uint32_t)(var_r30 + -1);
				// addi r29,r29,48
				var_r29 = (uint32_t)(var_r29 + 48);
				// cmpwi cr6,r30,0
				// bge cr6,0x82454a00
		}
		loc_82454A18:
			// mr r11,r28
			ctx.r11.u64 = var_r28;
			// b 0x82454a24
		} else {
		loc_82454A20:
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_82454A24:
		// cmplwi cr6,r11,0
		// stw r11,96(r31)
		PPC_STORE_U32(var_r31 + 96, ctx.r11.u32);
		// beq cr6,0x82454aac
		if (ctx.r11.u32 == 0) {
			// lis r3,-32761
			// ori r3,r3,14
			ctx.r3.u64 = ctx.r3.u64 | 14;
			return;
		}
		// li r4,0
		ctx.r4.s64 = 0;
	loc_82454A34:
		// clrlwi r30,r4,24
		var_r30 = (uint32_t)(ctx.r4.u32 & 0xFF);
		// cmplw cr6,r30,r26
		// bge cr6,0x82454aa0
		if (var_r30 >= var_r26) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x824540c8
		msgMsgSink_40C8_p39(ctx, base);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// lwz r3,32(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
		// bl 0x82448d90
		rage_8D90(ctx, base);
		// rlwinm r10,r30,1,0,30
		ctx.r10.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 1) & 0xFFFFFFFE;
		// mr r6,r3
		ctx.r6.u64 = ctx.r3.u64;
		// lwz r11,96(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 96);
		// add r3,r30,r10
		ctx.r3.u64 = var_r30 + ctx.r10.u64;
		// lwz r4,32(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 32);
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// rlwinm r29,r3,4,0,27
		var_r29 = (uint32_t)(__builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 4) & 0xFFFFFFF0);
		// add r3,r11,r29
		ctx.r3.u64 = ctx.r11.u64 + var_r29;
		// bl 0x82453198
		rage_3198(ctx, base);
		// mr r27,r3
		var_r27 = ctx.r3.u32;
		// cmpwi cr6,r27,0
		// blt cr6,0x82454a90
		if ((int32_t)var_r27 >= 0) {
			// lwz r11,96(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 96);
			// add r3,r11,r29
			ctx.r3.u64 = ctx.r11.u64 + var_r29;
			// bl 0x82452ee0
			rage_2EE0(ctx, base);
		}
	loc_82454A90:
		// addi r11,r30,1
		ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 1;
		// cmpwi cr6,r27,0
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// bge cr6,0x82454a34
		if ((int32_t)var_r27 >= 0) goto loc_82454A34;
	}
loc_82454AA0:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__atSingleton_4AC0_g"))) PPC_WEAK_FUNC(atSingleton_4AC0_g);
PPC_FUNC_IMPL(__imp__atSingleton_4AC0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	double var_f29 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f29.u64);
	// stfd f30,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,28(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 28);
	// lfs f31,88(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 88);
	var_f31 = double(temp.f32);
	// lfs f30,8(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 8);
	var_f30 = double(temp.f32);
	// lwz r3,32(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
	// lfs f29,112(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 112);
	var_f29 = double(temp.f32);
	// lhz r4,1(r11)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1);
	// bl 0x82448ab8
	atSingleton_8AB8_wrh(ctx, base);
	// lwz r10,48(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 48);
	// lfs f0,28(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 28);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f13,f0,f31
	ctx.f13.f64 = double(float(ctx.f0.f64 * var_f31));
	// fmuls f12,f13,f1
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f1.f64));
	// fmuls f11,f12,f29
	ctx.f11.f64 = double(float(ctx.f12.f64 * var_f29));
	// fmuls f1,f11,f30
	ctx.f1.f64 = double(float(ctx.f11.f64 * var_f30));
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-40(r1)
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f30,-32(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_4B38_h"))) PPC_WEAK_FUNC(jumptable_4B38_h);
PPC_FUNC_IMPL(__imp__jumptable_4B38_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplwi cr6,r11,0
	// beqlr cr6
	if (ctx.r11.u32 == 0) return;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// cmplwi cr6,r11,0
	// beqlr cr6
	if (ctx.r11.u32 == 0) return;
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// clrlwi r8,r9,27
	ctx.r8.u64 = ctx.r9.u32 & 0x1F;
	// cmpwi cr6,r8,3
	// bnelr cr6
	if (ctx.r8.s32 != 3) return;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_3"))) PPC_WEAK_FUNC(msgMsgSink_vfn_3);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_3) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,60(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 60);
	// addi r9,r3,56
	ctx.r9.s64 = ctx.r3.s64 + 56;
	// lwz r8,40(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplw cr6,r11,r9
	// beqlr cr6
	if (ctx.r11.u32 == ctx.r9.u32) return;
loc_82454B90:
	// lwz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// lwz r7,16(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// cmplw cr6,r7,r8
	// bne cr6,0x82454bc4
	if (ctx.r7.u32 == ctx.r8.u32) {
		// lwz r6,16(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
		// lwz r10,28(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
		// lwz r5,0(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
		// clrlwi r4,r5,27
		ctx.r4.u64 = ctx.r5.u32 & 0x1F;
		// cmpwi cr6,r4,0
		// beq cr6,0x82454bd4
		if (ctx.r4.s32 == 0) {
			// mr r3,r10
			ctx.r3.u64 = ctx.r10.u64;
			// blr
			return;
		}
		// cmpw cr6,r10,r3
		// ble cr6,0x82454bc4
		if (ctx.r10.s32 <= ctx.r3.s32) goto loc_82454BC4;
		// mr r3,r10
		ctx.r3.u64 = ctx.r10.u64;
	}
loc_82454BC4:
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplw cr6,r11,r9
	// bne cr6,0x82454b90
	if (ctx.r11.u32 != ctx.r9.u32) goto loc_82454B90;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_4BE0"))) PPC_WEAK_FUNC(atSingleton_4BE0);
PPC_FUNC_IMPL(__imp__atSingleton_4BE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=1168, savegprlr_26
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// lwz r26,40(r31)
	var_r26 = (uint32_t)(PPC_LOAD_U32(var_r31 + 40));
	// li r5,1024
	ctx.r5.s64 = 1024;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8242fed0
	memset(ctx, base);
	// lwz r11,60(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 60);
	// addi r6,r31,56
	ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 56;
	// li r8,0
	ctx.r8.s64 = 0;
	// cmplw cr6,r11,r6
	// beq cr6,0x82454ca4
	if (ctx.r11.u32 != ctx.r6.u32) {
		// addi r7,r1,80
		ctx.r7.s64 = ctx.r1.s64 + 80;
	loc_82454C30:
		// lwz r10,44(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
		// lwz r9,16(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
		// cmplw cr6,r9,r26
		// bne cr6,0x82454c98
		if (ctx.r9.u32 == var_r26) {
			// lwz r9,16(r11)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
			// lwz r5,0(r9)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
			// clrlwi r10,r5,27
			ctx.r10.u64 = ctx.r5.u32 & 0x1F;
			// cmpwi cr6,r10,9
			// beq cr6,0x82454c5c
			if (ctx.r10.s32 != 9) {
				// cmpwi cr6,r10,18
				// bne cr6,0x82454c98
				if (ctx.r10.s32 != 18) goto loc_82454C98;
			}
		loc_82454C5C:
			// lwz r4,28(r11)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
			// cmpw cr6,r4,r27
			// ble cr6,0x82454c98
			if (ctx.r4.s32 <= (int32_t)var_r27) goto loc_82454C98;
			// lwz r10,7(r9)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7);
			// cmplw cr6,r10,r29
			// blt cr6,0x82454c98
			if (ctx.r10.u32 < var_r29) goto loc_82454C98;
			// cmplw cr6,r10,r28
			// bgt cr6,0x82454c98
			if (ctx.r10.u32 > var_r28) goto loc_82454C98;
			// stw r11,0(r7)
			PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r11.u32);
			// addi r8,r8,1
			ctx.r8.s64 = ctx.r8.s64 + 1;
			// cmpwi cr6,r30,0
			// addi r7,r7,4
			ctx.r7.s64 = ctx.r7.s64 + 4;
			// bne cr6,0x82454ca4
			if ((int32_t)var_r30 != 0) goto loc_82454CA4;
			// cmpwi cr6,r8,256
			// beq cr6,0x82454ca4
			if (ctx.r8.s32 == 256) goto loc_82454CA4;
		}
	loc_82454C98:
		// lwz r11,4(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// cmplw cr6,r11,r6
		// bne cr6,0x82454c30
		if (ctx.r11.u32 != ctx.r6.u32) goto loc_82454C30;
	}
loc_82454CA4:
	// li r3,0
	ctx.r3.s64 = 0;
	// cmpwi cr6,r8,0
	// ble cr6,0x82454ce8
	if (ctx.r8.s32 > 0) {
		// cmpwi cr6,r30,0
		// beq cr6,0x82454ccc
		if ((int32_t)var_r30 != 0) {
			// rlwinm r3,r3,2,0,29
			ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r11,r1,80
			ctx.r11.s64 = ctx.r1.s64 + 80;
			// lwzx r3,r3,r11
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + ctx.r11.u32);
			return;
		}
	loc_82454CCC:
		// addi r5,r8,-1
		ctx.r5.s64 = ctx.r8.s64 + -1;
		// lwz r3,32(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
		// li r4,0
		ctx.r4.s64 = 0;
		// bl 0x82447ce0
		atSingleton_7CE0_w(ctx, base);
		// rlwinm r3,r3,2,0,29
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
		// addi r11,r1,80
		ctx.r11.s64 = ctx.r1.s64 + 80;
		// lwzx r3,r3,r11
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + ctx.r11.u32);
	}
loc_82454CE8:
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_4CF0_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_4CF0_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_4CF0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	PPCRegister temp{};
	// FRAME: size=144, savegprlr_27
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15908
	ctx.r11.s64 = ctx.r11.s64 + 15908;
	// addi r30,r31,4
	var_r30 = (uint32_t)(var_r31 + 4);
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r27,r6
	var_r27 = ctx.r6.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x82453a08
	snNotifyGamerAdded_3A08_2h(ctx, base);
	// lis r11,-32256
	// lis r7,-32256
	// stw r29,32(r31)
	PPC_STORE_U32(var_r31 + 32, var_r29);
	// addi r10,r31,36
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 36;
	// stw r27,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r27);
	// addi r6,r7,20200
	ctx.r6.s64 = ctx.r7.s64 + 20200;
	// lis r7,-32256
	// lfs f0,15788(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	// addi r7,r7,15816
	ctx.r7.s64 = ctx.r7.s64 + 15816;
	// addi r9,r31,56
	ctx.r9.s64 = (int64_t)(int32_t)var_r31 + 56;
	// addi r8,r31,68
	ctx.r8.s64 = (int64_t)(int32_t)var_r31 + 68;
	// stw r6,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r6.u32);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lfs f13,15784(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32256
	// addi r11,r11,20144
	ctx.r11.s64 = ctx.r11.s64 + 20144;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r7,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r7.u32);
	// stw r10,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r10.u32);
	// stw r10,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r10.u32);
	// stw r28,48(r31)
	PPC_STORE_U32(var_r31 + 48, var_r28);
	// stb r11,52(r31)
	PPC_STORE_U8(var_r31 + 52, ctx.r11.u8);
	// stw r7,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r7.u32);
	// stw r9,4(r9)
	PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r9.u32);
	// stw r9,8(r9)
	PPC_STORE_U32(ctx.r9.u32 + 8, ctx.r9.u32);
	// stw r7,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r7.u32);
	// stw r8,4(r8)
	PPC_STORE_U32(ctx.r8.u32 + 4, ctx.r8.u32);
	// stw r8,8(r8)
	PPC_STORE_U32(ctx.r8.u32 + 8, ctx.r8.u32);
	// stfs f0,112(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 112, temp.u32);
	// lwz r10,48(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 48);
	// stfs f0,116(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 116, temp.u32);
	// stfs f13,120(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 120, temp.u32);
	// stw r11,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r11.u32);
	// addi r3,r10,4
	ctx.r3.s64 = ctx.r10.s64 + 4;
	// stw r11,96(r31)
	PPC_STORE_U32(var_r31 + 96, ctx.r11.u32);
	// stw r11,100(r31)
	PPC_STORE_U32(var_r31 + 100, ctx.r11.u32);
	// stw r11,104(r31)
	PPC_STORE_U32(var_r31 + 104, ctx.r11.u32);
	// stw r11,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r11.u32);
	// stw r11,108(r31)
	PPC_STORE_U32(var_r31 + 108, ctx.r11.u32);
	// stw r11,124(r31)
	PPC_STORE_U32(var_r31 + 124, ctx.r11.u32);
	// lwz r10,0(r11)
	// bctrl
	DTOR(ctx.r3.u32, ctx, base);  // vtable slot 0 (destructor)
	// lwz r11,28(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 28);
	// lis r10,-32256
	// addi r10,r10,19120
	ctx.r10.s64 = ctx.r10.s64 + 19120;
	// lbz r9,3(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3);
	// rotlwi r8,r9,2
	ctx.r8.u64 = __builtin_rotateleft32(ctx.r9.u32, 2);
	// lfsx f0,r8,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + ctx.r10.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 88, temp.u32);
	// lhz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r11.u32 + 4);
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// cmpwi cr6,r11,2400
	// blt cr6,0x82454e14
	if (ctx.r11.s32 >= 2400) {
		// lis r11,-32256
		ctx.r11.s64 = -2113929216;
		// lfs f0,16056(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);  /* glob:lbl_82003EB8 @ 0x82003eb8 */
		ctx.f0.f64 = double(temp.f32);
		// b 0x82454e48
	} else {
	loc_82454E14:
		// cmpwi cr6,r11,-2400
		// bgt cr6,0x82454e28
		if (ctx.r11.s32 <= -2400) {
			// lis r11,-32248
			ctx.r11.s64 = -2113404928;
			// lfs f0,-25416(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25416);  /* glob:lbl_82079CB8 @ 0x82079cb8 */
			ctx.f0.f64 = double(temp.f32);
			// b 0x82454e48
		} else {
		loc_82454E28:
			// extsw r7,r11
			ctx.r7.s64 = ctx.r11.s32;
			// lis r11,-32256
			// std r7,80(r1)
			PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
			// lfs f0,18960(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18960);  /* glob:0x82084a10 */
			ctx.f0.f64 = double(temp.f32);
			// lfd f13,80(r1)
			ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
			// fcfid f12,f13
			ctx.f12.f64 = double(ctx.f13.s64);
			// frsp f11,f12
			ctx.f11.f64 = double(float(ctx.f12.f64));
			// fmuls f0,f11,f0
			ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
		}
	}
loc_82454E48:
	// li r6,1
	ctx.r6.s64 = 1;
	// stfs f0,92(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 92, temp.u32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r6,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r6.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_1_4E60_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_1_4E60_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_1_4E60_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x824543a8
	msgMsgSink_43A8_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82454ea0
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32864
		ctx.r4.u64 = ctx.r4.u64 | 32864;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82454EA0:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_4EB8_g"))) PPC_WEAK_FUNC(msgMsgSink_4EB8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_4EB8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r28,0
	var_r28 = 0;
	// lwz r11,80(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 80);
	// cmpwi cr6,r11,3
	// bne cr6,0x82455060
	if (ctx.r11.s32 == 3) {
		// lwz r3,48(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
		// lwz r10,124(r11)
		// bctrl
		VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
		// cmpwi cr6,r3,0
		// bne cr6,0x82454fa4
		if (ctx.r3.s32 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x82453fd0
			game_3FD0_h(ctx, base);
			// cmpwi cr6,r3,1
			// beq cr6,0x82454f18
			if (ctx.r3.s32 != 1) {
				// cmpwi cr6,r3,2
				// bne cr6,0x82454fa4
				if (ctx.r3.s32 != 2) goto loc_82454FA4;
				// li r3,0
				ctx.r3.s64 = 0;
				return;
			}
		loc_82454F18:
			// lwz r9,80(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 80);
			// cmpwi cr6,r9,6
			// bge cr6,0x82454f8c
			if (ctx.r9.s32 < 6) {
				// li r8,6
				ctx.r8.s64 = 6;
				// lwz r7,84(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 84);
				// cmpwi cr6,r7,0
				// stw r8,80(r31)
				PPC_STORE_U32(var_r31 + 80, ctx.r8.u32);
				// beq cr6,0x82454f4c
				if (ctx.r7.s32 != 0) {
					// lwz r6,28(r31)
					ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 28);
					// stw r28,84(r31)
					PPC_STORE_U32(var_r31 + 84, var_r28);
					// lwz r3,32(r31)
					ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
					// lhz r4,1(r6)
					ctx.r4.u64 = PPC_LOAD_U16(ctx.r6.u32 + 1);
					// bl 0x82448b90
					msgMsgSink_8B90_g(ctx, base);
				}
			loc_82454F4C:
				// lwz r3,48(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
				// lwz r4,124(r5)
				// bctrl
				VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
				// cmpwi cr6,r3,0
				// bne cr6,0x82454f94
				if (ctx.r3.s32 != 0) {
					// lis r3,-30009
					// ori r3,r3,8
					ctx.r3.u64 = ctx.r3.u64 | 8;
					return;
				}
				// lwz r3,48(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
				// lwz r10,108(r11)
				// bctrl
				VCALL(ctx.r3.u32, 27, ctx, base);  // vtable slot 27 (byte +108)
				// lis r3,-30009
				// ori r3,r3,8
				ctx.r3.u64 = ctx.r3.u64 | 8;
				return;
			}
		loc_82454F8C:
			// li r9,6
			ctx.r9.s64 = 6;
			// stw r9,80(r31)
			PPC_STORE_U32(var_r31 + 80, ctx.r9.u32);
		loc_82454F94:
			// lis r3,-30009
			// ori r3,r3,8
			ctx.r3.u64 = ctx.r3.u64 | 8;
			return;
		}
	loc_82454FA4:
		// lwz r30,40(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 40));
		// addi r29,r31,36
		var_r29 = (uint32_t)(var_r31 + 36);
	loc_82454FAC:
		// cmplw cr6,r30,r29
		// beq cr6,0x82454fd4
		if (var_r30 == var_r29) goto loc_82454FD4;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82459a48
		game_9A48_h(ctx, base);
		// mr r28,r3
		var_r28 = ctx.r3.u32;
		// lwz r30,4(r30)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r30 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmpwi cr6,r28,0
		// bge cr6,0x82454fac
		if ((int32_t)var_r28 >= 0) goto loc_82454FAC;
		return;
	loc_82454FD4:
		// cmpwi cr6,r28,0
		// blt cr6,0x82455088
		if ((int32_t)var_r28 < 0) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			return;
		}
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// lwz r8,80(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 80);
		// stw r3,100(r31)
		PPC_STORE_U32(var_r31 + 100, ctx.r3.u32);
		// cmpwi cr6,r8,4
		// bge cr6,0x8245504c
		if (ctx.r8.s32 < 4) {
			// li r7,4
			ctx.r7.s64 = 4;
			// lwz r5,28(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 28);
			// li r6,1
			ctx.r6.s64 = 1;
			// lwz r3,32(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
			// stw r7,80(r31)
			PPC_STORE_U32(var_r31 + 80, ctx.r7.u32);
			// stw r6,84(r31)
			PPC_STORE_U32(var_r31 + 84, ctx.r6.u32);
			// lhz r4,1(r5)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r5.u32 + 1);
			// bl 0x82448b78
			msgMsgSink_8B78_g(ctx, base);
			// lwz r3,48(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
			// lwz r11,124(r4)
			// bctrl
			VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
			// cmpwi cr6,r3,0
			// bne cr6,0x82455088
			if (ctx.r3.s32 != 0) {
				// mr r3,r28
				ctx.r3.u64 = var_r28;
				return;
			}
			// lwz r3,48(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 48);
			// lwz r9,112(r10)
			// bctrl
			VCALL(ctx.r3.u32, 28, ctx, base);  // vtable slot 28 (byte +112)
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			return;
		}
	loc_8245504C:
		// li r8,4
		ctx.r8.s64 = 4;
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// stw r8,80(r31)
		PPC_STORE_U32(var_r31 + 80, ctx.r8.u32);
		return;
	}
loc_82455060:
	// cmpwi cr6,r11,4
	// bge cr6,0x82455080
	if (ctx.r11.s32 < 4) {
		// lbz r7,52(r31)
		ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 52);
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// ori r6,r7,64
		ctx.r6.u64 = ctx.r7.u64 | 64;
		// stb r6,52(r31)
		PPC_STORE_U8(var_r31 + 52, ctx.r6.u8);
		return;
	}
loc_82455080:
	// lis r28,-30009
	var_r28 = (uint32_t)(-1966669824);
	// ori r28,r28,6
	var_r28 = (uint32_t)(var_r28 | 6);
loc_82455088:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	return;
}

__attribute__((alias("__imp__msgMsgSink_5098_g"))) PPC_WEAK_FUNC(msgMsgSink_5098_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_5098_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// li r30,0
	var_r30 = 0;
	// lbz r11,52(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 52);
	// rlwinm r10,r11,0,26,26
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r10,32
	// bne cr6,0x824550cc
	if (ctx.r10.u32 == 32) {
		// li r4,0
		ctx.r4.s64 = 0;
		// bl 0x82453c88
		msgMsgSink_3C88_g(ctx, base);
		// li r29,1
		var_r29 = 1;
	}
loc_824550CC:
	// lwz r11,80(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 80);
	// cmpwi cr6,r11,6
	// beq cr6,0x8245513c
	if (ctx.r11.s32 != 6) {
		// cmpwi cr6,r11,5
		// bne cr6,0x824550f8
		if (ctx.r11.s32 == 5) {
			// lwz r9,124(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 124);
			// cmpwi cr6,r9,0
			// bne cr6,0x8245513c
			if (ctx.r9.s32 != 0) {
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				return;
			}
			// cmpwi cr6,r29,0
			// beq cr6,0x8245513c
			if ((int32_t)var_r29 == 0) {
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				return;
			}
			// b 0x82455100
		} else {
		loc_824550F8:
			// cmpwi cr6,r29,0
			// beq cr6,0x82455108
			if ((int32_t)var_r29 == 0) goto loc_82455108;
		}
	loc_82455100:
		// li r8,1
		ctx.r8.s64 = 1;
		// stw r8,124(r31)
		PPC_STORE_U32(var_r31 + 124, ctx.r8.u32);
	loc_82455108:
		// li r7,5
		ctx.r7.s64 = 5;
		// addi r28,r31,36
		var_r28 = (uint32_t)(var_r31 + 36);
		// stw r7,80(r31)
		PPC_STORE_U32(var_r31 + 80, ctx.r7.u32);
		// lwz r31,40(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 40));
	loc_82455118:
		// cmplw cr6,r31,r28
		// beq cr6,0x8245513c
		if (var_r31 == var_r28) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8245a150
		phBoundCapsule_A150(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// lwz r31,4(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
		// cmpwi cr6,r30,0
		// bge cr6,0x82455118
		if ((int32_t)var_r30 >= 0) goto loc_82455118;
	}
loc_8245513C:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_5148_g"))) PPC_WEAK_FUNC(msgMsgSink_5148_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_5148_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lbz r11,52(r30)
	ctx.r11.u64 = PPC_LOAD_U8(var_r30 + 52);
	// rlwinm r10,r11,0,26,26
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r10,32
	// beq cr6,0x82455538
	if (ctx.r10.u32 != 32) {
		// lwz r11,80(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 80);
		// cmpwi cr6,r11,4
		// beq cr6,0x82455370
		if (ctx.r11.s32 != 4) {
			// cmpwi cr6,r11,5
			// beq cr6,0x82455370
			if (ctx.r11.s32 == 5) goto loc_82455370;
			// cmpwi cr6,r11,2
			// bne cr6,0x82455210
			if (ctx.r11.s32 != 2) goto loc_82455210;
			// lwz r11,40(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 40);
			// addi r9,r30,36
			ctx.r9.s64 = (int64_t)(int32_t)var_r30 + 36;
			// cmplw cr6,r11,r9
			// beq cr6,0x824551b8
		while (ctx.r11.u32 != ctx.r9.u32) {
			loc_82455194:
				// lwz r10,24(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
				// cmplwi cr6,r10,0
				// beq cr6,0x82455210
				if (ctx.r10.u32 == 0) goto loc_82455210;
				// lwz r10,40(r10)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 40);
				// cmpwi cr6,r10,3
				// bne cr6,0x82455210
				if (ctx.r10.s32 != 3) goto loc_82455210;
				// lwz r11,4(r11)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
				// cmplw cr6,r11,r9
				// bne cr6,0x82455194
		}
		loc_824551B8:
			// li r9,3
			ctx.r9.s64 = 3;
			// lwz r3,48(r30)
			ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 48);
			// stw r9,80(r30)
			PPC_STORE_U32(var_r30 + 80, ctx.r9.u32);
			// lwz r7,124(r8)
			// bctrl
			VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
			// cmpwi cr6,r3,0
			// bne cr6,0x824551e8
			if (ctx.r3.s32 == 0) {
				// li r4,1
				ctx.r4.s64 = 1;
				// lwz r3,48(r30)
				ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 48);
				// bl 0x8244dc40
				msgMsgSink_DC40_g(ctx, base);
			}
		loc_824551E8:
			// lbz r11,52(r30)
			ctx.r11.u64 = PPC_LOAD_U8(var_r30 + 52);
			// rlwinm r5,r11,0,25,25
			ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x40;
			// cmplwi cr6,r5,64
			// bne cr6,0x82455210
			if (ctx.r5.u32 != 64) goto loc_82455210;
			// andi. r4,r11,191
			ctx.r4.u64 = ctx.r11.u64 & 191;
			ctx.cr0.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// stb r4,52(r30)
			PPC_STORE_U8(var_r30 + 52, ctx.r4.u8);
			// bl 0x82454eb8
			msgMsgSink_4EB8_g(ctx, base);
			// cmpwi cr6,r3,0
			// blt cr6,0x82455538
			if (ctx.r3.s32 < 0) {
				return;
			}
		loc_82455210:
			// lwz r11,28(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
			// lbz r10,0(r11)
			ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// rlwinm r9,r10,0,30,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x2;
			// cmplwi cr6,r9,2
			// bne cr6,0x82455264
			if (ctx.r9.u32 == 2) {
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// bl 0x82454318
				rage_4318(ctx, base);
				// clrlwi r29,r3,24
				var_r29 = (uint32_t)(ctx.r3.u32 & 0xFF);
				// cmplwi cr6,r29,0
				// beq cr6,0x82455264
				if (var_r29 == 0) goto loc_82455264;
				// li r31,0
				var_r31 = 0;
			loc_8245523C:
				// rlwinm r11,r31,1,0,30
				ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 1) & 0xFFFFFFFE;
				// lwz r10,96(r30)
				ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 96);
				// add r8,r31,r11
				ctx.r8.u64 = var_r31 + ctx.r11.u64;
				// rlwinm r11,r8,4,0,27
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 4) & 0xFFFFFFF0;
				// add r3,r11,r10
				ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
				// bl 0x82453078
				atSingleton_3078_p39(ctx, base);
				// addi r7,r31,1
				ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 1;
				// clrlwi r31,r7,24
				var_r31 = (uint32_t)(ctx.r7.u32 & 0xFF);
				// cmplw cr6,r31,r29
				// blt cr6,0x8245523c
				if (var_r31 < var_r29) goto loc_8245523C;
			}
		loc_82455264:
			// lwz r31,40(r30)
			var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 40));
			// addi r29,r30,36
			var_r29 = (uint32_t)(var_r30 + 36);
			// cmplw cr6,r31,r29
			// beq cr6,0x82455288
		while (var_r31 != var_r29) {
			loc_82455274:
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// bl 0x8245a5a0
				atSingleton_A5A0_h(ctx, base);
				// lwz r31,4(r31)
				var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
				// cmplw cr6,r31,r29
				// bne cr6,0x82455274
		}
		loc_82455288:
			// lwz r3,60(r30)
			ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 60);
			// addi r29,r30,56
			var_r29 = (uint32_t)(var_r30 + 56);
			// cmplw cr6,r3,r29
			// beq cr6,0x824552e8
		while (ctx.r3.u32 != var_r29) {
			loc_82455298:
				// lwz r6,32(r30)
				ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 32);
				// lwz r5,20(r3)
				ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
				// lwz r4,24(r6)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r6.u32 + 24);
				// cmpw cr6,r5,r4
				// bgt cr6,0x824552e8
				if (ctx.r5.s32 > ctx.r4.s32) goto loc_824552E8;
				// lwz r11,8(r3)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
				// lwz r10,4(r3)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
				// stw r11,8(r10)
				PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
				// lwz r9,8(r3)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
				// lwz r8,4(r3)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */;
				// stw r8,4(r9)
				PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r8.u32);
				// lwz r7,0(r3)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
				// stw r3,4(r3)
				PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r3.u32);
				// stw r3,8(r3)
				PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r3.u32);
				// lwz r6,4(r7)
				ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
				// bctrl
				PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
				// lwz r3,60(r30)
				ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 60);
				// cmplw cr6,r3,r29
				// bne cr6,0x82455298
		}
		loc_824552E8:
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// lwz r5,32(r30)
			ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 32);
			// lwz r31,60(r30)
			var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 60));
			// cmplw cr6,r31,r29
			// lwz r11,40(r5)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 40);
			// add r27,r11,r3
			var_r27 = (uint32_t)(ctx.r11.u64 + ctx.r3.u64);
			// beq cr6,0x82455464
			if (var_r31 == var_r29) goto loc_82455464;
		loc_82455304:
			// lwz r4,20(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 20);
			// lwz r28,4(r31)
			var_r28 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
			// cmpw cr6,r4,r27
			// bgt cr6,0x82455464
			if (ctx.r4.s32 > (int32_t)var_r27) goto loc_82455464;
			// lwz r3,16(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
			// lwz r11,0(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
			// clrlwi r11,r11,27
			ctx.r11.u64 = ctx.r11.u32 & 0x1F;
			// cmpwi cr6,r11,10
			// beq cr6,0x82455330
			if (ctx.r11.s32 != 10) {
				// cmpwi cr6,r11,11
				// bne cr6,0x82455458
				if (ctx.r11.s32 != 11) goto loc_82455458;
			}
		loc_82455330:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8245a758
			game_A758_h(ctx, base);
			// cmpwi cr6,r3,0
			// beq cr6,0x82455450
			if (ctx.r3.s32 == 0) goto loc_82455450;
			// lwz r10,44(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 44);
			// lwz r9,16(r10)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
			// stw r31,36(r9)
			PPC_STORE_U32(ctx.r9.u32 + 36, var_r31);
			// lwz r3,4(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */;
			// cmplw cr6,r3,r29
			// beq cr6,0x82455458
			if (ctx.r3.u32 == var_r29) goto loc_82455458;
		loc_82455358:
			// lwz r31,4(r3)
			var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */);
			// bl 0x824527c0
			atSingleton_27C0(ctx, base);
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// cmplw cr6,r31,r29
			// bne cr6,0x82455358
			if (var_r31 != var_r29) goto loc_82455358;
			// b 0x82455458
			goto loc_82455458;
		}
	loc_82455370:
		// addi r3,r30,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
		// bl 0x82453b48
		msgMsgSink_3B48_g(ctx, base);
		// cmpwi cr6,r3,0
		// beq cr6,0x82455388
		if (ctx.r3.s32 != 0) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x82453dc8
			msgMsgSink_3DC8_g(ctx, base);
		}
	loc_82455388:
		// lwz r10,40(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 40);
		// addi r8,r30,36
		ctx.r8.s64 = (int64_t)(int32_t)var_r30 + 36;
		// cmplw cr6,r10,r8
		// beq cr6,0x824553d0
	while (ctx.r10.u32 != ctx.r8.u32) {
		loc_82455398:
			// lwz r11,24(r10)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 24);
			// cmplwi cr6,r11,0
			// beq cr6,0x824553c4
			if (ctx.r11.u32 == 0) goto loc_824553C4;
			// lwz r9,40(r11)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
			// cmpwi cr6,r9,4
			// beq cr6,0x82455210
			if (ctx.r9.s32 == 4) goto loc_82455210;
			// cmplwi cr6,r11,0
			// beq cr6,0x824553c4
			if (ctx.r11.u32 == 0) goto loc_824553C4;
			// rotlwi r11,r9,0
			ctx.r11.u64 = ctx.r9.u32;
			// cmpwi cr6,r11,5
			// beq cr6,0x82455210
			if (ctx.r11.s32 == 5) goto loc_82455210;
		loc_824553C4:
			// lwz r10,4(r10)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
			// cmplw cr6,r10,r8
			// bne cr6,0x82455398
	}
	loc_824553D0:
		// lwz r3,80(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 80);
		// cmpwi cr6,r3,6
		// bge cr6,0x82455440
		if (ctx.r3.s32 < 6) {
			// li r11,6
			ctx.r11.s64 = 6;
			// lwz r10,84(r30)
			ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 84);
			// cmpwi cr6,r10,0
			// stw r11,80(r30)
			PPC_STORE_U32(var_r30 + 80, ctx.r11.u32);
			// beq cr6,0x82455408
			if (ctx.r10.s32 != 0) {
				// li r9,0
				ctx.r9.s64 = 0;
				// lwz r8,28(r30)
				ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 28);
				// lwz r3,32(r30)
				ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 32);
				// stw r9,84(r30)
				PPC_STORE_U32(var_r30 + 84, ctx.r9.u32);
				// lhz r4,1(r8)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r8.u32 + 1);
				// bl 0x82448b90
				msgMsgSink_8B90_g(ctx, base);
			}
		loc_82455408:
			// lwz r3,48(r30)
			ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 48);
			// lwz r6,124(r7)
			// bctrl
			VCALL(ctx.r3.u32, 31, ctx, base);  // vtable slot 31 (byte +124)
			// cmpwi cr6,r3,0
			// bne cr6,0x82455538
			if (ctx.r3.s32 != 0) {
				return;
			}
			// lwz r3,48(r30)
			ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 48);
			// lwz r4,108(r5)
			// bctrl
			VCALL(ctx.r3.u32, 27, ctx, base);  // vtable slot 27 (byte +108)
			return;
		}
	loc_82455440:
		// li r3,6
		ctx.r3.s64 = 6;
		// stw r3,80(r30)
		PPC_STORE_U32(var_r30 + 80, ctx.r3.u32);
		return;
	loc_82455450:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x824527c0
		atSingleton_27C0(ctx, base);
	loc_82455458:
		// mr r31,r28
		var_r31 = (uint32_t)(var_r28);
		// cmplw cr6,r28,r29
		// bne cr6,0x82455304
		if (var_r28 != var_r29) goto loc_82455304;
	loc_82455464:
		// lwz r31,72(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 72));
		// addi r26,r30,68
		var_r26 = (uint32_t)(var_r30 + 68);
		// cmplw cr6,r31,r26
		// beq cr6,0x82455538
	while (var_r27 != var_r26) {
		loc_82455474:
			// lwz r11,36(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lwz r27,4(r31)
			var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */);
			// lbz r10,40(r11)
			ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 40);
			// rlwinm r8,r10,0,0,24
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
			// cmplwi cr6,r8,128
			// bne cr6,0x8245552c
			if (!(ctx.r8.u32 != 128)) {
				// lwz r7,16(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// lwz r6,0(r7)
				ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
				// rlwinm r5,r6,0,1,1
				ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0x40000000;
				// cmplwi cr6,r5,0
				// beq cr6,0x824554c0
				if (ctx.r5.u32 != 0) {
				// lwz r4,32(r30)
				ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 32);
				// lwz r29,20(r11)
				var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + 20));
				// lwz r28,24(r4)
				var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r4.u32 + 24));
				// bl 0x82452880
				game_2880_h(ctx, base);
				// subf r11,r29,r28
				ctx.r11.s64 = (int64_t)(int32_t)var_r28 - (int64_t)(int32_t)var_r29;
				// cmpw cr6,r11,r3
				ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r3.s32, ctx.xer);
				// b 0x82455500
				} else {
				loc_824554C0:
				// rlwinm r9,r10,0,25,25
				ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x40;
				// cmplwi cr6,r9,64
				// bne cr6,0x824554e8
				if (ctx.r9.u32 == 64) {
				// lwz r8,32(r30)
				ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 32);
				// lwz r29,24(r11)
				var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + 24));
				// lwz r28,24(r8)
				var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r8.u32 + 24));
				// bl 0x82452880
				game_2880_h(ctx, base);
				// subf r7,r29,r28
				ctx.r7.s64 = (int64_t)(int32_t)var_r28 - (int64_t)(int32_t)var_r29;
				// cmpw cr6,r7,r3
				ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r3.s32, ctx.xer);
				// b 0x82455500
				} else {
				loc_824554E8:
				// lwz r6,32(r30)
				ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 32);
				// lwz r29,28(r11)
				var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + 28));
				// lwz r28,24(r6)
				var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 24));
				// bl 0x82452880
				game_2880_h(ctx, base);
				// subf r5,r29,r28
				ctx.r5.s64 = (int64_t)(int32_t)var_r28 - (int64_t)(int32_t)var_r29;
				// cmpw cr6,r5,r3
				ctx.cr6.compare<int32_t>(ctx.r5.s32, ctx.r3.s32, ctx.xer);
				}
				}
				loc_82455500:
			} else {
				if (!(ctx.cr6.lt)) {
					// lwz r4,4(r31)
					ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */;
					// mr r3,r31
					ctx.r3.u64 = var_r31;
					// lwz r11,8(r31)
					ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
					// stw r11,8(r4)
					PPC_STORE_U32(ctx.r4.u32 + 8, ctx.r11.u32);
					// lwz r10,8(r31)
					ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
					// lwz r9,4(r31)
					ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 4)/* msgMsgSink::flags@+0x4 */;
					// stw r9,4(r10)
					PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r9.u32);
					// stw r31,4(r31)
					PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ var_r31);
					// stw r31,8(r31)
					PPC_STORE_U32(var_r31 + 8, var_r31);
					// bl 0x824528a8
					atSingleton_28A8_g_28A8_1(ctx, base);
				}
			}
		loc_8245552C:
			// mr r31,r27
			var_r31 = (uint32_t)(var_r27);
			// cmplw cr6,r27,r26
			// bne cr6,0x82455474
	}
	}
loc_82455538:
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_14"))) PPC_WEAK_FUNC(msgMsgSink_vfn_14);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r3,r3,-4
	ctx.r3.s64 = ctx.r3.s64 + -4;
	// b 0x82455098
	msgMsgSink_5098_g(ctx, base);
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_5550_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_5550_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_5550_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82454cf0
	snNotifyGamerAdded_4CF0_fw(ctx, base);
	// lis r11,-32256
	// lis r10,-32256
	// addi r11,r11,20172
	ctx.r11.s64 = ctx.r11.s64 + 20172;
	// addi r10,r10,20200
	ctx.r10.s64 = ctx.r10.s64 + 20200;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_5598_2hr"))) PPC_WEAK_FUNC(msgMsgSink_5598_2hr);
PPC_FUNC_IMPL(__imp__msgMsgSink_5598_2hr) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // msgMsgSink::vfn_4 (unnamed)  // vtable slot 4 (byte +16)
	// lwz r9,28(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 28);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lbz r11,0(r9)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// rlwinm r7,r11,0,30,30
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r7,2
	// beq cr6,0x824555f0
	if (ctx.r7.u32 != 2) {
		// rlwinm r5,r11,0,29,29
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4;
		// cmplwi cr6,r5,4
		// beq cr6,0x824555f0
		if (ctx.r5.u32 == 4) goto loc_824555F0;
		// rlwinm r3,r11,0,28,28
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
		// cmplwi cr6,r3,8
		// bne cr6,0x8245560c
		if (ctx.r3.u32 != 8) goto loc_8245560C;
	}
loc_824555F0:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lhz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 0);
	// b 0x82455610
	// lhzx r3,r11,r30
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r11.u32 + var_r30);
	// blr
	return;
loc_8245560C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82455610:
	// lhzx r3,r11,r30
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r11.u32 + var_r30);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_12"))) PPC_WEAK_FUNC(msgMsgSink_vfn_12);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_12) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// lhz r3,10(r11)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r11.u32 + 10);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_13"))) PPC_WEAK_FUNC(msgMsgSink_vfn_13);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r31,28(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 28));
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lhz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 0);
	// add r9,r11,r31
	ctx.r9.u64 = ctx.r11.u64 + var_r31;
	// lhz r3,10(r9)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_11"))) PPC_WEAK_FUNC(msgMsgSink_vfn_11);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_11) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// addi r3,r11,10
	ctx.r3.s64 = ctx.r11.s64 + 10;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_8"))) PPC_WEAK_FUNC(msgMsgSink_vfn_8);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20172
	ctx.r11.s64 = ctx.r11.s64 + 20172;
	// addi r10,r10,20200
	ctx.r10.s64 = ctx.r10.s64 + 20200;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// bl 0x824543a8
	msgMsgSink_43A8_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824556f0
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32789
		ctx.r4.u64 = ctx.r4.u64 | 32789;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824556F0:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_5708_fw"))) PPC_WEAK_FUNC(msgMsgSink_5708_fw);
PPC_FUNC_IMPL(__imp__msgMsgSink_5708_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// lwz r11,28(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 28);
	// lbz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r11,0,30,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r9,2
	// beq cr6,0x82455748
	if (ctx.r9.u32 != 2) {
		// rlwinm r7,r11,0,29,29
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4;
		// cmplwi cr6,r7,4
		// beq cr6,0x82455748
		if (ctx.r7.u32 == 4) goto loc_82455748;
		// rlwinm r5,r11,0,28,28
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
		// cmplwi cr6,r5,8
		// bne cr6,0x82455764
		if (ctx.r5.u32 != 8) goto loc_82455764;
	}
loc_82455748:
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,16(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lhz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 0);
	// b 0x82455768
	goto loc_82455768;
loc_82455764:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82455768:
	// lwz r10,28(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 28);
	// clrlwi r30,r11,16
	var_r30 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
	// lbz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// rlwinm r8,r9,0,27,27
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r8,16
	// bne cr6,0x82455790
	if (ctx.r8.u32 == 16) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82455598
		msgMsgSink_5598_2hr(ctx, base);
		// clrlwi r9,r3,16
		ctx.r9.u64 = ctx.r3.u32 & 0xFFFF;
		// b 0x82455794
	} else {
	loc_82455790:
		// li r9,0
		ctx.r9.s64 = 0;
	}
loc_82455794:
	// clrlwi r11,r29,16
	ctx.r11.u64 = var_r29 & 0xFFFF;
	// lwz r10,28(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 28);
	// clrlwi r8,r9,16
	ctx.r8.u64 = ctx.r9.u32 & 0xFFFF;
	// addi r11,r11,2
	ctx.r11.s64 = ctx.r11.s64 + 2;
	// clrlwi r9,r30,16
	ctx.r9.u64 = var_r30 & 0xFFFF;
	// rlwinm r7,r11,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r7
	ctx.r11.u64 = ctx.r11.u64 + ctx.r7.u64;
	// add r11,r11,r8
	ctx.r11.u64 = ctx.r11.u64 + ctx.r8.u64;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_9"))) PPC_WEAK_FUNC(msgMsgSink_vfn_9);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_9) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_25
	// mr r27,r4
	var_r27 = ctx.r4.u32;
	// li r4,2
	ctx.r4.s64 = 2;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r28,0
	var_r28 = 0;
	// li r25,1
	var_r25 = 1;
	// bl 0x824546b8
	msgMsgSink_46B8_g(ctx, base);
	// li r31,0
	var_r31 = 0;
loc_824557F0:
	// lwz r11,28(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
	// clrlwi r26,r31,24
	var_r26 = (uint32_t)(var_r31 & 0xFF);
	// lbz r10,9(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 9);
	// cmplw cr6,r26,r10
	// bge cr6,0x82455904
	if (var_r26 >= ctx.r10.u32) goto loc_82455904;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,64
	ctx.r3.s64 = 64;
	// ori r4,r4,32772
	ctx.r4.u64 = ctx.r4.u64 | 32772;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// cmplwi cr6,r29,0
	// beq cr6,0x82455894
	if (var_r29 == 0) goto loc_82455894;
	// clrlwi r4,r31,24
	ctx.r4.u64 = var_r31 & 0xFF;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82455708
	msgMsgSink_5708_fw(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// mr r7,r31
	ctx.r7.u64 = var_r31;
	// lwz r4,32(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 32);
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x8245a4f8
	msgMsgSink_A4F8_g(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// bne cr6,0x8245589c
	if (var_r31 != 0) goto loc_8245589C;
loc_82455850:
	// lis r28,-32761
	var_r28 = (uint32_t)(-2147024896);
	// ori r28,r28,14
	var_r28 = (uint32_t)(var_r28 | 14);
loc_82455858:
	// cmplwi cr6,r31,0
	// beq cr6,0x82455878
	if (var_r31 == 0) goto loc_82455878;
	// lwz r6,0(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r5,0(r6)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
loc_82455878:
	// addi r4,r26,1
	ctx.r4.s64 = (int64_t)(int32_t)var_r26 + 1;
	// cmpwi cr6,r28,0
	// clrlwi r31,r4,24
	var_r31 = (uint32_t)(ctx.r4.u32 & 0xFF);
	// bge cr6,0x824557f0
	if ((int32_t)var_r28 >= 0) goto loc_824557F0;
loc_82455888:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	return;
loc_82455894:
	// li r31,0
	var_r31 = 0;
	// b 0x82455850
	goto loc_82455850;
loc_8245589C:
	// cmpwi cr6,r28,0
	// blt cr6,0x82455858
	if ((int32_t)var_r28 < 0) goto loc_82455858;
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r5,r27
	ctx.r5.u64 = var_r27;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r4,1(r9)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1);
	// bl 0x8245a2c0
	rage_A2C0(ctx, base);
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// cmpwi cr6,r28,0
	// blt cr6,0x82455858
	if ((int32_t)var_r28 < 0) goto loc_82455858;
	// stw r27,104(r30)
	PPC_STORE_U32(var_r30 + 104, var_r27);
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x824558e0
	if (ctx.r11.u32 != 0) {
		// lwz r11,40(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
		// cmpwi cr6,r11,3
		// beq cr6,0x824558e4
		if (ctx.r11.s32 == 3) goto loc_824558E4;
	}
loc_824558E0:
	// li r25,0
	var_r25 = 0;
loc_824558E4:
	// addi r11,r30,36
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 36;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r11.u32);
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r8,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r31,4(r7)
	PPC_STORE_U32(ctx.r7.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// b 0x82455878
	goto loc_82455878;
loc_82455904:
	// cmpwi cr6,r28,0
	// blt cr6,0x82455888
	if ((int32_t)var_r28 < 0) {
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		return;
	}
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82454960
	rage_4960(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r31,0
	// blt cr6,0x82455934
	if ((int32_t)var_r31 >= 0) {
		// cmpwi cr6,r25,0
		// beq cr6,0x82455934
		if ((int32_t)var_r25 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			return;
		}
		// li r4,3
		ctx.r4.s64 = 3;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x824546b8
		msgMsgSink_46B8_g(ctx, base);
	}
loc_82455934:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_5940_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_5940_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_5940_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82454cf0
	snNotifyGamerAdded_4CF0_fw(ctx, base);
	// lis r11,-32256
	// lis r10,-32256
	// addi r11,r11,20208
	ctx.r11.s64 = ctx.r11.s64 + 20208;
	// addi r10,r10,20200
	ctx.r10.s64 = ctx.r10.s64 + 20200;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_21_5988_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_21_5988_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_21_5988_1) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// lhz r3,12(r11)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_22_5998_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_22_5998_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_22_5998_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r31,28(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 28));
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lhz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 0);
	// add r9,r11,r31
	ctx.r9.u64 = ctx.r11.u64 + var_r31;
	// lhz r3,12(r9)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r9.u32 + 12);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_20_59E0_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_20_59E0_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_20_59E0_1) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// addi r3,r11,12
	ctx.r3.s64 = ctx.r11.s64 + 12;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_17"))) PPC_WEAK_FUNC(msgMsgSink_vfn_17);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_17) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20208
	ctx.r11.s64 = ctx.r11.s64 + 20208;
	// addi r10,r10,20200
	ctx.r10.s64 = ctx.r10.s64 + 20200;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// bl 0x824543a8
	msgMsgSink_43A8_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82455a48
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32790
		ctx.r4.u64 = ctx.r4.u64 | 32790;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82455A48:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_19"))) PPC_WEAK_FUNC(msgMsgSink_vfn_19);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_19) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// lwz r10,100(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 100);
	// lwz r9,104(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 104);
	// subf r31,r9,r10
	var_r31 = (uint32_t)(ctx.r10.s64 - ctx.r9.s64);
	// lwz r8,24(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r3,36(r8)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 36);
	// lwz r6,12(r7)
	// bctrl
	VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
	// add r3,r31,r3
	ctx.r3.u64 = var_r31 + ctx.r3.u64;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_18"))) PPC_WEAK_FUNC(msgMsgSink_vfn_18);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_18) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// li r4,2
	ctx.r4.s64 = 2;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x824546b8
	msgMsgSink_46B8_g(ctx, base);
	// addi r31,r30,128
	var_r31 = (uint32_t)(var_r30 + 128);
	// li r11,1
	ctx.r11.s64 = 1;
	// li r29,0
	var_r29 = 0;
	// li r10,255
	ctx.r10.s64 = 255;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r3,64
	ctx.r3.s64 = 64;
	// ori r4,r4,32772
	ctx.r4.u64 = ctx.r4.u64 | 32772;
	// rlwimi r9,r11,0,27,31
	ctx.r9.u64 = (ctx.r11.u32 & 0x1F) | (ctx.r9.u64 & 0xFFFFFFFFFFFFFFE0);
	// rotlwi r8,r9,0
	ctx.r8.u64 = ctx.r9.u32;
	// rlwinm r7,r8,0,27,2
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFFE000001F;
	// stw r9,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r9.u32);
	// rotlwi r6,r7,0
	ctx.r6.u64 = ctx.r7.u32;
	// oris r5,r6,8192
	ctx.r5.u64 = ctx.r6.u64 | 536870912;
	// stw r7,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r7.u32);
	// stw r5,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r5.u32);
	// sth r29,4(r31)
	PPC_STORE_U16(var_r31 + 4, (uint16_t)var_r29);
	// stb r10,6(r31)
	PPC_STORE_U8(var_r31 + 6, ctx.r10.u8);
	// lwz r11,28(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
	// lhz r10,9(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 9);
	// sth r10,7(r31)
	PPC_STORE_U16(var_r31 + 7, ctx.r10.u16);
	// lwz r9,28(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 28);
	// lbz r8,11(r9)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 11);
	// stb r8,9(r31)
	PPC_STORE_U8(var_r31 + 9, ctx.r8.u8);
	// stb r29,10(r31)
	PPC_STORE_U8(var_r31 + 10, (uint8_t)var_r29);
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82455b5c
	if (ctx.r3.u32 != 0) {
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r4,32(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 32);
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// bl 0x8245a4f8
		msgMsgSink_A4F8_g(ctx, base);
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// cmplwi cr6,r29,0
		// bne cr6,0x82455bd0
		if (var_r29 != 0) goto loc_82455BD0;
	}
loc_82455B5C:
	// lis r31,-32761
	var_r31 = (uint32_t)(-2147024896);
	// ori r31,r31,14
	var_r31 = (uint32_t)(var_r31 | 14);
loc_82455B64:
	// cmplwi cr6,r29,0
	// beq cr6,0x82455b84
	if (var_r29 == 0) goto loc_82455B84;
	// lwz r5,0(r29)
	ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lwz r11,0(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
loc_82455B84:
	// cmpwi cr6,r31,0
	// blt cr6,0x82455bc4
	if ((int32_t)var_r31 >= 0) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82454960
		rage_4960(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmpwi cr6,r31,0
		// blt cr6,0x82455bc4
		if ((int32_t)var_r31 < 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			return;
		}
		// lwz r11,24(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 24);
		// cmplwi cr6,r11,0
		// beq cr6,0x82455bc4
		if (ctx.r11.u32 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			return;
		}
		// lwz r11,40(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
		// cmpwi cr6,r11,3
		// bne cr6,0x82455bc4
		if (ctx.r11.s32 != 3) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			return;
		}
		// li r4,3
		ctx.r4.s64 = 3;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x824546b8
		msgMsgSink_46B8_g(ctx, base);
	}
loc_82455BC4:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
loc_82455BD0:
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x8245a1f0
	rage_A1F0(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r31,0
	// blt cr6,0x82455b64
	if ((int32_t)var_r31 < 0) goto loc_82455B64;
	// addi r11,r30,36
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 36;
	// stw r28,104(r30)
	PPC_STORE_U32(var_r30 + 104, var_r28);
	// stw r11,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r11.u32);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r7,8(r29)
	PPC_STORE_U32(var_r29 + 8, ctx.r7.u32);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r29,4(r6)
	PPC_STORE_U32(ctx.r6.u32 + 4, var_r29);
	// stw r29,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r29);
	// b 0x82455b84
	goto loc_82455B84;
}

__attribute__((alias("__imp__atSingleton_5C10_g"))) PPC_WEAK_FUNC(atSingleton_5C10_g);
PPC_FUNC_IMPL(__imp__atSingleton_5C10_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// addi r11,r3,4
	ctx.r11.s64 = ctx.r3.s64 + 4;
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// stb r9,0(r31)
	PPC_STORE_U8(var_r31 + 0, ctx.r9.u8);
	// lbz r8,1(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1);
	// stb r8,1(r31)
	PPC_STORE_U8(var_r31 + 1, ctx.r8.u8);
	// lbz r7,2(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
	// stb r7,2(r31)
	PPC_STORE_U8(var_r31 + 2, ctx.r7.u8);
	// lbz r6,3(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3);
	// stb r6,3(r31)
	PPC_STORE_U8(var_r31 + 3, ctx.r6.u8);
	// blr
	return;
}

__attribute__((alias("__imp__game_5C70_h"))) PPC_WEAK_FUNC(game_5C70_h);
PPC_FUNC_IMPL(__imp__game_5C70_h) {
	PPC_FUNC_PROLOGUE();
	// mr r9,r5
	ctx.r9.u64 = ctx.r5.u64;
	// cmpwi cr6,r9,0
	// blelr cr6
	if (ctx.r9.s32 <= 0) return;
	// addi r11,r3,20
	ctx.r11.s64 = ctx.r3.s64 + 20;
	// addi r10,r4,8
	ctx.r10.s64 = ctx.r4.s64 + 8;
loc_82455C84:
	// lwz r8,-4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + -4);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stw r8,-8(r10)
	PPC_STORE_U32(ctx.r10.u32 + -8, ctx.r8.u32);
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// stw r6,-4(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4, ctx.r6.u32);
	// lwz r5,0(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r4,r5,30,30,31
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 30) & 0x3;
	// stb r4,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r4.u8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// clrlwi r8,r3,30
	ctx.r8.u64 = ctx.r3.u32 & 0x3;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// stb r8,1(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1, ctx.r8.u8);
	// addi r10,r10,12
	ctx.r10.s64 = ctx.r10.s64 + 12;
	// bne cr6,0x82455c84
	if (!ctx.cr6.eq) goto loc_82455C84;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_5CD0_fw"))) PPC_WEAK_FUNC(atSingleton_5CD0_fw);
PPC_FUNC_IMPL(__imp__atSingleton_5CD0_fw) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=128, manual
	// mr r8,r3
	ctx.r8.u64 = ctx.r3.u64;
	// addi r11,r1,81
	ctx.r11.s64 = ctx.r1.s64 + 81;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r10,28
	ctx.r10.s64 = 28;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82455CF0:
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bdnz 0x82455cf0
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82455CF0;
	// lwz r11,104(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
	// srawi r6,r8,24
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0xFFFFFF) != 0);
	ctx.r6.s64 = ctx.r8.s32 >> 24;
	// lbz r9,108(r1)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r1.u32 + 108);
	// rldicl r5,r8,56,8
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r8.u64, 56) & 0xFFFFFFFFFFFFFF;
	// rldicr r10,r11,8,63
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u64, 8);
	// stb r4,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r4.u8);
	// stb r8,92(r1)
	PPC_STORE_U8(ctx.r1.u32 + 92, ctx.r8.u8);
	// or r7,r9,r10
	ctx.r7.u64 = ctx.r9.u64 | ctx.r10.u64;
	// lwz r3,20(r8)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 20);
	// stb r6,89(r1)
	PPC_STORE_U8(ctx.r1.u32 + 89, ctx.r6.u8);
	// rldicr r7,r7,24,39
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u64, 24) & 0xFFFFFFFFFF000000;
	// sth r5,90(r1)
	PPC_STORE_U16(ctx.r1.u32 + 90, ctx.r5.u16);
	// ld r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// ld r5,88(r1)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
	// ld r6,96(r1)
	ctx.r6.u64 = PPC_LOAD_U64(ctx.r1.u32 + 96);
	// bl 0x82449980
	atSingleton_9980(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_25_5D50_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_25_5D50_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_25_5D50_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,236(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 236);
	// cntlzw r10,r11
	ctx.r10.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r9,r10,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// li r10,2
	ctx.r10.s64 = 2;
	// cntlzw r8,r9
	ctx.r8.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// rlwinm r11,r8,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0x1;
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// lwz r7,252(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 252);
	// cmpwi cr6,r7,0
	// bne cr6,0x82455da0
	if (ctx.r7.s32 == 0) {
		// mr r10,r11
		ctx.r10.u64 = ctx.r11.u64;
	}
loc_82455DA0:
	// or r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 | ctx.r10.u64;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// stw r6,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r6.u32);
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_24_5DC0_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_24_5DC0_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_24_5DC0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 20);
	// addi r28,r11,144
	var_r28 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// bl 0x824484c0
	msgMsgSink_84C0_gen(ctx, base);
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// cmpwi cr6,r27,0
	// blt cr6,0x82455e9c
	if ((int32_t)var_r27 >= 0) {
		// lwz r11,236(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 236);
		// cntlzw r10,r11
		ctx.r10.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
		// rlwinm r11,r10,27,31,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
		// cmpwi cr6,r11,0
		// bne cr6,0x82455e50
		if (ctx.r11.s32 == 0) {
			// addi r29,r30,24
			var_r29 = (uint32_t)(var_r30 + 24);
			// addi r31,r30,4
			var_r31 = (uint32_t)(var_r30 + 4);
		loc_82455E10:
			// lwz r11,204(r29)
			ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 204);
			// cmplwi cr6,r11,0
			// beq cr6,0x82455e24
			if (ctx.r11.u32 != 0) {
				// lwz r3,0(r11)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
				// b 0x82455e28
			} else {
			loc_82455E24:
				// li r3,0
				ctx.r3.s64 = 0;
			}
		loc_82455E28:
			// lwz r9,0(r3)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// lwz r8,0(r9)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
			// lwz r7,236(r30)
			ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 236);
			// cntlzw r6,r7
			ctx.r6.u64 = ctx.r7.u32 == 0 ? 32 : __builtin_clz(ctx.r7.u32);
			// rlwinm r11,r6,27,31,31
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0x1;
			// cmpwi cr6,r11,0
			// beq cr6,0x82455e10
			if (ctx.r11.s32 == 0) goto loc_82455E10;
		}
	loc_82455E50:
		// lwz r3,244(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 244);
		// addi r29,r30,240
		var_r29 = (uint32_t)(var_r30 + 240);
		// cmplw cr6,r3,r29
		// beq cr6,0x82455e84
	while (var_r31 != var_r29) {
		loc_82455E60:
			// lwz r5,0(r3)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// lwz r31,4(r3)
			var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */);
			// lwz r11,0(r5)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// cmplw cr6,r31,r29
			// bne cr6,0x82455e60
	}
	loc_82455E84:
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r9,0(r10)
		// bctrl
		DTOR(ctx.r3.u32, ctx, base);  // vtable slot 0 (destructor)
	}
loc_82455E9C:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__msgMsgSink_5EB0_g"))) PPC_WEAK_FUNC(msgMsgSink_5EB0_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_5EB0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// lis r28,-32761
	var_r28 = (uint32_t)(-2147024896);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// ori r28,r28,87
	var_r28 = (uint32_t)(var_r28 | 87);
	// cmplwi cr6,r29,0
	// beq cr6,0x82455f34
	if (var_r29 != 0) {
		// lwz r10,24(r11)
		// bctrl
		VCALL(ctx.r3.u32, 6, ctx, base);  // msgMsgSink::vfn_6 (unnamed)  // vtable slot 6 (byte +24)
		// clrlwi r9,r30,16
		ctx.r9.u64 = var_r30 & 0xFFFF;
		// cmplw cr6,r9,r3
		// bge cr6,0x82455f34
		if (ctx.r9.u32 >= ctx.r3.u32) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			return;
		}
		// lwz r8,0(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// sth r30,84(r1)
		PPC_STORE_U16(ctx.r1.u32 + 84, (uint16_t)var_r30);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// lwz r7,16(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r7.u32);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x82451bb8
		game_1BB8_h(ctx, base);
		// ld r6,80(r1)
		ctx.r6.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// li r3,0
		ctx.r3.s64 = 0;
		// std r6,40(r29)
		PPC_STORE_U64(var_r29 + 40, ctx.r6.u64);
		return;
	}
loc_82455F34:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	return;
}

__attribute__((alias("__imp__msgMsgSink_5F40_g"))) PPC_WEAK_FUNC(msgMsgSink_5F40_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_5F40_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=176, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// bl 0x82451b98
	msgMsgSink_1B98_g(ctx, base);
	// addi r11,r1,120
	ctx.r11.s64 = ctx.r1.s64 + 120;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// bl 0x82455eb0
	msgMsgSink_5EB0_g(ctx, base);
	// addi r4,r1,120
	ctx.r4.s64 = ctx.r1.s64 + 120;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451fd0
	atSingleton_1FD0_p42(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451de8
	game_1DE8_h(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r4,r1,120
	ctx.r4.s64 = ctx.r1.s64 + 120;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stw r11,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824520f0
	msgMsgSink_20F0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451de8
	game_1DE8_h(ctx, base);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r10,0(r28)
	PPC_STORE_U32(var_r28 + 0, ctx.r10.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_36_5FD0_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_36_5FD0_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_36_5FD0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// addi r30,r3,240
	var_r30 = (uint32_t)(ctx.r3.s64 + 240);
	// lwz r3,244(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 244);
	// cmplw cr6,r3,r30
	// beq cr6,0x82456014
while (var_r31 != var_r30) {
	loc_82455FF4:
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// lwz r31,4(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */);
		// lwz r10,24(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplw cr6,r31,r30
		// bne cr6,0x82455ff4
}
loc_82456014:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_37_6030_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_37_6030_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_37_6030_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// addi r30,r3,240
	var_r30 = (uint32_t)(ctx.r3.s64 + 240);
	// lwz r3,244(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 244);
	// cmplw cr6,r3,r30
	// beq cr6,0x82456068
while (var_r31 != var_r30) {
	loc_82456054:
		// lwz r31,4(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 4)/* msgMsgSink::flags@+0x4 */);
		// bl 0x8244aae8
		atSingleton_AAE8(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplw cr6,r31,r30
		// bne cr6,0x82456054
}
loc_82456068:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_6080_p33"))) PPC_WEAK_FUNC(msgMsgSink_6080_p33);
PPC_FUNC_IMPL(__imp__msgMsgSink_6080_p33) {
	PPC_FUNC_PROLOGUE();
	// li r11,1
	ctx.r11.s64 = 1;
	// li r4,14
	ctx.r4.s64 = 14;
	// stw r11,252(r3)
	PPC_STORE_U32(ctx.r3.u32 + 252, ctx.r11.u32);
	// b 0x82455cd0
	atSingleton_5CD0_fw(ctx, base);
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_6090_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_6090_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_6090_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r9,-32256
	// lis r10,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r9,15816
	var_r30 = (uint32_t)(ctx.r9.s64 + 15816);  // lbl_82003DC8 @ 0x82003dc8
	// lis r9,-32256
	// lis r8,-32256
	// lis r7,-32256
	// addi r10,r10,17168
	ctx.r10.s64 = ctx.r10.s64 + 17168;
	// addi r11,r31,8
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 8;
	// addi r9,r9,20240
	ctx.r9.s64 = ctx.r9.s64 + 20240;
	// addi r8,r8,20456
	ctx.r8.s64 = ctx.r8.s64 + 20456;
	// addi r7,r7,20236
	ctx.r7.s64 = ctx.r7.s64 + 20236;
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r30,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r30);
	// stw r11,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	// stw r9,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r9.u32);
	// stw r8,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
	// stw r7,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r7.u32);
	// stw r4,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r4.u32);
	// bl 0x8244c158
	snNotifyGamerAdded_C158_2h(ctx, base);
	// addi r11,r31,240
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 240;
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r30,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r30);
	// stw r11,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	// stw r10,252(r31)
	PPC_STORE_U32(var_r31 + 252, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_23_6130_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_23_6130_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_23_6130_1) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-8
	ctx.r3.s64 = ctx.r3.s64 + -8;
	// b 0x82456220
	msgMsgSink_6220_w(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_6138_2hr"))) PPC_WEAK_FUNC(msgMsgSink_6138_2hr);
PPC_FUNC_IMPL(__imp__msgMsgSink_6138_2hr) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// addi r30,r31,8
	var_r30 = (uint32_t)(var_r31 + 8);
	// addi r11,r11,20240
	ctx.r11.s64 = ctx.r11.s64 + 20240;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,20236
	ctx.r9.s64 = ctx.r9.s64 + 20236;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r9.u32);
	// lwz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 4)/* msgMsgSink::flags@+0x4 */;
	// cmplw cr6,r11,r30
	// beq cr6,0x824561a8
	if (ctx.r11.u32 != var_r30) {
		// lwz r10,8(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 8);
		// cmplw cr6,r10,r30
		// beq cr6,0x824561a8
		if (ctx.r10.u32 == var_r30) goto loc_824561A8;
		// rotlwi r9,r10,0
		ctx.r9.u64 = ctx.r10.u32;
		// rotlwi r8,r11,0
		ctx.r8.u64 = ctx.r11.u32;
		// stw r9,8(r8)
		PPC_STORE_U32(ctx.r8.u32 + 8, ctx.r9.u32);
		// lwz r7,8(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 8);
		// lwz r6,4(r30)
		ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 4)/* msgMsgSink::flags@+0x4 */;
		// stw r6,4(r7)
		PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r6.u32);
		// stw r30,4(r30)
		PPC_STORE_U32(var_r30 + 4,/* msgMsgSink::flags@+0x4 */ var_r30);
		// stw r30,8(r30)
		PPC_STORE_U32(var_r30 + 8, var_r30);
	}
loc_824561A8:
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x824489f8
	atSingleton_89F8_2hr(ctx, base);
	// li r4,7
	ctx.r4.s64 = 7;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82455cd0
	atSingleton_5CD0_fw(ctx, base);
	// lis r11,-32256
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// addi r29,r11,15816
	var_r29 = (uint32_t)(ctx.r11.s64 + 15816);  // lbl_82003DC8 @ 0x82003dc8
	// stw r29,240(r31)
	PPC_STORE_U32(var_r31 + 240, var_r29);
	// lwz r5,204(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 204);
	// cmplwi cr6,r5,0
	// beq cr6,0x82456200
while (!ctx.cr6.eq) {
	loc_824561DC:
		// lwz r11,204(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 204);
		// lwz r4,0(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// lwz r10,4(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r4,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r4.u32);
		// stw r11,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
		// rotlwi r11,r10,0
		ctx.r11.u64 = ctx.r10.u32;
		// cmplwi cr6,r11,0
		ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
		// stw r10,204(r3)
		PPC_STORE_U32(ctx.r3.u32 + 204, ctx.r10.u32);
		// bne cr6,0x824561dc
}
loc_82456200:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,212(r3)
	PPC_STORE_U32(ctx.r3.u32 + 212, ctx.r11.u32);
	// stw r11,208(r3)
	PPC_STORE_U32(ctx.r3.u32 + 208, ctx.r11.u32);
	// bl 0x8244ea00
	msgMsgSink_EA00(ctx, base);
	// stw r29,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ var_r29);
	return;
}

__attribute__((alias("__imp__msgMsgSink_6220_w"))) PPC_WEAK_FUNC(msgMsgSink_6220_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_6220_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x82456138
	msgMsgSink_6138_2hr(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82456260
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32865
		ctx.r4.u64 = ctx.r4.u64 | 32865;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82456260:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_75"))) PPC_WEAK_FUNC(msgMsgSink_vfn_75);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_75) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r9,80(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// stw r10,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x824562ec
	if ((int32_t)var_r30 >= 0) {
		// lwz r8,48(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 48);
		// li r6,0
		ctx.r6.s64 = 0;
		// addi r5,r1,80
		ctx.r5.s64 = ctx.r1.s64 + 80;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,1(r8)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r8.u32 + 1);
		// bl 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x824562ec
		if ((int32_t)var_r30 < 0) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lwz r7,80(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r7,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r7.u32);
	}
loc_824562EC:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_6300_fw"))) PPC_WEAK_FUNC(msgMsgSink_6300_fw);
PPC_FUNC_IMPL(__imp__msgMsgSink_6300_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// stw r10,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
	// lwz r9,80(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
	// cmpwi cr6,r3,0
	// blt cr6,0x82456360
	if (ctx.r3.s32 >= 0) {
		// li r6,0
		ctx.r6.s64 = 0;
		// addi r5,r1,80
		ctx.r5.s64 = ctx.r1.s64 + 80;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x82456360
		if (ctx.r3.s32 < 0) {
			// blr
			return;
		}
		// lwz r8,80(r1)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r8,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r8.u32);
	}
loc_82456360:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_109"))) PPC_WEAK_FUNC(msgMsgSink_vfn_109);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_109) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,80(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// li r9,-1
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// stw r9,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	// blt cr6,0x82456454
	if ((int32_t)var_r30 >= 0) {
		// lwz r11,48(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 48);
		// lbz r8,0(r11)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// rlwinm r7,r8,0,29,29
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x4;
		// cmplwi cr6,r7,0
		// beq cr6,0x824563e0
		if (ctx.r7.u32 != 0) {
			// lwz r6,1(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1);
			// stw r6,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r6.u32);
			// b 0x82456454
		} else {
		loc_824563E0:
			// lis r4,8324
			ctx.r4.s64 = 545521664;
			// li r3,16
			ctx.r3.s64 = 16;
			// ori r4,r4,32791
			ctx.r4.u64 = ctx.r4.u64 | 32791;
			// bl 0x820c01b8
			rage_01B8(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// cmplwi cr6,r30,0
			// beq cr6,0x8245642c
			if (var_r30 != 0) {
				// lwz r3,284(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 284);
				// lwz r5,48(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 48);
				// lwz r11,0(r3)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
				// lwz r4,1(r5)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 1);
				// lwz r10,32(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
				// bctrl
				PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
				// mr r5,r3
				ctx.r5.u64 = ctx.r3.u64;
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// lwz r4,56(r31)
				ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 56);
				// bl 0x8245b110
				msgMsgSink_B110_g(ctx, base);
				// b 0x82456430
			} else {
			loc_8245642C:
				// li r3,0
				ctx.r3.s64 = 0;
			}
		loc_82456430:
			// cmplwi cr6,r3,0
			// stw r3,288(r31)
			PPC_STORE_U32(var_r31 + 288, ctx.r3.u32);
			// beq cr6,0x8245644c
			if (ctx.r3.u32 != 0) {
				// addi r4,r1,80
				ctx.r4.s64 = ctx.r1.s64 + 80;
				// bl 0x8245b3e0
				msgMsgSink_B3E0_wrh(ctx, base);
				// mr r30,r3
				var_r30 = ctx.r3.u32;
				// b 0x82456454
			} else {
			loc_8245644C:
				// lis r30,-32761
				var_r30 = (uint32_t)(-2147024896);
				// ori r30,r30,14
				var_r30 = (uint32_t)(var_r30 | 14);
			}
		}
	}
loc_82456454:
	// li r9,0
	ctx.r9.s64 = 0;
	// cmpwi cr6,r30,0
	// stw r9,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// blt cr6,0x8245648c
	if ((int32_t)var_r30 >= 0) {
		// li r6,0
		ctx.r6.s64 = 0;
		// lwz r4,80(r1)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// addi r5,r1,84
		ctx.r5.s64 = ctx.r1.s64 + 84;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245648c
		if ((int32_t)var_r30 < 0) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lwz r8,84(r1)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// stw r8,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r8.u32);
	}
loc_8245648C:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_120"))) PPC_WEAK_FUNC(msgMsgSink_vfn_120);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_120) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,48(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 48);
	// li r4,2
	ctx.r4.s64 = 2;
	// addi r3,r3,24
	ctx.r3.s64 = ctx.r3.s64 + 24;
	// lbz r10,14(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 14);
	// lhz r5,10(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 10);
	// clrlwi r6,r10,30
	ctx.r6.u64 = ctx.r10.u32 & 0x3;
	// b 0x82453a48
	jumptable_3A48(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_60"))) PPC_WEAK_FUNC(msgMsgSink_vfn_60);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_60) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,348(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 348);
	// lwz r3,344(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 344);
	// cmpwi cr6,r11,0
	// beqlr cr6
	if (ctx.r11.s32 == 0) return;
	// ori r3,r3,64
	ctx.r3.u64 = ctx.r3.u64 | 64;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_64D8_g"))) PPC_WEAK_FUNC(atSingleton_64D8_g);
PPC_FUNC_IMPL(__imp__atSingleton_64D8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,364(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 364);
	// cmpwi cr6,r11,0
	// beq cr6,0x82456538
	if (ctx.r11.s32 != 0) {
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// lwz r11,372(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 372);
		// cmpwi cr6,r11,0
		// subf r11,r11,r3
		ctx.r11.s64 = ctx.r3.s64 - ctx.r11.s64;
		// bgt cr6,0x82456510
		if (ctx.r11.s32 <= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_82456510:
		// lwz r10,368(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 368);
		// lwz r9,364(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 364);
		// subf r8,r10,r3
		ctx.r8.s64 = ctx.r3.s64 - ctx.r10.s64;
		// subf r7,r11,r8
		ctx.r7.s64 = ctx.r8.s64 - ctx.r11.s64;
		// subf r3,r9,r7
		ctx.r3.s64 = ctx.r7.s64 - ctx.r9.s64;
		// blr
		return;
	}
loc_82456538:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_6550_fw"))) PPC_WEAK_FUNC(atSingleton_6550_fw);
PPC_FUNC_IMPL(__imp__atSingleton_6550_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,364(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 364);
	// cmpwi cr6,r11,0
	// beq cr6,0x82456594
	if (ctx.r11.s32 != 0) {
		// bl 0x824564d8
		atSingleton_64D8_g(ctx, base);
		// lwz r10,56(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 56);
		// lwz r11,40(r10)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 40);
		// add r3,r11,r3
		ctx.r3.u64 = ctx.r11.u64 + ctx.r3.u64;
		// blr
		return;
	}
loc_82456594:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_65B0_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_65B0_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_65B0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8244ed68
	snNotifyGamerAdded_ED68(ctx, base);
	// lis r11,-32256
	// lis r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,15908
	ctx.r9.s64 = ctx.r11.s64 + 15908;
	// lis r11,-32256
	// ori r10,r10,65535
	ctx.r10.u64 = ctx.r10.u64 | 65535;
	// li r29,1
	var_r29 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r9,340(r31)
	PPC_STORE_U32(var_r31 + 340, ctx.r9.u32);
	// lfs f0,15784(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	// stfs f0,356(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 356, temp.u32);
	// sth r10,404(r31)
	PPC_STORE_U16(var_r31 + 404, ctx.r10.u16);
	// addi r8,r11,20320
	ctx.r8.s64 = ctx.r11.s64 + 20320;
	// stw r29,344(r31)
	PPC_STORE_U32(var_r31 + 344, var_r29);
	// lis r11,-32256
	// sth r10,406(r31)
	PPC_STORE_U16(var_r31 + 406, ctx.r10.u16);
	// addi r7,r11,20312
	ctx.r7.s64 = ctx.r11.s64 + 20312;
	// lis r11,-32256
	// stw r8,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r8.u32);
	// addi r6,r11,17180
	ctx.r6.s64 = ctx.r11.s64 + 17180;
	// lis r11,-32256
	// stw r7,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r7.u32);
	// addi r5,r11,20308
	ctx.r5.s64 = ctx.r11.s64 + 20308;
	// lis r11,-32256
	// stw r6,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r6.u32);
	// addi r4,r11,20300
	ctx.r4.s64 = ctx.r11.s64 + 20300;
	// lis r11,-32256
	// stw r5,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r5.u32);
	// addi r30,r11,20296
	var_r30 = (uint32_t)(ctx.r11.s64 + 20296);  // lbl_82004F48 @ 0x82004f48
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r4,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r4.u32);
	// stw r30,340(r31)
	PPC_STORE_U32(var_r31 + 340, var_r30);
	// stw r11,348(r31)
	PPC_STORE_U32(var_r31 + 348, ctx.r11.u32);
	// stw r11,352(r31)
	PPC_STORE_U32(var_r31 + 352, ctx.r11.u32);
	// stw r11,360(r31)
	PPC_STORE_U32(var_r31 + 360, ctx.r11.u32);
	// stw r11,364(r31)
	PPC_STORE_U32(var_r31 + 364, ctx.r11.u32);
	// stw r11,368(r31)
	PPC_STORE_U32(var_r31 + 368, ctx.r11.u32);
	// stw r11,372(r31)
	PPC_STORE_U32(var_r31 + 372, ctx.r11.u32);
	// stw r11,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r11.u32);
	// stw r11,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r11.u32);
	// stw r11,384(r31)
	PPC_STORE_U32(var_r31 + 384, ctx.r11.u32);
	// stw r11,388(r31)
	PPC_STORE_U32(var_r31 + 388, ctx.r11.u32);
	// stw r11,392(r31)
	PPC_STORE_U32(var_r31 + 392, ctx.r11.u32);
	// stw r11,396(r31)
	PPC_STORE_U32(var_r31 + 396, ctx.r11.u32);
	// stw r11,400(r31)
	PPC_STORE_U32(var_r31 + 400, ctx.r11.u32);
	// stw r11,408(r31)
	PPC_STORE_U32(var_r31 + 408, ctx.r11.u32);
	// stw r11,412(r31)
	PPC_STORE_U32(var_r31 + 412, ctx.r11.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_41"))) PPC_WEAK_FUNC(msgMsgSink_vfn_41);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_41) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-12
	ctx.r3.s64 = ctx.r3.s64 + -12;
	// b 0x82457970
	msgMsgSink_7970_w(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_38_6690_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_38_6690_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_38_6690_1) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lfs f0,16(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f0,f1
	// beqlr cr6
	if (ctx.f0.f64 == ctx.f1.f64) return;
	// li r11,1
	ctx.r11.s64 = 1;
	// stfs f1,16(r3)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// stw r11,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_66B0_h"))) PPC_WEAK_FUNC(msgMsgSink_66B0_h);
PPC_FUNC_IMPL(__imp__msgMsgSink_66B0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,1
	var_r30 = 1;
	// lwz r11,400(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 400);
	// cmplwi cr6,r11,0
	// bne cr6,0x824567ec
	if (ctx.r11.u32 == 0) {
		// lwz r10,344(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 344);
		// cmplwi cr6,r10,8
		// beq cr6,0x824566f0
		if (ctx.r10.u32 != 8) {
			// lwz r9,60(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 60);
			// cmpwi cr6,r9,1
			// bne cr6,0x824567ec
			if (ctx.r9.s32 != 1) {
				// li r3,0
				ctx.r3.s64 = 0;
				// b 0x824567f8
				// blr
				return;
			}
		}
	loc_824566F0:
		// lwz r8,412(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 412);
		// cmpwi cr6,r8,1
		// bne cr6,0x824567ec
		if (ctx.r8.s32 != 1) {
			// li r3,0
			ctx.r3.s64 = 0;
			// b 0x824567f8
			// blr
			return;
		}
		// lwz r7,380(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 380);
		// cmpwi cr6,r7,0
		// beq cr6,0x82456724
		if (ctx.r7.s32 != 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x82456550
			atSingleton_6550_fw(ctx, base);
			// lwz r6,376(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 376);
			// cmpw cr6,r6,r3
			// bgt cr6,0x824567b4
			if (ctx.r6.s32 > ctx.r3.s32) goto loc_824567B4;
			// li r30,1
			var_r30 = 1;
			// b 0x824567b8
		} else {
		loc_82456724:
			// lwz r3,52(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
			// cmplwi cr6,r3,0
			// beq cr6,0x82456768
			if (ctx.r3.u32 != 0) {
				// lwz r5,408(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 408);
				// lhz r4,24(r5)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r5.u32 + 24);
				// rlwinm r11,r4,0,24,26
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xE0;
				// cmplwi cr6,r11,128
				// bne cr6,0x82456768
				if (ctx.r11.u32 != 128) goto loc_82456768;
				// lwz r11,80(r3)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 80);
				// cmpwi cr6,r11,5
				// beq cr6,0x82456768
				if (ctx.r11.s32 == 5) goto loc_82456768;
				// cmpwi cr6,r11,6
				// beq cr6,0x82456768
				if (ctx.r11.s32 == 6) goto loc_82456768;
				// bl 0x82454158
				msgMsgSink_4158_2hr(ctx, base);
				// cmpwi cr6,r3,0
				// bne cr6,0x82456768
				if (ctx.r3.s32 != 0) goto loc_82456768;
				// li r30,0
				var_r30 = 0;
			}
		loc_82456768:
			// lwz r10,52(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
			// cmplwi cr6,r10,0
			// beq cr6,0x8245678c
			if (ctx.r10.u32 != 0) {
				// lwz r9,408(r31)
				ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 408);
				// lhz r8,24(r9)
				ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 24);
				// rlwinm r7,r8,0,24,26
				ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xE0;
				// cmplwi cr6,r7,96
				// bne cr6,0x8245678c
				if (ctx.r7.u32 != 96) goto loc_8245678C;
				// li r30,0
				var_r30 = 0;
			}
		loc_8245678C:
			// lwz r6,408(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 408);
			// lhz r5,24(r6)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r6.u32 + 24);
			// rlwinm r11,r5,27,29,31
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 27) & 0x7;
			// cmplwi cr6,r11,1
			// beq cr6,0x824567a8
			if (ctx.r11.u32 != 1) {
				// cmplwi cr6,r11,2
				// bne cr6,0x824567b8
				if (ctx.r11.u32 != 2) goto loc_824567B8;
			}
		loc_824567A8:
			// lwz r4,388(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 388);
			// cmplwi cr6,r4,0
			// beq cr6,0x824567b8
			if (ctx.r4.u32 == 0) goto loc_824567B8;
		loc_824567B4:
			// li r30,0
			var_r30 = 0;
		}
	loc_824567B8:
		// lwz r11,396(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 396);
		// cmplwi cr6,r11,0
		// beq cr6,0x824567d4
		if (ctx.r11.u32 != 0) {
			// lwz r3,80(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
			// cmpwi cr6,r3,3
			// beq cr6,0x824567d4
			if (ctx.r3.s32 == 3) goto loc_824567D4;
			// li r30,0
			var_r30 = 0;
		}
	loc_824567D4:
		// lwz r11,392(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
		// cmplwi cr6,r11,0
		// beq cr6,0x824567f4
		if (ctx.r11.u32 == 0) goto loc_824567F4;
		// lwz r11,80(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
		// cmpwi cr6,r11,3
		// beq cr6,0x824567f4
		if (ctx.r11.s32 == 3) goto loc_824567F4;
	}
loc_824567EC:
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x824567f8
	// blr
	return;
loc_824567F4:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
loc_824567F8:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_65"))) PPC_WEAK_FUNC(msgMsgSink_vfn_65);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_65) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,52(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplw cr6,r4,r11
	// bne cr6,0x824568a0
	if (ctx.r4.u32 == ctx.r11.u32) {
		// li r11,0
		ctx.r11.s64 = 0;
		// lwz r9,412(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 412);
		// li r10,-1
		// cmpwi cr6,r9,1
		// stw r11,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r11.u32);
		// sth r10,404(r31)
		PPC_STORE_U16(var_r31 + 404, ctx.r10.u16);
		// stw r11,40(r31)
		PPC_STORE_U32(var_r31 + 40, ctx.r11.u32);
		// bne cr6,0x8245690c
		if (ctx.r9.s32 != 1) {
			// blr
			return;
		}
		// lwz r8,380(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 380);
		// cmpwi cr6,r8,0
		// bne cr6,0x8245690c
		if (ctx.r8.s32 != 0) {
			// blr
			return;
		}
		// lwz r7,408(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 408);
		// lhz r6,24(r7)
		ctx.r6.u64 = PPC_LOAD_U16(ctx.r7.u32 + 24);
		// rlwinm r11,r6,27,29,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0x7;
		// cmplwi cr6,r11,4
		// beq cr6,0x82456878
		if (ctx.r11.u32 != 4) {
			// cmplwi cr6,r11,3
			// bne cr6,0x8245690c
			if (ctx.r11.u32 != 3) {
				// blr
				return;
			}
		}
	loc_82456878:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x824564d8
		atSingleton_64D8_g(ctx, base);
		// li r5,1
		ctx.r5.s64 = 1;
		// stw r3,376(r31)
		PPC_STORE_U32(var_r31 + 376, ctx.r3.u32);
		// stw r5,380(r31)
		PPC_STORE_U32(var_r31 + 380, ctx.r5.u32);
		// blr
		return;
	}
loc_824568A0:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplw cr6,r4,r3
	// bne cr6,0x824568d0
	if (ctx.r4.u32 == ctx.r3.u32) {
		// li r11,-1
		// li r10,0
		ctx.r10.s64 = 0;
		// sth r11,406(r31)
		PPC_STORE_U16(var_r31 + 406, ctx.r11.u16);
		// stw r10,396(r31)
		PPC_STORE_U32(var_r31 + 396, ctx.r10.u32);
		// blr
		return;
	}
loc_824568D0:
	// lwz r9,392(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplw cr6,r4,r9
	// bne cr6,0x824568f8
	if (ctx.r4.u32 == ctx.r9.u32) {
		// li r8,0
		ctx.r8.s64 = 0;
		// stw r8,392(r31)
		PPC_STORE_U32(var_r31 + 392, ctx.r8.u32);
		// blr
		return;
	}
loc_824568F8:
	// lwz r7,400(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 400);
	// cmplw cr6,r4,r7
	// bne cr6,0x8245690c
	if (ctx.r4.u32 == ctx.r7.u32) {
		// li r6,0
		ctx.r6.s64 = 0;
		// stw r6,400(r31)
		PPC_STORE_U32(var_r31 + 400, ctx.r6.u32);
	}
loc_8245690C:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_66"))) PPC_WEAK_FUNC(msgMsgSink_vfn_66);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_66) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x824542d0
	msgMsgSink_42D0_v12(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x824564d8
	atSingleton_64D8_g(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r3,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r3.u32);
	// stw r11,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_67"))) PPC_WEAK_FUNC(msgMsgSink_vfn_67);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_67) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,388(r31)
	PPC_STORE_U32(var_r31 + 388, ctx.r11.u32);
	// bl 0x824564d8
	atSingleton_64D8_g(ctx, base);
	// li r10,1
	ctx.r10.s64 = 1;
	// stw r3,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r3.u32);
	// stw r10,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_69A8_wrh"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_69A8_wrh);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_69A8_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,408(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 408);
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// rlwinm r11,r10,27,29,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x7;
	// cmplwi cr6,r11,4
	// bgt cr6,0x82456b5c
	if (ctx.r11.u32 > 4) {
		// blr
		return;
	}
	// lis r12,-32187
	// addi r12,r12,27116
	ctx.r12.s64 = ctx.r12.s64 + 27116;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (ctx.r11.u64) {
	case 0:
		goto loc_82456A00;
	case 1:
		goto loc_82456AD4;
	case 2:
		goto loc_82456AD4;
	case 3:
		goto loc_82456A6C;
	case 4:
		goto loc_82456A18;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82456A00:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82456550
	atSingleton_6550_fw(ctx, base);
	// li r9,1
	ctx.r9.s64 = 1;
	// stw r3,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r3.u32);
	// stw r9,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r9.u32);
	// b 0x82456b5c
	// blr
	return;
loc_82456A18:
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// lwz r7,12(r8)
	// bctrl
	VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
	// lis r6,32767
	ctx.r6.s64 = 2147418112;
	// ori r5,r6,65535
	ctx.r5.u64 = ctx.r6.u64 | 65535;
	// cmpw cr6,r3,r5
	// bne cr6,0x82456a54
	if (ctx.r3.s32 != ctx.r5.s32) goto loc_82456A54;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82456550
	atSingleton_6550_fw(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// stw r3,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r3.u32);
	// stw r4,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r4.u32);
	// b 0x82456b5c
	// blr
	return;
loc_82456A54:
	// lwz r11,364(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 364);
	// li r10,0
	ctx.r10.s64 = 0;
	// subf r9,r11,r3
	ctx.r9.s64 = ctx.r3.s64 - ctx.r11.s64;
	// stw r10,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r10.u32);
	// stw r9,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r9.u32);
	// b 0x82456b5c
	// blr
	return;
loc_82456A6C:
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// stw r8,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r8.u32);
	// bl 0x824541c0
	jumptable_41C0_h(ctx, base);
	// cmpwi cr6,r3,0
	// bne cr6,0x82456ab4
	if (ctx.r3.s32 != 0) goto loc_82456AB4;
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// bl 0x82454b38
	jumptable_4B38_h(ctx, base);
	// cmpwi cr6,r3,0
	// bne cr6,0x82456ab4
	if (ctx.r3.s32 != 0) goto loc_82456AB4;
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// lwz r6,12(r7)
	// bctrl
	VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
	// lwz r5,364(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 364);
	// subf r4,r5,r3
	ctx.r4.s64 = ctx.r3.s64 - ctx.r5.s64;
	// b 0x82456b58
	goto loc_82456B58;
loc_82456AB4:
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// bl 0x82454228
	msgMsgSink_4228_p39(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// li r4,1
	ctx.r4.s64 = 1;
	// stw r11,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r11.u32);
	// bl 0x824542d0
	msgMsgSink_42D0_v12(ctx, base);
	// b 0x82456b5c
	// blr
	return;
loc_82456AD4:
	// li r30,1
	var_r30 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r30,380(r31)
	PPC_STORE_U32(var_r31 + 380, var_r30);
	// bl 0x82456550
	atSingleton_6550_fw(ctx, base);
	// lwz r11,408(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 408);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// stw r3,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r3.u32);
	// lhz r3,24(r11)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// rlwinm r10,r3,0,24,26
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xE0;
	// cmplwi cr6,r10,32
	// beq cr6,0x82456b04
	if (ctx.r10.u32 == 32) goto loc_82456B04;
	// li r4,0
	ctx.r4.s64 = 0;
loc_82456B04:
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// bl 0x82454be0
	atSingleton_4BE0(ctx, base);
	// cmplwi cr6,r3,0
	// stw r3,388(r31)
	PPC_STORE_U32(var_r31 + 388, ctx.r3.u32);
	// beq cr6,0x82456b5c
	if (ctx.r3.u32 == 0) {
		// blr
		return;
	}
	// stw r30,64(r3)
	PPC_STORE_U32(ctx.r3.u32 + 64, var_r30);
	// lwz r11,388(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 388);
	// lhz r9,56(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 56);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// cmplwi cr6,r9,0
	// beq cr6,0x82456b3c
	if (ctx.r9.u32 == 0) goto loc_82456B3C;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
loc_82456B3C:
	// lwz r8,56(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 56);
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r7,364(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 364);
	// lwz r6,40(r8)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r8.u32 + 40);
	// stw r5,380(r31)
	PPC_STORE_U32(var_r31 + 380, ctx.r5.u32);
	// subf r11,r7,r6
	ctx.r11.s64 = ctx.r6.s64 - ctx.r7.s64;
	// add r4,r11,r10
	ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
loc_82456B58:
	// stw r4,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r4.u32);
loc_82456B5C:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_55_6B78_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_55_6B78_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_55_6B78_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r28,r11,144
	var_r28 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,344(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 344);
	// cmplwi cr6,r11,32
	// bne cr6,0x82456bc4
	if (ctx.r11.u32 == 32) {
		// lis r30,-30009
		var_r30 = (uint32_t)(-1966669824);
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// ori r30,r30,6
		var_r30 = (uint32_t)(var_r30 | 6);
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		return;
	}
loc_82456BC4:
	// lwz r10,348(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 348);
	// li r30,0
	var_r30 = 0;
	// cmpw cr6,r29,r10
	// beq cr6,0x82456ca8
	if ((int32_t)var_r29 != ctx.r10.s32) {
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244dec0
		msgMsgSink_vfn_33(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x82456ca8
		if ((int32_t)var_r30 < 0) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lwz r3,392(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
		// cmplwi cr6,r3,0
		// beq cr6,0x82456c10
		if (ctx.r3.u32 != 0) {
			// cmpwi cr6,r29,0
			// li r4,1
			ctx.r4.s64 = 1;
			// bne cr6,0x82456c08
			if ((int32_t)var_r29 == 0) {
				// li r4,0
				ctx.r4.s64 = 0;
			}
		loc_82456C08:
			// bl 0x82453c88
			msgMsgSink_3C88_g(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
		}
	loc_82456C10:
		// cmpwi cr6,r30,0
		// blt cr6,0x82456ca8
		if ((int32_t)var_r30 < 0) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lwz r3,396(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
		// cmplwi cr6,r3,0
		// beq cr6,0x82456c3c
		if (ctx.r3.u32 != 0) {
			// cmpwi cr6,r29,0
			// li r4,1
			ctx.r4.s64 = 1;
			// bne cr6,0x82456c34
			if ((int32_t)var_r29 == 0) {
				// li r4,0
				ctx.r4.s64 = 0;
			}
		loc_82456C34:
			// bl 0x82453c88
			msgMsgSink_3C88_g(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
		}
	loc_82456C3C:
		// cmpwi cr6,r30,0
		// blt cr6,0x82456ca8
		if ((int32_t)var_r30 < 0) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// cmpwi cr6,r29,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bne cr6,0x82456c54
		if ((int32_t)var_r29 == 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_82456C54:
		// lwz r9,344(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 344);
		// stw r11,348(r31)
		PPC_STORE_U32(var_r31 + 348, ctx.r11.u32);
		// cmplwi cr6,r9,8
		// bne cr6,0x82456ca8
		if (ctx.r9.u32 != 8) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// cmpwi cr6,r11,0
		// beq cr6,0x82456c88
		if (ctx.r11.s32 != 0) {
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// stw r3,372(r31)
			PPC_STORE_U32(var_r31 + 372, ctx.r3.u32);
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
	loc_82456C88:
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// lwz r8,372(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 372);
		// lwz r7,368(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 368);
		// li r6,0
		ctx.r6.s64 = 0;
		// subf r11,r8,r7
		ctx.r11.s64 = ctx.r7.s64 - ctx.r8.s64;
		// add r5,r11,r3
		ctx.r5.u64 = ctx.r11.u64 + ctx.r3.u64;
		// stw r6,372(r31)
		PPC_STORE_U32(var_r31 + 372, ctx.r6.u32);
		// stw r5,368(r31)
		PPC_STORE_U32(var_r31 + 368, ctx.r5.u32);
	}
loc_82456CA8:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_49_6CC0_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_49_6CC0_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_49_6CC0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// cmplwi cr6,r30,0
	// bne cr6,0x82456d08
	if (var_r30 == 0) {
		// lis r30,-32761
		var_r30 = (uint32_t)(-2147024896);
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// ori r30,r30,87
		var_r30 = (uint32_t)(var_r30 | 87);
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		return;
	}
loc_82456D08:
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244e3b0
	msgMsgSink_vfn_27(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x82456d5c
	if ((int32_t)var_r30 >= 0) {
		// lwz r3,396(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
		// cmplwi cr6,r3,0
		// beq cr6,0x82456d34
		if (ctx.r3.u32 != 0) {
			// addi r4,r31,296
			ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 296;
			// bl 0x82453ee8
			msgMsgSink_3EE8_g(ctx, base);
		}
	loc_82456D34:
		// lwz r3,392(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
		// cmplwi cr6,r3,0
		// beq cr6,0x82456d48
		if (ctx.r3.u32 != 0) {
			// addi r4,r31,296
			ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 296;
			// bl 0x82453ee8
			msgMsgSink_3EE8_g(ctx, base);
		}
	loc_82456D48:
		// lwz r3,400(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 400);
		// cmplwi cr6,r3,0
		// beq cr6,0x82456d5c
		if (ctx.r3.u32 == 0) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// addi r4,r31,296
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 296;
		// bl 0x82453ee8
		msgMsgSink_3EE8_g(ctx, base);
	}
loc_82456D5C:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_6D70_h"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_6D70_h);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_6D70_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r28,r11,144
	var_r28 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r10,288(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 288);
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// li r29,0
	var_r29 = 0;
	// lwz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// lwz r3,124(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 124);
	// cmplwi cr6,r3,0
	// lhz r30,6(r9)
	var_r30 = (uint32_t)(PPC_LOAD_U16(ctx.r9.u32 + 6));
	// beq cr6,0x82456dd8
	if (ctx.r3.u32 != 0) {
		// lwz r8,0(r3)
  // [ph4a] vtable load collapsed
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// lwz r7,8(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 2, ctx, base);  // pattern-B slot 2 (byte +8)
		// lbz r6,0(r3)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
		// rlwinm r5,r6,30,31,31
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 30) & 0x1;
		// xori r11,r5,1
		ctx.r11.u64 = ctx.r5.u64 ^ 1;
		// cmpwi cr6,r11,0
		// bne cr6,0x82456e04
		if (ctx.r11.s32 != 0) goto loc_82456E04;
	}
loc_82456DD8:
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r31,352
	ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 352;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244ec28
	msgMsgSink_EC28_g(ctx, base);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
loc_82456E04:
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// lwz r3,56(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 56);
	// bl 0x82448c60
	ph_8C60(ctx, base);
	// cmplwi cr6,r3,0
	// stw r3,352(r31)
	PPC_STORE_U32(var_r31 + 352, ctx.r3.u32);
	// bne cr6,0x82456e24
	if (ctx.r3.u32 == 0) {
		// lis r29,-30009
		var_r29 = (uint32_t)(-1966669824);
		// ori r29,r29,10
		var_r29 = (uint32_t)(var_r29 | 10);
	}
loc_82456E24:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_56"))) PPC_WEAK_FUNC(msgMsgSink_vfn_56);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_56) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244e1d8
	msgMsgSink_vfn_34(ctx, base);
	// lwz r11,392(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmplwi cr6,r11,0
	// beq cr6,0x82456e84
	if (ctx.r11.u32 != 0) {
		// addi r4,r31,328
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 328;
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// bl 0x82453e68
		msgMsgSink_3E68_g(ctx, base);
	}
loc_82456E84:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r3,0
	// beq cr6,0x82456e98
	if (ctx.r3.u32 != 0) {
		// addi r4,r31,328
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 328;
		// bl 0x82453e68
		msgMsgSink_3E68_g(ctx, base);
	}
loc_82456E98:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_57"))) PPC_WEAK_FUNC(msgMsgSink_vfn_57);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_57) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244e2a0
	msgMsgSink_vfn_35(ctx, base);
	// lwz r11,392(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// cmplwi cr6,r11,0
	// beq cr6,0x82456efc
	if (ctx.r11.u32 != 0) {
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// bl 0x82453ea8
		msgMsgSink_3EA8_g(ctx, base);
	}
loc_82456EFC:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r3,0
	// beq cr6,0x82456f10
	if (ctx.r3.u32 != 0) {
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x82453ea8
		msgMsgSink_3EA8_g(ctx, base);
	}
loc_82456F10:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_68"))) PPC_WEAK_FUNC(msgMsgSink_vfn_68);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_68) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r3,288(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 288);
	// lwz r4,284(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 284);
	// b 0x8245b0e8
	atSingleton_B0E8_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__game_6F40_h"))) PPC_WEAK_FUNC(game_6F40_h);
PPC_FUNC_IMPL(__imp__game_6F40_h) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,0
	// addi r11,r3,340
	ctx.r11.s64 = ctx.r3.s64 + 340;
	// bne cr6,0x82456f50
	if (ctx.r3.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_82456F50:
	// lwz r10,352(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 352);
	// lwz r4,228(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 228);
	// cmplwi cr6,r4,0
	// beqlr cr6
	if (ctx.r4.u32 == 0) return;
loc_82456F60:
	// lwz r9,0(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// cmplw cr6,r9,r11
	// beq cr6,0x82456f7c
	if (ctx.r9.u32 == ctx.r11.u32) {
		// cmplwi cr6,r4,0
		// beqlr cr6
		if (ctx.r4.u32 == 0) return;
		// addi r3,r10,24
		ctx.r3.s64 = ctx.r10.s64 + 24;
		// b 0x8244ef40
		ph_EF40(ctx, base);
		return;
	}
	// lwz r4,4(r4)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
	// cmplwi cr6,r4,0
	// bne cr6,0x82456f60
	if (ctx.r4.u32 != 0) goto loc_82456F60;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_6F90_g"))) PPC_WEAK_FUNC(msgMsgSink_6F90_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_6F90_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20320
	ctx.r11.s64 = ctx.r11.s64 + 20320;
	// lis r10,-32256
	// lis r9,-32256
	// lis r8,-32256
	// lis r7,-32256
	// lis r6,-32256
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// addi r30,r31,340
	var_r30 = (uint32_t)(var_r31 + 340);
	// lwz r11,344(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 344);
	// addi r10,r10,20312
	ctx.r10.s64 = ctx.r10.s64 + 20312;
	// addi r9,r9,17180
	ctx.r9.s64 = ctx.r9.s64 + 17180;
	// addi r8,r8,20308
	ctx.r8.s64 = ctx.r8.s64 + 20308;
	// addi r7,r7,20300
	ctx.r7.s64 = ctx.r7.s64 + 20300;
	// addi r6,r6,20296
	ctx.r6.s64 = ctx.r6.s64 + 20296;
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// cmplwi cr6,r11,16
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// stw r8,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r8.u32);
	// stw r7,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r7.u32);
	// stw r6,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r6.u32);
	// beq cr6,0x82457008
	if (ctx.r11.u32 != 16) {
		// cmplwi cr6,r11,8
		// bne cr6,0x82457018
		if (ctx.r11.u32 != 8) goto loc_82457018;
	}
loc_82457008:
	// li r11,32
	ctx.r11.s64 = 32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,344(r31)
	PPC_STORE_U32(var_r31 + 344, ctx.r11.u32);
	// bl 0x8244e628
	msgMsgSink_vfn_49(ctx, base);
loc_82457018:
	// lwz r11,352(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 352);
	// lwz r4,228(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 228);
	// cmplwi cr6,r4,0
	// beq cr6,0x82457054
	if (ctx.r4.u32 != 0) {
	loc_82457028:
		// lwz r10,0(r4)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
		// cmplw cr6,r10,r30
		// beq cr6,0x82457044
		if (ctx.r10.u32 != var_r30) {
			// lwz r4,4(r4)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
			// cmplwi cr6,r4,0
			// bne cr6,0x82457028
			if (ctx.r4.u32 != 0) goto loc_82457028;
			// b 0x82457054
		} else {
		loc_82457044:
			// cmplwi cr6,r4,0
			// beq cr6,0x82457054
			if (ctx.r4.u32 == 0) goto loc_82457054;
			// addi r3,r11,24
			ctx.r3.s64 = ctx.r11.s64 + 24;
			// bl 0x8244ef40
			ph_EF40(ctx, base);
		}
	}
loc_82457054:
	// lwz r3,384(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 384);
	// cmplwi cr6,r3,0
	// beq cr6,0x82457074
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r8,0(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	}
loc_82457074:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r3,0
	// beq cr6,0x82457094
	if (ctx.r3.u32 != 0) {
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r6,4(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
	}
loc_82457094:
	// lwz r3,392(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r3,0
	// beq cr6,0x824570b4
	if (ctx.r3.u32 != 0) {
		// lwz r5,0(r3)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r11,4(r5)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 4);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	}
loc_824570B4:
	// lwz r3,400(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 400);
	// cmplwi cr6,r3,0
	// beq cr6,0x824570d4
	if (ctx.r3.u32 != 0) {
		// lwz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r9,4(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
	}
loc_824570D4:
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// lis r8,-32256
	// lis r7,-32256
	// addi r11,r11,17456
	ctx.r11.s64 = ctx.r11.s64 + 17456;
	// addi r10,r10,20312
	ctx.r10.s64 = ctx.r10.s64 + 20312;
	// addi r9,r9,17180
	ctx.r9.s64 = ctx.r9.s64 + 17180;
	// addi r8,r8,17452
	ctx.r8.s64 = ctx.r8.s64 + 17452;
	// addi r7,r7,20300
	ctx.r7.s64 = ctx.r7.s64 + 20300;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// stw r8,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r8.u32);
	// stw r7,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r7.u32);
	// bl 0x8244eff0
	msgMsgSink_EFF0_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_61"))) PPC_WEAK_FUNC(msgMsgSink_vfn_61);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_61) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,412(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 412);
	// cmpwi cr6,r11,1
	// beq cr6,0x82457284
	if (ctx.r11.s32 == 1) goto loc_82457284;
	// cmpwi cr6,r11,2
	// beq cr6,0x824571c8
	if (ctx.r11.s32 == 2) goto loc_824571C8;
	// cmpwi cr6,r11,3
	// bne cr6,0x824574a8
	if (ctx.r11.s32 != 3) goto loc_824574A8;
	// lwz r10,52(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplwi cr6,r10,0
	// beq cr6,0x8245717c
	if (ctx.r10.u32 == 0) goto loc_8245717C;
	// lwz r11,80(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 80);
	// cmpwi cr6,r11,4
	// beq cr6,0x824574a8
	if (ctx.r11.s32 == 4) goto loc_824574A8;
	// cmpwi cr6,r11,5
	// beq cr6,0x824574a8
	if (ctx.r11.s32 == 5) goto loc_824574A8;
loc_8245717C:
	// lwz r11,392(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245719c
	if (ctx.r11.u32 == 0) goto loc_8245719C;
	// lwz r11,80(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// cmpwi cr6,r11,4
	// beq cr6,0x824574a8
	if (ctx.r11.s32 == 4) goto loc_824574A8;
	// cmpwi cr6,r11,5
	// beq cr6,0x824574a8
	if (ctx.r11.s32 == 5) goto loc_824574A8;
loc_8245719C:
	// stw r10,400(r31)
	PPC_STORE_U32(var_r31 + 400, ctx.r10.u32);
	// li r28,0
	var_r28 = 0;
	// lhz r10,406(r31)
	ctx.r10.u64 = PPC_LOAD_U16(var_r31 + 406);
	// li r11,-1
	// lwz r9,396(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 396);
	// stw r28,412(r31)
	PPC_STORE_U32(var_r31 + 412, var_r28);
	// sth r11,406(r31)
	PPC_STORE_U16(var_r31 + 406, ctx.r11.u16);
	// stw r28,396(r31)
	PPC_STORE_U32(var_r31 + 396, var_r28);
	// sth r10,404(r31)
	PPC_STORE_U16(var_r31 + 404, ctx.r10.u16);
	// stw r9,52(r31)
	PPC_STORE_U32(var_r31 + 52, ctx.r9.u32);
	// b 0x824574a8
	goto loc_824574A8;
loc_824571C8:
	// lwz r8,420(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 420);
	// cmpwi cr6,r8,0
	// beq cr6,0x82457250
	if (ctx.r8.s32 == 0) goto loc_82457250;
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r3,392(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
	// lwz r6,12(r7)
	// bctrl
	VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
	// lwz r11,408(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 408);
	// lhz r5,20(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
	// subf r3,r5,r3
	ctx.r3.s64 = ctx.r3.s64 - ctx.r5.s64;
	// cmpw cr6,r30,r3
	// blt cr6,0x82457250
	if ((int32_t)var_r30 < ctx.r3.s32) goto loc_82457250;
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,392(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
	// clrlwi r6,r10,30
	ctx.r6.u64 = ctx.r10.u32 & 0x3;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82453a48
	jumptable_3A48(ctx, base);
	// lwz r11,408(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 408);
	// lwz r10,396(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 396);
	// li r4,2
	ctx.r4.s64 = 2;
	// addi r3,r10,4
	ctx.r3.s64 = ctx.r10.s64 + 4;
	// lhz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// lhz r5,20(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
	// clrlwi r6,r9,30
	ctx.r6.u64 = ctx.r9.u32 & 0x3;
	// bl 0x82453a48
	jumptable_3A48(ctx, base);
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// bl 0x82454eb8
	msgMsgSink_4EB8_g(ctx, base);
	// li r8,3
	ctx.r8.s64 = 3;
	// stw r8,412(r31)
	PPC_STORE_U32(var_r31 + 412, ctx.r8.u32);
	// b 0x824574a8
	goto loc_824574A8;
loc_82457250:
	// lwz r11,392(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r11,0
	// beq cr6,0x82457270
	if (ctx.r11.u32 == 0) goto loc_82457270;
	// lwz r11,80(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// cmpwi cr6,r11,4
	// beq cr6,0x824574a8
	if (ctx.r11.s32 == 4) goto loc_824574A8;
	// cmpwi cr6,r11,5
	// beq cr6,0x824574a8
	if (ctx.r11.s32 == 5) goto loc_824574A8;
loc_82457270:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// bl 0x82454eb8
	msgMsgSink_4EB8_g(ctx, base);
	// li r8,3
	ctx.r8.s64 = 3;
	// stw r8,412(r31)
	PPC_STORE_U32(var_r31 + 412, ctx.r8.u32);
	// b 0x824574a8
	goto loc_824574A8;
loc_82457284:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x824566b0
	msgMsgSink_66B0_h(ctx, base);
	// cmpwi cr6,r3,0
	// beq cr6,0x824574a8
	if (ctx.r3.s32 == 0) goto loc_824574A8;
	// lwz r9,408(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 408);
	// li r10,2
	ctx.r10.s64 = 2;
	// li r28,0
	var_r28 = 0;
	// li r27,3
	var_r27 = 3;
	// lhz r7,24(r9)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 24);
	// rlwinm r11,r7,30,29,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 30) & 0x7;
	// cmplwi cr6,r11,7
	// bgt cr6,0x82457354
	if (!(ctx.r11.u32 > 7)) {
		// lis r12,-32187
		// addi r12,r12,29388
		ctx.r12.s64 = ctx.r12.s64 + 29388;
		// rlwinm r0,r11,2,0,29
		ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
		// lwzx r0,r12,r0
		ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
		// mtctr r0
		ctx.ctr.u64 = ctx.r0.u64;
		// bctr
		switch (ctx.r11.u64) {
		case 0:
		goto loc_824572EC;
		case 1:
		goto loc_82457344;
		case 2:
		goto loc_824572F8;
		case 3:
		goto loc_82457308;
		case 4:
		goto loc_8245731C;
		case 5:
		goto loc_82457330;
		case 6:
		goto loc_824572EC;
		case 7:
		goto loc_82457344;
		default:
		__builtin_trap(); // Switch case out of range
		}
		loc_824572EC:
		// stw r28,416(r31)
		PPC_STORE_U32(var_r31 + 416, var_r28);
		// stw r28,420(r31)
		PPC_STORE_U32(var_r31 + 420, var_r28);
		// b 0x82457350
		goto loc_82457350;
		loc_824572F8:
		// stw r10,412(r31)
		PPC_STORE_U32(var_r31 + 412, ctx.r10.u32);
		// stw r28,416(r31)
		PPC_STORE_U32(var_r31 + 416, var_r28);
		// stw r28,420(r31)
		PPC_STORE_U32(var_r31 + 420, var_r28);
	} else {
		loc_82457308:
		// li r6,1
		ctx.r6.s64 = 1;
		// stw r10,412(r31)
		PPC_STORE_U32(var_r31 + 412, ctx.r10.u32);
		// stw r28,420(r31)
		PPC_STORE_U32(var_r31 + 420, var_r28);
		// stw r6,416(r31)
		PPC_STORE_U32(var_r31 + 416, ctx.r6.u32);
		loc_8245731C:
		// li r5,1
		ctx.r5.s64 = 1;
		// stw r10,412(r31)
		PPC_STORE_U32(var_r31 + 412, ctx.r10.u32);
		// stw r28,416(r31)
		PPC_STORE_U32(var_r31 + 416, var_r28);
		// stw r5,420(r31)
		PPC_STORE_U32(var_r31 + 420, ctx.r5.u32);
		loc_82457330:
		// li r11,1
		ctx.r11.s64 = 1;
		// stw r10,412(r31)
		PPC_STORE_U32(var_r31 + 412, ctx.r10.u32);
		// stw r11,416(r31)
		PPC_STORE_U32(var_r31 + 416, ctx.r11.u32);
		// stw r11,420(r31)
		PPC_STORE_U32(var_r31 + 420, ctx.r11.u32);
		loc_82457344:
		// li r11,1
		ctx.r11.s64 = 1;
		// stw r11,420(r31)
		PPC_STORE_U32(var_r31 + 420, ctx.r11.u32);
		// stw r11,416(r31)
		PPC_STORE_U32(var_r31 + 416, ctx.r11.u32);
		loc_82457350:
		// stw r27,412(r31)
		PPC_STORE_U32(var_r31 + 412, var_r27);
	}
loc_82457354:
	// mr r29,r10
	var_r29 = ctx.r10.u32;
	// lwz r10,52(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
	// li r30,60
	var_r30 = 60;
	// cmplwi cr6,r10,0
	// beq cr6,0x824573cc
	if (!(ctx.r10.u32 == 0)) {
		// lwz r11,80(r10)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 80);
		// cmpwi cr6,r11,4
		// beq cr6,0x8245737c
		if (ctx.r11.s32 == 4) goto loc_8245737C;
	} else {
		if (!(ctx.r11.s32 != 5)) {
			loc_8245737C:
			// lwz r4,392(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 392);
			// cmplwi cr6,r4,0
			// bne cr6,0x82457398
			if (ctx.r4.u32 != 0) goto loc_82457398;
			// lwz r3,396(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
			// mr r11,r28
			ctx.r11.u64 = var_r28;
			// cmplwi cr6,r3,0
			// beq cr6,0x8245739c
			if (ctx.r3.u32 == 0) goto loc_8245739C;
			loc_82457398:
			// li r11,60
			ctx.r11.s64 = 60;
			loc_8245739C:
			// lwz r8,416(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 416);
			// clrlwi r30,r11,16
			var_r30 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
			// cmpwi cr6,r8,0
			// beq cr6,0x824573b8
			if (ctx.r8.s32 == 0) goto loc_824573B8;
			// lhz r7,24(r9)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 24);
			// lhz r30,22(r9)
			var_r30 = (uint32_t)(PPC_LOAD_U16(ctx.r9.u32 + 22));
			// clrlwi r29,r7,30
			var_r29 = (uint32_t)(ctx.r7.u32 & 0x3);
			loc_824573B8:
			// mr r6,r29
			ctx.r6.u64 = var_r29;
			// mr r5,r30
			ctx.r5.u64 = var_r30;
			// li r4,1
			ctx.r4.s64 = 1;
			// addi r3,r10,4
			ctx.r3.s64 = ctx.r10.s64 + 4;
			// bl 0x82453a48
			jumptable_3A48(ctx, base);
		}
	}
loc_824573CC:
	// lwz r10,392(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r10,0
	// beq cr6,0x82457428
	if (ctx.r10.u32 == 0) goto loc_82457428;
	// lwz r6,52(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 52);
	// clrlwi r11,r30,16
	ctx.r11.u64 = var_r30 & 0xFFFF;
	// cmplwi cr6,r6,0
	// bne cr6,0x824573ec
	if (ctx.r6.u32 != 0) goto loc_824573EC;
	// mr r11,r28
	ctx.r11.u64 = var_r28;
loc_824573EC:
	// lwz r5,416(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 416);
	// clrlwi r30,r11,16
	var_r30 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
	// cmpwi cr6,r5,0
	// beq cr6,0x8245740c
	if (ctx.r5.s32 == 0) goto loc_8245740C;
	// lwz r11,408(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 408);
	// lhz r4,24(r11)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// lhz r30,22(r11)
	var_r30 = (uint32_t)(PPC_LOAD_U16(ctx.r11.u32 + 22));
	// clrlwi r29,r4,30
	var_r29 = (uint32_t)(ctx.r4.u32 & 0x3);
loc_8245740C:
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// li r4,2
	ctx.r4.s64 = 2;
	// addi r3,r10,4
	ctx.r3.s64 = ctx.r10.s64 + 4;
	// bl 0x82453a48
	jumptable_3A48(ctx, base);
	// lwz r3,392(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
	// bl 0x82454eb8
	msgMsgSink_4EB8_g(ctx, base);
loc_82457428:
	// lwz r10,396(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r10,0
	// beq cr6,0x82457498
	if (!(ctx.r10.u32 == 0)) {
		// lwz r3,412(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 412);
	} else {
		if (!(ctx.r3.s32 != 3)) {
			// lwz r11,52(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 52);
			// cmplwi cr6,r11,0
			// bne cr6,0x8245745c
			if (ctx.r11.u32 != 0) goto loc_8245745C;
			// lwz r9,392(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 392);
			// mr r11,r28
			ctx.r11.u64 = var_r28;
			// cmplwi cr6,r9,0
			// beq cr6,0x82457460
			if (ctx.r9.u32 == 0) goto loc_82457460;
			loc_8245745C:
			// clrlwi r11,r30,16
			ctx.r11.u64 = var_r30 & 0xFFFF;
			loc_82457460:
			// lwz r8,420(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 420);
			// clrlwi r5,r11,16
			ctx.r5.u64 = ctx.r11.u32 & 0xFFFF;
			// cmpwi cr6,r8,0
			// beq cr6,0x82457480
			if (ctx.r8.s32 == 0) goto loc_82457480;
			// lwz r11,408(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 408);
			// lhz r7,24(r11)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
			// lhz r5,20(r11)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
			// clrlwi r29,r7,30
			var_r29 = (uint32_t)(ctx.r7.u32 & 0x3);
			loc_82457480:
			// mr r6,r29
			ctx.r6.u64 = var_r29;
			// li r4,2
			ctx.r4.s64 = 2;
			// addi r3,r10,4
			ctx.r3.s64 = ctx.r10.s64 + 4;
			// bl 0x82453a48
			jumptable_3A48(ctx, base);
			// lwz r3,396(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
			// bl 0x82454eb8
			msgMsgSink_4EB8_g(ctx, base);
		}
	}
loc_82457498:
	// lwz r6,396(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r6,0
	// bne cr6,0x824574a8
	if (ctx.r6.u32 != 0) goto loc_824574A8;
	// stw r27,412(r31)
	PPC_STORE_U32(var_r31 + 412, var_r27);
loc_824574A8:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244e050
	msgMsgSink_vfn_39(ctx, base);
	// lwz r3,392(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r3,0
	// beq cr6,0x824574c0
	if (ctx.r3.u32 == 0) goto loc_824574C0;
	// bl 0x82455148
	msgMsgSink_5148_g(ctx, base);
loc_824574C0:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r3,0
	// beq cr6,0x824574d0
	if (ctx.r3.u32 == 0) goto loc_824574D0;
	// bl 0x82455148
	msgMsgSink_5148_g(ctx, base);
loc_824574D0:
	// lwz r3,400(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 400);
	// cmplwi cr6,r3,0
	// beq cr6,0x824574e0
	if (ctx.r3.u32 == 0) goto loc_824574E0;
	// bl 0x82455148
	msgMsgSink_5148_g(ctx, base);
loc_824574E0:
	// lwz r5,344(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 344);
	// cmplwi cr6,r5,16
	// bne cr6,0x82457538
	if (ctx.r5.u32 != 16) {
		return;
	}
	// lwz r4,52(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplwi cr6,r4,0
	// bne cr6,0x82457538
	if (ctx.r4.u32 != 0) {
		return;
	}
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r3,0
	// bne cr6,0x82457538
	if (ctx.r3.u32 != 0) {
		return;
	}
	// lwz r11,392(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r11,0
	// bne cr6,0x82457538
	if (ctx.r11.u32 != 0) {
		return;
	}
	// lwz r10,400(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 400);
	// cmplwi cr6,r10,0
	// bne cr6,0x82457538
	if (ctx.r10.u32 != 0) {
		return;
	}
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r8,32
	ctx.r8.s64 = 32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r7,108(r9)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 108);
	// stw r8,344(r31)
	PPC_STORE_U32(var_r31 + 344, ctx.r8.u32);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r7.u32);
loc_82457538:
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_63"))) PPC_WEAK_FUNC(msgMsgSink_vfn_63);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_63) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r27,r11,144
	var_r27 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// li r11,2
	ctx.r11.s64 = 2;
	// lwz r30,284(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 284));
	// lwz r10,48(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 48);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,344(r31)
	PPC_STORE_U32(var_r31 + 344, ctx.r11.u32);
	// lwz r9,0(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r4,1(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1);
	// lwz r8,32(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 32);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	// lwz r7,48(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 48);
	// lwz r6,0(r30)
	ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r4,5(r7)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r7.u32 + 5);
	// lwz r5,36(r6)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 36);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,80(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 80);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x8245767c
	if ((int32_t)var_r30 >= 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// li r3,16
		ctx.r3.s64 = 16;
		// ori r4,r4,32791
		ctx.r4.u64 = ctx.r4.u64 | 32791;
		// bl 0x820c01b8
		rage_01B8(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x824575fc
		if (ctx.r3.u32 == 0) {
			// lis r30,-32761
			var_r30 = (uint32_t)(-2147024896);
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// ori r30,r30,14
			var_r30 = (uint32_t)(var_r30 | 14);
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// lwz r4,56(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 56);
		// bl 0x8245b110
		msgMsgSink_B110_g(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// cmplwi cr6,r11,0
		// bne cr6,0x82457618
		if (ctx.r11.u32 == 0) {
		loc_824575FC:
			// lis r30,-32761
			var_r30 = (uint32_t)(-2147024896);
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// ori r30,r30,14
			var_r30 = (uint32_t)(var_r30 | 14);
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
	loc_82457618:
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// stw r11,288(r31)
		PPC_STORE_U32(var_r31 + 288, ctx.r11.u32);
		// li r3,12
		ctx.r3.s64 = 12;
		// ori r4,r4,32813
		ctx.r4.u64 = ctx.r4.u64 | 32813;
		// bl 0x820c01b8
		rage_01B8(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x824575fc
		if (ctx.r3.u32 == 0) {
			// lis r30,-32761
			var_r30 = (uint32_t)(-2147024896);
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// ori r30,r30,14
			var_r30 = (uint32_t)(var_r30 | 14);
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lwz r10,0(r29)
		ctx.r10.u64 = PPC_LOAD_U32(var_r29 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// clrlwi r5,r10,16
		ctx.r5.u64 = ctx.r10.u32 & 0xFFFF;
		// bl 0x8245b450
		game_B450_h(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// cmplwi cr6,r11,0
		// beq cr6,0x824575fc
		if (ctx.r11.u32 == 0) {
			// lis r30,-32761
			var_r30 = (uint32_t)(-2147024896);
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// ori r30,r30,14
			var_r30 = (uint32_t)(var_r30 | 14);
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,384(r31)
		PPC_STORE_U32(var_r31 + 384, ctx.r11.u32);
		// bl 0x82456d70
		RtlEnterCriticalSection_6D70_h(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245767c
		if ((int32_t)var_r30 < 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// li r9,4
		ctx.r9.s64 = 4;
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r9,344(r31)
		PPC_STORE_U32(var_r31 + 344, ctx.r9.u32);
		// bl 0x8244dc40
		msgMsgSink_DC40_g(ctx, base);
	}
loc_8245767C:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_7690_wrh"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_7690_wrh);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_7690_wrh) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// lwz r10,408(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 408);
	// lhz r8,24(r10)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r10.u32 + 24);
	// rlwinm r10,r8,24,29,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 24) & 0x7;
	// cmplwi cr6,r10,5
	// bgtlr cr6
	if (ctx.r10.u32 > 5) return;
	// lis r12,-32187
	// addi r12,r12,30404
	ctx.r12.s64 = ctx.r12.s64 + 30404;
	// rlwinm r0,r10,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (ctx.r10.u64) {
	case 0:
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// b 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		return;
	case 1:
		goto loc_824576E8;
	case 2:
		goto loc_8245773C;
	case 3:
		goto loc_8245773C;
	case 4:
		goto loc_82457798;
	case 5:
		goto loc_82457798;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_824576DC:
	// li r6,0
	ctx.r6.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// b 0x8244e860
	msgMsgSink_E860_g(ctx, base);
	return;
loc_824576E8:
	// lwz r10,52(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	// li r6,0
	ctx.r6.s64 = 0;
	// cmplwi cr6,r10,0
	// beq cr6,0x82457734
	if (ctx.r10.u32 == 0) {
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// b 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		return;
	}
	// lwz r8,80(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 80);
	// cmpwi cr6,r8,4
	// beq cr6,0x8245770c
	if (ctx.r8.s32 == 4) goto loc_8245770C;
	// cmpwi cr6,r8,5
	// bne cr6,0x82457734
	if (ctx.r8.s32 != 5) {
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// b 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		return;
	}
loc_8245770C:
	// lwz r9,56(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// lwz r3,104(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 104);
	// lwz r10,100(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 100);
	// lwz r7,376(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 376);
	// lwz r8,364(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 364);
	// lwz r6,40(r9)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + 40);
	// subf r9,r6,r3
	ctx.r9.s64 = ctx.r3.s64 - ctx.r6.s64;
	// subf r10,r10,r9
	ctx.r10.s64 = ctx.r9.s64 - ctx.r10.s64;
	// add r10,r10,r7
	ctx.r10.u64 = ctx.r10.u64 + ctx.r7.u64;
	// add r6,r10,r8
	ctx.r6.u64 = ctx.r10.u64 + ctx.r8.u64;
loc_82457734:
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// b 0x8244e860
	msgMsgSink_E860_g(ctx, base);
	return;
loc_8245773C:
	// lwz r10,388(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 388);
	// cmplwi cr6,r10,0
	// beq cr6,0x8245778c
	if (ctx.r10.u32 == 0) {
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// b 0x8244e860
		msgMsgSink_E860_g(ctx, base);
		return;
	}
	// rlwinm r7,r8,0,21,23
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x700;
	// cmplwi cr6,r7,768
	// bne cr6,0x82457760
	if (ctx.r7.u32 != 768) goto loc_82457760;
	// li r8,1
	ctx.r8.s64 = 1;
	// li r10,0
	ctx.r10.s64 = 0;
	// b 0x82457768
	// lwz r7,388(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 388);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// mr r9,r5
	ctx.r9.u64 = ctx.r5.u64;
	// mr r5,r8
	ctx.r5.u64 = ctx.r8.u64;
	// clrlwi r6,r10,16
	ctx.r6.u64 = ctx.r10.u32 & 0xFFFF;
	// lwz r11,16(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
	// lwz r8,7(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7);
	// mr r7,r8
	ctx.r7.u64 = ctx.r8.u64;
	// b 0x8244e978
	msgMsgSink_E978_w(ctx, base);
	return;
loc_82457760:
	// lhz r10,48(r10)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r10.u32 + 48);
	// li r8,0
	ctx.r8.s64 = 0;
loc_82457768:
	// lwz r7,388(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 388);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// mr r9,r5
	ctx.r9.u64 = ctx.r5.u64;
	// mr r5,r8
	ctx.r5.u64 = ctx.r8.u64;
	// clrlwi r6,r10,16
	ctx.r6.u64 = ctx.r10.u32 & 0xFFFF;
	// lwz r11,16(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
	// lwz r8,7(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7);
	// mr r7,r8
	ctx.r7.u64 = ctx.r8.u64;
	// b 0x8244e978
	msgMsgSink_E978_w(ctx, base);
	return;
loc_82457798:
	// rlwinm r9,r8,0,21,23
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x700;
	// li r8,1
	ctx.r8.s64 = 1;
	// cmplwi cr6,r9,1024
	// beq cr6,0x824577ac
	if (ctx.r9.u32 == 1024) {
		// lwz r10,408(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 408);
		// mr r9,r5
		ctx.r9.u64 = ctx.r5.u64;
		// mr r5,r8
		ctx.r5.u64 = ctx.r8.u64;
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// lwz r8,16(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
		// lwz r7,12(r10)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
		// b 0x8244e978
		msgMsgSink_E978_w(ctx, base);
		return;
	}
	// li r8,0
	ctx.r8.s64 = 0;
loc_824577AC:
	// lwz r10,408(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 408);
	// mr r9,r5
	ctx.r9.u64 = ctx.r5.u64;
	// mr r5,r8
	ctx.r5.u64 = ctx.r8.u64;
	// li r6,0
	ctx.r6.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// lwz r8,16(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// lwz r7,12(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// b 0x8244e978
	msgMsgSink_E978_w(ctx, base);
	return;
}

__attribute__((alias("__imp__sub_824577CC"))) PPC_WEAK_FUNC(sub_824577CC);
PPC_FUNC_IMPL(__imp__sub_824577CC) {
	PPC_FUNC_PROLOGUE();
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_45"))) PPC_WEAK_FUNC(msgMsgSink_vfn_45);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_45) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r27,r11,144
	var_r27 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,344(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 344);
	// li r28,0
	var_r28 = 0;
	// cmplwi cr6,r11,32
	// mr r29,r28
	var_r29 = (uint32_t)(var_r28);
	// beq cr6,0x82457900
	if (ctx.r11.u32 != 32) {
		// cmplwi cr6,r11,16
		// bne cr6,0x82457828
		if (ctx.r11.u32 == 16) {
			// lwz r11,60(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 60);
			// cmpwi cr6,r11,3
			// beq cr6,0x82457900
			if (ctx.r11.s32 == 3) {
				// mr r3,r27
				ctx.r3.u64 = var_r27;
				// bl 0x82585dfc
				__imp__RtlLeaveCriticalSection(ctx, base);
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				return;
			}
			// clrlwi r10,r30,31
			ctx.r10.u64 = var_r30 & 0x1;
			// cmplwi cr6,r10,0
			// beq cr6,0x82457900
			if (ctx.r10.u32 == 0) {
				// mr r3,r27
				ctx.r3.u64 = var_r27;
				// bl 0x82585dfc
				__imp__RtlLeaveCriticalSection(ctx, base);
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				return;
			}
		}
	loc_82457828:
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244db70
		msgMsgSink_DB70_g(ctx, base);
		// lwz r3,52(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
		// cmplwi cr6,r3,0
		// beq cr6,0x82457864
		if (ctx.r3.u32 != 0) {
			// lwz r9,60(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 60);
			// li r4,1
			ctx.r4.s64 = 1;
			// cmpwi cr6,r9,3
			// beq cr6,0x82457854
			if (ctx.r9.s32 != 3) {
				// mr r4,r28
				ctx.r4.u64 = var_r28;
			}
		loc_82457854:
			// bl 0x82455098
			msgMsgSink_5098_g(ctx, base);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
			// cmpwi cr6,r29,0
			// blt cr6,0x82457900
			if ((int32_t)var_r29 < 0) {
				// mr r3,r27
				ctx.r3.u64 = var_r27;
				// bl 0x82585dfc
				__imp__RtlLeaveCriticalSection(ctx, base);
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				return;
			}
		}
	loc_82457864:
		// lwz r3,392(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
		// cmplwi cr6,r3,0
		// beq cr6,0x8245788c
		if (ctx.r3.u32 != 0) {
			// lwz r8,60(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 60);
			// li r4,1
			ctx.r4.s64 = 1;
			// cmpwi cr6,r8,3
			// beq cr6,0x82457884
			if (ctx.r8.s32 != 3) {
				// mr r4,r28
				ctx.r4.u64 = var_r28;
			}
		loc_82457884:
			// bl 0x82455098
			msgMsgSink_5098_g(ctx, base);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
		}
	loc_8245788C:
		// cmpwi cr6,r29,0
		// blt cr6,0x82457900
		if ((int32_t)var_r29 < 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			return;
		}
		// lwz r3,396(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
		// cmplwi cr6,r3,0
		// beq cr6,0x824578bc
		if (ctx.r3.u32 != 0) {
			// lwz r7,60(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 60);
			// li r4,1
			ctx.r4.s64 = 1;
			// cmpwi cr6,r7,3
			// beq cr6,0x824578b4
			if (ctx.r7.s32 != 3) {
				// mr r4,r28
				ctx.r4.u64 = var_r28;
			}
		loc_824578B4:
			// bl 0x82455098
			msgMsgSink_5098_g(ctx, base);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
		}
	loc_824578BC:
		// cmpwi cr6,r29,0
		// blt cr6,0x82457900
		if ((int32_t)var_r29 < 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			return;
		}
		// li r6,16
		ctx.r6.s64 = 16;
		// lwz r11,388(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 388);
		// cmplwi cr6,r11,0
		// stw r6,344(r31)
		PPC_STORE_U32(var_r31 + 344, ctx.r6.u32);
		// beq cr6,0x824578e0
		if (ctx.r11.u32 != 0) {
			// stw r28,64(r11)
			PPC_STORE_U32(ctx.r11.u32 + 64, var_r28);
			// stw r28,388(r31)
			PPC_STORE_U32(var_r31 + 388, var_r28);
		}
	loc_824578E0:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r28,348(r31)
		PPC_STORE_U32(var_r31 + 348, var_r28);
		// stw r28,364(r31)
		PPC_STORE_U32(var_r31 + 364, var_r28);
		// stw r28,368(r31)
		PPC_STORE_U32(var_r31 + 368, var_r28);
		// stw r28,372(r31)
		PPC_STORE_U32(var_r31 + 372, var_r28);
		// stw r28,360(r31)
		PPC_STORE_U32(var_r31 + 360, var_r28);
		// stw r28,412(r31)
		PPC_STORE_U32(var_r31 + 412, var_r28);
		// bl 0x82456f40
		game_6F40_h(ctx, base);
	}
loc_82457900:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__rage_7918"))) PPC_WEAK_FUNC(rage_7918);
PPC_FUNC_IMPL(__imp__rage_7918) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// addi r4,r31,340
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 340;
	// bne cr6,0x8245793c
	if (var_r31 == 0) {
		// li r4,0
		ctx.r4.s64 = 0;
	}
loc_8245793C:
	// lwz r11,352(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 352);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// bl 0x8244c1a8
	rage_C1A8(ctx, base);
	// lwz r11,352(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 352);
	// li r10,1
	ctx.r10.s64 = 1;
	// lfs f0,20(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,356(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 356, temp.u32);
	// stw r10,360(r31)
	PPC_STORE_U32(var_r31 + 360, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_7970_w"))) PPC_WEAK_FUNC(msgMsgSink_7970_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_7970_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x82456f90
	msgMsgSink_6F90_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824579b0
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32775
		ctx.r4.u64 = ctx.r4.u64 | 32775;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824579B0:
	// blr
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_79C8_h"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_79C8_h);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_79C8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	double var_f31 = 0.0;
	// FRAME: size=160, savegprlr_26
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r27,r11,144
	var_r27 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// li r26,0
	var_r26 = 0;
	// li r11,-1
	// li r10,-1
	// mr r3,r26
	ctx.r3.u64 = var_r26;
	// stw r26,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r26);
	// lhz r4,404(r31)
	ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 404);
	// lwz r29,288(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 288));
	// sth r11,80(r1)
	PPC_STORE_U16(ctx.r1.u32 + 80, ctx.r11.u16);
	// cmplwi cr6,r4,65535
	// stw r10,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r10.u32);
	// beq cr6,0x82457a2c
	if (ctx.r4.u32 != 65535) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// fmr f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = var_f31;
		// bl 0x8245ac18
		RtlEnterCriticalSection_AC18_2hr(ctx, base);
	}
loc_82457A2C:
	// mr r30,r26
	var_r30 = (uint32_t)(var_r26);
	// cmpwi cr6,r3,0
	// bne cr6,0x82457b24
	if (ctx.r3.s32 == 0) {
		// lwz r8,60(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 60);
		// cmpwi cr6,r8,1
		// beq cr6,0x82457a64
		if (ctx.r8.s32 != 1) {
			// addi r6,r1,84
			ctx.r6.s64 = ctx.r1.s64 + 84;
			// fmr f1,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = var_f31;
			// addi r5,r1,80
			ctx.r5.s64 = ctx.r1.s64 + 80;
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x8245aac8
			pongNetMessageHolder_AAC8_w(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// cmpwi cr6,r30,0
			// blt cr6,0x82457b24
			if ((int32_t)var_r30 < 0) {
				// mr r3,r27
				ctx.r3.u64 = var_r27;
				// bl 0x82585dfc
				__imp__RtlLeaveCriticalSection(ctx, base);
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				return;
			}
		}
	loc_82457A64:
		// lhz r4,404(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 404);
		// lhz r5,80(r1)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r1.u32 + 80);
		// mr r6,r4
		ctx.r6.u64 = ctx.r4.u64;
		// mr r7,r5
		ctx.r7.u64 = ctx.r5.u64;
		// cmplw cr6,r6,r7
		// beq cr6,0x82457b24
		if (ctx.r6.u32 == ctx.r7.u32) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// addi r29,r31,408
		var_r29 = (uint32_t)(var_r31 + 408);
		// lwz r3,384(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 384);
		// mr r6,r29
		ctx.r6.u64 = var_r29;
		// bl 0x8245b540
		RtlEnterCriticalSection_B540_2h(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x82457aa0
		if ((int32_t)var_r30 >= 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x824569a8
			RtlEnterCriticalSection_69A8_wrh(ctx, base);
		}
	loc_82457AA0:
		// stw r26,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r26);
		// cmpwi cr6,r30,0
		// blt cr6,0x82457b24
		if ((int32_t)var_r30 < 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lhz r4,80(r1)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r1.u32 + 80);
		// cmplwi cr6,r4,65535
		// beq cr6,0x82457acc
		if (ctx.r4.u32 != 65535) {
			// addi r5,r1,88
			ctx.r5.s64 = ctx.r1.s64 + 88;
			// lwz r4,84(r1)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x82457690
			RtlEnterCriticalSection_7690_wrh(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
		}
	loc_82457ACC:
		// cmpwi cr6,r30,0
		// blt cr6,0x82457b24
		if ((int32_t)var_r30 < 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lhz r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r1.u32 + 80);
		// lwz r10,88(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// lwz r3,0(r29)
		ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 0);
		// stw r26,92(r1)
		PPC_STORE_U32(ctx.r1.u32 + 92, var_r26);
		// sth r11,406(r31)
		PPC_STORE_U16(var_r31 + 406, ctx.r11.u16);
		// stw r10,396(r31)
		PPC_STORE_U32(var_r31 + 396, ctx.r10.u32);
		// lwz r4,0(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// cmpwi cr6,r4,-1
		// beq cr6,0x82457b0c
		if (ctx.r4.s32 != -1) {
			// li r6,0
			ctx.r6.s64 = 0;
			// addi r5,r1,92
			ctx.r5.s64 = ctx.r1.s64 + 92;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8244e860
			msgMsgSink_E860_g(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
		}
	loc_82457B0C:
		// cmpwi cr6,r30,0
		// blt cr6,0x82457b24
		if ((int32_t)var_r30 < 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// lwz r8,92(r1)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
		// li r9,1
		ctx.r9.s64 = 1;
		// stw r8,392(r31)
		PPC_STORE_U32(var_r31 + 392, ctx.r8.u32);
		// stw r9,0(r28)
		PPC_STORE_U32(var_r28 + 0, ctx.r9.u32);
	}
loc_82457B24:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_44"))) PPC_WEAK_FUNC(msgMsgSink_vfn_44);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_44) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
	// addi r29,r11,144
	var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x82000090
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,344(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 344);
	// li r30,0
	var_r30 = 0;
	// cmplwi cr6,r11,4
	// bne cr6,0x82457b7c
	if (ctx.r11.u32 == 4) {
		// lwz r10,348(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 348);
		// cmpwi cr6,r10,0
		// beq cr6,0x82457b84
		if (ctx.r10.s32 == 0) goto loc_82457B84;
	}
loc_82457B7C:
	// lis r30,-30009
	var_r30 = (uint32_t)(-1966669824);
	// ori r30,r30,6
	var_r30 = (uint32_t)(var_r30 | 6);
loc_82457B84:
	// cmpwi cr6,r30,0
	// blt cr6,0x82457be0
	if ((int32_t)var_r30 >= 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,116(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 116);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// cmpwi cr6,r3,1
		// beq cr6,0x82457bf4
		if (ctx.r3.s32 == 1) {
			// lis r30,-30009
			var_r30 = (uint32_t)(-1966669824);
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// ori r30,r30,8
			var_r30 = (uint32_t)(var_r30 | 8);
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// cmpwi cr6,r3,2
		// beq cr6,0x82457be0
		if (ctx.r3.s32 == 2) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x82585dfc
			__imp__RtlLeaveCriticalSection(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,364(r31)
		PPC_STORE_U32(var_r31 + 364, ctx.r11.u32);
		// bl 0x82457918
		rage_7918(ctx, base);
		// lwz r7,0(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// li r6,8
		ctx.r6.s64 = 8;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,112(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 112);
		// stw r6,344(r31)
		PPC_STORE_U32(var_r31 + 344, ctx.r6.u32);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
	}
loc_82457BE0:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_62"))) PPC_WEAK_FUNC(msgMsgSink_vfn_62);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_62) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,360(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 360);
	// cmpwi cr6,r11,0
	// bne cr6,0x82457c40
	if (ctx.r11.s32 == 0) {
		// lwz r10,60(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 60);
		// cmpwi cr6,r10,1
		// bne cr6,0x82457d00
		if (ctx.r10.s32 != 1) goto loc_82457D00;
	}
loc_82457C40:
	// lwz r9,412(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 412);
	// cmpwi cr6,r9,0
	// bne cr6,0x82457d00
	if (ctx.r9.s32 == 0) {
		// lwz r11,388(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 388);
		// li r30,0
		var_r30 = 0;
		// cmplwi cr6,r11,0
		// beq cr6,0x82457c64
		if (ctx.r11.u32 != 0) {
			// stw r30,64(r11)
			PPC_STORE_U32(ctx.r11.u32 + 64, var_r30);
			// stw r30,388(r31)
			PPC_STORE_U32(var_r31 + 388, var_r30);
		}
	loc_82457C64:
		// lwz r3,392(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
		// cmplwi cr6,r3,0
		// beq cr6,0x82457c98
		if (ctx.r3.u32 != 0) {
			// lwz r11,80(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 80);
			// cmpwi cr6,r11,4
			// beq cr6,0x82457c98
			if (ctx.r11.s32 == 4) goto loc_82457C98;
			// cmpwi cr6,r11,5
			// beq cr6,0x82457c98
			if (ctx.r11.s32 == 5) goto loc_82457C98;
			// lwz r8,0(r3)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// lwz r7,4(r8)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 4);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r7.u32);
		}
	loc_82457C98:
		// lwz r3,396(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
		// cmplwi cr6,r3,0
		// beq cr6,0x82457ccc
		if (ctx.r3.u32 != 0) {
			// lwz r11,80(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 80);
			// cmpwi cr6,r11,4
			// beq cr6,0x82457ccc
			if (ctx.r11.s32 == 4) goto loc_82457CCC;
			// cmpwi cr6,r11,5
			// beq cr6,0x82457ccc
			if (ctx.r11.s32 == 5) goto loc_82457CCC;
			// lwz r6,0(r3)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// lwz r5,4(r6)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 4);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		}
	loc_82457CCC:
		// addi r5,r1,80
		ctx.r5.s64 = ctx.r1.s64 + 80;
		// lfs f1,356(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 356);
		ctx.f1.f64 = double(temp.f32);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bl 0x824579c8
		RtlEnterCriticalSection_79C8_h(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x82457cfc
		if (ctx.r3.s32 >= 0) {
			// lwz r4,80(r1)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// cmpwi cr6,r4,0
			// beq cr6,0x82457cfc
			if (ctx.r4.s32 == 0) goto loc_82457CFC;
			// li r3,1
			ctx.r3.s64 = 1;
			// stw r3,412(r31)
			PPC_STORE_U32(var_r31 + 412, ctx.r3.u32);
		}
	loc_82457CFC:
		// stw r30,360(r31)
		PPC_STORE_U32(var_r31 + 360, var_r30);
	}
loc_82457D00:
	// lwz r3,392(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 392);
	// cmplwi cr6,r3,0
	// beq cr6,0x82457d10
	if (ctx.r3.u32 != 0) {
		// bl 0x82454550
		msgMsgSink_4550_g(ctx, base);
	}
loc_82457D10:
	// lwz r3,396(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 396);
	// cmplwi cr6,r3,0
	// beq cr6,0x82457d20
	if (ctx.r3.u32 != 0) {
		// bl 0x82454550
		msgMsgSink_4550_g(ctx, base);
	}
loc_82457D20:
	// lwz r3,400(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 400);
	// cmplwi cr6,r3,0
	// beq cr6,0x82457d30
	if (ctx.r3.u32 != 0) {
		// bl 0x82454550
		msgMsgSink_4550_g(ctx, base);
	}
loc_82457D30:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244e0c0
	msgMsgSink_vfn_40(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_7D50_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_7D50_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_7D50_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82456090
	snNotifyGamerAdded_6090_fw(ctx, base);
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// addi r11,r11,20464
	ctx.r11.s64 = ctx.r11.s64 + 20464;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,20452
	ctx.r9.s64 = ctx.r9.s64 + 20452;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_7DA8_p33"))) PPC_WEAK_FUNC(msgMsgSink_7DA8_p33);
PPC_FUNC_IMPL(__imp__msgMsgSink_7DA8_p33) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// addi r11,r11,20464
	ctx.r11.s64 = ctx.r11.s64 + 20464;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,20452
	ctx.r9.s64 = ctx.r9.s64 + 20452;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r9.u32);
	// b 0x82456138
	msgMsgSink_6138_2hr(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_77"))) PPC_WEAK_FUNC(msgMsgSink_vfn_77);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_77) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-8
	ctx.r3.s64 = ctx.r3.s64 + -8;
	// b 0x82457dd8
	msgMsgSink_7DD8_w(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_7DD8_w"))) PPC_WEAK_FUNC(msgMsgSink_7DD8_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_7DD8_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20464
	ctx.r11.s64 = ctx.r11.s64 + 20464;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,20452
	ctx.r9.s64 = ctx.r9.s64 + 20452;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// bl 0x82456138
	msgMsgSink_6138_2hr(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82457e3c
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32863
		ctx.r4.u64 = ctx.r4.u64 | 32863;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82457E3C:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_89_7E58_1"))) PPC_WEAK_FUNC(msgMsgSink_vfn_89_7E58_1);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_89_7E58_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// lwz r10,24(r11)
	// bctrl
	VCALL(ctx.r3.u32, 6, ctx, base);  // msgMsgSink::vfn_6 (unnamed)  // vtable slot 6 (byte +24)
	// clrlwi r9,r30,16
	ctx.r9.u64 = var_r30 & 0xFFFF;
	// cmplw cr6,r9,r3
	// blt cr6,0x82457ea0
	if (ctx.r9.u32 >= ctx.r3.u32) {
		// lis r3,-30009
		// ori r3,r3,13
		ctx.r3.u64 = ctx.r3.u64 | 13;
		return;
	}
loc_82457EA0:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,92
	ctx.r3.s64 = 92;
	// ori r4,r4,32794
	ctx.r4.u64 = ctx.r4.u64 | 32794;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82457ed8
	if (ctx.r3.u32 != 0) {
		// mr r7,r29
		ctx.r7.u64 = var_r29;
		// lwz r4,20(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r6,r30
		ctx.r6.u64 = var_r30;
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// bl 0x8245b570
		msgMsgSink_B570_fw(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// cmplwi cr6,r11,0
		// bne cr6,0x82457ee8
		if (ctx.r11.u32 != 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			// stw r11,0(r28)
			PPC_STORE_U32(var_r28 + 0, ctx.r11.u32);
			return;
		}
	}
loc_82457ED8:
	// lis r3,-32761
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	return;
}

__attribute__((alias("__imp__msgMsgSink_7EF8_g"))) PPC_WEAK_FUNC(msgMsgSink_7EF8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_7EF8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,64(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 64);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	// lwz r7,0(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r6,64(r7)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 64);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
	// add r11,r29,r30
	ctx.r11.u64 = var_r29 + var_r30;
	// twllei r28,0
	if ((int32_t)var_r28 == 0 || var_r28 < 0u) __builtin_trap();
	// addi r5,r11,-1
	ctx.r5.s64 = ctx.r11.s64 + -1;
	// divwu r4,r5,r28
	ctx.r4.u32 = var_r28 ? ctx.r5.u32 / var_r28 : 0;
	// mullw r3,r4,r3
	ctx.r3.s64 = int64_t(ctx.r4.s32) * int64_t(ctx.r3.s32);
	return;
}

__attribute__((alias("__imp__atSingleton_7F68_g"))) PPC_WEAK_FUNC(atSingleton_7F68_g);
PPC_FUNC_IMPL(__imp__atSingleton_7F68_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lwz r10,64(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 64);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// divwu r9,r31,r3
	ctx.r9.u32 = ctx.r3.u32 ? var_r31 / ctx.r3.u32 : 0;
	// twllei r3,0
	if (ctx.r3.s32 == 0 || ctx.r3.u32 < 0u) __builtin_trap();
	// mullw r8,r9,r3
	ctx.r8.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r3.s32);
	// subf r7,r8,r31
	ctx.r7.s64 = (int64_t)(int32_t)var_r31 - ctx.r8.s64;
	// subf r3,r7,r31
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 - ctx.r7.s64;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_7FB8_p33"))) PPC_WEAK_FUNC(msgMsgSink_7FB8_p33);
PPC_FUNC_IMPL(__imp__msgMsgSink_7FB8_p33) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// lis r8,-32256
	// addi r11,r11,20528
	ctx.r11.s64 = ctx.r11.s64 + 20528;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,20524
	ctx.r9.s64 = ctx.r9.s64 + 20524;
	// addi r8,r8,15816
	ctx.r8.s64 = ctx.r8.s64 + 15816;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r9.u32);
	// stw r8,292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 292, ctx.r8.u32);
	// b 0x82456138
	msgMsgSink_6138_2hr(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_95"))) PPC_WEAK_FUNC(msgMsgSink_vfn_95);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_95) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-8
	ctx.r3.s64 = ctx.r3.s64 + -8;
	// b 0x82458638
	msgMsgSink_8638_w(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_105"))) PPC_WEAK_FUNC(msgMsgSink_vfn_105);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_105) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// li r27,0
	var_r27 = 0;
	// lwz r11,252(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 252);
	// cmpwi cr6,r11,0
	// bne cr6,0x82458034
	if (ctx.r11.s32 == 0) {
		// lis r3,-30009
		// ori r3,r3,22
		ctx.r3.u64 = ctx.r3.u64 | 22;
		return;
	}
loc_82458034:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r9,24(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
	// clrlwi r8,r30,16
	ctx.r8.u64 = var_r30 & 0xFFFF;
	// cmplw cr6,r8,r3
	// blt cr6,0x82458064
	if (ctx.r8.u32 >= ctx.r3.u32) {
		// lis r3,-30009
		// ori r3,r3,13
		ctx.r3.u64 = ctx.r3.u64 | 13;
		return;
	}
loc_82458064:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,216
	ctx.r3.s64 = 216;
	// ori r4,r4,32795
	ctx.r4.u64 = ctx.r4.u64 | 32795;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245809c
	if (ctx.r3.u32 != 0) {
		// mr r7,r29
		ctx.r7.u64 = var_r29;
		// lwz r4,20(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r6,r30
		ctx.r6.u64 = var_r30;
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// bl 0x8245c078
		msgMsgSink_C078_fw(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// cmplwi cr6,r11,0
		// bne cr6,0x824580ac
		if (ctx.r11.u32 != 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// stw r11,0(r28)
			PPC_STORE_U32(var_r28 + 0, ctx.r11.u32);
			return;
		}
	}
loc_8245809C:
	// lis r3,-32761
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_108"))) PPC_WEAK_FUNC(msgMsgSink_vfn_108);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_108) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,252(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 252);
	// cmpwi cr6,r11,0
	// bne cr6,0x82458130
	if (ctx.r11.s32 == 0) {
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// li r6,0
		ctx.r6.s64 = 0;
		// lwz r3,256(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 256);
		// addi r5,r1,80
		ctx.r5.s64 = ctx.r1.s64 + 80;
		// addi r4,r31,272
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 272;
		// lwz r10,188(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 188);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		// cmpwi cr6,r3,0
		// beq cr6,0x82458138
		if (ctx.r3.s32 == 0) {
			// blr
			return;
		}
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,76(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 76);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// blr
		return;
	}
loc_82458130:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82455fd0
	msgMsgSink_vfn_36_5FD0_1(ctx, base);
loc_82458138:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_8150_p39"))) PPC_WEAK_FUNC(msgMsgSink_8150_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_8150_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// mr r5,r6
	ctx.r5.u64 = ctx.r6.u64;
	// li r31,0
	var_r31 = 0;
	// lwz r11,20(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// li r6,0
	ctx.r6.s64 = 0;
	// lwz r11,184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 184);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// cmpwi cr6,r3,0
	// bne cr6,0x824581c0
	if (ctx.r3.s32 == 0) {
		// bl 0x8242c3b0
		thunk_fn_8242C368(ctx, base);
		// cmpwi cr6,r3,997
		// beq cr6,0x824581c0
		if (ctx.r3.s32 == 997) goto loc_824581C0;
		// cmpwi cr6,r3,0
		// ble cr6,0x824581c4
		if (ctx.r3.s32 <= 0) {
			// blr
			return;
		}
		// clrlwi r10,r3,16
		ctx.r10.u64 = ctx.r3.u32 & 0xFFFF;
		// oris r3,r10,32775
		ctx.r3.u64 = ctx.r10.u64 | 2147942400;
		// blr
		return;
	}
loc_824581C0:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
loc_824581C4:
	// blr
	return;
}

__attribute__((alias("__imp__game_81D8"))) PPC_WEAK_FUNC(game_81D8);
PPC_FUNC_IMPL(__imp__game_81D8) {
	PPC_FUNC_PROLOGUE();
	// mr r7,r6
	ctx.r7.u64 = ctx.r6.u64;
	// lwz r11,260(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 260);
	// mr r6,r5
	ctx.r6.u64 = ctx.r5.u64;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r10,8(r7)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// rldicl r9,r11,32,32
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFF;
	// rotlwi r11,r9,0
	ctx.r11.u64 = ctx.r9.u32;
	// stw r10,8(r7)
	PPC_STORE_U32(ctx.r7.u32 + 8, ctx.r10.u32);
	// lwz r10,12(r7)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r7.u32 + 12);
	// add r8,r11,r10
	ctx.r8.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stw r8,12(r7)
	PPC_STORE_U32(ctx.r7.u32 + 12, ctx.r8.u32);
	// lwz r4,256(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 256);
	// b 0x82458150
	msgMsgSink_8150_p39(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_113"))) PPC_WEAK_FUNC(msgMsgSink_vfn_113);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_113) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r26 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r21 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=240, savegprlr_21
	// li r26,0
	var_r26 = 0;
	// addi r11,r1,84
	ctx.r11.s64 = ctx.r1.s64 + 84;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r26,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r26);
	// mr r22,r5
	var_r22 = ctx.r5.u32;
	// stw r26,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r26);
	// mr r21,r6
	var_r21 = ctx.r6.u32;
	// mr r23,r7
	var_r23 = ctx.r7.u32;
	// mr r25,r26
	var_r25 = (uint32_t)(var_r26);
	// bl 0x82451b98
	msgMsgSink_1B98_g(ctx, base);
	// addi r11,r1,136
	ctx.r11.s64 = ctx.r1.s64 + 136;
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r26,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r26);
	// stw r26,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r26);
	// bl 0x82455eb0
	msgMsgSink_5EB0_g(ctx, base);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8215d6a8
	grcTextureReferenceBase_vfn_4(ctx, base);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82451d30
	msgMsgSink_1D30_g(ctx, base);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82451d80
	msgMsgSink_1D80_g(ctx, base);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82451c08
	msgMsgSink_1C08_g(ctx, base);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82451be0
	msgMsgSink_1BE0_g(ctx, base);
	// cmpwi cr6,r3,1
	// bne cr6,0x8245834c
	if (ctx.r3.s32 == 1) {
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x82451d20
		msgMsgSink_1D20_g(ctx, base);
		// cmplwi cr6,r3,6
		// bne cr6,0x8245834c
		if (ctx.r3.u32 != 6) goto loc_8245834C;
		// lis r11,10922
		ctx.r11.s64 = 715784192;
		// ori r30,r11,43691
		var_r30 = (uint32_t)(ctx.r11.u64 | 43691);
		// lhz r11,268(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 268);
		// mulhw r10,r11,r30
		ctx.r10.s64 = (int64_t(ctx.r11.s32) * int64_t((int32_t)var_r30)) >> 32;
		// srawi r10,r10,1
		ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0x1) != 0);
		ctx.r10.s64 = ctx.r10.s32 >> 1;
		// rlwinm r9,r10,1,31,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0x1;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r9,r10,r9
		ctx.r9.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r8,r9,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
		// subf. r7,r8,r11
		ctx.r7.s64 = ctx.r11.s64 - ctx.r8.s64;
		// beq 0x8245834c
		if (ctx.r7.s32 == 0) goto loc_8245834C;
		// cmplwi cr6,r11,12
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bge cr6,0x82458310
		if (ctx.r11.u32 < 12) {
			// lwz r5,64(r6)
			// bctrl
			VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
			// rlwinm r11,r3,1,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r3,r11
			ctx.r4.u64 = ctx.r3.u64 + ctx.r11.u64;
			// rlwinm r27,r4,2,0,29
			var_r27 = (uint32_t)(__builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC);
			// b 0x82458368
			goto loc_82458368;
		}
		// lwz r10,64(r11)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
		// lhz r11,268(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 268);
		// mulhw r9,r11,r30
		ctx.r9.s64 = (int64_t(ctx.r11.s32) * int64_t((int32_t)var_r30)) >> 32;
		// srawi r11,r9,1
		ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0x1) != 0);
		ctx.r11.s64 = ctx.r9.s32 >> 1;
		// rlwinm r10,r11,1,31,31
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0x1;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// addi r8,r11,1
		ctx.r8.s64 = ctx.r11.s64 + 1;
		// mullw r11,r8,r3
		ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r3.s32);
		// rlwinm r10,r11,1,0,30
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// add r7,r11,r10
		ctx.r7.u64 = ctx.r11.u64 + ctx.r10.u64;
		// rlwinm r27,r7,2,0,29
		var_r27 = (uint32_t)(__builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC);
		// b 0x82458368
	} else {
	loc_8245834C:
		// lwz r6,0(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,64(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 64);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		// lhz r4,268(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 268);
		// mullw r27,r4,r3
		var_r27 = (uint32_t)(int64_t(ctx.r4.s32) * int64_t(ctx.r3.s32));
	}
loc_82458368:
	// rlwinm r3,r27,1,0,30
	ctx.r3.u64 = __builtin_rotateleft64(var_r27 | (var_r27 << 32), 1) & 0xFFFFFFFE;
	// cmplw cr6,r29,r3
	// ble cr6,0x82458398
	if (var_r29 > ctx.r3.u32) {
		// clrlwi r11,r23,24
		ctx.r11.u64 = var_r23 & 0xFF;
		// cmplwi cr6,r11,0
		// beq cr6,0x82458390
		if (ctx.r11.u32 != 0) {
			// rlwinm r11,r27,1,0,30
			ctx.r11.u64 = __builtin_rotateleft64(var_r27 | (var_r27 << 32), 1) & 0xFFFFFFFE;
			// add r10,r27,r11
			ctx.r10.u64 = var_r27 + ctx.r11.u64;
			// cmplw cr6,r29,r10
			// ble cr6,0x82458398
			if (var_r29 <= ctx.r10.u32) goto loc_82458398;
		}
	loc_82458390:
		// li r29,2
		var_r29 = 2;
		// b 0x824583ac
	} else {
	loc_82458398:
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// li r29,1
		var_r29 = 1;
		// bl 0x82457ef8
		msgMsgSink_7EF8_g(ctx, base);
		// mr r27,r3
		var_r27 = ctx.r3.u32;
	}
loc_824583AC:
	// lis r9,8324
	ctx.r9.s64 = 545521664;
	// mr r31,r26
	var_r31 = (uint32_t)(var_r26);
	// addi r30,r1,80
	var_r30 = (uint32_t)(ctx.r1.s64 + 80);
	// ori r24,r9,32804
	var_r24 = (uint32_t)(ctx.r9.u64 | 32804);
loc_824583BC:
	// cmpw cr6,r31,r29
	// bge cr6,0x82458450
	if ((int32_t)var_r31 >= (int32_t)var_r29) goto loc_82458450;
	// mr r4,r24
	ctx.r4.u64 = var_r24;
	// li r3,140
	ctx.r3.s64 = 140;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x824583e0
	if (ctx.r3.u32 != 0) {
		// bl 0x8245bf40
		msgMsgSink_BF40_g(ctx, base);
		// b 0x824583e4
	} else {
	loc_824583E0:
		// mr r3,r26
		ctx.r3.u64 = var_r26;
	}
loc_824583E4:
	// cmplwi cr6,r3,0
	// stw r3,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r3.u32);
	// bne cr6,0x824583f8
	if (ctx.r3.u32 == 0) {
		// lis r25,-32761
		var_r25 = (uint32_t)(-2147024896);
		// ori r25,r25,14
		var_r25 = (uint32_t)(var_r25 | 14);
	}
loc_824583F8:
	// addi r31,r31,1
	var_r31 = (uint32_t)(var_r31 + 1);
	// addi r30,r30,4
	var_r30 = (uint32_t)(var_r30 + 4);
	// cmpwi cr6,r25,0
	// bge cr6,0x824583bc
	if ((int32_t)var_r25 >= 0) goto loc_824583BC;
loc_82458408:
	// cmpwi cr6,r29,0
	// ble cr6,0x82458444
	if ((int32_t)var_r29 > 0) {
		// addi r30,r1,80
		var_r30 = (uint32_t)(ctx.r1.s64 + 80);
	loc_82458414:
		// lwz r31,0(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 0)/* msgMsgSink::vtable@+0x0 */);
		// cmplwi cr6,r31,0
		// beq cr6,0x82458434
		if (var_r31 != 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8245bfa0
			atSingleton_BFA0_g(ctx, base);
			// mr r4,r24
			ctx.r4.u64 = var_r24;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x820c02d0
			_locale_register(ctx, base);
		}
	loc_82458434:
		// addi r29,r29,-1
		var_r29 = (uint32_t)(var_r29 + -1);
		// addi r30,r30,4
		var_r30 = (uint32_t)(var_r30 + 4);
		// cmplwi cr6,r29,0
		// bne cr6,0x82458414
		if (var_r29 != 0) goto loc_82458414;
	}
loc_82458444:
	// mr r3,r25
	ctx.r3.u64 = var_r25;
	return;
loc_82458450:
	// cmpwi cr6,r25,0
	// blt cr6,0x82458408
	if ((int32_t)var_r25 < 0) goto loc_82458408;
	// clrlwi r8,r23,24
	ctx.r8.u64 = var_r23 & 0xFF;
	// lwz r28,80(r1)
	var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 80));
	// cmplwi cr6,r8,0
	// beq cr6,0x824584b4
	if (ctx.r8.u32 == 0) goto loc_824584B4;
	// cmpwi cr6,r29,1
	// bne cr6,0x824584b4
	if ((int32_t)var_r29 != 1) goto loc_824584B4;
	// mr r5,r23
	ctx.r5.u64 = var_r23;
	// mr r4,r27
	ctx.r4.u64 = var_r27;
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x8245bfc0
	msgMsgSink_BFC0_g(ctx, base);
	// mr r25,r3
	var_r25 = ctx.r3.u32;
	// addi r5,r1,136
	ctx.r5.s64 = ctx.r1.s64 + 136;
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82451c40
	msgMsgSink_1C40_wrh(ctx, base);
loc_82458494:
	// cmpwi cr6,r25,0
	// blt cr6,0x82458408
	if ((int32_t)var_r25 < 0) goto loc_82458408;
	// lwz r7,84(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// mr r3,r25
	ctx.r3.u64 = var_r25;
	// stw r28,0(r22)
	PPC_STORE_U32(var_r22 + 0, var_r28);
	// stw r7,0(r21)
	PPC_STORE_U32(var_r21 + 0, ctx.r7.u32);
	return;
loc_824584B4:
	// mr r31,r26
	var_r31 = (uint32_t)(var_r26);
	// addi r30,r1,80
	var_r30 = (uint32_t)(ctx.r1.s64 + 80);
loc_824584BC:
	// cmpw cr6,r31,r29
	// bge cr6,0x82458494
	if ((int32_t)var_r31 >= (int32_t)var_r29) goto loc_82458494;
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r4,r27
	ctx.r4.u64 = var_r27;
	// bl 0x8245bfc0
	msgMsgSink_BFC0_g(ctx, base);
	// mr r25,r3
	var_r25 = ctx.r3.u32;
	// addi r31,r31,1
	var_r31 = (uint32_t)(var_r31 + 1);
	// addi r30,r30,4
	var_r30 = (uint32_t)(var_r30 + 4);
	// cmpwi cr6,r25,0
	// bge cr6,0x824584bc
	if ((int32_t)var_r25 >= 0) goto loc_824584BC;
	// b 0x82458408
	goto loc_82458408;
}

__attribute__((alias("__imp__msgMsgSink_vfn_116"))) PPC_WEAK_FUNC(msgMsgSink_vfn_116);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_116) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-12
	ctx.r3.s64 = ctx.r3.s64 + -12;
	// b 0x82458a88
	msgMsgSink_vfn_121(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_84F8_h"))) PPC_WEAK_FUNC(atSingleton_84F8_h);
PPC_FUNC_IMPL(__imp__atSingleton_84F8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20628
	ctx.r11.s64 = ctx.r11.s64 + 20628;
	// lis r10,-32256
	// addi r10,r10,20608
	ctx.r10.s64 = ctx.r10.s64 + 20608;
	// lwz r9,4(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// stw r10,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r10.u32);
	// stw r11,8(r9)
	PPC_STORE_U32(ctx.r9.u32 + 8, ctx.r11.u32);
	// lwz r8,8(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r7,4(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// stw r7,4(r8)
	PPC_STORE_U32(ctx.r8.u32 + 4, ctx.r7.u32);
	// lwz r30,24(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 24));
	// stw r31,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ var_r31);
	// stw r31,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r31);
	// cmplwi cr6,r30,0
	// beq cr6,0x8245856c
	if (var_r30 != 0) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8245bfa0
		atSingleton_BFA0_g(ctx, base);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// ori r4,r4,32804
		ctx.r4.u64 = ctx.r4.u64 | 32804;
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_8245856C:
	// lis r11,-32256
	// lis r10,-32256
	// addi r11,r11,15792
	ctx.r11.s64 = ctx.r11.s64 + 15792;
	// addi r10,r10,15816
	ctx.r10.s64 = ctx.r10.s64 + 15816;
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyGamerAdded_85A0_fw"))) PPC_WEAK_FUNC(snNotifyGamerAdded_85A0_fw);
PPC_FUNC_IMPL(__imp__snNotifyGamerAdded_85A0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82456090
	snNotifyGamerAdded_6090_fw(ctx, base);
	// lis r10,-32256
	// addi r11,r31,292
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 292;
	// addi r9,r10,20528
	ctx.r9.s64 = ctx.r10.s64 + 20528;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// addi r8,r10,20456
	ctx.r8.s64 = ctx.r10.s64 + 20456;
	// lis r10,-32256
	// stw r9,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r9.u32);
	// li r9,20
	ctx.r9.s64 = 20;
	// addi r7,r10,20524
	ctx.r7.s64 = ctx.r10.s64 + 20524;
	// lis r10,-32256
	// stw r8,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
	// addi r8,r31,272
	ctx.r8.s64 = (int64_t)(int32_t)var_r31 + 272;
	// addi r6,r10,15816
	ctx.r6.s64 = ctx.r10.s64 + 15816;
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r7,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r7.u32);
	// stw r11,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	// stw r6,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r6.u32);
	// stw r10,256(r31)
	PPC_STORE_U32(var_r31 + 256, ctx.r10.u32);
	// stw r10,260(r31)
	PPC_STORE_U32(var_r31 + 260, ctx.r10.u32);
	// stw r10,264(r31)
	PPC_STORE_U32(var_r31 + 264, ctx.r10.u32);
	// stw r10,268(r31)
	PPC_STORE_U32(var_r31 + 268, ctx.r10.u32);
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_82458614:
	// stb r10,0(r8)
	PPC_STORE_U8(ctx.r8.u32 + 0, ctx.r10.u8);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// bdnz 0x82458614
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82458614;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_8638_w"))) PPC_WEAK_FUNC(msgMsgSink_8638_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_8638_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// lis r9,-32256
	// lis r8,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20528
	ctx.r11.s64 = ctx.r11.s64 + 20528;
	// addi r10,r10,20456
	ctx.r10.s64 = ctx.r10.s64 + 20456;
	// addi r9,r9,20524
	ctx.r9.s64 = ctx.r9.s64 + 20524;
	// addi r8,r8,15816
	ctx.r8.s64 = ctx.r8.s64 + 15816;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// stw r8,292(r31)
	PPC_STORE_U32(var_r31 + 292, ctx.r8.u32);
	// bl 0x82456138
	msgMsgSink_6138_2hr(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824586a8
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32859
		ctx.r4.u64 = ctx.r4.u64 | 32859;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824586A8:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_86C0_g"))) PPC_WEAK_FUNC(atSingleton_86C0_g);
PPC_FUNC_IMPL(__imp__atSingleton_86C0_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,296(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 296);
	// addi r10,r3,292
	ctx.r10.s64 = ctx.r3.s64 + 292;
	// cmplw cr6,r11,r10
	// beq cr6,0x824586ec
	if (ctx.r11.u32 != ctx.r10.u32) {
		// clrlwi r9,r4,16
		ctx.r9.u64 = ctx.r4.u32 & 0xFFFF;
	loc_824586D4:
		// lhz r8,20(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
		// cmplw cr6,r8,r9
		// beq cr6,0x824586f0
		if (ctx.r8.u32 == ctx.r9.u32) {
			// addi r3,r11,12
			ctx.r3.s64 = ctx.r11.s64 + 12;
			// lwz r7,0(r3)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
			// lwz r6,8(r7)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
			// mtctr r6
			ctx.ctr.u64 = ctx.r6.u64;
			// bctr
			PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
			return;
		}
		// lwz r11,4(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// cmplw cr6,r11,r10
		// bne cr6,0x824586d4
		if (ctx.r11.u32 != ctx.r10.u32) goto loc_824586D4;
	}
loc_824586EC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_824586F0:
	// addi r3,r11,12
	ctx.r3.s64 = ctx.r11.s64 + 12;
	// lwz r7,0(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// lwz r6,8(r7)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
	// mtctr r6
	ctx.ctr.u64 = ctx.r6.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_114"))) PPC_WEAK_FUNC(msgMsgSink_vfn_114);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_114) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r21 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=224, savegprlr_21
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// li r24,0
	var_r24 = 0;
	// addi r22,r29,292
	var_r22 = (uint32_t)(var_r29 + 292);
	// mr r25,r4
	var_r25 = ctx.r4.u32;
	// mr r21,r5
	var_r21 = ctx.r5.u32;
	// lwz r11,296(r29)
	ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 296);
	// mr r28,r24
	var_r28 = (uint32_t)(var_r24);
	// cmplw cr6,r11,r22
	// beq cr6,0x82458754
	if (ctx.r11.u32 != var_r22) {
		// clrlwi r10,r25,16
		ctx.r10.u64 = var_r25 & 0xFFFF;
	loc_8245873C:
		// lhz r9,20(r11)
		ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
		// cmplw cr6,r9,r10
		// beq cr6,0x824587e8
		if (ctx.r9.u32 == ctx.r10.u32) goto loc_824587E8;
		// lwz r11,4(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// cmplw cr6,r11,r22
		// bne cr6,0x8245873c
		if (ctx.r11.u32 != var_r22) goto loc_8245873C;
	}
loc_82458754:
	// mr r31,r24
	var_r31 = (uint32_t)(var_r24);
loc_82458758:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r23,r31
	var_r23 = (uint32_t)(var_r31);
	// bl 0x82451b98
	msgMsgSink_1B98_g(ctx, base);
	// addi r11,r1,120
	ctx.r11.s64 = ctx.r1.s64 + 120;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// mr r4,r25
	ctx.r4.u64 = var_r25;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// stw r24,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r24);
	// stw r24,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r24);
	// bl 0x82455eb0
	msgMsgSink_5EB0_g(ctx, base);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451d30
	msgMsgSink_1D30_g(ctx, base);
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451d80
	msgMsgSink_1D80_g(ctx, base);
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451c08
	msgMsgSink_1C08_g(ctx, base);
	// cmpwi cr6,r3,0
	// bne cr6,0x824587b8
	if (ctx.r3.s32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// mr r26,r24
		var_r26 = (uint32_t)(var_r24);
		// bl 0x8215d6a8
		grcTextureReferenceBase_vfn_4(ctx, base);
		// mr r27,r3
		var_r27 = ctx.r3.u32;
	}
loc_824587B8:
	// cmplwi cr6,r31,0
	// beq cr6,0x824587f0
	if (var_r31 == 0) goto loc_824587F0;
	// addi r3,r31,12
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lwz r30,24(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 24));
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// stw r30,0(r21)
	PPC_STORE_U32(var_r21 + 0, var_r30);
	return;
loc_824587E8:
	// mr r31,r11
	var_r31 = ctx.r11.u32;
	// b 0x82458758
	goto loc_82458758;
loc_824587F0:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82451be0
	msgMsgSink_1BE0_g(ctx, base);
	// cmpwi cr6,r3,1
	// bne cr6,0x824588a8
	if (ctx.r3.s32 == 1) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x82451d20
		msgMsgSink_1D20_g(ctx, base);
		// cmplwi cr6,r3,6
		// bne cr6,0x824588a8
		if (ctx.r3.u32 != 6) goto loc_824588A8;
		// lis r6,10922
		ctx.r6.s64 = 715784192;
		// lhz r11,268(r29)
		ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 268);
		// ori r31,r6,43691
		var_r31 = (uint32_t)(ctx.r6.u64 | 43691);
		// mulhw r5,r11,r31
		ctx.r5.s64 = (int64_t(ctx.r11.s32) * int64_t((int32_t)var_r31)) >> 32;
		// srawi r10,r5,1
		ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0x1) != 0);
		ctx.r10.s64 = ctx.r5.s32 >> 1;
		// rlwinm r9,r10,1,31,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0x1;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r4,r10,r9
		ctx.r4.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r3,r4,2,0,29
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
		// subf. r10,r3,r11
		ctx.r10.s64 = ctx.r11.s64 - ctx.r3.s64;
		// beq 0x824588a8
		if (ctx.r10.s32 == 0) goto loc_824588A8;
		// cmplwi cr6,r11,12
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bge cr6,0x8245886c
		if (ctx.r11.u32 < 12) {
			// lwz r8,64(r9)
			// bctrl
			VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
			// rlwinm r11,r3,1,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0xFFFFFFFE;
			// add r7,r3,r11
			ctx.r7.u64 = ctx.r3.u64 + ctx.r11.u64;
			// rlwinm r31,r7,2,0,29
			var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC);
			// b 0x824588c4
			goto loc_824588C4;
		}
		// lwz r5,64(r6)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
		// lhz r11,268(r29)
		ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 268);
		// mulhw r4,r11,r31
		ctx.r4.s64 = (int64_t(ctx.r11.s32) * int64_t((int32_t)var_r31)) >> 32;
		// srawi r11,r4,1
		ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0x1) != 0);
		ctx.r11.s64 = ctx.r4.s32 >> 1;
		// rlwinm r10,r11,1,31,31
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0x1;
		// add r11,r11,r10
		ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// mullw r11,r11,r3
		ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r3.s32);
		// rlwinm r10,r11,1,0,30
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r11,r10
		ctx.r10.u64 = ctx.r11.u64 + ctx.r10.u64;
		// rlwinm r31,r10,2,0,29
		var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC);
		// b 0x824588c4
	} else {
	loc_824588A8:
		// lwz r9,0(r29)
		ctx.r9.u64 = PPC_LOAD_U32(var_r29 + 0)/* msgMsgSink::vtable@+0x0 */;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lwz r8,64(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 64);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lhz r7,268(r29)
		ctx.r7.u64 = PPC_LOAD_U16(var_r29 + 268);
		// mullw r31,r7,r3
		var_r31 = (uint32_t)(int64_t(ctx.r7.s32) * int64_t(ctx.r3.s32));
	}
loc_824588C4:
	// cmplw cr6,r27,r31
	// bge cr6,0x824588e0
	if (var_r27 < var_r31) {
		// mr r4,r27
		ctx.r4.u64 = var_r27;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x82457ef8
		msgMsgSink_7EF8_g(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// b 0x824588e4
	} else {
	loc_824588E0:
		// mr r27,r31
		var_r27 = (uint32_t)(var_r31);
	}
loc_824588E4:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,140
	ctx.r3.s64 = 140;
	// ori r4,r4,32804
	ctx.r4.u64 = ctx.r4.u64 | 32804;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245894c
	if (ctx.r3.u32 != 0) {
		// bl 0x8245bf40
		msgMsgSink_BF40_g(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmplwi cr6,r30,0
		// bne cr6,0x82458954
		if (var_r30 != 0) goto loc_82458954;
	loc_8245890C:
		// lis r28,-32761
		var_r28 = (uint32_t)(-2147024896);
		// ori r28,r28,14
		var_r28 = (uint32_t)(var_r28 | 14);
	loc_82458914:
		// cmplwi cr6,r23,0
		// beq cr6,0x82458934
		if (var_r23 == 0) goto loc_82458934;
		// lwz r8,0(r23)
  // [ph4a] vtable load collapsed
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r23
		ctx.r3.u64 = var_r23;
		// lwz r7,0(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r23, 0, ctx, base);  // pattern-B slot 0 (byte +0)
	loc_82458934:
		// cmpwi cr6,r28,0
		// blt cr6,0x82458940
		if ((int32_t)var_r28 >= 0) {
			// stw r30,0(r21)
			PPC_STORE_U32(var_r21 + 0, var_r30);
		}
	loc_82458940:
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		return;
	}
loc_8245894C:
	// mr r30,r24
	var_r30 = (uint32_t)(var_r24);
	// b 0x8245890c
	goto loc_8245890C;
loc_82458954:
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245bfc0
	msgMsgSink_BFC0_g(ctx, base);
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// cmpwi cr6,r28,0
	// blt cr6,0x82458914
	if ((int32_t)var_r28 < 0) goto loc_82458914;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,28
	ctx.r3.s64 = 28;
	// ori r4,r4,32805
	ctx.r4.u64 = ctx.r4.u64 | 32805;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x82458a40
	if (var_r31 == 0) goto loc_82458A40;
	// lis r11,-32256
	// stw r31,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ var_r31);
	// lis r10,-32256
	// stw r31,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r31);
	// lis r9,-32256
	// addi r11,r11,15792
	ctx.r11.s64 = ctx.r11.s64 + 15792;
	// li r28,1
	var_r28 = 1;
	// addi r10,r10,20628
	ctx.r10.s64 = ctx.r10.s64 + 20628;
	// addi r9,r9,20608
	ctx.r9.s64 = ctx.r9.s64 + 20608;
	// mr r23,r31
	var_r23 = (uint32_t)(var_r31);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// stw r28,16(r31)
	PPC_STORE_U32(var_r31 + 16, var_r28);
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r10.u32);
	// stw r9,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r9.u32);
	// sth r25,20(r31)
	PPC_STORE_U16(var_r31 + 20, (uint16_t)var_r25);
	// stw r30,24(r31)
	PPC_STORE_U32(var_r31 + 24, var_r30);
	// lwz r5,64(r6)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // msgMsgSink::vfn_16 (unnamed)  // vtable slot 16 (byte +64)
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// stw r27,116(r30)
	PPC_STORE_U32(var_r30 + 116, var_r27);
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// divwu r10,r26,r11
	ctx.r10.u32 = ctx.r11.u32 ? var_r26 / ctx.r11.u32 : 0;
	// stw r28,108(r30)
	PPC_STORE_U32(var_r30 + 108, var_r28);
	// twllei r11,0
	if (ctx.r11.s32 == 0 || ctx.r11.u32 < 0u) __builtin_trap();
	// mullw r9,r10,r11
	ctx.r9.s64 = int64_t(ctx.r10.s32) * int64_t(ctx.r11.s32);
	// subf r8,r9,r26
	ctx.r8.s64 = (int64_t)(int32_t)var_r26 - ctx.r9.s64;
	// mr r4,r25
	ctx.r4.u64 = var_r25;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// subf r11,r8,r26
	ctx.r11.s64 = (int64_t)(int32_t)var_r26 - ctx.r8.s64;
	// subf r7,r11,r26
	ctx.r7.s64 = (int64_t)(int32_t)var_r26 - ctx.r11.s64;
	// stw r11,96(r30)
	PPC_STORE_U32(var_r30 + 96, ctx.r11.u32);
	// stw r24,100(r30)
	PPC_STORE_U32(var_r30 + 100, var_r24);
	// stw r7,124(r30)
	PPC_STORE_U32(var_r30 + 124, ctx.r7.u32);
	// lwz r11,56(r6)
	// bctrl
	msgMsgSink_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // msgMsgSink::vfn_14
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// cmpwi cr6,r28,0
	// bge cr6,0x82458a68
	if ((int32_t)var_r28 >= 0) goto loc_82458A68;
	// stw r24,108(r30)
	PPC_STORE_U32(var_r30 + 108, var_r24);
	// b 0x82458914
	goto loc_82458914;
loc_82458A40:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245bfa0
	atSingleton_BFA0_g(ctx, base);
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// ori r4,r4,32804
	ctx.r4.u64 = ctx.r4.u64 | 32804;
	// bl 0x820c02d0
	_locale_register(ctx, base);
	// lis r3,-32761
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	return;
loc_82458A68:
	// stw r22,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ var_r22);
	// lwz r10,8(r22)
	ctx.r10.u64 = PPC_LOAD_U32(var_r22 + 8);
	// stw r10,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r10.u32);
	// lwz r9,8(r22)
	ctx.r9.u64 = PPC_LOAD_U32(var_r22 + 8);
	// stw r31,4(r9)
	PPC_STORE_U32(ctx.r9.u32 + 4, var_r31);
	// stw r31,8(r22)
	PPC_STORE_U32(var_r22 + 8, var_r31);
	// b 0x82458934
	goto loc_82458934;
}

__attribute__((alias("__imp__msgMsgSink_vfn_121"))) PPC_WEAK_FUNC(msgMsgSink_vfn_121);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_121) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x824584f8
	atSingleton_84F8_h(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x82458ac8
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32805
		ctx.r4.u64 = ctx.r4.u64 | 32805;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_82458AC8:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_8AE0_w"))) PPC_WEAK_FUNC(msgMsgSink_8AE0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_8AE0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r11,2
	ctx.r11.s64 = 2;
	// li r10,1
	ctx.r10.s64 = 1;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,4
	ctx.r3.s64 = 4;
	// ori r4,r4,1
	ctx.r4.u64 = ctx.r4.u64 | 1;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// stw r11,40(r31)
	PPC_STORE_U32(var_r31 + 40, ctx.r11.u32);
	// stb r10,24(r31)
	PPC_STORE_U8(var_r31 + 24, ctx.r10.u8);
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// stw r3,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r3.u32);
	// cmplwi cr6,r3,0
	// li r28,0
	var_r28 = 0;
	// beq cr6,0x82458bd4
	if (ctx.r3.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// li r3,72
		ctx.r3.s64 = 72;
		// ori r4,r4,32781
		ctx.r4.u64 = ctx.r4.u64 | 32781;
		// bl 0x820c01b8
		rage_01B8(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x82458b60
		if (ctx.r3.u32 != 0) {
			// li r8,0
			ctx.r8.s64 = 0;
			// lwz r4,12(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 12);
			// li r7,0
			ctx.r7.s64 = 0;
			// mr r6,r30
			ctx.r6.u64 = var_r30;
			// mr r5,r31
			ctx.r5.u64 = var_r31;
			// bl 0x8245d3e8
			msgMsgSink_D3E8_v12(ctx, base);
			// b 0x82458b64
		} else {
		loc_82458B60:
			// mr r3,r28
			ctx.r3.u64 = var_r28;
		}
	loc_82458B64:
		// lwz r9,20(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
		// stw r3,0(r9)
		PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r3.u32);
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r8,0(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// cmplwi cr6,r8,0
		// beq cr6,0x82458bd4
		if (ctx.r8.u32 == 0) goto loc_82458BD4;
		// rotlwi r3,r8,0
		ctx.r3.u64 = ctx.r8.u32;
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// stw r3,36(r31)
		PPC_STORE_U32(var_r31 + 36, ctx.r3.u32);
		// lwz r6,8(r7)
		// bctrl
		VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x82458c1c
		if ((int32_t)var_r30 < 0) goto loc_82458C1C;
		// lwz r3,36(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
		// lwz r4,20(r5)
		// bctrl
		VCALL(ctx.r3.u32, 5, ctx, base);  // vtable slot 5 (byte +20)
		// cmpwi cr6,r3,0
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// beq cr6,0x82458c20
		if (ctx.r3.s32 == 0) {
			return;
		}
		// li r11,3
		ctx.r11.s64 = 3;
		// stw r11,40(r31)
		PPC_STORE_U32(var_r31 + 40, ctx.r11.u32);
		return;
	}
loc_82458BD4:
	// lis r30,-32761
	var_r30 = (uint32_t)(-2147024896);
	// stb r28,24(r31)
	PPC_STORE_U8(var_r31 + 24, (uint8_t)var_r28);
	// cmplwi cr6,r11,0
	// ori r30,r30,14
	var_r30 = (uint32_t)(var_r30 | 14);
	// beq cr6,0x82458c1c
	if (ctx.r11.u32 != 0) {
		// lwz r3,0(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// cmplwi cr6,r3,0
		// beq cr6,0x82458c08
		if (ctx.r3.u32 != 0) {
			// lwz r10,0(r3)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// lwz r9,0(r10)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
		}
	loc_82458C08:
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// ori r4,r4,1
		ctx.r4.u64 = ctx.r4.u64 | 1;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// stw r28,20(r31)
		PPC_STORE_U32(var_r31 + 20, var_r28);
	}
loc_82458C1C:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
loc_82458C20:
	return;
}

__attribute__((alias("__imp__RtlLeaveCriticalSection_8C28"))) PPC_WEAK_FUNC(RtlLeaveCriticalSection_8C28);
PPC_FUNC_IMPL(__imp__RtlLeaveCriticalSection_8C28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r29,0
	var_r29 = 0;
	// lwz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 12);
	// addi r3,r11,144
	ctx.r3.s64 = ctx.r11.s64 + 144;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x82458c9c
	if (ctx.r11.u32 != 0) {
		// li r30,0
		var_r30 = 0;
	loc_82458C58:
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// rlwinm r10,r30,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 2) & 0xFFFFFFFC;
		// lwzx r9,r10,r11
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
		// lbz r8,40(r9)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 40);
		// rlwinm r7,r8,0,26,26
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r7,32
		// bne cr6,0x82458c88
		if (ctx.r7.u32 == 32) {
			// lwz r6,16(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 16);
			// rotlwi r4,r9,0
			ctx.r4.u64 = ctx.r9.u32;
			// lwz r3,20(r6)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 20);
			// bl 0x82453c80
			RtlLeaveCriticalSection_3C80_2hr(ctx, base);
			// addi r29,r29,1
			var_r29 = (uint32_t)(var_r29 + 1);
		}
	loc_82458C88:
		// addi r5,r30,1
		ctx.r5.s64 = (int64_t)(int32_t)var_r30 + 1;
		// lbz r4,24(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 24);
		// clrlwi r30,r5,24
		var_r30 = (uint32_t)(ctx.r5.u32 & 0xFF);
		// cmplw cr6,r30,r4
		// blt cr6,0x82458c58
		if (var_r30 < ctx.r4.u32) goto loc_82458C58;
	}
loc_82458C9C:
	// addi r30,r31,48
	var_r30 = (uint32_t)(var_r31 + 48);
loc_82458CA0:
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// cmplw cr6,r3,r30
	// beq cr6,0x82458cd0
	if (ctx.r3.u32 == var_r30) {
		// lwz r11,12(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 12);
		// addi r3,r11,144
		ctx.r3.s64 = ctx.r11.s64 + 144;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// cmplw cr6,r11,r30
	// beq cr6,0x82458cd0
	if (ctx.r11.u32 == var_r30) {
		// lwz r11,12(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 12);
		// addi r3,r11,144
		ctx.r3.s64 = ctx.r11.s64 + 144;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
	// lwz r10,52(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r4,12(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r3,20(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
	// bl 0x82453c80
	RtlLeaveCriticalSection_3C80_2hr(ctx, base);
	// b 0x82458ca0
	goto loc_82458CA0;
}

__attribute__((alias("__imp__atSingleton_8CE8"))) PPC_WEAK_FUNC(atSingleton_8CE8);
PPC_FUNC_IMPL(__imp__atSingleton_8CE8) {
	PPC_FUNC_PROLOGUE();
	// lbz r11,44(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 44);
	// cmplwi cr6,r11,0
	// bnelr cr6
	if (ctx.r11.u32 != 0) return;
	// lbz r10,45(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 45);
	// cmplwi cr6,r10,0
	// bnelr cr6
	if (ctx.r10.u32 != 0) return;
	// addi r11,r3,48
	ctx.r11.s64 = ctx.r3.s64 + 48;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplw cr6,r9,r11
	// bnelr cr6
	if (ctx.r9.u32 != ctx.r11.u32) return;
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmplw cr6,r8,r11
	// bnelr cr6
	if (ctx.r8.u32 != ctx.r11.u32) return;
	// li r7,6
	ctx.r7.s64 = 6;
	// stw r7,40(r3)
	PPC_STORE_U32(ctx.r3.u32 + 40, ctx.r7.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_8D28_p39"))) PPC_WEAK_FUNC(msgMsgSink_8D28_p39);
PPC_FUNC_IMPL(__imp__msgMsgSink_8D28_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// cmplwi cr6,r3,0
	// beq cr6,0x82458d64
	if (ctx.r3.u32 != 0) {
		// lwz r10,52(r11)
		// bctrl
		VCALL(ctx.r3.u32, 13, ctx, base);  // vtable slot 13 (byte +52)
		// cmplw cr6,r3,r31
		// li r3,1
		ctx.r3.s64 = 1;
		// beq cr6,0x82458d68
		if (ctx.r3.u32 == var_r31) {
			// blr
			return;
		}
	}
loc_82458D64:
	// li r3,0
	ctx.r3.s64 = 0;
loc_82458D68:
	// blr
	return;
}

__attribute__((alias("__imp__game_8D80_1"))) PPC_WEAK_FUNC(game_8D80_1);
PPC_FUNC_IMPL(__imp__game_8D80_1) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r10,64(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 64);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_8DA0_sp"))) PPC_WEAK_FUNC(msgMsgSink_8DA0_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8DA0_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r10,68(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 68);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__atSingleton_8DC0"))) PPC_WEAK_FUNC(atSingleton_8DC0);
PPC_FUNC_IMPL(__imp__atSingleton_8DC0) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// lwz r10,72(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_8DE0_sp"))) PPC_WEAK_FUNC(msgMsgSink_8DE0_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8DE0_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r10,116(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 116);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_8E00_sp"))) PPC_WEAK_FUNC(msgMsgSink_8E00_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8E00_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r10,120(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 120);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_8E20_sp"))) PPC_WEAK_FUNC(msgMsgSink_8E20_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8E20_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r10,76(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 76);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_8E40_sp"))) PPC_WEAK_FUNC(msgMsgSink_8E40_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8E40_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// lwz r10,80(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__msgMsgSink_8E60_sp"))) PPC_WEAK_FUNC(msgMsgSink_8E60_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8E60_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beq cr6,0x82458e7c
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// lwz r10,44(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
		// mtctr r10
		ctx.ctr.u64 = ctx.r10.u64;
		// bctr
		PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
		return;
	}
loc_82458E7C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_8E88_sp"))) PPC_WEAK_FUNC(msgMsgSink_8E88_sp);
PPC_FUNC_IMPL(__imp__msgMsgSink_8E88_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,36(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmplwi cr6,r3,0
	// beq cr6,0x82458ea4
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
		// lwz r10,84(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 84);
		// mtctr r10
		ctx.ctr.u64 = ctx.r10.u64;
		// bctr
		PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
		return;
	}
loc_82458EA4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_8EB0_w"))) PPC_WEAK_FUNC(msgMsgSink_8EB0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_8EB0_w) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// stw r3,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r3.u32);
	// addi r10,r3,48
	ctx.r10.s64 = ctx.r3.s64 + 48;
	// stw r3,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r3.u32);
	// addi r9,r11,20632
	ctx.r9.s64 = ctx.r11.s64 + 20632;
	// stw r4,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r4.u32);
	// lis r11,-32256
	// stw r5,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r5.u32);
	// addi r8,r11,15816
	ctx.r8.s64 = ctx.r11.s64 + 15816;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r9,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r9.u32);
	// li r9,1
	ctx.r9.s64 = 1;
	// stw r11,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r11.u32);
	// stb r11,24(r3)
	PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r11.u8);
	// stw r11,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r11.u32);
	// stw r11,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r11.u32);
	// stw r9,40(r3)
	PPC_STORE_U32(ctx.r3.u32 + 40, ctx.r9.u32);
	// stb r11,44(r3)
	PPC_STORE_U8(ctx.r3.u32 + 44, ctx.r11.u8);
	// stb r11,45(r3)
	PPC_STORE_U8(ctx.r3.u32 + 45, ctx.r11.u8);
	// stb r11,32(r3)
	PPC_STORE_U8(ctx.r3.u32 + 32, ctx.r11.u8);
	// stw r8,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r8.u32);
	// stw r10,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r10.u32);
	// stw r10,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r10.u32);
	// stw r11,60(r3)
	PPC_STORE_U32(ctx.r3.u32 + 60, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__RtlLeaveCriticalSection_8F18_h"))) PPC_WEAK_FUNC(RtlLeaveCriticalSection_8F18_h);
PPC_FUNC_IMPL(__imp__RtlLeaveCriticalSection_8F18_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,20632
	ctx.r11.s64 = ctx.r11.s64 + 20632;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmplw cr6,r11,r31
	// beq cr6,0x82458f6c
	if (ctx.r11.u32 != var_r31) {
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r10,r31
		// beq cr6,0x82458f6c
		if (ctx.r10.u32 == var_r31) goto loc_82458F6C;
		// stw r10,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
		// lwz r11,8(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
		// stw r10,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
		// stw r31,4(r31)
		PPC_STORE_U32(var_r31 + 4, var_r31);
		// stw r31,8(r31)
		PPC_STORE_U32(var_r31 + 8, var_r31);
	}
loc_82458F6C:
	// lwz r9,20(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r9,0
	// beq cr6,0x82458fdc
	if (ctx.r9.u32 != 0) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82458c28
		RtlLeaveCriticalSection_8C28(ctx, base);
		// lbz r8,24(r31)
		ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 24);
		// cmplwi cr6,r8,0
		// beq cr6,0x82458fcc
		if (ctx.r8.u32 != 0) {
			// li r30,0
			var_r30 = 0;
		loc_82458F90:
			// lwz r7,20(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 20);
			// rlwinm r6,r30,2,0,29
			ctx.r6.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 2) & 0xFFFFFFFC;
			// lwzx r3,r6,r7
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + ctx.r7.u32);
			// cmplwi cr6,r3,0
			// beq cr6,0x82458fb8
			if (ctx.r3.u32 != 0) {
				// lwz r5,0(r3)
  // [ph4a] vtable load collapsed
				// li r4,1
				ctx.r4.s64 = 1;
				// lwz r11,0(r5)
  // [ph4a] slot load collapsed
				// bctrl
				VCALL(ctx.r3.u32, 0, ctx, base);  // pattern-B slot 0 (byte +0)
			}
		loc_82458FB8:
			// addi r10,r30,1
			ctx.r10.s64 = (int64_t)(int32_t)var_r30 + 1;
			// lbz r9,24(r31)
			ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 24);
			// clrlwi r30,r10,24
			var_r30 = (uint32_t)(ctx.r10.u32 & 0xFF);
			// cmplw cr6,r30,r9
			// blt cr6,0x82458f90
			if (var_r30 < ctx.r9.u32) goto loc_82458F90;
		}
	loc_82458FCC:
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// ori r4,r4,1
		ctx.r4.u64 = ctx.r4.u64 | 1;
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_82458FDC:
	// addi r30,r31,48
	var_r30 = (uint32_t)(var_r31 + 48);
loc_82458FE0:
	// lwz r8,4(r30)
	ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 4);
	// cmplw cr6,r8,r30
	// beq cr6,0x8245901c
	if (ctx.r8.u32 == var_r30) {
		// lis r11,-32256
		// addi r11,r11,15816
		ctx.r11.s64 = ctx.r11.s64 + 15816;
		// stw r11,0(r30)
		PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
		// blr
		return;
	}
	// lwz r7,8(r30)
	ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 8);
	// cmplw cr6,r7,r30
	// beq cr6,0x8245901c
	if (ctx.r7.u32 == var_r30) {
		// lis r11,-32256
		// addi r11,r11,15816
		ctx.r11.s64 = ctx.r11.s64 + 15816;
		// stw r11,0(r30)
		PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
		// blr
		return;
	}
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplwi cr6,r3,0
	// beq cr6,0x82458fe0
	if (ctx.r3.u32 == 0) goto loc_82458FE0;
	// lwz r6,0(r3)
  // [ph4a] vtable load collapsed
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r5,0(r6)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 0, ctx, base);  // pattern-B slot 0 (byte +0)
	// b 0x82458fe0
	goto loc_82458FE0;
}

__attribute__((alias("__imp__atSingleton_9048_g"))) PPC_WEAK_FUNC(atSingleton_9048_g);
PPC_FUNC_IMPL(__imp__atSingleton_9048_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmpwi cr6,r11,3
	// bne cr6,0x824590e4
	if (ctx.r11.s32 == 3) {
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// lbz r11,24(r31)
		ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 24);
		// li r10,4
		ctx.r10.s64 = 4;
		// stw r3,28(r31)
		PPC_STORE_U32(var_r31 + 28, ctx.r3.u32);
		// cmplwi cr6,r11,0
		// stb r11,44(r31)
		PPC_STORE_U8(var_r31 + 44, ctx.r11.u8);
		// stw r10,40(r31)
		PPC_STORE_U32(var_r31 + 40, ctx.r10.u32);
		// beq cr6,0x824590f0
		if (ctx.r11.u32 == 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			// blr
			return;
		}
		// li r30,0
		var_r30 = 0;
	loc_82459090:
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// rlwinm r10,r30,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 2) & 0xFFFFFFFC;
		// lwzx r8,r10,r11
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
		// lwz r7,16(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		// lwz r6,0(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// rlwinm r5,r6,0,2,2
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0x20000000;
		// cmplwi cr6,r5,0
		// beq cr6,0x824590bc
		if (ctx.r5.u32 != 0) {
			// rotlwi r3,r8,0
			ctx.r3.u64 = ctx.r8.u32;
			// bl 0x824528a8
			atSingleton_28A8_g_28A8_1(ctx, base);
			// b 0x824590cc
		} else {
		loc_824590BC:
			// lwz r3,16(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
			// lwzx r4,r10,r11
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
			// lwz r3,20(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
			// bl 0x82454698
			game_4698_h(ctx, base);
		}
	loc_824590CC:
		// addi r10,r30,1
		ctx.r10.s64 = (int64_t)(int32_t)var_r30 + 1;
		// lbz r9,24(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 24);
		// clrlwi r30,r10,24
		var_r30 = (uint32_t)(ctx.r10.u32 & 0xFF);
		// cmplw cr6,r30,r9
		// blt cr6,0x82459090
		if (var_r30 < ctx.r9.u32) goto loc_82459090;
		// b 0x824590f0
	} else {
	loc_824590E4:
		// lbz r8,32(r31)
		ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 32);
		// ori r7,r8,64
		ctx.r7.u64 = ctx.r8.u64 | 64;
		// stb r7,32(r31)
		PPC_STORE_U8(var_r31 + 32, ctx.r7.u8);
	}
loc_824590F0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_9110_w"))) PPC_WEAK_FUNC(msgMsgSink_9110_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_9110_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r18 = 0;
	uint32_t var_r19 = 0;
	uint32_t var_r20 = 0;
	uint32_t var_r21 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=240, savegprlr_18
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// addi r11,r1,100
	ctx.r11.s64 = ctx.r1.s64 + 100;
	// li r27,0
	var_r27 = 0;
	// li r8,2
	ctx.r8.s64 = 2;
	// lwz r9,16(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 16);
	// mr r26,r5
	var_r26 = ctx.r5.u32;
	// mr r24,r27
	var_r24 = (uint32_t)(var_r27);
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// lwz r7,20(r9)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
	// lwz r6,48(r7)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 48);
	// lwz r25,284(r6)
	var_r25 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 284));
	// stw r10,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r10.u32);
	// stw r27,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r27);
	// mr r3,r25
	ctx.r3.u64 = var_r25;
	// stw r27,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r27);
	// stw r8,40(r30)
	PPC_STORE_U32(var_r30 + 40, ctx.r8.u32);
	// lwz r11,24(r5)
	// bctrl
	VCALL(ctx.r3.u32, 6, ctx, base);  // vtable slot 6 (byte +24)
	// lbz r11,104(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 104);
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// ori r4,r4,1
	ctx.r4.u64 = ctx.r4.u64 | 1;
	// rotlwi r3,r11,2
	ctx.r3.u64 = __builtin_rotateleft32(ctx.r11.u32, 2);
	// stb r11,24(r30)
	PPC_STORE_U8(var_r30 + 24, ctx.r11.u8);
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// lis r10,8324
	ctx.r10.s64 = 545521664;
	// lis r9,8324
	ctx.r9.s64 = 545521664;
	// stw r3,20(r30)
	PPC_STORE_U32(var_r30 + 20, ctx.r3.u32);
	// lis r8,8324
	ctx.r8.s64 = 545521664;
	// lis r7,8324
	ctx.r7.s64 = 545521664;
	// lis r6,8324
	ctx.r6.s64 = 545521664;
	// lis r5,8324
	ctx.r5.s64 = 545521664;
	// ori r18,r10,32781
	var_r18 = (uint32_t)(ctx.r10.u64 | 32781);
	// ori r19,r9,32778
	var_r19 = (uint32_t)(ctx.r9.u64 | 32778);
	// ori r20,r8,32785
	var_r20 = (uint32_t)(ctx.r8.u64 | 32785);
	// ori r21,r7,32784
	var_r21 = (uint32_t)(ctx.r7.u64 | 32784);
	// ori r22,r6,32786
	var_r22 = (uint32_t)(ctx.r6.u64 | 32786);
	// ori r23,r5,32779
	var_r23 = (uint32_t)(ctx.r5.u64 | 32779);
loc_824591C0:
	// lwz r11,0(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 0);
	// mr r31,r27
	var_r31 = (uint32_t)(var_r27);
	// rlwinm r4,r11,0,2,2
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20000000;
	// cmplwi cr6,r4,0
	// beq cr6,0x824591dc
	if (ctx.r4.u32 != 0) {
		// mr r29,r27
		var_r29 = (uint32_t)(var_r27);
		// b 0x824591ec
	} else {
	loc_824591DC:
		// lbz r3,6(r28)
		ctx.r3.u64 = PPC_LOAD_U8(var_r28 + 6);
		// lwz r10,20(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 20);
		// rotlwi r9,r3,2
		ctx.r9.u64 = __builtin_rotateleft32(ctx.r3.u32, 2);
		// lwzx r29,r9,r10
		var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r9.u32 + ctx.r10.u32));
	}
loc_824591EC:
	// clrlwi r11,r11,27
	ctx.r11.u64 = ctx.r11.u32 & 0x1F;
	// cmplwi cr6,r11,20
	// bgt cr6,0x82459298
	if (ctx.r11.u32 > 20) goto loc_82459298;
	// lis r12,-32186
	// addi r12,r12,-28144
	ctx.r12.s64 = ctx.r12.s64 + -28144;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (ctx.r11.u64) {
	case 0:
		goto loc_82459264;
	case 1:
		goto loc_82459314;
	case 2:
		goto loc_82459354;
	case 3:
		goto loc_82459370;
	case 4:
		goto loc_82459314;
	case 5:
		goto loc_82459354;
	case 6:
		goto loc_82459370;
	case 7:
		goto loc_82459490;
	case 8:
		goto loc_824594C0;
	case 9:
		goto loc_824594F0;
	case 10:
		goto loc_824593D0;
	case 11:
		goto loc_82459400;
	case 12:
		goto loc_82459430;
	case 13:
		goto loc_82459460;
	case 14:
		goto loc_8245952C;
	case 15:
		goto loc_8245955C;
	case 16:
		goto loc_8245958C;
	case 17:
		goto loc_824595BC;
	case 18:
		goto loc_824595EC;
	case 19:
		goto loc_824593A0;
	case 20:
		goto loc_82459298;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82459264:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,48
	ctx.r3.s64 = 48;
	// ori r4,r4,32787
	ctx.r4.u64 = ctx.r4.u64 | 32787;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245f828
	jumptable_F828_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_82459294:
	// mr r31,r27
	var_r31 = (uint32_t)(var_r27);
loc_82459298:
	// lis r29,-32761
	var_r29 = (uint32_t)(-2147024896);
	// ori r29,r29,14
	var_r29 = (uint32_t)(var_r29 | 14);
loc_824592A0:
	// cmplwi cr6,r31,0
	// beq cr6,0x824592c0
	if (var_r31 == 0) goto loc_824592C0;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
loc_824592C0:
	// cmpwi cr6,r29,0
	// blt cr6,0x824596e8
	if ((int32_t)var_r29 < 0) goto loc_824596E8;
loc_824592C8:
	// cmplwi cr6,r28,0
	// bne cr6,0x824591c0
	if (var_r28 != 0) goto loc_824591C0;
	// cmpwi cr6,r29,0
	// blt cr6,0x824596e8
	if ((int32_t)var_r29 < 0) goto loc_824596E8;
	// lwz r3,36(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 36);
	// stb r24,24(r30)
	PPC_STORE_U8(var_r30 + 24, (uint8_t)var_r24);
	// cmplwi cr6,r3,0
	// beq cr6,0x824596d4
	if (ctx.r3.u32 == 0) {
		// li r5,3
		ctx.r5.s64 = 3;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// stw r5,40(r30)
		PPC_STORE_U32(var_r30 + 40, ctx.r5.u32);
		return;
	}
	// lwz r7,20(r8)
	// bctrl
	VCALL(ctx.r3.u32, 5, ctx, base);  // vtable slot 5 (byte +20)
	// cmpwi cr6,r3,0
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// beq cr6,0x82459754
	if (ctx.r3.s32 == 0) {
		return;
	}
	// li r6,3
	ctx.r6.s64 = 3;
	// stw r6,40(r30)
	PPC_STORE_U32(var_r30 + 40, ctx.r6.u32);
	return;
loc_82459314:
	// mr r4,r18
	ctx.r4.u64 = var_r18;
	// li r3,72
	ctx.r3.s64 = 72;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r8,0
	ctx.r8.s64 = 0;
loc_8245932C:
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245d3e8
	msgMsgSink_D3E8_v12(ctx, base);
loc_82459340:
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x82459298
	if (var_r31 == 0) goto loc_82459298;
	// stw r31,36(r30)
	PPC_STORE_U32(var_r30 + 36, var_r31);
	// b 0x82459628
	goto loc_82459628;
loc_82459354:
	// mr r4,r18
	ctx.r4.u64 = var_r18;
	// li r3,72
	ctx.r3.s64 = 72;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// lwz r8,11(r28)
	ctx.r8.u64 = PPC_LOAD_U32(var_r28 + 11);
	// b 0x8245932c
	goto loc_8245932C;
loc_82459370:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,188
	ctx.r3.s64 = 188;
	// ori r4,r4,32782
	ctx.r4.u64 = ctx.r4.u64 | 32782;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245f420
	msgMsgSink_F420_w(ctx, base);
	// b 0x82459340
	goto loc_82459340;
loc_824593A0:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,56
	ctx.r3.s64 = 56;
	// ori r4,r4,32780
	ctx.r4.u64 = ctx.r4.u64 | 32780;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245ecd0
	jumptable_ECD0_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_824593D0:
	// mr r4,r19
	ctx.r4.u64 = var_r19;
	// li r3,56
	ctx.r3.s64 = 56;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245a698
	jumptable_A698_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_82459400:
	// mr r4,r19
	ctx.r4.u64 = var_r19;
	// li r3,56
	ctx.r3.s64 = 56;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r8,1
	ctx.r8.s64 = 1;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245a698
	jumptable_A698_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_82459430:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,52
	ctx.r3.s64 = 52;
	// ori r4,r4,32788
	ctx.r4.u64 = ctx.r4.u64 | 32788;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245ebe8
	jumptable_EBE8_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_82459460:
	// mr r4,r20
	ctx.r4.u64 = var_r20;
	// li r3,64
	ctx.r3.s64 = 64;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245e800
	jumptable_E800_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_82459490:
	// mr r4,r21
	ctx.r4.u64 = var_r21;
	// li r3,76
	ctx.r3.s64 = 76;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245e290
	jumptable_E290_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_824594C0:
	// mr r4,r22
	ctx.r4.u64 = var_r22;
	// li r3,76
	ctx.r3.s64 = 76;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245dd88
	jumptable_DD88_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_824594F0:
	// mr r4,r23
	ctx.r4.u64 = var_r23;
	// li r3,72
	ctx.r3.s64 = 72;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r27,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, var_r27);
	// mr r8,r29
	ctx.r8.u64 = var_r29;
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245dae8
	msgMsgSink_DAE8_v12(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_8245952C:
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// li r3,48
	ctx.r3.s64 = 48;
	// ori r4,r4,32783
	ctx.r4.u64 = ctx.r4.u64 | 32783;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245da40
	jumptable_DA40_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_8245955C:
	// mr r4,r20
	ctx.r4.u64 = var_r20;
	// li r3,64
	ctx.r3.s64 = 64;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// addi r8,r28,18
	ctx.r8.s64 = (int64_t)(int32_t)var_r28 + 18;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245e800
	jumptable_E800_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_8245958C:
	// mr r4,r21
	ctx.r4.u64 = var_r21;
	// li r3,76
	ctx.r3.s64 = 76;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// addi r8,r28,22
	ctx.r8.s64 = (int64_t)(int32_t)var_r28 + 22;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245e290
	jumptable_E290_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_824595BC:
	// mr r4,r22
	ctx.r4.u64 = var_r22;
	// li r3,76
	ctx.r3.s64 = 76;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// addi r8,r28,22
	ctx.r8.s64 = (int64_t)(int32_t)var_r28 + 22;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// mr r7,r29
	ctx.r7.u64 = var_r29;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245dd88
	jumptable_DD88_h(ctx, base);
	// b 0x82459624
	goto loc_82459624;
loc_824595EC:
	// mr r4,r23
	ctx.r4.u64 = var_r23;
	// li r3,72
	ctx.r3.s64 = 72;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459294
	if (ctx.r3.u32 == 0) goto loc_82459294;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// addi r9,r28,11
	ctx.r9.s64 = (int64_t)(int32_t)var_r28 + 11;
	// stw r27,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, var_r27);
	// mr r8,r29
	ctx.r8.u64 = var_r29;
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// bl 0x8245dae8
	msgMsgSink_DAE8_v12(ctx, base);
loc_82459624:
	// mr r31,r3
	var_r31 = ctx.r3.u32;
loc_82459628:
	// cmplwi cr6,r31,0
	// beq cr6,0x82459298
	if (var_r31 == 0) goto loc_82459298;
	// lwz r8,16(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r29,r27
	var_r29 = (uint32_t)(var_r27);
	// lwz r11,0(r8)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r8.u32 + 0);
	// clrlwi r7,r11,27
	ctx.r7.u64 = ctx.r11.u32 & 0x1F;
	// cmpwi cr6,r7,9
	// bne cr6,0x82459678
	if (ctx.r7.s32 != 9) goto loc_82459678;
	// lwz r10,20(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 20);
	// rlwinm r11,r11,27,8,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0xFFFFFF;
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// cmpw cr6,r6,r26
	// bge cr6,0x82459678
	if (ctx.r6.s32 >= (int32_t)var_r26) goto loc_82459678;
	// lwz r5,0(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,0(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// b 0x824596b4
	goto loc_824596B4;
loc_82459678:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
	// mr r4,r26
	ctx.r4.u64 = var_r26;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// cmpwi cr6,r29,0
	// blt cr6,0x824592a0
	if ((int32_t)var_r29 < 0) goto loc_824592A0;
	// clrlwi r11,r24,24
	ctx.r11.u64 = var_r24 & 0xFF;
	// lwz r8,20(r30)
	ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 20);
	// rlwinm r7,r11,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r6,r11,1
	ctx.r6.s64 = ctx.r11.s64 + 1;
	// clrlwi r24,r6,24
	var_r24 = (uint32_t)(ctx.r6.u32 & 0xFF);
	// stwx r31,r7,r8
	PPC_STORE_U32(ctx.r7.u32 + ctx.r8.u32, var_r31);
loc_824596B4:
	// lwz r5,0(r25)
  // [ph4a] vtable load collapsed
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// mr r3,r25
	ctx.r3.u64 = var_r25;
	// lwz r11,28(r5)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r25, 7, ctx, base);  // pattern-B slot 7 (byte +28)
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// b 0x824592c8
	goto loc_824592C8;
loc_824596E8:
	// clrlwi r31,r24,24
	var_r31 = (uint32_t)(var_r24 & 0xFF);
	// cmplwi cr6,r31,0
	// beq cr6,0x8245972c
	if (var_r31 == 0) goto loc_8245972C;
loc_824596F4:
	// addi r4,r31,255
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 255;
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// clrlwi r31,r4,24
	var_r31 = (uint32_t)(ctx.r4.u32 & 0xFF);
	// rlwinm r11,r31,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 2) & 0xFFFFFFFC;
	// lwzx r3,r11,r3
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r3.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459724
	if (ctx.r3.u32 == 0) goto loc_82459724;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* msgMsgSink::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
loc_82459724:
	// cmplwi cr6,r31,0
	// bne cr6,0x824596f4
	if (var_r31 != 0) goto loc_824596F4;
loc_8245972C:
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// stb r27,24(r30)
	PPC_STORE_U8(var_r30 + 24, (uint8_t)var_r27);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245974c
	if (ctx.r3.u32 == 0) goto loc_8245974C;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// ori r4,r4,1
	ctx.r4.u64 = ctx.r4.u64 | 1;
	// bl 0x820c02d0
	_locale_register(ctx, base);
	// stw r27,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r27);
loc_8245974C:
	// stw r27,36(r30)
	PPC_STORE_U32(var_r30 + 36, var_r27);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
loc_82459754:
	return;
}

__attribute__((alias("__imp__game_9760"))) PPC_WEAK_FUNC(game_9760);
PPC_FUNC_IMPL(__imp__game_9760) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r29,0
	var_r29 = 0;
	// lwz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmpwi cr6,r11,6
	// beq cr6,0x82459824
	if (ctx.r11.s32 != 6) {
		// cmpwi cr6,r11,5
		// bne cr6,0x824597a0
		if (ctx.r11.s32 == 5) {
			// lwz r11,60(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 60);
			// cmpwi cr6,r11,0
			// bne cr6,0x82459824
			if (ctx.r11.s32 != 0) goto loc_82459824;
			// cmpwi cr6,r30,0
			// beq cr6,0x82459824
			if ((int32_t)var_r30 == 0) goto loc_82459824;
		}
	loc_824597A0:
		// li r10,5
		ctx.r10.s64 = 5;
		// cmpwi cr6,r30,0
		// stw r10,40(r31)
		PPC_STORE_U32(var_r31 + 40, ctx.r10.u32);
		// beq cr6,0x824597c4
		if ((int32_t)var_r30 != 0) {
			// li r9,1
			ctx.r9.s64 = 1;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r9,60(r31)
			PPC_STORE_U32(var_r31 + 60, ctx.r9.u32);
			// bl 0x82458c28
			RtlLeaveCriticalSection_8C28(ctx, base);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
		}
	loc_824597C4:
		// lwz r3,36(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
		// cmplwi cr6,r3,0
		// beq cr6,0x82459800
		if (ctx.r3.u32 != 0) {
			// lwz r7,24(r8)
			// bctrl
			VCALL(ctx.r3.u32, 6, ctx, base);  // vtable slot 6 (byte +24)
			// cmpwi cr6,r3,0
			// beq cr6,0x82459800
			if (ctx.r3.s32 == 0) goto loc_82459800;
			// lwz r3,36(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// lwz r5,60(r6)
			// bctrl
			VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
		}
	loc_82459800:
		// cmpwi cr6,r30,0
		// beq cr6,0x82459824
		if ((int32_t)var_r30 == 0) goto loc_82459824;
		// cmpwi cr6,r29,0
		// li r3,0
		ctx.r3.s64 = 0;
		// bne cr6,0x82459828
		if ((int32_t)var_r29 != 0) {
			return;
		}
		// li r4,6
		ctx.r4.s64 = 6;
		// stw r4,40(r31)
		PPC_STORE_U32(var_r31 + 40, ctx.r4.u32);
		return;
	}
loc_82459824:
	// li r3,0
	ctx.r3.s64 = 0;
loc_82459828:
	return;
}

__attribute__((alias("__imp__game_9830_h"))) PPC_WEAK_FUNC(game_9830_h);
PPC_FUNC_IMPL(__imp__game_9830_h) {
	PPC_FUNC_PROLOGUE();
	// addi r11,r3,48
	ctx.r11.s64 = ctx.r3.s64 + 48;
	// stw r11,4(r4)
	PPC_STORE_U32(ctx.r4.u32 + 4, ctx.r11.u32);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r10,8(r4)
	PPC_STORE_U32(ctx.r4.u32 + 8, ctx.r10.u32);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r4,4(r9)
	PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r4.u32);
	// stw r4,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r4.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_9850"))) PPC_WEAK_FUNC(atSingleton_9850);
PPC_FUNC_IMPL(__imp__atSingleton_9850) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,52(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 52);
	// addi r10,r3,48
	ctx.r10.s64 = ctx.r3.s64 + 48;
	// cmplw cr6,r11,r10
	// beqlr cr6
	if (ctx.r11.u32 == ctx.r10.u32) return;
loc_82459860:
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmplw cr6,r9,r4
	// beq cr6,0x8245987c
	if (ctx.r9.u32 == ctx.r4.u32) {
		// lwz r8,8(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// lwz r7,4(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r8,8(r7)
		PPC_STORE_U32(ctx.r7.u32 + 8, ctx.r8.u32);
		// lwz r6,8(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// lwz r5,4(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r5,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r5.u32);
		// stw r11,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
		// stw r11,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
		// blr
		return;
	}
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplw cr6,r11,r10
	// bne cr6,0x82459860
	if (ctx.r11.u32 != ctx.r10.u32) goto loc_82459860;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_vfn_122"))) PPC_WEAK_FUNC(msgMsgSink_vfn_122);
PPC_FUNC_IMPL(__imp__msgMsgSink_vfn_122) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x82458f18
	RtlLeaveCriticalSection_8F18_h(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x824598e0
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32776
		ctx.r4.u64 = ctx.r4.u64 | 32776;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_824598E0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_98F8_fw"))) PPC_WEAK_FUNC(atSingleton_98F8_fw);
PPC_FUNC_IMPL(__imp__atSingleton_98F8_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmpwi cr6,r11,2
	// bne cr6,0x82459944
	if (ctx.r11.s32 == 2) {
		// lwz r3,36(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
		// cmplwi cr6,r3,0
		// beq cr6,0x82459944
		if (ctx.r3.u32 == 0) goto loc_82459944;
		// lwz r9,20(r10)
		// bctrl
		VCALL(ctx.r3.u32, 5, ctx, base);  // vtable slot 5 (byte +20)
		// cmpwi cr6,r3,0
		// beq cr6,0x82459944
		if (ctx.r3.s32 == 0) goto loc_82459944;
		// li r8,3
		ctx.r8.s64 = 3;
		// stw r8,40(r31)
		PPC_STORE_U32(var_r31 + 40, ctx.r8.u32);
	}
loc_82459944:
	// lbz r11,32(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 32);
	// rlwinm r6,r11,0,25,25
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x40;
	// cmplwi cr6,r6,64
	// bne cr6,0x82459970
	if (ctx.r6.u32 == 64) {
		// lwz r5,40(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 40);
		// cmpwi cr6,r5,3
		// bne cr6,0x82459970
		if (ctx.r5.s32 != 3) {
			// blr
			return;
		}
		// andi. r4,r11,191
		ctx.r4.u64 = ctx.r11.u64 & 191;
		ctx.cr0.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r4,32(r31)
		PPC_STORE_U8(var_r31 + 32, ctx.r4.u8);
		// bl 0x82459048
		atSingleton_9048_g(ctx, base);
	}
loc_82459970:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_9988_wrh"))) PPC_WEAK_FUNC(atSingleton_9988_wrh);
PPC_FUNC_IMPL(__imp__atSingleton_9988_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// addi r11,r11,21664
	ctx.r11.s64 = ctx.r11.s64 + 21664;
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x824599c8
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r10,0(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	}
loc_824599C8:
	// lwz r3,24(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r3,0
	// beq cr6,0x824599e8
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r8,0(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	}
loc_824599E8:
	// lwz r11,52(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplwi cr6,r11,0
	// beq cr6,0x82459a24
	if (ctx.r11.u32 != 0) {
		// addi r3,r11,-4
		ctx.r3.s64 = ctx.r11.s64 + -4;
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// cmpwi cr6,r7,0
		// beq cr6,0x82459a20
		if (ctx.r7.s32 != 0) {
			// addi r3,r11,4
			ctx.r3.s64 = ctx.r11.s64 + 4;
			// li r4,3
			ctx.r4.s64 = 3;
			// lwz r5,0(r6)
			// bctrl
			DTOR(ctx.r3.u32, ctx, base);  // vtable slot 0 (destructor)
			// b 0x82459a24
		} else {
		loc_82459A20:
			// bl 0x820c0120
			thunk_rage_free(ctx, base);
		}
	}
loc_82459A24:
	// lis r11,-32256
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__game_9A48_h"))) PPC_WEAK_FUNC(game_9A48_h);
PPC_FUNC_IMPL(__imp__game_9A48_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// b 0x82459048
	atSingleton_9048_g(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_9A50_g"))) PPC_WEAK_FUNC(atSingleton_9A50_g);
PPC_FUNC_IMPL(__imp__atSingleton_9A50_g) {
	PPC_FUNC_PROLOGUE();
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f30,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lfs f31,40(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	var_f31 = double(temp.f32);
	// lwz r3,20(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f30,56(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	var_f30 = double(temp.f32);
	// bl 0x82454ac0
	atSingleton_4AC0_g(ctx, base);
	// fmuls f0,f30,f31
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(var_f30 * var_f31));
	// fmuls f1,f0,f1
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f1.f64));
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_9A98_g"))) PPC_WEAK_FUNC(atSingleton_9A98_g);
PPC_FUNC_IMPL(__imp__atSingleton_9A98_g) {
	PPC_FUNC_PROLOGUE();
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f30,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lfs f31,44(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	var_f31 = double(temp.f32);
	// lwz r3,20(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f30,60(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	var_f30 = double(temp.f32);
	// bl 0x82454148
	atSingleton_4148_g(ctx, base);
	// fadds f0,f30,f31
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(var_f30 + var_f31));
	// fadds f1,f0,f1
	ctx.f1.f64 = double(float(ctx.f0.f64 + ctx.f1.f64));
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_9AE0_w"))) PPC_WEAK_FUNC(msgMsgSink_9AE0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_9AE0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// ori r4,r4,32776
	ctx.r4.u64 = ctx.r4.u64 | 32776;
	// li r3,64
	ctx.r3.s64 = 64;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r29,r6
	var_r29 = ctx.r6.u32;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459b64
	if (ctx.r3.u32 != 0) {
		// li r6,-1
		// lwz r4,12(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 12);
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// bl 0x82458eb0
		msgMsgSink_8EB0_w(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// bne cr6,0x82459b6c
		if (var_r31 != 0) goto loc_82459B6C;
	loc_82459B30:
		// lis r30,-32761
		var_r30 = (uint32_t)(-2147024896);
		// ori r30,r30,14
		var_r30 = (uint32_t)(var_r30 | 14);
	loc_82459B38:
		// cmplwi cr6,r31,0
		// beq cr6,0x82459b58
		if (var_r31 != 0) {
			// lwz r11,0(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* msgMsgSink::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lwz r10,0(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		}
	loc_82459B58:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		return;
	}
loc_82459B64:
	// li r31,0
	var_r31 = 0;
	// b 0x82459b30
	goto loc_82459B30;
loc_82459B6C:
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82459110
	msgMsgSink_9110_w(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x82459b38
	if ((int32_t)var_r30 < 0) goto loc_82459B38;
	// stw r31,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r31);
	return;
}

__attribute__((alias("__imp__atSingleton_9B98_p39"))) PPC_WEAK_FUNC(atSingleton_9B98_p39);
PPC_FUNC_IMPL(__imp__atSingleton_9B98_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lwz r3,24(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 24);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459bd4
	if (ctx.r3.u32 != 0) {
		// bl 0x82458d28
		msgMsgSink_8D28_p39(ctx, base);
		// cmpwi cr6,r3,0
		// beq cr6,0x82459bd4
		if (ctx.r3.s32 == 0) goto loc_82459BD4;
		// li r3,1
		ctx.r3.s64 = 1;
		// b 0x82459bf8
	} else {
	loc_82459BD4:
		// lwz r3,28(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 28);
		// cmplwi cr6,r3,0
		// beq cr6,0x82459bf4
		if (ctx.r3.u32 != 0) {
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// bl 0x82458d28
			msgMsgSink_8D28_p39(ctx, base);
			// cmpwi cr6,r3,0
			// li r3,1
			ctx.r3.s64 = 1;
			// bne cr6,0x82459bf8
			if (ctx.r3.s32 != 0) {
				// blr
				return;
			}
		}
	loc_82459BF4:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_82459BF8:
	// blr
	return;
}

__attribute__((alias("__imp__game_9C10_h"))) PPC_WEAK_FUNC(game_9C10_h);
PPC_FUNC_IMPL(__imp__game_9C10_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x82459c58
	if (ctx.r11.u32 != 0) {
		// lfs f31,40(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 40);
		var_f31 = double(temp.f32);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lfs f30,56(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 56);
		var_f30 = double(temp.f32);
		// bl 0x82454ac0
		atSingleton_4AC0_g(ctx, base);
		// fmuls f0,f31,f1
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = double(float(var_f31 * ctx.f1.f64));
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// fmuls f1,f0,f30
		ctx.f1.f64 = double(float(ctx.f0.f64 * var_f30));
		// bl 0x82458d80
		game_8D80_1(ctx, base);
	}
loc_82459C58:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__phBoundCapsule_9C78_2hr"))) PPC_WEAK_FUNC(phBoundCapsule_9C78_2hr);
PPC_FUNC_IMPL(__imp__phBoundCapsule_9C78_2hr) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// b 0x82458da0
	msgMsgSink_8DA0_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_9C90_h"))) PPC_WEAK_FUNC(atSingleton_9C90_h);
PPC_FUNC_IMPL(__imp__atSingleton_9C90_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x82459cd8
	if (ctx.r11.u32 != 0) {
		// lfs f31,44(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 44);
		var_f31 = double(temp.f32);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lfs f30,60(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 60);
		var_f30 = double(temp.f32);
		// bl 0x82454148
		atSingleton_4148_g(ctx, base);
		// fadds f0,f31,f1
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = double(float(var_f31 + ctx.f1.f64));
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// fadds f1,f0,f30
		ctx.f1.f64 = double(float(ctx.f0.f64 + var_f30));
		// bl 0x82458dc0
		atSingleton_8DC0(ctx, base);
	}
loc_82459CD8:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__game_9CF8_h"))) PPC_WEAK_FUNC(game_9CF8_h);
PPC_FUNC_IMPL(__imp__game_9CF8_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// b 0x82458de0
	msgMsgSink_8DE0_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__game_9D10_h"))) PPC_WEAK_FUNC(game_9D10_h);
PPC_FUNC_IMPL(__imp__game_9D10_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// b 0x82458e00
	msgMsgSink_8E00_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__game_9D28_h"))) PPC_WEAK_FUNC(game_9D28_h);
PPC_FUNC_IMPL(__imp__game_9D28_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// b 0x82458e20
	msgMsgSink_8E20_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__game_9D40_h"))) PPC_WEAK_FUNC(game_9D40_h);
PPC_FUNC_IMPL(__imp__game_9D40_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// b 0x82458e40
	msgMsgSink_8E40_sp(ctx, base);
	return;
}

__attribute__((alias("__imp__game_9D58_h"))) PPC_WEAK_FUNC(game_9D58_h);
PPC_FUNC_IMPL(__imp__game_9D58_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459d68
	if (ctx.r3.u32 != 0) {
		// b 0x82458e60
		msgMsgSink_8E60_sp(ctx, base);
		return;
	}
loc_82459D68:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__game_9D70_h"))) PPC_WEAK_FUNC(game_9D70_h);
PPC_FUNC_IMPL(__imp__game_9D70_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,24(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplwi cr6,r3,0
	// beq cr6,0x82459d80
	if (ctx.r3.u32 != 0) {
		// b 0x82458e88
		msgMsgSink_8E88_sp(ctx, base);
		return;
	}
loc_82459D80:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__game_9D88"))) PPC_WEAK_FUNC(game_9D88);
PPC_FUNC_IMPL(__imp__game_9D88) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r10,0,29,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r9,4
	// bne cr6,0x82459e0c
	if (ctx.r9.u32 == 4) {
		// lbz r4,48(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 48);
		// bl 0x82454838
		rage_4838(ctx, base);
		// clrlwi r8,r3,24
		ctx.r8.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// beq cr6,0x82459e0c
		if (ctx.r8.u32 == 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			// blr
			return;
		}
		// lwz r9,52(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 52);
		// li r11,0
		ctx.r11.s64 = 0;
	loc_82459DD0:
		// rlwinm r10,r11,1,0,30
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// add r7,r11,r10
		ctx.r7.u64 = ctx.r11.u64 + ctx.r10.u64;
		// rlwinm r10,r7,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 4) & 0xFFFFFFF0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// lwz r10,20(r10)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
		// lhz r6,0(r10)
		ctx.r6.u64 = PPC_LOAD_U16(ctx.r10.u32 + 0);
		// cmplwi cr6,r6,2
		// bne cr6,0x82459dfc
		if (ctx.r6.u32 == 2) {
			// lhz r5,3(r10)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r10.u32 + 3);
			// cmplwi cr6,r5,0
			// beq cr6,0x82459e24
			if (ctx.r5.u32 == 0) {
				// li r3,1
				ctx.r3.s64 = 1;
				// blr
				return;
			}
		}
	loc_82459DFC:
		// addi r4,r11,1
		ctx.r4.s64 = ctx.r11.s64 + 1;
		// clrlwi r11,r4,24
		ctx.r11.u64 = ctx.r4.u32 & 0xFF;
		// cmplw cr6,r11,r8
		// blt cr6,0x82459dd0
		if (ctx.r11.u32 < ctx.r8.u32) goto loc_82459DD0;
	}
loc_82459E0C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__game_9E40"))) PPC_WEAK_FUNC(game_9E40);
PPC_FUNC_IMPL(__imp__game_9E40) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lfs f0,15784(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// blt cr6,0x82459e78
	if (ctx.f1.f64 >= ctx.f0.f64) {
		// bso cr6,0x82459e78
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x82459E6C, "bso");
		// stfs f1,56(r31)
		temp.f32 = float(ctx.f1.f64);
		PPC_STORE_U32(var_r31 + 56, temp.u32);
		// b 0x82459e7c
	} else {
	loc_82459E78:
		// stfs f0,56(r31)
		ctx.fpscr.disableFlushMode();
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(var_r31 + 56, temp.u32);
	}
loc_82459E7C:
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x82459ea8
	if (ctx.r11.u32 != 0) {
		// lfs f31,40(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 40);
		var_f31 = double(temp.f32);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lfs f30,56(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 56);
		var_f30 = double(temp.f32);
		// bl 0x82454ac0
		atSingleton_4AC0_g(ctx, base);
		// fmuls f0,f31,f1
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = double(float(var_f31 * ctx.f1.f64));
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// fmuls f1,f0,f30
		ctx.f1.f64 = double(float(ctx.f0.f64 * var_f30));
		// bl 0x82458d80
		game_8D80_1(ctx, base);
	}
loc_82459EA8:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_9EC8_h"))) PPC_WEAK_FUNC(atSingleton_9EC8_h);
PPC_FUNC_IMPL(__imp__atSingleton_9EC8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// fmr f30,f1
	var_f30 = ctx.f1.f64;
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// stfs f30,60(r31)
	temp.f32 = float(var_f30);
	PPC_STORE_U32(var_r31 + 60, temp.u32);
	// cmplwi cr6,r11,0
	// beq cr6,0x82459f14
	if (ctx.r11.u32 != 0) {
		// lfs f31,44(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 44);
		var_f31 = double(temp.f32);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x82454148
		atSingleton_4148_g(ctx, base);
		// fadds f0,f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = double(float(ctx.f1.f64 + var_f31));
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// fadds f1,f0,f30
		ctx.f1.f64 = double(float(ctx.f0.f64 + var_f30));
		// bl 0x82458dc0
		atSingleton_8DC0(ctx, base);
	}
loc_82459F14:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__rage_9F30"))) PPC_WEAK_FUNC(rage_9F30);
PPC_FUNC_IMPL(__imp__rage_9F30) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r27,0
	var_r27 = 0;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r10,0,29,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r9,4
	// bne cr6,0x8245a068
	if (ctx.r9.u32 == 4) {
		// lbz r4,48(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 48);
		// bl 0x82454838
		rage_4838(ctx, base);
		// clrlwi r26,r3,24
		var_r26 = (uint32_t)(ctx.r3.u32 & 0xFF);
		// cmplwi cr6,r26,0
		// bne cr6,0x82459f7c
		if (var_r26 == 0) {
			// li r3,4
			ctx.r3.s64 = 4;
			// bl 0x820dec88
			xe_EC88(ctx, base);
			// b 0x82459fb8
		} else {
		loc_82459F7C:
			// lis r8,1365
			ctx.r8.s64 = 89456640;
			// ori r7,r8,21845
			ctx.r7.u64 = ctx.r8.u64 | 21845;
			// cmplw cr6,r26,r7
			// bgt cr6,0x82459fa8
			if (var_r26 <= ctx.r7.u32) {
				// rlwinm r11,r26,1,0,30
				ctx.r11.u64 = __builtin_rotateleft64(var_r26 | (var_r26 << 32), 1) & 0xFFFFFFFE;
				// li r6,-5
				// add r5,r26,r11
				ctx.r5.u64 = var_r26 + ctx.r11.u64;
				// rlwinm r11,r5,4,0,27
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 4) & 0xFFFFFFF0;
				// cmplw cr6,r11,r6
				// addi r3,r11,4
				ctx.r3.s64 = ctx.r11.s64 + 4;
				// ble cr6,0x82459fac
				if (ctx.r11.u32 <= ctx.r6.u32) goto loc_82459FAC;
			}
		loc_82459FA8:
			// li r3,-1
		loc_82459FAC:
			// lis r4,8324
			ctx.r4.s64 = 545521664;
			// ori r4,r4,83
			ctx.r4.u64 = ctx.r4.u64 | 83;
			// bl 0x820c01b8
			rage_01B8(ctx, base);
		}
	loc_82459FB8:
		// cmplwi cr6,r3,0
		// beq cr6,0x82459ff8
		if (ctx.r3.u32 != 0) {
			// addi r28,r3,4
			var_r28 = (uint32_t)(ctx.r3.s64 + 4);
			// stw r26,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* rage_GameObject::vtable@+0x0 */ var_r26);
			// addi r30,r26,-1
			var_r30 = (uint32_t)(var_r26 + -1);
			// mr r29,r28
			var_r29 = (uint32_t)(var_r28);
			// cmpwi cr6,r30,0
			// blt cr6,0x82459ff0
		while ((int32_t)var_r30 >= 0) {
			loc_82459FD8:
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x82453098
				rage_3098(ctx, base);
				// addi r30,r30,-1
				var_r30 = (uint32_t)(var_r30 + -1);
				// addi r29,r29,48
				var_r29 = (uint32_t)(var_r29 + 48);
				// cmpwi cr6,r30,0
				// bge cr6,0x82459fd8
		}
		loc_82459FF0:
			// mr r11,r28
			ctx.r11.u64 = var_r28;
			// b 0x82459ffc
		} else {
		loc_82459FF8:
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_82459FFC:
		// cmplwi cr6,r11,0
		// stw r11,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r11.u32);
		// beq cr6,0x8245a074
		if (ctx.r11.u32 == 0) {
			// lis r3,-32761
			// ori r3,r3,14
			ctx.r3.u64 = ctx.r3.u64 | 14;
			return;
		}
		// li r5,0
		ctx.r5.s64 = 0;
	loc_8245A00C:
		// clrlwi r30,r5,24
		var_r30 = (uint32_t)(ctx.r5.u32 & 0xFF);
		// cmplw cr6,r30,r26
		// bge cr6,0x8245a068
		if (var_r30 >= var_r26) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// lbz r4,48(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 48);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x824548c8
		msgMsgSink_48C8_p39(ctx, base);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// bl 0x82448d90
		rage_8D90(ctx, base);
		// rlwinm r11,r30,1,0,30
		ctx.r11.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 1) & 0xFFFFFFFE;
		// mr r6,r3
		ctx.r6.u64 = ctx.r3.u64;
		// lwz r10,52(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
		// add r3,r30,r11
		ctx.r3.u64 = var_r30 + ctx.r11.u64;
		// lwz r4,12(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 12);
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// rlwinm r11,r3,4,0,27
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 4) & 0xFFFFFFF0;
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// bl 0x82453250
		rage_3250(ctx, base);
		// mr r27,r3
		var_r27 = ctx.r3.u32;
		// addi r11,r30,1
		ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 1;
		// cmpwi cr6,r27,0
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// bge cr6,0x8245a00c
		if ((int32_t)var_r27 >= 0) goto loc_8245A00C;
	}
loc_8245A068:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__rage_A088"))) PPC_WEAK_FUNC(rage_A088);
PPC_FUNC_IMPL(__imp__rage_A088) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// lwz r3,20(r29)
	ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 20);
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r10,0,29,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r9,4
	// bne cr6,0x8245a0f0
	if (ctx.r9.u32 == 4) {
		// lbz r4,48(r29)
		ctx.r4.u64 = PPC_LOAD_U8(var_r29 + 48);
		// bl 0x82454838
		rage_4838(ctx, base);
		// clrlwi r30,r3,24
		var_r30 = (uint32_t)(ctx.r3.u32 & 0xFF);
		// cmplwi cr6,r30,0
		// beq cr6,0x8245a0f0
		if (var_r30 == 0) {
			return;
		}
		// li r31,0
		var_r31 = 0;
	loc_8245A0C8:
		// rlwinm r11,r31,1,0,30
		ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 1) & 0xFFFFFFFE;
		// lwz r10,52(r29)
		ctx.r10.u64 = PPC_LOAD_U32(var_r29 + 52);
		// add r8,r31,r11
		ctx.r8.u64 = var_r31 + ctx.r11.u64;
		// rlwinm r11,r8,4,0,27
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 4) & 0xFFFFFFF0;
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// bl 0x82452ee0
		rage_2EE0(ctx, base);
		// addi r7,r31,1
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 1;
		// clrlwi r31,r7,24
		var_r31 = (uint32_t)(ctx.r7.u32 & 0xFF);
		// cmplw cr6,r31,r30
		// blt cr6,0x8245a0c8
		if (var_r31 < var_r30) goto loc_8245A0C8;
	}
loc_8245A0F0:
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_0_A0F8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_0_A0F8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_0_A0F8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x82459988
	atSingleton_9988_wrh(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a138
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32772
		ctx.r4.u64 = ctx.r4.u64 | 32772;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245A138:
	// blr
	return;
}

__attribute__((alias("__imp__phBoundCapsule_A150"))) PPC_WEAK_FUNC(phBoundCapsule_A150);
PPC_FUNC_IMPL(__imp__phBoundCapsule_A150) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// li r30,0
	var_r30 = 0;
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a1c4
	if (ctx.r11.u32 != 0) {
		// cmpwi cr6,r29,0
		// bne cr6,0x8245a1b4
		if ((int32_t)var_r29 == 0) {
			// bl 0x82459d88
			game_9D88(ctx, base);
			// cmpwi cr6,r3,0
			// beq cr6,0x8245a1b0
			if (ctx.r3.s32 != 0) {
				// lwz r10,24(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 24);
				// lwz r3,36(r10)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 36);
				// cmplwi cr6,r3,0
				// beq cr6,0x8245a1b0
				if (ctx.r3.u32 == 0) goto loc_8245A1B0;
				// lwz r8,24(r9)
				// bctrl
				VCALL(ctx.r3.u32, 6, ctx, base);  // vtable slot 6 (byte +24)
				// cmpwi cr6,r3,0
				// bne cr6,0x8245a1b4
				if (ctx.r3.s32 != 0) goto loc_8245A1B4;
			}
		loc_8245A1B0:
			// li r29,1
			var_r29 = 1;
		}
	loc_8245A1B4:
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// bl 0x82459760
		game_9760(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
	}
loc_8245A1C4:
	// lwz r4,28(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 28);
	// cmplwi cr6,r4,0
	// beq cr6,0x8245a1e0
	if (ctx.r4.u32 != 0) {
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// bl 0x82449b30
		atSingleton_9B30_p42(ctx, base);
		// li r7,0
		ctx.r7.s64 = 0;
		// stw r7,28(r31)
		PPC_STORE_U32(var_r31 + 28, ctx.r7.u32);
	}
loc_8245A1E0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__rage_A1F0"))) PPC_WEAK_FUNC(rage_A1F0);
PPC_FUNC_IMPL(__imp__rage_A1F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// bl 0x82459f30
	rage_9F30(ctx, base);
	// lis r4,8324
	ctx.r4.s64 = 545521664;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// ori r4,r4,32776
	ctx.r4.u64 = ctx.r4.u64 | 32776;
	// li r3,64
	ctx.r3.s64 = 64;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245a23c
	if (ctx.r3.u32 != 0) {
		// li r6,-1
		// lwz r4,12(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 12);
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// bl 0x82458eb0
		msgMsgSink_8EB0_w(ctx, base);
		// b 0x8245a240
	} else {
	loc_8245A23C:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_8245A240:
	// cmplwi cr6,r3,0
	// stw r3,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r3.u32);
	// bne cr6,0x8245a288
	if (ctx.r3.u32 == 0) {
		// lis r30,-32761
		var_r30 = (uint32_t)(-2147024896);
		// ori r30,r30,14
		var_r30 = (uint32_t)(var_r30 | 14);
	loc_8245A254:
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// cmplwi cr6,r3,0
		// beq cr6,0x8245a274
		if (ctx.r3.u32 != 0) {
			// lwz r11,0(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* rage_GameObject::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// lwz r10,0(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		}
	loc_8245A274:
		// li r9,0
		ctx.r9.s64 = 0;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r9,24(r31)
		PPC_STORE_U32(var_r31 + 24, ctx.r9.u32);
		return;
	}
loc_8245A288:
	// cmpwi cr6,r30,0
	// blt cr6,0x8245a254
	if ((int32_t)var_r30 < 0) goto loc_8245A254;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x82458ae0
	msgMsgSink_8AE0_w(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x8245a254
	if ((int32_t)var_r30 < 0) goto loc_8245A254;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8245a088
	rage_A088(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__rage_A2C0"))) PPC_WEAK_FUNC(rage_A2C0);
PPC_FUNC_IMPL(__imp__rage_A2C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// bl 0x82459f30
	rage_9F30(ctx, base);
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82459ae0
	msgMsgSink_9AE0_w(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x8245a30c
	if ((int32_t)var_r30 >= 0) {
		// lwz r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,24(r31)
		PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
		// bl 0x8245a088
		rage_A088(ctx, base);
	}
loc_8245A30C:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__game_A318_h"))) PPC_WEAK_FUNC(game_A318_h);
PPC_FUNC_IMPL(__imp__game_A318_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,52(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 52);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r11,0
	// beq cr6,0x8245a344
	if (ctx.r11.s32 != 0) {
		// lwz r10,16(r4)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
		// lwz r4,41(r10)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 41);
		// b 0x8245a348
	} else {
	loc_8245A344:
		// li r4,-1
	}
loc_8245A348:
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82459ae0
	msgMsgSink_9AE0_w(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x8245a368
	if (ctx.r3.s32 >= 0) {
		// lwz r9,80(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r9,28(r31)
		PPC_STORE_U32(var_r31 + 28, ctx.r9.u32);
	}
loc_8245A368:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A380"))) PPC_WEAK_FUNC(atSingleton_A380);
PPC_FUNC_IMPL(__imp__atSingleton_A380) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r3,0
	ctx.r3.s64 = 0;
	// lwz r11,28(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 28);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a3e8
	if (ctx.r11.u32 != 0) {
		// lwz r11,40(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
		// cmpwi cr6,r11,3
		// bne cr6,0x8245a3e8
		if (ctx.r11.s32 != 3) {
			// lbz r9,32(r31)
			ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 32);
			// ori r8,r9,128
			ctx.r8.u64 = ctx.r9.u64 | 128;
			// stb r8,32(r31)
			PPC_STORE_U8(var_r31 + 32, ctx.r8.u8);
			// blr
			return;
		}
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// lwz r4,24(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 24);
		// bl 0x82449b30
		atSingleton_9B30_p42(ctx, base);
		// lwz r11,28(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 28);
		// li r10,0
		ctx.r10.s64 = 0;
		// rotlwi r3,r11,0
		ctx.r3.u64 = ctx.r11.u32;
		// stw r11,24(r31)
		PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
		// stw r10,28(r31)
		PPC_STORE_U32(var_r31 + 28, ctx.r10.u32);
		// bl 0x82459048
		atSingleton_9048_g(ctx, base);
		// blr
		return;
	}
loc_8245A3E8:
	// lbz r9,32(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 32);
	// ori r8,r9,128
	ctx.r8.u64 = ctx.r9.u64 | 128;
	// stb r8,32(r31)
	PPC_STORE_U8(var_r31 + 32, ctx.r8.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A408_g"))) PPC_WEAK_FUNC(atSingleton_A408_g);
PPC_FUNC_IMPL(__imp__atSingleton_A408_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lfs f0,15784(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// blt cr6,0x8245a440
	if (ctx.f1.f64 >= ctx.f0.f64) {
		// bso cr6,0x8245a440
		// UNIMPLEMENTED: bso
		PPC_UNIMPLEMENTED(0x8245A434, "bso");
		// stfs f1,40(r31)
		temp.f32 = float(ctx.f1.f64);
		PPC_STORE_U32(var_r31 + 40, temp.u32);
		// b 0x8245a444
	} else {
	loc_8245A440:
		// stfs f0,40(r31)
		ctx.fpscr.disableFlushMode();
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(var_r31 + 40, temp.u32);
	}
loc_8245A444:
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a470
	if (ctx.r11.u32 != 0) {
		// lfs f31,40(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 40);
		var_f31 = double(temp.f32);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lfs f30,56(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 56);
		var_f30 = double(temp.f32);
		// bl 0x82454ac0
		atSingleton_4AC0_g(ctx, base);
		// fmuls f0,f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = double(float(ctx.f1.f64 * var_f31));
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// fmuls f1,f0,f30
		ctx.f1.f64 = double(float(ctx.f0.f64 * var_f30));
		// bl 0x82458d80
		game_8D80_1(ctx, base);
	}
loc_8245A470:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A490_g"))) PPC_WEAK_FUNC(atSingleton_A490_g);
PPC_FUNC_IMPL(__imp__atSingleton_A490_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// fmr f30,f1
	var_f30 = ctx.f1.f64;
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
	// stfs f30,44(r31)
	temp.f32 = float(var_f30);
	PPC_STORE_U32(var_r31 + 44, temp.u32);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a4dc
	if (ctx.r11.u32 != 0) {
		// lfs f31,60(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 60);
		var_f31 = double(temp.f32);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x82454148
		atSingleton_4148_g(ctx, base);
		// fadds f0,f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = double(float(ctx.f1.f64 + var_f31));
		// lwz r3,24(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
		// fadds f1,f0,f30
		ctx.f1.f64 = double(float(ctx.f0.f64 + var_f30));
		// bl 0x82458dc0
		atSingleton_8DC0(ctx, base);
	}
loc_8245A4DC:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_A4F8_g"))) PPC_WEAK_FUNC(msgMsgSink_A4F8_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_A4F8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r6,0
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 0, ctx.xer);
	// lfs f0,15784(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	// stfs f0,44(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 44, temp.u32);
	// stw r31,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ var_r31);
	// addi r10,r11,21664
	ctx.r10.s64 = ctx.r11.s64 + 21664;
	// stfs f0,60(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 60, temp.u32);
	// lis r11,-32256
	// stw r31,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r31);
	// stw r4,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r4.u32);
	// stw r6,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r6.u32);
	// stw r5,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r5.u32);
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r10.u32);
	// lfs f1,15788(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
	ctx.f1.f64 = double(temp.f32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stfs f1,56(r31)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(var_r31 + 56, temp.u32);
	// stb r7,48(r31)
	PPC_STORE_U8(var_r31 + 48, ctx.r7.u8);
	// stw r11,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
	// stw r11,36(r31)
	PPC_STORE_U32(var_r31 + 36, ctx.r11.u32);
	// stw r11,52(r31)
	PPC_STORE_U32(var_r31 + 52, ctx.r11.u32);
	// stb r11,32(r31)
	PPC_STORE_U8(var_r31 + 32, ctx.r11.u8);
	// beq cr6,0x8245a580
	if (!(ctx.cr6.eq)) {
		// lbz r10,0(r6)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r6.u32 + 0);
		// lis r11,-32256
		// addi r11,r11,20640
		ctx.r11.s64 = ctx.r11.s64 + 20640;
		// rotlwi r9,r10,2
		ctx.r9.u64 = __builtin_rotateleft32(ctx.r10.u32, 2);
		// lfsx f1,r9,r11
		temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
		ctx.f1.f64 = double(temp.f32);
	}
loc_8245A580:
	// bl 0x8245a408
	atSingleton_A408_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A5A0_h"))) PPC_WEAK_FUNC(atSingleton_A5A0_h);
PPC_FUNC_IMPL(__imp__atSingleton_A5A0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// lwz r11,28(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// rlwinm r9,r10,0,29,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r9,4
	// bne cr6,0x8245a608
	if (ctx.r9.u32 == 4) {
		// lbz r4,48(r30)
		ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 48);
		// bl 0x82454838
		rage_4838(ctx, base);
		// clrlwi r29,r3,24
		var_r29 = (uint32_t)(ctx.r3.u32 & 0xFF);
		// cmplwi cr6,r29,0
		// beq cr6,0x8245a608
		if (var_r29 == 0) goto loc_8245A608;
		// li r31,0
		var_r31 = 0;
	loc_8245A5E0:
		// rlwinm r11,r31,1,0,30
		ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 1) & 0xFFFFFFFE;
		// lwz r10,52(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 52);
		// add r8,r31,r11
		ctx.r8.u64 = var_r31 + ctx.r11.u64;
		// rlwinm r11,r8,4,0,27
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 4) & 0xFFFFFFF0;
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// bl 0x82453078
		atSingleton_3078_p39(ctx, base);
		// addi r7,r31,1
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 1;
		// clrlwi r31,r7,24
		var_r31 = (uint32_t)(ctx.r7.u32 & 0xFF);
		// cmplw cr6,r31,r29
		// blt cr6,0x8245a5e0
		if (var_r31 < var_r29) goto loc_8245A5E0;
	}
loc_8245A608:
	// lwz r3,24(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 24);
	// bl 0x824598f8
	atSingleton_98F8_fw(ctx, base);
	// lbz r10,32(r30)
	ctx.r10.u64 = PPC_LOAD_U8(var_r30 + 32);
	// rlwinm r6,r10,0,0,24
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r6,128
	// bne cr6,0x8245a648
	if (ctx.r6.u32 == 128) {
		// lwz r11,28(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
		// cmplwi cr6,r11,0
		// beq cr6,0x8245a648
		if (ctx.r11.u32 == 0) {
			return;
		}
		// lwz r11,40(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
		// cmpwi cr6,r11,3
		// bne cr6,0x8245a648
		if (ctx.r11.s32 != 3) {
			return;
		}
		// clrlwi r5,r10,25
		ctx.r5.u64 = ctx.r10.u32 & 0x7F;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stb r5,32(r30)
		PPC_STORE_U8(var_r30 + 32, ctx.r5.u8);
		// bl 0x8245a380
		atSingleton_A380(ctx, base);
	}
loc_8245A648:
	return;
}

__attribute__((alias("__imp__game_A650_h"))) PPC_WEAK_FUNC(game_A650_h);
PPC_FUNC_IMPL(__imp__game_A650_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r4,36(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 36);
	// cmplwi cr6,r4,0
	// beq cr6,0x8245a67c
	if (ctx.r4.u32 != 0) {
		// bl 0x8245a318
		game_A318_h(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,36(r31)
		PPC_STORE_U32(var_r31 + 36, ctx.r11.u32);
	}
loc_8245A67C:
	// lwz r3,24(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 24);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_A698_h"))) PPC_WEAK_FUNC(jumptable_A698_h);
PPC_FUNC_IMPL(__imp__jumptable_A698_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r8
	var_r30 = ctx.r8.u32;
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r30,52(r31)
	PPC_STORE_U32(var_r31 + 52, var_r30);
	// addi r11,r11,21668
	ctx.r11.s64 = ctx.r11.s64 + 21668;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,48(r31)
	PPC_STORE_U32(var_r31 + 48, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_1_A6F0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_1_A6F0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_1_A6F0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,21668
	ctx.r11.s64 = ctx.r11.s64 + 21668;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a73c
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32778
		ctx.r4.u64 = ctx.r4.u64 | 32778;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245A73C:
	// blr
	return;
}

__attribute__((alias("__imp__game_A758_h"))) PPC_WEAK_FUNC(game_A758_h);
PPC_FUNC_IMPL(__imp__game_A758_h) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lwz r11,48(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 48);
	// cmpwi cr6,r11,0
	// bne cr6,0x8245a78c
	if (ctx.r11.s32 == 0) {
		// lwz r11,16(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// addi r4,r11,7
		ctx.r4.s64 = ctx.r11.s64 + 7;
		// bl 0x82447dd0
		game_7DD0(ctx, base);
		// cmpwi cr6,r3,0
		// li r3,0
		ctx.r3.s64 = 0;
		// beq cr6,0x8245a790
		if (ctx.r3.s32 == 0) {
			// blr
			return;
		}
	}
loc_8245A78C:
	// li r3,1
	ctx.r3.s64 = 1;
loc_8245A790:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_2_A7A0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_2_A7A0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_2_A7A0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r11,48(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 48);
	// cmpwi cr6,r11,0
	// bne cr6,0x8245a7dc
	if (ctx.r11.s32 == 0) {
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// addi r4,r11,7
		ctx.r4.s64 = ctx.r11.s64 + 7;
		// bl 0x82447dd0
		game_7DD0(ctx, base);
		// cmpwi cr6,r3,0
		// beq cr6,0x8245a814
		if (ctx.r3.s32 == 0) {
			// blr
			return;
		}
	}
loc_8245A7DC:
	// lwz r10,44(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r10,16(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// lwz r11,28(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245a7fc
	if (ctx.r11.u32 != 0) {
		// lwz r11,40(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
		// cmpwi cr6,r11,0
		// bne cr6,0x8245a800
		if (ctx.r11.s32 != 0) goto loc_8245A800;
	}
loc_8245A7FC:
	// stw r31,36(r10)
	PPC_STORE_U32(ctx.r10.u32 + 36, var_r31);
loc_8245A800:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82452378
	atSingleton_2378_g(ctx, base);
	// lwz r9,44(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r3,16(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
	// bl 0x8245a380
	atSingleton_A380(ctx, base);
loc_8245A814:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A828_2hr"))) PPC_WEAK_FUNC(atSingleton_A828_2hr);
PPC_FUNC_IMPL(__imp__atSingleton_A828_2hr) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// addi r11,r11,21688
	ctx.r11.s64 = ctx.r11.s64 + 21688;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A838_fw"))) PPC_WEAK_FUNC(atSingleton_A838_fw);
PPC_FUNC_IMPL(__imp__atSingleton_A838_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// mr r26,r6
	var_r26 = ctx.r6.u32;
	// li r29,0
	var_r29 = 0;
	// addi r30,r28,8
	var_r30 = (uint32_t)(var_r28 + 8);
	// li r11,0
	ctx.r11.s64 = 0;
loc_8245A85C:
	// clrlwi r31,r11,16
	var_r31 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
	// lhz r11,2(r28)
	ctx.r11.u64 = PPC_LOAD_U16(var_r28 + 2);
	// cmplw cr6,r31,r11
	// bge cr6,0x8245a8c0
	if (var_r31 >= ctx.r11.u32) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
	// rlwinm r11,r31,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// add r11,r31,r11
	ctx.r11.u64 = var_r31 + ctx.r11.u64;
	// li r9,5
	ctx.r9.s64 = 5;
	// add r11,r11,r30
	ctx.r11.u64 = ctx.r11.u64 + var_r30;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_8245A884:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stb r9,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r9.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x8245a884
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245A884;
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// lbz r4,82(r1)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 82);
	// bl 0x82450090
	atSingleton_0090_g(ctx, base);
	// cmplw cr6,r3,r26
	// bne cr6,0x8245a8b0
	if (ctx.r3.u32 == var_r26) {
		// li r29,1
		var_r29 = 1;
	}
loc_8245A8B0:
	// addi r8,r31,1
	ctx.r8.s64 = (int64_t)(int32_t)var_r31 + 1;
	// cmpwi cr6,r29,0
	// clrlwi r11,r8,16
	ctx.r11.u64 = ctx.r8.u32 & 0xFFFF;
	// beq cr6,0x8245a85c
	if ((int32_t)var_r29 == 0) goto loc_8245A85C;
loc_8245A8C0:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__atSingleton_A8D0"))) PPC_WEAK_FUNC(atSingleton_A8D0);
PPC_FUNC_IMPL(__imp__atSingleton_A8D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=176, savegprlr_24
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r25,r6
	var_r25 = ctx.r6.u32;
	// li r27,0
	var_r27 = 0;
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r30,r27
	var_r30 = (uint32_t)(var_r27);
	// mr r4,r25
	ctx.r4.u64 = var_r25;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// mr r24,r3
	var_r24 = ctx.r3.u32;
	// lbz r9,0(r24)
	ctx.r9.u64 = PPC_LOAD_U8(var_r24 + 0);
	// clrlwi r8,r9,31
	ctx.r8.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r8,1
	// bne cr6,0x8245aa4c
	if (ctx.r8.u32 == 1) {
		// lbz r7,9(r24)
		ctx.r7.u64 = PPC_LOAD_U8(var_r24 + 9);
		// mr r28,r27
		var_r28 = (uint32_t)(var_r27);
		// cmplwi cr6,r7,0
		// beq cr6,0x8245aa40
	while (ctx.r8.u32 < ctx.r10.u32) {
		loc_8245A930:
			// cmpwi cr6,r30,0
			// bne cr6,0x8245aa40
			if ((int32_t)var_r30 != 0) {
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				return;
			}
			// lwz r6,0(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
			// mr r5,r28
			ctx.r5.u64 = var_r28;
			// mr r4,r25
			ctx.r4.u64 = var_r25;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lwz r11,44(r6)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r6.u32 + 44);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
			// mr r10,r3
			ctx.r10.u64 = ctx.r3.u64;
			// addi r11,r1,84
			ctx.r11.s64 = ctx.r1.s64 + 84;
			// lwz r9,0(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lwz r7,1(r10)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1);
			// lwz r8,24(r9)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 24);
			// stw r7,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r7.u32);
			// stw r27,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, var_r27);
			// stw r27,4(r11)
			PPC_STORE_U32(ctx.r11.u32 + 4, var_r27);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
			// mr r11,r3
			ctx.r11.u64 = ctx.r3.u64;
			// cmplwi cr6,r11,0
			// beq cr6,0x8245aa24
		while (ctx.r11.u32 != 0) {
			loc_8245A990:
				// lwz r6,0(r11)
				ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
				// clrlwi r10,r6,27
				ctx.r10.u64 = ctx.r6.u32 & 0x1F;
				// cmplwi cr6,r10,1
				// beq cr6,0x8245a9e4
				if (ctx.r10.u32 != 1) {
					// cmplwi cr6,r10,4
					// beq cr6,0x8245a9e4
					if (ctx.r10.u32 == 4) goto loc_8245A9E4;
					// cmplwi cr6,r10,2
					// beq cr6,0x8245a9e4
					if (ctx.r10.u32 == 2) goto loc_8245A9E4;
					// cmplwi cr6,r10,3
					// beq cr6,0x8245a9c8
					if (ctx.r10.u32 != 3) {
						// cmplwi cr6,r10,6
						// beq cr6,0x8245a9c8
						if (ctx.r10.u32 == 6) goto loc_8245A9C8;
						// cmplwi cr6,r10,5
						// bne cr6,0x8245aa00
						if (ctx.r10.u32 != 5) goto loc_8245AA00;
					}
				loc_8245A9C8:
					// mr r6,r29
					ctx.r6.u64 = var_r29;
					// mr r5,r31
					ctx.r5.u64 = var_r31;
					// addi r4,r11,8
					ctx.r4.s64 = ctx.r11.s64 + 8;
					// mr r3,r26
					ctx.r3.u64 = var_r26;
					// bl 0x8245a838
					atSingleton_A838_fw(ctx, base);
					// mr r30,r3
					var_r30 = ctx.r3.u32;
					// b 0x8245aa00
				} else {
				loc_8245A9E4:
					// mr r3,r31
					ctx.r3.u64 = var_r31;
					// lbz r4,9(r11)
					ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 9);
					// bl 0x82450090
					atSingleton_0090_g(ctx, base);
					// cmplw cr6,r3,r29
					// li r30,1
					var_r30 = 1;
					// beq cr6,0x8245aa00
					if (ctx.r3.u32 == var_r29) goto loc_8245AA00;
					// mr r30,r27
					var_r30 = (uint32_t)(var_r27);
				}
			loc_8245AA00:
				// lwz r5,0(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// addi r4,r1,80
				ctx.r4.s64 = ctx.r1.s64 + 80;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// lwz r11,28(r5)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 28);
				// bctrl
				PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
				// mr r11,r3
				ctx.r11.u64 = ctx.r3.u64;
				// cmplwi cr6,r11,0
				// bne cr6,0x8245a990
		}
		loc_8245AA24:
			// clrlwi r11,r28,24
			ctx.r11.u64 = var_r28 & 0xFF;
			// lbz r10,9(r24)
			ctx.r10.u64 = PPC_LOAD_U8(var_r24 + 9);
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// clrlwi r28,r9,24
			var_r28 = (uint32_t)(ctx.r9.u32 & 0xFF);
			// mr r8,r28
			ctx.r8.u64 = var_r28;
			// cmplw cr6,r8,r10
			// blt cr6,0x8245a930
	}
	loc_8245AA40:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		return;
	}
loc_8245AA4C:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lbz r4,11(r24)
	ctx.r4.u64 = PPC_LOAD_U8(var_r24 + 11);
	// bl 0x82450090
	atSingleton_0090_g(ctx, base);
	// cmplw cr6,r3,r29
	// bne cr6,0x8245aa6c
	if (ctx.r3.u32 == var_r29) {
		// li r3,1
		ctx.r3.s64 = 1;
		return;
	}
loc_8245AA6C:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_6_AA78_1"))) PPC_WEAK_FUNC(atSingleton_vfn_6_AA78_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_6_AA78_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,21688
	ctx.r11.s64 = ctx.r11.s64 + 21688;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x8245aab4
	if (ctx.r10.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32791
		ctx.r4.u64 = ctx.r4.u64 | 32791;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245AAB4:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_AAC8_w"))) PPC_WEAK_FUNC(pongNetMessageHolder_AAC8_w);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_AAC8_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=192, savegprlr_25
	// mr r26,r6
	var_r26 = ctx.r6.u32;
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// li r11,-1
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// li r10,-1
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// stw r11,0(r26)
	PPC_STORE_U32(var_r26 + 0, ctx.r11.u32);
	// sth r10,0(r27)
	PPC_STORE_U16(var_r27 + 0, ctx.r10.u16);
	// lwz r9,8(r28)
	ctx.r9.u64 = PPC_LOAD_U32(var_r28 + 8);
	// lwz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// clrlwi r30,r8,16
	var_r30 = (uint32_t)(ctx.r8.u32 & 0xFFFF);
	// rlwinm r3,r30,1,0,30
	ctx.r3.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 1) & 0xFFFFFFFE;
	// bl 0x820dec88
	xe_EC88(ctx, base);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// cmplwi cr6,r29,0
	// beq cr6,0x8245ac04
	if (var_r29 != 0) {
		// lwz r11,8(r28)
		ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 8);
		// li r25,0
		var_r25 = 0;
		// li r6,0
		ctx.r6.s64 = 0;
		// addi r31,r11,8
		var_r31 = (uint32_t)(ctx.r11.s64 + 8);  // addr:0x82000008
		// li r7,0
		ctx.r7.s64 = 0;
		// cmplwi cr6,r30,0
		// beq cr6,0x8245abb8
		if (var_r30 != 0) {
			// li r8,0
			ctx.r8.s64 = 0;
		loc_8245AB38:
			// rlwinm r11,r8,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 4) & 0xFFFFFFF0;
			// addi r10,r1,96
			ctx.r10.s64 = ctx.r1.s64 + 96;
			// add r11,r11,r31
			ctx.r11.u64 = ctx.r11.u64 + var_r31;
			// li r9,16
			ctx.r9.s64 = 16;
			// mtctr r9
			ctx.ctr.u64 = ctx.r9.u64;
		loc_8245AB4C:
			// lbz r5,0(r11)
			ctx.r5.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// stb r5,0(r10)
			PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r5.u8);
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// bdnz 0x8245ab4c
			--ctx.ctr.u64;
			if (ctx.ctr.u32 != 0) goto loc_8245AB4C;
			// lwz r4,100(r1)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
			// stw r4,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r4.u32);
			// lfs f0,80(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f0.f64 = double(temp.f32);
			// fcmpu cr6,f0,f31
			// bgt cr6,0x8245aba4
			if (ctx.f0.f64 <= var_f31) {
				// bso cr6,0x8245aba4
				// UNIMPLEMENTED: bso
				PPC_UNIMPLEMENTED(0x8245AB74, "bso");
				// lwz r3,104(r1)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
				// stw r3,84(r1)
				PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r3.u32);
				// lfs f13,84(r1)
				temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
				ctx.f13.f64 = double(temp.f32);
				// fcmpu cr6,f13,f31
				// blt cr6,0x8245aba4
				if (ctx.f13.f64 < var_f31) goto loc_8245ABA4;
				// bso cr6,0x8245aba4
				// UNIMPLEMENTED: bso
				PPC_UNIMPLEMENTED(0x8245AB8C, "bso");
				// clrlwi r11,r6,16
				ctx.r11.u64 = ctx.r6.u32 & 0xFFFF;
				// rlwinm r10,r11,1,0,30
				ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
				// addi r9,r11,1
				ctx.r9.s64 = ctx.r11.s64 + 1;
				// clrlwi r6,r9,16
				ctx.r6.u64 = ctx.r9.u32 & 0xFFFF;
				// sthx r7,r10,r29
				PPC_STORE_U16(ctx.r10.u32 + var_r29, ctx.r7.u16);
			}
		loc_8245ABA4:
			// addi r8,r8,1
			ctx.r8.s64 = ctx.r8.s64 + 1;
			// clrlwi r7,r8,16
			ctx.r7.u64 = ctx.r8.u32 & 0xFFFF;
			// mr r8,r7
			ctx.r8.u64 = ctx.r7.u64;
			// cmplw cr6,r8,r30
			// blt cr6,0x8245ab38
			if (ctx.r8.u32 < var_r30) goto loc_8245AB38;
		}
	loc_8245ABB8:
		// clrlwi r11,r6,16
		ctx.r11.u64 = ctx.r6.u32 & 0xFFFF;
		// cmplwi cr6,r11,0
		// beq cr6,0x8245abec
		if (ctx.r11.u32 != 0) {
			// addi r5,r11,-1
			ctx.r5.s64 = ctx.r11.s64 + -1;
			// lwz r3,4(r28)
			ctx.r3.u64 = PPC_LOAD_U32(var_r28 + 4);
			// li r4,0
			ctx.r4.s64 = 0;
			// bl 0x82447ce0
			atSingleton_7CE0_w(ctx, base);
			// rlwinm r6,r3,1,15,30
			ctx.r6.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0x1FFFE;
			// lhzx r11,r6,r29
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r6.u32 + var_r29);
			// rotlwi r5,r11,4
			ctx.r5.u64 = __builtin_rotateleft32(ctx.r11.u32, 4);
			// lwzx r4,r5,r31
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + var_r31);
			// stw r4,0(r26)
			PPC_STORE_U32(var_r26 + 0, ctx.r4.u32);
			// sth r11,0(r27)
			PPC_STORE_U16(var_r27 + 0, ctx.r11.u16);
		}
	loc_8245ABEC:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x820c0120
		thunk_rage_free(ctx, base);
		// mr r3,r25
		ctx.r3.u64 = var_r25;
		return;
	}
loc_8245AC04:
	// lis r3,-32761
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_AC18_2hr"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_AC18_2hr);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_AC18_2hr) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// rlwinm r10,r4,4,12,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 4) & 0xFFFF0;
	// li r3,0
	ctx.r3.s64 = 0;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmpwi cr6,r10,0
	// beqlr cr6
	if (ctx.r10.s32 == 0) return;
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,-16(r1)
	PPC_STORE_U32(ctx.r1.u32 + -16, ctx.r9.u32);
	// lfs f0,-16(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	// bltlr cr6
	if (ctx.f1.f64 < ctx.f0.f64) return;
	// bsolr cr6
	// UNIMPLEMENTED: bsolr
	PPC_UNIMPLEMENTED(0x8245AC48, "bsolr");
	// lwz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// stw r8,-16(r1)
	PPC_STORE_U32(ctx.r1.u32 + -16, ctx.r8.u32);
	// lfs f13,-16(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f1,f13
	// bgtlr cr6
	if (ctx.f1.f64 > ctx.f13.f64) return;
	// bsolr cr6
	// UNIMPLEMENTED: bsolr
	PPC_UNIMPLEMENTED(0x8245AC60, "bsolr");
	// li r3,1
	ctx.r3.s64 = 1;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_AC70_fw"))) PPC_WEAK_FUNC(atSingleton_AC70_fw);
PPC_FUNC_IMPL(__imp__atSingleton_AC70_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r27,r4
	var_r27 = ctx.r4.u32;
	// mr r26,r5
	var_r26 = ctx.r5.u32;
	// li r29,0
	var_r29 = 0;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,8(r28)
	ctx.r10.u64 = PPC_LOAD_U32(var_r28 + 8);
	// addi r30,r10,8
	var_r30 = (uint32_t)(ctx.r10.s64 + 8);  // addr:0x82000008
loc_8245AC98:
	// clrlwi r31,r11,16
	var_r31 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
	// lwz r11,8(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r8,r10,16
	ctx.r8.u64 = ctx.r10.u32 & 0xFFFF;
	// cmplw cr6,r31,r8
	// bge cr6,0x8245ad04
	if (var_r31 >= ctx.r8.u32) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
	// rlwinm r11,r31,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// add r11,r31,r11
	ctx.r11.u64 = var_r31 + ctx.r11.u64;
	// li r9,5
	ctx.r9.s64 = 5;
	// add r11,r11,r30
	ctx.r11.u64 = ctx.r11.u64 + var_r30;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_8245ACC8:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stb r7,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r7.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x8245acc8
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245ACC8;
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// lbz r4,82(r1)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 82);
	// bl 0x82450090
	atSingleton_0090_g(ctx, base);
	// cmplw cr6,r3,r26
	// bne cr6,0x8245acf4
	if (ctx.r3.u32 == var_r26) {
		// li r29,1
		var_r29 = 1;
	}
loc_8245ACF4:
	// addi r6,r31,1
	ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 1;
	// cmpwi cr6,r29,0
	// clrlwi r11,r6,16
	ctx.r11.u64 = ctx.r6.u32 & 0xFFFF;
	// beq cr6,0x8245ac98
	if ((int32_t)var_r29 == 0) goto loc_8245AC98;
loc_8245AD04:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__atSingleton_AD10_fw"))) PPC_WEAK_FUNC(atSingleton_AD10_fw);
PPC_FUNC_IMPL(__imp__atSingleton_AD10_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// li r3,0
	ctx.r3.s64 = 0;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,8(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 8);
	// addi r29,r10,8
	var_r29 = (uint32_t)(ctx.r10.s64 + 8);  // addr:0x82000008
loc_8245AD38:
	// clrlwi r31,r11,16
	var_r31 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r8,r10,16
	ctx.r8.u64 = ctx.r10.u32 & 0xFFFF;
	// cmplw cr6,r31,r8
	// bge cr6,0x8245ada4
	if (var_r31 >= ctx.r8.u32) {
		return;
	}
	// rlwinm r11,r31,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 1) & 0xFFFFFFFE;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// add r7,r31,r11
	ctx.r7.u64 = var_r31 + ctx.r11.u64;
	// li r9,6
	ctx.r9.s64 = 6;
	// rlwinm r11,r7,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r29
	ctx.r11.u64 = ctx.r11.u64 + var_r29;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_8245AD6C:
	// lbz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stb r6,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r6.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x8245ad6c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245AD6C;
	// mr r5,r27
	ctx.r5.u64 = var_r27;
	// lwz r6,80(r1)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245a8d0
	atSingleton_A8D0(ctx, base);
	// addi r5,r31,1
	ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 1;
	// cmpwi cr6,r3,0
	// clrlwi r11,r5,16
	ctx.r11.u64 = ctx.r5.u32 & 0xFFFF;
	// beq cr6,0x8245ad38
	if (ctx.r3.s32 == 0) goto loc_8245AD38;
loc_8245ADA4:
	return;
}

__attribute__((alias("__imp__atSingleton_ADB0_fw"))) PPC_WEAK_FUNC(atSingleton_ADB0_fw);
PPC_FUNC_IMPL(__imp__atSingleton_ADB0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// li r3,0
	ctx.r3.s64 = 0;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,8(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 8);
	// addi r29,r10,8
	var_r29 = (uint32_t)(ctx.r10.s64 + 8);  // addr:0x82000008
loc_8245ADD8:
	// clrlwi r31,r11,16
	var_r31 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r8,r10,16
	ctx.r8.u64 = ctx.r10.u32 & 0xFFFF;
	// cmplw cr6,r31,r8
	// bge cr6,0x8245ae3c
	if (var_r31 >= ctx.r8.u32) {
		return;
	}
	// rlwinm r11,r31,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 4) & 0xFFFFFFF0;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// add r11,r11,r29
	ctx.r11.u64 = ctx.r11.u64 + var_r29;
	// li r9,16
	ctx.r9.s64 = 16;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_8245AE04:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stb r7,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r7.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x8245ae04
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245AE04;
	// mr r5,r27
	ctx.r5.u64 = var_r27;
	// lwz r6,80(r1)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245a8d0
	atSingleton_A8D0(ctx, base);
	// addi r6,r31,1
	ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 1;
	// cmpwi cr6,r3,0
	// clrlwi r11,r6,16
	ctx.r11.u64 = ctx.r6.u32 & 0xFFFF;
	// beq cr6,0x8245add8
	if (ctx.r3.s32 == 0) goto loc_8245ADD8;
loc_8245AE3C:
	return;
}

__attribute__((alias("__imp__atSingleton_AE48_p39"))) PPC_WEAK_FUNC(atSingleton_AE48_p39);
PPC_FUNC_IMPL(__imp__atSingleton_AE48_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r4,1
	ctx.r4.s64 = 1;
	// li r30,0
	var_r30 = 0;
	// lwz r5,12(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 12);
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// bl 0x82447c78
	jumptable_7C78(ctx, base);
	// lwz r9,8(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lis r11,0
	ctx.r11.s64 = 0;
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// ori r8,r11,65535
	ctx.r8.u64 = ctx.r11.u64 | 65535;
	// lwz r10,0(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// add r6,r10,r8
	ctx.r6.u64 = ctx.r10.u64 + ctx.r8.u64;
	// clrlwi r3,r6,16
	ctx.r3.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245af10
	if (ctx.r11.u32 != 0) {
		// rlwinm r6,r10,13,29,31
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 13) & 0x7;
	loc_8245AEA0:
		// cmplwi cr6,r6,1
		// blt cr6,0x8245aed8
		if (ctx.r6.u32 >= 1) {
			// rlwinm r10,r11,1,0,30
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// beq cr6,0x8245aec4
			if (!(ctx.cr6.eq)) {
				// add r5,r11,r10
				ctx.r5.u64 = ctx.r11.u64 + ctx.r10.u64;
				// rlwinm r10,r5,1,0,30
				ctx.r10.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 1) & 0xFFFFFFFE;
				// add r4,r10,r9
				ctx.r4.u64 = ctx.r10.u64 + ctx.r9.u64;
				// lbz r10,13(r4)
				ctx.r10.u64 = PPC_LOAD_U8(ctx.r4.u32 + 13);
				// b 0x8245aee8
				goto loc_8245AEE8;
			}
		loc_8245AEC4:
			// add r10,r11,r10
			ctx.r10.u64 = ctx.r11.u64 + ctx.r10.u64;
			// rlwinm r10,r10,1,0,30
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r5,r10,r9
			ctx.r5.u64 = ctx.r10.u64 + ctx.r9.u64;
			// lbz r10,13(r5)
			ctx.r10.u64 = PPC_LOAD_U8(ctx.r5.u32 + 13);
			// b 0x8245aee8
		} else {
		loc_8245AED8:
			// rlwinm r10,r11,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r11,r10
			ctx.r10.u64 = ctx.r11.u64 + ctx.r10.u64;
			// add r4,r10,r9
			ctx.r4.u64 = ctx.r10.u64 + ctx.r9.u64;
			// lbz r10,12(r4)
			ctx.r10.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
		}
	loc_8245AEE8:
		// clrlwi r10,r10,24
		ctx.r10.u64 = ctx.r10.u32 & 0xFF;
		// add r10,r10,r30
		ctx.r10.u64 = ctx.r10.u64 + var_r30;
		// addi r30,r10,1
		var_r30 = (uint32_t)(ctx.r10.s64 + 1);  // addr:0x82000001
		// cmplw cr6,r7,r30
		// ble cr6,0x8245af10
		if (ctx.r7.u32 <= var_r30) {
			// blr
			return;
		}
		// add r3,r11,r8
		ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
		// clrlwi r3,r3,16
		ctx.r3.u64 = ctx.r3.u32 & 0xFFFF;
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// cmplwi cr6,r11,0
		// bne cr6,0x8245aea0
		if (ctx.r11.u32 != 0) goto loc_8245AEA0;
	}
loc_8245AF10:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_AF28_p39"))) PPC_WEAK_FUNC(atSingleton_AF28_p39);
PPC_FUNC_IMPL(__imp__atSingleton_AF28_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_25
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// lwz r11,8(r26)
	ctx.r11.u64 = PPC_LOAD_U32(var_r26 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r8,r10,16
	ctx.r8.u64 = ctx.r10.u32 & 0xFFFF;
	// cmplwi cr6,r8,2
	// ble cr6,0x8245b0e0
	if (ctx.r8.u32 <= 2) {
		return;
	}
	// rotlwi r10,r11,0
	ctx.r10.u64 = ctx.r11.u32;
	// addi r27,r10,8
	var_r27 = (uint32_t)(ctx.r10.s64 + 8);  // addr:0x82000008
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// addis r7,r9,1
	ctx.r7.s64 = ctx.r9.s64 + 65536;
	// rlwinm r10,r9,13,29,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 13) & 0x7;
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// cmplwi cr6,r10,3
	// clrlwi r25,r7,16
	var_r25 = (uint32_t)(ctx.r7.u32 & 0xFFFF);
	// bgt cr6,0x8245afb0
	if (ctx.r10.u32 > 3) goto loc_8245AFB0;
	// lis r12,-32186
	// addi r12,r12,-20600
	ctx.r12.s64 = ctx.r12.s64 + -20600;
	// rlwinm r0,r10,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (ctx.r10.u64) {
	case 0:
		goto loc_8245AF98;
	case 1:
		goto loc_8245AFA0;
	case 2:
		goto loc_8245AFA0;
	case 3:
		goto loc_8245AFA8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_8245AF98:
	// li r31,5
	var_r31 = 5;
	// b 0x8245afb4
	goto loc_8245AFB4;
loc_8245AFA0:
	// li r31,6
	var_r31 = 6;
	// b 0x8245afb4
	goto loc_8245AFB4;
loc_8245AFA8:
	// li r31,16
	var_r31 = 16;
	// b 0x8245afb4
	goto loc_8245AFB4;
loc_8245AFB0:
	// li r31,3
	var_r31 = 3;
loc_8245AFB4:
	// lwz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// li r30,0
	var_r30 = 0;
	// clrlwi r5,r6,16
	ctx.r5.u64 = ctx.r6.u32 & 0xFFFF;
	// cmplwi cr6,r5,0
	// beq cr6,0x8245b074
	if (ctx.r5.u32 == 0) goto loc_8245B074;
	// li r28,0
	var_r28 = 0;
loc_8245AFCC:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,4(r26)
	ctx.r3.u64 = PPC_LOAD_U32(var_r26 + 4);
	// clrlwi r11,r11,16
	ctx.r11.u64 = ctx.r11.u32 & 0xFFFF;
	// addi r5,r11,-1
	ctx.r5.s64 = ctx.r11.s64 + -1;
	// bl 0x82447c78
	jumptable_7C78(ctx, base);
	// clrlwi r10,r3,16
	ctx.r10.u64 = ctx.r3.u32 & 0xFFFF;
	// clrlwi r11,r25,16
	ctx.r11.u64 = var_r25 & 0xFFFF;
	// mr r29,r10
	var_r29 = ctx.r10.u32;
	// cmplw cr6,r29,r11
	// bne cr6,0x8245b000
	if (var_r29 != ctx.r11.u32) goto loc_8245B000;
	// mr r25,r30
	var_r25 = (uint32_t)(var_r30);
	// b 0x8245b00c
	goto loc_8245B00C;
loc_8245B000:
	// cmplw cr6,r28,r11
	// bne cr6,0x8245b00c
	if (var_r28 != ctx.r11.u32) goto loc_8245B00C;
	// mr r25,r10
	var_r25 = ctx.r10.u32;
loc_8245B00C:
	// cmplw cr6,r28,r29
	// beq cr6,0x8245b054
	if (var_r28 == var_r29) goto loc_8245B054;
	// mullw r11,r28,r31
	ctx.r11.s64 = int64_t((int32_t)var_r28) * int64_t((int32_t)var_r31);
	// add r30,r11,r27
	var_r30 = (uint32_t)(ctx.r11.u64 + var_r27);
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x82434100
	memcpy(ctx, base);
	// mullw r11,r29,r31
	ctx.r11.s64 = int64_t((int32_t)var_r29) * int64_t((int32_t)var_r31);
	// add r29,r11,r27
	var_r29 = (uint32_t)(ctx.r11.u64 + var_r27);
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x82434100
	memcpy(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82434100
	memcpy(ctx, base);
loc_8245B054:
	// lwz r11,8(r26)
	ctx.r11.u64 = PPC_LOAD_U32(var_r26 + 8);
	// addi r9,r28,1
	ctx.r9.s64 = (int64_t)(int32_t)var_r28 + 1;
	// clrlwi r30,r9,16
	var_r30 = (uint32_t)(ctx.r9.u32 & 0xFFFF);
	// mr r28,r30
	var_r28 = (uint32_t)(var_r30);
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r7,r8,16
	ctx.r7.u64 = ctx.r8.u32 & 0xFFFF;
	// cmplw cr6,r28,r7
	// blt cr6,0x8245afcc
	if (var_r28 < ctx.r7.u32) goto loc_8245AFCC;
loc_8245B074:
	// clrlwi r6,r25,16
	ctx.r6.u64 = var_r25 & 0xFFFF;
	// cmplwi cr6,r6,0
	// bne cr6,0x8245b0e0
	if (ctx.r6.u32 != 0) {
		return;
	}
	// lwz r5,8(r26)
	ctx.r5.u64 = PPC_LOAD_U32(var_r26 + 8);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,4(r26)
	ctx.r3.u64 = PPC_LOAD_U32(var_r26 + 4);
	// lwz r11,0(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// clrlwi r11,r11,16
	ctx.r11.u64 = ctx.r11.u32 & 0xFFFF;
	// addi r5,r11,-2
	ctx.r5.s64 = ctx.r11.s64 + -2;
	// bl 0x82447c78
	jumptable_7C78(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// mr r4,r27
	ctx.r4.u64 = var_r27;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// addi r30,r11,1
	var_r30 = (uint32_t)(ctx.r11.s64 + 1);  // addr:0x82000001
	// bl 0x82434100
	memcpy(ctx, base);
	// clrlwi r8,r30,16
	ctx.r8.u64 = var_r30 & 0xFFFF;
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// mullw r11,r8,r31
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t((int32_t)var_r31);
	// add r30,r11,r27
	var_r30 = (uint32_t)(ctx.r11.u64 + var_r27);
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x82434100
	memcpy(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82434100
	memcpy(ctx, base);
loc_8245B0E0:
	return;
}

__attribute__((alias("__imp__atSingleton_B0E8_sp"))) PPC_WEAK_FUNC(atSingleton_B0E8_sp);
PPC_FUNC_IMPL(__imp__atSingleton_B0E8_sp) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r11,r10,13,29,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 13) & 0x7;
	// cmpwi cr6,r11,0
	// bne cr6,0x8245b100
	if (ctx.r11.s32 == 0) {
		// b 0x8245ac70
		atSingleton_AC70_fw(ctx, base);
		return;
	}
loc_8245B100:
	// cmpwi cr6,r11,1
	// bne cr6,0x8245b10c
	if (ctx.r11.s32 == 1) {
		// b 0x8245ad10
		atSingleton_AD10_fw(ctx, base);
		return;
	}
loc_8245B10C:
	// b 0x8245adb0
	atSingleton_ADB0_fw(ctx, base);
	return;
}

__attribute__((alias("__imp__msgMsgSink_B110_g"))) PPC_WEAK_FUNC(msgMsgSink_B110_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_B110_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// stw r4,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4,/* msgMsgSink::flags@+0x4 */ ctx.r4.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r5,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r5.u32);
	// addi r11,r11,21688
	ctx.r11.s64 = ctx.r11.s64 + 21688;
	// lis r10,24
	ctx.r10.s64 = 1572864;
	// stw r9,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// lwz r11,0(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// rlwinm r8,r11,0,10,12
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x380000;
	// cmplw cr6,r8,r10
	// beqlr cr6
	if (ctx.r8.u32 == ctx.r10.u32) return;
	// rlwinm r10,r11,16,29,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0x7;
	// cmpwi cr6,r10,2
	// beq cr6,0x8245b154
	if (ctx.r10.s32 != 2) {
		// cmpwi cr6,r10,3
		// bnelr cr6
		if (ctx.r10.s32 != 3) return;
	}
loc_8245B154:
	// clrlwi r7,r11,16
	ctx.r7.u64 = ctx.r11.u32 & 0xFFFF;
	// cmplwi cr6,r7,0
	// beqlr cr6
	if (ctx.r7.u32 == 0) return;
	// addi r8,r5,8
	ctx.r8.s64 = ctx.r5.s64 + 8;
	// li r11,0
	ctx.r11.s64 = 0;
loc_8245B168:
	// lwz r6,0(r5)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// rlwinm r10,r6,13,29,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 13) & 0x7;
	// cmplwi cr6,r10,1
	// blt cr6,0x8245b1a8
	if (ctx.r10.u32 >= 1) {
		// rlwinm r10,r11,1,0,30
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// beq cr6,0x8245b194
		if (!(ctx.cr6.eq)) {
			// add r4,r11,r10
			ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
			// rlwinm r10,r4,1,0,30
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 1) & 0xFFFFFFFE;
			// add r10,r10,r8
			ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
			// lbz r10,5(r10)
			ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + 5);
			// b 0x8245b1b8
			goto loc_8245B1B8;
		}
	loc_8245B194:
		// add r9,r11,r10
		ctx.r9.u64 = ctx.r11.u64 + ctx.r10.u64;
		// rlwinm r10,r9,1,0,30
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 1) & 0xFFFFFFFE;
		// add r7,r10,r8
		ctx.r7.u64 = ctx.r10.u64 + ctx.r8.u64;
		// lbz r10,5(r7)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r7.u32 + 5);
		// b 0x8245b1b8
	} else {
	loc_8245B1A8:
		// rlwinm r10,r11,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r11,r10
		ctx.r10.u64 = ctx.r11.u64 + ctx.r10.u64;
		// add r6,r10,r8
		ctx.r6.u64 = ctx.r10.u64 + ctx.r8.u64;
		// lbz r10,4(r6)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r6.u32 + 4);
	}
loc_8245B1B8:
	// lwz r9,12(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// addi r4,r11,1
	ctx.r4.s64 = ctx.r11.s64 + 1;
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// clrlwi r11,r4,16
	ctx.r11.u64 = ctx.r4.u32 & 0xFFFF;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// clrlwi r7,r9,16
	ctx.r7.u64 = ctx.r9.u32 & 0xFFFF;
	// cmplw cr6,r11,r7
	// blt cr6,0x8245b168
	if (ctx.r11.u32 < ctx.r7.u32) goto loc_8245B168;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_B1F0_w"))) PPC_WEAK_FUNC(msgMsgSink_B1F0_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_B1F0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r31,-1
	var_r31 = (uint32_t)(-1);
	// lwz r10,8(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 8);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// rlwinm r9,r11,16,29,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0x7;
	// cmplwi cr6,r9,4
	// bgt cr6,0x8245b348
	if (ctx.r9.u32 > 4) {
		// lwz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// sth r31,4(r4)
		PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
		// blr
		return;
	}
	// lis r12,-32186
	// addi r12,r12,-19912
	ctx.r12.s64 = ctx.r12.s64 + -19912;
	// rlwinm r0,r9,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (ctx.r9.u64) {
	case 0:
		goto loc_8245B24C;
	case 1:
		goto loc_8245B280;
	case 2:
		goto loc_8245B2D0;
	case 3:
		goto loc_8245B2E0;
	case 4:
		goto loc_8245B304;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_8245B24C:
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lhz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 4);
	// clrlwi r11,r9,16
	ctx.r11.u64 = ctx.r9.u32 & 0xFFFF;
	// addi r6,r11,-1
	ctx.r6.s64 = ctx.r11.s64 + -1;
	// cmpw cr6,r8,r6
	// bge cr6,0x8245b278
	if (ctx.r8.s32 >= ctx.r6.s32) {
		// li r31,0
		var_r31 = 0;
		// b 0x8245b348
		// lwz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// sth r31,4(r4)
		PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
		// blr
		return;
	}
	// lhz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r10.u32 + 4);
	// addi r5,r11,1
	ctx.r5.s64 = ctx.r11.s64 + 1;
	// clrlwi r31,r5,16
	var_r31 = (uint32_t)(ctx.r5.u32 & 0xFFFF);
	// b 0x8245b348
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
loc_8245B278:
	// li r31,0
	var_r31 = 0;
	// b 0x8245b348
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
loc_8245B280:
	// lhz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r10.u32 + 4);
	// cmplwi cr6,r10,65535
	// beq cr6,0x8245b2b4
	if (ctx.r10.u32 == 65535) goto loc_8245B2B4;
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lhz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U16(ctx.r11.u32 + 4);
	// clrlwi r11,r4,16
	ctx.r11.u64 = ctx.r4.u32 & 0xFFFF;
	// addi r9,r11,-1
	ctx.r9.s64 = ctx.r11.s64 + -1;
	// cmpw cr6,r3,r9
	// bge cr6,0x8245b278
	if (ctx.r3.s32 >= ctx.r9.s32) {
		// li r31,0
		var_r31 = 0;
		// b 0x8245b348
		// lwz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// sth r31,4(r4)
		PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
		// blr
		return;
	}
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// clrlwi r31,r8,16
	var_r31 = (uint32_t)(ctx.r8.u32 & 0xFFFF);
	// b 0x8245b348
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
loc_8245B2B4:
	// clrlwi r11,r11,16
	ctx.r11.u64 = ctx.r11.u32 & 0xFFFF;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4)/* msgMsgSink::flags@+0x4 */;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,-1
	ctx.r5.s64 = ctx.r11.s64 + -1;
	// bl 0x82447c78
	jumptable_7C78(ctx, base);
	// clrlwi r31,r3,16
	var_r31 = (uint32_t)(ctx.r3.u32 & 0xFFFF);
	// b 0x8245b348
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
loc_8245B2D0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245ae48
	atSingleton_AE48_p39(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// b 0x8245b348
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
loc_8245B2E0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245ae48
	atSingleton_AE48_p39(ctx, base);
	// lwz r6,8(r30)
	ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// clrlwi r4,r31,16
	ctx.r4.u64 = var_r31 & 0xFFFF;
	// lhz r5,4(r6)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r6.u32 + 4);
	// cmplw cr6,r4,r5
	// beq cr6,0x8245b2e0
	if (ctx.r4.u32 == ctx.r5.u32) goto loc_8245B2E0;
	// b 0x8245b348
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
loc_8245B304:
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lhz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 4);
	// clrlwi r11,r3,16
	ctx.r11.u64 = ctx.r3.u32 & 0xFFFF;
	// addi r7,r11,-1
	ctx.r7.s64 = ctx.r11.s64 + -1;
	// cmpw cr6,r9,r7
	// bge cr6,0x8245b330
	if (ctx.r9.s32 >= ctx.r7.s32) goto loc_8245B330;
	// lhz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r10.u32 + 4);
	// addi r6,r11,1
	ctx.r6.s64 = ctx.r11.s64 + 1;
	// clrlwi r31,r6,16
	var_r31 = (uint32_t)(ctx.r6.u32 & 0xFFFF);
	// b 0x8245b334
	goto loc_8245B334;
loc_8245B330:
	// li r31,0
	var_r31 = 0;
loc_8245B334:
	// clrlwi r5,r31,16
	ctx.r5.u64 = var_r31 & 0xFFFF;
	// cmplwi cr6,r5,0
	// bne cr6,0x8245b348
	if (ctx.r5.u32 != 0) {
		// lwz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// sth r31,4(r4)
		PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
		// blr
		return;
	}
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8245af28
	atSingleton_AF28_p39(ctx, base);
loc_8245B348:
	// lwz r4,8(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r31,4(r4)
	PPC_STORE_U16(ctx.r4.u32 + 4, (uint16_t)var_r31);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_B370_2hr"))) PPC_WEAK_FUNC(msgMsgSink_B370_2hr);
PPC_FUNC_IMPL(__imp__msgMsgSink_B370_2hr) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// bl 0x8245b1f0
	msgMsgSink_B1F0_w(ctx, base);
	// clrlwi r11,r3,16
	ctx.r11.u64 = ctx.r3.u32 & 0xFFFF;
	// cmpwi cr6,r11,-1
	// beq cr6,0x8245b3cc
	if (ctx.r11.s32 != -1) {
		// rlwinm r9,r11,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// li r3,0
		ctx.r3.s64 = 0;
		// add r11,r11,r9
		ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
		// add r10,r10,r11
		ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lhz r9,8(r10)
		ctx.r9.u64 = PPC_LOAD_U16(ctx.r10.u32 + 8);
		// sth r9,0(r30)
		PPC_STORE_U16(var_r30 + 0, ctx.r9.u16);
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lbz r7,10(r8)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r8.u32 + 10);
		// stb r7,0(r29)
		PPC_STORE_U8(var_r29 + 0, ctx.r7.u8);
		return;
	}
loc_8245B3CC:
	// lis r3,-30009
	// ori r3,r3,20
	ctx.r3.u64 = ctx.r3.u64 | 20;
	return;
}

__attribute__((alias("__imp__msgMsgSink_B3E0_wrh"))) PPC_WEAK_FUNC(msgMsgSink_B3E0_wrh);
PPC_FUNC_IMPL(__imp__msgMsgSink_B3E0_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x8245b1f0
	msgMsgSink_B1F0_w(ctx, base);
	// clrlwi r11,r3,16
	ctx.r11.u64 = ctx.r3.u32 & 0xFFFF;
	// cmpwi cr6,r11,-1
	// beq cr6,0x8245b430
	if (ctx.r11.s32 != -1) {
		// rlwinm r9,r11,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// li r3,0
		ctx.r3.s64 = 0;
		// add r11,r11,r9
		ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
		// rlwinm r11,r11,1,0,30
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r11,r10
		ctx.r10.u64 = ctx.r11.u64 + ctx.r10.u64;
		// lwz r9,8(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		// stw r9,0(r30)
		PPC_STORE_U32(var_r30 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r9.u32);
		// b 0x8245b438
	} else {
	loc_8245B430:
		// lis r3,-30009
		// ori r3,r3,20
		ctx.r3.u64 = ctx.r3.u64 | 20;
	}
loc_8245B438:
	// blr
	return;
}

__attribute__((alias("__imp__game_B450_h"))) PPC_WEAK_FUNC(game_B450_h);
PPC_FUNC_IMPL(__imp__game_B450_h) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// stw r4,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r4.u32);
	// sth r5,8(r3)
	PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r5.u16);
	// addi r11,r11,21692
	ctx.r11.s64 = ctx.r11.s64 + 21692;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_7_B468_1"))) PPC_WEAK_FUNC(atSingleton_vfn_7_B468_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_7_B468_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,21692
	ctx.r11.s64 = ctx.r11.s64 + 21692;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x8245b4a4
	if (ctx.r10.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32813
		ctx.r4.u64 = ctx.r4.u64 | 32813;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245B4A4:
	// blr
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_B4B8_2hr"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_B4B8_2hr);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_B4B8_2hr) {
	PPC_FUNC_PROLOGUE();
	// clrlwi r8,r4,16
	ctx.r8.u64 = ctx.r4.u32 & 0xFFFF;
	// lhz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,65535
	// bne cr6,0x8245b4d0
	if (ctx.r8.u32 == 65535) {
		// mullw r11,r11,r11
		ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r11.s32);
		// b 0x8245b4d4
	} else {
	loc_8245B4D0:
		// mullw r11,r11,r8
		ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r8.s32);
	}
loc_8245B4D4:
	// clrlwi r9,r5,16
	ctx.r9.u64 = ctx.r5.u32 & 0xFFFF;
	// clrlwi r7,r11,16
	ctx.r7.u64 = ctx.r11.u32 & 0xFFFF;
	// cmplwi cr6,r9,65535
	// bne cr6,0x8245b4f0
	if (ctx.r9.u32 == 65535) {
		// lhz r11,8(r3)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 8);
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// b 0x8245b4f4
	} else {
	loc_8245B4F0:
		// mr r11,r9
		ctx.r11.u64 = ctx.r9.u64;
	}
loc_8245B4F4:
	// clrlwi r11,r11,16
	ctx.r11.u64 = ctx.r11.u32 & 0xFFFF;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// cmplw cr6,r10,r8
	// ble cr6,0x8245b518
	if (ctx.r10.u32 > ctx.r8.u32) {
		// cmplwi cr6,r9,65535
		// beq cr6,0x8245b518
		if (ctx.r9.u32 == 65535) {
			// clrlwi r11,r11,16
			ctx.r11.u64 = ctx.r11.u32 & 0xFFFF;
			// lwz r10,4(r3)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
			// clrlwi r9,r7,16
			ctx.r9.u64 = ctx.r7.u32 & 0xFFFF;
			// add r9,r11,r9
			ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
			// clrlwi r7,r9,16
			ctx.r7.u64 = ctx.r9.u32 & 0xFFFF;
			// mulli r11,r7,26
			ctx.r11.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(26));
			// add r11,r11,r10
			ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
			// addi r3,r11,4
			ctx.r3.s64 = ctx.r11.s64 + 4;
			// blr
			return;
		}
		// addis r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 65536;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// clrlwi r11,r10,16
		ctx.r11.u64 = ctx.r10.u32 & 0xFFFF;
	}
loc_8245B518:
	// clrlwi r11,r11,16
	ctx.r11.u64 = ctx.r11.u32 & 0xFFFF;
	// lwz r10,4(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// clrlwi r9,r7,16
	ctx.r9.u64 = ctx.r7.u32 & 0xFFFF;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// clrlwi r7,r9,16
	ctx.r7.u64 = ctx.r9.u32 & 0xFFFF;
	// mulli r11,r7,26
	ctx.r11.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(26));
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// blr
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_B540_2h"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_B540_2h);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_B540_2h) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// bl 0x8245b4b8
	RtlEnterCriticalSection_B4B8_2hr(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r11,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_B570_fw"))) PPC_WEAK_FUNC(msgMsgSink_B570_fw);
PPC_FUNC_IMPL(__imp__msgMsgSink_B570_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8244a970
	msgMsgSink_A970_2h(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r10,r11,21696
	ctx.r10.s64 = ctx.r11.s64 + 21696;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r9,r11,15912
	ctx.r9.s64 = ctx.r11.s64 + 15912;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r10.u32);
	// stw r9,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r9.u32);
	// stw r11,68(r31)
	PPC_STORE_U32(var_r31 + 68, ctx.r11.u32);
	// stw r11,72(r31)
	PPC_STORE_U32(var_r31 + 72, ctx.r11.u32);
	// stw r11,76(r31)
	PPC_STORE_U32(var_r31 + 76, ctx.r11.u32);
	// stw r11,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r11.u32);
	// stw r11,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r11.u32);
	// stw r11,88(r31)
	PPC_STORE_U32(var_r31 + 88, ctx.r11.u32);
	// stw r31,68(r31)
	PPC_STORE_U32(var_r31 + 68, var_r31);
	// stw r31,80(r31)
	PPC_STORE_U32(var_r31 + 80, var_r31);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_8_B5E0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_8_B5E0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_8_B5E0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// lis r10,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,21696
	ctx.r11.s64 = ctx.r11.s64 + 21696;
	// addi r10,r10,15912
	ctx.r10.s64 = ctx.r10.s64 + 15912;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r10.u32);
	// bl 0x8244af18
	atSingleton_AF18_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245b638
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32794
		ctx.r4.u64 = ctx.r4.u64 | 32794;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245B638:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_14"))) PPC_WEAK_FUNC(atSingleton_vfn_14);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8244acd8
	ph_vt3DB0_37_ACD8(ctx, base);
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 24);
	// rlwinm r10,r11,0,27,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r10,16
	// bne cr6,0x8245b6a4
	if (ctx.r10.u32 == 16) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,0(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// blr
		return;
	}
loc_8245B6A4:
	// lwz r4,36(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 36);
	// cmplwi cr6,r4,0
	// beq cr6,0x8245b6e0
	if (ctx.r4.u32 != 0) {
		// lwz r7,32(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 32);
		// cmpwi cr6,r7,5
		// bne cr6,0x8245b6e0
		if (ctx.r7.s32 != 5) {
			// blr
			return;
		}
		// lwz r6,20(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r3,12(r6)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 12);
		// lwz r11,12(r5)
		// bctrl
		VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
		// li r4,0
		ctx.r4.s64 = 0;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244a4f8
		atSingleton_A4F8_g(ctx, base);
	}
loc_8245B6E0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_10_B6F8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_10_B6F8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_10_B6F8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,40(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245b758
	if (ctx.r3.u32 != 0) {
		// lbz r11,24(r31)
		ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 24);
		// rlwinm r10,r11,0,26,26
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r10,32
		// bne cr6,0x8245b744
		if (ctx.r10.u32 == 32) {
			// lbz r9,56(r31)
			ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 56);
			// cmplwi cr6,r9,0
			// beq cr6,0x8245b744
			if (ctx.r9.u32 == 0) goto loc_8245B744;
			// lwz r7,96(r8)
			// bctrl
			VCALL(ctx.r3.u32, 24, ctx, base);  // vtable slot 24 (byte +96)
		}
	loc_8245B744:
		// lwz r3,40(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 40);
		// lwz r5,92(r6)
		// bctrl
		VCALL(ctx.r3.u32, 23, ctx, base);  // vtable slot 23 (byte +92)
	}
loc_8245B758:
	// lbz r4,24(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 24);
	// rlwinm r3,r4,0,0,24
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r3,128
	// beq cr6,0x8245b790
	if (ctx.r3.u32 != 128) {
		// lwz r3,36(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
		// lwz r10,16(r11)
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
		// cmpwi cr6,r3,0
		// blt cr6,0x8245b7bc
		if (ctx.r3.s32 < 0) {
			// blr
			return;
		}
		// lbz r9,24(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 24);
		// ori r8,r9,2
		ctx.r8.u64 = ctx.r9.u64 | 2;
		// stb r8,24(r31)
		PPC_STORE_U8(var_r31 + 24, ctx.r8.u8);
	}
loc_8245B790:
	// li r4,3
	ctx.r4.s64 = 3;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// lbz r7,24(r31)
	ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 24);
	// lwz r3,36(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
	// ori r6,r7,64
	ctx.r6.u64 = ctx.r7.u64 | 64;
	// stb r6,24(r31)
	PPC_STORE_U8(var_r31 + 24, ctx.r6.u8);
	// lwz r4,24(r5)
	// bctrl
	VCALL(ctx.r3.u32, 6, ctx, base);  // vtable slot 6 (byte +24)
loc_8245B7BC:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_11_B7D0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_11_B7D0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_11_B7D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// lwz r11,32(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 32);
	// cmpwi cr6,r11,5
	// beq cr6,0x8245b838
	if (ctx.r11.s32 == 5) goto loc_8245B838;
	// cmpwi cr6,r11,4
	// bne cr6,0x8245b814
	if (ctx.r11.s32 == 4) {
		// lwz r10,16(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
		// cmpwi cr6,r10,0
		// bne cr6,0x8245b838
		if (ctx.r10.s32 != 0) goto loc_8245B838;
		// cmpwi cr6,r4,0
		// bne cr6,0x8245b838
		if (ctx.r4.s32 != 0) goto loc_8245B838;
	}
loc_8245B814:
	// cmpwi cr6,r11,3
	// beq cr6,0x8245b854
	if (ctx.r11.s32 != 3) {
		// cmpwi cr6,r11,4
		// bne cr6,0x8245b82c
		if (ctx.r11.s32 == 4) {
			// cmpwi cr6,r4,0
			// beq cr6,0x8245b854
			if (ctx.r4.s32 == 0) goto loc_8245B854;
		}
	loc_8245B82C:
		// li r4,5
		ctx.r4.s64 = 5;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244aa58
		atSingleton_AA58_g(ctx, base);
	loc_8245B838:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	loc_8245B83C:
		// blr
		return;
	}
loc_8245B854:
	// cmplwi cr6,r4,1
	// blt cr6,0x8245b888
	if (ctx.r4.u32 < 1) goto loc_8245B888;
	// bne cr6,0x8245b838
	if (!ctx.cr6.eq) goto loc_8245B838;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// lwz r3,36(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
	// stb r30,56(r31)
	PPC_STORE_U8(var_r31 + 56, (uint8_t)var_r30);
	// lwz r8,48(r9)
	// bctrl
	VCALL(ctx.r3.u32, 12, ctx, base);  // vtable slot 12 (byte +48)
	// b 0x8245b83c
	// blr
	return;
loc_8245B888:
	// li r7,1
	ctx.r7.s64 = 1;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r7,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r7.u32);
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// lwz r3,36(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r5,20(r6)
	// bctrl
	VCALL(ctx.r3.u32, 5, ctx, base);  // vtable slot 5 (byte +20)
	// b 0x8245b83c
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_15_B8B8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_15_B8B8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_15_B8B8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=224, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// li r29,0
	var_r29 = 0;
	// lhz r30,44(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U16(var_r31 + 44));
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x82455c10
	atSingleton_5C10_g(ctx, base);
	// stw r29,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, var_r29);
	// addi r11,r1,100
	ctx.r11.s64 = ctx.r1.s64 + 100;
	// mr r9,r29
	ctx.r9.u64 = var_r29;
	// li r10,21
	ctx.r10.s64 = 21;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_8245B8F4:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x8245b8f4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245B8F4;
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// lwz r10,56(r11)
	// bctrl
	VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
	// cmpwi cr6,r3,0
	// blt cr6,0x8245b98c
	if (ctx.r3.s32 >= 0) {
		// lbz r9,56(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 56);
		// cmplwi cr6,r9,0
		// beq cr6,0x8245b98c
		if (ctx.r9.u32 == 0) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			return;
		}
		// lwz r9,80(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// clrlwi r8,r9,31
		ctx.r8.u64 = ctx.r9.u32 & 0x1;
		// cmplwi cr6,r8,1
		// bne cr6,0x8245b94c
		if (ctx.r8.u32 == 1) {
			// lwz r7,112(r1)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 112);
			// rlwinm r10,r7,29,3,31
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
			// b 0x8245b958
		} else {
		loc_8245B94C:
			// lwz r10,112(r1)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 112);
			// lwz r11,108(r1)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 108);
			// add r10,r11,r10
			ctx.r10.u64 = ctx.r11.u64 + ctx.r10.u64;
		}
	loc_8245B958:
		// rlwinm r6,r9,0,0,0
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x80000000;
		// li r11,1
		ctx.r11.s64 = 1;
		// cmplwi cr6,r6,0
		// beq cr6,0x8245b96c
		if (ctx.r6.u32 != 0) {
			// li r11,2
			ctx.r11.s64 = 2;
		}
	loc_8245B96C:
		// divwu r5,r10,r11
		ctx.r5.u32 = ctx.r11.u32 ? ctx.r10.u32 / ctx.r11.u32 : 0;
		// rlwinm r4,r9,28,5,31
		ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 28) & 0x7FFFFFF;
		// mulli r3,r5,1000
		ctx.r3.s64 = static_cast<int64_t>(ctx.r5.u64 * static_cast<uint64_t>(1000));
		// twllei r11,0
		if (ctx.r11.s32 == 0 || ctx.r11.u32 < 0u) __builtin_trap();
		// divwu r3,r3,r4
		ctx.r3.u32 = ctx.r4.u32 ? ctx.r3.u32 / ctx.r4.u32 : 0;
		// twllei r4,0
		if (ctx.r4.s32 == 0 || ctx.r4.u32 < 0u) __builtin_trap();
		return;
	}
loc_8245B98C:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_16_B998_1"))) PPC_WEAK_FUNC(atSingleton_vfn_16_B998_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_16_B998_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=224, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// li r29,0
	var_r29 = 0;
	// lhz r30,44(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U16(var_r31 + 44));
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x82455c10
	atSingleton_5C10_g(ctx, base);
	// stw r29,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, var_r29);
	// addi r11,r1,100
	ctx.r11.s64 = ctx.r1.s64 + 100;
	// mr r9,r29
	ctx.r9.u64 = var_r29;
	// li r10,21
	ctx.r10.s64 = 21;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_8245B9D4:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x8245b9d4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245B9D4;
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// lwz r10,56(r11)
	// bctrl
	VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
	// cmpwi cr6,r3,0
	// blt cr6,0x8245ba40
	if (ctx.r3.s32 >= 0) {
		// lwz r10,80(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// li r11,1
		ctx.r11.s64 = 1;
		// rlwinm r9,r10,0,0,0
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x80000000;
		// cmplwi cr6,r9,0
		// beq cr6,0x8245ba1c
		if (ctx.r9.u32 != 0) {
			// li r11,2
			ctx.r11.s64 = 2;
		}
	loc_8245BA1C:
		// lwz r8,100(r1)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
		// rlwinm r6,r10,28,5,31
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0x7FFFFFF;
		// twllei r11,0
		if (ctx.r11.s32 == 0 || ctx.r11.u32 < 0u) __builtin_trap();
		// divwu r7,r8,r11
		ctx.r7.u32 = ctx.r11.u32 ? ctx.r8.u32 / ctx.r11.u32 : 0;
		// twllei r6,0
		if (ctx.r6.s32 == 0 || ctx.r6.u32 < 0u) __builtin_trap();
		// mulli r5,r7,1000
		ctx.r5.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(1000));
		// divwu r3,r5,r6
		ctx.r3.u32 = ctx.r6.u32 ? ctx.r5.u32 / ctx.r6.u32 : 0;
		return;
	}
loc_8245BA40:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_9_BA50_1"))) PPC_WEAK_FUNC(atSingleton_vfn_9_BA50_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_9_BA50_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r26 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r21 = 0;
	uint32_t var_r20 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r27 = 0;
	PPCRegister temp{};
	// FRAME: size=288, savegprlr_20
	// li r26,0
	var_r26 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// mr r21,r26
	var_r21 = (uint32_t)(var_r26);
	// stw r26,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, var_r26);
	// addi r11,r1,100
	ctx.r11.s64 = ctx.r1.s64 + 100;
	// mr r9,r26
	ctx.r9.u64 = var_r26;
	// li r10,21
	ctx.r10.s64 = 21;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_8245BA88:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x8245ba88
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245BA88;
	// li r4,1
	ctx.r4.s64 = 1;
	// stw r26,64(r31)
	PPC_STORE_U32(var_r31 + 64, var_r26);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// li r20,1
	var_r20 = 1;
	// cmpwi cr6,r29,0
	// stb r30,56(r31)
	PPC_STORE_U8(var_r31 + 56, (uint8_t)var_r30);
	// mr r11,r20
	ctx.r11.u64 = var_r20;
	// bne cr6,0x8245babc
	if ((int32_t)var_r29 == 0) {
		// mr r11,r26
		ctx.r11.u64 = var_r26;
	}
loc_8245BABC:
	// lbz r10,24(r31)
	ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 24);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// rlwimi r10,r11,5,26,26
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 5) & 0x20) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFDF);
	// lhz r4,44(r31)
	ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
	// stb r10,24(r31)
	PPC_STORE_U8(var_r31 + 24, ctx.r10.u8);
	// bl 0x82455c10
	atSingleton_5C10_g(ctx, base);
	// lwz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245bb04
	if (ctx.r11.u32 != 0) {
		// lwz r7,44(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r6,16(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
		// lwz r5,20(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 20);
		// lwz r4,28(r5)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 28);
		// lhz r4,1(r4)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r4.u32 + 1);
		// bl 0x82448cd0
		atSingleton_8CD0_g(ctx, base);
		// mr r21,r3
		var_r21 = ctx.r3.u32;
	}
loc_8245BB04:
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// lhz r4,44(r31)
	ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
	// lwz r10,56(r11)
	// bctrl
	VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
	// cmpwi cr6,r3,0
	// blt cr6,0x8245bf28
	if (ctx.r3.s32 >= 0) {
		// lwz r9,80(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// mr r10,r20
		ctx.r10.u64 = var_r20;
		// rlwinm r8,r9,0,0,0
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x80000000;
		// cmplwi cr6,r8,0
		// beq cr6,0x8245bb40
		if (ctx.r8.u32 != 0) {
			// li r10,2
			ctx.r10.s64 = 2;
		}
	loc_8245BB40:
		// lis r6,4194
		ctx.r6.s64 = 274857984;
		// lwz r22,96(r1)
		var_r22 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 96));
		// rlwinm r7,r9,28,5,31
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 28) & 0x7FFFFFF;
		// lwz r23,100(r1)
		var_r23 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 100));
		// ori r5,r6,19923
		ctx.r5.u64 = ctx.r6.u64 | 19923;
		// rlwinm r11,r9,31,29,31
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7;
		// mulhwu r3,r7,r5
		ctx.r3.u64 = (uint64_t(ctx.r7.u32) * uint64_t(ctx.r5.u32)) >> 32;
		// clrlwi r4,r9,31
		ctx.r4.u64 = ctx.r9.u32 & 0x1;
		// rlwinm r9,r3,26,6,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 26) & 0x3FFFFFF;
		// mullw r5,r11,r10
		ctx.r5.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r10.s32);
		// mullw r8,r9,r11
		ctx.r8.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r11.s32);
		// mullw r7,r8,r10
		ctx.r7.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
		// mullw r6,r7,r28
		ctx.r6.s64 = int64_t(ctx.r7.s32) * int64_t((int32_t)var_r28);
		// cmplwi cr6,r4,1
		// divwu r4,r6,r5
		ctx.r4.u32 = ctx.r5.u32 ? ctx.r6.u32 / ctx.r5.u32 : 0;
		// twllei r5,0
		if (ctx.r5.s32 == 0 || ctx.r5.u32 < 0u) __builtin_trap();
		// mullw r3,r4,r11
		ctx.r3.s64 = int64_t(ctx.r4.s32) * int64_t(ctx.r11.s32);
		// lwz r11,108(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 108);
		// mullw r30,r3,r10
		var_r30 = (uint32_t)(int64_t(ctx.r3.s32) * int64_t(ctx.r10.s32));
		// lwz r10,112(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 112);
		// bne cr6,0x8245bba4
		if (ctx.r4.u32 == 1) {
			// rlwinm r28,r11,29,3,31
			var_r28 = (uint32_t)(__builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF);
			// rlwinm r24,r10,29,3,31
			var_r24 = (uint32_t)(__builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF);
			// subf r25,r28,r24
			var_r25 = var_r24 - var_r28;
			// b 0x8245bbb0
		} else {
		loc_8245BBA4:
			// mr r28,r11
			var_r28 = ctx.r11.u32;
			// mr r25,r10
			var_r25 = ctx.r10.u32;
			// add r24,r10,r11
			var_r24 = (uint32_t)(ctx.r10.u64 + ctx.r11.u64);
		}
	loc_8245BBB0:
		// lbz r9,24(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 24);
		// rlwinm r8,r9,0,26,26
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r8,32
		// beq cr6,0x8245bc0c
		if (ctx.r8.u32 != 32) {
			// lbz r27,56(r31)
			var_r27 = (uint32_t)(PPC_LOAD_U8(var_r31 + 56));
			// cmplwi cr6,r27,0
			// beq cr6,0x8245bc2c
			if (var_r27 == 0) goto loc_8245BC2C;
			// cmplwi cr6,r27,255
			// beq cr6,0x8245bc34
			if (var_r27 == 255) goto loc_8245BC34;
			// mullw r11,r27,r25
			ctx.r11.s64 = int64_t((int32_t)var_r27) * int64_t((int32_t)var_r25);
			// add r11,r11,r28
			ctx.r11.u64 = ctx.r11.u64 + var_r28;
			// cmplw cr6,r30,r28
			// ble cr6,0x8245bc14
			if (var_r30 <= var_r28) goto loc_8245BC14;
			// cmplw cr6,r30,r11
			// bge cr6,0x8245bc18
			if (var_r30 >= ctx.r11.u32) goto loc_8245BC18;
			// subf r11,r28,r30
			ctx.r11.s64 = (int64_t)(int32_t)var_r30 - (int64_t)(int32_t)var_r28;
			// twllei r25,0
			if ((int32_t)var_r25 == 0 || var_r25 < 0u) __builtin_trap();
			// divwu r7,r11,r25
			ctx.r7.u32 = var_r25 ? ctx.r11.u32 / var_r25 : 0;
			// divwu r10,r11,r25
			ctx.r10.u32 = var_r25 ? ctx.r11.u32 / var_r25 : 0;
			// mullw r6,r7,r25
			ctx.r6.s64 = int64_t(ctx.r7.s32) * int64_t((int32_t)var_r25);
			// subf r11,r6,r11
			ctx.r11.s64 = ctx.r11.s64 - ctx.r6.s64;
			// subf r27,r10,r27
			var_r27 = (uint32_t)((int64_t)(int32_t)var_r27 - ctx.r10.s64);
			// b 0x8245bc4c
		} else {
		loc_8245BC0C:
			// mr r27,r26
			var_r27 = (uint32_t)(var_r26);
			// b 0x8245bc70
			goto loc_8245BC70;
		loc_8245BC14:
			// cmplw cr6,r30,r11
			ctx.cr6.compare<uint32_t>(var_r30, ctx.r11.u32, ctx.xer);
		loc_8245BC18:
			// ble cr6,0x8245bc54
			if (!ctx.cr6.gt) goto loc_8245BC54;
			// subf r11,r11,r24
			ctx.r11.s64 = (int64_t)(int32_t)var_r24 - ctx.r11.s64;
			// mr r27,r26
			var_r27 = (uint32_t)(var_r26);
			// add r30,r11,r30
			var_r30 = (uint32_t)(ctx.r11.u64 + var_r30);
			// b 0x8245bc70
			goto loc_8245BC70;
		loc_8245BC2C:
			// cmplwi cr6,r27,255
			// bne cr6,0x8245bc54
			if (var_r27 != 255) goto loc_8245BC54;
		loc_8245BC34:
			// cmplw cr6,r30,r28
			// ble cr6,0x8245bc54
			if (var_r30 <= var_r28) goto loc_8245BC54;
			// subf r5,r28,r30
			ctx.r5.s64 = (int64_t)(int32_t)var_r30 - (int64_t)(int32_t)var_r28;
			// divwu r4,r5,r25
			ctx.r4.u32 = var_r25 ? ctx.r5.u32 / var_r25 : 0;
			// mullw r3,r4,r25
			ctx.r3.s64 = int64_t(ctx.r4.s32) * int64_t((int32_t)var_r25);
			// subf r11,r3,r5
			ctx.r11.s64 = ctx.r5.s64 - ctx.r3.s64;
		}
	loc_8245BC4C:
		// add r30,r11,r28
		var_r30 = (uint32_t)(ctx.r11.u64 + var_r28);
		// twllei r25,0
		if ((int32_t)var_r25 == 0 || var_r25 < 0u) __builtin_trap();
	loc_8245BC54:
		// cmplwi cr6,r27,0
		// beq cr6,0x8245bc70
		if (var_r27 != 0) {
			// cmplw cr6,r30,r28
			// ble cr6,0x8245bc70
			if (var_r30 <= var_r28) goto loc_8245BC70;
			// cmplw cr6,r30,r24
			// mr r29,r20
			var_r29 = (uint32_t)(var_r20);
			// blt cr6,0x8245bc74
			if (var_r30 < var_r24) goto loc_8245BC74;
		}
	loc_8245BC70:
		// mr r29,r26
		var_r29 = (uint32_t)(var_r26);
	loc_8245BC74:
		// lis r10,-32256
		// lwz r11,40(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
		// stw r26,92(r1)
		PPC_STORE_U32(ctx.r1.u32 + 92, var_r26);
		// mr r9,r26
		ctx.r9.u64 = var_r26;
		// stw r26,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r26);
		// cmplwi cr6,r11,0
		// lfs f0,15788(r10)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15788);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,84(r1)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
		// beq cr6,0x8245bcd4
		if (ctx.r11.u32 != 0) {
			// lwz r11,44(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
			// lwz r10,16(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
			// lwz r11,20(r10)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
			// lwz r8,28(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
			// lbz r7,0(r8)
			ctx.r7.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
			// rlwinm r6,r7,0,27,27
			ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x10;
			// cmplwi cr6,r6,16
			// bne cr6,0x8245bcd4
			if (ctx.r6.u32 != 16) goto loc_8245BCD4;
			// lwz r5,20(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 20);
			// mr r9,r20
			ctx.r9.u64 = var_r20;
			// lwz r4,124(r5)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 124);
			// lwz r3,8(r4)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
			// stw r3,88(r1)
			PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r3.u32);
			// lfs f0,116(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 116);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,84(r1)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
		}
	loc_8245BCD4:
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// cmpwi cr6,r29,0
		// lwz r3,12(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// li r11,2
		ctx.r11.s64 = 2;
		// bne cr6,0x8245bcec
		if ((int32_t)var_r29 == 0) {
			// mr r11,r20
			ctx.r11.u64 = var_r20;
		}
	loc_8245BCEC:
		// lwz r5,0(r3)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r6,r21
		ctx.r6.u64 = var_r21;
		// addi r10,r1,92
		ctx.r10.s64 = ctx.r1.s64 + 92;
		// addi r8,r1,84
		ctx.r8.s64 = ctx.r1.s64 + 84;
		// addi r7,r1,88
		ctx.r7.s64 = ctx.r1.s64 + 88;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// lwz r21,4(r5)
		var_r21 = (uint32_t)(PPC_LOAD_U32(ctx.r5.u32 + 4));
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// bctrl
		PPC_CALL_INDIRECT_FUNC(var_r21);
		// cmpwi cr6,r3,0
		// blt cr6,0x8245bf28
		if (ctx.r3.s32 < 0) {
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,92(r1)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
		// bl 0x8244a4f8
		atSingleton_A4F8_g(ctx, base);
		// cmplw cr6,r30,r23
		// blt cr6,0x8245bd40
		if (var_r30 >= var_r23) {
			// lis r3,-32761
			// ori r3,r3,87
			ctx.r3.u64 = ctx.r3.u64 | 87;
			return;
		}
	loc_8245BD40:
		// lwz r4,80(r1)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// clrlwi r3,r4,31
		ctx.r3.u64 = ctx.r4.u32 & 0x1;
		// cmplwi cr6,r3,1
		// bne cr6,0x8245bd5c
		if (ctx.r3.u32 == 1) {
			// stw r27,104(r1)
			PPC_STORE_U32(ctx.r1.u32 + 104, var_r27);
			// li r5,1
			ctx.r5.s64 = 1;
			// b 0x8245be88
		} else {
		loc_8245BD5C:
			// cmpwi cr6,r29,0
			// beq cr6,0x8245be34
			if ((int32_t)var_r29 != 0) {
				// subf r8,r23,r24
				ctx.r8.s64 = (int64_t)(int32_t)var_r24 - (int64_t)(int32_t)var_r23;
				// lwz r5,100(r1)
				ctx.r5.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
				// lwz r7,96(r1)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
				// addi r29,r31,68
				var_r29 = (uint32_t)(var_r31 + 68);
				// subf r11,r30,r8
				ctx.r11.s64 = ctx.r8.s64 - (int64_t)(int32_t)var_r30;
				// lwz r3,36(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
				// add r6,r30,r7
				ctx.r6.u64 = var_r30 + ctx.r7.u64;
				// stw r26,104(r1)
				PPC_STORE_U32(ctx.r1.u32 + 104, var_r26);
				// add r4,r11,r5
				ctx.r4.u64 = ctx.r11.u64 + ctx.r5.u64;
				// stw r26,108(r1)
				PPC_STORE_U32(ctx.r1.u32 + 108, var_r26);
				// stw r26,76(r31)
				PPC_STORE_U32(var_r31 + 76, var_r26);
				// li r5,0
				ctx.r5.s64 = 0;
				// stw r26,112(r1)
				PPC_STORE_U32(ctx.r1.u32 + 112, var_r26);
				// stw r29,180(r1)
				PPC_STORE_U32(ctx.r1.u32 + 180, var_r29);
				// stw r6,96(r1)
				PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r6.u32);
				// stw r4,100(r1)
				PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r4.u32);
				// addi r4,r1,96
				ctx.r4.s64 = ctx.r1.s64 + 96;
				// lwz r10,12(r11)
				// bctrl
				VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
				// cmpwi cr6,r3,0
				// blt cr6,0x8245bf28
				if (ctx.r3.s32 < 0) {
					return;
				}
				// add r9,r28,r22
				ctx.r9.u64 = var_r28 + var_r22;
				// subf r8,r28,r23
				ctx.r8.s64 = (int64_t)(int32_t)var_r23 - (int64_t)(int32_t)var_r28;
				// cmplwi cr6,r27,255
				// stw r9,96(r1)
				PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r9.u32);
				// stw r8,100(r1)
				PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r8.u32);
				// bne cr6,0x8245bde4
				if (var_r27 == 255) {
					// li r7,255
					ctx.r7.s64 = 255;
					// stw r7,104(r1)
					PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r7.u32);
					// b 0x8245bdec
				} else {
				loc_8245BDE4:
					// addi r6,r27,-1
					ctx.r6.s64 = (int64_t)(int32_t)var_r27 + -1;
					// stw r6,104(r1)
					PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r6.u32);
				}
			loc_8245BDEC:
				// lwz r3,0(r29)
				ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 0)/* atSingleton::vtable@+0x0 */;
				// addi r11,r31,80
				ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 80;
				// stw r26,108(r1)
				PPC_STORE_U32(ctx.r1.u32 + 108, var_r26);
				// li r5,0
				ctx.r5.s64 = 0;
				// stw r25,112(r1)
				PPC_STORE_U32(ctx.r1.u32 + 112, var_r25);
				// addi r4,r1,96
				ctx.r4.s64 = ctx.r1.s64 + 96;
				// stw r3,0(r11)
				PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
				// lwz r10,4(r29)
				ctx.r10.u64 = PPC_LOAD_U32(var_r29 + 4)/* atSingleton::flags@+0x4 */;
				// stw r11,180(r1)
				PPC_STORE_U32(ctx.r1.u32 + 180, ctx.r11.u32);
				// stw r10,4(r11)
				PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
				// lwz r9,8(r29)
				ctx.r9.u64 = PPC_LOAD_U32(var_r29 + 8);
				// stw r9,8(r11)
				PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
				// lwz r3,36(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
				// stw r20,88(r31)
				PPC_STORE_U32(var_r31 + 88, var_r20);
				// lwz r8,0(r3)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
				// lwz r7,12(r8)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 12);
				// mtctr r7
				ctx.ctr.u64 = ctx.r7.u64;
				// b 0x8245bea8
				goto loc_8245BEA8;
			}
		loc_8245BE34:
			// lwz r6,96(r1)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
			// cmplw cr6,r30,r28
			// lwz r4,100(r1)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
			// add r5,r30,r6
			ctx.r5.u64 = var_r30 + ctx.r6.u64;
			// subf r3,r30,r4
			ctx.r3.s64 = ctx.r4.s64 - (int64_t)(int32_t)var_r30;
			// stw r5,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r5.u32);
			// stw r3,100(r1)
			PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r3.u32);
			// bgt cr6,0x8245bf30
			if (var_r30 > var_r28) goto loc_8245BF30;
			// mr r11,r27
			ctx.r11.u64 = var_r27;
			// stw r28,108(r1)
			PPC_STORE_U32(ctx.r1.u32 + 108, var_r28);
			// cmplwi cr6,r27,0
			// stw r11,104(r1)
			PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r11.u32);
			// bne cr6,0x8245be6c
			if (var_r27 == 0) {
			loc_8245BE68:
				// stw r26,108(r1)
				PPC_STORE_U32(ctx.r1.u32 + 108, var_r26);
			}
		loc_8245BE6C:
			// cmplwi cr6,r11,0
			// beq cr6,0x8245be80
			if (ctx.r11.u32 != 0) {
				// cmplw cr6,r30,r28
				// stw r25,112(r1)
				PPC_STORE_U32(ctx.r1.u32 + 112, var_r25);
				// ble cr6,0x8245be84
				if (var_r30 <= var_r28) goto loc_8245BE84;
			}
		loc_8245BE80:
			// stw r26,112(r1)
			PPC_STORE_U32(ctx.r1.u32 + 112, var_r26);
		loc_8245BE84:
			// li r5,0
			ctx.r5.s64 = 0;
		}
	loc_8245BE88:
		// addi r11,r31,68
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 68;
		// lwz r3,36(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
		// stw r20,76(r31)
		PPC_STORE_U32(var_r31 + 76, var_r20);
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// stw r11,180(r1)
		PPC_STORE_U32(ctx.r1.u32 + 180, ctx.r11.u32);
		// lwz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// lwz r9,12(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
	loc_8245BEA8:
		// bctrl
		ctx.lr = 0x8245BEAC;
		PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245bf24
		if ((int32_t)var_r30 >= 0) {
			// li r4,2
			ctx.r4.s64 = 2;
			// stw r20,64(r31)
			PPC_STORE_U32(var_r31 + 64, var_r20);
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8244aa58
			atSingleton_AA58_g(ctx, base);
			// lwz r11,40(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
			// cmplwi cr6,r11,0
			// beq cr6,0x8245bf24
			if (ctx.r11.u32 == 0) goto loc_8245BF24;
			// lwz r8,36(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 36);
			// cmplwi cr6,r8,0
			// beq cr6,0x8245bf24
			if (ctx.r8.u32 == 0) goto loc_8245BF24;
			// lwz r7,44(r11)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
			// lwz r3,16(r7)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
			// bl 0x82459a50
			atSingleton_9A50_g(ctx, base);
			// lwz r3,36(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lwz r5,4(r6)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// lwz r4,40(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 40);
			// lwz r3,44(r4)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 44);
			// lwz r3,16(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
			// bl 0x82459a98
			atSingleton_9A98_g(ctx, base);
			// lwz r3,36(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lwz r10,8(r11)
			// bctrl
			VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		}
	loc_8245BF24:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_8245BF28:
	return;
loc_8245BF30:
	// mr r11,r26
	ctx.r11.u64 = var_r26;
	// stw r11,104(r1)
	PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r11.u32);
	// b 0x8245be68
	goto loc_8245BE68;
}

__attribute__((alias("__imp__msgMsgSink_BF40_g"))) PPC_WEAK_FUNC(msgMsgSink_BF40_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_BF40_g) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r8,r3,108
	ctx.r8.s64 = ctx.r3.s64 + 108;
	// addi r10,r3,88
	ctx.r10.s64 = ctx.r3.s64 + 88;
	// li r9,20
	ctx.r9.s64 = 20;
	// stw r11,112(r3)
	PPC_STORE_U32(ctx.r3.u32 + 112, ctx.r11.u32);
	// stw r11,116(r3)
	PPC_STORE_U32(ctx.r3.u32 + 116, ctx.r11.u32);
	// stw r11,120(r3)
	PPC_STORE_U32(ctx.r3.u32 + 120, ctx.r11.u32);
	// stw r11,124(r3)
	PPC_STORE_U32(ctx.r3.u32 + 124, ctx.r11.u32);
	// stw r11,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r11.u32);
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_8245BF68:
	// stb r11,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r11.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x8245bf68
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245BF68;
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// li r9,22
	ctx.r9.s64 = 22;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_8245BF80:
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x8245bf80
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245BF80;
	// stw r11,128(r3)
	PPC_STORE_U32(ctx.r3.u32 + 128, ctx.r11.u32);
	// stw r11,132(r3)
	PPC_STORE_U32(ctx.r3.u32 + 132, ctx.r11.u32);
	// stw r11,136(r3)
	PPC_STORE_U32(ctx.r3.u32 + 136, ctx.r11.u32);
	// stw r8,132(r3)
	PPC_STORE_U32(ctx.r3.u32 + 132, ctx.r8.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_BFA0_g"))) PPC_WEAK_FUNC(atSingleton_BFA0_g);
PPC_FUNC_IMPL(__imp__atSingleton_BFA0_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,120(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 120);
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
	// lis r4,-21628
	// ori r4,r4,3
	ctx.r4.u64 = ctx.r4.u64 | 3;
	// b 0x820c02d0
	_locale_register(ctx, base);
	return;
}

__attribute__((alias("__imp__sub_8245BFB8"))) PPC_WEAK_FUNC(sub_8245BFB8);
PPC_FUNC_IMPL(__imp__sub_8245BFB8) {
	PPC_FUNC_PROLOGUE();
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_BFC0_g"))) PPC_WEAK_FUNC(msgMsgSink_BFC0_g);
PPC_FUNC_IMPL(__imp__msgMsgSink_BFC0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lis r4,-21628
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// ori r4,r4,3
	ctx.r4.u64 = ctx.r4.u64 | 3;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// bl 0x820c01b8
	rage_01B8(ctx, base);
	// cmplwi cr6,r3,0
	// stw r3,120(r31)
	PPC_STORE_U32(var_r31 + 120, ctx.r3.u32);
	// bne cr6,0x8245c004
	if (ctx.r3.u32 == 0) {
		// lis r3,-32761
		// ori r3,r3,14
		ctx.r3.u64 = ctx.r3.u64 | 14;
		return;
	}
loc_8245C004:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r30,112(r31)
	PPC_STORE_U32(var_r31 + 112, var_r30);
	// addi r10,r31,128
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 128;
	// stw r30,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* msgMsgSink::flags@+0x4 */ var_r30);
	// clrlwi r9,r29,24
	ctx.r9.u64 = var_r29 & 0xFF;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r10.u32);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	return;
}

__attribute__((alias("__imp__game_C030_h"))) PPC_WEAK_FUNC(game_C030_h);
PPC_FUNC_IMPL(__imp__game_C030_h) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,124(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 124);
	// cmplw cr6,r4,r10
	// ble cr6,0x8245c06c
	if (ctx.r4.u32 > ctx.r10.u32) {
		// lwz r7,120(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 120);
		// subf r8,r10,r4
		ctx.r8.s64 = ctx.r4.s64 - ctx.r10.s64;
		// lwz r9,116(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 116);
		// add r10,r7,r10
		ctx.r10.u64 = ctx.r7.u64 + ctx.r10.u64;
		// cmplw cr6,r8,r9
		// stw r10,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
		// bge cr6,0x8245c060
		if (ctx.r8.u32 < ctx.r9.u32) {
			// mr r9,r8
			ctx.r9.u64 = ctx.r8.u64;
		}
	loc_8245C060:
		// mr r3,r9
		ctx.r3.u64 = ctx.r9.u64;
		// stw r9,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
		// blr
		return;
	}
loc_8245C06C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_C078_fw"))) PPC_WEAK_FUNC(msgMsgSink_C078_fw);
PPC_FUNC_IMPL(__imp__msgMsgSink_C078_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8244a970
	msgMsgSink_A970_2h(ctx, base);
	// lis r11,-32256
	// lbz r8,212(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 212);
	// addi r10,r11,21736
	ctx.r10.s64 = ctx.r11.s64 + 21736;
	// lis r11,-32256
	// clrlwi r7,r8,27
	ctx.r7.u64 = ctx.r8.u32 & 0x1F;
	// addi r9,r11,15912
	ctx.r9.s64 = ctx.r11.s64 + 15912;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r10.u32);
	// li r10,22
	ctx.r10.s64 = 22;
	// stb r7,212(r31)
	PPC_STORE_U8(var_r31 + 212, ctx.r7.u8);
	// stw r9,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r9.u32);
	// addi r9,r31,112
	ctx.r9.s64 = (int64_t)(int32_t)var_r31 + 112;
	// stw r11,68(r31)
	PPC_STORE_U32(var_r31 + 68, ctx.r11.u32);
	// stw r11,72(r31)
	PPC_STORE_U32(var_r31 + 72, ctx.r11.u32);
	// stw r11,76(r31)
	PPC_STORE_U32(var_r31 + 76, ctx.r11.u32);
	// stw r11,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r11.u32);
	// stw r11,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r11.u32);
	// stw r11,88(r31)
	PPC_STORE_U32(var_r31 + 88, ctx.r11.u32);
	// stw r11,92(r31)
	PPC_STORE_U32(var_r31 + 92, ctx.r11.u32);
	// stw r11,96(r31)
	PPC_STORE_U32(var_r31 + 96, ctx.r11.u32);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_8245C0E4:
	// stw r11,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r11.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bdnz 0x8245c0e4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_8245C0E4;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_21"))) PPC_WEAK_FUNC(atSingleton_vfn_21);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_21) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// lwz r11,32(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 32);
	// cmpwi cr6,r11,5
	// beq cr6,0x8245c170
	if (ctx.r11.s32 == 5) goto loc_8245C170;
	// cmpwi cr6,r11,4
	// bne cr6,0x8245c14c
	if (ctx.r11.s32 == 4) {
		// lwz r10,16(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
		// cmpwi cr6,r10,0
		// bne cr6,0x8245c170
		if (ctx.r10.s32 != 0) goto loc_8245C170;
		// cmpwi cr6,r4,0
		// bne cr6,0x8245c170
		if (ctx.r4.s32 != 0) goto loc_8245C170;
	}
loc_8245C14C:
	// cmpwi cr6,r11,3
	// beq cr6,0x8245c18c
	if (ctx.r11.s32 != 3) {
		// cmpwi cr6,r11,4
		// bne cr6,0x8245c164
		if (ctx.r11.s32 == 4) {
			// cmpwi cr6,r4,0
			// beq cr6,0x8245c18c
			if (ctx.r4.s32 == 0) goto loc_8245C18C;
		}
	loc_8245C164:
		// li r4,5
		ctx.r4.s64 = 5;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244aa58
		atSingleton_AA58_g(ctx, base);
	loc_8245C170:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	loc_8245C174:
		// blr
		return;
	}
loc_8245C18C:
	// cmplwi cr6,r4,1
	// blt cr6,0x8245c1c0
	if (ctx.r4.u32 < 1) goto loc_8245C1C0;
	// bne cr6,0x8245c170
	if (!ctx.cr6.eq) goto loc_8245C170;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// lwz r3,36(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
	// stb r30,56(r31)
	PPC_STORE_U8(var_r31 + 56, (uint8_t)var_r30);
	// lwz r8,48(r9)
	// bctrl
	VCALL(ctx.r3.u32, 12, ctx, base);  // vtable slot 12 (byte +48)
	// b 0x8245c170
	goto loc_8245C170;
loc_8245C1C0:
	// lbz r6,212(r31)
	ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 212);
	// li r7,1
	ctx.r7.s64 = 1;
	// li r4,4
	ctx.r4.s64 = 4;
	// ori r5,r6,128
	ctx.r5.u64 = ctx.r6.u64 | 128;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r7,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r7.u32);
	// stb r5,212(r31)
	PPC_STORE_U8(var_r31 + 212, ctx.r5.u8);
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// lwz r3,36(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r10,20(r11)
	// bctrl
	VCALL(ctx.r3.u32, 5, ctx, base);  // vtable slot 5 (byte +20)
	// b 0x8245c174
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_12_C200_1"))) PPC_WEAK_FUNC(atSingleton_vfn_12_C200_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_12_C200_1) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,0
	// li r4,1
	ctx.r4.s64 = 1;
	// bne cr6,0x8245c210
	if (ctx.r4.s32 == 0) {
		// li r4,0
		ctx.r4.s64 = 0;
	}
loc_8245C210:
	// lwz r11,40(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 40);
	// lwz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// lwz r9,16(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// lwz r8,20(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
	// lwz r3,48(r8)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 48);
	// lwz r7,0(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// lwz r6,4(r7)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
	// mtctr r6
	ctx.ctr.u64 = ctx.r6.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_23_C238_1"))) PPC_WEAK_FUNC(atSingleton_vfn_23_C238_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_23_C238_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,68(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 68);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245c2a4
	if (ctx.r11.u32 != 0) {
		// lwz r10,108(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 108);
		// cmpwi cr6,r10,1
		// bne cr6,0x8245c2a4
		if (ctx.r10.s32 != 1) goto loc_8245C2A4;
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// addi r5,r11,88
		ctx.r5.s64 = ctx.r11.s64 + 88;
		// lwz r8,60(r9)
		// bctrl
		VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
		// cmpwi cr6,r3,0
		// bne cr6,0x8245c2a4
		if (ctx.r3.s32 != 0) goto loc_8245C2A4;
	loc_8245C28C:
		// li r3,1
		ctx.r3.s64 = 1;
		// blr
		return;
	}
loc_8245C2A4:
	// lwz r11,72(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 72);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245c2e4
	if (!(ctx.r11.u32 == 0)) {
		// lwz r7,108(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 108);
	} else {
		if (!(ctx.r7.s32 != 1)) {
			// lwz r3,28(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
			// addi r5,r11,88
			ctx.r5.s64 = ctx.r11.s64 + 88;
			// lhz r4,44(r31)
			ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
			// addi r6,r1,80
			ctx.r6.s64 = ctx.r1.s64 + 80;
			// lwz r10,60(r11)
			// bctrl
			VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
			// cmpwi cr6,r3,0
			// beq cr6,0x8245c28c
			if (ctx.r3.s32 == 0) {
			// li r3,1
			ctx.r3.s64 = 1;
			// blr
			return;
			}
		}
	}
loc_8245C2E4:
	// lwz r11,76(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 76);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245c338
	if (ctx.r11.u32 != 0) {
		// lbz r9,212(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 212);
		// rlwinm r8,r9,0,25,25
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x40;
		// cmplwi cr6,r8,64
		// beq cr6,0x8245c338
		if (ctx.r8.u32 == 64) goto loc_8245C338;
		// lwz r7,108(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 108);
		// cmpwi cr6,r7,1
		// bne cr6,0x8245c338
		if (ctx.r7.s32 != 1) goto loc_8245C338;
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// addi r5,r11,88
		ctx.r5.s64 = ctx.r11.s64 + 88;
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// lwz r10,60(r11)
		// bctrl
		VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
		// cmpwi cr6,r3,0
		// li r3,1
		ctx.r3.s64 = 1;
		// beq cr6,0x8245c33c
		if (ctx.r3.s32 == 0) {
			// blr
			return;
		}
	}
loc_8245C338:
	// li r3,0
	ctx.r3.s64 = 0;
loc_8245C33C:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_C350"))) PPC_WEAK_FUNC(atSingleton_C350);
PPC_FUNC_IMPL(__imp__atSingleton_C350) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// lwz r3,40(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 40);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245c3b8
	if (ctx.r3.u32 != 0) {
		// lbz r11,24(r31)
		ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 24);
		// rlwinm r10,r11,0,26,26
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r10,32
		// bne cr6,0x8245c3a4
		if (ctx.r10.u32 == 32) {
			// lbz r9,56(r31)
			ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 56);
			// cmplwi cr6,r9,0
			// beq cr6,0x8245c3a4
			if (ctx.r9.u32 == 0) goto loc_8245C3A4;
			// lwz r7,96(r8)
			// bctrl
			VCALL(ctx.r3.u32, 24, ctx, base);  // vtable slot 24 (byte +96)
		}
	loc_8245C3A4:
		// lwz r3,40(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 40);
		// lwz r5,92(r6)
		// bctrl
		VCALL(ctx.r3.u32, 23, ctx, base);  // vtable slot 23 (byte +92)
	}
loc_8245C3B8:
	// lbz r4,24(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 24);
	// rlwinm r3,r4,0,0,24
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r3,128
	// beq cr6,0x8245c3f4
	if (ctx.r3.u32 != 128) {
		// lwz r3,36(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
		// lwz r10,16(r11)
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245c400
		if ((int32_t)var_r30 < 0) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// blr
			return;
		}
		// lbz r9,24(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 24);
		// ori r8,r9,2
		ctx.r8.u64 = ctx.r9.u64 | 2;
		// stb r8,24(r31)
		PPC_STORE_U8(var_r31 + 24, ctx.r8.u8);
	}
loc_8245C3F4:
	// li r4,3
	ctx.r4.s64 = 3;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
loc_8245C400:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_25"))) PPC_WEAK_FUNC(atSingleton_vfn_25);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_25) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r3,28(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 28);
	// lhz r4,44(r30)
	ctx.r4.u64 = PPC_LOAD_U16(var_r30 + 44);
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// cmplwi cr6,r9,0
	// bne cr6,0x8245c474
	if (ctx.r9.u32 == 0) {
		// lwz r8,20(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
		// cmplwi cr6,r8,0
		// bne cr6,0x8245c474
		if (ctx.r8.u32 != 0) goto loc_8245C474;
		// lwz r7,12(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 12);
		// stw r7,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r7.u32);
	}
loc_8245C474:
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// lhz r4,44(r30)
	ctx.r4.u64 = PPC_LOAD_U16(var_r30 + 44);
	// lwz r3,28(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 28);
	// bl 0x82455c10
	atSingleton_5C10_g(ctx, base);
	// lbz r6,56(r30)
	ctx.r6.u64 = PPC_LOAD_U8(var_r30 + 56);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplwi cr6,r6,0
	// beq cr6,0x8245c4d0
	if (ctx.r6.u32 != 0) {
		// lwz r10,80(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// li r9,1
		ctx.r9.s64 = 1;
		// rlwinm r5,r10,0,0,0
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x80000000;
		// cmplwi cr6,r5,0
		// beq cr6,0x8245c4ac
		if (ctx.r5.u32 != 0) {
			// li r9,2
			ctx.r9.s64 = 2;
		}
	loc_8245C4AC:
		// rlwinm r4,r10,28,5,31
		ctx.r4.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0x7FFFFFF;
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r10,16(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
		// twllei r9,0
		if (ctx.r9.s32 == 0 || ctx.r9.u32 < 0u) __builtin_trap();
		// twllei r4,0
		if (ctx.r4.s32 == 0 || ctx.r4.u32 < 0u) __builtin_trap();
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// divwu r11,r3,r9
		ctx.r11.u32 = ctx.r9.u32 ? ctx.r3.u32 / ctx.r9.u32 : 0;
		// mulli r10,r11,1000
		ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(1000));
		// divwu r3,r10,r4
		ctx.r3.u32 = ctx.r4.u32 ? ctx.r10.u32 / ctx.r4.u32 : 0;
	}
loc_8245C4D0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_26_C4E8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_26_C4E8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_26_C4E8_1) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r3,28(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lhz r4,44(r11)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// lwz r10,16(r11)
	// bctrl
	VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
	// rlwinm r9,r11,0,0,0
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x80000000;
	// cmplwi cr6,r9,0
	// li r9,1
	ctx.r9.s64 = 1;
	// beq cr6,0x8245c528
	if (ctx.r9.u32 != 0) {
		// li r9,2
		ctx.r9.s64 = 2;
	}
loc_8245C528:
	// rlwinm r8,r11,28,5,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 28) & 0x7FFFFFF;
	// lwz r10,12(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// twllei r9,0
	if (ctx.r9.s32 == 0 || ctx.r9.u32 < 0u) __builtin_trap();
	// twllei r8,0
	if (ctx.r8.s32 == 0 || ctx.r8.u32 < 0u) __builtin_trap();
	// add r7,r10,r11
	ctx.r7.u64 = ctx.r10.u64 + ctx.r11.u64;
	// divwu r6,r7,r9
	ctx.r6.u32 = ctx.r9.u32 ? ctx.r7.u32 / ctx.r9.u32 : 0;
	// mulli r5,r6,1000
	ctx.r5.s64 = static_cast<int64_t>(ctx.r6.u64 * static_cast<uint64_t>(1000));
	// divwu r3,r5,r8
	ctx.r3.u32 = ctx.r8.u32 ? ctx.r5.u32 / ctx.r8.u32 : 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_C560_g"))) PPC_WEAK_FUNC(atSingleton_C560_g);
PPC_FUNC_IMPL(__imp__atSingleton_C560_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,21736
	ctx.r11.s64 = ctx.r11.s64 + 21736;
	// lis r10,-32256
	// addi r10,r10,15912
	ctx.r10.s64 = ctx.r10.s64 + 15912;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// lbz r11,212(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 212);
	// stw r10,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r10.u32);
	// ori r10,r11,128
	ctx.r10.u64 = ctx.r11.u64 | 128;
	// stb r10,212(r31)
	PPC_STORE_U8(var_r31 + 212, ctx.r10.u8);
loc_8245C59C:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8245c238
	atSingleton_vfn_23_C238_1(ctx, base);
	// cmpwi cr6,r3,0
	// bne cr6,0x8245c59c
	if (ctx.r3.s32 != 0) goto loc_8245C59C;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244ae38
	atSingleton_AE38_g(ctx, base);
	// lwz r30,68(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 68));
	// cmplwi cr6,r30,0
	// beq cr6,0x8245c5d8
	if (var_r30 != 0) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8245bfa0
		atSingleton_BFA0_g(ctx, base);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// ori r4,r4,32804
		ctx.r4.u64 = ctx.r4.u64 | 32804;
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_8245C5D8:
	// lwz r30,72(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 72));
	// cmplwi cr6,r30,0
	// beq cr6,0x8245c5fc
	if (var_r30 != 0) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8245bfa0
		atSingleton_BFA0_g(ctx, base);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// ori r4,r4,32804
		ctx.r4.u64 = ctx.r4.u64 | 32804;
		// bl 0x820c02d0
		_locale_register(ctx, base);
	}
loc_8245C5FC:
	// lwz r9,76(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 76);
	// cmplwi cr6,r9,0
	// beq cr6,0x8245c614
	if (ctx.r9.u32 != 0) {
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// bl 0x824586c0
		atSingleton_86C0_g(ctx, base);
	}
loc_8245C614:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244af18
	atSingleton_AF18_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_24_C638_1"))) PPC_WEAK_FUNC(atSingleton_vfn_24_C638_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_24_C638_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8244acd8
	ph_vt3DB0_37_ACD8(ctx, base);
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 24);
	// rlwinm r10,r11,0,27,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r10,16
	// bne cr6,0x8245c67c
	if (ctx.r10.u32 == 16) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,0(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		return;
	}
loc_8245C67C:
	// lbz r11,212(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 212);
	// rlwinm r6,r11,0,26,26
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r6,32
	// bne cr6,0x8245c6d0
	if (ctx.r6.u32 == 32) {
		// lwz r5,32(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 32);
		// cmpwi cr6,r5,2
		// bne cr6,0x8245c6d0
		if (ctx.r5.s32 != 2) goto loc_8245C6D0;
		// andi. r4,r11,223
		ctx.r4.u64 = ctx.r11.u64 & 223;
		ctx.cr0.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r4,212(r31)
		PPC_STORE_U8(var_r31 + 212, ctx.r4.u8);
		// bl 0x8245c350
		atSingleton_C350(ctx, base);
		// cmpwi cr6,r3,0
		// bge cr6,0x8245c6d0
		if (ctx.r3.s32 >= 0) goto loc_8245C6D0;
		// lwz r11,0(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r10,0(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		return;
	}
loc_8245C6D0:
	// lwz r4,36(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 36);
	// cmplwi cr6,r4,0
	// beq cr6,0x8245c714
	if (ctx.r4.u32 != 0) {
		// lwz r9,32(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 32);
		// cmpwi cr6,r9,5
		// bne cr6,0x8245c714
		if (ctx.r9.s32 != 5) goto loc_8245C714;
		// lwz r8,20(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r3,12(r8)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 12);
		// lwz r6,12(r7)
		// bctrl
		VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
		// li r4,0
		ctx.r4.s64 = 0;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8244a4f8
		atSingleton_A4F8_g(ctx, base);
		return;
	}
loc_8245C714:
	// lbz r10,212(r31)
	ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 212);
	// rlwinm r5,r10,0,0,24
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r5,128
	// beq cr6,0x8245cb10
	if (ctx.r5.u32 != 128) {
		// lwz r11,76(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 76);
		// li r28,0
		var_r28 = 0;
		// cmplwi cr6,r11,0
		// beq cr6,0x8245c80c
		if (ctx.r11.u32 != 0) {
			// rlwinm r3,r10,0,25,25
			ctx.r3.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x40;
			// cmplwi cr6,r3,64
			// beq cr6,0x8245c80c
			if (ctx.r3.u32 == 64) goto loc_8245C80C;
			// lwz r10,108(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 108);
			// cmpwi cr6,r10,1
			// bne cr6,0x8245c7c8
			if (ctx.r10.s32 == 1) {
				// lwz r3,28(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
				// addi r6,r1,80
				ctx.r6.s64 = ctx.r1.s64 + 80;
				// lhz r4,44(r31)
				ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
				// addi r5,r11,88
				ctx.r5.s64 = ctx.r11.s64 + 88;
				// lwz r8,60(r9)
				// bctrl
				VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
				// cmpwi cr6,r3,0
				// beq cr6,0x8245cb10
				if (ctx.r3.s32 == 0) {
					return;
				}
				// lwz r4,80(r1)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				// lwz r3,76(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 76);
				// bl 0x8245c030
				game_C030_h(ctx, base);
				// lwz r7,76(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 76);
				// addi r11,r31,100
				ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 100;
				// addi r6,r31,96
				ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 96;
				// stw r28,108(r7)
				PPC_STORE_U32(ctx.r7.u32 + 108, var_r28);
				// lwz r5,76(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 76);
				// lwz r4,0(r5)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
				// stw r4,112(r31)
				PPC_STORE_U32(var_r31 + 112, ctx.r4.u32);
				// lwz r3,76(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 76);
				// lwz r10,4(r3)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* atSingleton::flags@+0x4 */;
				// stw r10,116(r31)
				PPC_STORE_U32(var_r31 + 116, ctx.r10.u32);
				// lwz r9,76(r31)
				ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 76);
				// lwz r8,4(r9)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
				// stw r8,128(r31)
				PPC_STORE_U32(var_r31 + 128, ctx.r8.u32);
				// stw r11,196(r31)
				PPC_STORE_U32(var_r31 + 196, ctx.r11.u32);
				// lbz r7,212(r31)
				ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 212);
				// stw r6,104(r31)
				PPC_STORE_U32(var_r31 + 104, ctx.r6.u32);
				// ori r5,r7,64
				ctx.r5.u64 = ctx.r7.u64 | 64;
				// b 0x8245c800
			} else {
			loc_8245C7C8:
				// lwz r4,0(r11)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
				// addi r11,r31,100
				ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 100;
				// addi r3,r31,96
				ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 96;
				// stw r4,112(r31)
				PPC_STORE_U32(var_r31 + 112, ctx.r4.u32);
				// lwz r10,76(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 76);
				// lwz r9,4(r10)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
				// stw r9,116(r31)
				PPC_STORE_U32(var_r31 + 116, ctx.r9.u32);
				// lwz r8,76(r31)
				ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 76);
				// lwz r7,4(r8)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 4);
				// stw r7,128(r31)
				PPC_STORE_U32(var_r31 + 128, ctx.r7.u32);
				// stw r11,196(r31)
				PPC_STORE_U32(var_r31 + 196, ctx.r11.u32);
				// lbz r6,212(r31)
				ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 212);
				// stw r3,104(r31)
				PPC_STORE_U32(var_r31 + 104, ctx.r3.u32);
				// ori r5,r6,64
				ctx.r5.u64 = ctx.r6.u64 | 64;
			}
		loc_8245C800:
			// stb r5,212(r31)
			PPC_STORE_U8(var_r31 + 212, ctx.r5.u8);
			// stw r28,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, var_r28);
			// stw r28,108(r31)
			PPC_STORE_U32(var_r31 + 108, var_r28);
		}
	loc_8245C80C:
		// lwz r11,92(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 92);
		// li r29,2
		var_r29 = 2;
		// lwz r4,108(r11)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 108);
		// cmpwi cr6,r4,1
		// bne cr6,0x8245c924
		if (ctx.r4.s32 == 1) {
			// lwz r3,28(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
			// addi r5,r11,88
			ctx.r5.s64 = ctx.r11.s64 + 88;
			// lhz r4,44(r31)
			ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
			// addi r6,r1,80
			ctx.r6.s64 = ctx.r1.s64 + 80;
			// lwz r10,60(r11)
			// bctrl
			VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
			// cmpwi cr6,r3,0
			// beq cr6,0x8245c924
			if (ctx.r3.s32 == 0) goto loc_8245C924;
			// lwz r4,80(r1)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// lwz r3,92(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 92);
			// bl 0x8245c030
			game_C030_h(ctx, base);
			// stw r3,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r3.u32);
			// cmplwi cr6,r3,0
			// beq cr6,0x8245c8d0
			if (ctx.r3.u32 != 0) {
				// lwz r11,84(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
				// lwz r9,92(r31)
				ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 92);
				// add r8,r11,r3
				ctx.r8.u64 = ctx.r11.u64 + ctx.r3.u64;
				// stw r8,84(r31)
				PPC_STORE_U32(var_r31 + 84, ctx.r8.u32);
				// stw r29,108(r9)
				PPC_STORE_U32(ctx.r9.u32 + 108, var_r29);
				// lwz r3,36(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
				// lwz r11,72(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 72);
				// lwz r4,92(r31)
				ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 92);
				// cntlzw r6,r11
				ctx.r6.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
				// lwz r7,0(r3)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
				// rlwinm r5,r6,27,31,31
				ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0x1;
				// lwz r11,12(r7)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 12);
				// bctrl
				PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
				// cmpwi cr6,r3,0
				// blt cr6,0x8245c8ac
				if (ctx.r3.s32 >= 0) {
					// lwz r10,92(r31)
					ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 92);
					// stw r10,80(r31)
					PPC_STORE_U32(var_r31 + 80, ctx.r10.u32);
					// b 0x8245c8b4
				} else {
				loc_8245C8AC:
					// lwz r9,92(r31)
					ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 92);
					// stw r28,108(r9)
					PPC_STORE_U32(ctx.r9.u32 + 108, var_r28);
				}
			loc_8245C8B4:
				// lwz r8,32(r31)
				ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 32);
				// cmpwi cr6,r8,1
				// bne cr6,0x8245c8d8
				if (ctx.r8.s32 != 1) goto loc_8245C8D8;
				// li r4,2
				ctx.r4.s64 = 2;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// bl 0x8244aa58
				atSingleton_AA58_g(ctx, base);
				// b 0x8245c8d8
			} else {
			loc_8245C8D0:
				// lwz r7,92(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 92);
				// stw r28,108(r7)
				PPC_STORE_U32(ctx.r7.u32 + 108, var_r28);
			}
		loc_8245C8D8:
			// lwz r10,68(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 68);
			// lwz r6,92(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 92);
			// cmplw cr6,r6,r10
			// bne cr6,0x8245c8fc
			if (ctx.r6.u32 == ctx.r10.u32) {
				// lwz r11,72(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 72);
				// cmplwi cr6,r11,0
				// beq cr6,0x8245c900
				if (ctx.r11.u32 == 0) goto loc_8245C900;
				// stw r11,92(r31)
				PPC_STORE_U32(var_r31 + 92, ctx.r11.u32);
				// b 0x8245c900
			} else {
			loc_8245C8FC:
				// stw r10,92(r31)
				PPC_STORE_U32(var_r31 + 92, ctx.r10.u32);
			}
		loc_8245C900:
			// lwz r5,88(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 88);
			// cmplw cr6,r5,r10
			// bne cr6,0x8245c920
			if (ctx.r5.u32 == ctx.r10.u32) {
				// lwz r11,72(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 72);
				// cmplwi cr6,r11,0
				// beq cr6,0x8245c924
				if (ctx.r11.u32 == 0) goto loc_8245C924;
				// stw r11,88(r31)
				PPC_STORE_U32(var_r31 + 88, ctx.r11.u32);
				// b 0x8245c924
			} else {
			loc_8245C920:
				// stw r10,88(r31)
				PPC_STORE_U32(var_r31 + 88, ctx.r10.u32);
			}
		}
	loc_8245C924:
		// lbz r4,56(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 56);
		// cmplwi cr6,r4,0
		// beq cr6,0x8245c9ec
		if (ctx.r4.u32 != 0) {
			// lbz r3,24(r31)
			ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 24);
			// rlwinm r11,r3,0,26,26
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0x20;
			// cmplwi cr6,r11,32
			// beq cr6,0x8245c9ec
			if (ctx.r11.u32 == 32) goto loc_8245C9EC;
			// lwz r10,208(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 208);
			// lwz r9,84(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 84);
			// cmplw cr6,r9,r10
			// blt cr6,0x8245c9ec
			if (ctx.r9.u32 < ctx.r10.u32) goto loc_8245C9EC;
			// lwz r8,96(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 96);
			// cmpwi cr6,r8,0
			// bne cr6,0x8245cb10
			if (ctx.r8.s32 != 0) {
				return;
			}
			// addi r30,r31,112
			var_r30 = (uint32_t)(var_r31 + 112);
			// lwz r7,4(r30)
			ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 4)/* atSingleton::flags@+0x4 */;
			// cmplwi cr6,r7,0
			// beq cr6,0x8245c998
			if (ctx.r7.u32 != 0) {
				// lwz r3,36(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
				// li r5,0
				ctx.r5.s64 = 0;
				// stw r29,96(r31)
				PPC_STORE_U32(var_r31 + 96, var_r29);
				// mr r4,r30
				ctx.r4.u64 = var_r30;
				// lwz r11,12(r6)
				// bctrl
				VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
				// cmpwi cr6,r3,0
				// bge cr6,0x8245c998
				if (ctx.r3.s32 >= 0) goto loc_8245C998;
				// stw r28,96(r31)
				PPC_STORE_U32(var_r31 + 96, var_r28);
			}
		loc_8245C998:
			// lwz r10,8(r30)
			ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 8);
			// cmplwi cr6,r10,0
			// beq cr6,0x8245c9cc
			if (ctx.r10.u32 != 0) {
				// lbz r9,56(r31)
				ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 56);
				// rotlwi r8,r10,0
				ctx.r8.u64 = ctx.r10.u32;
				// lwz r10,204(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 204);
				// subf r7,r8,r9
				ctx.r7.s64 = ctx.r9.s64 - ctx.r8.s64;
				// stb r7,56(r31)
				PPC_STORE_U8(var_r31 + 56, ctx.r7.u8);
				// lwz r11,4(r30)
				ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 4)/* atSingleton::flags@+0x4 */;
				// add r5,r11,r10
				ctx.r5.u64 = ctx.r11.u64 + ctx.r10.u64;
				// stw r5,84(r31)
				PPC_STORE_U32(var_r31 + 84, ctx.r5.u32);
				return;
			}
		loc_8245C9CC:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8244a380
			game_A380_h(ctx, base);
			// lwz r11,4(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 4)/* atSingleton::flags@+0x4 */;
			// lwz r10,204(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 204);
			// add r5,r11,r10
			ctx.r5.u64 = ctx.r11.u64 + ctx.r10.u64;
			// stw r5,84(r31)
			PPC_STORE_U32(var_r31 + 84, ctx.r5.u32);
			return;
		}
	loc_8245C9EC:
		// lwz r4,84(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 84);
		// lwz r3,200(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 200);
		// cmplw cr6,r4,r3
		// blt cr6,0x8245ca44
		if (ctx.r4.u32 >= ctx.r3.u32) {
			// lwz r11,80(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 80);
			// cmplwi cr6,r11,0
			// beq cr6,0x8245ca10
			if (ctx.r11.u32 != 0) {
				// li r10,1
				ctx.r10.s64 = 1;
				// stw r10,136(r11)
				PPC_STORE_U32(ctx.r11.u32 + 136, ctx.r10.u32);
			}
		loc_8245CA10:
			// lbz r9,24(r31)
			ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 24);
			// lwz r3,36(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
			// ori r8,r9,64
			ctx.r8.u64 = ctx.r9.u64 | 64;
			// stb r8,24(r31)
			PPC_STORE_U8(var_r31 + 24, ctx.r8.u8);
			// lwz r6,24(r7)
			// bctrl
			VCALL(ctx.r3.u32, 6, ctx, base);  // vtable slot 6 (byte +24)
			// lbz r5,212(r31)
			ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 212);
			// ori r4,r5,128
			ctx.r4.u64 = ctx.r5.u64 | 128;
			// stb r4,212(r31)
			PPC_STORE_U8(var_r31 + 212, ctx.r4.u8);
			return;
		}
	loc_8245CA44:
		// lwz r11,88(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 88);
		// lwz r3,108(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 108);
		// cmpwi cr6,r3,0
		// bne cr6,0x8245cb10
		if (ctx.r3.s32 != 0) {
			return;
		}
		// li r10,1
		ctx.r10.s64 = 1;
		// stw r10,108(r11)
		PPC_STORE_U32(ctx.r11.u32 + 108, ctx.r10.u32);
		// lwz r4,84(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 84);
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// bl 0x82457f68
		atSingleton_7F68_g(ctx, base);
		// lwz r11,88(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 88);
		// stw r3,96(r11)
		PPC_STORE_U32(ctx.r11.u32 + 96, ctx.r3.u32);
		// stw r28,100(r11)
		PPC_STORE_U32(ctx.r11.u32 + 100, var_r28);
		// lwz r9,84(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 84);
		// lwz r8,88(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 88);
		// subf r7,r3,r9
		ctx.r7.s64 = ctx.r9.s64 - ctx.r3.s64;
		// stw r7,124(r8)
		PPC_STORE_U32(ctx.r8.u32 + 124, ctx.r7.u32);
		// lbz r6,24(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 24);
		// rlwinm r5,r6,0,26,26
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r5,32
		// beq cr6,0x8245cab0
		if (ctx.r5.u32 != 32) {
			// lbz r4,56(r31)
			ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 56);
			// cmplwi cr6,r4,0
			// beq cr6,0x8245cab0
			if (ctx.r4.u32 == 0) goto loc_8245CAB0;
			// lwz r3,208(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 208);
			// lwz r11,84(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
			// subf r11,r11,r3
			ctx.r11.s64 = ctx.r3.s64 - ctx.r11.s64;
			// b 0x8245cabc
		} else {
		loc_8245CAB0:
			// lwz r10,200(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 200);
			// lwz r9,84(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 84);
			// subf r11,r9,r10
			ctx.r11.s64 = ctx.r10.s64 - ctx.r9.s64;
		}
	loc_8245CABC:
		// lwz r10,88(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 88);
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// lwz r8,112(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 112);
		// lwz r7,124(r10)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 124);
		// subf r9,r7,r8
		ctx.r9.s64 = ctx.r8.s64 - ctx.r7.s64;
		// cmplw cr6,r11,r9
		// ble cr6,0x8245cae0
		if (ctx.r11.u32 > ctx.r9.u32) {
			// mr r11,r9
			ctx.r11.u64 = ctx.r9.u64;
			// stw r11,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		}
	loc_8245CAE0:
		// stw r11,116(r10)
		PPC_STORE_U32(ctx.r10.u32 + 116, ctx.r11.u32);
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// lwz r5,88(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 88);
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// lwz r11,56(r6)
		// bctrl
		VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
		// cmpwi cr6,r3,0
		// bge cr6,0x8245cb10
		if (ctx.r3.s32 >= 0) {
			return;
		}
		// lwz r10,88(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 88);
		// stw r28,108(r10)
		PPC_STORE_U32(ctx.r10.u32 + 108, var_r28);
	}
loc_8245CB10:
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_19_CB18_1"))) PPC_WEAK_FUNC(atSingleton_vfn_19_CB18_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_19_CB18_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r27 = 0;
	PPCRegister temp{};
	// FRAME: size=256, savegprlr_22
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r23,0
	var_r23 = 0;
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// mr r25,r23
	var_r25 = (uint32_t)(var_r23);
	// bl 0x82451b98
	msgMsgSink_1B98_g(ctx, base);
	// addi r11,r1,152
	ctx.r11.s64 = ctx.r1.s64 + 152;
	// lhz r4,44(r31)
	ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
	// stw r23,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r23);
	// stw r23,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r23);
	// bl 0x82455eb0
	msgMsgSink_5EB0_g(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8244aa58
	atSingleton_AA58_g(ctx, base);
	// li r22,1
	var_r22 = 1;
	// cmpwi cr6,r29,0
	// stb r30,56(r31)
	PPC_STORE_U8(var_r31 + 56, (uint8_t)var_r30);
	// mr r11,r22
	ctx.r11.u64 = var_r22;
	// bne cr6,0x8245cb84
	if ((int32_t)var_r29 == 0) {
		// mr r11,r23
		ctx.r11.u64 = var_r23;
	}
loc_8245CB84:
	// lbz r10,24(r31)
	ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 24);
	// addi r6,r1,92
	ctx.r6.s64 = ctx.r1.s64 + 92;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// rlwimi r10,r11,5,26,26
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 5) & 0x20) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFDF);
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stb r10,24(r31)
	PPC_STORE_U8(var_r31 + 24, ctx.r10.u8);
	// bl 0x8244a7f8
	atSingleton_A7F8(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r4,92(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
	// addi r6,r1,152
	ctx.r6.s64 = ctx.r1.s64 + 152;
	// addi r5,r1,100
	ctx.r5.s64 = ctx.r1.s64 + 100;
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// bl 0x82451f60
	atSingleton_1F60_wrh(ctx, base);
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// bl 0x8215d6a8
	grcTextureReferenceBase_vfn_4(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// stw r11,200(r31)
	PPC_STORE_U32(var_r31 + 200, ctx.r11.u32);
	// bl 0x82451d80
	msgMsgSink_1D80_g(ctx, base);
	// mr r24,r3
	var_r24 = ctx.r3.u32;
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// bl 0x82451d30
	msgMsgSink_1D30_g(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// stw r11,204(r31)
	PPC_STORE_U32(var_r31 + 204, ctx.r11.u32);
	// bl 0x82451be0
	msgMsgSink_1BE0_g(ctx, base);
	// cmpwi cr6,r3,1
	// bne cr6,0x8245cc04
	if (ctx.r3.s32 == 1) {
		// lwz r24,200(r31)
		var_r24 = (uint32_t)(PPC_LOAD_U32(var_r31 + 200));
		// stw r23,204(r31)
		PPC_STORE_U32(var_r31 + 204, var_r23);
	}
loc_8245CC04:
	// cmpwi cr6,r30,0
	// blt cr6,0x8245cf2c
	if ((int32_t)var_r30 >= 0) {
		// lbz r7,24(r31)
		ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 24);
		// mr r11,r23
		ctx.r11.u64 = var_r23;
		// rlwinm r6,r7,0,26,26
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x20;
		// cmplwi cr6,r6,32
		// beq cr6,0x8245cc24
		if (ctx.r6.u32 != 32) {
			// lbz r11,56(r31)
			ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 56);
		}
	loc_8245CC24:
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// clrlwi r7,r11,24
		ctx.r7.u64 = ctx.r11.u32 & 0xFF;
		// addi r29,r31,72
		var_r29 = (uint32_t)(var_r31 + 72);
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// addi r27,r31,68
		var_r27 = (uint32_t)(var_r31 + 68);
		// mr r6,r29
		ctx.r6.u64 = var_r29;
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// lwz r10,68(r11)
		// bctrl
		VCALL(ctx.r3.u32, 17, ctx, base);  // vtable slot 17 (byte +68)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245cf2c
		if ((int32_t)var_r30 < 0) goto loc_8245CF2C;
		// lwz r9,0(r27)
		ctx.r9.u64 = PPC_LOAD_U32(var_r27 + 0);
		// mr r30,r22
		var_r30 = (uint32_t)(var_r22);
		// stw r31,128(r9)
		PPC_STORE_U32(ctx.r9.u32 + 128, var_r31);
		// lwz r11,0(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 0)/* atSingleton::vtable@+0x0 */;
		// cmplwi cr6,r11,0
		// beq cr6,0x8245cc7c
		if (ctx.r11.u32 != 0) {
			// li r30,2
			var_r30 = 2;
			// stw r31,128(r11)
			PPC_STORE_U32(ctx.r11.u32 + 128, var_r31);
		}
	loc_8245CC7C:
		// clrlwi r28,r30,24
		var_r28 = (uint32_t)(var_r30 & 0xFF);
		// cmplwi cr6,r28,1
		// bne cr6,0x8245cca0
		if (var_r28 == 1) {
			// lbz r8,24(r31)
			ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 24);
			// rlwinm r7,r8,0,26,26
			ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x20;
			// cmplwi cr6,r7,32
			// beq cr6,0x8245cca0
			if (ctx.r7.u32 == 32) goto loc_8245CCA0;
			// stb r23,56(r31)
			PPC_STORE_U8(var_r31 + 56, (uint8_t)var_r23);
			// b 0x8245cd1c
		} else {
		loc_8245CCA0:
			// lbz r6,24(r31)
			ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 24);
			// rlwinm r5,r6,0,26,26
			ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0x20;
			// cmplwi cr6,r5,32
			// beq cr6,0x8245cd1c
			if (ctx.r5.u32 == 32) goto loc_8245CD1C;
			// lbz r4,56(r31)
			ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 56);
			// cmplwi cr6,r4,0
			// beq cr6,0x8245cd1c
			if (ctx.r4.u32 == 0) goto loc_8245CD1C;
			// lwz r3,28(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
			// addi r29,r31,76
			var_r29 = (uint32_t)(var_r31 + 76);
			// lhz r4,44(r31)
			ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
			// mr r5,r29
			ctx.r5.u64 = var_r29;
			// lwz r10,72(r11)
			// bctrl
			VCALL(ctx.r3.u32, 18, ctx, base);  // vtable slot 18 (byte +72)
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// cmpwi cr6,r30,0
			// blt cr6,0x8245cf2c
			if ((int32_t)var_r30 < 0) goto loc_8245CF2C;
			// addi r9,r28,1
			ctx.r9.s64 = (int64_t)(int32_t)var_r28 + 1;
			// addi r3,r1,112
			ctx.r3.s64 = ctx.r1.s64 + 112;
			// clrlwi r30,r9,24
			var_r30 = (uint32_t)(ctx.r9.u32 & 0xFF);
			// bl 0x82451c08
			msgMsgSink_1C08_g(ctx, base);
			// cmpwi cr6,r3,0
			// bne cr6,0x8245cd1c
			if (ctx.r3.s32 != 0) goto loc_8245CD1C;
			// lwz r8,0(r29)
			ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 0)/* atSingleton::vtable@+0x0 */;
			// lwz r7,116(r8)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 116);
			// cmplw cr6,r7,r24
			// bne cr6,0x8245cd1c
			if (ctx.r7.u32 != var_r24) goto loc_8245CD1C;
			// lbz r6,56(r31)
			ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 56);
			// stw r6,120(r31)
			PPC_STORE_U32(var_r31 + 120, ctx.r6.u32);
			// stw r23,124(r31)
			PPC_STORE_U32(var_r31 + 124, var_r23);
		}
	loc_8245CD1C:
		// lwz r11,40(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
		// cmplwi cr6,r11,0
		// beq cr6,0x8245cd48
		if (ctx.r11.u32 != 0) {
			// lwz r5,44(r11)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
			// lwz r3,20(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
			// lwz r4,16(r5)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
			// lwz r11,20(r4)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
			// lwz r10,28(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
			// lhz r4,1(r10)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r10.u32 + 1);
			// bl 0x82448cd0
			atSingleton_8CD0_g(ctx, base);
			// mr r25,r3
			var_r25 = ctx.r3.u32;
		}
	loc_8245CD48:
		// addi r5,r1,96
		ctx.r5.s64 = ctx.r1.s64 + 96;
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// stw r23,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r23);
		// bl 0x82455c10
		atSingleton_5C10_g(ctx, base);
		// lis r10,-32256
		// lwz r11,40(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
		// stw r23,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, var_r23);
		// mr r9,r23
		ctx.r9.u64 = var_r23;
		// cmplwi cr6,r11,0
		// lfs f0,15788(r10)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15788);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,80(r1)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		// beq cr6,0x8245cdb8
		if (ctx.r11.u32 != 0) {
			// lwz r8,44(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
			// lwz r7,16(r8)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
			// lwz r11,20(r7)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 20);
			// lwz r6,28(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
			// lbz r5,0(r6)
			ctx.r5.u64 = PPC_LOAD_U8(ctx.r6.u32 + 0);
			// rlwinm r4,r5,0,27,27
			ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0x10;
			// cmplwi cr6,r4,16
			// bne cr6,0x8245cdb8
			if (ctx.r4.u32 != 16) goto loc_8245CDB8;
			// lwz r3,20(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
			// mr r9,r22
			ctx.r9.u64 = var_r22;
			// lwz r10,124(r3)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 124);
			// lwz r8,8(r10)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
			// stw r8,84(r1)
			PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r8.u32);
			// lfs f0,116(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 116);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,80(r1)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		}
	loc_8245CDB8:
		// lwz r5,20(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 20);
		// addi r10,r1,88
		ctx.r10.s64 = ctx.r1.s64 + 88;
		// addi r8,r1,80
		ctx.r8.s64 = ctx.r1.s64 + 80;
		// addi r7,r1,84
		ctx.r7.s64 = ctx.r1.s64 + 84;
		// mr r6,r25
		ctx.r6.u64 = var_r25;
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// lwz r3,12(r5)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 12);
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// lwz r11,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245cf2c
		if ((int32_t)var_r30 < 0) goto loc_8245CF2C;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,88(r1)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// bl 0x8244a4f8
		atSingleton_A4F8_g(ctx, base);
		// lwz r11,40(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 40);
		// cmplwi cr6,r11,0
		// beq cr6,0x8245ce50
		if (ctx.r11.u32 != 0) {
			// lwz r10,44(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
			// lwz r3,16(r10)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
			// bl 0x82459a50
			atSingleton_9A50_g(ctx, base);
			// lwz r3,36(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lwz r8,4(r9)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// lwz r7,40(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 40);
			// lwz r6,44(r7)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 44);
			// lwz r3,16(r6)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
			// bl 0x82459a98
			atSingleton_9A98_g(ctx, base);
			// lwz r3,36(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 36);
			// lwz r4,8(r5)
			// bctrl
			VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		}
	loc_8245CE50:
		// cmplw cr6,r26,r24
		// bge cr6,0x8245cf24
		if (var_r26 < var_r24) {
			// mr r4,r26
			ctx.r4.u64 = var_r26;
			// lwz r3,28(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
			// bl 0x82457f68
			atSingleton_7F68_g(ctx, base);
			// lwz r11,0(r27)
			ctx.r11.u64 = PPC_LOAD_U32(var_r27 + 0);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
			// subf r3,r29,r26
			ctx.r3.s64 = (int64_t)(int32_t)var_r26 - (int64_t)(int32_t)var_r29;
			// stw r29,96(r11)
			PPC_STORE_U32(ctx.r11.u32 + 96, var_r29);
			// stw r23,100(r11)
			PPC_STORE_U32(ctx.r11.u32 + 100, var_r23);
			// lwz r11,0(r27)
			ctx.r11.u64 = PPC_LOAD_U32(var_r27 + 0);
			// stw r3,124(r11)
			PPC_STORE_U32(ctx.r11.u32 + 124, ctx.r3.u32);
			// lbz r10,24(r31)
			ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 24);
			// rlwinm r9,r10,0,26,26
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x20;
			// cmplwi cr6,r9,32
			// beq cr6,0x8245ceac
			if (ctx.r9.u32 != 32) {
				// lbz r8,56(r31)
				ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 56);
				// cmplwi cr6,r8,0
				// beq cr6,0x8245ceac
				if (ctx.r8.u32 == 0) goto loc_8245CEAC;
				// lwz r11,204(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 204);
				// add r7,r11,r24
				ctx.r7.u64 = ctx.r11.u64 + var_r24;
				// stw r7,208(r31)
				PPC_STORE_U32(var_r31 + 208, ctx.r7.u32);
				// b 0x8245ceb4
			} else {
			loc_8245CEAC:
				// lwz r6,200(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 200);
				// stw r6,208(r31)
				PPC_STORE_U32(var_r31 + 208, ctx.r6.u32);
			}
		loc_8245CEB4:
			// lwz r11,0(r27)
			ctx.r11.u64 = PPC_LOAD_U32(var_r27 + 0);
			// lwz r10,208(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 208);
			// lwz r9,112(r11)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 112);
			// cmplw cr6,r9,r10
			// bge cr6,0x8245cecc
			if (ctx.r9.u32 < ctx.r10.u32) {
				// mr r10,r9
				ctx.r10.u64 = ctx.r9.u64;
			}
		loc_8245CECC:
			// lwz r5,124(r11)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 124);
			// subf r4,r5,r10
			ctx.r4.s64 = ctx.r10.s64 - ctx.r5.s64;
			// stw r4,116(r11)
			PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r4.u32);
			// lwz r3,0(r27)
			ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 0);
			// stw r22,108(r3)
			PPC_STORE_U32(ctx.r3.u32 + 108, var_r22);
			// lwz r3,28(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
			// lwz r5,0(r27)
			ctx.r5.u64 = PPC_LOAD_U32(var_r27 + 0);
			// lhz r4,44(r31)
			ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
			// lwz r10,56(r11)
			// bctrl
			VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// cmpwi cr6,r30,0
			// blt cr6,0x8245cf14
			if ((int32_t)var_r30 >= 0) {
				// lwz r11,84(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
				// add r9,r11,r29
				ctx.r9.u64 = ctx.r11.u64 + var_r29;
				// stw r9,84(r31)
				PPC_STORE_U32(var_r31 + 84, ctx.r9.u32);
			}
		loc_8245CF14:
			// lwz r11,0(r27)
			ctx.r11.u64 = PPC_LOAD_U32(var_r27 + 0);
			// stw r22,64(r31)
			PPC_STORE_U32(var_r31 + 64, var_r22);
			// stw r11,88(r31)
			PPC_STORE_U32(var_r31 + 88, ctx.r11.u32);
			// stw r11,92(r31)
			PPC_STORE_U32(var_r31 + 92, ctx.r11.u32);
		}
	loc_8245CF24:
		// cmpwi cr6,r30,0
		// bge cr6,0x8245cf98
		if ((int32_t)var_r30 >= 0) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
	}
loc_8245CF2C:
	// lwz r29,68(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 68));
	// cmplwi cr6,r29,0
	// beq cr6,0x8245cf54
	if (var_r29 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x8245bfa0
		atSingleton_BFA0_g(ctx, base);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// ori r4,r4,32804
		ctx.r4.u64 = ctx.r4.u64 | 32804;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// stw r23,68(r31)
		PPC_STORE_U32(var_r31 + 68, var_r23);
	}
loc_8245CF54:
	// lwz r29,72(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 72));
	// cmplwi cr6,r29,0
	// beq cr6,0x8245cf7c
	if (var_r29 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x8245bfa0
		atSingleton_BFA0_g(ctx, base);
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// ori r4,r4,32804
		ctx.r4.u64 = ctx.r4.u64 | 32804;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// stw r23,72(r31)
		PPC_STORE_U32(var_r31 + 72, var_r23);
	}
loc_8245CF7C:
	// lwz r8,76(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 76);
	// cmplwi cr6,r8,0
	// beq cr6,0x8245cf98
	if (ctx.r8.u32 != 0) {
		// lhz r4,44(r31)
		ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 44);
		// lwz r3,28(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
		// bl 0x824586c0
		atSingleton_86C0_g(ctx, base);
		// stw r23,76(r31)
		PPC_STORE_U32(var_r31 + 76, var_r23);
	}
loc_8245CF98:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_20_CFA8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_20_CFA8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_20_CFA8_1) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// lwz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// cmpwi cr6,r10,2
	// bne cr6,0x8245cfc4
	if (ctx.r10.s32 == 2) {
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// b 0x8245c350
		atSingleton_C350(ctx, base);
		return;
	}
loc_8245CFC4:
	// lbz r9,212(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 212);
	// ori r8,r9,32
	ctx.r8.u64 = ctx.r9.u64 | 32;
	// stb r8,212(r11)
	PPC_STORE_U8(ctx.r11.u32 + 212, ctx.r8.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_18_CFD8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_18_CFD8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_18_CFD8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x8245c560
	atSingleton_C560_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245d018
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32795
		ctx.r4.u64 = ctx.r4.u64 | 32795;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245D018:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_16_D030_1"))) PPC_WEAK_FUNC(atSingleton_vfn_16_D030_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_16_D030_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d080
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// fmr f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = var_f31;
		// bl 0x8244a3f0
		game_A3F0_h(ctx, base);
	}
loc_8245D080:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_17_D098_1"))) PPC_WEAK_FUNC(atSingleton_vfn_17_D098_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_17_D098_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d0e8
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// fmr f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = var_f31;
		// bl 0x8244a410
		game_A410_h(ctx, base);
	}
loc_8245D0E8:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_18_D100_1"))) PPC_WEAK_FUNC(atSingleton_vfn_18_D100_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_18_D100_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d150
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// fmr f1,f31
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = var_f31;
		// bl 0x8244a458
		game_A458_h(ctx, base);
	}
loc_8245D150:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_29_D168_1"))) PPC_WEAK_FUNC(atSingleton_vfn_29_D168_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_29_D168_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d1b8
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x8244a478
		game_A478_h(ctx, base);
	}
loc_8245D1B8:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_30"))) PPC_WEAK_FUNC(atSingleton_vfn_30);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_30) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d220
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x8244a498
		game_A498_h(ctx, base);
	}
loc_8245D220:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_19_D238_1"))) PPC_WEAK_FUNC(atSingleton_vfn_19_D238_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_19_D238_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d288
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x8244a4b8
		game_A4B8_h(ctx, base);
	}
loc_8245D288:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_20_D2A0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_20_D2A0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_20_D2A0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d2f0
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x8244a4d8
		game_A4D8_h(ctx, base);
	}
loc_8245D2F0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_21_D308_1"))) PPC_WEAK_FUNC(atSingleton_vfn_21_D308_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_21_D308_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d368
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// lwz r6,36(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 36);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
		// b 0x8245d36c
	} else {
	loc_8245D368:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_8245D36C:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_23_D388_1"))) PPC_WEAK_FUNC(atSingleton_vfn_23_D388_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_23_D388_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,88(r11)
	// bctrl
	atSingleton_vfn_22(ctx, base);  // vtable slot 22 (byte +88)  // atSingleton::vfn_22
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x8245d3c0
	if (ctx.r4.u32 != 0) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82452400
		atSingleton_2400_gen(ctx, base);
	}
loc_8245D3C0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_43"))) PPC_WEAK_FUNC(atSingleton_vfn_43);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_43) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-30009
	// ori r3,r3,6
	ctx.r3.u64 = ctx.r3.u64 | 6;
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_D3E8_v12"))) PPC_WEAK_FUNC(msgMsgSink_D3E8_v12);
PPC_FUNC_IMPL(__imp__msgMsgSink_D3E8_v12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r8
	var_r30 = ctx.r8.u32;
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r10,r11,21904
	ctx.r10.s64 = ctx.r11.s64 + 21904;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r10.u32);
	// stw r11,48(r31)
	PPC_STORE_U32(var_r31 + 48, ctx.r11.u32);
	// lbz r8,10(r9)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + 10);
	// stw r30,56(r31)
	PPC_STORE_U32(var_r31 + 56, var_r30);
	// stw r11,60(r31)
	PPC_STORE_U32(var_r31 + 60, ctx.r11.u32);
	// stw r11,64(r31)
	PPC_STORE_U32(var_r31 + 64, ctx.r11.u32);
	// stw r11,68(r31)
	PPC_STORE_U32(var_r31 + 68, ctx.r11.u32);
	// stb r8,52(r31)
	PPC_STORE_U8(var_r31 + 52, ctx.r8.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_12_D458_1"))) PPC_WEAK_FUNC(atSingleton_vfn_12_D458_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_12_D458_1) {
	PPC_FUNC_PROLOGUE();
	// lbz r3,52(r3)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r3.u32 + 52);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_1_D460_1"))) PPC_WEAK_FUNC(atSingleton_vfn_1_D460_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_1_D460_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,56(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d4c0
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r6,8(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
	}
loc_8245D4C0:
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// stw r3,64(r31)
	PPC_STORE_U32(var_r31 + 64, ctx.r3.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_15_D4E0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_15_D4E0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_15_D4E0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// bne cr6,0x8245d50c
	if ((int32_t)var_r30 == 0) {
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// stw r3,68(r31)
		PPC_STORE_U32(var_r31 + 68, ctx.r3.u32);
	}
loc_8245D50C:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,56(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d560
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// cntlzw r7,r30
		ctx.r7.u64 = var_r30 == 0 ? 32 : __builtin_clz(var_r30);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// rlwinm r4,r7,27,31,31
		ctx.r4.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x1;
		// lwz r10,12(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		// b 0x8245d564
	} else {
	loc_8245D560:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_8245D564:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_13_D580_1"))) PPC_WEAK_FUNC(atSingleton_vfn_13_D580_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_13_D580_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d5d8
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r3,28(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		// blr
		return;
	}
loc_8245D5D8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_4_D5F0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_4_D5F0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_4_D5F0_1) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x8245d600
	if (ctx.r4.s32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_8245D600:
	// lbz r9,40(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 40);
	// lwz r10,60(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 60);
	// rlwimi r9,r11,5,26,26
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r11.u32, 5) & 0x20) | (ctx.r9.u64 & 0xFFFFFFFFFFFFFFDF);
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// mr r7,r11
	ctx.r7.u64 = ctx.r11.u64;
	// stb r11,40(r3)
	PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r11.u8);
	// lwz r11,64(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 64);
	// cntlzw r6,r11
	ctx.r6.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r6,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0x1;
	// rlwimi r7,r11,6,25,25
	ctx.r7.u64 = (__builtin_rotateleft32(ctx.r11.u32, 6) & 0x40) | (ctx.r7.u64 & 0xFFFFFFFFFFFFFFBF);
	// stb r7,40(r3)
	PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r7.u8);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_8_D630_1"))) PPC_WEAK_FUNC(atSingleton_vfn_8_D630_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_8_D630_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,92(r11)
	// bctrl
	atSingleton_vfn_23(ctx, base);  // vtable slot 23 (byte +92)  // atSingleton::vfn_23
	// lwz r9,48(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 48);
	// cmpwi cr6,r9,0
	// beq cr6,0x8245d670
	if (ctx.r9.s32 != 0) {
		// lwz r8,44(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r7,16(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		// lwz r3,20(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 20);
		// bl 0x82454300
		atSingleton_4300_g(ctx, base);
	}
loc_8245D670:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_10_D688_1"))) PPC_WEAK_FUNC(atSingleton_vfn_10_D688_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_10_D688_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r29,20(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,56(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lwz r8,28(r9)
	// bctrl
	atSingleton_vfn_7(ctx, base);  // vtable slot 7 (byte +28)  // atSingleton::vfn_7
	// add r11,r29,r3
	ctx.r11.u64 = var_r29 + ctx.r3.u64;
	// cmpw cr6,r11,r30
	// bge cr6,0x8245d70c
	if (ctx.r11.s32 < (int32_t)var_r30) {
		// lwz r7,0(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r29,20(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
		// lwz r6,56(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
		// lwz r4,32(r5)
		// bctrl
		atSingleton_vfn_8(ctx, base);  // vtable slot 8 (byte +32)  // atSingleton::vfn_8
		// subf r11,r30,r29
		ctx.r11.s64 = (int64_t)(int32_t)var_r29 - (int64_t)(int32_t)var_r30;
		// add r3,r11,r3
		ctx.r3.u64 = ctx.r11.u64 + ctx.r3.u64;
		return;
	}
loc_8245D70C:
	// subf r3,r30,r11
	ctx.r3.s64 = ctx.r11.s64 - (int64_t)(int32_t)var_r30;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_28_D718_1"))) PPC_WEAK_FUNC(atSingleton_vfn_28_D718_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_28_D718_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,21776
	ctx.r11.s64 = ctx.r11.s64 + 21776;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245d764
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32867
		ctx.r4.u64 = ctx.r4.u64 | 32867;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245D764:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_0_D780_1"))) PPC_WEAK_FUNC(atSingleton_vfn_0_D780_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_0_D780_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r11,r11,21904
	ctx.r11.s64 = ctx.r11.s64 + 21904;
	// lwz r3,60(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 60);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x8245d7c8
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r10,0(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	}
loc_8245D7C8:
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r11,r11,21776
	ctx.r11.s64 = ctx.r11.s64 + 21776;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r9,r30,31
	ctx.r9.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245d7fc
	if (ctx.r9.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32781
		ctx.r4.u64 = ctx.r4.u64 | 32781;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245D7FC:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_2_D818_1"))) PPC_WEAK_FUNC(atSingleton_vfn_2_D818_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_2_D818_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r7,16(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lbz r4,9(r7)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r7.u32 + 9);
	// lwz r9,20(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// lwz r8,48(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 48);
	// lwz r3,284(r8)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 284);
	// bl 0x82450090
	atSingleton_0090_g(ctx, base);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// cmplwi cr6,r29,0
	// beq cr6,0x8245d94c
	if (var_r29 != 0) {
		// lwz r6,252(r29)
		ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 252);
		// cmpwi cr6,r6,0
		// beq cr6,0x8245d93c
		if (ctx.r6.s32 != 0) {
			// lwz r4,0(r29)
			ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 0)/* atSingleton::vtable@+0x0 */;
			// addi r6,r1,80
			ctx.r6.s64 = ctx.r1.s64 + 80;
			// lwz r11,16(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
			// mr r5,r31
			ctx.r5.u64 = var_r31;
			// lwz r10,36(r4)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 36);
			// lhz r4,7(r11)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 7);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
			// cmpwi cr6,r3,0
			// blt cr6,0x8245d908
			if (ctx.r3.s32 >= 0) {
				// lwz r9,0(r31)
				ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// lwz r8,48(r9)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 48);
				// bctrl
				PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
				// mr r4,r3
				ctx.r4.u64 = ctx.r3.u64;
				// lwz r3,80(r1)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				// mr r6,r30
				ctx.r6.u64 = var_r30;
				// li r5,0
				ctx.r5.s64 = 0;
				// lwz r11,4(r7)
				// bctrl
				VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
				// mr r30,r3
				var_r30 = ctx.r3.u32;
				// cmpwi cr6,r30,0
				// blt cr6,0x8245d910
				if ((int32_t)var_r30 < 0) goto loc_8245D910;
				// lwz r10,0(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// lwz r4,80(r1)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				// lwz r9,100(r10)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 100);
				// bctrl
				PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
				// lwz r10,80(r1)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				// addi r11,r29,240
				ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 240;
				// stw r11,4(r10)
				PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
				// lwz r8,8(r11)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
				// stw r8,8(r10)
				PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r8.u32);
				// lwz r7,8(r11)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
				// stw r10,4(r7)
				PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r10.u32);
				// stw r10,8(r11)
				PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
			loc_8245D904:
				// mr r3,r30
				ctx.r3.u64 = var_r30;
			}
		loc_8245D908:
			return;
		loc_8245D910:
			// lwz r3,80(r1)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// cmplwi cr6,r3,0
			// beq cr6,0x8245d904
			if (ctx.r3.u32 == 0) goto loc_8245D904;
			// lwz r6,0(r3)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
			// li r4,1
			ctx.r4.s64 = 1;
			// lwz r5,0(r6)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			return;
		}
	loc_8245D93C:
		// lis r3,-30009
		// ori r3,r3,22
		ctx.r3.u64 = ctx.r3.u64 | 22;
		return;
	}
loc_8245D94C:
	// lis r3,-30009
	// ori r3,r3,19
	ctx.r3.u64 = ctx.r3.u64 | 19;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_3_D960_1"))) PPC_WEAK_FUNC(atSingleton_vfn_3_D960_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_3_D960_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r7,16(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lbz r4,9(r7)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r7.u32 + 9);
	// lwz r9,20(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// lwz r8,48(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 48);
	// lwz r3,284(r8)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 284);
	// bl 0x82450090
	atSingleton_0090_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245d9f4
	if (ctx.r3.u32 != 0) {
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lbz r6,10(r11)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 10);
		// cmplwi cr6,r6,255
		// bne cr6,0x8245d9c0
		if (ctx.r6.u32 == 255) {
			// lis r3,32767
			ctx.r3.s64 = 2147418112;
			// ori r3,r3,65535
			ctx.r3.u64 = ctx.r3.u64 | 65535;
			// b 0x8245d9f8
			// blr
			return;
		}
	loc_8245D9C0:
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// lhz r4,7(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 7);
		// addi r5,r1,84
		ctx.r5.s64 = ctx.r1.s64 + 84;
		// bl 0x82455f40
		msgMsgSink_5F40_g(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x8245d9f4
		if (ctx.r3.s32 < 0) goto loc_8245D9F4;
		// lwz r5,16(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r3,80(r1)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// lwz r10,84(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lbz r4,10(r5)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r5.u32 + 10);
		// mullw r11,r4,r3
		ctx.r11.s64 = int64_t(ctx.r4.s32) * int64_t(ctx.r3.s32);
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// b 0x8245d9f8
	} else {
	loc_8245D9F4:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_8245D9F8:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_22_DA10_1"))) PPC_WEAK_FUNC(atSingleton_vfn_22_DA10_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_22_DA10_1) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r10,r10,27
	ctx.r10.u64 = ctx.r10.u32 & 0x1F;
	// cmpwi cr6,r10,4
	// bne cr6,0x8245da2c
	if (ctx.r10.s32 == 4) {
		// addi r3,r11,11
		ctx.r3.s64 = ctx.r11.s64 + 11;
		// blr
		return;
	}
loc_8245DA2C:
	// cmpwi cr6,r10,5
	// addi r3,r11,15
	ctx.r3.s64 = ctx.r11.s64 + 15;
	// beqlr cr6
	if (ctx.r10.s32 == 5) return;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_DA40_h"))) PPC_WEAK_FUNC(jumptable_DA40_h);
PPC_FUNC_IMPL(__imp__jumptable_DA40_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r11,r11,22028
	ctx.r11.s64 = ctx.r11.s64 + 22028;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_31_DA80_1"))) PPC_WEAK_FUNC(atSingleton_vfn_31_DA80_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_31_DA80_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22028
	ctx.r11.s64 = ctx.r11.s64 + 22028;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245dacc
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32783
		ctx.r4.u64 = ctx.r4.u64 | 32783;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245DACC:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_DAE8_v12"))) PPC_WEAK_FUNC(msgMsgSink_DAE8_v12);
PPC_FUNC_IMPL(__imp__msgMsgSink_DAE8_v12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r7
	var_r30 = ctx.r7.u32;
	// mr r7,r8
	ctx.r7.u64 = ctx.r8.u64;
	// mr r8,r9
	ctx.r8.u64 = ctx.r9.u64;
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8245f998
	jumptable_F998(ctx, base);
	// lis r11,-32256
	// lwz r9,196(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 196);
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,22052
	ctx.r11.s64 = ctx.r11.s64 + 22052;
	// stw r30,60(r31)
	PPC_STORE_U32(var_r31 + 60, var_r30);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,64(r31)
	PPC_STORE_U32(var_r31 + 64, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// stw r9,68(r31)
	PPC_STORE_U32(var_r31 + 68, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_DB50_2h"))) PPC_WEAK_FUNC(atSingleton_DB50_2h);
PPC_FUNC_IMPL(__imp__atSingleton_DB50_2h) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// addi r11,r11,22052
	ctx.r11.s64 = ctx.r11.s64 + 22052;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// b 0x8245fa08
	atSingleton_FA08_g(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_37_DB60_1"))) PPC_WEAK_FUNC(atSingleton_vfn_37_DB60_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_37_DB60_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22052
	ctx.r11.s64 = ctx.r11.s64 + 22052;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x8245fa08
	atSingleton_FA08_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245dbac
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32779
		ctx.r4.u64 = ctx.r4.u64 | 32779;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245DBAC:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_40"))) PPC_WEAK_FUNC(atSingleton_vfn_40);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_40) {
	PPC_FUNC_PROLOGUE();
	// lhz r11,56(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 56);
	// cmplwi cr6,r11,0
	// beq cr6,0x8245dbe8
	if (ctx.r11.u32 != 0) {
		// lwz r11,52(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 52);
		// lhz r10,2(r11)
		ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 2);
		// lhz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
		// mullw r3,r10,r9
		ctx.r3.s64 = int64_t(ctx.r10.s32) * int64_t(ctx.r9.s32);
		// blr
		return;
	}
loc_8245DBE8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_DBF0_fw"))) PPC_WEAK_FUNC(atSingleton_DBF0_fw);
PPC_FUNC_IMPL(__imp__atSingleton_DBF0_fw) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=128, manual
	// lwz r11,44(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 44);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r9,20(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// lwz r10,48(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 48);
	// lwz r7,284(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 284);
	// cmplwi cr6,r7,0
	// beq cr6,0x8245dcb4
	if (ctx.r7.u32 != 0) {
		// addi r11,r1,81
		ctx.r11.s64 = ctx.r1.s64 + 81;
		// li r9,0
		ctx.r9.s64 = 0;
		// li r8,28
		ctx.r8.s64 = 28;
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_8245DC28:
		// stb r9,0(r11)
		PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// bdnz 0x8245dc28
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_8245DC28;
		// lhz r11,292(r10)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r10.u32 + 292);
		// lwz r6,104(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
		// lwz r8,16(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// rlwinm r9,r11,24,8,31
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 24) & 0xFFFFFF;
		// rldicr r5,r6,8,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 8);
		// stb r10,98(r1)
		PPC_STORE_U8(ctx.r1.u32 + 98, ctx.r10.u8);
		// srawi r6,r10,24
		ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFFFFFF) != 0);
		ctx.r6.s64 = ctx.r10.s32 >> 24;
		// stb r4,80(r1)
		PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r4.u8);
		// stb r11,90(r1)
		PPC_STORE_U8(ctx.r1.u32 + 90, ctx.r11.u8);
		// rldicl r11,r10,56,8
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u64, 56) & 0xFFFFFFFFFFFFFF;
		// lbz r10,108(r1)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 108);
		// srawi r4,r7,24
		ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0xFFFFFF) != 0);
		ctx.r4.s64 = ctx.r7.s32 >> 24;
		// lwz r8,7(r8)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + 7);
		// stb r9,89(r1)
		PPC_STORE_U8(ctx.r1.u32 + 89, ctx.r9.u8);
		// or r9,r10,r5
		ctx.r9.u64 = ctx.r10.u64 | ctx.r5.u64;
		// stb r6,95(r1)
		PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r6.u8);
		// rldicl r5,r8,56,8
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r8.u64, 56) & 0xFFFFFFFFFFFFFF;
		// sth r11,96(r1)
		PPC_STORE_U16(ctx.r1.u32 + 96, ctx.r11.u16);
		// rlwinm r6,r8,8,24,31
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 8) & 0xFF;
		// rldicl r11,r7,56,8
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u64, 56) & 0xFFFFFFFFFFFFFF;
		// stb r7,94(r1)
		PPC_STORE_U8(ctx.r1.u32 + 94, ctx.r7.u8);
		// rldicr r7,r9,24,39
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u64, 24) & 0xFFFFFFFFFF000000;
		// stb r4,91(r1)
		PPC_STORE_U8(ctx.r1.u32 + 91, ctx.r4.u8);
		// stb r8,102(r1)
		PPC_STORE_U8(ctx.r1.u32 + 102, ctx.r8.u8);
		// sth r5,100(r1)
		PPC_STORE_U16(ctx.r1.u32 + 100, ctx.r5.u16);
		// stb r6,99(r1)
		PPC_STORE_U8(ctx.r1.u32 + 99, ctx.r6.u8);
		// sth r11,92(r1)
		PPC_STORE_U16(ctx.r1.u32 + 92, ctx.r11.u16);
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// ld r4,80(r1)
		ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// ld r5,88(r1)
		ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// ld r6,96(r1)
		ctx.r6.u64 = PPC_LOAD_U64(ctx.r1.u32 + 96);
		// bl 0x82449980
		atSingleton_9980(ctx, base);
	}
loc_8245DCB4:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_38"))) PPC_WEAK_FUNC(atSingleton_vfn_38);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_38) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8245fa18
	atSingleton_vfn_70(ctx, base);
	// lwz r11,60(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 60);
	// cmplwi cr6,r11,1
	// blt cr6,0x8245dd04
	if (ctx.r11.u32 >= 1) {
		// bne cr6,0x8245dd10
		if (!ctx.cr6.eq) goto loc_8245DD10;
		// lwz r11,68(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 68);
		// lbz r10,24(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 24);
		// ori r9,r10,8
		ctx.r9.u64 = ctx.r10.u64 | 8;
		// stb r9,24(r11)
		PPC_STORE_U8(ctx.r11.u32 + 24, ctx.r9.u8);
		// b 0x8245dd10
	} else {
	loc_8245DD04:
		// li r4,5
		ctx.r4.s64 = 5;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8245dbf0
		atSingleton_DBF0_fw(ctx, base);
	}
loc_8245DD10:
	// lwz r8,64(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 64);
	// cmpwi cr6,r8,0
	// beq cr6,0x8245dd48
	if (ctx.r8.s32 != 0) {
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r6,44(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 44);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// stw r7,64(r31)
		PPC_STORE_U32(var_r31 + 64, ctx.r7.u32);
		// lwz r5,16(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
		// lwz r3,20(r5)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 20);
		// lwz r3,48(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 48);
		// lwz r10,92(r11)
		// bctrl
		VCALL(ctx.r3.u32, 23, ctx, base);  // vtable slot 23 (byte +92)
	}
loc_8245DD48:
	// lhz r9,56(r31)
	ctx.r9.u64 = PPC_LOAD_U16(var_r31 + 56);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245dd70
	if (ctx.r9.u32 != 0) {
		// bl 0x8245fab8
		atSingleton_FAB8_g(ctx, base);
		// blr
		return;
	}
loc_8245DD70:
	// bl 0x82452378
	atSingleton_2378_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_DD88_h"))) PPC_WEAK_FUNC(jumptable_DD88_h);
PPC_FUNC_IMPL(__imp__jumptable_DD88_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// li r9,0
	ctx.r9.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r6
	var_r30 = ctx.r6.u32;
	// bl 0x8245f998
	jumptable_F998(ctx, base);
	// lis r11,-32256
	// addi r4,r30,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 8;
	// addi r11,r11,22072
	ctx.r11.s64 = ctx.r11.s64 + 22072;
	// addi r3,r31,60
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 60;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x8245fc40
	jumptable_FC40(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_42"))) PPC_WEAK_FUNC(atSingleton_vfn_42);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_42) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22072
	ctx.r11.s64 = ctx.r11.s64 + 22072;
	// addi r3,r31,60
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 60;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8245fa08
	atSingleton_FA08_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245de38
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32786
		ctx.r4.u64 = ctx.r4.u64 | 32786;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245DE38:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_43_DE50_1"))) PPC_WEAK_FUNC(atSingleton_vfn_43_DE50_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_43_DE50_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=128, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,1
	var_r30 = 1;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lbz r10,7(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 7);
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245deec
	if (ctx.r9.u32 != 0) {
		// addi r3,r31,60
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 60;
		// lfs f0,64(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 64);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,80(r1)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		// bl 0x8245fc80
		atSingleton_FC80_g(ctx, base);
		// cntlzw r8,r3
		ctx.r8.u64 = ctx.r3.u32 == 0 ? 32 : __builtin_clz(ctx.r3.u32);
		// rlwinm r30,r8,27,31,31
		var_r30 = (uint32_t)(__builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0x1);
		// cmpwi cr6,r30,0
		// bne cr6,0x8245df68
		if ((int32_t)var_r30 != 0) goto loc_8245DF68;
		// lwz r7,12(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 12);
		// li r6,0
		ctx.r6.s64 = 0;
		// lwz r3,32(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 32);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r11,44(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r5,24(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 24);
		// stw r6,32(r31)
		PPC_STORE_U32(var_r31 + 32, ctx.r6.u32);
		// subf r10,r3,r5
		ctx.r10.s64 = ctx.r5.s64 - ctx.r3.s64;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// stw r10,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r10.u32);
		// lbz r10,44(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 44);
		// addi r9,r10,1
		ctx.r9.s64 = ctx.r10.s64 + 1;
		// stb r9,44(r11)
		PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r9.u8);
		// lwz r7,44(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r6,16(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
		// lwz r3,20(r6)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 20);
		// bl 0x824545d8
		atSingleton_45D8_g(ctx, base);
		// b 0x8245df68
	} else {
	loc_8245DEEC:
		// lwz r5,44(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lis r11,-32256
		// lwz r4,16(r5)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
		// lfs f0,15784(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
		ctx.f0.f64 = double(temp.f32);
		// lfs f1,40(r4)
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 40);
		ctx.f1.f64 = double(temp.f32);
		// fcmpu cr6,f1,f0
		// bgt cr6,0x8245df14
		if (ctx.f1.f64 <= ctx.f0.f64) {
			// bso cr6,0x8245df14
			// UNIMPLEMENTED: bso
			PPC_UNIMPLEMENTED(0x8245DF08, "bso");
			// li r11,-9600
			// b 0x8245df34
		} else {
		loc_8245DF14:
			// bl 0x82432450
			jumptable_2450(ctx, base);
			// lis r11,-32256
			// addi r3,r1,80
			ctx.r3.s64 = ctx.r1.s64 + 80;
			// lfd f0,18968(r11)
			ctx.fpscr.disableFlushMode();
			ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18968);
			// fmul f13,f1,f0
			ctx.f13.f64 = ctx.f1.f64 * ctx.f0.f64;
			// fctiwz f12,f13
			ctx.f12.s64 = (ctx.f13.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f13.f64));
			// stfiwx f12,0,r3
			PPC_STORE_U32(ctx.r3.u32, ctx.f12.u32);
			// lwz r11,80(r1)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		}
	loc_8245DF34:
		// extsw r10,r11
		ctx.r10.s64 = ctx.r11.s32;
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// addi r5,r11,8
		ctx.r5.s64 = ctx.r11.s64 + 8;
		// lis r11,-32256
		// std r10,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r10.u64);
		// lfd f11,88(r1)
		ctx.fpscr.disableFlushMode();
		ctx.f11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// fcfid f10,f11
		ctx.f10.f64 = double(ctx.f11.s64);
		// lfs f1,22092(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22092);
		ctx.f1.f64 = double(temp.f32);
		// frsp f9,f10
		ctx.f9.f64 = double(float(ctx.f10.f64));
		// stfs f9,80(r1)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		// bl 0x82448170
		atSingleton_8170_g(ctx, base);
	}
loc_8245DF68:
	// addi r9,r1,88
	ctx.r9.s64 = ctx.r1.s64 + 88;
	// lfs f8,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f8.f64 = double(temp.f32);
	// fctiwz f7,f8
	ctx.f7.s64 = (ctx.f8.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f8.f64));
	// stfiwx f7,0,r9
	PPC_STORE_U32(ctx.r9.u32, ctx.f7.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// cmpwi cr6,r11,6400
	// ble cr6,0x8245df88
	if (ctx.r11.s32 > 6400) {
		// li r11,6400
		ctx.r11.s64 = 6400;
	}
loc_8245DF88:
	// extsw r8,r11
	ctx.r8.s64 = ctx.r11.s32;
	// lis r11,-32248
	// std r8,88(r1)
	PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r8.u64);
	// lfd f6,88(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f6.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
	// fcfid f5,f6
	ctx.f5.f64 = double(ctx.f6.s64);
	// lfd f1,-25752(r11)
	ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -25752);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfd f0,18976(r11)
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18976);  /* glob:lbl_82004A20 @ 0x82004a20 */
	// fmul f2,f5,f0
	ctx.f2.f64 = ctx.f5.f64 * ctx.f0.f64;
	// bl 0x82431308
	atSingleton_1308_g(ctx, base);
	// lwz r7,44(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 44);
	// frsp f1,f1
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = double(float(ctx.f1.f64));
	// lwz r3,16(r7)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
	// bl 0x8245a408
	atSingleton_A408_g(ctx, base);
	// lhz r6,56(r31)
	ctx.r6.u64 = PPC_LOAD_U16(var_r31 + 56);
	// cmplwi cr6,r6,0
	// beq cr6,0x8245dfd8
	if (ctx.r6.u32 != 0) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8245fab8
		atSingleton_FAB8_g(ctx, base);
		// b 0x8245dfe8
	} else {
	loc_8245DFD8:
		// cmpwi cr6,r30,0
		// beq cr6,0x8245dfe8
		if ((int32_t)var_r30 == 0) {
			// blr
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82452378
		atSingleton_2378_g(ctx, base);
	}
loc_8245DFE8:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_44"))) PPC_WEAK_FUNC(atSingleton_vfn_44);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_44) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	double var_f29 = 0.0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=192, savegprlr_25
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r25,0
	var_r25 = 0;
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// lwz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// rlwinm r29,r8,27,8,31
	var_r29 = (uint32_t)(__builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0xFFFFFF);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// mr r26,r30
	var_r26 = (uint32_t)(var_r30);
	// cmpw cr6,r29,r30
	// bge cr6,0x8245e270
	if ((int32_t)var_r29 < (int32_t)var_r30) {
		// add r7,r3,r29
		ctx.r7.u64 = ctx.r3.u64 + var_r29;
		// cmpw cr6,r7,r30
		// blt cr6,0x8245e270
		if (ctx.r7.s32 < (int32_t)var_r30) {
			// mr r3,r25
			ctx.r3.u64 = var_r25;
			// stw r26,32(r31)
			PPC_STORE_U32(var_r31 + 32, var_r26);
			return;
		}
		// lhz r27,56(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U16(var_r31 + 56));
		// li r28,1
		var_r28 = 1;
		// cmplwi cr6,r27,0
		// beq cr6,0x8245e0c0
		if (var_r27 != 0) {
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8245fb68
			atSingleton_vfn_39(ctx, base);
			// lwz r11,52(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 52);
			// mr r25,r3
			var_r25 = ctx.r3.u32;
			// lhz r5,0(r11)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
			// cmplwi cr6,r5,65535
			// beq cr6,0x8245e09c
			if (ctx.r5.u32 != 65535) {
				// lhz r4,56(r31)
				ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 56);
				// clrlwi r3,r27,16
				ctx.r3.u64 = var_r27 & 0xFFFF;
				// subf r11,r4,r3
				ctx.r11.s64 = ctx.r3.s64 - ctx.r4.s64;
				// clrlwi r28,r11,16
				var_r28 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
				// b 0x8245e0c0
			} else {
			loc_8245E09C:
				// subf r10,r29,r30
				ctx.r10.s64 = (int64_t)(int32_t)var_r30 - (int64_t)(int32_t)var_r29;
				// lhz r9,2(r11)
				ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 2);
				// rotlwi r11,r10,1
				ctx.r11.u64 = __builtin_rotateleft32(ctx.r10.u32, 1);
				// divw r7,r10,r9
				ctx.r7.s32 = ctx.r9.s32 ? ctx.r10.s32 / ctx.r9.s32 : 0;
				// addi r8,r11,-1
				ctx.r8.s64 = ctx.r11.s64 + -1;
				// twllei r9,0
				if (ctx.r9.s32 == 0 || ctx.r9.u32 < 0u) __builtin_trap();
				// andc r6,r9,r8
				ctx.r6.u64 = ctx.r9.u64 & ~ctx.r8.u64;
				// clrlwi r28,r7,16
				var_r28 = (uint32_t)(ctx.r7.u32 & 0xFFFF);
				// twlgei r6,-1
				if (ctx.r6.s32 == -1 || ctx.r6.u32 > 4294967295u) __builtin_trap();
			}
		}
	loc_8245E0C0:
		// lwz r5,16(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
		// subf r11,r29,r30
		ctx.r11.s64 = (int64_t)(int32_t)var_r30 - (int64_t)(int32_t)var_r29;
		// lbz r4,7(r5)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r5.u32 + 7);
		// clrlwi r3,r4,31
		ctx.r3.u64 = ctx.r4.u32 & 0x1;
		// cmplwi cr6,r3,0
		// beq cr6,0x8245e178
		if (ctx.r3.u32 != 0) {
			// lis r10,-13108
			ctx.r10.s64 = -859045888;
			// ori r9,r10,52429
			ctx.r9.u64 = ctx.r10.u64 | 52429;
			// mulhwu r8,r11,r9
			ctx.r8.u64 = (uint64_t(ctx.r11.u32) * uint64_t(ctx.r9.u32)) >> 32;
			// rlwinm r28,r8,30,2,31
			var_r28 = (uint32_t)(__builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF);
			// cmplwi cr6,r28,0
			// beq cr6,0x8245e16c
			if (var_r28 != 0) {
				// lis r10,-32248
				// lis r11,-32256
				// addi r29,r31,60
				var_r29 = (uint32_t)(var_r31 + 60);
				// mr r30,r28
				var_r30 = (uint32_t)(var_r28);
				// lfd f30,-25752(r10)
				ctx.fpscr.disableFlushMode();
				ctx.f30.u64 = PPC_LOAD_U64(ctx.r10.u32 + -25752);
				// lfd f31,18976(r11)
				ctx.f31.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18976);
			loc_8245E108:
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// lfs f29,64(r31)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(var_r31 + 64);
				var_f29 = double(temp.f32);
				// bl 0x8245fc80
				atSingleton_FC80_g(ctx, base);
				// addi r7,r1,80
				ctx.r7.s64 = ctx.r1.s64 + 80;
				// fctiwz f0,f29
				ctx.fpscr.disableFlushMode();
				ctx.f0.s64 = (var_f29 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&var_f29));
				// stfiwx f0,0,r7
				PPC_STORE_U32(ctx.r7.u32, ctx.f0.u32);
				// lwz r11,80(r1)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
				// cmpwi cr6,r11,6400
				// ble cr6,0x8245e134
				if (ctx.r11.s32 > 6400) {
					// li r11,6400
					ctx.r11.s64 = 6400;
					// stw r11,80(r1)
					PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
				}
			loc_8245E134:
				// extsw r6,r11
				ctx.r6.s64 = ctx.r11.s32;
				// fmr f1,f30
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = var_f30;
				// std r6,88(r1)
				PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r6.u64);
				// lfd f13,88(r1)
				ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
				// fcfid f12,f13
				ctx.f12.f64 = double(ctx.f13.s64);
				// fmul f2,f12,f31
				ctx.f2.f64 = ctx.f12.f64 * var_f31;
				// bl 0x82431308
				atSingleton_1308_g(ctx, base);
				// lwz r5,44(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 44);
				// frsp f1,f1
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = double(float(ctx.f1.f64));
				// lwz r3,16(r5)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
				// bl 0x8245a408
				atSingleton_A408_g(ctx, base);
				// addi r30,r30,-1
				var_r30 = (uint32_t)(var_r30 + -1);
				// cmplwi cr6,r30,0
				// bne cr6,0x8245e108
				if (var_r30 != 0) goto loc_8245E108;
			}
		loc_8245E16C:
			// rlwinm r11,r28,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFFFFFC;
			// add r26,r28,r11
			var_r26 = (uint32_t)(var_r28 + ctx.r11.u64);
			// b 0x8245e270
		} else {
		loc_8245E178:
			// lwz r4,44(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 44);
			// lis r11,-32256
			// lwz r3,16(r4)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
			// lfs f0,15784(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
			ctx.f0.f64 = double(temp.f32);
			// lfs f1,40(r3)
			temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 40);
			ctx.f1.f64 = double(temp.f32);
			// fcmpu cr6,f1,f0
			// bgt cr6,0x8245e1a0
			if (ctx.f1.f64 <= ctx.f0.f64) {
				// bso cr6,0x8245e1a0
				// UNIMPLEMENTED: bso
				PPC_UNIMPLEMENTED(0x8245E194, "bso");
				// li r11,-9600
				// b 0x8245e1c0
			} else {
			loc_8245E1A0:
				// bl 0x82432450
				jumptable_2450(ctx, base);
				// lis r11,-32256
				// addi r10,r1,80
				ctx.r10.s64 = ctx.r1.s64 + 80;
				// lfd f0,18968(r11)
				ctx.fpscr.disableFlushMode();
				ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18968);
				// fmul f11,f1,f0
				ctx.f11.f64 = ctx.f1.f64 * ctx.f0.f64;
				// fctiwz f10,f11
				ctx.f10.s64 = (ctx.f11.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f11.f64));
				// stfiwx f10,0,r10
				PPC_STORE_U32(ctx.r10.u32, ctx.f10.u32);
				// lwz r11,80(r1)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			}
		loc_8245E1C0:
			// extsw r9,r11
			ctx.r9.s64 = ctx.r11.s32;
			// clrlwi r29,r28,16
			var_r29 = (uint32_t)(var_r28 & 0xFFFF);
			// cmplwi cr6,r29,0
			// std r9,88(r1)
			PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r9.u64);
			// lfd f9,88(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f9.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
			// fcfid f8,f9
			ctx.f8.f64 = double(ctx.f9.s64);
			// frsp f7,f8
			ctx.f7.f64 = double(float(ctx.f8.f64));
			// stfs f7,80(r1)
			temp.f32 = float(ctx.f7.f64);
			PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
			// beq cr6,0x8245e218
			if (var_r29 != 0) {
				// lis r11,-32256
				// li r30,0
				var_r30 = 0;
				// lfs f31,15788(r11)
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
				var_f31 = double(temp.f32);
			loc_8245E1F0:
				// lwz r11,16(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
				// addi r4,r1,80
				ctx.r4.s64 = ctx.r1.s64 + 80;
				// lwz r3,12(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
				// fmr f1,f31
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = var_f31;
				// addi r5,r11,8
				ctx.r5.s64 = ctx.r11.s64 + 8;
				// bl 0x82448170
				atSingleton_8170_g(ctx, base);
				// addi r8,r30,1
				ctx.r8.s64 = (int64_t)(int32_t)var_r30 + 1;
				// clrlwi r30,r8,16
				var_r30 = (uint32_t)(ctx.r8.u32 & 0xFFFF);
				// cmplw cr6,r30,r29
				// blt cr6,0x8245e1f0
				if (var_r30 < var_r29) goto loc_8245E1F0;
			}
		loc_8245E218:
			// addi r7,r1,88
			ctx.r7.s64 = ctx.r1.s64 + 88;
			// lfs f6,80(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f6.f64 = double(temp.f32);
			// fctiwz f5,f6
			ctx.f5.s64 = (ctx.f6.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f6.f64));
			// stfiwx f5,0,r7
			PPC_STORE_U32(ctx.r7.u32, ctx.f5.u32);
			// lwz r11,88(r1)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			// cmpwi cr6,r11,6400
			// ble cr6,0x8245e238
			if (ctx.r11.s32 > 6400) {
				// li r11,6400
				ctx.r11.s64 = 6400;
			}
		loc_8245E238:
			// extsw r6,r11
			ctx.r6.s64 = ctx.r11.s32;
			// lis r11,-32248
			// std r6,88(r1)
			PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r6.u64);
			// lfd f4,88(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
			// fcfid f3,f4
			ctx.f3.f64 = double(ctx.f4.s64);
			// lfd f1,-25752(r11)
			ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -25752);
			// lis r11,-32256
			ctx.r11.s64 = -2113929216;
			// lfd f0,18976(r11)
			ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 18976);  /* glob:lbl_82004A20 @ 0x82004a20 */
			// fmul f2,f3,f0
			ctx.f2.f64 = ctx.f3.f64 * ctx.f0.f64;
			// bl 0x82431308
			atSingleton_1308_g(ctx, base);
			// lwz r5,44(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 44);
			// frsp f1,f1
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = double(float(ctx.f1.f64));
			// lwz r3,16(r5)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
			// bl 0x8245a408
			atSingleton_A408_g(ctx, base);
		}
	}
loc_8245E270:
	// mr r3,r25
	ctx.r3.u64 = var_r25;
	// stw r26,32(r31)
	PPC_STORE_U32(var_r31 + 32, var_r26);
	return;
}

__attribute__((alias("__imp__jumptable_E290_h"))) PPC_WEAK_FUNC(jumptable_E290_h);
PPC_FUNC_IMPL(__imp__jumptable_E290_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// li r9,0
	ctx.r9.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r6
	var_r30 = ctx.r6.u32;
	// bl 0x8245f998
	jumptable_F998(ctx, base);
	// lis r11,-32256
	// addi r4,r30,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 8;
	// addi r11,r11,22096
	ctx.r11.s64 = ctx.r11.s64 + 22096;
	// addi r3,r31,60
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 60;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x8245fc40
	jumptable_FC40(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_0_E2E8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_0_E2E8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_0_E2E8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22096
	ctx.r11.s64 = ctx.r11.s64 + 22096;
	// addi r3,r31,60
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 60;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8245fa08
	atSingleton_FA08_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245e340
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32784
		ctx.r4.u64 = ctx.r4.u64 | 32784;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245E340:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_1_E358_1"))) PPC_WEAK_FUNC(atSingleton_vfn_1_E358_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_1_E358_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	double var_f30 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, var_r30);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f30.u64);
	// stfd f31,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,1
	var_r30 = 1;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lbz r11,7(r9)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7);
	// clrlwi r10,r11,31
	ctx.r10.u64 = ctx.r11.u32 & 0x1;
	// lis r11,-32256
	// cmplwi cr6,r10,0
	// lis r10,-32248
	// lfs f31,16056(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);
	var_f31 = double(temp.f32);
	// lfs f30,-25416(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -25416);
	var_f30 = double(temp.f32);
	// beq cr6,0x8245e40c
	if (ctx.r10.u32 != 0) {
		// addi r3,r31,60
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 60;
		// lfs f0,64(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 64);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,80(r1)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		// bl 0x8245fc80
		atSingleton_FC80_g(ctx, base);
		// cntlzw r9,r3
		ctx.r9.u64 = ctx.r3.u32 == 0 ? 32 : __builtin_clz(ctx.r3.u32);
		// rlwinm r30,r9,27,31,31
		var_r30 = (uint32_t)(__builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1);
		// cmpwi cr6,r30,0
		// bne cr6,0x8245e48c
		if ((int32_t)var_r30 != 0) goto loc_8245E48C;
		// lwz r8,12(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 12);
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r5,32(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 32);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r11,44(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r6,24(r8)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r8.u32 + 24);
		// stw r7,32(r31)
		PPC_STORE_U32(var_r31 + 32, ctx.r7.u32);
		// subf r10,r5,r6
		ctx.r10.s64 = ctx.r6.s64 - ctx.r5.s64;
		// addi r3,r10,1
		ctx.r3.s64 = ctx.r10.s64 + 1;
		// stw r3,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r3.u32);
		// lbz r10,44(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 44);
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// stb r10,44(r11)
		PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r10.u8);
		// lwz r8,44(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r7,16(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		// lwz r3,20(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 20);
		// bl 0x824545d8
		atSingleton_45D8_g(ctx, base);
		// b 0x8245e48c
	} else {
	loc_8245E40C:
		// lwz r6,44(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r5,16(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
		// lfs f0,44(r5)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 44);
		ctx.f0.f64 = double(temp.f32);
		// fcmpu cr6,f0,f31
		// blt cr6,0x8245e42c
		if (ctx.f0.f64 >= var_f31) {
			// bso cr6,0x8245e42c
			// UNIMPLEMENTED: bso
			PPC_UNIMPLEMENTED(0x8245E420, "bso");
			// li r11,2400
			ctx.r11.s64 = 2400;
			// b 0x8245e458
		} else {
		loc_8245E42C:
			// fcmpu cr6,f0,f30
			ctx.fpscr.disableFlushMode();
			// bgt cr6,0x8245e440
			if (ctx.f0.f64 <= var_f30) {
				// bso cr6,0x8245e440
				// UNIMPLEMENTED: bso
				PPC_UNIMPLEMENTED(0x8245E434, "bso");
				// li r11,-2400
				// b 0x8245e458
			} else {
			loc_8245E440:
				// lis r11,-32248
				ctx.r11.s64 = -2113404928;
				// lfs f13,-25396(r11)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25396);  /* glob:lbl_82079CCC @ 0x82079ccc */
				ctx.f13.f64 = double(temp.f32);
				// fmuls f13,f0,f13
				ctx.f13.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
				// fctiwz f12,f13
				ctx.f12.s64 = (ctx.f13.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f13.f64));
				// stfd f12,80(r1)
				PPC_STORE_U64(ctx.r1.u32 + 80, ctx.f12.u64);
				// lhz r11,86(r1)
				ctx.r11.u64 = PPC_LOAD_U16(ctx.r1.u32 + 86);
			}
		}
	loc_8245E458:
		// mr r4,r11
		ctx.r4.u64 = ctx.r11.u64;
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// lis r11,-32256
		// addi r5,r9,8
		ctx.r5.s64 = ctx.r9.s64 + 8;
		// lfs f1,22092(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22092);
		ctx.f1.f64 = double(temp.f32);
		// extsh r11,r4
		ctx.r11.s64 = ctx.r4.s16;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// std r11,80(r1)
		PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
		// lfd f11,80(r1)
		ctx.f11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// fcfid f10,f11
		ctx.f10.f64 = double(ctx.f11.s64);
		// frsp f9,f10
		ctx.f9.f64 = double(float(ctx.f10.f64));
		// stfs f9,80(r1)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		// bl 0x82448170
		atSingleton_8170_g(ctx, base);
	}
loc_8245E48C:
	// lfs f8,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f8.f64 = double(temp.f32);
	// fctiwz f7,f8
	ctx.f7.s64 = (ctx.f8.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f8.f64));
	// stfd f7,88(r1)
	PPC_STORE_U64(ctx.r1.u32 + 88, ctx.f7.u64);
	// lhz r11,94(r1)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r1.u32 + 94);
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// cmpwi cr6,r11,2400
	// blt cr6,0x8245e4b0
	if (ctx.r11.s32 >= 2400) {
		// fmr f1,f31
		ctx.f1.f64 = var_f31;
		// b 0x8245e4e0
	} else {
	loc_8245E4B0:
		// cmpwi cr6,r11,-2400
		// bgt cr6,0x8245e4c0
		if (ctx.r11.s32 <= -2400) {
			// fmr f1,f30
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = var_f30;
			// b 0x8245e4e0
		} else {
		loc_8245E4C0:
			// extsw r10,r11
			ctx.r10.s64 = ctx.r11.s32;
			// lis r11,-32256
			// std r10,88(r1)
			PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r10.u64);
			// lfs f0,18960(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18960);
			ctx.f0.f64 = double(temp.f32);
			// lfd f6,88(r1)
			ctx.f6.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
			// fcfid f5,f6
			ctx.f5.f64 = double(ctx.f6.s64);
			// frsp f4,f5
			ctx.f4.f64 = double(float(ctx.f5.f64));
			// fmuls f1,f4,f0
			ctx.f1.f64 = double(float(ctx.f4.f64 * ctx.f0.f64));
		}
	}
loc_8245E4E0:
	// lwz r9,44(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r3,16(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
	// bl 0x8245a490
	atSingleton_A490_g(ctx, base);
	// lhz r8,56(r31)
	ctx.r8.u64 = PPC_LOAD_U16(var_r31 + 56);
	// cmplwi cr6,r8,0
	// beq cr6,0x8245e504
	if (ctx.r8.u32 != 0) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8245fab8
		atSingleton_FAB8_g(ctx, base);
		// b 0x8245e514
	} else {
	loc_8245E504:
		// cmpwi cr6,r30,0
		// beq cr6,0x8245e514
		if ((int32_t)var_r30 == 0) {
			// addi r1,r1,144
			ctx.r1.s64 = ctx.r1.s64 + 144;
			// lwz r12,-8(r1)
			ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
			// mtlr r12
			ctx.lr = ctx.r12.u64;
			// lfd f30,-40(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
			// lfd f31,-32(r1)
			ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
			// ld r30,-24(r1)
			var_r30 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -24));
			// ld r31,-16(r1)
			var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
			// blr
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82452378
		atSingleton_2378_g(ctx, base);
	}
loc_8245E514:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f31,-32(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	var_r30 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -24));
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_45"))) PPC_WEAK_FUNC(atSingleton_vfn_45);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_45) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lbz r10,7(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 7);
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245e560
	if (ctx.r9.u32 != 0) {
		// lwz r8,60(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 60);
		// lhz r11,12(r8)
		ctx.r11.u64 = PPC_LOAD_U16(ctx.r8.u32 + 12);
		// rotlwi r10,r11,2
		ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 2);
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// blr
		return;
	}
loc_8245E560:
	// lhz r7,56(r3)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 56);
	// cmplwi cr6,r7,0
	// beq cr6,0x8245e580
	if (ctx.r7.u32 != 0) {
		// lwz r11,52(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 52);
		// lhz r6,2(r11)
		ctx.r6.u64 = PPC_LOAD_U16(ctx.r11.u32 + 2);
		// lhz r5,0(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
		// mullw r3,r6,r5
		ctx.r3.s64 = int64_t(ctx.r6.s32) * int64_t(ctx.r5.s32);
		// blr
		return;
	}
loc_8245E580:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_2_E588_1"))) PPC_WEAK_FUNC(atSingleton_vfn_2_E588_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_2_E588_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	double var_f28 = 0.0;
	double var_f29 = 0.0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8242f88c
	ctx.lr = 0x8245E590;
	__savegprlr_25(ctx, base);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82436618
	__savefpr_28(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r25,0
	var_r25 = 0;
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// lwz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// rlwinm r29,r8,27,8,31
	var_r29 = (uint32_t)(__builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0xFFFFFF);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// mr r26,r30
	var_r26 = (uint32_t)(var_r30);
	// cmpw cr6,r29,r30
	// bgt cr6,0x8245e7e4
	if ((int32_t)var_r29 <= (int32_t)var_r30) {
		// add r7,r3,r29
		ctx.r7.u64 = ctx.r3.u64 + var_r29;
		// cmpw cr6,r7,r30
		// blt cr6,0x8245e7e4
		if (ctx.r7.s32 < (int32_t)var_r30) {
			// mr r3,r25
			ctx.r3.u64 = var_r25;
			// stw r26,32(r31)
			PPC_STORE_U32(var_r31 + 32, var_r26);
			// addi r1,r1,192
			ctx.r1.s64 = ctx.r1.s64 + 192;
			// addi r12,r1,-64
			ctx.r12.s64 = ctx.r1.s64 + -64;
			// bl 0x82436664
			__restfpr_28(ctx, base);
			// b 0x8242f8dc
			__restgprlr_25(ctx, base);
			return;
		}
		// lhz r27,56(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U16(var_r31 + 56));
		// li r28,1
		var_r28 = 1;
		// cmplwi cr6,r27,0
		// beq cr6,0x8245e644
		if (var_r27 != 0) {
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8245fb68
			atSingleton_vfn_39(ctx, base);
			// lwz r11,52(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 52);
			// mr r25,r3
			var_r25 = ctx.r3.u32;
			// lhz r5,0(r11)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
			// cmplwi cr6,r5,65535
			// beq cr6,0x8245e620
			if (ctx.r5.u32 != 65535) {
				// lhz r4,56(r31)
				ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 56);
				// clrlwi r3,r27,16
				ctx.r3.u64 = var_r27 & 0xFFFF;
				// subf r11,r4,r3
				ctx.r11.s64 = ctx.r3.s64 - ctx.r4.s64;
				// clrlwi r28,r11,16
				var_r28 = (uint32_t)(ctx.r11.u32 & 0xFFFF);
				// b 0x8245e644
			} else {
			loc_8245E620:
				// subf r10,r29,r30
				ctx.r10.s64 = (int64_t)(int32_t)var_r30 - (int64_t)(int32_t)var_r29;
				// lhz r9,2(r11)
				ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 2);
				// rotlwi r11,r10,1
				ctx.r11.u64 = __builtin_rotateleft32(ctx.r10.u32, 1);
				// divw r7,r10,r9
				ctx.r7.s32 = ctx.r9.s32 ? ctx.r10.s32 / ctx.r9.s32 : 0;
				// addi r8,r11,-1
				ctx.r8.s64 = ctx.r11.s64 + -1;
				// twllei r9,0
				if (ctx.r9.s32 == 0 || ctx.r9.u32 < 0u) __builtin_trap();
				// andc r6,r9,r8
				ctx.r6.u64 = ctx.r9.u64 & ~ctx.r8.u64;
				// clrlwi r28,r7,16
				var_r28 = (uint32_t)(ctx.r7.u32 & 0xFFFF);
				// twlgei r6,-1
				if (ctx.r6.s32 == -1 || ctx.r6.u32 > 4294967295u) __builtin_trap();
			}
		}
	loc_8245E644:
		// lwz r5,16(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r4,20(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 20);
		// subf r11,r4,r30
		ctx.r11.s64 = (int64_t)(int32_t)var_r30 - ctx.r4.s64;
		// lbz r3,7(r5)
		ctx.r3.u64 = PPC_LOAD_U8(ctx.r5.u32 + 7);
		// clrlwi r10,r3,31
		ctx.r10.u64 = ctx.r3.u32 & 0x1;
		// cmplwi cr6,r10,0
		// beq cr6,0x8245e710
		if (ctx.r10.u32 != 0) {
			// lis r9,-13108
			ctx.r9.s64 = -859045888;
			// ori r8,r9,52429
			ctx.r8.u64 = ctx.r9.u64 | 52429;
			// mulhwu r7,r11,r8
			ctx.r7.u64 = (uint64_t(ctx.r11.u32) * uint64_t(ctx.r8.u32)) >> 32;
			// rlwinm r28,r7,30,2,31
			var_r28 = (uint32_t)(__builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 30) & 0x3FFFFFFF);
			// cmplwi cr6,r28,0
			// beq cr6,0x8245e704
			if (var_r28 != 0) {
				// lis r9,-32256
				// lis r10,-32248
				// lis r11,-32256
				// addi r29,r31,60
				var_r29 = (uint32_t)(var_r31 + 60);
				// mr r30,r28
				var_r30 = (uint32_t)(var_r28);
				// lfs f28,18960(r9)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 18960);
				var_f28 = double(temp.f32);
				// lfs f29,-25416(r10)
				temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -25416);
				var_f29 = double(temp.f32);
				// lfs f30,16056(r11)
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);
				var_f30 = double(temp.f32);
			loc_8245E698:
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// lfs f31,64(r31)
				ctx.fpscr.disableFlushMode();
				temp.u32 = PPC_LOAD_U32(var_r31 + 64);
				var_f31 = double(temp.f32);
				// bl 0x8245fc80
				atSingleton_FC80_g(ctx, base);
				// fctiwz f0,f31
				ctx.fpscr.disableFlushMode();
				ctx.f0.s64 = (var_f31 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&var_f31));
				// stfd f0,80(r1)
				PPC_STORE_U64(ctx.r1.u32 + 80, ctx.f0.u64);
				// lhz r11,86(r1)
				ctx.r11.u64 = PPC_LOAD_U16(ctx.r1.u32 + 86);
				// extsh r11,r11
				ctx.r11.s64 = ctx.r11.s16;
				// cmpwi cr6,r11,2400
				// blt cr6,0x8245e6c4
				if (ctx.r11.s32 >= 2400) {
					// fmr f1,f30
					ctx.f1.f64 = var_f30;
					// b 0x8245e6ec
				} else {
				loc_8245E6C4:
					// cmpwi cr6,r11,-2400
					// bgt cr6,0x8245e6d4
					if (ctx.r11.s32 <= -2400) {
						// fmr f1,f29
						ctx.fpscr.disableFlushMode();
						ctx.f1.f64 = var_f29;
						// b 0x8245e6ec
					} else {
					loc_8245E6D4:
						// extsw r6,r11
						ctx.r6.s64 = ctx.r11.s32;
						// std r6,88(r1)
						PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r6.u64);
						// lfd f13,88(r1)
						ctx.fpscr.disableFlushMode();
						ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
						// fcfid f12,f13
						ctx.f12.f64 = double(ctx.f13.s64);
						// frsp f11,f12
						ctx.f11.f64 = double(float(ctx.f12.f64));
						// fmuls f1,f11,f28
						ctx.f1.f64 = double(float(ctx.f11.f64 * var_f28));
					}
				}
			loc_8245E6EC:
				// lwz r5,44(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 44);
				// lwz r3,16(r5)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
				// bl 0x8245a490
				atSingleton_A490_g(ctx, base);
				// addi r30,r30,-1
				var_r30 = (uint32_t)(var_r30 + -1);
				// cmplwi cr6,r30,0
				// bne cr6,0x8245e698
				if (var_r30 != 0) goto loc_8245E698;
			}
		loc_8245E704:
			// rlwinm r11,r28,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFFFFFC;
			// add r26,r28,r11
			var_r26 = (uint32_t)(var_r28 + ctx.r11.u64);
			// b 0x8245e7e4
		} else {
		loc_8245E710:
			// lwz r4,44(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 44);
			// lwz r3,12(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
			// lwz r11,16(r4)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
			// lfs f1,44(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 44);
			ctx.f1.f64 = double(temp.f32);
			// bl 0x82452278
			atSingleton_2278_w(ctx, base);
			// extsh r9,r3
			ctx.r9.s64 = ctx.r3.s16;
			// clrlwi r29,r28,16
			var_r29 = (uint32_t)(var_r28 & 0xFFFF);
			// cmplwi cr6,r29,0
			// std r9,88(r1)
			PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r9.u64);
			// lfd f10,88(r1)
			ctx.fpscr.disableFlushMode();
			ctx.f10.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
			// fcfid f9,f10
			ctx.f9.f64 = double(ctx.f10.s64);
			// frsp f8,f9
			ctx.f8.f64 = double(float(ctx.f9.f64));
			// stfs f8,80(r1)
			temp.f32 = float(ctx.f8.f64);
			PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
			// beq cr6,0x8245e77c
			if (var_r29 != 0) {
				// lis r11,-32256
				// li r30,0
				var_r30 = 0;
				// lfs f31,15788(r11)
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
				var_f31 = double(temp.f32);
			loc_8245E754:
				// lwz r11,16(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
				// addi r4,r1,80
				ctx.r4.s64 = ctx.r1.s64 + 80;
				// lwz r3,12(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
				// fmr f1,f31
				ctx.fpscr.disableFlushMode();
				ctx.f1.f64 = var_f31;
				// addi r5,r11,8
				ctx.r5.s64 = ctx.r11.s64 + 8;
				// bl 0x82448170
				atSingleton_8170_g(ctx, base);
				// addi r8,r30,1
				ctx.r8.s64 = (int64_t)(int32_t)var_r30 + 1;
				// clrlwi r30,r8,16
				var_r30 = (uint32_t)(ctx.r8.u32 & 0xFFFF);
				// cmplw cr6,r30,r29
				// blt cr6,0x8245e754
				if (var_r30 < var_r29) goto loc_8245E754;
			}
		loc_8245E77C:
			// lfs f7,80(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f7.f64 = double(temp.f32);
			// fctiwz f6,f7
			ctx.f6.s64 = (ctx.f7.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f7.f64));
			// stfd f6,88(r1)
			PPC_STORE_U64(ctx.r1.u32 + 88, ctx.f6.u64);
			// lhz r11,94(r1)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r1.u32 + 94);
			// extsh r11,r11
			ctx.r11.s64 = ctx.r11.s16;
			// cmpwi cr6,r11,2400
			// blt cr6,0x8245e7a4
			if (ctx.r11.s32 >= 2400) {
				// lis r11,-32256
				ctx.r11.s64 = -2113929216;
				// lfs f1,16056(r11)
				temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16056);  /* glob:lbl_82003EB8 @ 0x82003eb8 */
				ctx.f1.f64 = double(temp.f32);
				// b 0x8245e7d8
			} else {
			loc_8245E7A4:
				// cmpwi cr6,r11,-2400
				// bgt cr6,0x8245e7b8
				if (ctx.r11.s32 <= -2400) {
					// lis r11,-32248
					ctx.r11.s64 = -2113404928;
					// lfs f1,-25416(r11)
					ctx.fpscr.disableFlushMode();
					temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25416);  /* glob:lbl_82079CB8 @ 0x82079cb8 */
					ctx.f1.f64 = double(temp.f32);
					// b 0x8245e7d8
				} else {
				loc_8245E7B8:
					// extsw r7,r11
					ctx.r7.s64 = ctx.r11.s32;
					// lis r11,-32256
					// std r7,88(r1)
					PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r7.u64);
					// lfs f0,18960(r11)
					ctx.fpscr.disableFlushMode();
					temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18960);  /* glob:0x82084a10 */
					ctx.f0.f64 = double(temp.f32);
					// lfd f5,88(r1)
					ctx.f5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
					// fcfid f4,f5
					ctx.f4.f64 = double(ctx.f5.s64);
					// frsp f3,f4
					ctx.f3.f64 = double(float(ctx.f4.f64));
					// fmuls f1,f3,f0
					ctx.f1.f64 = double(float(ctx.f3.f64 * ctx.f0.f64));
				}
			}
		loc_8245E7D8:
			// lwz r6,44(r31)
			ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 44);
			// lwz r3,16(r6)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
			// bl 0x8245a490
			atSingleton_A490_g(ctx, base);
		}
	}
loc_8245E7E4:
	// mr r3,r25
	ctx.r3.u64 = var_r25;
	// stw r26,32(r31)
	PPC_STORE_U32(var_r31 + 32, var_r26);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82436664
	__restfpr_28(ctx, base);
	// b 0x8242f8dc
	__restgprlr_25(ctx, base);
	return;
}

__attribute__((alias("__imp__jumptable_E800_h"))) PPC_WEAK_FUNC(jumptable_E800_h);
PPC_FUNC_IMPL(__imp__jumptable_E800_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// li r9,0
	ctx.r9.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8245f998
	jumptable_F998(ctx, base);
	// lis r11,-32256
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,22116
	ctx.r11.s64 = ctx.r11.s64 + 22116;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,60(r31)
	PPC_STORE_U32(var_r31 + 60, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_5_E848_1"))) PPC_WEAK_FUNC(atSingleton_vfn_5_E848_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_5_E848_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22116
	ctx.r11.s64 = ctx.r11.s64 + 22116;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x8245fa08
	atSingleton_FA08_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245e894
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32785
		ctx.r4.u64 = ctx.r4.u64 | 32785;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245E894:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_6_E8B0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_6_E8B0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_6_E8B0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r11,60(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 60);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// cmpwi cr6,r11,0
	// beq cr6,0x8245e900
	if (ctx.r11.s32 != 0) {
		// lwz r10,44(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r7,16(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r9,16(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
		// lhz r4,7(r7)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r7.u32 + 7);
		// lwz r8,20(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
		// lwz r3,48(r8)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 48);
		// lwz r6,0(r3)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// lwz r11,40(r6)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r6.u32 + 40);
		// mtctr r11
		ctx.ctr.u64 = ctx.r11.u64;
		// b 0x8245e918
	} else {
	loc_8245E900:
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// lwz r9,16(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// lhz r4,7(r9)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r9.u32 + 7);
		// lwz r8,64(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 64);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	}
loc_8245E918:
	// bctrl
	ctx.lr = 0x8245E91C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lis r10,-32256
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
	// addi r5,r11,9
	ctx.r5.s64 = ctx.r11.s64 + 9;
	// lfs f1,15788(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15788);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82448170
	atSingleton_8170_g(ctx, base);
	// lwz r7,60(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 60);
	// lfs f1,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f1.f64 = double(temp.f32);
	// cmpwi cr6,r7,0
	// beq cr6,0x8245e970
	if (ctx.r7.s32 != 0) {
		// lwz r6,44(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r5,16(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r3,16(r6)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
		// lhz r4,7(r5)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r5.u32 + 7);
		// lwz r11,20(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// lwz r3,48(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
		// lwz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// lwz r9,36(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 36);
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
		// b 0x8245e988
	} else {
	loc_8245E970:
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// lwz r8,16(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// lhz r4,7(r8)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r8.u32 + 7);
		// lwz r6,60(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 60);
		// mtctr r6
		ctx.ctr.u64 = ctx.r6.u64;
	}
loc_8245E988:
	// bctrl
	ctx.lr = 0x8245E98C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lhz r5,56(r31)
	ctx.r5.u64 = PPC_LOAD_U16(var_r31 + 56);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r5,0
	// beq cr6,0x8245e9b4
	if (ctx.r5.u32 != 0) {
		// bl 0x8245fab8
		atSingleton_FAB8_g(ctx, base);
		// blr
		return;
	}
loc_8245E9B4:
	// bl 0x82452378
	atSingleton_2378_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_7_E9D0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_7_E9D0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_7_E9D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=160, savegprlr_26
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// li r26,0
	var_r26 = 0;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
	// lhz r4,7(r11)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 7);
	// bl 0x82448d10
	msgMsgSink_8D10_g(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x8245ea0c
	if (ctx.r3.u32 == 0) {
		// lis r26,-30009
		var_r26 = (uint32_t)(-1966669824);
		// ori r26,r26,10
		var_r26 = (uint32_t)(var_r26 | 10);
	}
loc_8245EA0C:
	// mr r30,r28
	var_r30 = (uint32_t)(var_r28);
	// cmpwi cr6,r26,0
	// blt cr6,0x8245ebd8
	if ((int32_t)var_r26 >= 0) {
		// lbz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
		// rlwinm r9,r10,0,29,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
		// cmplwi cr6,r9,0
		// beq cr6,0x8245ea60
		if (ctx.r9.u32 != 0) {
			// li r8,1
			ctx.r8.s64 = 1;
			// lwz r7,44(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 44);
			// lwz r4,16(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
			// li r6,0
			ctx.r6.s64 = 0;
			// li r5,0
			ctx.r5.s64 = 0;
			// stw r8,60(r31)
			PPC_STORE_U32(var_r31 + 60, ctx.r8.u32);
			// lwz r3,16(r7)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
			// lhz r4,7(r4)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r4.u32 + 7);
			// lwz r11,20(r3)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
			// lwz r3,48(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
			// bl 0x8244ec28
			msgMsgSink_EC28_g(ctx, base);
			return;
		}
	loc_8245EA60:
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r10,16(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r7,60(r31)
		PPC_STORE_U32(var_r31 + 60, ctx.r7.u32);
		// lwz r6,0(r10)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
		// lwz r8,12(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 12);
		// rlwinm r27,r6,27,8,31
		var_r27 = (uint32_t)(__builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0xFFFFFF);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// cmpw cr6,r28,r27
		// blt cr6,0x8245ebd4
		if ((int32_t)var_r28 >= (int32_t)var_r27) {
			// add r5,r3,r27
			ctx.r5.u64 = ctx.r3.u64 + var_r27;
			// cmpw cr6,r5,r28
			// blt cr6,0x8245ebd4
			if (ctx.r5.s32 < (int32_t)var_r28) goto loc_8245EBD4;
			// lhz r4,56(r31)
			ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 56);
			// li r30,0
			var_r30 = 0;
			// cmplwi cr6,r4,0
			// beq cr6,0x8245eae4
			if (ctx.r4.u32 != 0) {
				// mr r4,r28
				ctx.r4.u64 = var_r28;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// bl 0x8245fb68
				atSingleton_vfn_39(ctx, base);
				// lwz r10,52(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
				// subf r9,r27,r28
				ctx.r9.s64 = (int64_t)(int32_t)var_r28 - (int64_t)(int32_t)var_r27;
				// mr r26,r3
				var_r26 = ctx.r3.u32;
				// rotlwi r11,r9,1
				ctx.r11.u64 = __builtin_rotateleft32(ctx.r9.u32, 1);
				// lhz r8,2(r10)
				ctx.r8.u64 = PPC_LOAD_U16(ctx.r10.u32 + 2);
				// addi r7,r11,-1
				ctx.r7.s64 = ctx.r11.s64 + -1;
				// divw r6,r9,r8
				ctx.r6.s32 = ctx.r8.s32 ? ctx.r9.s32 / ctx.r8.s32 : 0;
				// andc r5,r8,r7
				ctx.r5.u64 = ctx.r8.u64 & ~ctx.r7.u64;
				// twllei r8,0
				if (ctx.r8.s32 == 0 || ctx.r8.u32 < 0u) __builtin_trap();
				// clrlwi r30,r6,16
				var_r30 = (uint32_t)(ctx.r6.u32 & 0xFFFF);
				// twlgei r5,-1
				if (ctx.r5.s32 == -1 || ctx.r5.u32 > 4294967295u) __builtin_trap();
			}
		loc_8245EAE4:
			// lis r11,-32256
			// lwz r4,60(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 60);
			// addi r5,r1,80
			ctx.r5.s64 = ctx.r1.s64 + 80;
			// cmpwi cr6,r4,0
			// lfs f0,15784(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15784);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,80(r1)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
			// beq cr6,0x8245eb28
			if (ctx.r4.s32 != 0) {
				// lwz r3,44(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 44);
				// lwz r11,16(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
				// lwz r10,16(r3)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
				// lhz r4,7(r11)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 7);
				// lwz r9,20(r10)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
				// lwz r3,48(r9)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 48);
				// lwz r8,0(r3)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
				// lwz r7,40(r8)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 40);
				// mtctr r7
				ctx.ctr.u64 = ctx.r7.u64;
				// b 0x8245eb40
			} else {
			loc_8245EB28:
				// lwz r3,12(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
				// lwz r6,16(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 16);
				// lwz r11,0(r3)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
				// lhz r4,7(r6)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r6.u32 + 7);
				// lwz r10,64(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 64);
				// mtctr r10
				ctx.ctr.u64 = ctx.r10.u64;
			}
		loc_8245EB40:
			// bctrl
			ctx.lr = 0x8245EB44;
			PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
			// lis r11,-32256
			// clrlwi r29,r30,16
			var_r29 = (uint32_t)(var_r30 & 0xFFFF);
			// li r30,0
			var_r30 = 0;
			// lfs f31,15788(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
			var_f31 = double(temp.f32);
		loc_8245EB54:
			// lwz r11,16(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// lwz r3,12(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
			// fmr f1,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = var_f31;
			// addi r5,r11,9
			ctx.r5.s64 = ctx.r11.s64 + 9;
			// bl 0x82448170
			atSingleton_8170_g(ctx, base);
			// addi r9,r30,1
			ctx.r9.s64 = (int64_t)(int32_t)var_r30 + 1;
			// clrlwi r30,r9,16
			var_r30 = (uint32_t)(ctx.r9.u32 & 0xFFFF);
			// cmplw cr6,r30,r29
			// ble cr6,0x8245eb54
			if (var_r30 <= var_r29) goto loc_8245EB54;
			// lwz r8,60(r31)
			ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 60);
			// lfs f1,80(r1)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			ctx.f1.f64 = double(temp.f32);
			// cmpwi cr6,r8,0
			// beq cr6,0x8245ebb4
			if (ctx.r8.s32 != 0) {
				// lwz r7,44(r31)
				ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 44);
				// lwz r6,16(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 16);
				// lwz r5,16(r7)
				ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
				// lhz r4,7(r6)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r6.u32 + 7);
				// lwz r3,20(r5)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 20);
				// lwz r3,48(r3)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 48);
				// lwz r11,0(r3)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
				// lwz r10,36(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
				// mtctr r10
				ctx.ctr.u64 = ctx.r10.u64;
				// b 0x8245ebcc
			} else {
			loc_8245EBB4:
				// lwz r3,12(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
				// lwz r9,16(r31)
				ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
				// lwz r8,0(r3)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
				// lhz r4,7(r9)
				ctx.r4.u64 = PPC_LOAD_U16(ctx.r9.u32 + 7);
				// lwz r7,60(r8)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 60);
				// mtctr r7
				ctx.ctr.u64 = ctx.r7.u64;
			}
		loc_8245EBCC:
			// bctrl
			ctx.lr = 0x8245EBD0;
			PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
			// subf r30,r27,r28
			var_r30 = var_r28 - var_r27;
		}
	loc_8245EBD4:
		// stw r30,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r30);
	}
loc_8245EBD8:
	// mr r3,r26
	ctx.r3.u64 = var_r26;
	return;
}

__attribute__((alias("__imp__jumptable_EBE8_h"))) PPC_WEAK_FUNC(jumptable_EBE8_h);
PPC_FUNC_IMPL(__imp__jumptable_EBE8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,22136
	ctx.r11.s64 = ctx.r11.s64 + 22136;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,48(r31)
	PPC_STORE_U32(var_r31 + 48, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_11_EC30_1"))) PPC_WEAK_FUNC(atSingleton_vfn_11_EC30_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_11_EC30_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82452378
	atSingleton_2378_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_10_EC68_1"))) PPC_WEAK_FUNC(atSingleton_vfn_10_EC68_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_10_EC68_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22136
	ctx.r11.s64 = ctx.r11.s64 + 22136;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245ecb4
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32788
		ctx.r4.u64 = ctx.r4.u64 | 32788;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245ECB4:
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_ECD0_h"))) PPC_WEAK_FUNC(jumptable_ECD0_h);
PPC_FUNC_IMPL(__imp__jumptable_ECD0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,22160
	ctx.r11.s64 = ctx.r11.s64 + 22160;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,48(r31)
	PPC_STORE_U32(var_r31 + 48, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// stw r10,52(r31)
	PPC_STORE_U32(var_r31 + 52, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_1_ED20_1"))) PPC_WEAK_FUNC(atSingleton_vfn_1_ED20_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_1_ED20_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245ed60
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r10,60(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		// li r9,0
		ctx.r9.s64 = 0;
		// stw r9,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r9.u32);
	}
loc_8245ED60:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82452378
	atSingleton_2378_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_5_ED80_1"))) PPC_WEAK_FUNC(atSingleton_vfn_5_ED80_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_5_ED80_1) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lwz r3,52(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 52);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245edcc
	if (ctx.r3.u32 != 0) {
		// lwz r10,64(r11)
		// bctrl
		VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
		// rlwinm r9,r3,0,29,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0x4;
		// li r3,1
		ctx.r3.s64 = 1;
		// cmplwi cr6,r9,0
		// bne cr6,0x8245edd0
		if (ctx.r9.u32 != 0) {
			// blr
			return;
		}
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
loc_8245EDCC:
	// li r3,1
	ctx.r3.s64 = 1;
loc_8245EDD0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_0_EDE0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_0_EDE0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_0_EDE0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r11,r11,22160
	ctx.r11.s64 = ctx.r11.s64 + 22160;
	// lwz r3,52(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 52);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x8245ee24
	if (ctx.r3.u32 != 0) {
		// lwz r10,12(r11)
		// bctrl
		VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
	}
loc_8245EE24:
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r11,r11,21776
	ctx.r11.s64 = ctx.r11.s64 + 21776;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r9,r30,31
	ctx.r9.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r9,0
	// beq cr6,0x8245ee58
	if (ctx.r9.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32780
		ctx.r4.u64 = ctx.r4.u64 | 32780;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245EE58:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_2_EE70_1"))) PPC_WEAK_FUNC(atSingleton_vfn_2_EE70_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_2_EE70_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r10,16(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,7(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7);
	// lwz r8,20(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
	// lwz r7,48(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 48);
	// lwz r3,284(r7)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 284);
	// bl 0x824501d0
	snNotifyGamerAdded_01D0_h(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x8245eeb8
	if (ctx.r3.s32 >= 0) {
		// lwz r6,80(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r6,52(r31)
		PPC_STORE_U32(var_r31 + 52, ctx.r6.u32);
	}
loc_8245EEB8:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_11_EED0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_11_EED0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_11_EED0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245ef34
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r6,20(r7)
		// bctrl
		atSingleton_vfn_5(ctx, base);  // vtable slot 5 (byte +20)  // atSingleton::vfn_5
		// blr
		return;
	}
loc_8245EF34:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_53"))) PPC_WEAK_FUNC(atSingleton_vfn_53);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_53) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r29,0
	var_r29 = 0;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245efb0
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// lwz r6,36(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 36);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
		return;
	}
loc_8245EFB0:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_33"))) PPC_WEAK_FUNC(atSingleton_vfn_33);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_33) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,56(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245f01c
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r8,8(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 8);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// cmpwi cr6,r3,0
		// blt cr6,0x8245f01c
		if (ctx.r3.s32 < 0) {
			// bl 0x82566f98
			atSingleton_6F98_g(ctx, base);
			// stw r3,88(r31)
			PPC_STORE_U32(var_r31 + 88, ctx.r3.u32);
			// blr
			return;
		}
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// stw r3,96(r31)
		PPC_STORE_U32(var_r31 + 96, ctx.r3.u32);
	}
loc_8245F01C:
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// stw r3,88(r31)
	PPC_STORE_U32(var_r31 + 88, ctx.r3.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_58"))) PPC_WEAK_FUNC(atSingleton_vfn_58);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_58) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,52
	ctx.r3.s64 = ctx.r3.s64 + 52;
	// b 0x8245b370
	msgMsgSink_B370_2hr(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_5_F040_1"))) PPC_WEAK_FUNC(atSingleton_vfn_5_F040_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_5_F040_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245f090
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r7,32(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 32);
		// li r3,1
		ctx.r3.s64 = 1;
		// cmpwi cr6,r7,2
		// bge cr6,0x8245f094
		if (ctx.r7.s32 >= 2) {
			// blr
			return;
		}
	}
loc_8245F090:
	// li r3,0
	ctx.r3.s64 = 0;
loc_8245F094:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_6_F0A8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_6_F0A8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_6_F0A8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245f118
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r7,32(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 32);
		// cmpwi cr6,r7,3
		// blt cr6,0x8245f118
		if (ctx.r7.s32 < 3) goto loc_8245F118;
		// lwz r6,0(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,56(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r5.u32);
		// lwz r4,32(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 32);
		// li r3,1
		ctx.r3.s64 = 1;
		// cmpwi cr6,r4,5
		// ble cr6,0x8245f11c
		if (ctx.r4.s32 <= 5) {
			// blr
			return;
		}
	}
loc_8245F118:
	// li r3,0
	ctx.r3.s64 = 0;
loc_8245F11C:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_35"))) PPC_WEAK_FUNC(atSingleton_vfn_35);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_35) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// cmplwi cr6,r3,0
	// beq cr6,0x8245f1c8
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r7,0(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r6,56(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 56);
		// lhz r30,44(r11)
		var_r30 = (uint32_t)(PPC_LOAD_U16(ctx.r11.u32 + 44));
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// addi r5,r1,84
		ctx.r5.s64 = ctx.r1.s64 + 84;
		// lwz r3,28(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x82455f40
		msgMsgSink_5F40_g(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x8245f1c8
		if (ctx.r3.s32 < 0) goto loc_8245F1C8;
		// lwz r5,16(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r3,80(r1)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// lwz r10,84(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lbz r4,7(r5)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r5.u32 + 7);
		// mullw r11,r4,r3
		ctx.r11.s64 = int64_t(ctx.r4.s32) * int64_t(ctx.r3.s32);
		// add r3,r11,r10
		ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
		// b 0x8245f1cc
	} else {
	loc_8245F1C8:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_8245F1CC:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_47"))) PPC_WEAK_FUNC(atSingleton_vfn_47);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_47) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// bne cr6,0x8245f214
	if ((int32_t)var_r30 == 0) {
		// bl 0x82566f98
		atSingleton_6F98_g(ctx, base);
		// stw r3,92(r31)
		PPC_STORE_U32(var_r31 + 92, ctx.r3.u32);
	}
loc_8245F214:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,56(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245f268
	if (ctx.r3.u32 != 0) {
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,56(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// cntlzw r7,r30
		ctx.r7.u64 = var_r30 == 0 ? 32 : __builtin_clz(var_r30);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// rlwinm r4,r7,27,31,31
		ctx.r4.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x1;
		// lwz r10,12(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		// b 0x8245f26c
	} else {
	loc_8245F268:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_8245F26C:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_56"))) PPC_WEAK_FUNC(atSingleton_vfn_56);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_56) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,56(r11)
	// bctrl
	atSingleton_vfn_14(ctx, base);  // vtable slot 14 (byte +56)  // atSingleton::vfn_14
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,56(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 56);
	// lhz r30,44(r11)
	var_r30 = (uint32_t)(PPC_LOAD_U16(ctx.r11.u32 + 44));
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// lwz r3,28(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x82455f40
	msgMsgSink_5F40_g(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x8245f360
	if (ctx.r3.s32 >= 0) {
		// lwz r7,12(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 12);
		// li r29,0
		var_r29 = 0;
		// lwz r10,80(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// lwz r11,40(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 40);
		// cmplw cr6,r11,r10
		// bge cr6,0x8245f304
		if (ctx.r11.u32 < ctx.r10.u32) {
			// subf r6,r11,r10
			ctx.r6.s64 = ctx.r10.s64 - ctx.r11.s64;
			// stw r6,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r6.u32);
			// b 0x8245f308
		} else {
		loc_8245F304:
			// stw r29,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, var_r29);
		}
	loc_8245F308:
		// addi r30,r31,100
		var_r30 = (uint32_t)(var_r31 + 100);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82452250
		game_2250_h(ctx, base);
		// lwz r5,0(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,56(r5)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 56);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r4.u32);
		// lis r10,8192
		ctx.r10.s64 = 536870912;
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// lwz r3,44(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 44);
		// ori r9,r10,41
		ctx.r9.u64 = ctx.r10.u64 | 41;
		// sth r29,80(r31)
		PPC_STORE_U16(var_r31 + 80, (uint16_t)var_r29);
		// li r8,255
		ctx.r8.s64 = 255;
		// stw r29,83(r31)
		PPC_STORE_U32(var_r31 + 83, var_r29);
		// addi r4,r31,172
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 172;
		// stw r11,168(r31)
		PPC_STORE_U32(var_r31 + 168, ctx.r11.u32);
		// stw r9,76(r31)
		PPC_STORE_U32(var_r31 + 76, ctx.r9.u32);
		// stb r8,82(r31)
		PPC_STORE_U8(var_r31 + 82, ctx.r8.u8);
		// bl 0x82459830
		game_9830_h(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x824528a8
		atSingleton_28A8_g_28A8_1(ctx, base);
	}
loc_8245F360:
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_42_F368_1"))) PPC_WEAK_FUNC(atSingleton_vfn_42_F368_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_42_F368_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x82566f98
	atSingleton_6F98_g(ctx, base);
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r9,96(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 96);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// subf r31,r9,r11
	var_r31 = (uint32_t)(ctx.r11.s64 - ctx.r9.s64);
	// lwz r8,56(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 56);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	// lwz r6,32(r7)
	// bctrl
	atSingleton_vfn_8(ctx, base);  // vtable slot 8 (byte +32)  // atSingleton::vfn_8
	// subf r3,r31,r3
	ctx.r3.s64 = ctx.r3.s64 - (int64_t)(int32_t)var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_63"))) PPC_WEAK_FUNC(atSingleton_vfn_63);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_63) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,100(r11)
	// bctrl
	atSingleton_vfn_25(ctx, base);  // vtable slot 25 (byte +100)  // atSingleton::vfn_25
	// lwz r9,48(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 48);
	// cmpwi cr6,r9,0
	// beq cr6,0x8245f408
	if (ctx.r9.s32 != 0) {
		// lwz r8,44(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lwz r7,16(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
		// lwz r3,20(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 20);
		// bl 0x82454300
		atSingleton_4300_g(ctx, base);
	}
loc_8245F408:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_F420_w"))) PPC_WEAK_FUNC(msgMsgSink_F420_w);
PPC_FUNC_IMPL(__imp__msgMsgSink_F420_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=144, savegprlr_27
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// li r30,0
	var_r30 = 0;
	// addi r11,r11,22288
	ctx.r11.s64 = ctx.r11.s64 + 22288;
	// stw r30,48(r31)
	PPC_STORE_U32(var_r31 + 48, var_r30);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* msgMsgSink::vtable@+0x0 */ ctx.r11.u32);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r5,r11,15
	ctx.r5.s64 = ctx.r11.s64 + 15;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r9,r10,27
	ctx.r9.u64 = ctx.r10.u32 & 0x1F;
	// cmpwi cr6,r9,6
	// beq cr6,0x8245f470
	if (ctx.r9.s32 != 6) {
		// addi r5,r11,8
		ctx.r5.s64 = ctx.r11.s64 + 8;
	}
loc_8245F470:
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// addi r3,r31,52
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 52;
	// bl 0x8245b110
	msgMsgSink_B110_g(ctx, base);
	// lwz r8,16(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r28,r31,100
	var_r28 = (uint32_t)(var_r31 + 100);
	// stw r30,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, var_r30);
	// li r10,1
	ctx.r10.s64 = 1;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r7,1
	ctx.r7.s64 = 1;
	// lbz r4,7(r8)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r8.u32 + 7);
	// addi r6,r31,76
	ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 76;
	// li r8,0
	ctx.r8.s64 = 0;
	// stw r30,72(r31)
	PPC_STORE_U32(var_r31 + 72, var_r30);
	// mr r5,r27
	ctx.r5.u64 = var_r27;
	// stw r30,88(r31)
	PPC_STORE_U32(var_r31 + 88, var_r30);
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// stw r30,92(r31)
	PPC_STORE_U32(var_r31 + 92, var_r30);
	// stw r30,96(r31)
	PPC_STORE_U32(var_r31 + 96, var_r30);
	// stb r4,68(r31)
	PPC_STORE_U8(var_r31 + 68, ctx.r4.u8);
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x8245dae8
	msgMsgSink_DAE8_v12(ctx, base);
	// lis r10,-32256
	// addi r11,r31,172
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 172;
	// addi r10,r10,22284
	ctx.r10.s64 = ctx.r10.s64 + 22284;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r28,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r28);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r11,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_44_F4F0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_44_F4F0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_44_F4F0_1) {
	PPC_FUNC_PROLOGUE();
	// lbz r3,68(r3)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r3.u32 + 68);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_7_F4F8_1"))) PPC_WEAK_FUNC(atSingleton_vfn_7_F4F8_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_7_F4F8_1) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r10,100(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 100);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_59"))) PPC_WEAK_FUNC(atSingleton_vfn_59);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_59) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,88(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 88);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_60"))) PPC_WEAK_FUNC(atSingleton_vfn_60);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_60) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,92(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 92);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_54"))) PPC_WEAK_FUNC(atSingleton_vfn_54);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_54) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r3,r11,8
	ctx.r3.s64 = ctx.r11.s64 + 8;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r9,r10,27
	ctx.r9.u64 = ctx.r10.u32 & 0x1F;
	// cmpwi cr6,r9,6
	// beqlr cr6
	if (ctx.r9.s32 == 6) return;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_F540"))) PPC_WEAK_FUNC(atSingleton_F540);
PPC_FUNC_IMPL(__imp__atSingleton_F540) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// addi r11,r11,22288
	ctx.r11.s64 = ctx.r11.s64 + 22288;
	// lwz r3,72(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 72);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x8245f580
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r10,0(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	}
loc_8245F580:
	// addi r11,r31,172
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 172;
	// lis r10,-32256
	// addi r3,r31,100
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 100;
	// addi r10,r10,15816
	ctx.r10.s64 = ctx.r10.s64 + 15816;
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r9,8(r8)
	PPC_STORE_U32(ctx.r8.u32 + 8, ctx.r9.u32);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,4(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r6,4(r7)
	PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r6.u32);
	// stw r11,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r11.u32);
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x8245db50
	atSingleton_DB50_2h(ctx, base);
	// addi r3,r31,52
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 52;
	// bl 0x8245a828
	atSingleton_A828_2hr(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r11,r11,21776
	ctx.r11.s64 = ctx.r11.s64 + 21776;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_34"))) PPC_WEAK_FUNC(atSingleton_vfn_34);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_34) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r4,r1,82
	ctx.r4.s64 = ctx.r1.s64 + 82;
	// lwz r10,104(r11)
	// bctrl
	atSingleton_vfn_26(ctx, base);  // vtable slot 26 (byte +104)  // atSingleton::vfn_26
	// cmpwi cr6,r3,0
	// blt cr6,0x8245f70c
	if (ctx.r3.s32 >= 0) {
		// lwz r9,44(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 44);
		// lbz r4,80(r1)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
		// lwz r8,16(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
		// lwz r7,20(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 20);
		// lwz r6,48(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 48);
		// lwz r3,284(r6)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 284);
		// bl 0x82450090
		atSingleton_0090_g(ctx, base);
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// cmplwi cr6,r29,0
		// beq cr6,0x8245f750
		if (var_r29 == 0) {
			// lis r3,-30009
			// ori r3,r3,19
			ctx.r3.u64 = ctx.r3.u64 | 19;
			return;
		}
		// lwz r5,252(r29)
		ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 252);
		// cmpwi cr6,r5,0
		// beq cr6,0x8245f740
		if (ctx.r5.s32 == 0) {
			// lis r3,-30009
			// ori r3,r3,22
			ctx.r3.u64 = ctx.r3.u64 | 22;
			return;
		}
		// lwz r11,0(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 0)/* atSingleton::vtable@+0x0 */;
		// addi r6,r1,84
		ctx.r6.s64 = ctx.r1.s64 + 84;
		// lhz r4,82(r1)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r1.u32 + 82);
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// lwz r10,36(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
		// cmpwi cr6,r3,0
		// blt cr6,0x8245f70c
		if (ctx.r3.s32 < 0) {
			return;
		}
		// lwz r9,60(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 60);
		// lis r8,64
		ctx.r8.s64 = 4194304;
		// li r30,1
		var_r30 = 1;
		// lwz r7,0(r9)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// rlwinm r6,r7,0,9,9
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x400000;
		// cmplw cr6,r6,r8
		// beq cr6,0x8245f690
		if (ctx.r6.u32 != ctx.r8.u32) {
			// li r30,0
			var_r30 = 0;
		}
	loc_8245F690:
		// lwz r5,0(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,48(r5)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 48);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r4.u32);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// lwz r3,84(r1)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// li r6,0
		ctx.r6.s64 = 0;
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// blt cr6,0x8245f714
		if ((int32_t)var_r30 < 0) goto loc_8245F714;
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,84(r1)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r8,100(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 100);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
		// lwz r10,84(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// addi r11,r29,240
		ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 240;
		// stw r11,4(r10)
		PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
		// lwz r7,8(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// stw r7,8(r10)
		PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
		// lwz r6,8(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// stw r10,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r10.u32);
		// stw r10,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	loc_8245F708:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_8245F70C:
	return;
loc_8245F714:
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// cmplwi cr6,r3,0
	// beq cr6,0x8245f708
	if (ctx.r3.u32 == 0) goto loc_8245F708;
	// lwz r5,0(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* atSingleton::vtable@+0x0 */;
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_31_F760_1"))) PPC_WEAK_FUNC(atSingleton_vfn_31_F760_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_31_F760_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// addi r11,r11,15816
	ctx.r11.s64 = ctx.r11.s64 + 15816;
	// cmplwi cr6,r10,0
	// lwz r9,8(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r8,4(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// stw r9,8(r8)
	PPC_STORE_U32(ctx.r8.u32 + 8, ctx.r9.u32);
	// lwz r7,8(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r6,4(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 4)/* atSingleton::flags@+0x4 */;
	// stw r6,4(r7)
	PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r6.u32);
	// stw r31,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ var_r31);
	// stw r31,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r31);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x8245f7bc
	if (ctx.r10.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32812
		ctx.r4.u64 = ctx.r4.u64 | 32812;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245F7BC:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_32_F7D0_1"))) PPC_WEAK_FUNC(atSingleton_vfn_32_F7D0_1);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_32_F7D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x8245f540
	atSingleton_F540(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245f810
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32782
		ctx.r4.u64 = ctx.r4.u64 | 32782;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245F810:
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_F828_h"))) PPC_WEAK_FUNC(jumptable_F828_h);
PPC_FUNC_IMPL(__imp__jumptable_F828_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// li r8,0
	ctx.r8.s64 = 0;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r11,r11,22416
	ctx.r11.s64 = ctx.r11.s64 + 22416;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_64"))) PPC_WEAK_FUNC(atSingleton_vfn_64);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_64) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22416
	ctx.r11.s64 = ctx.r11.s64 + 22416;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245f8b4
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32787
		ctx.r4.u64 = ctx.r4.u64 | 32787;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245F8B4:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_65"))) PPC_WEAK_FUNC(atSingleton_vfn_65);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_65) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r31,1
	var_r31 = 1;
	// lwz r11,16(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lbz r10,7(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 7);
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r9,0
	// bne cr6,0x8245f904
	if (ctx.r9.u32 == 0) {
		// li r31,0
		var_r31 = 0;
	}
loc_8245F904:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lwz r8,16(r30)
	ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lbz r7,7(r8)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r8.u32 + 7);
	// rlwinm r6,r7,0,30,30
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r6,0
	// beq cr6,0x8245f948
	if (ctx.r6.u32 != 0) {
		// lwz r5,44(r30)
		ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 44);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r3,16(r5)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 16);
		// lwz r11,20(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// lwz r3,48(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
		// lwz r9,4(r10)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// b 0x8245f974
	} else {
	loc_8245F948:
		// cmpwi cr6,r31,0
		// bne cr6,0x8245f968
		if ((int32_t)var_r31 == 0) {
			// lwz r8,44(r30)
			ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 44);
			// lwz r3,16(r8)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
			// bl 0x82459d88
			game_9D88(ctx, base);
			// cmpwi cr6,r3,0
			// bne cr6,0x8245f968
			if (ctx.r3.s32 != 0) goto loc_8245F968;
			// li r31,1
			var_r31 = 1;
		}
	loc_8245F968:
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r3,44(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 44);
		// bl 0x82459760
		game_9760(ctx, base);
	}
loc_8245F974:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82452378
	atSingleton_2378_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_F998"))) PPC_WEAK_FUNC(jumptable_F998);
PPC_FUNC_IMPL(__imp__jumptable_F998) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r8
	var_r30 = ctx.r8.u32;
	// mr r8,r9
	ctx.r8.u64 = ctx.r9.u64;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824526e8
	jumptable_26E8(ctx, base);
	// lis r11,-32256
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r30,52(r31)
	PPC_STORE_U32(var_r31 + 52, var_r30);
	// addi r11,r11,22436
	ctx.r11.s64 = ctx.r11.s64 + 22436;
	// cmplwi cr6,r30,0
	// sth r10,48(r31)
	PPC_STORE_U16(var_r31 + 48, ctx.r10.u16);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x8245f9e4
	if (var_r30 != 0) {
		// lhz r11,0(r30)
		ctx.r11.u64 = PPC_LOAD_U16(var_r30 + 0);
		// b 0x8245f9e8
	} else {
	loc_8245F9E4:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_8245F9E8:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// sth r11,56(r31)
	PPC_STORE_U16(var_r31 + 56, ctx.r11.u16);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FA08_g"))) PPC_WEAK_FUNC(atSingleton_FA08_g);
PPC_FUNC_IMPL(__imp__atSingleton_FA08_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// addi r11,r11,22436
	ctx.r11.s64 = ctx.r11.s64 + 22436;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// b 0x824522d0
	atSingleton_22D0_g(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_70"))) PPC_WEAK_FUNC(atSingleton_vfn_70);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_70) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x824522e0
	atSingleton_vfn_29(ctx, base);
	// lhz r11,48(r31)
	ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 48);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// sth r11,48(r31)
	PPC_STORE_U16(var_r31 + 48, ctx.r11.u16);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_69"))) PPC_WEAK_FUNC(atSingleton_vfn_69);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_69) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22436
	ctx.r11.s64 = ctx.r11.s64 + 22436;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x824522d0
	atSingleton_22D0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x8245fa9c
	if (ctx.r11.u32 != 0) {
		// lis r4,8324
		ctx.r4.s64 = 545521664;
		// ori r4,r4,32866
		ctx.r4.u64 = ctx.r4.u64 | 32866;
		// bl 0x820c02d0
		_locale_register(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_8245FA9C:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FAB8_g"))) PPC_WEAK_FUNC(atSingleton_FAB8_g);
PPC_FUNC_IMPL(__imp__atSingleton_FAB8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lhz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 56);
	// cmplwi cr6,r11,65535
	// beq cr6,0x8245fae4
	if (ctx.r11.u32 != 65535) {
		// addis r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 65536;
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// sth r11,56(r31)
		PPC_STORE_U16(var_r31 + 56, ctx.r11.u16);
	}
loc_8245FAE4:
	// lwz r10,52(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 52);
	// lwz r9,20(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lhz r10,2(r10)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r10.u32 + 2);
	// add r9,r10,r9
	ctx.r9.u64 = ctx.r10.u64 + ctx.r9.u64;
	// stw r9,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r9.u32);
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r7,r8,0,0,0
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0x80000000;
	// cmplwi cr6,r7,0
	// beq cr6,0x8245fb30
	if (ctx.r7.u32 != 0) {
		// lhz r5,4(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 4);
		// cmplwi cr6,r5,0
		// beq cr6,0x8245fb30
		if (ctx.r5.u32 == 0) goto loc_8245FB30;
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r3,12(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
		// bl 0x82447c78
		jumptable_7C78(ctx, base);
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// add r6,r11,r3
		ctx.r6.u64 = ctx.r11.u64 + ctx.r3.u64;
		// stw r6,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r6.u32);
	}
loc_8245FB30:
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lbz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 44);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// stb r5,44(r11)
	PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r5.u8);
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r3,20(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// bl 0x824545d8
	atSingleton_45D8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_39"))) PPC_WEAK_FUNC(atSingleton_vfn_39);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,12(r11)
	// bctrl
	atSingleton_vfn_3(ctx, base);  // vtable slot 3 (byte +12)  // atSingleton::vfn_3
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// cmpwi cr6,r30,0
	// lwz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// rlwinm r10,r8,27,8,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0xFFFFFF;
	// add r11,r10,r3
	ctx.r11.u64 = ctx.r10.u64 + ctx.r3.u64;
	// ble cr6,0x8245fc1c
	if ((int32_t)var_r30 > 0) {
		// lhz r9,56(r31)
		ctx.r9.u64 = PPC_LOAD_U16(var_r31 + 56);
		// cmplwi cr6,r9,0
		// beq cr6,0x8245fc1c
		if (ctx.r9.u32 == 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			// stw r30,32(r31)
			PPC_STORE_U32(var_r31 + 32, var_r30);
			// blr
			return;
		}
		// cmpw cr6,r10,r30
		// bge cr6,0x8245fc1c
		if (ctx.r10.s32 >= (int32_t)var_r30) {
			// li r3,0
			ctx.r3.s64 = 0;
			// stw r30,32(r31)
			PPC_STORE_U32(var_r31 + 32, var_r30);
			// blr
			return;
		}
		// cmpw cr6,r11,r30
		// blt cr6,0x8245fc1c
		if (ctx.r11.s32 < (int32_t)var_r30) {
			// li r3,0
			ctx.r3.s64 = 0;
			// stw r30,32(r31)
			PPC_STORE_U32(var_r31 + 32, var_r30);
			// blr
			return;
		}
		// lwz r11,52(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 52);
		// subf r7,r10,r30
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 - ctx.r10.s64;
		// cmplwi cr6,r9,65535
		// rotlwi r10,r7,1
		ctx.r10.u64 = __builtin_rotateleft32(ctx.r7.u32, 1);
		// addi r6,r10,-1
		ctx.r6.s64 = ctx.r10.s64 + -1;
		// lhz r5,2(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 2);
		// divw r4,r7,r5
		ctx.r4.s32 = ctx.r5.s32 ? ctx.r7.s32 / ctx.r5.s32 : 0;
		// andc r3,r5,r6
		ctx.r3.u64 = ctx.r5.u64 & ~ctx.r6.u64;
		// twllei r5,0
		if (ctx.r5.s32 == 0 || ctx.r5.u32 < 0u) __builtin_trap();
		// clrlwi r10,r4,16
		ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
		// twlgei r3,-1
		if (ctx.r3.s32 == -1 || ctx.r3.u32 > 4294967295u) __builtin_trap();
		// beq cr6,0x8245fc08
		if (ctx.r9.u32 != 65535) {
			// lhz r8,0(r11)
			ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
			// clrlwi r9,r10,16
			ctx.r9.u64 = ctx.r10.u32 & 0xFFFF;
			// subf r7,r9,r8
			ctx.r7.s64 = ctx.r8.s64 - ctx.r9.s64;
			// sth r7,56(r31)
			PPC_STORE_U16(var_r31 + 56, ctx.r7.u16);
		}
	loc_8245FC08:
		// sth r10,48(r31)
		PPC_STORE_U16(var_r31 + 48, ctx.r10.u16);
		// clrlwi r5,r10,16
		ctx.r5.u64 = ctx.r10.u32 & 0xFFFF;
		// lhz r4,2(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 2);
		// mullw r3,r4,r5
		ctx.r3.s64 = int64_t(ctx.r4.s32) * int64_t(ctx.r5.s32);
		// subf r30,r3,r30
		var_r30 = (uint32_t)((int64_t)(int32_t)var_r30 - ctx.r3.s64);
	}
loc_8245FC1C:
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r30,32(r31)
	PPC_STORE_U32(var_r31 + 32, var_r30);
	// blr
	return;
}

__attribute__((alias("__imp__jumptable_FC40"))) PPC_WEAK_FUNC(jumptable_FC40);
PPC_FUNC_IMPL(__imp__jumptable_FC40) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// stw r4,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r4.u32);
	// lwz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// stw r11,-16(r1)
	PPC_STORE_U32(ctx.r1.u32 + -16, ctx.r11.u32);
	// lfs f0,-16(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,4(r3)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// lwz r10,4(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
	// stw r10,-16(r1)
	PPC_STORE_U32(ctx.r1.u32 + -16, ctx.r10.u32);
	// lfs f13,-16(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,8(r3)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// lhz r11,12(r4)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r4.u32 + 12);
	// rotlwi r10,r11,1
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 1);
	// add r9,r11,r10
	ctx.r9.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rlwinm r8,r9,28,8,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 28) & 0xFFFFFF;
	// sth r8,12(r3)
	PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FC80_g"))) PPC_WEAK_FUNC(atSingleton_FC80_g);
PPC_FUNC_IMPL(__imp__atSingleton_FC80_g) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lhz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
	// cmplwi cr6,r10,0
	// bne cr6,0x8245fc98
	if (ctx.r10.u32 == 0) {
		// li r3,0
		ctx.r3.s64 = 0;
		// blr
		return;
	}
loc_8245FC98:
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// addis r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 65536;
	// fadds f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f12,4(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// li r3,1
	ctx.r3.s64 = 1;
	// lwz r6,8(r9)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + 8);
	// sth r8,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r8.u16);
	// stw r6,-16(r1)
	PPC_STORE_U32(ctx.r1.u32 + -16, ctx.r6.u32);
	// lfs f11,-16(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f11.f64 = double(temp.f32);
	// fadds f10,f11,f0
	ctx.f10.f64 = double(float(ctx.f11.f64 + ctx.f0.f64));
	// stfs f10,8(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FCD8_2h"))) PPC_WEAK_FUNC(atSingleton_FCD8_2h);
PPC_FUNC_IMPL(__imp__atSingleton_FCD8_2h) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// mr r7,r5
	ctx.r7.u64 = ctx.r5.u64;
	// addi r4,r11,8
	ctx.r4.s64 = ctx.r11.s64 + 8;
	// addi r3,r10,12
	ctx.r3.s64 = ctx.r10.s64 + 12;
	// lwz r9,72(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
	// stw r9,168(r10)
	PPC_STORE_U32(ctx.r10.u32 + 168, ctx.r9.u32);
	// lwz r8,76(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 76);
	// stw r8,172(r10)
	PPC_STORE_U32(ctx.r10.u32 + 172, ctx.r8.u32);
	// lbz r6,69(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 69);
	// lbz r5,68(r11)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r11.u32 + 68);
	// b 0x82466170
	ph_vt57D8_1_6170(ctx, base);
	return;
}

__attribute__((alias("__imp__ph_vt57D8_29_FDD0"))) PPC_WEAK_FUNC(ph_vt57D8_29_FDD0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_29_FDD0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// cmplwi cr6,r11,2
	// bne cr6,0x8245fe34
	if (ctx.r11.u32 == 2) {
		// clrlwi r10,r5,24
		ctx.r10.u64 = ctx.r5.u32 & 0xFF;
		// cmplwi cr6,r10,3
		// bne cr6,0x8245fe34
		if (ctx.r10.u32 != 3) goto loc_8245FE34;
		// lwz r11,4(r6)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r6.u32 + 4);
		// cmplwi cr6,r11,56
		// bne cr6,0x8245fe10
		if (ctx.r11.u32 == 56) {
			// addi r3,r3,12
			ctx.r3.s64 = ctx.r3.s64 + 12;
			// lwz r4,0(r6)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
			// li r5,0
			ctx.r5.s64 = 0;
			// lwz r9,0(r3)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
			// lwz r8,48(r9)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 48);
			// mtctr r8
			ctx.ctr.u64 = ctx.r8.u64;
			// bctr
			PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
			return;
		}
	loc_8245FE10:
		// cmplwi cr6,r11,60
		// bne cr6,0x8245fe8c
		if (ctx.r11.u32 != 60) {
			// lis r3,-32761
			// ori r3,r3,87
			ctx.r3.u64 = ctx.r3.u64 | 87;
			// blr
			return;
		}
		// addi r3,r3,12
		ctx.r3.s64 = ctx.r3.s64 + 12;
		// lwz r4,0(r6)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
		// lwz r7,0(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// lbz r5,56(r4)
		ctx.r5.u64 = PPC_LOAD_U8(ctx.r4.u32 + 56);
		// lwz r6,48(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 48);
		// mtctr r6
		ctx.ctr.u64 = ctx.r6.u64;
		// bctr
		PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
		return;
	}
loc_8245FE34:
	// cmplwi cr6,r11,0
	// bne cr6,0x8245fe60
	if (ctx.r11.u32 == 0) {
		// clrlwi r4,r5,24
		ctx.r4.u64 = ctx.r5.u32 & 0xFF;
		// cmplwi cr6,r4,1
		// bne cr6,0x8245fe60
		if (ctx.r4.u32 != 1) goto loc_8245FE60;
		// addi r3,r3,12
		ctx.r3.s64 = ctx.r3.s64 + 12;
		// lfs f1,0(r6)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 0);
		ctx.f1.f64 = double(temp.f32);
		// lwz r11,0(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// lwz r10,56(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
		// mtctr r10
		ctx.ctr.u64 = ctx.r10.u64;
		// bctr
		PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
		return;
	}
loc_8245FE60:
	// cmplwi cr6,r11,1
	// bne cr6,0x8245fe8c
	if (ctx.r11.u32 == 1) {
		// clrlwi r9,r5,24
		ctx.r9.u64 = ctx.r5.u32 & 0xFF;
		// cmplwi cr6,r9,1
		// bne cr6,0x8245fe8c
		if (ctx.r9.u32 != 1) {
			// lis r3,-32761
			// ori r3,r3,87
			ctx.r3.u64 = ctx.r3.u64 | 87;
			// blr
			return;
		}
		// addi r3,r3,12
		ctx.r3.s64 = ctx.r3.s64 + 12;
		// lfs f1,0(r6)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 0);
		ctx.f1.f64 = double(temp.f32);
		// lwz r8,0(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// lwz r7,64(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 64);
		// mtctr r7
		ctx.ctr.u64 = ctx.r7.u64;
		// bctr
		PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
		return;
	}
loc_8245FE8C:
	// lis r3,-32761
	// ori r3,r3,87
	ctx.r3.u64 = ctx.r3.u64 | 87;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_79"))) PPC_WEAK_FUNC(atSingleton_vfn_79);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_79) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r11,0(r4)
	PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FEA8_sp"))) PPC_WEAK_FUNC(atSingleton_FEA8_sp);
PPC_FUNC_IMPL(__imp__atSingleton_FEA8_sp) {
	PPC_FUNC_PROLOGUE();
	// lbz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,2
	// bgt cr6,0x8245feb8
	if (ctx.r11.u32 <= 2) {
		// b 0x82469070
		phInst_9070_p42(ctx, base);
		return;
	}
loc_8245FEB8:
	// cmplwi cr6,r11,3
	// bne cr6,0x8245fec4
	if (ctx.r11.u32 == 3) {
		// b 0x82467250
		atSingleton_7250_p42(ctx, base);
		return;
	}
loc_8245FEC4:
	// lis r3,-32768
	// ori r3,r3,16385
	ctx.r3.u64 = ctx.r3.u64 | 16385;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FED0_sp"))) PPC_WEAK_FUNC(atSingleton_FED0_sp);
PPC_FUNC_IMPL(__imp__atSingleton_FED0_sp) {
	PPC_FUNC_PROLOGUE();
	// lbz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,2
	// bgt cr6,0x8245fee0
	if (ctx.r11.u32 <= 2) {
		// b 0x82469818
		atSingleton_9818_2hr(ctx, base);
		return;
	}
loc_8245FEE0:
	// cmplwi cr6,r11,3
	// bne cr6,0x8245feec
	if (ctx.r11.u32 == 3) {
		// b 0x82468898
		atSingleton_8898_fw(ctx, base);
		return;
	}
loc_8245FEEC:
	// lis r3,-32768
	// ori r3,r3,16385
	ctx.r3.u64 = ctx.r3.u64 | 16385;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_FEF8_2h"))) PPC_WEAK_FUNC(atSingleton_FEF8_2h);
PPC_FUNC_IMPL(__imp__atSingleton_FEF8_2h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,22456
	ctx.r11.s64 = ctx.r11.s64 + 22456;
	// li r10,1
	ctx.r10.s64 = 1;
	// addi r30,r31,12
	var_r30 = (uint32_t)(var_r31 + 12);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ ctx.r10.u32);
	// lwz r9,4(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// bl 0x82466118
	atSingleton_6118_fw(ctx, base);
	// lis r11,-32256
	// lis r10,-32256
	// addi r11,r11,22568
	ctx.r11.s64 = ctx.r11.s64 + 22568;
	// addi r10,r10,22488
	ctx.r10.s64 = ctx.r10.s64 + 22488;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* atSingleton::vtable@+0x0 */ ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_0_FF70"))) PPC_WEAK_FUNC(ph_vt57D8_0_FF70);
PPC_FUNC_IMPL(__imp__ph_vt57D8_0_FF70) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-12
	ctx.r3.s64 = ctx.r3.s64 + -12;
	// b 0x82460718
	ph_vt57D8_20_0718(ctx, base);
	return;
}

__attribute__((alias("__imp__ph_vt57D8_7_FF78"))) PPC_WEAK_FUNC(ph_vt57D8_7_FF78);
PPC_FUNC_IMPL(__imp__ph_vt57D8_7_FF78) {
	PPC_FUNC_PROLOGUE();
	// lbz r11,152(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 152);
	// li r3,0
	ctx.r3.s64 = 0;
	// stb r11,0(r4)
	PPC_STORE_U8(ctx.r4.u32 + 0, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_4_FF88"))) PPC_WEAK_FUNC(ph_vt57D8_4_FF88);
PPC_FUNC_IMPL(__imp__ph_vt57D8_4_FF88) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r10,72(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 18, ctx, base);  // pattern-B slot 18 (byte +72)
	// cmpwi cr6,r3,0
	// blt cr6,0x8245ffc4
	if (ctx.r3.s32 >= 0) {
		// lwz r9,80(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// lwz r8,16(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
		// stw r8,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r8.u32);
	}
loc_8245FFC4:
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_5_FFD8"))) PPC_WEAK_FUNC(ph_vt57D8_5_FFD8);
PPC_FUNC_IMPL(__imp__ph_vt57D8_5_FFD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r10,72(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 18, ctx, base);  // pattern-B slot 18 (byte +72)
	// cmpwi cr6,r3,0
	// blt cr6,0x82460010
	if (ctx.r3.s32 >= 0) {
		// lwz r9,80(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r31,16(r9)
		PPC_STORE_U32(ctx.r9.u32 + 16, var_r31);
	}
loc_82460010:
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_6_0028"))) PPC_WEAK_FUNC(ph_vt57D8_6_0028);
PPC_FUNC_IMPL(__imp__ph_vt57D8_6_0028) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r10,72(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 18, ctx, base);  // pattern-B slot 18 (byte +72)
	// cmpwi cr6,r3,0
	// blt cr6,0x82460064
	if (ctx.r3.s32 >= 0) {
		// lwz r9,80(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// lwz r8,92(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 92);
		// stw r8,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r8.u32);
	}
loc_82460064:
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_11_0078"))) PPC_WEAK_FUNC(ph_vt57D8_11_0078);
PPC_FUNC_IMPL(__imp__ph_vt57D8_11_0078) {
	PPC_FUNC_PROLOGUE();
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// cmplwi cr6,r10,0
	// beq cr6,0x824600a4
	if (ctx.r10.u32 != 0) {
		// addi r11,r3,32
		ctx.r11.s64 = ctx.r3.s64 + 32;
		// li r9,14
		ctx.r9.s64 = 14;
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
	loc_82460090:
		// lwz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r9,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x82460090
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_82460090;
	}
loc_824600A4:
	// cmplwi cr6,r5,0
	// beq cr6,0x824600b4
	if (ctx.r5.u32 != 0) {
		// lbz r8,153(r3)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r3.u32 + 153);
		// stb r8,0(r5)
		PPC_STORE_U8(ctx.r5.u32 + 0, ctx.r8.u8);
	}
loc_824600B4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_13_00C0"))) PPC_WEAK_FUNC(ph_vt57D8_13_00C0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_13_00C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lfs f0,92(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 92);
	ctx.f0.f64 = double(temp.f32);
	// li r3,0
	ctx.r3.s64 = 0;
	// stfs f0,0(r4)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_14_00D0"))) PPC_WEAK_FUNC(ph_vt57D8_14_00D0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_14_00D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// stfs f1,92(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_15_00E0"))) PPC_WEAK_FUNC(ph_vt57D8_15_00E0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_15_00E0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lfs f0,96(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 96);
	ctx.f0.f64 = double(temp.f32);
	// li r3,0
	ctx.r3.s64 = 0;
	// stfs f0,0(r4)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_16_00F0"))) PPC_WEAK_FUNC(ph_vt57D8_16_00F0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_16_00F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// stfs f1,96(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_0100_p39"))) PPC_WEAK_FUNC(atSingleton_0100_p39);
PPC_FUNC_IMPL(__imp__atSingleton_0100_p39) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,0
	// beq cr6,0x82460114
	if (ctx.r4.u32 != 0) {
		// lwz r11,8(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// add r11,r11,r4
		ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
		// b 0x82460118
	} else {
	loc_82460114:
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
	}
loc_82460118:
	// cmpwi cr6,r5,0
	// beq cr6,0x82460128
	if (ctx.r5.s32 != 0) {
		// lwz r11,0(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// b 0x8246012c
	} else {
	loc_82460128:
		// lwz r11,4(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	}
loc_8246012C:
	// cmplw cr6,r3,r11
	// beq cr6,0x82460140
	if (ctx.r3.u32 != ctx.r11.u32) {
		// lwz r10,8(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// subf r3,r10,r11
		ctx.r3.s64 = ctx.r11.s64 - ctx.r10.s64;
		// blr
		return;
	}
loc_82460140:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_0148_p39"))) PPC_WEAK_FUNC(atSingleton_0148_p39);
PPC_FUNC_IMPL(__imp__atSingleton_0148_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// addi r3,r31,12
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
	// addi r11,r11,22568
	ctx.r11.s64 = ctx.r11.s64 + 22568;
	// addi r10,r10,22488
	ctx.r10.s64 = ctx.r10.s64 + 22488;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r10.u32);
	// bl 0x82466158
	phInst_6158_p39(ctx, base);
	// lis r11,-32256
	// addi r11,r11,15792
	ctx.r11.s64 = ctx.r11.s64 + 15792;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_vfn_74"))) PPC_WEAK_FUNC(atSingleton_vfn_74);
PPC_FUNC_IMPL(__imp__atSingleton_vfn_74) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32256
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15792
	ctx.r11.s64 = ctx.r11.s64 + 15792;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x824601e0
	if (ctx.r10.u32 != 0) {
		// lis r11,-32162
		// lis r5,24962
		ctx.r5.s64 = 1635909632;
		// addi r3,r11,30856
		ctx.r3.s64 = ctx.r11.s64 + 30856;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// bl 0x824667d0
		phDemoWorld_67D0_g(ctx, base);
	}
loc_824601E0:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_18_01F8"))) PPC_WEAK_FUNC(ph_vt57D8_18_01F8);
PPC_FUNC_IMPL(__imp__ph_vt57D8_18_01F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r27,r4
	var_r27 = ctx.r4.u32;
	// bl 0x8258653c
	__imp__KeRaiseIrqlToDpcLevel(ctx, base);
	// lis r11,-32162
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// addi r31,r11,30832
	var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
	// mr r30,r13
	var_r30 = ctx.r13.u32;
	// lwz r10,4(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r10,0
	// beq cr6,0x82460238
	if (ctx.r10.s32 != 0) {
		// lwz r8,8(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r30,r8
		// beq cr6,0x82460258
		if (var_r30 == ctx.r8.u32) goto loc_82460258;
	}
loc_82460238:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8258634c
	__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
	// mr r8,r30
	ctx.r8.u64 = var_r30;
	// mr r30,r29
	var_r30 = (uint32_t)(var_r29);
	// stw r8,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
	// stb r30,12(r31)
	PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r30);
	// lwz r10,4(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
	// b 0x8246025c
	goto loc_8246025C;
loc_82460258:
	// lbz r30,12(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
loc_8246025C:
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r11,r28,4
	ctx.r11.s64 = (int64_t)(int32_t)var_r28 + 4;
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplw cr6,r11,r9
	// beq cr6,0x82460284
	if (ctx.r11.u32 != ctx.r9.u32) {
		// lwz r11,8(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// subf r11,r11,r9
		ctx.r11.s64 = ctx.r9.s64 - ctx.r11.s64;
		// cmplwi cr6,r11,0
		// bne cr6,0x824602d0
		if (ctx.r11.u32 != 0) goto loc_824602D0;
	}
loc_82460284:
	// mr r11,r13
	ctx.r11.u64 = ctx.r13.u64;
	// cmpwi cr6,r10,0
	// beq cr6,0x824602c0
	if (ctx.r10.s32 != 0) {
		// cmplw cr6,r11,r8
		// bne cr6,0x824602c0
		if (ctx.r11.u32 != ctx.r8.u32) {
			// lis r3,-32768
			// ori r3,r3,16389
			ctx.r3.u64 = ctx.r3.u64 | 16389;
			return;
		}
		// addi r11,r10,-1
		ctx.r11.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x824602c0
		if (ctx.r11.s32 != 0) {
			// lis r3,-32768
			// ori r3,r3,16389
			ctx.r3.u64 = ctx.r3.u64 | 16389;
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_824602C0:
	// lis r3,-32768
	// ori r3,r3,16389
	ctx.r3.u64 = ctx.r3.u64 | 16389;
	return;
loc_824602D0:
	// cmplwi cr6,r27,0
	// beq cr6,0x824602e8
	if (var_r27 != 0) {
		// stw r11,0(r27)
		PPC_STORE_U32(var_r27 + 0, ctx.r11.u32);
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// lwz r8,8(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
	}
loc_824602E8:
	// mr r11,r13
	ctx.r11.u64 = ctx.r13.u64;
	// cmpwi cr6,r10,0
	// beq cr6,0x82460324
	if (ctx.r10.s32 != 0) {
		// cmplw cr6,r11,r8
		// bne cr6,0x82460324
		if (ctx.r11.u32 != ctx.r8.u32) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// addi r11,r10,-1
		ctx.r11.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x82460324
		if (ctx.r11.s32 != 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_82460324:
	// li r3,0
	ctx.r3.s64 = 0;
	return;
}

__attribute__((alias("__imp__ph_vt57D8_17_0330"))) PPC_WEAK_FUNC(ph_vt57D8_17_0330);
PPC_FUNC_IMPL(__imp__ph_vt57D8_17_0330) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// bl 0x8258653c
	__imp__KeRaiseIrqlToDpcLevel(ctx, base);
	// lis r11,-32162
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// addi r31,r11,30832
	var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
	// mr r29,r13
	var_r29 = ctx.r13.u32;
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r11,0
	// beq cr6,0x82460374
	if (ctx.r11.s32 != 0) {
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r29,r10
		// beq cr6,0x82460388
		if (var_r29 == ctx.r10.u32) goto loc_82460388;
	}
loc_82460374:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8258634c
	__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
	// stw r29,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r29);
	// stb r26,12(r31)
	PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r26);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
loc_82460388:
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// clrlwi r9,r27,24
	ctx.r9.u64 = var_r27 & 0xFF;
	// mr r10,r13
	ctx.r10.u64 = ctx.r13.u64;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// clrlwi r11,r28,24
	ctx.r11.u64 = var_r28 & 0xFF;
	// lbz r8,152(r30)
	ctx.r8.u64 = PPC_LOAD_U8(var_r30 + 152);
	// andc r7,r8,r11
	ctx.r7.u64 = ctx.r8.u64 & ~ctx.r11.u64;
	// or r6,r7,r9
	ctx.r6.u64 = ctx.r7.u64 | ctx.r9.u64;
	// stb r6,152(r30)
	PPC_STORE_U8(var_r30 + 152, ctx.r6.u8);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r11,0
	// beq cr6,0x824603f0
	if (ctx.r11.s32 != 0) {
		// lwz r9,8(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r10,r9
		// bne cr6,0x824603f0
		if (ctx.r10.u32 != ctx.r9.u32) {
			return;
		}
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x824603f0
		if (ctx.r11.s32 != 0) {
			return;
		}
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_824603F0:
	return;
}

__attribute__((alias("__imp__ph_vt57D8_8_03F8"))) PPC_WEAK_FUNC(ph_vt57D8_8_03F8);
PPC_FUNC_IMPL(__imp__ph_vt57D8_8_03F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x8258653c
	__imp__KeRaiseIrqlToDpcLevel(ctx, base);
	// lis r11,-32162
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// addi r31,r11,30832
	var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
	// mr r29,r13
	var_r29 = ctx.r13.u32;
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r11,0
	// beq cr6,0x82460434
	if (ctx.r11.s32 != 0) {
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r29,r10
		// beq cr6,0x82460448
		if (var_r29 == ctx.r10.u32) goto loc_82460448;
	}
loc_82460434:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8258634c
	__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
	// stw r29,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r29);
	// stb r28,12(r31)
	PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r28);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
loc_82460448:
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// lbz r10,152(r30)
	ctx.r10.u64 = PPC_LOAD_U8(var_r30 + 152);
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r9,0
	// bne cr6,0x8246047c
	if (ctx.r9.u32 == 0) {
		// lwz r8,0(r30)
		ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 0);
		// li r5,3
		ctx.r5.s64 = 3;
		// li r4,3
		ctx.r4.s64 = 3;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// lwz r7,68(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 68);
		// mtctr r7
		ctx.ctr.u64 = ctx.r7.u64;
		// b 0x824604a0
	} else {
	loc_8246047C:
		// rlwinm r6,r10,0,29,29
		ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
		// cmplwi cr6,r6,0
		// beq cr6,0x824604a8
		if (ctx.r6.u32 == 0) goto loc_824604A8;
		// lwz r11,0(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 0);
		// li r5,2
		ctx.r5.s64 = 2;
		// li r4,6
		ctx.r4.s64 = 6;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// lwz r10,68(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 68);
		// mtctr r10
		ctx.ctr.u64 = ctx.r10.u64;
	}
loc_824604A0:
	// bctrl
	ctx.lr = 0x824604A4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
loc_824604A8:
	// mr r10,r13
	ctx.r10.u64 = ctx.r13.u64;
	// cmpwi cr6,r11,0
	// beq cr6,0x824604ec
	if (ctx.r11.s32 != 0) {
		// lwz r9,8(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r10,r9
		// bne cr6,0x824604ec
		if (ctx.r10.u32 != ctx.r9.u32) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x824604ec
		if (ctx.r11.s32 != 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_824604EC:
	// li r3,0
	ctx.r3.s64 = 0;
	return;
}

__attribute__((alias("__imp__ph_vt57D8_9_04F8"))) PPC_WEAK_FUNC(ph_vt57D8_9_04F8);
PPC_FUNC_IMPL(__imp__ph_vt57D8_9_04F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r27,r4
	var_r27 = ctx.r4.u32;
	// bl 0x8258653c
	__imp__KeRaiseIrqlToDpcLevel(ctx, base);
	// lis r11,-32162
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// addi r31,r11,30832
	var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
	// mr r29,r13
	var_r29 = ctx.r13.u32;
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r11,0
	// beq cr6,0x82460538
	if (ctx.r11.s32 != 0) {
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r29,r10
		// beq cr6,0x8246054c
		if (var_r29 == ctx.r10.u32) goto loc_8246054C;
	}
loc_82460538:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8258634c
	__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
	// stw r29,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r29);
	// stb r28,12(r31)
	PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r28);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
loc_8246054C:
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// lbz r10,152(r30)
	ctx.r10.u64 = PPC_LOAD_U8(var_r30 + 152);
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r9,0
	// beq cr6,0x824605d4
	if (ctx.r9.u32 != 0) {
		// clrlwi r8,r27,31
		ctx.r8.u64 = var_r27 & 0x1;
		// cmplwi cr6,r8,0
		// bne cr6,0x824605b4
		if (ctx.r8.u32 == 0) {
			// andi. r7,r10,18
			ctx.r7.u64 = ctx.r10.u64 & 18;
			ctx.cr0.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
			// cmplwi cr6,r7,0
			// bne cr6,0x824605b4
			if (ctx.r7.u32 != 0) goto loc_824605B4;
			// rlwinm r6,r10,0,29,29
			ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
			// cmplwi cr6,r6,0
			// bne cr6,0x824605d4
			if (ctx.r6.u32 != 0) goto loc_824605D4;
			// lwz r11,0(r30)
  // [ph4a] vtable load collapsed
			// li r5,4
			ctx.r5.s64 = 4;
			// li r4,4
			ctx.r4.s64 = 4;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// lwz r10,68(r11)
  // [ph4a] slot load collapsed
			// bctrl
			VCALL(var_r30, 17, ctx, base);  // pattern-B slot 17 (byte +68)
			// lis r11,-32256
			ctx.r11.s64 = -2113929216;
			// lhz r11,23128(r11)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r11.u32 + 23128);
			// sth r11,154(r30)
			PPC_STORE_U16(var_r30 + 154, ctx.r11.u16);
			// b 0x824605d0
		} else {
		loc_824605B4:
			// lwz r9,0(r30)
  // [ph4a] vtable load collapsed
			// li r5,0
			ctx.r5.s64 = 0;
			// li r4,87
			ctx.r4.s64 = 87;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// lwz r8,68(r9)
  // [ph4a] slot load collapsed
			// bctrl
			VCALL(var_r30, 17, ctx, base);  // pattern-B slot 17 (byte +68)
		}
	loc_824605D0:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	}
loc_824605D4:
	// mr r10,r13
	ctx.r10.u64 = ctx.r13.u64;
	// cmpwi cr6,r11,0
	// beq cr6,0x82460618
	if (ctx.r11.s32 != 0) {
		// lwz r7,8(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r10,r7
		// bne cr6,0x82460618
		if (ctx.r10.u32 != ctx.r7.u32) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x82460618
		if (ctx.r11.s32 != 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_82460618:
	// li r3,0
	ctx.r3.s64 = 0;
	return;
}

__attribute__((alias("__imp__ph_vt57D8_10_0628"))) PPC_WEAK_FUNC(ph_vt57D8_10_0628);
PPC_FUNC_IMPL(__imp__ph_vt57D8_10_0628) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// bl 0x8258653c
	__imp__KeRaiseIrqlToDpcLevel(ctx, base);
	// lis r11,-32162
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// addi r31,r11,30832
	var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
	// mr r30,r13
	var_r30 = ctx.r13.u32;
	// lwz r10,4(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r10,0
	// beq cr6,0x82460664
	if (ctx.r10.s32 != 0) {
		// lwz r8,8(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r30,r8
		// beq cr6,0x8246067c
		if (var_r30 == ctx.r8.u32) goto loc_8246067C;
	}
loc_82460664:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8258634c
	__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
	// mr r8,r30
	ctx.r8.u64 = var_r30;
	// stw r8,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
	// stb r28,12(r31)
	PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r28);
	// lwz r10,4(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
loc_8246067C:
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r11,r29,4
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 4;
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplw cr6,r11,r9
	// beq cr6,0x824606c8
	if (ctx.r11.u32 != ctx.r9.u32) {
		// lwz r11,8(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// subf r11,r11,r9
		ctx.r11.s64 = ctx.r9.s64 - ctx.r11.s64;
		// cmplwi cr6,r11,0
		// beq cr6,0x824606c8
		if (ctx.r11.u32 == 0) goto loc_824606C8;
		// lwz r10,0(r29)
  // [ph4a] vtable load collapsed
		// li r5,8
		ctx.r5.s64 = 8;
		// li r4,8
		ctx.r4.s64 = 8;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lwz r9,68(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r29, 17, ctx, base);  // pattern-B slot 17 (byte +68)
		// lwz r8,8(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r10,4(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 4);
	}
loc_824606C8:
	// mr r11,r13
	ctx.r11.u64 = ctx.r13.u64;
	// cmpwi cr6,r10,0
	// beq cr6,0x82460708
	if (ctx.r10.s32 != 0) {
		// cmplw cr6,r11,r8
		// bne cr6,0x82460708
		if (ctx.r11.u32 != ctx.r8.u32) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// addi r11,r10,-1
		ctx.r11.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x82460708
		if (ctx.r11.s32 != 0) {
			// li r3,0
			ctx.r3.s64 = 0;
			return;
		}
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_82460708:
	// li r3,0
	ctx.r3.s64 = 0;
	return;
}

__attribute__((alias("__imp__ph_vt57D8_20_0718"))) PPC_WEAK_FUNC(ph_vt57D8_20_0718);
PPC_FUNC_IMPL(__imp__ph_vt57D8_20_0718) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// addi r3,r31,12
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
	// addi r11,r11,22568
	ctx.r11.s64 = ctx.r11.s64 + 22568;
	// addi r10,r10,22488
	ctx.r10.s64 = ctx.r10.s64 + 22488;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r10.u32);
	// bl 0x82466158
	phInst_6158_p39(ctx, base);
	// lis r11,-32256
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// addi r11,r11,15792
	ctx.r11.s64 = ctx.r11.s64 + 15792;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x82460780
	if (ctx.r10.u32 != 0) {
		// lis r11,-32162
		// lis r5,24962
		ctx.r5.s64 = 1635909632;
		// addi r3,r11,30856
		ctx.r3.s64 = ctx.r11.s64 + 30856;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// bl 0x824667d0
		phDemoWorld_67D0_g(ctx, base);
	}
loc_82460780:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_19_07A0"))) PPC_WEAK_FUNC(ph_vt57D8_19_07A0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_19_07A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// bl 0x824665e8
	ph_vt5A60_19_65E8(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x824607f0
	if (var_r31 != 0) {
		// lwz r11,156(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 156);
		// cmplwi cr6,r11,0
		// beq cr6,0x824607f0
		if (ctx.r11.u32 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			return;
		}
		// lwz r10,-4(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + -4);
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// lwz r9,92(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 92);
		// stw r29,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r29);
		// stw r10,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
		// stw r9,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	}
loc_824607F0:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phInst_0800_2h"))) PPC_WEAK_FUNC(phInst_0800_2h);
PPC_FUNC_IMPL(__imp__phInst_0800_2h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// mr r30,r6
	var_r30 = ctx.r6.u32;
	// bl 0x8246ac00
	phInst_AC00_2hr(ctx, base);
	// lis r11,-32162
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r5,r11,30860
	ctx.r5.s64 = ctx.r11.s64 + 30860;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8246ab00
	phInst_AB00(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__util_0850"))) PPC_WEAK_FUNC(util_0850);
PPC_FUNC_IMPL(__imp__util_0850) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,4(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 1, ctx, base);  // pattern-B slot 1 (byte +4)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__phInst_08B8"))) PPC_WEAK_FUNC(phInst_08B8);
PPC_FUNC_IMPL(__imp__phInst_08B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,8(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 2, ctx, base);  // pattern-B slot 2 (byte +8)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__game_0920"))) PPC_WEAK_FUNC(game_0920);
PPC_FUNC_IMPL(__imp__game_0920) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,24(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 6, ctx, base);  // pattern-B slot 6 (byte +24)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__game_0980"))) PPC_WEAK_FUNC(game_0980);
PPC_FUNC_IMPL(__imp__game_0980) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=144, savegprlr_26
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r27,r6
	var_r27 = ctx.r6.u32;
	// mr r26,r7
	var_r26 = ctx.r7.u32;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r7,r26
	ctx.r7.u64 = var_r26;
	// mr r6,r27
	ctx.r6.u64 = var_r27;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,32(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 8, ctx, base);  // pattern-B slot 8 (byte +32)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phInst_09F0_p28"))) PPC_WEAK_FUNC(phInst_09F0_p28);
PPC_FUNC_IMPL(__imp__phInst_09F0_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,36(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 9, ctx, base);  // pattern-B slot 9 (byte +36)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phBoundCapsule_0A48"))) PPC_WEAK_FUNC(phBoundCapsule_0A48);
PPC_FUNC_IMPL(__imp__phBoundCapsule_0A48) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* phBoundCapsule::vtable@+0x0 */;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phInst_0AA0_v12"))) PPC_WEAK_FUNC(phInst_0AA0_v12);
PPC_FUNC_IMPL(__imp__phInst_0AA0_v12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,48(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 12, ctx, base);  // pattern-B slot 12 (byte +48)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phInst_0AF8_h"))) PPC_WEAK_FUNC(phInst_0AF8_h);
PPC_FUNC_IMPL(__imp__phInst_0AF8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=128, savegprlr_27
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r27,r6
	var_r27 = ctx.r6.u32;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// mr r6,r27
	ctx.r6.u64 = var_r27;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8246c2c0
	phInst_C2C0_h(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__xam_0B50_g"))) PPC_WEAK_FUNC(xam_0B50_g);
PPC_FUNC_IMPL(__imp__xam_0B50_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 76);
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// lwz r10,28(r11)
	// bctrl
	VCALL(ctx.r3.u32, 7, ctx, base);  // vtable slot 7 (byte +28)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__phInst_0BB0_h"))) PPC_WEAK_FUNC(phInst_0BB0_h);
PPC_FUNC_IMPL(__imp__phInst_0BB0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8246c348
	phInst_C348_h(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__xam_0C08_g"))) PPC_WEAK_FUNC(xam_0C08_g);
PPC_FUNC_IMPL(__imp__xam_0C08_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8246d238
	ke_D238(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__phInst_0C58"))) PPC_WEAK_FUNC(phInst_0C58);
PPC_FUNC_IMPL(__imp__phInst_0C58) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,60(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 15, ctx, base);  // pattern-B slot 15 (byte +60)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phInst_0CB0_p28"))) PPC_WEAK_FUNC(phInst_0CB0_p28);
PPC_FUNC_IMPL(__imp__phInst_0CB0_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 76);
	// lwz r10,48(r11)
	// bctrl
	VCALL(ctx.r3.u32, 12, ctx, base);  // vtable slot 12 (byte +48)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__phInst_0D18_p28"))) PPC_WEAK_FUNC(phInst_0D18_p28);
PPC_FUNC_IMPL(__imp__phInst_0D18_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 76);
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// lwz r10,52(r11)
	// bctrl
	VCALL(ctx.r3.u32, 13, ctx, base);  // vtable slot 13 (byte +52)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__phInst_0D70_p28"))) PPC_WEAK_FUNC(phInst_0D70_p28);
PPC_FUNC_IMPL(__imp__phInst_0D70_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 76);
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// lwz r10,56(r11)
	// bctrl
	VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__phInst_0DC8_p28"))) PPC_WEAK_FUNC(phInst_0DC8_p28);
PPC_FUNC_IMPL(__imp__phInst_0DC8_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 76);
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// lwz r10,64(r11)
	// bctrl
	VCALL(ctx.r3.u32, 16, ctx, base);  // vtable slot 16 (byte +64)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__game_0E20"))) PPC_WEAK_FUNC(game_0E20);
PPC_FUNC_IMPL(__imp__game_0E20) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, var_r30);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f31,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32165
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// stfs f31,140(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r31 + 140, temp.u32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,88(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 22, ctx, base);  // pattern-B slot 22 (byte +88)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	var_r30 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -24));
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__phInst_0E98_p28"))) PPC_WEAK_FUNC(phInst_0E98_p28);
PPC_FUNC_IMPL(__imp__phInst_0E98_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	double var_f31 = 0.0;
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, var_r30);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, var_r31);
	// stfd f31,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32165
	// fmr f31,f1
	var_f31 = ctx.f1.f64;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 76);
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = var_f31;
	// lwz r10,88(r11)
	// bctrl
	VCALL(ctx.r3.u32, 22, ctx, base);  // vtable slot 22 (byte +88)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	var_r30 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -24));
	// ld r31,-16(r1)
	var_r31 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + -16));
	// blr
	return;
}

__attribute__((alias("__imp__phInst_0F10_p28"))) PPC_WEAK_FUNC(phInst_0F10_p28);
PPC_FUNC_IMPL(__imp__phInst_0F10_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// lis r11,-32165
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r11,8992
	var_r30 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lwz r3,30860(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30860);  /* glob:lbl_825E788C @ 0x825e788c */
	// cmplwi cr6,r3,0
	// beq cr6,0x82460f68
	if (ctx.r3.u32 != 0) {
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// addi r3,r30,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// li r3,0
		ctx.r3.s64 = 0;
		return;
	}
loc_82460F68:
	// cmplwi cr6,r31,0
	// bne cr6,0x82460f84
	if (var_r31 == 0) {
		// addi r11,r1,80
		ctx.r11.s64 = ctx.r1.s64 + 80;
		// li r10,0
		ctx.r10.s64 = 0;
		// addi r31,r1,80
		var_r31 = (uint32_t)(ctx.r1.s64 + 80);
		// std r10,0(r11)
		PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r10.u64);
		// stw r10,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	}
loc_82460F84:
	// lis r11,-32165
	// lis r10,-32165
	// addi r11,r11,2592
	ctx.r11.s64 = ctx.r11.s64 + 2592;
	// addi r4,r10,2600
	ctx.r4.s64 = ctx.r10.s64 + 2600;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// cmplw cr6,r3,r4
	// bge cr6,0x82460fb8
while (ctx.r3.u32 < ctx.r4.u32) {
	loc_82460FA0:
		// lwz r9,0(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// cmplwi cr6,r9,0
		// bne cr6,0x82460fb8
		if (ctx.r9.u32 != 0) goto loc_82460FB8;
		// addi r3,r3,4
		ctx.r3.s64 = ctx.r3.s64 + 4;
		// cmplw cr6,r3,r4
		// blt cr6,0x82460fa0
}
loc_82460FB8:
	// lwz r8,0(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r8.u32);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r30,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__phInst_0FE8_v12"))) PPC_WEAK_FUNC(phInst_0FE8_v12);
PPC_FUNC_IMPL(__imp__phInst_0FE8_v12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	ctx.r11.s64 = -2107965440;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lwz r3,30860(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30860);  /* glob:lbl_825E788C @ 0x825e788c */
	// cmplwi cr6,r3,0
	// bne cr6,0x8246102c
	if (ctx.r3.u32 == 0) {
		// addi r3,r31,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// li r3,0
		ctx.r3.s64 = 0;
		// b 0x8246104c
	} else {
		// lwz r10,8(r11)
		// bctrl
		VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// addi r3,r31,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_8246104C:
	// blr
	return;
}

__attribute__((alias("__imp__game_1068"))) PPC_WEAK_FUNC(game_1068);
PPC_FUNC_IMPL(__imp__game_1068) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lis r11,-32162
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// lwz r11,30860(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30860);
	// cmplwi cr6,r11,0
	// bne cr6,0x824610b4
	if (ctx.r11.u32 == 0) {
	loc_824610A4:
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// lis r3,-32768
		// ori r3,r3,16389
		ctx.r3.u64 = ctx.r3.u64 | 16389;
		// b 0x824610cc
		// blr
		return;
	}
loc_824610B4:
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// cmplwi cr6,r11,0
	// beq cr6,0x824610a4
	if (ctx.r11.u32 == 0) {
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// lis r3,-32768
		// ori r3,r3,16389
		ctx.r3.u64 = ctx.r3.u64 | 16389;
		// b 0x824610cc
		// blr
		return;
	}
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
loc_824610CC:
	// blr
	return;
}

__attribute__((alias("__imp__msgMsgSink_10E8_wrh"))) PPC_WEAK_FUNC(msgMsgSink_10E8_wrh);
PPC_FUNC_IMPL(__imp__msgMsgSink_10E8_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=128, savegprlr_27
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r27,r6
	var_r27 = ctx.r6.u32;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lwz r3,30860(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30860);  /* glob:lbl_825E788C @ 0x825e788c */
	// cmplwi cr6,r3,0
	// bne cr6,0x8246113c
	if (ctx.r3.u32 == 0) {
		// addi r3,r31,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// lis r3,-32768
		// ori r3,r3,16389
		ctx.r3.u64 = ctx.r3.u64 | 16389;
		return;
	}
loc_8246113C:
	// mr r7,r27
	ctx.r7.u64 = var_r27;
	// mr r6,r28
	ctx.r6.u64 = var_r28;
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x82469910
	ke_9910(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__phInst_1168_p28"))) PPC_WEAK_FUNC(phInst_1168_p28);
PPC_FUNC_IMPL(__imp__phInst_1168_p28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lwz r3,30860(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30860);  /* glob:lbl_825E788C @ 0x825e788c */
	// cmplwi cr6,r3,0
	// bne cr6,0x824611b4
	if (ctx.r3.u32 == 0) {
		// addi r3,r31,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// lis r3,-32768
		// ori r3,r3,16389
		ctx.r3.u64 = ctx.r3.u64 | 16389;
		// b 0x824611cc
	} else {
	loc_824611B4:
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x82469a90
		ke_9A90(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// addi r3,r31,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
	}
loc_824611CC:
	// blr
	return;
}

__attribute__((alias("__imp__RtlEnterCriticalSection_11E8"))) PPC_WEAK_FUNC(RtlEnterCriticalSection_11E8);
PPC_FUNC_IMPL(__imp__RtlEnterCriticalSection_11E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lwz r11,30860(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30860);
	// cmplwi cr6,r11,0
	// bne cr6,0x82461234
	if (ctx.r11.u32 == 0) {
	loc_82461220:
		// addi r3,r31,4
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
		// bl 0x82585dfc
		__imp__RtlLeaveCriticalSection(ctx, base);
		// lis r3,-32768
		// ori r3,r3,16389
		ctx.r3.u64 = ctx.r3.u64 | 16389;
		// b 0x82461258
		// blr
		return;
	}
loc_82461234:
	// lwz r3,20(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplwi cr6,r3,0
	// beq cr6,0x82461220
	if (ctx.r3.u32 == 0) goto loc_82461220;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x8246bb60
	RtlEnterCriticalSection_BB60_2hr(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
loc_82461258:
	// blr
	return;
}

__attribute__((alias("__imp__game_1270"))) PPC_WEAK_FUNC(game_1270);
PPC_FUNC_IMPL(__imp__game_1270) {
	PPC_FUNC_PROLOGUE();
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x82460f10
	phInst_0F10_p28(ctx, base);
	return;
}

__attribute__((alias("__imp__game_1278"))) PPC_WEAK_FUNC(game_1278);
PPC_FUNC_IMPL(__imp__game_1278) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8246d138
	phInst_D138_g(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x824612c0
	if ((int32_t)var_r30 >= 0) {
		// lwz r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r11,0(r29)
		PPC_STORE_U32(var_r29 + 0, ctx.r11.u32);
	}
loc_824612C0:
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__game_12D8"))) PPC_WEAK_FUNC(game_12D8);
PPC_FUNC_IMPL(__imp__game_12D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// lis r11,-32165
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r11,8992
	var_r31 = (uint32_t)(ctx.r11.s64 + 8992);  // lbl_825B2320 @ 0x825b2320
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585e0c
	__imp__RtlEnterCriticalSection(ctx, base);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x8246d720
	phInst_D720_g(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r30,0
	// blt cr6,0x82461320
	if ((int32_t)var_r30 >= 0) {
		// lwz r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
		// stw r11,0(r29)
		PPC_STORE_U32(var_r29 + 0, ctx.r11.u32);
	}
loc_82461320:
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82585dfc
	__imp__RtlLeaveCriticalSection(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__ph_vt57D8_45_1338"))) PPC_WEAK_FUNC(ph_vt57D8_45_1338);
PPC_FUNC_IMPL(__imp__ph_vt57D8_45_1338) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r4,8(r4)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__ph_vt57D8_44_1350"))) PPC_WEAK_FUNC(ph_vt57D8_44_1350);
PPC_FUNC_IMPL(__imp__ph_vt57D8_44_1350) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r31,r30,16
	var_r31 = (uint32_t)(var_r30 + 16);
	// li r3,0
	ctx.r3.s64 = 0;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0);
	// cmplwi cr6,r11,0
	// beq cr6,0x82461390
	if (ctx.r11.u32 != 0) {
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// bl 0x8258656c
		__imp__XAudioUnregisterRenderDriverClient(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x824613b0
		if (ctx.r3.s32 < 0) {
			return;
		}
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	}
loc_82461390:
	// cmplwi cr6,r29,0
	// beq cr6,0x824613b0
	if (var_r29 != 0) {
		// lwz r10,12(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 12);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// stw r29,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r29);
		// stw r10,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r10.u32);
		// bl 0x8258655c
		__imp__XAudioRegisterRenderDriverClient(ctx, base);
	}
loc_824613B0:
	return;
}

__attribute__((alias("__imp__ph_vt57D8_30_13B8"))) PPC_WEAK_FUNC(ph_vt57D8_30_13B8);
PPC_FUNC_IMPL(__imp__ph_vt57D8_30_13B8) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,-4
	ctx.r3.s64 = ctx.r3.s64 + -4;
	// b 0x82461498
	aud_1498(ctx, base);
	return;
}

__attribute__((alias("__imp__ph_vt57D8_37_13C0"))) PPC_WEAK_FUNC(ph_vt57D8_37_13C0);
PPC_FUNC_IMPL(__imp__ph_vt57D8_37_13C0) {
	PPC_FUNC_PROLOGUE();
	// addi r11,r3,4
	ctx.r11.s64 = ctx.r3.s64 + 4;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// addi r3,r10,1
	ctx.r3.s64 = ctx.r10.s64 + 1;
	// stw r3,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r3.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_38_13D8"))) PPC_WEAK_FUNC(ph_vt57D8_38_13D8);
PPC_FUNC_IMPL(__imp__ph_vt57D8_38_13D8) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// addi r11,r3,4
	ctx.r11.s64 = ctx.r3.s64 + 4;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// addic. r3,r10,-1
	ctx.xer.ca = ctx.r10.u32 > 0;
	ctx.r3.s64 = ctx.r10.s64 + -1;
	// stw r3,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r3.u32);
	// bne 0x82461410
	if (ctx.r3.s32 == 0) {
		// lwz r9,0(r11)
  // [ph4a] vtable load collapsed
		// mr r3,r11
		ctx.r3.u64 = ctx.r11.u64;
		// lwz r8,12(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r11.u32, 3, ctx, base);  // pattern-B slot 3 (byte +12)
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_82461410:
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_39_1420"))) PPC_WEAK_FUNC(ph_vt57D8_39_1420);
PPC_FUNC_IMPL(__imp__ph_vt57D8_39_1420) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// li r10,2
	ctx.r10.s64 = 2;
	// li r3,0
	ctx.r3.s64 = 0;
	// sth r11,2(r4)
	PPC_STORE_U16(ctx.r4.u32 + 2, ctx.r11.u16);
	// stb r10,0(r4)
	PPC_STORE_U8(ctx.r4.u32 + 0, ctx.r10.u8);
	// stb r11,4(r4)
	PPC_STORE_U8(ctx.r4.u32 + 4, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__ph_vt57D8_43_1440"))) PPC_WEAK_FUNC(ph_vt57D8_43_1440);
PPC_FUNC_IMPL(__imp__ph_vt57D8_43_1440) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// bl 0x82465da8
	util_5DA8(ctx, base);
	// cmpwi cr6,r3,0
	// blt cr6,0x82461474
	if (ctx.r3.s32 >= 0) {
		// lwz r4,88(r1)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x8258657c
		__imp__XAudioSubmitRenderDriverFrame(ctx, base);
	}
loc_82461474:
	// blr
	return;
}

__attribute__((alias("__imp__phInst_1488_sp"))) PPC_WEAK_FUNC(phInst_1488_sp);
PPC_FUNC_IMPL(__imp__phInst_1488_sp) {
	PPC_FUNC_PROLOGUE();
	// li r11,20
	ctx.r11.s64 = 20;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r11,0(r4)
	PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__aud_1498"))) PPC_WEAK_FUNC(aud_1498);
PPC_FUNC_IMPL(__imp__aud_1498) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32256
	// lis r10,-32256
	// addi r11,r11,22636
	ctx.r11.s64 = ctx.r11.s64 + 22636;
	// addi r10,r10,22608
	ctx.r10.s64 = ctx.r10.s64 + 22608;
	// lwz r3,16(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// stw r10,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r10.u32);
	// beq cr6,0x824614e4
	if (ctx.r3.u32 != 0) {
		// bl 0x8258656c
		__imp__XAudioUnregisterRenderDriverClient(ctx, base);
		// cmpwi cr6,r3,0
		// blt cr6,0x824614e4
		if (ctx.r3.s32 < 0) {
			// lis r11,-32256
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// addi r11,r11,15792
			ctx.r11.s64 = ctx.r11.s64 + 15792;
			// stw r11,4(r31)
			PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
			// blr
			return;
		}
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,16(r31)
		PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	}
loc_824614E4:
	// lis r11,-32256
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r11,r11,15792
	ctx.r11.s64 = ctx.r11.s64 + 15792;
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__phInst_1508_2hr"))) PPC_WEAK_FUNC(phInst_1508_2hr);
PPC_FUNC_IMPL(__imp__phInst_1508_2hr) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	// li r8,1
	ctx.r8.s64 = 1;
	// addi r11,r11,22456
	ctx.r11.s64 = ctx.r11.s64 + 22456;
	// lis r10,-32256
	// lis r9,-32256
	// addi r10,r10,22636
	ctx.r10.s64 = ctx.r10.s64 + 22636;
	// stw r8,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r8.u32);
	// addi r9,r9,22608
	ctx.r9.s64 = ctx.r9.s64 + 22608;
	// stw r11,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r11.u32);
	// lwz r7,4(r4)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
	// stw r7,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r7.u32);
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r10.u32);
	// stw r9,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__phInst_1540_sp"))) PPC_WEAK_FUNC(phInst_1540_sp);
PPC_FUNC_IMPL(__imp__phInst_1540_sp) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r27,r5
	var_r27 = ctx.r5.u32;
	// li r4,20
	ctx.r4.s64 = 20;
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// lwz r10,20(r11)
	// bctrl
	VCALL(ctx.r3.u32, 5, ctx, base);  // vtable slot 5 (byte +20)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmplwi cr6,r30,0
	// beq cr6,0x824615e4
	if (var_r30 != 0) {
		// lis r11,-32256
		// addi r31,r30,4
		var_r31 = (uint32_t)(var_r30 + 4);
		// addi r11,r11,22456
		ctx.r11.s64 = ctx.r11.s64 + 22456;
		// li r8,1
		ctx.r8.s64 = 1;
		// lis r10,-32256
		// lis r9,-32256
		// addi r10,r10,22636
		ctx.r10.s64 = ctx.r10.s64 + 22636;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
		// addi r9,r9,22608
		ctx.r9.s64 = ctx.r9.s64 + 22608;
		// stw r8,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r8.u32);
		// rotlwi r6,r10,0
		ctx.r6.u64 = ctx.r10.u32;
		// lwz r7,4(r29)
		ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 4);
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// lwz r11,32(r6)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r6.u32 + 32);
		// stw r7,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r7.u32);
		// stw r10,0(r30)
		PPC_STORE_U32(var_r30 + 0, ctx.r10.u32);
		// stw r9,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r9.u32);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// cmpwi cr6,r29,0
		// blt cr6,0x824615f4
		if ((int32_t)var_r29 < 0) {
			// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lwz r9,12(r10)
  // [ph4a] slot load collapsed
			// bctrl
			VCALL(var_r31, 3, ctx, base);  // pattern-B slot 3 (byte +12)
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			return;
		}
		// stw r30,0(r27)
		PPC_STORE_U32(var_r27 + 0, var_r30);
		return;
	}
loc_824615E4:
	// lis r3,-32761
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	return;
}

__attribute__((alias("__imp__game_vt58B0_11"))) PPC_WEAK_FUNC(game_vt58B0_11);
PPC_FUNC_IMPL(__imp__game_vt58B0_11) {
	PPC_FUNC_PROLOGUE();
	// li r11,6
	ctx.r11.s64 = 6;
	// li r10,3750
	ctx.r10.s64 = 3750;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r3,0
	ctx.r3.s64 = 0;
	// stb r11,0(r4)
	PPC_STORE_U8(ctx.r4.u32 + 0, ctx.r11.u8);
	// sth r10,2(r4)
	PPC_STORE_U16(ctx.r4.u32 + 2, ctx.r10.u16);
	// stb r9,4(r4)
	PPC_STORE_U8(ctx.r4.u32 + 4, ctx.r9.u8);
	// blr
	return;
}

__attribute__((alias("__imp__game_vt58B0_12"))) PPC_WEAK_FUNC(game_vt58B0_12);
PPC_FUNC_IMPL(__imp__game_vt58B0_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=176, savegprlr_26
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r26,r6
	var_r26 = ctx.r6.u32;
	// li r27,0
	var_r27 = 0;
	// cmpwi cr6,r11,2
	// beq cr6,0x8246178c
	if (ctx.r11.s32 != 2) {
		// cmpwi cr6,r11,3
		// beq cr6,0x8246167c
		if (ctx.r11.s32 != 3) {
			// lis r3,-32761
			// ori r3,r3,87
			ctx.r3.u64 = ctx.r3.u64 | 87;
			return;
		}
	loc_8246167C:
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x82568800
		rage_8800(ctx, base);
		// bl 0x8258653c
		__imp__KeRaiseIrqlToDpcLevel(ctx, base);
		// lis r11,-32162
		// mr r28,r3
		var_r28 = ctx.r3.u32;
		// addi r31,r11,30832
		var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
		// mr r30,r13
		var_r30 = ctx.r13.u32;
		// lwz r9,4(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 4);
		// cmpwi cr6,r9,0
		// beq cr6,0x824616b0
		if (ctx.r9.s32 != 0) {
			// lwz r11,8(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
			// cmplw cr6,r30,r11
			// beq cr6,0x824616c4
			if (var_r30 == ctx.r11.u32) goto loc_824616C4;
		}
	loc_824616B0:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8258634c
		__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
		// stw r30,8(r31)
		PPC_STORE_U32(var_r31 + 8, var_r30);
		// stb r28,12(r31)
		PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r28);
		// lwz r9,4(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 4);
	loc_824616C4:
		// ld r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// addis r10,r29,5
		ctx.r10.s64 = (int64_t)(int32_t)var_r29 + 327680;
		// addi r9,r9,1
		ctx.r9.s64 = ctx.r9.s64 + 1;
		// addi r10,r10,-17900
		ctx.r10.s64 = ctx.r10.s64 + -17900;
		// mr r8,r13
		ctx.r8.u64 = ctx.r13.u64;
		// std r11,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r11.u64);
		// addis r11,r29,5
		ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 327680;
		// stw r9,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r9.u32);
		// addi r11,r11,-17892
		ctx.r11.s64 = ctx.r11.s64 + -17892;
		// ld r7,0(r10)
		ctx.r7.u64 = PPC_LOAD_U64(ctx.r10.u32 + 0);
		// ld r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
		// std r7,96(r1)
		PPC_STORE_U64(ctx.r1.u32 + 96, ctx.r7.u64);
		// lfd f0,88(r1)
		ctx.fpscr.disableFlushMode();
		ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// std r9,88(r1)
		PPC_STORE_U64(ctx.r1.u32 + 88, ctx.r9.u64);
		// fcfid f13,f0
		ctx.f13.f64 = double(ctx.f0.s64);
		// lis r9,-32256
		ctx.r9.s64 = -2113929216;
		// lfd f0,22680(r9)
		ctx.f0.u64 = PPC_LOAD_U64(ctx.r9.u32 + 22680);  /* glob:lbl_82005898 @ 0x82005898 */
		// lfd f11,96(r1)
		ctx.f11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 96);
		// fcfid f9,f11
		ctx.f9.f64 = double(ctx.f11.s64);
		// fdiv f7,f9,f13
		ctx.f7.f64 = ctx.f9.f64 / ctx.f13.f64;
		// lfd f12,88(r1)
		ctx.f12.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// fcfid f10,f12
		ctx.f10.f64 = double(ctx.f12.s64);
		// fmul f8,f10,f0
		ctx.f8.f64 = ctx.f10.f64 * ctx.f0.f64;
		// fdiv f6,f7,f8
		ctx.f6.f64 = ctx.f7.f64 / ctx.f8.f64;
		// frsp f5,f6
		ctx.f5.f64 = double(float(ctx.f6.f64));
		// stfs f5,0(r26)
		temp.f32 = float(ctx.f5.f64);
		PPC_STORE_U32(var_r26 + 0, temp.u32);
		// std r27,0(r10)
		PPC_STORE_U64(ctx.r10.u32 + 0, var_r27);
		// std r27,0(r11)
		PPC_STORE_U64(ctx.r11.u32 + 0, var_r27);
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// cmpwi cr6,r11,0
		// beq cr6,0x82461850
		if (ctx.r11.s32 == 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// lwz r10,8(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r8,r10
		// bne cr6,0x82461850
		if (ctx.r8.u32 != ctx.r10.u32) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x82461850
		if (ctx.r11.s32 != 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// mr r11,r27
		ctx.r11.u64 = var_r27;
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
		// mr r3,r27
		ctx.r3.u64 = var_r27;
		return;
	}
loc_8246178C:
	// bl 0x8258653c
	__imp__KeRaiseIrqlToDpcLevel(ctx, base);
	// lis r11,-32162
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// addi r31,r11,30832
	var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
	// mr r30,r13
	var_r30 = ctx.r13.u32;
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmpwi cr6,r11,0
	// beq cr6,0x824617b8
	if (ctx.r11.s32 != 0) {
		// lwz r9,8(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r30,r9
		// beq cr6,0x824617d8
		if (var_r30 == ctx.r9.u32) goto loc_824617D8;
	}
loc_824617B8:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x8258634c
	__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
	// mr r9,r30
	ctx.r9.u64 = var_r30;
	// mr r30,r28
	var_r30 = (uint32_t)(var_r28);
	// stw r9,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r9.u32);
	// stb r30,12(r31)
	PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r30);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// b 0x824617dc
	goto loc_824617DC;
loc_824617D8:
	// lbz r30,12(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
loc_824617DC:
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// mr r8,r13
	ctx.r8.u64 = ctx.r13.u64;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// lwz r6,16(r29)
	ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 16);
	// lwz r10,4(r6)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r6.u32 + 4);
	// add r5,r10,r29
	ctx.r5.u64 = ctx.r10.u64 + var_r29;
	// lis r10,-32256
	// lfs f4,108(r5)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 108);
	ctx.f4.f64 = double(temp.f32);
	// lfs f0,22092(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 22092);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f31,f4,f0
	var_f31 = double(float(ctx.f4.f64 * ctx.f0.f64));
	// beq cr6,0x82461840
	if (!(ctx.cr6.eq)) {
		// cmplw cr6,r8,r9
		// bne cr6,0x82461840
		if (ctx.r8.u32 != ctx.r9.u32) goto loc_82461840;
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x82461840
		if (ctx.r11.s32 != 0) goto loc_82461840;
		// mr r11,r27
		ctx.r11.u64 = var_r27;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_82461840:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,18992(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 18992);  /* glob:lbl_82004A30 @ 0x82004a30 */
	ctx.f0.f64 = double(temp.f32);
	// fmuls f3,f31,f0
	ctx.f3.f64 = double(float(var_f31 * ctx.f0.f64));
	// stfs f3,0(r26)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(var_r26 + 0, temp.u32);
loc_82461850:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__game_vt58B0_13"))) PPC_WEAK_FUNC(game_vt58B0_13);
PPC_FUNC_IMPL(__imp__game_vt58B0_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=240, savegprlr_26
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// li r26,0
	var_r26 = 0;
	// cmplwi cr6,r11,1
	// blt cr6,0x82461978
	if (ctx.r11.u32 >= 1) {
		// beq cr6,0x82461918
		if (!(ctx.cr6.eq)) {
			// cmplwi cr6,r11,3
			// blt cr6,0x824618a8
			if (ctx.r11.u32 >= 3) {
				// lis r26,-32761
				var_r26 = (uint32_t)(-2147024896);
				// ori r26,r26,87
				var_r26 = (uint32_t)(var_r26 | 87);
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
		loc_824618A8:
			// lfs f31,0(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 0);
			var_f31 = double(temp.f32);
			// bl 0x8258653c
			__imp__KeRaiseIrqlToDpcLevel(ctx, base);
			// lis r11,-32162
			// mr r29,r3
			var_r29 = ctx.r3.u32;
			// addi r31,r11,30832
			var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
			// mr r30,r13
			var_r30 = ctx.r13.u32;
			// lwz r11,4(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
			// cmpwi cr6,r11,0
			// beq cr6,0x824618d8
			if (ctx.r11.s32 != 0) {
				// lwz r10,8(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
				// cmplw cr6,r30,r10
				// beq cr6,0x824618ec
				if (var_r30 == ctx.r10.u32) goto loc_824618EC;
			}
		loc_824618D8:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x8258634c
			__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
			// stw r30,8(r31)
			PPC_STORE_U32(var_r31 + 8, var_r30);
			// stb r29,12(r31)
			PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r29);
			// lwz r11,4(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		loc_824618EC:
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// stw r11,4(r31)
			PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
			// lis r11,-32256
			// lwz r10,16(r27)
			ctx.r10.u64 = PPC_LOAD_U32(var_r27 + 16);
			// lfs f0,22092(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22092);
			ctx.f0.f64 = double(temp.f32);
			// lwz r11,4(r10)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
			// fmuls f1,f31,f0
			ctx.f1.f64 = double(float(var_f31 * ctx.f0.f64));
			// add r11,r11,r27
			ctx.r11.u64 = ctx.r11.u64 + var_r27;
			// addi r3,r11,16
			ctx.r3.s64 = ctx.r11.s64 + 16;
			// bl 0x8246dcf8
			game_DCF8_h(ctx, base);
			// b 0x82461aa4
			goto loc_82461AA4;
		}
	loc_82461918:
		// lwz r28,0(r6)
		var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 0));
		// bl 0x8258653c
		__imp__KeRaiseIrqlToDpcLevel(ctx, base);
		// lis r11,-32162
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// addi r31,r11,30832
		var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
		// mr r30,r13
		var_r30 = ctx.r13.u32;
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// cmpwi cr6,r11,0
		// beq cr6,0x82461948
		if (ctx.r11.s32 != 0) {
			// lwz r10,8(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
			// cmplw cr6,r30,r10
			// beq cr6,0x8246195c
			if (var_r30 == ctx.r10.u32) goto loc_8246195C;
		}
	loc_82461948:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8258634c
		__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
		// stw r30,8(r31)
		PPC_STORE_U32(var_r31 + 8, var_r30);
		// stb r29,12(r31)
		PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r29);
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	loc_8246195C:
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r3,r27,16
		ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 16;
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bl 0x8246fa28
		phBoundBVH_FA28_p45(ctx, base);
		// b 0x82461aa4
	} else {
	loc_82461978:
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// lwz r31,0(r6)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 0));
		// bl 0x8246d830
		atSingleton_D830_p42(ctx, base);
		// lwz r9,0(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0);
		// stw r9,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
		// lbz r8,4(r31)
		ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 4);
		// stw r8,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r8.u32);
		// lbz r7,6(r31)
		ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 6);
		// stw r7,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r7.u32);
		// lbz r6,7(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 7);
		// stw r6,92(r1)
		PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r6.u32);
		// lbz r5,8(r31)
		ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 8);
		// stw r5,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r5.u32);
		// lbz r4,9(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 9);
		// stw r4,100(r1)
		PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r4.u32);
		// lbz r3,10(r31)
		ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 10);
		// stw r3,104(r1)
		PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r3.u32);
		// lbz r11,11(r31)
		ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 11);
		// stw r11,108(r1)
		PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
		// lis r11,-32256
		// lbz r10,12(r31)
		ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 12);
		// stw r10,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r10.u32);
		// lbz r9,13(r31)
		ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 13);
		// stw r9,116(r1)
		PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r9.u32);
		// lbz r8,14(r31)
		ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 14);
		// stw r8,120(r1)
		PPC_STORE_U32(ctx.r1.u32 + 120, ctx.r8.u32);
		// lbz r7,15(r31)
		ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 15);
		// stw r7,124(r1)
		PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r7.u32);
		// lbz r6,5(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 5);
		// stw r6,128(r1)
		PPC_STORE_U32(ctx.r1.u32 + 128, ctx.r6.u32);
		// lfs f0,16(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 16);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,132(r1)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
		// lfs f13,20(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 20);
		ctx.f13.f64 = double(temp.f32);
		// stfs f13,136(r1)
		temp.f32 = float(ctx.f13.f64);
		PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
		// lfs f12,24(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 24);
		ctx.f12.f64 = double(temp.f32);
		// stfs f12,140(r1)
		temp.f32 = float(ctx.f12.f64);
		PPC_STORE_U32(ctx.r1.u32 + 140, temp.u32);
		// lfs f11,28(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 28);
		ctx.f11.f64 = double(temp.f32);
		// stfs f11,144(r1)
		temp.f32 = float(ctx.f11.f64);
		PPC_STORE_U32(ctx.r1.u32 + 144, temp.u32);
		// lfs f10,32(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 32);
		ctx.f10.f64 = double(temp.f32);
		// lfs f0,22092(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22092);
		ctx.f0.f64 = double(temp.f32);
		// lis r11,-32256
		// stfs f10,148(r1)
		temp.f32 = float(ctx.f10.f64);
		PPC_STORE_U32(ctx.r1.u32 + 148, temp.u32);
		// lfs f9,36(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 36);
		ctx.f9.f64 = double(temp.f32);
		// stfs f9,152(r1)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r1.u32 + 152, temp.u32);
		// lfs f8,40(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 40);
		ctx.f8.f64 = double(temp.f32);
		// fmuls f7,f8,f0
		ctx.f7.f64 = double(float(ctx.f8.f64 * ctx.f0.f64));
		// stfs f7,156(r1)
		temp.f32 = float(ctx.f7.f64);
		PPC_STORE_U32(ctx.r1.u32 + 156, temp.u32);
		// lfs f6,44(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 44);
		ctx.f6.f64 = double(temp.f32);
		// lfs f0,22688(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22688);
		ctx.f0.f64 = double(temp.f32);
		// fmuls f5,f6,f0
		ctx.f5.f64 = double(float(ctx.f6.f64 * ctx.f0.f64));
		// stfs f5,160(r1)
		temp.f32 = float(ctx.f5.f64);
		PPC_STORE_U32(ctx.r1.u32 + 160, temp.u32);
		// bl 0x8258653c
		__imp__KeRaiseIrqlToDpcLevel(ctx, base);
		// lis r11,-32162
		// mr r29,r3
		var_r29 = ctx.r3.u32;
		// addi r31,r11,30832
		var_r31 = (uint32_t)(ctx.r11.s64 + 30832);  // lbl_825E7870 @ 0x825e7870
		// mr r30,r13
		var_r30 = ctx.r13.u32;
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// cmpwi cr6,r11,0
		// beq cr6,0x82461a70
		if (ctx.r11.s32 != 0) {
			// lwz r10,8(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
			// cmplw cr6,r30,r10
			// beq cr6,0x82461a84
			if (var_r30 == ctx.r10.u32) goto loc_82461A84;
		}
	loc_82461A70:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8258634c
		__imp__KeAcquireSpinLockAtRaisedIrql(ctx, base);
		// stw r30,8(r31)
		PPC_STORE_U32(var_r31 + 8, var_r30);
		// stb r29,12(r31)
		PPC_STORE_U8(var_r31 + 12, (uint8_t)var_r29);
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	loc_82461A84:
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// lwz r5,16(r27)
		ctx.r5.u64 = PPC_LOAD_U32(var_r27 + 16);
		// lwz r11,4(r5)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 4);
		// add r11,r11,r27
		ctx.r11.u64 = ctx.r11.u64 + var_r27;
		// addi r3,r11,16
		ctx.r3.s64 = ctx.r11.s64 + 16;
		// bl 0x8246dd10
		phBoundCapsule_DD10_g(ctx, base);
	}
loc_82461AA4:
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
	// mr r10,r13
	ctx.r10.u64 = ctx.r13.u64;
	// cmpwi cr6,r11,0
	// beq cr6,0x82461aec
	if (ctx.r11.s32 != 0) {
		// lwz r9,8(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r10,r9
		// bne cr6,0x82461aec
		if (ctx.r10.u32 != ctx.r9.u32) {
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// cmpwi cr6,r11,0
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// bne cr6,0x82461aec
		if (ctx.r11.s32 != 0) {
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
		// lbz r30,12(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U8(var_r31 + 12));
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stb r11,12(r31)
		PPC_STORE_U8(var_r31 + 12, ctx.r11.u8);
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
		// bl 0x8258631c
		__imp__KeReleaseSpinLockFromRaisedIrql(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x8258654c
		__imp__KfLowerIrql(ctx, base);
	}
loc_82461AEC:
	// mr r3,r26
	ctx.r3.u64 = var_r26;
	return;
}

__attribute__((alias("__imp__phInst_1B00_p39"))) PPC_WEAK_FUNC(phInst_1B00_p39);
PPC_FUNC_IMPL(__imp__phInst_1B00_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r20 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r21 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r19 = 0;
	uint32_t var_r15 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r18 = 0;
	uint32_t var_r17 = 0;
	uint32_t var_r16 = 0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8242f864
	ctx.lr = 0x82461B08;
	__savegprlr_15(ctx, base);
	// addi r20,r3,20
	var_r20 = (uint32_t)(ctx.r3.s64 + 20);
	// lwz r8,12(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// subf r23,r8,r9
	var_r23 = (uint32_t)(ctx.r9.s64 - ctx.r8.s64);
	// lfs f0,0(r20)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r20 + 0);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r23,4
	// stfs f0,0(r5)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// bgt cr6,0x82461b38
	if (var_r23 <= 4) {
		// subf r11,r9,r8
		ctx.r11.s64 = ctx.r8.s64 - ctx.r9.s64;
		// li r6,0
		ctx.r6.s64 = 0;
		// cmplwi cr6,r11,4
		// ble cr6,0x82461b3c
		if (ctx.r11.u32 <= 4) goto loc_82461B3C;
	}
loc_82461B38:
	// li r6,1
	ctx.r6.s64 = 1;
loc_82461B3C:
	// lis r11,-32256
	// addi r22,r3,4
	var_r22 = (uint32_t)(ctx.r3.s64 + 4);
	// addi r10,r1,-176
	ctx.r10.s64 = ctx.r1.s64 + -176;
	// lfs f0,22696(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22696);
	ctx.f0.f64 = double(temp.f32);
	// li r11,1
	ctx.r11.s64 = 1;
	// lfs f12,0(r22)
	temp.u32 = PPC_LOAD_U32(var_r22 + 0);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f13,f12,f0
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// fctidz f11,f13
	ctx.f11.s64 = (ctx.f13.f64 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&ctx.f13.f64));
	// stfiwx f11,0,r10
	PPC_STORE_U32(ctx.r10.u32, ctx.f11.u32);
	// lwz r7,-176(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + -176);
	// rlwinm r28,r7,31,1,31
	var_r28 = (uint32_t)(__builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF);
	// cmplwi cr6,r28,256
	// blt cr6,0x82461b74
	if (var_r28 >= 256) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_82461B74:
	// addi r21,r3,24
	var_r21 = (uint32_t)(ctx.r3.s64 + 24);
	// cmplwi cr6,r11,0
	// lwz r10,0(r21)
	ctx.r10.u64 = PPC_LOAD_U32(var_r21 + 0);
	// beq cr6,0x82461fd8
	if (ctx.r11.u32 != 0) {
		// lis r11,-32256
		// lis r7,-32248
		// li r24,1
		var_r24 = 1;
		// cmpwi cr6,r28,4
		// lfs f11,15788(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
		ctx.f11.f64 = double(temp.f32);
		// addi r11,r3,32
		ctx.r11.s64 = ctx.r3.s64 + 32;
		// lfs f0,-25744(r7)
		temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25744);
		ctx.f0.f64 = double(temp.f32);
		// fsubs f13,f11,f12
		ctx.f13.f64 = double(float(ctx.f11.f64 - ctx.f12.f64));
		// blt cr6,0x82461cec
		if ((int32_t)var_r28 >= 4) {
			// addi r6,r10,2
			ctx.r6.s64 = ctx.r10.s64 + 2;
			// rlwinm r29,r28,30,2,31
			var_r29 = (uint32_t)(__builtin_rotateleft64(var_r28 | (var_r28 << 32), 30) & 0x3FFFFFFF);
			// subf r30,r9,r10
			var_r30 = (uint32_t)(ctx.r10.s64 - ctx.r9.s64);
			// subf r9,r8,r10
			ctx.r9.s64 = ctx.r10.s64 - ctx.r8.s64;
			// rlwinm r8,r6,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
			// rlwinm r27,r29,2,0,29
			var_r27 = (uint32_t)(__builtin_rotateleft64(var_r29 | (var_r29 << 32), 2) & 0xFFFFFFFC);
			// addi r31,r9,2
			var_r31 = (uint32_t)(ctx.r9.s64 + 2);  // addr:0x82000002
			// add r8,r8,r11
			ctx.r8.u64 = ctx.r8.u64 + ctx.r11.u64;
			// addi r7,r4,8
			ctx.r7.s64 = ctx.r4.s64 + 8;
			// addi r9,r5,12
			ctx.r9.s64 = ctx.r5.s64 + 12;
			// subf r25,r5,r4
			var_r25 = (uint32_t)(ctx.r4.s64 - ctx.r5.s64);
			// addi r6,r30,2
			ctx.r6.s64 = (int64_t)(int32_t)var_r30 + 2;
			// add r10,r27,r10
			ctx.r10.u64 = var_r27 + ctx.r10.u64;
			// addi r24,r27,1
			var_r24 = (uint32_t)(var_r27 + 1);
		loc_82461BE0:
			// mr r27,r30
			var_r27 = (uint32_t)(var_r30);
			// fmr f10,f12
			ctx.fpscr.disableFlushMode();
			ctx.f10.f64 = ctx.f12.f64;
			// addi r19,r6,-1
			var_r19 = (uint32_t)(ctx.r6.s64 + -1);
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// rlwinm r15,r27,2,16,29
			var_r15 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFC);
			// lfs f9,-8(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -8);
			ctx.f9.f64 = double(temp.f32);
			// add r26,r30,r23
			var_r26 = (uint32_t)(var_r30 + var_r23);
			// lfs f8,-4(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -4);
			ctx.f8.f64 = double(temp.f32);
			// mr r27,r19
			var_r27 = (uint32_t)(var_r19);
			// lfs f7,0(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 0);
			ctx.f7.f64 = double(temp.f32);
			// rlwinm r19,r26,2,16,29
			var_r19 = (uint32_t)(__builtin_rotateleft64(var_r26 | (var_r26 << 32), 2) & 0xFFFC);
			// lfsx f6,r25,r9
			temp.u32 = PPC_LOAD_U32(var_r25 + ctx.r9.u32);
			ctx.f6.f64 = double(temp.f32);
			// addi r18,r31,-1
			var_r18 = (uint32_t)(var_r31 + -1);
			// lfsx f4,r15,r11
			temp.u32 = PPC_LOAD_U32(var_r15 + ctx.r11.u32);
			ctx.f4.f64 = double(temp.f32);
			// mr r26,r31
			var_r26 = (uint32_t)(var_r31);
			// fmuls f3,f4,f13
			ctx.f3.f64 = double(float(ctx.f4.f64 * ctx.f13.f64));
			// rlwinm r18,r18,2,16,29
			var_r18 = (uint32_t)(__builtin_rotateleft64(var_r18 | (var_r18 << 32), 2) & 0xFFFC);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// addi r17,r6,1
			var_r17 = (uint32_t)(ctx.r6.s64 + 1);  // addr:0x82000001
			// lfsx f2,r19,r11
			temp.u32 = PPC_LOAD_U32(var_r19 + ctx.r11.u32);
			ctx.f2.f64 = double(temp.f32);
			// rlwinm r19,r27,2,16,29
			var_r19 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFC);
			// stfs f9,-8(r8)
			temp.f32 = float(ctx.f9.f64);
			PPC_STORE_U32(ctx.r8.u32 + -8, temp.u32);
			// fmr f5,f12
			ctx.f5.f64 = ctx.f12.f64;
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// mr r27,r6
			var_r27 = ctx.r6.u32;
			// addi r16,r31,1
			var_r16 = (uint32_t)(var_r31 + 1);
			// addi r29,r29,-1
			var_r29 = (uint32_t)(var_r29 + -1);
			// lfsx f9,r19,r11
			temp.u32 = PPC_LOAD_U32(var_r19 + ctx.r11.u32);
			ctx.f9.f64 = double(temp.f32);
			// rlwinm r19,r27,2,16,29
			var_r19 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFC);
			// rlwinm r27,r17,2,16,29
			var_r27 = (uint32_t)(__builtin_rotateleft64(var_r17 | (var_r17 << 32), 2) & 0xFFFC);
			// addi r30,r30,4
			var_r30 = (uint32_t)(var_r30 + 4);
			// fmadds f1,f2,f10,f3
			ctx.f1.f64 = double(float(ctx.f2.f64 * ctx.f10.f64 + ctx.f3.f64));
			// lfsx f3,r18,r11
			temp.u32 = PPC_LOAD_U32(var_r18 + ctx.r11.u32);
			ctx.f3.f64 = double(temp.f32);
			// fmuls f4,f9,f13
			ctx.f4.f64 = double(float(ctx.f9.f64 * ctx.f13.f64));
			// stfs f8,-4(r8)
			temp.f32 = float(ctx.f8.f64);
			PPC_STORE_U32(ctx.r8.u32 + -4, temp.u32);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// lfsx f9,r19,r11
			temp.u32 = PPC_LOAD_U32(var_r19 + ctx.r11.u32);
			ctx.f9.f64 = double(temp.f32);
			// rlwinm r18,r26,2,16,29
			var_r18 = (uint32_t)(__builtin_rotateleft64(var_r26 | (var_r26 << 32), 2) & 0xFFFC);
			// stfs f1,-8(r9)
			temp.f32 = float(ctx.f1.f64);
			PPC_STORE_U32(ctx.r9.u32 + -8, temp.u32);
			// fmr f10,f12
			ctx.f10.f64 = ctx.f12.f64;
			// rlwinm r26,r16,2,16,29
			var_r26 = (uint32_t)(__builtin_rotateleft64(var_r16 | (var_r16 << 32), 2) & 0xFFFC);
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// addi r7,r7,16
			ctx.r7.s64 = ctx.r7.s64 + 16;
			// cmplwi cr6,r29,0
			ctx.cr6.compare<uint32_t>(var_r29, 0, ctx.xer);
			// addi r6,r6,4
			ctx.r6.s64 = ctx.r6.s64 + 4;
			// addi r31,r31,4
			var_r31 = (uint32_t)(var_r31 + 4);
			// fmadds f2,f3,f5,f4
			ctx.f2.f64 = double(float(ctx.f3.f64 * ctx.f5.f64 + ctx.f4.f64));
			// lfsx f5,r18,r11
			temp.u32 = PPC_LOAD_U32(var_r18 + ctx.r11.u32);
			ctx.f5.f64 = double(temp.f32);
			// fmuls f8,f9,f13
			ctx.f8.f64 = double(float(ctx.f9.f64 * ctx.f13.f64));
			// stfs f7,0(r8)
			temp.f32 = float(ctx.f7.f64);
			PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// lfsx f3,r27,r11
			temp.u32 = PPC_LOAD_U32(var_r27 + ctx.r11.u32);
			ctx.f3.f64 = double(temp.f32);
			// stfs f2,-4(r9)
			temp.f32 = float(ctx.f2.f64);
			PPC_STORE_U32(ctx.r9.u32 + -4, temp.u32);
			// fmr f1,f12
			ctx.f1.f64 = ctx.f12.f64;
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// fmadds f4,f5,f10,f8
			ctx.f4.f64 = double(float(ctx.f5.f64 * ctx.f10.f64 + ctx.f8.f64));
			// lfsx f10,r26,r11
			temp.u32 = PPC_LOAD_U32(var_r26 + ctx.r11.u32);
			ctx.f10.f64 = double(temp.f32);
			// fmuls f2,f3,f13
			ctx.f2.f64 = double(float(ctx.f3.f64 * ctx.f13.f64));
			// stfs f4,0(r9)
			temp.f32 = float(ctx.f4.f64);
			PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
			// stfs f6,4(r8)
			temp.f32 = float(ctx.f6.f64);
			PPC_STORE_U32(ctx.r8.u32 + 4, temp.u32);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// addi r8,r8,16
			ctx.r8.s64 = ctx.r8.s64 + 16;
			// fmadds f9,f10,f1,f2
			ctx.f9.f64 = double(float(ctx.f10.f64 * ctx.f1.f64 + ctx.f2.f64));
			// stfs f9,4(r9)
			temp.f32 = float(ctx.f9.f64);
			PPC_STORE_U32(ctx.r9.u32 + 4, temp.u32);
			// addi r9,r9,16
			ctx.r9.s64 = ctx.r9.s64 + 16;
			// bne cr6,0x82461be0
			if (!ctx.cr6.eq) goto loc_82461BE0;
			// stfs f12,0(r22)
			temp.f32 = float(ctx.f12.f64);
			PPC_STORE_U32(var_r22 + 0, temp.u32);
		}
	loc_82461CEC:
		// cmplw cr6,r24,r28
		// bgt cr6,0x82461d84
		if (var_r24 <= var_r28) {
			// rlwinm r7,r24,2,0,29
			ctx.r7.u64 = __builtin_rotateleft64(var_r24 | (var_r24 << 32), 2) & 0xFFFFFFFC;
			// lwz r30,8(r3)
			var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 8));
			// subf r9,r24,r28
			ctx.r9.s64 = (int64_t)(int32_t)var_r28 - (int64_t)(int32_t)var_r24;
			// lwz r27,12(r3)
			var_r27 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 12));
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// lfs f12,0(r22)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r22 + 0);
			ctx.f12.f64 = double(temp.f32);
			// addi r9,r9,1
			ctx.r9.s64 = ctx.r9.s64 + 1;
			// add r29,r7,r4
			var_r29 = (uint32_t)(ctx.r7.u64 + ctx.r4.u64);
			// add r6,r8,r11
			ctx.r6.u64 = ctx.r8.u64 + ctx.r11.u64;
			// subf r8,r30,r10
			ctx.r8.s64 = ctx.r10.s64 - (int64_t)(int32_t)var_r30;
			// add r31,r7,r5
			var_r31 = (uint32_t)(ctx.r7.u64 + ctx.r5.u64);
			// addi r7,r29,-4
			ctx.r7.s64 = (int64_t)(int32_t)var_r29 + -4;
			// subf r30,r27,r30
			var_r30 = var_r30 - var_r27;
			// add r10,r9,r10
			ctx.r10.u64 = ctx.r9.u64 + ctx.r10.u64;
		loc_82461D2C:
			// mr r29,r8
			var_r29 = ctx.r8.u32;
			// fmr f8,f12
			ctx.fpscr.disableFlushMode();
			ctx.f8.f64 = ctx.f12.f64;
			// add r27,r30,r8
			var_r27 = (uint32_t)(var_r30 + ctx.r8.u64);
			// lfs f7,0(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 0);
			ctx.f7.f64 = double(temp.f32);
			// rlwinm r29,r29,2,16,29
			var_r29 = (uint32_t)(__builtin_rotateleft64(var_r29 | (var_r29 << 32), 2) & 0xFFFC);
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// rlwinm r27,r27,2,16,29
			var_r27 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFC);
			// addi r9,r9,-1
			ctx.r9.s64 = ctx.r9.s64 + -1;
			// addi r8,r8,1
			ctx.r8.s64 = ctx.r8.s64 + 1;
			// addi r7,r7,4
			ctx.r7.s64 = ctx.r7.s64 + 4;
			// lfsx f6,r29,r11
			temp.u32 = PPC_LOAD_U32(var_r29 + ctx.r11.u32);
			ctx.f6.f64 = double(temp.f32);
			// cmplwi cr6,r9,0
			ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
			// fmuls f5,f6,f13
			ctx.f5.f64 = double(float(ctx.f6.f64 * ctx.f13.f64));
			// lfsx f4,r27,r11
			temp.u32 = PPC_LOAD_U32(var_r27 + ctx.r11.u32);
			ctx.f4.f64 = double(temp.f32);
			// stfs f7,0(r6)
			temp.f32 = float(ctx.f7.f64);
			PPC_STORE_U32(ctx.r6.u32 + 0, temp.u32);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// addi r6,r6,4
			ctx.r6.s64 = ctx.r6.s64 + 4;
			// fmadds f3,f4,f8,f5
			ctx.f3.f64 = double(float(ctx.f4.f64 * ctx.f8.f64 + ctx.f5.f64));
			// stfs f3,0(r31)
			temp.f32 = float(ctx.f3.f64);
			PPC_STORE_U32(var_r31 + 0, temp.u32);
			// addi r31,r31,4
			var_r31 = (uint32_t)(var_r31 + 4);
			// bne cr6,0x82461d2c
			if (!ctx.cr6.eq) goto loc_82461D2C;
			// stfs f12,0(r22)
			temp.f32 = float(ctx.f12.f64);
			PPC_STORE_U32(var_r22 + 0, temp.u32);
		}
	loc_82461D84:
		// lwz r24,16(r3)
		var_r24 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 16));
		// lwz r9,8(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// cmplw cr6,r24,r9
		// beq cr6,0x82461d9c
		if (var_r24 != ctx.r9.u32) {
			// fmr f12,f11
			ctx.fpscr.disableFlushMode();
			ctx.f12.f64 = ctx.f11.f64;
			// b 0x82461da4
		} else {
		loc_82461D9C:
			// lis r8,-32256
			ctx.r8.s64 = -2113929216;
			// lfs f12,15784(r8)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 15784);  /* glob:lbl_82003DA8 @ 0x82003da8 */
			ctx.f12.f64 = double(temp.f32);
		}
	loc_82461DA4:
		// subfic r8,r28,256
		ctx.xer.ca = var_r28 <= 256;
		ctx.r8.s64 = 256 - (int64_t)(int32_t)var_r28;
		// stfs f12,0(r22)
		ctx.fpscr.disableFlushMode();
		temp.f32 = float(ctx.f12.f64);
		PPC_STORE_U32(var_r22 + 0, temp.u32);
		// fsubs f13,f11,f12
		ctx.f13.f64 = double(float(ctx.f11.f64 - ctx.f12.f64));
		// mr r26,r28
		var_r26 = (uint32_t)(var_r28);
		// cmpwi cr6,r8,4
		// stw r9,12(r3)
		PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
		// stw r24,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r24);
		// blt cr6,0x82461f28
		if (ctx.r8.s32 >= 4) {
			// subfic r7,r28,252
			ctx.xer.ca = var_r28 <= 252;
			ctx.r7.s64 = 252 - (int64_t)(int32_t)var_r28;
			// rotlwi r27,r9,0
			var_r27 = (uint32_t)(ctx.r9.u32);
			// rlwinm r9,r7,30,2,31
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 30) & 0x3FFFFFFF;
			// addi r31,r28,2
			var_r31 = (uint32_t)(var_r28 + 2);
			// addi r29,r9,1
			var_r29 = (uint32_t)(ctx.r9.s64 + 1);  // addr:0x82000001
			// addi r9,r28,3
			ctx.r9.s64 = (int64_t)(int32_t)var_r28 + 3;
			// addi r6,r10,2
			ctx.r6.s64 = ctx.r10.s64 + 2;
			// rlwinm r7,r9,2,0,29
			ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
			// rlwinm r9,r31,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 2) & 0xFFFFFFFC;
			// rlwinm r26,r29,2,0,29
			var_r26 = (uint32_t)(__builtin_rotateleft64(var_r29 | (var_r29 << 32), 2) & 0xFFFFFFFC);
			// subf r30,r24,r10
			var_r30 = (uint32_t)(ctx.r10.s64 - (int64_t)(int32_t)var_r24);
			// rlwinm r8,r6,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
			// subf r31,r27,r10
			var_r31 = (uint32_t)(ctx.r10.s64 - (int64_t)(int32_t)var_r27);
			// add r10,r26,r10
			ctx.r10.u64 = var_r26 + ctx.r10.u64;
			// add r7,r7,r4
			ctx.r7.u64 = ctx.r7.u64 + ctx.r4.u64;
			// add r8,r8,r11
			ctx.r8.u64 = ctx.r8.u64 + ctx.r11.u64;
			// add r9,r9,r5
			ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
			// subf r25,r5,r4
			var_r25 = (uint32_t)(ctx.r4.s64 - ctx.r5.s64);
			// addi r6,r30,2
			ctx.r6.s64 = (int64_t)(int32_t)var_r30 + 2;
			// addi r31,r31,2
			var_r31 = (uint32_t)(var_r31 + 2);
			// subf r24,r27,r24
			var_r24 = var_r24 - var_r27;
			// add r26,r26,r28
			var_r26 = (uint32_t)(var_r26 + var_r28);
		loc_82461E1C:
			// mr r28,r30
			var_r28 = (uint32_t)(var_r30);
			// fmr f2,f12
			ctx.fpscr.disableFlushMode();
			ctx.f2.f64 = ctx.f12.f64;
			// addi r23,r6,-1
			var_r23 = (uint32_t)(ctx.r6.s64 + -1);
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// rlwinm r16,r28,2,16,29
			var_r16 = (uint32_t)(__builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFC);
			// lfs f1,-12(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -12);
			ctx.f1.f64 = double(temp.f32);
			// add r27,r30,r24
			var_r27 = (uint32_t)(var_r30 + var_r24);
			// lfs f11,-8(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -8);
			ctx.f11.f64 = double(temp.f32);
			// mr r28,r23
			var_r28 = (uint32_t)(var_r23);
			// lfsx f10,r9,r25
			temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + var_r25);
			ctx.f10.f64 = double(temp.f32);
			// rlwinm r23,r27,2,16,29
			var_r23 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFC);
			// lfs f9,0(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 0);
			ctx.f9.f64 = double(temp.f32);
			// addi r19,r31,-1
			var_r19 = (uint32_t)(var_r31 + -1);
			// lfsx f7,r16,r11
			temp.u32 = PPC_LOAD_U32(var_r16 + ctx.r11.u32);
			ctx.f7.f64 = double(temp.f32);
			// mr r27,r31
			var_r27 = (uint32_t)(var_r31);
			// fmuls f6,f7,f13
			ctx.f6.f64 = double(float(ctx.f7.f64 * ctx.f13.f64));
			// rlwinm r19,r19,2,16,29
			var_r19 = (uint32_t)(__builtin_rotateleft64(var_r19 | (var_r19 << 32), 2) & 0xFFFC);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// addi r18,r6,1
			var_r18 = (uint32_t)(ctx.r6.s64 + 1);  // addr:0x82000001
			// lfsx f5,r23,r11
			temp.u32 = PPC_LOAD_U32(var_r23 + ctx.r11.u32);
			ctx.f5.f64 = double(temp.f32);
			// rlwinm r23,r28,2,16,29
			var_r23 = (uint32_t)(__builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFC);
			// stfs f1,-8(r8)
			temp.f32 = float(ctx.f1.f64);
			PPC_STORE_U32(ctx.r8.u32 + -8, temp.u32);
			// fmr f8,f12
			ctx.f8.f64 = ctx.f12.f64;
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// mr r28,r6
			var_r28 = ctx.r6.u32;
			// lfsx f7,r19,r11
			temp.u32 = PPC_LOAD_U32(var_r19 + ctx.r11.u32);
			ctx.f7.f64 = double(temp.f32);
			// rlwinm r19,r27,2,16,29
			var_r19 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFC);
			// addi r17,r31,1
			var_r17 = (uint32_t)(var_r31 + 1);
			// addi r29,r29,-1
			var_r29 = (uint32_t)(var_r29 + -1);
			// rlwinm r27,r17,2,16,29
			var_r27 = (uint32_t)(__builtin_rotateleft64(var_r17 | (var_r17 << 32), 2) & 0xFFFC);
			// addi r30,r30,4
			var_r30 = (uint32_t)(var_r30 + 4);
			// fmadds f4,f5,f2,f6
			ctx.f4.f64 = double(float(ctx.f5.f64 * ctx.f2.f64 + ctx.f6.f64));
			// lfsx f2,r23,r11
			temp.u32 = PPC_LOAD_U32(var_r23 + ctx.r11.u32);
			ctx.f2.f64 = double(temp.f32);
			// fmuls f1,f2,f13
			ctx.f1.f64 = double(float(ctx.f2.f64 * ctx.f13.f64));
			// rlwinm r23,r28,2,16,29
			var_r23 = (uint32_t)(__builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFC);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// stfs f11,-4(r8)
			temp.f32 = float(ctx.f11.f64);
			PPC_STORE_U32(ctx.r8.u32 + -4, temp.u32);
			// stfs f4,-4(r9)
			temp.f32 = float(ctx.f4.f64);
			PPC_STORE_U32(ctx.r9.u32 + -4, temp.u32);
			// rlwinm r28,r18,2,16,29
			var_r28 = (uint32_t)(__builtin_rotateleft64(var_r18 | (var_r18 << 32), 2) & 0xFFFC);
			// fmr f3,f12
			ctx.f3.f64 = ctx.f12.f64;
			// addi r7,r7,16
			ctx.r7.s64 = ctx.r7.s64 + 16;
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// cmplwi cr6,r29,0
			ctx.cr6.compare<uint32_t>(var_r29, 0, ctx.xer);
			// lfsx f4,r23,r11
			temp.u32 = PPC_LOAD_U32(var_r23 + ctx.r11.u32);
			ctx.f4.f64 = double(temp.f32);
			// addi r6,r6,4
			ctx.r6.s64 = ctx.r6.s64 + 4;
			// addi r31,r31,4
			var_r31 = (uint32_t)(var_r31 + 4);
			// fmadds f6,f7,f8,f1
			ctx.f6.f64 = double(float(ctx.f7.f64 * ctx.f8.f64 + ctx.f1.f64));
			// lfsx f1,r19,r11
			temp.u32 = PPC_LOAD_U32(var_r19 + ctx.r11.u32);
			ctx.f1.f64 = double(temp.f32);
			// fmuls f2,f4,f13
			ctx.f2.f64 = double(float(ctx.f4.f64 * ctx.f13.f64));
			// stfs f10,0(r8)
			temp.f32 = float(ctx.f10.f64);
			PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// lfsx f10,r28,r11
			temp.u32 = PPC_LOAD_U32(var_r28 + ctx.r11.u32);
			ctx.f10.f64 = double(temp.f32);
			// lfsx f7,r27,r11
			temp.u32 = PPC_LOAD_U32(var_r27 + ctx.r11.u32);
			ctx.f7.f64 = double(temp.f32);
			// fmr f5,f12
			ctx.f5.f64 = ctx.f12.f64;
			// stfs f6,0(r9)
			temp.f32 = float(ctx.f6.f64);
			PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
			// stfs f9,4(r8)
			temp.f32 = float(ctx.f9.f64);
			PPC_STORE_U32(ctx.r8.u32 + 4, temp.u32);
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// addi r8,r8,16
			ctx.r8.s64 = ctx.r8.s64 + 16;
			// fmadds f11,f1,f3,f2
			ctx.f11.f64 = double(float(ctx.f1.f64 * ctx.f3.f64 + ctx.f2.f64));
			// stfs f11,4(r9)
			temp.f32 = float(ctx.f11.f64);
			PPC_STORE_U32(ctx.r9.u32 + 4, temp.u32);
			// fmuls f8,f10,f13
			ctx.f8.f64 = double(float(ctx.f10.f64 * ctx.f13.f64));
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// fmadds f6,f7,f5,f8
			ctx.f6.f64 = double(float(ctx.f7.f64 * ctx.f5.f64 + ctx.f8.f64));
			// stfs f6,8(r9)
			temp.f32 = float(ctx.f6.f64);
			PPC_STORE_U32(ctx.r9.u32 + 8, temp.u32);
			// addi r9,r9,16
			ctx.r9.s64 = ctx.r9.s64 + 16;
			// bne cr6,0x82461e1c
			if (!ctx.cr6.eq) goto loc_82461E1C;
			// stfs f12,0(r22)
			temp.f32 = float(ctx.f12.f64);
			PPC_STORE_U32(var_r22 + 0, temp.u32);
		}
	loc_82461F28:
		// cmplwi cr6,r26,256
		// bge cr6,0x82461fbc
		if (var_r26 < 256) {
			// rlwinm r7,r26,2,0,29
			ctx.r7.u64 = __builtin_rotateleft64(var_r26 | (var_r26 << 32), 2) & 0xFFFFFFFC;
			// lwz r31,8(r3)
			var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 8));
			// rlwinm r9,r10,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// lwz r30,12(r3)
			var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 12));
			// add r3,r7,r5
			ctx.r3.u64 = ctx.r7.u64 + ctx.r5.u64;
			// lfs f12,0(r22)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r22 + 0);
			ctx.f12.f64 = double(temp.f32);
			// subfic r8,r26,256
			ctx.xer.ca = var_r26 <= 256;
			ctx.r8.s64 = 256 - (int64_t)(int32_t)var_r26;
			// add r6,r9,r11
			ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
			// subf r9,r31,r10
			ctx.r9.s64 = ctx.r10.s64 - (int64_t)(int32_t)var_r31;
			// add r7,r7,r4
			ctx.r7.u64 = ctx.r7.u64 + ctx.r4.u64;
			// addi r4,r3,4
			ctx.r4.s64 = ctx.r3.s64 + 4;
			// subf r30,r30,r31
			var_r30 = var_r31 - var_r30;
			// add r10,r8,r10
			ctx.r10.u64 = ctx.r8.u64 + ctx.r10.u64;
		loc_82461F64:
			// mr r3,r9
			ctx.r3.u64 = ctx.r9.u64;
			// fmr f5,f12
			ctx.fpscr.disableFlushMode();
			ctx.f5.f64 = ctx.f12.f64;
			// add r31,r30,r9
			var_r31 = (uint32_t)(var_r30 + ctx.r9.u64);
			// lfs f4,0(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 0);
			ctx.f4.f64 = double(temp.f32);
			// rlwinm r3,r3,2,16,29
			ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFC;
			// fsubs f12,f12,f0
			ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// rlwinm r31,r31,2,16,29
			var_r31 = (uint32_t)(__builtin_rotateleft64(var_r31 | (var_r31 << 32), 2) & 0xFFFC);
			// addi r8,r8,-1
			ctx.r8.s64 = ctx.r8.s64 + -1;
			// addi r9,r9,1
			ctx.r9.s64 = ctx.r9.s64 + 1;
			// addi r7,r7,4
			ctx.r7.s64 = ctx.r7.s64 + 4;
			// lfsx f3,r3,r11
			temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + ctx.r11.u32);
			ctx.f3.f64 = double(temp.f32);
			// cmplwi cr6,r8,0
			ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
			// fmuls f2,f3,f13
			ctx.f2.f64 = double(float(ctx.f3.f64 * ctx.f13.f64));
			// lfsx f1,r31,r11
			temp.u32 = PPC_LOAD_U32(var_r31 + ctx.r11.u32);
			ctx.f1.f64 = double(temp.f32);
			// stfs f4,0(r6)
			temp.f32 = float(ctx.f4.f64);
			PPC_STORE_U32(ctx.r6.u32 + 0, temp.u32);
			// fadds f13,f13,f0
			ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// addi r6,r6,4
			ctx.r6.s64 = ctx.r6.s64 + 4;
			// fmadds f11,f1,f5,f2
			ctx.f11.f64 = double(float(ctx.f1.f64 * ctx.f5.f64 + ctx.f2.f64));
			// stfs f11,0(r4)
			temp.f32 = float(ctx.f11.f64);
			PPC_STORE_U32(ctx.r4.u32 + 0, temp.u32);
			// addi r4,r4,4
			ctx.r4.s64 = ctx.r4.s64 + 4;
			// bne cr6,0x82461f64
			if (!ctx.cr6.eq) goto loc_82461F64;
			// stfs f12,0(r22)
			temp.f32 = float(ctx.f12.f64);
			PPC_STORE_U32(var_r22 + 0, temp.u32);
		}
	loc_82461FBC:
		// clrlwi r11,r10,18
		ctx.r11.u64 = ctx.r10.u32 & 0x3FFF;
		// lfs f8,1024(r5)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 1024);
		ctx.f8.f64 = double(temp.f32);
		// stfs f8,0(r20)
		temp.f32 = float(ctx.f8.f64);
		PPC_STORE_U32(var_r20 + 0, temp.u32);
		// stw r11,0(r21)
		PPC_STORE_U32(var_r21 + 0, ctx.r11.u32);
		// clrlwi r11,r10,18
		ctx.r11.u64 = ctx.r10.u32 & 0x3FFF;
		// stw r11,0(r21)
		PPC_STORE_U32(var_r21 + 0, ctx.r11.u32);
		// b 0x8242f8b4
		__restgprlr_15(ctx, base);
		return;
	}
loc_82461FD8:
	// addi r31,r10,256
	var_r31 = (uint32_t)(ctx.r10.s64 + 256);  // addr:0x82000100
	// cmplwi cr6,r31,16384
	// bge cr6,0x82462140
	if (var_r31 < 16384) {
		// subf r7,r8,r10
		ctx.r7.s64 = ctx.r10.s64 - ctx.r8.s64;
		// addi r11,r7,256
		ctx.r11.s64 = ctx.r7.s64 + 256;
		// cmplwi cr6,r11,16384
		// bge cr6,0x82462140
		if (ctx.r11.u32 >= 16384) goto loc_82462140;
		// subf r11,r9,r10
		ctx.r11.s64 = ctx.r10.s64 - ctx.r9.s64;
		// addi r30,r11,256
		var_r30 = (uint32_t)(ctx.r11.s64 + 256);  // addr:0x82000100
		// cmplwi cr6,r30,16384
		// bge cr6,0x82462140
		if (var_r30 >= 16384) goto loc_82462140;
		// cmpwi cr6,r7,0
		// blt cr6,0x82462140
		if (ctx.r7.s32 < 0) goto loc_82462140;
		// cmpwi cr6,r11,0
		// blt cr6,0x82462140
		if (ctx.r11.s32 < 0) goto loc_82462140;
		// addi r10,r10,8
		ctx.r10.s64 = ctx.r10.s64 + 8;
		// cmplwi cr6,r6,0
		// rlwinm r11,r10,2,0,29
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r11,r11,r3
		ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
		// bne cr6,0x824620bc
		if (ctx.r6.u32 == 0) {
			// rlwinm r8,r8,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r9,r5,4
			ctx.r9.s64 = ctx.r5.s64 + 4;
			// subf r10,r8,r11
			ctx.r10.s64 = ctx.r11.s64 - ctx.r8.s64;
			// li r8,65
			ctx.r8.s64 = 65;
		loc_82462038:
			// li r7,16
			ctx.r7.s64 = 16;
			// lvlx v0,0,r4
			temp.u32 = ctx.r4.u32;
			simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
			// li r6,16
			ctx.r6.s64 = 16;
			// lvlx v12,0,r10
			temp.u32 = ctx.r10.u32;
			simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
			// mr r3,r11
			ctx.r3.u64 = ctx.r11.u64;
			// li r30,16
			var_r30 = 16;
			// mr r29,r9
			var_r29 = ctx.r9.u32;
			// lvrx v13,r4,r7
			temp.u32 = ctx.r4.u32 + ctx.r7.u32;
			simde_mm_store_si128((simde__m128i*)ctx.v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
			// mr r7,r11
			ctx.r7.u64 = ctx.r11.u64;
			// lvrx v11,r10,r6
			temp.u32 = ctx.r10.u32 + ctx.r6.u32;
			simde_mm_store_si128((simde__m128i*)ctx.v11.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
			// vor v0,v0,v13
			simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v13.u8)));
			// mr r6,r9
			ctx.r6.u64 = ctx.r9.u64;
			// vor v13,v12,v11
			simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v12.u8), simde_mm_load_si128((simde__m128i*)ctx.v11.u8)));
			// li r28,16
			var_r28 = 16;
			// addi r8,r8,-1
			ctx.r8.s64 = ctx.r8.s64 + -1;
			// stvlx v0,0,r3
			ea = ctx.r3.u32;
			for (size_t i = 0; i < (16 - (ea & 0xF)); i++)
				PPC_STORE_U8(ea + i, ctx.v0.u8[15 - i]);
			// addi r11,r11,16
			ctx.r11.s64 = ctx.r11.s64 + 16;
			// stvrx v0,r7,r30
			ea = ctx.r7.u32 + var_r30;
			for (size_t i = 0; i < (ea & 0xF); i++)
				PPC_STORE_U8(ea - i - 1, ctx.v0.u8[i]);
			// addi r10,r10,16
			ctx.r10.s64 = ctx.r10.s64 + 16;
			// stvlx v13,0,r6
			ea = ctx.r6.u32;
			for (size_t i = 0; i < (16 - (ea & 0xF)); i++)
				PPC_STORE_U8(ea + i, ctx.v13.u8[15 - i]);
			// addi r4,r4,16
			ctx.r4.s64 = ctx.r4.s64 + 16;
			// addi r9,r9,16
			ctx.r9.s64 = ctx.r9.s64 + 16;
			// stvrx v13,r29,r28
			ea = var_r29 + var_r28;
			for (size_t i = 0; i < (ea & 0xF); i++)
				PPC_STORE_U8(ea - i - 1, ctx.v13.u8[i]);
			// cmplwi cr6,r8,0
			// bne cr6,0x82462038
			if (ctx.r8.u32 != 0) goto loc_82462038;
			// lis r11,-32256
			// lfs f3,1024(r5)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 1024);
			ctx.f3.f64 = double(temp.f32);
			// stfs f3,0(r20)
			temp.f32 = float(ctx.f3.f64);
			PPC_STORE_U32(var_r20 + 0, temp.u32);
			// stw r31,0(r21)
			PPC_STORE_U32(var_r21 + 0, var_r31);
			// lfs f0,22692(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22692);
			ctx.f0.f64 = double(temp.f32);
			// fsubs f10,f12,f0
			ctx.f10.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
			// stfs f10,0(r22)
			temp.f32 = float(ctx.f10.f64);
			PPC_STORE_U32(var_r22 + 0, temp.u32);
			// b 0x8242f8b4
			__restgprlr_15(ctx, base);
			return;
		}
	loc_824620BC:
		// lis r10,-32256
		// lis r6,-32248
		// rlwinm r7,r9,2,0,29
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
		// rlwinm r3,r8,2,0,29
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
		// addi r8,r5,4
		ctx.r8.s64 = ctx.r5.s64 + 4;
		// lfs f0,15788(r10)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15788);
		ctx.f0.f64 = double(temp.f32);
		// subf r10,r7,r11
		ctx.r10.s64 = ctx.r11.s64 - ctx.r7.s64;
		// lfs f13,-25744(r6)
		temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -25744);
		ctx.f13.f64 = double(temp.f32);
		// fsubs f0,f0,f12
		ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
		// li r9,256
		ctx.r9.s64 = 256;
		// subf r7,r3,r7
		ctx.r7.s64 = ctx.r7.s64 - ctx.r3.s64;
	loc_824620E8:
		// lfs f9,0(r10)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
		ctx.f9.f64 = double(temp.f32);
		// fmr f8,f12
		ctx.f8.f64 = ctx.f12.f64;
		// fmuls f6,f9,f0
		ctx.f6.f64 = double(float(ctx.f9.f64 * ctx.f0.f64));
		// lfsx f7,r7,r10
		temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + ctx.r10.u32);
		ctx.f7.f64 = double(temp.f32);
		// addi r9,r9,-1
		ctx.r9.s64 = ctx.r9.s64 + -1;
		// lfs f5,0(r4)
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
		ctx.f5.f64 = double(temp.f32);
		// stfs f5,0(r11)
		temp.f32 = float(ctx.f5.f64);
		PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
		// fsubs f12,f12,f13
		ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
		// fadds f0,f0,f13
		ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// addi r4,r4,4
		ctx.r4.s64 = ctx.r4.s64 + 4;
		// cmplwi cr6,r9,0
		// fmadds f4,f7,f8,f6
		ctx.f4.f64 = double(float(ctx.f7.f64 * ctx.f8.f64 + ctx.f6.f64));
		// stfs f4,0(r8)
		temp.f32 = float(ctx.f4.f64);
		PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
		// addi r8,r8,4
		ctx.r8.s64 = ctx.r8.s64 + 4;
		// bne cr6,0x824620e8
		if (ctx.r9.u32 != 0) goto loc_824620E8;
		// lfs f3,1024(r5)
		temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 1024);
		ctx.f3.f64 = double(temp.f32);
		// stw r31,0(r21)
		PPC_STORE_U32(var_r21 + 0, var_r31);
		// stfs f12,0(r22)
		temp.f32 = float(ctx.f12.f64);
		PPC_STORE_U32(var_r22 + 0, temp.u32);
		// stfs f3,0(r20)
		temp.f32 = float(ctx.f3.f64);
		PPC_STORE_U32(var_r20 + 0, temp.u32);
		// b 0x8242f8b4
		__restgprlr_15(ctx, base);
		return;
	}
loc_82462140:
	// lis r11,-32256
	// addi r6,r3,32
	ctx.r6.s64 = ctx.r3.s64 + 32;
	// lis r3,-32248
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r7,r5,4
	ctx.r7.s64 = ctx.r5.s64 + 4;
	// lfs f0,15788(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 15788);
	ctx.f0.f64 = double(temp.f32);
	// subf r11,r9,r10
	ctx.r11.s64 = ctx.r10.s64 - ctx.r9.s64;
	// fsubs f13,f0,f12
	ctx.f13.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// li r9,256
	ctx.r9.s64 = 256;
	// fmr f0,f12
	ctx.f0.f64 = ctx.f12.f64;
	// lfs f12,-25744(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + -25744);
	ctx.f12.f64 = double(temp.f32);
	// addi r10,r10,256
	ctx.r10.s64 = ctx.r10.s64 + 256;
	// add r8,r8,r6
	ctx.r8.u64 = ctx.r8.u64 + ctx.r6.u64;
loc_82462174:
	// add r3,r11,r23
	ctx.r3.u64 = ctx.r11.u64 + var_r23;
	// lfs f2,0(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f2.f64 = double(temp.f32);
	// rlwinm r31,r11,2,16,29
	var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFC);
	// rlwinm r3,r3,2,16,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFC;
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r4,r4,4
	ctx.r4.s64 = ctx.r4.s64 + 4;
	// lfsx f1,r31,r6
	temp.u32 = PPC_LOAD_U32(var_r31 + ctx.r6.u32);
	ctx.f1.f64 = double(temp.f32);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// lfsx f10,r3,r6
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + ctx.r6.u32);
	ctx.f10.f64 = double(temp.f32);
	// fmuls f11,f1,f13
	ctx.f11.f64 = double(float(ctx.f1.f64 * ctx.f13.f64));
	// fmuls f10,f10,f0
	ctx.f10.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// stfs f2,0(r8)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fsubs f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// addi r8,r8,4
	ctx.r8.s64 = ctx.r8.s64 + 4;
	// fadds f13,f13,f12
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f12.f64));
	// fadds f9,f11,f10
	ctx.f9.f64 = double(float(ctx.f11.f64 + ctx.f10.f64));
	// stfs f9,0(r7)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// addi r7,r7,4
	ctx.r7.s64 = ctx.r7.s64 + 4;
	// bne cr6,0x82462174
	if (!ctx.cr6.eq) goto loc_82462174;
	// clrlwi r11,r10,18
	ctx.r11.u64 = ctx.r10.u32 & 0x3FFF;
	// lfs f8,1024(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 1024);
	ctx.f8.f64 = double(temp.f32);
	// stfs f0,0(r22)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r22 + 0, temp.u32);
	// stfs f8,0(r20)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(var_r20 + 0, temp.u32);
	// stw r11,0(r21)
	PPC_STORE_U32(var_r21 + 0, ctx.r11.u32);
	// b 0x8242f8b4
	__restgprlr_15(ctx, base);
	return;
}

__attribute__((alias("__imp__phInst_21E0_p39"))) PPC_WEAK_FUNC(phInst_21E0_p39);
PPC_FUNC_IMPL(__imp__phInst_21E0_p39) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32256
	// lfs f9,20(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// lfs f12,16(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f12.f64 = double(temp.f32);
	// li r9,1
	ctx.r9.s64 = 1;
	// stfs f9,0(r5)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// cmpwi cr6,r6,4
	// addi r11,r5,4
	ctx.r11.s64 = ctx.r5.s64 + 4;
	// lfs f7,15788(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15788);
	ctx.f7.f64 = double(temp.f32);
	// blt cr6,0x824622b0
	if (ctx.r6.s32 >= 4) {
		// lfs f0,28(r3)
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		ctx.f0.f64 = double(temp.f32);
		// rlwinm r10,r6,30,2,31
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 30) & 0x3FFFFFFF;
		// fsubs f10,f7,f0
		ctx.f10.f64 = double(float(ctx.f7.f64 - ctx.f0.f64));
		// lfs f11,24(r3)
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 24);
		ctx.f11.f64 = double(temp.f32);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// addi r9,r9,1
		ctx.r9.s64 = ctx.r9.s64 + 1;
		// fmuls f13,f11,f10
		ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f10.f64));
	loc_82462220:
		// lfs f6,0(r4)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
		ctx.f6.f64 = double(temp.f32);
		// fmr f8,f12
		ctx.f8.f64 = ctx.f12.f64;
		// fmuls f3,f6,f13
		ctx.f3.f64 = double(float(ctx.f6.f64 * ctx.f13.f64));
		// lfs f5,4(r4)
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 4);
		ctx.f5.f64 = double(temp.f32);
		// fmuls f2,f0,f9
		ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f9.f64));
		// lfs f4,8(r4)
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 8);
		ctx.f4.f64 = double(temp.f32);
		// fmuls f6,f5,f11
		ctx.f6.f64 = double(float(ctx.f5.f64 * ctx.f11.f64));
		// lfs f1,12(r4)
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 12);
		ctx.f1.f64 = double(temp.f32);
		// fmuls f5,f4,f11
		ctx.f5.f64 = double(float(ctx.f4.f64 * ctx.f11.f64));
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// fmuls f4,f1,f11
		ctx.f4.f64 = double(float(ctx.f1.f64 * ctx.f11.f64));
		// addi r4,r4,16
		ctx.r4.s64 = ctx.r4.s64 + 16;
		// cmplwi cr6,r10,0
		ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
		// fmadds f12,f0,f12,f3
		ctx.f12.f64 = double(float(ctx.f0.f64 * ctx.f12.f64 + ctx.f3.f64));
		// fmadds f9,f13,f8,f2
		ctx.f9.f64 = double(float(ctx.f13.f64 * ctx.f8.f64 + ctx.f2.f64));
		// stfs f9,0(r11)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
		// fmuls f3,f0,f12
		ctx.f3.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
		// fmr f8,f12
		ctx.f8.f64 = ctx.f12.f64;
		// fmuls f2,f0,f9
		ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f9.f64));
		// fmadds f12,f6,f10,f3
		ctx.f12.f64 = double(float(ctx.f6.f64 * ctx.f10.f64 + ctx.f3.f64));
		// fmadds f9,f13,f8,f2
		ctx.f9.f64 = double(float(ctx.f13.f64 * ctx.f8.f64 + ctx.f2.f64));
		// stfs f9,4(r11)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
		// fmuls f1,f0,f12
		ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
		// fmr f8,f12
		ctx.f8.f64 = ctx.f12.f64;
		// fmuls f6,f0,f9
		ctx.f6.f64 = double(float(ctx.f0.f64 * ctx.f9.f64));
		// fmadds f12,f5,f10,f1
		ctx.f12.f64 = double(float(ctx.f5.f64 * ctx.f10.f64 + ctx.f1.f64));
		// fmadds f9,f13,f8,f6
		ctx.f9.f64 = double(float(ctx.f13.f64 * ctx.f8.f64 + ctx.f6.f64));
		// stfs f9,8(r11)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
		// fmuls f5,f0,f12
		ctx.f5.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
		// fmr f8,f12
		ctx.f8.f64 = ctx.f12.f64;
		// fmadds f12,f4,f10,f5
		ctx.f12.f64 = double(float(ctx.f4.f64 * ctx.f10.f64 + ctx.f5.f64));
		// fmuls f4,f0,f9
		ctx.f4.f64 = double(float(ctx.f0.f64 * ctx.f9.f64));
		// fmadds f9,f13,f8,f4
		ctx.f9.f64 = double(float(ctx.f13.f64 * ctx.f8.f64 + ctx.f4.f64));
		// stfs f9,12(r11)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
		// addi r11,r11,16
		ctx.r11.s64 = ctx.r11.s64 + 16;
		// bne cr6,0x82462220
		if (!ctx.cr6.eq) goto loc_82462220;
	}
loc_824622B0:
	// cmplw cr6,r9,r6
	// bgt cr6,0x82462300
	if (ctx.r9.u32 <= ctx.r6.u32) {
		// lfs f0,28(r3)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		ctx.f0.f64 = double(temp.f32);
		// subf r10,r9,r6
		ctx.r10.s64 = ctx.r6.s64 - ctx.r9.s64;
		// fsubs f2,f7,f0
		ctx.f2.f64 = double(float(ctx.f7.f64 - ctx.f0.f64));
		// lfs f3,24(r3)
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 24);
		ctx.f3.f64 = double(temp.f32);
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// fmuls f13,f2,f3
		ctx.f13.f64 = double(float(ctx.f2.f64 * ctx.f3.f64));
	loc_824622D0:
		// fmuls f1,f0,f9
		ctx.fpscr.disableFlushMode();
		ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f9.f64));
		// lfs f10,0(r4)
		temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
		ctx.f10.f64 = double(temp.f32);
		// fmuls f9,f0,f12
		ctx.f9.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// fmr f11,f12
		ctx.f11.f64 = ctx.f12.f64;
		// addi r4,r4,4
		ctx.r4.s64 = ctx.r4.s64 + 4;
		// cmplwi cr6,r10,0
		// fmadds f12,f13,f10,f9
		ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f10.f64 + ctx.f9.f64));
		// fmadds f9,f13,f11,f1
		ctx.f9.f64 = double(float(ctx.f13.f64 * ctx.f11.f64 + ctx.f1.f64));
		// stfs f9,0(r11)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// bne cr6,0x824622d0
		if (ctx.r10.u32 != 0) goto loc_824622D0;
	}
loc_82462300:
	// stfs f12,16(r3)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// stfs f9,20(r3)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__phInst_2310_p39"))) PPC_WEAK_FUNC(phInst_2310_p39);
PPC_FUNC_IMPL(__imp__phInst_2310_p39) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8242f894
	ctx.lr = 0x82462318;
	__savegprlr_27(ctx, base);
	// lwz r11,12(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r10,r3,44
	ctx.r10.s64 = ctx.r3.s64 + 44;
	// lwz r8,0(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lfs f11,8(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f11.f64 = double(temp.f32);
	// lwz r7,36(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// lfs f10,32(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 32);
	ctx.f10.f64 = double(temp.f32);
	// lwz r30,24(r3)
	var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 24));
	// subf r31,r8,r11
	var_r31 = (uint32_t)(ctx.r11.s64 - ctx.r8.s64);
	// rlwinm r8,r7,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,16(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// subf r30,r30,r11
	var_r30 = (uint32_t)(ctx.r11.s64 - (int64_t)(int32_t)var_r30);
	// lfs f13,4(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// subf r11,r11,r7
	ctx.r11.s64 = ctx.r7.s64 - ctx.r11.s64;
	// lfs f12,28(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	ctx.f12.f64 = double(temp.f32);
	// stfs f11,0(r5)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// li r9,256
	ctx.r9.s64 = 256;
	// stfs f10,0(r6)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r6.u32 + 0, temp.u32);
	// add r8,r8,r10
	ctx.r8.u64 = ctx.r8.u64 + ctx.r10.u64;
	// addi r7,r7,256
	ctx.r7.s64 = ctx.r7.s64 + 256;
loc_82462364:
	// rlwinm r29,r11,2,21,29
	var_r29 = (uint32_t)(__builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0x7FC);
	// lfs f9,0(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f9.f64 = double(temp.f32);
	// add r28,r31,r11
	var_r28 = (uint32_t)(var_r31 + ctx.r11.u64);
	// add r27,r30,r11
	var_r27 = (uint32_t)(var_r30 + ctx.r11.u64);
	// rlwinm r28,r28,2,21,29
	var_r28 = (uint32_t)(__builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0x7FC);
	// rlwinm r27,r27,2,21,29
	var_r27 = (uint32_t)(__builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0x7FC);
	// lfsx f8,r29,r10
	temp.u32 = PPC_LOAD_U32(var_r29 + ctx.r10.u32);
	ctx.f8.f64 = double(temp.f32);
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// fmuls f7,f8,f0
	ctx.f7.f64 = double(float(ctx.f8.f64 * ctx.f0.f64));
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// lfsx f6,r28,r10
	temp.u32 = PPC_LOAD_U32(var_r28 + ctx.r10.u32);
	ctx.f6.f64 = double(temp.f32);
	// addi r4,r4,4
	ctx.r4.s64 = ctx.r4.s64 + 4;
	// lfsx f5,r27,r10
	temp.u32 = PPC_LOAD_U32(var_r27 + ctx.r10.u32);
	ctx.f5.f64 = double(temp.f32);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stfs f9,0(r8)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fmuls f4,f5,f12
	ctx.f4.f64 = double(float(ctx.f5.f64 * ctx.f12.f64));
	// stfs f4,0(r6)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r6.u32 + 0, temp.u32);
	// addi r8,r8,4
	ctx.r8.s64 = ctx.r8.s64 + 4;
	// cmplwi cr6,r9,0
	// fmadds f3,f6,f13,f7
	ctx.f3.f64 = double(float(ctx.f6.f64 * ctx.f13.f64 + ctx.f7.f64));
	// stfs f3,0(r5)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// bne cr6,0x82462364
	if (ctx.r9.u32 != 0) goto loc_82462364;
	// clrlwi r7,r7,23
	ctx.r7.u64 = ctx.r7.u32 & 0x1FF;
	// fmr f2,f3
	ctx.f2.f64 = ctx.f3.f64;
	// fmr f1,f4
	ctx.f1.f64 = ctx.f4.f64;
	// stfs f2,8(r3)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// stfs f1,32(r3)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r3.u32 + 32, temp.u32);
	// stw r7,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r7.u32);
	// b 0x8242f8e4
	__restgprlr_27(ctx, base);
	return;
}

