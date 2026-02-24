#include "tt-decomp_init.h"

__attribute__((alias("__imp__TimedGameUpdateTimerMessage_vfn_6"))) PPC_WEAK_FUNC(TimedGameUpdateTimerMessage_vfn_6);
PPC_FUNC_IMPL(__imp__TimedGameUpdateTimerMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4392(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4392);  /* glob:lbl_825D1128 @ 0x825d1128 */
	// blr
	return;
}

__attribute__((alias("__imp__TimedGameUpdateTimerMessage_vfn_7"))) PPC_WEAK_FUNC(TimedGameUpdateTimerMessage_vfn_7);
PPC_FUNC_IMPL(__imp__TimedGameUpdateTimerMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5840
	ctx.r3.s64 = ctx.r11.s64 + -5840;
	// blr
	return;
}

__attribute__((alias("__imp__TimedGameUpdateTimerMessage_vfn_1"))) PPC_WEAK_FUNC(TimedGameUpdateTimerMessage_vfn_1);
PPC_FUNC_IMPL(__imp__TimedGameUpdateTimerMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lfs f13,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,8(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 8, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__TimedGameUpdateTimerMessage_vfn_2"))) PPC_WEAK_FUNC(TimedGameUpdateTimerMessage_vfn_2);
PPC_FUNC_IMPL(__imp__TimedGameUpdateTimerMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 8);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PlayerUpdateMessage_vfn_5"))) PPC_WEAK_FUNC(PlayerUpdateMessage_vfn_5);
PPC_FUNC_IMPL(__imp__PlayerUpdateMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r8,-1501
	// addi r11,r11,4428
	ctx.r11.s64 = ctx.r11.s64 + 4428;
	// ori r7,r8,11507
	ctx.r7.u64 = ctx.r8.u64 | 11507;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r9,r11,r3
	ctx.r9.s64 = ctx.r3.s64 - ctx.r11.s64;
	// addis r10,r11,2
	ctx.r10.s64 = ctx.r11.s64 + 131072;
	// mulhwu r6,r9,r7
	ctx.r6.u64 = (uint64_t(ctx.r9.u32) * uint64_t(ctx.r7.u32)) >> 32;
	// rlwinm r5,r6,23,9,31
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 23) & 0x7FFFFF;
	// addi r10,r10,-26268
	ctx.r10.s64 = ctx.r10.s64 + -26268;
	// clrlwi r9,r5,16
	ctx.r9.u64 = ctx.r5.u32 & 0xFFFF;
	// mr r3,r9
	ctx.r3.u64 = ctx.r9.u64;
	// mulli r8,r3,524
	ctx.r8.s64 = static_cast<int64_t>(ctx.r3.u64 * static_cast<uint64_t>(524));
	// lhz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r10.u32 + 0);
	// add r8,r8,r11
	ctx.r8.u64 = ctx.r8.u64 + ctx.r11.u64;
	// sth r4,522(r8)
	PPC_STORE_U16(ctx.r8.u32 + 522, ctx.r4.u16);
	// lhz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r10.u32 + 0);
	// cmplwi cr6,r8,65535
	// beq cr6,0x823b7d10
	if (ctx.r8.u32 != 65535) {
		// mulli r8,r8,524
		ctx.r8.s64 = static_cast<int64_t>(ctx.r8.u64 * static_cast<uint64_t>(524));
		// add r7,r8,r11
		ctx.r7.u64 = ctx.r8.u64 + ctx.r11.u64;
		// sth r9,520(r7)
		PPC_STORE_U16(ctx.r7.u32 + 520, ctx.r9.u16);
	}
loc_823B7D10:
	// addis r11,r11,2
	ctx.r11.s64 = ctx.r11.s64 + 131072;
	// sth r9,0(r10)
	PPC_STORE_U16(ctx.r10.u32 + 0, ctx.r9.u16);
	// addi r11,r11,-26272
	ctx.r11.s64 = ctx.r11.s64 + -26272;
	// lhz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
	// addi r6,r10,1
	ctx.r6.s64 = ctx.r10.s64 + 1;
	// sth r6,0(r11)
	PPC_STORE_U16(ctx.r11.u32 + 0, ctx.r6.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_7D30"))) PPC_WEAK_FUNC(SinglesNetworkClient_7D30);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_7D30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4428
	ctx.r11.s64 = ctx.r11.s64 + 4428;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addis r10,r11,2
	ctx.r10.s64 = ctx.r11.s64 + 131072;
	// addi r10,r10,-26268
	ctx.r10.s64 = ctx.r10.s64 + -26268;
	// lhz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r10.u32 + 0);
	// mr r8,r9
	ctx.r8.u64 = ctx.r9.u64;
	// cmplwi cr6,r8,65535
	// beq cr6,0x823b7dac
	if (ctx.r8.u32 != 65535) {
		// mulli r8,r8,524
		ctx.r8.s64 = static_cast<int64_t>(ctx.r8.u64 * static_cast<uint64_t>(524));
		// add r8,r8,r11
		ctx.r8.u64 = ctx.r8.u64 + ctx.r11.u64;
		// clrlwi r6,r9,16
		ctx.r6.u64 = ctx.r9.u32 & 0xFFFF;
		// lis r5,0
		ctx.r5.s64 = 0;
		// mulli r9,r6,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r6.u64 * static_cast<uint64_t>(524));
		// lhz r7,522(r8)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r8.u32 + 522);
		// add r3,r9,r11
		ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r7,0(r10)
		PPC_STORE_U16(ctx.r10.u32 + 0, ctx.r7.u16);
		// clrlwi r9,r7,16
		ctx.r9.u64 = ctx.r7.u32 & 0xFFFF;
		// ori r10,r5,65535
		ctx.r10.u64 = ctx.r5.u64 | 65535;
		// cmplwi cr6,r9,65535
		// beq cr6,0x823b7d90
		if (ctx.r9.u32 != 65535) {
			// mulli r9,r9,524
			ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
			// add r4,r9,r11
			ctx.r4.u64 = ctx.r9.u64 + ctx.r11.u64;
			// sth r10,520(r4)
			PPC_STORE_U16(ctx.r4.u32 + 520, ctx.r10.u16);
		}
	loc_823B7D90:
		// addis r11,r11,2
		ctx.r11.s64 = ctx.r11.s64 + 131072;
		// sth r10,522(r3)
		PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r10.u16);
		// addi r11,r11,-26272
		ctx.r11.s64 = ctx.r11.s64 + -26272;
		// lhz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 0);
		// add r8,r9,r10
		ctx.r8.u64 = ctx.r9.u64 + ctx.r10.u64;
		// sth r8,0(r11)
		PPC_STORE_U16(ctx.r11.u32 + 0, ctx.r8.u16);
		// blr
		return;
	}
loc_823B7DAC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerUpdateMessage_vfn_6"))) PPC_WEAK_FUNC(PlayerUpdateMessage_vfn_6);
PPC_FUNC_IMPL(__imp__PlayerUpdateMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4416(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4416);  /* glob:lbl_825D1140 @ 0x825d1140 */
	// blr
	return;
}

__attribute__((alias("__imp__PlayerUpdateMessage_vfn_7"))) PPC_WEAK_FUNC(PlayerUpdateMessage_vfn_7);
PPC_FUNC_IMPL(__imp__PlayerUpdateMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5812
	ctx.r3.s64 = ctx.r11.s64 + -5812;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerUpdateMessage_vfn_1"))) PPC_WEAK_FUNC(PlayerUpdateMessage_vfn_1);
PPC_FUNC_IMPL(__imp__PlayerUpdateMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r30,518
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 518;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f0,4(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r30 + 4, temp.u32);
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// addi r30,r30,8
	var_r30 = (uint32_t)(var_r30 + 8);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// addi r29,r30,2
	var_r29 = (uint32_t)(var_r30 + 2);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x820d7970
	util_7970(ctx, base);
	// lhz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 0);
	// addi r4,r30,4
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 4;
	// extsh r5,r11
	ctx.r5.s64 = ctx.r11.s16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	return;
}

__attribute__((alias("__imp__PlayerUpdateMessage_vfn_2"))) PPC_WEAK_FUNC(PlayerUpdateMessage_vfn_2);
PPC_FUNC_IMPL(__imp__PlayerUpdateMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// lbz r4,518(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 518);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,8
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
	// bl 0x82390598
	SinglesNetworkClient_0598_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PlayerUpdateMessage_vfn_4"))) PPC_WEAK_FUNC(PlayerUpdateMessage_vfn_4);
PPC_FUNC_IMPL(__imp__PlayerUpdateMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r3,518(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 518);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823b7ee4
	if (ctx.r3.u32 != 0) {
		// addi r4,r31,8
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
		// bl 0x821952c0
		pongPlayer_52C0_g(ctx, base);
	}
loc_823B7EE4:
	// blr
	return;
}

__attribute__((alias("__imp__PlayerMovementMessage_vfn_5"))) PPC_WEAK_FUNC(PlayerMovementMessage_vfn_5);
PPC_FUNC_IMPL(__imp__PlayerMovementMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,4452
	ctx.r11.s64 = ctx.r11.s64 + 4452;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,16004(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16004);
	// rlwinm r6,r7,26,6,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 26) & 0x3FFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 4) & 0xFFFFFFF0;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,66(r3)
	PPC_STORE_U16(ctx.r3.u32 + 66, ctx.r5.u16);
	// lhz r10,16004(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16004);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823b7f58
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,64(r8)
		PPC_STORE_U16(ctx.r8.u32 + 64, ctx.r9.u16);
	}
loc_823B7F58:
	// lhz r10,16000(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16000);
	// sth r9,16004(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16004, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,16000(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16000, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_7F70"))) PPC_WEAK_FUNC(SinglesNetworkClient_7F70);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_7F70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4452
	ctx.r11.s64 = ctx.r11.s64 + 4452;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,16004(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16004);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823b7ff4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,66(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 66);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 4) & 0xFFFFFFF0;
		// sth r7,16004(r11)
		PPC_STORE_U16(ctx.r11.u32 + 16004, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823b7fe0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 4) & 0xFFFFFFF0;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,64(r10)
			PPC_STORE_U16(ctx.r10.u32 + 64, ctx.r9.u16);
		}
	loc_823B7FE0:
		// sth r9,66(r3)
		PPC_STORE_U16(ctx.r3.u32 + 66, ctx.r9.u16);
		// lhz r8,16000(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16000);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,16000(r11)
		PPC_STORE_U16(ctx.r11.u32 + 16000, ctx.r7.u16);
		// blr
		return;
	}
loc_823B7FF4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerMovementMessage_vfn_6"))) PPC_WEAK_FUNC(PlayerMovementMessage_vfn_6);
PPC_FUNC_IMPL(__imp__PlayerMovementMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4440(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4440);  /* glob:lbl_825D1158 @ 0x825d1158 */
	// blr
	return;
}

__attribute__((alias("__imp__PlayerMovementMessage_vfn_7"))) PPC_WEAK_FUNC(PlayerMovementMessage_vfn_7);
PPC_FUNC_IMPL(__imp__PlayerMovementMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5792
	ctx.r3.s64 = ctx.r11.s64 + -5792;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8020"))) PPC_WEAK_FUNC(SinglesNetworkClient_8020);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8020) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	uint32_t ea{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stb r11,48(r31)
	PPC_STORE_U8(var_r31 + 48, ctx.r11.u8);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// lis r11,-32253
	// addi r9,r31,16
	ctx.r9.s64 = (int64_t)(int32_t)var_r31 + 16;
	// addi r8,r31,32
	ctx.r8.s64 = (int64_t)(int32_t)var_r31 + 32;
	// lfs f0,-12016(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32163
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// lwz r11,452(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 452);
	// lwz r10,168(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 168);
	// lwz r11,164(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 164);
	// addi r11,r11,16
	ctx.r11.s64 = ctx.r11.s64 + 16;
	// lfs f0,48(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 48);
	ctx.f0.f64 = double(temp.f32);
	// addi r7,r11,48
	ctx.r7.s64 = ctx.r11.s64 + 48;
	// lfs f13,52(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 52);
	ctx.f13.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// lvx128 v13,r0,r6
	ea = (ctx.r6.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx v13,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bne cr6,0x823b80f4
	if (ctx.cr6.eq) {
		// lis r11,-32160
		ctx.r11.s64 = -2107637760;
		// lwz r11,25500(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 25500);
		// cmplwi cr6,r11,0
		// beq cr6,0x823b80f4
		if (ctx.r11.u32 == 0) {
			// li r11,0
			ctx.r11.s64 = 0;
			// stb r11,49(r31)
			PPC_STORE_U8(var_r31 + 49, ctx.r11.u8);
			// blr
			return;
		}
		// lwz r5,12(r11)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// cmpwi cr6,r5,5
		// bne cr6,0x823b80d8
		if (ctx.r5.s32 == 5) {
			// li r11,1
			ctx.r11.s64 = 1;
			// stb r11,49(r31)
			PPC_STORE_U8(var_r31 + 49, ctx.r11.u8);
			// blr
			return;
		}
	loc_823B80D8:
		// li r11,0
		ctx.r11.s64 = 0;
		// stb r11,49(r31)
		PPC_STORE_U8(var_r31 + 49, ctx.r11.u8);
		// blr
		return;
	}
loc_823B80F4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,49(r31)
	PPC_STORE_U8(var_r31 + 49, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__PlayerMovementMessage_vfn_1"))) PPC_WEAK_FUNC(PlayerMovementMessage_vfn_1);
PPC_FUNC_IMPL(__imp__PlayerMovementMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// lwz r29,84(r1)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 84));
	// clrlwi r9,r29,31
	ctx.r9.u64 = var_r29 & 0x1;
	// stb r11,48(r31)
	PPC_STORE_U8(var_r31 + 48, ctx.r11.u8);
	// lis r11,-32253
	// cmplwi cr6,r9,0
	// lfs f31,-12016(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
	var_f31 = double(temp.f32);
	// beq cr6,0x823b8180
	if (ctx.r9.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// lfs f0,84(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		ctx.f0.f64 = double(temp.f32);
		// b 0x823b8184
	} else {
	loc_823B8180:
		// fmr f0,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = var_f31;
	}
loc_823B8184:
	// rlwinm r8,r29,0,30,30
	ctx.r8.u64 = __builtin_rotateleft64(var_r29 | (var_r29 << 32), 0) & 0x2;
	// stfs f0,16(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 16, temp.u32);
	// stfs f31,20(r31)
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r31 + 20, temp.u32);
	// cmplwi cr6,r8,0
	// beq cr6,0x823b81b0
	if (ctx.r8.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// lfs f0,84(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		ctx.f0.f64 = double(temp.f32);
		// b 0x823b81b4
	} else {
	loc_823B81B0:
		// fmr f0,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = var_f31;
	}
loc_823B81B4:
	// rlwinm r7,r29,0,29,29
	ctx.r7.u64 = __builtin_rotateleft64(var_r29 | (var_r29 << 32), 0) & 0x4;
	// stfs f0,24(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 24, temp.u32);
	// cmplwi cr6,r7,0
	// beq cr6,0x823b81dc
	if (ctx.r7.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// lfs f0,84(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		ctx.f0.f64 = double(temp.f32);
		// b 0x823b81e0
	} else {
	loc_823B81DC:
		// fmr f0,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = var_f31;
	}
loc_823B81E0:
	// rlwinm r6,r29,0,28,28
	ctx.r6.u64 = __builtin_rotateleft64(var_r29 | (var_r29 << 32), 0) & 0x8;
	// stfs f0,32(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 32, temp.u32);
	// stfs f31,36(r31)
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r31 + 36, temp.u32);
	// cmplwi cr6,r6,0
	// beq cr6,0x823b8208
	if (ctx.r6.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// lfs f31,84(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		var_f31 = double(temp.f32);
	}
loc_823B8208:
	// li r5,1
	ctx.r5.s64 = 1;
	// stfs f31,40(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r31 + 40, temp.u32);
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r5,84(r1)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmplwi cr6,r5,0
	// bne cr6,0x823b8230
	if (ctx.r5.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B8230:
	// stb r11,49(r31)
	PPC_STORE_U8(var_r31 + 49, ctx.r11.u8);
	return;
}

__attribute__((alias("__imp__PlayerMovementMessage_vfn_2"))) PPC_WEAK_FUNC(PlayerMovementMessage_vfn_2);
PPC_FUNC_IMPL(__imp__PlayerMovementMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32253
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// li r10,1
	ctx.r10.s64 = 1;
	// lfs f13,16(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 16);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,-12016(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	// bne cr6,0x823b8270
	if (ctx.f13.f64 == ctx.f0.f64) {
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_823B8270:
	// lfs f12,24(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 24);
	ctx.f12.f64 = double(temp.f32);
	// li r11,1
	ctx.r11.s64 = 1;
	// fcmpu cr6,f12,f0
	// bne cr6,0x823b8284
	if (ctx.f12.f64 == ctx.f0.f64) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B8284:
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// lfs f11,32(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 32);
	ctx.f11.f64 = double(temp.f32);
	// fcmpu cr6,f11,f0
	// or r10,r11,r10
	ctx.r10.u64 = ctx.r11.u64 | ctx.r10.u64;
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823b82a0
	if (ctx.f11.f64 == ctx.f0.f64) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B82A0:
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f10,40(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 40);
	ctx.f10.f64 = double(temp.f32);
	// fcmpu cr6,f10,f0
	// li r11,1
	ctx.r11.s64 = 1;
	// or r10,r9,r10
	ctx.r10.u64 = ctx.r9.u64 | ctx.r10.u64;
	// bne cr6,0x823b82bc
	if (ctx.f10.f64 == ctx.f0.f64) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B82BC:
	// rlwinm r8,r11,3,0,28
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// li r5,4
	ctx.r5.s64 = 4;
	// or r30,r8,r10
	var_r30 = (uint32_t)(ctx.r8.u64 | ctx.r10.u64);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lbz r4,48(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 48);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// clrlwi r7,r30,31
	ctx.r7.u64 = var_r30 & 0x1;
	// cmplwi cr6,r7,0
	// beq cr6,0x823b82fc
	if (ctx.r7.u32 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lfs f1,16(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 16);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x820d6990
		game_6990(ctx, base);
	}
loc_823B82FC:
	// rlwinm r6,r30,0,30,30
	ctx.r6.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 0) & 0x2;
	// cmplwi cr6,r6,0
	// beq cr6,0x823b8314
	if (ctx.r6.u32 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lfs f1,24(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 24);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x820d6990
		game_6990(ctx, base);
	}
loc_823B8314:
	// rlwinm r5,r30,0,29,29
	ctx.r5.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 0) & 0x4;
	// cmplwi cr6,r5,0
	// beq cr6,0x823b832c
	if (ctx.r5.u32 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lfs f1,32(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 32);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x820d6990
		game_6990(ctx, base);
	}
loc_823B832C:
	// rlwinm r4,r30,0,28,28
	ctx.r4.u64 = __builtin_rotateleft64(var_r30 | (var_r30 << 32), 0) & 0x8;
	// cmplwi cr6,r4,0
	// beq cr6,0x823b8344
	if (ctx.r4.u32 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lfs f1,40(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 40);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x820d6990
		game_6990(ctx, base);
	}
loc_823B8344:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lbz r4,49(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 49);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	return;
}

__attribute__((alias("__imp__PlayerMovementMessage_vfn_4"))) PPC_WEAK_FUNC(PlayerMovementMessage_vfn_4);
PPC_FUNC_IMPL(__imp__PlayerMovementMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// lis r11,-32163
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,2
	// bne cr6,0x823b8394
	if (ctx.r11.s32 == 2) {
		// lis r11,-32160
		ctx.r11.s64 = -2107637760;
		// lwz r11,25500(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 25500);
		// cmplwi cr6,r11,0
		// beq cr6,0x823b8394
		if (ctx.r11.u32 == 0) goto loc_823B8394;
		// li r11,0
		ctx.r11.s64 = 0;
		// stb r11,49(r31)
		PPC_STORE_U8(var_r31 + 49, ctx.r11.u8);
	}
loc_823B8394:
	// lbz r3,48(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 48);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823b83d4
	if (ctx.r3.u32 != 0) {
		// lwz r11,464(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 464);
		// addi r5,r31,32
		ctx.r5.s64 = (int64_t)(int32_t)var_r31 + 32;
		// addi r4,r31,16
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 16;
		// lbz r6,49(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 49);
		// addi r10,r11,17
		ctx.r10.s64 = ctx.r11.s64 + 17;
		// lis r11,-32161
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lwz r11,-21720(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
		// lwzx r8,r9,r11
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
		// lfs f0,52(r8)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 52);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,20(r31)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(var_r31 + 20, temp.u32);
		// bl 0x821954b0
		PlayerMovementMessage_54B0_h(ctx, base);
	}
loc_823B83D4:
	// blr
	return;
}

__attribute__((alias("__imp__PlayerStopMessage_vfn_5"))) PPC_WEAK_FUNC(PlayerStopMessage_vfn_5);
PPC_FUNC_IMPL(__imp__PlayerStopMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4476
	ctx.r11.s64 = ctx.r11.s64 + 4476;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,26,6,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 26) & 0x3FFFFFF;
	// lhz r8,12804(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12804);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,6,10,25
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 6) & 0x3FFFC0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,50(r7)
	PPC_STORE_U16(ctx.r7.u32 + 50, ctx.r8.u16);
	// lhz r9,12804(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12804);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b8428
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,6,0,25
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 6) & 0xFFFFFFC0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,48(r6)
		PPC_STORE_U16(ctx.r6.u32 + 48, ctx.r10.u16);
	}
loc_823B8428:
	// sth r10,12804(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12804, ctx.r10.u16);
	// lhz r10,12800(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12800);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,12800(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12800, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8440_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_8440_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8440_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4476
	ctx.r11.s64 = ctx.r11.s64 + 4476;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,12804(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12804);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b84a8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,6,0,25
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 6) & 0xFFFFFFC0;
		// rlwinm r10,r10,6,10,25
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 6) & 0x3FFFC0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,50(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 50);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,12804(r11)
		PPC_STORE_U16(ctx.r11.u32 + 12804, ctx.r8.u16);
		// beq cr6,0x823b8494
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,6,0,25
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 6) & 0xFFFFFFC0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,48(r6)
			PPC_STORE_U16(ctx.r6.u32 + 48, ctx.r9.u16);
		}
	loc_823B8494:
		// sth r9,50(r3)
		PPC_STORE_U16(ctx.r3.u32 + 50, ctx.r9.u16);
		// lhz r5,12800(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12800);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,12800(r11)
		PPC_STORE_U16(ctx.r11.u32 + 12800, ctx.r4.u16);
		// blr
		return;
	}
loc_823B84A8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerStopMessage_vfn_6"))) PPC_WEAK_FUNC(PlayerStopMessage_vfn_6);
PPC_FUNC_IMPL(__imp__PlayerStopMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4464(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4464);  /* glob:lbl_825D1170 @ 0x825d1170 */
	// blr
	return;
}

__attribute__((alias("__imp__PlayerStopMessage_vfn_7"))) PPC_WEAK_FUNC(PlayerStopMessage_vfn_7);
PPC_FUNC_IMPL(__imp__PlayerStopMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5768
	ctx.r3.s64 = ctx.r11.s64 + -5768;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerStopMessage_vfn_1"))) PPC_WEAK_FUNC(PlayerStopMessage_vfn_1);
PPC_FUNC_IMPL(__imp__PlayerStopMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// lwz r29,84(r1)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r1.u32 + 84));
	// clrlwi r9,r29,31
	ctx.r9.u64 = var_r29 & 0x1;
	// stb r11,32(r31)
	PPC_STORE_U8(var_r31 + 32, ctx.r11.u8);
	// lis r11,-32253
	// cmplwi cr6,r9,0
	// lfs f31,-12016(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
	var_f31 = double(temp.f32);
	// beq cr6,0x823b8540
	if (ctx.r9.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// lfs f0,84(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		ctx.f0.f64 = double(temp.f32);
		// b 0x823b8544
	} else {
	loc_823B8540:
		// fmr f0,f31
		ctx.fpscr.disableFlushMode();
		ctx.f0.f64 = var_f31;
	}
loc_823B8544:
	// rlwinm r8,r29,0,30,30
	ctx.r8.u64 = __builtin_rotateleft64(var_r29 | (var_r29 << 32), 0) & 0x2;
	// stfs f0,16(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 16, temp.u32);
	// stfs f31,20(r31)
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r31 + 20, temp.u32);
	// cmplwi cr6,r8,0
	// beq cr6,0x823b856c
	if (ctx.r8.u32 != 0) {
		// li r5,32
		ctx.r5.s64 = 32;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x82238df8
		SinglesNetworkClient_8DF8_g(ctx, base);
		// lfs f31,84(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
		var_f31 = double(temp.f32);
	}
loc_823B856C:
	// li r5,1
	ctx.r5.s64 = 1;
	// stfs f31,24(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(var_f31);
	PPC_STORE_U32(var_r31 + 24, temp.u32);
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r7,84(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmplwi cr6,r7,0
	// bne cr6,0x823b8594
	if (ctx.r7.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B8594:
	// stb r11,33(r31)
	PPC_STORE_U8(var_r31 + 33, ctx.r11.u8);
	return;
}

__attribute__((alias("__imp__PlayerStopMessage_vfn_2"))) PPC_WEAK_FUNC(PlayerStopMessage_vfn_2);
PPC_FUNC_IMPL(__imp__PlayerStopMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lis r11,-32253
	// mr r29,r4
	var_r29 = ctx.r4.u32;
	// li r10,1
	ctx.r10.s64 = 1;
	// lfs f13,16(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 16);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,-12016(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	// bne cr6,0x823b85d8
	if (ctx.f13.f64 == ctx.f0.f64) {
		// li r10,0
		ctx.r10.s64 = 0;
	}
loc_823B85D8:
	// lfs f12,24(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 24);
	ctx.f12.f64 = double(temp.f32);
	// li r11,1
	ctx.r11.s64 = 1;
	// fcmpu cr6,f12,f0
	// bne cr6,0x823b85ec
	if (ctx.f12.f64 == ctx.f0.f64) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B85EC:
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// li r5,2
	ctx.r5.s64 = 2;
	// or r31,r11,r10
	var_r31 = (uint32_t)(ctx.r11.u64 | ctx.r10.u64);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lbz r4,32(r30)
	ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 32);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// clrlwi r10,r31,31
	ctx.r10.u64 = var_r31 & 0x1;
	// cmplwi cr6,r10,0
	// beq cr6,0x823b862c
	if (ctx.r10.u32 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lfs f1,16(r30)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r30 + 16);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x820d6990
		game_6990(ctx, base);
	}
loc_823B862C:
	// rlwinm r9,r31,0,30,30
	ctx.r9.u64 = __builtin_rotateleft64(var_r31 | (var_r31 << 32), 0) & 0x2;
	// cmplwi cr6,r9,0
	// beq cr6,0x823b8644
	if (ctx.r9.u32 != 0) {
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// lfs f1,24(r30)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r30 + 24);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x820d6990
		game_6990(ctx, base);
	}
loc_823B8644:
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lbz r4,33(r30)
	ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 33);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	return;
}

__attribute__((alias("__imp__PlayerStopMessage_vfn_4"))) PPC_WEAK_FUNC(PlayerStopMessage_vfn_4);
PPC_FUNC_IMPL(__imp__PlayerStopMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// lis r11,-32163
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,2
	// bne cr6,0x823b8694
	if (ctx.r11.s32 == 2) {
		// lis r11,-32160
		ctx.r11.s64 = -2107637760;
		// lwz r11,25500(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 25500);
		// cmplwi cr6,r11,0
		// beq cr6,0x823b8694
		if (ctx.r11.u32 == 0) goto loc_823B8694;
		// li r11,0
		ctx.r11.s64 = 0;
		// stb r11,33(r31)
		PPC_STORE_U8(var_r31 + 33, ctx.r11.u8);
	}
loc_823B8694:
	// lbz r3,32(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 32);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823b86d0
	if (ctx.r3.u32 != 0) {
		// lwz r11,464(r3)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 464);
		// addi r4,r31,16
		ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 16;
		// lbz r5,33(r31)
		ctx.r5.u64 = PPC_LOAD_U8(var_r31 + 33);
		// addi r10,r11,17
		ctx.r10.s64 = ctx.r11.s64 + 17;
		// lis r11,-32161
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lwz r11,-21720(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
		// lwzx r8,r9,r11
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
		// lfs f0,52(r8)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 52);
		ctx.f0.f64 = double(temp.f32);
		// stfs f0,20(r31)
		temp.f32 = float(ctx.f0.f64);
		PPC_STORE_U32(var_r31 + 20, temp.u32);
		// bl 0x82195598
		pongPlayer_5598_h(ctx, base);
	}
loc_823B86D0:
	// blr
	return;
}

__attribute__((alias("__imp__SignalReadyToServeMessage_vfn_8"))) PPC_WEAK_FUNC(SignalReadyToServeMessage_vfn_8);
PPC_FUNC_IMPL(__imp__SignalReadyToServeMessage_vfn_8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32142
	ctx.r11.s64 = -2106458112;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stb r4,8(r3)
	PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r4.u8);
	// extsw r8,r9
	ctx.r8.s64 = ctx.r9.s32;
	// std r8,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r8.u64);
	// lfd f13,-16(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r3)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__SignalReadyToServeMessage_vfn_5"))) PPC_WEAK_FUNC(SignalReadyToServeMessage_vfn_5);
PPC_FUNC_IMPL(__imp__SignalReadyToServeMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4500
	ctx.r11.s64 = ctx.r11.s64 + 4500;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,164(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 164);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,164(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 164);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b8760
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823B8760:
	// sth r10,164(r11)
	PPC_STORE_U16(ctx.r11.u32 + 164, ctx.r10.u16);
	// lhz r10,160(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 160);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,160(r11)
	PPC_STORE_U16(ctx.r11.u32 + 160, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8778_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_8778_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8778_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4500
	ctx.r11.s64 = ctx.r11.s64 + 4500;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,164(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 164);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b87e0
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,164(r11)
		PPC_STORE_U16(ctx.r11.u32 + 164, ctx.r8.u16);
		// beq cr6,0x823b87cc
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823B87CC:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,160(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 160);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,160(r11)
		PPC_STORE_U16(ctx.r11.u32 + 160, ctx.r4.u16);
		// blr
		return;
	}
loc_823B87E0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__SignalReadyToServeMessage_vfn_6"))) PPC_WEAK_FUNC(SignalReadyToServeMessage_vfn_6);
PPC_FUNC_IMPL(__imp__SignalReadyToServeMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4488(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4488);  /* glob:lbl_825D1188 @ 0x825d1188 */
	// blr
	return;
}

__attribute__((alias("__imp__SignalReadyToServeMessage_vfn_7"))) PPC_WEAK_FUNC(SignalReadyToServeMessage_vfn_7);
PPC_FUNC_IMPL(__imp__SignalReadyToServeMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5748
	ctx.r3.s64 = ctx.r11.s64 + -5748;
	// blr
	return;
}

__attribute__((alias("__imp__SignalReadyToServeMessage_vfn_4"))) PPC_WEAK_FUNC(SignalReadyToServeMessage_vfn_4);
PPC_FUNC_IMPL(__imp__SignalReadyToServeMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// lis r11,-32163
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,0
	// bne cr6,0x823b88b0
	if (ctx.r11.s32 == 0) {
		// lis r11,-32160
		ctx.r11.s64 = -2107637760;
		// lwz r11,25500(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 25500);
		// lwz r11,12(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// cmpwi cr6,r11,26
		// bne cr6,0x823b8860
		if (ctx.r11.s32 == 26) {
			// li r11,-1
			// addi r6,r1,112
			ctx.r6.s64 = ctx.r1.s64 + 112;
			// li r5,1
			ctx.r5.s64 = 1;
			// li r4,64
			ctx.r4.s64 = 64;
			// li r3,10244
			ctx.r3.s64 = 10244;
			// stw r11,112(r1)
			PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
			// bl 0x8225e6e0
			pg_E6E0(ctx, base);
			// b 0x823b88b8
			goto loc_823B88B8;
		}
	loc_823B8860:
		// cmpwi cr6,r11,25
		// beq cr6,0x823b8884
		if (ctx.r11.s32 != 25) {
			// cmpwi cr6,r11,23
			// beq cr6,0x823b8884
			if (ctx.r11.s32 == 23) goto loc_823B8884;
			// cmpwi cr6,r11,24
			// beq cr6,0x823b8884
			if (ctx.r11.s32 == 24) goto loc_823B8884;
			// cmpwi cr6,r11,8
			// ble cr6,0x823b88c4
			if (ctx.r11.s32 <= 8) {
				// blr
				return;
			}
			// b 0x823b88b8
			goto loc_823B88B8;
		}
	loc_823B8884:
		// lis r11,-32254
		// lbz r6,8(r31)
		ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 8);
		// li r4,64
		ctx.r4.s64 = 64;
		// addi r5,r11,30872
		ctx.r5.s64 = ctx.r11.s64 + 30872;
		// li r3,14371
		ctx.r3.s64 = 14371;
		// bl 0x8225e480
		pg_E480(ctx, base);
		// blr
		return;
	}
loc_823B88B0:
	// cmpwi cr6,r11,2
	// beq cr6,0x823b88c4
	if (ctx.r11.s32 != 2) {
	loc_823B88B8:
		// lbz r3,8(r31)
		ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 8);
		// bl 0x823b58e8
		SinglesNetworkClient_58E8_g(ctx, base);
		// bl 0x82192668
		pongPlayer_EnterServeMode(ctx, base);
	}
loc_823B88C4:
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_5"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,4524
	ctx.r11.s64 = ctx.r11.s64 + 4524;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,100(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 100);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 3) & 0xFFFFFFF8;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,22(r3)
	PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r5.u16);
	// lhz r10,100(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 100);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823b8938
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,20(r8)
		PPC_STORE_U16(ctx.r8.u32 + 20, ctx.r9.u16);
	}
loc_823B8938:
	// lhz r10,96(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 96);
	// sth r9,100(r11)
	PPC_STORE_U16(ctx.r11.u32 + 100, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,96(r11)
	PPC_STORE_U16(ctx.r11.u32 + 96, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8950_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_8950_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8950_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4524
	ctx.r11.s64 = ctx.r11.s64 + 4524;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,100(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 100);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823b89d4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,22(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 22);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 3) & 0xFFFFFFF8;
		// sth r7,100(r11)
		PPC_STORE_U16(ctx.r11.u32 + 100, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823b89c0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,3,0,28
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 3) & 0xFFFFFFF8;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,20(r10)
			PPC_STORE_U16(ctx.r10.u32 + 20, ctx.r9.u16);
		}
	loc_823B89C0:
		// sth r9,22(r3)
		PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r9.u16);
		// lhz r8,96(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 96);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,96(r11)
		PPC_STORE_U16(ctx.r11.u32 + 96, ctx.r7.u16);
		// blr
		return;
	}
loc_823B89D4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_6"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4512(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4512);  /* glob:lbl_825D11A0 @ 0x825d11a0 */
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_7"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5720
	ctx.r3.s64 = ctx.r11.s64 + -5720;
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_8"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_8);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_8) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// lis r11,-32142
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// clrlwi r10,r4,24
	ctx.r10.u64 = ctx.r4.u32 & 0xFF;
	// mr r3,r10
	ctx.r3.u64 = ctx.r10.u64;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stb r10,8(r31)
	PPC_STORE_U8(var_r31 + 8, ctx.r10.u8);
	// extsw r8,r9
	ctx.r8.s64 = ctx.r9.s32;
	// std r8,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r8.u64);
	// lfd f13,80(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r31)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// lwz r11,464(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 464);
	// addi r7,r11,17
	ctx.r7.s64 = ctx.r11.s64 + 17;
	// lis r11,-32161
	// rlwinm r6,r7,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwzx r5,r6,r11
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + ctx.r11.u32);
	// lfs f9,48(r5)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 48);
	ctx.f9.f64 = double(temp.f32);
	// stfs f9,12(r31)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(var_r31 + 12, temp.u32);
	// lwz r10,464(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 464);
	// addi r4,r10,17
	ctx.r4.s64 = ctx.r10.s64 + 17;
	// rlwinm r3,r4,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r3,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + ctx.r11.u32);
	// lfs f8,56(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	ctx.f8.f64 = double(temp.f32);
	// stfs f8,16(r31)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(var_r31 + 16, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_4"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 8);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// lwz r11,464(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 464);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,12(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 12);
	ctx.f0.f64 = double(temp.f32);
	// lwz r3,500(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 500);
	// addi r11,r11,17
	ctx.r11.s64 = ctx.r11.s64 + 17;
	// lfs f13,16(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 16);
	ctx.f13.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lis r11,-32161
	ctx.r11.s64 = -2107703296;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lfs f12,52(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	ctx.f12.f64 = double(temp.f32);
	// stfs f12,84(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x8239d468
	ServeReadyMessage_D468_fw(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_1"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823b5fe0
	HitMessage_vfn_1(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,12(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 12, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lfs f13,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,16(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 16, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ServeReadyMessage_vfn_2"))) PPC_WEAK_FUNC(ServeReadyMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ServeReadyMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// lbz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,12(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 12);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,16(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 16);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeAbortMessage_vfn_5"))) PPC_WEAK_FUNC(ServeAbortMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ServeAbortMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4548
	ctx.r11.s64 = ctx.r11.s64 + 4548;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,68(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,68(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b8c20
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823B8C20:
	// sth r10,68(r11)
	PPC_STORE_U16(ctx.r11.u32 + 68, ctx.r10.u16);
	// lhz r10,64(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 64);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,64(r11)
	PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8C38"))) PPC_WEAK_FUNC(SinglesNetworkClient_8C38);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8C38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4548
	ctx.r11.s64 = ctx.r11.s64 + 4548;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,68(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b8ca0
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,68(r11)
		PPC_STORE_U16(ctx.r11.u32 + 68, ctx.r8.u16);
		// beq cr6,0x823b8c8c
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823B8C8C:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,64(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 64);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,64(r11)
		PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r4.u16);
		// blr
		return;
	}
loc_823B8CA0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ServeAbortMessage_vfn_6"))) PPC_WEAK_FUNC(ServeAbortMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ServeAbortMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4536(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4536);  /* glob:lbl_825D11B8 @ 0x825d11b8 */
	// blr
	return;
}

__attribute__((alias("__imp__ServeAbortMessage_vfn_7"))) PPC_WEAK_FUNC(ServeAbortMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ServeAbortMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5700
	ctx.r3.s64 = ctx.r11.s64 + -5700;
	// blr
	return;
}

__attribute__((alias("__imp__ServeAbortMessage_vfn_4"))) PPC_WEAK_FUNC(ServeAbortMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ServeAbortMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// FRAME: size=112, manual
	// lbz r3,8(r3)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// lwz r11,464(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 464);
	// addi r8,r1,80
	ctx.r8.s64 = ctx.r1.s64 + 80;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r3,500(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 500);
	// addi r11,r11,17
	ctx.r11.s64 = ctx.r11.s64 + 17;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r11,-32161
	ctx.r11.s64 = -2107703296;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// addi r9,r11,48
	ctx.r9.s64 = ctx.r11.s64 + 48;
	// lvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx v0,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8239d588
	pongPlayer_D588_h(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeAbortMessage_vfn_1"))) PPC_WEAK_FUNC(ServeAbortMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ServeAbortMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// b 0x823b5fe0
	HitMessage_vfn_1(ctx, base);
	return;
}

__attribute__((alias("__imp__ServeStartedMessage_vfn_5"))) PPC_WEAK_FUNC(ServeStartedMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ServeStartedMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4572
	ctx.r11.s64 = ctx.r11.s64 + 4572;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,24,8,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 24) & 0xFFFFFF;
	// lhz r8,1028(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1028);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,8,8,23
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 8) & 0xFFFF00;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,242(r7)
	PPC_STORE_U16(ctx.r7.u32 + 242, ctx.r8.u16);
	// lhz r9,1028(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1028);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b8d68
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,8,0,23
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 8) & 0xFFFFFF00;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,240(r6)
		PPC_STORE_U16(ctx.r6.u32 + 240, ctx.r10.u16);
	}
loc_823B8D68:
	// sth r10,1028(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1028, ctx.r10.u16);
	// lhz r10,1024(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1024);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,1024(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1024, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_8D80_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_8D80_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_8D80_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4572
	ctx.r11.s64 = ctx.r11.s64 + 4572;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,1028(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1028);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b8de8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,8,0,23
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 8) & 0xFFFFFF00;
		// rlwinm r10,r10,8,8,23
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 8) & 0xFFFF00;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,242(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 242);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,1028(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1028, ctx.r8.u16);
		// beq cr6,0x823b8dd4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,8,0,23
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 8) & 0xFFFFFF00;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,240(r6)
			PPC_STORE_U16(ctx.r6.u32 + 240, ctx.r9.u16);
		}
	loc_823B8DD4:
		// sth r9,242(r3)
		PPC_STORE_U16(ctx.r3.u32 + 242, ctx.r9.u16);
		// lhz r5,1024(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1024);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,1024(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1024, ctx.r4.u16);
		// blr
		return;
	}
loc_823B8DE8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ServeStartedMessage_vfn_6"))) PPC_WEAK_FUNC(ServeStartedMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ServeStartedMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4560(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4560);  /* glob:lbl_825D11D0 @ 0x825d11d0 */
	// blr
	return;
}

__attribute__((alias("__imp__ServeStartedMessage_vfn_7"))) PPC_WEAK_FUNC(ServeStartedMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ServeStartedMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5680
	ctx.r3.s64 = ctx.r11.s64 + -5680;
	// blr
	return;
}

__attribute__((alias("__imp__atSingleton_8E10_h"))) PPC_WEAK_FUNC(atSingleton_8E10_h);
PPC_FUNC_IMPL(__imp__atSingleton_8E10_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stb r11,224(r31)
	PPC_STORE_U8(var_r31 + 224, ctx.r11.u8);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,468(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 468);
	// cmpwi cr6,r11,1
	// bne cr6,0x823b8e54
	if (ctx.r11.s32 == 1) {
		// lis r11,-32249
		ctx.r11.s64 = -2113470464;
		// addi r3,r11,-3352
		ctx.r3.s64 = ctx.r11.s64 + -3352;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
	}
loc_823B8E54:
	// lis r11,-32158
	// lwz r9,464(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 464);
	// addi r3,r31,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
	// addi r11,r11,-23600
	ctx.r11.s64 = ctx.r11.s64 + -23600;
	// addi r10,r11,64
	ctx.r10.s64 = ctx.r11.s64 + 64;
	// mulli r11,r9,416
	ctx.r11.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(416));
	// add r4,r11,r10
	ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
	// bl 0x821e6470
	util_6470(ctx, base);
	// lis r11,-32142
	// lis r10,-32158
	// addi r10,r10,-26224
	ctx.r10.s64 = ctx.r10.s64 + -26224;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// addi r10,r10,16
	ctx.r10.s64 = ctx.r10.s64 + 16;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// extsw r7,r8
	ctx.r7.s64 = ctx.r8.s32;
	// std r7,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
	// lfd f13,80(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r31)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(var_r31 + 4,/* atSingleton::flags@+0x4 */ temp.u32);
	// lwz r11,464(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 464);
	// addi r6,r11,17
	ctx.r6.s64 = ctx.r11.s64 + 17;
	// lis r11,-32161
	// rlwinm r5,r6,2,0,29
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwzx r4,r5,r11
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + ctx.r11.u32);
	// lfs f9,48(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 48);
	ctx.f9.f64 = double(temp.f32);
	// stfs f9,208(r31)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(var_r31 + 208, temp.u32);
	// lwz r9,464(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 464);
	// addi r3,r9,17
	ctx.r3.s64 = ctx.r9.s64 + 17;
	// rlwinm r9,r3,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r8,r9,r11
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
	// lfs f8,56(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 56);
	ctx.f8.f64 = double(temp.f32);
	// stfs f8,212(r31)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(var_r31 + 212, temp.u32);
	// lwz r11,464(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 464);
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r7,r11,r9
	ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
	// rlwinm r11,r7,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 4) & 0xFFFFFFF0;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lfs f7,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f7.f64 = double(temp.f32);
	// stfs f7,216(r31)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(var_r31 + 216, temp.u32);
	// lfs f6,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f6.f64 = double(temp.f32);
	// stfs f6,220(r31)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(var_r31 + 220, temp.u32);
	// lwz r6,452(r30)
	ctx.r6.u64 = PPC_LOAD_U32(var_r30 + 452);
	// lwz r5,188(r6)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 188);
	// lwz r11,148(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 148);
	// lwz r4,28(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// cmpwi cr6,r4,0
	// bne cr6,0x823b8f34
	if (ctx.r4.s32 == 0) {
		// lwz r3,36(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmpwi cr6,r3,3
		// beq cr6,0x823b8f38
		if (ctx.r3.s32 == 3) goto loc_823B8F38;
	}
loc_823B8F34:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823B8F38:
	// stb r11,225(r31)
	PPC_STORE_U8(var_r31 + 225, ctx.r11.u8);
	// lwz r10,452(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 452);
	// lwz r9,188(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 188);
	// lwz r8,128(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 128);
	// lwz r7,40(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 40);
	// stb r7,226(r31)
	PPC_STORE_U8(var_r31 + 226, ctx.r7.u8);
	// blr
	return;
}

__attribute__((alias("__imp__ServeStartedMessage_vfn_1"))) PPC_WEAK_FUNC(ServeStartedMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ServeStartedMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x821d5538
	util_5538(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f13,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f13.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f13,208(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 208, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f12,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f12.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f12,212(r31)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(var_r31 + 212, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f11,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f11.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f11,216(r31)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(var_r31 + 216, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r31,224
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 224;
	// lfs f10,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f10.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f10,220(r31)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(var_r31 + 220, temp.u32);
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r31,225
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 225;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r31,226
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 226;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeStartedMessage_vfn_2"))) PPC_WEAK_FUNC(ServeStartedMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ServeStartedMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
	// bl 0x821d5738
	game_5738(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,208(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 208);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,212(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 212);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,216(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 216);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,220(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 220);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,224(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 224);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,225(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 225);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,226(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 226);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeStartedMessage_vfn_4"))) PPC_WEAK_FUNC(ServeStartedMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ServeStartedMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=144, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r3,224(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 224);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,468(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 468);
	// cmpwi cr6,r11,0
	// bne cr6,0x823b913c
	if (ctx.r11.s32 == 0) {
		// lis r11,-32249
		ctx.r11.s64 = -2113470464;
		// addi r3,r11,-3352
		ctx.r3.s64 = ctx.r11.s64 + -3352;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
	}
loc_823B913C:
	// lwz r10,464(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 464);
	// lis r11,-32158
	// addi r4,r31,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 16;
	// addi r11,r11,-23600
	ctx.r11.s64 = ctx.r11.s64 + -23600;
	// mulli r10,r10,416
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(416));
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// bl 0x82287040
	util_7040(ctx, base);
	// lwz r8,452(r30)
	ctx.r8.u64 = PPC_LOAD_U32(var_r30 + 452);
	// lbz r9,226(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 226);
	// addi r6,r1,96
	ctx.r6.s64 = ctx.r1.s64 + 96;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r7,188(r8)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + 188);
	// lwz r4,128(r7)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r7.u32 + 128);
	// stw r9,40(r4)
	PPC_STORE_U32(ctx.r4.u32 + 40, ctx.r9.u32);
	// lfs f0,208(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 208);
	ctx.f0.f64 = double(temp.f32);
	// lwz r11,464(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 464);
	// lfs f13,212(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 212);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,216(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 216);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,17
	ctx.r11.s64 = ctx.r11.s64 + 17;
	// lfs f11,220(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 220);
	ctx.f11.f64 = double(temp.f32);
	// lfs f1,4(r31)
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lis r11,-32161
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f12,96(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f11,104(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lfs f10,52(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	ctx.f10.f64 = double(temp.f32);
	// fmr f9,f10
	ctx.f9.f64 = ctx.f10.f64;
	// stfs f10,84(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f9,100(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// bl 0x82195728
	ServeStartedMessage_5728(ctx, base);
	// lbz r8,225(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 225);
	// lwz r9,500(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 500);
	// cntlzw r7,r8
	ctx.r7.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// rlwinm r6,r7,27,31,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x1;
	// stb r6,360(r9)
	PPC_STORE_U8(ctx.r9.u32 + 360, ctx.r6.u8);
	// lwz r5,452(r30)
	ctx.r5.u64 = PPC_LOAD_U32(var_r30 + 452);
	// lwz r4,188(r5)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r5.u32 + 188);
	// lwz r11,148(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 148);
	// lwz r3,28(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// cmpwi cr6,r3,0
	// bne cr6,0x823b9204
	if (ctx.r3.s32 == 0) {
		// lwz r11,36(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
		// cmpwi cr6,r11,3
		// li r11,1
		ctx.r11.s64 = 1;
		// beq cr6,0x823b9208
		if (ctx.r11.s32 == 3) goto loc_823B9208;
	}
loc_823B9204:
	// li r11,0
	ctx.r11.s64 = 0;
loc_823B9208:
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r9,0
	// beq cr6,0x823b924c
	if (ctx.r9.u32 != 0) {
		// lbz r8,225(r31)
		ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 225);
		// cmplwi cr6,r8,0
		// beq cr6,0x823b924c
		if (ctx.r8.u32 == 0) goto loc_823B924C;
		// lwz r7,464(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 464);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmpwi cr6,r7,0
		// beq cr6,0x823b9234
		if (ctx.r7.s32 != 0) {
			// li r11,2
			ctx.r11.s64 = 2;
		}
	loc_823B9234:
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// ori r4,r4,64
		ctx.r4.u64 = ctx.r4.u64 | 64;
		// li r3,14348
		ctx.r3.s64 = 14348;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
	}
loc_823B924C:
	// lwz r3,464(r30)
	ctx.r3.u64 = PPC_LOAD_U32(var_r30 + 464);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmpwi cr6,r3,0
	// beq cr6,0x823b9260
	if (ctx.r3.s32 != 0) {
		// li r11,2
		ctx.r11.s64 = 2;
	}
loc_823B9260:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// ori r4,r11,64
	ctx.r4.u64 = ctx.r11.u64 | 64;
	// li r3,14377
	ctx.r3.s64 = 14377;
	// bl 0x8225e6e0
	pg_E6E0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_5"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4596
	ctx.r11.s64 = ctx.r11.s64 + 4596;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b92d0
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823B92D0:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_92E8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_92E8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_92E8_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4596
	ctx.r11.s64 = ctx.r11.s64 + 4596;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b9350
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823b933c
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823B933C:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823B9350:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_6"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4584(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4584);  /* glob:lbl_825D11E8 @ 0x825d11e8 */
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_7"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5660
	ctx.r3.s64 = ctx.r11.s64 + -5660;
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_8"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_8);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32161
	// stb r4,8(r3)
	PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r4.u8);
	// lwz r11,-21716(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21716);
	// lwz r11,20(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// sth r11,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
	// lis r11,-32142
	ctx.r11.s64 = -2106458112;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// extsw r8,r9
	ctx.r8.s64 = ctx.r9.s32;
	// std r8,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r8.u64);
	// lfd f13,-16(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r3)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_1"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,10
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 10;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_2"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// lbz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// lhz r11,10(r31)
	ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 10);
	// li r5,16
	ctx.r5.s64 = 16;
	// extsh r4,r11
	ctx.r4.s64 = ctx.r11.s16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeFaultMessage_vfn_4"))) PPC_WEAK_FUNC(ServeFaultMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ServeFaultMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=128, manual
	// lis r11,-32161
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// lwz r11,-21716(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21716);
	// lhz r10,10(r7)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r7.u32 + 10);
	// extsh r5,r10
	ctx.r5.s64 = ctx.r10.s16;
	// lwz r4,20(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmpw cr6,r5,r4
	// bne cr6,0x823b950c
	if (ctx.r5.s32 == ctx.r4.s32) {
		// li r3,0
		ctx.r3.s64 = 0;
		// bl 0x823919a0
		SinglesNetworkClient_19A0_g(ctx, base);
		// clrlwi r9,r3,24
		ctx.r9.u64 = ctx.r3.u32 & 0xFF;
		// li r11,0
		ctx.r11.s64 = 0;
		// cmplwi cr6,r9,0
		// bne cr6,0x823b94e0
		if (ctx.r9.u32 == 0) {
			// li r11,64
			ctx.r11.s64 = 64;
		}
	loc_823B94E0:
		// lbz r8,8(r7)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r7.u32 + 8);
		// addi r6,r1,112
		ctx.r6.s64 = ctx.r1.s64 + 112;
		// li r5,1
		ctx.r5.s64 = 1;
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// li r3,2084
		ctx.r3.s64 = 2084;
		// stw r8,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r8.u32);
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
		// blr
		return;
	}
loc_823B950C:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-3304
	ctx.r3.s64 = ctx.r11.s64 + -3304;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_5"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4620
	ctx.r11.s64 = ctx.r11.s64 + 4620;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b9568
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823B9568:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_9580_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_9580_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_9580_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4620
	ctx.r11.s64 = ctx.r11.s64 + 4620;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b95e8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823b95d4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823B95D4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823B95E8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_6"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4608(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4608);  /* glob:lbl_825D1200 @ 0x825d1200 */
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_7"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5640
	ctx.r3.s64 = ctx.r11.s64 + -5640;
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_8"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_8);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32161
	// stb r4,8(r3)
	PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r4.u8);
	// lwz r11,-21716(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21716);
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lis r11,-32142
	ctx.r11.s64 = -2106458112;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// sth r10,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r10.u16);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// extsw r7,r8
	ctx.r7.s64 = ctx.r8.s32;
	// std r7,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r7.u64);
	// lfd f13,-16(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r3)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_1"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82101668
	util_1668(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,10
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 10;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_2"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// lbz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 8);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// extsb r4,r11
	ctx.r4.s64 = ctx.r11.s8;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// lhz r10,10(r31)
	ctx.r10.u64 = PPC_LOAD_U16(var_r31 + 10);
	// li r5,16
	ctx.r5.s64 = 16;
	// extsh r4,r10
	ctx.r4.s64 = ctx.r10.s16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ServeLetMessage_vfn_4"))) PPC_WEAK_FUNC(ServeLetMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ServeLetMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=112, manual
	// lis r11,-32161
	// lhz r10,10(r3)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r3.u32 + 10);
	// extsh r5,r10
	ctx.r5.s64 = ctx.r10.s16;
	// lwz r11,-21716(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21716);
	// lwz r4,20(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmpw cr6,r5,r4
	// bne cr6,0x823b978c
	if (ctx.r5.s32 == ctx.r4.s32) {
		// lbz r9,8(r3)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
		// li r4,1
		ctx.r4.s64 = 1;
		// cmplwi cr6,r9,0
		// beq cr6,0x823b976c
		if (ctx.r9.u32 != 0) {
			// li r4,2
			ctx.r4.s64 = 2;
		}
	loc_823B976C:
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r3,2083
		ctx.r3.s64 = 2083;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
		// blr
		return;
	}
loc_823B978C:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-3256
	ctx.r3.s64 = ctx.r11.s64 + -3256;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ExitPostGameMessage_vfn_5"))) PPC_WEAK_FUNC(ExitPostGameMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ExitPostGameMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,4644
	ctx.r11.s64 = ctx.r11.s64 + 4644;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823b9808
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823B9808:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__statePostGame_9820_g"))) PPC_WEAK_FUNC(statePostGame_9820_g);
PPC_FUNC_IMPL(__imp__statePostGame_9820_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4644
	ctx.r11.s64 = ctx.r11.s64 + 4644;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823b98a4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823b9890
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823B9890:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823B98A4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ExitPostGameMessage_vfn_6"))) PPC_WEAK_FUNC(ExitPostGameMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ExitPostGameMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4632(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4632);  /* glob:lbl_825D1218 @ 0x825d1218 */
	// blr
	return;
}

__attribute__((alias("__imp__ExitPostGameMessage_vfn_7"))) PPC_WEAK_FUNC(ExitPostGameMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ExitPostGameMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5624
	ctx.r3.s64 = ctx.r11.s64 + -5624;
	// blr
	return;
}

__attribute__((alias("__imp__ExitPostGameMessage_vfn_4"))) PPC_WEAK_FUNC(ExitPostGameMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ExitPostGameMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,128
	ctx.r4.s64 = 128;
	// li r3,14379
	ctx.r3.s64 = 14379;
	// b 0x8225e6e0
	pg_E6E0(ctx, base);
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_5"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_5);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,20164
	ctx.r9.s64 = 1321467904;
	// addi r11,r11,4668
	ctx.r11.s64 = ctx.r11.s64 + 4668;
	// ori r8,r9,60495
	ctx.r8.u64 = ctx.r9.u64 | 60495;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,8324(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 8324);
	// rlwinm r6,r7,25,7,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 25) & 0x1FFFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,416
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(416));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,402(r3)
	PPC_STORE_U16(ctx.r3.u32 + 402, ctx.r5.u16);
	// lhz r9,8324(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 8324);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b9938
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,416
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(416));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,400(r9)
		PPC_STORE_U16(ctx.r9.u32 + 400, ctx.r10.u16);
	}
loc_823B9938:
	// sth r10,8324(r11)
	PPC_STORE_U16(ctx.r11.u32 + 8324, ctx.r10.u16);
	// lhz r10,8320(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 8320);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,8320(r11)
	PPC_STORE_U16(ctx.r11.u32 + 8320, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_9950_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_9950_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_9950_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4668
	ctx.r11.s64 = ctx.r11.s64 + 4668;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,8324(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 8324);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823b99bc
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,416
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(416));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,416
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(416));
		// lhz r8,402(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 402);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,8324(r11)
		PPC_STORE_U16(ctx.r11.u32 + 8324, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823b99a8
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,416
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(416));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,400(r5)
			PPC_STORE_U16(ctx.r5.u32 + 400, ctx.r9.u16);
		}
	loc_823B99A8:
		// sth r9,402(r3)
		PPC_STORE_U16(ctx.r3.u32 + 402, ctx.r9.u16);
		// lhz r4,8320(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 8320);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,8320(r11)
		PPC_STORE_U16(ctx.r11.u32 + 8320, ctx.r10.u16);
		// blr
		return;
	}
loc_823B99BC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_6"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_6);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4656(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4656);  /* glob:lbl_825D1230 @ 0x825d1230 */
	// blr
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_7"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_7);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5604
	ctx.r3.s64 = ctx.r11.s64 + -5604;
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_99E8_h"))) PPC_WEAK_FUNC(pongNetMessageHolder_99E8_h);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_99E8_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_28
	// lis r11,-32248
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r30,r31,16
	var_r30 = (uint32_t)(var_r31 + 16);
	// lfs f0,-25900(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);  /* glob:0x82069ad4 */
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32249
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
	// addi r11,r11,-2856
	ctx.r11.s64 = ctx.r11.s64 + -2856;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x820d69f8
	ph_ctor_69F8(ctx, base);
	// lis r11,-32253
	// li r28,255
	var_r28 = 255;
	// addi r29,r31,376
	var_r29 = (uint32_t)(var_r31 + 376);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lfs f0,-12016(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
	ctx.f0.f64 = double(temp.f32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// sth r11,320(r30)
	PPC_STORE_U16(var_r30 + 320, ctx.r11.u16);
	// sth r11,322(r30)
	PPC_STORE_U16(var_r30 + 322, ctx.r11.u16);
	// sth r11,324(r30)
	PPC_STORE_U16(var_r30 + 324, ctx.r11.u16);
	// stfs f0,352(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 352, temp.u32);
	// sth r11,372(r31)
	PPC_STORE_U16(var_r31 + 372, ctx.r11.u16);
	// stfs f0,356(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 356, temp.u32);
	// stb r28,374(r31)
	PPC_STORE_U8(var_r31 + 374, (uint8_t)var_r28);
	// stfs f0,360(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 360, temp.u32);
	// stfs f0,364(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 364, temp.u32);
	// stfs f0,368(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 368, temp.u32);
	// bl 0x821d6d08
	pongNetMessageHolder_6D08_2hr(ctx, base);
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r28,388(r31)
	PPC_STORE_U8(var_r31 + 388, (uint8_t)var_r28);
	// stb r28,389(r31)
	PPC_STORE_U8(var_r31 + 389, (uint8_t)var_r28);
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// stb r9,390(r31)
	PPC_STORE_U8(var_r31 + 390, ctx.r9.u8);
	// lhz r10,4(r29)
	ctx.r10.u64 = PPC_LOAD_U16(var_r29 + 4);
	// cmplwi cr6,r10,0
	// beq cr6,0x823b9aa0
	if (ctx.r10.u32 != 0) {
		// mr r10,r9
		ctx.r10.u64 = ctx.r9.u64;
	loc_823B9A84:
		// lwz r8,0(r29)
		ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 0)/* pongNetMessageHolder::vtable@+0x0 */;
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// stwx r9,r8,r10
		PPC_STORE_U32(ctx.r8.u32 + ctx.r10.u32, ctx.r9.u32);
		// addi r10,r10,4
		ctx.r10.s64 = ctx.r10.s64 + 4;
		// lhz r7,4(r29)
		ctx.r7.u64 = PPC_LOAD_U16(var_r29 + 4);
		// cmplw cr6,r11,r7
		// blt cr6,0x823b9a84
		if (ctx.r11.u32 < ctx.r7.u32) goto loc_823B9A84;
	}
loc_823B9AA0:
	// lis r11,-32164
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f0,22840(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22840);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,8(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r29 + 8, temp.u32);
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_0"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_0);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// bl 0x823b9b08
	pcrSwingBlend_ctor_9B08(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = var_r30 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// cmplwi cr6,r11,0
	// beq cr6,0x823b9af0
	if (ctx.r11.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823B9AF0:
	// blr
	return;
}

__attribute__((alias("__imp__pcrSwingBlend_ctor_9B08"))) PPC_WEAK_FUNC(pcrSwingBlend_ctor_9B08);
PPC_FUNC_IMPL(__imp__pcrSwingBlend_ctor_9B08) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,376(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 376);
	// bl 0x820c00c0
	rage_free_00C0(ctx, base);
	// lis r11,-32253
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r31,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
	// addi r11,r11,-30076
	ctx.r11.s64 = ctx.r11.s64 + -30076;
	// stw r10,376(r31)
	PPC_STORE_U32(var_r31 + 376, ctx.r10.u32);
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// bl 0x821a9420
	atSingleton_9420(ctx, base);
	// lis r11,-32249
	// addi r11,r11,-15612
	ctx.r11.s64 = ctx.r11.s64 + -15612;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_9B60_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_9B60_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_9B60_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r20 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r21 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	double var_f29 = 0.0;
	PPCRegister temp{};
	// FRAME: size=256, savegprlr_20
	// lis r20,-32142
	var_r20 = (uint32_t)(-2106458112);
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r23,0
	var_r23 = 0;
	// lwz r11,-23768(r20)
	ctx.r11.u64 = PPC_LOAD_U32(var_r20 + -23768);
	// stb r4,374(r31)
	PPC_STORE_U8(var_r31 + 374, ctx.r4.u8);
	// stb r23,390(r31)
	PPC_STORE_U8(var_r31 + 390, (uint8_t)var_r23);
	// lwz r3,12(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,8(r10)
	// bctrl
	VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
	// lis r25,-32142
	var_r25 = (uint32_t)(-2106458112);
	// stfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(var_r31 + 4,/* SinglesNetworkClient::flags@+0x4 */ temp.u32);
	// addi r3,r31,376
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 376;
	// lwz r11,-23816(r25)
	ctx.r11.u64 = PPC_LOAD_U32(var_r25 + -23816);
	// lwz r11,20(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// addi r4,r11,9736
	ctx.r4.s64 = ctx.r11.s64 + 9736;
	// bl 0x821d6d90
	pongPlayer_6D90_p46(ctx, base);
	// lbz r3,374(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 374);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// lis r11,-32158
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// addi r11,r11,-26224
	ctx.r11.s64 = ctx.r11.s64 + -26224;
	// addi r10,r11,16
	ctx.r10.s64 = ctx.r11.s64 + 16;
	// lwz r11,464(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 464);
	// addi r8,r11,17
	ctx.r8.s64 = ctx.r11.s64 + 17;
	// lis r11,-32161
	// rlwinm r7,r8,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwzx r6,r7,r11
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r7.u32 + ctx.r11.u32);
	// lfs f0,48(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 48);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,356(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 356, temp.u32);
	// lwz r9,464(r30)
	ctx.r9.u64 = PPC_LOAD_U32(var_r30 + 464);
	// addi r5,r9,17
	ctx.r5.s64 = ctx.r9.s64 + 17;
	// rlwinm r4,r5,2,0,29
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r3,r4,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + ctx.r11.u32);
	// lfs f13,56(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 56);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,360(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 360, temp.u32);
	// lwz r11,464(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 464);
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// rlwinm r11,r11,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lfs f12,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// stfs f12,364(r31)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(var_r31 + 364, temp.u32);
	// lfs f11,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,368(r31)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(var_r31 + 368, temp.u32);
	// lwz r10,452(r30)
	ctx.r10.u64 = PPC_LOAD_U32(var_r30 + 452);
	// lwz r5,188(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 188);
	// mr r3,r5
	ctx.r3.u64 = ctx.r5.u64;
	// bl 0x820ce380
	SinglesNetworkClient_E380_wrh(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r5
	ctx.r3.u64 = ctx.r5.u64;
	// bl 0x820ce3f0
	pongPlayer_E3F0_p45(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823b9c68
	if (ctx.r3.u32 != 0) {
		// lwz r9,172(r5)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r5.u32 + 172);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmpwi cr6,r9,0
		// beq cr6,0x823b9c6c
		if (ctx.r9.s32 == 0) goto loc_823B9C6C;
	}
loc_823B9C68:
	// mr r11,r23
	ctx.r11.u64 = var_r23;
loc_823B9C6C:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r8,0
	// beq cr6,0x823b9e78
	if (ctx.r8.u32 != 0) {
		// li r5,1
		ctx.r5.s64 = 1;
		// lwz r7,452(r30)
		ctx.r7.u64 = PPC_LOAD_U32(var_r30 + 452);
		// lis r11,-32244
		// stw r23,120(r1)
		PPC_STORE_U32(ctx.r1.u32 + 120, var_r23);
		// li r26,3
		var_r26 = 3;
		// addi r6,r11,31120
		ctx.r6.s64 = ctx.r11.s64 + 31120;
		// stb r5,342(r31)
		PPC_STORE_U8(var_r31 + 342, ctx.r5.u8);
		// lwz r4,452(r30)
		ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 452);
		// stw r7,112(r1)
		PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r7.u32);
		// stw r26,124(r1)
		PPC_STORE_U32(ctx.r1.u32 + 124, var_r26);
		// stw r6,116(r1)
		PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r6.u32);
		// lwz r5,188(r4)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r4.u32 + 188);
		// mr r3,r5
		ctx.r3.u64 = ctx.r5.u64;
		// bl 0x820ce380
		SinglesNetworkClient_E380_wrh(ctx, base);
		// lwz r11,120(r5)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 120);
		// addi r29,r31,16
		var_r29 = (uint32_t)(var_r31 + 16);
		// lwz r21,172(r5)
		var_r21 = (uint32_t)(PPC_LOAD_U32(ctx.r5.u32 + 172));
		// mr r22,r3
		var_r22 = ctx.r3.u32;
		// addi r4,r11,32
		ctx.r4.s64 = ctx.r11.s64 + 32;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x820d7358
		pongPlayer_7358_g(ctx, base);
		// addi r11,r29,320
		ctx.r11.s64 = (int64_t)(int32_t)var_r29 + 320;
		// li r24,-1
		var_r24 = (uint32_t)(-1);
		// addi r28,r29,16
		var_r28 = (uint32_t)(var_r29 + 16);
		// mr r27,r11
		var_r27 = ctx.r11.u32;
		// sth r24,0(r11)
		PPC_STORE_U16(ctx.r11.u32 + 0, (uint16_t)var_r24);
		// sth r24,2(r11)
		PPC_STORE_U16(ctx.r11.u32 + 2, (uint16_t)var_r24);
		// sth r24,4(r11)
		PPC_STORE_U16(ctx.r11.u32 + 4, (uint16_t)var_r24);
	loc_823B9CE8:
		// lwz r4,0(r28)
		ctx.r4.u64 = PPC_LOAD_U32(var_r28 + 0);
		// cmplwi cr6,r4,0
		// beq cr6,0x823b9d0c
		if (ctx.r4.u32 != 0) {
			// addi r11,r1,100
			ctx.r11.s64 = ctx.r1.s64 + 100;
			// addi r3,r1,112
			ctx.r3.s64 = ctx.r1.s64 + 112;
			// stw r11,120(r1)
			PPC_STORE_U32(ctx.r1.u32 + 120, ctx.r11.u32);
			// bl 0x8225fff8
			util_FFF8(ctx, base);
			// lwz r10,100(r1)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 100);
			// sth r10,0(r27)
			PPC_STORE_U16(var_r27 + 0, ctx.r10.u16);
		}
	loc_823B9D0C:
		// addi r26,r26,-1
		var_r26 = (uint32_t)(var_r26 + -1);
		// addi r28,r28,4
		var_r28 = (uint32_t)(var_r28 + 4);
		// addi r27,r27,2
		var_r27 = (uint32_t)(var_r27 + 2);
		// cmplwi cr6,r26,0
		// bne cr6,0x823b9ce8
		if (var_r26 != 0) goto loc_823B9CE8;
		// lis r11,-32160
		// lfs f10,144(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 144);
		ctx.f10.f64 = double(temp.f32);
		// lfs f9,148(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 148);
		ctx.f9.f64 = double(temp.f32);
		// stb r22,388(r31)
		PPC_STORE_U8(var_r31 + 388, (uint8_t)var_r22);
		// stb r21,389(r31)
		PPC_STORE_U8(var_r31 + 389, (uint8_t)var_r21);
		// lfs f0,25972(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 25972);
		ctx.f0.f64 = double(temp.f32);
		// lwz r11,-23816(r25)
		ctx.r11.u64 = PPC_LOAD_U32(var_r25 + -23816);
		// fsubs f8,f10,f0
		ctx.f8.f64 = double(float(ctx.f10.f64 - ctx.f0.f64));
		// stfs f8,144(r31)
		temp.f32 = float(ctx.f8.f64);
		PPC_STORE_U32(var_r31 + 144, temp.u32);
		// fsubs f7,f9,f0
		ctx.f7.f64 = double(float(ctx.f9.f64 - ctx.f0.f64));
		// stfs f7,148(r31)
		temp.f32 = float(ctx.f7.f64);
		PPC_STORE_U32(var_r31 + 148, temp.u32);
		// lwz r11,20(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
		// lhz r28,9958(r11)
		var_r28 = (uint32_t)(PPC_LOAD_U16(ctx.r11.u32 + 9958));
		// li r11,1
		ctx.r11.s64 = 1;
		// rlwinm r6,r28,0,0,16
		ctx.r6.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 0) & 0xFFFF8000;
		// cmplwi cr6,r6,0
		// bne cr6,0x823b9d68
		if (ctx.r6.u32 == 0) {
			// mr r11,r23
			ctx.r11.u64 = var_r23;
		}
	loc_823B9D68:
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r4,0
		// bne cr6,0x823b9db0
		if (ctx.r4.u32 == 0) {
			// rlwinm r3,r28,0,19,19
			ctx.r3.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 0) & 0x1000;
			// cmplwi cr6,r3,0
			// beq cr6,0x823b9d84
			if (ctx.r3.u32 != 0) {
				// li r23,1
				var_r23 = 1;
			}
		loc_823B9D84:
			// clrlwi r10,r23,24
			ctx.r10.u64 = var_r23 & 0xFF;
			// cmplwi cr6,r10,0
			// bne cr6,0x823b9db0
			if (!(ctx.r10.u32 != 0)) {
				// mr r3,r29
				ctx.r3.u64 = var_r29;
				// bl 0x820d6aa0
				pongPlayer_6AA0_g(ctx, base);
				// clrlwi r9,r3,24
				ctx.r9.u64 = ctx.r3.u32 & 0xFF;
			} else {
				if (!(ctx.r9.u32 == 0)) {
					// clrlwi r8,r28,21
					ctx.r8.u64 = var_r28 & 0x7FF;
					// sth r8,372(r31)
					PPC_STORE_U16(var_r31 + 372, ctx.r8.u16);
					// b 0x823b9dc0
					} else {
				}
			}
		loc_823B9DB0:
			// lis r11,-32164
			// sth r24,372(r31)
			PPC_STORE_U16(var_r31 + 372, (uint16_t)var_r24);
			// lfs f0,22840(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22840);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,352(r31)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(var_r31 + 352, temp.u32);
		}
	loc_823B9DC0:
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x820d6aa0
		pongPlayer_6AA0_g(ctx, base);
		// clrlwi r7,r3,24
		ctx.r7.u64 = ctx.r3.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x823b9e90
		if (ctx.r7.u32 == 0) {
			return;
		}
		// lis r11,-32253
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// lfs f30,-12016(r11)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
		var_f30 = double(temp.f32);
		// stfs f30,96(r1)
		temp.f32 = float(var_f30);
		PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
		// bl 0x82194d30
		util_4D30(ctx, base);
		// lis r11,-32142
		// lfs f5,96(r1)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 96);
		ctx.f5.f64 = double(temp.f32);
		// lfs f31,1516(r30)
		temp.u32 = PPC_LOAD_U32(var_r30 + 1516);
		var_f31 = double(temp.f32);
		// fcmpu cr6,f31,f30
		// lwz r11,-23832(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23832);
		// lfs f6,48(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 48);
		ctx.f6.f64 = double(temp.f32);
		// fsubs f29,f5,f6
		var_f29 = double(float(ctx.f5.f64 - ctx.f6.f64));
		// bne cr6,0x823b9e10
		if (var_f31 == var_f30) {
			// lfs f31,520(r30)
			temp.u32 = PPC_LOAD_U32(var_r30 + 520);
			var_f31 = double(temp.f32);
		}
	loc_823B9E10:
		// lwz r11,-23768(r20)
		ctx.r11.u64 = PPC_LOAD_U32(var_r20 + -23768);
		// lwz r3,12(r11)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// lwz r6,20(r11)
		ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
		// lwz r4,0(r3)
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
		// lwz r5,80(r6)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r6.u32 + 80);
		// lwz r11,8(r4)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
		// lfs f30,528(r5)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 528);
		var_f30 = double(temp.f32);
		// bctrl
		PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
		// fmadds f4,f30,f29,f1
		ctx.fpscr.disableFlushMode();
		ctx.f4.f64 = double(float(var_f30 * var_f29 + ctx.f1.f64));
		// lis r11,-32142
		// li r4,50
		ctx.r4.s64 = 50;
		// addi r30,r11,-20004
		var_r30 = (uint32_t)(ctx.r11.s64 + -20004);  // lbl_8271B1DC @ 0x8271b1dc
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// fadds f1,f4,f31
		ctx.f1.f64 = double(float(ctx.f4.f64 + var_f31));
		// stfs f1,352(r31)
		temp.f32 = float(ctx.f1.f64);
		PPC_STORE_U32(var_r31 + 352, temp.u32);
		// bl 0x8239be30
		SinglesNetworkClient_BE30_g(ctx, base);
		// lis r11,-32249
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// addi r3,r11,-3208
		ctx.r3.s64 = ctx.r11.s64 + -3208;
		// bl 0x8240e6d0
		nop_8240E6D0(ctx, base);
		return;
	}
loc_823B9E78:
	// lis r11,-32248
	// stb r23,342(r31)
	PPC_STORE_U8(var_r31 + 342, (uint8_t)var_r23);
	// li r10,-1
	// lfs f0,-25900(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,352(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 352, temp.u32);
	// sth r10,372(r31)
	PPC_STORE_U16(var_r31 + 372, ctx.r10.u16);
loc_823B9E90:
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_1"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_1);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// addi r29,r30,16
	var_r29 = (uint32_t)(var_r30 + 16);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lfs f0,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// stfs f0,4(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r30 + 4, temp.u32);
	// bl 0x820d74f8
	SinglesNetworkClient_74F8_h(ctx, base);
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r4,r29,320
	ctx.r4.s64 = (int64_t)(int32_t)var_r29 + 320;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,84(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// cmplwi cr6,r11,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823b9f18
	if (ctx.r11.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823B9F18:
	// li r5,32
	ctx.r5.s64 = 32;
	// stb r11,326(r29)
	PPC_STORE_U8(var_r29 + 326, ctx.r11.u8);
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// lfs f13,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f13.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f13,352(r30)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r30 + 352, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// lfs f12,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f12.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f12,356(r30)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(var_r30 + 356, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// lfs f11,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f11.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f11,360(r30)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(var_r30 + 360, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// lfs f10,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f10.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f10,364(r30)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(var_r30 + 364, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r30,372
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 372;
	// lfs f9,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f9.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f9,368(r30)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(var_r30 + 368, temp.u32);
	// bl 0x820d7970
	util_7970(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r30,374
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 374;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r29,r30,376
	var_r29 = (uint32_t)(var_r30 + 376);
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lbz r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// bl 0x821d6e90
	SwingStartedMessage_6E90_wrh(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r30,388
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 388;
	// lfs f8,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f8.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f8,8(r29)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(var_r29 + 8, temp.u32);
	// bl 0x820d7a08
	util_7A08(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r30,389
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 389;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x820d7a08
	util_7A08(ctx, base);
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_2"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_2);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,4(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// addi r29,r30,16
	var_r29 = (uint32_t)(var_r30 + 16);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x820d7688
	SinglesNetworkClient_7688_g(ctx, base);
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r4,r29,320
	ctx.r4.s64 = (int64_t)(int32_t)var_r29 + 320;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lbz r4,326(r29)
	ctx.r4.u64 = PPC_LOAD_U8(var_r29 + 326);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,352(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 352);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,356(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 356);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,360(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 360);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,364(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 364);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,368(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 368);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// lhz r11,372(r30)
	ctx.r11.u64 = PPC_LOAD_U16(var_r30 + 372);
	// li r5,16
	ctx.r5.s64 = 16;
	// extsh r4,r11
	ctx.r4.s64 = ctx.r11.s16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lbz r4,374(r30)
	ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 374);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// addi r29,r30,376
	var_r29 = (uint32_t)(var_r30 + 376);
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x821d6e18
	SwingStartedMessage_6E18_h(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,8(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r29 + 8);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lbz r4,388(r30)
	ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 388);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lbz r4,389(r30)
	ctx.r4.u64 = PPC_LOAD_U8(var_r30 + 389);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	return;
}

__attribute__((alias("__imp__SwingStartedMessage_vfn_4"))) PPC_WEAK_FUNC(SwingStartedMessage_vfn_4);
PPC_FUNC_IMPL(__imp__SwingStartedMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	double var_f30 = 0.0;
	double var_f31 = 0.0;
	PPCRegister temp{};
	// FRAME: size=256, savegprlr_23
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r3,374(r31)
	ctx.r3.u64 = PPC_LOAD_U8(var_r31 + 374);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// lis r27,-32142
	var_r27 = (uint32_t)(-2106458112);
	// mr r26,r3
	var_r26 = ctx.r3.u32;
	// lwz r11,-23768(r27)
	ctx.r11.u64 = PPC_LOAD_U32(var_r27 + -23768);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// lwz r9,8(r10)
	// bctrl
	VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
	// lwz r11,-23768(r27)
	ctx.r11.u64 = PPC_LOAD_U32(var_r27 + -23768);
	// lis r23,-32142
	var_r23 = (uint32_t)(-2106458112);
	// lfs f12,352(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 352);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f11,f12,f1
	ctx.f11.f64 = double(float(ctx.f12.f64 - ctx.f1.f64));
	// lbz r7,342(r31)
	ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 342);
	// cmplwi cr6,r7,0
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r10,-23832(r23)
	ctx.r10.u64 = PPC_LOAD_U32(var_r23 + -23832);
	// lwz r6,80(r8)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r8.u32 + 80);
	// lfs f0,48(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 48);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,532(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 532);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f30,f11,f13,f0
	var_f30 = double(float(ctx.f11.f64 * ctx.f13.f64 + ctx.f0.f64));
	// beq cr6,0x823ba4e8
	if (ctx.r7.u32 != 0) {
		// li r24,0
		var_r24 = 0;
		// lwz r4,452(r26)
		ctx.r4.u64 = PPC_LOAD_U32(var_r26 + 452);
		// lis r10,-32244
		// addi r11,r1,132
		ctx.r11.s64 = ctx.r1.s64 + 132;
		// li r28,3
		var_r28 = 3;
		// addi r5,r10,31216
		ctx.r5.s64 = ctx.r10.s64 + 31216;
		// stw r24,128(r1)
		PPC_STORE_U32(ctx.r1.u32 + 128, var_r24);
		// addi r25,r31,16
		var_r25 = (uint32_t)(var_r31 + 16);
		// stw r24,152(r1)
		PPC_STORE_U32(ctx.r1.u32 + 152, var_r24);
		// addi r29,r1,128
		var_r29 = (uint32_t)(ctx.r1.s64 + 128);
		// stw r24,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, var_r24);
		// addi r30,r25,320
		var_r30 = (uint32_t)(var_r25 + 320);
		// stw r24,4(r11)
		PPC_STORE_U32(ctx.r11.u32 + 4, var_r24);
		// stw r28,156(r1)
		PPC_STORE_U32(ctx.r1.u32 + 156, var_r28);
		// stw r5,148(r1)
		PPC_STORE_U32(ctx.r1.u32 + 148, ctx.r5.u32);
		// stw r4,144(r1)
		PPC_STORE_U32(ctx.r1.u32 + 144, ctx.r4.u32);
	loc_823BA1D0:
		// lhz r11,0(r30)
		ctx.r11.u64 = PPC_LOAD_U16(var_r30 + 0);
		// cmplwi cr6,r11,65535
		// beq cr6,0x823ba1f0
		if (ctx.r11.u32 != 65535) {
			// addi r4,r1,112
			ctx.r4.s64 = ctx.r1.s64 + 112;
			// stw r11,112(r1)
			PPC_STORE_U32(ctx.r1.u32 + 112, ctx.r11.u32);
			// addi r3,r1,144
			ctx.r3.s64 = ctx.r1.s64 + 144;
			// stw r29,152(r1)
			PPC_STORE_U32(ctx.r1.u32 + 152, var_r29);
			// bl 0x8225fff8
			util_FFF8(ctx, base);
		}
	loc_823BA1F0:
		// addi r28,r28,-1
		var_r28 = (uint32_t)(var_r28 + -1);
		// addi r30,r30,2
		var_r30 = (uint32_t)(var_r30 + 2);
		// addi r29,r29,4
		var_r29 = (uint32_t)(var_r29 + 4);
		// cmplwi cr6,r28,0
		// bne cr6,0x823ba1d0
		if (var_r28 != 0) goto loc_823BA1D0;
		// lwz r11,128(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 128);
		// mr r3,r25
		ctx.r3.u64 = var_r25;
		// lwz r10,132(r1)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 132);
		// lwz r9,136(r1)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 136);
		// stw r11,16(r25)
		PPC_STORE_U32(var_r25 + 16, ctx.r11.u32);
		// stw r10,20(r25)
		PPC_STORE_U32(var_r25 + 20, ctx.r10.u32);
		// stw r9,24(r25)
		PPC_STORE_U32(var_r25 + 24, ctx.r9.u32);
		// bl 0x820d6aa0
		pongPlayer_6AA0_g(ctx, base);
		// clrlwi r8,r3,24
		ctx.r8.u64 = ctx.r3.u32 & 0xFF;
		// lis r11,-32158
		ctx.r11.s64 = -2107506688;
		// cmplwi cr6,r8,0
		// addi r29,r11,-23600
		var_r29 = (uint32_t)(ctx.r11.s64 + -23600);  // lbl_8261A3D0 @ 0x8261a3d0
		// beq cr6,0x823ba364
		if (ctx.r8.u32 != 0) {
			// lhz r7,372(r31)
			ctx.r7.u64 = PPC_LOAD_U16(var_r31 + 372);
			// cmplwi cr6,r7,65535
			// beq cr6,0x823ba364
			if (ctx.r7.u32 == 65535) goto loc_823BA364;
			// lbz r6,390(r31)
			ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 390);
			// cmplwi cr6,r6,0
			// bne cr6,0x823ba364
			if (ctx.r6.u32 != 0) goto loc_823BA364;
			// lwz r10,-23768(r27)
			ctx.r10.u64 = PPC_LOAD_U32(var_r27 + -23768);
			// lfs f10,352(r31)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r31 + 352);
			ctx.f10.f64 = double(temp.f32);
			// lwz r11,12(r10)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
			// lwz r5,4(r11)
			ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
			// lfs f9,8(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
			ctx.f9.f64 = double(temp.f32);
			// lis r11,-32163
			// extsw r4,r5
			ctx.r4.s64 = ctx.r5.s32;
			// addi r11,r11,-20808
			ctx.r11.s64 = ctx.r11.s64 + -20808;
			// std r4,112(r1)
			PPC_STORE_U64(ctx.r1.u32 + 112, ctx.r4.u64);
			// lfd f8,112(r1)
			ctx.f8.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
			// fcfid f7,f8
			ctx.f7.f64 = double(ctx.f8.s64);
			// lfs f0,84(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 84);
			ctx.f0.f64 = double(temp.f32);
			// frsp f6,f7
			ctx.f6.f64 = double(float(ctx.f7.f64));
			// fadds f5,f9,f6
			ctx.f5.f64 = double(float(ctx.f9.f64 + ctx.f6.f64));
			// fsubs f1,f10,f5
			ctx.f1.f64 = double(float(ctx.f10.f64 - ctx.f5.f64));
			// fcmpu cr6,f1,f0
			// ble cr6,0x823ba364
			if (ctx.f1.f64 <= ctx.f0.f64) goto loc_823BA364;
			// lwz r3,4(r10)
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
			// lfs f31,272(r3)
			temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 272);
			var_f31 = double(temp.f32);
			// fcmpu cr6,f1,f31
			// bge cr6,0x823ba2a8
			if (ctx.f1.f64 < var_f31) {
				// fmr f31,f1
				var_f31 = ctx.f1.f64;
			}
		loc_823BA2A8:
			// lis r11,-32142
			// li r4,50
			ctx.r4.s64 = 50;
			// addi r30,r11,-20004
			var_r30 = (uint32_t)(ctx.r11.s64 + -20004);  // lbl_8271B1DC @ 0x8271b1dc
			// mr r3,r30
			ctx.r3.u64 = var_r30;
			// bl 0x8239be30
			SinglesNetworkClient_BE30_g(ctx, base);
			// lis r11,-32249
			// fmr f1,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = var_f31;
			// mr r4,r30
			ctx.r4.u64 = var_r30;
			// addi r3,r11,-3152
			ctx.r3.s64 = ctx.r11.s64 + -3152;
			// stfd f1,32(r1)
			PPC_STORE_U64(ctx.r1.u32 + 32, ctx.f1.u64);
			// ld r5,32(r1)
			ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 32);
			// bl 0x8240e6d0
			nop_8240E6D0(ctx, base);
			// lwz r11,-23768(r27)
			ctx.r11.u64 = PPC_LOAD_U32(var_r27 + -23768);
			// mr r7,r24
			ctx.r7.u64 = var_r24;
			// lwz r11,24(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
			// addi r10,r11,224
			ctx.r10.s64 = ctx.r11.s64 + 224;
			// lwz r11,216(r10)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 216);
			// addi r11,r11,-1
			ctx.r11.s64 = ctx.r11.s64 + -1;
			// cmpwi cr6,r11,0
			// blt cr6,0x823ba340
			if (ctx.r11.s32 >= 0) {
				// addi r9,r11,4
				ctx.r9.s64 = ctx.r11.s64 + 4;
				// lhz r8,372(r31)
				ctx.r8.u64 = PPC_LOAD_U16(var_r31 + 372);
				// rlwinm r9,r9,2,0,29
				ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
				// extsh r8,r8
				ctx.r8.s64 = ctx.r8.s16;
				// add r9,r9,r10
				ctx.r9.u64 = ctx.r9.u64 + ctx.r10.u64;
			loc_823BA30C:
				// lwz r6,0(r9)
				ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
				// lhz r5,198(r6)
				ctx.r5.u64 = PPC_LOAD_U16(ctx.r6.u32 + 198);
				// clrlwi r4,r5,21
				ctx.r4.u64 = ctx.r5.u32 & 0x7FF;
				// cmpw cr6,r4,r8
				// beq cr6,0x823ba334
				if (ctx.r4.s32 != ctx.r8.s32) {
					// addi r11,r11,-1
					ctx.r11.s64 = ctx.r11.s64 + -1;
					// addi r9,r9,-4
					ctx.r9.s64 = ctx.r9.s64 + -4;
					// cmpwi cr6,r11,0
					// bge cr6,0x823ba30c
					if (ctx.r11.s32 >= 0) goto loc_823BA30C;
					// b 0x823ba340
				} else {
				loc_823BA334:
					// addi r3,r11,4
					ctx.r3.s64 = ctx.r11.s64 + 4;
					// rlwinm r11,r3,2,0,29
					ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
					// lwzx r7,r11,r10
					ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
				}
			}
		loc_823BA340:
			// lwz r10,464(r26)
			ctx.r10.u64 = PPC_LOAD_U32(var_r26 + 464);
			// li r6,1
			ctx.r6.s64 = 1;
			// addi r5,r7,16
			ctx.r5.s64 = ctx.r7.s64 + 16;
			// fmr f1,f31
			ctx.fpscr.disableFlushMode();
			ctx.f1.f64 = var_f31;
			// cntlzw r9,r10
			ctx.r9.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
			// rlwinm r8,r9,27,31,31
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
			// mulli r11,r8,416
			ctx.r11.s64 = static_cast<int64_t>(ctx.r8.u64 * static_cast<uint64_t>(416));
			// add r3,r11,r29
			ctx.r3.u64 = ctx.r11.u64 + var_r29;
			// bl 0x82282a50
			gdBallPredictData_2A50_g(ctx, base);
		}
	loc_823BA364:
		// lis r11,-32253
		// lfs f4,352(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 352);
		ctx.f4.f64 = double(temp.f32);
		// addi r10,r11,-12024
		ctx.r10.s64 = ctx.r11.s64 + -12024;
		// lfs f13,8(r10)
		temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		ctx.f13.f64 = double(temp.f32);
		// fcmpu cr6,f4,f13
		// ble cr6,0x823ba394
		if (ctx.f4.f64 > ctx.f13.f64) {
			// stfs f30,144(r31)
			temp.f32 = float(var_f30);
			PPC_STORE_U32(var_r31 + 144, temp.u32);
			// lwz r7,464(r26)
			ctx.r7.u64 = PPC_LOAD_U32(var_r26 + 464);
			// addi r5,r29,356
			ctx.r5.s64 = (int64_t)(int32_t)var_r29 + 356;
			// mulli r6,r7,416
			ctx.r6.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(416));
			// stfsx f30,r6,r5
			temp.f32 = float(var_f30);
			PPC_STORE_U32(ctx.r6.u32 + ctx.r5.u32, temp.u32);
			// b 0x823ba3cc
		} else {
		loc_823BA394:
			// lis r11,-32160
			// lfs f3,144(r31)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(var_r31 + 144);
			ctx.f3.f64 = double(temp.f32);
			// lfs f0,25972(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 25972);
			ctx.f0.f64 = double(temp.f32);
			// fadds f2,f3,f0
			ctx.f2.f64 = double(float(ctx.f3.f64 + ctx.f0.f64));
			// stfs f2,144(r31)
			temp.f32 = float(ctx.f2.f64);
			PPC_STORE_U32(var_r31 + 144, temp.u32);
			// fmr f1,f2
			ctx.f1.f64 = ctx.f2.f64;
			// fcmpu cr6,f1,f13
			// bgt cr6,0x823ba3cc
			if (ctx.f1.f64 > ctx.f13.f64) goto loc_823BA3CC;
			// bso cr6,0x823ba3cc
			// UNIMPLEMENTED: bso
			PPC_UNIMPLEMENTED(0x823BA3B4, "bso");
			// lwz r11,-23832(r23)
			ctx.r11.u64 = PPC_LOAD_U32(var_r23 + -23832);
			// lfs f0,0(r10)
			temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
			ctx.f0.f64 = double(temp.f32);
			// lfs f13,48(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 48);
			ctx.f13.f64 = double(temp.f32);
			// fadds f12,f13,f0
			ctx.f12.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
			// stfs f12,144(r31)
			temp.f32 = float(ctx.f12.f64);
			PPC_STORE_U32(var_r31 + 144, temp.u32);
		}
	loc_823BA3CC:
		// lwz r4,464(r26)
		ctx.r4.u64 = PPC_LOAD_U32(var_r26 + 464);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmpwi cr6,r4,0
		// beq cr6,0x823ba3e0
		if (ctx.r4.s32 != 0) {
			// li r11,2
			ctx.r11.s64 = 2;
		}
	loc_823BA3E0:
		// clrlwi r3,r11,24
		ctx.r3.u64 = ctx.r11.u32 & 0xFF;
		// li r6,0
		ctx.r6.s64 = 0;
		// ori r4,r3,64
		ctx.r4.u64 = ctx.r3.u64 | 64;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r3,14404
		ctx.r3.s64 = 14404;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
		// lbz r11,390(r31)
		ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 390);
		// cmplwi cr6,r11,0
		// beq cr6,0x823ba41c
		if (ctx.r11.u32 != 0) {
			// lis r11,-32142
			// addi r4,r31,376
			ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 376;
			// lwz r11,-23816(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23816);
			// lwz r11,20(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
			// addi r3,r11,9736
			ctx.r3.s64 = ctx.r11.s64 + 9736;
			// bl 0x821d6d90
			pongPlayer_6D90_p46(ctx, base);
		}
	loc_823BA41C:
		// lwz r10,452(r26)
		ctx.r10.u64 = PPC_LOAD_U32(var_r26 + 452);
		// lbz r29,389(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U8(var_r31 + 389));
		// lbz r4,388(r31)
		ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 388);
		// lwz r30,188(r10)
		var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r10.u32 + 188));
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// bl 0x820ce3f0
		pongPlayer_E3F0_p45(ctx, base);
		// mr r11,r3
		ctx.r11.u64 = ctx.r3.u64;
		// mr r4,r25
		ctx.r4.u64 = var_r25;
		// addi r3,r11,32
		ctx.r3.s64 = ctx.r11.s64 + 32;
		// stw r11,120(r30)
		PPC_STORE_U32(var_r30 + 120, ctx.r11.u32);
		// bl 0x820d7358
		pongPlayer_7358_g(ctx, base);
		// stw r29,172(r30)
		PPC_STORE_U32(var_r30 + 172, var_r29);
		// lfs f11,356(r31)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(var_r31 + 356);
		ctx.f11.f64 = double(temp.f32);
		// lwz r11,464(r26)
		ctx.r11.u64 = PPC_LOAD_U32(var_r26 + 464);
		// lfs f10,360(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 360);
		ctx.f10.f64 = double(temp.f32);
		// lwz r3,500(r26)
		ctx.r3.u64 = PPC_LOAD_U32(var_r26 + 500);
		// lfs f9,364(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 364);
		ctx.f9.f64 = double(temp.f32);
		// addi r9,r11,17
		ctx.r9.s64 = ctx.r11.s64 + 17;
		// lfs f8,368(r31)
		temp.u32 = PPC_LOAD_U32(var_r31 + 368);
		ctx.f8.f64 = double(temp.f32);
		// lis r11,-32161
		// stfs f11,128(r1)
		temp.f32 = float(ctx.f11.f64);
		PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
		// rlwinm r8,r9,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
		// stfs f10,136(r1)
		temp.f32 = float(ctx.f10.f64);
		PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
		// stfs f9,144(r1)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r1.u32 + 144, temp.u32);
		// lwz r7,96(r3)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 96);
		// stfs f8,152(r1)
		temp.f32 = float(ctx.f8.f64);
		PPC_STORE_U32(ctx.r1.u32 + 152, temp.u32);
		// lwz r11,-21720(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
		// cmpwi cr6,r7,1
		// lwzx r11,r8,r11
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r8.u32 + ctx.r11.u32);
		// lfs f7,52(r11)
		temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 52);
		ctx.f7.f64 = double(temp.f32);
		// li r11,1
		ctx.r11.s64 = 1;
		// fmr f6,f7
		ctx.f6.f64 = ctx.f7.f64;
		// stfs f7,132(r1)
		temp.f32 = float(ctx.f7.f64);
		PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
		// stfs f6,148(r1)
		temp.f32 = float(ctx.f6.f64);
		PPC_STORE_U32(ctx.r1.u32 + 148, temp.u32);
		// beq cr6,0x823ba4ac
		if (ctx.r7.s32 != 1) {
			// mr r11,r24
			ctx.r11.u64 = var_r24;
		}
	loc_823BA4AC:
		// clrlwi r5,r11,24
		ctx.r5.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r5,0
		// beq cr6,0x823ba4c4
		if (ctx.r5.u32 != 0) {
			// addi r5,r1,144
			ctx.r5.s64 = ctx.r1.s64 + 144;
			// addi r4,r1,128
			ctx.r4.s64 = ctx.r1.s64 + 128;
			// bl 0x8239ccb0
			ref_PostLoadProperties_CCB0(ctx, base);
		}
	loc_823BA4C4:
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,64
		ctx.r4.s64 = 64;
		// li r3,14362
		ctx.r3.s64 = 14362;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
		return;
	}
loc_823BA4E8:
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mr r3,r26
	ctx.r3.u64 = var_r26;
	// lfs f1,292(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 292);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82195690
	pongRemotePlayer_StartJunkSwing(ctx, base);
	return;
}

__attribute__((alias("__imp__JunkSwingStartedMessage_vfn_5"))) PPC_WEAK_FUNC(JunkSwingStartedMessage_vfn_5);
PPC_FUNC_IMPL(__imp__JunkSwingStartedMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4692
	ctx.r11.s64 = ctx.r11.s64 + 4692;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,324(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 324);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,324(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 324);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823ba548
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BA548:
	// sth r10,324(r11)
	PPC_STORE_U16(ctx.r11.u32 + 324, ctx.r10.u16);
	// lhz r10,320(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 320);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,320(r11)
	PPC_STORE_U16(ctx.r11.u32 + 320, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_A560"))) PPC_WEAK_FUNC(SinglesNetworkClient_A560);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_A560) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4692
	ctx.r11.s64 = ctx.r11.s64 + 4692;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,324(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 324);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823ba5c8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,324(r11)
		PPC_STORE_U16(ctx.r11.u32 + 324, ctx.r8.u16);
		// beq cr6,0x823ba5b4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BA5B4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,320(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 320);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,320(r11)
		PPC_STORE_U16(ctx.r11.u32 + 320, ctx.r4.u16);
		// blr
		return;
	}
loc_823BA5C8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__JunkSwingStartedMessage_vfn_6"))) PPC_WEAK_FUNC(JunkSwingStartedMessage_vfn_6);
PPC_FUNC_IMPL(__imp__JunkSwingStartedMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4680(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4680);  /* glob:lbl_825D1248 @ 0x825d1248 */
	// blr
	return;
}

__attribute__((alias("__imp__JunkSwingStartedMessage_vfn_7"))) PPC_WEAK_FUNC(JunkSwingStartedMessage_vfn_7);
PPC_FUNC_IMPL(__imp__JunkSwingStartedMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5584
	ctx.r3.s64 = ctx.r11.s64 + -5584;
	// blr
	return;
}

__attribute__((alias("__imp__HitMessage_vfn_2"))) PPC_WEAK_FUNC(HitMessage_vfn_2);
PPC_FUNC_IMPL(__imp__HitMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// lbz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__JunkSwingStartedMessage_vfn_4"))) PPC_WEAK_FUNC(JunkSwingStartedMessage_vfn_4);
PPC_FUNC_IMPL(__imp__JunkSwingStartedMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	double var_f31 = 0.0;
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f31,-16(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32253
	// lbz r10,9(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 9);
	// cmplwi cr6,r10,0
	// lfs f0,-12016(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);  /* glob:0x8206d110 */
	ctx.f0.f64 = double(temp.f32);
	// fmr f31,f0
	var_f31 = ctx.f0.f64;
	// bne cr6,0x823ba6b4
	if (ctx.r10.u32 == 0) {
		// lis r11,-32142
		// lfs f13,4(r3)
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
		ctx.f13.f64 = double(temp.f32);
		// lwz r11,-23768(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
		// lwz r10,12(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// lwz r9,4(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// lwz r8,4(r10)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
		// lfs f10,8(r10)
		temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
		ctx.f10.f64 = double(temp.f32);
		// lfs f11,292(r9)
		temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 292);
		ctx.f11.f64 = double(temp.f32);
		// extsw r7,r8
		ctx.r7.s64 = ctx.r8.s32;
		// std r7,80(r1)
		PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
		// lfd f9,80(r1)
		ctx.f9.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
		// fcfid f8,f9
		ctx.f8.f64 = double(ctx.f9.s64);
		// frsp f7,f8
		ctx.f7.f64 = double(float(ctx.f8.f64));
		// fadds f12,f10,f7
		ctx.f12.f64 = double(float(ctx.f10.f64 + ctx.f7.f64));
		// fsubs f6,f12,f13
		ctx.f6.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
		// fadds f31,f6,f11
		var_f31 = double(float(ctx.f6.f64 + ctx.f11.f64));
		// fcmpu cr6,f31,f0
		// bge cr6,0x823ba6b4
		if (var_f31 >= ctx.f0.f64) goto loc_823BA6B4;
		// fmr f31,f0
		var_f31 = ctx.f0.f64;
	}
loc_823BA6B4:
	// lbz r3,8(r3)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// bl 0x823b58e8
	SinglesNetworkClient_58E8_g(ctx, base);
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = var_f31;
	// bl 0x82195690
	pongRemotePlayer_StartJunkSwing(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-16(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncRequestMessage_vfn_5"))) PPC_WEAK_FUNC(StateSyncRequestMessage_vfn_5);
PPC_FUNC_IMPL(__imp__StateSyncRequestMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,4716
	ctx.r11.s64 = ctx.r11.s64 + 4716;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,44(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,18(r3)
	PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r5.u16);
	// lhz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823ba738
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,16(r8)
		PPC_STORE_U16(ctx.r8.u32 + 16, ctx.r9.u16);
	}
loc_823BA738:
	// lhz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 40);
	// sth r9,44(r11)
	PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,40(r11)
	PPC_STORE_U16(ctx.r11.u32 + 40, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_A750_w"))) PPC_WEAK_FUNC(SinglesNetworkClient_A750_w);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_A750_w) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4716
	ctx.r11.s64 = ctx.r11.s64 + 4716;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,44(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823ba7d4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,18(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 18);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,44(r11)
		PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823ba7c0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,16(r10)
			PPC_STORE_U16(ctx.r10.u32 + 16, ctx.r9.u16);
		}
	loc_823BA7C0:
		// sth r9,18(r3)
		PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r9.u16);
		// lhz r8,40(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 40);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,40(r11)
		PPC_STORE_U16(ctx.r11.u32 + 40, ctx.r7.u16);
		// blr
		return;
	}
loc_823BA7D4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncRequestMessage_vfn_6"))) PPC_WEAK_FUNC(StateSyncRequestMessage_vfn_6);
PPC_FUNC_IMPL(__imp__StateSyncRequestMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4704(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4704);  /* glob:lbl_825D1260 @ 0x825d1260 */
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncRequestMessage_vfn_7"))) PPC_WEAK_FUNC(StateSyncRequestMessage_vfn_7);
PPC_FUNC_IMPL(__imp__StateSyncRequestMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5560
	ctx.r3.s64 = ctx.r11.s64 + -5560;
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncRequestMessage_vfn_1"))) PPC_WEAK_FUNC(StateSyncRequestMessage_vfn_1);
PPC_FUNC_IMPL(__imp__StateSyncRequestMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
	// lfs f13,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f13.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f13,8(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 8, temp.u32);
	// bl 0x820d7970
	util_7970(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,14
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 14;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d7830
	util_7830(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncRequestMessage_vfn_2"))) PPC_WEAK_FUNC(StateSyncRequestMessage_vfn_2);
PPC_FUNC_IMPL(__imp__StateSyncRequestMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 8);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// lhz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 12);
	// li r5,16
	ctx.r5.s64 = 16;
	// extsh r4,r11
	ctx.r4.s64 = ctx.r11.s16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lhz r4,14(r31)
	ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 14);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncMessage_vfn_5"))) PPC_WEAK_FUNC(StateSyncMessage_vfn_5);
PPC_FUNC_IMPL(__imp__StateSyncMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,4740
	ctx.r11.s64 = ctx.r11.s64 + 4740;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,44(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,18(r3)
	PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r5.u16);
	// lhz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823ba958
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,16(r8)
		PPC_STORE_U16(ctx.r8.u32 + 16, ctx.r9.u16);
	}
loc_823BA958:
	// lhz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 40);
	// sth r9,44(r11)
	PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,40(r11)
	PPC_STORE_U16(ctx.r11.u32 + 40, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_A970_w"))) PPC_WEAK_FUNC(SinglesNetworkClient_A970_w);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_A970_w) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4740
	ctx.r11.s64 = ctx.r11.s64 + 4740;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,44(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823ba9f4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,18(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 18);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,44(r11)
		PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823ba9e0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,16(r10)
			PPC_STORE_U16(ctx.r10.u32 + 16, ctx.r9.u16);
		}
	loc_823BA9E0:
		// sth r9,18(r3)
		PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r9.u16);
		// lhz r8,40(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 40);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,40(r11)
		PPC_STORE_U16(ctx.r11.u32 + 40, ctx.r7.u16);
		// blr
		return;
	}
loc_823BA9F4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncMessage_vfn_6"))) PPC_WEAK_FUNC(StateSyncMessage_vfn_6);
PPC_FUNC_IMPL(__imp__StateSyncMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4728(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4728);  /* glob:lbl_825D1278 @ 0x825d1278 */
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncMessage_vfn_7"))) PPC_WEAK_FUNC(StateSyncMessage_vfn_7);
PPC_FUNC_IMPL(__imp__StateSyncMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5536
	ctx.r3.s64 = ctx.r11.s64 + -5536;
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncMessage_vfn_1"))) PPC_WEAK_FUNC(StateSyncMessage_vfn_1);
PPC_FUNC_IMPL(__imp__StateSyncMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x820d7970
	util_7970(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lfs f13,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,8(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(var_r31 + 8, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__StateSyncMessage_vfn_2"))) PPC_WEAK_FUNC(StateSyncMessage_vfn_2);
PPC_FUNC_IMPL(__imp__StateSyncMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// lhz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 12);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// extsh r4,r11
	ctx.r4.s64 = ctx.r11.s16;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 8);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestMessage_vfn_5"))) PPC_WEAK_FUNC(DataRequestMessage_vfn_5);
PPC_FUNC_IMPL(__imp__DataRequestMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4764
	ctx.r11.s64 = ctx.r11.s64 + 4764;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bab38
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BAB38:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_AB50"))) PPC_WEAK_FUNC(SinglesNetworkClient_AB50);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_AB50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4764
	ctx.r11.s64 = ctx.r11.s64 + 4764;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823babb8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823baba4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BABA4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823BABB8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestMessage_vfn_6"))) PPC_WEAK_FUNC(DataRequestMessage_vfn_6);
PPC_FUNC_IMPL(__imp__DataRequestMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4752(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4752);  /* glob:lbl_825D1290 @ 0x825d1290 */
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestMessage_vfn_7"))) PPC_WEAK_FUNC(DataRequestMessage_vfn_7);
PPC_FUNC_IMPL(__imp__DataRequestMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5516
	ctx.r3.s64 = ctx.r11.s64 + -5516;
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestMessage_vfn_1"))) PPC_WEAK_FUNC(DataRequestMessage_vfn_1);
PPC_FUNC_IMPL(__imp__DataRequestMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x820d7970
	util_7970(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__DataSendMessage_vfn_5"))) PPC_WEAK_FUNC(DataSendMessage_vfn_5);
PPC_FUNC_IMPL(__imp__DataSendMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-1501
	// addi r11,r11,4788
	ctx.r11.s64 = ctx.r11.s64 + 4788;
	// ori r8,r9,11507
	ctx.r8.u64 = ctx.r9.u64 | 11507;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,1052(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1052);
	// rlwinm r6,r7,23,9,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 23) & 0x7FFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,524
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(524));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,522(r3)
	PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r5.u16);
	// lhz r9,1052(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1052);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bac90
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,520(r9)
		PPC_STORE_U16(ctx.r9.u32 + 520, ctx.r10.u16);
	}
loc_823BAC90:
	// sth r10,1052(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1052, ctx.r10.u16);
	// lhz r10,1048(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1048);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,1048(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1048, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_ACA8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_ACA8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_ACA8_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4788
	ctx.r11.s64 = ctx.r11.s64 + 4788;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,1052(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1052);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bad14
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,524
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(524));
		// lhz r8,522(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 522);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,1052(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1052, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bad00
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,524
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(524));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,520(r5)
			PPC_STORE_U16(ctx.r5.u32 + 520, ctx.r9.u16);
		}
	loc_823BAD00:
		// sth r9,522(r3)
		PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r9.u16);
		// lhz r4,1048(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1048);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,1048(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1048, ctx.r10.u16);
		// blr
		return;
	}
loc_823BAD14:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__DataSendMessage_vfn_6"))) PPC_WEAK_FUNC(DataSendMessage_vfn_6);
PPC_FUNC_IMPL(__imp__DataSendMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4776(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4776);  /* glob:lbl_825D12A8 @ 0x825d12a8 */
	// blr
	return;
}

__attribute__((alias("__imp__DataSendMessage_vfn_7"))) PPC_WEAK_FUNC(DataSendMessage_vfn_7);
PPC_FUNC_IMPL(__imp__DataSendMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5496
	ctx.r3.s64 = ctx.r11.s64 + -5496;
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestFailedMessage_vfn_5"))) PPC_WEAK_FUNC(DataRequestFailedMessage_vfn_5);
PPC_FUNC_IMPL(__imp__DataRequestFailedMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4812
	ctx.r11.s64 = ctx.r11.s64 + 4812;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bad80
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BAD80:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_AD98"))) PPC_WEAK_FUNC(game_AD98);
PPC_FUNC_IMPL(__imp__game_AD98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4812
	ctx.r11.s64 = ctx.r11.s64 + 4812;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bae00
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823badec
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BADEC:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823BAE00:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestFailedMessage_vfn_6"))) PPC_WEAK_FUNC(DataRequestFailedMessage_vfn_6);
PPC_FUNC_IMPL(__imp__DataRequestFailedMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4800(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4800);  /* glob:lbl_825D12C0 @ 0x825d12c0 */
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestFailedMessage_vfn_7"))) PPC_WEAK_FUNC(DataRequestFailedMessage_vfn_7);
PPC_FUNC_IMPL(__imp__DataRequestFailedMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5480
	ctx.r3.s64 = ctx.r11.s64 + -5480;
	// blr
	return;
}

__attribute__((alias("__imp__DataRequestMessage_vfn_2"))) PPC_WEAK_FUNC(DataRequestMessage_vfn_2);
PPC_FUNC_IMPL(__imp__DataRequestMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// lhz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 8);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// extsh r4,r11
	ctx.r4.s64 = ctx.r11.s16;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesConnectRequest_vfn_5"))) PPC_WEAK_FUNC(SinglesConnectRequest_vfn_5);
PPC_FUNC_IMPL(__imp__SinglesConnectRequest_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,4836
	ctx.r11.s64 = ctx.r11.s64 + 4836;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823baee0
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BAEE0:
	// lhz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
	// sth r9,16(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesConnectRequest_vfn_6"))) PPC_WEAK_FUNC(SinglesConnectRequest_vfn_6);
PPC_FUNC_IMPL(__imp__SinglesConnectRequest_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4824(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4824);  /* glob:lbl_825D12D8 @ 0x825d12d8 */
	// blr
	return;
}

__attribute__((alias("__imp__SinglesConnectRequest_vfn_7"))) PPC_WEAK_FUNC(SinglesConnectRequest_vfn_7);
PPC_FUNC_IMPL(__imp__SinglesConnectRequest_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5452
	ctx.r3.s64 = ctx.r11.s64 + -5452;
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorConnectRequest_vfn_5"))) PPC_WEAK_FUNC(SpectatorConnectRequest_vfn_5);
PPC_FUNC_IMPL(__imp__SpectatorConnectRequest_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,4860
	ctx.r11.s64 = ctx.r11.s64 + 4860;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823baf78
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BAF78:
	// lhz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
	// sth r9,16(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_AF90_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_AF90_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_AF90_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4860
	ctx.r11.s64 = ctx.r11.s64 + 4860;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bb014
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,16(r11)
		PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bb000
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BB000:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,12(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,12(r11)
		PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
		// blr
		return;
	}
loc_823BB014:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorConnectRequest_vfn_6"))) PPC_WEAK_FUNC(SpectatorConnectRequest_vfn_6);
PPC_FUNC_IMPL(__imp__SpectatorConnectRequest_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4848(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4848);  /* glob:lbl_825D12F0 @ 0x825d12f0 */
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorConnectRequest_vfn_7"))) PPC_WEAK_FUNC(SpectatorConnectRequest_vfn_7);
PPC_FUNC_IMPL(__imp__SpectatorConnectRequest_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5428
	ctx.r3.s64 = ctx.r11.s64 + -5428;
	// blr
	return;
}

__attribute__((alias("__imp__DropSpectatorConnection_vfn_5"))) PPC_WEAK_FUNC(DropSpectatorConnection_vfn_5);
PPC_FUNC_IMPL(__imp__DropSpectatorConnection_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4884
	ctx.r11.s64 = ctx.r11.s64 + 4884;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bb080
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BB080:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_B098_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_B098_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_B098_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4884
	ctx.r11.s64 = ctx.r11.s64 + 4884;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bb100
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823bb0ec
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BB0EC:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823BB100:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__DropSpectatorConnection_vfn_6"))) PPC_WEAK_FUNC(DropSpectatorConnection_vfn_6);
PPC_FUNC_IMPL(__imp__DropSpectatorConnection_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4872(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4872);  /* glob:lbl_825D1308 @ 0x825d1308 */
	// blr
	return;
}

__attribute__((alias("__imp__DropSpectatorConnection_vfn_7"))) PPC_WEAK_FUNC(DropSpectatorConnection_vfn_7);
PPC_FUNC_IMPL(__imp__DropSpectatorConnection_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5404
	ctx.r3.s64 = ctx.r11.s64 + -5404;
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorQuitMessage_vfn_5"))) PPC_WEAK_FUNC(SpectatorQuitMessage_vfn_5);
PPC_FUNC_IMPL(__imp__SpectatorQuitMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4908
	ctx.r11.s64 = ctx.r11.s64 + 4908;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,484(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 484);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,484(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 484);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bb168
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BB168:
	// sth r10,484(r11)
	PPC_STORE_U16(ctx.r11.u32 + 484, ctx.r10.u16);
	// lhz r10,480(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 480);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,480(r11)
	PPC_STORE_U16(ctx.r11.u32 + 480, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_B180"))) PPC_WEAK_FUNC(game_B180);
PPC_FUNC_IMPL(__imp__game_B180) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4908
	ctx.r11.s64 = ctx.r11.s64 + 4908;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,484(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 484);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bb1e8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,484(r11)
		PPC_STORE_U16(ctx.r11.u32 + 484, ctx.r8.u16);
		// beq cr6,0x823bb1d4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BB1D4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,480(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 480);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,480(r11)
		PPC_STORE_U16(ctx.r11.u32 + 480, ctx.r4.u16);
		// blr
		return;
	}
loc_823BB1E8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorQuitMessage_vfn_6"))) PPC_WEAK_FUNC(SpectatorQuitMessage_vfn_6);
PPC_FUNC_IMPL(__imp__SpectatorQuitMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4896(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4896);  /* glob:lbl_825D1320 @ 0x825d1320 */
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorQuitMessage_vfn_7"))) PPC_WEAK_FUNC(SpectatorQuitMessage_vfn_7);
PPC_FUNC_IMPL(__imp__SpectatorQuitMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5380
	ctx.r3.s64 = ctx.r11.s64 + -5380;
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorQuitMessage_vfn_1"))) PPC_WEAK_FUNC(SpectatorQuitMessage_vfn_1);
PPC_FUNC_IMPL(__imp__SpectatorQuitMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260e18
	SinglesNetworkClient_0E18_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__SpectatorQuitMessage_vfn_2"))) PPC_WEAK_FUNC(SpectatorQuitMessage_vfn_2);
PPC_FUNC_IMPL(__imp__SpectatorQuitMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// lwz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__AcceptMessage_vfn_5"))) PPC_WEAK_FUNC(AcceptMessage_vfn_5);
PPC_FUNC_IMPL(__imp__AcceptMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-17713
	// addi r11,r11,4932
	ctx.r11.s64 = ctx.r11.s64 + 4932;
	// ori r8,r9,37197
	ctx.r8.u64 = ctx.r9.u64 | 37197;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r9,r10,r8
	ctx.r9.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r7,596(r11)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r11.u32 + 596);
	// subf r6,r9,r10
	ctx.r6.s64 = ctx.r10.s64 - ctx.r9.s64;
	// rlwinm r10,r6,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 31) & 0x7FFFFFFF;
	// add r5,r10,r9
	ctx.r5.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rlwinm r4,r5,24,8,31
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 24) & 0xFFFFFF;
	// clrlwi r10,r4,16
	ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
	// mr r3,r10
	ctx.r3.u64 = ctx.r10.u64;
	// mulli r9,r3,296
	ctx.r9.s64 = static_cast<int64_t>(ctx.r3.u64 * static_cast<uint64_t>(296));
	// add r9,r9,r11
	ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r7,290(r9)
	PPC_STORE_U16(ctx.r9.u32 + 290, ctx.r7.u16);
	// lhz r9,596(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 596);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bb31c
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,296
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(296));
		// add r8,r9,r11
		ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,288(r8)
		PPC_STORE_U16(ctx.r8.u32 + 288, ctx.r10.u16);
	}
loc_823BB31C:
	// sth r10,596(r11)
	PPC_STORE_U16(ctx.r11.u32 + 596, ctx.r10.u16);
	// lhz r10,592(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 592);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,592(r11)
	PPC_STORE_U16(ctx.r11.u32 + 592, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__AcceptMessage_vfn_6"))) PPC_WEAK_FUNC(AcceptMessage_vfn_6);
PPC_FUNC_IMPL(__imp__AcceptMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4920(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4920);  /* glob:lbl_825D1338 @ 0x825d1338 */
	// blr
	return;
}

__attribute__((alias("__imp__AcceptMessage_vfn_7"))) PPC_WEAK_FUNC(AcceptMessage_vfn_7);
PPC_FUNC_IMPL(__imp__AcceptMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5356
	ctx.r3.s64 = ctx.r11.s64 + -5356;
	// blr
	return;
}

__attribute__((alias("__imp__AcceptMessage_vfn_1"))) PPC_WEAK_FUNC(AcceptMessage_vfn_1);
PPC_FUNC_IMPL(__imp__AcceptMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,280
	ctx.r5.s64 = 280;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__AcceptMessage_vfn_2"))) PPC_WEAK_FUNC(AcceptMessage_vfn_2);
PPC_FUNC_IMPL(__imp__AcceptMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,280
	ctx.r5.s64 = 280;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_5"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_5);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,4956
	ctx.r11.s64 = ctx.r11.s64 + 4956;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,164(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 164);
	// rlwinm r6,r7,26,6,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 26) & 0x3FFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 4) & 0xFFFFFFF0;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,74(r3)
	PPC_STORE_U16(ctx.r3.u32 + 74, ctx.r5.u16);
	// lhz r10,164(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 164);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bb460
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,72(r8)
		PPC_STORE_U16(ctx.r8.u32 + 72, ctx.r9.u16);
	}
loc_823BB460:
	// lhz r10,160(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 160);
	// sth r9,164(r11)
	PPC_STORE_U16(ctx.r11.u32 + 164, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,160(r11)
	PPC_STORE_U16(ctx.r11.u32 + 160, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__util_B478"))) PPC_WEAK_FUNC(util_B478);
PPC_FUNC_IMPL(__imp__util_B478) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,4956
	ctx.r11.s64 = ctx.r11.s64 + 4956;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,164(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 164);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bb4fc
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,74(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 74);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,4,0,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 4) & 0xFFFFFFF0;
		// sth r7,164(r11)
		PPC_STORE_U16(ctx.r11.u32 + 164, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bb4e8
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 4) & 0xFFFFFFF0;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,72(r10)
			PPC_STORE_U16(ctx.r10.u32 + 72, ctx.r9.u16);
		}
	loc_823BB4E8:
		// sth r9,74(r3)
		PPC_STORE_U16(ctx.r3.u32 + 74, ctx.r9.u16);
		// lhz r8,160(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 160);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,160(r11)
		PPC_STORE_U16(ctx.r11.u32 + 160, ctx.r7.u16);
		// blr
		return;
	}
loc_823BB4FC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_6"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_6);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4944(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4944);  /* glob:lbl_825D1350 @ 0x825d1350 */
	// blr
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_7"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_7);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5340
	ctx.r3.s64 = ctx.r11.s64 + -5340;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_B528_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_B528_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_B528_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	// FRAME: size=160, savegprlr_24
	// lis r11,-32142
	// lis r10,1
	ctx.r10.s64 = 65536;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// ori r26,r10,36256
	var_r26 = (uint32_t)(ctx.r10.u64 | 36256);
	// lis r10,-32142
	// lwz r25,-23780(r11)
	var_r25 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + -23780));  /* glob:0x8206a31c */
	// li r24,0
	var_r24 = 0;
	// lwz r27,-22608(r10)
	var_r27 = (uint32_t)(PPC_LOAD_U32(ctx.r10.u32 + -22608));
	// lwz r11,4(r25)
	ctx.r11.u64 = PPC_LOAD_U32(var_r25 + 4);
	// cmplwi cr6,r11,3
	// bgt cr6,0x823bb570
	if (ctx.r11.u32 <= 3) {
		// mulli r11,r11,2784
		ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(2784));
		// add r9,r11,r27
		ctx.r9.u64 = ctx.r11.u64 + var_r27;
		// add r11,r9,r26
		ctx.r11.u64 = ctx.r9.u64 + var_r26;
		// b 0x823bb574
	} else {
	loc_823BB570:
		// mr r11,r24
		ctx.r11.u64 = var_r24;
	}
loc_823BB574:
	// addi r30,r11,2652
	var_r30 = (uint32_t)(ctx.r11.s64 + 2652);  // addr:0x82720a5c
	// lis r11,-32161
	// li r7,48
	ctx.r7.s64 = 48;
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// lwz r8,76(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 76);
	// stb r8,8(r31)
	PPC_STORE_U8(var_r31 + 8, ctx.r8.u8);
	// lwz r3,80(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// stb r3,9(r31)
	PPC_STORE_U8(var_r31 + 9, ctx.r3.u8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r9,88(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// stb r9,10(r31)
	PPC_STORE_U8(var_r31 + 10, ctx.r9.u8);
	// lwz r6,92(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 92);
	// stb r6,11(r31)
	PPC_STORE_U8(var_r31 + 11, ctx.r6.u8);
	// lwz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// stb r10,12(r31)
	PPC_STORE_U8(var_r31 + 12, ctx.r10.u8);
	// lis r10,-32142
	ctx.r10.s64 = -2106458112;
	// lwz r29,-23768(r10)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r10.u32 + -23768));  /* glob:lbl_8271A328 @ 0x8271a328 */
	// lwz r8,64(r29)
	ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 64);
	// stb r8,13(r31)
	PPC_STORE_U8(var_r31 + 13, ctx.r8.u8);
	// lbz r11,333(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 333);
	// stb r11,14(r31)
	PPC_STORE_U8(var_r31 + 14, ctx.r11.u8);
	// lwz r28,28(r30)
	var_r28 = (uint32_t)(PPC_LOAD_U32(var_r30 + 28));
	// stw r7,28(r30)
	PPC_STORE_U32(var_r30 + 28, ctx.r7.u32);
	// bl 0x82260e18
	SinglesNetworkClient_0E18_g(ctx, base);
	// lwz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// lis r11,-32163
	// stw r28,28(r30)
	PPC_STORE_U32(var_r30 + 28, var_r28);
	// stw r10,64(r31)
	PPC_STORE_U32(var_r31 + 64, ctx.r10.u32);
	// lis r10,-32160
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,0
	// lwz r10,25500(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 25500);
	// bne cr6,0x823bb698
	if (ctx.r11.s32 == 0) {
		// cmplwi cr6,r10,0
		// beq cr6,0x823bb698
		if (ctx.r10.u32 == 0) goto loc_823BB698;
		// lwz r11,4(r25)
		ctx.r11.u64 = PPC_LOAD_U32(var_r25 + 4);
		// cmplwi cr6,r11,3
		// bgt cr6,0x823bb624
		if (ctx.r11.u32 <= 3) {
			// mulli r11,r11,2784
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(2784));
			// add r9,r11,r27
			ctx.r9.u64 = ctx.r11.u64 + var_r27;
			// add r11,r9,r26
			ctx.r11.u64 = ctx.r9.u64 + var_r26;
			// b 0x823bb628
		} else {
		loc_823BB624:
			// mr r11,r24
			ctx.r11.u64 = var_r24;
		}
	loc_823BB628:
		// addi r11,r11,96
		ctx.r11.s64 = ctx.r11.s64 + 96;
		// addi r10,r31,16
		ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 16;
		// lis r8,1
		ctx.r8.s64 = 65536;
		// ori r7,r8,62644
		ctx.r7.u64 = ctx.r8.u64 | 62644;
		// ld r6,0(r11)
		ctx.r6.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
		// std r6,0(r10)
		PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r6.u64);
		// ld r5,8(r11)
		ctx.r5.u64 = PPC_LOAD_U64(ctx.r11.u32 + 8);
		// std r5,8(r10)
		PPC_STORE_U64(ctx.r10.u32 + 8, ctx.r5.u64);
		// ld r4,16(r11)
		ctx.r4.u64 = PPC_LOAD_U64(ctx.r11.u32 + 16);
		// std r4,16(r10)
		PPC_STORE_U64(ctx.r10.u32 + 16, ctx.r4.u64);
		// lwzx r3,r27,r7
		ctx.r3.u64 = PPC_LOAD_U32(var_r27 + ctx.r7.u32);
		// cmpwi cr6,r3,-1
		// beq cr6,0x823bb688
		if (ctx.r3.s32 != -1) {
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x823acf68
			PongNetRoundRobinCoordinator_CF68_g(ctx, base);
			// ld r10,0(r3)
			ctx.r10.u64 = PPC_LOAD_U64(ctx.r3.u32 + 0);
			// addi r11,r31,40
			ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 40;
			// std r10,0(r11)
			PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r10.u64);
			// ld r9,8(r3)
			ctx.r9.u64 = PPC_LOAD_U64(ctx.r3.u32 + 8);
			// std r9,8(r11)
			PPC_STORE_U64(ctx.r11.u32 + 8, ctx.r9.u64);
			// ld r8,16(r3)
			ctx.r8.u64 = PPC_LOAD_U64(ctx.r3.u32 + 16);
			// std r8,16(r11)
			PPC_STORE_U64(ctx.r11.u32 + 16, ctx.r8.u64);
			return;
		}
	loc_823BB688:
		// stb r24,40(r31)
		PPC_STORE_U8(var_r31 + 40, (uint8_t)var_r24);
		// std r24,56(r31)
		PPC_STORE_U64(var_r31 + 56, var_r24);
		return;
	}
loc_823BB698:
	// cmpwi cr6,r11,2
	// bne cr6,0x823bb70c
	if (ctx.r11.s32 == 2) {
		// cmplwi cr6,r10,0
		// beq cr6,0x823bb70c
		if (ctx.r10.u32 == 0) {
			return;
		}
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// bl 0x82391378
		SinglesNetworkClient_1378_g(ctx, base);
		// lis r11,-32162
		// li r7,0
		ctx.r7.s64 = 0;
		// addi r6,r11,-4544
		ctx.r6.s64 = ctx.r11.s64 + -4544;
		// lis r11,-32162
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r5,r11,-4580
		ctx.r5.s64 = ctx.r11.s64 + -4580;
		// bl 0x82430978
		SinglesNetworkClient_0978_g(ctx, base);
		// addi r9,r3,4464
		ctx.r9.s64 = ctx.r3.s64 + 4464;
		// addi r11,r31,16
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r8,r3,4488
		ctx.r8.s64 = ctx.r3.s64 + 4488;
		// addi r10,r31,40
		ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 40;
		// ld r7,0(r9)
		ctx.r7.u64 = PPC_LOAD_U64(ctx.r9.u32 + 0);
		// std r7,0(r11)
		PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r7.u64);
		// ld r6,8(r9)
		ctx.r6.u64 = PPC_LOAD_U64(ctx.r9.u32 + 8);
		// std r6,8(r11)
		PPC_STORE_U64(ctx.r11.u32 + 8, ctx.r6.u64);
		// ld r5,16(r9)
		ctx.r5.u64 = PPC_LOAD_U64(ctx.r9.u32 + 16);
		// std r5,16(r11)
		PPC_STORE_U64(ctx.r11.u32 + 16, ctx.r5.u64);
		// ld r4,0(r8)
		ctx.r4.u64 = PPC_LOAD_U64(ctx.r8.u32 + 0);
		// std r4,0(r10)
		PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r4.u64);
		// ld r3,8(r8)
		ctx.r3.u64 = PPC_LOAD_U64(ctx.r8.u32 + 8);
		// std r3,8(r10)
		PPC_STORE_U64(ctx.r10.u32 + 8, ctx.r3.u64);
		// ld r11,16(r8)
		ctx.r11.u64 = PPC_LOAD_U64(ctx.r8.u32 + 16);
		// std r11,16(r10)
		PPC_STORE_U64(ctx.r10.u32 + 16, ctx.r11.u64);
	}
loc_823BB70C:
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_1"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_1);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// addi r30,r29,8
	var_r30 = (uint32_t)(var_r29 + 8);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r29 + 4, temp.u32);
	// bl 0x82390748
	SinglesNetworkClient_0748_h(ctx, base);
	// addi r29,r30,8
	var_r29 = (uint32_t)(var_r30 + 8);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r4,r29,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r29 + 16;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260a70
	SinglesNetworkClient_0A70_g(ctx, base);
	// addi r29,r30,32
	var_r29 = (uint32_t)(var_r30 + 32);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r4,r29,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r29 + 16;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260a70
	SinglesNetworkClient_0A70_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r30,56
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 56;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260e18
	SinglesNetworkClient_0E18_g(ctx, base);
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_2"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_2);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=112, savegprlr_29
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,4(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// addi r30,r30,8
	var_r30 = (uint32_t)(var_r30 + 8);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82390808
	SinglesNetworkClient_0808_g(ctx, base);
	// addi r29,r30,8
	var_r29 = (uint32_t)(var_r30 + 8);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r4,r29,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r29 + 16;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x821d9ad8
	SinglesNetworkClient_9AD8_g(ctx, base);
	// addi r29,r30,32
	var_r29 = (uint32_t)(var_r30 + 32);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// addi r4,r29,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r29 + 16;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x821d9ad8
	SinglesNetworkClient_9AD8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r4,56(r30)
	ctx.r4.u64 = PPC_LOAD_U32(var_r30 + 56);
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_4"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_4);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// bl 0x823bb888
	SinglesNetworkClient_B888_g(ctx, base);
	// lwz r11,0(r31)
  // [ph4a] vtable load collapsed
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lwz r10,12(r11)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r31, 3, ctx, base);  // pattern-B slot 3 (byte +12)
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_B888_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_B888_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_B888_g) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lbz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 8);
	// lbz r9,9(r31)
	ctx.r9.u64 = PPC_LOAD_U8(var_r31 + 9);
	// extsb r10,r11
	ctx.r10.s64 = ctx.r11.s8;
	// lis r11,-32161
	// extsb r9,r9
	ctx.r9.s64 = ctx.r9.s8;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// stw r10,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r10.u32);
	// stw r10,100(r11)
	PPC_STORE_U32(ctx.r11.u32 + 100, ctx.r10.u32);
	// stw r9,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r9.u32);
	// stw r9,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r9.u32);
	// lbz r8,10(r31)
	ctx.r8.u64 = PPC_LOAD_U8(var_r31 + 10);
	// lbz r7,11(r31)
	ctx.r7.u64 = PPC_LOAD_U8(var_r31 + 11);
	// extsb r10,r8
	ctx.r10.s64 = ctx.r8.s8;
	// extsb r9,r7
	ctx.r9.s64 = ctx.r7.s8;
	// stw r10,88(r11)
	PPC_STORE_U32(ctx.r11.u32 + 88, ctx.r10.u32);
	// stw r10,112(r11)
	PPC_STORE_U32(ctx.r11.u32 + 112, ctx.r10.u32);
	// stw r9,92(r11)
	PPC_STORE_U32(ctx.r11.u32 + 92, ctx.r9.u32);
	// stw r9,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r9.u32);
	// lbz r6,12(r31)
	ctx.r6.u64 = PPC_LOAD_U8(var_r31 + 12);
	// extsb r5,r6
	ctx.r5.s64 = ctx.r6.s8;
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// lbz r4,14(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 14);
	// stb r4,333(r11)
	PPC_STORE_U8(ctx.r11.u32 + 333, ctx.r4.u8);
	// lis r11,-32142
	ctx.r11.s64 = -2106458112;
	// lwz r3,-23768(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);  /* glob:lbl_8271A328 @ 0x8271a328 */
	// lwz r11,36(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmpwi cr6,r11,1
	// bne cr6,0x823bb9bc
	if (ctx.r11.s32 == 1) {
		// lis r11,-32142
		ctx.r11.s64 = -2106458112;
		// lwz r11,-23780(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23780);
		// lwz r11,4(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// cmplwi cr6,r11,3
		// bgt cr6,0x823bb940
		if (ctx.r11.u32 <= 3) {
			// mulli r10,r11,2784
			ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(2784));
			// lis r11,-32142
			ctx.r11.s64 = -2106458112;
			// lwz r11,-22608(r11)
			ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22608);
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// addis r30,r10,2
			var_r30 = (uint32_t)(ctx.r10.s64 + 131072);  // addr:0x82620000
			// addi r30,r30,-29280
			var_r30 = (uint32_t)(var_r30 + -29280);
			// b 0x823bb944
		} else {
		loc_823BB940:
			// li r30,0
			var_r30 = 0;
		}
	loc_823BB944:
		// bl 0x82391378
		SinglesNetworkClient_1378_g(ctx, base);
		// lis r11,-32162
		// li r7,0
		ctx.r7.s64 = 0;
		// addi r6,r11,-4544
		ctx.r6.s64 = ctx.r11.s64 + -4544;
		// lis r11,-32162
		// li r4,0
		ctx.r4.s64 = 0;
		// addi r5,r11,-4580
		ctx.r5.s64 = ctx.r11.s64 + -4580;
		// bl 0x82430978
		SinglesNetworkClient_0978_g(ctx, base);
		// mr r9,r3
		ctx.r9.u64 = ctx.r3.u64;
		// addi r11,r31,16
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 16;
		// addi r8,r9,4464
		ctx.r8.s64 = ctx.r9.s64 + 4464;
		// addi r7,r9,4488
		ctx.r7.s64 = ctx.r9.s64 + 4488;
		// mr r9,r8
		ctx.r9.u64 = ctx.r8.u64;
		// addi r10,r31,40
		ctx.r10.s64 = (int64_t)(int32_t)var_r31 + 40;
		// ld r6,0(r11)
		ctx.r6.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
		// mr r8,r7
		ctx.r8.u64 = ctx.r7.u64;
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// std r6,0(r9)
		PPC_STORE_U64(ctx.r9.u32 + 0, ctx.r6.u64);
		// ld r5,8(r11)
		ctx.r5.u64 = PPC_LOAD_U64(ctx.r11.u32 + 8);
		// std r5,8(r9)
		PPC_STORE_U64(ctx.r9.u32 + 8, ctx.r5.u64);
		// ld r4,16(r11)
		ctx.r4.u64 = PPC_LOAD_U64(ctx.r11.u32 + 16);
		// std r4,16(r9)
		PPC_STORE_U64(ctx.r9.u32 + 16, ctx.r4.u64);
		// ld r11,0(r10)
		ctx.r11.u64 = PPC_LOAD_U64(ctx.r10.u32 + 0);
		// std r11,0(r8)
		PPC_STORE_U64(ctx.r8.u32 + 0, ctx.r11.u64);
		// ld r9,8(r10)
		ctx.r9.u64 = PPC_LOAD_U64(ctx.r10.u32 + 8);
		// std r9,8(r8)
		PPC_STORE_U64(ctx.r8.u32 + 8, ctx.r9.u64);
		// ld r7,16(r10)
		ctx.r7.u64 = PPC_LOAD_U64(ctx.r10.u32 + 16);
		// std r7,16(r8)
		PPC_STORE_U64(ctx.r8.u32 + 16, ctx.r7.u64);
		// lwz r4,64(r31)
		ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 64);
		// bl 0x823b4e80
		SinglesNetworkClient_4E80_g(ctx, base);
	}
loc_823BB9BC:
	// blr
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_vfn_3"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_vfn_3);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_vfn_3) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=192, manual
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r10,r11,40
	ctx.r10.s64 = ctx.r11.s64 + 40;
	// mr r31,r10
	var_r31 = ctx.r10.u32;
	// lbz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmplwi cr6,r9,0
	// lis r9,-32253
	// addi r9,r9,-5732
	ctx.r9.s64 = ctx.r9.s64 + -5732;
	// bne cr6,0x823bba10
	if (ctx.r9.u32 == 0) {
		// mr r31,r9
		var_r31 = ctx.r9.u32;
	}
loc_823BBA10:
	// addi r10,r11,16
	ctx.r10.s64 = ctx.r11.s64 + 16;
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmplwi cr6,r8,0
	// bne cr6,0x823bba24
	if (ctx.r8.u32 == 0) {
		// mr r10,r9
		ctx.r10.u64 = ctx.r9.u64;
	}
loc_823BBA24:
	// lbz r7,14(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 14);
	// cmplwi cr6,r7,0
	// beq cr6,0x823bba3c
	if (ctx.r7.u32 != 0) {
		// lis r9,-32250
		// addi r9,r9,16404
		ctx.r9.s64 = ctx.r9.s64 + 16404;
		// b 0x823bba44
	} else {
	loc_823BBA3C:
		// lis r9,-32249
		// addi r9,r9,-3068
		ctx.r9.s64 = ctx.r9.s64 + -3068;
	}
loc_823BBA44:
	// lis r8,-32249
	// ld r4,56(r11)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r11.u32 + 56);
	// ld r30,32(r11)
	var_r30 = (uint32_t)(PPC_LOAD_U64(ctx.r11.u32 + 32));
	// addi r3,r8,-3056
	ctx.r3.s64 = ctx.r8.s64 + -3056;
	// lbz r7,11(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 11);
	// lbz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 12);
	// lbz r6,10(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 10);
	// extsb r7,r7
	ctx.r7.s64 = ctx.r7.s8;
	// lbz r5,9(r11)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r11.u32 + 9);
	// extsb r8,r8
	ctx.r8.s64 = ctx.r8.s8;
	// lbz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// extsb r6,r6
	ctx.r6.s64 = ctx.r6.s8;
	// std r4,96(r1)
	PPC_STORE_U64(ctx.r1.u32 + 96, ctx.r4.u64);
	// extsb r5,r5
	ctx.r5.s64 = ctx.r5.s8;
	// extsb r4,r11
	ctx.r4.s64 = ctx.r11.s8;
	// stw r31,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, var_r31);
	// std r30,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, var_r30);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__DenySpectatorMessage_vfn_5"))) PPC_WEAK_FUNC(DenySpectatorMessage_vfn_5);
PPC_FUNC_IMPL(__imp__DenySpectatorMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,4980
	ctx.r11.s64 = ctx.r11.s64 + 4980;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bbb08
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BBB08:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__DenySpectatorMessage_vfn_6"))) PPC_WEAK_FUNC(DenySpectatorMessage_vfn_6);
PPC_FUNC_IMPL(__imp__DenySpectatorMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4968(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4968);  /* glob:lbl_825D1368 @ 0x825d1368 */
	// blr
	return;
}

__attribute__((alias("__imp__DenySpectatorMessage_vfn_7"))) PPC_WEAK_FUNC(DenySpectatorMessage_vfn_7);
PPC_FUNC_IMPL(__imp__DenySpectatorMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5316
	ctx.r3.s64 = ctx.r11.s64 + -5316;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesConnectRequest_vfn_2"))) PPC_WEAK_FUNC(SinglesConnectRequest_vfn_2);
PPC_FUNC_IMPL(__imp__SinglesConnectRequest_vfn_2) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// b 0x820d6990
	game_6990(ctx, base);
	return;
}

__attribute__((alias("__imp__RemoveSpectatorMessage_vfn_5"))) PPC_WEAK_FUNC(RemoveSpectatorMessage_vfn_5);
PPC_FUNC_IMPL(__imp__RemoveSpectatorMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5004
	ctx.r11.s64 = ctx.r11.s64 + 5004;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bbbb0
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BBBB0:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__AcceptSpectatorMessage_BBC8_p46"))) PPC_WEAK_FUNC(AcceptSpectatorMessage_BBC8_p46);
PPC_FUNC_IMPL(__imp__AcceptSpectatorMessage_BBC8_p46) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5004
	ctx.r11.s64 = ctx.r11.s64 + 5004;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bbc4c
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bbc38
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BBC38:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BBC4C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__RemoveSpectatorMessage_vfn_6"))) PPC_WEAK_FUNC(RemoveSpectatorMessage_vfn_6);
PPC_FUNC_IMPL(__imp__RemoveSpectatorMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,4992(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4992);  /* glob:lbl_825D1380 @ 0x825d1380 */
	// blr
	return;
}

__attribute__((alias("__imp__RemoveSpectatorMessage_vfn_7"))) PPC_WEAK_FUNC(RemoveSpectatorMessage_vfn_7);
PPC_FUNC_IMPL(__imp__RemoveSpectatorMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5292
	ctx.r3.s64 = ctx.r11.s64 + -5292;
	// blr
	return;
}

__attribute__((alias("__imp__JoinSpectatorSessionMessage_vfn_5"))) PPC_WEAK_FUNC(JoinSpectatorSessionMessage_vfn_5);
PPC_FUNC_IMPL(__imp__JoinSpectatorSessionMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,12136
	ctx.r9.s64 = 795344896;
	// addi r11,r11,5028
	ctx.r11.s64 = ctx.r11.s64 + 5028;
	// ori r8,r9,19419
	ctx.r8.u64 = ctx.r9.u64 | 19419;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r9,r10,r8
	ctx.r9.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r7,220(r11)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r11.u32 + 220);
	// subf r6,r9,r10
	ctx.r6.s64 = ctx.r10.s64 - ctx.r9.s64;
	// rlwinm r10,r6,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 31) & 0x7FFFFFFF;
	// add r5,r10,r9
	ctx.r5.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rlwinm r4,r5,26,6,31
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 26) & 0x3FFFFFF;
	// clrlwi r10,r4,16
	ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
	// mr r3,r10
	ctx.r3.u64 = ctx.r10.u64;
	// mulli r9,r3,108
	ctx.r9.s64 = static_cast<int64_t>(ctx.r3.u64 * static_cast<uint64_t>(108));
	// add r9,r9,r11
	ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r7,106(r9)
	PPC_STORE_U16(ctx.r9.u32 + 106, ctx.r7.u16);
	// lhz r9,220(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 220);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bbcd4
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,108
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(108));
		// add r8,r9,r11
		ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,104(r8)
		PPC_STORE_U16(ctx.r8.u32 + 104, ctx.r10.u16);
	}
loc_823BBCD4:
	// sth r10,220(r11)
	PPC_STORE_U16(ctx.r11.u32 + 220, ctx.r10.u16);
	// lhz r10,216(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 216);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,216(r11)
	PPC_STORE_U16(ctx.r11.u32 + 216, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetGameModeCoordinator_BCE8_p23"))) PPC_WEAK_FUNC(PongNetGameModeCoordinator_BCE8_p23);
PPC_FUNC_IMPL(__imp__PongNetGameModeCoordinator_BCE8_p23) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5028
	ctx.r11.s64 = ctx.r11.s64 + 5028;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,220(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 220);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bbd54
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,108
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(108));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,108
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(108));
		// lhz r8,106(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 106);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,220(r11)
		PPC_STORE_U16(ctx.r11.u32 + 220, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bbd40
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,108
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(108));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,104(r5)
			PPC_STORE_U16(ctx.r5.u32 + 104, ctx.r9.u16);
		}
	loc_823BBD40:
		// sth r9,106(r3)
		PPC_STORE_U16(ctx.r3.u32 + 106, ctx.r9.u16);
		// lhz r4,216(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 216);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,216(r11)
		PPC_STORE_U16(ctx.r11.u32 + 216, ctx.r10.u16);
		// blr
		return;
	}
loc_823BBD54:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__JoinSpectatorSessionMessage_vfn_6"))) PPC_WEAK_FUNC(JoinSpectatorSessionMessage_vfn_6);
PPC_FUNC_IMPL(__imp__JoinSpectatorSessionMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5016(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5016);  /* glob:lbl_825D1398 @ 0x825d1398 */
	// blr
	return;
}

__attribute__((alias("__imp__JoinSpectatorSessionMessage_vfn_7"))) PPC_WEAK_FUNC(JoinSpectatorSessionMessage_vfn_7);
PPC_FUNC_IMPL(__imp__JoinSpectatorSessionMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5268
	ctx.r3.s64 = ctx.r11.s64 + -5268;
	// blr
	return;
}

__attribute__((alias("__imp__JoinSpectatorSessionMessage_vfn_1"))) PPC_WEAK_FUNC(JoinSpectatorSessionMessage_vfn_1);
PPC_FUNC_IMPL(__imp__JoinSpectatorSessionMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,96
	ctx.r5.s64 = 96;
	// addi r4,r11,8
	ctx.r4.s64 = ctx.r11.s64 + 8;
	// b 0x82260af0
	util_0AF0(ctx, base);
	return;
}

__attribute__((alias("__imp__JoinSpectatorSessionMessage_vfn_2"))) PPC_WEAK_FUNC(JoinSpectatorSessionMessage_vfn_2);
PPC_FUNC_IMPL(__imp__JoinSpectatorSessionMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,96
	ctx.r5.s64 = 96;
	// addi r4,r11,8
	ctx.r4.s64 = ctx.r11.s64 + 8;
	// b 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__JoinSpectatorSessionMessage_vfn_4"))) PPC_WEAK_FUNC(JoinSpectatorSessionMessage_vfn_4);
PPC_FUNC_IMPL(__imp__JoinSpectatorSessionMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32142
	// lis r10,-32142
	// addi r4,r3,8
	ctx.r4.s64 = ctx.r3.s64 + 8;
	// li r6,1
	ctx.r6.s64 = 1;
	// lwz r11,-23780(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23780);
	// lwz r3,-22608(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + -22608);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x823ad910
	SinglesNetworkClient_D910_g(ctx, base);
	return;
}

__attribute__((alias("__imp__LeaveSpectatorSessionMessage_vfn_5"))) PPC_WEAK_FUNC(LeaveSpectatorSessionMessage_vfn_5);
PPC_FUNC_IMPL(__imp__LeaveSpectatorSessionMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5052
	ctx.r11.s64 = ctx.r11.s64 + 5052;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bbe30
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BBE30:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetGameModeCoordinator_BE48"))) PPC_WEAK_FUNC(PongNetGameModeCoordinator_BE48);
PPC_FUNC_IMPL(__imp__PongNetGameModeCoordinator_BE48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5052
	ctx.r11.s64 = ctx.r11.s64 + 5052;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bbecc
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bbeb8
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BBEB8:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BBECC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__LeaveSpectatorSessionMessage_vfn_6"))) PPC_WEAK_FUNC(LeaveSpectatorSessionMessage_vfn_6);
PPC_FUNC_IMPL(__imp__LeaveSpectatorSessionMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5040(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5040);  /* glob:lbl_825D13B0 @ 0x825d13b0 */
	// blr
	return;
}

__attribute__((alias("__imp__LeaveSpectatorSessionMessage_vfn_7"))) PPC_WEAK_FUNC(LeaveSpectatorSessionMessage_vfn_7);
PPC_FUNC_IMPL(__imp__LeaveSpectatorSessionMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5240
	ctx.r3.s64 = ctx.r11.s64 + -5240;
	// blr
	return;
}

__attribute__((alias("__imp__LeaveSpectatorSessionMessage_vfn_4"))) PPC_WEAK_FUNC(LeaveSpectatorSessionMessage_vfn_4);
PPC_FUNC_IMPL(__imp__LeaveSpectatorSessionMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32142
	// lis r10,-32142
	// lwz r11,-23780(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23780);
	// lwz r31,-22608(r10)
	var_r31 = (uint32_t)(PPC_LOAD_U32(ctx.r10.u32 + -22608));
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplwi cr6,r11,3
	// bgt cr6,0x823bbf3c
	if (ctx.r11.u32 <= 3) {
		// mulli r11,r11,2784
		ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(2784));
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// addis r30,r11,2
		var_r30 = (uint32_t)(ctx.r11.s64 + 131072);  // addr:0x82740000
		// addi r30,r30,-29280
		var_r30 = (uint32_t)(var_r30 + -29280);
		// b 0x823bbf40
	} else {
	loc_823BBF3C:
		// li r30,0
		var_r30 = 0;
	}
loc_823BBF40:
	// lwz r10,60(r31)
	ctx.r10.u64 = PPC_LOAD_U32(var_r31 + 60);
	// cmpwi cr6,r10,1
	// ble cr6,0x823bbf58
	if (ctx.r10.s32 > 1) {
		// lwz r11,56(r31)
		ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
		// addi r11,r11,9680
		ctx.r11.s64 = ctx.r11.s64 + 9680;
		// b 0x823bbf5c
	} else {
	loc_823BBF58:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BBF5C:
	// addi r4,r30,96
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 96;
	// addi r3,r11,3756
	ctx.r3.s64 = ctx.r11.s64 + 3756;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// cmplwi cr6,r3,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823bbf78
	if (ctx.r3.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BBF78:
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r8,0
	// beq cr6,0x823bbfa8
	if (ctx.r8.u32 != 0) {
		// lwz r7,60(r31)
		ctx.r7.u64 = PPC_LOAD_U32(var_r31 + 60);
		// cmpwi cr6,r7,1
		// ble cr6,0x823bbf9c
		if (ctx.r7.s32 > 1) {
			// lwz r11,56(r31)
			ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 56);
			// addi r3,r11,9680
			ctx.r3.s64 = ctx.r11.s64 + 9680;
			// b 0x823bbfa0
		} else {
		loc_823BBF9C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BBFA0:
		// mr r4,r30
		ctx.r4.u64 = var_r30;
		// bl 0x823e6f48
		SinglesNetworkClient_6F48_w(ctx, base);
	}
loc_823BBFA8:
	// blr
	return;
}

__attribute__((alias("__imp__LevelConfigMessage_vfn_5"))) PPC_WEAK_FUNC(LevelConfigMessage_vfn_5);
PPC_FUNC_IMPL(__imp__LevelConfigMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5076
	ctx.r11.s64 = ctx.r11.s64 + 5076;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,68(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,68(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bc000
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BC000:
	// sth r10,68(r11)
	PPC_STORE_U16(ctx.r11.u32 + 68, ctx.r10.u16);
	// lhz r10,64(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 64);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,64(r11)
	PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_C018_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_C018_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_C018_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5076
	ctx.r11.s64 = ctx.r11.s64 + 5076;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,68(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bc080
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,68(r11)
		PPC_STORE_U16(ctx.r11.u32 + 68, ctx.r8.u16);
		// beq cr6,0x823bc06c
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BC06C:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,64(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 64);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,64(r11)
		PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r4.u16);
		// blr
		return;
	}
loc_823BC080:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__LevelConfigMessage_vfn_6"))) PPC_WEAK_FUNC(LevelConfigMessage_vfn_6);
PPC_FUNC_IMPL(__imp__LevelConfigMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5064(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5064);  /* glob:lbl_825D13C8 @ 0x825d13c8 */
	// blr
	return;
}

__attribute__((alias("__imp__LevelConfigMessage_vfn_7"))) PPC_WEAK_FUNC(LevelConfigMessage_vfn_7);
PPC_FUNC_IMPL(__imp__LevelConfigMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5208
	ctx.r3.s64 = ctx.r11.s64 + -5208;
	// blr
	return;
}

__attribute__((alias("__imp__PongNetMessage_vfn_0"))) PPC_WEAK_FUNC(PongNetMessage_vfn_0);
PPC_FUNC_IMPL(__imp__PongNetMessage_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,-15612
	ctx.r11.s64 = ctx.r11.s64 + -15612;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823bc0dc
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823BC0DC:
	// blr
	return;
}

__attribute__((alias("__imp__LevelConfigMessage_vfn_1"))) PPC_WEAK_FUNC(LevelConfigMessage_vfn_1);
PPC_FUNC_IMPL(__imp__LevelConfigMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__LevelConfigMessage_vfn_2"))) PPC_WEAK_FUNC(LevelConfigMessage_vfn_2);
PPC_FUNC_IMPL(__imp__LevelConfigMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__UserRequestReplayMessage_vfn_5"))) PPC_WEAK_FUNC(UserRequestReplayMessage_vfn_5);
PPC_FUNC_IMPL(__imp__UserRequestReplayMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5100
	ctx.r11.s64 = ctx.r11.s64 + 5100;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc200
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BC200:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_C218_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_C218_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_C218_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5100
	ctx.r11.s64 = ctx.r11.s64 + 5100;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc29c
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bc288
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BC288:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BC29C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__UserRequestReplayMessage_vfn_6"))) PPC_WEAK_FUNC(UserRequestReplayMessage_vfn_6);
PPC_FUNC_IMPL(__imp__UserRequestReplayMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5088(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5088);  /* glob:lbl_825D13E0 @ 0x825d13e0 */
	// blr
	return;
}

__attribute__((alias("__imp__UserRequestReplayMessage_vfn_7"))) PPC_WEAK_FUNC(UserRequestReplayMessage_vfn_7);
PPC_FUNC_IMPL(__imp__UserRequestReplayMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5188
	ctx.r3.s64 = ctx.r11.s64 + -5188;
	// blr
	return;
}

__attribute__((alias("__imp__UserRequestReplayMessage_vfn_4"))) PPC_WEAK_FUNC(UserRequestReplayMessage_vfn_4);
PPC_FUNC_IMPL(__imp__UserRequestReplayMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32142
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r3,-23828(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23828);  /* glob:0x8206a2ec */
	// b 0x821ccd78
	game_CD78(ctx, base);
	return;
}

__attribute__((alias("__imp__SkipReplayMessage_vfn_5"))) PPC_WEAK_FUNC(SkipReplayMessage_vfn_5);
PPC_FUNC_IMPL(__imp__SkipReplayMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5124
	ctx.r11.s64 = ctx.r11.s64 + 5124;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc338
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BC338:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_C350_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_C350_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_C350_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5124
	ctx.r11.s64 = ctx.r11.s64 + 5124;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc3d4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bc3c0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BC3C0:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BC3D4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__SkipReplayMessage_vfn_6"))) PPC_WEAK_FUNC(SkipReplayMessage_vfn_6);
PPC_FUNC_IMPL(__imp__SkipReplayMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5112(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5112);  /* glob:lbl_825D13F8 @ 0x825d13f8 */
	// blr
	return;
}

__attribute__((alias("__imp__SkipReplayMessage_vfn_7"))) PPC_WEAK_FUNC(SkipReplayMessage_vfn_7);
PPC_FUNC_IMPL(__imp__SkipReplayMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5160
	ctx.r3.s64 = ctx.r11.s64 + -5160;
	// blr
	return;
}

__attribute__((alias("__imp__SkipReplayMessage_vfn_1"))) PPC_WEAK_FUNC(SkipReplayMessage_vfn_1);
PPC_FUNC_IMPL(__imp__SkipReplayMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// lis r11,-32142
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// extsw r9,r10
	ctx.r9.s64 = ctx.r10.s32;
	// std r9,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r9.u64);
	// lfd f13,80(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r31)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lfs f9,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f9.f64 = double(temp.f32);
	// stfs f9,4(r31)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__SkipReplayMessage_vfn_4"))) PPC_WEAK_FUNC(SkipReplayMessage_vfn_4);
PPC_FUNC_IMPL(__imp__SkipReplayMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,64
	ctx.r4.s64 = 64;
	// li r3,14365
	ctx.r3.s64 = 14365;
	// b 0x8225e6e0
	pg_E6E0(ctx, base);
	return;
}

__attribute__((alias("__imp__DisconnectingSinglesClientMessage_vfn_5"))) PPC_WEAK_FUNC(DisconnectingSinglesClientMessage_vfn_5);
PPC_FUNC_IMPL(__imp__DisconnectingSinglesClientMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5148
	ctx.r11.s64 = ctx.r11.s64 + 5148;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc4e8
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BC4E8:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__DisconnectingSinglesClientMessage_vfn_6"))) PPC_WEAK_FUNC(DisconnectingSinglesClientMessage_vfn_6);
PPC_FUNC_IMPL(__imp__DisconnectingSinglesClientMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5136(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5136);  /* glob:lbl_825D1410 @ 0x825d1410 */
	// blr
	return;
}

__attribute__((alias("__imp__DisconnectingSinglesClientMessage_vfn_7"))) PPC_WEAK_FUNC(DisconnectingSinglesClientMessage_vfn_7);
PPC_FUNC_IMPL(__imp__DisconnectingSinglesClientMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5140
	ctx.r3.s64 = ctx.r11.s64 + -5140;
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchMessage_vfn_5"))) PPC_WEAK_FUNC(NextMatchMessage_vfn_5);
PPC_FUNC_IMPL(__imp__NextMatchMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5172
	ctx.r11.s64 = ctx.r11.s64 + 5172;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 3) & 0xFFFFFFF8;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,22(r3)
	PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc580
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,20(r8)
		PPC_STORE_U16(ctx.r8.u32 + 20, ctx.r9.u16);
	}
loc_823BC580:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_C598"))) PPC_WEAK_FUNC(game_C598);
PPC_FUNC_IMPL(__imp__game_C598) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5172
	ctx.r11.s64 = ctx.r11.s64 + 5172;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc61c
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,22(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 22);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 3) & 0xFFFFFFF8;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bc608
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,3,0,28
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 3) & 0xFFFFFFF8;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,20(r10)
			PPC_STORE_U16(ctx.r10.u32 + 20, ctx.r9.u16);
		}
	loc_823BC608:
		// sth r9,22(r3)
		PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BC61C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchMessage_vfn_6"))) PPC_WEAK_FUNC(NextMatchMessage_vfn_6);
PPC_FUNC_IMPL(__imp__NextMatchMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5160(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5160);  /* glob:lbl_825D1428 @ 0x825d1428 */
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchMessage_vfn_7"))) PPC_WEAK_FUNC(NextMatchMessage_vfn_7);
PPC_FUNC_IMPL(__imp__NextMatchMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5104
	ctx.r3.s64 = ctx.r11.s64 + -5104;
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchMessage_vfn_1"))) PPC_WEAK_FUNC(NextMatchMessage_vfn_1);
PPC_FUNC_IMPL(__imp__NextMatchMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r31,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchMessage_vfn_2"))) PPC_WEAK_FUNC(NextMatchMessage_vfn_2);
PPC_FUNC_IMPL(__imp__NextMatchMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r31,16
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 16;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchSpectatorMessage_vfn_5"))) PPC_WEAK_FUNC(NextMatchSpectatorMessage_vfn_5);
PPC_FUNC_IMPL(__imp__NextMatchSpectatorMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5196
	ctx.r11.s64 = ctx.r11.s64 + 5196;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,20(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 20);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bc778
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BC778:
	// sth r10,20(r11)
	PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r10.u16);
	// lhz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,16(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchSpectatorMessage_vfn_6"))) PPC_WEAK_FUNC(NextMatchSpectatorMessage_vfn_6);
PPC_FUNC_IMPL(__imp__NextMatchSpectatorMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5184(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5184);  /* glob:lbl_825D1440 @ 0x825d1440 */
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchSpectatorMessage_vfn_7"))) PPC_WEAK_FUNC(NextMatchSpectatorMessage_vfn_7);
PPC_FUNC_IMPL(__imp__NextMatchSpectatorMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5084
	ctx.r3.s64 = ctx.r11.s64 + -5084;
	// blr
	return;
}

__attribute__((alias("__imp__TournamentCompleteMessage_vfn_5"))) PPC_WEAK_FUNC(TournamentCompleteMessage_vfn_5);
PPC_FUNC_IMPL(__imp__TournamentCompleteMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5220
	ctx.r11.s64 = ctx.r11.s64 + 5220;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bc7f0
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BC7F0:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetGameModeCoordinator_C808_p23"))) PPC_WEAK_FUNC(PongNetGameModeCoordinator_C808_p23);
PPC_FUNC_IMPL(__imp__PongNetGameModeCoordinator_C808_p23) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5220
	ctx.r11.s64 = ctx.r11.s64 + 5220;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bc870
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823bc85c
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BC85C:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823BC870:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__TournamentCompleteMessage_vfn_6"))) PPC_WEAK_FUNC(TournamentCompleteMessage_vfn_6);
PPC_FUNC_IMPL(__imp__TournamentCompleteMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5208(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5208);  /* glob:lbl_825D1458 @ 0x825d1458 */
	// blr
	return;
}

__attribute__((alias("__imp__TournamentCompleteMessage_vfn_7"))) PPC_WEAK_FUNC(TournamentCompleteMessage_vfn_7);
PPC_FUNC_IMPL(__imp__TournamentCompleteMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5056
	ctx.r3.s64 = ctx.r11.s64 + -5056;
	// blr
	return;
}

__attribute__((alias("__imp__NextMatchSpectatorMessage_vfn_1"))) PPC_WEAK_FUNC(NextMatchSpectatorMessage_vfn_1);
PPC_FUNC_IMPL(__imp__NextMatchSpectatorMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r11,8
	ctx.r4.s64 = ctx.r11.s64 + 8;
	// b 0x82101668
	util_1668(ctx, base);
	return;
}

__attribute__((alias("__imp__NextMatchSpectatorMessage_vfn_2"))) PPC_WEAK_FUNC(NextMatchSpectatorMessage_vfn_2);
PPC_FUNC_IMPL(__imp__NextMatchSpectatorMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,8
	ctx.r5.s64 = 8;
	// lbz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// extsb r4,r11
	ctx.r4.s64 = ctx.r11.s8;
	// b 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	return;
}

__attribute__((alias("__imp__TournamentCompleteMessage_vfn_4"))) PPC_WEAK_FUNC(TournamentCompleteMessage_vfn_4);
PPC_FUNC_IMPL(__imp__TournamentCompleteMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r3,r11,-13484
	ctx.r3.s64 = ctx.r11.s64 + -13484;
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// lis r11,-32161
	ctx.r11.s64 = -2107703296;
	// lwz r11,-21712(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21712);
	// lbz r10,578(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 578);
	// cmplwi cr6,r10,0
	// bne cr6,0x823bc91c
	if (ctx.r10.u32 == 0) {
		// lwz r9,12(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// li r11,1
		ctx.r11.s64 = 1;
		// cmpwi cr6,r9,8
		// beq cr6,0x823bc910
		if (ctx.r9.s32 != 8) {
			// li r11,0
			ctx.r11.s64 = 0;
		}
	loc_823BC910:
		// clrlwi r7,r11,24
		ctx.r7.u64 = ctx.r11.u32 & 0xFF;
		// cmplwi cr6,r7,0
		// beq cr6,0x823bc92c
		if (ctx.r7.u32 == 0) goto loc_823BC92C;
	}
loc_823BC91C:
	// lis r11,-32161
	// li r6,4
	ctx.r6.s64 = 4;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// stw r6,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r6.u32);
loc_823BC92C:
	// lbz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 8);
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// li r5,1
	ctx.r5.s64 = 1;
	// extsb r10,r11
	ctx.r10.s64 = ctx.r11.s8;
	// li r4,128
	ctx.r4.s64 = 128;
	// li r3,14386
	ctx.r3.s64 = 14386;
	// stw r10,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
	// bl 0x8225e6e0
	pg_E6E0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncMessage_vfn_5"))) PPC_WEAK_FUNC(MatchDataSyncMessage_vfn_5);
PPC_FUNC_IMPL(__imp__MatchDataSyncMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5244
	ctx.r11.s64 = ctx.r11.s64 + 5244;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bc9c0
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BC9C0:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_C9D8_p46"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_C9D8_p46);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_C9D8_p46) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5244
	ctx.r11.s64 = ctx.r11.s64 + 5244;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bca5c
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bca48
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BCA48:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BCA5C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncMessage_vfn_6"))) PPC_WEAK_FUNC(MatchDataSyncMessage_vfn_6);
PPC_FUNC_IMPL(__imp__MatchDataSyncMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5232(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5232);  /* glob:lbl_825D1470 @ 0x825d1470 */
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncMessage_vfn_7"))) PPC_WEAK_FUNC(MatchDataSyncMessage_vfn_7);
PPC_FUNC_IMPL(__imp__MatchDataSyncMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4984
	ctx.r3.s64 = ctx.r11.s64 + -4984;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_vfn_5"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_vfn_5);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,5268
	ctx.r11.s64 = ctx.r11.s64 + 5268;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,1284(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1284);
	// rlwinm r6,r7,23,9,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 23) & 0x7FFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,7,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 7) & 0xFFFFFF80;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,638(r3)
	PPC_STORE_U16(ctx.r3.u32 + 638, ctx.r5.u16);
	// lhz r10,1284(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1284);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bcae8
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,7,0,24
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 7) & 0xFFFFFF80;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,636(r8)
		PPC_STORE_U16(ctx.r8.u32 + 636, ctx.r9.u16);
	}
loc_823BCAE8:
	// lhz r10,1280(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1280);
	// sth r9,1284(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1284, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,1280(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1280, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_CB00"))) PPC_WEAK_FUNC(game_CB00);
PPC_FUNC_IMPL(__imp__game_CB00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5268
	ctx.r11.s64 = ctx.r11.s64 + 5268;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,1284(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1284);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bcb84
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,7,0,24
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 7) & 0xFFFFFF80;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,638(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 638);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,7,0,24
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 7) & 0xFFFFFF80;
		// sth r7,1284(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1284, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bcb70
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,7,0,24
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 7) & 0xFFFFFF80;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,636(r10)
			PPC_STORE_U16(ctx.r10.u32 + 636, ctx.r9.u16);
		}
	loc_823BCB70:
		// sth r9,638(r3)
		PPC_STORE_U16(ctx.r3.u32 + 638, ctx.r9.u16);
		// lhz r8,1280(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1280);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,1280(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1280, ctx.r7.u16);
		// blr
		return;
	}
loc_823BCB84:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_vfn_6"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_vfn_6);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5256(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5256);  /* glob:lbl_825D1488 @ 0x825d1488 */
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_vfn_7"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_vfn_7);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4960
	ctx.r3.s64 = ctx.r11.s64 + -4960;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_vfn_1"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_vfn_1);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r30,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f0,4(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r30 + 4, temp.u32);
	// bl 0x82260e18
	SinglesNetworkClient_0E18_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r30,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 12;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260e18
	SinglesNetworkClient_0E18_g(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// addi r3,r30,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r30 + 16;
	// bl 0x82100640
	SinglesNetworkClient_0640_w(ctx, base);
	// addi r30,r30,124
	var_r30 = (uint32_t)(var_r30 + 124);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// addi r29,r30,2
	var_r29 = (uint32_t)(var_r30 + 2);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x820d7970
	util_7970(ctx, base);
	// lhz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 0);
	// addi r4,r30,4
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 4;
	// extsh r5,r11
	ctx.r5.s64 = ctx.r11.s16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_vfn_2"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_vfn_2);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// lwz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r4,12(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 12);
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,16
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
	// bl 0x82100860
	SinglesNetworkClient_0860_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,124
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 124;
	// bl 0x82390598
	SinglesNetworkClient_0598_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_vfn_4"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_vfn_4);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lis r11,-32161
	// addi r10,r3,16
	ctx.r10.s64 = ctx.r3.s64 + 16;
	// lwz r3,-21716(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21716);
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// addi r9,r3,4
	ctx.r9.s64 = ctx.r3.s64 + 4;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// li r9,27
	ctx.r9.s64 = 27;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823BCCF4:
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// stw r8,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r8.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// bdnz 0x823bccf4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823BCCF4;
	// lbz r7,288(r3)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r3.u32 + 288);
	// cmplwi cr6,r7,0
	// bne cr6,0x823bcd18
	if (ctx.r7.u32 == 0) {
		// bl 0x82100dd0
		pg_0DD0(ctx, base);
	}
loc_823BCD18:
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,2
	// bne cr6,0x823bcd68
	if (ctx.r11.s32 == 2) {
		// lis r11,-32160
		ctx.r11.s64 = -2107637760;
		// lwz r11,25500(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 25500);
		// cmplwi cr6,r11,0
		// beq cr6,0x823bcd68
		if (ctx.r11.u32 == 0) {
			// blr
			return;
		}
		// li r3,0
		ctx.r3.s64 = 0;
		// bl 0x823919a0
		SinglesNetworkClient_19A0_g(ctx, base);
		// clrlwi r6,r3,24
		ctx.r6.u64 = ctx.r3.u32 & 0xFF;
		// li r11,0
		ctx.r11.s64 = 0;
		// cmplwi cr6,r6,0
		// bne cr6,0x823bcd54
		if (ctx.r6.u32 == 0) {
			// li r11,64
			ctx.r11.s64 = 64;
		}
	loc_823BCD54:
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// clrlwi r4,r11,24
		ctx.r4.u64 = ctx.r11.u32 & 0xFF;
		// li r3,2154
		ctx.r3.s64 = 2154;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
	}
loc_823BCD68:
	// blr
	return;
}

__attribute__((alias("__imp__MatchScoreMessage_vfn_5"))) PPC_WEAK_FUNC(MatchScoreMessage_vfn_5);
PPC_FUNC_IMPL(__imp__MatchScoreMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,20971
	ctx.r9.s64 = 1374355456;
	// addi r11,r11,5292
	ctx.r11.s64 = ctx.r11.s64 + 5292;
	// ori r8,r9,34079
	ctx.r8.u64 = ctx.r9.u64 | 34079;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,504(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 504);
	// rlwinm r6,r7,27,5,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x7FFFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,100
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(100));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,98(r3)
	PPC_STORE_U16(ctx.r3.u32 + 98, ctx.r5.u16);
	// lhz r9,504(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 504);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bcdc8
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,100
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(100));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,96(r9)
		PPC_STORE_U16(ctx.r9.u32 + 96, ctx.r10.u16);
	}
loc_823BCDC8:
	// sth r10,504(r11)
	PPC_STORE_U16(ctx.r11.u32 + 504, ctx.r10.u16);
	// lhz r10,500(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 500);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,500(r11)
	PPC_STORE_U16(ctx.r11.u32 + 500, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataSyncResponseMessage_CDE0_p46"))) PPC_WEAK_FUNC(MatchDataSyncResponseMessage_CDE0_p46);
PPC_FUNC_IMPL(__imp__MatchDataSyncResponseMessage_CDE0_p46) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5292
	ctx.r11.s64 = ctx.r11.s64 + 5292;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,504(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 504);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bce4c
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,100
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(100));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,100
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(100));
		// lhz r8,98(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 98);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,504(r11)
		PPC_STORE_U16(ctx.r11.u32 + 504, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bce38
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,100
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(100));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,96(r5)
			PPC_STORE_U16(ctx.r5.u32 + 96, ctx.r9.u16);
		}
	loc_823BCE38:
		// sth r9,98(r3)
		PPC_STORE_U16(ctx.r3.u32 + 98, ctx.r9.u16);
		// lhz r4,500(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 500);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,500(r11)
		PPC_STORE_U16(ctx.r11.u32 + 500, ctx.r10.u16);
		// blr
		return;
	}
loc_823BCE4C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__MatchScoreMessage_vfn_6"))) PPC_WEAK_FUNC(MatchScoreMessage_vfn_6);
PPC_FUNC_IMPL(__imp__MatchScoreMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5280(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5280);  /* glob:lbl_825D14A0 @ 0x825d14a0 */
	// blr
	return;
}

__attribute__((alias("__imp__MatchScoreMessage_vfn_7"))) PPC_WEAK_FUNC(MatchScoreMessage_vfn_7);
PPC_FUNC_IMPL(__imp__MatchScoreMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5028
	ctx.r3.s64 = ctx.r11.s64 + -5028;
	// blr
	return;
}

__attribute__((alias("__imp__MatchScoreMessage_vfn_1"))) PPC_WEAK_FUNC(MatchScoreMessage_vfn_1);
PPC_FUNC_IMPL(__imp__MatchScoreMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,88
	ctx.r5.s64 = 88;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__MatchScoreMessage_vfn_2"))) PPC_WEAK_FUNC(MatchScoreMessage_vfn_2);
PPC_FUNC_IMPL(__imp__MatchScoreMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,88
	ctx.r5.s64 = 88;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PostPointExitMessage_vfn_5"))) PPC_WEAK_FUNC(PostPointExitMessage_vfn_5);
PPC_FUNC_IMPL(__imp__PostPointExitMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5316
	ctx.r11.s64 = ctx.r11.s64 + 5316;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bcf68
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BCF68:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_CF80_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_CF80_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_CF80_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5316
	ctx.r11.s64 = ctx.r11.s64 + 5316;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bcfe8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823bcfd4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BCFD4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823BCFE8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__PostPointExitMessage_vfn_6"))) PPC_WEAK_FUNC(PostPointExitMessage_vfn_6);
PPC_FUNC_IMPL(__imp__PostPointExitMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5304(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5304);  /* glob:lbl_825D14B8 @ 0x825d14b8 */
	// blr
	return;
}

__attribute__((alias("__imp__PostPointExitMessage_vfn_7"))) PPC_WEAK_FUNC(PostPointExitMessage_vfn_7);
PPC_FUNC_IMPL(__imp__PostPointExitMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-5008
	ctx.r3.s64 = ctx.r11.s64 + -5008;
	// blr
	return;
}

__attribute__((alias("__imp__DropSpectatorConnection_vfn_1"))) PPC_WEAK_FUNC(DropSpectatorConnection_vfn_1);
PPC_FUNC_IMPL(__imp__DropSpectatorConnection_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// cmplwi cr6,r11,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823bd068
	if (ctx.r11.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BD068:
	// stb r11,8(r31)
	PPC_STORE_U8(var_r31 + 8, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__DropSpectatorConnection_vfn_2"))) PPC_WEAK_FUNC(DropSpectatorConnection_vfn_2);
PPC_FUNC_IMPL(__imp__DropSpectatorConnection_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 8);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PostPointExitMessage_vfn_4"))) PPC_WEAK_FUNC(PostPointExitMessage_vfn_4);
PPC_FUNC_IMPL(__imp__PostPointExitMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lbz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// cmplwi cr6,r11,0
	// beq cr6,0x823bd0fc
	if (ctx.r11.u32 != 0) {
		// li r10,1
		ctx.r10.s64 = 1;
		// stw r10,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
		// b 0x823bd104
	} else {
	loc_823BD0FC:
		// li r9,0
		ctx.r9.s64 = 0;
		// stw r9,80(r1)
		PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	}
loc_823BD104:
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,64
	ctx.r4.s64 = 64;
	// li r3,14384
	ctx.r3.s64 = 14384;
	// bl 0x8225e6e0
	pg_E6E0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PointDeclaredMessage_vfn_5"))) PPC_WEAK_FUNC(PointDeclaredMessage_vfn_5);
PPC_FUNC_IMPL(__imp__PointDeclaredMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5340
	ctx.r11.s64 = ctx.r11.s64 + 5340;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,84(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 84);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,84(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 84);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bd168
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BD168:
	// sth r10,84(r11)
	PPC_STORE_U16(ctx.r11.u32 + 84, ctx.r10.u16);
	// lhz r10,80(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 80);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,80(r11)
	PPC_STORE_U16(ctx.r11.u32 + 80, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_D180"))) PPC_WEAK_FUNC(SinglesNetworkClient_D180);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_D180) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5340
	ctx.r11.s64 = ctx.r11.s64 + 5340;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,84(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 84);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bd1e8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,84(r11)
		PPC_STORE_U16(ctx.r11.u32 + 84, ctx.r8.u16);
		// beq cr6,0x823bd1d4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BD1D4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,80(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 80);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,80(r11)
		PPC_STORE_U16(ctx.r11.u32 + 80, ctx.r4.u16);
		// blr
		return;
	}
loc_823BD1E8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__PointDeclaredMessage_vfn_6"))) PPC_WEAK_FUNC(PointDeclaredMessage_vfn_6);
PPC_FUNC_IMPL(__imp__PointDeclaredMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5328(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5328);  /* glob:lbl_825D14D0 @ 0x825d14d0 */
	// blr
	return;
}

__attribute__((alias("__imp__PointDeclaredMessage_vfn_7"))) PPC_WEAK_FUNC(PointDeclaredMessage_vfn_7);
PPC_FUNC_IMPL(__imp__PointDeclaredMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4928
	ctx.r3.s64 = ctx.r11.s64 + -4928;
	// blr
	return;
}

__attribute__((alias("__imp__PointDeclaredMessage_vfn_1"))) PPC_WEAK_FUNC(PointDeclaredMessage_vfn_1);
PPC_FUNC_IMPL(__imp__PointDeclaredMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x820d7830
	util_7830(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PointDeclaredMessage_vfn_2"))) PPC_WEAK_FUNC(PointDeclaredMessage_vfn_2);
PPC_FUNC_IMPL(__imp__PointDeclaredMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,16
	ctx.r5.s64 = 16;
	// lhz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U16(var_r31 + 8);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__PointDeclaredMessage_vfn_4"))) PPC_WEAK_FUNC(PointDeclaredMessage_vfn_4);
PPC_FUNC_IMPL(__imp__PointDeclaredMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=128, manual
	// lis r11,-32254
	// lhz r6,8(r3)
	ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 8);
	// li r4,64
	ctx.r4.s64 = 64;
	// addi r5,r11,30872
	ctx.r5.s64 = ctx.r11.s64 + 30872;
	// li r3,14367
	ctx.r3.s64 = 14367;
	// bl 0x8225e480
	pg_E480(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ReplayEmoteMessage_vfn_5"))) PPC_WEAK_FUNC(ReplayEmoteMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ReplayEmoteMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5364
	ctx.r11.s64 = ctx.r11.s64 + 5364;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,84(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 84);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,84(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 84);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bd338
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BD338:
	// sth r10,84(r11)
	PPC_STORE_U16(ctx.r11.u32 + 84, ctx.r10.u16);
	// lhz r10,80(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 80);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,80(r11)
	PPC_STORE_U16(ctx.r11.u32 + 80, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_D350"))) PPC_WEAK_FUNC(SinglesNetworkClient_D350);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_D350) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5364
	ctx.r11.s64 = ctx.r11.s64 + 5364;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,84(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 84);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bd3b8
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,84(r11)
		PPC_STORE_U16(ctx.r11.u32 + 84, ctx.r8.u16);
		// beq cr6,0x823bd3a4
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BD3A4:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,80(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 80);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,80(r11)
		PPC_STORE_U16(ctx.r11.u32 + 80, ctx.r4.u16);
		// blr
		return;
	}
loc_823BD3B8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ReplayEmoteMessage_vfn_6"))) PPC_WEAK_FUNC(ReplayEmoteMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ReplayEmoteMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5352(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5352);  /* glob:lbl_825D14E8 @ 0x825d14e8 */
	// blr
	return;
}

__attribute__((alias("__imp__ReplayEmoteMessage_vfn_7"))) PPC_WEAK_FUNC(ReplayEmoteMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ReplayEmoteMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4904
	ctx.r3.s64 = ctx.r11.s64 + -4904;
	// blr
	return;
}

__attribute__((alias("__imp__ReplayEmoteMessage_vfn_1"))) PPC_WEAK_FUNC(ReplayEmoteMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ReplayEmoteMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// cmplwi cr6,r11,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823bd438
	if (ctx.r11.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BD438:
	// li r5,8
	ctx.r5.s64 = 8;
	// stb r11,9(r31)
	PPC_STORE_U8(var_r31 + 9, ctx.r11.u8);
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82101668
	util_1668(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r9,80(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// li r11,1
	ctx.r11.s64 = 1;
	// cmplwi cr6,r9,0
	// bne cr6,0x823bd470
	if (ctx.r9.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BD470:
	// stb r11,10(r31)
	PPC_STORE_U8(var_r31 + 10, ctx.r11.u8);
	// blr
	return;
}

__attribute__((alias("__imp__ReplayEmoteMessage_vfn_2"))) PPC_WEAK_FUNC(ReplayEmoteMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ReplayEmoteMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,9(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 9);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// lbz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 8);
	// li r5,8
	ctx.r5.s64 = 8;
	// extsb r4,r11
	ctx.r4.s64 = ctx.r11.s8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,10(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 10);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ReplayEmoteMessage_vfn_4"))) PPC_WEAK_FUNC(ReplayEmoteMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ReplayEmoteMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32142
	// lbz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// lbz r9,9(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 9);
	// li r7,1
	ctx.r7.s64 = 1;
	// extsb r8,r10
	ctx.r8.s64 = ctx.r10.s8;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r11,-23812(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23812);
	// lbz r6,24(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 24);
	// cmplwi cr6,r6,0
	// bne cr6,0x823bd538
	if (ctx.r6.u32 == 0) {
		// stb r7,16(r11)
		PPC_STORE_U8(ctx.r11.u32 + 16, ctx.r7.u8);
		// stb r9,17(r11)
		PPC_STORE_U8(ctx.r11.u32 + 17, ctx.r9.u8);
		// stw r8,20(r11)
		PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r8.u32);
		// stb r10,24(r11)
		PPC_STORE_U8(ctx.r11.u32 + 24, ctx.r10.u8);
	}
loc_823BD538:
	// lis r11,-32142
	// lbz r5,10(r3)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r3.u32 + 10);
	// cmplwi cr6,r5,0
	// lwz r3,-23828(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23828);
	// beq cr6,0x823bd55c
	if (ctx.r5.u32 != 0) {
		// lis r4,6
		ctx.r4.s64 = 393216;
		// ori r10,r4,36092
		ctx.r10.u64 = ctx.r4.u64 | 36092;
		// stbx r7,r3,r10
		PPC_STORE_U8(ctx.r3.u32 + ctx.r10.u32, ctx.r7.u8);
		// b 0x821cce00
		util_CE00(ctx, base);
		return;
	}
loc_823BD55C:
	// lis r9,6
	ctx.r9.s64 = 393216;
	// lis r7,6
	ctx.r7.s64 = 393216;
	// lis r5,6
	ctx.r5.s64 = 393216;
	// ori r8,r9,36093
	ctx.r8.u64 = ctx.r9.u64 | 36093;
	// ori r6,r7,36092
	ctx.r6.u64 = ctx.r7.u64 | 36092;
	// ori r4,r5,36091
	ctx.r4.u64 = ctx.r5.u64 | 36091;
	// stbx r10,r3,r8
	PPC_STORE_U8(ctx.r3.u32 + ctx.r8.u32, ctx.r10.u8);
	// stbx r10,r3,r6
	PPC_STORE_U8(ctx.r3.u32 + ctx.r6.u32, ctx.r10.u8);
	// stbx r10,r3,r4
	PPC_STORE_U8(ctx.r3.u32 + ctx.r4.u32, ctx.r10.u8);
	// b 0x821cce00
	util_CE00(ctx, base);
	return;
}

__attribute__((alias("__imp__EnteringReplayMessage_vfn_5"))) PPC_WEAK_FUNC(EnteringReplayMessage_vfn_5);
PPC_FUNC_IMPL(__imp__EnteringReplayMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5388
	ctx.r11.s64 = ctx.r11.s64 + 5388;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bd5e8
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BD5E8:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_D600_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_D600_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_D600_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5388
	ctx.r11.s64 = ctx.r11.s64 + 5388;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bd684
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bd670
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BD670:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BD684:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__EnteringReplayMessage_vfn_6"))) PPC_WEAK_FUNC(EnteringReplayMessage_vfn_6);
PPC_FUNC_IMPL(__imp__EnteringReplayMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5376(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5376);  /* glob:lbl_825D1500 @ 0x825d1500 */
	// blr
	return;
}

__attribute__((alias("__imp__EnteringReplayMessage_vfn_7"))) PPC_WEAK_FUNC(EnteringReplayMessage_vfn_7);
PPC_FUNC_IMPL(__imp__EnteringReplayMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4884
	ctx.r3.s64 = ctx.r11.s64 + -4884;
	// blr
	return;
}

__attribute__((alias("__imp__EnteringReplayMessage_vfn_4"))) PPC_WEAK_FUNC(EnteringReplayMessage_vfn_4);
PPC_FUNC_IMPL(__imp__EnteringReplayMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,64
	ctx.r4.s64 = 64;
	// li r3,14408
	ctx.r3.s64 = 14408;
	// b 0x8225e6e0
	pg_E6E0(ctx, base);
	return;
}

__attribute__((alias("__imp__ScoreMessage_vfn_5"))) PPC_WEAK_FUNC(ScoreMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ScoreMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-17874
	// addi r11,r11,5412
	ctx.r11.s64 = ctx.r11.s64 + 5412;
	// ori r8,r9,35747
	ctx.r8.u64 = ctx.r9.u64 | 35747;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,224(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 224);
	// rlwinm r6,r7,27,5,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x7FFFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,44
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(44));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,42(r3)
	PPC_STORE_U16(ctx.r3.u32 + 42, ctx.r5.u16);
	// lhz r9,224(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 224);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bd718
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,44
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(44));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,40(r9)
		PPC_STORE_U16(ctx.r9.u32 + 40, ctx.r10.u16);
	}
loc_823BD718:
	// sth r10,224(r11)
	PPC_STORE_U16(ctx.r11.u32 + 224, ctx.r10.u16);
	// lhz r10,220(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 220);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,220(r11)
	PPC_STORE_U16(ctx.r11.u32 + 220, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__util_D730"))) PPC_WEAK_FUNC(util_D730);
PPC_FUNC_IMPL(__imp__util_D730) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5412
	ctx.r11.s64 = ctx.r11.s64 + 5412;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,224(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 224);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bd79c
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,44
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(44));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,44
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(44));
		// lhz r8,42(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 42);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,224(r11)
		PPC_STORE_U16(ctx.r11.u32 + 224, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bd788
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,44
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(44));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,40(r5)
			PPC_STORE_U16(ctx.r5.u32 + 40, ctx.r9.u16);
		}
	loc_823BD788:
		// sth r9,42(r3)
		PPC_STORE_U16(ctx.r3.u32 + 42, ctx.r9.u16);
		// lhz r4,220(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 220);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,220(r11)
		PPC_STORE_U16(ctx.r11.u32 + 220, ctx.r10.u16);
		// blr
		return;
	}
loc_823BD79C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ScoreMessage_vfn_6"))) PPC_WEAK_FUNC(ScoreMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ScoreMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5400(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5400);  /* glob:lbl_825D1518 @ 0x825d1518 */
	// blr
	return;
}

__attribute__((alias("__imp__ScoreMessage_vfn_7"))) PPC_WEAK_FUNC(ScoreMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ScoreMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4860
	ctx.r3.s64 = ctx.r11.s64 + -4860;
	// blr
	return;
}

__attribute__((alias("__imp__ScoreMessage_vfn_1"))) PPC_WEAK_FUNC(ScoreMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ScoreMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,8
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82390608
	SinglesNetworkClient_0608_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ScoreMessage_vfn_2"))) PPC_WEAK_FUNC(ScoreMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ScoreMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// lbz r4,12(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 12);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,13(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 13);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,14(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 14);
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lwz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U32(var_r31 + 8);
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,15(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 15);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,16(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 16);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,17(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 17);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,18(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 18);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// lbz r11,19(r31)
	ctx.r11.u64 = PPC_LOAD_U8(var_r31 + 19);
	// li r5,8
	ctx.r5.s64 = 8;
	// extsb r4,r11
	ctx.r4.s64 = ctx.r11.s8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d68a8
	SinglesNetworkClient_68A8_g(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,20(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 20);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ScoreMessage_vfn_4"))) PPC_WEAK_FUNC(ScoreMessage_vfn_4);
PPC_FUNC_IMPL(__imp__ScoreMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// lwz r11,36(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 36);
	// cmpwi cr6,r11,0
	// beq cr6,0x823bd944
	if (ctx.r11.s32 != 0) {
		// addi r31,r3,24
		var_r31 = (uint32_t)(ctx.r3.s64 + 24);
		// addi r4,r3,8
		ctx.r4.s64 = ctx.r3.s64 + 8;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x8225fff8
		util_FFF8(ctx, base);
		// lis r11,-32158
		// addi r11,r11,-24384
		ctx.r11.s64 = ctx.r11.s64 + -24384;
		// lwz r10,0(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
		// stw r10,0(r31)
		PPC_STORE_U32(var_r31 + 0, ctx.r10.u32);
		// lwz r9,4(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
		// stw r9,4(r31)
		PPC_STORE_U32(var_r31 + 4, ctx.r9.u32);
		// lwz r8,8(r11)
		ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
		// stw r8,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r8.u32);
		// lwz r7,12(r11)
		ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// stw r7,12(r31)
		PPC_STORE_U32(var_r31 + 12, ctx.r7.u32);
	}
loc_823BD944:
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateRequestMessage_vfn_5"))) PPC_WEAK_FUNC(LoadingStateRequestMessage_vfn_5);
PPC_FUNC_IMPL(__imp__LoadingStateRequestMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5436
	ctx.r11.s64 = ctx.r11.s64 + 5436;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,52(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 52);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,52(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 52);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bd9b8
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BD9B8:
	// lhz r10,48(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 48);
	// sth r9,52(r11)
	PPC_STORE_U16(ctx.r11.u32 + 52, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,48(r11)
	PPC_STORE_U16(ctx.r11.u32 + 48, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateRequestMessage_vfn_6"))) PPC_WEAK_FUNC(LoadingStateRequestMessage_vfn_6);
PPC_FUNC_IMPL(__imp__LoadingStateRequestMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5424(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5424);  /* glob:lbl_825D1530 @ 0x825d1530 */
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateRequestMessage_vfn_7"))) PPC_WEAK_FUNC(LoadingStateRequestMessage_vfn_7);
PPC_FUNC_IMPL(__imp__LoadingStateRequestMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4844
	ctx.r3.s64 = ctx.r11.s64 + -4844;
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateResponseMessage_vfn_5"))) PPC_WEAK_FUNC(LoadingStateResponseMessage_vfn_5);
PPC_FUNC_IMPL(__imp__LoadingStateResponseMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5460
	ctx.r11.s64 = ctx.r11.s64 + 5460;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,68(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,68(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bda30
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BDA30:
	// sth r10,68(r11)
	PPC_STORE_U16(ctx.r11.u32 + 68, ctx.r10.u16);
	// lhz r10,64(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 64);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,64(r11)
	PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_DA48_h"))) PPC_WEAK_FUNC(SinglesNetworkClient_DA48_h);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_DA48_h) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5460
	ctx.r11.s64 = ctx.r11.s64 + 5460;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,68(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 68);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bdab0
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,68(r11)
		PPC_STORE_U16(ctx.r11.u32 + 68, ctx.r8.u16);
		// beq cr6,0x823bda9c
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BDA9C:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,64(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 64);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,64(r11)
		PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r4.u16);
		// blr
		return;
	}
loc_823BDAB0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateResponseMessage_vfn_6"))) PPC_WEAK_FUNC(LoadingStateResponseMessage_vfn_6);
PPC_FUNC_IMPL(__imp__LoadingStateResponseMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5448(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5448);  /* glob:lbl_825D1548 @ 0x825d1548 */
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateResponseMessage_vfn_7"))) PPC_WEAK_FUNC(LoadingStateResponseMessage_vfn_7);
PPC_FUNC_IMPL(__imp__LoadingStateResponseMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4816
	ctx.r3.s64 = ctx.r11.s64 + -4816;
	// blr
	return;
}

__attribute__((alias("__imp__LoadingStateResponseMessage_vfn_1"))) PPC_WEAK_FUNC(LoadingStateResponseMessage_vfn_1);
PPC_FUNC_IMPL(__imp__LoadingStateResponseMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,16
	ctx.r5.s64 = 16;
	// addi r4,r11,8
	ctx.r4.s64 = ctx.r11.s64 + 8;
	// b 0x820d7830
	util_7830(ctx, base);
	return;
}

__attribute__((alias("__imp__LoadingStateResponseMessage_vfn_2"))) PPC_WEAK_FUNC(LoadingStateResponseMessage_vfn_2);
PPC_FUNC_IMPL(__imp__LoadingStateResponseMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,16
	ctx.r5.s64 = 16;
	// lhz r4,8(r11)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 8);
	// b 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	return;
}

__attribute__((alias("__imp__ForfeitMatchMessage_vfn_5"))) PPC_WEAK_FUNC(ForfeitMatchMessage_vfn_5);
PPC_FUNC_IMPL(__imp__ForfeitMatchMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,5484
	ctx.r11.s64 = ctx.r11.s64 + 5484;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,104(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 104);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,18(r3)
	PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r5.u16);
	// lhz r10,104(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 104);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bdb68
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,16(r8)
		PPC_STORE_U16(ctx.r8.u32 + 16, ctx.r9.u16);
	}
loc_823BDB68:
	// lhz r10,100(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 100);
	// sth r9,104(r11)
	PPC_STORE_U16(ctx.r11.u32 + 104, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,100(r11)
	PPC_STORE_U16(ctx.r11.u32 + 100, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetGameModeCoordinator_DB80"))) PPC_WEAK_FUNC(PongNetGameModeCoordinator_DB80);
PPC_FUNC_IMPL(__imp__PongNetGameModeCoordinator_DB80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5484
	ctx.r11.s64 = ctx.r11.s64 + 5484;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,104(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 104);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bdc04
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,18(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 18);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,104(r11)
		PPC_STORE_U16(ctx.r11.u32 + 104, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bdbf0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,16(r10)
			PPC_STORE_U16(ctx.r10.u32 + 16, ctx.r9.u16);
		}
	loc_823BDBF0:
		// sth r9,18(r3)
		PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r9.u16);
		// lhz r8,100(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 100);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,100(r11)
		PPC_STORE_U16(ctx.r11.u32 + 100, ctx.r7.u16);
		// blr
		return;
	}
loc_823BDC04:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__ForfeitMatchMessage_vfn_6"))) PPC_WEAK_FUNC(ForfeitMatchMessage_vfn_6);
PPC_FUNC_IMPL(__imp__ForfeitMatchMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5472(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5472);  /* glob:lbl_825D1560 @ 0x825d1560 */
	// blr
	return;
}

__attribute__((alias("__imp__ForfeitMatchMessage_vfn_7"))) PPC_WEAK_FUNC(ForfeitMatchMessage_vfn_7);
PPC_FUNC_IMPL(__imp__ForfeitMatchMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4788
	ctx.r3.s64 = ctx.r11.s64 + -4788;
	// blr
	return;
}

__attribute__((alias("__imp__ForfeitMatchMessage_vfn_1"))) PPC_WEAK_FUNC(ForfeitMatchMessage_vfn_1);
PPC_FUNC_IMPL(__imp__ForfeitMatchMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__ForfeitMatchMessage_vfn_2"))) PPC_WEAK_FUNC(ForfeitMatchMessage_vfn_2);
PPC_FUNC_IMPL(__imp__ForfeitMatchMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r31,12
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 12;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__TerminateRallyMessage_vfn_5"))) PPC_WEAK_FUNC(TerminateRallyMessage_vfn_5);
PPC_FUNC_IMPL(__imp__TerminateRallyMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5508
	ctx.r11.s64 = ctx.r11.s64 + 5508;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,124(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 124);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,124(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 124);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bdd60
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BDD60:
	// lhz r10,120(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 120);
	// sth r9,124(r11)
	PPC_STORE_U16(ctx.r11.u32 + 124, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,120(r11)
	PPC_STORE_U16(ctx.r11.u32 + 120, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__TerminateRallyMessage_vfn_6"))) PPC_WEAK_FUNC(TerminateRallyMessage_vfn_6);
PPC_FUNC_IMPL(__imp__TerminateRallyMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5496(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5496);  /* glob:lbl_825D1578 @ 0x825d1578 */
	// blr
	return;
}

__attribute__((alias("__imp__TerminateRallyMessage_vfn_7"))) PPC_WEAK_FUNC(TerminateRallyMessage_vfn_7);
PPC_FUNC_IMPL(__imp__TerminateRallyMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4768
	ctx.r3.s64 = ctx.r11.s64 + -4768;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesConnectRequest_vfn_1"))) PPC_WEAK_FUNC(SinglesConnectRequest_vfn_1);
PPC_FUNC_IMPL(__imp__SinglesConnectRequest_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// blr
	return;
}

__attribute__((alias("__imp__TerminateRallyMessage_vfn_4"))) PPC_WEAK_FUNC(TerminateRallyMessage_vfn_4);
PPC_FUNC_IMPL(__imp__TerminateRallyMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r11,-26068(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26068);
	// cmpwi cr6,r11,0
	// bne cr6,0x823bde40
	if (ctx.r11.s32 == 0) {
		// lis r11,-32160
		ctx.r11.s64 = -2107637760;
		// lwz r11,25500(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 25500);
		// cmplwi cr6,r11,0
		// beq cr6,0x823bde40
		if (ctx.r11.u32 == 0) {
			// blr
			return;
		}
		// lwz r10,12(r11)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
		// cmpwi cr6,r10,17
		// bge cr6,0x823bde40
		if (ctx.r10.s32 >= 17) {
			// blr
			return;
		}
		// lwz r9,16(r11)
		ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
		// cmpwi cr6,r9,17
		// bge cr6,0x823bde40
		if (ctx.r9.s32 >= 17) {
			// blr
			return;
		}
		// lwz r7,12(r8)
		// bctrl
		VCALL(ctx.r3.u32, 3, ctx, base);  // TerminateRallyMessage::vfn_3 (unnamed)  // vtable slot 3 (byte +12)
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,64
		ctx.r4.s64 = 64;
		// li r3,14409
		ctx.r3.s64 = 14409;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
	}
loc_823BDE40:
	// blr
	return;
}

__attribute__((alias("__imp__InternalMessageRelay_vfn_5"))) PPC_WEAK_FUNC(InternalMessageRelay_vfn_5);
PPC_FUNC_IMPL(__imp__InternalMessageRelay_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-1501
	// addi r11,r11,5532
	ctx.r11.s64 = ctx.r11.s64 + 5532;
	// ori r8,r9,11507
	ctx.r8.u64 = ctx.r9.u64 | 11507;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,5244(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 5244);
	// rlwinm r6,r7,23,9,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 23) & 0x7FFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,524
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(524));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,522(r3)
	PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r5.u16);
	// lhz r9,5244(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 5244);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bdea0
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,520(r9)
		PPC_STORE_U16(ctx.r9.u32 + 520, ctx.r10.u16);
	}
loc_823BDEA0:
	// sth r10,5244(r11)
	PPC_STORE_U16(ctx.r11.u32 + 5244, ctx.r10.u16);
	// lhz r10,5240(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 5240);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,5240(r11)
	PPC_STORE_U16(ctx.r11.u32 + 5240, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_DEB8_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_DEB8_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_DEB8_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5532
	ctx.r11.s64 = ctx.r11.s64 + 5532;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,5244(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 5244);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bdf24
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,524
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(524));
		// lhz r8,522(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 522);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,5244(r11)
		PPC_STORE_U16(ctx.r11.u32 + 5244, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bdf10
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,524
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(524));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,520(r5)
			PPC_STORE_U16(ctx.r5.u32 + 520, ctx.r9.u16);
		}
	loc_823BDF10:
		// sth r9,522(r3)
		PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r9.u16);
		// lhz r4,5240(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 5240);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,5240(r11)
		PPC_STORE_U16(ctx.r11.u32 + 5240, ctx.r10.u16);
		// blr
		return;
	}
loc_823BDF24:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__InternalMessageRelay_vfn_6"))) PPC_WEAK_FUNC(InternalMessageRelay_vfn_6);
PPC_FUNC_IMPL(__imp__InternalMessageRelay_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5520(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5520);  /* glob:lbl_825D1590 @ 0x825d1590 */
	// blr
	return;
}

__attribute__((alias("__imp__InternalMessageRelay_vfn_7"))) PPC_WEAK_FUNC(InternalMessageRelay_vfn_7);
PPC_FUNC_IMPL(__imp__InternalMessageRelay_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4744
	ctx.r3.s64 = ctx.r11.s64 + -4744;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_DF50_wrh"))) PPC_WEAK_FUNC(SinglesNetworkClient_DF50_wrh);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_DF50_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_29
	// addi r31,r3,8
	var_r31 = (uint32_t)(ctx.r3.s64 + 8);
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r29,r7
	var_r29 = ctx.r7.u32;
	// cmplwi cr6,r5,0
	// sth r11,0(r31)
	PPC_STORE_U16(var_r31 + 0, ctx.r11.u16);
	// sth r10,2(r31)
	PPC_STORE_U16(var_r31 + 2, ctx.r10.u16);
	// beq cr6,0x823be008
	if (ctx.r5.u32 != 0) {
		// mr r11,r5
		ctx.r11.u64 = ctx.r5.u64;
		// mr r10,r11
		ctx.r10.u64 = ctx.r11.u64;
	loc_823BDF84:
		// lbz r9,0(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// addi r11,r11,1
		ctx.r11.s64 = ctx.r11.s64 + 1;
		// cmplwi cr6,r9,0
		// bne cr6,0x823bdf84
		if (ctx.r9.u32 != 0) goto loc_823BDF84;
		// subf r10,r10,r11
		ctx.r10.s64 = ctx.r11.s64 - ctx.r10.s64;
		// lhz r3,2(r31)
		ctx.r3.u64 = PPC_LOAD_U16(var_r31 + 2);
		// addi r11,r1,80
		ctx.r11.s64 = ctx.r1.s64 + 80;
		// addi r8,r10,-1
		ctx.r8.s64 = ctx.r10.s64 + -1;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,0
		// sth r10,80(r1)
		PPC_STORE_U16(ctx.r1.u32 + 80, ctx.r10.u16);
		// lbz r7,0(r11)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r6,1(r11)
		ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1);
		// extsh r11,r3
		ctx.r11.s64 = ctx.r3.s16;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// stb r7,4(r11)
		PPC_STORE_U8(ctx.r11.u32 + 4, ctx.r7.u8);
		// stb r6,5(r11)
		PPC_STORE_U8(ctx.r11.u32 + 5, ctx.r6.u8);
		// lhz r11,2(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 2);
		// addi r9,r11,2
		ctx.r9.s64 = ctx.r11.s64 + 2;
		// sth r9,2(r31)
		PPC_STORE_U16(var_r31 + 2, ctx.r9.u16);
		// beq cr6,0x823be038
		if (ctx.r10.u32 == 0) goto loc_823BE038;
		// clrlwi r7,r9,16
		ctx.r7.u64 = ctx.r9.u32 & 0xFFFF;
		// extsh r30,r10
		var_r30 = (uint32_t)(ctx.r10.s16);
		// extsh r11,r7
		ctx.r11.s64 = ctx.r7.s16;
		// mr r4,r5
		ctx.r4.u64 = ctx.r5.u64;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// mr r5,r30
		ctx.r5.u64 = var_r30;
		// addi r3,r11,4
		ctx.r3.s64 = ctx.r11.s64 + 4;
		// bl 0x82434100
		memcpy(ctx, base);
		// lhz r11,2(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 2);
		// add r5,r11,r30
		ctx.r5.u64 = ctx.r11.u64 + var_r30;
		// sth r5,2(r31)
		PPC_STORE_U16(var_r31 + 2, ctx.r5.u16);
		// b 0x823be038
	} else {
	loc_823BE008:
		// addi r11,r1,80
		ctx.r11.s64 = ctx.r1.s64 + 80;
		// lhz r9,2(r31)
		ctx.r9.u64 = PPC_LOAD_U16(var_r31 + 2);
		// sth r10,80(r1)
		PPC_STORE_U16(ctx.r1.u32 + 80, ctx.r10.u16);
		// lbz r3,0(r11)
		ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r10,1(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1);
		// extsh r11,r9
		ctx.r11.s64 = ctx.r9.s16;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// stb r3,4(r11)
		PPC_STORE_U8(ctx.r11.u32 + 4, ctx.r3.u8);
		// stb r10,5(r11)
		PPC_STORE_U8(ctx.r11.u32 + 5, ctx.r10.u8);
		// lhz r11,2(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 2);
		// addi r7,r11,2
		ctx.r7.s64 = ctx.r11.s64 + 2;
		// sth r7,2(r31)
		PPC_STORE_U16(var_r31 + 2, ctx.r7.u16);
	}
loc_823BE038:
	// lhz r5,2(r31)
	ctx.r5.u64 = PPC_LOAD_U16(var_r31 + 2);
	// mr r11,r29
	ctx.r11.u64 = var_r29;
	// li r9,8
	ctx.r9.s64 = 8;
	// extsh r10,r5
	ctx.r10.s64 = ctx.r5.s16;
	// add r10,r10,r31
	ctx.r10.u64 = ctx.r10.u64 + var_r31;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_823BE054:
	// lbz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stb r4,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r4.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bdnz 0x823be054
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_823BE054;
	// lhz r3,2(r31)
	ctx.r3.u64 = PPC_LOAD_U16(var_r31 + 2);
	// addi r11,r3,8
	ctx.r11.s64 = ctx.r3.s64 + 8;
	// sth r11,2(r31)
	PPC_STORE_U16(var_r31 + 2, ctx.r11.u16);
	return;
}

__attribute__((alias("__imp__InternalMessageRelay_vfn_1"))) PPC_WEAK_FUNC(InternalMessageRelay_vfn_1);
PPC_FUNC_IMPL(__imp__InternalMessageRelay_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// addi r30,r3,8
	var_r30 = (uint32_t)(ctx.r3.s64 + 8);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// addi r29,r30,2
	var_r29 = (uint32_t)(var_r30 + 2);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x820d7970
	util_7970(ctx, base);
	// lhz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 0);
	// addi r4,r30,4
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 4;
	// extsh r5,r11
	ctx.r5.s64 = ctx.r11.s16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	return;
}

__attribute__((alias("__imp__InternalMessageRelay_vfn_2"))) PPC_WEAK_FUNC(InternalMessageRelay_vfn_2);
PPC_FUNC_IMPL(__imp__InternalMessageRelay_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,8
	ctx.r3.s64 = ctx.r3.s64 + 8;
	// b 0x82390598
	SinglesNetworkClient_0598_g(ctx, base);
	return;
}

__attribute__((alias("__imp__InternalMessageRelay_vfn_4"))) PPC_WEAK_FUNC(InternalMessageRelay_vfn_4);
PPC_FUNC_IMPL(__imp__InternalMessageRelay_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r22 = 0;
	uint32_t var_r24 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r23 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r25 = 0;
	uint32_t ea{};
	// FRAME: size=240, savegprlr_22
	// addi r31,r1,-240
	var_r31 = (uint32_t)(ctx.r1.s64 + -240);
	// mr r22,r3
	var_r22 = ctx.r3.u32;
	// li r24,0
	var_r24 = 0;
	// addi r30,r22,8
	var_r30 = (uint32_t)(var_r22 + 8);
	// mr r29,r24
	var_r29 = (uint32_t)(var_r24);
	// sth r24,516(r22)
	PPC_STORE_U16(var_r22 + 516, (uint16_t)var_r24);
	// lhz r11,508(r30)
	ctx.r11.u64 = PPC_LOAD_U16(var_r30 + 508);
	// lhz r23,0(r30)
	var_r23 = (uint32_t)(PPC_LOAD_U16(var_r30 + 0));
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// addi r9,r11,2
	ctx.r9.s64 = ctx.r11.s64 + 2;
	// add r10,r11,r30
	ctx.r10.u64 = ctx.r11.u64 + var_r30;
	// sth r9,508(r30)
	PPC_STORE_U16(var_r30 + 508, ctx.r9.u16);
	// lhz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r10.u32 + 4);
	// mr r28,r10
	var_r28 = ctx.r10.u32;
	// cmplwi cr6,r28,0
	// beq cr6,0x823be1c4
	if (var_r28 != 0) {
		// addi r7,r28,1
		ctx.r7.s64 = (int64_t)(int32_t)var_r28 + 1;
		// neg r6,r7
		ctx.r6.s64 = static_cast<int64_t>(-ctx.r7.u64);
		// rlwinm r12,r6,0,0,27
		ctx.r12.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 0) & 0xFFFFFFF0;
		// bl 0x82569394
		_RtlCheckStack12(ctx, base);
		// lwz r5,0(r1)
		ctx.r5.u64 = PPC_LOAD_U32(ctx.r1.u32 + 0);
		// rlwinm r4,r28,2,0,29
		ctx.r4.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFFFFFC;
		// neg r3,r4
		ctx.r3.s64 = static_cast<int64_t>(-ctx.r4.u64);
		// stwux r5,r1,r12
		ea = ctx.r1.u32 + ctx.r12.u32;
		PPC_STORE_U32(ea, ctx.r5.u32);
		ctx.r1.u32 = ea;
		// rlwinm r12,r3,0,0,27
		ctx.r12.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 0) & 0xFFFFFFF0;
		// addi r26,r1,144
		var_r26 = (uint32_t)(ctx.r1.s64 + 144);
		// bl 0x82569394
		_RtlCheckStack12(ctx, base);
		// lwz r11,0(r1)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 0);
		// extsh r27,r10
		var_r27 = (uint32_t)(ctx.r10.s16);
		// stbx r24,r28,r26
		PPC_STORE_U8(var_r28 + var_r26, (uint8_t)var_r24);
		// mr r3,r26
		ctx.r3.u64 = var_r26;
		// lhz r10,508(r30)
		ctx.r10.u64 = PPC_LOAD_U16(var_r30 + 508);
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// stwux r11,r1,r12
		ea = ctx.r1.u32 + ctx.r12.u32;
		PPC_STORE_U32(ea, ctx.r11.u32);
		ctx.r1.u32 = ea;
		// extsh r11,r10
		ctx.r11.s64 = ctx.r10.s16;
		// add r11,r11,r30
		ctx.r11.u64 = ctx.r11.u64 + var_r30;
		// addi r29,r1,144
		var_r29 = (uint32_t)(ctx.r1.s64 + 144);
		// addi r4,r11,4
		ctx.r4.s64 = ctx.r11.s64 + 4;
		// bl 0x82434100
		memcpy(ctx, base);
		// lhz r10,508(r30)
		ctx.r10.u64 = PPC_LOAD_U16(var_r30 + 508);
		// rlwinm r9,r27,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(var_r27 | (var_r27 << 32), 2) & 0xFFFFFFFC;
		// add r7,r10,r27
		ctx.r7.u64 = ctx.r10.u64 + var_r27;
		// extsh r27,r9
		var_r27 = (uint32_t)(ctx.r9.s16);
		// clrlwi r5,r7,16
		ctx.r5.u64 = ctx.r7.u32 & 0xFFFF;
		// mr r3,r29
		ctx.r3.u64 = var_r29;
		// extsh r25,r5
		var_r25 = (uint32_t)(ctx.r5.s16);
		// mr r5,r27
		ctx.r5.u64 = var_r27;
		// sth r7,508(r30)
		PPC_STORE_U16(var_r30 + 508, ctx.r7.u16);
		// add r11,r25,r30
		ctx.r11.u64 = var_r25 + var_r30;
		// addi r4,r11,4
		ctx.r4.s64 = ctx.r11.s64 + 4;
		// bl 0x82434100
		memcpy(ctx, base);
		// add r4,r25,r27
		ctx.r4.u64 = var_r25 + var_r27;
		// sth r4,508(r30)
		PPC_STORE_U16(var_r30 + 508, ctx.r4.u16);
		// b 0x823be1cc
	} else {
	loc_823BE1C4:
		// lis r11,-32254
		ctx.r11.s64 = -2113798144;
		// addi r26,r11,29731
		var_r26 = (uint32_t)(ctx.r11.s64 + 29731);  // lbl_82027423 @ 0x82027423
	}
loc_823BE1CC:
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x823919a0
	SinglesNetworkClient_19A0_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// bne cr6,0x823be1e4
	if (ctx.r11.u32 == 0) {
		// li r24,64
		var_r24 = 64;
	}
loc_823BE1E4:
	// clrlwi r4,r24,24
	ctx.r4.u64 = var_r24 & 0xFF;
	// cmplwi cr6,r28,4
	// bgt cr6,0x823be2c4
	if (var_r28 > 4) {
		// li r10,-1
		ctx.r10.s64 = -1;
		// sth r10,516(r22)
		PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
		// addi r1,r31,240
		ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
		// b 0x8242f8d0
		__restgprlr_22(ctx, base);
		return;
	}
	// lis r12,-32196
	// addi r12,r12,-7672
	ctx.r12.s64 = ctx.r12.s64 + -7672;
	// rlwinm r0,r28,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(var_r28 | (var_r28 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr
	switch (var_r28) {
	case 0:
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// mr r3,r23
		ctx.r3.u64 = var_r23;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
		// li r10,-1
		ctx.r10.s64 = -1;
		// sth r10,516(r22)
		PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
		// addi r1,r31,240
		ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
		// b 0x8242f8d0
		__restgprlr_22(ctx, base);
		return;
	case 1:
		// mr r5,r26
		ctx.r5.u64 = var_r26;
		// lwz r6,0(r29)
		ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 0);
		// mr r3,r23
		ctx.r3.u64 = var_r23;
		// bl 0x8225e480
		pg_E480(ctx, base);
		// li r10,-1
		ctx.r10.s64 = -1;
		// sth r10,516(r22)
		PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
		// addi r1,r31,240
		ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
		// b 0x8242f8d0
		__restgprlr_22(ctx, base);
		return;
	case 2:
		goto loc_823BE25C;
	case 3:
		goto loc_823BE280;
	case 4:
		goto loc_823BE2A8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_823BE21C:
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r3,r23
	ctx.r3.u64 = var_r23;
	// bl 0x8225e6e0
	pg_E6E0(ctx, base);
	// li r10,-1
	ctx.r10.s64 = -1;
	// sth r10,516(r22)
	PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
	// addi r1,r31,240
	ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
	// b 0x8242f8d0
	__restgprlr_22(ctx, base);
	return;
loc_823BE25C:
	// mr r5,r26
	ctx.r5.u64 = var_r26;
	// lwz r7,4(r29)
	ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 4);
	// mr r3,r23
	ctx.r3.u64 = var_r23;
	// lwz r6,0(r29)
	ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 0);
	// bl 0x8225e480
	pg_E480(ctx, base);
	// li r10,-1
	ctx.r10.s64 = -1;
	// sth r10,516(r22)
	PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
	// addi r1,r31,240
	ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
	// b 0x8242f8d0
	__restgprlr_22(ctx, base);
	return;
loc_823BE280:
	// mr r5,r26
	ctx.r5.u64 = var_r26;
	// lwz r8,8(r29)
	ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 8);
	// mr r3,r23
	ctx.r3.u64 = var_r23;
	// lwz r7,4(r29)
	ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 4);
	// lwz r6,0(r29)
	ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 0);
	// bl 0x8225e480
	pg_E480(ctx, base);
	// li r10,-1
	ctx.r10.s64 = -1;
	// sth r10,516(r22)
	PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
	// addi r1,r31,240
	ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
	// b 0x8242f8d0
	__restgprlr_22(ctx, base);
	return;
loc_823BE2A8:
	// mr r5,r26
	ctx.r5.u64 = var_r26;
	// lwz r9,12(r29)
	ctx.r9.u64 = PPC_LOAD_U32(var_r29 + 12);
	// mr r3,r23
	ctx.r3.u64 = var_r23;
	// lwz r8,8(r29)
	ctx.r8.u64 = PPC_LOAD_U32(var_r29 + 8);
	// lwz r7,4(r29)
	ctx.r7.u64 = PPC_LOAD_U32(var_r29 + 4);
	// lwz r6,0(r29)
	ctx.r6.u64 = PPC_LOAD_U32(var_r29 + 0);
	// bl 0x8225e480
	pg_E480(ctx, base);
loc_823BE2C4:
	// li r10,-1
	ctx.r10.s64 = -1;
	// sth r10,516(r22)
	PPC_STORE_U16(var_r22 + 516, ctx.r10.u16);
	// addi r1,r31,240
	ctx.r1.s64 = (int64_t)(int32_t)var_r31 + 240;
	// b 0x8242f8d0
	__restgprlr_22(ctx, base);
	return;
}

__attribute__((alias("__imp__LoadDataQuery_vfn_5"))) PPC_WEAK_FUNC(LoadDataQuery_vfn_5);
PPC_FUNC_IMPL(__imp__LoadDataQuery_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5556
	ctx.r11.s64 = ctx.r11.s64 + 5556;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823be338
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BE338:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__LoadDataQuery_vfn_6"))) PPC_WEAK_FUNC(LoadDataQuery_vfn_6);
PPC_FUNC_IMPL(__imp__LoadDataQuery_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5544(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5544);  /* glob:lbl_825D15A8 @ 0x825d15a8 */
	// blr
	return;
}

__attribute__((alias("__imp__LoadDataQuery_vfn_7"))) PPC_WEAK_FUNC(LoadDataQuery_vfn_7);
PPC_FUNC_IMPL(__imp__LoadDataQuery_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4720
	ctx.r3.s64 = ctx.r11.s64 + -4720;
	// blr
	return;
}

__attribute__((alias("__imp__LoadDataQueryResponse_vfn_5"))) PPC_WEAK_FUNC(LoadDataQueryResponse_vfn_5);
PPC_FUNC_IMPL(__imp__LoadDataQueryResponse_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,5580
	ctx.r11.s64 = ctx.r11.s64 + 5580;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,44(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// rlwinm r6,r7,28,4,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,18(r3)
	PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r5.u16);
	// lhz r10,44(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823be3d0
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,16(r8)
		PPC_STORE_U16(ctx.r8.u32 + 16, ctx.r9.u16);
	}
loc_823BE3D0:
	// lhz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 40);
	// sth r9,44(r11)
	PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,40(r11)
	PPC_STORE_U16(ctx.r11.u32 + 40, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_E3E8"))) PPC_WEAK_FUNC(game_E3E8);
PPC_FUNC_IMPL(__imp__game_E3E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5580
	ctx.r11.s64 = ctx.r11.s64 + 5580;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,44(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 44);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823be46c
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,18(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 18);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,44(r11)
		PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823be458
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,16(r10)
			PPC_STORE_U16(ctx.r10.u32 + 16, ctx.r9.u16);
		}
	loc_823BE458:
		// sth r9,18(r3)
		PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r9.u16);
		// lhz r8,40(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 40);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,40(r11)
		PPC_STORE_U16(ctx.r11.u32 + 40, ctx.r7.u16);
		// blr
		return;
	}
loc_823BE46C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__LoadDataQueryResponse_vfn_6"))) PPC_WEAK_FUNC(LoadDataQueryResponse_vfn_6);
PPC_FUNC_IMPL(__imp__LoadDataQueryResponse_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5568(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5568);  /* glob:lbl_825D15C0 @ 0x825d15c0 */
	// blr
	return;
}

__attribute__((alias("__imp__LoadDataQueryResponse_vfn_7"))) PPC_WEAK_FUNC(LoadDataQueryResponse_vfn_7);
PPC_FUNC_IMPL(__imp__LoadDataQueryResponse_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4704
	ctx.r3.s64 = ctx.r11.s64 + -4704;
	// blr
	return;
}

__attribute__((alias("__imp__LoadDataQueryResponse_vfn_1"))) PPC_WEAK_FUNC(LoadDataQueryResponse_vfn_1);
PPC_FUNC_IMPL(__imp__LoadDataQueryResponse_vfn_1) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,8
	ctx.r3.s64 = ctx.r3.s64 + 8;
	// b 0x82390748
	SinglesNetworkClient_0748_h(ctx, base);
	return;
}

__attribute__((alias("__imp__LoadDataQueryResponse_vfn_2"))) PPC_WEAK_FUNC(LoadDataQueryResponse_vfn_2);
PPC_FUNC_IMPL(__imp__LoadDataQueryResponse_vfn_2) {
	PPC_FUNC_PROLOGUE();
	// addi r3,r3,8
	ctx.r3.s64 = ctx.r3.s64 + 8;
	// b 0x82390808
	SinglesNetworkClient_0808_g(ctx, base);
	return;
}

__attribute__((alias("__imp__LoadDataQueryResponse_vfn_4"))) PPC_WEAK_FUNC(LoadDataQueryResponse_vfn_4);
PPC_FUNC_IMPL(__imp__LoadDataQueryResponse_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// lbz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 8);
	// lbz r9,9(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + 9);
	// extsb r10,r11
	ctx.r10.s64 = ctx.r11.s8;
	// lis r11,-32161
	// extsb r9,r9
	ctx.r9.s64 = ctx.r9.s8;
	// lwz r11,-21720(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21720);
	// stw r10,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r10.u32);
	// stw r10,100(r11)
	PPC_STORE_U32(ctx.r11.u32 + 100, ctx.r10.u32);
	// stw r9,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r9.u32);
	// stw r9,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r9.u32);
	// lbz r8,10(r3)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r3.u32 + 10);
	// lbz r7,11(r3)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r3.u32 + 11);
	// extsb r10,r8
	ctx.r10.s64 = ctx.r8.s8;
	// extsb r9,r7
	ctx.r9.s64 = ctx.r7.s8;
	// stw r10,88(r11)
	PPC_STORE_U32(ctx.r11.u32 + 88, ctx.r10.u32);
	// stw r10,112(r11)
	PPC_STORE_U32(ctx.r11.u32 + 112, ctx.r10.u32);
	// stw r9,92(r11)
	PPC_STORE_U32(ctx.r11.u32 + 92, ctx.r9.u32);
	// stw r9,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r9.u32);
	// lbz r6,12(r3)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r3.u32 + 12);
	// extsb r5,r6
	ctx.r5.s64 = ctx.r6.s8;
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// lbz r4,14(r3)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r3.u32 + 14);
	// stb r4,333(r11)
	PPC_STORE_U8(ctx.r11.u32 + 333, ctx.r4.u8);
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_5"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_5);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-13108
	// addi r11,r11,5604
	ctx.r11.s64 = ctx.r11.s64 + 5604;
	// ori r8,r9,52429
	ctx.r8.u64 = ctx.r9.u64 | 52429;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,1244(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1244);
	// rlwinm r6,r7,27,5,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x7FFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 3) & 0xFFFFFFF8;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,34(r3)
	PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r5.u16);
	// lhz r10,1244(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1244);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823be568
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,2,0,29
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,32(r8)
		PPC_STORE_U16(ctx.r8.u32 + 32, ctx.r9.u16);
	}
loc_823BE568:
	// lhz r10,1240(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1240);
	// sth r9,1244(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1244, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,1240(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1240, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_E580"))) PPC_WEAK_FUNC(game_E580);
PPC_FUNC_IMPL(__imp__game_E580) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5604
	ctx.r11.s64 = ctx.r11.s64 + 5604;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,1244(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1244);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823be604
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,34(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 34);
		// rlwinm r9,r10,2,0,29
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,3,0,28
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 3) & 0xFFFFFFF8;
		// sth r7,1244(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1244, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823be5f0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,2,0,29
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,3,0,28
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 3) & 0xFFFFFFF8;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,32(r10)
			PPC_STORE_U16(ctx.r10.u32 + 32, ctx.r9.u16);
		}
	loc_823BE5F0:
		// sth r9,34(r3)
		PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r9.u16);
		// lhz r8,1240(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1240);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,1240(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1240, ctx.r7.u16);
		// blr
		return;
	}
loc_823BE604:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_6"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_6);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5592(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5592);  /* glob:lbl_825D15D8 @ 0x825d15d8 */
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_7"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_7);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4680
	ctx.r3.s64 = ctx.r11.s64 + -4680;
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_1"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_1);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,24
	ctx.r5.s64 = 24;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_2"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_2);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,24
	ctx.r5.s64 = 24;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_4"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_4);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// lis r11,-32142
	ctx.r11.s64 = -2106458112;
	// lwz r11,-22608(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22608);
	// lwz r10,60(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// cmpwi cr6,r10,0
	// ble cr6,0x823be708
	if (ctx.r10.s32 > 0) {
		// lwz r11,56(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
		// b 0x823be70c
	} else {
	loc_823BE708:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BE70C:
	// addi r4,r3,8
	ctx.r4.s64 = ctx.r3.s64 + 8;
	// addi r3,r11,3756
	ctx.r3.s64 = ctx.r11.s64 + 3756;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823be728
	if (ctx.r3.u32 != 0) {
		// lwz r3,0(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// b 0x823be72c
	} else {
	loc_823BE728:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_823BE72C:
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r11,-4404
	ctx.r6.s64 = ctx.r11.s64 + -4404;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,-4432
	ctx.r5.s64 = ctx.r11.s64 + -4432;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823be758
	if (ctx.r3.u32 != 0) {
		// li r9,1
		ctx.r9.s64 = 1;
		// stb r9,454(r3)
		PPC_STORE_U8(ctx.r3.u32 + 454, ctx.r9.u8);
	}
loc_823BE758:
	// blr
	return;
}

__attribute__((alias("__imp__GamerReadyMessage_vfn_3"))) PPC_WEAK_FUNC(GamerReadyMessage_vfn_3);
PPC_FUNC_IMPL(__imp__GamerReadyMessage_vfn_3) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=144, manual
	// lis r11,-32142
	// lis r10,-32249
	ctx.r10.s64 = -2113470464;
	// addi r30,r10,-2952
	var_r30 = (uint32_t)(ctx.r10.s64 + -2952);  // lbl_8206F478 @ 0x8206f478
	// lwz r11,-22608(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22608);
	// lwz r10,60(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// cmpwi cr6,r10,0
	// ble cr6,0x823be7a0
	if (ctx.r10.s32 > 0) {
		// lwz r11,56(r11)
		ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
		// b 0x823be7a4
	} else {
	loc_823BE7A0:
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BE7A4:
	// addi r31,r3,8
	var_r31 = (uint32_t)(ctx.r3.s64 + 8);
	// addi r3,r11,3756
	ctx.r3.s64 = ctx.r11.s64 + 3756;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// bl 0x823ea250
	SinglesNetworkClient_A250_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823be7c4
	if (ctx.r3.u32 != 0) {
		// lwz r3,0(r3)
		ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
		// b 0x823be7c8
	} else {
	loc_823BE7C4:
		// li r3,0
		ctx.r3.s64 = 0;
	}
loc_823BE7C8:
	// lis r11,-32162
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r11,-4404
	ctx.r6.s64 = ctx.r11.s64 + -4404;
	// lis r11,-32162
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,-4432
	ctx.r5.s64 = ctx.r11.s64 + -4432;
	// bl 0x82430978
	SinglesNetworkClient_0978_g(ctx, base);
	// cmplwi cr6,r3,0
	// beq cr6,0x823be7f0
	if (ctx.r3.u32 != 0) {
		// addi r30,r3,76
		var_r30 = (uint32_t)(ctx.r3.s64 + 76);
	}
loc_823BE7F0:
	// lis r11,-32249
	// ld r5,0(r31)
	ctx.r5.u64 = PPC_LOAD_U64(var_r31 + 0);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// ld r6,8(r31)
	ctx.r6.u64 = PPC_LOAD_U64(var_r31 + 8);
	// addi r3,r11,-2940
	ctx.r3.s64 = ctx.r11.s64 + -2940;
	// ld r7,16(r31)
	ctx.r7.u64 = PPC_LOAD_U64(var_r31 + 16);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_5"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_5);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,30393
	ctx.r9.s64 = 1991835648;
	// addi r11,r11,5628
	ctx.r11.s64 = ctx.r11.s64 + 5628;
	// ori r8,r9,33243
	ctx.r8.u64 = ctx.r9.u64 | 33243;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,17116(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 17116);
	// rlwinm r6,r7,24,8,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 24) & 0xFFFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,552
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(552));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,546(r3)
	PPC_STORE_U16(ctx.r3.u32 + 546, ctx.r5.u16);
	// lhz r9,17116(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 17116);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823be878
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,552
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(552));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,544(r9)
		PPC_STORE_U16(ctx.r9.u32 + 544, ctx.r10.u16);
	}
loc_823BE878:
	// sth r10,17116(r11)
	PPC_STORE_U16(ctx.r11.u32 + 17116, ctx.r10.u16);
	// lhz r10,17112(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 17112);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,17112(r11)
	PPC_STORE_U16(ctx.r11.u32 + 17112, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetGameModeCoordinator_E890_p23"))) PPC_WEAK_FUNC(PongNetGameModeCoordinator_E890_p23);
PPC_FUNC_IMPL(__imp__PongNetGameModeCoordinator_E890_p23) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5628
	ctx.r11.s64 = ctx.r11.s64 + 5628;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,17116(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 17116);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823be8fc
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,552
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(552));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,552
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(552));
		// lhz r8,546(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 546);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,17116(r11)
		PPC_STORE_U16(ctx.r11.u32 + 17116, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823be8e8
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,552
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(552));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,544(r5)
			PPC_STORE_U16(ctx.r5.u32 + 544, ctx.r9.u16);
		}
	loc_823BE8E8:
		// sth r9,546(r3)
		PPC_STORE_U16(ctx.r3.u32 + 546, ctx.r9.u16);
		// lhz r4,17112(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 17112);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,17112(r11)
		PPC_STORE_U16(ctx.r11.u32 + 17112, ctx.r10.u16);
		// blr
		return;
	}
loc_823BE8FC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_6"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_6);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5616(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5616);  /* glob:lbl_825D15F0 @ 0x825d15f0 */
	// blr
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_7"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_7);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4660
	ctx.r3.s64 = ctx.r11.s64 + -4660;
	// blr
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_1"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_1);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,24
	ctx.r5.s64 = 24;
	// addi r4,r30,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f0,4(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r30 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// addi r30,r30,32
	var_r30 = (uint32_t)(var_r30 + 32);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// addi r29,r30,2
	var_r29 = (uint32_t)(var_r30 + 2);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x820d7970
	util_7970(ctx, base);
	// lhz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 0);
	// addi r4,r30,4
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 4;
	// extsh r5,r11
	ctx.r5.s64 = ctx.r11.s16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_2"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_2);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,24
	ctx.r5.s64 = 24;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,32
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 32;
	// bl 0x82390598
	SinglesNetworkClient_0598_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_4"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_4);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// lis r11,-32142
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// li r31,0
	var_r31 = 0;
	// addi r29,r28,8
	var_r29 = (uint32_t)(var_r28 + 8);
	// lwz r30,-22608(r11)
	var_r30 = (uint32_t)(PPC_LOAD_U32(ctx.r11.u32 + -22608));
loc_823BEA28:
	// mr r5,r29
	ctx.r5.u64 = var_r29;
	// mr r4,r31
	ctx.r4.u64 = var_r31;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x823accf8
	SinglesNetworkClient_CCF8_g(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x823bea54
	if (ctx.r3.u32 != 0) goto loc_823BEA54;
	// addi r31,r31,1
	var_r31 = (uint32_t)(var_r31 + 1);
	// cmpwi cr6,r31,2
	// blt cr6,0x823bea28
	if ((int32_t)var_r31 < 2) goto loc_823BEA28;
	// cmplwi cr6,r3,0
	// beq cr6,0x823bea5c
	if (ctx.r3.u32 != 0) {
	loc_823BEA54:
		// addi r4,r28,32
		ctx.r4.s64 = (int64_t)(int32_t)var_r28 + 32;
		// bl 0x823b5338
		atSingleton_5338_g_5338_1(ctx, base);
	}
loc_823BEA5C:
	return;
}

__attribute__((alias("__imp__GamerUpdateMessage_vfn_3"))) PPC_WEAK_FUNC(GamerUpdateMessage_vfn_3);
PPC_FUNC_IMPL(__imp__GamerUpdateMessage_vfn_3) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_28
	// lis r10,-32142
	// lis r11,-32249
	// li r30,0
	var_r30 = 0;
	// addi r28,r11,-2952
	var_r28 = (uint32_t)(ctx.r11.s64 + -2952);  // lbl_8206F478 @ 0x8206f478
	// addi r31,r3,8
	var_r31 = (uint32_t)(ctx.r3.s64 + 8);
	// lwz r29,-22608(r10)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r10.u32 + -22608));
loc_823BEA8C:
	// mr r5,r31
	ctx.r5.u64 = var_r31;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// bl 0x823accf8
	SinglesNetworkClient_CCF8_g(ctx, base);
	// cmplwi cr6,r3,0
	// bne cr6,0x823beab8
	if (ctx.r3.u32 != 0) goto loc_823BEAB8;
	// addi r30,r30,1
	var_r30 = (uint32_t)(var_r30 + 1);
	// cmpwi cr6,r30,2
	// blt cr6,0x823bea8c
	if ((int32_t)var_r30 < 2) goto loc_823BEA8C;
	// cmplwi cr6,r3,0
	// beq cr6,0x823beabc
	if (ctx.r3.u32 != 0) {
	loc_823BEAB8:
		// addi r28,r3,76
		var_r28 = (uint32_t)(ctx.r3.s64 + 76);
	}
loc_823BEABC:
	// lis r11,-32249
	// ld r5,0(r31)
	ctx.r5.u64 = PPC_LOAD_U64(var_r31 + 0);
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// ld r6,8(r31)
	ctx.r6.u64 = PPC_LOAD_U64(var_r31 + 8);
	// addi r3,r11,-2900
	ctx.r3.s64 = ctx.r11.s64 + -2900;
	// ld r7,16(r31)
	ctx.r7.u64 = PPC_LOAD_U64(var_r31 + 16);
	// bl 0x8240e6d0
	nop_8240E6D0(ctx, base);
	return;
}

__attribute__((alias("__imp__MatchStatsMessage_vfn_5"))) PPC_WEAK_FUNC(MatchStatsMessage_vfn_5);
PPC_FUNC_IMPL(__imp__MatchStatsMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-1501
	// addi r11,r11,5652
	ctx.r11.s64 = ctx.r11.s64 + 5652;
	// ori r8,r9,11507
	ctx.r8.u64 = ctx.r9.u64 | 11507;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,1052(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1052);
	// rlwinm r6,r7,23,9,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 23) & 0x7FFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,524
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(524));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,522(r3)
	PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r5.u16);
	// lhz r9,1052(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1052);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823beb30
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,520(r9)
		PPC_STORE_U16(ctx.r9.u32 + 520, ctx.r10.u16);
	}
loc_823BEB30:
	// sth r10,1052(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1052, ctx.r10.u16);
	// lhz r10,1048(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1048);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,1048(r11)
	PPC_STORE_U16(ctx.r11.u32 + 1048, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__game_EB48"))) PPC_WEAK_FUNC(game_EB48);
PPC_FUNC_IMPL(__imp__game_EB48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5652
	ctx.r11.s64 = ctx.r11.s64 + 5652;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,1052(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1052);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bebb4
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,524
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(524));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,524
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(524));
		// lhz r8,522(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 522);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,1052(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1052, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823beba0
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,524
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(524));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,520(r5)
			PPC_STORE_U16(ctx.r5.u32 + 520, ctx.r9.u16);
		}
	loc_823BEBA0:
		// sth r9,522(r3)
		PPC_STORE_U16(ctx.r3.u32 + 522, ctx.r9.u16);
		// lhz r4,1048(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 1048);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,1048(r11)
		PPC_STORE_U16(ctx.r11.u32 + 1048, ctx.r10.u16);
		// blr
		return;
	}
loc_823BEBB4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__MatchStatsMessage_vfn_6"))) PPC_WEAK_FUNC(MatchStatsMessage_vfn_6);
PPC_FUNC_IMPL(__imp__MatchStatsMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5640(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5640);  /* glob:lbl_825D1608 @ 0x825d1608 */
	// blr
	return;
}

__attribute__((alias("__imp__MatchStatsMessage_vfn_7"))) PPC_WEAK_FUNC(MatchStatsMessage_vfn_7);
PPC_FUNC_IMPL(__imp__MatchStatsMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4640
	ctx.r3.s64 = ctx.r11.s64 + -4640;
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_EBE0_wrh"))) PPC_WEAK_FUNC(SinglesNetworkClient_EBE0_wrh);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_EBE0_wrh) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=144, savegprlr_26
	// lis r11,-32142
	// addi r31,r3,8
	var_r31 = (uint32_t)(ctx.r3.s64 + 8);
	// li r27,0
	var_r27 = 0;
	// li r10,-1
	// li r9,30
	ctx.r9.s64 = 30;
	// lwz r11,-23768(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -23768);
	// mr r28,r27
	var_r28 = (uint32_t)(var_r27);
	// lis r26,-32142
	var_r26 = (uint32_t)(-2106458112);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// extsw r7,r8
	ctx.r7.s64 = ctx.r8.s32;
	// std r7,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r7.u64);
	// lfd f13,80(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f12,f13
	ctx.f12.f64 = double(ctx.f13.s64);
	// frsp f11,f12
	ctx.f11.f64 = double(float(ctx.f12.f64));
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,4(r3)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r3.u32 + 4,/* SinglesNetworkClient::flags@+0x4 */ temp.u32);
	// sth r27,0(r31)
	PPC_STORE_U16(var_r31 + 0, (uint16_t)var_r27);
	// sth r27,2(r31)
	PPC_STORE_U16(var_r31 + 2, (uint16_t)var_r27);
	// sth r10,508(r31)
	PPC_STORE_U16(var_r31 + 508, ctx.r10.u16);
	// sth r9,0(r31)
	PPC_STORE_U16(var_r31 + 0, ctx.r9.u16);
	// sth r27,2(r31)
	PPC_STORE_U16(var_r31 + 2, (uint16_t)var_r27);
loc_823BEC48:
	// lwz r11,-23772(r26)
	ctx.r11.u64 = PPC_LOAD_U32(var_r26 + -23772);
	// mr r30,r27
	var_r30 = (uint32_t)(var_r27);
	// add r6,r28,r11
	ctx.r6.u64 = var_r28 + ctx.r11.u64;
	// lwz r29,252(r6)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r6.u32 + 252));
loc_823BEC58:
	// lwz r5,0(r29)
	ctx.r5.u64 = PPC_LOAD_U32(var_r29 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lwz r11,8(r5)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r5.u32 + 8);
	// bctrl
	PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4)/* SinglesNetworkClient::flags@+0x4 */;
	// cmpwi cr6,r11,2
	// bne cr6,0x823becc0
	if (ctx.r11.s32 == 2) {
		// addi r11,r1,80
		ctx.r11.s64 = ctx.r1.s64 + 80;
		// lfs f9,0(r3)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
		ctx.f9.f64 = double(temp.f32);
		// lhz r6,2(r31)
		ctx.r6.u64 = PPC_LOAD_U16(var_r31 + 2);
		// stfs f9,80(r1)
		temp.f32 = float(ctx.f9.f64);
		PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
		// lbz r10,0(r11)
		ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r9,1(r11)
		ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1);
		// lbz r8,2(r11)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
		// lbz r7,3(r11)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3);
		// extsh r11,r6
		ctx.r11.s64 = ctx.r6.s16;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// stb r10,4(r11)
		PPC_STORE_U8(ctx.r11.u32 + 4, ctx.r10.u8);
		// stb r9,5(r11)
		PPC_STORE_U8(ctx.r11.u32 + 5, ctx.r9.u8);
		// stb r8,6(r11)
		PPC_STORE_U8(ctx.r11.u32 + 6, ctx.r8.u8);
		// stb r7,7(r11)
		PPC_STORE_U8(ctx.r11.u32 + 7, ctx.r7.u8);
		// lhz r11,2(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 2);
		// addi r4,r11,4
		ctx.r4.s64 = ctx.r11.s64 + 4;
		// b 0x823becf8
	} else {
	loc_823BECC0:
		// cmpwi cr6,r11,1
		// bne cr6,0x823becfc
		if (ctx.r11.s32 != 1) goto loc_823BECFC;
		// lwz r10,0(r3)
		ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0)/* SinglesNetworkClient::vtable@+0x0 */;
		// addi r11,r1,80
		ctx.r11.s64 = ctx.r1.s64 + 80;
		// lhz r6,2(r31)
		ctx.r6.u64 = PPC_LOAD_U16(var_r31 + 2);
		// sth r10,80(r1)
		PPC_STORE_U16(ctx.r1.u32 + 80, ctx.r10.u16);
		// lbz r8,0(r11)
		ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
		// lbz r7,1(r11)
		ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1);
		// extsh r11,r6
		ctx.r11.s64 = ctx.r6.s16;
		// add r11,r11,r31
		ctx.r11.u64 = ctx.r11.u64 + var_r31;
		// stb r8,4(r11)
		PPC_STORE_U8(ctx.r11.u32 + 4, ctx.r8.u8);
		// stb r7,5(r11)
		PPC_STORE_U8(ctx.r11.u32 + 5, ctx.r7.u8);
		// lhz r11,2(r31)
		ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 2);
		// addi r4,r11,2
		ctx.r4.s64 = ctx.r11.s64 + 2;
	}
loc_823BECF8:
	// sth r4,2(r31)
	PPC_STORE_U16(var_r31 + 2, ctx.r4.u16);
loc_823BECFC:
	// addi r30,r30,1
	var_r30 = (uint32_t)(var_r30 + 1);
	// cmpwi cr6,r30,30
	// blt cr6,0x823bec58
	if ((int32_t)var_r30 < 30) goto loc_823BEC58;
	// addi r28,r28,8
	var_r28 = (uint32_t)(var_r28 + 8);
	// cmpwi cr6,r28,16
	// blt cr6,0x823bec48
	if ((int32_t)var_r28 < 16) goto loc_823BEC48;
	return;
}

__attribute__((alias("__imp__DataSendMessage_vfn_1"))) PPC_WEAK_FUNC(DataSendMessage_vfn_1);
PPC_FUNC_IMPL(__imp__DataSendMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_29
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// addi r30,r29,8
	var_r30 = (uint32_t)(var_r29 + 8);
	// li r5,16
	ctx.r5.s64 = 16;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stfs f0,4(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r29 + 4, temp.u32);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// bl 0x820d7970
	util_7970(ctx, base);
	// addi r29,r30,2
	var_r29 = (uint32_t)(var_r30 + 2);
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r29
	ctx.r4.u64 = var_r29;
	// bl 0x820d7970
	util_7970(ctx, base);
	// lhz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U16(var_r29 + 0);
	// addi r4,r30,4
	ctx.r4.s64 = (int64_t)(int32_t)var_r30 + 4;
	// extsh r5,r11
	ctx.r5.s64 = ctx.r11.s16;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82260af0
	util_0AF0(ctx, base);
	return;
}

__attribute__((alias("__imp__DataSendMessage_vfn_2"))) PPC_WEAK_FUNC(DataSendMessage_vfn_2);
PPC_FUNC_IMPL(__imp__DataSendMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,8
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
	// bl 0x82390598
	SinglesNetworkClient_0598_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__MatchStatsMessage_vfn_4"))) PPC_WEAK_FUNC(MatchStatsMessage_vfn_4);
PPC_FUNC_IMPL(__imp__MatchStatsMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_27
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r11,0
	ctx.r11.s64 = 0;
	// li r28,0
	var_r28 = 0;
	// lis r27,-32142
	var_r27 = (uint32_t)(-2106458112);
	// sth r11,516(r31)
	PPC_STORE_U16(var_r31 + 516, ctx.r11.u16);
loc_823BEE00:
	// lwz r11,-23772(r27)
	ctx.r11.u64 = PPC_LOAD_U32(var_r27 + -23772);
	// li r30,0
	var_r30 = 0;
	// add r10,r28,r11
	ctx.r10.u64 = var_r28 + ctx.r11.u64;
	// lwz r29,252(r10)
	var_r29 = (uint32_t)(PPC_LOAD_U32(ctx.r10.u32 + 252));
loc_823BEE10:
	// lwz r9,0(r29)
  // [ph4a] vtable load collapsed
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// mr r3,r29
	ctx.r3.u64 = var_r29;
	// lwz r8,12(r9)
  // [ph4a] slot load collapsed
	// bctrl
	VCALL(var_r29, 3, ctx, base);  // pattern-B slot 3 (byte +12)
	// lwz r11,4(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// cmpwi cr6,r11,2
	// bne cr6,0x823bee5c
	if (ctx.r11.s32 == 2) {
		// addi r11,r31,8
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 8;
		// li r5,3
		ctx.r5.s64 = 3;
		// lhz r7,508(r11)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r11.u32 + 508);
		// extsh r10,r7
		ctx.r10.s64 = ctx.r7.s16;
		// addi r6,r10,4
		ctx.r6.s64 = ctx.r10.s64 + 4;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r6,508(r11)
		PPC_STORE_U16(ctx.r11.u32 + 508, ctx.r6.u16);
		// lfs f1,4(r9)
		ctx.fpscr.disableFlushMode();
		temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 4);
		ctx.f1.f64 = double(temp.f32);
		// bl 0x821f3078
		util_3078(ctx, base);
		// b 0x823bee8c
	} else {
	loc_823BEE5C:
		// cmpwi cr6,r11,1
		// bne cr6,0x823bee8c
		if (ctx.r11.s32 != 1) goto loc_823BEE8C;
		// addi r11,r31,8
		ctx.r11.s64 = (int64_t)(int32_t)var_r31 + 8;
		// li r5,4
		ctx.r5.s64 = 4;
		// lhz r10,508(r11)
		ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 508);
		// extsh r10,r10
		ctx.r10.s64 = ctx.r10.s16;
		// addi r8,r10,2
		ctx.r8.s64 = ctx.r10.s64 + 2;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,508(r11)
		PPC_STORE_U16(ctx.r11.u32 + 508, ctx.r8.u16);
		// lhz r6,4(r9)
		ctx.r6.u64 = PPC_LOAD_U16(ctx.r9.u32 + 4);
		// extsh r4,r6
		ctx.r4.s64 = ctx.r6.s16;
		// bl 0x821f2fc0
		util_2FC0(ctx, base);
	}
loc_823BEE8C:
	// addi r30,r30,1
	var_r30 = (uint32_t)(var_r30 + 1);
	// cmpwi cr6,r30,30
	// blt cr6,0x823bee10
	if ((int32_t)var_r30 < 30) goto loc_823BEE10;
	// addi r28,r28,8
	var_r28 = (uint32_t)(var_r28 + 8);
	// cmpwi cr6,r28,16
	// blt cr6,0x823bee00
	if ((int32_t)var_r28 < 16) goto loc_823BEE00;
	// li r5,-1
	ctx.r5.s64 = -1;
	// sth r5,516(r31)
	PPC_STORE_U16(var_r31 + 516, ctx.r5.u16);
	return;
}

__attribute__((alias("__imp__TwoMinuteWarningMessage_vfn_5"))) PPC_WEAK_FUNC(TwoMinuteWarningMessage_vfn_5);
PPC_FUNC_IMPL(__imp__TwoMinuteWarningMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5676
	ctx.r11.s64 = ctx.r11.s64 + 5676;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,28(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bef18
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BEF18:
	// lhz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
	// sth r9,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_EF30_g"))) PPC_WEAK_FUNC(SinglesNetworkClient_EF30_g);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_EF30_g) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5676
	ctx.r11.s64 = ctx.r11.s64 + 5676;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,28(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823befb4
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,28(r11)
		PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823befa0
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BEFA0:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,24(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 24);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,24(r11)
		PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r7.u16);
		// blr
		return;
	}
loc_823BEFB4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__TwoMinuteWarningMessage_vfn_6"))) PPC_WEAK_FUNC(TwoMinuteWarningMessage_vfn_6);
PPC_FUNC_IMPL(__imp__TwoMinuteWarningMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5664(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5664);  /* glob:lbl_825D1620 @ 0x825d1620 */
	// blr
	return;
}

__attribute__((alias("__imp__TwoMinuteWarningMessage_vfn_7"))) PPC_WEAK_FUNC(TwoMinuteWarningMessage_vfn_7);
PPC_FUNC_IMPL(__imp__TwoMinuteWarningMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4620
	ctx.r3.s64 = ctx.r11.s64 + -4620;
	// blr
	return;
}

__attribute__((alias("__imp__TwoMinuteWarningMessage_vfn_4"))) PPC_WEAK_FUNC(TwoMinuteWarningMessage_vfn_4);
PPC_FUNC_IMPL(__imp__TwoMinuteWarningMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// FRAME: size=96, manual
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x823919a0
	SinglesNetworkClient_19A0_g(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	// li r11,0
	ctx.r11.s64 = 0;
	// bne cr6,0x823bf008
	if (ctx.r11.u32 == 0) {
		// li r11,64
		ctx.r11.s64 = 64;
	}
loc_823BF008:
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// li r3,2151
	ctx.r3.s64 = 2151;
	// bl 0x8225e6e0
	pg_E6E0(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x823919a0
	SinglesNetworkClient_19A0_g(ctx, base);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r10,0
	// bne cr6,0x823bf044
	if (ctx.r10.u32 == 0) {
		// li r6,0
		ctx.r6.s64 = 0;
		// li r5,0
		ctx.r5.s64 = 0;
		// li r4,64
		ctx.r4.s64 = 64;
		// li r3,6151
		ctx.r3.s64 = 6151;
		// bl 0x8225e6e0
		pg_E6E0(ctx, base);
	}
loc_823BF044:
	// blr
	return;
}

__attribute__((alias("__imp__RequestDataMessage_vfn_5"))) PPC_WEAK_FUNC(RequestDataMessage_vfn_5);
PPC_FUNC_IMPL(__imp__RequestDataMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5700
	ctx.r11.s64 = ctx.r11.s64 + 5700;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// rlwinm r9,r10,28,4,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// lhz r8,36(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// clrlwi r10,r9,16
	ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
	// rlwinm r9,r10,4,12,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
	// add r7,r9,r11
	ctx.r7.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r8,14(r7)
	PPC_STORE_U16(ctx.r7.u32 + 14, ctx.r8.u16);
	// lhz r9,36(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf098
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// add r6,r9,r11
		ctx.r6.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,12(r6)
		PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r10.u16);
	}
loc_823BF098:
	// sth r10,36(r11)
	PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r10.u16);
	// lhz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// addi r5,r10,1
	ctx.r5.s64 = ctx.r10.s64 + 1;
	// sth r5,32(r11)
	PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r5.u16);
	// blr
	return;
}

__attribute__((alias("__imp__util_F0B0"))) PPC_WEAK_FUNC(util_F0B0);
PPC_FUNC_IMPL(__imp__util_F0B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5700
	ctx.r11.s64 = ctx.r11.s64 + 5700;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 36);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf118
	if (ctx.r9.u32 != 65535) {
		// rlwinm r9,r9,4,0,27
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
		// rlwinm r10,r10,4,12,27
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFF0;
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// lis r7,0
		ctx.r7.s64 = 0;
		// lhz r8,14(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 14);
		// ori r9,r7,65535
		ctx.r9.u64 = ctx.r7.u64 | 65535;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// sth r8,36(r11)
		PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
		// beq cr6,0x823bf104
		if (ctx.r10.u32 != 65535) {
			// rlwinm r10,r10,4,0,27
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
			// add r6,r10,r11
			ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,12(r6)
			PPC_STORE_U16(ctx.r6.u32 + 12, ctx.r9.u16);
		}
	loc_823BF104:
		// sth r9,14(r3)
		PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
		// lhz r5,32(r11)
		ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
		// add r4,r5,r9
		ctx.r4.u64 = ctx.r5.u64 + ctx.r9.u64;
		// sth r4,32(r11)
		PPC_STORE_U16(ctx.r11.u32 + 32, ctx.r4.u16);
		// blr
		return;
	}
loc_823BF118:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__RequestDataMessage_vfn_6"))) PPC_WEAK_FUNC(RequestDataMessage_vfn_6);
PPC_FUNC_IMPL(__imp__RequestDataMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5688(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5688);  /* glob:lbl_825D1638 @ 0x825d1638 */
	// blr
	return;
}

__attribute__((alias("__imp__RequestDataMessage_vfn_7"))) PPC_WEAK_FUNC(RequestDataMessage_vfn_7);
PPC_FUNC_IMPL(__imp__RequestDataMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4596
	ctx.r3.s64 = ctx.r11.s64 + -4596;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataMessage_vfn_5"))) PPC_WEAK_FUNC(MatchDataMessage_vfn_5);
PPC_FUNC_IMPL(__imp__MatchDataMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,2114
	ctx.r9.s64 = 138543104;
	// addi r11,r11,5724
	ctx.r11.s64 = ctx.r11.s64 + 5724;
	// ori r8,r9,4229
	ctx.r8.u64 = ctx.r9.u64 | 4229;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r9,r10,r8
	ctx.r9.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r7,252(r11)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r11.u32 + 252);
	// subf r6,r9,r10
	ctx.r6.s64 = ctx.r10.s64 - ctx.r9.s64;
	// rlwinm r10,r6,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 31) & 0x7FFFFFFF;
	// add r5,r10,r9
	ctx.r5.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rlwinm r4,r5,26,6,31
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 26) & 0x3FFFFFF;
	// clrlwi r10,r4,16
	ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
	// mr r3,r10
	ctx.r3.u64 = ctx.r10.u64;
	// mulli r9,r3,124
	ctx.r9.s64 = static_cast<int64_t>(ctx.r3.u64 * static_cast<uint64_t>(124));
	// add r9,r9,r11
	ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r7,122(r9)
	PPC_STORE_U16(ctx.r9.u32 + 122, ctx.r7.u16);
	// lhz r9,252(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 252);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf19c
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,124
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(124));
		// add r8,r9,r11
		ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,120(r8)
		PPC_STORE_U16(ctx.r8.u32 + 120, ctx.r10.u16);
	}
loc_823BF19C:
	// sth r10,252(r11)
	PPC_STORE_U16(ctx.r11.u32 + 252, ctx.r10.u16);
	// lhz r10,248(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 248);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,248(r11)
	PPC_STORE_U16(ctx.r11.u32 + 248, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__util_F1B0"))) PPC_WEAK_FUNC(util_F1B0);
PPC_FUNC_IMPL(__imp__util_F1B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5724
	ctx.r11.s64 = ctx.r11.s64 + 5724;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,252(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 252);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf21c
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,124
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(124));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,124
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(124));
		// lhz r8,122(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 122);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,252(r11)
		PPC_STORE_U16(ctx.r11.u32 + 252, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bf208
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,124
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,120(r5)
			PPC_STORE_U16(ctx.r5.u32 + 120, ctx.r9.u16);
		}
	loc_823BF208:
		// sth r9,122(r3)
		PPC_STORE_U16(ctx.r3.u32 + 122, ctx.r9.u16);
		// lhz r4,248(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 248);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,248(r11)
		PPC_STORE_U16(ctx.r11.u32 + 248, ctx.r10.u16);
		// blr
		return;
	}
loc_823BF21C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataMessage_vfn_6"))) PPC_WEAK_FUNC(MatchDataMessage_vfn_6);
PPC_FUNC_IMPL(__imp__MatchDataMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5712(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5712);  /* glob:lbl_825D1650 @ 0x825d1650 */
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataMessage_vfn_7"))) PPC_WEAK_FUNC(MatchDataMessage_vfn_7);
PPC_FUNC_IMPL(__imp__MatchDataMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4576
	ctx.r3.s64 = ctx.r11.s64 + -4576;
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataMessage_vfn_1"))) PPC_WEAK_FUNC(MatchDataMessage_vfn_1);
PPC_FUNC_IMPL(__imp__MatchDataMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// cmplwi cr6,r11,0
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x823bf2a0
	if (ctx.r11.u32 == 0) {
		// li r11,0
		ctx.r11.s64 = 0;
	}
loc_823BF2A0:
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// stb r11,8(r31)
	PPC_STORE_U8(var_r31 + 8, ctx.r11.u8);
	// addi r3,r31,12
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
	// bl 0x82100640
	SinglesNetworkClient_0640_w(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__MatchDataMessage_vfn_2"))) PPC_WEAK_FUNC(MatchDataMessage_vfn_2);
PPC_FUNC_IMPL(__imp__MatchDataMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lbz r4,8(r31)
	ctx.r4.u64 = PPC_LOAD_U8(var_r31 + 8);
	// bl 0x820d6918
	SinglesNetworkClient_6918_g(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = var_r30;
	// addi r3,r31,12
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 12;
	// bl 0x82100860
	SinglesNetworkClient_0860_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__RoundRobinDataMessage_vfn_5"))) PPC_WEAK_FUNC(RoundRobinDataMessage_vfn_5);
PPC_FUNC_IMPL(__imp__RoundRobinDataMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-30438
	// addi r11,r11,5748
	ctx.r11.s64 = ctx.r11.s64 + 5748;
	// ori r8,r9,51003
	ctx.r8.u64 = ctx.r9.u64 | 51003;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,960(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 960);
	// rlwinm r6,r7,23,9,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 23) & 0x7FFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,956
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(956));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,954(r3)
	PPC_STORE_U16(ctx.r3.u32 + 954, ctx.r5.u16);
	// lhz r9,960(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 960);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf370
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,956
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(956));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,952(r9)
		PPC_STORE_U16(ctx.r9.u32 + 952, ctx.r10.u16);
	}
loc_823BF370:
	// sth r10,960(r11)
	PPC_STORE_U16(ctx.r11.u32 + 960, ctx.r10.u16);
	// lhz r10,956(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 956);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,956(r11)
	PPC_STORE_U16(ctx.r11.u32 + 956, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetRoundRobinCoordinator_F388_v12"))) PPC_WEAK_FUNC(PongNetRoundRobinCoordinator_F388_v12);
PPC_FUNC_IMPL(__imp__PongNetRoundRobinCoordinator_F388_v12) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5748
	ctx.r11.s64 = ctx.r11.s64 + 5748;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,960(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 960);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf3f4
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,956
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(956));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,956
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(956));
		// lhz r8,954(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 954);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,960(r11)
		PPC_STORE_U16(ctx.r11.u32 + 960, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bf3e0
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,956
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(956));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,952(r5)
			PPC_STORE_U16(ctx.r5.u32 + 952, ctx.r9.u16);
		}
	loc_823BF3E0:
		// sth r9,954(r3)
		PPC_STORE_U16(ctx.r3.u32 + 954, ctx.r9.u16);
		// lhz r4,956(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 956);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,956(r11)
		PPC_STORE_U16(ctx.r11.u32 + 956, ctx.r10.u16);
		// blr
		return;
	}
loc_823BF3F4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__RoundRobinDataMessage_vfn_6"))) PPC_WEAK_FUNC(RoundRobinDataMessage_vfn_6);
PPC_FUNC_IMPL(__imp__RoundRobinDataMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5736(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5736);  /* glob:lbl_825D1668 @ 0x825d1668 */
	// blr
	return;
}

__attribute__((alias("__imp__RoundRobinDataMessage_vfn_7"))) PPC_WEAK_FUNC(RoundRobinDataMessage_vfn_7);
PPC_FUNC_IMPL(__imp__RoundRobinDataMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4556
	ctx.r3.s64 = ctx.r11.s64 + -4556;
	// blr
	return;
}

__attribute__((alias("__imp__RoundRobinDataMessage_vfn_0"))) PPC_WEAK_FUNC(RoundRobinDataMessage_vfn_0);
PPC_FUNC_IMPL(__imp__RoundRobinDataMessage_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// addi r3,r31,8
	ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
	// bl 0x822100e0
	PongNetRoundRobinCoordinator_00E0_g(ctx, base);
	// lis r11,-32249
	// clrlwi r10,r30,31
	ctx.r10.u64 = var_r30 & 0x1;
	// addi r11,r11,-15612
	ctx.r11.s64 = ctx.r11.s64 + -15612;
	// cmplwi cr6,r10,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0, ctx.r11.u32);
	// beq cr6,0x823bf468
	if (ctx.r10.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823BF468:
	// blr
	return;
}

__attribute__((alias("__imp__RoundRobinDataMessage_vfn_1"))) PPC_WEAK_FUNC(RoundRobinDataMessage_vfn_1);
PPC_FUNC_IMPL(__imp__RoundRobinDataMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r5,944
	ctx.r5.s64 = 944;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r31 + 4, temp.u32);
	// bl 0x82260af0
	util_0AF0(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__RoundRobinDataMessage_vfn_2"))) PPC_WEAK_FUNC(RoundRobinDataMessage_vfn_2);
PPC_FUNC_IMPL(__imp__RoundRobinDataMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// lfs f1,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r31 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// li r5,944
	ctx.r5.s64 = 944;
	// addi r4,r31,8
	ctx.r4.s64 = (int64_t)(int32_t)var_r31 + 8;
	// mr r3,r30
	ctx.r3.u64 = var_r30;
	// bl 0x820d67c8
	SinglesNetworkClient_67C8_g(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__StartLobbyCountdownMessage_vfn_5"))) PPC_WEAK_FUNC(StartLobbyCountdownMessage_vfn_5);
PPC_FUNC_IMPL(__imp__StartLobbyCountdownMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5772
	ctx.r11.s64 = ctx.r11.s64 + 5772;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bf590
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BF590:
	// lhz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
	// sth r9,16(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_F5A8_p23"))) PPC_WEAK_FUNC(SinglesNetworkClient_F5A8_p23);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_F5A8_p23) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5772
	ctx.r11.s64 = ctx.r11.s64 + 5772;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bf62c
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,16(r11)
		PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bf618
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BF618:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,12(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,12(r11)
		PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
		// blr
		return;
	}
loc_823BF62C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__StartLobbyCountdownMessage_vfn_6"))) PPC_WEAK_FUNC(StartLobbyCountdownMessage_vfn_6);
PPC_FUNC_IMPL(__imp__StartLobbyCountdownMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5760(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5760);  /* glob:lbl_825D1680 @ 0x825d1680 */
	// blr
	return;
}

__attribute__((alias("__imp__StartLobbyCountdownMessage_vfn_7"))) PPC_WEAK_FUNC(StartLobbyCountdownMessage_vfn_7);
PPC_FUNC_IMPL(__imp__StartLobbyCountdownMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4532
	ctx.r3.s64 = ctx.r11.s64 + -4532;
	// blr
	return;
}

__attribute__((alias("__imp__StartLobbyCountdownMessage_vfn_4"))) PPC_WEAK_FUNC(StartLobbyCountdownMessage_vfn_4);
PPC_FUNC_IMPL(__imp__StartLobbyCountdownMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// FRAME: size=128, manual
	// lfs f0,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32252
	// stfs f0,112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// li r4,64
	ctx.r4.s64 = 64;
	// addi r5,r11,-14712
	ctx.r5.s64 = ctx.r11.s64 + -14712;
	// li r3,14402
	ctx.r3.s64 = 14402;
	// lwz r6,112(r1)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 112);
	// bl 0x8225e480
	pg_E480(ctx, base);
	// blr
	return;
}

__attribute__((alias("__imp__CancelLobbyCountdownMessage_vfn_5"))) PPC_WEAK_FUNC(CancelLobbyCountdownMessage_vfn_5);
PPC_FUNC_IMPL(__imp__CancelLobbyCountdownMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,-21846
	// addi r11,r11,5796
	ctx.r11.s64 = ctx.r11.s64 + 5796;
	// ori r8,r9,43691
	ctx.r8.u64 = ctx.r9.u64 | 43691;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// rlwinm r6,r7,29,3,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r9,r6,16
	ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r10,r9
	ctx.r10.u64 = ctx.r9.u64;
	// rlwinm r8,r10,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
	// add r4,r10,r8
	ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
	// rlwinm r10,r4,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// sth r5,10(r3)
	PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r5.u16);
	// lhz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bf6f8
	if (ctx.r10.u32 != 65535) {
		// rlwinm r8,r10,1,0,30
		ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r10,r10,r8
		ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r8,r10,r11
		ctx.r8.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r9,8(r8)
		PPC_STORE_U16(ctx.r8.u32 + 8, ctx.r9.u16);
	}
loc_823BF6F8:
	// lhz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
	// sth r9,16(r11)
	PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r9.u16);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// sth r7,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
	// blr
	return;
}

__attribute__((alias("__imp__SinglesNetworkClient_F710_p23"))) PPC_WEAK_FUNC(SinglesNetworkClient_F710_p23);
PPC_FUNC_IMPL(__imp__SinglesNetworkClient_F710_p23) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5796
	ctx.r11.s64 = ctx.r11.s64 + 5796;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 16);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// cmplwi cr6,r10,65535
	// beq cr6,0x823bf794
	if (ctx.r10.u32 != 65535) {
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// lis r5,0
		ctx.r5.s64 = 0;
		// add r10,r10,r9
		ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
		// rlwinm r10,r10,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
		// add r9,r10,r11
		ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// lhz r7,10(r9)
		ctx.r7.u64 = PPC_LOAD_U16(ctx.r9.u32 + 10);
		// rlwinm r9,r10,1,0,30
		ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
		// add r6,r10,r9
		ctx.r6.u64 = ctx.r10.u64 + ctx.r9.u64;
		// ori r9,r5,65535
		ctx.r9.u64 = ctx.r5.u64 | 65535;
		// rlwinm r10,r6,2,0,29
		ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
		// sth r7,16(r11)
		PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r7.u16);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// clrlwi r10,r7,16
		ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bf780
		if (ctx.r10.u32 != 65535) {
			// rlwinm r8,r10,1,0,30
			ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 1) & 0xFFFFFFFE;
			// add r4,r10,r8
			ctx.r4.u64 = ctx.r10.u64 + ctx.r8.u64;
			// rlwinm r10,r4,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// add r10,r10,r11
			ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,8(r10)
			PPC_STORE_U16(ctx.r10.u32 + 8, ctx.r9.u16);
		}
	loc_823BF780:
		// sth r9,10(r3)
		PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r9.u16);
		// lhz r8,12(r11)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r11.u32 + 12);
		// add r7,r8,r9
		ctx.r7.u64 = ctx.r8.u64 + ctx.r9.u64;
		// sth r7,12(r11)
		PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r7.u16);
		// blr
		return;
	}
loc_823BF794:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__CancelLobbyCountdownMessage_vfn_6"))) PPC_WEAK_FUNC(CancelLobbyCountdownMessage_vfn_6);
PPC_FUNC_IMPL(__imp__CancelLobbyCountdownMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5784(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5784);  /* glob:lbl_825D1698 @ 0x825d1698 */
	// blr
	return;
}

__attribute__((alias("__imp__CancelLobbyCountdownMessage_vfn_7"))) PPC_WEAK_FUNC(CancelLobbyCountdownMessage_vfn_7);
PPC_FUNC_IMPL(__imp__CancelLobbyCountdownMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4504
	ctx.r3.s64 = ctx.r11.s64 + -4504;
	// blr
	return;
}

__attribute__((alias("__imp__CancelLobbyCountdownMessage_vfn_4"))) PPC_WEAK_FUNC(CancelLobbyCountdownMessage_vfn_4);
PPC_FUNC_IMPL(__imp__CancelLobbyCountdownMessage_vfn_4) {
	PPC_FUNC_PROLOGUE();
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,64
	ctx.r4.s64 = 64;
	// li r3,14403
	ctx.r3.s64 = 14403;
	// b 0x8225e6e0
	pg_E6E0(ctx, base);
	return;
}

__attribute__((alias("__imp__PlayerCacheMessage_vfn_5"))) PPC_WEAK_FUNC(PlayerCacheMessage_vfn_5);
PPC_FUNC_IMPL(__imp__PlayerCacheMessage_vfn_5) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// lis r9,8886
	ctx.r9.s64 = 582352896;
	// addi r11,r11,5820
	ctx.r11.s64 = ctx.r11.s64 + 5820;
	// ori r8,r9,15551
	ctx.r8.u64 = ctx.r9.u64 | 15551;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r10,r11,r3
	ctx.r10.s64 = ctx.r3.s64 - ctx.r11.s64;
	// mulhwu r7,r10,r8
	ctx.r7.u64 = (uint64_t(ctx.r10.u32) * uint64_t(ctx.r8.u32)) >> 32;
	// lhz r5,476(r11)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r11.u32 + 476);
	// rlwinm r6,r7,26,6,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 26) & 0x3FFFFFF;
	// clrlwi r10,r6,16
	ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// mulli r9,r4,472
	ctx.r9.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(472));
	// add r3,r9,r11
	ctx.r3.u64 = ctx.r9.u64 + ctx.r11.u64;
	// sth r5,466(r3)
	PPC_STORE_U16(ctx.r3.u32 + 466, ctx.r5.u16);
	// lhz r9,476(r11)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r11.u32 + 476);
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf828
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,472
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(472));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// sth r10,464(r9)
		PPC_STORE_U16(ctx.r9.u32 + 464, ctx.r10.u16);
	}
loc_823BF828:
	// sth r10,476(r11)
	PPC_STORE_U16(ctx.r11.u32 + 476, ctx.r10.u16);
	// lhz r10,472(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 472);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// sth r8,472(r11)
	PPC_STORE_U16(ctx.r11.u32 + 472, ctx.r8.u16);
	// blr
	return;
}

__attribute__((alias("__imp__PongNetGameModeCoordinator_F840_p23"))) PPC_WEAK_FUNC(PongNetGameModeCoordinator_F840_p23);
PPC_FUNC_IMPL(__imp__PongNetGameModeCoordinator_F840_p23) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	// addi r11,r11,5820
	ctx.r11.s64 = ctx.r11.s64 + 5820;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lhz r10,476(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 476);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// cmplwi cr6,r9,65535
	// beq cr6,0x823bf8ac
	if (ctx.r9.u32 != 65535) {
		// mulli r9,r9,472
		ctx.r9.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(472));
		// add r9,r9,r11
		ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
		// clrlwi r7,r10,16
		ctx.r7.u64 = ctx.r10.u32 & 0xFFFF;
		// lis r6,0
		ctx.r6.s64 = 0;
		// mulli r10,r7,472
		ctx.r10.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(472));
		// lhz r8,466(r9)
		ctx.r8.u64 = PPC_LOAD_U16(ctx.r9.u32 + 466);
		// add r3,r10,r11
		ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
		// sth r8,476(r11)
		PPC_STORE_U16(ctx.r11.u32 + 476, ctx.r8.u16);
		// clrlwi r10,r8,16
		ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
		// ori r9,r6,65535
		ctx.r9.u64 = ctx.r6.u64 | 65535;
		// cmplwi cr6,r10,65535
		// beq cr6,0x823bf898
		if (ctx.r10.u32 != 65535) {
			// mulli r10,r10,472
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(472));
			// add r5,r10,r11
			ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
			// sth r9,464(r5)
			PPC_STORE_U16(ctx.r5.u32 + 464, ctx.r9.u16);
		}
	loc_823BF898:
		// sth r9,466(r3)
		PPC_STORE_U16(ctx.r3.u32 + 466, ctx.r9.u16);
		// lhz r4,472(r11)
		ctx.r4.u64 = PPC_LOAD_U16(ctx.r11.u32 + 472);
		// add r10,r4,r9
		ctx.r10.u64 = ctx.r4.u64 + ctx.r9.u64;
		// sth r10,472(r11)
		PPC_STORE_U16(ctx.r11.u32 + 472, ctx.r10.u16);
		// blr
		return;
	}
loc_823BF8AC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerCacheMessage_vfn_6"))) PPC_WEAK_FUNC(PlayerCacheMessage_vfn_6);
PPC_FUNC_IMPL(__imp__PlayerCacheMessage_vfn_6) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lwz r3,5808(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 5808);  /* glob:lbl_825D16B0 @ 0x825d16b0 */
	// blr
	return;
}

__attribute__((alias("__imp__PlayerCacheMessage_vfn_7"))) PPC_WEAK_FUNC(PlayerCacheMessage_vfn_7);
PPC_FUNC_IMPL(__imp__PlayerCacheMessage_vfn_7) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r3,r11,-4476
	ctx.r3.s64 = ctx.r11.s64 + -4476;
	// blr
	return;
}

__attribute__((alias("__imp__PlayerCacheMessage_vfn_1"))) PPC_WEAK_FUNC(PlayerCacheMessage_vfn_1);
PPC_FUNC_IMPL(__imp__PlayerCacheMessage_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r27 = 0;
	PPCRegister temp{};
	// FRAME: size=192, savegprlr_27
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// li r29,0
	var_r29 = 0;
	// li r5,8
	ctx.r5.s64 = 8;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// stfs f0,4(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(var_r30 + 4, temp.u32);
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r29,456(r30)
	PPC_STORE_U32(var_r30 + 456, var_r29);
	// bl 0x82238df8
	SinglesNetworkClient_8DF8_g(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// cmpwi cr6,r11,0
	// ble cr6,0x823bf9e0
	if (ctx.r11.s32 > 0) {
		// mr r28,r11
		var_r28 = ctx.r11.u32;
		// lis r11,-32249
		// addi r30,r30,8
		var_r30 = (uint32_t)(var_r30 + 8);
		// addi r27,r11,-6548
		var_r27 = (uint32_t)(ctx.r11.s64 + -6548);  // lbl_8206E66C @ 0x8206e66c
	loc_823BF938:
		// li r5,24
		ctx.r5.s64 = 24;
		// stb r29,112(r1)
		PPC_STORE_U8(ctx.r1.u32 + 112, (uint8_t)var_r29);
		// addi r4,r1,112
		ctx.r4.s64 = ctx.r1.s64 + 112;
		// std r29,128(r1)
		PPC_STORE_U64(ctx.r1.u32 + 128, var_r29);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260af0
		util_0AF0(ctx, base);
		// li r5,16
		ctx.r5.s64 = 16;
		// addi r4,r1,96
		ctx.r4.s64 = ctx.r1.s64 + 96;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260a70
		SinglesNetworkClient_0A70_g(ctx, base);
		// li r5,8
		ctx.r5.s64 = 8;
		// addi r4,r1,88
		ctx.r4.s64 = ctx.r1.s64 + 88;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260e18
		SinglesNetworkClient_0E18_g(ctx, base);
		// li r5,8
		ctx.r5.s64 = 8;
		// addi r4,r1,84
		ctx.r4.s64 = ctx.r1.s64 + 84;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260e18
		SinglesNetworkClient_0E18_g(ctx, base);
		// li r5,8
		ctx.r5.s64 = 8;
		// addi r4,r1,80
		ctx.r4.s64 = ctx.r1.s64 + 80;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260e18
		SinglesNetworkClient_0E18_g(ctx, base);
		// lwz r11,448(r30)
		ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 448);
		// cmpwi cr6,r11,8
		// bge cr6,0x823bf9c8
		if (ctx.r11.s32 < 8) {
			// mulli r10,r11,56
			ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(56));
			// lwz r8,80(r1)
			ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
			// addi r11,r11,1
			ctx.r11.s64 = ctx.r11.s64 + 1;
			// lwz r7,84(r1)
			ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
			// lwz r6,88(r1)
			ctx.r6.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
			// add r3,r10,r30
			ctx.r3.u64 = ctx.r10.u64 + var_r30;
			// addi r5,r1,96
			ctx.r5.s64 = ctx.r1.s64 + 96;
			// addi r4,r1,112
			ctx.r4.s64 = ctx.r1.s64 + 112;
			// stw r11,448(r30)
			PPC_STORE_U32(var_r30 + 448, ctx.r11.u32);
			// bl 0x823b3f08
			SinglesNetworkClient_3F08_g(ctx, base);
			// b 0x823bf9d4
		} else {
		loc_823BF9C8:
			// addi r4,r1,96
			ctx.r4.s64 = ctx.r1.s64 + 96;
			// mr r3,r27
			ctx.r3.u64 = var_r27;
			// bl 0x8240e6d0
			nop_8240E6D0(ctx, base);
		}
	loc_823BF9D4:
		// addi r28,r28,-1
		var_r28 = (uint32_t)(var_r28 + -1);
		// cmplwi cr6,r28,0
		// bne cr6,0x823bf938
		if (var_r28 != 0) goto loc_823BF938;
	}
loc_823BF9E0:
	return;
}

__attribute__((alias("__imp__PlayerCacheMessage_vfn_2"))) PPC_WEAK_FUNC(PlayerCacheMessage_vfn_2);
PPC_FUNC_IMPL(__imp__PlayerCacheMessage_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r27 = 0;
	PPCRegister temp{};
	// FRAME: size=128, savegprlr_27
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// mr r31,r4
	var_r31 = ctx.r4.u32;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// lfs f1,4(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(var_r30 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x820d6990
	game_6990(ctx, base);
	// lwz r28,456(r30)
	var_r28 = (uint32_t)(PPC_LOAD_U32(var_r30 + 456));
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// mr r4,r28
	ctx.r4.u64 = var_r28;
	// bl 0x820d6838
	SinglesNetworkClient_6838_g(ctx, base);
	// cmpwi cr6,r28,0
	// ble cr6,0x823bfb9c
	if ((int32_t)var_r28 > 0) {
		// addi r30,r30,48
		var_r30 = (uint32_t)(var_r30 + 48);
		// li r29,32
		var_r29 = 32;
	loc_823BFA2C:
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// li r5,24
		ctx.r5.s64 = 24;
		// addi r4,r30,-40
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + -40;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260738
		SinglesNetworkClient_0738_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r27,16(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 16));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// addi r11,r27,7
		ctx.r11.s64 = (int64_t)(int32_t)var_r27 + 7;
		// li r5,16
		ctx.r5.s64 = 16;
		// lwz r27,32(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// srawi r4,r11,3
		ctx.xer.ca = (ctx.r11.s32 < 0) & ((ctx.r11.u32 & 0x7) != 0);
		ctx.r4.s64 = ctx.r11.s32 >> 3;
		// stw r29,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r29);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260448
		SinglesNetworkClient_0448_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r27,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r27);
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// li r5,16
		ctx.r5.s64 = 16;
		// addi r4,r30,-16
		ctx.r4.s64 = (int64_t)(int32_t)var_r30 + -16;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260688
		SinglesNetworkClient_0688_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r27,16(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 16));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// addi r10,r27,7
		ctx.r10.s64 = (int64_t)(int32_t)var_r27 + 7;
		// li r5,16
		ctx.r5.s64 = 16;
		// lwz r27,32(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// srawi r4,r10,3
		ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0x7) != 0);
		ctx.r4.s64 = ctx.r10.s32 >> 3;
		// stw r29,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r29);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260448
		SinglesNetworkClient_0448_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r27,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r27);
		// lwz r27,0(r30)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r30 + 0));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// li r5,8
		ctx.r5.s64 = 8;
		// mr r4,r27
		ctx.r4.u64 = var_r27;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x822605f0
		atSingleton_05F0_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r27,16(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 16));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// addi r9,r27,7
		ctx.r9.s64 = (int64_t)(int32_t)var_r27 + 7;
		// li r5,16
		ctx.r5.s64 = 16;
		// lwz r27,32(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// srawi r4,r9,3
		ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0x7) != 0);
		ctx.r4.s64 = ctx.r9.s32 >> 3;
		// stw r29,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r29);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260448
		SinglesNetworkClient_0448_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r27,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r27);
		// lwz r27,4(r30)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r30 + 4));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// li r5,8
		ctx.r5.s64 = 8;
		// mr r4,r27
		ctx.r4.u64 = var_r27;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x822605f0
		atSingleton_05F0_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r27,16(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 16));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// addi r8,r27,7
		ctx.r8.s64 = (int64_t)(int32_t)var_r27 + 7;
		// li r5,16
		ctx.r5.s64 = 16;
		// lwz r27,32(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// srawi r4,r8,3
		ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0x7) != 0);
		ctx.r4.s64 = ctx.r8.s32 >> 3;
		// stw r29,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r29);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260448
		SinglesNetworkClient_0448_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// stw r27,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r27);
		// lwz r27,8(r30)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r30 + 8));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// li r5,8
		ctx.r5.s64 = 8;
		// mr r4,r27
		ctx.r4.u64 = var_r27;
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x822605f0
		atSingleton_05F0_g(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// lwz r27,16(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 16));
		// bl 0x82238ae0
		SinglesNetworkClient_8AE0_g(ctx, base);
		// addi r7,r27,7
		ctx.r7.s64 = (int64_t)(int32_t)var_r27 + 7;
		// li r5,16
		ctx.r5.s64 = 16;
		// lwz r27,32(r31)
		var_r27 = (uint32_t)(PPC_LOAD_U32(var_r31 + 32));
		// srawi r4,r7,3
		ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0x7) != 0);
		ctx.r4.s64 = ctx.r7.s32 >> 3;
		// stw r29,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r29);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
		// bl 0x82260448
		SinglesNetworkClient_0448_g(ctx, base);
		// addi r28,r28,-1
		var_r28 = (uint32_t)(var_r28 + -1);
		// addi r30,r30,56
		var_r30 = (uint32_t)(var_r30 + 56);
		// stw r27,32(r31)
		PPC_STORE_U32(var_r31 + 32, var_r27);
		// cmplwi cr6,r28,0
		// bne cr6,0x823bfa2c
		if (var_r28 != 0) goto loc_823BFA2C;
	}
loc_823BFB9C:
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bfbe8
	if (ctx.r11.u32 == 0) {
		// li r3,12016
		ctx.r3.s64 = 12016;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bfbe0
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6778
			pongNetMessageHolder_6778_wrh(ctx, base);
			// b 0x823bfbe4
		} else {
		loc_823BFBE0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFBE4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFBE8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823bfc50
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,49
		ctx.r10.s64 = 49;
		// addi r11,r3,12000
		ctx.r11.s64 = ctx.r3.s64 + 12000;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823BFC30:
		// addi r11,r11,-240
		ctx.r11.s64 = ctx.r11.s64 + -240;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823bfc30
		if (ctx.r10.s32 >= 0) goto loc_823BFC30;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823BFC50:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_FC68_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_FC68_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_FC68_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bfca8
	if (ctx.r11.u32 == 0) {
		// li r3,176
		ctx.r3.s64 = 176;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bfca0
		if (ctx.r3.u32 != 0) {
			// bl 0x823c68d0
			pongNetMessageHolder_68D0_wrh(ctx, base);
			// b 0x823bfca4
		} else {
		loc_823BFCA0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFCA4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFCA8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_FCC0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_FCC0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_FCC0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bfd00
	if (ctx.r11.u32 == 0) {
		// li r3,256
		ctx.r3.s64 = 256;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bfcf8
		if (ctx.r3.u32 != 0) {
			// bl 0x823c69c8
			BallHitMessage_ctor_69C8(ctx, base);
			// b 0x823bfcfc
		} else {
		loc_823BFCF8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFCFC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFD00:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_FD18_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_FD18_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_FD18_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bfd58
	if (ctx.r11.u32 == 0) {
		// li r3,8976
		ctx.r3.s64 = 8976;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bfd50
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6b48
			pongNetMessageHolder_6B48_wrh(ctx, base);
			// b 0x823bfd54
		} else {
		loc_823BFD50:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFD54:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFD58:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_FD70_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_FD70_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_FD70_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823bfdc0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,39
		ctx.r10.s64 = 39;
		// addi r11,r3,8960
		ctx.r11.s64 = ctx.r3.s64 + 8960;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823BFDA0:
		// addi r11,r11,-224
		ctx.r11.s64 = ctx.r11.s64 + -224;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823bfda0
		if (ctx.r10.s32 >= 0) goto loc_823BFDA0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823BFDC0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_FDD8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_FDD8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_FDD8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bfec8
	if (ctx.r11.u32 == 0) {
		// li r3,80
		ctx.r3.s64 = 80;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bfec0
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r9,255
			ctx.r9.s64 = 255;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r8,4
			ctx.r8.s64 = 4;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-1292
			ctx.r11.s64 = ctx.r11.s64 + -1292;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// li r5,0
			ctx.r5.s64 = 0;
			// stfs f0,36(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 36, temp.u32);
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// stfs f0,52(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 52, temp.u32);
			// stb r9,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r9.u8);
			// li r10,0
			ctx.r10.s64 = 0;
			// stb r9,24(r3)
			PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r9.u8);
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
			// stw r11,32(r3)
			PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r11.u32);
			// stw r11,48(r3)
			PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r11.u32);
			// stb r9,40(r3)
			PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r9.u8);
			// stb r9,56(r3)
			PPC_STORE_U8(ctx.r3.u32 + 56, ctx.r9.u8);
			// sth r8,66(r3)
			PPC_STORE_U16(ctx.r3.u32 + 66, ctx.r8.u16);
			// sth r5,68(r3)
			PPC_STORE_U16(ctx.r3.u32 + 68, ctx.r5.u16);
			// sth r7,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r7.u16);
			// sth r8,64(r3)
			PPC_STORE_U16(ctx.r3.u32 + 64, ctx.r8.u16);
			// sth r6,62(r3)
			PPC_STORE_U16(ctx.r3.u32 + 62, ctx.r6.u16);
			// lhz r4,66(r3)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addic. r11,r4,-1
			ctx.xer.ca = ctx.r4.u32 > 0;
			ctx.r11.s64 = ctx.r4.s64 + -1;
			// ble 0x823bfec4
			if (ctx.r11.s32 <= 0) goto loc_823BFEC4;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823BFE8C:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,66(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823bfe8c
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823BFE8C;
			// b 0x823bfec4
		} else {
		loc_823BFEC0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFEC4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFEC8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_FEE0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_FEE0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_FEE0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bff20
	if (ctx.r11.u32 == 0) {
		// li r3,96
		ctx.r3.s64 = 96;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bff18
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6c98
			pongNetMessageHolder_6C98_wrh(ctx, base);
			// b 0x823bff1c
		} else {
		loc_823BFF18:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFF1C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFF20:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_FF38_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_FF38_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_FF38_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823bff88
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,3
		ctx.r10.s64 = 3;
		// addi r11,r3,80
		ctx.r11.s64 = ctx.r3.s64 + 80;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823BFF68:
		// addi r11,r11,-20
		ctx.r11.s64 = ctx.r11.s64 + -20;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823bff68
		if (ctx.r10.s32 >= 0) goto loc_823BFF68;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823BFF88:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_FFA0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_FFA0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_FFA0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823bffe0
	if (ctx.r11.u32 == 0) {
		// li r3,368
		ctx.r3.s64 = 368;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823bffd8
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6d68
			pongNetMessageHolder_6D68_wrh(ctx, base);
			// b 0x823bffdc
		} else {
		loc_823BFFD8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823BFFDC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823BFFE0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_FFF8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_FFF8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_FFF8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0048
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,360
		ctx.r11.s64 = ctx.r3.s64 + 360;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0028:
		// addi r11,r11,-36
		ctx.r11.s64 = ctx.r11.s64 + -36;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0028
		if (ctx.r10.s32 >= 0) goto loc_823C0028;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0048:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0060_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0060_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0060_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0148
	if (ctx.r11.u32 == 0) {
		// li r3,208
		ctx.r3.s64 = 208;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0140
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r10,-32248
			// lis r11,-32249
			// li r8,9
			ctx.r8.s64 = 9;
			// addi r7,r11,-2692
			ctx.r7.s64 = ctx.r11.s64 + -2692;
			// lfs f13,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f13.f64 = double(temp.f32);
			// addi r9,r3,4
			ctx.r9.s64 = ctx.r3.s64 + 4;
			// lfs f0,-25900(r10)
			temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C00B4:
			// addi r8,r8,-1
			ctx.r8.s64 = ctx.r8.s64 + -1;
			// stfs f0,0(r9)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
			// stw r7,-4(r9)
			PPC_STORE_U32(ctx.r9.u32 + -4, ctx.r7.u32);
			// stfs f13,8(r9)
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r9.u32 + 8, temp.u32);
			// stw r11,4(r9)
			PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r11.u32);
			// cmpwi cr6,r8,0
			// addi r9,r9,20
			ctx.r9.s64 = ctx.r9.s64 + 20;
			// bge cr6,0x823c00b4
			if (ctx.r8.s32 >= 0) goto loc_823C00B4;
			// lis r10,0
			ctx.r10.s64 = 0;
			// sth r11,204(r3)
			PPC_STORE_U16(ctx.r3.u32 + 204, ctx.r11.u16);
			// li r8,10
			ctx.r8.s64 = 10;
			// ori r9,r10,65535
			ctx.r9.u64 = ctx.r10.u64 | 65535;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r8,202(r3)
			PPC_STORE_U16(ctx.r3.u32 + 202, ctx.r8.u16);
			// sth r9,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r9.u16);
			// sth r8,200(r3)
			PPC_STORE_U16(ctx.r3.u32 + 200, ctx.r8.u16);
			// sth r9,198(r3)
			PPC_STORE_U16(ctx.r3.u32 + 198, ctx.r9.u16);
			// lhz r9,202(r3)
			ctx.r9.u64 = PPC_LOAD_U16(ctx.r3.u32 + 202);
			// addic. r8,r9,-1
			ctx.xer.ca = ctx.r9.u32 > 0;
			ctx.r8.s64 = ctx.r9.s64 + -1;
			// ble 0x823c0144
			if (ctx.r8.s32 <= 0) goto loc_823C0144;
		loc_823C0104:
			// rlwinm r9,r11,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r7,r11,1
			ctx.r7.s64 = ctx.r11.s64 + 1;
			// add r5,r11,r9
			ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// rlwinm r11,r5,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r7,16
			ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,18(r11)
			PPC_STORE_U16(ctx.r11.u32 + 18, ctx.r10.u16);
			// sth r6,36(r11)
			PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r6.u16);
			// lhz r11,202(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 202);
			// addi r4,r11,-1
			ctx.r4.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r4
			// blt cr6,0x823c0104
			if (ctx.r11.s32 < ctx.r4.s32) goto loc_823C0104;
			// b 0x823c0144
		} else {
		loc_823C0140:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0144:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0148:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0160_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0160_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0160_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0248
	if (ctx.r11.u32 == 0) {
		// li r3,288
		ctx.r3.s64 = 288;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0240
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r7,-32248
			// lis r10,-32249
			// li r9,9
			ctx.r9.s64 = 9;
			// addi r8,r10,-1204
			ctx.r8.s64 = ctx.r10.s64 + -1204;
			// lfs f0,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f0.f64 = double(temp.f32);
			// addi r11,r3,4
			ctx.r11.s64 = ctx.r3.s64 + 4;
			// lfs f13,-25900(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25900);
			ctx.f13.f64 = double(temp.f32);
			// li r10,0
			ctx.r10.s64 = 0;
		loc_823C01B4:
			// stfs f13,0(r11)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
			// stw r8,-4(r11)
			PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r8.u32);
			// addi r9,r9,-1
			ctx.r9.s64 = ctx.r9.s64 + -1;
			// stfs f0,8(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
			// stw r10,4(r11)
			PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
			// stfs f0,12(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
			// sth r10,16(r11)
			PPC_STORE_U16(ctx.r11.u32 + 16, ctx.r10.u16);
			// cmpwi cr6,r9,0
			// addi r11,r11,28
			ctx.r11.s64 = ctx.r11.s64 + 28;
			// bge cr6,0x823c01b4
			if (ctx.r9.s32 >= 0) goto loc_823C01B4;
			// lis r8,0
			ctx.r8.s64 = 0;
			// sth r10,284(r3)
			PPC_STORE_U16(ctx.r3.u32 + 284, ctx.r10.u16);
			// li r9,10
			ctx.r9.s64 = 10;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// sth r9,282(r3)
			PPC_STORE_U16(ctx.r3.u32 + 282, ctx.r9.u16);
			// sth r11,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r11.u16);
			// sth r9,280(r3)
			PPC_STORE_U16(ctx.r3.u32 + 280, ctx.r9.u16);
			// sth r11,278(r3)
			PPC_STORE_U16(ctx.r3.u32 + 278, ctx.r11.u16);
			// lhz r7,282(r3)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 282);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c0244
			if (ctx.r6.s32 <= 0) goto loc_823C0244;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C020C:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,28
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(28));
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,52(r11)
			PPC_STORE_U16(ctx.r11.u32 + 52, ctx.r4.u16);
			// sth r10,26(r11)
			PPC_STORE_U16(ctx.r11.u32 + 26, ctx.r10.u16);
			// lhz r11,282(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 282);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c020c
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C020C;
			// b 0x823c0244
		} else {
		loc_823C0240:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0244:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0248:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0260_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0260_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0260_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c02b0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,280
		ctx.r11.s64 = ctx.r3.s64 + 280;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0290:
		// addi r11,r11,-28
		ctx.r11.s64 = ctx.r11.s64 + -28;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0290
		if (ctx.r10.s32 >= 0) goto loc_823C0290;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C02B0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_02C8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_02C8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_02C8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c03bc
	if (ctx.r11.u32 == 0) {
		// li r3,256
		ctx.r3.s64 = 256;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c03b4
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r7,-32248
			// lis r11,-32249
			// li r9,9
			ctx.r9.s64 = 9;
			// addi r8,r11,-2652
			ctx.r8.s64 = ctx.r11.s64 + -2652;
			// lfs f13,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f13.f64 = double(temp.f32);
			// addi r10,r3,4
			ctx.r10.s64 = ctx.r3.s64 + 4;
			// lfs f0,-25900(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C031C:
			// stfs f0,0(r10)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
			// stw r8,-4(r10)
			PPC_STORE_U32(ctx.r10.u32 + -4, ctx.r8.u32);
			// addi r9,r9,-1
			ctx.r9.s64 = ctx.r9.s64 + -1;
			// stfs f13,8(r10)
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
			// stw r11,4(r10)
			PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
			// sth r11,12(r10)
			PPC_STORE_U16(ctx.r10.u32 + 12, ctx.r11.u16);
			// cmpwi cr6,r9,0
			// addi r10,r10,24
			ctx.r10.s64 = ctx.r10.s64 + 24;
			// bge cr6,0x823c031c
			if (ctx.r9.s32 >= 0) goto loc_823C031C;
			// lis r10,0
			ctx.r10.s64 = 0;
			// sth r11,244(r3)
			PPC_STORE_U16(ctx.r3.u32 + 244, ctx.r11.u16);
			// li r9,10
			ctx.r9.s64 = 10;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// sth r9,242(r3)
			PPC_STORE_U16(ctx.r3.u32 + 242, ctx.r9.u16);
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r9,240(r3)
			PPC_STORE_U16(ctx.r3.u32 + 240, ctx.r9.u16);
			// sth r8,20(r3)
			PPC_STORE_U16(ctx.r3.u32 + 20, ctx.r8.u16);
			// sth r7,238(r3)
			PPC_STORE_U16(ctx.r3.u32 + 238, ctx.r7.u16);
			// lhz r6,242(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 242);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c03b8
			if (ctx.r5.s32 <= 0) goto loc_823C03B8;
		loc_823C0378:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,3,0,28
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 3) & 0xFFFFFFF8;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,22(r11)
			PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
			// sth r8,44(r11)
			PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r8.u16);
			// lhz r11,242(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 242);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c0378
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C0378;
			// b 0x823c03b8
		} else {
		loc_823C03B4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C03B8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C03BC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_03D0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_03D0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_03D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c04bc
	if (ctx.r11.u32 == 0) {
		// li r3,336
		ctx.r3.s64 = 336;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c04b4
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r7,-32248
			// lis r10,-32249
			// li r9,9
			ctx.r9.s64 = 9;
			// addi r8,r10,-2612
			ctx.r8.s64 = ctx.r10.s64 + -2612;
			// lfs f0,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f0.f64 = double(temp.f32);
			// addi r11,r3,20
			ctx.r11.s64 = ctx.r3.s64 + 20;
			// lfs f13,-25900(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25900);
			ctx.f13.f64 = double(temp.f32);
			// li r10,0
			ctx.r10.s64 = 0;
		loc_823C0424:
			// stfs f13,-16(r11)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r11.u32 + -16, temp.u32);
			// stw r8,-20(r11)
			PPC_STORE_U32(ctx.r11.u32 + -20, ctx.r8.u32);
			// stfs f0,-8(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
			// stw r10,-12(r11)
			PPC_STORE_U32(ctx.r11.u32 + -12, ctx.r10.u32);
			// addi r9,r9,-1
			ctx.r9.s64 = ctx.r9.s64 + -1;
			// stfs f0,0(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
			// stw r10,-4(r11)
			PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r10.u32);
			// sth r10,4(r11)
			PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
			// cmpwi cr6,r9,0
			// addi r11,r11,32
			ctx.r11.s64 = ctx.r11.s64 + 32;
			// bge cr6,0x823c0424
			if (ctx.r9.s32 >= 0) goto loc_823C0424;
			// lis r8,0
			ctx.r8.s64 = 0;
			// sth r10,324(r3)
			PPC_STORE_U16(ctx.r3.u32 + 324, ctx.r10.u16);
			// li r9,10
			ctx.r9.s64 = 10;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// sth r9,322(r3)
			PPC_STORE_U16(ctx.r3.u32 + 322, ctx.r9.u16);
			// sth r11,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r11.u16);
			// sth r9,320(r3)
			PPC_STORE_U16(ctx.r3.u32 + 320, ctx.r9.u16);
			// sth r11,318(r3)
			PPC_STORE_U16(ctx.r3.u32 + 318, ctx.r11.u16);
			// lhz r7,322(r3)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 322);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c04b8
			if (ctx.r6.s32 <= 0) goto loc_823C04B8;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C0480:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,5,0,26
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 5) & 0xFFFFFFE0;
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,60(r11)
			PPC_STORE_U16(ctx.r11.u32 + 60, ctx.r4.u16);
			// sth r10,30(r11)
			PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r10.u16);
			// lhz r11,322(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 322);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c0480
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C0480;
			// b 0x823c04b8
		} else {
		loc_823C04B4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C04B8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C04BC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_04D0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_04D0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_04D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0520
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,320
		ctx.r11.s64 = ctx.r3.s64 + 320;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0500:
		// addi r11,r11,-32
		ctx.r11.s64 = ctx.r11.s64 + -32;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0500
		if (ctx.r10.s32 >= 0) goto loc_823C0500;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0520:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0538_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0538_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0538_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0620
	if (ctx.r11.u32 == 0) {
		// li r3,208
		ctx.r3.s64 = 208;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0618
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r10,-32248
			// lis r11,-32249
			// li r8,9
			ctx.r8.s64 = 9;
			// addi r7,r11,-2572
			ctx.r7.s64 = ctx.r11.s64 + -2572;
			// lfs f13,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f13.f64 = double(temp.f32);
			// addi r9,r3,4
			ctx.r9.s64 = ctx.r3.s64 + 4;
			// lfs f0,-25900(r10)
			temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C058C:
			// addi r8,r8,-1
			ctx.r8.s64 = ctx.r8.s64 + -1;
			// stfs f0,0(r9)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
			// stw r7,-4(r9)
			PPC_STORE_U32(ctx.r9.u32 + -4, ctx.r7.u32);
			// stfs f13,8(r9)
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r9.u32 + 8, temp.u32);
			// stw r11,4(r9)
			PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r11.u32);
			// cmpwi cr6,r8,0
			// addi r9,r9,20
			ctx.r9.s64 = ctx.r9.s64 + 20;
			// bge cr6,0x823c058c
			if (ctx.r8.s32 >= 0) goto loc_823C058C;
			// lis r10,0
			ctx.r10.s64 = 0;
			// sth r11,204(r3)
			PPC_STORE_U16(ctx.r3.u32 + 204, ctx.r11.u16);
			// li r8,10
			ctx.r8.s64 = 10;
			// ori r9,r10,65535
			ctx.r9.u64 = ctx.r10.u64 | 65535;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r8,202(r3)
			PPC_STORE_U16(ctx.r3.u32 + 202, ctx.r8.u16);
			// sth r9,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r9.u16);
			// sth r8,200(r3)
			PPC_STORE_U16(ctx.r3.u32 + 200, ctx.r8.u16);
			// sth r9,198(r3)
			PPC_STORE_U16(ctx.r3.u32 + 198, ctx.r9.u16);
			// lhz r9,202(r3)
			ctx.r9.u64 = PPC_LOAD_U16(ctx.r3.u32 + 202);
			// addic. r8,r9,-1
			ctx.xer.ca = ctx.r9.u32 > 0;
			ctx.r8.s64 = ctx.r9.s64 + -1;
			// ble 0x823c061c
			if (ctx.r8.s32 <= 0) goto loc_823C061C;
		loc_823C05DC:
			// rlwinm r9,r11,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r7,r11,1
			ctx.r7.s64 = ctx.r11.s64 + 1;
			// add r5,r11,r9
			ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// rlwinm r11,r5,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r7,16
			ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,18(r11)
			PPC_STORE_U16(ctx.r11.u32 + 18, ctx.r10.u16);
			// sth r6,36(r11)
			PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r6.u16);
			// lhz r11,202(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 202);
			// addi r4,r11,-1
			ctx.r4.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r4
			// blt cr6,0x823c05dc
			if (ctx.r11.s32 < ctx.r4.s32) goto loc_823C05DC;
			// b 0x823c061c
		} else {
		loc_823C0618:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C061C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0620:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0638_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0638_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0638_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0688
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,200
		ctx.r11.s64 = ctx.r3.s64 + 200;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0668:
		// addi r11,r11,-20
		ctx.r11.s64 = ctx.r11.s64 + -20;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0668
		if (ctx.r10.s32 >= 0) goto loc_823C0668;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0688:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_06A0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_06A0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_06A0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0794
	if (ctx.r11.u32 == 0) {
		// li r3,256
		ctx.r3.s64 = 256;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c078c
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r7,-32248
			// lis r11,-32249
			// li r9,9
			ctx.r9.s64 = 9;
			// addi r8,r11,-2532
			ctx.r8.s64 = ctx.r11.s64 + -2532;
			// lfs f13,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f13.f64 = double(temp.f32);
			// addi r10,r3,4
			ctx.r10.s64 = ctx.r3.s64 + 4;
			// lfs f0,-25900(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C06F4:
			// stfs f0,0(r10)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
			// stw r8,-4(r10)
			PPC_STORE_U32(ctx.r10.u32 + -4, ctx.r8.u32);
			// addi r9,r9,-1
			ctx.r9.s64 = ctx.r9.s64 + -1;
			// stfs f13,8(r10)
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
			// stw r11,4(r10)
			PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
			// sth r11,12(r10)
			PPC_STORE_U16(ctx.r10.u32 + 12, ctx.r11.u16);
			// cmpwi cr6,r9,0
			// addi r10,r10,24
			ctx.r10.s64 = ctx.r10.s64 + 24;
			// bge cr6,0x823c06f4
			if (ctx.r9.s32 >= 0) goto loc_823C06F4;
			// lis r10,0
			ctx.r10.s64 = 0;
			// sth r11,244(r3)
			PPC_STORE_U16(ctx.r3.u32 + 244, ctx.r11.u16);
			// li r9,10
			ctx.r9.s64 = 10;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// sth r9,242(r3)
			PPC_STORE_U16(ctx.r3.u32 + 242, ctx.r9.u16);
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r9,240(r3)
			PPC_STORE_U16(ctx.r3.u32 + 240, ctx.r9.u16);
			// sth r8,20(r3)
			PPC_STORE_U16(ctx.r3.u32 + 20, ctx.r8.u16);
			// sth r7,238(r3)
			PPC_STORE_U16(ctx.r3.u32 + 238, ctx.r7.u16);
			// lhz r6,242(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 242);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c0790
			if (ctx.r5.s32 <= 0) goto loc_823C0790;
		loc_823C0750:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,3,0,28
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 3) & 0xFFFFFFF8;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,22(r11)
			PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
			// sth r8,44(r11)
			PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r8.u16);
			// lhz r11,242(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 242);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c0750
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C0750;
			// b 0x823c0790
		} else {
		loc_823C078C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0790:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0794:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_07A8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_07A8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_07A8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c07f8
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,240
		ctx.r11.s64 = ctx.r3.s64 + 240;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C07D8:
		// addi r11,r11,-24
		ctx.r11.s64 = ctx.r11.s64 + -24;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c07d8
		if (ctx.r10.s32 >= 0) goto loc_823C07D8;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C07F8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0810_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0810_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0810_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0850
	if (ctx.r11.u32 == 0) {
		// li r3,176
		ctx.r3.s64 = 176;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0848
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6e30
			pongNetMessageHolder_6E30_wrh(ctx, base);
			// b 0x823c084c
		} else {
		loc_823C0848:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C084C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0850:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0868_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0868_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0868_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c08b8
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,160
		ctx.r11.s64 = ctx.r3.s64 + 160;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0898:
		// addi r11,r11,-16
		ctx.r11.s64 = ctx.r11.s64 + -16;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0898
		if (ctx.r10.s32 >= 0) goto loc_823C0898;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C08B8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_08D0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_08D0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_08D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0914
	if (ctx.r11.u32 == 0) {
		// lis r3,1
		ctx.r3.s64 = 65536;
		// ori r3,r3,39280
		ctx.r3.u64 = ctx.r3.u64 | 39280;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c090c
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6f30
			pongNetMessageHolder_6F30_wrh(ctx, base);
			// b 0x823c0910
		} else {
		loc_823C090C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0910:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0914:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0928_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0928_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0928_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c097c
	if (ctx.r3.u32 != 0) {
		// addis r11,r3,2
		ctx.r11.s64 = ctx.r3.s64 + 131072;
		// lis r9,-32249
		// li r10,199
		ctx.r10.s64 = 199;
		// addi r11,r11,-26272
		ctx.r11.s64 = ctx.r11.s64 + -26272;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C095C:
		// addi r11,r11,-524
		ctx.r11.s64 = ctx.r11.s64 + -524;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c095c
		if (ctx.r10.s32 >= 0) goto loc_823C095C;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C097C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0990_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0990_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0990_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c09d0
	if (ctx.r11.u32 == 0) {
		// li r3,16016
		ctx.r3.s64 = 16016;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c09c8
		if (ctx.r3.u32 != 0) {
			// bl 0x823c6ff8
			pongNetMessageHolder_6FF8_wrh(ctx, base);
			// b 0x823c09cc
		} else {
		loc_823C09C8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C09CC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C09D0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_09E8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_09E8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_09E8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0a38
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,199
		ctx.r10.s64 = 199;
		// addi r11,r3,16000
		ctx.r11.s64 = ctx.r3.s64 + 16000;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0A18:
		// addi r11,r11,-80
		ctx.r11.s64 = ctx.r11.s64 + -80;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0a18
		if (ctx.r10.s32 >= 0) goto loc_823C0A18;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0A38:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0A50_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0A50_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0A50_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0b40
	if (ctx.r11.u32 == 0) {
		// li r3,12816
		ctx.r3.s64 = 12816;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0b38
		if (ctx.r3.u32 != 0) {
			// lis r6,-32253
			// lis r7,-32248
			// lis r10,-32249
			// li r9,199
			ctx.r9.s64 = 199;
			// addi r8,r10,-2412
			ctx.r8.s64 = ctx.r10.s64 + -2412;
			// lfs f0,-12016(r6)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -12016);
			ctx.f0.f64 = double(temp.f32);
			// addi r11,r3,24
			ctx.r11.s64 = ctx.r3.s64 + 24;
			// lfs f13,-25900(r7)
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25900);
			ctx.f13.f64 = double(temp.f32);
			// li r5,255
			ctx.r5.s64 = 255;
			// li r10,0
			ctx.r10.s64 = 0;
		loc_823C0AA8:
			// stfs f13,-20(r11)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r11.u32 + -20, temp.u32);
			// stw r8,-24(r11)
			PPC_STORE_U32(ctx.r11.u32 + -24, ctx.r8.u32);
			// addi r9,r9,-1
			ctx.r9.s64 = ctx.r9.s64 + -1;
			// stfs f0,-8(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
			// stfs f0,-4(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
			// stfs f0,0(r11)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
			// stb r5,8(r11)
			PPC_STORE_U8(ctx.r11.u32 + 8, ctx.r5.u8);
			// stb r10,9(r11)
			PPC_STORE_U8(ctx.r11.u32 + 9, ctx.r10.u8);
			// cmpwi cr6,r9,0
			// addi r11,r11,64
			ctx.r11.s64 = ctx.r11.s64 + 64;
			// bge cr6,0x823c0aa8
			if (ctx.r9.s32 >= 0) goto loc_823C0AA8;
			// lis r8,0
			ctx.r8.s64 = 0;
			// sth r10,12804(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12804, ctx.r10.u16);
			// li r9,200
			ctx.r9.s64 = 200;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// sth r9,12802(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12802, ctx.r9.u16);
			// sth r11,48(r3)
			PPC_STORE_U16(ctx.r3.u32 + 48, ctx.r11.u16);
			// sth r9,12800(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12800, ctx.r9.u16);
			// sth r11,12786(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12786, ctx.r11.u16);
			// lhz r7,12802(r3)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 12802);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c0b3c
			if (ctx.r6.s32 <= 0) goto loc_823C0B3C;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C0B04:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,6,0,25
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 6) & 0xFFFFFFC0;
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,112(r11)
			PPC_STORE_U16(ctx.r11.u32 + 112, ctx.r4.u16);
			// sth r10,50(r11)
			PPC_STORE_U16(ctx.r11.u32 + 50, ctx.r10.u16);
			// lhz r11,12802(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 12802);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c0b04
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C0B04;
			// b 0x823c0b3c
		} else {
		loc_823C0B38:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0B3C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0B40:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0B58_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0B58_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0B58_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0ba8
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,199
		ctx.r10.s64 = 199;
		// addi r11,r3,12800
		ctx.r11.s64 = ctx.r3.s64 + 12800;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0B88:
		// addi r11,r11,-64
		ctx.r11.s64 = ctx.r11.s64 + -64;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0b88
		if (ctx.r10.s32 >= 0) goto loc_823C0B88;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0BA8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0BC0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0BC0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0BC0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0c00
	if (ctx.r11.u32 == 0) {
		// li r3,176
		ctx.r3.s64 = 176;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0bf8
		if (ctx.r3.u32 != 0) {
			// bl 0x823c70c8
			pongNetMessageHolder_70C8_wrh(ctx, base);
			// b 0x823c0bfc
		} else {
		loc_823C0BF8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0BFC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0C00:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0C18_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0C18_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0C18_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0c58
	if (ctx.r11.u32 == 0) {
		// li r3,112
		ctx.r3.s64 = 112;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0c50
		if (ctx.r3.u32 != 0) {
			// bl 0x823c71c0
			pongNetMessageHolder_71C0_wrh(ctx, base);
			// b 0x823c0c54
		} else {
		loc_823C0C50:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0C54:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0C58:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0C70_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0C70_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0C70_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0cc0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,3
		ctx.r10.s64 = 3;
		// addi r11,r3,96
		ctx.r11.s64 = ctx.r3.s64 + 96;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0CA0:
		// addi r11,r11,-24
		ctx.r11.s64 = ctx.r11.s64 + -24;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0ca0
		if (ctx.r10.s32 >= 0) goto loc_823C0CA0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0CC0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0CD8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0CD8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0CD8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0dc8
	if (ctx.r11.u32 == 0) {
		// li r3,80
		ctx.r3.s64 = 80;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0dc0
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r9,255
			ctx.r9.s64 = 255;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r8,4
			ctx.r8.s64 = 4;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-2284
			ctx.r11.s64 = ctx.r11.s64 + -2284;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// li r5,0
			ctx.r5.s64 = 0;
			// stfs f0,36(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 36, temp.u32);
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// stfs f0,52(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 52, temp.u32);
			// stb r9,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r9.u8);
			// li r10,0
			ctx.r10.s64 = 0;
			// stb r9,24(r3)
			PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r9.u8);
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
			// stw r11,32(r3)
			PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r11.u32);
			// stw r11,48(r3)
			PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r11.u32);
			// stb r9,40(r3)
			PPC_STORE_U8(ctx.r3.u32 + 40, ctx.r9.u8);
			// stb r9,56(r3)
			PPC_STORE_U8(ctx.r3.u32 + 56, ctx.r9.u8);
			// sth r8,66(r3)
			PPC_STORE_U16(ctx.r3.u32 + 66, ctx.r8.u16);
			// sth r5,68(r3)
			PPC_STORE_U16(ctx.r3.u32 + 68, ctx.r5.u16);
			// sth r7,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r7.u16);
			// sth r8,64(r3)
			PPC_STORE_U16(ctx.r3.u32 + 64, ctx.r8.u16);
			// sth r6,62(r3)
			PPC_STORE_U16(ctx.r3.u32 + 62, ctx.r6.u16);
			// lhz r4,66(r3)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addic. r11,r4,-1
			ctx.xer.ca = ctx.r4.u32 > 0;
			ctx.r11.s64 = ctx.r4.s64 + -1;
			// ble 0x823c0dc4
			if (ctx.r11.s32 <= 0) goto loc_823C0DC4;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C0D8C:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,66(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823c0d8c
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823C0D8C;
			// b 0x823c0dc4
		} else {
		loc_823C0DC0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0DC4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0DC8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0DE0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0DE0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0DE0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0e20
	if (ctx.r11.u32 == 0) {
		// li r3,1040
		ctx.r3.s64 = 1040;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0e18
		if (ctx.r3.u32 != 0) {
			// bl 0x823c72a8
			pongNetMessageHolder_72A8_wrh(ctx, base);
			// b 0x823c0e1c
		} else {
		loc_823C0E18:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0E1C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0E20:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_0E38_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_0E38_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_0E38_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c0e88
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,3
		ctx.r10.s64 = 3;
		// addi r11,r3,1024
		ctx.r11.s64 = ctx.r3.s64 + 1024;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C0E68:
		// addi r11,r11,-256
		ctx.r11.s64 = ctx.r11.s64 + -256;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c0e68
		if (ctx.r10.s32 >= 0) goto loc_823C0E68;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C0E88:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0EA0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0EA0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0EA0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c0f7c
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c0f74
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r7,0
			ctx.r7.s64 = 0;
			// li r10,-1
			// li r8,255
			ctx.r8.s64 = 255;
			// li r9,2
			ctx.r9.s64 = 2;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// li r6,0
			ctx.r6.s64 = 0;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-1124
			ctx.r11.s64 = ctx.r11.s64 + -1124;
			// sth r10,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r10.u16);
			// sth r10,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r10.u16);
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// stb r8,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r8.u8);
			// li r10,0
			ctx.r10.s64 = 0;
			// stb r8,24(r3)
			PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r8.u8);
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
			// ori r11,r7,65535
			ctx.r11.u64 = ctx.r7.u64 | 65535;
			// sth r9,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r9.u16);
			// sth r6,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r6.u16);
			// sth r9,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r9.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r11,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r11.u16);
			// lhz r5,34(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c0f78
			if (ctx.r4.s32 <= 0) goto loc_823C0F78;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C0F40:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823c0f40
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823C0F40;
			// b 0x823c0f78
		} else {
		loc_823C0F74:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C0F78:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C0F7C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_0F90_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_0F90_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_0F90_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1068
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1060
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r10,-1
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-1080
			ctx.r11.s64 = ctx.r11.s64 + -1080;
			// stb r10,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r10.u8);
			// sth r10,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r10.u16);
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// stb r10,24(r3)
			PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r10.u8);
			// sth r10,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r10.u16);
			// li r10,0
			ctx.r10.s64 = 0;
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// sth r9,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r9.u16);
			// sth r7,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r7.u16);
			// sth r9,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r9.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r11,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r11.u16);
			// lhz r6,34(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c1064
			if (ctx.r5.s32 <= 0) goto loc_823C1064;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C102C:
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r9,r10
			ctx.r9.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// sth r9,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r8,r11,-1
			ctx.r8.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r8
			// blt cr6,0x823c102c
			if (ctx.r11.s32 < ctx.r8.s32) goto loc_823C102C;
			// b 0x823c1064
		} else {
		loc_823C1060:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1064:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1068:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1080_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1080_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1080_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c114c
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1144
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1036
			ctx.r10.s64 = ctx.r11.s64 + -1036;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c1148
			if (ctx.r5.s32 <= 0) goto loc_823C1148;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C1108:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c1108
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C1108;
			// b 0x823c1148
		} else {
		loc_823C1144:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1148:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C114C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1160_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1160_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1160_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// lwz r11,8(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1228
	if (ctx.r11.u32 == 0) {
		// li r3,8336
		ctx.r3.s64 = 8336;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// beq cr6,0x823c1220
		if (var_r31 != 0) {
			// mr r29,r31
			var_r29 = (uint32_t)(var_r31);
			// li r30,19
			var_r30 = 19;
		loc_823C1198:
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x823b99e8
			pongNetMessageHolder_99E8_h(ctx, base);
			// addi r30,r30,-1
			var_r30 = (uint32_t)(var_r30 + -1);
			// addi r29,r29,416
			var_r29 = (uint32_t)(var_r29 + 416);
			// cmpwi cr6,r30,0
			// bge cr6,0x823c1198
			if ((int32_t)var_r30 >= 0) goto loc_823C1198;
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,20
			ctx.r9.s64 = 20;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,8322(r31)
			PPC_STORE_U16(var_r31 + 8322, ctx.r9.u16);
			// sth r11,400(r31)
			PPC_STORE_U16(var_r31 + 400, ctx.r11.u16);
			// sth r10,8324(r31)
			PPC_STORE_U16(var_r31 + 8324, ctx.r10.u16);
			// sth r9,8320(r31)
			PPC_STORE_U16(var_r31 + 8320, ctx.r9.u16);
			// sth r11,8306(r31)
			PPC_STORE_U16(var_r31 + 8306, ctx.r11.u16);
			// lhz r7,8322(r31)
			ctx.r7.u64 = PPC_LOAD_U16(var_r31 + 8322);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c1224
			if (ctx.r6.s32 <= 0) goto loc_823C1224;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C11E4:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,416
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(416));
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r31
			ctx.r11.u64 = ctx.r11.u64 + var_r31;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,816(r11)
			PPC_STORE_U16(ctx.r11.u32 + 816, ctx.r4.u16);
			// sth r10,402(r11)
			PPC_STORE_U16(ctx.r11.u32 + 402, ctx.r10.u16);
			// lhz r11,8322(r31)
			ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 8322);
			// addi r3,r11,-1
			ctx.r3.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r3
			// blt cr6,0x823c11e4
			if (ctx.r11.s32 < ctx.r3.s32) goto loc_823C11E4;
			// stw r31,8(r28)
			PPC_STORE_U32(var_r28 + 8, var_r31);
			return;
		}
	loc_823C1220:
		// li r31,0
		var_r31 = 0;
	loc_823C1224:
		// stw r31,8(r28)
		PPC_STORE_U32(var_r28 + 8, var_r31);
	}
loc_823C1228:
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_1230_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_1230_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_1230_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r24 = 0;
	uint32_t var_r25 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r27 = 0;
	uint32_t var_r26 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=160, savegprlr_24
	// mr r24,r3
	var_r24 = ctx.r3.u32;
	// lwz r25,8(r24)
	var_r25 = (uint32_t)(PPC_LOAD_U32(var_r24 + 8));
	// cmplwi cr6,r25,0
	// beq cr6,0x823c12d8
	if (var_r25 != 0) {
		// lis r11,-32249
		// li r30,19
		var_r30 = 19;
		// addi r28,r11,-15612
		var_r28 = (uint32_t)(ctx.r11.s64 + -15612);  // lbl_8206C304 @ 0x8206c304
		// lis r11,-32253
		// addi r31,r25,8320
		var_r31 = (uint32_t)(var_r25 + 8320);
		// addi r27,r11,-30076
		var_r27 = (uint32_t)(ctx.r11.s64 + -30076);  // lbl_82028A84 @ 0x82028a84
		// li r26,0
		var_r26 = 0;
	loc_823C1268:
		// addi r31,r31,-416
		var_r31 = (uint32_t)(var_r31 + -416);
		// lwz r29,376(r31)
		var_r29 = (uint32_t)(PPC_LOAD_U32(var_r31 + 376));
		// cmplwi cr6,r29,0
		// beq cr6,0x823c12ac
		if (var_r29 != 0) {
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x820f90d0
			atSingleton_Find_90D0(ctx, base);
			// clrlwi r11,r3,24
			ctx.r11.u64 = ctx.r3.u32 & 0xFF;
			// cmplwi cr6,r11,0
			// bne cr6,0x823c12ac
			if (ctx.r11.u32 != 0) goto loc_823C12AC;
			// lwz r10,0(r13)
			ctx.r10.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0)  /* SDA:0x82600000 */;
			// li r9,4
			ctx.r9.s64 = 4;
			// mr r4,r29
			ctx.r4.u64 = var_r29;
			// lwzx r3,r9,r10
			ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r10.u32);
			// lwz r7,8(r8)
			// bctrl
			VCALL(ctx.r3.u32, 2, ctx, base);  // vtable slot 2 (byte +8)
		}
	loc_823C12AC:
		// addi r3,r31,16
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 16;
		// stw r26,376(r31)
		PPC_STORE_U32(var_r31 + 376, var_r26);
		// stw r27,0(r3)
		PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ var_r27);
		// bl 0x821a9420
		atSingleton_9420(ctx, base);
		// addi r30,r30,-1
		var_r30 = (uint32_t)(var_r30 + -1);
		// stw r28,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ var_r28);
		// cmpwi cr6,r30,0
		// bge cr6,0x823c1268
		if ((int32_t)var_r30 >= 0) goto loc_823C1268;
		// mr r3,r25
		ctx.r3.u64 = var_r25;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// stw r26,8(r24)
		PPC_STORE_U32(var_r24 + 8, var_r26);
	}
loc_823C12D8:
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_12E0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_12E0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_12E0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c13b8
	if (ctx.r11.u32 == 0) {
		// li r3,336
		ctx.r3.s64 = 336;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c13b0
		if (ctx.r3.u32 != 0) {
			// lis r8,-32248
			// lis r9,-32249
			// li r10,19
			ctx.r10.s64 = 19;
			// addi r11,r3,8
			ctx.r11.s64 = ctx.r3.s64 + 8;
			// addi r9,r9,-2200
			ctx.r9.s64 = ctx.r9.s64 + -2200;
			// lfs f0,-25900(r8)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r7,255
			ctx.r7.s64 = 255;
		loc_823C132C:
			// addi r10,r10,-1
			ctx.r10.s64 = ctx.r10.s64 + -1;
			// stfs f0,-4(r11)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
			// stb r7,0(r11)
			PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r7.u8);
			// stw r9,-8(r11)
			PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r9.u32);
			// cmpwi cr6,r10,0
			// addi r11,r11,16
			ctx.r11.s64 = ctx.r11.s64 + 16;
			// bge cr6,0x823c132c
			if (ctx.r10.s32 >= 0) goto loc_823C132C;
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,20
			ctx.r9.s64 = 20;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,322(r3)
			PPC_STORE_U16(ctx.r3.u32 + 322, ctx.r9.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r10,324(r3)
			PPC_STORE_U16(ctx.r3.u32 + 324, ctx.r10.u16);
			// sth r9,320(r3)
			PPC_STORE_U16(ctx.r3.u32 + 320, ctx.r9.u16);
			// sth r11,318(r3)
			PPC_STORE_U16(ctx.r3.u32 + 318, ctx.r11.u16);
			// lhz r7,322(r3)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 322);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c13b4
			if (ctx.r6.s32 <= 0) goto loc_823C13B4;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C137C:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r4.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,322(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 322);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c137c
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C137C;
			// b 0x823c13b4
		} else {
		loc_823C13B0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C13B4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C13B8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_13D0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_13D0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_13D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c1420
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,19
		ctx.r10.s64 = 19;
		// addi r11,r3,320
		ctx.r11.s64 = ctx.r3.s64 + 320;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C1400:
		// addi r11,r11,-16
		ctx.r11.s64 = ctx.r11.s64 + -16;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c1400
		if (ctx.r10.s32 >= 0) goto loc_823C1400;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1420:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1438_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1438_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1438_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1524
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c151c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// li r10,-1
			// li r8,2
			ctx.r8.s64 = 2;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// sth r10,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r10.u16);
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r9,r11,-2160
			ctx.r9.s64 = ctx.r11.s64 + -2160;
			// sth r10,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r10.u16);
			// lis r11,-32253
			// stfs f0,24(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 24, temp.u32);
			// stw r9,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r9.u32);
			// stw r9,20(r3)
			PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r9.u32);
			// lis r9,0
			ctx.r9.s64 = 0;
			// lfs f13,-12016(r11)
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -12016);
			ctx.f13.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
			// ori r9,r9,65535
			ctx.r9.u64 = ctx.r9.u64 | 65535;
			// stfs f13,8(r3)
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r3.u32 + 8, temp.u32);
			// stfs f13,28(r3)
			temp.f32 = float(ctx.f13.f64);
			PPC_STORE_U32(ctx.r3.u32 + 28, temp.u32);
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r11,14(r3)
			PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r11.u16);
			// sth r11,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r11.u16);
			// sth r8,40(r3)
			PPC_STORE_U16(ctx.r3.u32 + 40, ctx.r8.u16);
			// sth r8,42(r3)
			PPC_STORE_U16(ctx.r3.u32 + 42, ctx.r8.u16);
			// sth r11,44(r3)
			PPC_STORE_U16(ctx.r3.u32 + 44, ctx.r11.u16);
			// sth r9,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r9.u16);
			// sth r9,38(r3)
			PPC_STORE_U16(ctx.r3.u32 + 38, ctx.r9.u16);
			// lhz r8,42(r3)
			ctx.r8.u64 = PPC_LOAD_U16(ctx.r3.u32 + 42);
			// addic. r7,r8,-1
			ctx.xer.ca = ctx.r8.u32 > 0;
			ctx.r7.s64 = ctx.r8.s64 + -1;
			// ble 0x823c1520
			if (ctx.r7.s32 <= 0) goto loc_823C1520;
		loc_823C14E0:
			// rlwinm r9,r11,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r6,r11,1
			ctx.r6.s64 = ctx.r11.s64 + 1;
			// add r4,r11,r9
			ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r5,r10
			ctx.r5.u64 = ctx.r10.u64;
			// rlwinm r11,r4,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r6,16
			ctx.r10.u64 = ctx.r6.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,18(r11)
			PPC_STORE_U16(ctx.r11.u32 + 18, ctx.r10.u16);
			// sth r5,36(r11)
			PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r5.u16);
			// lhz r11,42(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 42);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c14e0
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C14E0;
			// b 0x823c1520
		} else {
		loc_823C151C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1520:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1524:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1538_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1538_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1538_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1610
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1608
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r7,0
			ctx.r7.s64 = 0;
			// li r8,-1
			// li r9,2
			ctx.r9.s64 = 2;
			// li r6,0
			ctx.r6.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-2120
			ctx.r10.s64 = ctx.r11.s64 + -2120;
			// stfs f0,24(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 24, temp.u32);
			// ori r11,r7,65535
			ctx.r11.u64 = ctx.r7.u64 | 65535;
			// sth r8,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r8.u16);
			// sth r8,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r8.u16);
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,20(r3)
			PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,42(r3)
			PPC_STORE_U16(ctx.r3.u32 + 42, ctx.r9.u16);
			// sth r6,44(r3)
			PPC_STORE_U16(ctx.r3.u32 + 44, ctx.r6.u16);
			// sth r11,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r11.u16);
			// sth r9,40(r3)
			PPC_STORE_U16(ctx.r3.u32 + 40, ctx.r9.u16);
			// sth r11,38(r3)
			PPC_STORE_U16(ctx.r3.u32 + 38, ctx.r11.u16);
			// lhz r5,42(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 42);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c160c
			if (ctx.r4.s32 <= 0) goto loc_823C160C;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C15CC:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// rlwinm r9,r11,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,18(r11)
			PPC_STORE_U16(ctx.r11.u32 + 18, ctx.r10.u16);
			// sth r8,36(r11)
			PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r8.u16);
			// lhz r11,42(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 42);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c15cc
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C15CC;
			// b 0x823c160c
		} else {
		loc_823C1608:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C160C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1610:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_1628_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_1628_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_1628_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c1678
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,40
		ctx.r11.s64 = ctx.r3.s64 + 40;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C1658:
		// addi r11,r11,-20
		ctx.r11.s64 = ctx.r11.s64 + -20;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c1658
		if (ctx.r10.s32 >= 0) goto loc_823C1658;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1678:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1690_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1690_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1690_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1758
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1750
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r7,2
			ctx.r7.s64 = 2;
			// ori r9,r10,65535
			ctx.r9.u64 = ctx.r10.u64 | 65535;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r8,r11,-2080
			ctx.r8.s64 = ctx.r11.s64 + -2080;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// li r11,0
			ctx.r11.s64 = 0;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// stw r8,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r8.u32);
			// stw r8,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r8.u32);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r11,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r11.u16);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r7,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r7.u16);
			// sth r11,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r11.u16);
			// sth r7,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r7.u16);
			// sth r9,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r9.u16);
			// lhz r9,34(r3)
			ctx.r9.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r8,r9,-1
			ctx.xer.ca = ctx.r9.u32 > 0;
			ctx.r8.s64 = ctx.r9.s64 + -1;
			// ble 0x823c1754
			if (ctx.r8.s32 <= 0) goto loc_823C1754;
		loc_823C171C:
			// addi r7,r11,1
			ctx.r7.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r7,16
			ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
			// sth r6,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r6.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r5,r11,-1
			ctx.r5.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r5
			// blt cr6,0x823c171c
			if (ctx.r11.s32 < ctx.r5.s32) goto loc_823C171C;
			// b 0x823c1754
		} else {
		loc_823C1750:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1754:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1758:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_1770_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_1770_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_1770_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c17c0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,32
		ctx.r11.s64 = ctx.r3.s64 + 32;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C17A0:
		// addi r11,r11,-16
		ctx.r11.s64 = ctx.r11.s64 + -16;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c17a0
		if (ctx.r10.s32 >= 0) goto loc_823C17A0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C17C0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_17D8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_17D8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_17D8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c18bc
	if (ctx.r11.u32 == 0) {
		// li r3,1056
		ctx.r3.s64 = 1056;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c18b4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32249
			// lis r10,0
			ctx.r10.s64 = 0;
			// addi r9,r11,-2040
			ctx.r9.s64 = ctx.r11.s64 + -2040;
			// lis r11,-32248
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r7,-1
			// li r8,2
			ctx.r8.s64 = 2;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// stw r9,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r9.u32);
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
			// mr r5,r10
			ctx.r5.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// sth r7,516(r3)
			PPC_STORE_U16(ctx.r3.u32 + 516, ctx.r7.u16);
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r11,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
			// stfs f0,528(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 528, temp.u32);
			// stw r9,524(r3)
			PPC_STORE_U32(ctx.r3.u32 + 524, ctx.r9.u32);
			// sth r11,532(r3)
			PPC_STORE_U16(ctx.r3.u32 + 532, ctx.r11.u16);
			// sth r11,534(r3)
			PPC_STORE_U16(ctx.r3.u32 + 534, ctx.r11.u16);
			// sth r7,1040(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1040, ctx.r7.u16);
			// sth r8,1050(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1050, ctx.r8.u16);
			// sth r8,1048(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1048, ctx.r8.u16);
			// sth r11,1052(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1052, ctx.r11.u16);
			// sth r6,520(r3)
			PPC_STORE_U16(ctx.r3.u32 + 520, ctx.r6.u16);
			// sth r5,1046(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1046, ctx.r5.u16);
			// lhz r4,1050(r3)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r3.u32 + 1050);
			// addic. r9,r4,-1
			ctx.xer.ca = ctx.r4.u32 > 0;
			ctx.r9.s64 = ctx.r4.s64 + -1;
			// ble 0x823c18b8
			if (ctx.r9.s32 <= 0) goto loc_823C18B8;
		loc_823C1880:
			// addi r8,r11,1
			ctx.r8.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,524
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(524));
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r8,16
			ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
			// sth r7,1044(r11)
			PPC_STORE_U16(ctx.r11.u32 + 1044, ctx.r7.u16);
			// sth r10,522(r11)
			PPC_STORE_U16(ctx.r11.u32 + 522, ctx.r10.u16);
			// lhz r11,1050(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 1050);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c1880
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C1880;
			// b 0x823c18b8
		} else {
		loc_823C18B4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C18B8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C18BC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_18D0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_18D0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_18D0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c1920
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,1048
		ctx.r11.s64 = ctx.r3.s64 + 1048;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C1900:
		// addi r11,r11,-524
		ctx.r11.s64 = ctx.r11.s64 + -524;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c1900
		if (ctx.r10.s32 >= 0) goto loc_823C1900;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1920:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1938_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1938_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1938_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1a00
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c19f8
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r7,2
			ctx.r7.s64 = 2;
			// ori r9,r10,65535
			ctx.r9.u64 = ctx.r10.u64 | 65535;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r8,r11,-996
			ctx.r8.s64 = ctx.r11.s64 + -996;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// li r11,0
			ctx.r11.s64 = 0;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// stw r8,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r8.u32);
			// stw r8,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r8.u32);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r11,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r11.u16);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r7,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r7.u16);
			// sth r11,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r11.u16);
			// sth r7,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r7.u16);
			// sth r9,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r9.u16);
			// lhz r9,34(r3)
			ctx.r9.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r8,r9,-1
			ctx.xer.ca = ctx.r9.u32 > 0;
			ctx.r8.s64 = ctx.r9.s64 + -1;
			// ble 0x823c19fc
			if (ctx.r8.s32 <= 0) goto loc_823C19FC;
		loc_823C19C4:
			// addi r7,r11,1
			ctx.r7.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r7,16
			ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
			// sth r6,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r6.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r5,r11,-1
			ctx.r5.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r5
			// blt cr6,0x823c19c4
			if (ctx.r11.s32 < ctx.r5.s32) goto loc_823C19C4;
			// b 0x823c19fc
		} else {
		loc_823C19F8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C19FC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1A00:
	// blr
	return;
}

__attribute__((alias("__imp__rage_1A18"))) PPC_WEAK_FUNC(rage_1A18);
PPC_FUNC_IMPL(__imp__rage_1A18) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,744
	ctx.r11.s64 = ctx.r11.s64 + 744;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c1a60
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,12
		ctx.r10.s64 = ctx.r3.s64 + 12;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-12(r10)
		PPC_STORE_U32(ctx.r10.u32 + -12, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1A60:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1A90_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1A90_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1A90_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1b54
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1b4c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,1
			ctx.r9.s64 = 1;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-2000
			ctx.r10.s64 = ctx.r11.s64 + -2000;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,14(r3)
			PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
			// sth r7,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r11,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
			// lhz r6,14(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c1b50
			if (ctx.r5.s32 <= 0) goto loc_823C1B50;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C1B10:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,14(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c1b10
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C1B10;
			// b 0x823c1b50
		} else {
		loc_823C1B4C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1B50:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1B54:
	// blr
	return;
}

__attribute__((alias("__imp__rage_1B68"))) PPC_WEAK_FUNC(rage_1B68);
PPC_FUNC_IMPL(__imp__rage_1B68) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,764
	ctx.r11.s64 = ctx.r11.s64 + 764;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c1bb0
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,12
		ctx.r10.s64 = ctx.r3.s64 + 12;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-12(r10)
		PPC_STORE_U32(ctx.r10.u32 + -12, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1BB0:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1BE0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1BE0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1BE0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1ca4
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1c9c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,1
			ctx.r9.s64 = 1;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1960
			ctx.r10.s64 = ctx.r11.s64 + -1960;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,14(r3)
			PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
			// sth r7,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r11,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
			// lhz r6,14(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c1ca0
			if (ctx.r5.s32 <= 0) goto loc_823C1CA0;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C1C60:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,14(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c1c60
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C1C60;
			// b 0x823c1ca0
		} else {
		loc_823C1C9C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1CA0:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1CA4:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1CB8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1CB8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1CB8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1d7c
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1d74
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-956
			ctx.r10.s64 = ctx.r11.s64 + -956;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r9.u16);
			// sth r7,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r7.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r9,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r9.u16);
			// sth r11,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r11.u16);
			// lhz r6,34(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c1d78
			if (ctx.r5.s32 <= 0) goto loc_823C1D78;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C1D40:
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r9,r10
			ctx.r9.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// sth r9,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r8,r11,-1
			ctx.r8.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r8
			// blt cr6,0x823c1d40
			if (ctx.r11.s32 < ctx.r8.s32) goto loc_823C1D40;
			// b 0x823c1d78
		} else {
		loc_823C1D74:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1D78:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1D7C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1D90_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1D90_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1D90_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1e68
	if (ctx.r11.u32 == 0) {
		// li r3,496
		ctx.r3.s64 = 496;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1e60
		if (ctx.r3.u32 != 0) {
			// lis r8,-32248
			// lis r9,-32249
			// li r10,29
			ctx.r10.s64 = 29;
			// addi r11,r3,8
			ctx.r11.s64 = ctx.r3.s64 + 8;
			// addi r9,r9,-916
			ctx.r9.s64 = ctx.r9.s64 + -916;
			// lfs f0,-25900(r8)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r7,-1
		loc_823C1DDC:
			// addi r10,r10,-1
			ctx.r10.s64 = ctx.r10.s64 + -1;
			// stfs f0,-4(r11)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
			// stw r7,0(r11)
			PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r7.u32);
			// stw r9,-8(r11)
			PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r9.u32);
			// cmpwi cr6,r10,0
			// addi r11,r11,16
			ctx.r11.s64 = ctx.r11.s64 + 16;
			// bge cr6,0x823c1ddc
			if (ctx.r10.s32 >= 0) goto loc_823C1DDC;
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,30
			ctx.r9.s64 = 30;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,482(r3)
			PPC_STORE_U16(ctx.r3.u32 + 482, ctx.r9.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r10,484(r3)
			PPC_STORE_U16(ctx.r3.u32 + 484, ctx.r10.u16);
			// sth r9,480(r3)
			PPC_STORE_U16(ctx.r3.u32 + 480, ctx.r9.u16);
			// sth r11,478(r3)
			PPC_STORE_U16(ctx.r3.u32 + 478, ctx.r11.u16);
			// lhz r7,482(r3)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 482);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c1e64
			if (ctx.r6.s32 <= 0) goto loc_823C1E64;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C1E2C:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r4.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,482(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 482);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c1e2c
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C1E2C;
			// b 0x823c1e64
		} else {
		loc_823C1E60:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1E64:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1E68:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_1E80_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_1E80_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_1E80_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c1ed0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,29
		ctx.r10.s64 = 29;
		// addi r11,r3,480
		ctx.r11.s64 = ctx.r3.s64 + 480;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C1EB0:
		// addi r11,r11,-16
		ctx.r11.s64 = ctx.r11.s64 + -16;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c1eb0
		if (ctx.r10.s32 >= 0) goto loc_823C1EB0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1ED0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1EE8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1EE8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1EE8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1f28
	if (ctx.r11.u32 == 0) {
		// li r3,608
		ctx.r3.s64 = 608;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1f20
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7410
			pongNetMessageHolder_7410_wrh(ctx, base);
			// b 0x823c1f24
		} else {
		loc_823C1F20:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1F24:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1F28:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_1F40_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_1F40_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_1F40_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c1f90
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,592
		ctx.r11.s64 = ctx.r3.s64 + 592;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C1F70:
		// addi r11,r11,-296
		ctx.r11.s64 = ctx.r11.s64 + -296;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c1f70
		if (ctx.r10.s32 >= 0) goto loc_823C1F70;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C1F90:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_1FA8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_1FA8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_1FA8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c1fe8
	if (ctx.r11.u32 == 0) {
		// li r3,176
		ctx.r3.s64 = 176;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c1fe0
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7530
			pongNetMessageHolder_7530_wrh(ctx, base);
			// b 0x823c1fe4
		} else {
		loc_823C1FE0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C1FE4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C1FE8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_2000_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_2000_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_2000_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c2050
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,160
		ctx.r11.s64 = ctx.r3.s64 + 160;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C2030:
		// addi r11,r11,-80
		ctx.r11.s64 = ctx.r11.s64 + -80;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c2030
		if (ctx.r10.s32 >= 0) goto loc_823C2030;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C2050:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2068_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2068_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2068_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2134
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c212c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-876
			ctx.r10.s64 = ctx.r11.s64 + -876;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c2130
			if (ctx.r5.s32 <= 0) goto loc_823C2130;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C20F0:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c20f0
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C20F0;
			// b 0x823c2130
		} else {
		loc_823C212C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2130:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2134:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2148_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2148_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2148_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2214
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c220c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-836
			ctx.r10.s64 = ctx.r11.s64 + -836;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c2210
			if (ctx.r5.s32 <= 0) goto loc_823C2210;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C21D0:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c21d0
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C21D0;
			// b 0x823c2210
		} else {
		loc_823C220C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2210:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2214:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2228_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2228_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2228_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2268
	if (ctx.r11.u32 == 0) {
		// li r3,224
		ctx.r3.s64 = 224;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c2260
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7618
			pongNetMessageHolder_7618_wrh(ctx, base);
			// b 0x823c2264
		} else {
		loc_823C2260:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2264:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2268:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_2280_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_2280_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_2280_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c22d0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,216
		ctx.r11.s64 = ctx.r3.s64 + 216;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C22B0:
		// addi r11,r11,-108
		ctx.r11.s64 = ctx.r11.s64 + -108;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c22b0
		if (ctx.r10.s32 >= 0) goto loc_823C22B0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C22D0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_22E8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_22E8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_22E8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c23b4
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c23ac
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-756
			ctx.r10.s64 = ctx.r11.s64 + -756;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c23b0
			if (ctx.r5.s32 <= 0) goto loc_823C23B0;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2370:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c2370
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C2370;
			// b 0x823c23b0
		} else {
		loc_823C23AC:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C23B0:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C23B4:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_23C8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_23C8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_23C8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c24a4
	if (ctx.r11.u32 == 0) {
		// li r3,80
		ctx.r3.s64 = 80;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c249c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r9,4
			ctx.r9.s64 = 4;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r6,0
			ctx.r6.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-1840
			ctx.r11.s64 = ctx.r11.s64 + -1840;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// stfs f0,36(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 36, temp.u32);
			// stfs f0,52(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 52, temp.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
			// stw r11,32(r3)
			PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r11.u32);
			// stw r11,48(r3)
			PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r11.u32);
			// sth r9,66(r3)
			PPC_STORE_U16(ctx.r3.u32 + 66, ctx.r9.u16);
			// sth r6,68(r3)
			PPC_STORE_U16(ctx.r3.u32 + 68, ctx.r6.u16);
			// sth r8,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r8.u16);
			// sth r9,64(r3)
			PPC_STORE_U16(ctx.r3.u32 + 64, ctx.r9.u16);
			// sth r7,62(r3)
			PPC_STORE_U16(ctx.r3.u32 + 62, ctx.r7.u16);
			// lhz r5,66(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c24a0
			if (ctx.r4.s32 <= 0) goto loc_823C24A0;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2468:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,66(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823c2468
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823C2468;
			// b 0x823c24a0
		} else {
		loc_823C249C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C24A0:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C24A4:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_24B8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_24B8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_24B8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c2508
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,3
		ctx.r10.s64 = 3;
		// addi r11,r3,64
		ctx.r11.s64 = ctx.r3.s64 + 64;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C24E8:
		// addi r11,r11,-16
		ctx.r11.s64 = ctx.r11.s64 + -16;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c24e8
		if (ctx.r10.s32 >= 0) goto loc_823C24E8;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C2508:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2520_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2520_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2520_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c25ec
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c25e4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-716
			ctx.r10.s64 = ctx.r11.s64 + -716;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c25e8
			if (ctx.r5.s32 <= 0) goto loc_823C25E8;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C25A8:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c25a8
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C25A8;
			// b 0x823c25e8
		} else {
		loc_823C25E4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C25E8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C25EC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2600_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2600_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2600_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c26cc
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c26c4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1800
			ctx.r10.s64 = ctx.r11.s64 + -1800;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c26c8
			if (ctx.r5.s32 <= 0) goto loc_823C26C8;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2688:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c2688
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C2688;
			// b 0x823c26c8
		} else {
		loc_823C26C4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C26C8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C26CC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_26E0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_26E0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_26E0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c27ac
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c27a4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1760
			ctx.r10.s64 = ctx.r11.s64 + -1760;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c27a8
			if (ctx.r5.s32 <= 0) goto loc_823C27A8;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2768:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c2768
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C2768;
			// b 0x823c27a8
		} else {
		loc_823C27A4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C27A8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C27AC:
	// blr
	return;
}

__attribute__((alias("__imp__rage_27C0"))) PPC_WEAK_FUNC(rage_27C0);
PPC_FUNC_IMPL(__imp__rage_27C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,1024
	ctx.r11.s64 = ctx.r11.s64 + 1024;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c2808
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,24
		ctx.r10.s64 = ctx.r3.s64 + 24;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-24(r10)
		PPC_STORE_U32(ctx.r10.u32 + -24, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C2808:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2838_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2838_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2838_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c28fc
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c28f4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,1
			ctx.r9.s64 = 1;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1720
			ctx.r10.s64 = ctx.r11.s64 + -1720;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,20(r3)
			PPC_STORE_U16(ctx.r3.u32 + 20, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c28f8
			if (ctx.r5.s32 <= 0) goto loc_823C28F8;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C28B8:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,3,0,28
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 3) & 0xFFFFFFF8;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,22(r11)
			PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
			// sth r8,44(r11)
			PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c28b8
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C28B8;
			// b 0x823c28f8
		} else {
		loc_823C28F4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C28F8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C28FC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_2910_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_2910_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_2910_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c294c
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,24
		ctx.r10.s64 = ctx.r3.s64 + 24;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-24(r10)
		PPC_STORE_U32(ctx.r10.u32 + -24, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C294C:
	// blr
	return;
}

__attribute__((alias("__imp__rage_2960"))) PPC_WEAK_FUNC(rage_2960);
PPC_FUNC_IMPL(__imp__rage_2960) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,1044
	ctx.r11.s64 = ctx.r11.s64 + 1044;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c29a8
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,16
		ctx.r10.s64 = ctx.r3.s64 + 16;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-16(r10)
		PPC_STORE_U32(ctx.r10.u32 + -16, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C29A8:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_29D8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_29D8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_29D8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2a9c
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c2a94
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r7,0
			ctx.r7.s64 = 0;
			// li r8,-1
			// li r9,1
			ctx.r9.s64 = 1;
			// li r6,0
			ctx.r6.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-676
			ctx.r10.s64 = ctx.r11.s64 + -676;
			// stb r8,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r8.u8);
			// ori r11,r7,65535
			ctx.r11.u64 = ctx.r7.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,18(r3)
			PPC_STORE_U16(ctx.r3.u32 + 18, ctx.r9.u16);
			// sth r6,20(r3)
			PPC_STORE_U16(ctx.r3.u32 + 20, ctx.r6.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r9,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r9.u16);
			// sth r11,14(r3)
			PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r11.u16);
			// lhz r5,18(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 18);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c2a98
			if (ctx.r4.s32 <= 0) goto loc_823C2A98;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2A60:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,18(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 18);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823c2a60
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823C2A60;
			// b 0x823c2a98
		} else {
		loc_823C2A94:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2A98:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2A9C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_2AB0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_2AB0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_2AB0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c2aec
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,16
		ctx.r10.s64 = ctx.r3.s64 + 16;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-16(r10)
		PPC_STORE_U32(ctx.r10.u32 + -16, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C2AEC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2B00_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2B00_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2B00_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2bd0
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c2bc8
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r7,0
			ctx.r7.s64 = 0;
			// li r8,-1
			// li r9,2
			ctx.r9.s64 = 2;
			// li r6,0
			ctx.r6.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-636
			ctx.r10.s64 = ctx.r11.s64 + -636;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// ori r11,r7,65535
			ctx.r11.u64 = ctx.r7.u64 | 65535;
			// stb r8,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r8.u8);
			// stb r8,24(r3)
			PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r8.u8);
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r9.u16);
			// sth r6,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r6.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r9,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r9.u16);
			// sth r11,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r11.u16);
			// lhz r5,34(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c2bcc
			if (ctx.r4.s32 <= 0) goto loc_823C2BCC;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2B94:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823c2b94
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823C2B94;
			// b 0x823c2bcc
		} else {
		loc_823C2BC8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2BCC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2BD0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2BE8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2BE8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2BE8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2cb4
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c2cac
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1640
			ctx.r10.s64 = ctx.r11.s64 + -1640;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c2cb0
			if (ctx.r5.s32 <= 0) goto loc_823C2CB0;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C2C70:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c2c70
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C2C70;
			// b 0x823c2cb0
		} else {
		loc_823C2CAC:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2CB0:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2CB4:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2CC8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2CC8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2CC8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2d08
	if (ctx.r11.u32 == 0) {
		// li r3,1296
		ctx.r3.s64 = 1296;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c2d00
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7700
			pongNetMessageHolder_7700_wrh(ctx, base);
			// b 0x823c2d04
		} else {
		loc_823C2D00:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2D04:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2D08:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_2D20_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_2D20_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_2D20_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c2d70
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,1280
		ctx.r11.s64 = ctx.r3.s64 + 1280;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C2D50:
		// addi r11,r11,-640
		ctx.r11.s64 = ctx.r11.s64 + -640;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c2d50
		if (ctx.r10.s32 >= 0) goto loc_823C2D50;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C2D70:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2D88_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2D88_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2D88_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r28 = 0;
	uint32_t var_r31 = 0;
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=128, savegprlr_28
	// mr r28,r3
	var_r28 = ctx.r3.u32;
	// lwz r11,8(r28)
	ctx.r11.u64 = PPC_LOAD_U32(var_r28 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2e50
	if (ctx.r11.u32 == 0) {
		// li r3,512
		ctx.r3.s64 = 512;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// beq cr6,0x823c2e48
		if (var_r31 != 0) {
			// mr r29,r31
			var_r29 = (uint32_t)(var_r31);
			// li r30,4
			var_r30 = 4;
		loc_823C2DC0:
			// mr r3,r29
			ctx.r3.u64 = var_r29;
			// bl 0x823c7f80
			ref_io_7F80(ctx, base);
			// addi r30,r30,-1
			var_r30 = (uint32_t)(var_r30 + -1);
			// addi r29,r29,100
			var_r29 = (uint32_t)(var_r29 + 100);
			// cmpwi cr6,r30,0
			// bge cr6,0x823c2dc0
			if ((int32_t)var_r30 >= 0) goto loc_823C2DC0;
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,5
			ctx.r9.s64 = 5;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,502(r31)
			PPC_STORE_U16(var_r31 + 502, ctx.r9.u16);
			// sth r11,96(r31)
			PPC_STORE_U16(var_r31 + 96, ctx.r11.u16);
			// sth r10,504(r31)
			PPC_STORE_U16(var_r31 + 504, ctx.r10.u16);
			// sth r9,500(r31)
			PPC_STORE_U16(var_r31 + 500, ctx.r9.u16);
			// sth r11,498(r31)
			PPC_STORE_U16(var_r31 + 498, ctx.r11.u16);
			// lhz r7,502(r31)
			ctx.r7.u64 = PPC_LOAD_U16(var_r31 + 502);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c2e4c
			if (ctx.r6.s32 <= 0) goto loc_823C2E4C;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C2E0C:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,100
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(100));
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r31
			ctx.r11.u64 = ctx.r11.u64 + var_r31;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,196(r11)
			PPC_STORE_U16(ctx.r11.u32 + 196, ctx.r4.u16);
			// sth r10,98(r11)
			PPC_STORE_U16(ctx.r11.u32 + 98, ctx.r10.u16);
			// lhz r11,502(r31)
			ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 502);
			// addi r3,r11,-1
			ctx.r3.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r3
			// blt cr6,0x823c2e0c
			if (ctx.r11.s32 < ctx.r3.s32) goto loc_823C2E0C;
			// stw r31,8(r28)
			PPC_STORE_U32(var_r28 + 8, var_r31);
			return;
		}
	loc_823C2E48:
		// li r31,0
		var_r31 = 0;
	loc_823C2E4C:
		// stw r31,8(r28)
		PPC_STORE_U32(var_r28 + 8, var_r31);
	}
loc_823C2E50:
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_2E58_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_2E58_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_2E58_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c2ea8
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,4
		ctx.r10.s64 = 4;
		// addi r11,r3,500
		ctx.r11.s64 = ctx.r3.s64 + 500;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C2E88:
		// addi r11,r11,-100
		ctx.r11.s64 = ctx.r11.s64 + -100;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c2e88
		if (ctx.r10.s32 >= 0) goto loc_823C2E88;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C2EA8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2EC0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2EC0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2EC0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c2f88
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c2f80
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r7,2
			ctx.r7.s64 = 2;
			// ori r9,r10,65535
			ctx.r9.u64 = ctx.r10.u64 | 65535;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r8,r11,-596
			ctx.r8.s64 = ctx.r11.s64 + -596;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// li r11,0
			ctx.r11.s64 = 0;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// stw r8,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r8.u32);
			// stw r8,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r8.u32);
			// stb r11,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r11.u8);
			// stb r11,24(r3)
			PPC_STORE_U8(ctx.r3.u32 + 24, ctx.r11.u8);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r7,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r7.u16);
			// sth r11,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r11.u16);
			// sth r7,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r7.u16);
			// sth r9,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r9.u16);
			// lhz r9,34(r3)
			ctx.r9.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r8,r9,-1
			ctx.xer.ca = ctx.r9.u32 > 0;
			ctx.r8.s64 = ctx.r9.s64 + -1;
			// ble 0x823c2f84
			if (ctx.r8.s32 <= 0) goto loc_823C2F84;
		loc_823C2F4C:
			// addi r7,r11,1
			ctx.r7.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r7,16
			ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
			// sth r6,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r6.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r5,r11,-1
			ctx.r5.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r5
			// blt cr6,0x823c2f4c
			if (ctx.r11.s32 < ctx.r5.s32) goto loc_823C2F4C;
			// b 0x823c2f84
		} else {
		loc_823C2F80:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C2F84:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C2F88:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_2FA0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_2FA0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_2FA0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3090
	if (ctx.r11.u32 == 0) {
		// li r3,96
		ctx.r3.s64 = 96;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3088
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r9,0
			ctx.r9.s64 = 0;
			// li r8,0
			ctx.r8.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-1560
			ctx.r10.s64 = ctx.r11.s64 + -1560;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// ori r11,r9,65535
			ctx.r11.u64 = ctx.r9.u64 | 65535;
			// stfs f0,36(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 36, temp.u32);
			// li r9,5
			ctx.r9.s64 = 5;
			// stfs f0,52(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 52, temp.u32);
			// stfs f0,68(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 68, temp.u32);
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r10.u32);
			// stw r10,32(r3)
			PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r10.u32);
			// stw r10,48(r3)
			PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r10.u32);
			// stw r10,64(r3)
			PPC_STORE_U32(ctx.r3.u32 + 64, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r11,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r11.u16);
			// sth r11,40(r3)
			PPC_STORE_U16(ctx.r3.u32 + 40, ctx.r11.u16);
			// sth r11,56(r3)
			PPC_STORE_U16(ctx.r3.u32 + 56, ctx.r11.u16);
			// sth r11,72(r3)
			PPC_STORE_U16(ctx.r3.u32 + 72, ctx.r11.u16);
			// sth r9,82(r3)
			PPC_STORE_U16(ctx.r3.u32 + 82, ctx.r9.u16);
			// sth r8,84(r3)
			PPC_STORE_U16(ctx.r3.u32 + 84, ctx.r8.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r9,80(r3)
			PPC_STORE_U16(ctx.r3.u32 + 80, ctx.r9.u16);
			// sth r11,78(r3)
			PPC_STORE_U16(ctx.r3.u32 + 78, ctx.r11.u16);
			// lhz r7,82(r3)
			ctx.r7.u64 = PPC_LOAD_U16(ctx.r3.u32 + 82);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c308c
			if (ctx.r6.s32 <= 0) goto loc_823C308C;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C3054:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r4.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,82(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 82);
			// addi r9,r11,-1
			ctx.r9.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r9
			// blt cr6,0x823c3054
			if (ctx.r11.s32 < ctx.r9.s32) goto loc_823C3054;
			// b 0x823c308c
		} else {
		loc_823C3088:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C308C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3090:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_30A8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_30A8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_30A8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c30f8
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,4
		ctx.r10.s64 = 4;
		// addi r11,r3,80
		ctx.r11.s64 = ctx.r3.s64 + 80;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C30D8:
		// addi r11,r11,-16
		ctx.r11.s64 = ctx.r11.s64 + -16;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c30d8
		if (ctx.r10.s32 >= 0) goto loc_823C30D8;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C30F8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3110_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3110_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3110_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3150
	if (ctx.r11.u32 == 0) {
		// li r3,96
		ctx.r3.s64 = 96;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3148
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7828
			pongNetMessageHolder_7828_wrh(ctx, base);
			// b 0x823c314c
		} else {
		loc_823C3148:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C314C:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3150:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3168_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3168_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3168_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3234
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c322c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-516
			ctx.r10.s64 = ctx.r11.s64 + -516;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c3230
			if (ctx.r5.s32 <= 0) goto loc_823C3230;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C31F0:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c31f0
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C31F0;
			// b 0x823c3230
		} else {
		loc_823C322C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3230:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3234:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3248_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3248_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3248_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3288
	if (ctx.r11.u32 == 0) {
		// li r3,240
		ctx.r3.s64 = 240;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3280
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7910
			ScoreMessage_ctor_7910(ctx, base);
			// b 0x823c3284
		} else {
		loc_823C3280:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3284:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3288:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_32A0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_32A0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_32A0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c32f0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,4
		ctx.r10.s64 = 4;
		// addi r11,r3,220
		ctx.r11.s64 = ctx.r3.s64 + 220;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C32D0:
		// addi r11,r11,-44
		ctx.r11.s64 = ctx.r11.s64 + -44;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c32d0
		if (ctx.r10.s32 >= 0) goto loc_823C32D0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C32F0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3308_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3308_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3308_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c33ec
	if (ctx.r11.u32 == 0) {
		// li r3,64
		ctx.r3.s64 = 64;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c33e4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r9,4
			ctx.r9.s64 = 4;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r6,0
			ctx.r6.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-1520
			ctx.r11.s64 = ctx.r11.s64 + -1520;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// stfs f0,28(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 28, temp.u32);
			// stfs f0,40(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 40, temp.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r11.u32);
			// stw r11,24(r3)
			PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r11.u32);
			// stw r11,36(r3)
			PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r11.u32);
			// sth r9,50(r3)
			PPC_STORE_U16(ctx.r3.u32 + 50, ctx.r9.u16);
			// sth r6,52(r3)
			PPC_STORE_U16(ctx.r3.u32 + 52, ctx.r6.u16);
			// sth r8,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r8.u16);
			// sth r9,48(r3)
			PPC_STORE_U16(ctx.r3.u32 + 48, ctx.r9.u16);
			// sth r7,46(r3)
			PPC_STORE_U16(ctx.r3.u32 + 46, ctx.r7.u16);
			// lhz r5,50(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 50);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c33e8
			if (ctx.r4.s32 <= 0) goto loc_823C33E8;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C33A8:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,50(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 50);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c33a8
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C33A8;
			// b 0x823c33e8
		} else {
		loc_823C33E4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C33E8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C33EC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3400_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3400_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3400_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c3450
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,3
		ctx.r10.s64 = 3;
		// addi r11,r3,48
		ctx.r11.s64 = ctx.r3.s64 + 48;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C3430:
		// addi r11,r11,-12
		ctx.r11.s64 = ctx.r11.s64 + -12;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c3430
		if (ctx.r10.s32 >= 0) goto loc_823C3430;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C3450:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3468_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3468_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3468_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3544
	if (ctx.r11.u32 == 0) {
		// li r3,80
		ctx.r3.s64 = 80;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c353c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r9,4
			ctx.r9.s64 = 4;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r6,0
			ctx.r6.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r11,r11,-1480
			ctx.r11.s64 = ctx.r11.s64 + -1480;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// stfs f0,36(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 36, temp.u32);
			// stfs f0,52(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 52, temp.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// stw r11,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// stw r11,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r11.u32);
			// stw r11,32(r3)
			PPC_STORE_U32(ctx.r3.u32 + 32, ctx.r11.u32);
			// stw r11,48(r3)
			PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r11.u32);
			// sth r9,66(r3)
			PPC_STORE_U16(ctx.r3.u32 + 66, ctx.r9.u16);
			// sth r6,68(r3)
			PPC_STORE_U16(ctx.r3.u32 + 68, ctx.r6.u16);
			// sth r8,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r8.u16);
			// sth r9,64(r3)
			PPC_STORE_U16(ctx.r3.u32 + 64, ctx.r9.u16);
			// sth r7,62(r3)
			PPC_STORE_U16(ctx.r3.u32 + 62, ctx.r7.u16);
			// lhz r5,66(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c3540
			if (ctx.r4.s32 <= 0) goto loc_823C3540;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C3508:
			// addi r9,r11,1
			ctx.r9.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r9,16
			ctx.r10.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r8.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,66(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 66);
			// addi r7,r11,-1
			ctx.r7.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r7
			// blt cr6,0x823c3508
			if (ctx.r11.s32 < ctx.r7.s32) goto loc_823C3508;
			// b 0x823c3540
		} else {
		loc_823C353C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3540:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3544:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3558_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3558_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3558_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3598
	if (ctx.r11.u32 == 0) {
		// li r3,112
		ctx.r3.s64 = 112;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3590
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7b68
			pongNetMessageHolder_7B68_wrh(ctx, base);
			// b 0x823c3594
		} else {
		loc_823C3590:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3594:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3598:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_35B0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_35B0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_35B0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c3600
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,4
		ctx.r10.s64 = 4;
		// addi r11,r3,100
		ctx.r11.s64 = ctx.r3.s64 + 100;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C35E0:
		// addi r11,r11,-20
		ctx.r11.s64 = ctx.r11.s64 + -20;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c35e0
		if (ctx.r10.s32 >= 0) goto loc_823C35E0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C3600:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3618_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3618_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3618_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3658
	if (ctx.r11.u32 == 0) {
		// li r3,128
		ctx.r3.s64 = 128;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3650
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7c48
			pongNetMessageHolder_7C48_wrh(ctx, base);
			// b 0x823c3654
		} else {
		loc_823C3650:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3654:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3658:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3670_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3670_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3670_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c36c0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,120
		ctx.r11.s64 = ctx.r3.s64 + 120;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C36A0:
		// addi r11,r11,-12
		ctx.r11.s64 = ctx.r11.s64 + -12;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c36a0
		if (ctx.r10.s32 >= 0) goto loc_823C36A0;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C36C0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_36D8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_36D8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_36D8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3718
	if (ctx.r11.u32 == 0) {
		// li r3,5248
		ctx.r3.s64 = 5248;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3710
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7d28
			InternalMessageRelay_ctor_7D28(ctx, base);
			// b 0x823c3714
		} else {
		loc_823C3710:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3714:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3718:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3730_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3730_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3730_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c3780
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,9
		ctx.r10.s64 = 9;
		// addi r11,r3,5240
		ctx.r11.s64 = ctx.r3.s64 + 5240;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C3760:
		// addi r11,r11,-524
		ctx.r11.s64 = ctx.r11.s64 + -524;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c3760
		if (ctx.r10.s32 >= 0) goto loc_823C3760;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C3780:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3798_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3798_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3798_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3864
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c385c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-356
			ctx.r10.s64 = ctx.r11.s64 + -356;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c3860
			if (ctx.r5.s32 <= 0) goto loc_823C3860;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C3820:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c3820
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C3820;
			// b 0x823c3860
		} else {
		loc_823C385C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3860:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3864:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3878_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3878_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3878_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c38c8
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,24
		ctx.r11.s64 = ctx.r3.s64 + 24;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C38A8:
		// addi r11,r11,-12
		ctx.r11.s64 = ctx.r11.s64 + -12;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c38a8
		if (ctx.r10.s32 >= 0) goto loc_823C38A8;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C38C8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_38E0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_38E0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_38E0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c39d4
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c39cc
		if (ctx.r3.u32 != 0) {
			// lis r11,-32249
			// li r10,-1
			// addi r7,r11,-316
			ctx.r7.s64 = ctx.r11.s64 + -316;
			// lis r11,-32248
			// lis r9,0
			ctx.r9.s64 = 0;
			// li r6,2
			ctx.r6.s64 = 2;
			// ori r8,r9,65535
			ctx.r8.u64 = ctx.r9.u64 | 65535;
			// stw r7,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r7.u32);
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// stb r10,8(r3)
			PPC_STORE_U8(ctx.r3.u32 + 8, ctx.r10.u8);
			// stb r10,10(r3)
			PPC_STORE_U8(ctx.r3.u32 + 10, ctx.r10.u8);
			// mr r9,r11
			ctx.r9.u64 = ctx.r11.u64;
			// stb r10,12(r3)
			PPC_STORE_U8(ctx.r3.u32 + 12, ctx.r10.u8);
			// stb r10,13(r3)
			PPC_STORE_U8(ctx.r3.u32 + 13, ctx.r10.u8);
			// stb r11,14(r3)
			PPC_STORE_U8(ctx.r3.u32 + 14, ctx.r11.u8);
			// stfs f0,24(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 24, temp.u32);
			// stw r7,20(r3)
			PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r7.u32);
			// stb r10,28(r3)
			PPC_STORE_U8(ctx.r3.u32 + 28, ctx.r10.u8);
			// stb r10,30(r3)
			PPC_STORE_U8(ctx.r3.u32 + 30, ctx.r10.u8);
			// stb r10,32(r3)
			PPC_STORE_U8(ctx.r3.u32 + 32, ctx.r10.u8);
			// stb r10,33(r3)
			PPC_STORE_U8(ctx.r3.u32 + 33, ctx.r10.u8);
			// stb r11,34(r3)
			PPC_STORE_U8(ctx.r3.u32 + 34, ctx.r11.u8);
			// sth r6,42(r3)
			PPC_STORE_U16(ctx.r3.u32 + 42, ctx.r6.u16);
			// sth r8,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r8.u16);
			// sth r6,40(r3)
			PPC_STORE_U16(ctx.r3.u32 + 40, ctx.r6.u16);
			// sth r11,44(r3)
			PPC_STORE_U16(ctx.r3.u32 + 44, ctx.r11.u16);
			// sth r8,38(r3)
			PPC_STORE_U16(ctx.r3.u32 + 38, ctx.r8.u16);
			// lhz r8,42(r3)
			ctx.r8.u64 = PPC_LOAD_U16(ctx.r3.u32 + 42);
			// addic. r7,r8,-1
			ctx.xer.ca = ctx.r8.u32 > 0;
			ctx.r7.s64 = ctx.r8.s64 + -1;
			// ble 0x823c39d0
			if (ctx.r7.s32 <= 0) goto loc_823C39D0;
		loc_823C3990:
			// rlwinm r10,r11,2,0,29
			ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r6,r11,1
			ctx.r6.s64 = ctx.r11.s64 + 1;
			// add r4,r11,r10
			ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
			// mr r5,r9
			ctx.r5.u64 = ctx.r9.u64;
			// rlwinm r11,r4,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r9,r6,16
			ctx.r9.u64 = ctx.r6.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r9,18(r11)
			PPC_STORE_U16(ctx.r11.u32 + 18, ctx.r9.u16);
			// sth r5,36(r11)
			PPC_STORE_U16(ctx.r11.u32 + 36, ctx.r5.u16);
			// lhz r11,42(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 42);
			// addi r10,r11,-1
			ctx.r10.s64 = ctx.r11.s64 + -1;
			// mr r11,r9
			ctx.r11.u64 = ctx.r9.u64;
			// cmpw cr6,r11,r10
			// blt cr6,0x823c3990
			if (ctx.r11.s32 < ctx.r10.s32) goto loc_823C3990;
			// b 0x823c39d0
		} else {
		loc_823C39CC:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C39D0:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C39D4:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_39E8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_39E8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_39E8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3ad0
	if (ctx.r11.u32 == 0) {
		// li r3,1248
		ctx.r3.s64 = 1248;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3ac8
		if (ctx.r3.u32 != 0) {
			// lis r7,-32248
			// lis r11,-32249
			// li r8,30
			ctx.r8.s64 = 30;
			// addi r10,r11,-276
			ctx.r10.s64 = ctx.r11.s64 + -276;
			// addi r9,r3,4
			ctx.r9.s64 = ctx.r3.s64 + 4;
			// lfs f0,-25900(r7)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C3A34:
			// addi r8,r8,-1
			ctx.r8.s64 = ctx.r8.s64 + -1;
			// stfs f0,0(r9)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
			// stw r10,-4(r9)
			PPC_STORE_U32(ctx.r9.u32 + -4, ctx.r10.u32);
			// stb r11,4(r9)
			PPC_STORE_U8(ctx.r9.u32 + 4, ctx.r11.u8);
			// cmpwi cr6,r8,0
			// std r11,20(r9)
			PPC_STORE_U64(ctx.r9.u32 + 20, ctx.r11.u64);
			// addi r9,r9,40
			ctx.r9.s64 = ctx.r9.s64 + 40;
			// bge cr6,0x823c3a34
			if (ctx.r8.s32 >= 0) goto loc_823C3A34;
			// lis r10,0
			ctx.r10.s64 = 0;
			// sth r11,1244(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1244, ctx.r11.u16);
			// li r9,31
			ctx.r9.s64 = 31;
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// sth r9,1242(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1242, ctx.r9.u16);
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r9,1240(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1240, ctx.r9.u16);
			// sth r8,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r8.u16);
			// sth r7,1234(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1234, ctx.r7.u16);
			// lhz r6,1242(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 1242);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c3acc
			if (ctx.r5.s32 <= 0) goto loc_823C3ACC;
		loc_823C3A8C:
			// rlwinm r9,r11,2,0,29
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,3,0,28
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 3) & 0xFFFFFFF8;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,34(r11)
			PPC_STORE_U16(ctx.r11.u32 + 34, ctx.r10.u16);
			// sth r8,72(r11)
			PPC_STORE_U16(ctx.r11.u32 + 72, ctx.r8.u16);
			// lhz r11,1242(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 1242);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c3a8c
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C3A8C;
			// b 0x823c3acc
		} else {
		loc_823C3AC8:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3ACC:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3AD0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3AE8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3AE8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3AE8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c3b38
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,30
		ctx.r10.s64 = 30;
		// addi r11,r3,1240
		ctx.r11.s64 = ctx.r3.s64 + 1240;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C3B18:
		// addi r11,r11,-40
		ctx.r11.s64 = ctx.r11.s64 + -40;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c3b18
		if (ctx.r10.s32 >= 0) goto loc_823C3B18;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C3B38:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3B50_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3B50_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3B50_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3c4c
	if (ctx.r11.u32 == 0) {
		// li r3,17120
		ctx.r3.s64 = 17120;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3c44
		if (ctx.r3.u32 != 0) {
			// lis r9,-32248
			// lis r10,-32249
			// li r8,30
			ctx.r8.s64 = 30;
			// addi r7,r10,-236
			ctx.r7.s64 = ctx.r10.s64 + -236;
			// addi r11,r3,24
			ctx.r11.s64 = ctx.r3.s64 + 24;
			// lfs f0,-25900(r9)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r10,0
			ctx.r10.s64 = 0;
			// li r6,-1
		loc_823C3BA0:
			// addi r9,r11,-16
			ctx.r9.s64 = ctx.r11.s64 + -16;
			// stfs f0,-20(r11)
			ctx.fpscr.disableFlushMode();
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r11.u32 + -20, temp.u32);
			// stw r7,-24(r11)
			PPC_STORE_U32(ctx.r11.u32 + -24, ctx.r7.u32);
			// addi r8,r8,-1
			ctx.r8.s64 = ctx.r8.s64 + -1;
			// std r10,0(r11)
			PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r10.u64);
			// cmpwi cr6,r8,0
			// stb r10,0(r9)
			PPC_STORE_U8(ctx.r9.u32 + 0, ctx.r10.u8);
			// sth r10,8(r11)
			PPC_STORE_U16(ctx.r11.u32 + 8, ctx.r10.u16);
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r6,516(r11)
			PPC_STORE_U16(ctx.r11.u32 + 516, ctx.r6.u16);
			// std r10,0(r11)
			PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r10.u64);
			// addi r11,r11,552
			ctx.r11.s64 = ctx.r11.s64 + 552;
			// stb r10,0(r9)
			PPC_STORE_U8(ctx.r9.u32 + 0, ctx.r10.u8);
			// bge cr6,0x823c3ba0
			if (ctx.r8.s32 >= 0) goto loc_823C3BA0;
			// lis r8,0
			ctx.r8.s64 = 0;
			// sth r10,17116(r3)
			PPC_STORE_U16(ctx.r3.u32 + 17116, ctx.r10.u16);
			// li r9,31
			ctx.r9.s64 = 31;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// mr r7,r11
			ctx.r7.u64 = ctx.r11.u64;
			// mr r6,r11
			ctx.r6.u64 = ctx.r11.u64;
			// sth r9,17114(r3)
			PPC_STORE_U16(ctx.r3.u32 + 17114, ctx.r9.u16);
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// sth r9,17112(r3)
			PPC_STORE_U16(ctx.r3.u32 + 17112, ctx.r9.u16);
			// sth r7,544(r3)
			PPC_STORE_U16(ctx.r3.u32 + 544, ctx.r7.u16);
			// sth r6,17106(r3)
			PPC_STORE_U16(ctx.r3.u32 + 17106, ctx.r6.u16);
			// lhz r5,17114(r3)
			ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 17114);
			// addic. r4,r5,-1
			ctx.xer.ca = ctx.r5.u32 > 0;
			ctx.r4.s64 = ctx.r5.s64 + -1;
			// ble 0x823c3c48
			if (ctx.r4.s32 <= 0) goto loc_823C3C48;
		loc_823C3C10:
			// addi r9,r10,1
			ctx.r9.s64 = ctx.r10.s64 + 1;
			// mulli r10,r10,552
			ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(552));
			// mr r8,r11
			ctx.r8.u64 = ctx.r11.u64;
			// add r10,r10,r3
			ctx.r10.u64 = ctx.r10.u64 + ctx.r3.u64;
			// clrlwi r11,r9,16
			ctx.r11.u64 = ctx.r9.u32 & 0xFFFF;
			// sth r8,1096(r10)
			PPC_STORE_U16(ctx.r10.u32 + 1096, ctx.r8.u16);
			// sth r11,546(r10)
			PPC_STORE_U16(ctx.r10.u32 + 546, ctx.r11.u16);
			// lhz r10,17114(r3)
			ctx.r10.u64 = PPC_LOAD_U16(ctx.r3.u32 + 17114);
			// addi r7,r10,-1
			ctx.r7.s64 = ctx.r10.s64 + -1;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// cmpw cr6,r10,r7
			// blt cr6,0x823c3c10
			if (ctx.r10.s32 < ctx.r7.s32) goto loc_823C3C10;
			// b 0x823c3c48
		} else {
		loc_823C3C44:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3C48:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3C4C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3C60_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3C60_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3C60_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c3cb0
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,30
		ctx.r10.s64 = 30;
		// addi r11,r3,17112
		ctx.r11.s64 = ctx.r3.s64 + 17112;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C3C90:
		// addi r11,r11,-552
		ctx.r11.s64 = ctx.r11.s64 + -552;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c3c90
		if (ctx.r10.s32 >= 0) goto loc_823C3C90;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C3CB0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3CC8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3CC8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3CC8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3dac
	if (ctx.r11.u32 == 0) {
		// li r3,1056
		ctx.r3.s64 = 1056;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3da4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32249
			// lis r10,0
			ctx.r10.s64 = 0;
			// addi r9,r11,-196
			ctx.r9.s64 = ctx.r11.s64 + -196;
			// lis r11,-32248
			// ori r10,r10,65535
			ctx.r10.u64 = ctx.r10.u64 | 65535;
			// li r7,-1
			// li r8,2
			ctx.r8.s64 = 2;
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// stw r9,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r9.u32);
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// li r11,0
			ctx.r11.s64 = 0;
			// mr r5,r10
			ctx.r5.u64 = ctx.r10.u64;
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// sth r7,516(r3)
			PPC_STORE_U16(ctx.r3.u32 + 516, ctx.r7.u16);
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r11,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
			// stfs f0,528(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 528, temp.u32);
			// stw r9,524(r3)
			PPC_STORE_U32(ctx.r3.u32 + 524, ctx.r9.u32);
			// sth r11,532(r3)
			PPC_STORE_U16(ctx.r3.u32 + 532, ctx.r11.u16);
			// sth r11,534(r3)
			PPC_STORE_U16(ctx.r3.u32 + 534, ctx.r11.u16);
			// sth r7,1040(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1040, ctx.r7.u16);
			// sth r8,1050(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1050, ctx.r8.u16);
			// sth r8,1048(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1048, ctx.r8.u16);
			// sth r11,1052(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1052, ctx.r11.u16);
			// sth r6,520(r3)
			PPC_STORE_U16(ctx.r3.u32 + 520, ctx.r6.u16);
			// sth r5,1046(r3)
			PPC_STORE_U16(ctx.r3.u32 + 1046, ctx.r5.u16);
			// lhz r4,1050(r3)
			ctx.r4.u64 = PPC_LOAD_U16(ctx.r3.u32 + 1050);
			// addic. r9,r4,-1
			ctx.xer.ca = ctx.r4.u32 > 0;
			ctx.r9.s64 = ctx.r4.s64 + -1;
			// ble 0x823c3da8
			if (ctx.r9.s32 <= 0) goto loc_823C3DA8;
		loc_823C3D70:
			// addi r8,r11,1
			ctx.r8.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,524
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(524));
			// mr r7,r10
			ctx.r7.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r8,16
			ctx.r10.u64 = ctx.r8.u32 & 0xFFFF;
			// sth r7,1044(r11)
			PPC_STORE_U16(ctx.r11.u32 + 1044, ctx.r7.u16);
			// sth r10,522(r11)
			PPC_STORE_U16(ctx.r11.u32 + 522, ctx.r10.u16);
			// lhz r11,1050(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 1050);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c3d70
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C3D70;
			// b 0x823c3da8
		} else {
		loc_823C3DA4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3DA8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3DAC:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3DC0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3DC0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3DC0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3e8c
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3e84
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-156
			ctx.r10.s64 = ctx.r11.s64 + -156;
			// stfs f0,16(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 16, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,12(r3)
			PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,26(r3)
			PPC_STORE_U16(ctx.r3.u32 + 26, ctx.r9.u16);
			// sth r7,28(r3)
			PPC_STORE_U16(ctx.r3.u32 + 28, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,24(r3)
			PPC_STORE_U16(ctx.r3.u32 + 24, ctx.r9.u16);
			// sth r11,22(r3)
			PPC_STORE_U16(ctx.r3.u32 + 22, ctx.r11.u16);
			// lhz r6,26(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c3e88
			if (ctx.r5.s32 <= 0) goto loc_823C3E88;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C3E48:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,26(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 26);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c3e48
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C3E48;
			// b 0x823c3e88
		} else {
		loc_823C3E84:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3E88:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3E8C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3EA0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3EA0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3EA0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3f64
	if (ctx.r11.u32 == 0) {
		// li r3,48
		ctx.r3.s64 = 48;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3f5c
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,2
			ctx.r9.s64 = 2;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,-116
			ctx.r10.s64 = ctx.r11.s64 + -116;
			// stfs f0,20(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 20, temp.u32);
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// stw r10,16(r3)
			PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,34(r3)
			PPC_STORE_U16(ctx.r3.u32 + 34, ctx.r9.u16);
			// sth r7,36(r3)
			PPC_STORE_U16(ctx.r3.u32 + 36, ctx.r7.u16);
			// sth r11,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r11.u16);
			// sth r9,32(r3)
			PPC_STORE_U16(ctx.r3.u32 + 32, ctx.r9.u16);
			// sth r11,30(r3)
			PPC_STORE_U16(ctx.r3.u32 + 30, ctx.r11.u16);
			// lhz r6,34(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c3f60
			if (ctx.r5.s32 <= 0) goto loc_823C3F60;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C3F28:
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// rlwinm r11,r11,4,0,27
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
			// mr r9,r10
			ctx.r9.u64 = ctx.r10.u64;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// sth r9,28(r11)
			PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r9.u16);
			// sth r10,14(r11)
			PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
			// lhz r11,34(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 34);
			// addi r8,r11,-1
			ctx.r8.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r8
			// blt cr6,0x823c3f28
			if (ctx.r11.s32 < ctx.r8.s32) goto loc_823C3F28;
			// b 0x823c3f60
		} else {
		loc_823C3F5C:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3F60:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3F64:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_3F78_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_3F78_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_3F78_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c3fb8
	if (ctx.r11.u32 == 0) {
		// li r3,256
		ctx.r3.s64 = 256;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c3fb0
		if (ctx.r3.u32 != 0) {
			// bl 0x823c7e78
			pongNetMessageHolder_7E78_wrh(ctx, base);
			// b 0x823c3fb4
		} else {
		loc_823C3FB0:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C3FB4:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C3FB8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_3FD0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_3FD0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_3FD0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c4020
	if (ctx.r3.u32 != 0) {
		// lis r9,-32249
		// li r10,1
		ctx.r10.s64 = 1;
		// addi r11,r3,248
		ctx.r11.s64 = ctx.r3.s64 + 248;
		// addi r9,r9,-15612
		ctx.r9.s64 = ctx.r9.s64 + -15612;
	loc_823C4000:
		// addi r11,r11,-124
		ctx.r11.s64 = ctx.r11.s64 + -124;
		// addi r10,r10,-1
		ctx.r10.s64 = ctx.r10.s64 + -1;
		// cmpwi cr6,r10,0
		// stw r9,0(r11)
		PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
		// bge cr6,0x823c4000
		if (ctx.r10.s32 >= 0) goto loc_823C4000;
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C4020:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_4038_h"))) PPC_WEAK_FUNC(pongNetMessageHolder_4038_h);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_4038_h) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r29 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,1504
	ctx.r11.s64 = ctx.r11.s64 + 1504;
	// lwz r30,8(r31)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r31 + 8));
	// cmplwi cr6,r30,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c408c
	if (var_r30 != 0) {
		// addi r11,r30,956
		ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 956;
		// addi r29,r11,-956
		var_r29 = (uint32_t)(ctx.r11.s64 + -956);  // lbl_8206FC44 @ 0x8206fc44
		// addi r3,r29,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r29 + 8;
		// bl 0x822100e0
		PongNetRoundRobinCoordinator_00E0_g(ctx, base);
		// lis r11,-32249
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,0(r29)
		PPC_STORE_U32(var_r29 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C408C:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_40B0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_40B0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_40B0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c4178
	if (ctx.r11.u32 == 0) {
		// li r3,976
		ctx.r3.s64 = 976;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// beq cr6,0x823c4170
		if (var_r31 != 0) {
			// lis r11,-32249
			// lis r10,-32248
			// addi r11,r11,-36
			ctx.r11.s64 = ctx.r11.s64 + -36;
			// addi r3,r31,8
			ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
			// lfs f0,-25900(r10)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,4(r31)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(var_r31 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// stw r11,0(r31)
			PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// bl 0x8220fc20
			PongNetRoundRobinCoordinator_FC20_w(ctx, base);
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,1
			ctx.r9.s64 = 1;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,958(r31)
			PPC_STORE_U16(var_r31 + 958, ctx.r9.u16);
			// sth r11,952(r31)
			PPC_STORE_U16(var_r31 + 952, ctx.r11.u16);
			// sth r10,960(r31)
			PPC_STORE_U16(var_r31 + 960, ctx.r10.u16);
			// sth r9,956(r31)
			PPC_STORE_U16(var_r31 + 956, ctx.r9.u16);
			// sth r11,954(r31)
			PPC_STORE_U16(var_r31 + 954, ctx.r11.u16);
			// lhz r7,958(r31)
			ctx.r7.u64 = PPC_LOAD_U16(var_r31 + 958);
			// addic. r6,r7,-1
			ctx.xer.ca = ctx.r7.u32 > 0;
			ctx.r6.s64 = ctx.r7.s64 + -1;
			// ble 0x823c4174
			if (ctx.r6.s32 <= 0) goto loc_823C4174;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
		loc_823C413C:
			// addi r5,r11,1
			ctx.r5.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,956
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(956));
			// mr r4,r10
			ctx.r4.u64 = ctx.r10.u64;
			// add r11,r11,r31
			ctx.r11.u64 = ctx.r11.u64 + var_r31;
			// clrlwi r10,r5,16
			ctx.r10.u64 = ctx.r5.u32 & 0xFFFF;
			// sth r4,1908(r11)
			PPC_STORE_U16(ctx.r11.u32 + 1908, ctx.r4.u16);
			// sth r10,954(r11)
			PPC_STORE_U16(ctx.r11.u32 + 954, ctx.r10.u16);
			// lhz r11,958(r31)
			ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 958);
			// addi r3,r11,-1
			ctx.r3.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r3
			// blt cr6,0x823c413c
			if (ctx.r11.s32 < ctx.r3.s32) goto loc_823C413C;
			// b 0x823c4174
		} else {
		loc_823C4170:
			// li r31,0
			var_r31 = 0;
		}
	loc_823C4174:
		// stw r31,8(r30)
		PPC_STORE_U32(var_r30 + 8, var_r31);
	}
loc_823C4178:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_4190_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_4190_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_4190_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r29 = 0;
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	// FRAME: size=112, savegprlr_29
	// mr r29,r3
	var_r29 = ctx.r3.u32;
	// lwz r30,8(r29)
	var_r30 = (uint32_t)(PPC_LOAD_U32(var_r29 + 8));
	// cmplwi cr6,r30,0
	// beq cr6,0x823c41d8
	if (var_r30 != 0) {
		// addi r11,r30,956
		ctx.r11.s64 = (int64_t)(int32_t)var_r30 + 956;
		// addi r31,r11,-956
		var_r31 = (uint32_t)(ctx.r11.s64 + -956);  // lbl_8206FC44 @ 0x8206fc44
		// addi r3,r31,8
		ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
		// bl 0x822100e0
		PongNetRoundRobinCoordinator_00E0_g(ctx, base);
		// lis r11,-32249
		// mr r3,r30
		ctx.r3.u64 = var_r30;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,0(r31)
		PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r29)
		PPC_STORE_U32(var_r29 + 8, ctx.r11.u32);
	}
loc_823C41D8:
	return;
}

__attribute__((alias("__imp__rage_41E0"))) PPC_WEAK_FUNC(rage_41E0);
PPC_FUNC_IMPL(__imp__rage_41E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,1524
	ctx.r11.s64 = ctx.r11.s64 + 1524;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c4228
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,12
		ctx.r10.s64 = ctx.r3.s64 + 12;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-12(r10)
		PPC_STORE_U32(ctx.r10.u32 + -12, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C4228:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_4258_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_4258_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_4258_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c431c
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c4314
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,1
			ctx.r9.s64 = 1;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,4
			ctx.r10.s64 = ctx.r11.s64 + 4;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,14(r3)
			PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
			// sth r7,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r11,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
			// lhz r6,14(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c4318
			if (ctx.r5.s32 <= 0) goto loc_823C4318;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C42D8:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,14(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c42d8
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C42D8;
			// b 0x823c4318
		} else {
		loc_823C4314:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C4318:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C431C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_4330_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_4330_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_4330_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c436c
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,12
		ctx.r10.s64 = ctx.r3.s64 + 12;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-12(r10)
		PPC_STORE_U32(ctx.r10.u32 + -12, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C436C:
	// blr
	return;
}

__attribute__((alias("__imp__rage_4380"))) PPC_WEAK_FUNC(rage_4380);
PPC_FUNC_IMPL(__imp__rage_4380) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,1544
	ctx.r11.s64 = ctx.r11.s64 + 1544;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c43c8
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,12
		ctx.r10.s64 = ctx.r3.s64 + 12;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-12(r10)
		PPC_STORE_U32(ctx.r10.u32 + -12, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C43C8:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_43F8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_43F8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_43F8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c44bc
	if (ctx.r11.u32 == 0) {
		// li r3,32
		ctx.r3.s64 = 32;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// cmplwi cr6,r3,0
		// beq cr6,0x823c44b4
		if (ctx.r3.u32 != 0) {
			// lis r11,-32248
			// lis r8,0
			ctx.r8.s64 = 0;
			// li r9,1
			ctx.r9.s64 = 1;
			// li r7,0
			ctx.r7.s64 = 0;
			// lfs f0,-25900(r11)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// lis r11,-32249
			// stfs f0,4(r3)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(ctx.r3.u32 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// addi r10,r11,44
			ctx.r10.s64 = ctx.r11.s64 + 44;
			// ori r11,r8,65535
			ctx.r11.u64 = ctx.r8.u64 | 65535;
			// stw r10,0(r3)
			PPC_STORE_U32(ctx.r3.u32 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
			// li r10,0
			ctx.r10.s64 = 0;
			// sth r9,14(r3)
			PPC_STORE_U16(ctx.r3.u32 + 14, ctx.r9.u16);
			// sth r7,16(r3)
			PPC_STORE_U16(ctx.r3.u32 + 16, ctx.r7.u16);
			// sth r11,8(r3)
			PPC_STORE_U16(ctx.r3.u32 + 8, ctx.r11.u16);
			// sth r9,12(r3)
			PPC_STORE_U16(ctx.r3.u32 + 12, ctx.r9.u16);
			// sth r11,10(r3)
			PPC_STORE_U16(ctx.r3.u32 + 10, ctx.r11.u16);
			// lhz r6,14(r3)
			ctx.r6.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addic. r5,r6,-1
			ctx.xer.ca = ctx.r6.u32 > 0;
			ctx.r5.s64 = ctx.r6.s64 + -1;
			// ble 0x823c44b8
			if (ctx.r5.s32 <= 0) goto loc_823C44B8;
			// li r11,0
			ctx.r11.s64 = 0;
		loc_823C4478:
			// rlwinm r9,r11,1,0,30
			ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
			// addi r4,r11,1
			ctx.r4.s64 = ctx.r11.s64 + 1;
			// add r7,r11,r9
			ctx.r7.u64 = ctx.r11.u64 + ctx.r9.u64;
			// mr r8,r10
			ctx.r8.u64 = ctx.r10.u64;
			// rlwinm r11,r7,2,0,29
			ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
			// clrlwi r10,r4,16
			ctx.r10.u64 = ctx.r4.u32 & 0xFFFF;
			// add r11,r11,r3
			ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
			// sth r10,10(r11)
			PPC_STORE_U16(ctx.r11.u32 + 10, ctx.r10.u16);
			// sth r8,20(r11)
			PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r8.u16);
			// lhz r11,14(r3)
			ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 14);
			// addi r6,r11,-1
			ctx.r6.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r6
			// blt cr6,0x823c4478
			if (ctx.r11.s32 < ctx.r6.s32) goto loc_823C4478;
			// b 0x823c44b8
		} else {
		loc_823C44B4:
			// li r3,0
			ctx.r3.s64 = 0;
		}
	loc_823C44B8:
		// stw r3,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r3.u32);
	}
loc_823C44BC:
	// blr
	return;
}

__attribute__((alias("__imp__rage_44D0"))) PPC_WEAK_FUNC(rage_44D0);
PPC_FUNC_IMPL(__imp__rage_44D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lis r11,-32249
	// addi r11,r11,1564
	ctx.r11.s64 = ctx.r11.s64 + 1564;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r11.u32);
	// beq cr6,0x823c4518
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,472
		ctx.r10.s64 = ctx.r3.s64 + 472;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-472(r10)
		PPC_STORE_U32(ctx.r10.u32 + -472, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C4518:
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* rage_GameObject::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);  /* glob:0x820766a4 */
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);  /* glob:0x820766a4 */
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_1_4548_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_1_4548_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_1_4548_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r30 = 0;
	uint32_t var_r31 = 0;
	PPCRegister temp{};
	// FRAME: size=112, manual
	// mr r30,r3
	var_r30 = ctx.r3.u32;
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(var_r30 + 8);
	// cmplwi cr6,r11,0
	// bne cr6,0x823c4614
	if (ctx.r11.u32 == 0) {
		// li r3,480
		ctx.r3.s64 = 480;
		// bl 0x820dec88
		xe_EC88(ctx, base);
		// mr r31,r3
		var_r31 = ctx.r3.u32;
		// cmplwi cr6,r31,0
		// beq cr6,0x823c460c
		if (var_r31 != 0) {
			// lis r11,-32249
			// lis r10,-32248
			// addi r11,r11,84
			ctx.r11.s64 = ctx.r11.s64 + 84;
			// addi r3,r31,8
			ctx.r3.s64 = (int64_t)(int32_t)var_r31 + 8;
			// lfs f0,-25900(r10)
			ctx.fpscr.disableFlushMode();
			temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -25900);
			ctx.f0.f64 = double(temp.f32);
			// stfs f0,4(r31)
			temp.f32 = float(ctx.f0.f64);
			PPC_STORE_U32(var_r31 + 4,/* pongNetMessageHolder::flags@+0x4 */ temp.u32);
			// stw r11,0(r31)
			PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
			// bl 0x823b35e0
			SinglesNetworkClient_35E0_p45(ctx, base);
			// lis r10,0
			ctx.r10.s64 = 0;
			// li r11,0
			ctx.r11.s64 = 0;
			// ori r9,r10,65535
			ctx.r9.u64 = ctx.r10.u64 | 65535;
			// li r8,1
			ctx.r8.s64 = 1;
			// mr r10,r11
			ctx.r10.u64 = ctx.r11.u64;
			// stw r11,456(r31)
			PPC_STORE_U32(var_r31 + 456, ctx.r11.u32);
			// sth r9,464(r31)
			PPC_STORE_U16(var_r31 + 464, ctx.r9.u16);
			// sth r8,474(r31)
			PPC_STORE_U16(var_r31 + 474, ctx.r8.u16);
			// sth r8,472(r31)
			PPC_STORE_U16(var_r31 + 472, ctx.r8.u16);
			// sth r11,476(r31)
			PPC_STORE_U16(var_r31 + 476, ctx.r11.u16);
			// sth r9,466(r31)
			PPC_STORE_U16(var_r31 + 466, ctx.r9.u16);
			// lhz r9,474(r31)
			ctx.r9.u64 = PPC_LOAD_U16(var_r31 + 474);
			// addic. r8,r9,-1
			ctx.xer.ca = ctx.r9.u32 > 0;
			ctx.r8.s64 = ctx.r9.s64 + -1;
			// ble 0x823c4610
			if (ctx.r8.s32 <= 0) goto loc_823C4610;
		loc_823C45D8:
			// addi r7,r11,1
			ctx.r7.s64 = ctx.r11.s64 + 1;
			// mulli r11,r11,472
			ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(472));
			// mr r6,r10
			ctx.r6.u64 = ctx.r10.u64;
			// add r11,r11,r31
			ctx.r11.u64 = ctx.r11.u64 + var_r31;
			// clrlwi r10,r7,16
			ctx.r10.u64 = ctx.r7.u32 & 0xFFFF;
			// sth r6,936(r11)
			PPC_STORE_U16(ctx.r11.u32 + 936, ctx.r6.u16);
			// sth r10,466(r11)
			PPC_STORE_U16(ctx.r11.u32 + 466, ctx.r10.u16);
			// lhz r11,474(r31)
			ctx.r11.u64 = PPC_LOAD_U16(var_r31 + 474);
			// addi r5,r11,-1
			ctx.r5.s64 = ctx.r11.s64 + -1;
			// mr r11,r10
			ctx.r11.u64 = ctx.r10.u64;
			// cmpw cr6,r11,r5
			// blt cr6,0x823c45d8
			if (ctx.r11.s32 < ctx.r5.s32) goto loc_823C45D8;
			// b 0x823c4610
		} else {
		loc_823C460C:
			// li r31,0
			var_r31 = 0;
		}
	loc_823C4610:
		// stw r31,8(r30)
		PPC_STORE_U32(var_r30 + 8, var_r31);
	}
loc_823C4614:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_2_4630_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_2_4630_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_2_4630_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	// FRAME: size=96, manual
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(var_r31 + 8);
	// cmplwi cr6,r3,0
	// beq cr6,0x823c466c
	if (ctx.r3.u32 != 0) {
		// lis r11,-32249
		// addi r10,r3,472
		ctx.r10.s64 = ctx.r3.s64 + 472;
		// addi r11,r11,-15612
		ctx.r11.s64 = ctx.r11.s64 + -15612;
		// stw r11,-472(r10)
		PPC_STORE_U32(ctx.r10.u32 + -472, ctx.r11.u32);
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// li r11,0
		ctx.r11.s64 = 0;
		// stw r11,8(r31)
		PPC_STORE_U32(var_r31 + 8, ctx.r11.u32);
	}
loc_823C466C:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,124
	ctx.r11.s64 = ctx.r11.s64 + 124;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823bfc00
	pongNetMessageHolder_vfn_2(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c46e0
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C46E0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_46F8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_46F8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_46F8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,144
	ctx.r11.s64 = ctx.r11.s64 + 144;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0868
	pongNetMessageHolder_vfn_2_0868_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4758
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4758:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4770_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4770_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4770_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,164
	ctx.r11.s64 = ctx.r11.s64 + 164;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c07a8
	pongNetMessageHolder_vfn_2_07A8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c47d0
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C47D0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_47E8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_47E8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_47E8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,184
	ctx.r11.s64 = ctx.r11.s64 + 184;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823bfd70
	pongNetMessageHolder_vfn_2_FD70_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4848
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4848:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4860_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4860_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4860_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,204
	ctx.r11.s64 = ctx.r11.s64 + 204;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c24b8
	pongNetMessageHolder_vfn_2_24B8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c48c0
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C48C0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_48D8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_48D8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_48D8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,224
	ctx.r11.s64 = ctx.r11.s64 + 224;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823bff38
	pongNetMessageHolder_vfn_2_FF38_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4938
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4938:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4950_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4950_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4950_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,244
	ctx.r11.s64 = ctx.r11.s64 + 244;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823bfff8
	pongNetMessageHolder_vfn_2_FFF8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c49b0
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C49B0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_49C8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_49C8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_49C8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,264
	ctx.r11.s64 = ctx.r11.s64 + 264;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0638
	pongNetMessageHolder_vfn_2_0638_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4a28
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4A28:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4A40_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4A40_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4A40_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,284
	ctx.r11.s64 = ctx.r11.s64 + 284;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0260
	pongNetMessageHolder_vfn_2_0260_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4aa0
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4AA0:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4AB8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4AB8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4AB8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,304
	ctx.r11.s64 = ctx.r11.s64 + 304;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c07a8
	pongNetMessageHolder_vfn_2_07A8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4b18
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4B18:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4B30_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4B30_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4B30_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,324
	ctx.r11.s64 = ctx.r11.s64 + 324;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c04d0
	pongNetMessageHolder_vfn_2_04D0_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4b90
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4B90:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4BA8_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4BA8_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4BA8_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,344
	ctx.r11.s64 = ctx.r11.s64 + 344;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0638
	pongNetMessageHolder_vfn_2_0638_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4c08
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4C08:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4C20_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4C20_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4C20_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,364
	ctx.r11.s64 = ctx.r11.s64 + 364;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c07a8
	pongNetMessageHolder_vfn_2_07A8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4c80
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4C80:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4C98_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4C98_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4C98_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,384
	ctx.r11.s64 = ctx.r11.s64 + 384;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0868
	pongNetMessageHolder_vfn_2_0868_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4cf8
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4CF8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4D10_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4D10_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4D10_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,404
	ctx.r11.s64 = ctx.r11.s64 + 404;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0928
	pongNetMessageHolder_vfn_2_0928_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4d70
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4D70:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4D88_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4D88_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4D88_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,424
	ctx.r11.s64 = ctx.r11.s64 + 424;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c09e8
	pongNetMessageHolder_vfn_2_09E8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4de8
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4DE8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4E00_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4E00_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4E00_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,444
	ctx.r11.s64 = ctx.r11.s64 + 444;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0b58
	pongNetMessageHolder_vfn_2_0B58_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4e60
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4E60:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4E78_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4E78_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4E78_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0868
	pongNetMessageHolder_vfn_2_0868_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4ed8
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4ED8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4EF0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4EF0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4EF0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,484
	ctx.r11.s64 = ctx.r11.s64 + 484;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0c70
	pongNetMessageHolder_vfn_2_0C70_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4f50
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4F50:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4F68_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4F68_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4F68_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,504
	ctx.r11.s64 = ctx.r11.s64 + 504;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c24b8
	pongNetMessageHolder_vfn_2_24B8_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c4fc8
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C4FC8:
	// blr
	return;
}

__attribute__((alias("__imp__pongNetMessageHolder_vfn_0_4FE0_1"))) PPC_WEAK_FUNC(pongNetMessageHolder_vfn_0_4FE0_1);
PPC_FUNC_IMPL(__imp__pongNetMessageHolder_vfn_0_4FE0_1) {
	PPC_FUNC_PROLOGUE();
	uint32_t var_r31 = 0;
	uint32_t var_r30 = 0;
	// FRAME: size=112, manual
	// lis r11,-32249
	// mr r31,r3
	var_r31 = ctx.r3.u32;
	// addi r11,r11,524
	ctx.r11.s64 = ctx.r11.s64 + 524;
	// mr r30,r4
	var_r30 = ctx.r4.u32;
	// stw r11,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r11.u32);
	// bl 0x823c0e38
	pongNetMessageHolder_vfn_2_0E38_1(ctx, base);
	// lis r11,-32249
	// clrlwi r8,r30,31
	ctx.r8.u64 = var_r30 & 0x1;
	// addi r10,r11,-1400
	ctx.r10.s64 = ctx.r11.s64 + -1400;
	// lis r11,-32160
	// cmplwi cr6,r8,0
	// mr r3,r31
	ctx.r3.u64 = var_r31;
	// stw r10,0(r31)
	PPC_STORE_U32(var_r31 + 0,/* pongNetMessageHolder::vtable@+0x0 */ ctx.r10.u32);
	// lwz r9,26276(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 26276);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r9,26276(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26276, ctx.r9.u32);
	// beq cr6,0x823c5040
	if (ctx.r8.u32 != 0) {
		// bl 0x820c00c0
		rage_free_00C0(ctx, base);
		// mr r3,r31
		ctx.r3.u64 = var_r31;
	}
loc_823C5040:
	// blr
	return;
}

