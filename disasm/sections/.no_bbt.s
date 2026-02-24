# Section: .no_bbt
# Base Address: 0x82586A00
# Size: 0x0000044C

.section .no_bbt

0x82586A00:  mflr         r12
0x82586A04:  bl           0x8242f88c
0x82586A08:  stwu         r1, -0x90(r1)
0x82586A0C:  mr           r31, r3
0x82586A10:  mr           r30, r4
0x82586A14:  li           r27, 1
0x82586A18:  lwz          r11, 0x2c0(r31)
0x82586A1C:  lwz          r28, 0x10(r30)
0x82586A20:  ld           r29, 0x18(r30)
0x82586A24:  cmpwi        cr6, r11, 0
0x82586A28:  lwz          r25, 0x20(r30)
0x82586A2C:  lwz          r26, 8(r30)
0x82586A30:  beq          cr6, 0x82586a5c
0x82586A34:  lwz          r10, 0xe0(r31)
0x82586A38:  cmplwi       cr6, r10, 0
0x82586A3C:  bne          cr6, 0x82586a5c
0x82586A40:  lwz          r9, 0x2b8(r31)
0x82586A44:  cmpwi        cr6, r9, 0
0x82586A48:  bne          cr6, 0x82586a5c
0x82586A4C:  cmpwi        cr6, r26, 0
0x82586A50:  beq          cr6, 0x82586ad4
0x82586A54:  bl           0x82498bc8
0x82586A58:  stw          r27, 0x2b8(r31)
0x82586A5C:  cmpwi        cr6, r26, 0
0x82586A60:  beq          cr6, 0x82586ad4
0x82586A64:  cmpwi        cr6, r28, 0
0x82586A68:  beq          cr6, 0x82586ad4
0x82586A6C:  lhz          r8, 0x9a(r31)
0x82586A70:  extsh        r11, r8
0x82586A74:  cmpwi        cr6, r11, 1
0x82586A78:  ble          cr6, 0x82586a84
0x82586A7C:  addi         r7, r11, -1
0x82586A80:  sth          r7, 0x9a(r31)
0x82586A84:  ld           r5, 0xa8(r31)
0x82586A88:  cmpd         cr6, r5, r29
0x82586A8C:  beq          cr6, 0x82586ad4
0x82586A90:  lwz          r4, 0x9c(r31)
0x82586A94:  cmpwi        cr6, r4, 0
0x82586A98:  bne          cr6, 0x82586aac
0x82586A9C:  stw          r27, 0x9c(r31)
0x82586AA0:  std          r29, 0xa8(r31)
0x82586AA4:  sth          r27, 0x9a(r31)
0x82586AA8:  b            0x82586ad4
0x82586AAC:  lhz          r3, 0x9a(r31)
0x82586AB0:  std          r29, 0xb0(r31)
0x82586AB4:  addi         r11, r3, 1
0x82586AB8:  extsh        r11, r11
0x82586ABC:  cmpwi        cr6, r11, 2
0x82586AC0:  sth          r11, 0x9a(r31)
0x82586AC4:  beq          cr6, 0x82586ad4
0x82586AC8:  li           r11, 0
0x82586ACC:  stw          r11, 0x9c(r31)
0x82586AD0:  sth          r11, 0x9a(r31)
0x82586AD4:  lwz          r9, 0x2c0(r31)
0x82586AD8:  cmpwi        cr6, r9, 0
0x82586ADC:  beq          cr6, 0x82586af0
0x82586AE0:  cmpwi        cr6, r26, 0
0x82586AE4:  beq          cr6, 0x82586af0
0x82586AE8:  addi         r3, r31, 0xe0
0x82586AEC:  bl           0x824b3768
0x82586AF0:  lwz          r10, 0x24(r30)
0x82586AF4:  addi         r29, r31, 0xe0
0x82586AF8:  lwz          r8, 0x2c0(r31)
0x82586AFC:  lwz          r7, 0xc(r30)
0x82586B00:  cmplwi       cr6, r10, 0
0x82586B04:  lwz          r6, 8(r30)
0x82586B08:  lwz          r5, 4(r30)
0x82586B0C:  beq          cr6, 0x82586b28
0x82586B10:  lwz          r4, 0(r31)
0x82586B14:  mr           r3, r29
0x82586B18:  lwz          r9, 0x3c(r4)
0x82586B1C:  lwz          r4, 0(r30)
0x82586B20:  bl           0x82586e40
0x82586B24:  b            0x82586b3c
0x82586B28:  lwz          r3, 0(r31)
0x82586B2C:  lwz          r4, 0(r30)
0x82586B30:  lwz          r9, 0x3c(r3)
0x82586B34:  mr           r3, r29
0x82586B38:  bl           0x82586e38
0x82586B3C:  cmpwi        cr6, r3, 0
0x82586B40:  blt          cr6, 0x82586ba4
0x82586B44:  cmpwi        cr6, r3, 6
0x82586B48:  bne          cr6, 0x82586b5c
0x82586B4C:  lwz          r11, 0(r31)
0x82586B50:  li           r10, 6
0x82586B54:  stw          r10, 0x48(r11)
0x82586B58:  stw          r27, 0x2b8(r31)
0x82586B5C:  lwz          r9, 0x2c0(r31)
0x82586B60:  cmpwi        cr6, r9, 0
0x82586B64:  beq          cr6, 0x82586ba4
0x82586B68:  cmpwi        cr6, r26, 0
0x82586B6C:  bne          cr6, 0x82586b88
0x82586B70:  cmpwi        cr6, r25, 0
0x82586B74:  beq          cr6, 0x82586ba4
0x82586B78:  lis          r3, -0x7ff9
0x82586B7C:  ori          r3, r3, 0x57
0x82586B80:  addi         r1, r1, 0x90
0x82586B84:  b            0x8242f8dc
0x82586B88:  cmpwi        cr6, r25, 0
0x82586B8C:  beq          cr6, 0x82586ba4
0x82586B90:  cmpwi        cr6, r25, 8
0x82586B94:  bge          cr6, 0x82586b78
0x82586B98:  mr           r4, r25
0x82586B9C:  mr           r3, r29
0x82586BA0:  bl           0x824b4270
0x82586BA4:  addi         r1, r1, 0x90
0x82586BA8:  b            0x8242f8dc
0x82586BAC:  .align 0                       # padding
0x82586BB0:  mflr         r12
0x82586BB4:  bl           0x8242f880
0x82586BB8:  stwu         r1, -0xb0(r1)
0x82586BBC:  mr           r30, r5
0x82586BC0:  li           r25, 0
0x82586BC4:  mr           r31, r3
0x82586BC8:  mr           r29, r4
0x82586BCC:  mr           r27, r6
0x82586BD0:  mr           r28, r8
0x82586BD4:  mr           r23, r9
0x82586BD8:  mr           r24, r25
0x82586BDC:  li           r22, 1
0x82586BE0:  cmpwi        cr6, r30, 0
0x82586BE4:  beq          cr6, 0x82586bf0
0x82586BE8:  cmpwi        cr6, r7, 0
0x82586BEC:  beq          cr6, 0x82586bf4
0x82586BF0:  stw          r22, 0x4c(r31)
0x82586BF4:  cmpwi        cr6, r30, 0
0x82586BF8:  ble          cr6, 0x82586e28
0x82586BFC:  cmplwi       cr6, r10, 0
0x82586C00:  stw          r29, 0x1c(r31)
0x82586C04:  stw          r30, 0x20(r31)
0x82586C08:  beq          cr6, 0x82586c14
0x82586C0C:  stw          r10, 0x54(r31)
0x82586C10:  b            0x82586c20
0x82586C14:  lis          r11, -0x7dbf
0x82586C18:  addi         r11, r11, -0x1930
0x82586C1C:  stw          r11, 0x54(r31)
0x82586C20:  cmpwi        cr6, r27, 0
0x82586C24:  bne          cr6, 0x82586c30
0x82586C28:  cmpwi        cr6, r28, 0
0x82586C2C:  beq          cr6, 0x82586c98
0x82586C30:  cmpwi        cr6, r28, 0
0x82586C34:  stw          r29, 0x14(r31)
0x82586C38:  stw          r30, 0x18(r31)
0x82586C3C:  beq          cr6, 0x82586c98
0x82586C40:  lwz          r3, 8(r31)
0x82586C44:  bl           0x82498070
0x82586C48:  clrlwi       r11, r3, 0x18
0x82586C4C:  srawi        r10, r11, 3
0x82586C50:  addze        r9, r10
0x82586C54:  slwi         r8, r9, 3
0x82586C58:  subf.        r7, r8, r11
0x82586C5C:  beq          0x82586c6c
0x82586C60:  lwz          r11, 0x50(r31)
0x82586C64:  add          r6, r11, r30
0x82586C68:  stw          r6, 0x50(r31)
0x82586C6C:  lwz          r5, 0x44(r31)
0x82586C70:  cmpwi        cr6, r5, 0
0x82586C74:  beq          cr6, 0x82586c98
0x82586C78:  lwz          r11, 0x48(r31)
0x82586C7C:  cmplw        cr6, r30, r11
0x82586C80:  ble          cr6, 0x82586c90
0x82586C84:  stw          r11, 0x20(r31)
0x82586C88:  stw          r25, 0x44(r31)
0x82586C8C:  b            0x82586c98
0x82586C90:  subf         r4, r30, r11
0x82586C94:  stw          r4, 0x48(r31)
0x82586C98:  lwz          r3, 0x38(r31)
0x82586C9C:  cmpwi        cr6, r3, 0
0x82586CA0:  bne          cr6, 0x82586cc0
0x82586CA4:  cmpwi        cr6, r27, 0
0x82586CA8:  beq          cr6, 0x82586e1c
0x82586CAC:  lwz          r11, 0xc(r31)
0x82586CB0:  addi         r11, r11, 1
0x82586CB4:  clrlwi       r10, r11, 0x16
0x82586CB8:  stw          r10, 0xc(r31)
0x82586CBC:  b            0x82586e1c
0x82586CC0:  cmpwi        cr6, r27, 0
0x82586CC4:  beq          cr6, 0x82586e1c
0x82586CC8:  cmpwi        cr6, r28, 0
0x82586CCC:  bne          cr6, 0x82586e1c
0x82586CD0:  lwz          r8, 0x10(r31)
0x82586CD4:  stw          r25, 0x10(r31)
0x82586CD8:  lwz          r9, 0x54(r31)
0x82586CDC:  stw          r8, 0xc(r31)
0x82586CE0:  lbz          r3, 0(r29)
0x82586CE4:  mtctr        r9
0x82586CE8:  bctrl        
0x82586CEC:  lwz          r7, 0x1c(r31)
0x82586CF0:  mr           r29, r3
0x82586CF4:  lwz          r6, 0x54(r31)
0x82586CF8:  lbz          r3, 1(r7)
0x82586CFC:  mtctr        r6
0x82586D00:  bctrl        
0x82586D04:  lwz          r5, 0x1c(r31)
0x82586D08:  mr           r28, r3
0x82586D0C:  lwz          r4, 0x54(r31)
0x82586D10:  lbz          r3, 2(r5)
0x82586D14:  mtctr        r4
0x82586D18:  bctrl        
0x82586D1C:  lwz          r11, 0x1c(r31)
0x82586D20:  mr           r27, r3
0x82586D24:  lwz          r10, 0x54(r31)
0x82586D28:  lbz          r3, 3(r11)
0x82586D2C:  mtctr        r10
0x82586D30:  bctrl        
0x82586D34:  rlwimi       r28, r29, 8, 0x10, 0x17
0x82586D38:  mr           r30, r3
0x82586D3C:  lwz          r26, 8(r31)
0x82586D40:  clrlwi       r9, r28, 0x10
0x82586D44:  mr           r3, r26
0x82586D48:  rlwimi       r27, r9, 8, 0, 0x17
0x82586D4C:  rlwimi       r30, r27, 8, 0, 0x17
0x82586D50:  stw          r30, 0x10(r31)
0x82586D54:  bl           0x82498070
0x82586D58:  clrlwi       r11, r3, 0x18
0x82586D5C:  lwz          r9, 0x1c(r31)
0x82586D60:  lwz          r6, 0x20(r31)
0x82586D64:  srwi         r8, r30, 0x1c
0x82586D68:  srwi         r10, r11, 3
0x82586D6C:  lwz          r5, 0x3c(r31)
0x82586D70:  lwz          r7, 0x34(r31)
0x82586D74:  add          r4, r10, r9
0x82586D78:  subf         r3, r10, r6
0x82586D7C:  srawi        r10, r11, 3
0x82586D80:  stw          r8, 0x34(r31)
0x82586D84:  cmpwi        cr6, r5, 0
0x82586D88:  addze        r9, r10
0x82586D8C:  stw          r4, 0x1c(r31)
0x82586D90:  slwi         r6, r9, 3
0x82586D94:  stw          r3, 0x20(r31)
0x82586D98:  subf         r24, r6, r11
0x82586D9C:  bne          cr6, 0x82586e18
0x82586DA0:  mr           r9, r25
0x82586DA4:  cmpwi        cr6, r23, 2
0x82586DA8:  ble          cr6, 0x82586dbc
0x82586DAC:  rlwinm       r5, r30, 0, 5, 5
0x82586DB0:  cmplwi       cr6, r5, 0
0x82586DB4:  beq          cr6, 0x82586dbc
0x82586DB8:  mr           r9, r22
0x82586DBC:  subf         r10, r7, r8
0x82586DC0:  cmpwi        cr6, r10, 1
0x82586DC4:  beq          cr6, 0x82586dd4
0x82586DC8:  addi         r4, r10, 0x10
0x82586DCC:  cmpwi        cr6, r4, 1
0x82586DD0:  bne          cr6, 0x82586de4
0x82586DD4:  cmpwi        cr6, r9, 0
0x82586DD8:  bne          cr6, 0x82586de4
0x82586DDC:  stw          r25, 0xc(r26)
0x82586DE0:  b            0x82586e1c
0x82586DE4:  lwz          r9, 0x14(r31)
0x82586DE8:  subfic       r7, r11, 0x20
0x82586DEC:  lwz          r10, 0x18(r31)
0x82586DF0:  li           r3, 6
0x82586DF4:  addi         r9, r9, 4
0x82586DF8:  stw          r30, 0x24(r31)
0x82586DFC:  addi         r8, r10, -4
0x82586E00:  stw          r7, 0x28(r31)
0x82586E04:  stw          r9, 0x1c(r31)
0x82586E08:  stw          r8, 0x20(r31)
0x82586E0C:  stw          r22, 0xc(r26)
0x82586E10:  addi         r1, r1, 0xb0
0x82586E14:  b            0x8242f8d0
0x82586E18:  stw          r25, 0x3c(r31)
0x82586E1C:  mr           r4, r24
0x82586E20:  mr           r3, r31
0x82586E24:  bl           0x824b37c0
0x82586E28:  mr           r3, r25
0x82586E2C:  addi         r1, r1, 0xb0
0x82586E30:  b            0x8242f8d0
0x82586E34:  .align 0                       # padding
0x82586E38:  li           r10, 0
0x82586E3C:  b            0x82586bb0
0x82586E40:  xoris        r10, r10, 0xab76
0x82586E44:  xori         r10, r10, 0x38de
0x82586E48:  b            0x82586bb0
0x82586E4C:  .align 0                       # padding
0x82586E50:  .align 0                       # padding
0x82586E54:  .align 0                       # padding
0x82586E58:  .align 0                       # padding
0x82586E5C:  .align 0                       # padding
0x82586E60:  .align 0                       # padding
0x82586E64:  .align 0                       # padding
0x82586E68:  .align 0                       # padding
0x82586E6C:  .align 0                       # padding
0x82586E70:  .align 0                       # padding
0x82586E74:  .align 0                       # padding
0x82586E78:  .align 0                       # padding
0x82586E7C:  .align 0                       # padding
0x82586E80:  .align 0                       # padding
0x82586E84:  .align 0                       # padding
0x82586E88:  .align 0                       # padding
0x82586E8C:  .align 0                       # padding
0x82586E90:  .align 0                       # padding
0x82586E94:  .align 0                       # padding
0x82586E98:  .align 0                       # padding
0x82586E9C:  .align 0                       # padding
0x82586EA0:  .align 0                       # padding
0x82586EA4:  .align 0                       # padding
0x82586EA8:  .align 0                       # padding
0x82586EAC:  .align 0                       # padding
0x82586EB0:  .align 0                       # padding
0x82586EB4:  .align 0                       # padding
0x82586EB8:  .align 0                       # padding
0x82586EBC:  .align 0                       # padding
0x82586EC0:  .align 0                       # padding
0x82586EC4:  .align 0                       # padding
0x82586EC8:  .align 0                       # padding
0x82586ECC:  .align 0                       # padding
0x82586ED0:  .align 0                       # padding
0x82586ED4:  .align 0                       # padding
0x82586ED8:  .align 0                       # padding
0x82586EDC:  .align 0                       # padding
0x82586EE0:  .align 0                       # padding
0x82586EE4:  .align 0                       # padding
0x82586EE8:  .align 0                       # padding
0x82586EEC:  .align 0                       # padding
0x82586EF0:  .align 0                       # padding
0x82586EF4:  .align 0                       # padding
0x82586EF8:  .align 0                       # padding
0x82586EFC:  .align 0                       # padding
0x82586F00:  .align 0                       # padding
0x82586F04:  .align 0                       # padding
0x82586F08:  .align 0                       # padding
0x82586F0C:  .align 0                       # padding
0x82586F10:  .align 0                       # padding
0x82586F14:  .align 0                       # padding
0x82586F18:  .align 0                       # padding
0x82586F1C:  .align 0                       # padding
0x82586F20:  .align 0                       # padding
0x82586F24:  .align 0                       # padding
0x82586F28:  .align 0                       # padding
0x82586F2C:  .align 0                       # padding
0x82586F30:  .align 0                       # padding
0x82586F34:  .align 0                       # padding
0x82586F38:  .align 0                       # padding
0x82586F3C:  .align 0                       # padding
0x82586F40:  .align 0                       # padding
0x82586F44:  .align 0                       # padding
0x82586F48:  .align 0                       # padding
0x82586F4C:  .align 0                       # padding
0x82586F50:  .align 0                       # padding
0x82586F54:  .align 0                       # padding
0x82586F58:  .align 0                       # padding
0x82586F5C:  .align 0                       # padding
0x82586F60:  .align 0                       # padding
0x82586F64:  .align 0                       # padding
0x82586F68:  .align 0                       # padding
0x82586F6C:  .align 0                       # padding
0x82586F70:  .align 0                       # padding
0x82586F74:  .align 0                       # padding
0x82586F78:  .align 0                       # padding
0x82586F7C:  .align 0                       # padding
0x82586F80:  .align 0                       # padding
0x82586F84:  .align 0                       # padding
0x82586F88:  .align 0                       # padding
0x82586F8C:  .align 0                       # padding
0x82586F90:  .align 0                       # padding
0x82586F94:  .align 0                       # padding
0x82586F98:  .align 0                       # padding
0x82586F9C:  .align 0                       # padding
0x82586FA0:  .align 0                       # padding
0x82586FA4:  .align 0                       # padding
0x82586FA8:  .align 0                       # padding
0x82586FAC:  .align 0                       # padding
0x82586FB0:  .align 0                       # padding
0x82586FB4:  .align 0                       # padding
0x82586FB8:  .align 0                       # padding
0x82586FBC:  .align 0                       # padding
0x82586FC0:  .align 0                       # padding
0x82586FC4:  .align 0                       # padding
0x82586FC8:  .align 0                       # padding
0x82586FCC:  .align 0                       # padding
0x82586FD0:  .align 0                       # padding
0x82586FD4:  .align 0                       # padding
0x82586FD8:  .align 0                       # padding
0x82586FDC:  .align 0                       # padding
0x82586FE0:  .align 0                       # padding
0x82586FE4:  .align 0                       # padding
0x82586FE8:  .align 0                       # padding
0x82586FEC:  .align 0                       # padding
0x82586FF0:  .align 0                       # padding
0x82586FF4:  .align 0                       # padding
0x82586FF8:  .align 0                       # padding
0x82586FFC:  .align 0                       # padding
