#include "tt-decomp_init.h"

__attribute__((alias("__imp__EvtCreateSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtCreateSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtCreateSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6748(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6748);  /* glob:lbl_825D1A5C @ 0x825d1a5c */
	// blr
	return;
}

__attribute__((alias("__imp__EvtCreateSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtCreateSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtCreateSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8524
	ctx.r3.s64 = ctx.r11.s64 + 8524;
	// blr
	return;
}

__attribute__((alias("__imp__util_DA08"))) PPC_WEAK_FUNC(util_DA08);
PPC_FUNC_IMPL(__imp__util_DA08) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// addi r30,r31,4
	var_r30 = (uint32_t)(var_r31 + 4);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r29,r30,4
	var_r29 = (uint32_t)(var_r30 + 4);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,10360
	ctx.r11.s64 = ctx.r11.s64 + 10360;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r29)
	PPC_STORE_U32(var_r29 + 0, ctx.r10.u32);
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	return;
}

__attribute__((alias("__imp__snCreateMachine_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8544
	ctx.r3.s64 = ctx.r11.s64 + 8544;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r4,r11,24
	ctx.r4.s64 = ctx.r11.s64 + 24;
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// lbz r8,300(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 300);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// clrlwi r7,r8,25
	ctx.r7.u64 = ctx.r8.u32 & 0x7F;
	// stw r10,276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 276, ctx.r10.u32);
	// addi r3,r9,232
	ctx.r3.s64 = ctx.r9.s64 + 232;
	// std r10,280(r11)
	PPC_STORE_U64(ctx.r11.u32 + 280, ctx.r10.u64);
	// stw r10,296(r11)
	PPC_STORE_U32(ctx.r11.u32 + 296, ctx.r10.u32);
	// std r10,288(r11)
	PPC_STORE_U64(ctx.r11.u32 + 288, ctx.r10.u64);
	// stb r7,300(r11)
	PPC_STORE_U8(ctx.r11.u32 + 300, ctx.r7.u8);
	// b 0x823dd170
	util_D170(ctx, base);
	return;
}

__attribute__((alias("__imp__snCreateMachine_vfn_13"))) PPC_WEAK_FUNC(snCreateMachine_vfn_13);
PPC_FUNC_IMPL(__imp__snCreateMachine_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6268(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6268);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823ddb34
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,44(r7)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snCreateMachine::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,84
		var_r29 = (uint32_t)(var_r31 + 84);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r4,40(r5)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6280(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6280);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823ddba8
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,44(r9)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snCreateMachine::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,108
		var_r29 = (uint32_t)(var_r31 + 108);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r6,40(r7)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r4,4(r5)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6292(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6292);
	// cmpw cr6,r3,r11
	// bne cr6,0x823ddc1c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r10,44(r11)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// addi r29,r31,132
		var_r29 = (uint32_t)(var_r31 + 132);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823DDC1C:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,10792
	ctx.r11.s64 = ctx.r11.s64 + 10792;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e93d0
	util_93D0(ctx, base);
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingHost_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingHost_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingHost_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8560
	ctx.r3.s64 = ctx.r11.s64 + 8560;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingHost_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingHost_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingHost_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmCreatingHost::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r29,r11,20984
	var_r29 = (uint32_t)(ctx.r11.s64 + 20984);  // lbl_825E51F8 @ 0x825e51f8
	// lis r11,-32162
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// addi r28,r11,21576
	var_r28 = (uint32_t)(ctx.r11.s64 + 21576);  // lbl_825E5448 @ 0x825e5448
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r9,12(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r8,20(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
	// stw r9,276(r8)
	PPC_STORE_U32(ctx.r8.u32 + 276, ctx.r9.u32);
	// bl 0x82416650
	util_6650(ctx, base);
	// lwz r7,20(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 20);
	// std r3,280(r7)
	PPC_STORE_U64(ctx.r7.u32 + 280, ctx.r3.u64);
	// lwz r30,20(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,16(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 16);
	// addi r8,r11,28
	ctx.r8.s64 = ctx.r11.s64 + 28;
	// addi r3,r10,232
	ctx.r3.s64 = ctx.r10.s64 + 232;
	// lwz r9,284(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 284);
	// lwz r7,24(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r6,20(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,12(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x8236d758
	xam_D758_g(ctx, base);
	// clrlwi r4,r3,24
	ctx.r4.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// bne cr6,0x823ddd3c
	if (ctx.r4.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DDD3C:
	// lis r10,-32193
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r3,r10,-20288
	ctx.r3.s64 = ctx.r10.s64 + -20288;
	// stw r31,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, var_r31);
	// stw r3,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r3.u32);
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingHost_vfn_15"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingHost_vfn_15);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingHost_vfn_15) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,20(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_DD70_w"))) PPC_WEAK_FUNC(snSession_DD70_w);
PPC_FUNC_IMPL(__imp__snSession_DD70_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,2
	// bne cr6,0x823dddc4
	if (ctx.r5.s32 == 2) {
		// lwz r31,20(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 20));
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// lwz r29,276(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 276));
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// stw r29,108(r1)
		PPC_STORE_U32(ctx.r1.u32 + 108, var_r29);
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10912
		ctx.r11.s64 = ctx.r11.s64 + 10912;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// ld r11,280(r31)
		ctx.r11.u64 = PPC_LOAD_U64(var_r31 + 280);
		// std r11,112(r1)
		PPC_STORE_U64(ctx.r1.u32 + 112, ctx.r11.u64);
		// bl 0x823e9478
		snSession_9478_fw(ctx, base);
		return;
	}
loc_823DDDC4:
	// cmpwi cr6,r5,3
	// bne cr6,0x823dddec
	if (ctx.r5.s32 == 3) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DDDEC:
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingGuest_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingGuest_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingGuest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8576
	ctx.r3.s64 = ctx.r11.s64 + 8576;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingGuest_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingGuest_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingGuest_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmCreatingGuest::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r29,r11,20948
	var_r29 = (uint32_t)(ctx.r11.s64 + 20948);  // lbl_825E51D4 @ 0x825e51d4
	// lis r11,-32162
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// addi r28,r11,21576
	var_r28 = (uint32_t)(ctx.r11.s64 + 21576);  // lbl_825E5448 @ 0x825e5448
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r7,12(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r8,20(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r7,276(r8)
	PPC_STORE_U32(ctx.r8.u32 + 276, ctx.r7.u32);
	// lwz r6,20(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 20);
	// std r9,280(r6)
	PPC_STORE_U64(ctx.r6.u32 + 280, ctx.r9.u64);
	// lwz r30,20(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r4,40(r5)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,16(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 16);
	// addi r6,r11,20
	ctx.r6.s64 = ctx.r11.s64 + 20;
	// addi r3,r10,232
	ctx.r3.s64 = ctx.r10.s64 + 232;
	// lbz r10,120(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 120);
	// lwz r7,116(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 116);
	// rlwinm r8,r10,25,7,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 25) & 0x1FFFFFF;
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,12(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x8236d900
	xam_D900_g(ctx, base);
	// clrlwi r9,r3,24
	ctx.r9.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// bne cr6,0x823ddeec
	if (ctx.r9.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DDEEC:
	// lis r10,-32193
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r8,r10,-20280
	ctx.r8.s64 = ctx.r10.s64 + -20280;
	// stw r31,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, var_r31);
	// stw r8,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r8.u32);
	return;
}

__attribute__((alias("__imp__snSession_DF08_w"))) PPC_WEAK_FUNC(snSession_DF08_w);
PPC_FUNC_IMPL(__imp__snSession_DF08_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,4
	// bne cr6,0x823ddfac
	if (ctx.r5.s32 == 4) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r30)
  // [ph4a] vtable load collapsed
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r31,r11,10812
		var_r31 = (uint32_t)(ctx.r11.s64 + 10812);  // lbl_82072A3C @ 0x82072a3c
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// bctrl
		VCALL(var_r30, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r30,56(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823ddfd4
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r31,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r31);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823ddfd4
	} else {
	loc_823DDFAC:
		// cmpwi cr6,r5,5
		// bne cr6,0x823ddfd4
		if (ctx.r5.s32 != 5) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DDFD4:
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingGuest_vfn_12"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingGuest_vfn_12);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingGuest_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmCreatingGuest::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6772);
	// cmpw cr6,r3,r11
	// bne cr6,0x823de084
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,156
		var_r29 = (uint32_t)(ctx.r11.s64 + 156);  // addr:0x825d009c
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823DE084:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingOffline_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingOffline_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingOffline_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8592
	ctx.r3.s64 = ctx.r11.s64 + 8592;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingOffline_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingOffline_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingOffline_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmCreatingOffline::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r29,r11,20912
	var_r29 = (uint32_t)(ctx.r11.s64 + 20912);  // lbl_825E51B0 @ 0x825e51b0
	// lis r11,-32162
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// addi r28,r11,21576
	var_r28 = (uint32_t)(ctx.r11.s64 + 21576);  // lbl_825E5448 @ 0x825e5448
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r9,12(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r8,20(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
	// stw r9,276(r8)
	PPC_STORE_U32(ctx.r8.u32 + 276, ctx.r9.u32);
	// bl 0x82416650
	util_6650(ctx, base);
	// lwz r7,20(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 20);
	// std r3,280(r7)
	PPC_STORE_U64(ctx.r7.u32 + 280, ctx.r3.u64);
	// lwz r30,20(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,16(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 16);
	// addi r5,r11,16
	ctx.r5.s64 = ctx.r11.s64 + 16;
	// addi r3,r10,232
	ctx.r3.s64 = ctx.r10.s64 + 232;
	// lwz r6,272(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 272);
	// lwz r4,12(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x8236dd68
	xam_DD68_g(ctx, base);
	// clrlwi r4,r3,24
	ctx.r4.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// bne cr6,0x823de180
	if (ctx.r4.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DE180:
	// lis r10,-32193
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r3,r10,-20272
	ctx.r3.s64 = ctx.r10.s64 + -20272;
	// stw r31,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, var_r31);
	// stw r3,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r3.u32);
	return;
}

__attribute__((alias("__imp__snSession_E1A0_w"))) PPC_WEAK_FUNC(snSession_E1A0_w);
PPC_FUNC_IMPL(__imp__snSession_E1A0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,8
	// bne cr6,0x823de1f4
	if (ctx.r5.s32 == 8) {
		// lwz r31,20(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 20));
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// lwz r29,276(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 276));
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// stw r29,108(r1)
		PPC_STORE_U32(ctx.r1.u32 + 108, var_r29);
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10912
		ctx.r11.s64 = ctx.r11.s64 + 10912;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// ld r11,280(r31)
		ctx.r11.u64 = PPC_LOAD_U64(var_r31 + 280);
		// std r11,112(r1)
		PPC_STORE_U64(ctx.r1.u32 + 112, ctx.r11.u64);
		// bl 0x823e9478
		snSession_9478_fw(ctx, base);
		return;
	}
loc_823DE1F4:
	// cmpwi cr6,r5,9
	// bne cr6,0x823de21c
	if (ctx.r5.s32 == 9) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DE21C:
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmRequestingConfig_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmRequestingConfig_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmRequestingConfig_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8608
	ctx.r3.s64 = ctx.r11.s64 + 8608;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmRequestingConfig_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmRequestingConfig_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmRequestingConfig_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, manual
	// lis r11,-32193
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,-20264
	ctx.r11.s64 = ctx.r11.s64 + -20264;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// lwz r10,20(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r9,16(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// lwz r3,164(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// lwz r30,20(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
	// addi r4,r30,48
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 48;
	// lwz r8,16(r30)
	ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lwz r11,164(r8)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r8.u32 + 164);
	// addi r3,r11,636
	ctx.r3.s64 = ctx.r11.s64 + 636;
	// bl 0x823dc2b0
	SinglesNetworkClient_C2B0_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823de2a4
	if (ctx.r3.u32 != 0) {
		// lwz r11,84(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 84);
		// cmpwi cr6,r11,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bge cr6,0x823de2a8
		if (ctx.r11.s32 >= 0) goto loc_823DE2A8;
	}
loc_823DE2A4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823DE2A8:
	// clrlwi r6,r11,24
	ctx.r6.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r6,0
	// beq cr6,0x823de2c0
	if (ctx.r6.u32 != 0) {
		// lwz r5,296(r30)
		ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 296);
		// cmpwi cr6,r5,20
		// bge cr6,0x823de308
		if (ctx.r5.s32 >= 20) goto loc_823DE308;
	}
loc_823DE2C0:
	// lwz r3,16(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 16);
	// addi r4,r30,48
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 48;
	// lwz r11,164(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 164);
	// addi r3,r11,636
	ctx.r3.s64 = ctx.r11.s64 + 636;
	// bl 0x823dc2b0
	SinglesNetworkClient_C2B0_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823de2ec
	if (ctx.r3.u32 != 0) {
		// lwz r11,84(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 84);
		// cmpwi cr6,r11,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bge cr6,0x823de2f0
		if (ctx.r11.s32 >= 0) goto loc_823DE2F0;
	}
loc_823DE2EC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823DE2F0:
	// clrlwi r10,r11,24
	ctx.r10.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bne cr6,0x823de310
	if (ctx.r10.u32 == 0) {
		// lwz r9,296(r30)
		ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 296);
		// cmpwi cr6,r9,10
		// blt cr6,0x823de310
		if (ctx.r9.s32 < 10) goto loc_823DE310;
	loc_823DE308:
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823de314
	} else {
	loc_823DE310:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823DE314:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r8,0
	// bne cr6,0x823de36c
	if (ctx.r8.u32 == 0) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823f2178
		SinglesNetworkClient_2178_g(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x823de36c
		if (ctx.r7.u32 == 0) goto loc_823DE36C;
		// lwz r6,20(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,8628
		ctx.r4.s64 = ctx.r11.s64 + 8628;
		// lwz r5,296(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 296);
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// li r4,500
		ctx.r4.s64 = 500;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r10,296(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 296);
		// addi r5,r10,1
		ctx.r5.s64 = ctx.r10.s64 + 1;
		// stw r5,296(r11)
		PPC_STORE_U32(ctx.r11.u32 + 296, ctx.r5.u32);
		// bl 0x823ed4f8
		util_D4F8(ctx, base);
		// b 0x823de3ec
	} else {
	loc_823DE36C:
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,10832
		var_r30 = (uint32_t)(ctx.r11.s64 + 10832);  // lbl_82072A50 @ 0x82072a50
		// lwz r11,44(r4)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r9,4(r10)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823de3ec
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r8,84(r1)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r6,88(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r7,r8,32,63
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r5,r6,r7
		ctx.r5.u64 = ctx.r6.u64 | ctx.r7.u64;
		// std r5,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r5.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823DE3EC:
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmRequestingConfig_vfn_15"))) PPC_WEAK_FUNC(snCreateMachine_snHsmRequestingConfig_vfn_15);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmRequestingConfig_vfn_15) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,20(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// addi r31,r3,24
	var_r31 = (uint32_t)(ctx.r3.s64 + 24);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r11,164(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 164);
	// addi r3,r11,672
	ctx.r3.s64 = ctx.r11.s64 + 672;
	// bl 0x823b3d80
	NotifyHandler_3D80_g(ctx, base);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r9,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmRequestingConfig_vfn_12"))) PPC_WEAK_FUNC(snCreateMachine_snHsmRequestingConfig_vfn_12);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmRequestingConfig_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmRequestingConfig::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6808(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6808);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823de4e4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snCreateMachine_snHsmRequestingConfig::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,204
		var_r29 = (uint32_t)(ctx.r11.s64 + 204);  // addr:0x825d00cc
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmRequestingConfig::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6784(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6784);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823de55c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,44(r8)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snCreateMachine_snHsmRequestingConfig::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,252
		var_r29 = (uint32_t)(ctx.r11.s64 + 252);  // addr:0x825d00fc
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmRequestingConfig::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823de5cc
	if (ctx.r3.s32 == 0) {
		// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,156
		var_r29 = (uint32_t)(ctx.r11.s64 + 156);  // addr:0x825d009c
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823DE5CC:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmApplyingConfig_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmApplyingConfig_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmApplyingConfig_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8676
	ctx.r3.s64 = ctx.r11.s64 + 8676;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmApplyingConfig_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmApplyingConfig_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmApplyingConfig_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r31,20(r30)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 20));
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,22524
	ctx.r6.s64 = ctx.r11.s64 + 22524;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// ld r8,24(r3)
	ctx.r8.u64 = PPC_LOAD_U64(ctx.r3.u32 + 24);
	// lwz r9,20(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r4,r3,32
	ctx.r4.s64 = ctx.r3.s64 + 32;
	// std r8,280(r9)
	PPC_STORE_U64(ctx.r9.u32 + 280, ctx.r8.u64);
	// lbz r7,312(r3)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r3.u32 + 312);
	// lbz r6,300(r31)
	ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 300);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// rlwimi r6,r7,7,0,24
	ctx.r6.u64 = (__builtin_rotateleft32(ctx.r7.u32, 7) & 0xFFFFFF80) | (ctx.r6.u64 & 0xFFFFFFFF0000007F);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// stb r6,300(r31)
	PPC_STORE_U8(var_r31 + 300, ctx.r6.u8);
	// bl 0x8236dab8
	xam_DAB8_g(ctx, base);
	// clrlwi r4,r3,24
	ctx.r4.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// bne cr6,0x823de698
	if (ctx.r4.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10852
		ctx.r11.s64 = ctx.r11.s64 + 10852;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9530
		snSession_9530_fw(ctx, base);
	}
loc_823DE698:
	// lis r10,-32193
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 20);
	// addi r3,r10,-20256
	ctx.r3.s64 = ctx.r10.s64 + -20256;
	// stw r30,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, var_r30);
	// stw r3,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r3.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_E6C8"))) PPC_WEAK_FUNC(snSession_E6C8);
PPC_FUNC_IMPL(__imp__snSession_E6C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=176, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r5,6
	// bne cr6,0x823de7b8
	if (ctx.r5.s32 == 6) {
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lbz r10,300(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 300);
		// rlwinm r9,r10,0,0,24
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r9,0
		// beq cr6,0x823de77c
		if (ctx.r9.u32 != 0) {
			// addi r3,r1,80
			ctx.r3.s64 = ctx.r1.s64 + 80;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// addi r30,r11,10872
			var_r30 = (uint32_t)(ctx.r11.s64 + 10872);  // lbl_82072A78 @ 0x82072a78
			// lwz r7,44(r8)
  // [ph4a] slot load collapsed
			// stw r30,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
			// bctrl
			VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
			// lwz r31,56(r3)
			var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
			// li r5,0
			ctx.r5.s64 = 0;
			// li r4,12
			ctx.r4.s64 = 12;
			// lwz r3,4(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
			// lwz r11,4(r6)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// mr r4,r3
			ctx.r4.u64 = ctx.r3.u64;
			// cmplwi cr6,r4,0
			// beq cr6,0x823de7e0
			if (ctx.r4.u32 == 0) {
				return;
			}
			// lis r11,-32249
			// addi r3,r31,8
			ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
			// addi r11,r11,10340
			ctx.r11.s64 = ctx.r11.s64 + 10340;
			// stw r11,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
			// lwz r10,84(r1)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
			// lwz r8,88(r1)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			// rldicr r9,r10,32,63
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u64, 32);
			// stw r30,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
			// or r7,r8,r9
			ctx.r7.u64 = ctx.r8.u64 | ctx.r9.u64;
			// std r7,4(r4)
			PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r7.u64);
			// bl 0x823ec068
			snSession_AddNode_C068(ctx, base);
			return;
		}
	loc_823DE77C:
		// lwz r30,20(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 20));
		// addi r3,r1,112
		ctx.r3.s64 = ctx.r1.s64 + 112;
		// lwz r29,276(r30)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r30 + 276));
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// stw r29,124(r1)
		PPC_STORE_U32(ctx.r1.u32 + 124, var_r29);
		// addi r4,r1,112
		ctx.r4.s64 = ctx.r1.s64 + 112;
		// addi r11,r11,10912
		ctx.r11.s64 = ctx.r11.s64 + 10912;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
		// ld r6,280(r30)
		ctx.r6.u64 = PPC_LOAD_U64(var_r30 + 280);
		// std r6,128(r1)
		PPC_STORE_U64(ctx.r1.u32 + 128, ctx.r6.u64);
		// bl 0x823e9478
		snSession_9478_fw(ctx, base);
		return;
	}
loc_823DE7B8:
	// cmpwi cr6,r5,7
	// bne cr6,0x823de7e0
	if (ctx.r5.s32 == 7) {
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10852
		ctx.r11.s64 = ctx.r11.s64 + 10852;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823e9530
		snSession_9530_fw(ctx, base);
	}
loc_823DE7E0:
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmApplyingConfig_vfn_12"))) PPC_WEAK_FUNC(snCreateMachine_snHsmApplyingConfig_vfn_12);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmApplyingConfig_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmApplyingConfig::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6820(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6820);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823de87c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snCreateMachine_snHsmApplyingConfig::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,228
		var_r29 = (uint32_t)(ctx.r11.s64 + 228);  // addr:0x825d00e4
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmApplyingConfig::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6796(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6796);
	// cmpw cr6,r3,r11
	// bne cr6,0x823de8f4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,252
		var_r29 = (uint32_t)(ctx.r11.s64 + 252);  // addr:0x825d00fc
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823DE8F4:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmStartingSession_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmStartingSession_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmStartingSession_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8692
	ctx.r3.s64 = ctx.r11.s64 + 8692;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmStartingSession_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmStartingSession_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmStartingSession_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236e6c0
	xam_E6C0_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823de968
	if (ctx.r11.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10892
		ctx.r11.s64 = ctx.r11.s64 + 10892;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e95d8
		snSession_95D8_fw(ctx, base);
	}
loc_823DE968:
	// lis r10,-32193
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r10,r10,-20248
	ctx.r10.s64 = ctx.r10.s64 + -20248;
	// stw r31,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, var_r31);
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_E990"))) PPC_WEAK_FUNC(snSession_E990);
PPC_FUNC_IMPL(__imp__snSession_E990) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,10
	// bne cr6,0x823de9e4
	if (ctx.r5.s32 == 10) {
		// lwz r31,20(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 20));
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// lwz r29,276(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 276));
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// stw r29,108(r1)
		PPC_STORE_U32(ctx.r1.u32 + 108, var_r29);
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10912
		ctx.r11.s64 = ctx.r11.s64 + 10912;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// ld r11,280(r31)
		ctx.r11.u64 = PPC_LOAD_U64(var_r31 + 280);
		// std r11,112(r1)
		PPC_STORE_U64(ctx.r1.u32 + 112, ctx.r11.u64);
		// bl 0x823e9478
		snSession_9478_fw(ctx, base);
		return;
	}
loc_823DE9E4:
	// cmpwi cr6,r5,11
	// bne cr6,0x823dea0c
	if (ctx.r5.s32 == 11) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10892
		ctx.r11.s64 = ctx.r11.s64 + 10892;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e95d8
		snSession_95D8_fw(ctx, base);
	}
loc_823DEA0C:
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmStartingSession_vfn_12"))) PPC_WEAK_FUNC(snCreateMachine_snHsmStartingSession_vfn_12);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmStartingSession_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snCreateMachine_snHsmStartingSession::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6832(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6832);
	// cmpw cr6,r3,r11
	// bne cr6,0x823deaac
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,252
		var_r29 = (uint32_t)(ctx.r11.s64 + 252);  // addr:0x825d00fc
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823DEAAC:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmDestroyingSession_vfn_2"))) PPC_WEAK_FUNC(snCreateMachine_snHsmDestroyingSession_vfn_2);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmDestroyingSession_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8708
	ctx.r3.s64 = ctx.r11.s64 + 8708;
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmDestroyingSession_vfn_14"))) PPC_WEAK_FUNC(snCreateMachine_snHsmDestroyingSession_vfn_14);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmDestroyingSession_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236eab8
	xam_EAB8_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823deb20
	if (ctx.r11.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e93d0
		util_93D0(ctx, base);
	}
loc_823DEB20:
	// lis r10,-32193
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r10,r10,-20240
	ctx.r10.s64 = ctx.r10.s64 + -20240;
	// stw r31,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, var_r31);
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_EB48_w"))) PPC_WEAK_FUNC(snSession_EB48_w);
PPC_FUNC_IMPL(__imp__snSession_EB48_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=128, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r5,14
	// bne cr6,0x823deb80
	if (ctx.r5.s32 == 14) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// b 0x823deba0
	} else {
	loc_823DEB80:
		// cmpwi cr6,r5,15
		// bne cr6,0x823deba8
		if (ctx.r5.s32 != 15) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10792
		ctx.r11.s64 = ctx.r11.s64 + 10792;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	}
loc_823DEBA0:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e93d0
	util_93D0(ctx, base);
loc_823DEBA8:
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_vfn_5"))) PPC_WEAK_FUNC(snCreateMachine_vfn_5);
PPC_FUNC_IMPL(__imp__snCreateMachine_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,84
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 84;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,108
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 108;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,132
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 132;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,156
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 156;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,204
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 204;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,228
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 228;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,252
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 252;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_vfn_6"))) PPC_WEAK_FUNC(snCreateMachine_vfn_6);
PPC_FUNC_IMPL(__imp__snCreateMachine_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,84
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 84;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823dec78
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DEC78:
	// addi r3,r31,108
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 108;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823dec9c
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DEC9C:
	// addi r3,r31,132
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 132;
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823decc0
	if (ctx.r5.u32 != 0) {
		// lwz r4,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r11,16(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DECC0:
	// addi r3,r31,156
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 156;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r10,0
	// beq cr6,0x823dece4
	if (ctx.r10.u32 != 0) {
		// lwz r9,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r8,16(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DECE4:
	// addi r3,r31,204
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 204;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r7,0
	// beq cr6,0x823ded08
	if (ctx.r7.u32 != 0) {
		// lwz r6,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r5,16(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DED08:
	// addi r3,r31,228
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 228;
	// lwz r4,8(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r4,0
	// beq cr6,0x823ded2c
	if (ctx.r4.u32 != 0) {
		// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r10,16(r11)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DED2C:
	// addi r3,r31,252
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 252;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r9,0
	// beq cr6,0x823ded50
	if (ctx.r9.u32 != 0) {
		// lwz r8,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r7,16(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DED50:
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtJoinSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtJoinSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6052(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6052);  /* glob:lbl_825D17A4 @ 0x825d17a4 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtJoinSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtJoinSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8728
	ctx.r3.s64 = ctx.r11.s64 + 8728;
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinFailed_vfn_1"))) PPC_WEAK_FUNC(EvtJoinFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtJoinFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6064(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6064);  /* glob:lbl_825D17B0 @ 0x825d17b0 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinFailed_vfn_2"))) PPC_WEAK_FUNC(EvtJoinFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtJoinFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8748
	ctx.r3.s64 = ctx.r11.s64 + 8748;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalJoinSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtLocalJoinSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLocalJoinSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6076(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6076);  /* glob:lbl_825D17BC @ 0x825d17bc */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalJoinSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtLocalJoinSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLocalJoinSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8764
	ctx.r3.s64 = ctx.r11.s64 + 8764;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalJoinFailed_vfn_1"))) PPC_WEAK_FUNC(EvtLocalJoinFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLocalJoinFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6088(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6088);  /* glob:lbl_825D17C8 @ 0x825d17c8 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalJoinFailed_vfn_2"))) PPC_WEAK_FUNC(EvtLocalJoinFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLocalJoinFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8788
	ctx.r3.s64 = ctx.r11.s64 + 8788;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteJoinSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtRemoteJoinSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRemoteJoinSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6100(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6100);  /* glob:lbl_825D17D4 @ 0x825d17d4 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteJoinSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtRemoteJoinSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRemoteJoinSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8808
	ctx.r3.s64 = ctx.r11.s64 + 8808;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteJoinFailed_vfn_1"))) PPC_WEAK_FUNC(EvtRemoteJoinFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRemoteJoinFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6112(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6112);  /* glob:lbl_825D17E0 @ 0x825d17e0 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteJoinFailed_vfn_2"))) PPC_WEAK_FUNC(EvtRemoteJoinFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRemoteJoinFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8832
	ctx.r3.s64 = ctx.r11.s64 + 8832;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestJoinSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtRequestJoinSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRequestJoinSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6124(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6124);  /* glob:lbl_825D17EC @ 0x825d17ec */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestJoinSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtRequestJoinSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRequestJoinSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8852
	ctx.r3.s64 = ctx.r11.s64 + 8852;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestJoinFailed_vfn_1"))) PPC_WEAK_FUNC(EvtRequestJoinFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRequestJoinFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6136(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6136);  /* glob:lbl_825D17F8 @ 0x825d17f8 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestJoinFailed_vfn_2"))) PPC_WEAK_FUNC(EvtRequestJoinFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRequestJoinFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8876
	ctx.r3.s64 = ctx.r11.s64 + 8876;
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptJoinRequestSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtAcceptJoinRequestSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtAcceptJoinRequestSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6148(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6148);  /* glob:lbl_825D1804 @ 0x825d1804 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptJoinRequestSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtAcceptJoinRequestSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtAcceptJoinRequestSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8900
	ctx.r3.s64 = ctx.r11.s64 + 8900;
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptJoinRequestFailed_vfn_1"))) PPC_WEAK_FUNC(EvtAcceptJoinRequestFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtAcceptJoinRequestFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6160(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6160);  /* glob:lbl_825D1810 @ 0x825d1810 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptJoinRequestFailed_vfn_2"))) PPC_WEAK_FUNC(EvtAcceptJoinRequestFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtAcceptJoinRequestFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8932
	ctx.r3.s64 = ctx.r11.s64 + 8932;
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionJoinSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtSessionJoinSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtSessionJoinSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6172(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6172);  /* glob:lbl_825D181C @ 0x825d181c */
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionJoinSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtSessionJoinSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtSessionJoinSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8960
	ctx.r3.s64 = ctx.r11.s64 + 8960;
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionJoinFailed_vfn_1"))) PPC_WEAK_FUNC(EvtSessionJoinFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtSessionJoinFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6184(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6184);  /* glob:lbl_825D1828 @ 0x825d1828 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionJoinFailed_vfn_2"))) PPC_WEAK_FUNC(EvtSessionJoinFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtSessionJoinFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,8984
	ctx.r3.s64 = ctx.r11.s64 + 8984;
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoin_vfn_1"))) PPC_WEAK_FUNC(EvtJoin_vfn_1);
PPC_FUNC_IMPL(__imp__EvtJoin_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6196(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6196);  /* glob:lbl_825D1834 @ 0x825d1834 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoin_vfn_2"))) PPC_WEAK_FUNC(EvtJoin_vfn_2);
PPC_FUNC_IMPL(__imp__EvtJoin_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9008
	ctx.r3.s64 = ctx.r11.s64 + 9008;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteJoin_vfn_1"))) PPC_WEAK_FUNC(EvtRemoteJoin_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRemoteJoin_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6208(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6208);  /* glob:lbl_825D1840 @ 0x825d1840 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteJoin_vfn_2"))) PPC_WEAK_FUNC(EvtRemoteJoin_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRemoteJoin_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9016
	ctx.r3.s64 = ctx.r11.s64 + 9016;
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinRequest_vfn_1"))) PPC_WEAK_FUNC(EvtJoinRequest_vfn_1);
PPC_FUNC_IMPL(__imp__EvtJoinRequest_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6220(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6220);  /* glob:lbl_825D184C @ 0x825d184c */
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinRequest_vfn_2"))) PPC_WEAK_FUNC(EvtJoinRequest_vfn_2);
PPC_FUNC_IMPL(__imp__EvtJoinRequest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9032
	ctx.r3.s64 = ctx.r11.s64 + 9032;
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinReply_vfn_1"))) PPC_WEAK_FUNC(EvtJoinReply_vfn_1);
PPC_FUNC_IMPL(__imp__EvtJoinReply_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6232(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6232);  /* glob:lbl_825D1858 @ 0x825d1858 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoinReply_vfn_2"))) PPC_WEAK_FUNC(EvtJoinReply_vfn_2);
PPC_FUNC_IMPL(__imp__EvtJoinReply_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9048
	ctx.r3.s64 = ctx.r11.s64 + 9048;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_EF68"))) PPC_WEAK_FUNC(snSession_EF68);
PPC_FUNC_IMPL(__imp__snSession_EF68) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,16
	// bne cr6,0x823df00c
	if (ctx.r5.s32 == 16) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r30)
  // [ph4a] vtable load collapsed
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r31,r11,11708
		var_r31 = (uint32_t)(ctx.r11.s64 + 11708);  // lbl_82072DBC @ 0x82072dbc
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// bctrl
		VCALL(var_r30, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r30,56(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823df034
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r31,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r31);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823df034
	} else {
	loc_823DF00C:
		// cmpwi cr6,r5,17
		// bne cr6,0x823df034
		if (ctx.r5.s32 != 17) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,11728
		ctx.r11.s64 = ctx.r11.s64 + 11728;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823e9680
		snSession_9680_fw(ctx, base);
	}
loc_823DF034:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9064
	ctx.r3.s64 = ctx.r11.s64 + 9064;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=256, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r10,0
	ctx.r10.s64 = 0;
	// li r8,-1
	// addi r11,r31,324
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 324;
	// li r9,9
	ctx.r9.s64 = 9;
	// stw r10,292(r31)
	PPC_STORE_U32(var_r31 + 292, ctx.r10.u32);
	// stb r10,296(r31)
	PPC_STORE_U8(var_r31 + 296, ctx.r10.u8);
	// std r10,312(r31)
	PPC_STORE_U64(var_r31 + 312, ctx.r10.u64);
	// stw r8,320(r31)
	PPC_STORE_U32(var_r31 + 320, ctx.r8.u32);
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823DF098:
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823df098
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823DF098;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r31,360
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 360;
	// bl 0x822603d0
	snJoinMachine_03D0_g(ctx, base);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823e04a0
	SinglesNetworkClient_04A0_g(ctx, base);
	// addi r7,r3,4
	ctx.r7.s64 = ctx.r3.s64 + 4;
	// addi r11,r31,464
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 464;
	// addi r4,r3,16
	ctx.r4.s64 = ctx.r3.s64 + 16;
	// addi r6,r11,4
	ctx.r6.s64 = ctx.r11.s64 + 4;
	// addi r3,r11,16
	ctx.r3.s64 = ctx.r11.s64 + 16;
	// lwz r5,0(r7)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// stw r5,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
	// lwz r11,4(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
	// stw r11,4(r6)
	PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
	// bl 0x823e0500
	util_0500(ctx, base);
	// addi r3,r1,132
	ctx.r3.s64 = ctx.r1.s64 + 132;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32193
	// lbz r9,624(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 624);
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// addi r10,r11,-20232
	ctx.r10.s64 = ctx.r11.s64 + -20232;
	// lis r11,-32249
	// clrlwi r8,r9,25
	ctx.r8.u64 = ctx.r9.u32 & 0x7F;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// stb r8,624(r31)
	PPC_STORE_U8(var_r31 + 624, ctx.r8.u8);
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// stw r10,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r10.u32);
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x823dd170
	util_D170(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_vfn_12"))) PPC_WEAK_FUNC(snJoinMachine_vfn_12);
PPC_FUNC_IMPL(__imp__snJoinMachine_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=608, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6076(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6076);
	// cmpw cr6,r3,r11
	// bne cr6,0x823df248
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,292(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 292);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x823e7310
		SinglesNetworkClient_7310_g(ctx, base);
		// lis r11,-32249
		// lis r10,-32249
		// addi r11,r11,17352
		ctx.r11.s64 = ctx.r11.s64 + 17352;
		// addi r10,r10,17340
		ctx.r10.s64 = ctx.r10.s64 + 17340;
		// addi r3,r1,248
		ctx.r3.s64 = ctx.r1.s64 + 248;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r10,240(r1)
		PPC_STORE_U32(ctx.r1.u32 + 240, ctx.r10.u32);
		// stw r11,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
		// stw r11,244(r1)
		PPC_STORE_U32(ctx.r1.u32 + 244, ctx.r11.u32);
		// bl 0x823b3fd8
		SinglesNetworkClient_3FD8_g(ctx, base);
		// lwz r6,292(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 292);
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r3,r1,248
		ctx.r3.s64 = ctx.r1.s64 + 248;
		// stw r6,244(r1)
		PPC_STORE_U32(ctx.r1.u32 + 244, ctx.r6.u32);
		// bl 0x822603d0
		snJoinMachine_03D0_g(ctx, base);
		// lwz r5,292(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 292);
		// addi r6,r1,240
		ctx.r6.s64 = ctx.r1.s64 + 240;
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// stw r5,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r5.u32);
		// li r5,6
		ctx.r5.s64 = 6;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// li r5,10
		ctx.r5.s64 = 10;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// addi r3,r1,248
		ctx.r3.s64 = ctx.r1.s64 + 248;
		// bl 0x82260268
		SinglesNetworkClient_0268_g(ctx, base);
		// lis r11,-32249
		// addi r3,r1,200
		ctx.r3.s64 = ctx.r1.s64 + 200;
		// addi r11,r11,10780
		ctx.r11.s64 = ctx.r11.s64 + 10780;
		// stw r11,240(r1)
		PPC_STORE_U32(ctx.r1.u32 + 240, ctx.r11.u32);
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,200
		ctx.r4.s64 = ctx.r1.s64 + 200;
		// addi r11,r11,11508
		ctx.r11.s64 = ctx.r11.s64 + 11508;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,200(r1)
		PPC_STORE_U32(ctx.r1.u32 + 200, ctx.r11.u32);
		// bl 0x823e9728
		snSession_9728_fw(ctx, base);
		// b 0x823df610
	} else {
	loc_823DF248:
		// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r11,40(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r9,4(r10)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6088(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6088);
		// cmpw cr6,r3,r11
		// bne cr6,0x823df30c
		if (ctx.r3.s32 == ctx.r11.s32) {
			// lwz r11,292(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 292);
			// lwz r3,16(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
			// addi r4,r11,96
			ctx.r4.s64 = ctx.r11.s64 + 96;
			// bl 0x823e81c0
			snLeaveMachine_81C0_g(ctx, base);
			// lis r11,-32249
			// li r8,0
			ctx.r8.s64 = 0;
			// addi r11,r11,17340
			ctx.r11.s64 = ctx.r11.s64 + 17340;
			// addi r3,r1,360
			ctx.r3.s64 = ctx.r1.s64 + 360;
			// stw r8,356(r1)
			PPC_STORE_U32(ctx.r1.u32 + 356, ctx.r8.u32);
			// stw r11,352(r1)
			PPC_STORE_U32(ctx.r1.u32 + 352, ctx.r11.u32);
			// bl 0x823b3fd8
			SinglesNetworkClient_3FD8_g(ctx, base);
			// lwz r7,292(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 292);
			// li r4,0
			ctx.r4.s64 = 0;
			// addi r3,r1,360
			ctx.r3.s64 = ctx.r1.s64 + 360;
			// stw r7,356(r1)
			PPC_STORE_U32(ctx.r1.u32 + 356, ctx.r7.u32);
			// bl 0x822603d0
			snJoinMachine_03D0_g(ctx, base);
			// lwz r4,16(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
			// addi r6,r1,352
			ctx.r6.s64 = ctx.r1.s64 + 352;
			// li r5,7
			ctx.r5.s64 = 7;
			// addi r3,r4,108
			ctx.r3.s64 = ctx.r4.s64 + 108;
			// bl 0x82371bf8
			snSession_1BF8_g(ctx, base);
			// addi r3,r1,360
			ctx.r3.s64 = ctx.r1.s64 + 360;
			// bl 0x82260268
			SinglesNetworkClient_0268_g(ctx, base);
			// lis r11,-32249
			// addi r3,r1,216
			ctx.r3.s64 = ctx.r1.s64 + 216;
			// addi r11,r11,10780
			ctx.r11.s64 = ctx.r11.s64 + 10780;
			// stw r11,352(r1)
			PPC_STORE_U32(ctx.r1.u32 + 352, ctx.r11.u32);
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,216
			ctx.r4.s64 = ctx.r1.s64 + 216;
			// addi r11,r11,11528
			ctx.r11.s64 = ctx.r11.s64 + 11528;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,216(r1)
			PPC_STORE_U32(ctx.r1.u32 + 216, ctx.r11.u32);
			// bl 0x823e97d0
			snJoinMachine_97D0(ctx, base);
			// b 0x823df610
		} else {
		loc_823DF30C:
			// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lwz r5,40(r6)
  // [ph4a] slot load collapsed
			// bctrl
			VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
			// lwz r3,12(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
			// lwz r11,4(r4)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// lis r11,-32163
			ctx.r11.s64 = -2107834368;
			// lwz r11,6100(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6100);
			// cmpw cr6,r3,r11
			// bne cr6,0x823df39c
			if (ctx.r3.s32 == ctx.r11.s32) {
				// lis r11,-32249
				// lwz r10,292(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 292);
				// lwz r4,16(r31)
				ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
				// addi r6,r1,88
				ctx.r6.s64 = ctx.r1.s64 + 88;
				// addi r11,r11,17352
				ctx.r11.s64 = ctx.r11.s64 + 17352;
				// li r5,10
				ctx.r5.s64 = 10;
				// addi r3,r4,108
				ctx.r3.s64 = ctx.r4.s64 + 108;
				// stw r10,92(r1)
				PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
				// stw r11,88(r1)
				PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
				// bl 0x82371bf8
				snSession_1BF8_g(ctx, base);
				// lis r11,-32249
				// addi r3,r1,152
				ctx.r3.s64 = ctx.r1.s64 + 152;
				// addi r11,r11,10780
				ctx.r11.s64 = ctx.r11.s64 + 10780;
				// stw r11,88(r1)
				PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
				// bl 0x823dda08
				util_DA08(ctx, base);
				// lis r11,-32249
				// addi r4,r1,152
				ctx.r4.s64 = ctx.r1.s64 + 152;
				// addi r11,r11,11508
				ctx.r11.s64 = ctx.r11.s64 + 11508;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// stw r11,152(r1)
				PPC_STORE_U32(ctx.r1.u32 + 152, ctx.r11.u32);
				// bl 0x823e9728
				snSession_9728_fw(ctx, base);
				// b 0x823df610
			} else {
			loc_823DF39C:
				// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// lwz r8,40(r9)
  // [ph4a] slot load collapsed
				// bctrl
				VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
				// lwz r3,12(r3)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
				// lwz r6,4(r7)
				// bctrl
				VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
				// lis r11,-32163
				ctx.r11.s64 = -2107834368;
				// lwz r11,6112(r11)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6112);
				// cmpw cr6,r3,r11
				// bne cr6,0x823df404
				if (ctx.r3.s32 == ctx.r11.s32) {
					// addi r4,r31,296
					ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 296;
					// lwz r3,16(r31)
					ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
					// bl 0x823e81c0
					snLeaveMachine_81C0_g(ctx, base);
					// addi r3,r1,104
					ctx.r3.s64 = ctx.r1.s64 + 104;
					// bl 0x823dda08
					util_DA08(ctx, base);
					// lis r11,-32249
					// addi r4,r1,104
					ctx.r4.s64 = ctx.r1.s64 + 104;
					// addi r11,r11,11528
					ctx.r11.s64 = ctx.r11.s64 + 11528;
					// mr r3,r31
					ctx.r3.u64 = var_r31;
					// stw r11,104(r1)
					PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r11.u32);
					// bl 0x823e97d0
					snJoinMachine_97D0(ctx, base);
					// b 0x823df610
				} else {
				loc_823DF404:
					// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
					// mr r3,r31
					ctx.r3.u64 = var_r31;
					// lwz r4,40(r5)
  // [ph4a] slot load collapsed
					// bctrl
					VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
					// lwz r3,12(r3)
					ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
					// lwz r10,4(r11)
					// bctrl
					VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
					// lis r11,-32163
					ctx.r11.s64 = -2107834368;
					// lwz r11,6124(r11)
					ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6124);
					// cmpw cr6,r3,r11
					// bne cr6,0x823df4e4
					if (ctx.r3.s32 == ctx.r11.s32) {
						// lis r11,-32249
						// li r9,0
						ctx.r9.s64 = 0;
						// addi r11,r11,17340
						ctx.r11.s64 = ctx.r11.s64 + 17340;
						// addi r3,r1,472
						ctx.r3.s64 = ctx.r1.s64 + 472;
						// stw r9,468(r1)
						PPC_STORE_U32(ctx.r1.u32 + 468, ctx.r9.u32);
						// stw r11,464(r1)
						PPC_STORE_U32(ctx.r1.u32 + 464, ctx.r11.u32);
						// bl 0x823b3fd8
						SinglesNetworkClient_3FD8_g(ctx, base);
						// lwz r11,20(r31)
						ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
						// addi r3,r1,464
						ctx.r3.s64 = ctx.r1.s64 + 464;
						// lwz r4,292(r31)
						ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 292);
						// addi r5,r11,516
						ctx.r5.s64 = ctx.r11.s64 + 516;
						// bl 0x823efdf0
						snJoinMachine_FDF0_g(ctx, base);
						// lwz r4,16(r31)
						ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
						// addi r6,r1,464
						ctx.r6.s64 = ctx.r1.s64 + 464;
						// li r5,6
						ctx.r5.s64 = 6;
						// addi r3,r4,108
						ctx.r3.s64 = ctx.r4.s64 + 108;
						// bl 0x82371bf8
						snSession_1BF8_g(ctx, base);
						// lis r11,-32249
						// lwz r8,292(r31)
						ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 292);
						// addi r6,r1,96
						ctx.r6.s64 = ctx.r1.s64 + 96;
						// addi r11,r11,17352
						ctx.r11.s64 = ctx.r11.s64 + 17352;
						// lwz r4,16(r31)
						ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
						// li r5,10
						ctx.r5.s64 = 10;
						// addi r3,r4,108
						ctx.r3.s64 = ctx.r4.s64 + 108;
						// stw r8,100(r1)
						PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r8.u32);
						// stw r11,96(r1)
						PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
						// bl 0x82371bf8
						snSession_1BF8_g(ctx, base);
						// lis r11,-32249
						// addi r3,r1,472
						ctx.r3.s64 = ctx.r1.s64 + 472;
						// addi r30,r11,10780
						var_r30 = (uint32_t)(ctx.r11.s64 + 10780);  // lbl_82072A1C @ 0x82072a1c
						// stw r30,96(r1)
						PPC_STORE_U32(ctx.r1.u32 + 96, var_r30);
						// bl 0x82260268
						SinglesNetworkClient_0268_g(ctx, base);
						// addi r3,r1,184
						ctx.r3.s64 = ctx.r1.s64 + 184;
						// stw r30,464(r1)
						PPC_STORE_U32(ctx.r1.u32 + 464, var_r30);
						// bl 0x823dda08
						util_DA08(ctx, base);
						// lis r11,-32249
						// addi r4,r1,184
						ctx.r4.s64 = ctx.r1.s64 + 184;
						// addi r11,r11,11508
						ctx.r11.s64 = ctx.r11.s64 + 11508;
						// mr r3,r31
						ctx.r3.u64 = var_r31;
						// stw r11,184(r1)
						PPC_STORE_U32(ctx.r1.u32 + 184, ctx.r11.u32);
						// bl 0x823e9728
						snSession_9728_fw(ctx, base);
						// b 0x823df610
					} else {
					loc_823DF4E4:
						// lwz r7,0(r31)
  // [ph4a] vtable load collapsed
						// mr r3,r31
						ctx.r3.u64 = var_r31;
						// lwz r6,40(r7)
  // [ph4a] slot load collapsed
						// bctrl
						VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
						// lwz r3,12(r3)
						ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
						// lwz r4,4(r5)
						// bctrl
						VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
						// lis r11,-32163
						ctx.r11.s64 = -2107834368;
						// lwz r11,6136(r11)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6136);
						// cmpw cr6,r3,r11
						// mr r3,r31
						ctx.r3.u64 = var_r31;
						// bne cr6,0x823df548
						if (ctx.r3.s32 == ctx.r11.s32) {
							// bl 0x823f0438
							snLeaveMachine_0438_g(ctx, base);
							// addi r3,r1,120
							ctx.r3.s64 = ctx.r1.s64 + 120;
							// bl 0x823dda08
							util_DA08(ctx, base);
							// lis r11,-32249
							// addi r4,r1,120
							ctx.r4.s64 = ctx.r1.s64 + 120;
							// addi r11,r11,11528
							ctx.r11.s64 = ctx.r11.s64 + 11528;
							// mr r3,r31
							ctx.r3.u64 = var_r31;
							// stw r11,120(r1)
							PPC_STORE_U32(ctx.r1.u32 + 120, ctx.r11.u32);
							// bl 0x823e97d0
							snJoinMachine_97D0(ctx, base);
							// b 0x823df610
						} else {
							// lwz r10,40(r11)
							// bctrl
							VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
							// lwz r3,12(r3)
							ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
							// lwz r8,4(r9)
							// bctrl
							VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
							// lis r11,-32163
							ctx.r11.s64 = -2107834368;
							// lwz r11,6148(r11)
							ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6148);
							// cmpw cr6,r3,r11
							// mr r3,r31
							ctx.r3.u64 = var_r31;
							// bne cr6,0x823df5a8
							if (ctx.r3.s32 == ctx.r11.s32) {
								// bl 0x823f0778
								snSession_0778_g(ctx, base);
								// addi r3,r1,136
								ctx.r3.s64 = ctx.r1.s64 + 136;
								// bl 0x823dda08
								util_DA08(ctx, base);
								// lis r11,-32249
								// addi r4,r1,136
								ctx.r4.s64 = ctx.r1.s64 + 136;
								// addi r11,r11,11508
								ctx.r11.s64 = ctx.r11.s64 + 11508;
								// mr r3,r31
								ctx.r3.u64 = var_r31;
								// stw r11,136(r1)
								PPC_STORE_U32(ctx.r1.u32 + 136, ctx.r11.u32);
								// bl 0x823e9728
								snSession_9728_fw(ctx, base);
								// b 0x823df610
							} else {
								// lwz r6,40(r7)
								// bctrl
								VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
								// lwz r3,12(r3)
								ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
								// lwz r4,4(r5)
								// bctrl
								VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
								// lis r11,-32163
								ctx.r11.s64 = -2107834368;
								// lwz r11,6160(r11)
								ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6160);
								// cmpw cr6,r3,r11
								// bne cr6,0x823df608
								if (ctx.r3.s32 == ctx.r11.s32) {
									// mr r3,r31
									ctx.r3.u64 = var_r31;
									// bl 0x823f0880
									snLeaveMachine_0880_g(ctx, base);
									// addi r3,r1,168
									ctx.r3.s64 = ctx.r1.s64 + 168;
									// bl 0x823dda08
									util_DA08(ctx, base);
									// lis r11,-32249
									// addi r4,r1,168
									ctx.r4.s64 = ctx.r1.s64 + 168;
									// addi r11,r11,11528
									ctx.r11.s64 = ctx.r11.s64 + 11528;
									// mr r3,r31
									ctx.r3.u64 = var_r31;
									// stw r11,168(r1)
									PPC_STORE_U32(ctx.r1.u32 + 168, ctx.r11.u32);
									// bl 0x823e97d0
									snJoinMachine_97D0(ctx, base);
									// b 0x823df610
								} else {
								loc_823DF608:
									// li r3,0
									ctx.r3.s64 = 0;
									// stb r3,0(r30)
									PPC_STORE_U8(var_r30 + 0, ctx.r3.u8);
								}
							}
						}
					}
				}
			}
		}
	}
loc_823DF610:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmLocalJoinPending_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmLocalJoinPending_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmLocalJoinPending_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9076
	ctx.r3.s64 = ctx.r11.s64 + 9076;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmLocalJoinPending_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snHsmLocalJoinPending_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmLocalJoinPending_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823eff38
	SinglesNetworkClient_FF38_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823df674
	if (ctx.r11.u32 != 0) {
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x823efeb8
		util_FEB8(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// bne cr6,0x823df694
		if (ctx.r10.u32 != 0) {
			// blr
			return;
		}
	}
loc_823DF674:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,11568
	ctx.r11.s64 = ctx.r11.s64 + 11568;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e9878
	snHsmLocalJoinPending_9878(ctx, base);
loc_823DF694:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmLocalJoinPending_vfn_12"))) PPC_WEAK_FUNC(snJoinMachine_snHsmLocalJoinPending_vfn_12);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmLocalJoinPending_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmLocalJoinPending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6172(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6172);
	// cmpw cr6,r3,r11
	// bne cr6,0x823df784
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,11548
		var_r30 = (uint32_t)(ctx.r11.s64 + 11548);  // lbl_82072D1C @ 0x82072d1c
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823df7e8
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823df7e8
	} else {
	loc_823DF784:
		// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6184(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6184);
		// cmpw cr6,r3,r11
		// bne cr6,0x823df7e0
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,11568
			ctx.r11.s64 = ctx.r11.s64 + 11568;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e9878
			snHsmLocalJoinPending_9878(ctx, base);
			// b 0x823df7e8
		} else {
		loc_823DF7E0:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823DF7E8:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9096
	ctx.r3.s64 = ctx.r11.s64 + 9096;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_vfn_13"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_vfn_13);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,44(r11)
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // snJoinMachine_snHsmRequestingJoin::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// addi r29,r11,96
	var_r29 = (uint32_t)(ctx.r11.s64 + 96);  // addr:0x82070060
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// bl 0x823ed988
	util_D988(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9112
	ctx.r3.s64 = ctx.r11.s64 + 9112;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32193
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,5384
	ctx.r11.s64 = ctx.r11.s64 + 5384;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// lwz r10,16(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r3,164(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f0140
	SinglesNetworkClient_0140_g(ctx, base);
	// clrlwi r9,r3,24
	ctx.r9.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// bne cr6,0x823df8ec
	if (ctx.r9.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,11648
		ctx.r11.s64 = ctx.r11.s64 + 11648;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9920
		snHsmWaitingForReply_9920(ctx, base);
		// blr
		return;
	}
loc_823DF8EC:
	// lwz r8,20(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,16(r8)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
	// lwz r10,408(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 408);
	// lwz r11,404(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 404);
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// addi r7,r11,50
	ctx.r7.s64 = ctx.r11.s64 + 50;
	// mullw r6,r7,r11
	ctx.r6.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r11.s32);
	// mulli r4,r6,100
	ctx.r4.s64 = static_cast<int64_t>(ctx.r6.u64 * static_cast<uint64_t>(100));
	// bl 0x823ed4f8
	util_D4F8(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_12"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_12);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6244(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6244);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823df9bc
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,144
		var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x825d0090
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6256(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6256);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823dfa78
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,40(r8)
		// bctrl
		VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
		// lis r11,-32162
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// addi r6,r11,21668
		ctx.r6.s64 = ctx.r11.s64 + 21668;
		// lis r11,-32162
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r5,r11,21576
		ctx.r5.s64 = ctx.r11.s64 + 21576;
		// bl 0x82430978
		SinglesNetworkClient_0978_g(ctx, base);
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// addi r6,r3,4
		ctx.r6.s64 = ctx.r3.s64 + 4;
		// addi r11,r11,464
		ctx.r11.s64 = ctx.r11.s64 + 464;
		// addi r4,r3,16
		ctx.r4.s64 = ctx.r3.s64 + 16;
		// addi r5,r11,4
		ctx.r5.s64 = ctx.r11.s64 + 4;
		// addi r3,r11,16
		ctx.r3.s64 = ctx.r11.s64 + 16;
		// lwz r11,0(r6)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
		// stw r11,0(r5)
		PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r11.u32);
		// lwz r10,4(r6)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r6.u32 + 4);
		// stw r10,4(r5)
		PPC_STORE_U32(ctx.r5.u32 + 4, ctx.r10.u32);
		// bl 0x823e0500
		util_0500(ctx, base);
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,11648
		ctx.r11.s64 = ctx.r11.s64 + 11648;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9920
		snHsmWaitingForReply_9920(ctx, base);
		return;
	}
	// lwz r8,40(r9)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r6,4(r7)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823dfacc
	if (ctx.r3.s32 == 0) {
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,11648
		ctx.r11.s64 = ctx.r11.s64 + 11648;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823e9920
		snHsmWaitingForReply_9920(ctx, base);
		return;
	}
loc_823DFACC:
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r5,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r5.u8);
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9128
	ctx.r3.s64 = ctx.r11.s64 + 9128;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmJoinPending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,21704
	ctx.r6.s64 = ctx.r11.s64 + 21704;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r9,r3,4
	ctx.r9.s64 = ctx.r3.s64 + 4;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// addi r4,r3,16
	ctx.r4.s64 = ctx.r3.s64 + 16;
	// addi r7,r11,4
	ctx.r7.s64 = ctx.r11.s64 + 4;
	// addi r3,r11,16
	ctx.r3.s64 = ctx.r11.s64 + 16;
	// lwz r8,0(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// stw r8,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r8.u32);
	// lwz r6,4(r9)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	// stw r6,4(r7)
	PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r6.u32);
	// bl 0x823e0500
	util_0500(ctx, base);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r5,620(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 620);
	// stw r5,320(r11)
	PPC_STORE_U32(ctx.r11.u32 + 320, ctx.r5.u32);
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823efeb8
	util_FEB8(ctx, base);
	// clrlwi r4,r3,24
	ctx.r4.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// bne cr6,0x823dfba0
	if (ctx.r4.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,11728
		ctx.r11.s64 = ctx.r11.s64 + 11728;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9680
		snSession_9680_fw(ctx, base);
	}
loc_823DFBA0:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_12"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_12);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmJoinPending_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRequestingJoin_snHsmJoinPending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6172(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6172);
	// cmpw cr6,r3,r11
	// bne cr6,0x823dfc90
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,11628
		var_r30 = (uint32_t)(ctx.r11.s64 + 11628);  // lbl_82072D6C @ 0x82072d6c
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823dfd14
		if (ctx.r4.u32 == 0) {
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		return;
	}
loc_823DFC90:
	// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r4,40(r5)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6184);
	// cmpw cr6,r3,r11
	// bne cr6,0x823dfd0c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r8,44(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,168
		var_r29 = (uint32_t)(ctx.r11.s64 + 168);  // addr:0x825d00a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823DFD0C:
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r7,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r7.u8);
loc_823DFD14:
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmUndoingJoinRequest_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmUndoingJoinRequest_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmUndoingJoinRequest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9140
	ctx.r3.s64 = ctx.r11.s64 + 9140;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmUndoingJoinRequest_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmUndoingJoinRequest_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmUndoingJoinRequest_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,11648
	ctx.r11.s64 = ctx.r11.s64 + 11648;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e9920
	snHsmWaitingForReply_9920(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_vfn_5"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_vfn_5);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,72
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 72;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,96
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 96;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_vfn_6"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_vfn_6);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823dfe00
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DFE00:
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823dfe24
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DFE24:
	// addi r3,r31,96
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 96;
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823dfe48
	if (ctx.r5.u32 != 0) {
		// lwz r4,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r11,16(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823DFE48:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRemoteJoinPending_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRemoteJoinPending_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRemoteJoinPending_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9160
	ctx.r3.s64 = ctx.r11.s64 + 9160;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRemoteJoinPending_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRemoteJoinPending_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRemoteJoinPending_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f0040
	game_0040(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823dfeac
	if (ctx.r11.u32 != 0) {
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x823efeb8
		util_FEB8(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// bne cr6,0x823dfecc
		if (ctx.r10.u32 != 0) {
			// blr
			return;
		}
	}
loc_823DFEAC:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,11608
	ctx.r11.s64 = ctx.r11.s64 + 11608;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e99c8
	snHsmRemoteJoinPending_99C8(ctx, base);
loc_823DFECC:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRemoteJoinPending_vfn_12"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRemoteJoinPending_vfn_12);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRemoteJoinPending_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmRemoteJoinPending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6172(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6172);
	// cmpw cr6,r3,r11
	// bne cr6,0x823dffbc
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,11588
		var_r30 = (uint32_t)(ctx.r11.s64 + 11588);  // lbl_82072D44 @ 0x82072d44
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e0020
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e0020
	} else {
	loc_823DFFBC:
		// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6184(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6184);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e0018
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,11608
			ctx.r11.s64 = ctx.r11.s64 + 11608;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e99c8
			snHsmRemoteJoinPending_99C8(ctx, base);
			// b 0x823e0020
		} else {
		loc_823E0018:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823E0020:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmAcceptingJoinRequest_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snHsmAcceptingJoinRequest_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmAcceptingJoinRequest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9180
	ctx.r3.s64 = ctx.r11.s64 + 9180;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmAcceptingJoinRequest_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snHsmAcceptingJoinRequest_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmAcceptingJoinRequest_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f0528
	SinglesNetworkClient_0528_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e0084
	if (ctx.r11.u32 != 0) {
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x823efeb8
		util_FEB8(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// bne cr6,0x823e00a4
		if (ctx.r10.u32 != 0) {
			// blr
			return;
		}
	}
loc_823E0084:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,11688
	ctx.r11.s64 = ctx.r11.s64 + 11688;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e9a70
	snHsmAcceptingJoinRequest_9A70(ctx, base);
loc_823E00A4:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmAcceptingJoinRequest_vfn_12"))) PPC_WEAK_FUNC(snJoinMachine_snHsmAcceptingJoinRequest_vfn_12);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmAcceptingJoinRequest_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmAcceptingJoinRequest::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6172(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6172);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e014c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snJoinMachine_snHsmAcceptingJoinRequest::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,240
		var_r29 = (uint32_t)(ctx.r11.s64 + 240);  // addr:0x825d00f0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snJoinMachine_snHsmAcceptingJoinRequest::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6184);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e01a8
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,11688
		ctx.r11.s64 = ctx.r11.s64 + 11688;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9a70
		snHsmAcceptingJoinRequest_9A70(ctx, base);
		return;
	}
loc_823E01A8:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snJoinMachine_snSendingGamerData_vfn_2"))) PPC_WEAK_FUNC(snJoinMachine_snSendingGamerData_vfn_2);
PPC_FUNC_IMPL(__imp__snJoinMachine_snSendingGamerData_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9204
	ctx.r3.s64 = ctx.r11.s64 + 9204;
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snSendingGamerData_vfn_9"))) PPC_WEAK_FUNC(snJoinMachine_snSendingGamerData_vfn_9);
PPC_FUNC_IMPL(__imp__snJoinMachine_snSendingGamerData_vfn_9) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e01f8
	SinglesNetworkClient_01F8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__snJoinMachine_snSendingGamerData_vfn_8"))) PPC_WEAK_FUNC(snJoinMachine_snSendingGamerData_vfn_8);
PPC_FUNC_IMPL(__imp__snJoinMachine_snSendingGamerData_vfn_8) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snSendingGamerData_vfn_14"))) PPC_WEAK_FUNC(snJoinMachine_snSendingGamerData_vfn_14);
PPC_FUNC_IMPL(__imp__snJoinMachine_snSendingGamerData_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,40(r3)
	PPC_STORE_U32(ctx.r3.u32 + 40, ctx.r11.u32);
	// stw r11,44(r3)
	PPC_STORE_U32(ctx.r3.u32 + 44, ctx.r11.u32);
	// stw r11,48(r3)
	PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_01F8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_01F8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_01F8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,48(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 48);
	// cmpwi cr6,r11,0
	// bne cr6,0x823e024c
	if (ctx.r11.s32 == 0) {
		// lwz r11,44(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 44);
		// subf. r10,r11,r30
		ctx.r10.s64 = (int64_t)(int32_t)var_r30 - ctx.r11.s64;
		// bge 0x823e0238
		if (ctx.r10.s32 < 0) {
			// lwz r9,40(r31)
			ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 40);
			// cmpwi cr6,r9,0
			// bne cr6,0x823e033c
			if (ctx.r9.s32 != 0) {
				// blr
				return;
			}
		}
	loc_823E0238:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823f0f80
		SinglesNetworkClient_0F80_g(ctx, base);
		// addi r8,r30,100
		ctx.r8.s64 = (int64_t)(int32_t)var_r30 + 100;
		// stw r8,44(r31)
		PPC_STORE_U32(var_r31 + 44, ctx.r8.u32);
		// b 0x823e033c
	} else {
	loc_823E024C:
		// cmpwi cr6,r11,1
		// bne cr6,0x823e02b4
		if (ctx.r11.s32 == 1) {
			// lwz r7,44(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 44);
			// subf. r6,r7,r30
			ctx.r6.s64 = (int64_t)(int32_t)var_r30 - ctx.r7.s64;
			// bge 0x823e026c
			if (ctx.r6.s32 < 0) {
				// lwz r5,40(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 40);
				// cmpwi cr6,r5,0
				// bne cr6,0x823e033c
				if (ctx.r5.s32 != 0) {
					// blr
					return;
				}
			}
		loc_823E026C:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x823f1178
			SinglesNetworkClient_1178_g(ctx, base);
			// clrlwi r4,r3,24
			ctx.r4.u64 = ctx.r3.u32 & 0xFF;
			// cmplwi cr6,r4,0
			// bne cr6,0x823e02a8
			if (ctx.r4.u32 == 0) {
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// bl 0x823ed3b0
				game_D3B0_h(ctx, base);
				// addi r3,r1,96
				ctx.r3.s64 = ctx.r1.s64 + 96;
				// bl 0x823dda08
				util_DA08(ctx, base);
				// lis r11,-32249
				// addi r4,r1,96
				ctx.r4.s64 = ctx.r1.s64 + 96;
				// addi r11,r11,11688
				ctx.r11.s64 = ctx.r11.s64 + 11688;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// stw r11,96(r1)
				PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
				// bl 0x823e9a70
				snHsmAcceptingJoinRequest_9A70(ctx, base);
			}
		loc_823E02A8:
			// addi r3,r30,100
			ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 100;
			// stw r3,44(r31)
			PPC_STORE_U32(var_r31 + 44, ctx.r3.u32);
			// b 0x823e033c
		} else {
		loc_823E02B4:
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x823ed3b0
			game_D3B0_h(ctx, base);
			// addi r3,r1,80
			ctx.r3.s64 = ctx.r1.s64 + 80;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// lwz r10,0(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// addi r30,r11,11668
			var_r30 = (uint32_t)(ctx.r11.s64 + 11668);  // lbl_82072D94 @ 0x82072d94
			// lwz r9,44(r10)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 44);
			// stw r30,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
			// bctrl
			PPC_CALL_INDIRECT_FUNC(ctx.r9.u32);
			// lwz r31,56(r3)
			var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
			// li r5,0
			ctx.r5.s64 = 0;
			// li r4,12
			ctx.r4.s64 = 12;
			// lwz r3,4(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
			// lwz r7,4(r8)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// mr r4,r3
			ctx.r4.u64 = ctx.r3.u64;
			// cmplwi cr6,r4,0
			// beq cr6,0x823e033c
			if (ctx.r4.u32 == 0) {
				// blr
				return;
			}
			// lis r11,-32249
			// addi r3,r31,8
			ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
			// addi r11,r11,10340
			ctx.r11.s64 = ctx.r11.s64 + 10340;
			// stw r11,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
			// lwz r6,84(r1)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
			// lwz r11,88(r1)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			// rldicr r5,r6,32,63
			ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
			// stw r30,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
			// or r10,r11,r5
			ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
			// std r10,4(r4)
			PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
			// bl 0x823ec068
			snSession_AddNode_C068(ctx, base);
		}
	}
loc_823E033C:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_vfn_5"))) PPC_WEAK_FUNC(snJoinMachine_vfn_5);
PPC_FUNC_IMPL(__imp__snJoinMachine_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,48
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 48;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,72
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 72;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,192
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 192;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,216
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 216;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,240
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 240;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_vfn_6"))) PPC_WEAK_FUNC(snJoinMachine_vfn_6);
PPC_FUNC_IMPL(__imp__snJoinMachine_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,48
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 48;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e03f8
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E03F8:
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e041c
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E041C:
	// addi r3,r31,192
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 192;
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823e0440
	if (ctx.r5.u32 != 0) {
		// lwz r4,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r11,16(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E0440:
	// addi r3,r31,216
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 216;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r10,0
	// beq cr6,0x823e0464
	if (ctx.r10.u32 != 0) {
		// lwz r9,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r8,16(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E0464:
	// addi r3,r31,240
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 240;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r7,0
	// beq cr6,0x823e0488
	if (ctx.r7.u32 != 0) {
		// lwz r6,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r5,16(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E0488:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_04A0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_04A0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_04A0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// addi r30,r31,4
	var_r30 = (uint32_t)(var_r31 + 4);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r29,r30,4
	var_r29 = (uint32_t)(var_r30 + 4);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,11808
	ctx.r11.s64 = ctx.r11.s64 + 11808;
	// addi r3,r31,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
	// stw r10,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823dd440
	SinglesNetworkClient_D440_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__util_0500"))) PPC_WEAK_FUNC(util_0500);
PPC_FUNC_IMPL(__imp__util_0500) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r30,36
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 36;
	// addi r3,r31,36
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 36;
	// ld r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U64(var_r30 + 0);
	// std r11,0(r31)
	PPC_STORE_U64(var_r31 + 0, ctx.r11.u64);
	// ld r10,8(r30)
	ctx.r10.u64 = PPC_LOAD_U64(var_r30 + 8);
	// std r10,8(r31)
	PPC_STORE_U64(var_r31 + 8, ctx.r10.u64);
	// ld r9,16(r30)
	ctx.r9.u64 = PPC_LOAD_U64(var_r30 + 16);
	// std r9,16(r31)
	PPC_STORE_U64(var_r31 + 16, ctx.r9.u64);
	// ld r8,24(r30)
	ctx.r8.u64 = PPC_LOAD_U64(var_r30 + 24);
	// std r8,24(r31)
	PPC_STORE_U64(var_r31 + 24, ctx.r8.u64);
	// lbz r7,32(r30)
	ctx.r7.u64 = PPC_LOAD_U8(var_r30 + 32);
	// stb r7,32(r31)
	PPC_STORE_U8(var_r31 + 32, ctx.r7.u8);
	// bl 0x823a9b98
	SinglesNetworkClient_9B98_g_9B98_1(ctx, base);
	// lwz r6,140(r30)
	ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 140);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r6,140(r31)
	PPC_STORE_U32(var_r31 + 140, ctx.r6.u32);
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeaveSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtLeaveSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLeaveSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6544(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6544);  /* glob:lbl_825D1990 @ 0x825d1990 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeaveSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtLeaveSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLeaveSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9224
	ctx.r3.s64 = ctx.r11.s64 + 9224;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeaveFailed_vfn_1"))) PPC_WEAK_FUNC(EvtLeaveFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLeaveFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6556(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6556);  /* glob:lbl_825D199C @ 0x825d199c */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeaveFailed_vfn_2"))) PPC_WEAK_FUNC(EvtLeaveFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLeaveFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9244
	ctx.r3.s64 = ctx.r11.s64 + 9244;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalLeaveSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtLocalLeaveSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLocalLeaveSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6568(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6568);  /* glob:lbl_825D19A8 @ 0x825d19a8 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalLeaveSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtLocalLeaveSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLocalLeaveSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9260
	ctx.r3.s64 = ctx.r11.s64 + 9260;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalLeaveFailed_vfn_1"))) PPC_WEAK_FUNC(EvtLocalLeaveFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLocalLeaveFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6580(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6580);  /* glob:lbl_825D19B4 @ 0x825d19b4 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLocalLeaveFailed_vfn_2"))) PPC_WEAK_FUNC(EvtLocalLeaveFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLocalLeaveFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9284
	ctx.r3.s64 = ctx.r11.s64 + 9284;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteLeaveSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtRemoteLeaveSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRemoteLeaveSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6592(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6592);  /* glob:lbl_825D19C0 @ 0x825d19c0 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteLeaveSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtRemoteLeaveSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRemoteLeaveSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9304
	ctx.r3.s64 = ctx.r11.s64 + 9304;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteLeaveFailed_vfn_1"))) PPC_WEAK_FUNC(EvtRemoteLeaveFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRemoteLeaveFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6604(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6604);  /* glob:lbl_825D19CC @ 0x825d19cc */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteLeaveFailed_vfn_2"))) PPC_WEAK_FUNC(EvtRemoteLeaveFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRemoteLeaveFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9328
	ctx.r3.s64 = ctx.r11.s64 + 9328;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestLeaveSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtRequestLeaveSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRequestLeaveSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6616(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6616);  /* glob:lbl_825D19D8 @ 0x825d19d8 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestLeaveSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtRequestLeaveSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRequestLeaveSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9352
	ctx.r3.s64 = ctx.r11.s64 + 9352;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestLeaveFailed_vfn_1"))) PPC_WEAK_FUNC(EvtRequestLeaveFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRequestLeaveFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6628(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6628);  /* glob:lbl_825D19E4 @ 0x825d19e4 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRequestLeaveFailed_vfn_2"))) PPC_WEAK_FUNC(EvtRequestLeaveFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRequestLeaveFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9380
	ctx.r3.s64 = ctx.r11.s64 + 9380;
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptLeaveRequestSucceeded_rtti_315C_1"))) PPC_WEAK_FUNC(EvtAcceptLeaveRequestSucceeded_rtti_315C_1);
PPC_FUNC_IMPL(__imp__EvtAcceptLeaveRequestSucceeded_rtti_315C_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6640(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6640);  /* glob:lbl_825D19F0 @ 0x825d19f0 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptLeaveRequestSucceeded_rtti_315C_2"))) PPC_WEAK_FUNC(EvtAcceptLeaveRequestSucceeded_rtti_315C_2);
PPC_FUNC_IMPL(__imp__EvtAcceptLeaveRequestSucceeded_rtti_315C_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9404
	ctx.r3.s64 = ctx.r11.s64 + 9404;
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptLeaveRequestFailed_vfn_1"))) PPC_WEAK_FUNC(EvtAcceptLeaveRequestFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtAcceptLeaveRequestFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6652(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6652);  /* glob:lbl_825D19FC @ 0x825d19fc */
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcceptLeaveRequestFailed_vfn_2"))) PPC_WEAK_FUNC(EvtAcceptLeaveRequestFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtAcceptLeaveRequestFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9436
	ctx.r3.s64 = ctx.r11.s64 + 9436;
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionLeaveSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtSessionLeaveSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtSessionLeaveSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6688(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6688);  /* glob:lbl_825D1A20 @ 0x825d1a20 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionLeaveSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtSessionLeaveSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtSessionLeaveSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9464
	ctx.r3.s64 = ctx.r11.s64 + 9464;
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionLeaveFailed_vfn_1"))) PPC_WEAK_FUNC(EvtSessionLeaveFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtSessionLeaveFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6700(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6700);  /* glob:lbl_825D1A2C @ 0x825d1a2c */
	// blr
	return;
}

__attribute__((alias("__imp__EvtSessionLeaveFailed_vfn_2"))) PPC_WEAK_FUNC(EvtSessionLeaveFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtSessionLeaveFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9492
	ctx.r3.s64 = ctx.r11.s64 + 9492;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeave_vfn_1"))) PPC_WEAK_FUNC(EvtLeave_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLeave_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6712(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6712);  /* glob:lbl_825D1A38 @ 0x825d1a38 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeave_vfn_2"))) PPC_WEAK_FUNC(EvtLeave_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLeave_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9516
	ctx.r3.s64 = ctx.r11.s64 + 9516;
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteLeave_vfn_1"))) PPC_WEAK_FUNC(EvtRemoteLeave_vfn_1);
PPC_FUNC_IMPL(__imp__EvtRemoteLeave_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6724(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6724);  /* glob:lbl_825D1A44 @ 0x825d1a44 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtRemoteLeave_vfn_2"))) PPC_WEAK_FUNC(EvtRemoteLeave_vfn_2);
PPC_FUNC_IMPL(__imp__EvtRemoteLeave_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9528
	ctx.r3.s64 = ctx.r11.s64 + 9528;
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeaveRequest_vfn_1"))) PPC_WEAK_FUNC(EvtLeaveRequest_vfn_1);
PPC_FUNC_IMPL(__imp__EvtLeaveRequest_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6736(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6736);  /* glob:lbl_825D1A50 @ 0x825d1a50 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtLeaveRequest_vfn_2"))) PPC_WEAK_FUNC(EvtLeaveRequest_vfn_2);
PPC_FUNC_IMPL(__imp__EvtLeaveRequest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9544
	ctx.r3.s64 = ctx.r11.s64 + 9544;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_0758_w"))) PPC_WEAK_FUNC(snSession_0758_w);
PPC_FUNC_IMPL(__imp__snSession_0758_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r5,18
	// bne cr6,0x823e07f0
	if (ctx.r5.s32 == 18) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,12676
		var_r30 = (uint32_t)(ctx.r11.s64 + 12676);  // lbl_82073184 @ 0x82073184
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e0878
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// b 0x823e086c
	} else {
	loc_823E07F0:
		// cmpwi cr6,r5,19
		// bne cr6,0x823e0878
		if (ctx.r5.s32 != 19) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,12696
		var_r30 = (uint32_t)(ctx.r11.s64 + 12696);  // lbl_82073198 @ 0x82073198
		// lwz r8,44(r9)
  // [ph4a] slot load collapsed
		// stw r30,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r6,4(r7)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e0878
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,100(r1)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
		// lwz r10,104(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
		// rldicr r11,r5,32,63
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u64, 32);
		// or r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 | ctx.r11.u64;
		// std r9,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r9.u64);
	}
loc_823E086C:
	// addi r3,r31,8
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
	// stw r30,0(r4)
	PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
	// bl 0x823ec068
	snSession_AddNode_C068(ctx, base);
loc_823E0878:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_vfn_2"))) PPC_WEAK_FUNC(snLeaveMachine_vfn_2);
PPC_FUNC_IMPL(__imp__snLeaveMachine_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9560
	ctx.r3.s64 = ctx.r11.s64 + 9560;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_vfn_14"))) PPC_WEAK_FUNC(snLeaveMachine_vfn_14);
PPC_FUNC_IMPL(__imp__snLeaveMachine_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32193
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,-20224
	ctx.r9.s64 = ctx.r11.s64 + -20224;
	// addi r4,r3,24
	ctx.r4.s64 = ctx.r3.s64 + 24;
	// stw r10,216(r3)
	PPC_STORE_U32(ctx.r3.u32 + 216, ctx.r10.u32);
	// stw r9,32(r3)
	PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r9.u32);
	// stw r3,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r3.u32);
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// b 0x823dd170
	util_D170(ctx, base);
	return;
}

__attribute__((alias("__imp__snLeaveMachine_vfn_12"))) PPC_WEAK_FUNC(snLeaveMachine_vfn_12);
PPC_FUNC_IMPL(__imp__snLeaveMachine_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=368, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6568(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6568);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e0948
	if (ctx.r3.s32 == ctx.r11.s32) {
		// bl 0x823f15c8
		snSession_15C8_g(ctx, base);
		// addi r3,r1,168
		ctx.r3.s64 = ctx.r1.s64 + 168;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,168
		ctx.r4.s64 = ctx.r1.s64 + 168;
		// addi r11,r11,12476
		ctx.r11.s64 = ctx.r11.s64 + 12476;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,168(r1)
		PPC_STORE_U32(ctx.r1.u32 + 168, ctx.r11.u32);
		// bl 0x823e9b18
		snSession_9B18_fw(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6580(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6580);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e09ac
	if (ctx.r3.s32 == ctx.r11.s32) {
		// bl 0x823f15c8
		snSession_15C8_g(ctx, base);
		// addi r3,r1,136
		ctx.r3.s64 = ctx.r1.s64 + 136;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,136
		ctx.r4.s64 = ctx.r1.s64 + 136;
		// addi r11,r11,12496
		ctx.r11.s64 = ctx.r11.s64 + 12496;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,136(r1)
		PPC_STORE_U32(ctx.r1.u32 + 136, ctx.r11.u32);
		// bl 0x823e9bc0
		snLeaveMachine_9BC0(ctx, base);
		return;
	}
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6592(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6592);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e0a10
	if (ctx.r3.s32 == ctx.r11.s32) {
		// bl 0x823f1728
		snSession_1728_g(ctx, base);
		// addi r3,r1,200
		ctx.r3.s64 = ctx.r1.s64 + 200;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,200
		ctx.r4.s64 = ctx.r1.s64 + 200;
		// addi r11,r11,12476
		ctx.r11.s64 = ctx.r11.s64 + 12476;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,200(r1)
		PPC_STORE_U32(ctx.r1.u32 + 200, ctx.r11.u32);
		// bl 0x823e9b18
		snSession_9B18_fw(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6604(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6604);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e0a74
	if (ctx.r3.s32 == ctx.r11.s32) {
		// bl 0x823f1728
		snSession_1728_g(ctx, base);
		// addi r3,r1,104
		ctx.r3.s64 = ctx.r1.s64 + 104;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,104
		ctx.r4.s64 = ctx.r1.s64 + 104;
		// addi r11,r11,12496
		ctx.r11.s64 = ctx.r11.s64 + 12496;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,104(r1)
		PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r11.u32);
		// bl 0x823e9bc0
		snLeaveMachine_9BC0(ctx, base);
		return;
	}
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6616(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6616);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e0b3c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,20(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r30,216(r6)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 216));
		// addi r4,r30,96
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 96;
		// bl 0x823e81c0
		snLeaveMachine_81C0_g(ctx, base);
		// lis r11,-32249
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r6,r1,96
		ctx.r6.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,17512
		ctx.r11.s64 = ctx.r11.s64 + 17512;
		// stw r30,100(r1)
		PPC_STORE_U32(ctx.r1.u32 + 100, var_r30);
		// li r5,8
		ctx.r5.s64 = 8;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lis r11,-32249
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r6,r1,88
		ctx.r6.s64 = ctx.r1.s64 + 88;
		// stw r30,92(r1)
		PPC_STORE_U32(ctx.r1.u32 + 92, var_r30);
		// addi r11,r11,17524
		ctx.r11.s64 = ctx.r11.s64 + 17524;
		// li r5,11
		ctx.r5.s64 = 11;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// stw r11,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lis r11,-32249
		// addi r3,r1,216
		ctx.r3.s64 = ctx.r1.s64 + 216;
		// addi r11,r11,10780
		ctx.r11.s64 = ctx.r11.s64 + 10780;
		// stw r11,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,216
		ctx.r4.s64 = ctx.r1.s64 + 216;
		// addi r11,r11,12476
		ctx.r11.s64 = ctx.r11.s64 + 12476;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,216(r1)
		PPC_STORE_U32(ctx.r1.u32 + 216, ctx.r11.u32);
		// bl 0x823e9b18
		snSession_9B18_fw(ctx, base);
		return;
	}
loc_823E0B3C:
	// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r4,40(r5)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6628(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6628);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e0bdc
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lis r11,-32249
		// lwz r9,20(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
		// li r8,0
		ctx.r8.s64 = 0;
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r11,r11,17512
		ctx.r11.s64 = ctx.r11.s64 + 17512;
		// addi r6,r1,80
		ctx.r6.s64 = ctx.r1.s64 + 80;
		// li r5,9
		ctx.r5.s64 = 9;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// stw r8,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r8.u32);
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// lwz r7,216(r9)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 216);
		// stw r7,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r7.u32);
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lis r11,-32249
		// addi r3,r1,120
		ctx.r3.s64 = ctx.r1.s64 + 120;
		// addi r11,r11,10780
		ctx.r11.s64 = ctx.r11.s64 + 10780;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,120
		ctx.r4.s64 = ctx.r1.s64 + 120;
		// addi r11,r11,12496
		ctx.r11.s64 = ctx.r11.s64 + 12496;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,120(r1)
		PPC_STORE_U32(ctx.r1.u32 + 120, ctx.r11.u32);
		// bl 0x823e9bc0
		snLeaveMachine_9BC0(ctx, base);
		return;
	}
loc_823E0BDC:
	// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r5,40(r6)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6640(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6640);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e0c44
	if (ctx.r3.s32 == ctx.r11.s32) {
		// bl 0x823f1ab8
		snSession_1AB8_g(ctx, base);
		// addi r3,r1,152
		ctx.r3.s64 = ctx.r1.s64 + 152;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,152
		ctx.r4.s64 = ctx.r1.s64 + 152;
		// addi r11,r11,12476
		ctx.r11.s64 = ctx.r11.s64 + 12476;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,152(r1)
		PPC_STORE_U32(ctx.r1.u32 + 152, ctx.r11.u32);
		// bl 0x823e9b18
		snSession_9B18_fw(ctx, base);
		return;
	}
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6652(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6652);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e0d30
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,20(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r30,216(r6)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 216));
		// cmplwi cr6,r30,0
		// beq cr6,0x823e0d08
		if (var_r30 != 0) {
			// lwz r5,16(r31)
			ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
			// addi r29,r30,40
			var_r29 = (uint32_t)(var_r30 + 40);
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// lwz r3,164(r5)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 164);
			// bl 0x823daaf0
			SinglesNetworkClient_AAF0_g(ctx, base);
			// clrlwi r5,r3,24
			ctx.r5.u64 = ctx.r3.u32 & 0xFF;
			// cmplwi cr6,r5,0
			// beq cr6,0x823e0d08
			if (ctx.r5.u32 == 0) goto loc_823E0D08;
			// lwz r10,16(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
			// addi r11,r1,240
			ctx.r11.s64 = ctx.r1.s64 + 240;
			// ld r9,96(r30)
			ctx.r9.u64 = PPC_LOAD_U64(var_r30 + 96);
			// li r5,0
			ctx.r5.s64 = 0;
			// ld r8,104(r30)
			ctx.r8.u64 = PPC_LOAD_U64(var_r30 + 104);
			// addi r4,r1,240
			ctx.r4.s64 = ctx.r1.s64 + 240;
			// ld r7,112(r30)
			ctx.r7.u64 = PPC_LOAD_U64(var_r30 + 112);
			// addi r3,r1,288
			ctx.r3.s64 = ctx.r1.s64 + 288;
			// ld r6,168(r10)
			ctx.r6.u64 = PPC_LOAD_U64(ctx.r10.u32 + 168);
			// std r9,0(r11)
			PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r9.u64);
			// std r8,8(r11)
			PPC_STORE_U64(ctx.r11.u32 + 8, ctx.r8.u64);
			// std r7,16(r11)
			PPC_STORE_U64(ctx.r11.u32 + 16, ctx.r7.u64);
			// stb r5,272(r1)
			PPC_STORE_U8(ctx.r1.u32 + 272, ctx.r5.u8);
			// std r6,264(r1)
			PPC_STORE_U64(ctx.r1.u32 + 264, ctx.r6.u64);
			// bl 0x823f2080
			SinglesNetworkClient_2080_g(ctx, base);
			// mr r5,r3
			ctx.r5.u64 = ctx.r3.u64;
			// lwz r3,16(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
			// li r7,0
			ctx.r7.s64 = 0;
			// li r6,1
			ctx.r6.s64 = 1;
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// lwz r3,164(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 164);
			// bl 0x823dae48
			SinglesNetworkClient_AE48_g(ctx, base);
			// addi r3,r1,288
			ctx.r3.s64 = ctx.r1.s64 + 288;
			// bl 0x82260268
			SinglesNetworkClient_0268_g(ctx, base);
		}
	loc_823E0D08:
		// addi r3,r1,184
		ctx.r3.s64 = ctx.r1.s64 + 184;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,184
		ctx.r4.s64 = ctx.r1.s64 + 184;
		// addi r11,r11,12496
		ctx.r11.s64 = ctx.r11.s64 + 12496;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,184(r1)
		PPC_STORE_U32(ctx.r1.u32 + 184, ctx.r11.u32);
		// bl 0x823e9bc0
		snLeaveMachine_9BC0(ctx, base);
		return;
	}
loc_823E0D30:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_2"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmLocalLeavePending_vfn_2);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9576
	ctx.r3.s64 = ctx.r11.s64 + 9576;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_14"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmLocalLeavePending_vfn_14);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r31,20(r30)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 20));
	// lwz r10,16(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r9,1992(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1992);
	// cmplwi cr6,r9,0
	// li r9,1
	ctx.r9.s64 = 1;
	// bne cr6,0x823e0d88
	if (ctx.r9.u32 == 0) {
		// li r9,0
		ctx.r9.s64 = 0;
	}
loc_823E0D88:
	// clrlwi r7,r9,24
	ctx.r7.u64 = ctx.r9.u32 & 0xFF;
	// cmplwi cr6,r7,0
	// beq cr6,0x823e0de8
	if (ctx.r7.u32 != 0) {
		// lbz r6,3744(r10)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r10.u32 + 3744);
		// rlwinm r5,r6,0,0,24
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e0de8
		if (ctx.r5.u32 == 0) goto loc_823E0DE8;
		// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r11,40(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lis r11,-32162
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// addi r6,r11,18144
		ctx.r6.s64 = ctx.r11.s64 + 18144;
		// lis r11,-32162
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r5,r11,21576
		ctx.r5.s64 = ctx.r11.s64 + 21576;
		// bl 0x82430978
		SinglesNetworkClient_0978_g(ctx, base);
		// lwz r10,20(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 20);
		// lwz r9,12(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// li r11,1
		ctx.r11.s64 = 1;
		// stw r9,216(r10)
		PPC_STORE_U32(ctx.r10.u32 + 216, ctx.r9.u32);
	}
loc_823E0DE8:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r8,0
	// beq cr6,0x823e0e08
	if (ctx.r8.u32 != 0) {
		// lwz r3,20(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
		// bl 0x823f1550
		util_1550(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// bne cr6,0x823e0e28
		if (ctx.r7.u32 != 0) {
			// blr
			return;
		}
	}
loc_823E0E08:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,12536
	ctx.r11.s64 = ctx.r11.s64 + 12536;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e9c68
	snHsmLocalLeavePending_9C68(ctx, base);
loc_823E0E28:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_12"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmLocalLeavePending_vfn_12);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine_snHsmLocalLeavePending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6688(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6688);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e0f1c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,12516
		var_r30 = (uint32_t)(ctx.r11.s64 + 12516);  // lbl_820730E4 @ 0x820730e4
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e0f80
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e0f80
	} else {
	loc_823E0F1C:
		// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6700(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6700);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e0f78
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,12536
			ctx.r11.s64 = ctx.r11.s64 + 12536;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e9c68
			snHsmLocalLeavePending_9C68(ctx, base);
			// b 0x823e0f80
		} else {
		loc_823E0F78:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823E0F80:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_vfn_2"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_vfn_2);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9596
	ctx.r3.s64 = ctx.r11.s64 + 9596;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_14"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_14);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32193
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,8528
	ctx.r11.s64 = ctx.r11.s64 + 8528;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// lwz r10,16(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r3,164(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f17c0
	SinglesNetworkClient_17C0_g(ctx, base);
	// clrlwi r9,r3,24
	ctx.r9.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// bne cr6,0x823e1010
	if (ctx.r9.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,12616
		ctx.r11.s64 = ctx.r11.s64 + 12616;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9d10
		snHsmWaitingForReply_9D10(ctx, base);
	}
loc_823E1010:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_12"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_12);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6664(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6664);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e10bc
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,144
		var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // addr:0x825d0090
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6676(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6676);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e1118
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,12616
		ctx.r11.s64 = ctx.r11.s64 + 12616;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9d10
		snHsmWaitingForReply_9D10(ctx, base);
		return;
	}
loc_823E1118:
	// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r7,40(r8)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r5,4(r6)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823e1170
	if (ctx.r3.s32 == 0) {
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,12616
		ctx.r11.s64 = ctx.r11.s64 + 12616;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823e9d10
		snHsmWaitingForReply_9D10(ctx, base);
		return;
	}
loc_823E1170:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_2"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_2);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9612
	ctx.r3.s64 = ctx.r11.s64 + 9612;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_14"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_14);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f1550
	util_1550(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823e11d8
	if (ctx.r11.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,12616
		ctx.r11.s64 = ctx.r11.s64 + 12616;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9d10
		snHsmWaitingForReply_9D10(ctx, base);
	}
loc_823E11D8:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_12"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_12);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine_snHsmRequestingLeave_snHsmLeavePending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6688(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6688);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e12cc
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,12596
		var_r30 = (uint32_t)(ctx.r11.s64 + 12596);  // lbl_82073134 @ 0x82073134
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e1330
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e1330
	} else {
	loc_823E12CC:
		// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6700(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6700);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e1328
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,12616
			ctx.r11.s64 = ctx.r11.s64 + 12616;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e9d10
			snHsmWaitingForReply_9D10(ctx, base);
			// b 0x823e1330
		} else {
		loc_823E1328:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823E1330:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRemoteLeavePending_vfn_2"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRemoteLeavePending_vfn_2);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRemoteLeavePending_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9628
	ctx.r3.s64 = ctx.r11.s64 + 9628;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRemoteLeavePending_vfn_14"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRemoteLeavePending_vfn_14);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRemoteLeavePending_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f1660
	SinglesNetworkClient_1660_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e1394
	if (ctx.r11.u32 != 0) {
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x823f1550
		util_1550(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// bne cr6,0x823e13b4
		if (ctx.r10.u32 != 0) {
			// blr
			return;
		}
	}
loc_823E1394:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,12576
	ctx.r11.s64 = ctx.r11.s64 + 12576;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e9db8
	snHsmRemoteLeavePending_9DB8(ctx, base);
loc_823E13B4:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRemoteLeavePending_vfn_12"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRemoteLeavePending_vfn_12);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRemoteLeavePending_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine_snHsmRemoteLeavePending::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6688(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6688);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e14a4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,12556
		var_r30 = (uint32_t)(ctx.r11.s64 + 12556);  // lbl_8207310C @ 0x8207310c
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e1508
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e1508
	} else {
	loc_823E14A4:
		// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6700(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6700);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e1500
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,12576
			ctx.r11.s64 = ctx.r11.s64 + 12576;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e9db8
			snHsmRemoteLeavePending_9DB8(ctx, base);
			// b 0x823e1508
		} else {
		loc_823E1500:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823E1508:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_2"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_2);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9648
	ctx.r3.s64 = ctx.r11.s64 + 9648;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_14"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_14);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// bl 0x823f19e0
	SinglesNetworkClient_19E0_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e156c
	if (ctx.r11.u32 != 0) {
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x823f1550
		util_1550(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// bne cr6,0x823e158c
		if (ctx.r10.u32 != 0) {
			// blr
			return;
		}
	}
loc_823E156C:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r11,r11,12656
	ctx.r11.s64 = ctx.r11.s64 + 12656;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x823e9e60
	snHsmAcceptingLeaveRequest_9E60(ctx, base);
loc_823E158C:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_12"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_12);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmAcceptingLeaveRequest_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snLeaveMachine_snHsmAcceptingLeaveRequest::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6688(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6688);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e167c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,12636
		var_r30 = (uint32_t)(ctx.r11.s64 + 12636);  // lbl_8207315C @ 0x8207315c
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e16e0
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e16e0
	} else {
	loc_823E167C:
		// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r4,40(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6700(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6700);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e16d8
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,12656
			ctx.r11.s64 = ctx.r11.s64 + 12656;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e9e60
			snHsmAcceptingLeaveRequest_9E60(ctx, base);
			// b 0x823e16e0
		} else {
		loc_823E16D8:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823E16E0:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_vfn_5"))) PPC_WEAK_FUNC(snLeaveMachine_vfn_5);
PPC_FUNC_IMPL(__imp__snLeaveMachine_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,48
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 48;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,72
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 72;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,168
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 168;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,192
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 192;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_vfn_6"))) PPC_WEAK_FUNC(snLeaveMachine_vfn_6);
PPC_FUNC_IMPL(__imp__snLeaveMachine_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,48
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 48;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e1790
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E1790:
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e17b4
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E17B4:
	// addi r3,r31,168
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 168;
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823e17d8
	if (ctx.r5.u32 != 0) {
		// lwz r4,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r11,16(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E17D8:
	// addi r3,r31,192
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 192;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r10,0
	// beq cr6,0x823e17fc
	if (ctx.r10.u32 != 0) {
		// lwz r9,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r8,16(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E17FC:
	// blr
	return;
}

__attribute__((alias("__imp__EvtArbGuestRegister_vfn_1"))) PPC_WEAK_FUNC(EvtArbGuestRegister_vfn_1);
PPC_FUNC_IMPL(__imp__EvtArbGuestRegister_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6952(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6952);  /* glob:lbl_825D1B28 @ 0x825d1b28 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtArbGuestRegister_vfn_2"))) PPC_WEAK_FUNC(EvtArbGuestRegister_vfn_2);
PPC_FUNC_IMPL(__imp__EvtArbGuestRegister_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9672
	ctx.r3.s64 = ctx.r11.s64 + 9672;
	// blr
	return;
}

__attribute__((alias("__imp__snArbMachine_vfn_2"))) PPC_WEAK_FUNC(snArbMachine_vfn_2);
PPC_FUNC_IMPL(__imp__snArbMachine_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9692
	ctx.r3.s64 = ctx.r11.s64 + 9692;
	// blr
	return;
}

__attribute__((alias("__imp__EvtMigrateSession_vfn_1"))) PPC_WEAK_FUNC(EvtMigrateSession_vfn_1);
PPC_FUNC_IMPL(__imp__EvtMigrateSession_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6844(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6844);  /* glob:lbl_825D1ABC @ 0x825d1abc */
	// blr
	return;
}

__attribute__((alias("__imp__EvtMigrateSession_vfn_2"))) PPC_WEAK_FUNC(EvtMigrateSession_vfn_2);
PPC_FUNC_IMPL(__imp__EvtMigrateSession_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9704
	ctx.r3.s64 = ctx.r11.s64 + 9704;
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcquireHostSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtAcquireHostSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtAcquireHostSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6892(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6892);  /* glob:lbl_825D1AEC @ 0x825d1aec */
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcquireHostSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtAcquireHostSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtAcquireHostSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9724
	ctx.r3.s64 = ctx.r11.s64 + 9724;
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcquireHostFailed_vfn_1"))) PPC_WEAK_FUNC(EvtAcquireHostFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtAcquireHostFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6904(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6904);  /* glob:lbl_825D1AF8 @ 0x825d1af8 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtAcquireHostFailed_vfn_2"))) PPC_WEAK_FUNC(EvtAcquireHostFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtAcquireHostFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9748
	ctx.r3.s64 = ctx.r11.s64 + 9748;
	// blr
	return;
}

__attribute__((alias("__imp__EvtMigrateAsGuestSucceeded_vfn_1"))) PPC_WEAK_FUNC(EvtMigrateAsGuestSucceeded_vfn_1);
PPC_FUNC_IMPL(__imp__EvtMigrateAsGuestSucceeded_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6916(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6916);  /* glob:lbl_825D1B04 @ 0x825d1b04 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtMigrateAsGuestSucceeded_vfn_2"))) PPC_WEAK_FUNC(EvtMigrateAsGuestSucceeded_vfn_2);
PPC_FUNC_IMPL(__imp__EvtMigrateAsGuestSucceeded_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9772
	ctx.r3.s64 = ctx.r11.s64 + 9772;
	// blr
	return;
}

__attribute__((alias("__imp__EvtMigrateAsGuestFailed_vfn_1"))) PPC_WEAK_FUNC(EvtMigrateAsGuestFailed_vfn_1);
PPC_FUNC_IMPL(__imp__EvtMigrateAsGuestFailed_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6928(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6928);  /* glob:lbl_825D1B10 @ 0x825d1b10 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtMigrateAsGuestFailed_vfn_2"))) PPC_WEAK_FUNC(EvtMigrateAsGuestFailed_vfn_2);
PPC_FUNC_IMPL(__imp__EvtMigrateAsGuestFailed_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9800
	ctx.r3.s64 = ctx.r11.s64 + 9800;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9824
	ctx.r3.s64 = ctx.r11.s64 + 9824;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_vfn_13"))) PPC_WEAK_FUNC(snMigrateMachine_vfn_13);
PPC_FUNC_IMPL(__imp__snMigrateMachine_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,17392
	ctx.r6.s64 = ctx.r11.s64 + 17392;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// addi r10,r31,48
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 48;
	// li r9,9
	ctx.r9.s64 = 9;
	// addi r11,r3,12
	ctx.r11.s64 = ctx.r3.s64 + 12;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823E1948:
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x823e1948
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E1948;
	// lwz r8,20(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 20);
	// addi r10,r31,58
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 58;
	// lwz r7,16(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
	// lwz r11,3784(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 3784);
	// addi r11,r11,50
	ctx.r11.s64 = ctx.r11.s64 + 50;
	// addi r8,r11,6
	ctx.r8.s64 = ctx.r11.s64 + 6;
loc_823E1974:
	// lbz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r5,0(r10)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// subf. r9,r5,r6
	ctx.r9.s64 = ctx.r6.s64 - ctx.r5.s64;
	// bne 0x823e1994
	if (ctx.r9.s32 != 0) goto loc_823E1994;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r11,r8
	// bne cr6,0x823e1974
	if (ctx.r11.s32 != ctx.r8.s32) goto loc_823E1974;
loc_823E1994:
	// cmpwi cr6,r9,0
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x823e19a4
	if (ctx.r9.s32 != 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E19A4:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r3,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// beq cr6,0x823e19cc
	if (ctx.r3.u32 != 0) {
		// lwz r10,44(r11)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snMigrateMachine::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,84
		var_r29 = (uint32_t)(var_r31 + 84);
		// b 0x823e19e0
	} else {
		// lwz r8,44(r9)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snMigrateMachine::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,1340
		var_r29 = (uint32_t)(var_r31 + 1340);
	}
loc_823E19E0:
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x823ed988
	util_D988(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_vfn_12"))) PPC_WEAK_FUNC(snMigrateMachine_vfn_12);
PPC_FUNC_IMPL(__imp__snMigrateMachine_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=176, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6892(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6892);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e1a8c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r3,20(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
		// bl 0x823f2720
		game_2720(ctx, base);
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10440
		ctx.r11.s64 = ctx.r11.s64 + 10440;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823e9f08
		snSession_9F08_fw(ctx, base);
		// b 0x823e1be0
	} else {
	loc_823E1A8C:
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,40(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r11,4(r4)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6904(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6904);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e1b00
		if (ctx.r3.s32 == ctx.r11.s32) {
			// lwz r10,20(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 20);
			// li r6,0
			ctx.r6.s64 = 0;
			// li r5,23
			ctx.r5.s64 = 23;
			// lwz r4,16(r10)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
			// addi r3,r4,108
			ctx.r3.s64 = ctx.r4.s64 + 108;
			// bl 0x82371bf8
			snSession_1BF8_g(ctx, base);
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,10460
			ctx.r11.s64 = ctx.r11.s64 + 10460;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823e9fb0
			snSession_9FB0_fw(ctx, base);
			// b 0x823e1be0
		} else {
		loc_823E1B00:
			// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// lwz r8,40(r9)
  // [ph4a] slot load collapsed
			// bctrl
			VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
			// lwz r3,12(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
			// lwz r6,4(r7)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// lis r11,-32163
			ctx.r11.s64 = -2107834368;
			// lwz r11,6916(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6916);
			// cmpw cr6,r3,r11
			// bne cr6,0x823e1b64
			if (ctx.r3.s32 == ctx.r11.s32) {
				// lwz r3,20(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
				// bl 0x823f2720
				game_2720(ctx, base);
				// addi r3,r1,112
				ctx.r3.s64 = ctx.r1.s64 + 112;
				// bl 0x823dda08
				util_DA08(ctx, base);
				// lis r11,-32249
				// addi r4,r1,112
				ctx.r4.s64 = ctx.r1.s64 + 112;
				// addi r11,r11,10440
				ctx.r11.s64 = ctx.r11.s64 + 10440;
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// stw r11,112(r1)
				PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
				// bl 0x823e9f08
				snSession_9F08_fw(ctx, base);
				// b 0x823e1be0
			} else {
			loc_823E1B64:
				// lwz r5,0(r31)
  // [ph4a] vtable load collapsed
				// mr r3,r31
				ctx.r3.u64 = var_r31;
				// lwz r4,40(r5)
  // [ph4a] slot load collapsed
				// bctrl
				VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
				// lwz r3,12(r3)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
				// lwz r10,4(r11)
				// bctrl
				VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
				// lis r11,-32163
				ctx.r11.s64 = -2107834368;
				// lwz r11,6928(r11)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6928);
				// cmpw cr6,r3,r11
				// bne cr6,0x823e1bd8
				if (ctx.r3.s32 == ctx.r11.s32) {
					// lwz r9,20(r31)
					ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
					// li r6,0
					ctx.r6.s64 = 0;
					// li r5,23
					ctx.r5.s64 = 23;
					// lwz r4,16(r9)
					ctx.r4.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
					// addi r3,r4,108
					ctx.r3.s64 = ctx.r4.s64 + 108;
					// bl 0x82371bf8
					snSession_1BF8_g(ctx, base);
					// addi r3,r1,128
					ctx.r3.s64 = ctx.r1.s64 + 128;
					// bl 0x823dda08
					util_DA08(ctx, base);
					// lis r11,-32249
					// addi r4,r1,128
					ctx.r4.s64 = ctx.r1.s64 + 128;
					// addi r11,r11,10460
					ctx.r11.s64 = ctx.r11.s64 + 10460;
					// mr r3,r31
					ctx.r3.u64 = var_r31;
					// stw r11,128(r1)
					PPC_STORE_U32(ctx.r1.u32 + 128, ctx.r11.u32);
					// bl 0x823e9fb0
					snSession_9FB0_fw(ctx, base);
					// b 0x823e1be0
				} else {
				loc_823E1BD8:
					// li r8,0
					ctx.r8.s64 = 0;
					// stb r8,0(r30)
					PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
				}
			}
		}
	}
loc_823E1BE0:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9840
	ctx.r3.s64 = ctx.r11.s64 + 9840;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_13"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_13);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,44(r11)
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // snMigrateMachine_snAcquiringHost::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
	// addi r29,r31,1184
	var_r29 = (uint32_t)(var_r31 + 1184);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x823ed988
	util_D988(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_14"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_14);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// b 0x823f2828
	SinglesNetworkClient_2828_p45(ctx, base);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_15"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_15);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_15) {
	PPC_FUNC_PROLOGUE();
	// b 0x823f2a08
	SinglesNetworkClient_2A08_w(ctx, base);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snMigrating_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9856
	ctx.r3.s64 = ctx.r11.s64 + 9856;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_14"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snMigrating_vfn_14);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236e430
	xam_E430_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823e1cd0
	if (ctx.r11.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,13420
		ctx.r11.s64 = ctx.r11.s64 + 13420;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823ea058
		snMigrating_A058(ctx, base);
	}
loc_823E1CD0:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_12"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snMigrating_vfn_12);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine_snAcquiringHost_snMigrating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6868(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6868);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e1d7c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snMigrateMachine_snAcquiringHost_snMigrating::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,1292
		var_r29 = (uint32_t)(ctx.r11.s64 + 1292);  // addr:0x825d050c
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine_snAcquiringHost_snMigrating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6880(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6880);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e1dd8
	if (ctx.r3.s32 == ctx.r11.s32) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,13420
		ctx.r11.s64 = ctx.r11.s64 + 13420;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823ea058
		snMigrating_A058(ctx, base);
		return;
	}
loc_823E1DD8:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9868
	ctx.r3.s64 = ctx.r11.s64 + 9868;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_14"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_14);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32193
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,12696
	ctx.r11.s64 = ctx.r11.s64 + 12696;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// lwz r10,16(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r3,164(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// lwz r10,1264(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1264);
	// addi r9,r10,1
	ctx.r9.s64 = ctx.r10.s64 + 1;
	// cmpwi cr6,r10,10
	// stw r9,1264(r11)
	PPC_STORE_U32(ctx.r11.u32 + 1264, ctx.r9.u32);
	// blt cr6,0x823e1e78
	if (ctx.r10.s32 >= 10) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,13400
		ctx.r11.s64 = ctx.r11.s64 + 13400;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823ea100
		SinglesNetworkClient_A100_g(ctx, base);
		// blr
		return;
	}
loc_823E1E78:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823f2af0
	SinglesNetworkClient_2AF0_g(ctx, base);
	// li r4,500
	ctx.r4.s64 = 500;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ed4f8
	util_D4F8(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_12"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_12);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine_snAcquiringHost_snNotifyingGuests::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823e1f2c
	if (ctx.r3.s32 == 0) {
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,1292
		var_r29 = (uint32_t)(ctx.r11.s64 + 1292);  // addr:0x8207050c
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E1F2C:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_5"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_5);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,1184
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 1184;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,1208
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 1208;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_6"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_6);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,1184
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1184;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e1fc0
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E1FC0:
	// addi r3,r31,1208
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1208;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e1fe4
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E1FE4:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9884
	ctx.r3.s64 = ctx.r11.s64 + 9884;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9904
	ctx.r3.s64 = ctx.r11.s64 + 9904;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_14"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_14);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// mr r8,r3
	ctx.r8.u64 = ctx.r3.u64;
	// lis r11,-32193
	// addi r4,r8,24
	ctx.r4.s64 = ctx.r8.s64 + 24;
	// addi r11,r11,12704
	ctx.r11.s64 = ctx.r11.s64 + 12704;
	// lbz r10,48(r8)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r8.u32 + 48);
	// clrlwi r9,r10,25
	ctx.r9.u64 = ctx.r10.u32 & 0x7F;
	// stb r9,48(r8)
	PPC_STORE_U8(ctx.r8.u32 + 48, ctx.r9.u8);
	// stw r11,32(r8)
	PPC_STORE_U32(ctx.r8.u32 + 32, ctx.r11.u32);
	// stw r8,28(r8)
	PPC_STORE_U32(ctx.r8.u32 + 28, ctx.r8.u32);
	// lwz r7,16(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 16);
	// lwz r3,164(r7)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// li r4,10000
	ctx.r4.s64 = 10000;
	// mr r3,r8
	ctx.r3.u64 = ctx.r8.u64;
	// bl 0x823ed4f8
	util_D4F8(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_12"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_12);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine_snMigratingAsGuest_snWaitingForMigrateMsg::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823e20e8
	if (ctx.r3.s32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,13460
		ctx.r11.s64 = ctx.r11.s64 + 13460;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823ea1a8
		snMigrating_A1A8(ctx, base);
		return;
	}
loc_823E20E8:
	// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r5,40(r6)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6856(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6856);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e2164
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,1416
		var_r29 = (uint32_t)(ctx.r11.s64 + 1416);  // addr:0x825d0588
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E2164:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_2"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_2);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9928
	ctx.r3.s64 = ctx.r11.s64 + 9928;
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_14"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_14);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine_snMigratingAsGuest_snMigrating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,21740
	ctx.r6.s64 = ctx.r11.s64 + 21740;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// ld r5,24(r3)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r3.u32 + 24);
	// addi r4,r3,32
	ctx.r4.s64 = ctx.r3.s64 + 32;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236e430
	xam_E430_g(ctx, base);
	// clrlwi r9,r3,24
	ctx.r9.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// bne cr6,0x823e2210
	if (ctx.r9.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,13460
		ctx.r11.s64 = ctx.r11.s64 + 13460;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823ea1a8
		snMigrating_A1A8(ctx, base);
	}
loc_823E2210:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_12"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_12);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_snMigrating_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snMigrateMachine_snMigratingAsGuest_snMigrating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6868(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6868);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e230c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// bl 0x823f2fd0
		SinglesNetworkClient_2FD0_g(ctx, base);
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// addi r30,r11,13440
		var_r30 = (uint32_t)(ctx.r11.s64 + 13440);  // lbl_82073480 @ 0x82073480
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r31,56(r3)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e236c
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e236c
	} else {
		// lwz r4,40(r5)
		// bctrl
		VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r10,4(r11)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6880(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6880);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e2364
		if (ctx.r3.s32 == ctx.r11.s32) {
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,13460
			ctx.r11.s64 = ctx.r11.s64 + 13460;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823ea1a8
			snMigrating_A1A8(ctx, base);
			// b 0x823e236c
		} else {
		loc_823E2364:
			// li r9,0
			ctx.r9.s64 = 0;
			// stb r9,0(r30)
			PPC_STORE_U8(var_r30 + 0, ctx.r9.u8);
		}
	}
loc_823E236C:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_vfn_5"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_vfn_5);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,76
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 76;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_vfn_6"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_vfn_6);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e2408
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E2408:
	// addi r3,r31,76
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 76;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e242c
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E242C:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_vfn_5"))) PPC_WEAK_FUNC(snMigrateMachine_vfn_5);
PPC_FUNC_IMPL(__imp__snMigrateMachine_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,84
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 84;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,1340
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 1340;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_vfn_6"))) PPC_WEAK_FUNC(snMigrateMachine_vfn_6);
PPC_FUNC_IMPL(__imp__snMigrateMachine_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,84
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 84;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e24c8
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E24C8:
	// addi r3,r31,1340
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1340;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e24ec
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E24EC:
	// blr
	return;
}

__attribute__((alias("__imp__util_2508"))) PPC_WEAK_FUNC(util_2508);
PPC_FUNC_IMPL(__imp__util_2508) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,1992(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 1992);
	// cmplwi cr6,r11,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823e251c
	if (ctx.r11.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E251C:
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// beq cr6,0x823e2560
	if (ctx.r9.u32 != 0) {
		// lbz r8,3744(r3)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r3.u32 + 3744);
		// rlwinm r7,r8,0,0,24
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r7,0
		// bne cr6,0x823e2560
		if (ctx.r7.u32 != 0) goto loc_823E2560;
		// addi r11,r3,312
		ctx.r11.s64 = ctx.r3.s64 + 312;
		// addi r10,r3,176
		ctx.r10.s64 = ctx.r3.s64 + 176;
		// li r9,9
		ctx.r9.s64 = 9;
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
	loc_823E2548:
		// lwz r6,0(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r6,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r6.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823e2548
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823E2548;
		// blr
		return;
	}
loc_823E2560:
	// addi r11,r3,176
	ctx.r11.s64 = ctx.r3.s64 + 176;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E2570:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e2570
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E2570;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9940
	ctx.r3.s64 = ctx.r11.s64 + 9940;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6520(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6520);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// beq cr6,0x823e2624
	if (ctx.r3.s32 != ctx.r11.s32) {
		// lwz r5,40(r6)
		// bctrl
		VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r11,4(r4)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,6532(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6532);
		// cmpw cr6,r3,r11
		// bne cr6,0x823e262c
		if (ctx.r3.s32 != ctx.r11.s32) goto loc_823E262C;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E2624:
	// bl 0x823e8bb0
	snSession_8BB0_g(ctx, base);
	// b 0x823e2634
	// blr
	return;
loc_823E262C:
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r10,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r10.u8);
loc_823E2634:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9948
	ctx.r3.s64 = ctx.r11.s64 + 9948;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r10,9672(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 9672);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,9672(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9672, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_15"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_15);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_15) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r10,9672(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 9672);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r10,9672(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9672, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_vfn_13"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_vfn_13);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,44(r11)
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // snMigrateMachine_snMigratingAsGuest::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
	// addi r29,r31,24
	var_r29 = (uint32_t)(var_r31 + 24);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x823ed988
	util_D988(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snJoining::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6052(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6052);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e2774
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snJoining::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snJoining::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6064(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6064);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e27ec
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E27EC:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snLeaving_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snLeaving_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snLeaving_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9956
	ctx.r3.s64 = ctx.r11.s64 + 9956;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snLeaving_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snLeaving_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snLeaving_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snLeaving::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6544);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e28a4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snLeaving::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snLeaving::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6556(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6556);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e291c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E291C:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_6"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_6);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,24
	ctx.r3.s64 = ctx.r3.s64 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beqlr cr6
	if (ctx.r11.u32 == 0) return;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r9,0
	ctx.r9.s64 = 0;
	// lwz r8,16(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// stw r9,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r9.u32);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
	// bctr
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDormant_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snDormant_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDormant_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9964
	ctx.r3.s64 = ctx.r11.s64 + 9964;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDormant_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snDormant_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDormant_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,3784(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3784, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDormant_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snDormant_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDormant_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snDormant::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6268(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6268);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e2a14
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snDormant::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,48
		var_r29 = (uint32_t)(ctx.r11.s64 + 48);  // lbl_825D0030 @ 0x825d0030
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snDormant::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6280(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6280);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e2a8c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,44(r8)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snDormant::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,48
		var_r29 = (uint32_t)(ctx.r11.s64 + 48);  // lbl_825D0030 @ 0x825d0030
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snDormant::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6292(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6292);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e2b04
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,48
		var_r29 = (uint32_t)(ctx.r11.s64 + 48);  // lbl_825D0030 @ 0x825d0030
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E2B04:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snCreating_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snCreating_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snCreating_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9972
	ctx.r3.s64 = ctx.r11.s64 + 9972;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snCreating_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snCreating_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snCreating_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snCreating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6748(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6748);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e2c9c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,40(r6)
		// bctrl
		VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snCreating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
		// lis r11,-32162
		// li r7,0
		ctx.r7.s64 = 0;
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// addi r6,r11,20456
		ctx.r6.s64 = ctx.r11.s64 + 20456;
		// lis r11,-32162
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r5,r11,21576
		ctx.r5.s64 = ctx.r11.s64 + 21576;
		// bl 0x82430978
		SinglesNetworkClient_0978_g(ctx, base);
		// lwz r4,12(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// stw r4,3784(r11)
		PPC_STORE_U32(ctx.r11.u32 + 3784, ctx.r4.u32);
		// ld r9,16(r3)
		ctx.r9.u64 = PPC_LOAD_U64(ctx.r3.u32 + 16);
		// lwz r10,16(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
		// std r9,168(r10)
		PPC_STORE_U64(ctx.r10.u32 + 168, ctx.r9.u64);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x823e2508
		util_2508(ctx, base);
		// lwz r8,16(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r11,1
		ctx.r11.s64 = 1;
		// lwz r7,272(r8)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 272);
		// cmpwi cr6,r7,3
		// beq cr6,0x823e2be4
		if (ctx.r7.s32 != 3) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823E2BE4:
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e2c48
		if (ctx.r5.u32 != 0) {
			// lwz r11,44(r4)
			// bctrl
			VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snCreating::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
			// lwz r11,20(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// addi r29,r11,1860
			var_r29 = (uint32_t)(ctx.r11.s64 + 1860);  // addr:0x825e0744
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// mr r5,r29
			ctx.r5.u64 = var_r29;
			// bl 0x823ed988
			util_D988(ctx, base);
			// lwz r4,16(r31)
			ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
			// li r6,0
			ctx.r6.s64 = 0;
			// li r5,0
			ctx.r5.s64 = 0;
			// addi r3,r4,108
			ctx.r3.s64 = ctx.r4.s64 + 108;
			// bl 0x82371bf8
			snSession_1BF8_g(ctx, base);
			// mr r5,r29
			ctx.r5.u64 = var_r29;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x823eda90
			util_DA90(ctx, base);
			return;
		}
		// lwz r9,44(r10)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snCreating::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,448
		var_r29 = (uint32_t)(ctx.r11.s64 + 448);  // addr:0x825e01c0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r7,40(r8)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snCreating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r5,4(r6)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6760(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6760);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e2d28
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r11,44(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,24
		var_r29 = (uint32_t)(ctx.r11.s64 + 24);  // addr:0x825d0018
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,1
		ctx.r5.s64 = 1;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E2D28:
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r10,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r10.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDestroying_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snDestroying_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDestroying_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9984
	ctx.r3.s64 = ctx.r11.s64 + 9984;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_2D48"))) PPC_WEAK_FUNC(snSession_2D48);
PPC_FUNC_IMPL(__imp__snSession_2D48) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,14
	// bne cr6,0x823e2dec
	if (ctx.r5.s32 == 14) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r30)
  // [ph4a] vtable load collapsed
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r31,r11,10620
		var_r31 = (uint32_t)(ctx.r11.s64 + 10620);  // lbl_8207297C @ 0x8207297c
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// bctrl
		VCALL(var_r30, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r30,56(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e2e14
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r31,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r31);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e2e14
	} else {
	loc_823E2DEC:
		// cmpwi cr6,r5,15
		// bne cr6,0x823e2e14
		if (ctx.r5.s32 != 15) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10640
		ctx.r11.s64 = ctx.r11.s64 + 10640;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823eaa20
		snSession_AA20_fw(ctx, base);
	}
loc_823E2E14:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDestroying_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snDestroying_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDestroying_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32193
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// addi r9,r11,-20216
	ctx.r9.s64 = ctx.r11.s64 + -20216;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r10,9672(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 9672);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// stw r8,9672(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9672, ctx.r8.u32);
	// stw r9,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r9.u32);
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x823dd170
	util_D170(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8558
	snLeaveMachine_8558_g(ctx, base);
	// clrlwi r7,r3,24
	ctx.r7.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r7,0
	// bne cr6,0x823e2ea8
	if (ctx.r7.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10640
		ctx.r11.s64 = ctx.r11.s64 + 10640;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823eaa20
		snSession_AA20_fw(ctx, base);
	}
loc_823E2EA8:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDestroying_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snDestroying_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDestroying_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snDestroying::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6484(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6484);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e2f7c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snDestroying::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,24
		var_r29 = (uint32_t)(ctx.r11.s64 + 24);  // addr:0x825d0018
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x823e2508
		util_2508(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r3,0
		ctx.r3.s64 = 0;
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,18
		ctx.r5.s64 = 18;
		// stw r3,3784(r4)
		PPC_STORE_U32(ctx.r4.u32 + 3784, ctx.r3.u32);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snDestroying::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6496(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6496);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e301c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r6,44(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,24
		var_r29 = (uint32_t)(ctx.r11.s64 + 24);  // addr:0x825d0018
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x823e2508
		util_2508(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r3,0
		ctx.r3.s64 = 0;
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,19
		ctx.r5.s64 = 19;
		// stw r3,3784(r4)
		PPC_STORE_U32(ctx.r4.u32 + 3784, ctx.r3.u32);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E301C:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,9996
	ctx.r3.s64 = ctx.r11.s64 + 9996;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_vfn_13"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_vfn_13);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,44(r11)
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snChangingPresence::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// addi r29,r11,5780
	var_r29 = (uint32_t)(ctx.r11.s64 + 5780);  // addr:0x82071694
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// bl 0x823ed988
	util_D988(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=192, savegprlr_27
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snChangingPresence::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6328(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6328);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e312c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snChangingPresence::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823e8730
		snSession_8730_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snChangingPresence::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6340(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6340);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e3244
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r27,r11,424
		var_r27 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r28,r3
		var_r28 = ctx.r3.u32;
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lbz r6,3744(r11)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
		// rlwinm r5,r6,0,0,24
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r5,0
		// bne cr6,0x823e3218
		if (ctx.r5.u32 == 0) {
			// ld r11,168(r11)
			ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 168);
			// li r10,0
			ctx.r10.s64 = 0;
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// std r11,80(r1)
			PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
			// stb r10,88(r1)
			PPC_STORE_U8(ctx.r1.u32 + 88, ctx.r10.u8);
			// bl 0x823eaf10
			SinglesNetworkClient_AF10_g(ctx, base);
			// lwz r11,16(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
			// lbz r9,3744(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
			// rlwinm r8,r9,0,0,24
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
			// cmplwi cr6,r8,0
			// beq cr6,0x823e31e0
			if (ctx.r8.u32 != 0) {
				// lwz r10,3784(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 3784);
				// addi r4,r10,40
				ctx.r4.s64 = ctx.r10.s64 + 40;
				// b 0x823e31e4
			} else {
			loc_823E31E0:
				// addi r4,r11,176
				ctx.r4.s64 = ctx.r11.s64 + 176;
			}
		loc_823E31E4:
			// lwz r30,164(r11)
			var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + 164));
			// addi r3,r30,636
			ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 636;
			// bl 0x823dc2b0
			SinglesNetworkClient_C2B0_g(ctx, base);
			// cmplwi cr6,r3,0
			// beq cr6,0x823e3210
			if (ctx.r3.u32 != 0) {
				// lwz r4,84(r3)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 84);
				// li r7,0
				ctx.r7.s64 = 0;
				// li r6,1
				ctx.r6.s64 = 1;
				// mr r5,r29
				ctx.r5.u64 = var_r29;
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// bl 0x823daea0
				SinglesNetworkClient_AEA0_g(ctx, base);
			}
		loc_823E3210:
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x82260268
			SinglesNetworkClient_0268_g(ctx, base);
		}
	loc_823E3218:
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,4
		ctx.r5.s64 = 4;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E3244:
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r7,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r7.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snChanging_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snChanging_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snChanging_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10016
	ctx.r3.s64 = ctx.r11.s64 + 10016;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_3268"))) PPC_WEAK_FUNC(snSession_3268);
PPC_FUNC_IMPL(__imp__snSession_3268) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=160, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmpwi cr6,r5,22
	// bne cr6,0x823e3350
	if (ctx.r5.s32 == 22) {
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lbz r10,3744(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
		// rlwinm r9,r10,0,0,24
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r9,0
		// beq cr6,0x823e332c
		if (ctx.r9.u32 != 0) {
			// lwz r8,3776(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 3776);
			// cmpwi cr6,r8,0
			// ble cr6,0x823e332c
			if (ctx.r8.s32 <= 0) goto loc_823E332C;
			// addi r3,r1,80
			ctx.r3.s64 = ctx.r1.s64 + 80;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// lwz r7,0(r31)
  // [ph4a] vtable load collapsed
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// addi r30,r11,10380
			var_r30 = (uint32_t)(ctx.r11.s64 + 10380);  // lbl_8207288C @ 0x8207288c
			// lwz r6,44(r7)
  // [ph4a] slot load collapsed
			// stw r30,80(r1)
			PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
			// bctrl
			VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
			// lwz r31,56(r3)
			var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
			// li r5,0
			ctx.r5.s64 = 0;
			// li r4,12
			ctx.r4.s64 = 12;
			// lwz r3,4(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
			// lwz r10,4(r11)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// mr r4,r3
			ctx.r4.u64 = ctx.r3.u64;
			// cmplwi cr6,r4,0
			// beq cr6,0x823e3378
			if (ctx.r4.u32 == 0) {
				// blr
				return;
			}
			// lis r11,-32249
			// addi r3,r31,8
			ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
			// addi r11,r11,10340
			ctx.r11.s64 = ctx.r11.s64 + 10340;
			// stw r11,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
			// lwz r9,84(r1)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
			// lwz r7,88(r1)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			// rldicr r8,r9,32,63
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
			// stw r30,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
			// or r6,r7,r8
			ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
			// std r6,4(r4)
			PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
			// bl 0x823ec068
			snSession_AddNode_C068(ctx, base);
			// b 0x823e3378
			// blr
			return;
		}
	loc_823E332C:
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10400
		ctx.r11.s64 = ctx.r11.s64 + 10400;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823eaac8
		snSession_AAC8_fw(ctx, base);
		// b 0x823e3378
	} else {
	loc_823E3350:
		// cmpwi cr6,r5,23
		// bne cr6,0x823e3378
		if (ctx.r5.s32 != 23) {
			// blr
			return;
		}
		// addi r3,r1,112
		ctx.r3.s64 = ctx.r1.s64 + 112;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,112
		ctx.r4.s64 = ctx.r1.s64 + 112;
		// addi r11,r11,10420
		ctx.r11.s64 = ctx.r11.s64 + 10420;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
		// bl 0x823eab70
		snSession_AB70_gen(ctx, base);
	}
loc_823E3378:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snChanging_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snChanging_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snChanging_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// lis r11,-32193
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,-20208
	ctx.r11.s64 = ctx.r11.s64 + -20208;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x823dd170
	util_D170(ctx, base);
	// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r9,40(r10)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,20872
	ctx.r6.s64 = ctx.r11.s64 + 20872;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r4,16(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r29,0
	var_r29 = 0;
	// lbz r8,3744(r4)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r4.u32 + 3744);
	// rlwinm r7,r8,0,0,24
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r7,0
	// bne cr6,0x823e3414
	if (ctx.r7.u32 == 0) {
		// addi r29,r30,16
		var_r29 = (uint32_t)(var_r30 + 16);
	}
loc_823E3414:
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r3,r4,108
	ctx.r3.s64 = ctx.r4.s64 + 108;
	// bl 0x82371bf8
	snSession_1BF8_g(ctx, base);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// ld r6,112(r30)
	ctx.r6.u64 = PPC_LOAD_U64(var_r30 + 112);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// lwz r4,12(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
	// bl 0x8236df00
	xam_DF00(ctx, base);
	// clrlwi r6,r3,24
	ctx.r6.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r6,0
	// bne cr6,0x823e3468
	if (ctx.r6.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10420
		ctx.r11.s64 = ctx.r11.s64 + 10420;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823eab70
		snSession_AB70_gen(ctx, base);
	}
loc_823E3468:
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snChanging_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snChanging_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snChanging_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snChangingPresence_snChanging::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6316(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6316);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e350c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,5828
		var_r29 = (uint32_t)(ctx.r11.s64 + 5828);  // addr:0x825d16c4
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823e87f8
		SinglesNetworkClient_87F8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E350C:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10028
	ctx.r3.s64 = ctx.r11.s64 + 10028;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r11,0
	ctx.r11.s64 = 0;
	// li r4,10000
	ctx.r4.s64 = 10000;
	// stw r11,48(r31)
	PPC_STORE_U32(var_r31 + 48, ctx.r11.u32);
	// bl 0x823ed4f8
	util_D4F8(ctx, base);
	// lis r11,-32193
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// addi r10,r11,-20200
	ctx.r10.s64 = ctx.r11.s64 + -20200;
	// stw r10,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r10.u32);
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r3,164(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_15"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_15);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_15) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r31,r3,24
	var_r31 = (uint32_t)(ctx.r3.s64 + 24);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lwz r11,164(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 164);
	// addi r3,r11,672
	ctx.r3.s64 = ctx.r11.s64 + 672;
	// bl 0x823b3d80
	NotifyHandler_3D80_g(ctx, base);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, manual
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snChangingPresence_snWaitingForReplies::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823e3644
	if (ctx.r3.s32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10420
		ctx.r11.s64 = ctx.r11.s64 + 10420;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823eab70
		snSession_AB70_gen(ctx, base);
		// b 0x823e364c
	} else {
	loc_823E3644:
		// li r6,0
		ctx.r6.s64 = 0;
		// stb r6,0(r30)
		PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	}
loc_823E364C:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_vfn_5"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_vfn_5);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,72
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 72;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_vfn_6"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_vfn_6);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e36e8
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E36E8:
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e370c
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E370C:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snMigrating_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snMigrating_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snMigrating_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snMigrating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6352(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6352);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e37c4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snMigrating::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823e8a38
		SinglesNetworkClient_8A38_h(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snMigrating::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6364(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6364);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e383c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E383C:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10048
	ctx.r3.s64 = ctx.r11.s64 + 10048;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6196(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6196);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e38f4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,3392
		var_r29 = (uint32_t)(ctx.r11.s64 + 3392);  // addr:0x825d0d40
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6208(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6208);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e396c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,44(r8)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,3392
		var_r29 = (uint32_t)(ctx.r11.s64 + 3392);  // addr:0x825d0d40
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6220(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6220);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e39e4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r9,44(r10)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,3392
		var_r29 = (uint32_t)(ctx.r11.s64 + 3392);  // addr:0x825d0d40
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r7,40(r8)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r5,4(r6)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6712(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6712);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e3a5c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r11,44(r4)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,4048
		var_r29 = (uint32_t)(ctx.r11.s64 + 4048);  // addr:0x825d0fd0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6724(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6724);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e3ad4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,4048
		var_r29 = (uint32_t)(ctx.r11.s64 + 4048);  // addr:0x825d0fd0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6736(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6736);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e3b4c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,44(r8)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,4048
		var_r29 = (uint32_t)(ctx.r11.s64 + 4048);  // addr:0x825d0fd0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6844(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6844);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e3bc4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r9,44(r10)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,4292
		var_r29 = (uint32_t)(ctx.r11.s64 + 4292);  // addr:0x825d10c4
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r7,40(r8)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r5,4(r6)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6472(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6472);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e3c3c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r11,44(r4)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,376
		var_r29 = (uint32_t)(ctx.r11.s64 + 376);  // addr:0x825d0178
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6304(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6304);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e3cb4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,5756
		var_r29 = (uint32_t)(ctx.r11.s64 + 5756);  // addr:0x825d167c
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E3CB4:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snIdle_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snIdle_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snIdle_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snIdle::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6952(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6952);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e3d58
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive_snIdle::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,24
		var_r29 = (uint32_t)(var_r31 + 24);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snIdle::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6400(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6400);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e3dd0
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,1884
		var_r29 = (uint32_t)(ctx.r11.s64 + 1884);  // addr:0x825d075c
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E3DD0:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10056
	ctx.r3.s64 = ctx.r11.s64 + 10056;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snIdle_snArbGuestRegistering::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6964(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6964);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e3ea4
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,12
		ctx.r5.s64 = 12;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// li r4,1
		ctx.r4.s64 = 1;
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x823e75e8
		SinglesNetworkClient_75E8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E3EA4:
	// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,40(r4)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6976(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6976);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e3f40
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,13
		ctx.r5.s64 = 13;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,424
		var_r29 = (uint32_t)(ctx.r11.s64 + 424);  // addr:0x825d01a8
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r3,16(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
		// bl 0x823e75e8
		SinglesNetworkClient_75E8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E3F40:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_5"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_5);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,16(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
	// stw r31,20(r31)
	PPC_STORE_U32(var_r31 + 20, var_r31);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// lwz r9,20(r10)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 5, ctx, base);  // pattern-B slot 5 (byte +20)
	// stw r30,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r30);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snInProgress_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snInProgress_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snInProgress_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10076
	ctx.r3.s64 = ctx.r11.s64 + 10076;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snInProgress_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snInProgress_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snInProgress_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snInProgress::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6436(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6436);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e4054
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,3344
		var_r29 = (uint32_t)(ctx.r11.s64 + 3344);  // addr:0x825d0d10
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E4054:
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r4,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r4.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10088
	ctx.r3.s64 = ctx.r11.s64 + 10088;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_vfn_13"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_vfn_13);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_vfn_13) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lbz r11,3744(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
	// rlwinm r10,r11,0,28,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r10,0
	// beq cr6,0x823e40c0
	if (ctx.r10.u32 != 0) {
		// rlwinm r9,r11,0,24,24
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x80;
		// cmplwi cr6,r9,0
		// beq cr6,0x823e40c0
		if (ctx.r9.u32 == 0) goto loc_823E40C0;
		// lwz r7,44(r8)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive_snStarting::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,24
		var_r29 = (uint32_t)(var_r31 + 24);
		// b 0x823e40d8
	} else {
	loc_823E40C0:
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// addi r29,r31,1412
		var_r29 = (uint32_t)(var_r31 + 1412);
	}
loc_823E40D8:
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x823ed988
	util_D988(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_snArbHostRegistering_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_snArbHostRegistering_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_snArbHostRegistering_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10100
	ctx.r3.s64 = ctx.r11.s64 + 10100;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_4110"))) PPC_WEAK_FUNC(snSession_4110);
PPC_FUNC_IMPL(__imp__snSession_4110) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,10
	// bne cr6,0x823e41b4
	if (ctx.r5.s32 == 10) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r30)
  // [ph4a] vtable load collapsed
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r31,r11,10500
		var_r31 = (uint32_t)(ctx.r11.s64 + 10500);  // lbl_82072904 @ 0x82072904
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// bctrl
		VCALL(var_r30, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r30,56(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e41dc
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r31,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r31);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e41dc
	} else {
	loc_823E41B4:
		// cmpwi cr6,r5,11
		// bne cr6,0x823e41dc
		if (ctx.r5.s32 != 11) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10520
		ctx.r11.s64 = ctx.r11.s64 + 10520;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823eac18
		snSession_AC18_fw(ctx, base);
	}
loc_823E41DC:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_snStartingSession_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_snStartingSession_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_snStartingSession_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32193
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,-20192
	ctx.r11.s64 = ctx.r11.s64 + -20192;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x823dd170
	util_D170(ctx, base);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236e6c0
	xam_E6C0_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bne cr6,0x823e4264
	if (ctx.r10.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10520
		ctx.r11.s64 = ctx.r11.s64 + 10520;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823eac18
		snSession_AC18_fw(ctx, base);
	}
loc_823E4264:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snStarting::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6964(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6964);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e431c
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,12
		ctx.r5.s64 = 12;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,44(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// addi r29,r31,1412
		var_r29 = (uint32_t)(var_r31 + 1412);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E431C:
	// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,40(r4)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6976(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6976);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e43c0
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,13
		ctx.r5.s64 = 13;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,448
		var_r29 = (uint32_t)(ctx.r11.s64 + 448);  // addr:0x825d01c0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,15
		ctx.r5.s64 = 15;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E43C0:
	// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r5,40(r6)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 10, ctx, base);  // pattern-B slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6412(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6412);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e4444
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r9,44(r10)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive_snStarting::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,1860
		var_r29 = (uint32_t)(ctx.r11.s64 + 1860);  // addr:0x825d0744
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823e8660
		snSession_8660_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r7,40(r8)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snStarting::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r5,4(r6)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6424(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6424);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e44d0
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r11,44(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,448
		var_r29 = (uint32_t)(ctx.r11.s64 + 448);  // addr:0x825d01c0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,15
		ctx.r5.s64 = 15;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E44D0:
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r10,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r10.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_vfn_5"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_vfn_5);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,1412
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 1412;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_vfn_6"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_vfn_6);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e4560
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4560:
	// addi r3,r31,1412
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1412;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e4584
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4584:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snEnding_vfn_2"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snEnding_vfn_2);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snEnding_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,10120
	ctx.r3.s64 = ctx.r11.s64 + 10120;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_45B0"))) PPC_WEAK_FUNC(snSession_45B0);
PPC_FUNC_IMPL(__imp__snSession_45B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmpwi cr6,r5,12
	// bne cr6,0x823e4654
	if (ctx.r5.s32 == 12) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r30)
  // [ph4a] vtable load collapsed
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r31,r11,10560
		var_r31 = (uint32_t)(ctx.r11.s64 + 10560);  // lbl_82072940 @ 0x82072940
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// bctrl
		VCALL(var_r30, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r30,56(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e467c
		if (ctx.r4.u32 == 0) {
			// blr
			return;
		}
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r31,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r31);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// b 0x823e467c
	} else {
	loc_823E4654:
		// cmpwi cr6,r5,13
		// bne cr6,0x823e467c
		if (ctx.r5.s32 != 13) {
			// blr
			return;
		}
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// addi r11,r11,10580
		ctx.r11.s64 = ctx.r11.s64 + 10580;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// bl 0x823eacc0
		snSession_ACC0_fw(ctx, base);
	}
loc_823E467C:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snEnding_vfn_14"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snEnding_vfn_14);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snEnding_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32193
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// addi r9,r11,-20184
	ctx.r9.s64 = ctx.r11.s64 + -20184;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r10,9672(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 9672);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// stw r8,9672(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9672, ctx.r8.u32);
	// stw r9,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r9.u32);
	// stw r31,28(r31)
	PPC_STORE_U32(var_r31 + 28, var_r31);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x823dd170
	util_D170(ctx, base);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236e920
	xam_E920_g(ctx, base);
	// clrlwi r7,r3,24
	ctx.r7.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r7,0
	// bne cr6,0x823e4714
	if (ctx.r7.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r11,r11,10580
		ctx.r11.s64 = ctx.r11.s64 + 10580;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x823eacc0
		snSession_ACC0_fw(ctx, base);
	}
loc_823E4714:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDestroying_vfn_15"))) PPC_WEAK_FUNC(snSession_snRoot_snDestroying_vfn_15);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDestroying_vfn_15) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r31,r3,24
	var_r31 = (uint32_t)(ctx.r3.s64 + 24);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lwz r10,9672(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 9672);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r10,9672(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9672, ctx.r10.u32);
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r3,r11,240
	ctx.r3.s64 = ctx.r11.s64 + 240;
	// bl 0x823b3d80
	NotifyHandler_3D80_g(ctx, base);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r9,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snEnding_vfn_12"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snEnding_vfn_12);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snEnding_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snEnding::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6448(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6448);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823e4820
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSession_snRoot_snActive_snEnding::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,448
		var_r29 = (uint32_t)(ctx.r11.s64 + 448);  // addr:0x825d01c0
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,16
		ctx.r5.s64 = 16;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSession_snRoot_snActive_snEnding::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6460(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6460);
	// cmpw cr6,r3,r11
	// bne cr6,0x823e48ac
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,44(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,1860
		var_r29 = (uint32_t)(ctx.r11.s64 + 1860);  // addr:0x825d0744
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// lwz r4,16(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,17
		ctx.r5.s64 = 17;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823E48AC:
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r6,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r6.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_vfn_5"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_vfn_5);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,1436
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 1436;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,1460
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 1460;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,2920
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 2920;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_vfn_6"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_vfn_6);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e4958
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4958:
	// addi r3,r31,1436
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1436;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e497c
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E497C:
	// addi r3,r31,1460
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1460;
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823e49a0
	if (ctx.r5.u32 != 0) {
		// lwz r4,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r11,16(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E49A0:
	// addi r3,r31,2920
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 2920;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r10,0
	// beq cr6,0x823e49c4
	if (ctx.r10.u32 != 0) {
		// lwz r9,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r8,16(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E49C4:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_vfn_5"))) PPC_WEAK_FUNC(snSession_snRoot_vfn_5);
PPC_FUNC_IMPL(__imp__snSession_snRoot_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,48
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 48;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,376
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 376;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,424
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 424;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,3392
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 3392;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,4048
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 4048;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,4292
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 4292;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,5756
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 5756;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_vfn_6"))) PPC_WEAK_FUNC(snSession_snRoot_vfn_6);
PPC_FUNC_IMPL(__imp__snSession_snRoot_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e4aa8
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4AA8:
	// addi r3,r31,48
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 48;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823e4acc
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4ACC:
	// addi r3,r31,376
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 376;
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823e4af0
	if (ctx.r5.u32 != 0) {
		// lwz r4,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r11,16(r4)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4AF0:
	// addi r3,r31,424
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 424;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r10,0
	// beq cr6,0x823e4b14
	if (ctx.r10.u32 != 0) {
		// lwz r9,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r8,16(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4B14:
	// addi r3,r31,3392
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3392;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r7,0
	// beq cr6,0x823e4b38
	if (ctx.r7.u32 != 0) {
		// lwz r6,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r5,16(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4B38:
	// addi r3,r31,4048
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4048;
	// lwz r4,8(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r4,0
	// beq cr6,0x823e4b5c
	if (ctx.r4.u32 != 0) {
		// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r10,16(r11)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4B5C:
	// addi r3,r31,4292
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4292;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r9,0
	// beq cr6,0x823e4b80
	if (ctx.r9.u32 != 0) {
		// lwz r8,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r7,16(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4B80:
	// addi r3,r31,5756
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 5756;
	// lwz r6,8(r3)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r6,0
	// beq cr6,0x823e4ba4
	if (ctx.r6.u32 != 0) {
		// lwz r5,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r4,16(r5)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823E4BA4:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_4BC0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_4BC0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_4BC0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x823ed570
	hsmContext_ctor_D570(ctx, base);
	// lis r10,-32249
	// lis r9,-32249
	// addi r10,r10,10324
	ctx.r10.s64 = ctx.r10.s64 + 10324;
	// addi r11,r31,108
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 108;
	// li r30,0
	var_r30 = 0;
	// addi r9,r9,15676
	ctx.r9.s64 = ctx.r9.s64 + 15676;
	// addi r3,r31,136
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 136;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r30,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r30);
	// stw r30,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r30);
	// stw r30,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r30);
	// stw r30,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r30);
	// stw r30,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r30);
	// stw r30,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, var_r30);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// addi r11,r31,176
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 176;
	// stw r30,160(r31)
	PPC_STORE_U32(var_r31 + 160, var_r30);
	// mr r9,r30
	ctx.r9.u64 = var_r30;
	// stw r30,164(r31)
	PPC_STORE_U32(var_r31 + 164, var_r30);
	// li r10,9
	ctx.r10.s64 = 9;
	// std r30,168(r31)
	PPC_STORE_U64(var_r31 + 168, var_r30);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E4C38:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e4c38
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E4C38;
	// addi r11,r31,212
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 212;
	// addi r3,r31,232
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 232;
	// stw r30,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r30);
	// stw r30,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r30);
	// stw r30,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r30);
	// stw r30,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r30);
	// stw r30,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r30);
	// bl 0x8236d3d0
	pg_D3D0_h(ctx, base);
	// addi r11,r31,3756
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 3756;
	// stw r30,3752(r31)
	PPC_STORE_U32(var_r31 + 3752, var_r30);
	// li r9,1
	ctx.r9.s64 = 1;
	// addi r10,r31,3772
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 3772;
	// addi r3,r31,3792
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3792;
	// stw r30,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, var_r30);
	// stw r30,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r30);
	// lbz r8,9(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 9);
	// rlwimi r8,r9,7,0,25
	ctx.r8.u64 = (__builtin_rotateleft32(ctx.r9.u32, 7) & 0xFFFFFFC0) | (ctx.r8.u64 & 0xFFFFFFFF0000003F);
	// stb r8,9(r11)
	PPC_STORE_U8(ctx.r11.u32 + 9, ctx.r8.u8);
	// stw r30,3768(r31)
	PPC_STORE_U32(var_r31 + 3768, var_r30);
	// stw r30,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, var_r30);
	// stw r30,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, var_r30);
	// lbz r7,9(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 9);
	// rlwimi r7,r9,7,0,25
	ctx.r7.u64 = (__builtin_rotateleft32(ctx.r9.u32, 7) & 0xFFFFFFC0) | (ctx.r7.u64 & 0xFFFFFFFF0000003F);
	// stb r7,9(r10)
	PPC_STORE_U8(ctx.r10.u32 + 9, ctx.r7.u8);
	// stw r30,3784(r31)
	PPC_STORE_U32(var_r31 + 3784, var_r30);
	// stw r30,3788(r31)
	PPC_STORE_U32(var_r31 + 3788, var_r30);
	// bl 0x823e4cf0
	SinglesNetworkClient_4CF0_g(ctx, base);
	// lbz r5,9676(r31)
	ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 9676);
	// lis r11,-32193
	// stw r30,9672(r31)
	PPC_STORE_U32(var_r31 + 9672, var_r30);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// clrlwi r4,r5,25
	ctx.r4.u64 = ctx.r5.u32 & 0x7F;
	// addi r6,r11,-20176
	ctx.r6.s64 = ctx.r11.s64 + -20176;
	// stb r4,9676(r31)
	PPC_STORE_U8(var_r31 + 9676, ctx.r4.u8);
	// stw r6,144(r31)
	PPC_STORE_U32(var_r31 + 144, ctx.r6.u32);
	// stw r31,140(r31)
	PPC_STORE_U32(var_r31 + 140, var_r31);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_4CF0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_4CF0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_4CF0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r10,-32249
	// lis r9,-32249
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r8,-32249
	// li r31,0
	var_r31 = 0;
	// addi r10,r10,14004
	ctx.r10.s64 = ctx.r10.s64 + 14004;
	// addi r9,r9,14220
	ctx.r9.s64 = ctx.r9.s64 + 14220;
	// addi r11,r30,48
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 48;
	// addi r8,r8,14292
	ctx.r8.s64 = ctx.r8.s64 + 14292;
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r31,28(r30)
	PPC_STORE_U32(var_r30 + 28, var_r31);
	// stw r31,32(r30)
	PPC_STORE_U32(var_r30 + 32, var_r31);
	// stw r31,36(r30)
	PPC_STORE_U32(var_r30 + 36, var_r31);
	// stw r31,40(r30)
	PPC_STORE_U32(var_r30 + 40, var_r31);
	// stw r31,44(r30)
	PPC_STORE_U32(var_r30 + 44, var_r31);
	// stw r9,24(r30)
	PPC_STORE_U32(var_r30 + 24, ctx.r9.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r8,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// bl 0x823e5240
	SinglesNetworkClient_5240_h(ctx, base);
	// lis r10,-32249
	// addi r11,r30,376
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 376;
	// addi r10,r10,14364
	ctx.r10.s64 = ctx.r10.s64 + 14364;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// addi r3,r30,424
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 424;
	// bl 0x823e4fc0
	SinglesNetworkClient_4FC0_h(ctx, base);
	// lis r10,-32249
	// addi r11,r30,3392
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 3392;
	// addi r10,r10,14076
	ctx.r10.s64 = ctx.r10.s64 + 14076;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e54f8
	SinglesNetworkClient_54F8_g(ctx, base);
	// lis r10,-32249
	// addi r11,r30,4048
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 4048;
	// addi r10,r10,14148
	ctx.r10.s64 = ctx.r10.s64 + 14148;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e57e8
	SinglesNetworkClient_57E8_h(ctx, base);
	// lis r10,-32249
	// addi r11,r30,4292
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 4292;
	// addi r10,r10,14652
	ctx.r10.s64 = ctx.r10.s64 + 14652;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e59f8
	SinglesNetworkClient_59F8_h(ctx, base);
	// lis r11,-32249
	// addi r29,r30,5756
	var_r29 = (uint32_t)(var_r30 + 5756);
	// addi r10,r11,14436
	ctx.r10.s64 = ctx.r11.s64 + 14436;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r9,r11,14508
	ctx.r9.s64 = ctx.r11.s64 + 14508;
	// addi r11,r29,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 24;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r31,12(r29)
	PPC_STORE_U32(var_r29 + 12, var_r31);
	// stw r31,16(r29)
	PPC_STORE_U32(var_r29 + 16, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r10,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// lis r10,-32249
	// addi r11,r29,72
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 72;
	// addi r10,r10,14580
	ctx.r10.s64 = ctx.r10.s64 + 14580;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__snSession_4EB0_w"))) PPC_WEAK_FUNC(snSession_4EB0_w);
PPC_FUNC_IMPL(__imp__snSession_4EB0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r31,5756
	var_r30 = (uint32_t)(var_r31 + 5756);
	// addi r3,r30,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 72;
	// bl 0x823e5c58
	util_5C58(ctx, base);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r30,r31,4292
	var_r30 = (uint32_t)(var_r31 + 4292);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e5b50
	util_5B50(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r30,r31,4048
	var_r30 = (uint32_t)(var_r31 + 4048);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e5900
	util_5900(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r30,r31,3392
	var_r30 = (uint32_t)(var_r31 + 3392);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e56c0
	NotifyHandler_56C0_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r3,r31,424
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 424;
	// bl 0x823e5118
	snSession_5118_w(ctx, base);
	// addi r3,r31,376
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 376;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// addi r30,r31,48
	var_r30 = (uint32_t)(var_r31 + 48);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e53a8
	util_53A8(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e4eb0
	snSession_4EB0_w(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e4fa8
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E4FA8:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_4FC0_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_4FC0_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_4FC0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r10,-32249
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r10,r10,14724
	ctx.r10.s64 = ctx.r10.s64 + 14724;
	// lis r9,-32249
	// addi r11,r30,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 24;
	// lis r8,-32249
	// li r31,0
	var_r31 = 0;
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// addi r9,r9,14796
	ctx.r9.s64 = ctx.r9.s64 + 14796;
	// addi r10,r11,24
	ctx.r10.s64 = ctx.r11.s64 + 24;
	// addi r8,r8,14868
	ctx.r8.s64 = ctx.r8.s64 + 14868;
	// addi r3,r10,24
	ctx.r3.s64 = ctx.r10.s64 + 24;
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// stw r31,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, var_r31);
	// stw r31,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, var_r31);
	// stw r31,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, var_r31);
	// stw r31,16(r10)
	PPC_STORE_U32(ctx.r10.u32 + 16, var_r31);
	// stw r31,20(r10)
	PPC_STORE_U32(ctx.r10.u32 + 20, var_r31);
	// stw r8,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r8.u32);
	// bl 0x823e60c8
	SinglesNetworkClient_60C8(ctx, base);
	// lis r11,-32249
	// stw r31,1440(r30)
	PPC_STORE_U32(var_r30 + 1440, var_r31);
	// addi r29,r30,1460
	var_r29 = (uint32_t)(var_r30 + 1460);
	// stw r31,1444(r30)
	PPC_STORE_U32(var_r30 + 1444, var_r31);
	// addi r10,r11,14940
	ctx.r10.s64 = ctx.r11.s64 + 14940;
	// stw r31,1448(r30)
	PPC_STORE_U32(var_r30 + 1448, var_r31);
	// lis r11,-32249
	// stw r31,1452(r30)
	PPC_STORE_U32(var_r30 + 1452, var_r31);
	// stw r31,1456(r30)
	PPC_STORE_U32(var_r30 + 1456, var_r31);
	// addi r9,r11,15012
	ctx.r9.s64 = ctx.r11.s64 + 15012;
	// addi r11,r29,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 24;
	// stw r10,1436(r30)
	PPC_STORE_U32(var_r30 + 1436, ctx.r10.u32);
	// lis r10,-32249
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// addi r10,r10,15084
	ctx.r10.s64 = ctx.r10.s64 + 15084;
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r31,12(r29)
	PPC_STORE_U32(var_r29 + 12, var_r31);
	// stw r31,16(r29)
	PPC_STORE_U32(var_r29 + 16, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r9,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r9.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e60c8
	SinglesNetworkClient_60C8(ctx, base);
	// lis r10,-32249
	// addi r11,r29,1412
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 1412;
	// addi r10,r10,15156
	ctx.r10.s64 = ctx.r10.s64 + 15156;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// lis r10,-32249
	// addi r11,r30,2920
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 2920;
	// addi r10,r10,15228
	ctx.r10.s64 = ctx.r10.s64 + 15228;
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__snSession_5118_w"))) PPC_WEAK_FUNC(snSession_5118_w);
PPC_FUNC_IMPL(__imp__snSession_5118_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r31,2920
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 2920;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// addi r30,r31,1460
	var_r30 = (uint32_t)(var_r31 + 1460);
	// addi r3,r30,1412
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 1412;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// addi r29,r30,24
	var_r29 = (uint32_t)(var_r30 + 24);
	// addi r3,r29,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 24;
	// bl 0x823e6178
	util_6178(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r3,r31,1436
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1436;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// addi r29,r30,24
	var_r29 = (uint32_t)(var_r30 + 24);
	// addi r3,r29,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 24;
	// bl 0x823e6178
	util_6178(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDormant_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snDormant_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDormant_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e51c8
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E51C8:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snCreating_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snCreating_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snCreating_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e53a8
	util_53A8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5224
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5224:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_5240_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_5240_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_5240_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r11,r11,10932
	ctx.r11.s64 = ctx.r11.s64 + 10932;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r30,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r30);
	// stw r30,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r30);
	// stw r30,12(r31)
	PPC_STORE_U32(var_r31 + 12, var_r30);
	// stw r30,16(r31)
	PPC_STORE_U32(var_r31 + 16, var_r30);
	// stw r30,20(r31)
	PPC_STORE_U32(var_r31 + 20, var_r30);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 48;
	// mr r9,r30
	ctx.r9.u64 = var_r30;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E5294:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e5294
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E5294;
	// lis r10,-32249
	// stw r30,88(r31)
	PPC_STORE_U32(var_r31 + 88, var_r30);
	// lis r9,-32249
	// stw r30,92(r31)
	PPC_STORE_U32(var_r31 + 92, var_r30);
	// lis r8,-32249
	// stw r30,96(r31)
	PPC_STORE_U32(var_r31 + 96, var_r30);
	// addi r10,r10,11004
	ctx.r10.s64 = ctx.r10.s64 + 11004;
	// stw r30,100(r31)
	PPC_STORE_U32(var_r31 + 100, var_r30);
	// lis r7,-32249
	// stw r30,104(r31)
	PPC_STORE_U32(var_r31 + 104, var_r30);
	// addi r9,r9,11076
	ctx.r9.s64 = ctx.r9.s64 + 11076;
	// addi r8,r8,11148
	ctx.r8.s64 = ctx.r8.s64 + 11148;
	// addi r11,r31,156
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 156;
	// addi r7,r7,11220
	ctx.r7.s64 = ctx.r7.s64 + 11220;
	// stw r10,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r10.u32);
	// stw r30,112(r31)
	PPC_STORE_U32(var_r31 + 112, var_r30);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r30,116(r31)
	PPC_STORE_U32(var_r31 + 116, var_r30);
	// stw r30,120(r31)
	PPC_STORE_U32(var_r31 + 120, var_r30);
	// stw r30,124(r31)
	PPC_STORE_U32(var_r31 + 124, var_r30);
	// stw r30,128(r31)
	PPC_STORE_U32(var_r31 + 128, var_r30);
	// stw r9,108(r31)
	PPC_STORE_U32(var_r31 + 108, ctx.r9.u32);
	// stw r30,136(r31)
	PPC_STORE_U32(var_r31 + 136, var_r30);
	// stw r30,140(r31)
	PPC_STORE_U32(var_r31 + 140, var_r30);
	// stw r30,144(r31)
	PPC_STORE_U32(var_r31 + 144, var_r30);
	// stw r30,148(r31)
	PPC_STORE_U32(var_r31 + 148, var_r30);
	// stw r30,152(r31)
	PPC_STORE_U32(var_r31 + 152, var_r30);
	// stw r8,132(r31)
	PPC_STORE_U32(var_r31 + 132, ctx.r8.u32);
	// stw r30,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r30);
	// stw r30,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r30);
	// stw r30,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r30);
	// stw r30,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r30);
	// stw r30,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r30);
	// stw r7,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r7.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// lis r11,-32249
	// stw r30,208(r31)
	PPC_STORE_U32(var_r31 + 208, var_r30);
	// lis r10,-32249
	// stw r30,212(r31)
	PPC_STORE_U32(var_r31 + 212, var_r30);
	// addi r11,r11,11292
	ctx.r11.s64 = ctx.r11.s64 + 11292;
	// stw r30,216(r31)
	PPC_STORE_U32(var_r31 + 216, var_r30);
	// addi r10,r10,11436
	ctx.r10.s64 = ctx.r10.s64 + 11436;
	// stw r30,220(r31)
	PPC_STORE_U32(var_r31 + 220, var_r30);
	// stw r30,224(r31)
	PPC_STORE_U32(var_r31 + 224, var_r30);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,204(r31)
	PPC_STORE_U32(var_r31 + 204, ctx.r11.u32);
	// lis r11,-32249
	// stw r30,232(r31)
	PPC_STORE_U32(var_r31 + 232, var_r30);
	// addi r11,r11,11364
	ctx.r11.s64 = ctx.r11.s64 + 11364;
	// stw r30,236(r31)
	PPC_STORE_U32(var_r31 + 236, var_r30);
	// stw r30,240(r31)
	PPC_STORE_U32(var_r31 + 240, var_r30);
	// stw r30,244(r31)
	PPC_STORE_U32(var_r31 + 244, var_r30);
	// stw r30,248(r31)
	PPC_STORE_U32(var_r31 + 248, var_r30);
	// stw r11,228(r31)
	PPC_STORE_U32(var_r31 + 228, ctx.r11.u32);
	// stw r30,256(r31)
	PPC_STORE_U32(var_r31 + 256, var_r30);
	// stw r30,260(r31)
	PPC_STORE_U32(var_r31 + 260, var_r30);
	// stw r30,264(r31)
	PPC_STORE_U32(var_r31 + 264, var_r30);
	// stw r30,268(r31)
	PPC_STORE_U32(var_r31 + 268, var_r30);
	// stw r30,272(r31)
	PPC_STORE_U32(var_r31 + 272, var_r30);
	// stw r10,252(r31)
	PPC_STORE_U32(var_r31 + 252, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__util_53A8"))) PPC_WEAK_FUNC(util_53A8);
PPC_FUNC_IMPL(__imp__util_53A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r31,252
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 252;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// addi r3,r31,228
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 228;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// addi r3,r31,204
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 204;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// addi r3,r31,156
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 156;
	// bl 0x823e5d10
	NotifyHandler_5D10(ctx, base);
	// addi r3,r31,132
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 132;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// addi r3,r31,108
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 108;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// addi r3,r31,84
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 84;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// lis r11,-32249
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// cmplwi cr6,r3,0
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
	// beq cr6,0x823e5424
	if (ctx.r3.u32 != 0) {
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r30)
		PPC_STORE_U32(var_r30 + 20, ctx.r11.u32);
	}
loc_823E5424:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e5118
	snSession_5118_w(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5480
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5480:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snJoining_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snJoining_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snJoining_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e56c0
	NotifyHandler_56C0_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e54dc
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E54DC:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_54F8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_54F8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_54F8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=128, savegprlr_27
	// lis r11,-32249
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r31,0
	var_r31 = 0;
	// addi r11,r11,11828
	ctx.r11.s64 = ctx.r11.s64 + 11828;
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// lis r11,-32249
	// stw r31,52(r30)
	PPC_STORE_U32(var_r30 + 52, var_r31);
	// addi r29,r30,72
	var_r29 = (uint32_t)(var_r30 + 72);
	// stw r31,56(r30)
	PPC_STORE_U32(var_r30 + 56, var_r31);
	// addi r10,r11,11900
	ctx.r10.s64 = ctx.r11.s64 + 11900;
	// stw r31,60(r30)
	PPC_STORE_U32(var_r30 + 60, var_r31);
	// lis r11,-32249
	// stw r31,64(r30)
	PPC_STORE_U32(var_r30 + 64, var_r31);
	// stw r31,68(r30)
	PPC_STORE_U32(var_r30 + 68, var_r31);
	// addi r9,r11,11972
	ctx.r9.s64 = ctx.r11.s64 + 11972;
	// lis r11,-32249
	// stw r10,48(r30)
	PPC_STORE_U32(var_r30 + 48, ctx.r10.u32);
	// addi r8,r11,12044
	ctx.r8.s64 = ctx.r11.s64 + 12044;
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// addi r11,r29,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 24;
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r31,12(r29)
	PPC_STORE_U32(var_r29 + 12, var_r31);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,16(r29)
	PPC_STORE_U32(var_r29 + 16, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r9,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r9.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r8,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// lis r11,-32249
	// stw r31,76(r29)
	PPC_STORE_U32(var_r29 + 76, var_r31);
	// lis r10,-32249
	// stw r31,80(r29)
	PPC_STORE_U32(var_r29 + 80, var_r31);
	// addi r11,r11,12116
	ctx.r11.s64 = ctx.r11.s64 + 12116;
	// stw r31,84(r29)
	PPC_STORE_U32(var_r29 + 84, var_r31);
	// lis r9,-32249
	// stw r31,88(r29)
	PPC_STORE_U32(var_r29 + 88, var_r31);
	// lis r8,-32249
	// stw r31,92(r29)
	PPC_STORE_U32(var_r29 + 92, var_r31);
	// addi r10,r10,12188
	ctx.r10.s64 = ctx.r10.s64 + 12188;
	// addi r9,r9,12260
	ctx.r9.s64 = ctx.r9.s64 + 12260;
	// stw r11,72(r29)
	PPC_STORE_U32(var_r29 + 72, ctx.r11.u32);
	// addi r8,r8,12332
	ctx.r8.s64 = ctx.r8.s64 + 12332;
	// addi r11,r30,240
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 240;
	// stw r31,100(r29)
	PPC_STORE_U32(var_r29 + 100, var_r31);
	// stw r31,104(r29)
	PPC_STORE_U32(var_r29 + 104, var_r31);
	// lis r7,-32249
	// stw r31,108(r29)
	PPC_STORE_U32(var_r29 + 108, var_r31);
	// stw r31,112(r29)
	PPC_STORE_U32(var_r29 + 112, var_r31);
	// addi r7,r7,12404
	ctx.r7.s64 = ctx.r7.s64 + 12404;
	// stw r31,116(r29)
	PPC_STORE_U32(var_r29 + 116, var_r31);
	// stw r10,96(r29)
	PPC_STORE_U32(var_r29 + 96, ctx.r10.u32);
	// addi r29,r11,24
	var_r29 = (uint32_t)(ctx.r11.s64 + 24);  // addr:0x82070018
	// stw r31,196(r30)
	PPC_STORE_U32(var_r30 + 196, var_r31);
	// stw r31,200(r30)
	PPC_STORE_U32(var_r30 + 200, var_r31);
	// stw r31,204(r30)
	PPC_STORE_U32(var_r30 + 204, var_r31);
	// stw r31,208(r30)
	PPC_STORE_U32(var_r30 + 208, var_r31);
	// stw r31,212(r30)
	PPC_STORE_U32(var_r30 + 212, var_r31);
	// stw r9,192(r30)
	PPC_STORE_U32(var_r30 + 192, ctx.r9.u32);
	// stw r31,220(r30)
	PPC_STORE_U32(var_r30 + 220, var_r31);
	// stw r31,224(r30)
	PPC_STORE_U32(var_r30 + 224, var_r31);
	// stw r31,228(r30)
	PPC_STORE_U32(var_r30 + 228, var_r31);
	// stw r31,232(r30)
	PPC_STORE_U32(var_r30 + 232, var_r31);
	// stw r31,236(r30)
	PPC_STORE_U32(var_r30 + 236, var_r31);
	// stw r8,216(r30)
	PPC_STORE_U32(var_r30 + 216, ctx.r8.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// addi r28,r29,4
	var_r28 = (uint32_t)(var_r29 + 4);
	// stw r7,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r7.u32);
	// stw r31,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ var_r31);
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r27,r28,4
	var_r27 = (uint32_t)(var_r28 + 4);
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// stw r31,0(r27)
	PPC_STORE_U32(var_r27 + 0, var_r31);
	// stw r31,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r31);
	// addi r10,r30,324
	ctx.r10.s64 = (int64_t)(int32_t)var_r30 + 324;
	// lbz r8,12(r29)
	ctx.r8.u64 = PPC_LOAD_U8(var_r29 + 12);
	// mr r9,r31
	ctx.r9.u64 = var_r31;
	// li r11,9
	ctx.r11.s64 = 9;
	// ori r7,r8,128
	ctx.r7.u64 = ctx.r8.u64 | 128;
	// stb r7,12(r29)
	PPC_STORE_U8(var_r29 + 12, ctx.r7.u8);
	// stb r31,296(r30)
	PPC_STORE_U8(var_r30 + 296, (uint8_t)var_r31);
	// std r31,312(r30)
	PPC_STORE_U64(var_r30 + 312, var_r31);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
loc_823E5694:
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x823e5694
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E5694;
	// addi r3,r30,360
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 360;
	// bl 0x823b3fd8
	SinglesNetworkClient_3FD8_g(ctx, base);
	// addi r3,r30,464
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 464;
	// bl 0x823e04a0
	SinglesNetworkClient_04A0_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__NotifyHandler_56C0_g"))) PPC_WEAK_FUNC(NotifyHandler_56C0_g);
PPC_FUNC_IMPL(__imp__NotifyHandler_56C0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r31,464
	var_r30 = (uint32_t)(var_r31 + 464);
	// addi r3,r30,52
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 52;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32249
	// addi r3,r31,360
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 360;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// addi r3,r31,240
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 240;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r31,216
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 216;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r31,192
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 192;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r30,r31,72
	var_r30 = (uint32_t)(var_r31 + 72);
	// addi r3,r30,96
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 96;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r30,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 72;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e6300
	util_6300(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r31,48
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 48;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// lis r11,-32249
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// cmplwi cr6,r3,0
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
	// beq cr6,0x823e5768
	if (ctx.r3.u32 != 0) {
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r30)
		PPC_STORE_U32(var_r30 + 20, ctx.r11.u32);
	}
loc_823E5768:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snLeaving_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snLeaving_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snLeaving_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e5900
	util_5900(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e57cc
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E57CC:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_57E8_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_57E8_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_57E8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32249
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r31,0
	var_r31 = 0;
	// addi r11,r11,12780
	ctx.r11.s64 = ctx.r11.s64 + 12780;
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// lis r11,-32249
	// stw r31,52(r30)
	PPC_STORE_U32(var_r30 + 52, var_r31);
	// addi r29,r30,72
	var_r29 = (uint32_t)(var_r30 + 72);
	// stw r31,56(r30)
	PPC_STORE_U32(var_r30 + 56, var_r31);
	// addi r10,r11,12852
	ctx.r10.s64 = ctx.r11.s64 + 12852;
	// stw r31,60(r30)
	PPC_STORE_U32(var_r30 + 60, var_r31);
	// lis r11,-32249
	// stw r31,64(r30)
	PPC_STORE_U32(var_r30 + 64, var_r31);
	// stw r31,68(r30)
	PPC_STORE_U32(var_r30 + 68, var_r31);
	// addi r9,r11,12924
	ctx.r9.s64 = ctx.r11.s64 + 12924;
	// lis r11,-32249
	// stw r10,48(r30)
	PPC_STORE_U32(var_r30 + 48, ctx.r10.u32);
	// addi r8,r11,12996
	ctx.r8.s64 = ctx.r11.s64 + 12996;
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// addi r11,r29,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 24;
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r31,12(r29)
	PPC_STORE_U32(var_r29 + 12, var_r31);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,16(r29)
	PPC_STORE_U32(var_r29 + 16, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r9,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r9.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r8,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// lis r11,-32249
	// stw r31,76(r29)
	PPC_STORE_U32(var_r29 + 76, var_r31);
	// lis r10,-32249
	// stw r31,80(r29)
	PPC_STORE_U32(var_r29 + 80, var_r31);
	// addi r11,r11,13068
	ctx.r11.s64 = ctx.r11.s64 + 13068;
	// stw r31,84(r29)
	PPC_STORE_U32(var_r29 + 84, var_r31);
	// addi r10,r10,13212
	ctx.r10.s64 = ctx.r10.s64 + 13212;
	// stw r31,88(r29)
	PPC_STORE_U32(var_r29 + 88, var_r31);
	// stw r31,92(r29)
	PPC_STORE_U32(var_r29 + 92, var_r31);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,72(r29)
	PPC_STORE_U32(var_r29 + 72, ctx.r11.u32);
	// lis r11,-32249
	// stw r31,172(r30)
	PPC_STORE_U32(var_r30 + 172, var_r31);
	// addi r11,r11,13140
	ctx.r11.s64 = ctx.r11.s64 + 13140;
	// stw r31,176(r30)
	PPC_STORE_U32(var_r30 + 176, var_r31);
	// stw r31,180(r30)
	PPC_STORE_U32(var_r30 + 180, var_r31);
	// stw r31,184(r30)
	PPC_STORE_U32(var_r30 + 184, var_r31);
	// stw r31,188(r30)
	PPC_STORE_U32(var_r30 + 188, var_r31);
	// stw r11,168(r30)
	PPC_STORE_U32(var_r30 + 168, ctx.r11.u32);
	// stw r31,196(r30)
	PPC_STORE_U32(var_r30 + 196, var_r31);
	// stw r31,200(r30)
	PPC_STORE_U32(var_r30 + 200, var_r31);
	// stw r31,204(r30)
	PPC_STORE_U32(var_r30 + 204, var_r31);
	// stw r31,208(r30)
	PPC_STORE_U32(var_r30 + 208, var_r31);
	// stw r31,212(r30)
	PPC_STORE_U32(var_r30 + 212, var_r31);
	// stw r10,192(r30)
	PPC_STORE_U32(var_r30 + 192, ctx.r10.u32);
	return;
}

__attribute__((alias("__imp__util_5900"))) PPC_WEAK_FUNC(util_5900);
PPC_FUNC_IMPL(__imp__util_5900) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r31,192
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 192;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// addi r3,r31,168
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 168;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// addi r30,r31,72
	var_r30 = (uint32_t)(var_r31 + 72);
	// addi r3,r30,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 72;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e6420
	util_6420(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// addi r3,r31,48
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 48;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// lis r11,-32249
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 20);
	// cmplwi cr6,r3,0
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
	// beq cr6,0x823e5978
	if (ctx.r3.u32 != 0) {
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r30)
		PPC_STORE_U32(var_r30 + 20, ctx.r11.u32);
	}
loc_823E5978:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snMigrating_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snMigrating_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snMigrating_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e5b50
	util_5B50(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e59dc
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E59DC:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_59F8_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_59F8_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_59F8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32249
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// li r31,0
	var_r31 = 0;
	// addi r11,r11,13484
	ctx.r11.s64 = ctx.r11.s64 + 13484;
	// addi r3,r29,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 24;
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r31,12(r29)
	PPC_STORE_U32(var_r29 + 12, var_r31);
	// stw r31,16(r29)
	PPC_STORE_U32(var_r29 + 16, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r11,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// addi r11,r29,48
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 48;
	// mr r9,r31
	ctx.r9.u64 = var_r31;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E5A44:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e5a44
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E5A44;
	// lis r11,-32249
	// addi r30,r29,84
	var_r30 = (uint32_t)(var_r29 + 84);
	// addi r11,r11,13556
	ctx.r11.s64 = ctx.r11.s64 + 13556;
	// li r5,1152
	ctx.r5.s64 = 1152;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x8242fed0
	memset(ctx, base);
	// lis r10,-32249
	// stw r31,1188(r30)
	PPC_STORE_U32(var_r30 + 1188, var_r31);
	// lis r9,-32249
	// stw r31,1192(r30)
	PPC_STORE_U32(var_r30 + 1192, var_r31);
	// addi r10,r10,13628
	ctx.r10.s64 = ctx.r10.s64 + 13628;
	// stw r31,1196(r30)
	PPC_STORE_U32(var_r30 + 1196, var_r31);
	// addi r11,r30,1208
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 1208;
	// stw r31,1200(r30)
	PPC_STORE_U32(var_r30 + 1200, var_r31);
	// addi r9,r9,13700
	ctx.r9.s64 = ctx.r9.s64 + 13700;
	// stw r31,1204(r30)
	PPC_STORE_U32(var_r30 + 1204, var_r31);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r10,1184(r30)
	PPC_STORE_U32(var_r30 + 1184, ctx.r10.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// lis r11,-32249
	// addi r30,r29,1340
	var_r30 = (uint32_t)(var_r29 + 1340);
	// addi r10,r11,13772
	ctx.r10.s64 = ctx.r11.s64 + 13772;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r9,r11,13844
	ctx.r9.s64 = ctx.r11.s64 + 13844;
	// addi r11,r30,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 24;
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// addi r3,r11,24
	ctx.r3.s64 = ctx.r11.s64 + 24;
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// lis r11,-32249
	// stw r31,80(r30)
	PPC_STORE_U32(var_r30 + 80, var_r31);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// stw r31,84(r30)
	PPC_STORE_U32(var_r30 + 84, var_r31);
	// addi r11,r11,13916
	ctx.r11.s64 = ctx.r11.s64 + 13916;
	// stw r31,88(r30)
	PPC_STORE_U32(var_r30 + 88, var_r31);
	// stw r31,92(r30)
	PPC_STORE_U32(var_r30 + 92, var_r31);
	// stw r31,96(r30)
	PPC_STORE_U32(var_r30 + 96, var_r31);
	// stw r11,76(r30)
	PPC_STORE_U32(var_r30 + 76, ctx.r11.u32);
	return;
}

__attribute__((alias("__imp__util_5B50"))) PPC_WEAK_FUNC(util_5B50);
PPC_FUNC_IMPL(__imp__util_5B50) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r31,r30,1340
	var_r31 = (uint32_t)(var_r30 + 1340);
	// addi r3,r31,76
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 76;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e6558
	util_6558(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// addi r31,r30,84
	var_r31 = (uint32_t)(var_r30 + 84);
	// addi r3,r31,1208
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1208;
	// bl 0x823e6558
	util_6558(ctx, base);
	// addi r3,r31,1184
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1184;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// lis r11,-32249
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823e5bcc
	if (ctx.r3.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E5BCC:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// bl 0x823e5c58
	util_5C58(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5c3c
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5C3C:
	// blr
	return;
}

__attribute__((alias("__imp__util_5C58"))) PPC_WEAK_FUNC(util_5C58);
PPC_FUNC_IMPL(__imp__util_5C58) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32250
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5ca0
	if (ctx.r11.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E5CA0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_vfn_0"))) PPC_WEAK_FUNC(snCreateMachine_vfn_0);
PPC_FUNC_IMPL(__imp__snCreateMachine_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e53a8
	util_53A8(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5cf8
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5CF8:
	// blr
	return;
}

__attribute__((alias("__imp__NotifyHandler_5D10"))) PPC_WEAK_FUNC(NotifyHandler_5D10);
PPC_FUNC_IMPL(__imp__NotifyHandler_5D10) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32250
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5d58
	if (ctx.r11.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E5D58:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snIdle_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snIdle_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snIdle_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e6178
	util_6178(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r29,31
	ctx.r11.u64 = var_r29 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5dc0
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5DC0:
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snStarting_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snStarting_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snStarting_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,1412
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1412;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823e6178
	util_6178(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r29,31
	ctx.r11.u64 = var_r29 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5e18
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5E18:
	return;
}

__attribute__((alias("__imp__snSession_5E20_gen"))) PPC_WEAK_FUNC(snSession_5E20_gen);
PPC_FUNC_IMPL(__imp__snSession_5E20_gen) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32249
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x823e5e64
	if (ctx.r3.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E5E64:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_vfn_0"))) PPC_WEAK_FUNC(snJoinMachine_vfn_0);
PPC_FUNC_IMPL(__imp__snJoinMachine_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e56c0
	NotifyHandler_56C0_g(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5ec0
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5EC0:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_vfn_0"))) PPC_WEAK_FUNC(snLeaveMachine_vfn_0);
PPC_FUNC_IMPL(__imp__snLeaveMachine_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e5900
	util_5900(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5f10
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5F10:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_vfn_0"))) PPC_WEAK_FUNC(snMigrateMachine_vfn_0);
PPC_FUNC_IMPL(__imp__snMigrateMachine_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e5b50
	util_5B50(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5f60
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5F60:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snChangingPresence_snWaitingForReplies_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e5c58
	util_5C58(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e5fb0
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E5FB0:
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmCreatingHost_vfn_0"))) PPC_WEAK_FUNC(snCreateMachine_snHsmCreatingHost_vfn_0);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmCreatingHost_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e8da8
	NotifyHandler_8DA8_w(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6000
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6000:
	// blr
	return;
}

__attribute__((alias("__imp__snCreateMachine_snHsmRequestingConfig_vfn_0"))) PPC_WEAK_FUNC(snCreateMachine_snHsmRequestingConfig_vfn_0);
PPC_FUNC_IMPL(__imp__snCreateMachine_snHsmRequestingConfig_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e5d10
	NotifyHandler_5D10(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6050
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6050:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snIdle_snArbGuestRegistering_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e6178
	util_6178(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e9010
	snSession_9010_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e60ac
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E60AC:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_60C8"))) PPC_WEAK_FUNC(SinglesNetworkClient_60C8);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_60C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r11,r11,13308
	ctx.r11.s64 = ctx.r11.s64 + 13308;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// stw r30,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r30);
	// stw r30,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r30);
	// stw r30,12(r31)
	PPC_STORE_U32(var_r31 + 12, var_r30);
	// stw r30,16(r31)
	PPC_STORE_U32(var_r31 + 16, var_r30);
	// stw r30,20(r31)
	PPC_STORE_U32(var_r31 + 20, var_r30);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x822eda08
	SinglesNetworkClient_DA08(ctx, base);
	// addi r10,r31,48
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 48;
	// li r7,31
	ctx.r7.s64 = 31;
loc_823E6114:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// mr r8,r30
	ctx.r8.u64 = var_r30;
	// li r9,9
	ctx.r9.s64 = 9;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823E6124:
	// stw r8,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e6124
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E6124;
	// addi r11,r10,36
	ctx.r11.s64 = ctx.r10.s64 + 36;
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// addi r10,r10,40
	ctx.r10.s64 = ctx.r10.s64 + 40;
	// cmpwi cr6,r7,0
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// clrlwi r8,r9,26
	ctx.r8.u64 = ctx.r9.u32 & 0x3F;
	// stb r8,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r8.u8);
	// bge cr6,0x823e6114
	if (ctx.r7.s32 >= 0) goto loc_823E6114;
	// addi r3,r31,1340
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1340;
	// bl 0x823e8d00
	SinglesNetworkClient_8D00(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__util_6178"))) PPC_WEAK_FUNC(util_6178);
PPC_FUNC_IMPL(__imp__util_6178) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32249
	// addi r31,r30,1340
	var_r31 = (uint32_t)(var_r30 + 1340);
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// li r29,0
	var_r29 = 0;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// cmplwi cr6,r3,0
	// beq cr6,0x823e61b4
	if (ctx.r3.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// stw r29,20(r31)
		PPC_STORE_U32(var_r31 + 20, var_r29);
	}
loc_823E61B4:
	// lis r11,-32250
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e61e0
	if (ctx.r11.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// stw r29,20(r31)
		PPC_STORE_U32(var_r31 + 20, var_r29);
	}
loc_823E61E0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8ee0
	snSession_8EE0_w(ctx, base);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snDestroying_vfn_0"))) PPC_WEAK_FUNC(snSession_snRoot_snDestroying_vfn_0);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snDestroying_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e5e20
	snSession_5E20_gen(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6228
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6228:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmLocalJoinPending_vfn_0"))) PPC_WEAK_FUNC(snJoinMachine_snHsmLocalJoinPending_vfn_0);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmLocalJoinPending_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6278
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6278:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_vfn_0"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_vfn_0);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,96
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 96;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e6300
	util_6300(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e62e4
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E62E4:
	// blr
	return;
}

__attribute__((alias("__imp__util_6300"))) PPC_WEAK_FUNC(util_6300);
PPC_FUNC_IMPL(__imp__util_6300) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32250
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6348
	if (ctx.r11.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E6348:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8e10
	util_8E10(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_0"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmLocalLeavePending_vfn_0);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmLocalLeavePending_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e63a0
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E63A0:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_vfn_0"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_vfn_0);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e6420
	util_6420(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6404
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6404:
	// blr
	return;
}

__attribute__((alias("__imp__util_6420"))) PPC_WEAK_FUNC(util_6420);
PPC_FUNC_IMPL(__imp__util_6420) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32250
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6468
	if (ctx.r11.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E6468:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8e78
	util_8E78(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_vfn_0"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_vfn_0);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,1208
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1208;
	// bl 0x823e6558
	util_6558(ctx, base);
	// addi r3,r31,1184
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 1184;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e64d4
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E64D4:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snMigratingAsGuest_vfn_0"))) PPC_WEAK_FUNC(snMigrateMachine_snMigratingAsGuest_vfn_0);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snMigratingAsGuest_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,76
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 76;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823e6558
	util_6558(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e653c
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E653C:
	// blr
	return;
}

__attribute__((alias("__imp__util_6558"))) PPC_WEAK_FUNC(util_6558);
PPC_FUNC_IMPL(__imp__util_6558) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32250
	// addi r31,r30,24
	var_r31 = (uint32_t)(var_r30 + 24);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e65a0
	if (ctx.r11.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E65A0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snArbMachine_vfn_0"))) PPC_WEAK_FUNC(snArbMachine_vfn_0);
PPC_FUNC_IMPL(__imp__snArbMachine_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e6178
	util_6178(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e65f8
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E65F8:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_0"))) PPC_WEAK_FUNC(snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_0);
PPC_FUNC_IMPL(__imp__snJoinMachine_snHsmRequestingJoin_snHsmWaitingForReply_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e6300
	util_6300(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6648
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6648:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_0"))) PPC_WEAK_FUNC(snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_0);
PPC_FUNC_IMPL(__imp__snLeaveMachine_snHsmRequestingLeave_snHsmWaitingForReply_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e6420
	util_6420(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6698
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6698:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_0"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snMigrating_vfn_0);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snMigrating_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e8f48
	util_8F48(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e66e8
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E66E8:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_0"))) PPC_WEAK_FUNC(snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_0);
PPC_FUNC_IMPL(__imp__snMigrateMachine_snAcquiringHost_snNotifyingGuests_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e6558
	util_6558(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e6738
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E6738:
	// blr
	return;
}

__attribute__((alias("__imp__atSafeDLListSimple_ctor_6750"))) PPC_WEAK_FUNC(atSafeDLListSimple_ctor_6750);
PPC_FUNC_IMPL(__imp__atSafeDLListSimple_ctor_6750) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,10324
	ctx.r11.s64 = ctx.r11.s64 + 10324;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x823e6968
	rage_6968(ctx, base);
	// addi r3,r31,3792
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3792;
	// bl 0x823e4eb0
	snSession_4EB0_w(ctx, base);
	// addi r3,r31,3772
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3772;
	// bl 0x823ea768
	util_A768(ctx, base);
	// addi r3,r31,3756
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3756;
	// bl 0x824160b8
	game_60B8(ctx, base);
	// addi r3,r31,232
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 232;
	// bl 0x8236d610
	atSafeDLListSimple_ctor_D610(ctx, base);
	// addi r29,r31,212
	var_r29 = (uint32_t)(var_r31 + 212);
loc_823E6794:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823ed188
	util_D188(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x823e6794
	if (ctx.r3.u32 != 0) goto loc_823E6794;
	// lis r11,-32250
	// li r28,0
	var_r28 = 0;
	// addi r30,r31,136
	var_r30 = (uint32_t)(var_r31 + 136);
	// addi r11,r11,-14512
	ctx.r11.s64 = ctx.r11.s64 + -14512;
	// stw r28,0(r29)
	PPC_STORE_U32(var_r29 + 0, var_r28);
	// stw r28,4(r29)
	PPC_STORE_U32(var_r29 + 4, var_r28);
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0, ctx.r11.u32);
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823e67dc
	if (ctx.r11.u32 != 0) {
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// stw r28,20(r30)
		PPC_STORE_U32(var_r30 + 20, var_r28);
	}
loc_823E67DC:
	// lis r11,-32249
	// addi r3,r31,108
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 108;
	// addi r11,r11,15676
	ctx.r11.s64 = ctx.r11.s64 + 15676;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// bl 0x823e8fb0
	atSafeDLListSimple_8FB0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ed6a8
	util_D6A8(ctx, base);
	return;
}

__attribute__((alias("__imp__game_6800"))) PPC_WEAK_FUNC(game_6800);
PPC_FUNC_IMPL(__imp__game_6800) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, savegprlr_28
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r4,r11,10128
	ctx.r4.s64 = ctx.r11.s64 + 10128;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// addi r30,r31,3792
	var_r30 = (uint32_t)(var_r31 + 3792);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r11,0(r30)
  // [ph4a] vtable load collapsed
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4, var_r31);
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r30,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r30);
	// lwz r10,20(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r30, 5, ctx, base);  // pattern-B slot 5 (byte +20)
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ed720
	snSessionFinder_D720_p46(ctx, base);
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r9,r28,12
	ctx.r9.s64 = (int64_t)(int32_t)var_r28 + 12;
	// stw r29,216(r31)
	PPC_STORE_U32(var_r31 + 216, var_r29);
	// stw r8,212(r31)
	PPC_STORE_U32(var_r31 + 212, ctx.r8.u32);
	// stw r28,160(r31)
	PPC_STORE_U32(var_r31 + 160, var_r28);
	// stw r9,164(r31)
	PPC_STORE_U32(var_r31 + 164, ctx.r9.u32);
	// bl 0x820c0038
	xe_main_thread_init_0038(ctx, base);
	// lwz r28,0(r13)
	var_r28 = (uint32_t)(PPC_LOAD_U32(ctx.r13.u32 + 0)  /* SDA:0x82600000 */);
	// li r29,4
	var_r29 = 4;
	// li r5,16
	ctx.r5.s64 = 16;
	// li r4,20
	ctx.r4.s64 = 20;
	// lwzx r3,r29,r28
	ctx.r3.u64 = PPC_LOAD_U32(var_r29 + var_r28);
	// lwz r6,4(r7)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmplwi cr6,r30,0
	// beq cr6,0x823e68b8
	if (var_r30 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// li r4,48
		ctx.r4.s64 = 48;
		// bl 0x82122c60
		game_2C60(ctx, base);
		// b 0x823e68bc
	} else {
	loc_823E68B8:
		// li r30,0
		var_r30 = 0;
	}
loc_823E68BC:
	// addi r3,r31,3756
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3756;
	// stw r30,3752(r31)
	PPC_STORE_U32(var_r31 + 3752, var_r30);
	// bl 0x824160b8
	game_60B8(ctx, base);
	// bl 0x820c0038
	xe_main_thread_init_0038(ctx, base);
	// lwzx r3,r29,r28
	ctx.r3.u64 = PPC_LOAD_U32(var_r29 + var_r28);
	// li r5,16
	ctx.r5.s64 = 16;
	// li r4,20
	ctx.r4.s64 = 20;
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmplwi cr6,r30,0
	// beq cr6,0x823e6904
	if (var_r30 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// li r4,68
		ctx.r4.s64 = 68;
		// bl 0x82122c60
		game_2C60(ctx, base);
		// b 0x823e6908
	} else {
	loc_823E6904:
		// li r30,0
		var_r30 = 0;
	}
loc_823E6908:
	// addi r3,r31,3772
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3772;
	// stw r30,3768(r31)
	PPC_STORE_U32(var_r31 + 3768, var_r30);
	// bl 0x823ea768
	util_A768(ctx, base);
	// addi r3,r31,232
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 232;
	// bl 0x8236f9b8
	pg_F9B8_g(ctx, base);
	// addi r4,r31,136
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 136;
	// lwz r3,164(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 164);
	// bl 0x823da940
	SinglesNetworkClient_A940_g(ctx, base);
	// lwz r11,84(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
	// lbz r9,9676(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 9676);
	// li r4,0
	ctx.r4.s64 = 0;
	// lbz r8,104(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 104);
	// addi r6,r11,1
	ctx.r6.s64 = ctx.r11.s64 + 1;
	// ori r7,r9,128
	ctx.r7.u64 = ctx.r9.u64 | 128;
	// lwz r5,4(r31)
	ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 4);
	// clrlwi r11,r8,25
	ctx.r11.u64 = ctx.r8.u32 & 0x7F;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r6,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r6.u32);
	// stb r7,9676(r31)
	PPC_STORE_U8(var_r31 + 9676, ctx.r7.u8);
	// stb r11,104(r31)
	PPC_STORE_U8(var_r31 + 104, ctx.r11.u8);
	// bl 0x823eda90
	util_DA90(ctx, base);
	return;
}

__attribute__((alias("__imp__rage_6968"))) PPC_WEAK_FUNC(rage_6968);
PPC_FUNC_IMPL(__imp__rage_6968) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r11,9676(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 9676);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// beq cr6,0x823e6abc
	if (ctx.r10.u32 != 0) {
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,10152
		ctx.r4.s64 = ctx.r11.s64 + 10152;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// lwz r11,164(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 164);
		// addi r30,r31,136
		var_r30 = (uint32_t)(var_r31 + 136);
		// addi r3,r11,672
		ctx.r3.s64 = ctx.r11.s64 + 672;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r29,0
		var_r29 = 0;
		// stw r29,20(r30)
		PPC_STORE_U32(var_r30 + 20, var_r29);
		// lwz r11,3760(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 3760);
		// cmpwi cr6,r11,0
		// beq cr6,0x823e6a0c
	while (ctx.r11.s32 != 0) {
		loc_823E69C0:
			// lwz r11,3756(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 3756);
			// cmplwi cr6,r11,0
			// beq cr6,0x823e69ec
			if (ctx.r11.u32 != 0) {
				// lwz r9,8(r11)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
				// cmplwi cr6,r9,0
				// beq cr6,0x823e69f0
				if (ctx.r9.u32 == 0) goto loc_823E69F0;
			loc_823E69D8:
				// lwz r11,8(r11)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
				// lwz r8,8(r11)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
				// cmplwi cr6,r8,0
				// bne cr6,0x823e69d8
				if (ctx.r8.u32 != 0) goto loc_823E69D8;
				// b 0x823e69f0
			} else {
			loc_823E69EC:
				// mr r11,r29
				ctx.r11.u64 = var_r29;
			}
		loc_823E69F0:
			// lwz r11,0(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// addi r4,r11,96
			ctx.r4.s64 = ctx.r11.s64 + 96;
			// bl 0x823e81c0
			snLeaveMachine_81C0_g(ctx, base);
			// lwz r11,3760(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 3760);
			// cmpwi cr6,r11,0
			// bne cr6,0x823e69c0
	}
	loc_823E6A0C:
		// addi r30,r31,212
		var_r30 = (uint32_t)(var_r31 + 212);
	loc_823E6A10:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823ed188
		util_D188(ctx, base);
		// cmplwi cr6,r3,0
		// bne cr6,0x823e6a10
		if (ctx.r3.u32 != 0) goto loc_823E6A10;
		// addi r3,r31,3792
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3792;
		// stw r29,0(r30)
		PPC_STORE_U32(var_r30 + 0,/* rage_GameObject::vtable@+0x0 */ var_r29);
		// stw r29,4(r30)
		PPC_STORE_U32(var_r30 + 4,/* rage_GameObject::flags@+0x4 */ var_r29);
		// lwz r6,16(r7)
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823ed788
		util_D788(ctx, base);
		// addi r3,r31,232
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 232;
		// bl 0x8236d6f0
		atSingleton_D6F0_w(ctx, base);
		// addi r3,r31,3756
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3756;
		// stw r29,164(r31)
		PPC_STORE_U32(var_r31 + 164, var_r29);
		// bl 0x824160b8
		game_60B8(ctx, base);
		// lwz r30,3752(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 3752));
		// cmplwi cr6,r30,0
		// beq cr6,0x823e6a74
		if (var_r30 != 0) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x82122c00
			rage_2C00(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x820c00c0
			rage_free_00C0(ctx, base);
		}
	loc_823E6A74:
		// addi r3,r31,3772
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3772;
		// stw r29,3752(r31)
		PPC_STORE_U32(var_r31 + 3752, var_r29);
		// bl 0x823ea768
		util_A768(ctx, base);
		// lwz r30,3768(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 3768));
		// cmplwi cr6,r30,0
		// beq cr6,0x823e6a9c
		if (var_r30 != 0) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x82122c00
			rage_2C00(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x820c00c0
			rage_free_00C0(ctx, base);
		}
	loc_823E6A9C:
		// lbz r5,9676(r31)
		ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 9676);
		// stw r29,3768(r31)
		PPC_STORE_U32(var_r31 + 3768, var_r29);
		// clrlwi r4,r5,25
		ctx.r4.u64 = ctx.r5.u32 & 0x7F;
		// stw r29,160(r31)
		PPC_STORE_U32(var_r31 + 160, var_r29);
		// stw r29,3784(r31)
		PPC_STORE_U32(var_r31 + 3784, var_r29);
		// stw r29,3788(r31)
		PPC_STORE_U32(var_r31 + 3788, var_r29);
		// stw r29,9672(r31)
		PPC_STORE_U32(var_r31 + 9672, var_r29);
		// stb r4,9676(r31)
		PPC_STORE_U8(var_r31 + 9676, ctx.r4.u8);
	}
loc_823E6ABC:
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6AC8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_6AC8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6AC8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r25 = 0;
	// FRAME: size=432, savegprlr_25
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// mr r27,r7
	var_r27 = ctx.r7.u32;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// mr r26,r8
	var_r26 = ctx.r8.u32;
	// mr r25,r9
	var_r25 = ctx.r9.u32;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r3,r1,88
	ctx.r3.s64 = ctx.r1.s64 + 88;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r30,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, var_r30);
	// addi r11,r11,10660
	ctx.r11.s64 = ctx.r11.s64 + 10660;
	// stw r29,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, var_r29);
	// addi r3,r1,108
	ctx.r3.s64 = ctx.r1.s64 + 108;
	// stw r28,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, var_r28);
	// li r5,256
	ctx.r5.s64 = 256;
	// stw r27,104(r1)
	PPC_STORE_U32(ctx.r1.u32 + 104, var_r27);
	// mr r4,r26
	ctx.r4.u64 = var_r26;
	// stw r10,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r10.u32);
	// stw r10,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r10.u32);
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x82434100
	memcpy(ctx, base);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// stw r25,364(r1)
	PPC_STORE_U32(ctx.r1.u32 + 364, var_r25);
	// addi r3,r31,212
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 212;
	// bl 0x823eb788
	snSession_B788_h(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6B60_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_6B60_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6B60_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=272, savegprlr_26
	// lis r11,-32249
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// mr r29,r6
	var_r29 = ctx.r6.u32;
	// mr r27,r7
	var_r27 = ctx.r7.u32;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// mr r26,r8
	var_r26 = ctx.r8.u32;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r3,r1,88
	ctx.r3.s64 = ctx.r1.s64 + 88;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r31,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, var_r31);
	// addi r11,r11,10680
	ctx.r11.s64 = ctx.r11.s64 + 10680;
	// stw r30,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, var_r30);
	// stw r10,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r10.u32);
	// stw r10,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r10.u32);
	// addi r10,r1,100
	ctx.r10.s64 = ctx.r1.s64 + 100;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// li r11,24
	ctx.r11.s64 = 24;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
loc_823E6BCC:
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
	// addi r29,r29,4
	var_r29 = (uint32_t)(var_r29 + 4);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x823e6bcc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E6BCC;
	// lbz r10,200(r1)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 200);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r28,212
	ctx.r3.s64 = (int64_t)(int32_t)var_r28 + 212;
	// stw r27,196(r1)
	PPC_STORE_U32(ctx.r1.u32 + 196, var_r27);
	// rlwimi r10,r26,7,17,24
	ctx.r10.u64 = (__builtin_rotateleft32(var_r26, 7) & 0x7F80) | (ctx.r10.u64 & 0xFFFFFFFFFFFF807F);
	// stb r10,200(r1)
	PPC_STORE_U8(ctx.r1.u32 + 200, ctx.r10.u8);
	// bl 0x823eb858
	snSession_B858_fw(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6C08_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_6C08_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6C08_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// addi r31,r31,212
	var_r31 = (uint32_t)(var_r31 + 212);
	// lis r11,-32249
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r30,r11,10480
	var_r30 = (uint32_t)(ctx.r11.s64 + 10480);  // lbl_820728F0 @ 0x820728f0
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// stw r30,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e6c94
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823e6c98
	} else {
	loc_823E6C94:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E6C98:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6CB8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_6CB8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6CB8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// addi r31,r31,212
	var_r31 = (uint32_t)(var_r31 + 212);
	// lis r11,-32249
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r30,r11,10540
	var_r30 = (uint32_t)(ctx.r11.s64 + 10540);  // lbl_8207292C @ 0x8207292c
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// stw r30,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e6d44
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823e6d48
	} else {
	loc_823E6D44:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E6D48:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_6D68_h"))) PPC_WEAK_FUNC(snSession_6D68_h);
PPC_FUNC_IMPL(__imp__snSession_6D68_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// addi r31,r31,212
	var_r31 = (uint32_t)(var_r31 + 212);
	// lis r11,-32249
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r30,r11,10600
	var_r30 = (uint32_t)(ctx.r11.s64 + 10600);  // lbl_82072968 @ 0x82072968
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// stw r30,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e6df4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r9,84(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r7,88(r1)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// stw r30,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r30);
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823e6df8
	} else {
	loc_823E6DF4:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E6DF8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6E18_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_6E18_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6E18_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r24 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=288, savegprlr_24
	// lis r11,-32249
	// mr r24,r3
	var_r24 = ctx.r3.u32;
	// addi r27,r11,10340
	var_r27 = (uint32_t)(ctx.r11.s64 + 10340);  // lbl_82072864 @ 0x82072864
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// mr r28,r6
	var_r28 = ctx.r6.u32;
	// mr r31,r7
	var_r31 = ctx.r7.u32;
	// stw r27,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r27);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r3,r1,88
	ctx.r3.s64 = ctx.r1.s64 + 88;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// li r25,0
	var_r25 = 0;
	// stw r30,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, var_r30);
	// addi r26,r11,11748
	var_r26 = (uint32_t)(ctx.r11.s64 + 11748);  // lbl_82072DE4 @ 0x82072de4
	// stw r29,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, var_r29);
	// addi r3,r1,100
	ctx.r3.s64 = ctx.r1.s64 + 100;
	// stw r25,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, var_r25);
	// stw r25,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, var_r25);
	// stw r26,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, var_r26);
	// bl 0x823b3fd8
	SinglesNetworkClient_3FD8_g(ctx, base);
	// lbz r11,204(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 204);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,100
	ctx.r3.s64 = ctx.r1.s64 + 100;
	// rlwimi r11,r28,7,17,24
	ctx.r11.u64 = (__builtin_rotateleft32(var_r28, 7) & 0x7F80) | (ctx.r11.u64 & 0xFFFFFFFFFFFF807F);
	// stb r11,204(r1)
	PPC_STORE_U8(ctx.r1.u32 + 204, ctx.r11.u8);
	// bl 0x822603d0
	snJoinMachine_03D0_g(ctx, base);
	// cmplwi cr6,r31,0
	// beq cr6,0x823e6eb0
	if (var_r31 != 0) {
		// addi r3,r1,100
		ctx.r3.s64 = ctx.r1.s64 + 100;
		// lwz r6,16(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 16);
		// lwz r5,24(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 24);
		// lwz r4,4(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
		// bl 0x822608c8
		SinglesNetworkClient_08C8_g(ctx, base);
	}
loc_823E6EB0:
	// addi r30,r24,212
	var_r30 = (uint32_t)(var_r24 + 212);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,128
	ctx.r4.s64 = 128;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x823e6f34
	if (var_r31 != 0) {
		// stw r27,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ var_r27);
		// addi r4,r1,100
		ctx.r4.s64 = ctx.r1.s64 + 100;
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// addi r3,r31,20
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 20;
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r26,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ var_r26);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r31)
		PPC_STORE_U64(var_r31 + 4, ctx.r10.u64);
		// lwz r9,92(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
		// stw r9,12(r31)
		PPC_STORE_U32(var_r31 + 12, ctx.r9.u32);
		// lwz r8,96(r1)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
		// stw r8,16(r31)
		PPC_STORE_U32(var_r31 + 16, ctx.r8.u32);
		// bl 0x823ec008
		SinglesNetworkClient_C008_fw(ctx, base);
		// lbz r7,204(r1)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r1.u32 + 204);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lbz r6,124(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 124);
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// rlwimi r6,r7,0,24,24
		ctx.r6.u64 = (ctx.r7.u32 & 0x80) | (ctx.r6.u64 & 0xFFFFFFFFFFFFFF7F);
		// stb r6,124(r31)
		PPC_STORE_U8(var_r31 + 124, ctx.r6.u8);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r25,1
		var_r25 = 1;
	}
loc_823E6F34:
	// addi r3,r1,100
	ctx.r3.s64 = ctx.r1.s64 + 100;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// clrlwi r3,r25,24
	ctx.r3.u64 = var_r25 & 0xFF;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6F48_w"))) PPC_WEAK_FUNC(SinglesNetworkClient_6F48_w);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6F48_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r3,r1,88
	ctx.r3.s64 = ctx.r1.s64 + 88;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r31,r31,212
	var_r31 = (uint32_t)(var_r31 + 212);
	// li r30,0
	var_r30 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,16
	ctx.r4.s64 = 16;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// stw r30,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, var_r30);
	// stw r30,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, var_r30);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e6fec
	if (ctx.r4.u32 != 0) {
		// rotlwi r9,r30,0
		ctx.r9.u64 = var_r30;
		// stw r29,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, var_r29);
		// rotlwi r7,r30,0
		ctx.r7.u64 = var_r30;
		// rldicr r8,r9,32,63
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
		// lis r11,-32249
		// or r6,r7,r8
		ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
		// addi r11,r11,12716
		ctx.r11.s64 = ctx.r11.s64 + 12716;
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// std r6,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		return;
	}
loc_823E6FEC:
	// clrlwi r3,r30,24
	ctx.r3.u64 = var_r30 & 0xFF;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_6FF8_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_6FF8_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_6FF8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=160, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// bl 0x823e7068
	SinglesNetworkClient_7068(ctx, base);
	// ld r10,96(r31)
	ctx.r10.u64 = PPC_LOAD_U64(var_r31 + 96);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// ld r9,104(r31)
	ctx.r9.u64 = PPC_LOAD_U64(var_r31 + 104);
	// ld r8,112(r31)
	ctx.r8.u64 = PPC_LOAD_U64(var_r31 + 112);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// ld r7,168(r30)
	ctx.r7.u64 = PPC_LOAD_U64(var_r30 + 168);
	// addi r3,r30,212
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 212;
	// std r10,0(r11)
	PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r10.u64);
	// std r9,8(r11)
	PPC_STORE_U64(ctx.r11.u32 + 8, ctx.r9.u64);
	// std r8,16(r11)
	PPC_STORE_U64(ctx.r11.u32 + 16, ctx.r8.u64);
	// std r7,120(r1)
	PPC_STORE_U64(ctx.r1.u32 + 120, ctx.r7.u64);
	// bl 0x823eba30
	SinglesNetworkClient_BA30(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_7068"))) PPC_WEAK_FUNC(SinglesNetworkClient_7068);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_7068) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// addi r30,r31,4
	var_r30 = (uint32_t)(var_r31 + 4);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r29,r30,4
	var_r29 = (uint32_t)(var_r30 + 4);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r10,r11,12756
	ctx.r10.s64 = ctx.r11.s64 + 12756;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// std r11,40(r31)
	PPC_STORE_U64(var_r31 + 40, ctx.r11.u64);
	// stb r11,16(r31)
	PPC_STORE_U8(var_r31 + 16, ctx.r11.u8);
	// std r11,32(r31)
	PPC_STORE_U64(var_r31 + 32, ctx.r11.u64);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_70D0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_70D0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_70D0_g) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lwz r11,3784(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 3784);
	// addi r10,r4,10
	ctx.r10.s64 = ctx.r4.s64 + 10;
	// addi r11,r11,50
	ctx.r11.s64 = ctx.r11.s64 + 50;
	// addi r8,r11,6
	ctx.r8.s64 = ctx.r11.s64 + 6;
loc_823E70EC:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// subf. r9,r7,r9
	ctx.r9.s64 = ctx.r9.s64 - ctx.r7.s64;
	// bne 0x823e710c
	if (ctx.r9.s32 != 0) goto loc_823E710C;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r11,r8
	// bne cr6,0x823e70ec
	if (ctx.r11.s32 != ctx.r8.s32) goto loc_823E70EC;
loc_823E710C:
	// cmpwi cr6,r9,0
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x823e711c
	if (ctx.r9.s32 != 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E711C:
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r5,0
	// beq cr6,0x823e713c
	if (ctx.r5.u32 != 0) {
		// lwz r3,3788(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 3788);
		// blr
		return;
	}
loc_823E713C:
	// addi r3,r3,3772
	ctx.r3.s64 = ctx.r3.s64 + 3772;
	// bl 0x823ea7c8
	snLeaveMachine_A7C8_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823e7160
	if (ctx.r3.u32 != 0) {
		// lwz r3,16(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// blr
		return;
	}
loc_823E7160:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_7178_fw"))) PPC_WEAK_FUNC(snLeaveMachine_7178_fw);
PPC_FUNC_IMPL(__imp__snLeaveMachine_7178_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// addi r3,r3,3772
	ctx.r3.s64 = ctx.r3.s64 + 3772;
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// mr r29,r6
	var_r29 = ctx.r6.u32;
	// li r30,0
	var_r30 = 0;
	// bl 0x823ea7c8
	snLeaveMachine_A7C8_g(ctx, base);
	// mr r9,r3
	ctx.r9.u64 = ctx.r3.u64;
	// cmplwi cr6,r9,0
	// beq cr6,0x823e71e4
	if (ctx.r9.u32 != 0) {
		// lwz r3,16(r9)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
		// li r10,0
		ctx.r10.s64 = 0;
		// cmpwi cr6,r3,0
		// ble cr6,0x823e71e8
		if (ctx.r3.s32 <= 0) {
			return;
		}
		// mr r11,r31
		ctx.r11.u64 = var_r31;
		// subf r9,r31,r9
		ctx.r9.s64 = ctx.r9.s64 - (int64_t)(int32_t)var_r31;
	loc_823E71BC:
		// cmpw cr6,r10,r29
		// bge cr6,0x823e71e8
		if (ctx.r10.s32 >= (int32_t)var_r29) {
			return;
		}
		// lwzx r8,r9,r11
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpw cr6,r10,r3
		// stw r8,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// blt cr6,0x823e71bc
		if (ctx.r10.s32 < ctx.r3.s32) goto loc_823E71BC;
		return;
	}
loc_823E71E4:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
loc_823E71E8:
	return;
}

__attribute__((alias("__imp__snSession_71F0_h"))) PPC_WEAK_FUNC(snSession_71F0_h);
PPC_FUNC_IMPL(__imp__snSession_71F0_h) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,9672(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 9672);
	// cmpwi cr6,r11,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bgt cr6,0x823e7204
	if (ctx.r11.s32 <= 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E7204:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_rtti_2854_1"))) PPC_WEAK_FUNC(snSession_rtti_2854_1);
PPC_FUNC_IMPL(__imp__snSession_rtti_2854_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,232
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 232;
	// bl 0x8236f4e0
	xam_F4E0_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e71f0
	snSession_71F0_h(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823e7294
	if (ctx.r11.u32 == 0) {
		// addi r30,r31,212
		var_r30 = (uint32_t)(var_r31 + 212);
	loc_823E7244:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823ed188
		util_D188(ctx, base);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e7294
		if (ctx.r4.u32 == 0) {
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x823ed820
			hsmContext_rtti_3F08_1(ctx, base);
			return;
		}
		// lwz r10,84(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 84);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmpwi cr6,r10,0
		// bgt cr6,0x823e726c
		if (ctx.r10.s32 <= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823E726C:
		// clrlwi r8,r11,24
		ctx.r8.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// bne cr6,0x823e7280
		if (ctx.r8.u32 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x823edc40
			SinglesNetworkClient_DC40_g(ctx, base);
		}
	loc_823E7280:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823e71f0
		snSession_71F0_h(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x823e7244
		if (ctx.r7.u32 == 0) goto loc_823E7244;
	}
loc_823E7294:
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ed820
	hsmContext_rtti_3F08_1(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_72A8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_72A8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_72A8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r31,32
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 32;
	// li r10,9
	ctx.r10.s64 = 9;
	// std r9,24(r31)
	PPC_STORE_U64(var_r31 + 24, ctx.r9.u64);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E72D0:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e72d0
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E72D0;
	// li r11,-1
	// stb r9,0(r31)
	PPC_STORE_U8(var_r31 + 0, ctx.r9.u8);
	// addi r3,r31,72
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 72;
	// std r9,16(r31)
	PPC_STORE_U64(var_r31 + 16, ctx.r9.u64);
	// stw r11,68(r31)
	PPC_STORE_U32(var_r31 + 68, ctx.r11.u32);
	// bl 0x823b3fd8
	SinglesNetworkClient_3FD8_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_7310_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_7310_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_7310_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=400, savegprlr_28
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// addi r3,r1,176
	ctx.r3.s64 = ctx.r1.s64 + 176;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// bl 0x823e72a8
	SinglesNetworkClient_72A8_g(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// addi r4,r31,96
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 96;
	// addi r3,r28,232
	ctx.r3.s64 = (int64_t)(int32_t)var_r28 + 232;
	// bl 0x8236f3d8
	SinglesNetworkClient_F3D8_g(ctx, base);
	// addi r30,r31,40
	var_r30 = (uint32_t)(var_r31 + 40);
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// ld r5,168(r28)
	ctx.r5.u64 = PPC_LOAD_U64(var_r28 + 168);
	// mr r8,r29
	ctx.r8.u64 = var_r29;
	// addi r3,r1,176
	ctx.r3.s64 = ctx.r1.s64 + 176;
	// mr r6,r30
	ctx.r6.u64 = var_r30;
	// bl 0x823dd328
	SinglesNetworkClient_D328_g(ctx, base);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E7378:
	// lwz r9,0(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
	// addi r30,r30,4
	var_r30 = (uint32_t)(var_r30 + 4);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e7378
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E7378;
	// lwz r10,36(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 36);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmpwi cr6,r10,0
	// bge cr6,0x823e73a0
	if (ctx.r10.s32 < 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E73A0:
	// clrlwi r7,r11,24
	ctx.r7.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r7,0
	// bne cr6,0x823e73bc
	if (ctx.r7.u32 == 0) {
		// lbz r6,76(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 76);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmplwi cr6,r6,0
		// bne cr6,0x823e73c0
		if (ctx.r6.u32 != 0) goto loc_823E73C0;
	}
loc_823E73BC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823E73C0:
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// li r30,1
	var_r30 = 1;
	// cmplwi cr6,r4,0
	// bne cr6,0x823e73d4
	if (ctx.r4.u32 == 0) {
		// li r30,0
		var_r30 = 0;
	}
loc_823E73D4:
	// cmpwi cr6,r10,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bge cr6,0x823e73e4
	if (ctx.r10.s32 < 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E73E4:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823e7400
	if (ctx.r11.u32 == 0) {
		// lbz r10,76(r31)
		ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 76);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmplwi cr6,r10,0
		// bne cr6,0x823e7404
		if (ctx.r10.u32 != 0) goto loc_823E7404;
	}
loc_823E7400:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823E7404:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// addi r31,r1,80
	var_r31 = (uint32_t)(ctx.r1.s64 + 80);
	// cmplwi cr6,r8,0
	// bne cr6,0x823e7418
	if (ctx.r8.u32 == 0) {
		// li r31,0
		var_r31 = 0;
	}
loc_823E7418:
	// addi r4,r1,176
	ctx.r4.s64 = ctx.r1.s64 + 176;
	// addi r3,r1,128
	ctx.r3.s64 = ctx.r1.s64 + 128;
	// bl 0x823ead68
	SinglesNetworkClient_AD68_g(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r7,1
	ctx.r7.s64 = 1;
	// lwz r3,164(r28)
	ctx.r3.u64 = PPC_LOAD_U32(var_r28 + 164);
	// mr r6,r30
	ctx.r6.u64 = var_r30;
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// bl 0x823dad10
	SinglesNetworkClient_AD10_g(ctx, base);
	// addi r3,r1,128
	ctx.r3.s64 = ctx.r1.s64 + 128;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// addi r3,r1,248
	ctx.r3.s64 = ctx.r1.s64 + 248;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_7458_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_7458_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_7458_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=240, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// ld r7,96(r4)
	ctx.r7.u64 = PPC_LOAD_U64(ctx.r4.u32 + 96);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// ld r6,104(r4)
	ctx.r6.u64 = PPC_LOAD_U64(ctx.r4.u32 + 104);
	// ld r5,112(r4)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r4.u32 + 112);
	// addi r10,r4,40
	ctx.r10.s64 = ctx.r4.s64 + 40;
	// addi r9,r1,112
	ctx.r9.s64 = ctx.r1.s64 + 112;
	// li r8,9
	ctx.r8.s64 = 9;
	// ld r3,168(r29)
	ctx.r3.u64 = PPC_LOAD_U64(var_r29 + 168);
	// std r7,0(r11)
	PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r7.u64);
	// std r6,8(r11)
	PPC_STORE_U64(ctx.r11.u32 + 8, ctx.r6.u64);
	// std r5,16(r11)
	PPC_STORE_U64(ctx.r11.u32 + 16, ctx.r5.u64);
	// std r3,104(r1)
	PPC_STORE_U64(ctx.r1.u32 + 104, ctx.r3.u64);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
loc_823E749C:
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// stw r11,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r11.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bdnz 0x823e749c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E749C;
	// lwz r9,36(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 36);
	// li r11,0
	ctx.r11.s64 = 0;
	// li r10,1
	ctx.r10.s64 = 1;
	// cmpwi cr6,r9,0
	// bge cr6,0x823e74c8
	if (ctx.r9.s32 < 0) {
		// mr r10,r11
		ctx.r10.u64 = ctx.r11.u64;
	}
loc_823E74C8:
	// clrlwi r8,r10,24
	ctx.r8.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r8,0
	// bne cr6,0x823e74e4
	if (ctx.r8.u32 == 0) {
		// lbz r7,76(r4)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r4.u32 + 76);
		// li r10,1
		ctx.r10.s64 = 1;
		// cmplwi cr6,r7,0
		// bne cr6,0x823e74e8
		if (ctx.r7.u32 != 0) goto loc_823E74E8;
	}
loc_823E74E4:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_823E74E8:
	// clrlwi r5,r10,24
	ctx.r5.u64 = ctx.r10.u32 & 0xFF;
	// li r30,1
	var_r30 = 1;
	// cmplwi cr6,r5,0
	// bne cr6,0x823e74fc
	if (ctx.r5.u32 == 0) {
		// mr r30,r11
		var_r30 = ctx.r11.u32;
	}
loc_823E74FC:
	// cmpwi cr6,r9,0
	// li r10,1
	ctx.r10.s64 = 1;
	// bge cr6,0x823e750c
	if (ctx.r9.s32 < 0) {
		// mr r10,r11
		ctx.r10.u64 = ctx.r11.u64;
	}
loc_823E750C:
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bne cr6,0x823e7528
	if (ctx.r10.u32 == 0) {
		// lbz r9,76(r4)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r4.u32 + 76);
		// li r10,1
		ctx.r10.s64 = 1;
		// cmplwi cr6,r9,0
		// bne cr6,0x823e752c
		if (ctx.r9.u32 != 0) goto loc_823E752C;
	}
loc_823E7528:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_823E752C:
	// clrlwi r7,r10,24
	ctx.r7.u64 = ctx.r10.u32 & 0xFF;
	// addi r31,r1,112
	var_r31 = (uint32_t)(ctx.r1.s64 + 112);
	// cmplwi cr6,r7,0
	// bne cr6,0x823e7540
	if (ctx.r7.u32 == 0) {
		// mr r31,r11
		var_r31 = ctx.r11.u32;
	}
loc_823E7540:
	// lis r10,-32161
	// stw r11,160(r1)
	PPC_STORE_U32(ctx.r1.u32 + 160, ctx.r11.u32);
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// stw r11,164(r1)
	PPC_STORE_U32(ctx.r1.u32 + 164, ctx.r11.u32);
	// stw r11,168(r1)
	PPC_STORE_U32(ctx.r1.u32 + 168, ctx.r11.u32);
	// stw r11,172(r1)
	PPC_STORE_U32(ctx.r1.u32 + 172, ctx.r11.u32);
	// stw r11,176(r1)
	PPC_STORE_U32(ctx.r1.u32 + 176, ctx.r11.u32);
	// lwz r10,13260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 13260);
	// stw r11,180(r1)
	PPC_STORE_U32(ctx.r1.u32 + 180, ctx.r11.u32);
	// stw r11,184(r1)
	PPC_STORE_U32(ctx.r1.u32 + 184, ctx.r11.u32);
	// stw r11,188(r1)
	PPC_STORE_U32(ctx.r1.u32 + 188, ctx.r11.u32);
	// stw r11,192(r1)
	PPC_STORE_U32(ctx.r1.u32 + 192, ctx.r11.u32);
	// stw r10,196(r1)
	PPC_STORE_U32(ctx.r1.u32 + 196, ctx.r10.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32163
	// li r6,8128
	ctx.r6.s64 = 8128;
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// lwz r4,7048(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7048);
	// stw r6,180(r1)
	PPC_STORE_U32(ctx.r1.u32 + 180, ctx.r6.u32);
	// bl 0x822386f0
	SinglesNetworkClient_86F0_g(ctx, base);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// bl 0x823f3e78
	SinglesNetworkClient_3E78_g(ctx, base);
	// clrlwi r5,r3,24
	ctx.r5.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r5,0
	// beq cr6,0x823e75c0
	if (ctx.r5.u32 != 0) {
		// li r5,64
		ctx.r5.s64 = 64;
		// ld r4,104(r1)
		ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 104);
		// addi r3,r1,160
		ctx.r3.s64 = ctx.r1.s64 + 160;
		// bl 0x823dd1f0
		SinglesNetworkClient_D1F0_g(ctx, base);
	}
loc_823E75C0:
	// li r7,1
	ctx.r7.s64 = 1;
	// lwz r3,164(r29)
	ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 164);
	// mr r6,r30
	ctx.r6.u64 = var_r30;
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// addi r4,r1,160
	ctx.r4.s64 = ctx.r1.s64 + 160;
	// bl 0x823dad10
	SinglesNetworkClient_AD10_g(ctx, base);
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_75E8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_75E8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_75E8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_29
	// lis r11,-32161
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r11,13260(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 13260);
	// ld r29,168(r31)
	var_r29 = (uint32_t)(PPC_LOAD_U64(var_r31 + 168));
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// stw r11,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,104(r1)
	PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,112(r1)
	PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32163
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r4,7192(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7192);
	// li r11,8128
	ctx.r11.s64 = 8128;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x822386f0
	SinglesNetworkClient_86F0_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// beq cr6,0x823e7684
	if (ctx.r10.u32 != 0) {
		// li r5,64
		ctx.r5.s64 = 64;
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dd1f0
		SinglesNetworkClient_D1F0_g(ctx, base);
	}
loc_823E7684:
	// lbz r9,3744(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 3744);
	// rlwinm r8,r9,0,0,24
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r8,0
	// beq cr6,0x823e76a0
	if (ctx.r8.u32 != 0) {
		// lwz r11,3784(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 3784);
		// addi r4,r11,40
		ctx.r4.s64 = ctx.r11.s64 + 40;
		// b 0x823e76a4
	} else {
	loc_823E76A0:
		// addi r4,r31,176
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 176;
	}
loc_823E76A4:
	// lwz r31,164(r31)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 164));
	// addi r3,r31,636
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 636;
	// bl 0x823dc2b0
	SinglesNetworkClient_C2B0_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823e76d0
	if (ctx.r3.u32 != 0) {
		// lwz r4,84(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 84);
		// li r7,0
		ctx.r7.s64 = 0;
		// li r6,1
		ctx.r6.s64 = 1;
		// addi r5,r1,80
		ctx.r5.s64 = ctx.r1.s64 + 80;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823daea0
		SinglesNetworkClient_AEA0_g(ctx, base);
	}
loc_823E76D0:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_76E0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_76E0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_76E0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=1392, savegprlr_26
	// mr r29,r6
	var_r29 = ctx.r6.u32;
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// mr r26,r30
	var_r26 = (uint32_t)(var_r30);
	// bl 0x82238758
	SinglesNetworkClient_8758_g(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,7332(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7332);
	// cmplw cr6,r11,r3
	// bne cr6,0x823e7770
	if (ctx.r11.u32 == ctx.r3.u32) {
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// lwz r3,164(r27)
		ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 164);
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// bl 0x823daaf0
		SinglesNetworkClient_AAF0_g(ctx, base);
		// clrlwi r11,r3,24
		ctx.r11.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r11,0
		// beq cr6,0x823e7ed8
		if (ctx.r11.u32 == 0) goto loc_823E7ED8;
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r1,784
		ctx.r3.s64 = ctx.r1.s64 + 784;
		// bl 0x823dd818
		SinglesNetworkClient_D818(ctx, base);
		// lis r11,-32249
		// addi r4,r1,784
		ctx.r4.s64 = ctx.r1.s64 + 784;
		// addi r11,r11,10740
		ctx.r11.s64 = ctx.r11.s64 + 10740;
		// addi r3,r27,212
		ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
		// stw r11,784(r1)
		PPC_STORE_U32(ctx.r1.u32 + 784, ctx.r11.u32);
		// bl 0x823ebbd0
		snSession_BBD0_fw(ctx, base);
		// li r26,2
		var_r26 = 2;
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		return;
	}
loc_823E7770:
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,7368(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7368);
	// cmplw cr6,r11,r3
	// bne cr6,0x823e77d4
	if (ctx.r11.u32 == ctx.r3.u32) {
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// lwz r3,164(r27)
		ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 164);
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// bl 0x823daaf0
		SinglesNetworkClient_AAF0_g(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823e7ed8
		if (ctx.r10.u32 == 0) goto loc_823E7ED8;
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r1,720
		ctx.r3.s64 = ctx.r1.s64 + 720;
		// bl 0x823dd818
		SinglesNetworkClient_D818(ctx, base);
		// lis r11,-32249
		// addi r4,r1,720
		ctx.r4.s64 = ctx.r1.s64 + 720;
		// addi r11,r11,10760
		ctx.r11.s64 = ctx.r11.s64 + 10760;
		// addi r3,r27,212
		ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
		// stw r11,720(r1)
		PPC_STORE_U32(ctx.r1.u32 + 720, ctx.r11.u32);
		// bl 0x823ebcb0
		snSession_BCB0_fw(ctx, base);
		// li r26,2
		var_r26 = 2;
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		return;
	}
loc_823E77D4:
	// lwz r9,1992(r27)
	ctx.r9.u64 = PPC_LOAD_U32(var_r27 + 1992);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmplwi cr6,r9,0
	// bne cr6,0x823e77e8
	if (ctx.r9.u32 == 0) {
		// mr r11,r30
		ctx.r11.u64 = var_r30;
	}
loc_823E77E8:
	// clrlwi r7,r11,24
	ctx.r7.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r7,0
	// beq cr6,0x823e7edc
	if (ctx.r7.u32 != 0) {
		// lbz r6,3744(r27)
		ctx.r6.u64 = PPC_LOAD_U8(var_r27 + 3744);
		// rlwinm r5,r6,0,0,24
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e7ac4
		if (ctx.r5.u32 != 0) {
			// lis r11,-32163
			ctx.r11.s64 = -2107834368;
			// lwz r11,6988(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6988);
			// cmplw cr6,r11,r3
			// bne cr6,0x823e79a0
			if (ctx.r11.u32 == ctx.r3.u32) {
				// addi r11,r1,212
				ctx.r11.s64 = ctx.r1.s64 + 212;
				// mr r9,r30
				ctx.r9.u64 = var_r30;
				// li r10,9
				ctx.r10.s64 = 9;
				// mtctr r10
				ctx.ctr.u64 = ctx.r10.u64;
			loc_823E7824:
				// stw r9,0(r11)
				PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
				// addi r11,r11,4
				ctx.r11.s64 = ctx.r11.s64 + 4;
				// bdnz 0x823e7824
				--ctx.ctr.u64;
				if (ctx.ctr.u32 != 0) goto loc_823E7824;
				// addi r11,r1,152
				ctx.r11.s64 = ctx.r1.s64 + 152;
				// mr r10,r30
				ctx.r10.u64 = var_r30;
				// li r9,15
				ctx.r9.s64 = 15;
				// mtctr r9
				ctx.ctr.u64 = ctx.r9.u64;
			loc_823E7840:
				// stw r10,0(r11)
				PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
				// addi r11,r11,4
				ctx.r11.s64 = ctx.r11.s64 + 4;
				// bdnz 0x823e7840
				--ctx.ctr.u64;
				if (ctx.ctr.u32 != 0) goto loc_823E7840;
				// addi r11,r1,212
				ctx.r11.s64 = ctx.r1.s64 + 212;
				// mr r9,r30
				ctx.r9.u64 = var_r30;
				// li r10,9
				ctx.r10.s64 = 9;
				// mtctr r10
				ctx.ctr.u64 = ctx.r10.u64;
			loc_823E785C:
				// stw r9,0(r11)
				PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
				// addi r11,r11,4
				ctx.r11.s64 = ctx.r11.s64 + 4;
				// bdnz 0x823e785c
				--ctx.ctr.u64;
				if (ctx.ctr.u32 != 0) goto loc_823E785C;
				// li r5,64
				ctx.r5.s64 = 64;
				// addi r4,r1,144
				ctx.r4.s64 = ctx.r1.s64 + 144;
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x822609f0
				SinglesNetworkClient_09F0_g(ctx, base);
				// clrlwi r4,r3,24
				ctx.r4.u64 = ctx.r3.u32 & 0xFF;
				// cmplwi cr6,r4,0
				// beq cr6,0x823e7894
				if (ctx.r4.u32 != 0) {
					// li r5,96
					ctx.r5.s64 = 96;
					// addi r4,r1,152
					ctx.r4.s64 = ctx.r1.s64 + 152;
					// mr r3,r29
					ctx.r3.u64 = var_r29;
					// bl 0x82260af0
					util_0AF0(ctx, base);
				}
			loc_823E7894:
				// addi r11,r1,152
				ctx.r11.s64 = ctx.r1.s64 + 152;
				// addi r10,r27,304
				ctx.r10.s64 = (int64_t)(int32_t)var_r27 + 304;
				// addi r8,r11,60
				ctx.r8.s64 = ctx.r11.s64 + 60;
			loc_823E78A0:
				// lbz r3,0(r11)
				ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
				// lbz r9,0(r10)
				ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
				// subf. r9,r9,r3
				ctx.r9.s64 = ctx.r3.s64 - ctx.r9.s64;
				// bne 0x823e78c0
				if (ctx.r9.s32 != 0) goto loc_823E78C0;
				// addi r11,r11,1
				ctx.r11.s64 = ctx.r11.s64 + 1;
				// addi r10,r10,1
				ctx.r10.s64 = ctx.r10.s64 + 1;
				// cmpw cr6,r11,r8
				// bne cr6,0x823e78a0
				if (ctx.r11.s32 != ctx.r8.s32) goto loc_823E78A0;
			loc_823E78C0:
				// cmpwi cr6,r9,0
				// li r11,1
				ctx.r11.s64 = 1;
				// beq cr6,0x823e78d0
				if (ctx.r9.s32 != 0) {
					// mr r11,r30
					ctx.r11.u64 = var_r30;
				}
			loc_823E78D0:
				// clrlwi r7,r11,24
				ctx.r7.u64 = ctx.r11.u32 & 0xFF;
				// cmplwi cr6,r7,0
				// beq cr6,0x823e7edc
				if (ctx.r7.u32 == 0) {
					// mr r3,r26
					ctx.r3.u64 = var_r26;
					return;
				}
				// addi r11,r1,1052
				ctx.r11.s64 = ctx.r1.s64 + 1052;
				// mr r10,r30
				ctx.r10.u64 = var_r30;
				// li r9,12
				ctx.r9.s64 = 12;
				// mtctr r9
				ctx.ctr.u64 = ctx.r9.u64;
			loc_823E78EC:
				// stb r10,0(r11)
				PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
				// addi r11,r11,1
				ctx.r11.s64 = ctx.r11.s64 + 1;
				// bdnz 0x823e78ec
				--ctx.ctr.u64;
				if (ctx.ctr.u32 != 0) goto loc_823E78EC;
				// lwz r6,272(r27)
				ctx.r6.u64 = PPC_LOAD_U32(var_r27 + 272);
				// li r8,1
				ctx.r8.s64 = 1;
				// cmpwi cr6,r6,3
				// beq cr6,0x823e790c
				if (ctx.r6.s32 != 3) {
					// mr r8,r30
					ctx.r8.u64 = var_r30;
				}
			loc_823E790C:
				// ld r5,168(r27)
				ctx.r5.u64 = PPC_LOAD_U64(var_r27 + 168);
				// addi r11,r27,400
				ctx.r11.s64 = (int64_t)(int32_t)var_r27 + 400;
				// ld r4,144(r1)
				ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 144);
				// addi r10,r1,1040
				ctx.r10.s64 = ctx.r1.s64 + 1040;
				// li r9,35
				ctx.r9.s64 = 35;
				// std r5,1032(r1)
				PPC_STORE_U64(ctx.r1.u32 + 1032, ctx.r5.u64);
				// std r4,1024(r1)
				PPC_STORE_U64(ctx.r1.u32 + 1024, ctx.r4.u64);
				// mtctr r9
				ctx.ctr.u64 = ctx.r9.u64;
			loc_823E792C:
				// ld r3,0(r11)
				ctx.r3.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
				// addi r11,r11,8
				ctx.r11.s64 = ctx.r11.s64 + 8;
				// std r3,0(r10)
				PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r3.u64);
				// addi r10,r10,8
				ctx.r10.s64 = ctx.r10.s64 + 8;
				// bdnz 0x823e792c
				--ctx.ctr.u64;
				if (ctx.ctr.u32 != 0) goto loc_823E792C;
				// addi r6,r1,80
				ctx.r6.s64 = ctx.r1.s64 + 80;
				// lwz r3,164(r27)
				ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 164);
				// mr r5,r27
				ctx.r5.u64 = var_r27;
				// stb r8,1320(r1)
				PPC_STORE_U8(ctx.r1.u32 + 1320, ctx.r8.u8);
				// mr r4,r28
				ctx.r4.u64 = var_r28;
				// stw r30,80(r1)
				PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
				// sth r30,84(r1)
				PPC_STORE_U16(ctx.r1.u32 + 84, (uint16_t)var_r30);
				// bl 0x823daa40
				SinglesNetworkClient_AA40_g(ctx, base);
				// clrlwi r10,r3,24
				ctx.r10.u64 = ctx.r3.u32 & 0xFF;
				// cmplwi cr6,r10,0
				// beq cr6,0x823e7ed8
				if (ctx.r10.u32 == 0) goto loc_823E7ED8;
				// addi r4,r1,1024
				ctx.r4.s64 = ctx.r1.s64 + 1024;
				// addi r3,r1,848
				ctx.r3.s64 = ctx.r1.s64 + 848;
				// bl 0x823eae20
				SinglesNetworkClient_AE20_g(ctx, base);
				// mr r5,r3
				ctx.r5.u64 = ctx.r3.u64;
				// addi r4,r1,80
				ctx.r4.s64 = ctx.r1.s64 + 80;
				// lwz r3,164(r27)
				ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 164);
				// bl 0x82414098
				SinglesNetworkClient_4098_g(ctx, base);
				// addi r3,r1,848
				ctx.r3.s64 = ctx.r1.s64 + 848;
				// bl 0x82260268
				SinglesNetworkClient_0268_g(ctx, base);
				// li r26,2
				var_r26 = 2;
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
		loc_823E79A0:
			// lis r11,-32163
			ctx.r11.s64 = -2107834368;
			// lwz r11,7024(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7024);
			// cmplw cr6,r11,r3
			// bne cr6,0x823e7a74
			if (ctx.r11.u32 == ctx.r3.u32) {
				// lis r11,-32249
				// addi r3,r1,516
				ctx.r3.s64 = ctx.r1.s64 + 516;
				// addi r11,r11,10340
				ctx.r11.s64 = ctx.r11.s64 + 10340;
				// stw r11,512(r1)
				PPC_STORE_U32(ctx.r1.u32 + 512, ctx.r11.u32);
				// bl 0x82121b00
				ke_1B00(ctx, base);
				// addi r3,r1,520
				ctx.r3.s64 = ctx.r1.s64 + 520;
				// bl 0x82121b00
				ke_1B00(ctx, base);
				// lis r11,-32249
				// addi r3,r1,528
				ctx.r3.s64 = ctx.r1.s64 + 528;
				// stw r30,520(r1)
				PPC_STORE_U32(ctx.r1.u32 + 520, var_r30);
				// addi r11,r11,11788
				ctx.r11.s64 = ctx.r11.s64 + 11788;
				// stw r30,516(r1)
				PPC_STORE_U32(ctx.r1.u32 + 516, var_r30);
				// stw r11,512(r1)
				PPC_STORE_U32(ctx.r1.u32 + 512, ctx.r11.u32);
				// bl 0x823dd3c8
				SinglesNetworkClient_D3C8_g(ctx, base);
				// mr r4,r29
				ctx.r4.u64 = var_r29;
				// addi r3,r1,528
				ctx.r3.s64 = ctx.r1.s64 + 528;
				// bl 0x823dd258
				SinglesNetworkClient_D258_g(ctx, base);
				// clrlwi r9,r3,24
				ctx.r9.u64 = ctx.r3.u32 & 0xFF;
				// cmplwi cr6,r9,0
				// beq cr6,0x823e7a40
				if (ctx.r9.u32 != 0) {
					// addi r4,r1,600
					ctx.r4.s64 = ctx.r1.s64 + 600;
					// mr r3,r29
					ctx.r3.u64 = var_r29;
					// bl 0x823e9348
					SinglesNetworkClient_9348_g(ctx, base);
					// clrlwi r8,r3,24
					ctx.r8.u64 = ctx.r3.u32 & 0xFF;
					// cmplwi cr6,r8,0
					// beq cr6,0x823e7a40
					if (ctx.r8.u32 == 0) goto loc_823E7A40;
					// li r5,1
					ctx.r5.s64 = 1;
					// addi r4,r1,80
					ctx.r4.s64 = ctx.r1.s64 + 80;
					// mr r3,r29
					ctx.r3.u64 = var_r29;
					// bl 0x82238df8
					SinglesNetworkClient_8DF8_g(ctx, base);
					// lwz r7,80(r1)
					ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
					// li r11,1
					ctx.r11.s64 = 1;
					// cmplwi cr6,r7,0
					// bne cr6,0x823e7a3c
					if (ctx.r7.u32 == 0) {
						// mr r11,r30
						ctx.r11.u64 = var_r30;
					}
				loc_823E7A3C:
					// stb r11,704(r1)
					PPC_STORE_U8(ctx.r1.u32 + 704, ctx.r11.u8);
				}
			loc_823E7A40:
				// ld r5,168(r27)
				ctx.r5.u64 = PPC_LOAD_U64(var_r27 + 168);
				// ld r4,552(r1)
				ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 552);
				// cmpld cr6,r5,r4
				// bne cr6,0x823e7a60
				if (ctx.r5.u64 == ctx.r4.u64) {
					// addi r4,r1,512
					ctx.r4.s64 = ctx.r1.s64 + 512;
					// addi r3,r27,212
					ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
					// bl 0x823ebd90
					SinglesNetworkClient_BD90_h(ctx, base);
					// li r26,2
					var_r26 = 2;
				}
			loc_823E7A60:
				// addi r3,r1,600
				ctx.r3.s64 = ctx.r1.s64 + 600;
				// bl 0x82260268
				SinglesNetworkClient_0268_g(ctx, base);
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
		loc_823E7A74:
			// lis r11,-32163
			ctx.r11.s64 = -2107834368;
			// lwz r11,7060(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7060);
			// cmplw cr6,r11,r3
			// bne cr6,0x823e7edc
			if (ctx.r11.u32 != ctx.r3.u32) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// addi r3,r1,272
			ctx.r3.s64 = ctx.r1.s64 + 272;
			// bl 0x823e7068
			SinglesNetworkClient_7068(ctx, base);
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// addi r3,r1,288
			ctx.r3.s64 = ctx.r1.s64 + 288;
			// bl 0x823dd4a0
			SinglesNetworkClient_D4A0_g(ctx, base);
			// ld r3,168(r27)
			ctx.r3.u64 = PPC_LOAD_U64(var_r27 + 168);
			// ld r11,312(r1)
			ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 312);
			// cmpld cr6,r3,r11
			// bne cr6,0x823e7edc
			if (ctx.r3.u64 != ctx.r11.u64) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// addi r4,r1,272
			ctx.r4.s64 = ctx.r1.s64 + 272;
			// addi r3,r27,212
			ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
			// bl 0x823eba30
			SinglesNetworkClient_BA30(ctx, base);
			// li r26,2
			var_r26 = 2;
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
	loc_823E7AC4:
		// cmpwi cr6,r31,0
		// blt cr6,0x823e7edc
		if ((int32_t)var_r31 < 0) {
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
		// addi r11,r27,176
		ctx.r11.s64 = (int64_t)(int32_t)var_r27 + 176;
		// addi r10,r28,10
		ctx.r10.s64 = (int64_t)(int32_t)var_r28 + 10;
		// addi r11,r11,10
		ctx.r11.s64 = ctx.r11.s64 + 10;
		// addi r8,r11,6
		ctx.r8.s64 = ctx.r11.s64 + 6;
	loc_823E7ADC:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r7,0(r10)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// subf. r9,r7,r9
		ctx.r9.s64 = ctx.r9.s64 - ctx.r7.s64;
		// bne 0x823e7afc
		if (ctx.r9.s32 != 0) goto loc_823E7AFC;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpw cr6,r11,r8
		// bne cr6,0x823e7adc
		if (ctx.r11.s32 != ctx.r8.s32) goto loc_823E7ADC;
	loc_823E7AFC:
		// cmpwi cr6,r9,0
		// li r11,1
		ctx.r11.s64 = 1;
		// beq cr6,0x823e7b0c
		if (ctx.r9.s32 != 0) {
			// mr r11,r30
			ctx.r11.u64 = var_r30;
		}
	loc_823E7B0C:
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e7edc
		if (ctx.r5.u32 == 0) {
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,7108(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7108);
		// cmplw cr6,r11,r3
		// bne cr6,0x823e7c54
		if (ctx.r11.u32 == ctx.r3.u32) {
			// stw r30,152(r1)
			PPC_STORE_U32(ctx.r1.u32 + 152, var_r30);
			// addi r11,r1,228
			ctx.r11.s64 = ctx.r1.s64 + 228;
			// mr r9,r30
			ctx.r9.u64 = var_r30;
			// li r10,9
			ctx.r10.s64 = 9;
			// mtctr r10
			ctx.ctr.u64 = ctx.r10.u64;
		loc_823E7B3C:
			// stw r9,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
			// addi r11,r11,4
			ctx.r11.s64 = ctx.r11.s64 + 4;
			// bdnz 0x823e7b3c
			--ctx.ctr.u64;
			if (ctx.ctr.u32 != 0) goto loc_823E7B3C;
			// addi r11,r1,168
			ctx.r11.s64 = ctx.r1.s64 + 168;
			// mr r10,r30
			ctx.r10.u64 = var_r30;
			// li r9,15
			ctx.r9.s64 = 15;
			// mtctr r9
			ctx.ctr.u64 = ctx.r9.u64;
		loc_823E7B58:
			// stw r10,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
			// addi r11,r11,4
			ctx.r11.s64 = ctx.r11.s64 + 4;
			// bdnz 0x823e7b58
			--ctx.ctr.u64;
			if (ctx.ctr.u32 != 0) goto loc_823E7B58;
			// addi r11,r1,228
			ctx.r11.s64 = ctx.r1.s64 + 228;
			// mr r9,r30
			ctx.r9.u64 = var_r30;
			// li r10,9
			ctx.r10.s64 = 9;
			// mtctr r10
			ctx.ctr.u64 = ctx.r10.u64;
		loc_823E7B74:
			// stw r9,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
			// addi r11,r11,4
			ctx.r11.s64 = ctx.r11.s64 + 4;
			// bdnz 0x823e7b74
			--ctx.ctr.u64;
			if (ctx.ctr.u32 != 0) goto loc_823E7B74;
			// li r5,64
			ctx.r5.s64 = 64;
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x822609f0
			SinglesNetworkClient_09F0_g(ctx, base);
			// ld r4,80(r1)
			ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
			// clrlwi r3,r3,24
			ctx.r3.u64 = ctx.r3.u32 & 0xFF;
			// cmplwi cr6,r3,0
			// std r4,144(r1)
			PPC_STORE_U64(ctx.r1.u32 + 144, ctx.r4.u64);
			// beq cr6,0x823e7bec
			if (ctx.r3.u32 != 0) {
				// li r5,32
				ctx.r5.s64 = 32;
				// addi r4,r1,152
				ctx.r4.s64 = ctx.r1.s64 + 152;
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x82238df8
				SinglesNetworkClient_8DF8_g(ctx, base);
				// clrlwi r11,r3,24
				ctx.r11.u64 = ctx.r3.u32 & 0xFF;
				// cmplwi cr6,r11,0
				// beq cr6,0x823e7bec
				if (ctx.r11.u32 == 0) goto loc_823E7BEC;
				// li r5,64
				ctx.r5.s64 = 64;
				// addi r4,r1,160
				ctx.r4.s64 = ctx.r1.s64 + 160;
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x822609f0
				SinglesNetworkClient_09F0_g(ctx, base);
				// clrlwi r10,r3,24
				ctx.r10.u64 = ctx.r3.u32 & 0xFF;
				// cmplwi cr6,r10,0
				// beq cr6,0x823e7bec
				if (ctx.r10.u32 == 0) goto loc_823E7BEC;
				// li r5,96
				ctx.r5.s64 = 96;
				// addi r4,r1,168
				ctx.r4.s64 = ctx.r1.s64 + 168;
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x82260af0
				util_0AF0(ctx, base);
			}
		loc_823E7BEC:
			// ld r9,168(r27)
			ctx.r9.u64 = PPC_LOAD_U64(var_r27 + 168);
			// ld r8,144(r1)
			ctx.r8.u64 = PPC_LOAD_U64(ctx.r1.u32 + 144);
			// cmpld cr6,r9,r8
			// bne cr6,0x823e7edc
			if (ctx.r9.u64 != ctx.r8.u64) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// addi r3,r1,896
			ctx.r3.s64 = ctx.r1.s64 + 896;
			// bl 0x823dd770
			SinglesNetworkClient_D770_g(ctx, base);
			// lwz r7,152(r1)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 152);
			// addi r10,r1,912
			ctx.r10.s64 = ctx.r1.s64 + 912;
			// addi r11,r1,168
			ctx.r11.s64 = ctx.r1.s64 + 168;
			// li r9,12
			ctx.r9.s64 = 12;
			// stw r7,908(r1)
			PPC_STORE_U32(ctx.r1.u32 + 908, ctx.r7.u32);
			// mtctr r9
			ctx.ctr.u64 = ctx.r9.u64;
		loc_823E7C1C:
			// ld r6,0(r11)
			ctx.r6.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
			// addi r11,r11,8
			ctx.r11.s64 = ctx.r11.s64 + 8;
			// std r6,0(r10)
			PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r6.u64);
			// addi r10,r10,8
			ctx.r10.s64 = ctx.r10.s64 + 8;
			// bdnz 0x823e7c1c
			--ctx.ctr.u64;
			if (ctx.ctr.u32 != 0) goto loc_823E7C1C;
			// ld r5,160(r1)
			ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 160);
			// addi r4,r1,896
			ctx.r4.s64 = ctx.r1.s64 + 896;
			// addi r3,r27,212
			ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
			// std r5,1008(r1)
			PPC_STORE_U64(ctx.r1.u32 + 1008, ctx.r5.u64);
			// bl 0x823eb950
			game_B950(ctx, base);
			// li r26,2
			var_r26 = 2;
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
	loc_823E7C54:
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,7012(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7012);
		// cmplw cr6,r11,r3
		// bne cr6,0x823e7d1c
		if (ctx.r11.u32 == ctx.r3.u32) {
			// lis r11,-32249
			// addi r3,r1,324
			ctx.r3.s64 = ctx.r1.s64 + 324;
			// addi r11,r11,10340
			ctx.r11.s64 = ctx.r11.s64 + 10340;
			// stw r11,320(r1)
			PPC_STORE_U32(ctx.r1.u32 + 320, ctx.r11.u32);
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// addi r3,r1,328
			ctx.r3.s64 = ctx.r1.s64 + 328;
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// lis r11,-32249
			// addi r3,r1,336
			ctx.r3.s64 = ctx.r1.s64 + 336;
			// stw r30,328(r1)
			PPC_STORE_U32(ctx.r1.u32 + 328, var_r30);
			// addi r11,r11,11768
			ctx.r11.s64 = ctx.r11.s64 + 11768;
			// stw r30,324(r1)
			PPC_STORE_U32(ctx.r1.u32 + 324, var_r30);
			// stw r11,320(r1)
			PPC_STORE_U32(ctx.r1.u32 + 320, ctx.r11.u32);
			// bl 0x823e72a8
			SinglesNetworkClient_72A8_g(ctx, base);
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// addi r3,r1,336
			ctx.r3.s64 = ctx.r1.s64 + 336;
			// bl 0x823dd258
			SinglesNetworkClient_D258_g(ctx, base);
			// clrlwi r4,r3,24
			ctx.r4.u64 = ctx.r3.u32 & 0xFF;
			// cmplwi cr6,r4,0
			// beq cr6,0x823e7cc0
			if (ctx.r4.u32 != 0) {
				// addi r4,r1,408
				ctx.r4.s64 = ctx.r1.s64 + 408;
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x823e9348
				SinglesNetworkClient_9348_g(ctx, base);
			}
		loc_823E7CC0:
			// ld r3,168(r27)
			ctx.r3.u64 = PPC_LOAD_U64(var_r27 + 168);
			// ld r11,360(r1)
			ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 360);
			// cmpld cr6,r3,r11
			// bne cr6,0x823e7d08
			if (ctx.r3.u64 == ctx.r11.u64) {
				// addi r4,r1,336
				ctx.r4.s64 = ctx.r1.s64 + 336;
				// addi r3,r27,3756
				ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 3756;
				// bl 0x823ea250
				SinglesNetworkClient_A250_g(ctx, base);
				// cmplwi cr6,r3,0
				// li r11,1
				ctx.r11.s64 = 1;
				// bne cr6,0x823e7cec
				if (ctx.r3.u32 == 0) {
					// mr r11,r30
					ctx.r11.u64 = var_r30;
				}
			loc_823E7CEC:
				// clrlwi r9,r11,24
				ctx.r9.u64 = ctx.r11.u32 & 0xFF;
				// cmplwi cr6,r9,0
				// bne cr6,0x823e7d04
				if (ctx.r9.u32 == 0) {
					// addi r4,r1,320
					ctx.r4.s64 = ctx.r1.s64 + 320;
					// addi r3,r27,212
					ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
					// bl 0x823ebe68
					SinglesNetworkClient_BE68_h(ctx, base);
				}
			loc_823E7D04:
				// li r26,2
				var_r26 = 2;
			}
		loc_823E7D08:
			// addi r3,r1,408
			ctx.r3.s64 = ctx.r1.s64 + 408;
			// bl 0x82260268
			SinglesNetworkClient_0268_g(ctx, base);
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
	loc_823E7D1C:
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,7048(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7048);
		// cmplw cr6,r11,r3
		// bne cr6,0x823e7da0
		if (ctx.r11.u32 == ctx.r3.u32) {
			// lis r11,-32249
			// addi r3,r1,100
			ctx.r3.s64 = ctx.r1.s64 + 100;
			// addi r11,r11,10340
			ctx.r11.s64 = ctx.r11.s64 + 10340;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// addi r3,r1,104
			ctx.r3.s64 = ctx.r1.s64 + 104;
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// lis r11,-32249
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// stw r30,104(r1)
			PPC_STORE_U32(ctx.r1.u32 + 104, var_r30);
			// addi r11,r11,12736
			ctx.r11.s64 = ctx.r11.s64 + 12736;
			// stw r30,100(r1)
			PPC_STORE_U32(ctx.r1.u32 + 100, var_r30);
			// addi r3,r1,112
			ctx.r3.s64 = ctx.r1.s64 + 112;
			// std r30,136(r1)
			PPC_STORE_U64(ctx.r1.u32 + 136, var_r30);
			// stb r30,112(r1)
			PPC_STORE_U8(ctx.r1.u32 + 112, (uint8_t)var_r30);
			// std r30,128(r1)
			PPC_STORE_U64(ctx.r1.u32 + 128, var_r30);
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823dd4a0
			SinglesNetworkClient_D4A0_g(ctx, base);
			// ld r8,168(r27)
			ctx.r8.u64 = PPC_LOAD_U64(var_r27 + 168);
			// ld r7,136(r1)
			ctx.r7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 136);
			// cmpld cr6,r8,r7
			// bne cr6,0x823e7edc
			if (ctx.r8.u64 != ctx.r7.u64) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r3,r27,212
			ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 212;
			// bl 0x823ebf38
			snSession_BF38_fw(ctx, base);
			// li r26,2
			var_r26 = 2;
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
	loc_823E7DA0:
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,7180(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7180);
		// cmplw cr6,r11,r3
		// bne cr6,0x823e7e58
		if (ctx.r11.u32 == ctx.r3.u32) {
			// lis r11,-32249
			// addi r3,r1,100
			ctx.r3.s64 = ctx.r1.s64 + 100;
			// addi r11,r11,10340
			ctx.r11.s64 = ctx.r11.s64 + 10340;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// addi r3,r1,104
			ctx.r3.s64 = ctx.r1.s64 + 104;
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// li r5,64
			ctx.r5.s64 = 64;
			// addi r4,r1,112
			ctx.r4.s64 = ctx.r1.s64 + 112;
			// stw r30,104(r1)
			PPC_STORE_U32(ctx.r1.u32 + 104, var_r30);
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// stw r30,100(r1)
			PPC_STORE_U32(ctx.r1.u32 + 100, var_r30);
			// bl 0x822609f0
			SinglesNetworkClient_09F0_g(ctx, base);
			// ld r6,168(r27)
			ctx.r6.u64 = PPC_LOAD_U64(var_r27 + 168);
			// ld r30,112(r1)
			var_r30 = (uint32_t)(PPC_LOAD_U64(ctx.r1.u32 + 112));
			// cmpld cr6,r6,r30
			// bne cr6,0x823e7edc
			if (ctx.r6.u64 != var_r30) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// addi r31,r27,212
			var_r31 = (uint32_t)(var_r27 + 212);
			// li r5,0
			ctx.r5.s64 = 0;
			// li r4,24
			ctx.r4.s64 = 24;
			// lwz r3,4(r31)
			ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
			// lwz r10,4(r11)
			// bctrl
			VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
			// mr r4,r3
			ctx.r4.u64 = ctx.r3.u64;
			// cmplwi cr6,r4,0
			// beq cr6,0x823e7edc
			if (ctx.r4.u32 == 0) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// lwz r9,100(r1)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
			// lis r11,-32249
			// lwz r7,104(r1)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
			// addi r3,r31,8
			ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
			// rldicr r8,r9,32,63
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u64, 32);
			// std r30,16(r4)
			PPC_STORE_U64(ctx.r4.u32 + 16, var_r30);
			// addi r11,r11,13284
			ctx.r11.s64 = ctx.r11.s64 + 13284;
			// or r6,r7,r8
			ctx.r6.u64 = ctx.r7.u64 | ctx.r8.u64;
			// stw r11,0(r4)
			PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
			// std r6,4(r4)
			PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r6.u64);
			// bl 0x823ec068
			snSession_AddNode_C068(ctx, base);
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
	loc_823E7E58:
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,7084(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7084);
		// cmplw cr6,r11,r3
		// bne cr6,0x823e7ea0
		if (ctx.r11.u32 == ctx.r3.u32) {
			// li r5,64
			ctx.r5.s64 = 64;
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x822609f0
			SinglesNetworkClient_09F0_g(ctx, base);
			// ld r5,168(r27)
			ctx.r5.u64 = PPC_LOAD_U64(var_r27 + 168);
			// ld r4,80(r1)
			ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
			// cmpld cr6,r5,r4
			// bne cr6,0x823e7edc
			if (ctx.r5.u64 != ctx.r4.u64) {
				// mr r3,r26
				ctx.r3.u64 = var_r26;
				return;
			}
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x823e6c08
			SinglesNetworkClient_6C08_g(ctx, base);
			// li r26,2
			var_r26 = 2;
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
	loc_823E7EA0:
		// lis r11,-32163
		ctx.r11.s64 = -2107834368;
		// lwz r11,7096(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7096);
		// cmplw cr6,r11,r3
		// bne cr6,0x823e7edc
		if (ctx.r11.u32 != ctx.r3.u32) {
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
		// li r5,64
		ctx.r5.s64 = 64;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x822609f0
		SinglesNetworkClient_09F0_g(ctx, base);
		// ld r3,168(r27)
		ctx.r3.u64 = PPC_LOAD_U64(var_r27 + 168);
		// ld r11,80(r1)
		ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// cmpld cr6,r3,r11
		// bne cr6,0x823e7edc
		if (ctx.r3.u64 != ctx.r11.u64) {
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			return;
		}
		// mr r3,r27
		ctx.r3.u64 = var_r27;
		// bl 0x823e6cb8
		SinglesNetworkClient_6CB8_g(ctx, base);
	loc_823E7ED8:
		// li r26,2
		var_r26 = 2;
	}
loc_823E7EDC:
	// mr r3,r26
	ctx.r3.u64 = var_r26;
	return;
}

__attribute__((alias("__imp__EvtRemoteJoin_vfn_0"))) PPC_WEAK_FUNC(EvtRemoteJoin_vfn_0);
PPC_FUNC_IMPL(__imp__EvtRemoteJoin_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,88
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 88;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32249
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// cmplwi cr6,r10,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823e7f30
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E7F30:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_7F48_g"))) PPC_WEAK_FUNC(snLeaveMachine_7F48_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_7F48_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r26 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=160, savegprlr_25
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// mr r25,r6
	var_r25 = ctx.r6.u32;
	// mr r27,r7
	var_r27 = ctx.r7.u32;
	// lwz r29,160(r26)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r26 + 160));
	// addi r3,r29,720
	ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 720;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823e7f88
	if (ctx.r3.u32 != 0) {
		// lwz r30,0(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 0));
		// cmplwi cr6,r30,0
		// bne cr6,0x823e7fcc
		if (var_r30 != 0) goto loc_823E7FCC;
	}
loc_823E7F88:
	// mr r7,r27
	ctx.r7.u64 = var_r27;
	// mr r6,r25
	ctx.r6.u64 = var_r25;
	// mr r5,r28
	ctx.r5.u64 = var_r28;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823ec480
	game_C480(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmplwi cr6,r30,0
	// bne cr6,0x823e7fcc
	if (var_r30 == 0) {
		// lis r11,-32249
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// addi r4,r11,10176
		ctx.r4.s64 = ctx.r11.s64 + 10176;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		return;
	}
loc_823E7FCC:
	// lwz r11,444(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 444);
	// mr r4,r25
	ctx.r4.u64 = var_r25;
	// addi r3,r26,3772
	ctx.r3.s64 = (int64_t)(int32_t)var_r26 + 3772;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,444(r30)
	PPC_STORE_U32(var_r30 + 444, ctx.r11.u32);
	// bl 0x823ea7c8
	snLeaveMachine_A7C8_g(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// bne cr6,0x823e801c
	if (var_r31 == 0) {
		// mr r4,r25
		ctx.r4.u64 = var_r25;
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		// bl 0x823e82e8
		snLeaveMachine_82E8_g(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// bne cr6,0x823e801c
		if (var_r31 != 0) goto loc_823E801C;
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,10204
		ctx.r4.s64 = ctx.r11.s64 + 10204;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// b 0x823e80ec
	} else {
	loc_823E801C:
		// lwz r8,16(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
		// li r7,0
		ctx.r7.s64 = 0;
		// cmpwi cr6,r8,4
		// bge cr6,0x823e8080
		if (ctx.r8.s32 < 4) {
			// li r9,0
			ctx.r9.s64 = 0;
			// li r10,0
			ctx.r10.s64 = 0;
			// mr r11,r31
			ctx.r11.u64 = var_r31;
		loc_823E8038:
			// lwz r6,0(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// cmplw cr6,r6,r30
			// beq cr6,0x823e8058
			if (ctx.r6.u32 != var_r30) {
				// addi r10,r10,1
				ctx.r10.s64 = ctx.r10.s64 + 1;
				// addi r11,r11,4
				ctx.r11.s64 = ctx.r11.s64 + 4;
				// cmpwi cr6,r10,4
				// blt cr6,0x823e8038
				if (ctx.r10.s32 < 4) goto loc_823E8038;
				// b 0x823e805c
			} else {
			loc_823E8058:
				// li r9,1
				ctx.r9.s64 = 1;
			}
		loc_823E805C:
			// clrlwi r5,r9,24
			ctx.r5.u64 = ctx.r9.u32 & 0xFF;
			// cmplwi cr6,r5,0
			// bne cr6,0x823e8080
			if (ctx.r5.u32 != 0) goto loc_823E8080;
			// rlwinm r4,r8,2,0,29
			ctx.r4.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
			// li r7,1
			ctx.r7.s64 = 1;
			// stwx r30,r4,r31
			PPC_STORE_U32(ctx.r4.u32 + var_r31, var_r30);
			// lwz r11,16(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
			// addi r3,r11,1
			ctx.r3.s64 = ctx.r11.s64 + 1;
			// stw r3,16(r31)
			PPC_STORE_U32(var_r31 + 16, ctx.r3.u32);
		}
	loc_823E8080:
		// clrlwi r11,r7,24
		ctx.r11.u64 = ctx.r7.u32 & 0xFF;
		// cmplwi cr6,r11,0
		// bne cr6,0x823e80a0
		if (ctx.r11.u32 == 0) {
			// lis r11,-32249
			// addi r3,r1,80
			ctx.r3.s64 = ctx.r1.s64 + 80;
			// addi r4,r11,10224
			ctx.r4.s64 = ctx.r11.s64 + 10224;
			// bl 0x8240e6d0
			nop_8240E6D0(ctx, base);
			// b 0x823e80d4
		} else {
		loc_823E80A0:
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// mr r3,r26
			ctx.r3.u64 = var_r26;
			// bl 0x823e8108
			SinglesNetworkClient_8108_g(ctx, base);
			// clrlwi r10,r3,24
			ctx.r10.u64 = ctx.r3.u32 & 0xFF;
			// cmplwi cr6,r10,0
			// bne cr6,0x823e80f8
			if (ctx.r10.u32 != 0) {
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				return;
			}
			// lis r11,-32249
			// addi r3,r1,80
			ctx.r3.s64 = ctx.r1.s64 + 80;
			// addi r4,r11,10252
			ctx.r4.s64 = ctx.r11.s64 + 10252;
			// bl 0x8240e6d0
			nop_8240E6D0(ctx, base);
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x823e84e8
			snLeaveMachine_84E8_g(ctx, base);
		}
	loc_823E80D4:
		// lwz r9,16(r31)
		ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
		// cmpwi cr6,r9,0
		// bne cr6,0x823e80ec
		if (ctx.r9.s32 != 0) goto loc_823E80EC;
		// mr r4,r25
		ctx.r4.u64 = var_r25;
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		// bl 0x823e8410
		snLeaveMachine_8410_g(ctx, base);
	}
loc_823E80EC:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823ed098
	snLeaveMachine_D098_g(ctx, base);
	// li r30,0
	var_r30 = 0;
loc_823E80F8:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8108_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_8108_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8108_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r27,r30,3756
	var_r27 = (uint32_t)(var_r30 + 3756);
	// addi r28,r29,96
	var_r28 = (uint32_t)(var_r29 + 96);
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// li r31,0
	var_r31 = 0;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// cmplwi cr6,r3,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823e8144
	if (ctx.r3.u32 == 0) {
		// mr r11,r31
		ctx.r11.u64 = var_r31;
	}
loc_823E8144:
	// clrlwi r10,r11,24
	ctx.r10.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bne cr6,0x823e81b4
	if (ctx.r10.u32 == 0) {
		// lwz r11,3752(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 3752);
		// lhz r9,6(r11)
		ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
		// cmplwi cr6,r9,0
		// beq cr6,0x823e81b4
		if (ctx.r9.u32 == 0) {
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			return;
		}
		// lwz r5,12(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// clrlwi r8,r9,16
		ctx.r8.u64 = ctx.r9.u32 & 0xFFFF;
		// addis r7,r8,1
		ctx.r7.s64 = ctx.r8.s64 + 65536;
		// cmplwi cr6,r5,0
		// addi r7,r7,-1
		ctx.r7.s64 = ctx.r7.s64 + -1;
		// lwz r4,0(r5)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
		// sth r7,6(r11)
		PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r7.u16);
		// stw r4,12(r11)
		PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r4.u32);
		// beq cr6,0x823e81a0
		if (ctx.r5.u32 != 0) {
			// addi r11,r5,8
			ctx.r11.s64 = ctx.r5.s64 + 8;
			// stw r29,0(r5)
			PPC_STORE_U32(ctx.r5.u32 + 0, var_r29);
			// stw r31,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, var_r31);
			// stw r31,4(r11)
			PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
			// stw r31,8(r11)
			PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
			// stb r31,16(r11)
			PPC_STORE_U8(ctx.r11.u32 + 16, (uint8_t)var_r31);
			// std r31,32(r11)
			PPC_STORE_U64(ctx.r11.u32 + 32, var_r31);
		}
	loc_823E81A0:
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// mr r3,r27
		ctx.r3.u64 = var_r27;
		// bl 0x823ec5d8
		SinglesNetworkClient_C5D8_g(ctx, base);
		return;
	}
loc_823E81B4:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	return;
}

__attribute__((alias("__imp__snLeaveMachine_81C0_g"))) PPC_WEAK_FUNC(snLeaveMachine_81C0_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_81C0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=144, savegprlr_26
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r26,r29,3756
	var_r26 = (uint32_t)(var_r29 + 3756);
	// mr r3,r26
	ctx.r3.u64 = var_r26;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x823e82d8
	if (var_r31 != 0) {
		// lwz r28,0(r31)
		var_r28 = (uint32_t)(PPC_LOAD_U32(var_r31 + 0));
		// lwz r11,36(r28)
		ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 36);
		// cmpwi cr6,r11,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bge cr6,0x823e8204
		if (ctx.r11.s32 < 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823E8204:
		// clrlwi r9,r11,24
		ctx.r9.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r9,0
		// beq cr6,0x823e8220
		if (ctx.r9.u32 != 0) {
			// lwz r11,3788(r29)
			ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 3788);
			// addi r8,r11,-1
			ctx.r8.s64 = ctx.r11.s64 + -1;
			// stw r8,3788(r29)
			PPC_STORE_U32(var_r29 + 3788, ctx.r8.u32);
			// b 0x823e829c
		} else {
		loc_823E8220:
			// addi r27,r28,40
			var_r27 = (uint32_t)(var_r28 + 40);
			// addi r3,r29,3772
			ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 3772;
			// mr r4,r27
			ctx.r4.u64 = var_r27;
			// bl 0x823ea7c8
			snLeaveMachine_A7C8_g(ctx, base);
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// cmplwi cr6,r30,0
			// beq cr6,0x823e829c
			if (var_r30 == 0) goto loc_823E829C;
			// li r9,0
			ctx.r9.s64 = 0;
			// li r10,0
			ctx.r10.s64 = 0;
			// mr r11,r30
			ctx.r11.u64 = var_r30;
		loc_823E8248:
			// lwz r7,0(r11)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// cmplw cr6,r7,r28
			// beq cr6,0x823e8268
			if (ctx.r7.u32 != var_r28) {
				// addi r10,r10,1
				ctx.r10.s64 = ctx.r10.s64 + 1;
				// addi r11,r11,4
				ctx.r11.s64 = ctx.r11.s64 + 4;
				// cmpwi cr6,r10,4
				// blt cr6,0x823e8248
				if (ctx.r10.s32 < 4) goto loc_823E8248;
				// b 0x823e826c
			} else {
			loc_823E8268:
				// li r9,1
				ctx.r9.s64 = 1;
			}
		loc_823E826C:
			// clrlwi r6,r9,24
			ctx.r6.u64 = ctx.r9.u32 & 0xFF;
			// cmplwi cr6,r6,0
			// beq cr6,0x823e829c
			if (ctx.r6.u32 == 0) goto loc_823E829C;
			// mr r4,r28
			ctx.r4.u64 = var_r28;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x823e84e8
			snLeaveMachine_84E8_g(ctx, base);
			// lwz r5,16(r30)
			ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 16);
			// cmpwi cr6,r5,0
			// bne cr6,0x823e829c
			if (ctx.r5.s32 != 0) goto loc_823E829C;
			// mr r4,r27
			ctx.r4.u64 = var_r27;
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x823e8410
			snLeaveMachine_8410_g(ctx, base);
		}
	loc_823E829C:
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// bl 0x823ed098
		snLeaveMachine_D098_g(ctx, base);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		// bl 0x8236c340
		SinglesNetworkClient_C340_g(ctx, base);
		// lwz r11,3752(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 3752);
		// li r3,1
		ctx.r3.s64 = 1;
		// lwz r4,12(r11)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// stw r4,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r4.u32);
		// lhz r10,6(r11)
		ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
		// stw r31,12(r11)
		PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// sth r10,6(r11)
		PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
		return;
	}
loc_823E82D8:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__snLeaveMachine_82E8_g"))) PPC_WEAK_FUNC(snLeaveMachine_82E8_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_82E8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=144, savegprlr_28
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// li r31,0
	var_r31 = 0;
	// lwz r11,3768(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 3768);
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	// bne cr6,0x823e832c
	if (ctx.r10.u32 == 0) {
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,10284
		ctx.r4.s64 = ctx.r11.s64 + 10284;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		return;
	}
loc_823E832C:
	// lwz r5,12(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lhz r9,6(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// addis r8,r9,1
	ctx.r8.s64 = ctx.r9.s64 + 65536;
	// cmplwi cr6,r5,0
	// lwz r6,0(r5)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// sth r8,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r8.u16);
	// stw r6,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r6.u32);
	// beq cr6,0x823e83a0
	if (ctx.r5.u32 != 0) {
		// addi r11,r5,20
		ctx.r11.s64 = ctx.r5.s64 + 20;
		// stw r31,16(r5)
		PPC_STORE_U32(ctx.r5.u32 + 16, var_r31);
		// mr r8,r31
		ctx.r8.u64 = var_r31;
		// li r9,9
		ctx.r9.s64 = 9;
		// addi r10,r11,12
		ctx.r10.s64 = ctx.r11.s64 + 12;
		// stw r31,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, var_r31);
		// stw r31,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
		// stw r31,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
	loc_823E8378:
		// stw r8,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r8.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823e8378
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823E8378;
		// addi r11,r5,32
		ctx.r11.s64 = ctx.r5.s64 + 32;
		// mr r9,r31
		ctx.r9.u64 = var_r31;
		// li r10,9
		ctx.r10.s64 = 9;
		// mtctr r10
		ctx.ctr.u64 = ctx.r10.u64;
	loc_823E8394:
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// bdnz 0x823e8394
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823E8394;
	}
loc_823E83A0:
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// addi r3,r30,3772
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 3772;
	// bl 0x823ea408
	atSingleton_A408(ctx, base);
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r30
	ctx.r6.u64 = var_r30;
	// lwz r3,164(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 164);
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// li r4,2
	ctx.r4.s64 = 2;
	// bl 0x823db2d8
	SinglesNetworkClient_B2D8_g(ctx, base);
	// cmplwi cr6,r3,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823e83d4
	if (ctx.r3.u32 == 0) {
		// mr r11,r31
		ctx.r11.u64 = var_r31;
	}
loc_823E83D4:
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// bne cr6,0x823e8400
	if (ctx.r4.u32 == 0) {
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,7784
		ctx.r4.s64 = ctx.r11.s64 + 7784;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823e8410
		snLeaveMachine_8410_g(ctx, base);
		// mr r28,r31
		var_r28 = (uint32_t)(var_r31);
	}
loc_823E8400:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	return;
}

__attribute__((alias("__imp__snLeaveMachine_8410_g"))) PPC_WEAK_FUNC(snLeaveMachine_8410_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_8410_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r27,r28,3772
	var_r27 = (uint32_t)(var_r28 + 3772);
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x823ea7c8
	snLeaveMachine_A7C8_g(ctx, base);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// cmplwi cr6,r31,0
	// bne cr6,0x823e8444
	if (var_r31 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E8444:
	// clrlwi r10,r11,24
	ctx.r10.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// beq cr6,0x823e84d8
	if (ctx.r10.u32 != 0) {
		// lwz r29,164(r28)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r28 + 164));
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x823daaf0
		SinglesNetworkClient_AAF0_g(ctx, base);
		// clrlwi r9,r3,24
		ctx.r9.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r9,0
		// beq cr6,0x823e8480
		if (ctx.r9.u32 != 0) {
			// mr r5,r28
			ctx.r5.u64 = var_r28;
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x823da9b8
			SinglesNetworkClient_A9B8_g(ctx, base);
		}
	loc_823E8480:
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r27
		ctx.r3.u64 = var_r27;
		// bl 0x823eb138
		atSingleton_B138(ctx, base);
		// addi r11,r31,32
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 32;
		// li r9,0
		ctx.r9.s64 = 0;
		// li r10,9
		ctx.r10.s64 = 9;
		// mtctr r10
		ctx.ctr.u64 = ctx.r10.u64;
	loc_823E849C:
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// bdnz 0x823e849c
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823E849C;
		// lwz r11,3768(r28)
		ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 3768);
		// cmplwi cr6,r31,0
		// li r3,1
		ctx.r3.s64 = 1;
		// beq cr6,0x823e84dc
		if (var_r31 == 0) {
			return;
		}
		// lwz r8,12(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// stw r8,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r8.u32);
		// lhz r10,6(r11)
		ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
		// stw r31,12(r11)
		PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
		// addi r7,r10,1
		ctx.r7.s64 = ctx.r10.s64 + 1;
		// sth r7,6(r11)
		PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r7.u16);
		return;
	}
loc_823E84D8:
	// li r3,1
	ctx.r3.s64 = 1;
loc_823E84DC:
	return;
}

__attribute__((alias("__imp__snLeaveMachine_84E8_g"))) PPC_WEAK_FUNC(snLeaveMachine_84E8_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_84E8_g) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmpwi cr6,r8,0
	// blelr cr6
	if (ctx.r8.s32 <= 0) return;
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
loc_823E8504:
	// lwz r7,0(r9)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// cmplw cr6,r4,r7
	// beq cr6,0x823e8528
	if (ctx.r4.u32 == ctx.r7.u32) goto loc_823E8528;
	// lwz r6,16(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// cmpw cr6,r10,r6
	// blt cr6,0x823e8504
	if (ctx.r10.s32 < ctx.r6.s32) goto loc_823E8504;
	// blr
	return;
loc_823E8528:
	// addi r9,r8,-1
	ctx.r9.s64 = ctx.r8.s64 + -1;
	// rlwinm r5,r10,2,0,29
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r4,r9,2,0,29
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r3,1
	ctx.r3.s64 = 1;
	// stw r9,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r9.u32);
	// lwzx r10,r4,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + ctx.r11.u32);
	// stwx r10,r5,r11
	PPC_STORE_U32(ctx.r5.u32 + ctx.r11.u32, ctx.r10.u32);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// rlwinm r8,r9,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// stwx r7,r8,r11
	PPC_STORE_U32(ctx.r8.u32 + ctx.r11.u32, ctx.r7.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_8558_g"))) PPC_WEAK_FUNC(snLeaveMachine_8558_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_8558_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r3,16(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lwz r11,3760(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 3760);
	// cmpwi cr6,r11,0
	// beq cr6,0x823e85cc
while (ctx.r11.s32 != 0) {
	loc_823E8580:
		// lwz r11,3756(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 3756);
		// cmplwi cr6,r11,0
		// beq cr6,0x823e85ac
		if (ctx.r11.u32 != 0) {
			// lwz r10,8(r11)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplwi cr6,r10,0
			// beq cr6,0x823e85b0
			if (ctx.r10.u32 == 0) goto loc_823E85B0;
		loc_823E8598:
			// lwz r11,8(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// lwz r9,8(r11)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplwi cr6,r9,0
			// bne cr6,0x823e8598
			if (ctx.r9.u32 != 0) goto loc_823E8598;
			// b 0x823e85b0
		} else {
		loc_823E85AC:
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823E85B0:
		// lwz r11,0(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r4,r11,96
		ctx.r4.s64 = ctx.r11.s64 + 96;
		// bl 0x823e81c0
		snLeaveMachine_81C0_g(ctx, base);
		// lwz r3,16(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 16);
		// lwz r11,3760(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 3760);
		// cmpwi cr6,r11,0
		// bne cr6,0x823e8580
}
loc_823E85CC:
	// lwz r31,16(r30)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 16));
	// lbz r8,3744(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 3744);
	// rlwinm r7,r8,0,0,24
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r7,0
	// bne cr6,0x823e8638
	if (ctx.r7.u32 == 0) {
		// clrlwi r6,r8,24
		ctx.r6.u64 = ctx.r8.u32 & 0xFF;
		// rlwinm r5,r6,0,0,24
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e85fc
		if (ctx.r5.u32 != 0) {
			// lwz r11,3784(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 3784);
			// addi r4,r11,40
			ctx.r4.s64 = ctx.r11.s64 + 40;
			// b 0x823e8600
		} else {
		loc_823E85FC:
			// addi r4,r31,176
			ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 176;
		}
	loc_823E8600:
		// addi r3,r31,3772
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3772;
		// bl 0x823ea7c8
		snLeaveMachine_A7C8_g(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823e8638
		if (ctx.r3.u32 == 0) {
			// lwz r11,16(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 16);
			// addi r3,r11,232
			ctx.r3.s64 = ctx.r11.s64 + 232;
			// bl 0x8236eab8
			xam_EAB8_g(ctx, base);
			// blr
			return;
		}
		// lbz r4,3744(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 3744);
		// rlwinm r3,r4,0,0,24
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r3,0
		// beq cr6,0x823e862c
		if (ctx.r3.u32 != 0) {
			// lwz r11,3784(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 3784);
			// addi r4,r11,40
			ctx.r4.s64 = ctx.r11.s64 + 40;
			// b 0x823e8630
		} else {
		loc_823E862C:
			// addi r4,r31,176
			ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 176;
		}
	loc_823E8630:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823e8410
		snLeaveMachine_8410_g(ctx, base);
	}
loc_823E8638:
	// lwz r11,16(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 16);
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8236eab8
	xam_EAB8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_8660_g"))) PPC_WEAK_FUNC(snSession_8660_g);
PPC_FUNC_IMPL(__imp__snSession_8660_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=160, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// lwz r31,16(r29)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r29 + 16));
	// lbz r11,3744(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 3744);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// beq cr6,0x823e8714
	if (ctx.r10.u32 != 0) {
		// lis r11,-32161
		// ld r30,168(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U64(var_r31 + 168));
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// lwz r11,13260(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 13260);
		// stw r11,116(r1)
		PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// stw r11,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
		// stw r11,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
		// stw r11,92(r1)
		PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
		// stw r11,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
		// stw r11,100(r1)
		PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
		// stw r11,104(r1)
		PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r11.u32);
		// stw r11,108(r1)
		PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
		// stw r11,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
		// bl 0x82260268
		SinglesNetworkClient_0268_g(ctx, base);
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x82260268
		SinglesNetworkClient_0268_g(ctx, base);
		// lis r11,-32163
		// li r9,8128
		ctx.r9.s64 = 8128;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// lwz r4,7084(r11)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7084);
		// stw r9,100(r1)
		PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r9.u32);
		// bl 0x822386f0
		SinglesNetworkClient_86F0_g(ctx, base);
		// li r5,64
		ctx.r5.s64 = 64;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dd1f0
		SinglesNetworkClient_D1F0_g(ctx, base);
		// li r7,1
		ctx.r7.s64 = 1;
		// li r6,0
		ctx.r6.s64 = 0;
		// lwz r3,164(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 164);
		// li r5,0
		ctx.r5.s64 = 0;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// bl 0x823dad10
		SinglesNetworkClient_AD10_g(ctx, base);
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x82260268
		SinglesNetworkClient_0268_g(ctx, base);
	}
loc_823E8714:
	// lwz r4,16(r29)
	ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 16);
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,14
	ctx.r5.s64 = 14;
	// addi r3,r4,108
	ctx.r3.s64 = ctx.r4.s64 + 108;
	// bl 0x82371bf8
	snSession_1BF8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__snSession_8730_g"))) PPC_WEAK_FUNC(snSession_8730_g);
PPC_FUNC_IMPL(__imp__snSession_8730_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=160, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,16(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 16);
	// lbz r10,3744(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
	// rlwinm r9,r10,0,0,24
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r9,0
	// bne cr6,0x823e87cc
	if (ctx.r9.u32 == 0) {
		// rotlwi r11,r11,0
		ctx.r11.u64 = ctx.r11.u32;
		// li r8,1
		ctx.r8.s64 = 1;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// ld r7,168(r11)
		ctx.r7.u64 = PPC_LOAD_U64(ctx.r11.u32 + 168);
		// lwz r31,164(r11)
		var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + 164));
		// stb r8,88(r1)
		PPC_STORE_U8(ctx.r1.u32 + 88, ctx.r8.u8);
		// std r7,80(r1)
		PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
		// bl 0x823eaf10
		SinglesNetworkClient_AF10_g(ctx, base);
		// lwz r11,16(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 16);
		// mr r5,r3
		ctx.r5.u64 = ctx.r3.u64;
		// lbz r6,3744(r11)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
		// rlwinm r4,r6,0,0,24
		ctx.r4.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r4,0
		// beq cr6,0x823e87a4
		if (ctx.r4.u32 != 0) {
			// lwz r11,3784(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 3784);
			// addi r4,r11,40
			ctx.r4.s64 = ctx.r11.s64 + 40;
			// b 0x823e87a8
		} else {
		loc_823E87A4:
			// addi r4,r11,176
			ctx.r4.s64 = ctx.r11.s64 + 176;
		}
	loc_823E87A8:
		// li r7,0
		ctx.r7.s64 = 0;
		// li r6,1
		ctx.r6.s64 = 1;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823dae48
		SinglesNetworkClient_AE48_g(ctx, base);
		// addi r3,r1,96
		ctx.r3.s64 = ctx.r1.s64 + 96;
		// bl 0x82260268
		SinglesNetworkClient_0268_g(ctx, base);
		// li r4,3
		ctx.r4.s64 = 3;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823dab78
		SinglesNetworkClient_AB78_g(ctx, base);
	}
loc_823E87CC:
	// lwz r4,16(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 16);
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r3,r4,108
	ctx.r3.s64 = ctx.r4.s64 + 108;
	// bl 0x82371bf8
	snSession_1BF8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_87F8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_87F8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_87F8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=272, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r1,164
	ctx.r11.s64 = ctx.r1.s64 + 164;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E881C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e881c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E881C;
	// addi r11,r1,104
	ctx.r11.s64 = ctx.r1.s64 + 104;
	// li r10,0
	ctx.r10.s64 = 0;
	// li r9,15
	ctx.r9.s64 = 15;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823E8838:
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e8838
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E8838;
	// addi r11,r1,164
	ctx.r11.s64 = ctx.r1.s64 + 164;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E8854:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e8854
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E8854;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r9,r1,104
	ctx.r9.s64 = ctx.r1.s64 + 104;
	// li r8,24
	ctx.r8.s64 = 24;
	// lwz r7,668(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 668);
	// ld r10,168(r11)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r11.u32 + 168);
	// rlwinm r6,r7,0,25,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0x7C;
	// std r10,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r10.u64);
	// addi r10,r11,304
	ctx.r10.s64 = ctx.r11.s64 + 304;
	// stw r6,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r6.u32);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
loc_823E8888:
	// lwz r5,0(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// stw r5,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r5.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bdnz 0x823e8888
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E8888;
	// ld r11,672(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 672);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// std r11,96(r1)
	PPC_STORE_U64(ctx.r1.u32 + 96, ctx.r11.u64);
	// bl 0x823eafc8
	SinglesNetworkClient_AFC8_g(ctx, base);
	// lwz r10,16(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r7,3
	ctx.r7.s64 = 3;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r3,164(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 164);
	// bl 0x823dad10
	SinglesNetworkClient_AD10_g(ctx, base);
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,164(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 164);
	// bl 0x823dab78
	SinglesNetworkClient_AB78_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_88F8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_88F8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_88F8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=176, savegprlr_27
	// mr r31,r6
	var_r31 = ctx.r6.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// li r27,0
	var_r27 = 0;
	// bl 0x82238758
	SinglesNetworkClient_8758_g(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,7120(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7120);
	// cmplw cr6,r11,r3
	// bne cr6,0x823e8a2c
	if (ctx.r11.u32 == ctx.r3.u32) {
		// li r5,64
		ctx.r5.s64 = 64;
		// addi r4,r1,88
		ctx.r4.s64 = ctx.r1.s64 + 88;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// li r29,0
		var_r29 = 0;
		// bl 0x822609f0
		SinglesNetworkClient_09F0_g(ctx, base);
		// clrlwi r11,r3,24
		ctx.r11.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r11,0
		// beq cr6,0x823e8974
		if (ctx.r11.u32 != 0) {
			// li r5,1
			ctx.r5.s64 = 1;
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x82238df8
			SinglesNetworkClient_8DF8_g(ctx, base);
			// lwz r10,80(r1)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// li r11,1
			ctx.r11.s64 = 1;
			// cmplwi cr6,r10,0
			// bne cr6,0x823e8970
			if (ctx.r10.u32 == 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823E8970:
			// clrlwi r29,r11,24
			var_r29 = (uint32_t)(ctx.r11.u32 & 0xFF);
		}
	loc_823E8974:
		// lwz r31,16(r30)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 16));
		// ld r8,88(r1)
		ctx.r8.u64 = PPC_LOAD_U64(ctx.r1.u32 + 88);
		// ld r9,168(r31)
		ctx.r9.u64 = PPC_LOAD_U64(var_r31 + 168);
		// cmpld cr6,r9,r8
		// bne cr6,0x823e8a2c
		if (ctx.r9.u64 != ctx.r8.u64) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// addi r3,r31,3772
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 3772;
		// bl 0x823ea7c8
		snLeaveMachine_A7C8_g(ctx, base);
		// cmplwi cr6,r3,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bne cr6,0x823e89a4
		if (ctx.r3.u32 == 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823E89A4:
		// clrlwi r6,r11,24
		ctx.r6.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r6,0
		// beq cr6,0x823e8a2c
		if (ctx.r6.u32 == 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// clrlwi r5,r29,24
		ctx.r5.u64 = var_r29 & 0xFF;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e8a00
		if (ctx.r5.u32 != 0) {
			// lwz r11,48(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 48);
			// lwz r10,3776(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 3776);
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// cmpw cr6,r11,r10
			// stw r11,48(r30)
			PPC_STORE_U32(var_r30 + 48, ctx.r11.u32);
			// bne cr6,0x823e8a20
			if (ctx.r11.s32 != ctx.r10.s32) {
				// li r3,2
				ctx.r3.s64 = 2;
				return;
			}
			// addi r3,r1,96
			ctx.r3.s64 = ctx.r1.s64 + 96;
			// bl 0x823dda08
			util_DA08(ctx, base);
			// lis r11,-32249
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// addi r11,r11,10400
			ctx.r11.s64 = ctx.r11.s64 + 10400;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// stw r11,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
			// bl 0x823eaac8
			snSession_AAC8_fw(ctx, base);
			// li r3,2
			ctx.r3.s64 = 2;
			return;
		}
	loc_823E8A00:
		// addi r3,r1,112
		ctx.r3.s64 = ctx.r1.s64 + 112;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// addi r4,r1,112
		ctx.r4.s64 = ctx.r1.s64 + 112;
		// addi r11,r11,10420
		ctx.r11.s64 = ctx.r11.s64 + 10420;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// stw r11,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
		// bl 0x823eab70
		snSession_AB70_gen(ctx, base);
	loc_823E8A20:
		// li r3,2
		ctx.r3.s64 = 2;
		return;
	}
loc_823E8A2C:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8A38_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_8A38_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8A38_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=224, savegprlr_24
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// lwz r11,16(r26)
	ctx.r11.u64 = PPC_LOAD_U32(var_r26 + 16);
	// lbz r10,3744(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3744);
	// rlwinm r9,r10,0,0,24
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r9,0
	// beq cr6,0x823e8ba8
	if (ctx.r9.u32 != 0) {
		// lwz r11,3772(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 3772);
		// li r27,0
		var_r27 = 0;
		// cmplwi cr6,r11,0
		// beq cr6,0x823e8a8c
		if (ctx.r11.u32 != 0) {
			// lwz r8,20(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// cmplwi cr6,r8,0
			// beq cr6,0x823e8a90
			if (ctx.r8.u32 == 0) goto loc_823E8A90;
		loc_823E8A78:
			// lwz r11,20(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// lwz r7,20(r11)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// cmplwi cr6,r7,0
			// bne cr6,0x823e8a78
			if (ctx.r7.u32 != 0) goto loc_823E8A78;
			// b 0x823e8a90
		} else {
		loc_823E8A8C:
			// mr r11,r27
			ctx.r11.u64 = var_r27;
		}
	loc_823E8A90:
		// mr r24,r11
		var_r24 = ctx.r11.u32;
		// lis r11,-32249
		ctx.r11.s64 = -2113470464;
		// addi r28,r11,10340
		var_r28 = (uint32_t)(ctx.r11.s64 + 10340);  // lbl_82072864 @ 0x82072864
		// lis r11,-32249
		ctx.r11.s64 = -2113470464;
		// addi r25,r11,12756
		var_r25 = (uint32_t)(ctx.r11.s64 + 12756);  // lbl_820731D4 @ 0x820731d4
	loc_823E8AA4:
		// cmplwi cr6,r24,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bne cr6,0x823e8ab4
		if (var_r24 == 0) {
			// mr r11,r27
			ctx.r11.u64 = var_r27;
		}
	loc_823E8AB4:
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r5,0
		// beq cr6,0x823e8ba8
		if (ctx.r5.u32 == 0) {
			return;
		}
		// cmplwi cr6,r24,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bne cr6,0x823e8ad0
		if (var_r24 == 0) {
			// mr r11,r27
			ctx.r11.u64 = var_r27;
		}
	loc_823E8AD0:
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		// addi r31,r24,32
		var_r31 = (uint32_t)(var_r24 + 32);
		// cmplwi cr6,r3,0
		// bne cr6,0x823e8ae4
		if (ctx.r3.u32 == 0) {
			// mr r31,r27
			var_r31 = (uint32_t)(var_r27);
		}
	loc_823E8AE4:
		// lwz r5,16(r26)
		ctx.r5.u64 = PPC_LOAD_U32(var_r26 + 16);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// lwz r3,164(r5)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32 + 164);
		// bl 0x823daaf0
		SinglesNetworkClient_AAF0_g(ctx, base);
		// clrlwi r11,r3,24
		ctx.r11.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r11,0
		// bne cr6,0x823e8b90
		if (ctx.r11.u32 == 0) {
			// li r6,4
			ctx.r6.s64 = 4;
			// lwz r3,16(r26)
			ctx.r3.u64 = PPC_LOAD_U32(var_r26 + 16);
			// addi r5,r1,80
			ctx.r5.s64 = ctx.r1.s64 + 80;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// bl 0x823e7178
			snLeaveMachine_7178_fw(ctx, base);
			// cmpwi cr6,r3,0
			// ble cr6,0x823e8b90
			if (ctx.r3.s32 <= 0) goto loc_823E8B90;
			// addi r30,r1,80
			var_r30 = (uint32_t)(ctx.r1.s64 + 80);
			// mr r31,r3
			var_r31 = ctx.r3.u32;
		loc_823E8B24:
			// addi r3,r1,100
			ctx.r3.s64 = ctx.r1.s64 + 100;
			// lwz r29,16(r26)
			var_r29 = (uint32_t)(PPC_LOAD_U32(var_r26 + 16));
			// stw r28,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, var_r28);
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// addi r3,r1,104
			ctx.r3.s64 = ctx.r1.s64 + 104;
			// bl 0x82121b00
			ke_1B00(ctx, base);
			// lwz r11,0(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
			// addi r10,r1,112
			ctx.r10.s64 = ctx.r1.s64 + 112;
			// ld r6,168(r29)
			ctx.r6.u64 = PPC_LOAD_U64(var_r29 + 168);
			// stw r27,104(r1)
			PPC_STORE_U32(ctx.r1.u32 + 104, var_r27);
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// stw r27,100(r1)
			PPC_STORE_U32(ctx.r1.u32 + 100, var_r27);
			// addi r3,r29,212
			ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 212;
			// stw r25,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, var_r25);
			// ld r9,96(r11)
			ctx.r9.u64 = PPC_LOAD_U64(ctx.r11.u32 + 96);
			// ld r8,104(r11)
			ctx.r8.u64 = PPC_LOAD_U64(ctx.r11.u32 + 104);
			// ld r7,112(r11)
			ctx.r7.u64 = PPC_LOAD_U64(ctx.r11.u32 + 112);
			// std r9,0(r10)
			PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r9.u64);
			// std r8,8(r10)
			PPC_STORE_U64(ctx.r10.u32 + 8, ctx.r8.u64);
			// std r7,16(r10)
			PPC_STORE_U64(ctx.r10.u32 + 16, ctx.r7.u64);
			// std r6,136(r1)
			PPC_STORE_U64(ctx.r1.u32 + 136, ctx.r6.u64);
			// bl 0x823eba30
			SinglesNetworkClient_BA30(ctx, base);
			// addi r31,r31,-1
			var_r31 = (uint32_t)(var_r31 + -1);
			// stw r28,96(r1)
			PPC_STORE_U32(ctx.r1.u32 + 96, var_r28);
			// addi r30,r30,4
			var_r30 = (uint32_t)(var_r30 + 4);
			// cmplwi cr6,r31,0
			// bne cr6,0x823e8b24
			if (var_r31 != 0) goto loc_823E8B24;
		}
	loc_823E8B90:
		// cmplwi cr6,r24,0
		// beq cr6,0x823e8aa4
		if (var_r24 == 0) goto loc_823E8AA4;
		// mr r3,r24
		ctx.r3.u64 = var_r24;
		// bl 0x823ea9b8
		util_A9B8(ctx, base);
		// mr r24,r3
		var_r24 = ctx.r3.u32;
		// b 0x823e8aa4
		goto loc_823E8AA4;
	}
loc_823E8BA8:
	return;
}

__attribute__((alias("__imp__snSession_8BB0_g"))) PPC_WEAK_FUNC(snSession_8BB0_g);
PPC_FUNC_IMPL(__imp__snSession_8BB0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t ea{};
	// FRAME: size=192, savegprlr_26
	// addi r31,r1,-192
	var_r31 = (uint32_t)(ctx.r1.s64 + -192);
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,20832
	ctx.r6.s64 = ctx.r11.s64 + 20832;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r29,16(r27)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r27 + 16));
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// lbz r9,3744(r29)
	ctx.r9.u64 = PPC_LOAD_U8(var_r29 + 3744);
	// rlwinm r8,r9,0,0,24
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r8,0
	// beq cr6,0x823e8c84
	if (ctx.r8.u32 != 0) {
		// addi r28,r26,16
		var_r28 = (uint32_t)(var_r26 + 16);
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// bl 0x823e70d0
		SinglesNetworkClient_70D0_g(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmpwi cr6,r30,0
		// ble cr6,0x823e8c84
		if ((int32_t)var_r30 <= 0) goto loc_823E8C84;
		// rlwinm r7,r30,2,0,29
		ctx.r7.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 2) & 0xFFFFFFFC;
		// neg r6,r7
		ctx.r6.s64 = static_cast<int64_t>(-ctx.r7.u64);
		// rlwinm r12,r6,0,0,27
		ctx.r12.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFF0;
		// bl 0x82569394
		_RtlCheckStack12(ctx, base);
		// lwz r5,0(r1)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r1.u32 + 0);
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// mr r6,r30
		ctx.r6.u64 = var_r30;
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// stwux r5,r1,r12
		ea = ctx.r1.u32 + ctx.r12.u32;
		PPC_STORE_U32(ea, ctx.r5.u32);
		ctx.r1.u32 = ea;
		// addi r29,r1,80
		var_r29 = (uint32_t)(ctx.r1.s64 + 80);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823e7178
		snLeaveMachine_7178_fw(ctx, base);
		// cmpw cr6,r30,r3
		// bne cr6,0x823e8c84
		if ((int32_t)var_r30 != ctx.r3.s32) goto loc_823E8C84;
		// cmpwi cr6,r30,0
		// ble cr6,0x823e8c84
	while (var_r30 != 0) {
		loc_823E8C68:
			// lwz r4,0(r29)
			ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 0);
			// lwz r3,16(r27)
			ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 16);
			// bl 0x823e6ff8
			SinglesNetworkClient_6FF8_h(ctx, base);
			// addi r30,r30,-1
			var_r30 = (uint32_t)(var_r30 + -1);
			// addi r29,r29,4
			var_r29 = (uint32_t)(var_r29 + 4);
			// cmplwi cr6,r30,0
			// bne cr6,0x823e8c68
	}
	}
loc_823E8C84:
	// lis r11,-32249
	// li r4,-1
	// addi r11,r11,13988
	ctx.r11.s64 = ctx.r11.s64 + 13988;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r10,9
	ctx.r10.s64 = 9;
	// stw r4,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r4.u32);
	// stw r11,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r11.u32);
	// addi r11,r31,88
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 88;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823E8CA8:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823e8ca8
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E8CA8;
	// lwz r3,12(r26)
	ctx.r3.u64 = PPC_LOAD_U32(var_r26 + 12);
	// addi r11,r26,16
	ctx.r11.s64 = (int64_t)(int32_t)var_r26 + 16;
	// addi r10,r31,88
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 88;
	// li r9,9
	ctx.r9.s64 = 9;
	// stw r3,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r3.u32);
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823E8CCC:
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x823e8ccc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823E8CCC;
	// lwz r4,16(r27)
	ctx.r4.u64 = PPC_LOAD_U32(var_r27 + 16);
	// addi r6,r31,80
	ctx.r6.s64 = (int64_t)(int32_t)var_r31 + 80;
	// li r5,20
	ctx.r5.s64 = 20;
	// addi r3,r4,108
	ctx.r3.s64 = ctx.r4.s64 + 108;
	// bl 0x82371bf8
	snSession_1BF8_g(ctx, base);
	// addi r1,r31,192
	ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 192;
	// b 0x8242f8e0
	__restgprlr_26(ctx, base);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8D00"))) PPC_WEAK_FUNC(SinglesNetworkClient_8D00);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8D00) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// addi r29,r31,12
	var_r29 = (uint32_t)(var_r31 + 12);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// stw r30,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r30);
	// stw r30,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r30);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r28,r29,4
	var_r28 = (uint32_t)(var_r29 + 4);
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// stw r30,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r30);
	// stw r30,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ var_r30);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r30,20(r31)
	PPC_STORE_U32(var_r31 + 20, var_r30);
	return;
}

__attribute__((alias("__imp__hsmState_vfn_4"))) PPC_WEAK_FUNC(hsmState_vfn_4);
PPC_FUNC_IMPL(__imp__hsmState_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,24(r11)
	// bctrl
	VCALL(ctx.r3.u32, 6, ctx, base);  // hsmState::vfn_6 (unnamed)  // vtable slot 6 (byte +24)
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__NotifyHandler_8DA8_w"))) PPC_WEAK_FUNC(NotifyHandler_8DA8_w);
PPC_FUNC_IMPL(__imp__NotifyHandler_8DA8_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15316
	ctx.r11.s64 = ctx.r11.s64 + 15316;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__util_8E10"))) PPC_WEAK_FUNC(util_8E10);
PPC_FUNC_IMPL(__imp__util_8E10) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15388
	ctx.r11.s64 = ctx.r11.s64 + 15388;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__util_8E78"))) PPC_WEAK_FUNC(util_8E78);
PPC_FUNC_IMPL(__imp__util_8E78) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15460
	ctx.r11.s64 = ctx.r11.s64 + 15460;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_8EE0_w"))) PPC_WEAK_FUNC(snSession_8EE0_w);
PPC_FUNC_IMPL(__imp__snSession_8EE0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15532
	ctx.r11.s64 = ctx.r11.s64 + 15532;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__util_8F48"))) PPC_WEAK_FUNC(util_8F48);
PPC_FUNC_IMPL(__imp__util_8F48) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15604
	ctx.r11.s64 = ctx.r11.s64 + 15604;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSafeDLListSimple_8FB0"))) PPC_WEAK_FUNC(atSafeDLListSimple_8FB0);
PPC_FUNC_IMPL(__imp__atSafeDLListSimple_8FB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15688
	ctx.r11.s64 = ctx.r11.s64 + 15688;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
loc_823E8FD0:
	// lwz r4,16(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e8fe4
	if (ctx.r4.u32 != 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823E8FE4:
	// clrlwi r10,r11,24
	ctx.r10.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bne cr6,0x823e8ffc
	if (ctx.r10.u32 != 0) {
		// blr
		return;
	}
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823735a8
	atSafeDLListSimple_35A8_g(ctx, base);
	// b 0x823e8fd0
	goto loc_823E8FD0;
}

__attribute__((alias("__imp__snSession_9010_gen"))) PPC_WEAK_FUNC(snSession_9010_gen);
PPC_FUNC_IMPL(__imp__snSession_9010_gen) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15700
	ctx.r11.s64 = ctx.r11.s64 + 15700;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__NotifyHandler_rtti_3BC4_0"))) PPC_WEAK_FUNC(NotifyHandler_rtti_3BC4_0);
PPC_FUNC_IMPL(__imp__NotifyHandler_rtti_3BC4_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r11,r11,15300
	ctx.r11.s64 = ctx.r11.s64 + 15300;
	// lwz r3,20(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 20);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823e90bc
	if (ctx.r3.u32 != 0) {
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	}
loc_823E90BC:
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r10,0
	// beq cr6,0x823e90d4
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E90D4:
	// blr
	return;
}

__attribute__((alias("__imp__hsmState_vfn_0_90F0_1"))) PPC_WEAK_FUNC(hsmState_vfn_0_90F0_1);
PPC_FUNC_IMPL(__imp__hsmState_vfn_0_90F0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e8ee0
	snSession_8EE0_w(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e9128
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E9128:
	// blr
	return;
}

__attribute__((alias("__imp__atSafeDLListSimple_vfn_0_9140_1"))) PPC_WEAK_FUNC(atSafeDLListSimple_vfn_0_9140_1);
PPC_FUNC_IMPL(__imp__atSafeDLListSimple_vfn_0_9140_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15676
	ctx.r11.s64 = ctx.r11.s64 + 15676;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x823e8fb0
	atSafeDLListSimple_8FB0(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e9184
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E9184:
	// blr
	return;
}

__attribute__((alias("__imp__rage_91A0"))) PPC_WEAK_FUNC(rage_91A0);
PPC_FUNC_IMPL(__imp__rage_91A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823e8fb0
	atSafeDLListSimple_8FB0(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e91d8
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823E91D8:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_91F0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_91F0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_91F0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823f3e78
	SinglesNetworkClient_3E78_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e9284
	if (ctx.r11.u32 != 0) {
		// li r5,64
		ctx.r5.s64 = 64;
		// ld r4,24(r30)
		ctx.r4.u64 = PPC_LOAD_U64(var_r30 + 24);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823dd1f0
		SinglesNetworkClient_D1F0_g(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823e9284
		if (ctx.r10.u32 == 0) goto loc_823E9284;
		// li r5,36
		ctx.r5.s64 = 36;
		// addi r4,r30,32
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 32;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x820d67c8
		SinglesNetworkClient_67C8_g(ctx, base);
		// cmplwi cr6,r3,36
		// li r11,1
		ctx.r11.s64 = 1;
		// beq cr6,0x823e9258
		if (ctx.r3.u32 != 36) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823E9258:
		// clrlwi r8,r11,24
		ctx.r8.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// beq cr6,0x823e9284
		if (ctx.r8.u32 == 0) goto loc_823E9284;
		// li r5,3
		ctx.r5.s64 = 3;
		// lwz r4,68(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 68);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x820d68a8
		SinglesNetworkClient_68A8_g(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// li r11,1
		ctx.r11.s64 = 1;
		// cmplwi cr6,r7,0
		// bne cr6,0x823e9288
		if (ctx.r7.u32 != 0) {
			// clrlwi r3,r11,24
			ctx.r3.u64 = ctx.r11.u32 & 0xFF;
			// blr
			return;
		}
	}
loc_823E9284:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823E9288:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_92A8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_92A8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_92A8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r5,10
	ctx.r5.s64 = 10;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r29,16(r30)
	var_r29 = (uint32_t)(PPC_LOAD_U32(var_r30 + 16));
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823e9334
	if (ctx.r11.u32 != 0) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// mr r6,r29
		ctx.r6.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,24(r30)
		ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 24);
		// lwz r4,4(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 4)/* SinglesNetworkClient::flags@+0x4 */;
		// bl 0x822608c8
		SinglesNetworkClient_08C8_g(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r28,16(r31)
		var_r28 = (uint32_t)(PPC_LOAD_U32(var_r31 + 16));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// li r10,32
		ctx.r10.s64 = 32;
		// addi r9,r28,7
		ctx.r9.s64 = (int64_t)(int32_t)var_r28 + 7;
		// lwz r28,32(r31)
		var_r28 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// li r5,16
		ctx.r5.s64 = 16;
		// srawi r4,r9,3
		ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0x7) != 0);
		ctx.r4.s64 = ctx.r9.s32 >> 3;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r10,32(r31)
		PPC_STORE_U32(var_r31 + 32, ctx.r10.u32);
		// bl 0x82260448
		SinglesNetworkClient_0448_g(ctx, base);
		// cmplw cr6,r29,r30
		// stw r28,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r28);
		// li r11,1
		ctx.r11.s64 = 1;
		// beq cr6,0x823e9338
		if (var_r29 == var_r30) {
			// clrlwi r3,r11,24
			ctx.r3.u64 = ctx.r11.u32 & 0xFF;
			return;
		}
	}
loc_823E9334:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823E9338:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_9348_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_9348_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_9348_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x822603d0
	snJoinMachine_03D0_g(ctx, base);
	// li r5,10
	ctx.r5.s64 = 10;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lwz r29,80(r1)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 80));
	// cmplwi cr6,r11,0
	// beq cr6,0x823e9394
	if (ctx.r11.u32 != 0) {
		// cmplwi cr6,r29,512
		// li r11,1
		ctx.r11.s64 = 1;
		// ble cr6,0x823e9398
		if (var_r29 <= 512) goto loc_823E9398;
	}
loc_823E9394:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823E9398:
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// beq cr6,0x823e93c0
	if (ctx.r9.u32 != 0) {
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260c10
		SinglesNetworkClient_0C10_g(ctx, base);
		// cmplw cr6,r29,r3
		// li r11,1
		ctx.r11.s64 = 1;
		// beq cr6,0x823e93c4
		if (var_r29 == ctx.r3.u32) {
			// clrlwi r3,r11,24
			ctx.r3.u64 = ctx.r11.u32 & 0xFF;
			return;
		}
	}
loc_823E93C0:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823E93C4:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	return;
}

__attribute__((alias("__imp__util_93D0"))) PPC_WEAK_FUNC(util_93D0);
PPC_FUNC_IMPL(__imp__util_93D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e945c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10792
		ctx.r10.s64 = ctx.r10.s64 + 10792;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E945C:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9478_fw"))) PPC_WEAK_FUNC(snSession_9478_fw);
PPC_FUNC_IMPL(__imp__snSession_9478_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r30,56(r3)
	var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,24
	ctx.r4.s64 = 24;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9514
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r10,r10,10912
		ctx.r10.s64 = ctx.r10.s64 + 10912;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// lwz r10,12(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 12);
		// stw r10,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r10.u32);
		// ld r9,16(r31)
		ctx.r9.u64 = PPC_LOAD_U64(var_r31 + 16);
		// std r9,16(r4)
		PPC_STORE_U64(ctx.r4.u32 + 16, ctx.r9.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9514:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9530_fw"))) PPC_WEAK_FUNC(snSession_9530_fw);
PPC_FUNC_IMPL(__imp__snSession_9530_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e95bc
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10852
		ctx.r10.s64 = ctx.r10.s64 + 10852;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E95BC:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_95D8_fw"))) PPC_WEAK_FUNC(snSession_95D8_fw);
PPC_FUNC_IMPL(__imp__snSession_95D8_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9664
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10892
		ctx.r10.s64 = ctx.r10.s64 + 10892;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9664:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9680_fw"))) PPC_WEAK_FUNC(snSession_9680_fw);
PPC_FUNC_IMPL(__imp__snSession_9680_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e970c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11728
		ctx.r10.s64 = ctx.r10.s64 + 11728;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E970C:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9728_fw"))) PPC_WEAK_FUNC(snSession_9728_fw);
PPC_FUNC_IMPL(__imp__snSession_9728_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e97b4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11508
		ctx.r10.s64 = ctx.r10.s64 + 11508;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E97B4:
	// blr
	return;
}

__attribute__((alias("__imp__snJoinMachine_97D0"))) PPC_WEAK_FUNC(snJoinMachine_97D0);
PPC_FUNC_IMPL(__imp__snJoinMachine_97D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // snJoinMachine::vfn_11 (unnamed)  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e985c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11528
		ctx.r10.s64 = ctx.r10.s64 + 11528;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E985C:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmLocalJoinPending_9878"))) PPC_WEAK_FUNC(snHsmLocalJoinPending_9878);
PPC_FUNC_IMPL(__imp__snHsmLocalJoinPending_9878) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9904
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11568
		ctx.r10.s64 = ctx.r10.s64 + 11568;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9904:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmWaitingForReply_9920"))) PPC_WEAK_FUNC(snHsmWaitingForReply_9920);
PPC_FUNC_IMPL(__imp__snHsmWaitingForReply_9920) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e99ac
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11648
		ctx.r10.s64 = ctx.r10.s64 + 11648;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E99AC:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmRemoteJoinPending_99C8"))) PPC_WEAK_FUNC(snHsmRemoteJoinPending_99C8);
PPC_FUNC_IMPL(__imp__snHsmRemoteJoinPending_99C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9a54
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11608
		ctx.r10.s64 = ctx.r10.s64 + 11608;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9A54:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmAcceptingJoinRequest_9A70"))) PPC_WEAK_FUNC(snHsmAcceptingJoinRequest_9A70);
PPC_FUNC_IMPL(__imp__snHsmAcceptingJoinRequest_9A70) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9afc
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,11688
		ctx.r10.s64 = ctx.r10.s64 + 11688;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9AFC:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9B18_fw"))) PPC_WEAK_FUNC(snSession_9B18_fw);
PPC_FUNC_IMPL(__imp__snSession_9B18_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9ba4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,12476
		ctx.r10.s64 = ctx.r10.s64 + 12476;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9BA4:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_9BC0"))) PPC_WEAK_FUNC(snLeaveMachine_9BC0);
PPC_FUNC_IMPL(__imp__snLeaveMachine_9BC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // snLeaveMachine::vfn_11 (unnamed)  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9c4c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,12496
		ctx.r10.s64 = ctx.r10.s64 + 12496;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9C4C:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmLocalLeavePending_9C68"))) PPC_WEAK_FUNC(snHsmLocalLeavePending_9C68);
PPC_FUNC_IMPL(__imp__snHsmLocalLeavePending_9C68) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9cf4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,12536
		ctx.r10.s64 = ctx.r10.s64 + 12536;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9CF4:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmWaitingForReply_9D10"))) PPC_WEAK_FUNC(snHsmWaitingForReply_9D10);
PPC_FUNC_IMPL(__imp__snHsmWaitingForReply_9D10) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9d9c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,12616
		ctx.r10.s64 = ctx.r10.s64 + 12616;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9D9C:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmRemoteLeavePending_9DB8"))) PPC_WEAK_FUNC(snHsmRemoteLeavePending_9DB8);
PPC_FUNC_IMPL(__imp__snHsmRemoteLeavePending_9DB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9e44
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,12576
		ctx.r10.s64 = ctx.r10.s64 + 12576;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9E44:
	// blr
	return;
}

__attribute__((alias("__imp__snHsmAcceptingLeaveRequest_9E60"))) PPC_WEAK_FUNC(snHsmAcceptingLeaveRequest_9E60);
PPC_FUNC_IMPL(__imp__snHsmAcceptingLeaveRequest_9E60) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9eec
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,12656
		ctx.r10.s64 = ctx.r10.s64 + 12656;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9EEC:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9F08_fw"))) PPC_WEAK_FUNC(snSession_9F08_fw);
PPC_FUNC_IMPL(__imp__snSession_9F08_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823e9f94
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10440
		ctx.r10.s64 = ctx.r10.s64 + 10440;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823E9F94:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_9FB0_fw"))) PPC_WEAK_FUNC(snSession_9FB0_fw);
PPC_FUNC_IMPL(__imp__snSession_9FB0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ea03c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10460
		ctx.r10.s64 = ctx.r10.s64 + 10460;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EA03C:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrating_A058"))) PPC_WEAK_FUNC(snMigrating_A058);
PPC_FUNC_IMPL(__imp__snMigrating_A058) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ea0e4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,13420
		ctx.r10.s64 = ctx.r10.s64 + 13420;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EA0E4:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_A100_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_A100_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_A100_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r10.u32);
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ea18c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,13400
		ctx.r10.s64 = ctx.r10.s64 + 13400;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EA18C:
	// blr
	return;
}

__attribute__((alias("__imp__snMigrating_A1A8"))) PPC_WEAK_FUNC(snMigrating_A1A8);
PPC_FUNC_IMPL(__imp__snMigrating_A1A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ea234
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,13460
		ctx.r10.s64 = ctx.r10.s64 + 13460;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EA234:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_A250_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_A250_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_A250_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r30,0(r3)
	var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 0)/* SinglesNetworkClient::vtable@+0x0 */);
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// cmplwi cr6,r30,0
	// beq cr6,0x823ea3ec
	if (var_r30 != 0) {
	loc_823EA274:
		// addi r7,r30,24
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 24;
		// mr r10,r31
		ctx.r10.u64 = var_r31;
		// mr r11,r7
		ctx.r11.u64 = ctx.r7.u64;
	loc_823EA280:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r8,0(r10)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// cmpwi cr6,r9,0
		// subf r8,r8,r9
		ctx.r8.s64 = ctx.r9.s64 - ctx.r8.s64;
		// beq cr6,0x823ea2a4
		if (ctx.r9.s32 == 0) goto loc_823EA2A4;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpwi cr6,r8,0
		// beq cr6,0x823ea280
		if (ctx.r8.s32 == 0) goto loc_823EA280;
	loc_823EA2A4:
		// cmpwi cr6,r8,0
		// li r11,1
		ctx.r11.s64 = 1;
		// blt cr6,0x823ea2b4
		if (ctx.r8.s32 >= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EA2B4:
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r5,0
		// beq cr6,0x823ea2c8
		if (ctx.r5.u32 != 0) {
			// lwz r30,12(r30)
			var_r30 = (uint32_t)(PPC_LOAD_U32(var_r30 + 12));
			// b 0x823ea314
		} else {
		loc_823EA2C8:
			// mr r10,r7
			ctx.r10.u64 = ctx.r7.u64;
			// mr r11,r31
			ctx.r11.u64 = var_r31;
		loc_823EA2D0:
			// lbz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// lbz r4,0(r10)
			ctx.r4.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
			// cmpwi cr6,r9,0
			// subf r8,r4,r9
			ctx.r8.s64 = ctx.r9.s64 - ctx.r4.s64;
			// beq cr6,0x823ea2f4
			if (ctx.r9.s32 == 0) goto loc_823EA2F4;
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// cmpwi cr6,r8,0
			// beq cr6,0x823ea2d0
			if (ctx.r8.s32 == 0) goto loc_823EA2D0;
		loc_823EA2F4:
			// cmpwi cr6,r8,0
			// li r11,1
			ctx.r11.s64 = 1;
			// blt cr6,0x823ea304
			if (ctx.r8.s32 >= 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EA304:
			// clrlwi r10,r11,24
			ctx.r10.u64 = ctx.r11.u32 & 0xFF;
			// cmplwi cr6,r10,0
			// beq cr6,0x823ea320
			if (ctx.r10.u32 == 0) goto loc_823EA320;
			// lwz r30,8(r30)
			var_r30 = (uint32_t)(PPC_LOAD_U32(var_r30 + 8));
		}
	loc_823EA314:
		// cmplwi cr6,r30,0
		// bne cr6,0x823ea274
		if (var_r30 != 0) goto loc_823EA274;
		// b 0x823ea3ec
		goto loc_823EA3EC;
	loc_823EA320:
		// cmplwi cr6,r30,0
		// beq cr6,0x823ea3ec
		if (var_r30 == 0) goto loc_823EA3EC;
		// lbz r9,9(r3)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 9);
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// rlwinm r8,r9,0,25,25
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x40;
		// cmplwi cr6,r8,0
		// beq cr6,0x823ea3f0
		if (ctx.r8.u32 == 0) {
			// blr
			return;
		}
		// bl 0x823ecec0
		SinglesNetworkClient_CEC0_g(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823ea3ec
	while (ctx.r3.u32 != 0) {
		loc_823EA348:
			// addi r7,r3,24
			ctx.r7.s64 = ctx.r3.s64 + 24;
			// mr r10,r31
			ctx.r10.u64 = var_r31;
			// mr r11,r7
			ctx.r11.u64 = ctx.r7.u64;
		loc_823EA354:
			// lbz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// lbz r6,0(r10)
			ctx.r6.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
			// cmpwi cr6,r9,0
			// subf r8,r6,r9
			ctx.r8.s64 = ctx.r9.s64 - ctx.r6.s64;
			// beq cr6,0x823ea378
			if (ctx.r9.s32 == 0) goto loc_823EA378;
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// cmpwi cr6,r8,0
			// beq cr6,0x823ea354
			if (ctx.r8.s32 == 0) goto loc_823EA354;
		loc_823EA378:
			// cmpwi cr6,r8,0
			// li r11,1
			ctx.r11.s64 = 1;
			// blt cr6,0x823ea388
			if (ctx.r8.s32 >= 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EA388:
			// clrlwi r4,r11,24
			ctx.r4.u64 = ctx.r11.u32 & 0xFF;
			// cmplwi cr6,r4,0
			// bne cr6,0x823ea3ec
			if (ctx.r4.u32 != 0) goto loc_823EA3EC;
			// mr r10,r7
			ctx.r10.u64 = ctx.r7.u64;
			// mr r11,r31
			ctx.r11.u64 = var_r31;
		loc_823EA39C:
			// lbz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// lbz r8,0(r10)
			ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
			// cmpwi cr6,r9,0
			// subf r8,r8,r9
			ctx.r8.s64 = ctx.r9.s64 - ctx.r8.s64;
			// beq cr6,0x823ea3c0
			if (ctx.r9.s32 == 0) goto loc_823EA3C0;
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// cmpwi cr6,r8,0
			// beq cr6,0x823ea39c
			if (ctx.r8.s32 == 0) goto loc_823EA39C;
		loc_823EA3C0:
			// cmpwi cr6,r8,0
			// li r11,1
			ctx.r11.s64 = 1;
			// blt cr6,0x823ea3d0
			if (ctx.r8.s32 >= 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EA3D0:
			// clrlwi r6,r11,24
			ctx.r6.u64 = ctx.r11.u32 & 0xFF;
			// cmplwi cr6,r6,0
			// bne cr6,0x823ea3ec
			if (ctx.r6.u32 != 0) goto loc_823EA3EC;
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// bl 0x823ecec0
			SinglesNetworkClient_CEC0_g(ctx, base);
			// cmplwi cr6,r3,0
			// bne cr6,0x823ea348
	}
	}
loc_823EA3EC:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
loc_823EA3F0:
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_A408"))) PPC_WEAK_FUNC(atSingleton_A408);
PPC_FUNC_IMPL(__imp__atSingleton_A408) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r27,1
	var_r27 = 1;
	// lwz r11,0(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 0);
	// cmplwi cr6,r11,0
	// bne cr6,0x823ea438
	if (ctx.r11.u32 == 0) {
		// stw r30,0(r28)
		PPC_STORE_U32(var_r28 + 0, var_r30);
		// b 0x823ea548
	} else {
	loc_823EA438:
		// mr r31,r11
		var_r31 = ctx.r11.u32;
		// addi r6,r29,10
		ctx.r6.s64 = (int64_t)(int32_t)var_r29 + 10;
	loc_823EA440:
		// addi r7,r31,42
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 42;
		// mr r11,r6
		ctx.r11.u64 = ctx.r6.u64;
		// mr r10,r7
		ctx.r10.u64 = ctx.r7.u64;
		// addi r8,r11,6
		ctx.r8.s64 = ctx.r11.s64 + 6;
	loc_823EA450:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r5,0(r10)
		ctx.r5.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// subf. r9,r5,r9
		ctx.r9.s64 = ctx.r9.s64 - ctx.r5.s64;
		// bne 0x823ea470
		if (ctx.r9.s32 != 0) goto loc_823EA470;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpw cr6,r11,r8
		// bne cr6,0x823ea450
		if (ctx.r11.s32 != ctx.r8.s32) goto loc_823EA450;
	loc_823EA470:
		// cmpwi cr6,r9,0
		// li r11,1
		ctx.r11.s64 = 1;
		// blt cr6,0x823ea480
		if (ctx.r9.s32 >= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EA480:
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r3,0
		// beq cr6,0x823ea4a0
		if (ctx.r3.u32 != 0) {
			// lwz r11,20(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
			// cmplwi cr6,r11,0
			// bne cr6,0x823ea504
			if (ctx.r11.u32 != 0) goto loc_823EA504;
			// stw r30,20(r31)
			PPC_STORE_U32(var_r31 + 20, var_r30);
			// b 0x823ea52c
			goto loc_823EA52C;
		}
	loc_823EA4A0:
		// mr r11,r7
		ctx.r11.u64 = ctx.r7.u64;
		// mr r10,r6
		ctx.r10.u64 = ctx.r6.u64;
		// addi r8,r11,6
		ctx.r8.s64 = ctx.r11.s64 + 6;
	loc_823EA4AC:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r7,0(r10)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// subf. r9,r7,r9
		ctx.r9.s64 = ctx.r9.s64 - ctx.r7.s64;
		// bne 0x823ea4cc
		if (ctx.r9.s32 != 0) goto loc_823EA4CC;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpw cr6,r11,r8
		// bne cr6,0x823ea4ac
		if (ctx.r11.s32 != ctx.r8.s32) goto loc_823EA4AC;
	loc_823EA4CC:
		// cmpwi cr6,r9,0
		// li r11,1
		ctx.r11.s64 = 1;
		// blt cr6,0x823ea4dc
		if (ctx.r9.s32 >= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EA4DC:
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r4,0
		// bne cr6,0x823ea4f8
		if (ctx.r4.u32 == 0) {
			// lbz r3,9(r28)
			ctx.r3.u64 = PPC_LOAD_U8(var_r28 + 9);
			// rlwinm r11,r3,0,25,25
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0x40;
			// cmplwi cr6,r11,0
			// beq cr6,0x823ea514
			if (ctx.r11.u32 == 0) goto loc_823EA514;
		}
	loc_823EA4F8:
		// lwz r11,24(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 24);
		// cmplwi cr6,r11,0
		// beq cr6,0x823ea528
		if (ctx.r11.u32 != 0) {
		loc_823EA504:
			// mr r31,r11
			var_r31 = ctx.r11.u32;
			// cmplwi cr6,r11,0
			// bne cr6,0x823ea440
			if (ctx.r11.u32 != 0) goto loc_823EA440;
			// b 0x823ea52c
			goto loc_823EA52C;
		loc_823EA514:
			// lis r11,-32250
			ctx.r11.s64 = -2113536000;
			// addi r3,r11,28520
			ctx.r3.s64 = ctx.r11.s64 + 28520;
			// bl 0x8240e6d0
			nop_8240E6D0(ctx, base);
			// li r27,0
			var_r27 = 0;
			// b 0x823ea52c
		} else {
		loc_823EA528:
			// stw r30,24(r31)
			PPC_STORE_U32(var_r31 + 24, var_r30);
		}
	loc_823EA52C:
		// clrlwi r10,r27,24
		ctx.r10.u64 = var_r27 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823ea548
		if (ctx.r10.u32 == 0) goto loc_823EA548;
		// lwz r9,28(r30)
		ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 28);
		// clrlwi r8,r9,31
		ctx.r8.u64 = ctx.r9.u32 & 0x1;
		// or r7,r8,r31
		ctx.r7.u64 = ctx.r8.u64 | var_r31;
		// stw r7,28(r30)
		PPC_STORE_U32(var_r30 + 28, ctx.r7.u32);
	}
loc_823EA548:
	// clrlwi r6,r27,24
	ctx.r6.u64 = var_r27 & 0xFF;
	// cmplwi cr6,r6,0
	// beq cr6,0x823ea75c
	if (ctx.r6.u32 != 0) {
		// addi r10,r30,32
		ctx.r10.s64 = (int64_t)(int32_t)var_r30 + 32;
		// mr r11,r29
		ctx.r11.u64 = var_r29;
		// li r9,9
		ctx.r9.s64 = 9;
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
	loc_823EA564:
		// lwz r5,0(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r5,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r5.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ea564
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EA564;
		// lbz r4,9(r28)
		ctx.r4.u64 = PPC_LOAD_U8(var_r28 + 9);
		// rlwinm r3,r4,0,0,24
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r3,0
		// beq cr6,0x823ea750
		if (ctx.r3.u32 != 0) {
			// lwz r11,28(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 28);
			// rlwinm r11,r11,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
			// mr r31,r11
			var_r31 = ctx.r11.u32;
			// cmplwi cr6,r31,0
			// stw r11,28(r30)
			PPC_STORE_U32(var_r30 + 28, ctx.r11.u32);
			// beq cr6,0x823ea740
			if (var_r31 != 0) {
				// lwz r10,28(r31)
				ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 28);
				// rlwinm r4,r10,0,0,30
				ctx.r4.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFE;
				// cmplwi cr6,r4,0
				// beq cr6,0x823ea740
			while (ctx.r4.u32 != 0) {
				loc_823EA5B0:
					// lwz r10,28(r31)
					ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 28);
					// clrlwi r9,r10,31
					ctx.r9.u64 = ctx.r10.u32 & 0x1;
					// cmplwi cr6,r9,0
					// bne cr6,0x823ea740
					if (ctx.r9.u32 != 0) goto loc_823EA740;
					// lwz r8,20(r4)
					ctx.r8.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
					// cmplw cr6,r8,r31
					// bne cr6,0x823ea678
					if (ctx.r8.u32 == var_r31) {
						// lwz r11,24(r4)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
						// cmplwi cr6,r11,0
						// beq cr6,0x823ea610
						if (ctx.r11.u32 != 0) {
							// lwz r7,28(r11)
							ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
							// clrlwi r6,r7,31
							ctx.r6.u64 = ctx.r7.u32 & 0x1;
							// cmplwi cr6,r6,0
							// bne cr6,0x823ea610
							if (ctx.r6.u32 != 0) goto loc_823EA610;
							// ori r5,r10,1
							ctx.r5.u64 = ctx.r10.u64 | 1;
							// mr r30,r4
							var_r30 = ctx.r4.u32;
							// stw r5,28(r31)
							PPC_STORE_U32(var_r31 + 28, ctx.r5.u32);
							// lwz r3,28(r11)
							ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
							// ori r10,r3,1
							ctx.r10.u64 = ctx.r3.u64 | 1;
							// stw r10,28(r11)
							PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
							// lwz r9,28(r4)
							ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
							// rlwinm r8,r9,0,0,30
							ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
							// stw r8,28(r4)
							PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r8.u32);
							// b 0x823ea720
							goto loc_823EA720;
						}
					loc_823EA610:
						// lwz r7,24(r31)
						ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 24);
						// cmplw cr6,r7,r30
						// bne cr6,0x823ea64c
						if (ctx.r7.u32 == var_r30) {
							// mr r4,r31
							ctx.r4.u64 = var_r31;
							// mr r3,r28
							ctx.r3.u64 = var_r28;
							// mr r30,r31
							var_r30 = (uint32_t)(var_r31);
							// bl 0x823eb700
							atSingleton_B700_2h(ctx, base);
							// lwz r6,28(r31)
							ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 28);
							// rlwinm r31,r6,0,0,30
							var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFFE);
							// cmplwi cr6,r31,0
							// beq cr6,0x823ea648
							if (var_r31 != 0) {
								// lwz r5,28(r30)
								ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 28);
								// rlwinm r4,r5,0,0,30
								ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
								// b 0x823ea64c
							} else {
							loc_823EA648:
								// li r4,0
								ctx.r4.s64 = 0;
							}
						}
					loc_823EA64C:
						// lwz r3,28(r31)
						ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 28);
						// cmplwi cr6,r4,0
						// ori r11,r3,1
						ctx.r11.u64 = ctx.r3.u64 | 1;
						// stw r11,28(r31)
						PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
						// beq cr6,0x823ea720
						if (ctx.r4.u32 == 0) goto loc_823EA720;
						// lwz r10,28(r4)
						ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
						// mr r3,r28
						ctx.r3.u64 = var_r28;
						// rlwinm r9,r10,0,0,30
						ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFE;
						// stw r9,28(r4)
						PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r9.u32);
						// bl 0x823eb678
						atSingleton_B678_2hr(ctx, base);
						// b 0x823ea720
					} else {
					loc_823EA678:
						// lwz r11,20(r4)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
						// cmplwi cr6,r11,0
						// beq cr6,0x823ea6bc
						if (ctx.r11.u32 != 0) {
							// lwz r8,28(r11)
							ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
							// clrlwi r7,r8,31
							ctx.r7.u64 = ctx.r8.u32 & 0x1;
							// cmplwi cr6,r7,0
							// bne cr6,0x823ea6bc
							if (ctx.r7.u32 != 0) goto loc_823EA6BC;
							// ori r6,r10,1
							ctx.r6.u64 = ctx.r10.u64 | 1;
							// mr r30,r4
							var_r30 = ctx.r4.u32;
							// stw r6,28(r31)
							PPC_STORE_U32(var_r31 + 28, ctx.r6.u32);
							// lwz r5,28(r11)
							ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
							// ori r3,r5,1
							ctx.r3.u64 = ctx.r5.u64 | 1;
							// stw r3,28(r11)
							PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r3.u32);
							// lwz r11,28(r4)
							ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
							// rlwinm r10,r11,0,0,30
							ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
							// stw r10,28(r4)
							PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r10.u32);
							// b 0x823ea720
						} else {
						loc_823EA6BC:
							// lwz r9,20(r31)
							ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 20);
							// cmplw cr6,r9,r30
							// bne cr6,0x823ea6f8
							if (ctx.r9.u32 == var_r30) {
								// mr r4,r31
								ctx.r4.u64 = var_r31;
								// mr r3,r28
								ctx.r3.u64 = var_r28;
								// mr r30,r31
								var_r30 = (uint32_t)(var_r31);
								// bl 0x823eb678
								atSingleton_B678_2hr(ctx, base);
								// lwz r8,28(r31)
								ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 28);
								// rlwinm r31,r8,0,0,30
								var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE);
								// cmplwi cr6,r31,0
								// beq cr6,0x823ea6f4
								if (var_r31 != 0) {
									// lwz r7,28(r30)
									ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 28);
									// rlwinm r4,r7,0,0,30
									ctx.r4.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0xFFFFFFFE;
									// b 0x823ea6f8
								} else {
								loc_823EA6F4:
									// li r4,0
									ctx.r4.s64 = 0;
								}
							}
						loc_823EA6F8:
							// lwz r6,28(r31)
							ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 28);
							// cmplwi cr6,r4,0
							// ori r5,r6,1
							ctx.r5.u64 = ctx.r6.u64 | 1;
							// stw r5,28(r31)
							PPC_STORE_U32(var_r31 + 28, ctx.r5.u32);
							// beq cr6,0x823ea720
							if (ctx.r4.u32 == 0) goto loc_823EA720;
							// lwz r11,28(r4)
							ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
							// mr r3,r28
							ctx.r3.u64 = var_r28;
							// rlwinm r10,r11,0,0,30
							ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
							// stw r10,28(r4)
							PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r10.u32);
							// bl 0x823eb700
							atSingleton_B700_2h(ctx, base);
						}
					}
				loc_823EA720:
					// lwz r9,28(r30)
					ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 28);
					// rlwinm r31,r9,0,0,30
					var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE);
					// cmplwi cr6,r31,0
					// beq cr6,0x823ea740
					if (var_r31 == 0) goto loc_823EA740;
					// lwz r8,28(r31)
					ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 28);
					// rlwinm r4,r8,0,0,30
					ctx.r4.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
					// cmplwi cr6,r4,0
					// bne cr6,0x823ea5b0
			}
			}
		loc_823EA740:
			// lwz r11,0(r28)
			ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 0);
			// lwz r7,28(r11)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
			// ori r6,r7,1
			ctx.r6.u64 = ctx.r7.u64 | 1;
			// stw r6,28(r11)
			PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
		}
	loc_823EA750:
		// lwz r11,4(r28)
		ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 4);
		// addi r5,r11,1
		ctx.r5.s64 = ctx.r11.s64 + 1;
		// stw r5,4(r28)
		PPC_STORE_U32(var_r28 + 4, ctx.r5.u32);
	}
loc_823EA75C:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__util_A768"))) PPC_WEAK_FUNC(util_A768);
PPC_FUNC_IMPL(__imp__util_A768) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8236c818
	game_C818(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823ea7b4
while (ctx.r10.u32 == 0) {
	loc_823EA78C:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823ea980
		snSession_A980_p45(ctx, base);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823eb138
		atSingleton_B138(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8236c818
		game_C818(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823ea78c
}
loc_823EA7B4:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_A7C8_g"))) PPC_WEAK_FUNC(snLeaveMachine_A7C8_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_A7C8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r30,0(r3)
	var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 0));
	// cmplwi cr6,r30,0
	// beq cr6,0x823ea964
	if (var_r30 != 0) {
		// addi r31,r4,10
		var_r31 = (uint32_t)(ctx.r4.s64 + 10);
	loc_823EA7EC:
		// addi r7,r30,42
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 42;
		// mr r10,r31
		ctx.r10.u64 = var_r31;
		// mr r11,r7
		ctx.r11.u64 = ctx.r7.u64;
		// addi r8,r11,6
		ctx.r8.s64 = ctx.r11.s64 + 6;
	loc_823EA7FC:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r6,0(r10)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// subf. r9,r6,r9
		ctx.r9.s64 = ctx.r9.s64 - ctx.r6.s64;
		// bne 0x823ea81c
		if (ctx.r9.s32 != 0) goto loc_823EA81C;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpw cr6,r11,r8
		// bne cr6,0x823ea7fc
		if (ctx.r11.s32 != ctx.r8.s32) goto loc_823EA7FC;
	loc_823EA81C:
		// cmpwi cr6,r9,0
		// li r11,1
		ctx.r11.s64 = 1;
		// blt cr6,0x823ea82c
		if (ctx.r9.s32 >= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EA82C:
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r4,0
		// beq cr6,0x823ea840
		if (ctx.r4.u32 != 0) {
			// lwz r30,24(r30)
			var_r30 = (uint32_t)(PPC_LOAD_U32(var_r30 + 24));
			// b 0x823ea88c
		} else {
		loc_823EA840:
			// mr r11,r31
			ctx.r11.u64 = var_r31;
			// mr r10,r7
			ctx.r10.u64 = ctx.r7.u64;
			// addi r8,r11,6
			ctx.r8.s64 = ctx.r11.s64 + 6;
		loc_823EA84C:
			// lbz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// lbz r7,0(r10)
			ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
			// subf. r9,r7,r9
			ctx.r9.s64 = ctx.r9.s64 - ctx.r7.s64;
			// bne 0x823ea86c
			if (ctx.r9.s32 != 0) goto loc_823EA86C;
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// cmpw cr6,r11,r8
			// bne cr6,0x823ea84c
			if (ctx.r11.s32 != ctx.r8.s32) goto loc_823EA84C;
		loc_823EA86C:
			// cmpwi cr6,r9,0
			// li r11,1
			ctx.r11.s64 = 1;
			// blt cr6,0x823ea87c
			if (ctx.r9.s32 >= 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EA87C:
			// clrlwi r5,r11,24
			ctx.r5.u64 = ctx.r11.u32 & 0xFF;
			// cmplwi cr6,r5,0
			// beq cr6,0x823ea898
			if (ctx.r5.u32 == 0) goto loc_823EA898;
			// lwz r30,20(r30)
			var_r30 = (uint32_t)(PPC_LOAD_U32(var_r30 + 20));
		}
	loc_823EA88C:
		// cmplwi cr6,r30,0
		// bne cr6,0x823ea7ec
		if (var_r30 != 0) goto loc_823EA7EC;
		// b 0x823ea964
		goto loc_823EA964;
	loc_823EA898:
		// cmplwi cr6,r30,0
		// beq cr6,0x823ea964
		if (var_r30 == 0) goto loc_823EA964;
		// lbz r4,9(r3)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r3.u32 + 9);
		// rlwinm r3,r4,0,25,25
		ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0x40;
		// cmplwi cr6,r3,0
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// beq cr6,0x823ea968
		if (ctx.r3.u32 == 0) {
			// blr
			return;
		}
		// bl 0x823eb610
		snSession_B610_p44(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823ea964
	while (ctx.r3.u32 != 0) {
		loc_823EA8C0:
			// addi r7,r3,42
			ctx.r7.s64 = ctx.r3.s64 + 42;
			// mr r10,r31
			ctx.r10.u64 = var_r31;
			// mr r11,r7
			ctx.r11.u64 = ctx.r7.u64;
			// addi r8,r11,6
			ctx.r8.s64 = ctx.r11.s64 + 6;
		loc_823EA8D0:
			// lbz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// lbz r6,0(r10)
			ctx.r6.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
			// subf. r9,r6,r9
			ctx.r9.s64 = ctx.r9.s64 - ctx.r6.s64;
			// bne 0x823ea8f0
			if (ctx.r9.s32 != 0) goto loc_823EA8F0;
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// cmpw cr6,r11,r8
			// bne cr6,0x823ea8d0
			if (ctx.r11.s32 != ctx.r8.s32) goto loc_823EA8D0;
		loc_823EA8F0:
			// cmpwi cr6,r9,0
			// li r11,1
			ctx.r11.s64 = 1;
			// blt cr6,0x823ea900
			if (ctx.r9.s32 >= 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EA900:
			// clrlwi r4,r11,24
			ctx.r4.u64 = ctx.r11.u32 & 0xFF;
			// cmplwi cr6,r4,0
			// bne cr6,0x823ea964
			if (ctx.r4.u32 != 0) goto loc_823EA964;
			// mr r11,r31
			ctx.r11.u64 = var_r31;
			// mr r10,r7
			ctx.r10.u64 = ctx.r7.u64;
			// addi r8,r11,6
			ctx.r8.s64 = ctx.r11.s64 + 6;
		loc_823EA918:
			// lbz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
			// lbz r7,0(r10)
			ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
			// subf. r9,r7,r9
			ctx.r9.s64 = ctx.r9.s64 - ctx.r7.s64;
			// bne 0x823ea938
			if (ctx.r9.s32 != 0) goto loc_823EA938;
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// addi r10,r10,1
			ctx.r10.s64 = ctx.r10.s64 + 1;
			// cmpw cr6,r11,r8
			// bne cr6,0x823ea918
			if (ctx.r11.s32 != ctx.r8.s32) goto loc_823EA918;
		loc_823EA938:
			// cmpwi cr6,r9,0
			// li r11,1
			ctx.r11.s64 = 1;
			// blt cr6,0x823ea948
			if (ctx.r9.s32 >= 0) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EA948:
			// clrlwi r5,r11,24
			ctx.r5.u64 = ctx.r11.u32 & 0xFF;
			// cmplwi cr6,r5,0
			// bne cr6,0x823ea964
			if (ctx.r5.u32 != 0) goto loc_823EA964;
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// bl 0x823eb610
			snSession_B610_p44(ctx, base);
			// cmplwi cr6,r3,0
			// bne cr6,0x823ea8c0
	}
	}
loc_823EA964:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
loc_823EA968:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_A980_p45"))) PPC_WEAK_FUNC(snSession_A980_p45);
PPC_FUNC_IMPL(__imp__snSession_A980_p45) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,0(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r3,0
	// beq cr6,0x823ea9ac
	if (ctx.r3.u32 != 0) {
		// lwz r11,20(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// cmplwi cr6,r11,0
		// beqlr cr6
		if (ctx.r11.u32 == 0) return;
	loc_823EA998:
		// lwz r3,20(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// lwz r10,20(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// cmplwi cr6,r10,0
		// bne cr6,0x823ea998
		if (ctx.r10.u32 != 0) goto loc_823EA998;
		// blr
		return;
	}
loc_823EA9AC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__util_A9B8"))) PPC_WEAK_FUNC(util_A9B8);
PPC_FUNC_IMPL(__imp__util_A9B8) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// cmplwi cr6,r10,0
	// beq cr6,0x823ea9ec
	if (ctx.r10.u32 != 0) {
		// rotlwi r3,r10,0
		ctx.r3.u64 = ctx.r10.u32;
		// lwz r9,20(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// cmplwi cr6,r9,0
		// beqlr cr6
		if (ctx.r9.u32 == 0) return;
	loc_823EA9D8:
		// lwz r3,20(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// lwz r8,20(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
		// cmplwi cr6,r8,0
		// bne cr6,0x823ea9d8
		if (ctx.r8.u32 != 0) goto loc_823EA9D8;
		// blr
		return;
	}
loc_823EA9EC:
	// lwz r7,28(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// rlwinm r3,r7,0,0,30
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
loc_823EA9FC:
	// lwz r6,24(r3)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// cmplw cr6,r6,r11
	// bnelr cr6
	if (ctx.r6.u32 != ctx.r11.u32) return;
	// lwz r5,28(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// rlwinm r3,r5,0,0,30
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r3,0
	// bne cr6,0x823ea9fc
	if (ctx.r3.u32 != 0) goto loc_823EA9FC;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_AA20_fw"))) PPC_WEAK_FUNC(snSession_AA20_fw);
PPC_FUNC_IMPL(__imp__snSession_AA20_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eaaac
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10640
		ctx.r10.s64 = ctx.r10.s64 + 10640;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EAAAC:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_AAC8_fw"))) PPC_WEAK_FUNC(snSession_AAC8_fw);
PPC_FUNC_IMPL(__imp__snSession_AAC8_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eab54
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10400
		ctx.r10.s64 = ctx.r10.s64 + 10400;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EAB54:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_AB70_gen"))) PPC_WEAK_FUNC(snSession_AB70_gen);
PPC_FUNC_IMPL(__imp__snSession_AB70_gen) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eabfc
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10420
		ctx.r10.s64 = ctx.r10.s64 + 10420;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EABFC:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_AC18_fw"))) PPC_WEAK_FUNC(snSession_AC18_fw);
PPC_FUNC_IMPL(__imp__snSession_AC18_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eaca4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10520
		ctx.r10.s64 = ctx.r10.s64 + 10520;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EACA4:
	// blr
	return;
}

__attribute__((alias("__imp__snSession_ACC0_fw"))) PPC_WEAK_FUNC(snSession_ACC0_fw);
PPC_FUNC_IMPL(__imp__snSession_ACC0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// lwz r31,56(r3)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,12
	ctx.r4.s64 = 12;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// lwz r8,4(r9)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ead4c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r30,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// addi r10,r10,10580
		ctx.r10.s64 = ctx.r10.s64 + 10580;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r5,0(r7)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// stw r5,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r5.u32);
		// lwz r11,4(r7)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r10,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
		// stw r11,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EAD4C:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_AD68_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_AD68_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_AD68_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32161
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,13260(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 13260);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,36(r31)
	PPC_STORE_U32(var_r31 + 36, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// li r11,8128
	ctx.r11.s64 = 8128;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r4,7012(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7012);  /* glob:lbl_825D1B64 @ 0x825d1b64 */
	// bl 0x822386f0
	SinglesNetworkClient_86F0_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823e91f0
	SinglesNetworkClient_91F0_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r10,0
	// beq cr6,0x823eae04
	if (ctx.r10.u32 != 0) {
		// addi r4,r30,72
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 72;
		// bl 0x823e92a8
		SinglesNetworkClient_92A8_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EAE04:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_AE20_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_AE20_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_AE20_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32161
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,13260(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 13260);  /* glob:0x825d33cc */
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,36(r31)
	PPC_STORE_U32(var_r31 + 36, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// li r11,8128
	ctx.r11.s64 = 8128;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r4,7000(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7000);  /* glob:lbl_825D1B58 @ 0x825d1b58 */
	// bl 0x822386f0
	SinglesNetworkClient_86F0_g(ctx, base);
	// li r5,64
	ctx.r5.s64 = 64;
	// ld r4,0(r30)
	ctx.r4.u64 = PPC_LOAD_U64(var_r30 + 0);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823dd1f0
	SinglesNetworkClient_D1F0_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r10,0
	// beq cr6,0x823eaef4
	if (ctx.r10.u32 != 0) {
		// li r5,64
		ctx.r5.s64 = 64;
		// ld r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U64(var_r30 + 8);
		// bl 0x823dd1f0
		SinglesNetworkClient_D1F0_g(ctx, base);
		// clrlwi r9,r3,24
		ctx.r9.u64 = ctx.r3.u32 & 0xFF;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplwi cr6,r9,0
		// beq cr6,0x823eaef4
		if (ctx.r9.u32 == 0) {
			// blr
			return;
		}
		// addi r4,r30,16
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 16;
		// bl 0x823f3c88
		SinglesNetworkClient_3C88_g(ctx, base);
		// clrlwi r8,r3,24
		ctx.r8.u64 = ctx.r3.u32 & 0xFF;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplwi cr6,r8,0
		// beq cr6,0x823eaef4
		if (ctx.r8.u32 == 0) {
			// blr
			return;
		}
		// lbz r4,296(r30)
		ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 296);
		// bl 0x820d6918
		SinglesNetworkClient_6918_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EAEF4:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_AF10_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_AF10_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_AF10_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32161
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,13260(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 13260);  /* glob:0x825d33cc */
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,36(r31)
	PPC_STORE_U32(var_r31 + 36, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// li r11,8128
	ctx.r11.s64 = 8128;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r4,7120(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7120);  /* glob:lbl_825D1BD0 @ 0x825d1bd0 */
	// bl 0x822386f0
	SinglesNetworkClient_86F0_g(ctx, base);
	// li r5,64
	ctx.r5.s64 = 64;
	// ld r4,0(r30)
	ctx.r4.u64 = PPC_LOAD_U64(var_r30 + 0);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823dd1f0
	SinglesNetworkClient_D1F0_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r10,0
	// beq cr6,0x823eafb0
	if (ctx.r10.u32 != 0) {
		// lbz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 8);
		// bl 0x820d6918
		SinglesNetworkClient_6918_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EAFB0:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_AFC8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_AFC8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_AFC8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32161
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,13260(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 13260);  /* glob:0x825d33cc */
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,36(r31)
	PPC_STORE_U32(var_r31 + 36, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r11,24(r31)
	PPC_STORE_U32(var_r31 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(var_r31 + 28, ctx.r11.u32);
	// stw r11,32(r31)
	PPC_STORE_U32(var_r31 + 32, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// li r11,8128
	ctx.r11.s64 = 8128;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r4,7108(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7108);  /* glob:lbl_825D1BC4 @ 0x825d1bc4 */
	// bl 0x822386f0
	SinglesNetworkClient_86F0_g(ctx, base);
	// li r5,64
	ctx.r5.s64 = 64;
	// ld r4,0(r30)
	ctx.r4.u64 = PPC_LOAD_U64(var_r30 + 0);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823dd1f0
	SinglesNetworkClient_D1F0_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r10,0
	// beq cr6,0x823eb0a4
	if (ctx.r10.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// lwz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
		// bl 0x820d6838
		SinglesNetworkClient_6838_g(ctx, base);
		// clrlwi r9,r3,24
		ctx.r9.u64 = ctx.r3.u32 & 0xFF;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplwi cr6,r9,0
		// beq cr6,0x823eb0a4
		if (ctx.r9.u32 == 0) {
			// blr
			return;
		}
		// li r5,64
		ctx.r5.s64 = 64;
		// ld r4,16(r30)
		ctx.r4.u64 = PPC_LOAD_U64(var_r30 + 16);
		// bl 0x823dd1f0
		SinglesNetworkClient_D1F0_g(ctx, base);
		// clrlwi r8,r3,24
		ctx.r8.u64 = ctx.r3.u32 & 0xFF;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// cmplwi cr6,r8,0
		// beq cr6,0x823eb0a4
		if (ctx.r8.u32 == 0) {
			// blr
			return;
		}
		// li r5,96
		ctx.r5.s64 = 96;
		// addi r4,r30,24
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 24;
		// bl 0x820d67c8
		SinglesNetworkClient_67C8_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EB0A4:
	// blr
	return;
}

__attribute__((alias("__imp__thunk_fn_823DDD70"))) PPC_WEAK_FUNC(thunk_fn_823DDD70);
PPC_FUNC_IMPL(__imp__thunk_fn_823DDD70) {
	PPC_FUNC_PROLOGUE();
	// b 0x823ddd70
	snSession_DD70_w(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823DDF08"))) PPC_WEAK_FUNC(thunk_fn_823DDF08);
PPC_FUNC_IMPL(__imp__thunk_fn_823DDF08) {
	PPC_FUNC_PROLOGUE();
	// b 0x823ddf08
	snSession_DF08_w(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823DE1A0"))) PPC_WEAK_FUNC(thunk_fn_823DE1A0);
PPC_FUNC_IMPL(__imp__thunk_fn_823DE1A0) {
	PPC_FUNC_PROLOGUE();
	// b 0x823de1a0
	snSession_E1A0_w(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823F23E8"))) PPC_WEAK_FUNC(thunk_fn_823F23E8);
PPC_FUNC_IMPL(__imp__thunk_fn_823F23E8) {
	PPC_FUNC_PROLOGUE();
	// b 0x823f23e8
	SinglesNetworkClient_23E8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823DE6C8"))) PPC_WEAK_FUNC(thunk_fn_823DE6C8);
PPC_FUNC_IMPL(__imp__thunk_fn_823DE6C8) {
	PPC_FUNC_PROLOGUE();
	// b 0x823de6c8
	snSession_E6C8(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823DE990"))) PPC_WEAK_FUNC(thunk_fn_823DE990);
PPC_FUNC_IMPL(__imp__thunk_fn_823DE990) {
	PPC_FUNC_PROLOGUE();
	// b 0x823de990
	snSession_E990(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823DEB48"))) PPC_WEAK_FUNC(thunk_fn_823DEB48);
PPC_FUNC_IMPL(__imp__thunk_fn_823DEB48) {
	PPC_FUNC_PROLOGUE();
	// b 0x823deb48
	snSession_EB48_w(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823DEF68"))) PPC_WEAK_FUNC(thunk_fn_823DEF68);
PPC_FUNC_IMPL(__imp__thunk_fn_823DEF68) {
	PPC_FUNC_PROLOGUE();
	// b 0x823def68
	snSession_EF68(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E0758"))) PPC_WEAK_FUNC(thunk_fn_823E0758);
PPC_FUNC_IMPL(__imp__thunk_fn_823E0758) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e0758
	snSession_0758_w(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E2D48"))) PPC_WEAK_FUNC(thunk_fn_823E2D48);
PPC_FUNC_IMPL(__imp__thunk_fn_823E2D48) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e2d48
	snSession_2D48(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E3268"))) PPC_WEAK_FUNC(thunk_fn_823E3268);
PPC_FUNC_IMPL(__imp__thunk_fn_823E3268) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e3268
	snSession_3268(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E88F8"))) PPC_WEAK_FUNC(thunk_fn_823E88F8);
PPC_FUNC_IMPL(__imp__thunk_fn_823E88F8) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e88f8
	SinglesNetworkClient_88F8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E4110"))) PPC_WEAK_FUNC(thunk_fn_823E4110);
PPC_FUNC_IMPL(__imp__thunk_fn_823E4110) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e4110
	snSession_4110(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E45B0"))) PPC_WEAK_FUNC(thunk_fn_823E45B0);
PPC_FUNC_IMPL(__imp__thunk_fn_823E45B0) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e45b0
	snSession_45B0(ctx, base);
	return;
}

__attribute__((alias("__imp__thunk_fn_823E76E0"))) PPC_WEAK_FUNC(thunk_fn_823E76E0);
PPC_FUNC_IMPL(__imp__thunk_fn_823E76E0) {
	PPC_FUNC_PROLOGUE();
	// b 0x823e76e0
	SinglesNetworkClient_76E0_g(ctx, base);
	return;
}

__attribute__((alias("__imp__atSingleton_B138"))) PPC_WEAK_FUNC(atSingleton_B138);
PPC_FUNC_IMPL(__imp__atSingleton_B138) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,20(r29)
	ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 20);
	// cmplwi cr6,r11,0
	// beq cr6,0x823eb188
	if (ctx.r11.u32 != 0) {
		// lwz r10,24(r29)
		ctx.r10.u64 = PPC_LOAD_U32(var_r29 + 24);
		// cmplwi cr6,r10,0
		// beq cr6,0x823eb188
		if (ctx.r10.u32 == 0) goto loc_823EB188;
		// lwz r9,4(r30)
		ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 4)/* atSingleton::flags@+0x4 */;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// clrlwi r8,r9,31
		ctx.r8.u64 = ctx.r9.u32 & 0x1;
		// cmpwi cr6,r8,0
		// beq cr6,0x823eb180
		if (ctx.r8.s32 != 0) {
			// bl 0x823ea9b8
			util_A9B8(ctx, base);
			// b 0x823eb18c
			goto loc_823EB18C;
		}
	loc_823EB180:
		// bl 0x823eb610
		snSession_B610_p44(ctx, base);
		// b 0x823eb18c
	} else {
	loc_823EB188:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
	}
loc_823EB18C:
	// lwz r7,20(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// cmplwi cr6,r7,0
	// beq cr6,0x823eb1a0
	if (ctx.r7.u32 != 0) {
		// rotlwi r10,r7,0
		ctx.r10.u64 = ctx.r7.u32;
		// b 0x823eb1a4
	} else {
	loc_823EB1A0:
		// lwz r10,24(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	}
loc_823EB1A4:
	// cmplwi cr6,r10,0
	// beq cr6,0x823eb1bc
	if (ctx.r10.u32 != 0) {
		// lwz r6,28(r3)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		// lwz r5,28(r10)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
		// rlwimi r6,r5,0,31,31
		ctx.r6.u64 = (ctx.r5.u32 & 0x1) | (ctx.r6.u64 & 0xFFFFFFFFFFFFFFFE);
		// stw r6,28(r10)
		PPC_STORE_U32(ctx.r10.u32 + 28, ctx.r6.u32);
	}
loc_823EB1BC:
	// lwz r4,28(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// rlwinm r11,r4,0,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r11,0
	// bne cr6,0x823eb1d4
	if (ctx.r11.u32 == 0) {
		// stw r10,0(r30)
		PPC_STORE_U32(var_r30 + 0,/* atSingleton::vtable@+0x0 */ ctx.r10.u32);
		// b 0x823eb1ec
	} else {
	loc_823EB1D4:
		// lwz r9,20(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
		// cmplw cr6,r9,r3
		// bne cr6,0x823eb1e8
		if (ctx.r9.u32 == ctx.r3.u32) {
			// stw r10,20(r11)
			PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
			// b 0x823eb1ec
		} else {
		loc_823EB1E8:
			// stw r10,24(r11)
			PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
		}
	}
loc_823EB1EC:
	// lwz r8,28(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// cmplw cr6,r3,r29
	// clrlwi r9,r8,31
	ctx.r9.u64 = ctx.r8.u32 & 0x1;
	// beq cr6,0x823eb2b0
	if (ctx.r3.u32 != var_r29) {
		// lwz r7,0(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 0)/* atSingleton::vtable@+0x0 */;
		// cmplw cr6,r7,r29
		// bne cr6,0x823eb220
		if (ctx.r7.u32 == var_r29) {
			// lwz r6,20(r29)
			ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 20);
			// stw r6,20(r3)
			PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r6.u32);
			// lwz r5,24(r29)
			ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 24);
			// stw r5,24(r3)
			PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r5.u32);
			// stw r3,0(r30)
			PPC_STORE_U32(var_r30 + 0,/* atSingleton::vtable@+0x0 */ ctx.r3.u32);
			// b 0x823eb240
		} else {
		loc_823EB220:
			// lwz r4,28(r29)
			ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 28);
			// rlwinm r11,r4,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFFFE;
			// lwz r8,20(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// cmplw cr6,r8,r29
			// bne cr6,0x823eb23c
			if (ctx.r8.u32 == var_r29) {
				// stw r3,20(r11)
				PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r3.u32);
				// b 0x823eb240
			} else {
			loc_823EB23C:
				// stw r3,24(r11)
				PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r3.u32);
			}
		}
	loc_823EB240:
		// lwz r7,28(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		// lwz r6,28(r29)
		ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 28);
		// rlwimi r6,r7,0,31,31
		ctx.r6.u64 = (ctx.r7.u32 & 0x1) | (ctx.r6.u64 & 0xFFFFFFFFFFFFFFFE);
		// stw r6,28(r3)
		PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r6.u32);
		// lwz r5,20(r29)
		ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 20);
		// stw r5,20(r3)
		PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r5.u32);
		// lwz r4,24(r29)
		ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 24);
		// stw r4,24(r3)
		PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r4.u32);
		// lwz r11,28(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
		// lwz r8,28(r29)
		ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 28);
		// rlwimi r11,r8,0,31,31
		ctx.r11.u64 = (ctx.r8.u32 & 0x1) | (ctx.r11.u64 & 0xFFFFFFFFFFFFFFFE);
		// stw r11,28(r3)
		PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r11.u32);
		// lwz r7,20(r29)
		ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 20);
		// cmplwi cr6,r7,0
		// beq cr6,0x823eb290
		if (ctx.r7.u32 != 0) {
			// rotlwi r11,r7,0
			ctx.r11.u64 = ctx.r7.u32;
			// lwz r6,28(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
			// clrlwi r5,r6,31
			ctx.r5.u64 = ctx.r6.u32 & 0x1;
			// or r4,r5,r3
			ctx.r4.u64 = ctx.r5.u64 | ctx.r3.u64;
			// stw r4,28(r11)
			PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r4.u32);
		}
	loc_823EB290:
		// lwz r11,24(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 24);
		// cmplwi cr6,r11,0
		// beq cr6,0x823eb2b0
		if (ctx.r11.u32 == 0) goto loc_823EB2B0;
		// rotlwi r11,r11,0
		ctx.r11.u64 = ctx.r11.u32;
		// lwz r8,28(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
		// clrlwi r7,r8,31
		ctx.r7.u64 = ctx.r8.u32 & 0x1;
		// or r6,r7,r3
		ctx.r6.u64 = ctx.r7.u64 | ctx.r3.u64;
		// stw r6,28(r11)
		PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	}
loc_823EB2B0:
	// lbz r5,9(r30)
	ctx.r5.u64 = PPC_LOAD_U8(var_r30 + 9);
	// rlwinm r4,r5,0,0,24
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eb5e4
	if (ctx.r4.u32 != 0) {
		// cmplwi cr6,r10,0
		// beq cr6,0x823eb5e4
		if (ctx.r10.u32 == 0) goto loc_823EB5E4;
		// cmplwi cr6,r9,1
		// bne cr6,0x823eb5e4
		if (ctx.r9.u32 != 1) goto loc_823EB5E4;
		// addi r31,r10,28
		var_r31 = (uint32_t)(ctx.r10.s64 + 28);  // addr:0x8207001c
		// lwz r3,0(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
		// rlwinm r11,r3,0,0,30
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
		// cmplwi cr6,r11,0
		// beq cr6,0x823eb5e4
	while (ctx.r11.u32 != 0) {
		loc_823EB2E4:
			// lwz r11,0(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
			// clrlwi r9,r11,31
			ctx.r9.u64 = ctx.r11.u32 & 0x1;
			// cmplwi cr6,r9,1
			// bne cr6,0x823eb5e4
			if (ctx.r9.u32 != 1) goto loc_823EB5E4;
			// rlwinm r11,r11,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
			// lwz r8,20(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// cmplw cr6,r8,r10
			// bne cr6,0x823eb460
			if (ctx.r8.u32 == ctx.r10.u32) {
				// lwz r4,24(r11)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
				// cmplwi cr6,r4,0
				// beq cr6,0x823eb360
				if (ctx.r4.u32 != 0) {
					// lwz r10,28(r4)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
					// clrlwi r7,r10,31
					ctx.r7.u64 = ctx.r10.u32 & 0x1;
					// cmplwi cr6,r7,0
					// bne cr6,0x823eb358
					if (ctx.r7.u32 == 0) {
						// ori r6,r10,1
						ctx.r6.u64 = ctx.r10.u64 | 1;
						// mr r3,r30
						ctx.r3.u64 = var_r30;
						// stw r6,28(r4)
						PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r6.u32);
						// lwz r5,0(r31)
						ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
						// rlwinm r11,r5,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,28(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
						// rlwinm r10,r4,0,0,30
						ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFFFE;
						// stw r10,28(r11)
						PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
						// lwz r9,0(r31)
						ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
						// rlwinm r4,r9,0,0,30
						ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
						// bl 0x823eb700
						atSingleton_B700_2h(ctx, base);
						// lwz r8,0(r31)
						ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
						// rlwinm r11,r8,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,24(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
					}
				loc_823EB358:
					// cmplwi cr6,r4,0
					// bne cr6,0x823eb368
					if (ctx.r4.u32 != 0) goto loc_823EB368;
				}
			loc_823EB360:
				// mr r10,r11
				ctx.r10.u64 = ctx.r11.u64;
				// b 0x823eb5d0
				goto loc_823EB5D0;
			loc_823EB368:
				// lwz r11,20(r4)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
				// cmplwi cr6,r11,0
				// beq cr6,0x823eb388
				if (ctx.r11.u32 != 0) {
					// rotlwi r7,r11,0
					ctx.r7.u64 = ctx.r11.u32;
					// lwz r6,28(r7)
					ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 28);
					// clrlwi r5,r6,31
					ctx.r5.u64 = ctx.r6.u32 & 0x1;
					// cmplwi cr6,r5,1
					// bne cr6,0x823eb3a8
					if (ctx.r5.u32 != 1) goto loc_823EB3A8;
				}
			loc_823EB388:
				// lwz r3,24(r4)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
				// cmplwi cr6,r3,0
				// beq cr6,0x823eb5bc
				if (ctx.r3.u32 == 0) goto loc_823EB5BC;
				// rotlwi r10,r3,0
				ctx.r10.u64 = ctx.r3.u32;
				// lwz r9,28(r10)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
				// clrlwi r8,r9,31
				ctx.r8.u64 = ctx.r9.u32 & 0x1;
				// cmplwi cr6,r8,1
				// beq cr6,0x823eb5bc
				if (ctx.r8.u32 == 1) goto loc_823EB5BC;
			loc_823EB3A8:
				// lwz r7,24(r4)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
				// cmplwi cr6,r7,0
				// beq cr6,0x823eb3c8
				if (ctx.r7.u32 != 0) {
					// rotlwi r6,r7,0
					ctx.r6.u64 = ctx.r7.u32;
					// lwz r5,28(r6)
					ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 28);
					// clrlwi r3,r5,31
					ctx.r3.u64 = ctx.r5.u32 & 0x1;
					// cmplwi cr6,r3,1
					// bne cr6,0x823eb400
					if (ctx.r3.u32 != 1) goto loc_823EB400;
				}
			loc_823EB3C8:
				// cmplwi cr6,r11,0
				// beq cr6,0x823eb3e0
				if (ctx.r11.u32 != 0) {
					// lwz r11,20(r4)
					ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
					// lwz r10,28(r11)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
					// ori r9,r10,1
					ctx.r9.u64 = ctx.r10.u64 | 1;
					// stw r9,28(r11)
					PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r9.u32);
				}
			loc_823EB3E0:
				// lwz r8,28(r4)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// rlwinm r7,r8,0,0,30
				ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
				// stw r7,28(r4)
				PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r7.u32);
				// bl 0x823eb678
				atSingleton_B678_2hr(ctx, base);
				// lwz r6,0(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// rlwinm r5,r6,0,0,30
				ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFFE;
				// lwz r4,24(r5)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 24);
			loc_823EB400:
				// lwz r3,0(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// lwz r9,28(r4)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
				// rlwinm r11,r3,0,0,30
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
				// lwz r10,28(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
				// rlwimi r10,r9,0,0,30
				ctx.r10.u64 = (ctx.r9.u32 & 0xFFFFFFFE) | (ctx.r10.u64 & 0xFFFFFFFF00000001);
				// stw r10,28(r4)
				PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r10.u32);
				// lwz r8,0(r31)
				ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// rlwinm r11,r8,0,0,30
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
				// lwz r7,28(r11)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
				// ori r6,r7,1
				ctx.r6.u64 = ctx.r7.u64 | 1;
				// stw r6,28(r11)
				PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
				// lwz r5,24(r4)
				ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
				// cmplwi cr6,r5,0
				// beq cr6,0x823eb448
				if (ctx.r5.u32 != 0) {
					// rotlwi r11,r5,0
					ctx.r11.u64 = ctx.r5.u32;
					// lwz r4,28(r11)
					ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
					// ori r3,r4,1
					ctx.r3.u64 = ctx.r4.u64 | 1;
					// stw r3,28(r11)
					PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r3.u32);
				}
			loc_823EB448:
				// lwz r11,0(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// rlwinm r4,r11,0,0,30
				ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
				// bl 0x823eb700
				atSingleton_B700_2h(ctx, base);
				// lwz r10,0(r30)
				ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 0)/* atSingleton::vtable@+0x0 */;
				// b 0x823eb5d0
			} else {
			loc_823EB460:
				// lwz r4,20(r11)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
				// cmplwi cr6,r4,0
				// beq cr6,0x823eb4bc
				if (ctx.r4.u32 != 0) {
					// lwz r10,28(r4)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
					// clrlwi r7,r10,31
					ctx.r7.u64 = ctx.r10.u32 & 0x1;
					// cmplwi cr6,r7,0
					// bne cr6,0x823eb4b4
					if (ctx.r7.u32 == 0) {
						// ori r6,r10,1
						ctx.r6.u64 = ctx.r10.u64 | 1;
						// mr r3,r30
						ctx.r3.u64 = var_r30;
						// stw r6,28(r4)
						PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r6.u32);
						// lwz r5,0(r31)
						ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
						// rlwinm r11,r5,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,28(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
						// rlwinm r10,r4,0,0,30
						ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFFFE;
						// stw r10,28(r11)
						PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
						// lwz r9,0(r31)
						ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
						// rlwinm r4,r9,0,0,30
						ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
						// bl 0x823eb678
						atSingleton_B678_2hr(ctx, base);
						// lwz r8,0(r31)
						ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
						// rlwinm r11,r8,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,20(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
					}
				loc_823EB4B4:
					// cmplwi cr6,r4,0
					// bne cr6,0x823eb4c4
					if (ctx.r4.u32 != 0) goto loc_823EB4C4;
				}
			loc_823EB4BC:
				// mr r10,r11
				ctx.r10.u64 = ctx.r11.u64;
				// b 0x823eb5d0
				goto loc_823EB5D0;
			loc_823EB4C4:
				// lwz r11,24(r4)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
				// cmplwi cr6,r11,0
				// beq cr6,0x823eb4e4
				if (ctx.r11.u32 != 0) {
					// rotlwi r7,r11,0
					ctx.r7.u64 = ctx.r11.u32;
					// lwz r6,28(r7)
					ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 28);
					// clrlwi r5,r6,31
					ctx.r5.u64 = ctx.r6.u32 & 0x1;
					// cmplwi cr6,r5,1
					// bne cr6,0x823eb504
					if (ctx.r5.u32 != 1) goto loc_823EB504;
				}
			loc_823EB4E4:
				// lwz r3,20(r4)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
				// cmplwi cr6,r3,0
				// beq cr6,0x823eb5bc
				if (ctx.r3.u32 != 0) {
					// rotlwi r10,r3,0
					ctx.r10.u64 = ctx.r3.u32;
					// lwz r9,28(r10)
					ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
					// clrlwi r8,r9,31
					ctx.r8.u64 = ctx.r9.u32 & 0x1;
					// cmplwi cr6,r8,1
					// beq cr6,0x823eb5bc
					if (ctx.r8.u32 == 1) goto loc_823EB5BC;
				loc_823EB504:
					// lwz r7,20(r4)
					ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
					// cmplwi cr6,r7,0
					// beq cr6,0x823eb524
					if (ctx.r7.u32 != 0) {
						// rotlwi r6,r7,0
						ctx.r6.u64 = ctx.r7.u32;
						// lwz r5,28(r6)
						ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 28);
						// clrlwi r3,r5,31
						ctx.r3.u64 = ctx.r5.u32 & 0x1;
						// cmplwi cr6,r3,1
						// bne cr6,0x823eb55c
						if (ctx.r3.u32 != 1) goto loc_823EB55C;
					}
				loc_823EB524:
					// cmplwi cr6,r11,0
					// beq cr6,0x823eb53c
					if (ctx.r11.u32 != 0) {
						// lwz r11,24(r4)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
						// lwz r10,28(r11)
						ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
						// ori r9,r10,1
						ctx.r9.u64 = ctx.r10.u64 | 1;
						// stw r9,28(r11)
						PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r9.u32);
					}
				loc_823EB53C:
					// lwz r8,28(r4)
					ctx.r8.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
					// mr r3,r30
					ctx.r3.u64 = var_r30;
					// rlwinm r7,r8,0,0,30
					ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
					// stw r7,28(r4)
					PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r7.u32);
					// bl 0x823eb700
					atSingleton_B700_2h(ctx, base);
					// lwz r6,0(r31)
					ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
					// rlwinm r5,r6,0,0,30
					ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFFE;
					// lwz r4,20(r5)
					ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 20);
				loc_823EB55C:
					// lwz r3,0(r31)
					ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
					// lwz r9,28(r4)
					ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
					// rlwinm r11,r3,0,0,30
					ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
					// lwz r10,28(r11)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
					// rlwimi r10,r9,0,0,30
					ctx.r10.u64 = (ctx.r9.u32 & 0xFFFFFFFE) | (ctx.r10.u64 & 0xFFFFFFFF00000001);
					// stw r10,28(r4)
					PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r10.u32);
					// lwz r8,0(r31)
					ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
					// rlwinm r11,r8,0,0,30
					ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
					// lwz r7,28(r11)
					ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
					// ori r6,r7,1
					ctx.r6.u64 = ctx.r7.u64 | 1;
					// stw r6,28(r11)
					PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
					// lwz r5,20(r4)
					ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
					// cmplwi cr6,r5,0
					// beq cr6,0x823eb5a4
					if (ctx.r5.u32 != 0) {
						// rotlwi r11,r5,0
						ctx.r11.u64 = ctx.r5.u32;
						// lwz r4,28(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
						// ori r3,r4,1
						ctx.r3.u64 = ctx.r4.u64 | 1;
						// stw r3,28(r11)
						PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r3.u32);
					}
				loc_823EB5A4:
					// lwz r11,0(r31)
					ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
					// mr r3,r30
					ctx.r3.u64 = var_r30;
					// rlwinm r4,r11,0,0,30
					ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
					// bl 0x823eb678
					atSingleton_B678_2hr(ctx, base);
					// lwz r10,0(r30)
					ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 0)/* atSingleton::vtable@+0x0 */;
					// b 0x823eb5d0
				} else {
				loc_823EB5BC:
					// lwz r10,28(r4)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
					// rlwinm r9,r10,0,0,30
					ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFE;
					// stw r9,28(r4)
					PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r9.u32);
					// lwz r8,0(r31)
					ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
					// rlwinm r10,r8,0,0,30
					ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
				}
			}
		loc_823EB5D0:
			// addi r31,r10,28
			var_r31 = (uint32_t)(ctx.r10.s64 + 28);  // addr:0x8207001c
			// lwz r7,0(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* atSingleton::vtable@+0x0 */;
			// rlwinm r11,r7,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0xFFFFFFFE;
			// cmplwi cr6,r11,0
			// bne cr6,0x823eb2e4
	}
	}
loc_823EB5E4:
	// lwz r6,28(r29)
	ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 28);
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r5,r6,31
	ctx.r5.u64 = ctx.r6.u32 & 0x1;
	// stw r5,28(r29)
	PPC_STORE_U32(var_r29 + 28, ctx.r5.u32);
	// stw r11,24(r29)
	PPC_STORE_U32(var_r29 + 24, ctx.r11.u32);
	// stw r11,20(r29)
	PPC_STORE_U32(var_r29 + 20, ctx.r11.u32);
	// lwz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 4)/* atSingleton::flags@+0x4 */;
	// addi r4,r11,-1
	ctx.r4.s64 = ctx.r11.s64 + -1;
	// stw r4,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* atSingleton::flags@+0x4 */ ctx.r4.u32);
	return;
}

__attribute__((alias("__imp__snSession_B610_p44"))) PPC_WEAK_FUNC(snSession_B610_p44);
PPC_FUNC_IMPL(__imp__snSession_B610_p44) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplwi cr6,r10,0
	// beq cr6,0x823eb644
	if (ctx.r10.u32 != 0) {
		// rotlwi r3,r10,0
		ctx.r3.u64 = ctx.r10.u32;
		// lwz r9,24(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
		// cmplwi cr6,r9,0
		// beqlr cr6
		if (ctx.r9.u32 == 0) return;
	loc_823EB630:
		// lwz r3,24(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
		// lwz r8,24(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
		// cmplwi cr6,r8,0
		// bne cr6,0x823eb630
		if (ctx.r8.u32 != 0) goto loc_823EB630;
		// blr
		return;
	}
loc_823EB644:
	// lwz r7,28(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// rlwinm r3,r7,0,0,30
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
loc_823EB654:
	// lwz r6,20(r3)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// cmplw cr6,r6,r11
	// bnelr cr6
	if (ctx.r6.u32 != ctx.r11.u32) return;
	// lwz r5,28(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 28);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// rlwinm r3,r5,0,0,30
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r3,0
	// bne cr6,0x823eb654
	if (ctx.r3.u32 != 0) goto loc_823EB654;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_B678_2hr"))) PPC_WEAK_FUNC(atSingleton_B678_2hr);
PPC_FUNC_IMPL(__imp__atSingleton_B678_2hr) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,20(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 20);
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// stw r10,20(r4)
	PPC_STORE_U32(ctx.r4.u32 + 20, ctx.r10.u32);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// cmplwi cr6,r9,0
	// beq cr6,0x823eb6a4
	if (ctx.r9.u32 != 0) {
		// rotlwi r10,r9,0
		ctx.r10.u64 = ctx.r9.u32;
		// lwz r8,28(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
		// clrlwi r7,r8,31
		ctx.r7.u64 = ctx.r8.u32 & 0x1;
		// or r6,r7,r4
		ctx.r6.u64 = ctx.r7.u64 | ctx.r4.u64;
		// stw r6,28(r10)
		PPC_STORE_U32(ctx.r10.u32 + 28, ctx.r6.u32);
	}
loc_823EB6A4:
	// lwz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r10,28(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
	// rlwimi r10,r5,0,31,31
	ctx.r10.u64 = (ctx.r5.u32 & 0x1) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFFE);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// lwz r9,28(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
	// rlwinm r10,r9,0,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r10,0
	// bne cr6,0x823eb6cc
	if (ctx.r10.u32 == 0) {
		// stw r11,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
		// b 0x823eb6e4
	} else {
	loc_823EB6CC:
		// lwz r8,24(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 24);
		// cmplw cr6,r8,r4
		// bne cr6,0x823eb6e0
		if (ctx.r8.u32 == ctx.r4.u32) {
			// stw r11,24(r10)
			PPC_STORE_U32(ctx.r10.u32 + 24, ctx.r11.u32);
			// b 0x823eb6e4
		} else {
		loc_823EB6E0:
			// stw r11,20(r10)
			PPC_STORE_U32(ctx.r10.u32 + 20, ctx.r11.u32);
		}
	}
loc_823EB6E4:
	// stw r4,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r4.u32);
	// lwz r7,28(r4)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
	// clrlwi r6,r7,31
	ctx.r6.u64 = ctx.r7.u32 & 0x1;
	// or r5,r6,r11
	ctx.r5.u64 = ctx.r6.u64 | ctx.r11.u64;
	// stw r5,28(r4)
	PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r5.u32);
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_B700_2h"))) PPC_WEAK_FUNC(atSingleton_B700_2h);
PPC_FUNC_IMPL(__imp__atSingleton_B700_2h) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,24(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 24);
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// stw r10,24(r4)
	PPC_STORE_U32(ctx.r4.u32 + 24, ctx.r10.u32);
	// lwz r9,20(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplwi cr6,r9,0
	// beq cr6,0x823eb72c
	if (ctx.r9.u32 != 0) {
		// rotlwi r10,r9,0
		ctx.r10.u64 = ctx.r9.u32;
		// lwz r8,28(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
		// clrlwi r7,r8,31
		ctx.r7.u64 = ctx.r8.u32 & 0x1;
		// or r6,r7,r4
		ctx.r6.u64 = ctx.r7.u64 | ctx.r4.u64;
		// stw r6,28(r10)
		PPC_STORE_U32(ctx.r10.u32 + 28, ctx.r6.u32);
	}
loc_823EB72C:
	// lwz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r10,28(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
	// rlwimi r10,r5,0,31,31
	ctx.r10.u64 = (ctx.r5.u32 & 0x1) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFFE);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// lwz r9,28(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
	// rlwinm r10,r9,0,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r10,0
	// bne cr6,0x823eb754
	if (ctx.r10.u32 == 0) {
		// stw r11,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* atSingleton::vtable@+0x0 */ ctx.r11.u32);
		// b 0x823eb76c
	} else {
	loc_823EB754:
		// lwz r8,20(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
		// cmplw cr6,r8,r4
		// bne cr6,0x823eb768
		if (ctx.r8.u32 == ctx.r4.u32) {
			// stw r11,20(r10)
			PPC_STORE_U32(ctx.r10.u32 + 20, ctx.r11.u32);
			// b 0x823eb76c
		} else {
		loc_823EB768:
			// stw r11,24(r10)
			PPC_STORE_U32(ctx.r10.u32 + 24, ctx.r11.u32);
		}
	}
loc_823EB76C:
	// stw r4,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r4.u32);
	// lwz r7,28(r4)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 28);
	// clrlwi r6,r7,31
	ctx.r6.u64 = ctx.r7.u32 & 0x1;
	// or r5,r6,r11
	ctx.r5.u64 = ctx.r6.u64 | ctx.r11.u64;
	// stw r5,28(r4)
	PPC_STORE_U32(ctx.r4.u32 + 28, ctx.r5.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_B788_h"))) PPC_WEAK_FUNC(snSession_B788_h);
PPC_FUNC_IMPL(__imp__snSession_B788_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,288
	ctx.r4.s64 = 288;
	// lwz r3,4(r29)
	ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x823eb848
	if (var_r31 != 0) {
		// lis r11,-32249
		// addi r9,r30,4
		ctx.r9.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r8,r31,4
		ctx.r8.s64 = (int64_t)(int32_t)var_r31 + 4;
		// lis r10,-32249
		// addi r4,r30,28
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 28;
		// addi r10,r10,10660
		ctx.r10.s64 = ctx.r10.s64 + 10660;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
		// addi r3,r31,28
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 28;
		// lwz r7,0(r9)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
		// li r5,256
		ctx.r5.s64 = 256;
		// stw r7,0(r8)
		PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r7.u32);
		// lwz r6,4(r9)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
		// stw r10,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
		// stw r6,4(r8)
		PPC_STORE_U32(ctx.r8.u32 + 4, ctx.r6.u32);
		// lwz r11,12(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 12);
		// stw r11,12(r31)
		PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
		// lwz r10,16(r30)
		ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 16);
		// stw r10,16(r31)
		PPC_STORE_U32(var_r31 + 16, ctx.r10.u32);
		// lwz r9,20(r30)
		ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 20);
		// stw r9,20(r31)
		PPC_STORE_U32(var_r31 + 20, ctx.r9.u32);
		// lwz r8,24(r30)
		ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 24);
		// stw r8,24(r31)
		PPC_STORE_U32(var_r31 + 24, ctx.r8.u32);
		// bl 0x82434100
		memcpy(ctx, base);
		// lwz r7,284(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 284);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r29,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 8;
		// stw r7,284(r31)
		PPC_STORE_U32(var_r31 + 284, ctx.r7.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		return;
	}
loc_823EB848:
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	return;
}

__attribute__((alias("__imp__snSession_B858_fw"))) PPC_WEAK_FUNC(snSession_B858_fw);
PPC_FUNC_IMPL(__imp__snSession_B858_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,124
	ctx.r4.s64 = 124;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eb92c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,20
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 20;
		// addi r9,r10,10680
		ctx.r9.s64 = ctx.r10.s64 + 10680;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// addi r10,r4,20
		ctx.r10.s64 = ctx.r4.s64 + 20;
		// li r8,24
		ctx.r8.s64 = 24;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// lwz r6,12(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 12);
		// stw r6,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r6.u32);
		// lwz r5,16(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
		// stw r5,16(r4)
		PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r5.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EB8F0:
		// lwz r3,0(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r3,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r3.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823eb8f0
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EB8F0;
		// lwz r11,116(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 116);
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// lbz r9,120(r4)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r4.u32 + 120);
		// stw r11,116(r4)
		PPC_STORE_U32(ctx.r4.u32 + 116, ctx.r11.u32);
		// lbz r10,120(r31)
		ctx.r10.u64 = PPC_LOAD_U8(var_r31 + 120);
		// rlwimi r10,r9,0,25,31
		ctx.r10.u64 = (ctx.r9.u32 & 0x7F) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFF80);
		// stb r10,120(r4)
		PPC_STORE_U8(ctx.r4.u32 + 120, ctx.r10.u8);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823eb930
	} else {
	loc_823EB92C:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EB930:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__game_B950"))) PPC_WEAK_FUNC(game_B950);
PPC_FUNC_IMPL(__imp__game_B950) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,120
	ctx.r4.s64 = 120;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823eba0c
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,16
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r9,r10,10700
		ctx.r9.s64 = ctx.r10.s64 + 10700;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// addi r10,r4,16
		ctx.r10.s64 = ctx.r4.s64 + 16;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// li r8,24
		ctx.r8.s64 = 24;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// lwz r6,12(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 12);
		// stw r6,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r6.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EB9E0:
		// lwz r5,0(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r5,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r5.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823eb9e0
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EB9E0;
		// ld r11,112(r31)
		ctx.r11.u64 = PPC_LOAD_U64(var_r31 + 112);
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// std r11,112(r4)
		PPC_STORE_U64(ctx.r4.u32 + 112, ctx.r11.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823eba10
	} else {
	loc_823EBA0C:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EBA10:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_BA30"))) PPC_WEAK_FUNC(SinglesNetworkClient_BA30);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_BA30) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,48
	ctx.r4.s64 = 48;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ebadc
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,16
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r9,r10,12756
		ctx.r9.s64 = ctx.r10.s64 + 12756;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// addi r10,r4,16
		ctx.r10.s64 = ctx.r4.s64 + 16;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// li r8,8
		ctx.r8.s64 = 8;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EBAB8:
		// lwz r6,0(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r6,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r6.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebab8
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBAB8;
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823ebae0
	} else {
	loc_823EBADC:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EBAE0:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_BB00_fw"))) PPC_WEAK_FUNC(snSession_BB00_fw);
PPC_FUNC_IMPL(__imp__snSession_BB00_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,48
	ctx.r4.s64 = 48;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ebbac
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,12
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 12;
		// addi r9,r10,13380
		ctx.r9.s64 = ctx.r10.s64 + 13380;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// addi r10,r4,12
		ctx.r10.s64 = ctx.r4.s64 + 12;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// li r8,9
		ctx.r8.s64 = 9;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EBB88:
		// lwz r6,0(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r6,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r6.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebb88
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBB88;
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823ebbb0
	} else {
	loc_823EBBAC:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EBBB0:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_BBD0_fw"))) PPC_WEAK_FUNC(snSession_BBD0_fw);
PPC_FUNC_IMPL(__imp__snSession_BBD0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,52
	ctx.r4.s64 = 52;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ebc90
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,16
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r9,r10,10720
		ctx.r9.s64 = ctx.r10.s64 + 10720;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// addi r10,r4,16
		ctx.r10.s64 = ctx.r4.s64 + 16;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// li r8,9
		ctx.r8.s64 = 9;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// lwz r6,12(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 12);
		// stw r6,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r6.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EBC60:
		// lwz r5,0(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r5,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r5.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebc60
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBC60;
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10740
		ctx.r11.s64 = ctx.r11.s64 + 10740;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823ebc94
	} else {
	loc_823EBC90:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EBC94:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_BCB0_fw"))) PPC_WEAK_FUNC(snSession_BCB0_fw);
PPC_FUNC_IMPL(__imp__snSession_BCB0_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,52
	ctx.r4.s64 = 52;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ebd70
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,16
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r9,r10,10720
		ctx.r9.s64 = ctx.r10.s64 + 10720;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// addi r10,r4,16
		ctx.r10.s64 = ctx.r4.s64 + 16;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// li r8,9
		ctx.r8.s64 = 9;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// lwz r6,12(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 12);
		// stw r6,12(r4)
		PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r6.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EBD40:
		// lwz r5,0(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r5,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r5.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebd40
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBD40;
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10760
		ctx.r11.s64 = ctx.r11.s64 + 10760;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823ebd74
	} else {
	loc_823EBD70:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EBD74:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_BD90_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_BD90_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_BD90_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r27,r3
	var_r27 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,200
	ctx.r4.s64 = 200;
	// lwz r3,4(r27)
	ctx.r3.u64 = PPC_LOAD_U32(var_r27 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x823ebe58
	if (var_r31 != 0) {
		// lis r11,-32249
		// addi r8,r28,4
		ctx.r8.s64 = (int64_t)(int32_t)var_r28 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// lis r10,-32249
		// addi r30,r28,16
		var_r30 = (uint32_t)(var_r28 + 16);
		// addi r10,r10,11788
		ctx.r10.s64 = ctx.r10.s64 + 11788;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
		// addi r29,r31,16
		var_r29 = (uint32_t)(var_r31 + 16);
		// lwz r6,0(r8)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r8.u32 + 0);
		// li r9,18
		ctx.r9.s64 = 18;
		// mr r11,r30
		ctx.r11.u64 = var_r30;
		// stw r6,0(r7)
		PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r6.u32);
		// lwz r5,4(r8)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r8.u32 + 4);
		// stw r10,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
		// mr r10,r29
		ctx.r10.u64 = var_r29;
		// stw r5,4(r7)
		PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r5.u32);
		// mtctr r9
		ctx.ctr.u64 = ctx.r9.u64;
	loc_823EBE14:
		// lwz r4,0(r11)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r4,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r4.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebe14
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBE14;
		// addi r4,r30,72
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 72;
		// addi r3,r29,72
		ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 72;
		// bl 0x823ec008
		SinglesNetworkClient_C008_fw(ctx, base);
		// lbz r11,176(r30)
		ctx.r11.u64 = PPC_LOAD_U8(var_r30 + 176);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r27,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r27 + 8;
		// stb r11,176(r29)
		PPC_STORE_U8(var_r29 + 176, ctx.r11.u8);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		return;
	}
loc_823EBE58:
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_BE68_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_BE68_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_BE68_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,192
	ctx.r4.s64 = 192;
	// lwz r3,4(r29)
	ctx.r3.u64 = PPC_LOAD_U32(var_r29 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// cmplwi cr6,r31,0
	// beq cr6,0x823ebf28
	if (var_r31 != 0) {
		// lis r11,-32249
		// addi r6,r30,4
		ctx.r6.s64 = (int64_t)(int32_t)var_r30 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r5,r31,4
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 4;
		// lis r10,-32249
		// addi r9,r30,16
		ctx.r9.s64 = (int64_t)(int32_t)var_r30 + 16;
		// addi r10,r10,11768
		ctx.r10.s64 = ctx.r10.s64 + 11768;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
		// addi r8,r31,16
		ctx.r8.s64 = (int64_t)(int32_t)var_r31 + 16;
		// lwz r4,0(r6)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r6.u32 + 0);
		// li r7,18
		ctx.r7.s64 = 18;
		// mr r11,r9
		ctx.r11.u64 = ctx.r9.u64;
		// stw r4,0(r5)
		PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r4.u32);
		// lwz r3,4(r6)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r6.u32 + 4);
		// stw r10,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
		// mr r10,r8
		ctx.r10.u64 = ctx.r8.u64;
		// stw r3,4(r5)
		PPC_STORE_U32(ctx.r5.u32 + 4, ctx.r3.u32);
		// mtctr r7
		ctx.ctr.u64 = ctx.r7.u64;
	loc_823EBEEC:
		// lwz r7,0(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r7,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r7.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebeec
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBEEC;
		// addi r4,r9,72
		ctx.r4.s64 = ctx.r9.s64 + 72;
		// addi r3,r8,72
		ctx.r3.s64 = ctx.r8.s64 + 72;
		// bl 0x823ec008
		SinglesNetworkClient_C008_fw(ctx, base);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r3,r29,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 8;
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		return;
	}
loc_823EBF28:
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	return;
}

__attribute__((alias("__imp__snSession_BF38_fw"))) PPC_WEAK_FUNC(snSession_BF38_fw);
PPC_FUNC_IMPL(__imp__snSession_BF38_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,48
	ctx.r4.s64 = 48;
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
	// lwz r10,4(r11)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ebfe4
	if (ctx.r4.u32 != 0) {
		// lis r11,-32249
		// addi r7,r31,4
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 4;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// addi r6,r4,4
		ctx.r6.s64 = ctx.r4.s64 + 4;
		// lis r10,-32249
		// addi r5,r31,16
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r9,r10,12736
		ctx.r9.s64 = ctx.r10.s64 + 12736;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// addi r10,r4,16
		ctx.r10.s64 = ctx.r4.s64 + 16;
		// lwz r3,0(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// li r8,8
		ctx.r8.s64 = 8;
		// stw r3,0(r6)
		PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r3.u32);
		// lwz r7,4(r7)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 4);
		// stw r9,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r9.u32);
		// stw r7,4(r6)
		PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r7.u32);
		// mtctr r8
		ctx.ctr.u64 = ctx.r8.u64;
	loc_823EBFC0:
		// lwz r6,0(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// stw r6,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r6.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// bdnz 0x823ebfc0
		--ctx.ctr.u64;
		if (ctx.ctr.u32 != 0) goto loc_823EBFC0;
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
		// li r11,1
		ctx.r11.s64 = 1;
		// b 0x823ebfe8
	} else {
	loc_823EBFE4:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EBFE8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_C008_fw"))) PPC_WEAK_FUNC(SinglesNetworkClient_C008_fw);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_C008_fw) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// bl 0x82260128
	SinglesNetworkClient_0128_g(ctx, base);
	// addi r10,r30,40
	ctx.r10.s64 = (int64_t)(int32_t)var_r30 + 40;
	// addi r11,r31,40
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 40;
	// li r9,16
	ctx.r9.s64 = 16;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823EC038:
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x823ec038
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823EC038;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// blr
	return;
}

__attribute__((alias("__imp__snSession_AddNode_C068"))) PPC_WEAK_FUNC(snSession_AddNode_C068);
PPC_FUNC_IMPL(__imp__snSession_AddNode_C068) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	// beq cr6,0x823ec0b0
	if (ctx.r11.u32 != 0) {
		// lwz r11,4(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
		// lwz r10,8(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// cmplwi cr6,r10,0
		// stw r10,8(r4)
		PPC_STORE_U32(ctx.r4.u32 + 8, ctx.r10.u32);
		// beq cr6,0x823ec094
		if (ctx.r10.u32 != 0) {
			// rotlwi r10,r10,0
			ctx.r10.u64 = ctx.r10.u32;
			// stw r4,4(r10)
			PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r4.u32);
			// b 0x823ec098
		} else {
		loc_823EC094:
			// stw r4,4(r3)
			PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r4.u32);
		}
	loc_823EC098:
		// stw r4,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r4.u32);
		// stw r11,4(r4)
		PPC_STORE_U32(ctx.r4.u32 + 4, ctx.r11.u32);
		// lwz r11,8(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// addi r9,r11,1
		ctx.r9.s64 = ctx.r11.s64 + 1;
		// stw r9,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r9.u32);
		// blr
		return;
	}
loc_823EC0B0:
	// addi r11,r4,4
	ctx.r11.s64 = ctx.r4.s64 + 4;
	// stw r4,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r4.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r4,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r4.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r8,r11,1
	ctx.r8.s64 = ctx.r11.s64 + 1;
	// stw r8,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r8.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionManager_vfn_0"))) PPC_WEAK_FUNC(snSessionManager_vfn_0);
PPC_FUNC_IMPL(__imp__snSessionManager_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15952
	ctx.r11.s64 = ctx.r11.s64 + 15952;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x823ec378
	rage_C378(ctx, base);
	// addi r3,r31,720
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 720;
	// bl 0x823ece28
	game_CE28(ctx, base);
	// addi r3,r31,12
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
	// bl 0x823da798
	atSafeDLListSimple_ctor_A798(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ec12c
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EC12C:
	// blr
	return;
}

__attribute__((alias("__imp__xe_C148"))) PPC_WEAK_FUNC(xe_C148);
PPC_FUNC_IMPL(__imp__xe_C148) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=160, savegprlr_27
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r4,r11,15768
	ctx.r4.s64 = ctx.r11.s64 + 15768;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r28,r5
	var_r28 = ctx.r5.u32;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// lbz r11,736(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 736);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// bne cr6,0x823ec2d0
	if (ctx.r10.u32 == 0) {
		// addi r30,r31,12
		var_r30 = (uint32_t)(var_r31 + 12);
		// stw r29,732(r31)
		PPC_STORE_U32(var_r31 + 732, var_r29);
		// lbz r9,700(r30)
		ctx.r9.u64 = PPC_LOAD_U8(var_r30 + 700);
		// rlwinm r8,r9,0,0,24
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r8,0
		// bne cr6,0x823ec1f8
		if (ctx.r8.u32 == 0) {
			// li r7,19153
			ctx.r7.s64 = 19153;
			// li r6,256
			ctx.r6.s64 = 256;
			// li r5,32
			ctx.r5.s64 = 32;
			// mr r4,r28
			ctx.r4.u64 = var_r28;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x82413538
			netHardware_StartNetwork(ctx, base);
			// addi r4,r30,612
			ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 612;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x82413840
			util_3840(ctx, base);
			// li r3,20
			ctx.r3.s64 = 20;
			// stw r31,668(r30)
			PPC_STORE_U32(var_r30 + 668, var_r31);
			// bl 0x820dec88
			xe_EC88(ctx, base);
			// mr r29,r3
			var_r29 = ctx.r3.u32;
			// cmplwi cr6,r29,0
			// beq cr6,0x823ec1e4
			if (var_r29 != 0) {
				// li r5,32
				ctx.r5.s64 = 32;
				// li r4,88
				ctx.r4.s64 = 88;
				// bl 0x82122c60
				game_2C60(ctx, base);
				// b 0x823ec1e8
			} else {
			loc_823EC1E4:
				// li r29,0
				var_r29 = 0;
			}
		loc_823EC1E8:
			// lbz r7,700(r30)
			ctx.r7.u64 = PPC_LOAD_U8(var_r30 + 700);
			// stw r29,660(r30)
			PPC_STORE_U32(var_r30 + 660, var_r29);
			// ori r6,r7,128
			ctx.r6.u64 = ctx.r7.u64 | 128;
			// stb r6,700(r30)
			PPC_STORE_U8(var_r30 + 700, ctx.r6.u8);
		}
	loc_823EC1F8:
		// li r3,19368
		ctx.r3.s64 = 19368;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// li r27,2
		var_r27 = 2;
		// cmplwi cr6,r3,0
		// beq cr6,0x823ec23c
		if (ctx.r3.u32 != 0) {
			// addi r28,r3,8
			var_r28 = (uint32_t)(ctx.r3.s64 + 8);
			// stw r27,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0, var_r27);
			// li r30,1
			var_r30 = 1;
			// mr r29,r28
			var_r29 = (uint32_t)(var_r28);
		loc_823EC21C:
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x823e4bc0
			SinglesNetworkClient_4BC0_g(ctx, base);
			// addi r30,r30,-1
			var_r30 = (uint32_t)(var_r30 + -1);
			// addi r29,r29,9680
			var_r29 = (uint32_t)(var_r29 + 9680);
			// cmpwi cr6,r30,0
			// bge cr6,0x823ec21c
			if ((int32_t)var_r30 >= 0) goto loc_823EC21C;
			// mr r11,r28
			ctx.r11.u64 = var_r28;
			// b 0x823ec240
		} else {
		loc_823EC23C:
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EC240:
		// li r30,0
		var_r30 = 0;
		// stw r11,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
		// li r29,0
		var_r29 = 0;
		// stw r27,8(r31)
		PPC_STORE_U32(var_r31 + 8, var_r27);
	loc_823EC250:
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4);
		// mr r5,r31
		ctx.r5.u64 = var_r31;
		// lwz r4,732(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 732);
		// add r3,r29,r11
		ctx.r3.u64 = var_r29 + ctx.r11.u64;
		// bl 0x823e6800
		game_6800(ctx, base);
		// lwz r5,8(r31)
		ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 8);
		// addi r30,r30,1
		var_r30 = (uint32_t)(var_r30 + 1);
		// addi r29,r29,9680
		var_r29 = (uint32_t)(var_r29 + 9680);
		// cmpw cr6,r30,r5
		// blt cr6,0x823ec250
		if ((int32_t)var_r30 < ctx.r5.s32) goto loc_823EC250;
		// bl 0x820c0038
		xe_main_thread_init_0038(ctx, base);
		// lwz r3,0(r13)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0)  /* SDA:0x82600000 */;
		// li r11,4
		ctx.r11.s64 = 4;
		// li r5,16
		ctx.r5.s64 = 16;
		// li r4,20
		ctx.r4.s64 = 20;
		// lwzx r3,r11,r3
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r3.u32);
		// lwz r9,4(r10)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmplwi cr6,r30,0
		// beq cr6,0x823ec2bc
		if (var_r30 != 0) {
			// li r5,32
			ctx.r5.s64 = 32;
			// li r4,48
			ctx.r4.s64 = 48;
			// bl 0x82122c60
			game_2C60(ctx, base);
			// b 0x823ec2c0
		} else {
		loc_823EC2BC:
			// li r30,0
			var_r30 = 0;
		}
	loc_823EC2C0:
		// lbz r8,736(r31)
		ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 736);
		// stw r30,716(r31)
		PPC_STORE_U32(var_r31 + 716, var_r30);
		// ori r7,r8,128
		ctx.r7.u64 = ctx.r8.u64 | 128;
		// stb r7,736(r31)
		PPC_STORE_U8(var_r31 + 736, ctx.r7.u8);
	}
loc_823EC2D0:
	return;
}

__attribute__((alias("__imp__snSession_rtti_2854_0"))) PPC_WEAK_FUNC(snSession_rtti_2854_0);
PPC_FUNC_IMPL(__imp__snSession_rtti_2854_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// rlwinm r11,r28,0,30,30
	ctx.r11.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 0) & 0x2;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ec34c
	if (ctx.r11.u32 != 0) {
		// addi r29,r30,-8
		var_r29 = (uint32_t)(var_r30 + -8);
		// lwz r11,0(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 0);
		// mulli r10,r11,9680
		ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(9680));
		// addi r31,r11,-1
		var_r31 = (uint32_t)(ctx.r11.s64 + -1);  // addr:0x8206ffff
		// add r30,r10,r30
		var_r30 = (uint32_t)(ctx.r10.u64 + var_r30);
		// cmpwi cr6,r31,0
		// blt cr6,0x823ec32c
	while ((int32_t)var_r31 >= 0) {
		loc_823EC314:
			// addi r30,r30,-9680
			var_r30 = (uint32_t)(var_r30 + -9680);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x823e6750
			atSafeDLListSimple_ctor_6750(ctx, base);
			// addi r31,r31,-1
			var_r31 = (uint32_t)(var_r31 + -1);
			// cmpwi cr6,r31,0
			// bge cr6,0x823ec314
	}
	loc_823EC32C:
		// clrlwi r10,r28,31
		ctx.r10.u64 = var_r28 & 0x1;
		// cmplwi cr6,r10,0
		// beq cr6,0x823ec340
		if (ctx.r10.u32 != 0) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x820c00c0
			rage_free_00C0(ctx, base);
		}
	loc_823EC340:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		return;
	}
loc_823EC34C:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823e6750
	atSafeDLListSimple_ctor_6750(ctx, base);
	// clrlwi r9,r28,31
	ctx.r9.u64 = var_r28 & 0x1;
	// cmplwi cr6,r9,0
	// beq cr6,0x823ec368
	if (ctx.r9.u32 != 0) {
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
	}
loc_823EC368:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	return;
}

__attribute__((alias("__imp__rage_C378"))) PPC_WEAK_FUNC(rage_C378);
PPC_FUNC_IMPL(__imp__rage_C378) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=144, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r11,736(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 736);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// beq cr6,0x823ec42c
	if (ctx.r10.u32 != 0) {
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,15800
		ctx.r4.s64 = ctx.r11.s64 + 15800;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// lwz r11,4(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 4)/* rage_GameObject::flags@+0x4 */;
		// cmplwi cr6,r11,0
		// beq cr6,0x823ec3e4
		if (ctx.r11.u32 != 0) {
			// addi r3,r11,-8
			ctx.r3.s64 = ctx.r11.s64 + -8;
			// lwz r9,0(r3)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* rage_GameObject::vtable@+0x0 */;
			// cmpwi cr6,r9,0
			// beq cr6,0x823ec3e0
			if (ctx.r9.s32 != 0) {
				// lwz r8,0(r11)
  // [ph4a] vtable load collapsed
				// li r4,3
				ctx.r4.s64 = 3;
				// mr r3,r11
				ctx.r3.u64 = ctx.r11.u64;
				// lwz r7,0(r8)
  // [ph4a] slot load collapsed
				// bctrl
				VCALL(ctx.r11.u32, 0, ctx, base);  // pattern-B slot 0 (byte +0)
				// b 0x823ec3e4
			} else {
			loc_823EC3E0:
				// bl 0x820c00c0
				rage_free_00C0(ctx, base);
			}
		}
	loc_823EC3E4:
		// li r29,0
		var_r29 = 0;
		// addi r3,r31,12
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
		// stw r29,8(r31)
		PPC_STORE_U32(var_r31 + 8, var_r29);
		// bl 0x823da828
		rage_A828(ctx, base);
		// addi r3,r31,720
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 720;
		// bl 0x823ece28
		game_CE28(ctx, base);
		// lwz r30,716(r31)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 716));
		// cmplwi cr6,r30,0
		// beq cr6,0x823ec418
		if (var_r30 != 0) {
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x82122c00
			rage_2C00(ctx, base);
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x820c00c0
			rage_free_00C0(ctx, base);
		}
	loc_823EC418:
		// lbz r6,736(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 736);
		// stw r29,716(r31)
		PPC_STORE_U32(var_r31 + 716, var_r29);
		// clrlwi r5,r6,25
		ctx.r5.u64 = ctx.r6.u32 & 0x7F;
		// stw r29,732(r31)
		PPC_STORE_U32(var_r31 + 732, var_r29);
		// stb r5,736(r31)
		PPC_STORE_U8(var_r31 + 736, ctx.r5.u8);
	}
loc_823EC42C:
	return;
}

__attribute__((alias("__imp__game_C438_w"))) PPC_WEAK_FUNC(game_C438_w);
PPC_FUNC_IMPL(__imp__game_C438_w) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// addi r3,r3,720
	ctx.r3.s64 = ctx.r3.s64 + 720;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823ec468
	if (ctx.r3.u32 != 0) {
		// lwz r3,0(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// blr
		return;
	}
loc_823EC468:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__game_C480"))) PPC_WEAK_FUNC(game_C480);
PPC_FUNC_IMPL(__imp__game_C480) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	// FRAME: size=160, savegprlr_26
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// addi r4,r11,15832
	ctx.r4.s64 = ctx.r11.s64 + 15832;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// mr r29,r6
	var_r29 = ctx.r6.u32;
	// mr r27,r7
	var_r27 = ctx.r7.u32;
	// li r26,0
	var_r26 = 0;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ec438
	game_C438_w(ctx, base);
	// cmplwi cr6,r3,0
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// beq cr6,0x823ec4e8
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,15860
		ctx.r4.s64 = ctx.r11.s64 + 15860;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		return;
	}
loc_823EC4E8:
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r7,r27
	ctx.r7.u64 = var_r27;
	// mr r6,r29
	ctx.r6.u64 = var_r29;
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,4(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 1, ctx, base);  // pattern-B slot 1 (byte +4)
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// cmplwi cr6,r29,0
	// bne cr6,0x823ec52c
	if (var_r29 == 0) {
		// lis r11,-32249
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// addi r4,r11,15884
		ctx.r4.s64 = ctx.r11.s64 + 15884;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// b 0x823ec550
	} else {
	loc_823EC52C:
		// stw r31,440(r29)
		PPC_STORE_U32(var_r29 + 440, var_r31);
		// lwz r11,716(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 716);
		// lhz r9,6(r11)
		ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
		// cmplwi cr6,r9,0
		// bne cr6,0x823ec57c
		if (ctx.r9.u32 != 0) goto loc_823EC57C;
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,15916
		ctx.r4.s64 = ctx.r11.s64 + 15916;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
	}
loc_823EC550:
	// cmplwi cr6,r29,0
	// beq cr6,0x823ec5c8
	if (var_r29 != 0) {
		// lwz r8,0(r31)
  // [ph4a] vtable load collapsed
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r7,8(r8)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 2, ctx, base);  // pattern-B slot 2 (byte +8)
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		return;
	loc_823EC57C:
		// lwz r5,12(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// lhz r6,6(r11)
		ctx.r6.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
		// cmplwi cr6,r5,0
		// addis r4,r6,1
		ctx.r4.s64 = ctx.r6.s64 + 65536;
		// lwz r10,0(r5)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r5.u32 + 0);
		// addi r4,r4,-1
		ctx.r4.s64 = ctx.r4.s64 + -1;
		// sth r4,6(r11)
		PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r4.u16);
		// stw r10,12(r11)
		PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
		// beq cr6,0x823ec5bc
		if (ctx.r5.u32 != 0) {
			// addi r11,r5,8
			ctx.r11.s64 = ctx.r5.s64 + 8;
			// stw r29,0(r5)
			PPC_STORE_U32(ctx.r5.u32 + 0, var_r29);
			// stw r26,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, var_r26);
			// stw r26,4(r11)
			PPC_STORE_U32(ctx.r11.u32 + 4, var_r26);
			// stw r26,8(r11)
			PPC_STORE_U32(ctx.r11.u32 + 8, var_r26);
			// stb r26,16(r11)
			PPC_STORE_U8(ctx.r11.u32 + 16, (uint8_t)var_r26);
			// std r26,32(r11)
			PPC_STORE_U64(ctx.r11.u32 + 32, var_r26);
		}
	loc_823EC5BC:
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// addi r3,r31,720
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 720;
		// bl 0x823ec5d8
		SinglesNetworkClient_C5D8_g(ctx, base);
	}
loc_823EC5C8:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_C5D8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_C5D8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_C5D8_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r29,r5
	var_r29 = ctx.r5.u32;
	// li r27,1
	var_r27 = 1;
	// lwz r11,0(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 0);
	// cmplwi cr6,r11,0
	// bne cr6,0x823ec608
	if (ctx.r11.u32 == 0) {
		// stw r29,0(r28)
		PPC_STORE_U32(var_r28 + 0, var_r29);
		// b 0x823ec714
	} else {
	loc_823EC608:
		// mr r31,r11
		var_r31 = ctx.r11.u32;
	loc_823EC60C:
		// addi r7,r31,24
		ctx.r7.s64 = (int64_t)(int32_t)var_r31 + 24;
		// mr r11,r30
		ctx.r11.u64 = var_r30;
		// mr r10,r7
		ctx.r10.u64 = ctx.r7.u64;
	loc_823EC618:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r8,0(r10)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// cmpwi cr6,r9,0
		// subf r8,r8,r9
		ctx.r8.s64 = ctx.r9.s64 - ctx.r8.s64;
		// beq cr6,0x823ec63c
		if (ctx.r9.s32 == 0) goto loc_823EC63C;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpwi cr6,r8,0
		// beq cr6,0x823ec618
		if (ctx.r8.s32 == 0) goto loc_823EC618;
	loc_823EC63C:
		// cmpwi cr6,r8,0
		// li r11,1
		ctx.r11.s64 = 1;
		// blt cr6,0x823ec64c
		if (ctx.r8.s32 >= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EC64C:
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r5,0
		// beq cr6,0x823ec66c
		if (ctx.r5.u32 != 0) {
			// lwz r11,8(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
			// cmplwi cr6,r11,0
			// bne cr6,0x823ec6d0
			if (ctx.r11.u32 != 0) goto loc_823EC6D0;
			// stw r29,8(r31)
			PPC_STORE_U32(var_r31 + 8, var_r29);
			// b 0x823ec6f8
			goto loc_823EC6F8;
		}
	loc_823EC66C:
		// mr r10,r30
		ctx.r10.u64 = var_r30;
		// mr r11,r7
		ctx.r11.u64 = ctx.r7.u64;
	loc_823EC674:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r4,0(r10)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
		// cmpwi cr6,r9,0
		// subf r8,r4,r9
		ctx.r8.s64 = ctx.r9.s64 - ctx.r4.s64;
		// beq cr6,0x823ec698
		if (ctx.r9.s32 == 0) goto loc_823EC698;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpwi cr6,r8,0
		// beq cr6,0x823ec674
		if (ctx.r8.s32 == 0) goto loc_823EC674;
	loc_823EC698:
		// cmpwi cr6,r8,0
		// li r11,1
		ctx.r11.s64 = 1;
		// blt cr6,0x823ec6a8
		if (ctx.r8.s32 >= 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823EC6A8:
		// clrlwi r11,r11,24
		ctx.r11.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r11,0
		// bne cr6,0x823ec6c4
		if (ctx.r11.u32 == 0) {
			// lbz r10,9(r28)
			ctx.r10.u64 = PPC_LOAD_U8(var_r28 + 9);
			// rlwinm r9,r10,0,25,25
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x40;
			// cmplwi cr6,r9,0
			// beq cr6,0x823ec6e0
			if (ctx.r9.u32 == 0) goto loc_823EC6E0;
		}
	loc_823EC6C4:
		// lwz r11,12(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 12);
		// cmplwi cr6,r11,0
		// beq cr6,0x823ec6f4
		if (ctx.r11.u32 != 0) {
		loc_823EC6D0:
			// mr r31,r11
			var_r31 = ctx.r11.u32;
			// cmplwi cr6,r11,0
			// bne cr6,0x823ec60c
			if (ctx.r11.u32 != 0) goto loc_823EC60C;
			// b 0x823ec6f8
			goto loc_823EC6F8;
		loc_823EC6E0:
			// lis r11,-32250
			ctx.r11.s64 = -2113536000;
			// addi r3,r11,28520
			ctx.r3.s64 = ctx.r11.s64 + 28520;
			// bl 0x8240e6d0
			nop_8240E6D0(ctx, base);
			// li r27,0
			var_r27 = 0;
			// b 0x823ec6f8
		} else {
		loc_823EC6F4:
			// stw r29,12(r31)
			PPC_STORE_U32(var_r31 + 12, var_r29);
		}
	loc_823EC6F8:
		// clrlwi r8,r27,24
		ctx.r8.u64 = var_r27 & 0xFF;
		// cmplwi cr6,r8,0
		// beq cr6,0x823ec714
		if (ctx.r8.u32 == 0) goto loc_823EC714;
		// lwz r7,16(r29)
		ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 16);
		// clrlwi r6,r7,31
		ctx.r6.u64 = ctx.r7.u32 & 0x1;
		// or r5,r6,r31
		ctx.r5.u64 = ctx.r6.u64 | var_r31;
		// stw r5,16(r29)
		PPC_STORE_U32(var_r29 + 16, ctx.r5.u32);
	}
loc_823EC714:
	// clrlwi r4,r27,24
	ctx.r4.u64 = var_r27 & 0xFF;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ec920
	if (ctx.r4.u32 != 0) {
		// ld r3,0(r30)
		ctx.r3.u64 = PPC_LOAD_U64(var_r30 + 0);
		// addi r11,r29,24
		ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 24;
		// std r3,0(r11)
		PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r3.u64);
		// ld r10,8(r30)
		ctx.r10.u64 = PPC_LOAD_U64(var_r30 + 8);
		// std r10,8(r11)
		PPC_STORE_U64(ctx.r11.u32 + 8, ctx.r10.u64);
		// ld r9,16(r30)
		ctx.r9.u64 = PPC_LOAD_U64(var_r30 + 16);
		// std r9,16(r11)
		PPC_STORE_U64(ctx.r11.u32 + 16, ctx.r9.u64);
		// lbz r8,9(r28)
		ctx.r8.u64 = PPC_LOAD_U8(var_r28 + 9);
		// rlwinm r7,r8,0,0,24
		ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFF80;
		// cmplwi cr6,r7,0
		// beq cr6,0x823ec914
		if (ctx.r7.u32 != 0) {
			// lwz r6,16(r29)
			ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 16);
			// rlwinm r11,r6,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFFE;
			// mr r31,r11
			var_r31 = ctx.r11.u32;
			// cmplwi cr6,r31,0
			// stw r11,16(r29)
			PPC_STORE_U32(var_r29 + 16, ctx.r11.u32);
			// beq cr6,0x823ec904
			if (var_r31 != 0) {
				// lwz r5,16(r31)
				ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 16);
				// rlwinm r4,r5,0,0,30
				ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
				// cmplwi cr6,r4,0
				// beq cr6,0x823ec904
			while (ctx.r4.u32 != 0) {
				loc_823EC774:
					// lwz r10,16(r31)
					ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 16);
					// clrlwi r3,r10,31
					ctx.r3.u64 = ctx.r10.u32 & 0x1;
					// cmplwi cr6,r3,0
					// bne cr6,0x823ec904
					if (ctx.r3.u32 != 0) goto loc_823EC904;
					// lwz r11,8(r4)
					ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
					// cmplw cr6,r11,r31
					// bne cr6,0x823ec83c
					if (ctx.r11.u32 == var_r31) {
						// lwz r11,12(r4)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
						// cmplwi cr6,r11,0
						// beq cr6,0x823ec7d4
						if (ctx.r11.u32 != 0) {
							// lwz r9,16(r11)
							ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
							// clrlwi r8,r9,31
							ctx.r8.u64 = ctx.r9.u32 & 0x1;
							// cmplwi cr6,r8,0
							// bne cr6,0x823ec7d4
							if (ctx.r8.u32 != 0) goto loc_823EC7D4;
							// ori r7,r10,1
							ctx.r7.u64 = ctx.r10.u64 | 1;
							// mr r29,r4
							var_r29 = ctx.r4.u32;
							// stw r7,16(r31)
							PPC_STORE_U32(var_r31 + 16, ctx.r7.u32);
							// lwz r6,16(r11)
							ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
							// ori r5,r6,1
							ctx.r5.u64 = ctx.r6.u64 | 1;
							// stw r5,16(r11)
							PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r5.u32);
							// lwz r3,16(r4)
							ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
							// rlwinm r11,r3,0,0,30
							ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
							// stw r11,16(r4)
							PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r11.u32);
							// b 0x823ec8e4
							goto loc_823EC8E4;
						}
					loc_823EC7D4:
						// lwz r10,12(r31)
						ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 12);
						// cmplw cr6,r10,r29
						// bne cr6,0x823ec810
						if (ctx.r10.u32 == var_r29) {
							// mr r4,r31
							ctx.r4.u64 = var_r31;
							// mr r3,r28
							ctx.r3.u64 = var_r28;
							// mr r29,r31
							var_r29 = (uint32_t)(var_r31);
							// bl 0x823ecfb0
							SinglesNetworkClient_CFB0_g(ctx, base);
							// lwz r9,16(r31)
							ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
							// rlwinm r31,r9,0,0,30
							var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE);
							// cmplwi cr6,r31,0
							// beq cr6,0x823ec80c
							if (var_r31 != 0) {
								// lwz r8,16(r29)
								ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 16);
								// rlwinm r4,r8,0,0,30
								ctx.r4.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
								// b 0x823ec810
							} else {
							loc_823EC80C:
								// li r4,0
								ctx.r4.s64 = 0;
							}
						}
					loc_823EC810:
						// lwz r7,16(r31)
						ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
						// cmplwi cr6,r4,0
						// ori r6,r7,1
						ctx.r6.u64 = ctx.r7.u64 | 1;
						// stw r6,16(r31)
						PPC_STORE_U32(var_r31 + 16, ctx.r6.u32);
						// beq cr6,0x823ec8e4
						if (ctx.r4.u32 == 0) goto loc_823EC8E4;
						// lwz r5,16(r4)
						ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
						// mr r3,r28
						ctx.r3.u64 = var_r28;
						// rlwinm r11,r5,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
						// stw r11,16(r4)
						PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r11.u32);
						// bl 0x823ecf28
						SinglesNetworkClient_CF28_g(ctx, base);
						// b 0x823ec8e4
					} else {
					loc_823EC83C:
						// lwz r11,8(r4)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
						// cmplwi cr6,r11,0
						// beq cr6,0x823ec880
						if (ctx.r11.u32 != 0) {
							// lwz r9,16(r11)
							ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
							// clrlwi r8,r9,31
							ctx.r8.u64 = ctx.r9.u32 & 0x1;
							// cmplwi cr6,r8,0
							// bne cr6,0x823ec880
							if (ctx.r8.u32 != 0) goto loc_823EC880;
							// ori r7,r10,1
							ctx.r7.u64 = ctx.r10.u64 | 1;
							// mr r29,r4
							var_r29 = ctx.r4.u32;
							// stw r7,16(r31)
							PPC_STORE_U32(var_r31 + 16, ctx.r7.u32);
							// lwz r6,16(r11)
							ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
							// ori r5,r6,1
							ctx.r5.u64 = ctx.r6.u64 | 1;
							// stw r5,16(r11)
							PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r5.u32);
							// lwz r3,16(r4)
							ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
							// rlwinm r11,r3,0,0,30
							ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
							// stw r11,16(r4)
							PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r11.u32);
							// b 0x823ec8e4
						} else {
						loc_823EC880:
							// lwz r10,8(r31)
							ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 8);
							// cmplw cr6,r10,r29
							// bne cr6,0x823ec8bc
							if (ctx.r10.u32 == var_r29) {
								// mr r4,r31
								ctx.r4.u64 = var_r31;
								// mr r3,r28
								ctx.r3.u64 = var_r28;
								// mr r29,r31
								var_r29 = (uint32_t)(var_r31);
								// bl 0x823ecf28
								SinglesNetworkClient_CF28_g(ctx, base);
								// lwz r9,16(r31)
								ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
								// rlwinm r31,r9,0,0,30
								var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE);
								// cmplwi cr6,r31,0
								// beq cr6,0x823ec8b8
								if (var_r31 != 0) {
									// lwz r8,16(r29)
									ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 16);
									// rlwinm r4,r8,0,0,30
									ctx.r4.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
									// b 0x823ec8bc
								} else {
								loc_823EC8B8:
									// li r4,0
									ctx.r4.s64 = 0;
								}
							}
						loc_823EC8BC:
							// lwz r7,16(r31)
							ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
							// cmplwi cr6,r4,0
							// ori r6,r7,1
							ctx.r6.u64 = ctx.r7.u64 | 1;
							// stw r6,16(r31)
							PPC_STORE_U32(var_r31 + 16, ctx.r6.u32);
							// beq cr6,0x823ec8e4
							if (ctx.r4.u32 == 0) goto loc_823EC8E4;
							// lwz r5,16(r4)
							ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
							// mr r3,r28
							ctx.r3.u64 = var_r28;
							// rlwinm r11,r5,0,0,30
							ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
							// stw r11,16(r4)
							PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r11.u32);
							// bl 0x823ecfb0
							SinglesNetworkClient_CFB0_g(ctx, base);
						}
					}
				loc_823EC8E4:
					// lwz r10,16(r29)
					ctx.r10.u64 = PPC_LOAD_U32(var_r29 + 16);
					// rlwinm r31,r10,0,0,30
					var_r31 = (uint32_t)(__builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFE);
					// cmplwi cr6,r31,0
					// beq cr6,0x823ec904
					if (var_r31 == 0) goto loc_823EC904;
					// lwz r9,16(r31)
					ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
					// rlwinm r4,r9,0,0,30
					ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
					// cmplwi cr6,r4,0
					// bne cr6,0x823ec774
			}
			}
		loc_823EC904:
			// lwz r11,0(r28)
			ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 0);
			// lwz r8,16(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
			// ori r7,r8,1
			ctx.r7.u64 = ctx.r8.u64 | 1;
			// stw r7,16(r11)
			PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r7.u32);
		}
	loc_823EC914:
		// lwz r11,4(r28)
		ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 4);
		// addi r6,r11,1
		ctx.r6.s64 = ctx.r11.s64 + 1;
		// stw r6,4(r28)
		PPC_STORE_U32(var_r28 + 4, ctx.r6.u32);
	}
loc_823EC920:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_C930_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_C930_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_C930_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,8(r29)
	ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823ec99c
	if (ctx.r11.u32 != 0) {
		// lwz r10,12(r29)
		ctx.r10.u64 = PPC_LOAD_U32(var_r29 + 12);
		// cmplwi cr6,r10,0
		// beq cr6,0x823ec99c
		if (ctx.r10.u32 == 0) goto loc_823EC99C;
		// lwz r9,4(r30)
		ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 4)/* SinglesNetworkClient::flags@+0x4 */;
		// clrlwi r8,r9,31
		ctx.r8.u64 = ctx.r9.u32 & 0x1;
		// cmpwi cr6,r8,0
		// beq cr6,0x823ec990
		if (ctx.r8.s32 != 0) {
			// rotlwi r3,r10,0
			ctx.r3.u64 = ctx.r10.u32;
			// lwz r7,8(r3)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
			// cmplwi cr6,r7,0
			// beq cr6,0x823ec9a0
			if (ctx.r7.u32 == 0) goto loc_823EC9A0;
		loc_823EC97C:
			// lwz r3,8(r3)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
			// lwz r6,8(r3)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
			// cmplwi cr6,r6,0
			// bne cr6,0x823ec97c
			if (ctx.r6.u32 != 0) goto loc_823EC97C;
			// b 0x823ec9a0
			goto loc_823EC9A0;
		}
	loc_823EC990:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x823ecec0
		SinglesNetworkClient_CEC0_g(ctx, base);
		// b 0x823ec9a0
	} else {
	loc_823EC99C:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
	}
loc_823EC9A0:
	// lwz r5,8(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r5,0
	// beq cr6,0x823ec9b4
	if (ctx.r5.u32 != 0) {
		// rotlwi r10,r5,0
		ctx.r10.u64 = ctx.r5.u32;
		// b 0x823ec9b8
	} else {
	loc_823EC9B4:
		// lwz r10,12(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	}
loc_823EC9B8:
	// cmplwi cr6,r10,0
	// beq cr6,0x823ec9d0
	if (ctx.r10.u32 != 0) {
		// lwz r4,16(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// lwz r11,16(r10)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
		// rlwimi r4,r11,0,31,31
		ctx.r4.u64 = (ctx.r11.u32 & 0x1) | (ctx.r4.u64 & 0xFFFFFFFFFFFFFFFE);
		// stw r4,16(r10)
		PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r4.u32);
	}
loc_823EC9D0:
	// lwz r9,16(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// rlwinm r11,r9,0,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r11,0
	// bne cr6,0x823ec9e8
	if (ctx.r11.u32 == 0) {
		// stw r10,0(r30)
		PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
		// b 0x823eca00
	} else {
	loc_823EC9E8:
		// lwz r8,8(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// cmplw cr6,r8,r3
		// bne cr6,0x823ec9fc
		if (ctx.r8.u32 == ctx.r3.u32) {
			// stw r10,8(r11)
			PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
			// b 0x823eca00
		} else {
		loc_823EC9FC:
			// stw r10,12(r11)
			PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
		}
	}
loc_823ECA00:
	// lwz r7,16(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// cmplw cr6,r3,r29
	// clrlwi r9,r7,31
	ctx.r9.u64 = ctx.r7.u32 & 0x1;
	// beq cr6,0x823ecac4
	if (ctx.r3.u32 != var_r29) {
		// lwz r6,0(r30)
		ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
		// cmplw cr6,r6,r29
		// bne cr6,0x823eca34
		if (ctx.r6.u32 == var_r29) {
			// lwz r5,8(r29)
			ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 8);
			// stw r5,8(r3)
			PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r5.u32);
			// lwz r4,12(r29)
			ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 12);
			// stw r4,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r4.u32);
			// stw r3,0(r30)
			PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r3.u32);
			// b 0x823eca54
		} else {
		loc_823ECA34:
			// lwz r11,16(r29)
			ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 16);
			// rlwinm r11,r11,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
			// lwz r8,8(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplw cr6,r8,r29
			// bne cr6,0x823eca50
			if (ctx.r8.u32 == var_r29) {
				// stw r3,8(r11)
				PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r3.u32);
				// b 0x823eca54
			} else {
			loc_823ECA50:
				// stw r3,12(r11)
				PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r3.u32);
			}
		}
	loc_823ECA54:
		// lwz r7,16(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// lwz r6,16(r29)
		ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 16);
		// rlwimi r6,r7,0,31,31
		ctx.r6.u64 = (ctx.r7.u32 & 0x1) | (ctx.r6.u64 & 0xFFFFFFFFFFFFFFFE);
		// stw r6,16(r3)
		PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r6.u32);
		// lwz r5,8(r29)
		ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 8);
		// stw r5,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r5.u32);
		// lwz r4,12(r29)
		ctx.r4.u64 = PPC_LOAD_U32(var_r29 + 12);
		// stw r4,12(r3)
		PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r4.u32);
		// lwz r11,16(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
		// lwz r8,16(r29)
		ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 16);
		// rlwimi r11,r8,0,31,31
		ctx.r11.u64 = (ctx.r8.u32 & 0x1) | (ctx.r11.u64 & 0xFFFFFFFFFFFFFFFE);
		// stw r11,16(r3)
		PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
		// lwz r7,8(r29)
		ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 8);
		// cmplwi cr6,r7,0
		// beq cr6,0x823ecaa4
		if (ctx.r7.u32 != 0) {
			// rotlwi r11,r7,0
			ctx.r11.u64 = ctx.r7.u32;
			// lwz r6,16(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
			// clrlwi r5,r6,31
			ctx.r5.u64 = ctx.r6.u32 & 0x1;
			// or r4,r5,r3
			ctx.r4.u64 = ctx.r5.u64 | ctx.r3.u64;
			// stw r4,16(r11)
			PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r4.u32);
		}
	loc_823ECAA4:
		// lwz r11,12(r29)
		ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 12);
		// cmplwi cr6,r11,0
		// beq cr6,0x823ecac4
		if (ctx.r11.u32 == 0) goto loc_823ECAC4;
		// rotlwi r11,r11,0
		ctx.r11.u64 = ctx.r11.u32;
		// lwz r8,16(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
		// clrlwi r7,r8,31
		ctx.r7.u64 = ctx.r8.u32 & 0x1;
		// or r6,r7,r3
		ctx.r6.u64 = ctx.r7.u64 | ctx.r3.u64;
		// stw r6,16(r11)
		PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r6.u32);
	}
loc_823ECAC4:
	// lbz r5,9(r30)
	ctx.r5.u64 = PPC_LOAD_U8(var_r30 + 9);
	// rlwinm r4,r5,0,0,24
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r4,0
	// beq cr6,0x823ecdf8
	if (ctx.r4.u32 != 0) {
		// cmplwi cr6,r10,0
		// beq cr6,0x823ecdf8
		if (ctx.r10.u32 == 0) goto loc_823ECDF8;
		// cmplwi cr6,r9,1
		// bne cr6,0x823ecdf8
		if (ctx.r9.u32 != 1) goto loc_823ECDF8;
		// addi r31,r10,16
		var_r31 = (uint32_t)(ctx.r10.s64 + 16);  // addr:0x82070010
		// lwz r3,0(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
		// rlwinm r11,r3,0,0,30
		ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
		// cmplwi cr6,r11,0
		// beq cr6,0x823ecdf8
	while (ctx.r11.u32 != 0) {
		loc_823ECAF8:
			// lwz r11,0(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
			// clrlwi r9,r11,31
			ctx.r9.u64 = ctx.r11.u32 & 0x1;
			// cmplwi cr6,r9,1
			// bne cr6,0x823ecdf8
			if (ctx.r9.u32 != 1) goto loc_823ECDF8;
			// rlwinm r11,r11,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
			// lwz r8,8(r11)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplw cr6,r8,r10
			// bne cr6,0x823ecc74
			if (ctx.r8.u32 == ctx.r10.u32) {
				// lwz r4,12(r11)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
				// cmplwi cr6,r4,0
				// beq cr6,0x823ecb74
				if (ctx.r4.u32 != 0) {
					// lwz r10,16(r4)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
					// clrlwi r7,r10,31
					ctx.r7.u64 = ctx.r10.u32 & 0x1;
					// cmplwi cr6,r7,0
					// bne cr6,0x823ecb6c
					if (ctx.r7.u32 == 0) {
						// ori r6,r10,1
						ctx.r6.u64 = ctx.r10.u64 | 1;
						// mr r3,r30
						ctx.r3.u64 = var_r30;
						// stw r6,16(r4)
						PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r6.u32);
						// lwz r5,0(r31)
						ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
						// rlwinm r11,r5,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,16(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
						// rlwinm r10,r4,0,0,30
						ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFFFE;
						// stw r10,16(r11)
						PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
						// lwz r9,0(r31)
						ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
						// rlwinm r4,r9,0,0,30
						ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
						// bl 0x823ecfb0
						SinglesNetworkClient_CFB0_g(ctx, base);
						// lwz r8,0(r31)
						ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
						// rlwinm r11,r8,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,12(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
					}
				loc_823ECB6C:
					// cmplwi cr6,r4,0
					// bne cr6,0x823ecb7c
					if (ctx.r4.u32 != 0) goto loc_823ECB7C;
				}
			loc_823ECB74:
				// mr r10,r11
				ctx.r10.u64 = ctx.r11.u64;
				// b 0x823ecde4
				goto loc_823ECDE4;
			loc_823ECB7C:
				// lwz r11,8(r4)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
				// cmplwi cr6,r11,0
				// beq cr6,0x823ecb9c
				if (ctx.r11.u32 != 0) {
					// rotlwi r7,r11,0
					ctx.r7.u64 = ctx.r11.u32;
					// lwz r6,16(r7)
					ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
					// clrlwi r5,r6,31
					ctx.r5.u64 = ctx.r6.u32 & 0x1;
					// cmplwi cr6,r5,1
					// bne cr6,0x823ecbbc
					if (ctx.r5.u32 != 1) goto loc_823ECBBC;
				}
			loc_823ECB9C:
				// lwz r3,12(r4)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
				// cmplwi cr6,r3,0
				// beq cr6,0x823ecdd0
				if (ctx.r3.u32 == 0) goto loc_823ECDD0;
				// rotlwi r10,r3,0
				ctx.r10.u64 = ctx.r3.u32;
				// lwz r9,16(r10)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
				// clrlwi r8,r9,31
				ctx.r8.u64 = ctx.r9.u32 & 0x1;
				// cmplwi cr6,r8,1
				// beq cr6,0x823ecdd0
				if (ctx.r8.u32 == 1) goto loc_823ECDD0;
			loc_823ECBBC:
				// lwz r7,12(r4)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
				// cmplwi cr6,r7,0
				// beq cr6,0x823ecbdc
				if (ctx.r7.u32 != 0) {
					// rotlwi r6,r7,0
					ctx.r6.u64 = ctx.r7.u32;
					// lwz r5,16(r6)
					ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
					// clrlwi r3,r5,31
					ctx.r3.u64 = ctx.r5.u32 & 0x1;
					// cmplwi cr6,r3,1
					// bne cr6,0x823ecc14
					if (ctx.r3.u32 != 1) goto loc_823ECC14;
				}
			loc_823ECBDC:
				// cmplwi cr6,r11,0
				// beq cr6,0x823ecbf4
				if (ctx.r11.u32 != 0) {
					// lwz r11,8(r4)
					ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
					// lwz r10,16(r11)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
					// ori r9,r10,1
					ctx.r9.u64 = ctx.r10.u64 | 1;
					// stw r9,16(r11)
					PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r9.u32);
				}
			loc_823ECBF4:
				// lwz r8,16(r4)
				ctx.r8.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// rlwinm r7,r8,0,0,30
				ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
				// stw r7,16(r4)
				PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r7.u32);
				// bl 0x823ecf28
				SinglesNetworkClient_CF28_g(ctx, base);
				// lwz r6,0(r31)
				ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
				// rlwinm r5,r6,0,0,30
				ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFFE;
				// lwz r4,12(r5)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 12);
			loc_823ECC14:
				// lwz r3,0(r31)
				ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
				// lwz r9,16(r4)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
				// rlwinm r11,r3,0,0,30
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
				// lwz r10,16(r11)
				ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
				// rlwimi r10,r9,0,0,30
				ctx.r10.u64 = (ctx.r9.u32 & 0xFFFFFFFE) | (ctx.r10.u64 & 0xFFFFFFFF00000001);
				// stw r10,16(r4)
				PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r10.u32);
				// lwz r8,0(r31)
				ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
				// rlwinm r11,r8,0,0,30
				ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
				// lwz r7,16(r11)
				ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
				// ori r6,r7,1
				ctx.r6.u64 = ctx.r7.u64 | 1;
				// stw r6,16(r11)
				PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r6.u32);
				// lwz r5,12(r4)
				ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
				// cmplwi cr6,r5,0
				// beq cr6,0x823ecc5c
				if (ctx.r5.u32 != 0) {
					// rotlwi r11,r5,0
					ctx.r11.u64 = ctx.r5.u32;
					// lwz r4,16(r11)
					ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
					// ori r3,r4,1
					ctx.r3.u64 = ctx.r4.u64 | 1;
					// stw r3,16(r11)
					PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r3.u32);
				}
			loc_823ECC5C:
				// lwz r11,0(r31)
				ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// rlwinm r4,r11,0,0,30
				ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
				// bl 0x823ecfb0
				SinglesNetworkClient_CFB0_g(ctx, base);
				// lwz r10,0(r30)
				ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
				// b 0x823ecde4
			} else {
			loc_823ECC74:
				// lwz r4,8(r11)
				ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
				// cmplwi cr6,r4,0
				// beq cr6,0x823eccd0
				if (ctx.r4.u32 != 0) {
					// lwz r10,16(r4)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
					// clrlwi r7,r10,31
					ctx.r7.u64 = ctx.r10.u32 & 0x1;
					// cmplwi cr6,r7,0
					// bne cr6,0x823eccc8
					if (ctx.r7.u32 == 0) {
						// ori r6,r10,1
						ctx.r6.u64 = ctx.r10.u64 | 1;
						// mr r3,r30
						ctx.r3.u64 = var_r30;
						// stw r6,16(r4)
						PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r6.u32);
						// lwz r5,0(r31)
						ctx.r5.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
						// rlwinm r11,r5,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,16(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
						// rlwinm r10,r4,0,0,30
						ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0xFFFFFFFE;
						// stw r10,16(r11)
						PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
						// lwz r9,0(r31)
						ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
						// rlwinm r4,r9,0,0,30
						ctx.r4.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
						// bl 0x823ecf28
						SinglesNetworkClient_CF28_g(ctx, base);
						// lwz r8,0(r31)
						ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
						// rlwinm r11,r8,0,0,30
						ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
						// lwz r4,8(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
					}
				loc_823ECCC8:
					// cmplwi cr6,r4,0
					// bne cr6,0x823eccd8
					if (ctx.r4.u32 != 0) goto loc_823ECCD8;
				}
			loc_823ECCD0:
				// mr r10,r11
				ctx.r10.u64 = ctx.r11.u64;
				// b 0x823ecde4
				goto loc_823ECDE4;
			loc_823ECCD8:
				// lwz r11,12(r4)
				ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
				// cmplwi cr6,r11,0
				// beq cr6,0x823eccf8
				if (ctx.r11.u32 != 0) {
					// rotlwi r7,r11,0
					ctx.r7.u64 = ctx.r11.u32;
					// lwz r6,16(r7)
					ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 16);
					// clrlwi r5,r6,31
					ctx.r5.u64 = ctx.r6.u32 & 0x1;
					// cmplwi cr6,r5,1
					// bne cr6,0x823ecd18
					if (ctx.r5.u32 != 1) goto loc_823ECD18;
				}
			loc_823ECCF8:
				// lwz r3,8(r4)
				ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
				// cmplwi cr6,r3,0
				// beq cr6,0x823ecdd0
				if (ctx.r3.u32 != 0) {
					// rotlwi r10,r3,0
					ctx.r10.u64 = ctx.r3.u32;
					// lwz r9,16(r10)
					ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
					// clrlwi r8,r9,31
					ctx.r8.u64 = ctx.r9.u32 & 0x1;
					// cmplwi cr6,r8,1
					// beq cr6,0x823ecdd0
					if (ctx.r8.u32 == 1) goto loc_823ECDD0;
				loc_823ECD18:
					// lwz r7,8(r4)
					ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
					// cmplwi cr6,r7,0
					// beq cr6,0x823ecd38
					if (ctx.r7.u32 != 0) {
						// rotlwi r6,r7,0
						ctx.r6.u64 = ctx.r7.u32;
						// lwz r5,16(r6)
						ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 16);
						// clrlwi r3,r5,31
						ctx.r3.u64 = ctx.r5.u32 & 0x1;
						// cmplwi cr6,r3,1
						// bne cr6,0x823ecd70
						if (ctx.r3.u32 != 1) goto loc_823ECD70;
					}
				loc_823ECD38:
					// cmplwi cr6,r11,0
					// beq cr6,0x823ecd50
					if (ctx.r11.u32 != 0) {
						// lwz r11,12(r4)
						ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
						// lwz r10,16(r11)
						ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
						// ori r9,r10,1
						ctx.r9.u64 = ctx.r10.u64 | 1;
						// stw r9,16(r11)
						PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r9.u32);
					}
				loc_823ECD50:
					// lwz r8,16(r4)
					ctx.r8.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
					// mr r3,r30
					ctx.r3.u64 = var_r30;
					// rlwinm r7,r8,0,0,30
					ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
					// stw r7,16(r4)
					PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r7.u32);
					// bl 0x823ecfb0
					SinglesNetworkClient_CFB0_g(ctx, base);
					// lwz r6,0(r31)
					ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
					// rlwinm r5,r6,0,0,30
					ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFFE;
					// lwz r4,8(r5)
					ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 8);
				loc_823ECD70:
					// lwz r3,0(r31)
					ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
					// lwz r9,16(r4)
					ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
					// rlwinm r11,r3,0,0,30
					ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFFE;
					// lwz r10,16(r11)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
					// rlwimi r10,r9,0,0,30
					ctx.r10.u64 = (ctx.r9.u32 & 0xFFFFFFFE) | (ctx.r10.u64 & 0xFFFFFFFF00000001);
					// stw r10,16(r4)
					PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r10.u32);
					// lwz r8,0(r31)
					ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
					// rlwinm r11,r8,0,0,30
					ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
					// lwz r7,16(r11)
					ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
					// ori r6,r7,1
					ctx.r6.u64 = ctx.r7.u64 | 1;
					// stw r6,16(r11)
					PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r6.u32);
					// lwz r5,8(r4)
					ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
					// cmplwi cr6,r5,0
					// beq cr6,0x823ecdb8
					if (ctx.r5.u32 != 0) {
						// rotlwi r11,r5,0
						ctx.r11.u64 = ctx.r5.u32;
						// lwz r4,16(r11)
						ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
						// ori r3,r4,1
						ctx.r3.u64 = ctx.r4.u64 | 1;
						// stw r3,16(r11)
						PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r3.u32);
					}
				loc_823ECDB8:
					// lwz r11,0(r31)
					ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
					// mr r3,r30
					ctx.r3.u64 = var_r30;
					// rlwinm r4,r11,0,0,30
					ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFFFE;
					// bl 0x823ecf28
					SinglesNetworkClient_CF28_g(ctx, base);
					// lwz r10,0(r30)
					ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
					// b 0x823ecde4
				} else {
				loc_823ECDD0:
					// lwz r10,16(r4)
					ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
					// rlwinm r9,r10,0,0,30
					ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFE;
					// stw r9,16(r4)
					PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r9.u32);
					// lwz r8,0(r31)
					ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
					// rlwinm r10,r8,0,0,30
					ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFE;
				}
			}
		loc_823ECDE4:
			// addi r31,r10,16
			var_r31 = (uint32_t)(ctx.r10.s64 + 16);  // addr:0x82070010
			// lwz r7,0(r31)
			ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
			// rlwinm r11,r7,0,0,30
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0xFFFFFFFE;
			// cmplwi cr6,r11,0
			// bne cr6,0x823ecaf8
	}
	}
loc_823ECDF8:
	// lwz r6,16(r29)
	ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 16);
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r5,r6,31
	ctx.r5.u64 = ctx.r6.u32 & 0x1;
	// stw r5,16(r29)
	PPC_STORE_U32(var_r29 + 16, ctx.r5.u32);
	// stw r11,12(r29)
	PPC_STORE_U32(var_r29 + 12, ctx.r11.u32);
	// stw r11,8(r29)
	PPC_STORE_U32(var_r29 + 8, ctx.r11.u32);
	// lwz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// addi r4,r11,-1
	ctx.r4.s64 = ctx.r11.s64 + -1;
	// stw r4,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ ctx.r4.u32);
	return;
}

__attribute__((alias("__imp__game_CE28"))) PPC_WEAK_FUNC(game_CE28);
PPC_FUNC_IMPL(__imp__game_CE28) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x8236c818
	game_C818(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823ece74
while (ctx.r10.u32 == 0) {
	loc_823ECE4C:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823ece88
		game_CE88(ctx, base);
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823ec930
		SinglesNetworkClient_C930_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8236c818
		game_C818(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823ece4c
}
loc_823ECE74:
	// blr
	return;
}

__attribute__((alias("__imp__game_CE88"))) PPC_WEAK_FUNC(game_CE88);
PPC_FUNC_IMPL(__imp__game_CE88) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,0(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r3,0
	// beq cr6,0x823eceb4
	if (ctx.r3.u32 != 0) {
		// lwz r11,8(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// cmplwi cr6,r11,0
		// beqlr cr6
		if (ctx.r11.u32 == 0) return;
	loc_823ECEA0:
		// lwz r3,8(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// lwz r10,8(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
		// cmplwi cr6,r10,0
		// bne cr6,0x823ecea0
		if (ctx.r10.u32 != 0) goto loc_823ECEA0;
		// blr
		return;
	}
loc_823ECEB4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_CEC0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_CEC0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_CEC0_g) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	// beq cr6,0x823ecef4
	if (ctx.r10.u32 != 0) {
		// rotlwi r3,r10,0
		ctx.r3.u64 = ctx.r10.u32;
		// lwz r9,12(r3)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// cmplwi cr6,r9,0
		// beqlr cr6
		if (ctx.r9.u32 == 0) return;
	loc_823ECEE0:
		// lwz r3,12(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// lwz r8,12(r3)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
		// cmplwi cr6,r8,0
		// bne cr6,0x823ecee0
		if (ctx.r8.u32 != 0) goto loc_823ECEE0;
		// blr
		return;
	}
loc_823ECEF4:
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// rlwinm r3,r7,0,0,30
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
loc_823ECF04:
	// lwz r6,8(r3)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplw cr6,r6,r11
	// bnelr cr6
	if (ctx.r6.u32 != ctx.r11.u32) return;
	// lwz r5,16(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// rlwinm r3,r5,0,0,30
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r3,0
	// bne cr6,0x823ecf04
	if (ctx.r3.u32 != 0) goto loc_823ECF04;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_CF28_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_CF28_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_CF28_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r10,8(r4)
	PPC_STORE_U32(ctx.r4.u32 + 8, ctx.r10.u32);
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmplwi cr6,r9,0
	// beq cr6,0x823ecf54
	if (ctx.r9.u32 != 0) {
		// rotlwi r10,r9,0
		ctx.r10.u64 = ctx.r9.u32;
		// lwz r8,16(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
		// clrlwi r7,r8,31
		ctx.r7.u64 = ctx.r8.u32 & 0x1;
		// or r6,r7,r4
		ctx.r6.u64 = ctx.r7.u64 | ctx.r4.u64;
		// stw r6,16(r10)
		PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r6.u32);
	}
loc_823ECF54:
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r10,16(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// rlwimi r10,r5,0,31,31
	ctx.r10.u64 = (ctx.r5.u32 & 0x1) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFFE);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lwz r9,16(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// rlwinm r10,r9,0,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r10,0
	// bne cr6,0x823ecf7c
	if (ctx.r10.u32 == 0) {
		// stw r11,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
		// b 0x823ecf94
	} else {
	loc_823ECF7C:
		// lwz r8,12(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
		// cmplw cr6,r8,r4
		// bne cr6,0x823ecf90
		if (ctx.r8.u32 == ctx.r4.u32) {
			// stw r11,12(r10)
			PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
			// b 0x823ecf94
		} else {
		loc_823ECF90:
			// stw r11,8(r10)
			PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
		}
	}
loc_823ECF94:
	// stw r4,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r4.u32);
	// lwz r7,16(r4)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// clrlwi r6,r7,31
	ctx.r6.u64 = ctx.r7.u32 & 0x1;
	// or r5,r6,r11
	ctx.r5.u64 = ctx.r6.u64 | ctx.r11.u64;
	// stw r5,16(r4)
	PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r5.u32);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_CFB0_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_CFB0_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_CFB0_g) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,12(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 12);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r10,12(r4)
	PPC_STORE_U32(ctx.r4.u32 + 12, ctx.r10.u32);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmplwi cr6,r9,0
	// beq cr6,0x823ecfdc
	if (ctx.r9.u32 != 0) {
		// rotlwi r10,r9,0
		ctx.r10.u64 = ctx.r9.u32;
		// lwz r8,16(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
		// clrlwi r7,r8,31
		ctx.r7.u64 = ctx.r8.u32 & 0x1;
		// or r6,r7,r4
		ctx.r6.u64 = ctx.r7.u64 | ctx.r4.u64;
		// stw r6,16(r10)
		PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r6.u32);
	}
loc_823ECFDC:
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r10,16(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// rlwimi r10,r5,0,31,31
	ctx.r10.u64 = (ctx.r5.u32 & 0x1) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFFE);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lwz r9,16(r4)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// rlwinm r10,r9,0,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFFFE;
	// cmplwi cr6,r10,0
	// bne cr6,0x823ed004
	if (ctx.r10.u32 == 0) {
		// stw r11,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
		// b 0x823ed01c
	} else {
	loc_823ED004:
		// lwz r8,8(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		// cmplw cr6,r8,r4
		// bne cr6,0x823ed018
		if (ctx.r8.u32 == ctx.r4.u32) {
			// stw r11,8(r10)
			PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
			// b 0x823ed01c
		} else {
		loc_823ED018:
			// stw r11,12(r10)
			PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
		}
	}
loc_823ED01C:
	// stw r4,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r4.u32);
	// lwz r7,16(r4)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r4.u32 + 16);
	// clrlwi r6,r7,31
	ctx.r6.u64 = ctx.r7.u32 & 0x1;
	// or r5,r6,r11
	ctx.r5.u64 = ctx.r6.u64 | ctx.r11.u64;
	// stw r5,16(r4)
	PPC_STORE_U32(ctx.r4.u32 + 16, ctx.r5.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snGamer_vfn_0"))) PPC_WEAK_FUNC(snGamer_vfn_0);
PPC_FUNC_IMPL(__imp__snGamer_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,15972
	ctx.r11.s64 = ctx.r11.s64 + 15972;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x82368ef8
	atSafeDLListSimple_ctor_8EF8(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed07c
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823ED07C:
	// blr
	return;
}

__attribute__((alias("__imp__snLeaveMachine_D098_g"))) PPC_WEAK_FUNC(snLeaveMachine_D098_g);
PPC_FUNC_IMPL(__imp__snLeaveMachine_D098_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// lwz r11,444(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 444);
	// addi r27,r11,-1
	var_r27 = (uint32_t)(ctx.r11.s64 + -1);  // addr:0x8206ffff
	// cmpwi cr6,r27,0
	// stw r27,444(r28)
	PPC_STORE_U32(var_r28 + 444, var_r27);
	// bne cr6,0x823ed15c
	if ((int32_t)var_r27 == 0) {
		// lwz r11,36(r28)
		ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 36);
		// cmpwi cr6,r11,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bge cr6,0x823ed0d0
		if (ctx.r11.s32 < 0) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823ED0D0:
		// clrlwi r9,r11,24
		ctx.r9.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r9,0
		// bne cr6,0x823ed0ec
		if (ctx.r9.u32 == 0) {
			// lbz r8,76(r28)
			ctx.r8.u64 = PPC_LOAD_U8(var_r28 + 76);
			// li r11,1
			ctx.r11.s64 = 1;
			// cmplwi cr6,r8,0
			// bne cr6,0x823ed0f0
			if (ctx.r8.u32 != 0) goto loc_823ED0F0;
		}
	loc_823ED0EC:
		// li r11,0
		ctx.r11.s64 = 0;
	loc_823ED0F0:
		// clrlwi r6,r11,24
		ctx.r6.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r6,0
		// beq cr6,0x823ed15c
		if (ctx.r6.u32 == 0) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			return;
		}
		// lwz r30,440(r28)
		var_r30 = (uint32_t)(PPC_LOAD_U32(var_r28 + 440));
		// addi r4,r28,96
		ctx.r4.s64 = (int64_t)(int32_t)var_r28 + 96;
		// addi r29,r30,720
		var_r29 = (uint32_t)(var_r30 + 720);
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x823ea250
		SinglesNetworkClient_A250_g(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// beq cr6,0x823ed144
		if (var_r31 != 0) {
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x823ec930
			SinglesNetworkClient_C930_g(ctx, base);
			// lwz r11,716(r30)
			ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 716);
			// lwz r5,12(r11)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
			// stw r5,0(r31)
			PPC_STORE_U32(var_r31 + 0, ctx.r5.u32);
			// lhz r10,6(r11)
			ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
			// stw r31,12(r11)
			PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
			// addi r4,r10,1
			ctx.r4.s64 = ctx.r10.s64 + 1;
			// sth r4,6(r11)
			PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r4.u16);
		}
	loc_823ED144:
		// lwz r11,0(r30)
  // [ph4a] vtable load collapsed
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// lwz r10,8(r11)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r30, 2, ctx, base);  // pattern-B slot 2 (byte +8)
	}
loc_823ED15C:
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	return;
}

__attribute__((alias("__imp__hsmEventTimeout_vfn_2"))) PPC_WEAK_FUNC(hsmEventTimeout_vfn_2);
PPC_FUNC_IMPL(__imp__hsmEventTimeout_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16000
	ctx.r3.s64 = ctx.r11.s64 + 16000;
	// blr
	return;
}

__attribute__((alias("__imp__hsmContext_Top_vfn_2"))) PPC_WEAK_FUNC(hsmContext_Top_vfn_2);
PPC_FUNC_IMPL(__imp__hsmContext_Top_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-7764
	ctx.r3.s64 = ctx.r11.s64 + -7764;
	// blr
	return;
}

__attribute__((alias("__imp__util_D188"))) PPC_WEAK_FUNC(util_D188);
PPC_FUNC_IMPL(__imp__util_D188) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0);
	// cmplwi cr6,r3,0
	// beq cr6,0x823ed1d4
	if (ctx.r3.u32 != 0) {
		// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
		// li r4,0
		ctx.r4.s64 = 0;
		// lwz r10,0(r11)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 0, ctx, base);  // pattern-B slot 0 (byte +0)
		// lwz r3,4(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
		// lwz r4,0(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 0);
		// lwz r8,8(r9)
		// bctrl
		VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
	}
loc_823ED1D4:
	// addi r10,r31,8
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmplwi cr6,r11,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823ed264
	if (ctx.r11.u32 != 0) {
		// addi r11,r11,4
		ctx.r11.s64 = ctx.r11.s64 + 4;
		// li r8,0
		ctx.r8.s64 = 0;
		// lwz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// cmplwi cr6,r9,0
		// bne cr6,0x823ed224
		if (ctx.r9.u32 == 0) {
			// lwz r7,4(r11)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
			// stw r7,0(r10)
			PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r7.u32);
			// lwz r6,4(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
			// cmplwi cr6,r6,0
			// beq cr6,0x823ed21c
			if (ctx.r6.u32 != 0) {
				// rotlwi r5,r6,0
				ctx.r5.u64 = ctx.r6.u32;
				// stw r8,4(r5)
				PPC_STORE_U32(ctx.r5.u32 + 4, ctx.r8.u32);
				// b 0x823ed250
				goto loc_823ED250;
			}
		loc_823ED21C:
			// stw r8,4(r10)
			PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
			// b 0x823ed250
		} else {
		loc_823ED224:
			// lwz r4,4(r11)
			ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
			// stw r4,8(r9)
			PPC_STORE_U32(ctx.r9.u32 + 8, ctx.r4.u32);
			// lwz r3,4(r11)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
			// cmplwi cr6,r3,0
			// beq cr6,0x823ed248
			if (ctx.r3.u32 != 0) {
				// lwz r9,0(r11)
				ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
				// rotlwi r7,r3,0
				ctx.r7.u64 = ctx.r3.u32;
				// stw r9,4(r7)
				PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r9.u32);
				// b 0x823ed250
			} else {
			loc_823ED248:
				// lwz r6,0(r11)
				ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
				// stw r6,4(r10)
				PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r6.u32);
			}
		}
	loc_823ED250:
		// stw r8,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r8.u32);
		// stw r8,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
		// lwz r11,8(r10)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		// addi r5,r11,-1
		ctx.r5.s64 = ctx.r11.s64 + -1;
		// stw r5,8(r10)
		PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r5.u32);
	}
loc_823ED264:
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0);
	// blr
	return;
}

__attribute__((alias("__imp__hsmState_vfn_2"))) PPC_WEAK_FUNC(hsmState_vfn_2);
PPC_FUNC_IMPL(__imp__hsmState_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16028
	ctx.r3.s64 = ctx.r11.s64 + 16028;
	// blr
	return;
}

__attribute__((alias("__imp__hsmState_vfn_3"))) PPC_WEAK_FUNC(hsmState_vfn_3);
PPC_FUNC_IMPL(__imp__hsmState_vfn_3) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed32c
	if (ctx.r11.u32 != 0) {
		// rotlwi r3,r11,0
		ctx.r3.u64 = ctx.r11.u32;
		// lwz r9,12(r10)
		// bctrl
		VCALL(ctx.r3.u32, 3, ctx, base);  // vtable slot 3 (byte +12)
		// mr r11,r28
		ctx.r11.u64 = var_r28;
		// mr r10,r11
		ctx.r10.u64 = ctx.r11.u64;
	loc_823ED2D0:
		// lbz r8,0(r11)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// cmplwi cr6,r8,0
		// bne cr6,0x823ed2d0
		if (ctx.r8.u32 != 0) goto loc_823ED2D0;
		// subf r11,r10,r11
		ctx.r11.s64 = ctx.r11.s64 - ctx.r10.s64;
		// addi r7,r11,-1
		ctx.r7.s64 = ctx.r11.s64 + -1;
		// rotlwi r29,r7,0
		var_r29 = (uint32_t)(ctx.r7.u32);
		// cmpw cr6,r29,r30
		// bge cr6,0x823ed37c
		if ((int32_t)var_r29 >= (int32_t)var_r30) {
			// mr r3,r28
			ctx.r3.u64 = var_r28;
			return;
		}
		// lwz r6,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r5,8(r6)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 2, ctx, base);  // pattern-B slot 2 (byte +8)
		// lis r11,-32249
		// mr r6,r3
		ctx.r6.u64 = ctx.r3.u64;
		// addi r5,r11,16040
		ctx.r5.s64 = ctx.r11.s64 + 16040;
		// subf r4,r29,r30
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 - (int64_t)(int32_t)var_r29;
		// add r3,r29,r28
		ctx.r3.u64 = var_r29 + var_r28;
		// bl 0x820c0b40
		_snprintf(ctx, base);
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		return;
	}
loc_823ED32C:
	// lwz r4,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r11,8(r4)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 2, ctx, base);  // pattern-B slot 2 (byte +8)
	// addi r11,r30,-1
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + -1;
	// mr r10,r28
	ctx.r10.u64 = var_r28;
	// cmpwi cr6,r11,0
	// ble cr6,0x823ed374
while (ctx.r11.s32 > 0) {
	loc_823ED350:
		// lbz r9,0(r3)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
		// cmplwi cr6,r9,0
		// beq cr6,0x823ed374
		if (ctx.r9.u32 == 0) goto loc_823ED374;
		// addi r11,r11,-1
		ctx.r11.s64 = ctx.r11.s64 + -1;
		// stb r9,0(r10)
		PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r9.u8);
		// addi r3,r3,1
		ctx.r3.s64 = ctx.r3.s64 + 1;
		// addi r10,r10,1
		ctx.r10.s64 = ctx.r10.s64 + 1;
		// cmpwi cr6,r11,0
		// bgt cr6,0x823ed350
}
loc_823ED374:
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r7,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r7.u8);
loc_823ED37C:
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	return;
}

__attribute__((alias("__imp__hsmStateBase_vfn_4"))) PPC_WEAK_FUNC(hsmStateBase_vfn_4);
PPC_FUNC_IMPL(__imp__hsmStateBase_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r11.u32);
	// stw r11,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r11.u32);
	// stw r11,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__hsmState_vfn_12"))) PPC_WEAK_FUNC(hsmState_vfn_12);
PPC_FUNC_IMPL(__imp__hsmState_vfn_12) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,0(r5)
	PPC_STORE_U8(ctx.r5.u32 + 0, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__game_D3B0_h"))) PPC_WEAK_FUNC(game_D3B0_h);
PPC_FUNC_IMPL(__imp__game_D3B0_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,32(r11)
	// bctrl
	VCALL(ctx.r3.u32, 8, ctx, base);  // vtable slot 8 (byte +32)
	// lwz r9,0(r31)
  // [ph4a] vtable load collapsed
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r8,44(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x823eddb8
	snSession_DDB8_w(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__exception_vfn_1"))) PPC_WEAK_FUNC(exception_vfn_1);
PPC_FUNC_IMPL(__imp__exception_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lwz r3,4(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_D418_w"))) PPC_WEAK_FUNC(snSession_D418_w);
PPC_FUNC_IMPL(__imp__snSession_D418_w) {
	PPC_FUNC_PROLOGUE();
	// mr r9,r3
	ctx.r9.u64 = ctx.r3.u64;
	// cmplw cr6,r9,r4
	// lwz r3,8(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 8);
	// beqlr cr6
	if (ctx.r9.u32 == ctx.r4.u32) return;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed454
	if (ctx.r11.u32 != 0) {
	loc_823ED438:
		// cmplw cr6,r11,r4
		// beq cr6,0x823ed450
		if (ctx.r11.u32 != ctx.r4.u32) {
			// lwz r11,8(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplwi cr6,r11,0
			// bne cr6,0x823ed438
			if (ctx.r11.u32 != 0) goto loc_823ED438;
			// b 0x823ed454
		} else {
		loc_823ED450:
			// li r10,1
			ctx.r10.s64 = 1;
		}
	}
loc_823ED454:
	// clrlwi r11,r10,24
	ctx.r11.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed468
	if (ctx.r11.u32 != 0) {
		// lwz r3,8(r4)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
		// blr
		return;
	}
loc_823ED468:
	// lwz r8,8(r4)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed498
	if (ctx.r11.u32 != 0) {
	loc_823ED47C:
		// cmplw cr6,r11,r9
		// beq cr6,0x823ed494
		if (ctx.r11.u32 != ctx.r9.u32) {
			// lwz r11,8(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplwi cr6,r11,0
			// bne cr6,0x823ed47c
			if (ctx.r11.u32 != 0) goto loc_823ED47C;
			// b 0x823ed498
		} else {
		loc_823ED494:
			// li r10,1
			ctx.r10.s64 = 1;
		}
	}
loc_823ED498:
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bnelr cr6
	if (ctx.r10.u32 != 0) return;
	// cmplwi cr6,r3,0
	// beqlr cr6
	if (ctx.r3.u32 == 0) return;
loc_823ED4AC:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed4d8
	if (ctx.r11.u32 != 0) {
	loc_823ED4BC:
		// cmplw cr6,r11,r3
		// beq cr6,0x823ed4d4
		if (ctx.r11.u32 != ctx.r3.u32) {
			// lwz r11,8(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			// cmplwi cr6,r11,0
			// bne cr6,0x823ed4bc
			if (ctx.r11.u32 != 0) goto loc_823ED4BC;
			// b 0x823ed4d8
		} else {
		loc_823ED4D4:
			// li r10,1
			ctx.r10.s64 = 1;
		}
	}
loc_823ED4D8:
	// clrlwi r9,r10,24
	ctx.r9.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// bnelr cr6
	if (ctx.r9.u32 != 0) return;
	// lwz r3,8(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r3,0
	// bne cr6,0x823ed4ac
	if (ctx.r3.u32 != 0) goto loc_823ED4AC;
	// blr
	return;
}

__attribute__((alias("__imp__util_D4F8"))) PPC_WEAK_FUNC(util_D4F8);
PPC_FUNC_IMPL(__imp__util_D4F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=128, manual
	// lwz r11,0(r3)
  // [ph4a] vtable load collapsed
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// lwz r10,44(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(ctx.r3.u32, 11, ctx, base);  // pattern-B slot 11 (byte +44)
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// mr r5,r30
	ctx.r5.u64 = var_r30;
	// addi r4,r11,16092
	ctx.r4.s64 = ctx.r11.s64 + 16092;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r6,80(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 80);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// lbz r8,104(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 104);
	// lwz r11,80(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 80);
	// ori r7,r8,128
	ctx.r7.u64 = ctx.r8.u64 | 128;
	// add r9,r11,r30
	ctx.r9.u64 = ctx.r11.u64 + var_r30;
	// stb r7,104(r31)
	PPC_STORE_U8(var_r31 + 104, ctx.r7.u8);
	// stw r9,76(r31)
	PPC_STORE_U32(var_r31 + 76, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__hsmContext_ctor_D570"))) PPC_WEAK_FUNC(hsmContext_ctor_D570);
PPC_FUNC_IMPL(__imp__hsmContext_ctor_D570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	// addi r10,r3,16
	ctx.r10.s64 = ctx.r3.s64 + 16;
	// addi r9,r11,16136
	ctx.r9.s64 = ctx.r11.s64 + 16136;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r9,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r9.u32);
	// lis r9,-32249
	// stw r11,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r11.u32);
	// stw r11,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r11.u32);
	// addi r9,r9,16252
	ctx.r9.s64 = ctx.r9.s64 + 16252;
	// stw r11,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r11.u32);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// stw r11,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
	// stw r11,16(r10)
	PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r11.u32);
	// stw r11,20(r10)
	PPC_STORE_U32(ctx.r10.u32 + 20, ctx.r11.u32);
	// stw r11,24(r10)
	PPC_STORE_U32(ctx.r10.u32 + 24, ctx.r11.u32);
	// stw r11,32(r10)
	PPC_STORE_U32(ctx.r10.u32 + 32, ctx.r11.u32);
	// stw r11,28(r10)
	PPC_STORE_U32(ctx.r10.u32 + 28, ctx.r11.u32);
	// stw r11,36(r10)
	PPC_STORE_U32(ctx.r10.u32 + 36, ctx.r11.u32);
	// stw r10,56(r3)
	PPC_STORE_U32(ctx.r3.u32 + 56, ctx.r10.u32);
	// stw r11,64(r3)
	PPC_STORE_U32(ctx.r3.u32 + 64, ctx.r11.u32);
	// stw r11,68(r3)
	PPC_STORE_U32(ctx.r3.u32 + 68, ctx.r11.u32);
	// stw r11,72(r3)
	PPC_STORE_U32(ctx.r3.u32 + 72, ctx.r11.u32);
	// stw r9,60(r3)
	PPC_STORE_U32(ctx.r3.u32 + 60, ctx.r9.u32);
	// stw r11,76(r3)
	PPC_STORE_U32(ctx.r3.u32 + 76, ctx.r11.u32);
	// stw r11,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r11.u32);
	// stw r11,84(r3)
	PPC_STORE_U32(ctx.r3.u32 + 84, ctx.r11.u32);
	// stw r11,88(r3)
	PPC_STORE_U32(ctx.r3.u32 + 88, ctx.r11.u32);
	// stw r11,96(r3)
	PPC_STORE_U32(ctx.r3.u32 + 96, ctx.r11.u32);
	// stw r11,92(r3)
	PPC_STORE_U32(ctx.r3.u32 + 92, ctx.r11.u32);
	// stw r11,100(r3)
	PPC_STORE_U32(ctx.r3.u32 + 100, ctx.r11.u32);
	// lbz r11,104(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 104);
	// clrlwi r10,r11,25
	ctx.r10.u64 = ctx.r11.u32 & 0x7F;
	// stb r10,104(r3)
	PPC_STORE_U8(ctx.r3.u32 + 104, ctx.r10.u8);
	// blr
	return;
}

__attribute__((alias("__imp__hsmContext_vfn_0"))) PPC_WEAK_FUNC(hsmContext_vfn_0);
PPC_FUNC_IMPL(__imp__hsmContext_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823ed6a8
	util_D6A8(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ed638
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823ED638:
	// blr
	return;
}

__attribute__((alias("__imp__hsmStateBase_vfn_0"))) PPC_WEAK_FUNC(hsmStateBase_vfn_0);
PPC_FUNC_IMPL(__imp__hsmStateBase_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r10,r11,16156
	ctx.r10.s64 = ctx.r11.s64 + 16156;
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r9,r4,31
	ctx.r9.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r9,0
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(var_r31 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(var_r31 + 12, ctx.r11.u32);
	// beq cr6,0x823ed694
	if (ctx.r9.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823ED694:
	// blr
	return;
}

__attribute__((alias("__imp__util_D6A8"))) PPC_WEAK_FUNC(util_D6A8);
PPC_FUNC_IMPL(__imp__util_D6A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,16136
	ctx.r11.s64 = ctx.r11.s64 + 16136;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// bl 0x823ed788
	util_D788(ctx, base);
	// lis r11,-32249
	// li r29,0
	var_r29 = 0;
	// addi r11,r11,16156
	ctx.r11.s64 = ctx.r11.s64 + 16156;
	// addi r30,r31,56
	var_r30 = (uint32_t)(var_r31 + 56);
	// li r28,1
	var_r28 = 1;
	// stw r29,64(r31)
	PPC_STORE_U32(var_r31 + 64, var_r29);
	// stw r11,60(r31)
	PPC_STORE_U32(var_r31 + 60, ctx.r11.u32);
	// stw r29,68(r31)
	PPC_STORE_U32(var_r31 + 68, var_r29);
	// stw r29,72(r31)
	PPC_STORE_U32(var_r31 + 72, var_r29);
loc_823ED6EC:
	// addi r30,r30,-20
	var_r30 = (uint32_t)(var_r30 + -20);
loc_823ED6F0:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823ed188
	util_D188(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x823ed6f0
	if (ctx.r3.u32 != 0) goto loc_823ED6F0;
	// addi r28,r28,-1
	var_r28 = (uint32_t)(var_r28 + -1);
	// stw r29,0(r30)
	PPC_STORE_U32(var_r30 + 0, var_r29);
	// stw r29,4(r30)
	PPC_STORE_U32(var_r30 + 4, var_r29);
	// cmpwi cr6,r28,0
	// bge cr6,0x823ed6ec
	if ((int32_t)var_r28 >= 0) goto loc_823ED6EC;
	return;
}

__attribute__((alias("__imp__snSessionFinder_D720_p46"))) PPC_WEAK_FUNC(snSessionFinder_D720_p46);
PPC_FUNC_IMPL(__imp__snSessionFinder_D720_p46) {
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
	// bl 0x823ed788
	util_D788(ctx, base);
	// lbz r9,104(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 104);
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r10,r31,60
	ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 60;
	// stw r29,4(r31)
	PPC_STORE_U32(var_r31 + 4, var_r29);
	// clrlwi r8,r9,25
	ctx.r8.u64 = ctx.r9.u32 & 0x7F;
	// stw r11,76(r31)
	PPC_STORE_U32(var_r31 + 76, ctx.r11.u32);
	// stw r11,80(r31)
	PPC_STORE_U32(var_r31 + 80, ctx.r11.u32);
	// stw r11,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r11.u32);
	// stw r11,88(r31)
	PPC_STORE_U32(var_r31 + 88, ctx.r11.u32);
	// stw r10,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r10.u32);
	// stb r8,104(r31)
	PPC_STORE_U8(var_r31 + 104, ctx.r8.u8);
	// stw r11,16(r31)
	PPC_STORE_U32(var_r31 + 16, ctx.r11.u32);
	// stw r30,20(r31)
	PPC_STORE_U32(var_r31 + 20, var_r30);
	// stw r11,36(r31)
	PPC_STORE_U32(var_r31 + 36, ctx.r11.u32);
	// stw r30,40(r31)
	PPC_STORE_U32(var_r31 + 40, var_r30);
	// lwz r7,4(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 4);
	// stw r10,8(r7)
	PPC_STORE_U32(ctx.r7.u32 + 8, ctx.r10.u32);
	return;
}

__attribute__((alias("__imp__util_D788"))) PPC_WEAK_FUNC(util_D788);
PPC_FUNC_IMPL(__imp__util_D788) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 4);
	// cmplwi cr6,r3,0
	// beq cr6,0x823ed7b4
	if (ctx.r3.u32 != 0) {
		// lwz r10,16(r11)
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // vtable slot 4 (byte +16)
	}
loc_823ED7B4:
	// lbz r9,104(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 104);
	// li r30,0
	var_r30 = 0;
	// addi r29,r31,16
	var_r29 = (uint32_t)(var_r31 + 16);
	// clrlwi r8,r9,25
	ctx.r8.u64 = ctx.r9.u32 & 0x7F;
	// stw r30,4(r31)
	PPC_STORE_U32(var_r31 + 4, var_r30);
	// stw r30,8(r31)
	PPC_STORE_U32(var_r31 + 8, var_r30);
	// stw r30,76(r31)
	PPC_STORE_U32(var_r31 + 76, var_r30);
	// stw r30,80(r31)
	PPC_STORE_U32(var_r31 + 80, var_r30);
	// stw r30,84(r31)
	PPC_STORE_U32(var_r31 + 84, var_r30);
	// stw r30,88(r31)
	PPC_STORE_U32(var_r31 + 88, var_r30);
	// stb r8,104(r31)
	PPC_STORE_U8(var_r31 + 104, ctx.r8.u8);
loc_823ED7E0:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823ed188
	util_D188(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x823ed7e0
	if (ctx.r3.u32 != 0) goto loc_823ED7E0;
	// addi r31,r31,36
	var_r31 = (uint32_t)(var_r31 + 36);
	// stw r30,0(r29)
	PPC_STORE_U32(var_r29 + 0, var_r30);
	// stw r30,4(r29)
	PPC_STORE_U32(var_r29 + 4, var_r30);
loc_823ED7FC:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ed188
	util_D188(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x823ed7fc
	if (ctx.r3.u32 != 0) goto loc_823ED7FC;
	// stw r30,0(r31)
	PPC_STORE_U32(var_r31 + 0, var_r30);
	// stw r30,4(r31)
	PPC_STORE_U32(var_r31 + 4, var_r30);
	return;
}

__attribute__((alias("__imp__hsmContext_rtti_3F08_1"))) PPC_WEAK_FUNC(hsmContext_rtti_3F08_1);
PPC_FUNC_IMPL(__imp__hsmContext_rtti_3F08_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// addi r11,r31,16
	ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 16;
	// lwz r30,56(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 56));
	// cmplw cr6,r11,r30
	// bne cr6,0x823ed848
	if (ctx.r11.u32 == var_r30) {
		// addi r11,r31,36
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 36;
	}
loc_823ED848:
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r11,56(r31)
	PPC_STORE_U32(var_r31 + 56, ctx.r11.u32);
	// bl 0x823ed188
	util_D188(ctx, base);
	// li r29,0
	var_r29 = 0;
	// cmplwi cr6,r3,0
	// beq cr6,0x823ed89c
while (ctx.r3.u32 != 0) {
	loc_823ED860:
		// lwz r11,84(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
		// cmpwi cr6,r11,0
		// li r11,1
		ctx.r11.s64 = 1;
		// bgt cr6,0x823ed874
		if (ctx.r11.s32 <= 0) {
			// mr r11,r29
			ctx.r11.u64 = var_r29;
		}
	loc_823ED874:
		// clrlwi r9,r11,24
		ctx.r9.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r9,0
		// bne cr6,0x823ed88c
		if (ctx.r9.u32 != 0) goto loc_823ED88C;
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823edc40
		SinglesNetworkClient_DC40_g(ctx, base);
	loc_823ED88C:
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823ed188
		util_D188(ctx, base);
		// cmplwi cr6,r3,0
		// bne cr6,0x823ed860
}
loc_823ED89C:
	// lbz r8,104(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 104);
	// stw r28,80(r31)
	PPC_STORE_U32(var_r31 + 80, var_r28);
	// rlwinm r7,r8,0,0,24
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r7,0
	// beq cr6,0x823ed93c
	if (ctx.r7.u32 != 0) {
		// lwz r6,76(r31)
		ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 76);
		// subf. r5,r6,r28
		ctx.r5.s64 = (int64_t)(int32_t)var_r28 - ctx.r6.s64;
		// blt 0x823ed93c
		if (ctx.r5.s32 < 0) goto loc_823ED93C;
		// lis r11,-32249
		// mr r5,r28
		ctx.r5.u64 = var_r28;
		// addi r4,r11,16056
		ctx.r4.s64 = ctx.r11.s64 + 16056;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// lis r11,-32249
		// lbz r4,104(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 104);
		// addi r3,r1,84
		ctx.r3.s64 = ctx.r1.s64 + 84;
		// addi r30,r11,10340
		var_r30 = (uint32_t)(ctx.r11.s64 + 10340);  // lbl_82072864 @ 0x82072864
		// clrlwi r11,r4,25
		ctx.r11.u64 = ctx.r4.u32 & 0x7F;
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
		// stb r11,104(r31)
		PPC_STORE_U8(var_r31 + 104, ctx.r11.u8);
		// bl 0x82121b00
		ke_1B00(ctx, base);
		// addi r3,r1,88
		ctx.r3.s64 = ctx.r1.s64 + 88;
		// bl 0x82121b00
		ke_1B00(ctx, base);
		// lis r11,-32249
		// lwz r10,84(r31)
		ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 84);
		// addi r11,r11,16228
		ctx.r11.s64 = ctx.r11.s64 + 16228;
		// stw r29,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r29);
		// stw r29,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, var_r29);
		// cmpwi cr6,r10,0
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// li r11,1
		ctx.r11.s64 = 1;
		// bgt cr6,0x823ed920
		if (ctx.r10.s32 <= 0) {
			// mr r11,r29
			ctx.r11.u64 = var_r29;
		}
	loc_823ED920:
		// clrlwi r8,r11,24
		ctx.r8.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// bne cr6,0x823ed938
		if (ctx.r8.u32 == 0) {
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x823edc40
			SinglesNetworkClient_DC40_g(ctx, base);
		}
	loc_823ED938:
		// stw r30,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r30);
	}
loc_823ED93C:
	// lwz r31,92(r31)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 92));
loc_823ED940:
	// cmplwi cr6,r31,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823ed950
	if (var_r31 == 0) {
		// mr r11,r29
		ctx.r11.u64 = var_r29;
	}
loc_823ED950:
	// clrlwi r6,r11,24
	ctx.r6.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r6,0
	// beq cr6,0x823ed97c
	if (ctx.r6.u32 == 0) {
		return;
	}
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 0);
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// lwz r11,36(r5)
	// bctrl
	VCALL(ctx.r3.u32, 9, ctx, base);  // vtable slot 9 (byte +36)
	// lwz r31,8(r31)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 8));
	// b 0x823ed940
	goto loc_823ED940;
}

__attribute__((alias("__imp__util_D988"))) PPC_WEAK_FUNC(util_D988);
PPC_FUNC_IMPL(__imp__util_D988) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// cmplwi cr6,r3,0
	// beq cr6,0x823eda6c
	if (ctx.r3.u32 != 0) {
		// lwz r11,88(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 88);
		// cmpwi cr6,r11,0
		// ble cr6,0x823ed9c0
		if (ctx.r11.s32 > 0) {
			// lwz r10,84(r31)
			ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 84);
			// cmpw cr6,r11,r10
			// li r11,1
			ctx.r11.s64 = 1;
			// beq cr6,0x823ed9c4
			if (ctx.r11.s32 == ctx.r10.s32) goto loc_823ED9C4;
		}
	loc_823ED9C0:
		// li r11,0
		ctx.r11.s64 = 0;
	loc_823ED9C4:
		// clrlwi r8,r11,24
		ctx.r8.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// bne cr6,0x823eda6c
		if (ctx.r8.u32 != 0) {
			// lwz r11,84(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
			// lbz r4,104(r31)
			ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 104);
			// addi r3,r11,1
			ctx.r3.s64 = ctx.r11.s64 + 1;
			// clrlwi r11,r4,25
			ctx.r11.u64 = ctx.r4.u32 & 0x7F;
			// stw r3,84(r31)
			PPC_STORE_U32(var_r31 + 84, ctx.r3.u32);
			// stb r11,104(r31)
			PPC_STORE_U8(var_r31 + 104, ctx.r11.u8);
			return;
		}
		// mr r4,r5
		ctx.r4.u64 = ctx.r5.u64;
		// bl 0x823ed418
		snSession_D418_w(ctx, base);
		// lwz r7,8(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 8);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// cmplw cr6,r7,r30
		// beq cr6,0x823eda6c
		if (ctx.r7.u32 == var_r30) {
			// lwz r11,84(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
			// lbz r4,104(r31)
			ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 104);
			// addi r3,r11,1
			ctx.r3.s64 = ctx.r11.s64 + 1;
			// clrlwi r11,r4,25
			ctx.r11.u64 = ctx.r4.u32 & 0x7F;
			// stw r3,84(r31)
			PPC_STORE_U32(var_r31 + 84, ctx.r3.u32);
			// stb r11,104(r31)
			PPC_STORE_U8(var_r31 + 104, ctx.r11.u8);
			return;
		}
		// lis r11,-32249
		ctx.r11.s64 = -2113470464;
		// addi r29,r11,16044
		var_r29 = (uint32_t)(ctx.r11.s64 + 16044);  // lbl_82073EAC @ 0x82073eac
	loc_823ED9F0:
		// lwz r3,8(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r5,8(r6)
		// bctrl
		VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		// mr r5,r3
		ctx.r5.u64 = ctx.r3.u64;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// lwz r3,8(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r11,60(r4)
		// bctrl
		VCALL(ctx.r3.u32, 15, ctx, base);  // vtable slot 15 (byte +60)
		// lwz r3,8(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r9,32(r10)
		// bctrl
		VCALL(ctx.r3.u32, 8, ctx, base);  // vtable slot 8 (byte +32)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823eddb8
		snSession_DDB8_w(ctx, base);
		// lwz r11,8(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r8,8(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// stw r11,12(r8)
		PPC_STORE_U32(ctx.r8.u32 + 12, ctx.r11.u32);
		// lwz r7,8(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r6,8(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
		// rotlwi r5,r6,0
		ctx.r5.u64 = ctx.r6.u32;
		// cmplw cr6,r5,r30
		// stw r6,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r6.u32);
		// bne cr6,0x823ed9f0
		if (ctx.r5.u32 != var_r30) goto loc_823ED9F0;
	}
loc_823EDA6C:
	// lwz r11,84(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
	// lbz r4,104(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 104);
	// addi r3,r11,1
	ctx.r3.s64 = ctx.r11.s64 + 1;
	// clrlwi r11,r4,25
	ctx.r11.u64 = ctx.r4.u32 & 0x7F;
	// stw r3,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r3.u32);
	// stb r11,104(r31)
	PPC_STORE_U8(var_r31 + 104, ctx.r11.u8);
	return;
}

__attribute__((alias("__imp__util_DA90"))) PPC_WEAK_FUNC(util_DA90);
PPC_FUNC_IMPL(__imp__util_DA90) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=144, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r11,r30
	ctx.r11.u64 = var_r30;
loc_823EDAA8:
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r9,8(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplw cr6,r11,r9
	// bne cr6,0x823edaa8
	if (ctx.r11.u32 != ctx.r9.u32) goto loc_823EDAA8;
	// rotlwi r8,r9,0
	ctx.r8.u64 = ctx.r9.u32;
	// lis r11,-32249
	// cmplw cr6,r8,r30
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, var_r30, ctx.xer);
	// addi r29,r11,16076
	var_r29 = (uint32_t)(ctx.r11.s64 + 16076);  // lbl_82073ECC @ 0x82073ecc
	// beq cr6,0x823edb40
while (ctx.r8.u32 != var_r30) {
	loc_823EDAD4:
		// lwz r7,8(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r3,12(r7)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 12);
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
		// lwz r5,8(r6)
		// bctrl
		VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		// mr r5,r3
		ctx.r5.u64 = ctx.r3.u64;
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// lwz r3,8(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r11,56(r4)
		// bctrl
		VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
		// lwz r3,8(r31)
		ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
		// lwz r9,32(r10)
		// bctrl
		VCALL(ctx.r3.u32, 8, ctx, base);  // vtable slot 8 (byte +32)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823edd10
		snSession_DD10_w(ctx, base);
		// lwz r8,8(r31)
		ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 8);
		// cmplw cr6,r8,r30
		// bne cr6,0x823edad4
}
loc_823EDB40:
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r6,28(r7)
	// bctrl
	VCALL(ctx.r3.u32, 7, ctx, base);  // vtable slot 7 (byte +28)
	// cmpwi cr6,r3,1
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x823edb68
	if (ctx.r3.s32 != 1) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EDB68:
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// beq cr6,0x823edc04
	if (ctx.r4.u32 == 0) goto loc_823EDC04;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r11,12(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// cmplwi cr6,r11,0
	// beq cr6,0x823edc04
	if (ctx.r11.u32 == 0) goto loc_823EDC04;
	// rotlwi r10,r3,0
	ctx.r10.u64 = ctx.r3.u32;
	// lwz r3,12(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// stw r3,8(r31)
	PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	// lwz r8,8(r9)
	// bctrl
	VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r6,56(r7)
	// bctrl
	VCALL(ctx.r3.u32, 14, ctx, base);  // vtable slot 14 (byte +56)
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r4,32(r5)
	// bctrl
	VCALL(ctx.r3.u32, 8, ctx, base);  // vtable slot 8 (byte +32)
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823edd10
	snSession_DD10_w(ctx, base);
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplw cr6,r30,r3
	// bne cr6,0x823edb40
	if (var_r30 != ctx.r3.u32) goto loc_823EDB40;
	// lwz r11,84(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
	// addi r8,r11,-1
	ctx.r8.s64 = ctx.r11.s64 + -1;
	// stw r8,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r8.u32);
	return;
loc_823EDC04:
	// lwz r11,84(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// lwz r30,88(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 88));
	// stw r11,88(r31)
	PPC_STORE_U32(var_r31 + 88, ctx.r11.u32);
	// lwz r9,52(r10)
	// bctrl
	VCALL(ctx.r3.u32, 13, ctx, base);  // vtable slot 13 (byte +52)
	// lwz r11,84(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 84);
	// stw r30,88(r31)
	PPC_STORE_U32(var_r31 + 88, var_r30);
	// addi r8,r11,-1
	ctx.r8.s64 = ctx.r11.s64 + -1;
	// stw r8,84(r31)
	PPC_STORE_U32(var_r31 + 84, ctx.r8.u32);
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_DC40_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_DC40_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_DC40_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=176, savegprlr_27
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// cmplwi cr6,r28,0
	// beq cr6,0x823edc90
	if (var_r28 != 0) {
		// lwz r11,0(r28)
  // [ph4a] vtable load collapsed
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// lwz r10,4(r11)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r28, 1, ctx, base);  // pattern-B slot 1 (byte +4)
		// lwz r9,0(r28)
  // [ph4a] vtable load collapsed
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// mr r3,r28
		ctx.r3.u64 = var_r28;
		// lwz r8,8(r9)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r28, 2, ctx, base);  // pattern-B slot 2 (byte +8)
		// mr r5,r3
		ctx.r5.u64 = ctx.r3.u64;
		// b 0x823edc9c
	} else {
	loc_823EDC90:
		// lis r11,-32249
		// li r31,-1
		var_r31 = (uint32_t)(-1);
		// addi r5,r11,16120
		ctx.r5.s64 = ctx.r11.s64 + 16120;
	}
loc_823EDC9C:
	// lis r11,-32249
	// mr r6,r31
	ctx.r6.u64 = var_r31;
	// addi r4,r11,16124
	ctx.r4.s64 = ctx.r11.s64 + 16124;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// li r27,0
	var_r27 = 0;
	// lwz r31,8(r30)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r30 + 8));
	// addi r29,r30,60
	var_r29 = (uint32_t)(var_r30 + 60);
	// stw r28,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r28);
	// cmplw cr6,r29,r31
	ctx.cr6.compare<uint32_t>(var_r29, var_r31, ctx.xer);
	// stb r27,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, (uint8_t)var_r27);
	// beq cr6,0x823edd00
while (var_r29 != var_r31) {
	loc_823EDCCC:
		// lwz r7,0(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
		// addi r5,r1,80
		ctx.r5.s64 = ctx.r1.s64 + 80;
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r6,48(r7)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + 48);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r6.u32);
		// lbz r4,80(r1)
		ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
		// cmplwi cr6,r4,0
		// bne cr6,0x823edd00
		if (ctx.r4.u32 != 0) {
			// stw r27,12(r30)
			PPC_STORE_U32(var_r30 + 12, var_r27);
			return;
		}
		// lwz r31,8(r31)
		var_r31 = (uint32_t)(PPC_LOAD_U32(var_r31 + 8));
		// cmplw cr6,r29,r31
		// bne cr6,0x823edccc
}
loc_823EDD00:
	// stw r27,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r27);
	return;
}

__attribute__((alias("__imp__snSession_DD10_w"))) PPC_WEAK_FUNC(snSession_DD10_w);
PPC_FUNC_IMPL(__imp__snSession_DD10_w) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,0
	// beqlr cr6
	if (ctx.r4.u32 == 0) return;
	// lbz r11,12(r4)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// beqlr cr6
	if (ctx.r10.u32 == 0) return;
	// addi r11,r3,92
	ctx.r11.s64 = ctx.r3.s64 + 92;
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r9,0
	// beq cr6,0x823edd80
	if (ctx.r9.u32 != 0) {
		// lwz r10,4(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// lwz r9,8(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		// cmplwi cr6,r9,0
		// stw r9,8(r4)
		PPC_STORE_U32(ctx.r4.u32 + 8, ctx.r9.u32);
		// beq cr6,0x823edd58
		if (ctx.r9.u32 != 0) {
			// rotlwi r8,r9,0
			ctx.r8.u64 = ctx.r9.u32;
			// stw r4,4(r8)
			PPC_STORE_U32(ctx.r8.u32 + 4, ctx.r4.u32);
			// b 0x823edd5c
		} else {
		loc_823EDD58:
			// stw r4,4(r11)
			PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r4.u32);
		}
	loc_823EDD5C:
		// stw r4,8(r10)
		PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r4.u32);
		// stw r10,4(r4)
		PPC_STORE_U32(ctx.r4.u32 + 4, ctx.r10.u32);
		// lwz r10,8(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// addi r7,r10,1
		ctx.r7.s64 = ctx.r10.s64 + 1;
		// stw r7,8(r11)
		PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r7.u32);
		// lbz r5,12(r4)
		ctx.r5.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
		// clrlwi r3,r5,25
		ctx.r3.u64 = ctx.r5.u32 & 0x7F;
		// stb r3,12(r4)
		PPC_STORE_U8(ctx.r4.u32 + 12, ctx.r3.u8);
		// blr
		return;
	}
loc_823EDD80:
	// addi r10,r4,4
	ctx.r10.s64 = ctx.r4.s64 + 4;
	// stw r4,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r4.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r4,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r4.u32);
	// stw r9,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r9.u32);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r6,r10,1
	ctx.r6.s64 = ctx.r10.s64 + 1;
	// stw r6,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r6.u32);
	// lbz r5,12(r4)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
	// clrlwi r3,r5,25
	ctx.r3.u64 = ctx.r5.u32 & 0x7F;
	// stb r3,12(r4)
	PPC_STORE_U8(ctx.r4.u32 + 12, ctx.r3.u8);
	// blr
	return;
}

__attribute__((alias("__imp__snSession_DDB8_w"))) PPC_WEAK_FUNC(snSession_DDB8_w);
PPC_FUNC_IMPL(__imp__snSession_DDB8_w) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,0
	// beqlr cr6
	if (ctx.r4.u32 == 0) return;
	// lbz r11,12(r4)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	// bnelr cr6
	if (ctx.r10.u32 != 0) return;
	// addi r11,r4,4
	ctx.r11.s64 = ctx.r4.s64 + 4;
	// addi r10,r3,92
	ctx.r10.s64 = ctx.r3.s64 + 92;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r9,0
	// bne cr6,0x823ede10
	if (ctx.r9.u32 == 0) {
		// lwz r9,4(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r9,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
		// lwz r7,4(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// cmplwi cr6,r7,0
		// beq cr6,0x823ede08
		if (ctx.r7.u32 != 0) {
			// rotlwi r6,r7,0
			ctx.r6.u64 = ctx.r7.u32;
			// stw r8,4(r6)
			PPC_STORE_U32(ctx.r6.u32 + 4, ctx.r8.u32);
			// b 0x823ede3c
			// stw r8,4(r11)
			PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r8.u32);
			// stw r8,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
			// lwz r11,8(r10)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
			// addi r5,r11,-1
			ctx.r5.s64 = ctx.r11.s64 + -1;
			// stw r5,8(r10)
			PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r5.u32);
			// lbz r3,12(r4)
			ctx.r3.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
			// ori r11,r3,128
			ctx.r11.u64 = ctx.r3.u64 | 128;
			// stb r11,12(r4)
			PPC_STORE_U8(ctx.r4.u32 + 12, ctx.r11.u8);
			// blr
			return;
		}
	loc_823EDE08:
		// stw r8,4(r10)
		PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
		// b 0x823ede3c
	} else {
	loc_823EDE10:
		// lwz r5,4(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r5,8(r9)
		PPC_STORE_U32(ctx.r9.u32 + 8, ctx.r5.u32);
		// lwz r3,4(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// cmplwi cr6,r3,0
		// beq cr6,0x823ede34
		if (ctx.r3.u32 != 0) {
			// lwz r9,0(r11)
			ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// rotlwi r7,r3,0
			ctx.r7.u64 = ctx.r3.u32;
			// stw r9,4(r7)
			PPC_STORE_U32(ctx.r7.u32 + 4, ctx.r9.u32);
			// b 0x823ede3c
		} else {
		loc_823EDE34:
			// lwz r6,0(r11)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
			// stw r6,4(r10)
			PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r6.u32);
		}
	}
loc_823EDE3C:
	// stw r8,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r8.u32);
	// stw r8,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// lwz r11,8(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// addi r5,r11,-1
	ctx.r5.s64 = ctx.r11.s64 + -1;
	// stw r5,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r5.u32);
	// lbz r3,12(r4)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r4.u32 + 12);
	// ori r11,r3,128
	ctx.r11.u64 = ctx.r3.u64 | 128;
	// stb r11,12(r4)
	PPC_STORE_U8(ctx.r4.u32 + 12, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_DE60_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_DE60_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_DE60_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r4,0(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823edf84
	if (ctx.r11.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// lwz r4,4(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 4)/* SinglesNetworkClient::flags@+0x4 */;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x820d68a8
		SinglesNetworkClient_68A8_g(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823edf84
		if (ctx.r10.u32 == 0) goto loc_823EDF84;
		// li r5,32
		ctx.r5.s64 = 32;
		// lwz r4,8(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 8);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x820d68a8
		SinglesNetworkClient_68A8_g(ctx, base);
		// clrlwi r9,r3,24
		ctx.r9.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r9,0
		// beq cr6,0x823edf84
		if (ctx.r9.u32 == 0) goto loc_823EDF84;
		// li r5,32
		ctx.r5.s64 = 32;
		// lwz r4,12(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 12);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x820d68a8
		SinglesNetworkClient_68A8_g(ctx, base);
		// clrlwi r8,r3,24
		ctx.r8.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// beq cr6,0x823edf84
		if (ctx.r8.u32 == 0) goto loc_823EDF84;
		// li r5,16
		ctx.r5.s64 = 16;
		// addi r4,r30,16
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 16;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x821d9ad8
		SinglesNetworkClient_9AD8_g(ctx, base);
		// cmpwi cr6,r3,0
		// beq cr6,0x823edf84
		if (ctx.r3.s32 == 0) goto loc_823EDF84;
		// li r5,96
		ctx.r5.s64 = 96;
		// addi r4,r30,32
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 32;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x820d67c8
		SinglesNetworkClient_67C8_g(ctx, base);
		// cmplwi cr6,r3,96
		// bne cr6,0x823edf84
		if (ctx.r3.u32 != 96) goto loc_823EDF84;
		// addi r29,r30,128
		var_r29 = (uint32_t)(var_r30 + 128);
		// li r28,0
		var_r28 = 0;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x82372160
		SinglesNetworkClient_2160_g(ctx, base);
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// li r5,32
		ctx.r5.s64 = 32;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x820d6838
		SinglesNetworkClient_6838_g(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x823edf74
		if (ctx.r7.u32 != 0) {
			// li r28,1
			var_r28 = 1;
			// cmplwi cr6,r30,0
			// beq cr6,0x823edf74
			if (var_r30 == 0) goto loc_823EDF74;
			// mr r5,r30
			ctx.r5.u64 = var_r30;
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// mr r3,r31
			ctx.r3.u64 = var_r31;
			// bl 0x820d67c8
			SinglesNetworkClient_67C8_g(ctx, base);
			// cmplw cr6,r30,r3
			// li r11,1
			ctx.r11.s64 = 1;
			// beq cr6,0x823edf70
			if (var_r30 != ctx.r3.u32) {
				// li r11,0
				ctx.r11.s64 = 0;
			}
		loc_823EDF70:
			// clrlwi r28,r11,24
			var_r28 = (uint32_t)(ctx.r11.u32 & 0xFF);
		}
	loc_823EDF74:
		// clrlwi r6,r28,24
		ctx.r6.u64 = var_r28 & 0xFF;
		// li r11,1
		ctx.r11.s64 = 1;
		// cmplwi cr6,r6,0
		// bne cr6,0x823edf88
		if (ctx.r6.u32 != 0) {
			// clrlwi r3,r11,24
			ctx.r3.u64 = ctx.r11.u32 & 0xFF;
			return;
		}
	}
loc_823EDF84:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823EDF88:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_DF98_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_DF98_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_DF98_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=160, savegprlr_25
	// mr r31,r5
	var_r31 = ctx.r5.u32;
	// li r25,0
	var_r25 = 0;
	// addi r27,r31,8
	var_r27 = (uint32_t)(var_r31 + 8);
	// addi r26,r31,12
	var_r26 = (uint32_t)(var_r31 + 12);
	// addi r29,r31,4
	var_r29 = (uint32_t)(var_r31 + 4);
	// addi r28,r31,32
	var_r28 = (uint32_t)(var_r31 + 32);
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r25,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* SinglesNetworkClient::vtable@+0x0 */ var_r25);
	// mr r9,r25
	ctx.r9.u64 = var_r25;
	// stw r25,0(r27)
	PPC_STORE_U32(var_r27 + 0, var_r25);
	// li r10,15
	ctx.r10.s64 = 15;
	// stw r25,0(r26)
	PPC_STORE_U32(var_r26 + 0, var_r25);
	// stw r25,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ var_r25);
	// mr r11,r28
	ctx.r11.u64 = var_r28;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823EDFE0:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823edfe0
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823EDFE0;
	// addi r11,r28,60
	ctx.r11.s64 = (int64_t)(int32_t)var_r28 + 60;
	// mr r9,r25
	ctx.r9.u64 = var_r25;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823EDFFC:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823edffc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823EDFFC;
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82260e18
	SinglesNetworkClient_0E18_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// beq cr6,0x823ee120
	if (ctx.r11.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// mr r4,r29
		ctx.r4.u64 = var_r29;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82260e18
		SinglesNetworkClient_0E18_g(ctx, base);
		// clrlwi r10,r3,24
		ctx.r10.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r10,0
		// beq cr6,0x823ee120
		if (ctx.r10.u32 == 0) {
			// clrlwi r3,r25,24
			ctx.r3.u64 = var_r25 & 0xFF;
			return;
		}
		// li r5,32
		ctx.r5.s64 = 32;
		// mr r4,r27
		ctx.r4.u64 = var_r27;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82260e18
		SinglesNetworkClient_0E18_g(ctx, base);
		// clrlwi r9,r3,24
		ctx.r9.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r9,0
		// beq cr6,0x823ee120
		if (ctx.r9.u32 == 0) {
			// clrlwi r3,r25,24
			ctx.r3.u64 = var_r25 & 0xFF;
			return;
		}
		// li r5,32
		ctx.r5.s64 = 32;
		// mr r4,r26
		ctx.r4.u64 = var_r26;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82260e18
		SinglesNetworkClient_0E18_g(ctx, base);
		// clrlwi r8,r3,24
		ctx.r8.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r8,0
		// beq cr6,0x823ee120
		if (ctx.r8.u32 == 0) {
			// clrlwi r3,r25,24
			ctx.r3.u64 = var_r25 & 0xFF;
			return;
		}
		// li r5,16
		ctx.r5.s64 = 16;
		// addi r4,r31,16
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 16;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82260a70
		SinglesNetworkClient_0A70_g(ctx, base);
		// cmpwi cr6,r3,0
		// beq cr6,0x823ee120
		if (ctx.r3.s32 == 0) {
			// clrlwi r3,r25,24
			ctx.r3.u64 = var_r25 & 0xFF;
			return;
		}
		// li r5,96
		ctx.r5.s64 = 96;
		// mr r4,r28
		ctx.r4.u64 = var_r28;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82260af0
		util_0AF0(ctx, base);
		// cmplwi cr6,r3,96
		// bne cr6,0x823ee120
		if (ctx.r3.u32 != 96) {
			// clrlwi r3,r25,24
			ctx.r3.u64 = var_r25 & 0xFF;
			return;
		}
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// mr r28,r25
		var_r28 = (uint32_t)(var_r25);
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x823ee104
		if (ctx.r7.u32 != 0) {
			// lwz r29,80(r1)
			var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 80));
			// li r28,1
			var_r28 = 1;
			// cmplwi cr6,r29,0
			// beq cr6,0x823ee104
			if (var_r29 == 0) goto loc_823EE104;
			// cmplwi cr6,r29,256
			// bgt cr6,0x823ee0fc
			if (var_r29 <= 256) {
				// mr r5,r29
				ctx.r5.u64 = var_r29;
				// addi r4,r31,128
				ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 128;
				// mr r3,r30
				ctx.r3.u64 = var_r30;
				// bl 0x82260af0
				util_0AF0(ctx, base);
				// cmplw cr6,r29,r3
				// li r11,1
				ctx.r11.s64 = 1;
				// beq cr6,0x823ee100
				if (var_r29 == ctx.r3.u32) goto loc_823EE100;
			}
		loc_823EE0FC:
			// mr r11,r25
			ctx.r11.u64 = var_r25;
		loc_823EE100:
			// clrlwi r28,r11,24
			var_r28 = (uint32_t)(ctx.r11.u32 & 0xFF);
		}
	loc_823EE104:
		// clrlwi r6,r28,24
		ctx.r6.u64 = var_r28 & 0xFF;
		// cmplwi cr6,r6,0
		// beq cr6,0x823ee120
		if (ctx.r6.u32 == 0) {
			// clrlwi r3,r25,24
			ctx.r3.u64 = var_r25 & 0xFF;
			return;
		}
		// li r11,1
		ctx.r11.s64 = 1;
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		return;
	}
loc_823EE120:
	// clrlwi r3,r25,24
	ctx.r3.u64 = var_r25 & 0xFF;
	return;
}

__attribute__((alias("__imp__EvtFindSessionCancel_vfn_1"))) PPC_WEAK_FUNC(EvtFindSessionCancel_vfn_1);
PPC_FUNC_IMPL(__imp__EvtFindSessionCancel_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6508(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6508);  /* glob:lbl_825D196C @ 0x825d196c */
	// blr
	return;
}

__attribute__((alias("__imp__EvtFindSessionCancel_vfn_2"))) PPC_WEAK_FUNC(EvtFindSessionCancel_vfn_2);
PPC_FUNC_IMPL(__imp__EvtFindSessionCancel_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16320
	ctx.r3.s64 = ctx.r11.s64 + 16320;
	// blr
	return;
}

__attribute__((alias("__imp__EvtFindSessionSyslink_vfn_1"))) PPC_WEAK_FUNC(EvtFindSessionSyslink_vfn_1);
PPC_FUNC_IMPL(__imp__EvtFindSessionSyslink_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6376(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6376);  /* glob:lbl_825D18E8 @ 0x825d18e8 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtFindSessionSyslink_vfn_2"))) PPC_WEAK_FUNC(EvtFindSessionSyslink_vfn_2);
PPC_FUNC_IMPL(__imp__EvtFindSessionSyslink_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16344
	ctx.r3.s64 = ctx.r11.s64 + 16344;
	// blr
	return;
}

__attribute__((alias("__imp__EvtJoin_vfn_0"))) PPC_WEAK_FUNC(EvtJoin_vfn_0);
PPC_FUNC_IMPL(__imp__EvtJoin_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,20
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 20;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32249
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// cmplwi cr6,r10,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823ee1b8
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EE1B8:
	// blr
	return;
}

__attribute__((alias("__imp__EvtFindSessionOnline_vfn_1"))) PPC_WEAK_FUNC(EvtFindSessionOnline_vfn_1);
PPC_FUNC_IMPL(__imp__EvtFindSessionOnline_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,6388(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6388);  /* glob:lbl_825D18F4 @ 0x825d18f4 */
	// blr
	return;
}

__attribute__((alias("__imp__EvtFindSessionOnline_vfn_2"))) PPC_WEAK_FUNC(EvtFindSessionOnline_vfn_2);
PPC_FUNC_IMPL(__imp__EvtFindSessionOnline_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16368
	ctx.r3.s64 = ctx.r11.s64 + 16368;
	// blr
	return;
}

__attribute__((alias("__imp__snNotifySyslinkRequest_rtti_4100_0"))) PPC_WEAK_FUNC(snNotifySyslinkRequest_rtti_4100_0);
PPC_FUNC_IMPL(__imp__snNotifySyslinkRequest_rtti_4100_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,108
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 108;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32249
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// addi r11,r11,10780
	ctx.r11.s64 = ctx.r11.s64 + 10780;
	// cmplwi cr6,r10,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823ee240
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EE240:
	// blr
	return;
}

__attribute__((alias("__imp__snNotifyFoundSyslink_rtti_410C_0"))) PPC_WEAK_FUNC(snNotifyFoundSyslink_rtti_410C_0);
PPC_FUNC_IMPL(__imp__snNotifyFoundSyslink_rtti_410C_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,4
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 4;
	// bl 0x82260268
	SinglesNetworkClient_0268_g(ctx, base);
	// lis r11,-32249
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// addi r11,r11,10780
	ctx.r11.s64 = ctx.r11.s64 + 10780;
	// cmplwi cr6,r10,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823ee2a0
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EE2A0:
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_vfn_2"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_vfn_2);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16392
	ctx.r3.s64 = ctx.r11.s64 + 16392;
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_vfn_12"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_vfn_12);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSessionFinder_slfRoot::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6376(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6376);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823ee358
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r5,44(r6)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSessionFinder_slfRoot::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,48
		var_r29 = (uint32_t)(var_r31 + 48);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r11,40(r4)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSessionFinder_slfRoot::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r9,4(r10)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6388(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6388);
	// cmpw cr6,r3,r11
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bne cr6,0x823ee3cc
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r7,44(r8)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSessionFinder_slfRoot::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// addi r29,r31,120
		var_r29 = (uint32_t)(var_r31 + 120);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
	// lwz r5,40(r6)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSessionFinder_slfRoot::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r11,4(r4)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,6508(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 6508);
	// cmpw cr6,r3,r11
	// bne cr6,0x823ee440
	if (ctx.r3.s32 == ctx.r11.s32) {
		// lwz r10,0(r31)
  // [ph4a] vtable load collapsed
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(var_r31, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// addi r29,r31,144
		var_r29 = (uint32_t)(var_r31 + 144);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823EE440:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_vfn_2"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_vfn_2);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16412
	ctx.r3.s64 = ctx.r11.s64 + 16412;
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_vfn_14"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_vfn_14);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_vfn_14) {
	PPC_FUNC_PROLOGUE();
	// lis r11,1
	ctx.r11.s64 = 65536;
	// lwz r8,16(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r11,r11,3360
	ctx.r11.u64 = ctx.r11.u64 | 3360;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// add r11,r8,r11
	ctx.r11.u64 = ctx.r8.u64 + ctx.r11.u64;
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// ori r6,r7,128
	ctx.r6.u64 = ctx.r7.u64 | 128;
	// stb r6,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r6.u8);
	// lwz r5,16(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// add r11,r5,r10
	ctx.r11.u64 = ctx.r5.u64 + ctx.r10.u64;
	// lbz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// andi. r10,r4,223
	ctx.r10.u64 = ctx.r4.u64 & 223;
	ctx.cr0.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// lwz r8,16(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// stw r9,2320(r8)
	PPC_STORE_U32(ctx.r8.u32 + 2320, ctx.r9.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_vfn_15"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_vfn_15);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_vfn_15) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// addi r11,r11,3360
	ctx.r11.s64 = ctx.r11.s64 + 3360;
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// clrlwi r9,r10,25
	ctx.r9.u64 = ctx.r10.u32 & 0x7F;
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_2"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_2);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16424
	ctx.r3.s64 = ctx.r11.s64 + 16424;
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_14"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_14);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r10,40(r11)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSessionFinder_slfRoot_slfSearching_slfFindingSyslink::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// addi r6,r11,22332
	ctx.r6.s64 = ctx.r11.s64 + 22332;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// lwz r8,12(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,16(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r30,r3,20
	var_r30 = (uint32_t)(ctx.r3.s64 + 20);
	// li r9,0
	ctx.r9.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r8,2324(r7)
	PPC_STORE_U32(ctx.r7.u32 + 2324, ctx.r8.u32);
	// lwz r5,16(r3)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r6,16(r31)
	ctx.r6.u64 = PPC_LOAD_U32(var_r31 + 16);
	// stw r5,2332(r6)
	PPC_STORE_U32(ctx.r6.u32 + 2332, ctx.r5.u32);
	// lwz r3,16(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 16);
	// stw r9,2328(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2328, ctx.r9.u32);
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r31,r11,2344
	var_r31 = (uint32_t)(ctx.r11.s64 + 2344);  // addr:0x825e0928
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x822603d0
	snJoinMachine_03D0_g(ctx, base);
	// cmplwi cr6,r30,0
	// beq cr6,0x823ee578
	if (var_r30 != 0) {
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r6,16(r30)
		ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 16);
		// lwz r5,24(r30)
		ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 24);
		// lwz r4,4(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 4);
		// bl 0x822608c8
		SinglesNetworkClient_08C8_g(ctx, base);
	}
loc_823EE578:
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_2"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_2);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16440
	ctx.r3.s64 = ctx.r11.s64 + 16440;
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_14"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_14);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r10,2328(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 2328);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,2328(r11)
	PPC_STORE_U32(ctx.r11.u32 + 2328, ctx.r10.u32);
	// lwz r9,16(r31)
	ctx.r9.u64 = PPC_LOAD_U32(var_r31 + 16);
	// lwz r4,2324(r9)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r9.u32 + 2324);
	// bl 0x823ed4f8
	util_D4F8(ctx, base);
	// bl 0x82416650
	util_6650(ctx, base);
	// lwz r8,16(r31)
	ctx.r8.u64 = PPC_LOAD_U32(var_r31 + 16);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// std r11,2336(r8)
	PPC_STORE_U64(ctx.r8.u32 + 2336, ctx.r11.u64);
	// bl 0x823ef530
	SinglesNetworkClient_F530_g(ctx, base);
	// lwz r7,16(r31)
	ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r3,328(r7)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r7.u32 + 328);
	// bl 0x82413840
	util_3840(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_15"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_15);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_15) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r31,r3,24
	var_r31 = (uint32_t)(ctx.r3.s64 + 24);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lwz r11,328(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 328);
	// addi r3,r11,464
	ctx.r3.s64 = ctx.r11.s64 + 464;
	// bl 0x823b3d80
	NotifyHandler_3D80_g(ctx, base);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r10.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_12"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_12);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_12) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r30,r5
	var_r30 = ctx.r5.u32;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// stb r11,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r11.u8);
	// lwz r9,40(r10)
	// bctrl
	VCALL(ctx.r3.u32, 10, ctx, base);  // snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting::vfn_10 (unnamed)  // vtable slot 10 (byte +40)
	// lwz r3,12(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 12);
	// lwz r7,4(r8)
	// bctrl
	VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
	// cmpwi cr6,r3,0
	// bne cr6,0x823ee730
	if (ctx.r3.s32 == 0) {
		// lwz r11,16(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r6,2328(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 2328);
		// lwz r5,2332(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 2332);
		// cmpw cr6,r6,r5
		// bge cr6,0x823ee6f0
		if (ctx.r6.s32 < ctx.r5.s32) {
			// lwz r11,44(r4)
			// bctrl
			VCALL(ctx.r3.u32, 11, ctx, base);  // snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
			// mr r5,r31
			ctx.r5.u64 = var_r31;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// mr r30,r3
			var_r30 = ctx.r3.u32;
			// bl 0x823ed988
			util_D988(ctx, base);
			// mr r5,r31
			ctx.r5.u64 = var_r31;
			// mr r4,r31
			ctx.r4.u64 = var_r31;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x823eda90
			util_DA90(ctx, base);
			return;
		}
		// lwz r9,44(r10)
		// bctrl
		VCALL(ctx.r3.u32, 11, ctx, base);  // snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting::vfn_11 (unnamed)  // vtable slot 11 (byte +44)
		// lwz r11,20(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 20);
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// addi r29,r11,144
		var_r29 = (uint32_t)(ctx.r11.s64 + 144);  // lbl_82070090 @ 0x82070090
		// mr r30,r3
		var_r30 = ctx.r3.u32;
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// bl 0x823ed988
		util_D988(ctx, base);
		// mr r5,r29
		ctx.r5.u64 = var_r29;
		// mr r4,r31
		ctx.r4.u64 = var_r31;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823eda90
		util_DA90(ctx, base);
		return;
	}
loc_823EE730:
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r8,0(r30)
	PPC_STORE_U8(var_r30 + 0, ctx.r8.u8);
	return;
}

__attribute__((alias("__imp__snSession_snRoot_snActive_snIdle_vfn_5"))) PPC_WEAK_FUNC(snSession_snRoot_snActive_snIdle_vfn_5);
PPC_FUNC_IMPL(__imp__snSession_snRoot_snActive_snIdle_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// addi r4,r3,24
	ctx.r4.s64 = ctx.r3.s64 + 24;
	// b 0x823ef850
	util_F850(ctx, base);
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_2"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_2);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16456
	ctx.r3.s64 = ctx.r11.s64 + 16456;
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_14"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_14);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// lwz r11,16(r29)
	ctx.r11.u64 = PPC_LOAD_U32(var_r29 + 16);
	// addi r7,r11,2480
	ctx.r7.s64 = ctx.r11.s64 + 2480;
	// addi r11,r11,2452
	ctx.r11.s64 = ctx.r11.s64 + 2452;
	// addi r10,r7,4
	ctx.r10.s64 = ctx.r7.s64 + 4;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmplwi cr6,r9,0
	// beq cr6,0x823ee7c0
	if (ctx.r9.u32 != 0) {
		// lwz r9,4(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
		// lwz r8,16(r9)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 16);
		// cmplwi cr6,r8,0
		// stw r8,16(r11)
		PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r8.u32);
		// beq cr6,0x823ee7a4
		if (ctx.r8.u32 != 0) {
			// rotlwi r8,r8,0
			ctx.r8.u64 = ctx.r8.u32;
			// stw r11,12(r8)
			PPC_STORE_U32(ctx.r8.u32 + 12, ctx.r11.u32);
			// b 0x823ee7a8
		} else {
		loc_823EE7A4:
			// stw r11,4(r10)
			PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
		}
	loc_823EE7A8:
		// stw r11,16(r9)
		PPC_STORE_U32(ctx.r9.u32 + 16, ctx.r11.u32);
		// stw r9,12(r11)
		PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r9.u32);
		// lwz r9,8(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		// addi r6,r9,1
		ctx.r6.s64 = ctx.r9.s64 + 1;
		// stw r6,8(r10)
		PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r6.u32);
		// b 0x823ee7e4
	} else {
	loc_823EE7C0:
		// addi r9,r11,12
		ctx.r9.s64 = ctx.r11.s64 + 12;
		// stw r11,0(r10)
		PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
		// li r8,0
		ctx.r8.s64 = 0;
		// stw r11,4(r10)
		PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
		// stw r8,4(r9)
		PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r8.u32);
		// stw r8,0(r9)
		PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
		// lwz r9,8(r10)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		// addi r5,r9,1
		ctx.r5.s64 = ctx.r9.s64 + 1;
		// stw r5,8(r10)
		PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r5.u32);
	}
loc_823EE7E4:
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// lwz r31,16(r29)
	var_r31 = (uint32_t)(PPC_LOAD_U32(var_r29 + 16));
	// addi r30,r31,2480
	var_r30 = (uint32_t)(var_r31 + 2480);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82425020
	xam_5020_g(ctx, base);
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 12);
	// addi r6,r11,22292
	ctx.r6.s64 = ctx.r11.s64 + 22292;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21576
	ctx.r5.s64 = ctx.r11.s64 + 21576;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// addi r6,r11,24
	ctx.r6.s64 = ctx.r11.s64 + 24;
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,280(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 280);
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,12(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x824250e0
	xam_50E0_g(ctx, base);
	// clrlwi r4,r3,24
	ctx.r4.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r4,0
	// bne cr6,0x823ee8c4
	if (ctx.r4.u32 == 0) {
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// bl 0x823dda08
		util_DA08(ctx, base);
		// lis r11,-32249
		// lwz r10,0(r29)
  // [ph4a] vtable load collapsed
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// addi r31,r11,16580
		var_r31 = (uint32_t)(ctx.r11.s64 + 16580);  // lbl_820740C4 @ 0x820740c4
		// lwz r9,44(r10)
  // [ph4a] slot load collapsed
		// stw r31,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, var_r31);
		// bctrl
		VCALL(var_r29, 11, ctx, base);  // pattern-B slot 11 (byte +44)
		// lwz r30,56(r3)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r3.u32 + 56));
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,12
		ctx.r4.s64 = 12;
		// lwz r3,4(r30)
		ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 4);
		// lwz r7,4(r8)
		// bctrl
		VCALL(ctx.r3.u32, 1, ctx, base);  // vtable slot 1 (byte +4)
		// mr r4,r3
		ctx.r4.u64 = ctx.r3.u64;
		// cmplwi cr6,r4,0
		// beq cr6,0x823ee8c4
		if (ctx.r4.u32 == 0) {
			return;
		}
		// lis r11,-32249
		// addi r3,r30,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 8;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
		// lwz r6,84(r1)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		// lwz r11,88(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
		// rldicr r5,r6,32,63
		ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u64, 32);
		// stw r31,0(r4)
		PPC_STORE_U32(ctx.r4.u32 + 0, var_r31);
		// or r10,r11,r5
		ctx.r10.u64 = ctx.r11.u64 | ctx.r5.u64;
		// std r10,4(r4)
		PPC_STORE_U64(ctx.r4.u32 + 4, ctx.r10.u64);
		// bl 0x823ec068
		snSession_AddNode_C068(ctx, base);
	}
loc_823EE8C4:
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_15"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_15);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingOnline_vfn_15) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// addi r31,r11,2452
	var_r31 = (uint32_t)(ctx.r11.s64 + 2452);  // addr:0x82070994
	// addi r3,r11,2480
	ctx.r3.s64 = ctx.r11.s64 + 2480;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// bl 0x823b3d80
	NotifyHandler_3D80_g(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,20(r31)
	PPC_STORE_U32(var_r31 + 20, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_vfn_5"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_vfn_5);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,96
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 96;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_vfn_6"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_vfn_6);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823ee990
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823EE990:
	// addi r3,r31,96
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 96;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823ee9b4
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823EE9B4:
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfFinal_vfn_2"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfFinal_vfn_2);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfFinal_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,16472
	ctx.r3.s64 = ctx.r11.s64 + 16472;
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfFinal_vfn_14"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfFinal_vfn_14);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfFinal_vfn_14) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 16);
	// addi r3,r11,2480
	ctx.r3.s64 = ctx.r11.s64 + 2480;
	// bl 0x82425648
	util_5648(ctx, base);
	// lis r11,1
	ctx.r11.s64 = 65536;
	// lwz r4,16(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 16);
	// ori r10,r11,3360
	ctx.r10.u64 = ctx.r11.u64 | 3360;
	// lbzx r9,r4,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r4.u32 + ctx.r10.u32);
	// rlwinm r8,r9,0,26,26
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r8,0
	// bne cr6,0x823eea2c
	if (ctx.r8.u32 == 0) {
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,3
		ctx.r5.s64 = 3;
		// addi r3,r4,108
		ctx.r3.s64 = ctx.r4.s64 + 108;
		// bl 0x82371bf8
		snSession_1BF8_g(ctx, base);
	}
loc_823EEA2C:
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_vfn_5"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_vfn_5);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_vfn_5) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r4,r31,24
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823ef850
	util_F850(ctx, base);
	// addi r4,r31,144
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 144;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823ef850
	util_F850(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_vfn_6"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_vfn_6);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_vfn_6) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r30,0
	var_r30 = 0;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823eeac0
	if (ctx.r11.u32 != 0) {
		// lwz r10,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r9,16(r10)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823EEAC0:
	// addi r3,r31,144
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 144;
	// lwz r8,8(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// cmplwi cr6,r8,0
	// beq cr6,0x823eeae4
	if (ctx.r8.u32 != 0) {
		// lwz r7,0(r3)
  // [ph4a] vtable load collapsed
		// stw r30,8(r3)
		PPC_STORE_U32(ctx.r3.u32 + 8, var_r30);
		// lwz r6,16(r7)
  // [ph4a] slot load collapsed
		// bctrl
		VCALL(ctx.r3.u32, 4, ctx, base);  // pattern-B slot 4 (byte +16)
	}
loc_823EEAE4:
	// blr
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_0"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_0);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_slfBroadcasting_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823a9bf8
	util_9BF8(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823eeb38
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EEB38:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_EB50_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_EB50_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_EB50_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	// FRAME: size=128, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// bl 0x823ed570
	hsmContext_ctor_D570(ctx, base);
	// lis r10,-32249
	// lis r9,-32249
	// addi r10,r10,16676
	ctx.r10.s64 = ctx.r10.s64 + 16676;
	// addi r11,r30,108
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 108;
	// li r31,0
	var_r31 = 0;
	// addi r9,r9,-6416
	ctx.r9.s64 = ctx.r9.s64 + -6416;
	// addi r3,r30,136
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 136;
	// stw r10,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, var_r31);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// bl 0x823da620
	SinglesNetworkClient_A620_w(ctx, base);
	// addi r3,r30,160
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 160;
	// bl 0x823eed80
	SinglesNetworkClient_ED80_w(ctx, base);
	// addi r3,r30,332
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 332;
	// stw r31,328(r30)
	PPC_STORE_U32(var_r30 + 328, var_r31);
	// lis r11,-32252
	// li r10,256
	ctx.r10.s64 = 256;
	// addi r11,r11,-29620
	ctx.r11.s64 = ctx.r11.s64 + -29620;
	// li r5,256
	ctx.r5.s64 = 256;
	// lbz r9,192(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 192);
	// addi r4,r30,528
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 528;
	// stb r31,184(r3)
	PPC_STORE_U8(ctx.r3.u32 + 184, (uint8_t)var_r31);
	// clrlwi r8,r9,25
	ctx.r8.u64 = ctx.r9.u32 & 0x7F;
	// stb r31,185(r3)
	PPC_STORE_U8(ctx.r3.u32 + 185, (uint8_t)var_r31);
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r10,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r10.u32);
	// stb r8,192(r3)
	PPC_STORE_U8(ctx.r3.u32 + 192, ctx.r8.u8);
	// bl 0x82187ae8
	util_7AE8(ctx, base);
	// addi r7,r30,784
	ctx.r7.s64 = (int64_t)(int32_t)var_r30 + 784;
	// li r6,15
	ctx.r6.s64 = 15;
	// addi r8,r7,60
	ctx.r8.s64 = ctx.r7.s64 + 60;
loc_823EEBF4:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// mr r9,r31
	ctx.r9.u64 = var_r31;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823EEC04:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823eec04
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823EEC04;
	// mr r11,r7
	ctx.r11.u64 = ctx.r7.u64;
	// mr r9,r31
	ctx.r9.u64 = var_r31;
	// li r10,15
	ctx.r10.s64 = 15;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823EEC20:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823eec20
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823EEC20;
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// mr r9,r31
	ctx.r9.u64 = var_r31;
	// li r10,9
	ctx.r10.s64 = 9;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_823EEC3C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x823eec3c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823EEC3C;
	// addi r6,r6,-1
	ctx.r6.s64 = ctx.r6.s64 + -1;
	// addi r7,r7,96
	ctx.r7.s64 = ctx.r7.s64 + 96;
	// addi r8,r8,96
	ctx.r8.s64 = ctx.r8.s64 + 96;
	// cmpwi cr6,r6,0
	// bge cr6,0x823eebf4
	if (ctx.r6.s32 >= 0) goto loc_823EEBF4;
	// addi r3,r30,2344
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 2344;
	// stw r31,2324(r30)
	PPC_STORE_U32(var_r30 + 2324, var_r31);
	// stw r31,2328(r30)
	PPC_STORE_U32(var_r30 + 2328, var_r31);
	// stw r31,2332(r30)
	PPC_STORE_U32(var_r30 + 2332, var_r31);
	// std r31,2336(r30)
	PPC_STORE_U64(var_r30 + 2336, var_r31);
	// bl 0x823b3fd8
	SinglesNetworkClient_3FD8_g(ctx, base);
	// lis r11,-32249
	// stw r31,2448(r30)
	PPC_STORE_U32(var_r30 + 2448, var_r31);
	// addi r29,r30,2452
	var_r29 = (uint32_t)(var_r30 + 2452);
	// addi r11,r11,-6440
	ctx.r11.s64 = ctx.r11.s64 + -6440;
	// addi r28,r29,12
	var_r28 = (uint32_t)(var_r29 + 12);
	// mr r3,r28
	ctx.r3.u64 = var_r28;
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r11,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r27,r28,4
	var_r27 = (uint32_t)(var_r28 + 4);
	// mr r3,r27
	ctx.r3.u64 = var_r27;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32248
	// addi r3,r30,2480
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 2480;
	// stw r31,0(r27)
	PPC_STORE_U32(var_r27 + 0, var_r31);
	// addi r11,r11,-27540
	ctx.r11.s64 = ctx.r11.s64 + -27540;
	// stw r31,0(r28)
	PPC_STORE_U32(var_r28 + 0, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r31,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, var_r31);
	// stw r31,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, var_r31);
	// stw r31,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, var_r31);
	// stw r31,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, var_r31);
	// stw r31,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, var_r31);
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// stw r31,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, var_r31);
	// stw r31,32(r3)
	PPC_STORE_U32(ctx.r3.u32 + 32, var_r31);
	// stw r31,840(r3)
	PPC_STORE_U32(ctx.r3.u32 + 840, var_r31);
	// bl 0x82425020
	xam_5020_g(ctx, base);
	// addis r11,r30,1
	ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 65536;
	// lis r9,-32193
	// addi r11,r11,3360
	ctx.r11.s64 = ctx.r11.s64 + 3360;
	// lis r10,-32193
	// addi r7,r9,-1648
	ctx.r7.s64 = ctx.r9.s64 + -1648;
	// addi r6,r10,-1640
	ctx.r6.s64 = ctx.r10.s64 + -1640;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r5,0(r11)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// clrlwi r4,r5,28
	ctx.r4.u64 = ctx.r5.u32 & 0xF;
	// stb r4,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r4.u8);
	// stw r7,144(r30)
	PPC_STORE_U32(var_r30 + 144, ctx.r7.u32);
	// stw r30,140(r30)
	PPC_STORE_U32(var_r30 + 140, var_r30);
	// stw r6,2460(r30)
	PPC_STORE_U32(var_r30 + 2460, ctx.r6.u32);
	// stw r30,2456(r30)
	PPC_STORE_U32(var_r30 + 2456, var_r30);
	return;
}

__attribute__((alias("__imp__snSessionFinder_vfn_0"))) PPC_WEAK_FUNC(snSessionFinder_vfn_0);
PPC_FUNC_IMPL(__imp__snSessionFinder_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823aae90
	game_AE90(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823eed68
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EED68:
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_ED80_w"))) PPC_WEAK_FUNC(SinglesNetworkClient_ED80_w);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_ED80_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32249
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r11,r11,16692
	ctx.r11.s64 = ctx.r11.s64 + 16692;
	// addi r29,r30,24
	var_r29 = (uint32_t)(var_r30 + 24);
	// lis r10,-32249
	// lis r9,-32249
	// li r31,0
	var_r31 = 0;
	// stw r11,0(r30)
	PPC_STORE_U32(var_r30 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r11.u32);
	// addi r11,r29,24
	ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 24;
	// lis r8,-32249
	// addi r10,r10,16764
	ctx.r10.s64 = ctx.r10.s64 + 16764;
	// addi r9,r9,16836
	ctx.r9.s64 = ctx.r9.s64 + 16836;
	// addi r28,r11,24
	var_r28 = (uint32_t)(ctx.r11.s64 + 24);  // addr:0x82070018
	// stw r31,4(r30)
	PPC_STORE_U32(var_r30 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// addi r8,r8,16908
	ctx.r8.s64 = ctx.r8.s64 + 16908;
	// stw r31,8(r30)
	PPC_STORE_U32(var_r30 + 8, var_r31);
	// stw r31,12(r30)
	PPC_STORE_U32(var_r30 + 12, var_r31);
	// addi r3,r28,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r28 + 24;
	// stw r31,16(r30)
	PPC_STORE_U32(var_r30 + 16, var_r31);
	// stw r31,20(r30)
	PPC_STORE_U32(var_r30 + 20, var_r31);
	// stw r31,4(r29)
	PPC_STORE_U32(var_r29 + 4,/* SinglesNetworkClient::flags@+0x4 */ var_r31);
	// stw r31,8(r29)
	PPC_STORE_U32(var_r29 + 8, var_r31);
	// stw r31,12(r29)
	PPC_STORE_U32(var_r29 + 12, var_r31);
	// stw r31,16(r29)
	PPC_STORE_U32(var_r29 + 16, var_r31);
	// stw r31,20(r29)
	PPC_STORE_U32(var_r29 + 20, var_r31);
	// stw r10,0(r29)
	PPC_STORE_U32(var_r29 + 0,/* SinglesNetworkClient::vtable@+0x0 */ ctx.r10.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, var_r31);
	// stw r31,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, var_r31);
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, var_r31);
	// stw r31,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, var_r31);
	// stw r31,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, var_r31);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// stw r31,4(r28)
	PPC_STORE_U32(var_r28 + 4, var_r31);
	// stw r31,8(r28)
	PPC_STORE_U32(var_r28 + 8, var_r31);
	// stw r31,12(r28)
	PPC_STORE_U32(var_r28 + 12, var_r31);
	// stw r31,16(r28)
	PPC_STORE_U32(var_r28 + 16, var_r31);
	// stw r31,20(r28)
	PPC_STORE_U32(var_r28 + 20, var_r31);
	// stw r8,0(r28)
	PPC_STORE_U32(var_r28 + 0, ctx.r8.u32);
	// bl 0x823da620
	SinglesNetworkClient_A620_w(ctx, base);
	// lis r10,-32193
	// stw r28,28(r28)
	PPC_STORE_U32(var_r28 + 28, var_r28);
	// lis r11,-32249
	// addi r10,r10,-1656
	ctx.r10.s64 = ctx.r10.s64 + -1656;
	// addi r11,r11,16980
	ctx.r11.s64 = ctx.r11.s64 + 16980;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stw r10,32(r28)
	PPC_STORE_U32(var_r28 + 32, ctx.r10.u32);
	// stw r11,96(r29)
	PPC_STORE_U32(var_r29 + 96, ctx.r11.u32);
	// lis r11,-32249
	// stw r31,100(r29)
	PPC_STORE_U32(var_r29 + 100, var_r31);
	// addi r11,r11,17052
	ctx.r11.s64 = ctx.r11.s64 + 17052;
	// stw r31,104(r29)
	PPC_STORE_U32(var_r29 + 104, var_r31);
	// stw r31,108(r29)
	PPC_STORE_U32(var_r29 + 108, var_r31);
	// stw r31,112(r29)
	PPC_STORE_U32(var_r29 + 112, var_r31);
	// stw r31,116(r29)
	PPC_STORE_U32(var_r29 + 116, var_r31);
	// stw r31,148(r30)
	PPC_STORE_U32(var_r30 + 148, var_r31);
	// stw r31,152(r30)
	PPC_STORE_U32(var_r30 + 152, var_r31);
	// stw r31,156(r30)
	PPC_STORE_U32(var_r30 + 156, var_r31);
	// stw r31,160(r30)
	PPC_STORE_U32(var_r30 + 160, var_r31);
	// stw r31,164(r30)
	PPC_STORE_U32(var_r30 + 164, var_r31);
	// stw r11,144(r30)
	PPC_STORE_U32(var_r30 + 144, ctx.r11.u32);
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_vfn_0"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_vfn_0);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r28,r4
	var_r28 = ctx.r4.u32;
	// addi r3,r31,144
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 144;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// addi r3,r30,96
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 96;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// addi r29,r30,24
	var_r29 = (uint32_t)(var_r30 + 24);
	// addi r3,r29,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 24;
	// bl 0x823a9bf8
	util_9BF8(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// clrlwi r11,r28,31
	ctx.r11.u64 = var_r28 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823eeeec
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EEEEC:
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_vfn_0"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_vfn_0);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// addi r3,r31,96
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 96;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// addi r30,r31,24
	var_r30 = (uint32_t)(var_r31 + 24);
	// addi r3,r30,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 24;
	// bl 0x823a9bf8
	util_9BF8(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// clrlwi r11,r29,31
	ctx.r11.u64 = var_r29 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823eef48
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EEF48:
	return;
}

__attribute__((alias("__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_0"))) PPC_WEAK_FUNC(snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_0);
PPC_FUNC_IMPL(__imp__snSessionFinder_slfRoot_slfSearching_slfFindingSyslink_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,24
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 24;
	// bl 0x823a9bf8
	util_9BF8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x823b34d8
	util_34D8(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823eef94
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823EEF94:
	// blr
	return;
}

__attribute__((alias("__imp__NotifyHandler_EFB0_g"))) PPC_WEAK_FUNC(NotifyHandler_EFB0_g);
PPC_FUNC_IMPL(__imp__NotifyHandler_EFB0_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addis r29,r31,1
	var_r29 = (uint32_t)(var_r31 + 65536);
	// addi r29,r29,3360
	var_r29 = (uint32_t)(var_r29 + 3360);
	// lbz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U8(var_r29 + 0);
	// rlwinm r10,r11,0,25,25
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x40;
	// cmplwi cr6,r10,0
	// beq cr6,0x823ef004
	if (ctx.r10.u32 != 0) {
		// lwz r11,328(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 328);
		// addi r30,r31,136
		var_r30 = (uint32_t)(var_r31 + 136);
		// addi r3,r11,464
		ctx.r3.s64 = ctx.r11.s64 + 464;
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x823b3d80
		NotifyHandler_3D80_g(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,20(r30)
		PPC_STORE_U32(var_r30 + 20, ctx.r11.u32);
		// lbz r9,0(r29)
		ctx.r9.u64 = PPC_LOAD_U8(var_r29 + 0);
		// stw r11,2448(r31)
		PPC_STORE_U32(var_r31 + 2448, ctx.r11.u32);
		// andi. r8,r9,191
		ctx.r8.u64 = ctx.r9.u64 & 191;
		ctx.cr0.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
		// stb r8,0(r29)
		PPC_STORE_U8(var_r29 + 0, ctx.r8.u8);
	}
loc_823EF004:
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_F010_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_F010_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_F010_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=256, manual
	// lis r11,1
	ctx.r11.s64 = 65536;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// ori r10,r11,3360
	ctx.r10.u64 = ctx.r11.u64 | 3360;
	// li r31,0
	var_r31 = 0;
	// lbzx r9,r30,r10
	ctx.r9.u64 = PPC_LOAD_U8(var_r30 + ctx.r10.u32);
	// rlwinm r8,r9,0,0,24
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r8,0
	// beq cr6,0x823ef058
	if (ctx.r8.u32 != 0) {
		// lis r11,-32249
		// addi r3,r1,208
		ctx.r3.s64 = ctx.r1.s64 + 208;
		// addi r4,r11,16512
		ctx.r4.s64 = ctx.r11.s64 + 16512;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// b 0x823ef0e0
	} else {
	loc_823EF058:
		// lis r11,-32249
		// addi r3,r1,84
		ctx.r3.s64 = ctx.r1.s64 + 84;
		// addi r11,r11,10340
		ctx.r11.s64 = ctx.r11.s64 + 10340;
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// bl 0x82121b00
		ke_1B00(ctx, base);
		// addi r3,r1,88
		ctx.r3.s64 = ctx.r1.s64 + 88;
		// bl 0x82121b00
		ke_1B00(ctx, base);
		// lis r11,-32249
		// li r7,500
		ctx.r7.s64 = 500;
		// stw r31,88(r1)
		PPC_STORE_U32(ctx.r1.u32 + 88, var_r31);
		// addi r11,r11,16600
		ctx.r11.s64 = ctx.r11.s64 + 16600;
		// stw r31,84(r1)
		PPC_STORE_U32(ctx.r1.u32 + 84, var_r31);
		// li r6,10
		ctx.r6.s64 = 10;
		// addi r3,r1,100
		ctx.r3.s64 = ctx.r1.s64 + 100;
		// stw r7,92(r1)
		PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r7.u32);
		// stw r11,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
		// stw r6,96(r1)
		PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r6.u32);
		// bl 0x823b3fd8
		SinglesNetworkClient_3FD8_g(ctx, base);
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r3,r1,100
		ctx.r3.s64 = ctx.r1.s64 + 100;
		// bl 0x822603d0
		snJoinMachine_03D0_g(ctx, base);
		// lwz r5,84(r30)
		ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 84);
		// cmpwi cr6,r5,0
		// ble cr6,0x823ef0bc
		if (ctx.r5.s32 > 0) {
			// li r31,1
			var_r31 = 1;
		}
	loc_823EF0BC:
		// clrlwi r3,r31,24
		ctx.r3.u64 = var_r31 & 0xFF;
		// cmplwi cr6,r3,0
		// bne cr6,0x823ef0d4
		if (ctx.r3.u32 == 0) {
			// addi r4,r1,80
			ctx.r4.s64 = ctx.r1.s64 + 80;
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x823edc40
			SinglesNetworkClient_DC40_g(ctx, base);
		}
	loc_823EF0D4:
		// addi r3,r1,100
		ctx.r3.s64 = ctx.r1.s64 + 100;
		// li r31,1
		var_r31 = 1;
		// bl 0x82260268
		SinglesNetworkClient_0268_g(ctx, base);
	}
loc_823EF0E0:
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_F100_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_F100_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_F100_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=432, savegprlr_28
	// lis r11,1
	ctx.r11.s64 = 65536;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// ori r10,r11,3360
	ctx.r10.u64 = ctx.r11.u64 | 3360;
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// mr r28,r7
	var_r28 = ctx.r7.u32;
	// li r31,0
	var_r31 = 0;
	// lbzx r9,r30,r10
	ctx.r9.u64 = PPC_LOAD_U8(var_r30 + ctx.r10.u32);
	// rlwinm r8,r9,0,0,24
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r8,0
	// beq cr6,0x823ef150
	if (ctx.r8.u32 != 0) {
		// lis r11,-32249
		// addi r3,r1,80
		ctx.r3.s64 = ctx.r1.s64 + 80;
		// addi r4,r11,16512
		ctx.r4.s64 = ctx.r11.s64 + 16512;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		return;
	}
loc_823EF150:
	// lis r11,-32249
	// addi r3,r1,100
	ctx.r3.s64 = ctx.r1.s64 + 100;
	// addi r11,r11,10340
	ctx.r11.s64 = ctx.r11.s64 + 10340;
	// stw r11,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// addi r3,r1,104
	ctx.r3.s64 = ctx.r1.s64 + 104;
	// bl 0x82121b00
	ke_1B00(ctx, base);
	// lis r11,-32249
	// li r7,1
	ctx.r7.s64 = 1;
	// stw r31,104(r1)
	PPC_STORE_U32(ctx.r1.u32 + 104, var_r31);
	// addi r11,r11,16620
	ctx.r11.s64 = ctx.r11.s64 + 16620;
	// stw r31,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, var_r31);
	// li r6,50
	ctx.r6.s64 = 50;
	// stw r29,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, var_r29);
	// addi r3,r1,120
	ctx.r3.s64 = ctx.r1.s64 + 120;
	// li r5,256
	ctx.r5.s64 = 256;
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// stw r7,112(r1)
	PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r7.u32);
	// stw r11,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	// stw r6,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r6.u32);
	// bl 0x82434100
	memcpy(ctx, base);
	// lwz r5,84(r30)
	ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 84);
	// stw r31,376(r1)
	PPC_STORE_U32(ctx.r1.u32 + 376, var_r31);
	// cmpwi cr6,r5,0
	// ble cr6,0x823ef1b8
	if (ctx.r5.s32 > 0) {
		// li r31,1
		var_r31 = 1;
	}
loc_823EF1B8:
	// clrlwi r3,r31,24
	ctx.r3.u64 = var_r31 & 0xFF;
	// cmplwi cr6,r3,0
	// bne cr6,0x823ef1d0
	if (ctx.r3.u32 == 0) {
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x823edc40
		SinglesNetworkClient_DC40_g(ctx, base);
	}
loc_823EF1D0:
	// li r3,1
	ctx.r3.s64 = 1;
	return;
}

__attribute__((alias("__imp__snSession_F1E0_w"))) PPC_WEAK_FUNC(snSession_F1E0_w);
PPC_FUNC_IMPL(__imp__snSession_F1E0_w) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r31,2480
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 2480;
	// bl 0x82425648
	util_5648(ctx, base);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823dda08
	util_DA08(ctx, base);
	// lis r11,-32249
	// lwz r10,84(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 84);
	// addi r11,r11,16580
	ctx.r11.s64 = ctx.r11.s64 + 16580;
	// cmpwi cr6,r10,0
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// bgt cr6,0x823ef224
	if (ctx.r10.s32 <= 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823EF224:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r8,0
	// bne cr6,0x823ef23c
	if (ctx.r8.u32 == 0) {
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x823edc40
		SinglesNetworkClient_DC40_g(ctx, base);
	}
loc_823EF23C:
	// blr
	return;
}

