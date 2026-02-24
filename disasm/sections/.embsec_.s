# Section: .embsec_
# Base Address: 0x8259A200
# Size: 0x000021E8

.section .embsec_

0x8259A200:  mflr         r12
0x8259A204:  bl           0x8242f860
0x8259A208:  stwu         r1, -0x110(r1)
0x8259A20C:  mr           r29, r3
0x8259A210:  li           r25, 1
0x8259A214:  addi         r21, r29, 0x41c0
0x8259A218:  li           r14, 0
0x8259A21C:  lwz          r11, 0xb50(r29)
0x8259A220:  lwz          r10, 0xb30(r29)
0x8259A224:  addi         r9, r11, 0x2ce
0x8259A228:  stw          r29, 0x124(r1)
0x8259A22C:  slwi         r8, r9, 2
0x8259A230:  lwzx         r7, r8, r29
0x8259A234:  stw          r7, 0(r10)
0x8259A238:  lwz          r11, 0xb50(r29)
0x8259A23C:  lwz          r6, 0xb34(r29)
0x8259A240:  addi         r5, r11, 0x2d1
0x8259A244:  slwi         r4, r5, 2
0x8259A248:  lwzx         r3, r4, r29
0x8259A24C:  stw          r3, 0(r6)
0x8259A250:  lwz          r11, 0x818(r29)
0x8259A254:  lwz          r9, 0xf4c(r29)
0x8259A258:  slwi         r10, r11, 3
0x8259A25C:  addi         r11, r11, 0x105
0x8259A260:  add          r10, r10, r29
0x8259A264:  slwi         r8, r11, 3
0x8259A268:  cmpwi        cr6, r9, 3
0x8259A26C:  lwz          r7, 0x824(r10)
0x8259A270:  stw          r7, 0x81c(r29)
0x8259A274:  lwzx         r6, r8, r29
0x8259A278:  stw          r6, 0x820(r29)
0x8259A27C:  bne          cr6, 0x8259a288
0x8259A280:  stw          r14, 0x1bc(r29)
0x8259A284:  b            0x8259a28c
0x8259A288:  stw          r25, 0x1bc(r29)
0x8259A28C:  lwz          r5, 0x3988(r29)
0x8259A290:  lwz          r4, 0xd20(r29)
0x8259A294:  mullw        r3, r5, r4
0x8259A298:  rlwinm       r11, r3, 0, 0, 0x18
0x8259A29C:  cmpwi        cr6, r11, 0
0x8259A2A0:  beq          cr6, 0x8259a2b8
0x8259A2A4:  li           r10, 3
0x8259A2A8:  li           r8, 4
0x8259A2AC:  stw          r10, 0x398c(r29)
0x8259A2B0:  stw          r8, 0x3990(r29)
0x8259A2B4:  b            0x8259a2c8
0x8259A2B8:  li           r7, 4
0x8259A2BC:  li           r6, 3
0x8259A2C0:  stw          r7, 0x398c(r29)
0x8259A2C4:  stw          r6, 0x3990(r29)
0x8259A2C8:  cmpwi        cr6, r9, 2
0x8259A2CC:  beq          cr6, 0x8259a2dc
0x8259A2D0:  cmpwi        cr6, r9, 3
0x8259A2D4:  mr           r11, r14
0x8259A2D8:  bne          cr6, 0x8259a2e0
0x8259A2DC:  mr           r11, r25
0x8259A2E0:  lwz          r4, 0x7a4(r29)
0x8259A2E4:  li           r5, 1
0x8259A2E8:  stw          r11, 0x4c(r4)
0x8259A2EC:  lwz          r4, 0xf4(r29)
0x8259A2F0:  lwz          r3, 0x7a4(r29)
0x8259A2F4:  bl           0x8255c150
0x8259A2F8:  lwz          r11, 0xf4(r29)
0x8259A2FC:  cmpwi        cr6, r11, 5
0x8259A300:  bge          cr6, 0x8259a320
0x8259A304:  addi         r3, r29, 0x990
0x8259A308:  addi         r11, r29, 0x9a0
0x8259A30C:  addi         r10, r29, 0x9c8
0x8259A310:  stw          r3, 0x99c(r29)
0x8259A314:  stw          r11, 0x9c4(r29)
0x8259A318:  stw          r10, 0x9ec(r29)
0x8259A31C:  b            0x8259a35c
0x8259A320:  cmpwi        cr6, r11, 0xd
0x8259A324:  bge          cr6, 0x8259a344
0x8259A328:  addi         r9, r29, 0x984
0x8259A32C:  addi         r8, r29, 0x9ac
0x8259A330:  addi         r7, r29, 0x9d4
0x8259A334:  stw          r9, 0x99c(r29)
0x8259A338:  stw          r8, 0x9c4(r29)
0x8259A33C:  stw          r7, 0x9ec(r29)
0x8259A340:  b            0x8259a35c
0x8259A344:  addi         r6, r29, 0x978
0x8259A348:  addi         r5, r29, 0x9b8
0x8259A34C:  addi         r4, r29, 0x9e0
0x8259A350:  stw          r6, 0x99c(r29)
0x8259A354:  stw          r5, 0x9c4(r29)
0x8259A358:  stw          r4, 0x9ec(r29)
0x8259A35C:  mr           r3, r29
0x8259A360:  lwz          r16, 0x108(r29)
0x8259A364:  bl           0x8250e7c8
0x8259A368:  mr           r3, r29
0x8259A36C:  bl           0x82547cc0
0x8259A370:  mr           r27, r14
0x8259A374:  mr           r26, r14
0x8259A378:  lwz          r3, 0x88(r29)
0x8259A37C:  mr           r28, r14
0x8259A380:  stw          r14, 0x50(r1)
0x8259A384:  stw          r14, 0x5c(r1)
0x8259A388:  cmpwi        cr6, r3, 0
0x8259A38C:  stw          r27, 0x60(r1)
0x8259A390:  stw          r26, 0x64(r1)
0x8259A394:  stw          r28, 0x58(r1)
0x8259A398:  ble          cr6, 0x8259c3bc
0x8259A39C:  lis          r11, -0x7da5
0x8259A3A0:  lis          r10, 0
0x8259A3A4:  addi         r11, r11, 0x5010
0x8259A3A8:  ori          r15, r10, 0x8000
0x8259A3AC:  lis          r17, 1
0x8259A3B0:  stw          r11, 0x68(r1)
0x8259A3B4:  lwz          r9, 0x4c58(r29)
0x8259A3B8:  stw          r27, 0x42ac(r29)
0x8259A3BC:  cmpwi        cr6, r9, 0
0x8259A3C0:  stw          r26, 0x42b0(r29)
0x8259A3C4:  sth          r14, 0x42b6(r29)
0x8259A3C8:  beq          cr6, 0x8259a4a4
0x8259A3CC:  lwz          r8, 0x4c74(r29)
0x8259A3D0:  slwi         r7, r28, 2
0x8259A3D4:  lwzx         r6, r8, r7
0x8259A3D8:  cmpwi        cr6, r6, 0
0x8259A3DC:  beq          cr6, 0x8259a4a4
0x8259A3E0:  lwz          r11, 0x4c7c(r29)
0x8259A3E4:  lwz          r31, 0x50(r29)
0x8259A3E8:  addi         r5, r11, 1
0x8259A3EC:  stw          r5, 0x4c7c(r29)
0x8259A3F0:  lwz          r4, 0x1c(r31)
0x8259A3F4:  cmpwi        cr6, r4, 0
0x8259A3F8:  beq          cr6, 0x8259a478
0x8259A3FC:  lwz          r10, 8(r31)
0x8259A400:  mr           r30, r25
0x8259A404:  addi         r11, r10, 0x10
0x8259A408:  cmplwi       cr6, r11, 1
0x8259A40C:  bge          cr6, 0x8259a450
0x8259A410:  cmplwi       cr6, r11, 0
0x8259A414:  beq          cr6, 0x8259a450
0x8259A418:  ld           r3, 0(r31)
0x8259A41C:  clrldi       r9, r11, 0x20
0x8259A420:  subf.        r10, r11, r10
0x8259A424:  subf         r30, r11, r30
0x8259A428:  stw          r10, 8(r31)
0x8259A42C:  sld          r8, r3, r9
0x8259A430:  std          r8, 0(r31)
0x8259A434:  bge          0x8259a440
0x8259A438:  mr           r3, r31
0x8259A43C:  bl           0x824d2748
0x8259A440:  lwz          r10, 8(r31)
0x8259A444:  addi         r11, r10, 0x10
0x8259A448:  cmplw        cr6, r30, r11
0x8259A44C:  bgt          cr6, 0x8259a410
0x8259A450:  ld           r5, 0(r31)
0x8259A454:  clrldi       r6, r30, 0x20
0x8259A458:  lwz          r7, 8(r31)
0x8259A45C:  subf.        r11, r30, r7
0x8259A460:  stw          r11, 8(r31)
0x8259A464:  sld          r4, r5, r6
0x8259A468:  std          r4, 0(r31)
0x8259A46C:  bge          0x8259a478
0x8259A470:  mr           r3, r31
0x8259A474:  bl           0x824d2748
0x8259A478:  lwz          r11, 8(r31)
0x8259A47C:  mr           r3, r31
0x8259A480:  clrlwi       r4, r11, 0x1d
0x8259A484:  bl           0x824d2818
0x8259A488:  mr           r4, r28
0x8259A48C:  mr           r3, r29
0x8259A490:  bl           0x824f6e20
0x8259A494:  stw          r25, 0x788(r29)
0x8259A498:  stb          r25, 0x303(r21)
0x8259A49C:  cmpwi        cr6, r3, 0
0x8259A4A0:  bne          cr6, 0x8259c3e0
0x8259A4A4:  lwz          r10, 0x84(r29)
0x8259A4A8:  stw          r14, 0x54(r1)
0x8259A4AC:  cmpwi        cr6, r10, 0
0x8259A4B0:  ble          cr6, 0x8259c344
0x8259A4B4:  lwz          r9, 0(r21)
0x8259A4B8:  li           r25, 1
0x8259A4BC:  li           r8, 0x80
0x8259A4C0:  lwz          r7, 0xc(r9)
0x8259A4C4:  dcbt         r8, r7
0x8259A4C8:  lis          r12, -0x10c1
0x8259A4CC:  lwz          r6, 0(r16)
0x8259A4D0:  mr           r18, r14
0x8259A4D4:  ori          r12, r12, 0xf8ff
0x8259A4D8:  mr           r22, r14
0x8259A4DC:  and          r5, r6, r12
0x8259A4E0:  mr           r30, r14
0x8259A4E4:  mr           r20, r14
0x8259A4E8:  mr           r19, r14
0x8259A4EC:  stw          r5, 0(r16)
0x8259A4F0:  lbz          r4, 0x10(r21)
0x8259A4F4:  stb          r4, 4(r16)
0x8259A4F8:  lwz          r3, 0x54c(r21)
0x8259A4FC:  cmpwi        cr6, r3, 0
0x8259A500:  bne          cr6, 0x8259a53c
0x8259A504:  lwz          r3, 0(r21)
0x8259A508:  ld           r11, 0(r3)
0x8259A50C:  lwz          r10, 8(r3)
0x8259A510:  sldi         r9, r11, 1
0x8259A514:  rldicl       r8, r11, 1, 0x3f
0x8259A518:  addic.       r11, r10, -1
0x8259A51C:  slwi         r31, r8, 0
0x8259A520:  std          r9, 0(r3)
0x8259A524:  stw          r11, 8(r3)
0x8259A528:  bge          0x8259a530
0x8259A52C:  bl           0x824d2748
0x8259A530:  lwz          r7, 0(r16)
0x8259A534:  rlwimi       r7, r31, 5, 0x18, 0x1a
0x8259A538:  stw          r7, 0(r16)
0x8259A53C:  lbz          r6, 0x12(r21)
0x8259A540:  cmplwi       cr6, r6, 0
0x8259A544:  bne          cr6, 0x8259a580
0x8259A548:  lwz          r3, 0(r21)
0x8259A54C:  ld           r11, 0(r3)
0x8259A550:  lwz          r5, 8(r3)
0x8259A554:  sldi         r4, r11, 1
0x8259A558:  rldicl       r10, r11, 1, 0x3f
0x8259A55C:  addic.       r11, r5, -1
0x8259A560:  slwi         r31, r10, 0
0x8259A564:  std          r4, 0(r3)
0x8259A568:  stw          r11, 8(r3)
0x8259A56C:  bge          0x8259a574
0x8259A570:  bl           0x824d2748
0x8259A574:  lwz          r9, 0(r16)
0x8259A578:  rlwimi       r9, r31, 0x1f, 0, 0
0x8259A57C:  stw          r9, 0(r16)
0x8259A580:  lwz          r11, 0(r16)
0x8259A584:  rlwinm       r8, r11, 0, 0x18, 0x1a
0x8259A588:  cmplwi       cr6, r8, 0x20
0x8259A58C:  bne          cr6, 0x8259a598
0x8259A590:  mr           r30, r25
0x8259A594:  b            0x8259aa5c
0x8259A598:  rlwinm       r7, r11, 0, 0, 0
0x8259A59C:  cmplwi       cr6, r7, 0
0x8259A5A0:  bne          cr6, 0x8259a998
0x8259A5A4:  lwz          r31, 0(r21)
0x8259A5A8:  lwz          r6, 0x64(r21)
0x8259A5AC:  ld           r10, 0(r31)
0x8259A5B0:  lwz          r28, 0(r6)
0x8259A5B4:  rldicl       r5, r10, 0xa, 0x36
0x8259A5B8:  slwi         r4, r5, 1
0x8259A5BC:  lhzx         r3, r4, r28
0x8259A5C0:  extsh        r30, r3
0x8259A5C4:  cmpwi        cr6, r30, 0
0x8259A5C8:  blt          cr6, 0x8259a68c
0x8259A5CC:  clrlwi       r11, r30, 0x1c
0x8259A5D0:  lwz          r9, 8(r31)
0x8259A5D4:  mr           r8, r11
0x8259A5D8:  subf.        r11, r11, r9
0x8259A5DC:  stw          r11, 8(r31)
0x8259A5E0:  sld          r7, r10, r8
0x8259A5E4:  std          r7, 0(r31)
0x8259A5E8:  bge          0x8259a684
0x8259A5EC:  lwz          r10, 0x10(r31)
0x8259A5F0:  lwz          r11, 0xc(r31)
0x8259A5F4:  addi         r6, r10, -4
0x8259A5F8:  cmplw        cr6, r11, r6
0x8259A5FC:  blt          cr6, 0x8259a618
0x8259A600:  mr           r3, r31
0x8259A604:  bl           0x824d2688
0x8259A608:  cmplwi       cr6, r3, 1
0x8259A60C:  beq          cr6, 0x8259a5ec
0x8259A610:  srawi        r30, r30, 4
0x8259A614:  b            0x8259a6cc
0x8259A618:  lbz          r3, 0(r11)
0x8259A61C:  addi         r4, r11, 6
0x8259A620:  lbz          r9, 1(r11)
0x8259A624:  rldicr       r8, r3, 8, 0x3f
0x8259A628:  lbz          r6, 3(r11)
0x8259A62C:  lbz          r7, 4(r11)
0x8259A630:  add          r9, r9, r8
0x8259A634:  lbz          r5, 2(r11)
0x8259A638:  lbz          r8, 5(r11)
0x8259A63C:  sldi         r11, r9, 8
0x8259A640:  stw          r4, 0xc(r31)
0x8259A644:  lwz          r10, 8(r31)
0x8259A648:  add          r5, r11, r5
0x8259A64C:  ld           r9, 0(r31)
0x8259A650:  sldi         r11, r5, 8
0x8259A654:  add          r4, r11, r6
0x8259A658:  sldi         r11, r4, 8
0x8259A65C:  add          r3, r11, r7
0x8259A660:  addi         r7, r10, 0x30
0x8259A664:  sldi         r11, r3, 8
0x8259A668:  add          r11, r11, r8
0x8259A66C:  neg          r8, r10
0x8259A670:  stw          r7, 8(r31)
0x8259A674:  extsw        r6, r8
0x8259A678:  sld          r11, r11, r6
0x8259A67C:  add          r5, r11, r9
0x8259A680:  std          r5, 0(r31)
0x8259A684:  srawi        r30, r30, 4
0x8259A688:  b            0x8259a6cc
0x8259A68C:  li           r4, 0xa
0x8259A690:  mr           r3, r31
0x8259A694:  bl           0x824d2818
0x8259A698:  ld           r11, 0(r31)
0x8259A69C:  li           r4, 1
0x8259A6A0:  mr           r3, r31
0x8259A6A4:  rldicl       r10, r11, 1, 0x3f
0x8259A6A8:  slwi         r29, r10, 0
0x8259A6AC:  bl           0x824d2818
0x8259A6B0:  add          r9, r29, r30
0x8259A6B4:  add          r8, r9, r15
0x8259A6B8:  slwi         r7, r8, 1
0x8259A6BC:  lhzx         r6, r7, r28
0x8259A6C0:  extsh        r30, r6
0x8259A6C4:  cmpwi        cr6, r30, 0
0x8259A6C8:  blt          cr6, 0x8259a698
0x8259A6CC:  addi         r11, r30, 1
0x8259A6D0:  mr           r10, r25
0x8259A6D4:  cmpwi        cr6, r11, 0x25
0x8259A6D8:  bge          cr6, 0x8259a6e0
0x8259A6DC:  mr           r10, r14
0x8259A6E0:  extsw        r23, r10
0x8259A6E4:  cmpwi        cr6, r10, 0
0x8259A6E8:  beq          cr6, 0x8259a6f0
0x8259A6EC:  addi         r11, r11, -0x25
0x8259A6F0:  mr           r24, r14
0x8259A6F4:  cmpwi        cr6, r11, 0
0x8259A6F8:  bne          cr6, 0x8259a704
0x8259A6FC:  mr           r11, r14
0x8259A700:  b            0x8259a970
0x8259A704:  cmpwi        cr6, r11, 0x23
0x8259A708:  bge          cr6, 0x8259a85c
0x8259A70C:  lwz          r5, 0xc(r21)
0x8259A710:  slwi         r4, r11, 2
0x8259A714:  lwzx         r11, r4, r5
0x8259A718:  srawi        r3, r11, 0x18
0x8259A71C:  srawi        r9, r11, 4
0x8259A720:  clrlwi       r10, r11, 0x1c
0x8259A724:  clrlwi       r28, r9, 0x1c
0x8259A728:  srawi        r8, r11, 8
0x8259A72C:  srawi        r7, r11, 0x10
0x8259A730:  add          r30, r10, r28
0x8259A734:  clrlwi       r27, r3, 0x18
0x8259A738:  clrlwi       r26, r8, 0x18
0x8259A73C:  clrlwi       r25, r7, 0x18
0x8259A740:  cmpwi        cr6, r30, 0
0x8259A744:  ble          cr6, 0x8259a800
0x8259A748:  lwz          r9, 8(r31)
0x8259A74C:  mr           r29, r14
0x8259A750:  cmplwi       cr6, r30, 0
0x8259A754:  addi         r11, r9, 0x10
0x8259A758:  beq          cr6, 0x8259a800
0x8259A75C:  cmplw        cr6, r30, r11
0x8259A760:  ble          cr6, 0x8259a7bc
0x8259A764:  cmplwi       cr6, r11, 0
0x8259A768:  beq          cr6, 0x8259a7bc
0x8259A76C:  subfic       r6, r11, 0x40
0x8259A770:  ld           r10, 0(r31)
0x8259A774:  clrldi       r5, r11, 0x20
0x8259A778:  clrldi       r4, r6, 0x20
0x8259A77C:  subf.        r9, r11, r9
0x8259A780:  subf         r30, r11, r30
0x8259A784:  stw          r9, 8(r31)
0x8259A788:  sld          r11, r10, r5
0x8259A78C:  srd          r3, r10, r4
0x8259A790:  slwi         r10, r3, 0
0x8259A794:  std          r11, 0(r31)
0x8259A798:  slw          r11, r10, r30
0x8259A79C:  add          r29, r11, r29
0x8259A7A0:  bge          0x8259a7ac
0x8259A7A4:  mr           r3, r31
0x8259A7A8:  bl           0x824d2748
0x8259A7AC:  lwz          r9, 8(r31)
0x8259A7B0:  addi         r11, r9, 0x10
0x8259A7B4:  cmplw        cr6, r30, r11
0x8259A7B8:  bgt          cr6, 0x8259a764
0x8259A7BC:  subfic       r8, r30, 0x40
0x8259A7C0:  ld           r11, 0(r31)
0x8259A7C4:  clrldi       r7, r30, 0x20
0x8259A7C8:  lwz          r9, 8(r31)
0x8259A7CC:  clrldi       r6, r8, 0x20
0x8259A7D0:  subf.        r10, r30, r9
0x8259A7D4:  stw          r10, 8(r31)
0x8259A7D8:  sld          r4, r11, r7
0x8259A7DC:  srd          r5, r11, r6
0x8259A7E0:  slwi         r11, r5, 0
0x8259A7E4:  add          r30, r11, r29
0x8259A7E8:  std          r4, 0(r31)
0x8259A7EC:  bge          0x8259a7f8
0x8259A7F0:  mr           r3, r31
0x8259A7F4:  bl           0x824d2748
0x8259A7F8:  mr           r10, r30
0x8259A7FC:  b            0x8259a804
0x8259A800:  mr           r10, r14
0x8259A804:  and          r11, r10, r27
0x8259A808:  slwi         r8, r25, 1
0x8259A80C:  rlwinm       r9, r11, 0, 0, 0x1e
0x8259A810:  sraw         r10, r10, r28
0x8259A814:  add          r9, r9, r8
0x8259A818:  clrlwi       r7, r10, 0x1f
0x8259A81C:  clrlwi       r3, r11, 0x1f
0x8259A820:  srawi        r10, r10, 1
0x8259A824:  slwi         r8, r9, 0xf
0x8259A828:  neg          r11, r3
0x8259A82C:  neg          r9, r7
0x8259A830:  add          r6, r10, r26
0x8259A834:  slwi         r5, r11, 0x10
0x8259A838:  xor          r10, r6, r9
0x8259A83C:  slwi         r4, r11, 0x10
0x8259A840:  xor          r3, r8, r5
0x8259A844:  subf         r9, r9, r10
0x8259A848:  subf         r11, r4, r3
0x8259A84C:  clrlwi       r8, r9, 0x10
0x8259A850:  li           r25, 1
0x8259A854:  or           r11, r11, r8
0x8259A858:  b            0x8259a970
0x8259A85C:  cmpwi        cr6, r11, 0x24
0x8259A860:  bne          cr6, 0x8259a870
0x8259A864:  mr           r24, r25
0x8259A868:  mr           r11, r14
0x8259A86C:  b            0x8259a970
0x8259A870:  lbz          r11, 0x16(r21)
0x8259A874:  mr           r29, r14
0x8259A878:  lhz          r7, 0x3e(r21)
0x8259A87C:  lhz          r6, 0x40(r21)
0x8259A880:  subf         r27, r11, r7
0x8259A884:  lwz          r10, 8(r31)
0x8259A888:  subf         r28, r11, r6
0x8259A88C:  addi         r11, r10, 0x10
0x8259A890:  add          r30, r28, r27
0x8259A894:  cmplwi       cr6, r30, 0
0x8259A898:  bne          cr6, 0x8259a8a4
0x8259A89C:  mr           r11, r14
0x8259A8A0:  b            0x8259a944
0x8259A8A4:  cmplw        cr6, r30, r11
0x8259A8A8:  ble          cr6, 0x8259a904
0x8259A8AC:  cmplwi       cr6, r11, 0
0x8259A8B0:  beq          cr6, 0x8259a904
0x8259A8B4:  subfic       r5, r11, 0x40
0x8259A8B8:  ld           r9, 0(r31)
0x8259A8BC:  subf.        r10, r11, r10
0x8259A8C0:  clrldi       r3, r5, 0x20
0x8259A8C4:  clrldi       r4, r11, 0x20
0x8259A8C8:  subf         r30, r11, r30
0x8259A8CC:  stw          r10, 8(r31)
0x8259A8D0:  srd          r11, r9, r3
0x8259A8D4:  sld          r9, r9, r4
0x8259A8D8:  slwi         r8, r11, 0
0x8259A8DC:  std          r9, 0(r31)
0x8259A8E0:  slw          r11, r8, r30
0x8259A8E4:  add          r29, r11, r29
0x8259A8E8:  bge          0x8259a8f4
0x8259A8EC:  mr           r3, r31
0x8259A8F0:  bl           0x824d2748
0x8259A8F4:  lwz          r10, 8(r31)
0x8259A8F8:  addi         r11, r10, 0x10
0x8259A8FC:  cmplw        cr6, r30, r11
0x8259A900:  bgt          cr6, 0x8259a8ac
0x8259A904:  subfic       r6, r30, 0x40
0x8259A908:  ld           r11, 0(r31)
0x8259A90C:  clrldi       r5, r30, 0x20
0x8259A910:  lwz          r7, 8(r31)
0x8259A914:  clrldi       r4, r6, 0x20
0x8259A918:  subf.        r10, r30, r7
0x8259A91C:  stw          r10, 8(r31)
0x8259A920:  sld          r9, r11, r5
0x8259A924:  srd          r3, r11, r4
0x8259A928:  slwi         r11, r3, 0
0x8259A92C:  add          r30, r11, r29
0x8259A930:  std          r9, 0(r31)
0x8259A934:  bge          0x8259a940
0x8259A938:  mr           r3, r31
0x8259A93C:  bl           0x824d2748
0x8259A940:  mr           r11, r30
0x8259A944:  slw          r10, r25, r27
0x8259A948:  slw          r7, r25, r28
0x8259A94C:  sraw         r5, r11, r28
0x8259A950:  addi         r4, r10, -1
0x8259A954:  slwi         r6, r7, 0x10
0x8259A958:  slwi         r8, r11, 0x10
0x8259A95C:  and          r11, r5, r4
0x8259A960:  subf         r3, r17, r6
0x8259A964:  clrlwi       r9, r11, 0x10
0x8259A968:  and          r10, r3, r8
0x8259A96C:  or           r11, r10, r9
0x8259A970:  sldi         r8, r23, 8
0x8259A974:  clrldi       r7, r11, 0x20
0x8259A978:  or           r6, r8, r24
0x8259A97C:  sldi         r5, r6, 0x20
0x8259A980:  or           r11, r5, r7
0x8259A984:  rldicl       r4, r11, 0x18, 0x28
0x8259A988:  rldicl       r3, r11, 0x20, 0x20
0x8259A98C:  clrlwi       r30, r4, 0x1f
0x8259A990:  clrlwi       r22, r3, 0x1f
0x8259A994:  slwi         r20, r11, 0
0x8259A998:  lwz          r11, 0(r16)
0x8259A99C:  rlwinm       r10, r11, 0, 0x18, 0x1a
0x8259A9A0:  cmplwi       cr6, r10, 0
0x8259A9A4:  bne          cr6, 0x8259aa5c
0x8259A9A8:  cmpwi        cr6, r22, 0
0x8259A9AC:  beq          cr6, 0x8259a9c0
0x8259A9B0:  li           r9, 3
0x8259A9B4:  rlwimi       r11, r9, 5, 0x18, 0x1a
0x8259A9B8:  stw          r11, 0(r16)
0x8259A9BC:  b            0x8259aa5c
0x8259A9C0:  lwz          r3, 0(r21)
0x8259A9C4:  ld           r11, 0(r3)
0x8259A9C8:  lwz          r8, 8(r3)
0x8259A9CC:  sldi         r7, r11, 1
0x8259A9D0:  rldicl       r6, r11, 1, 0x3f
0x8259A9D4:  addic.       r11, r8, -1
0x8259A9D8:  slwi         r31, r6, 0
0x8259A9DC:  std          r7, 0(r3)
0x8259A9E0:  stw          r11, 8(r3)
0x8259A9E4:  bge          0x8259a9ec
0x8259A9E8:  bl           0x824d2748
0x8259A9EC:  cmplwi       cr6, r31, 0
0x8259A9F0:  bne          cr6, 0x8259aa08
0x8259A9F4:  lwz          r5, 0x550(r21)
0x8259A9F8:  lwz          r4, 0(r16)
0x8259A9FC:  rlwimi       r4, r5, 5, 0x18, 0x1a
0x8259AA00:  stw          r4, 0(r16)
0x8259AA04:  b            0x8259aa5c
0x8259AA08:  lwz          r3, 0(r21)
0x8259AA0C:  ld           r11, 0(r3)
0x8259AA10:  lwz          r10, 8(r3)
0x8259AA14:  sldi         r9, r11, 1
0x8259AA18:  rldicl       r8, r11, 1, 0x3f
0x8259AA1C:  addic.       r11, r10, -1
0x8259AA20:  slwi         r31, r8, 0
0x8259AA24:  std          r9, 0(r3)
0x8259AA28:  stw          r11, 8(r3)
0x8259AA2C:  bge          0x8259aa34
0x8259AA30:  bl           0x824d2748
0x8259AA34:  cmplwi       cr6, r31, 0
0x8259AA38:  bne          cr6, 0x8259aa50
0x8259AA3C:  lwz          r7, 0x554(r21)
0x8259AA40:  lwz          r6, 0(r16)
0x8259AA44:  rlwimi       r6, r7, 5, 0x18, 0x1a
0x8259AA48:  stw          r6, 0(r16)
0x8259AA4C:  b            0x8259aa5c
0x8259AA50:  lwz          r5, 0(r16)
0x8259AA54:  rlwimi       r5, r25, 6, 0x18, 0x1a
0x8259AA58:  stw          r5, 0(r16)
0x8259AA5C:  lwz          r11, 0(r16)
0x8259AA60:  rlwinm       r4, r11, 0, 0, 0
0x8259AA64:  cmplwi       cr6, r4, 0
0x8259AA68:  beq          cr6, 0x8259aa74
0x8259AA6C:  stb          r14, 5(r16)
0x8259AA70:  b            0x8259b894
0x8259AA74:  cmpwi        cr6, r30, 0
0x8259AA78:  bne          cr6, 0x8259adac
0x8259AA7C:  oris         r11, r11, 0x4000
0x8259AA80:  stw          r11, 0(r16)
0x8259AA84:  lbz          r3, 0x13(r21)
0x8259AA88:  cmplwi       cr6, r3, 0
0x8259AA8C:  beq          cr6, 0x8259ad64
0x8259AA90:  cmpwi        cr6, r22, 0
0x8259AA94:  beq          cr6, 0x8259b890
0x8259AA98:  lbz          r10, 0x2fd(r21)
0x8259AA9C:  cmplwi       cr6, r10, 0
0x8259AAA0:  beq          cr6, 0x8259aae8
0x8259AAA4:  srwi         r11, r11, 0xc
0x8259AAA8:  and          r10, r11, r10
0x8259AAAC:  clrlwi       r9, r10, 0x1c
0x8259AAB0:  cmplwi       cr6, r9, 0
0x8259AAB4:  beq          cr6, 0x8259aacc
0x8259AAB8:  lbz          r8, 0x2fe(r21)
0x8259AABC:  rotlwi       r11, r8, 1
0x8259AAC0:  addi         r7, r11, 0xff
0x8259AAC4:  stb          r7, 4(r16)
0x8259AAC8:  b            0x8259ad50
0x8259AACC:  lbz          r5, 0x2fc(r21)
0x8259AAD0:  lbz          r10, 0x301(r21)
0x8259AAD4:  rotlwi       r11, r5, 1
0x8259AAD8:  add          r11, r11, r10
0x8259AADC:  addi         r4, r11, 0xff
0x8259AAE0:  stb          r4, 4(r16)
0x8259AAE4:  b            0x8259ad50
0x8259AAE8:  lwz          r31, 0(r21)
0x8259AAEC:  mr           r29, r14
0x8259AAF0:  lbz          r11, 0x302(r21)
0x8259AAF4:  cmplwi       cr6, r11, 0
0x8259AAF8:  lwz          r9, 8(r31)
0x8259AAFC:  addi         r11, r9, 0x10
0x8259AB00:  beq          cr6, 0x8259abdc
0x8259AB04:  mr           r30, r25
0x8259AB08:  cmplwi       cr6, r11, 1
0x8259AB0C:  bge          cr6, 0x8259ab68
0x8259AB10:  cmplwi       cr6, r11, 0
0x8259AB14:  beq          cr6, 0x8259ab68
0x8259AB18:  subfic       r8, r11, 0x40
0x8259AB1C:  ld           r10, 0(r31)
0x8259AB20:  clrldi       r7, r11, 0x20
0x8259AB24:  clrldi       r6, r8, 0x20
0x8259AB28:  subf         r30, r11, r30
0x8259AB2C:  subf.        r9, r11, r9
0x8259AB30:  stw          r9, 8(r31)
0x8259AB34:  sld          r4, r10, r7
0x8259AB38:  srd          r5, r10, r6
0x8259AB3C:  slwi         r3, r5, 0
0x8259AB40:  std          r4, 0(r31)
0x8259AB44:  slw          r11, r3, r30
0x8259AB48:  add          r29, r11, r29
0x8259AB4C:  bge          0x8259ab58
0x8259AB50:  mr           r3, r31
0x8259AB54:  bl           0x824d2748
0x8259AB58:  lwz          r9, 8(r31)
0x8259AB5C:  addi         r11, r9, 0x10
0x8259AB60:  cmplw        cr6, r30, r11
0x8259AB64:  bgt          cr6, 0x8259ab10
0x8259AB68:  subfic       r9, r30, 0x40
0x8259AB6C:  ld           r11, 0(r31)
0x8259AB70:  clrldi       r8, r30, 0x20
0x8259AB74:  lwz          r10, 8(r31)
0x8259AB78:  clrldi       r7, r9, 0x20
0x8259AB7C:  subf.        r10, r30, r10
0x8259AB80:  stw          r10, 8(r31)
0x8259AB84:  sld          r5, r11, r8
0x8259AB88:  srd          r6, r11, r7
0x8259AB8C:  slwi         r11, r6, 0
0x8259AB90:  add          r30, r11, r29
0x8259AB94:  std          r5, 0(r31)
0x8259AB98:  bge          0x8259aba4
0x8259AB9C:  mr           r3, r31
0x8259ABA0:  bl           0x824d2748
0x8259ABA4:  cmplwi       cr6, r30, 0
0x8259ABA8:  beq          cr6, 0x8259abc0
0x8259ABAC:  lbz          r4, 0x2fe(r21)
0x8259ABB0:  rotlwi       r11, r4, 1
0x8259ABB4:  addi         r11, r11, -1
0x8259ABB8:  stb          r11, 4(r16)
0x8259ABBC:  b            0x8259ad50
0x8259ABC0:  lbz          r3, 0x2fc(r21)
0x8259ABC4:  lbz          r10, 0x301(r21)
0x8259ABC8:  rotlwi       r11, r3, 1
0x8259ABCC:  add          r11, r11, r10
0x8259ABD0:  addi         r11, r11, -1
0x8259ABD4:  stb          r11, 4(r16)
0x8259ABD8:  b            0x8259ad50
0x8259ABDC:  li           r30, 3
0x8259ABE0:  cmplwi       cr6, r11, 3
0x8259ABE4:  bge          cr6, 0x8259ac40
0x8259ABE8:  cmplwi       cr6, r11, 0
0x8259ABEC:  beq          cr6, 0x8259ac40
0x8259ABF0:  subfic       r8, r11, 0x40
0x8259ABF4:  ld           r10, 0(r31)
0x8259ABF8:  clrldi       r7, r11, 0x20
0x8259ABFC:  clrldi       r6, r8, 0x20
0x8259AC00:  subf         r30, r11, r30
0x8259AC04:  subf.        r9, r11, r9
0x8259AC08:  stw          r9, 8(r31)
0x8259AC0C:  sld          r4, r10, r7
0x8259AC10:  srd          r5, r10, r6
0x8259AC14:  slwi         r3, r5, 0
0x8259AC18:  std          r4, 0(r31)
0x8259AC1C:  slw          r11, r3, r30
0x8259AC20:  add          r29, r11, r29
0x8259AC24:  bge          0x8259ac30
0x8259AC28:  mr           r3, r31
0x8259AC2C:  bl           0x824d2748
0x8259AC30:  lwz          r9, 8(r31)
0x8259AC34:  addi         r11, r9, 0x10
0x8259AC38:  cmplw        cr6, r30, r11
0x8259AC3C:  bgt          cr6, 0x8259abe8
0x8259AC40:  subfic       r9, r30, 0x40
0x8259AC44:  ld           r11, 0(r31)
0x8259AC48:  clrldi       r8, r30, 0x20
0x8259AC4C:  lwz          r10, 8(r31)
0x8259AC50:  clrldi       r7, r9, 0x20
0x8259AC54:  subf.        r10, r30, r10
0x8259AC58:  stw          r10, 8(r31)
0x8259AC5C:  sld          r5, r11, r8
0x8259AC60:  srd          r6, r11, r7
0x8259AC64:  slwi         r11, r6, 0
0x8259AC68:  add          r30, r11, r29
0x8259AC6C:  std          r5, 0(r31)
0x8259AC70:  bge          0x8259ac7c
0x8259AC74:  mr           r3, r31
0x8259AC78:  bl           0x824d2748
0x8259AC7C:  cmpwi        cr6, r30, 7
0x8259AC80:  bne          cr6, 0x8259ad3c
0x8259AC84:  lwz          r31, 0(r21)
0x8259AC88:  li           r30, 5
0x8259AC8C:  mr           r29, r14
0x8259AC90:  lwz          r9, 8(r31)
0x8259AC94:  addi         r11, r9, 0x10
0x8259AC98:  cmplwi       cr6, r11, 5
0x8259AC9C:  bge          cr6, 0x8259acf8
0x8259ACA0:  cmplwi       cr6, r11, 0
0x8259ACA4:  beq          cr6, 0x8259acf8
0x8259ACA8:  subfic       r4, r11, 0x40
0x8259ACAC:  ld           r10, 0(r31)
0x8259ACB0:  subf.        r9, r11, r9
0x8259ACB4:  clrldi       r3, r11, 0x20
0x8259ACB8:  subf         r30, r11, r30
0x8259ACBC:  clrldi       r11, r4, 0x20
0x8259ACC0:  stw          r9, 8(r31)
0x8259ACC4:  sld          r7, r10, r3
0x8259ACC8:  srd          r8, r10, r11
0x8259ACCC:  slwi         r6, r8, 0
0x8259ACD0:  std          r7, 0(r31)
0x8259ACD4:  slw          r11, r6, r30
0x8259ACD8:  add          r29, r11, r29
0x8259ACDC:  bge          0x8259ace8
0x8259ACE0:  mr           r3, r31
0x8259ACE4:  bl           0x824d2748
0x8259ACE8:  lwz          r9, 8(r31)
0x8259ACEC:  addi         r11, r9, 0x10
0x8259ACF0:  cmplw        cr6, r30, r11
0x8259ACF4:  bgt          cr6, 0x8259aca0
0x8259ACF8:  subfic       r4, r30, 0x40
0x8259ACFC:  ld           r11, 0(r31)
0x8259AD00:  clrldi       r3, r30, 0x20
0x8259AD04:  lwz          r5, 8(r31)
0x8259AD08:  clrldi       r9, r4, 0x20
0x8259AD0C:  subf.        r10, r30, r5
0x8259AD10:  stw          r10, 8(r31)
0x8259AD14:  sld          r7, r11, r3
0x8259AD18:  srd          r8, r11, r9
0x8259AD1C:  slwi         r11, r8, 0
0x8259AD20:  add          r30, r11, r29
0x8259AD24:  std          r7, 0(r31)
0x8259AD28:  bge          0x8259ad34
0x8259AD2C:  mr           r3, r31
0x8259AD30:  bl           0x824d2748
0x8259AD34:  mr           r11, r30
0x8259AD38:  b            0x8259ad44
0x8259AD3C:  lbz          r11, 0x2fc(r21)
0x8259AD40:  add          r11, r11, r30
0x8259AD44:  slwi         r11, r11, 1
0x8259AD48:  addi         r6, r11, 0xff
0x8259AD4C:  stb          r6, 4(r16)
0x8259AD50:  lbz          r11, 4(r16)
0x8259AD54:  cmplwi       cr6, r11, 1
0x8259AD58:  blt          cr6, 0x8259b65c
0x8259AD5C:  cmplwi       cr6, r11, 0x3e
0x8259AD60:  bgt          cr6, 0x8259b65c
0x8259AD64:  cmpwi        cr6, r22, 0
0x8259AD68:  beq          cr6, 0x8259b890
0x8259AD6C:  lwz          r3, 0(r21)
0x8259AD70:  ld           r11, 0(r3)
0x8259AD74:  lwz          r4, 8(r3)
0x8259AD78:  sldi         r10, r11, 1
0x8259AD7C:  rldicl       r9, r11, 1, 0x3f
0x8259AD80:  addic.       r11, r4, -1
0x8259AD84:  slwi         r31, r9, 0
0x8259AD88:  std          r10, 0(r3)
0x8259AD8C:  stw          r11, 8(r3)
0x8259AD90:  bge          0x8259ad98
0x8259AD94:  bl           0x824d2748
0x8259AD98:  lwz          r7, 0(r16)
0x8259AD9C:  clrlwi       r6, r31, 0x18
0x8259ADA0:  rlwimi       r7, r6, 3, 0x1b, 0x1c
0x8259ADA4:  stw          r7, 0(r16)
0x8259ADA8:  b            0x8259b890
0x8259ADAC:  rlwinm       r5, r11, 0, 0x18, 0x1a
0x8259ADB0:  cmplwi       cr6, r5, 0x40
0x8259ADB4:  bne          cr6, 0x8259b1d0
0x8259ADB8:  lwz          r31, 0(r21)
0x8259ADBC:  lwz          r4, 0x64(r21)
0x8259ADC0:  ld           r10, 0(r31)
0x8259ADC4:  lwz          r28, 0(r4)
0x8259ADC8:  rldicl       r3, r10, 0xa, 0x36
0x8259ADCC:  slwi         r11, r3, 1
0x8259ADD0:  lhzx         r9, r11, r28
0x8259ADD4:  extsh        r30, r9
0x8259ADD8:  cmpwi        cr6, r30, 0
0x8259ADDC:  blt          cr6, 0x8259aea0
0x8259ADE0:  clrlwi       r11, r30, 0x1c
0x8259ADE4:  lwz          r8, 8(r31)
0x8259ADE8:  mr           r7, r11
0x8259ADEC:  subf.        r11, r11, r8
0x8259ADF0:  stw          r11, 8(r31)
0x8259ADF4:  sld          r6, r10, r7
0x8259ADF8:  std          r6, 0(r31)
0x8259ADFC:  bge          0x8259ae98
0x8259AE00:  lwz          r10, 0x10(r31)
0x8259AE04:  lwz          r11, 0xc(r31)
0x8259AE08:  addi         r5, r10, -4
0x8259AE0C:  cmplw        cr6, r11, r5
0x8259AE10:  blt          cr6, 0x8259ae2c
0x8259AE14:  mr           r3, r31
0x8259AE18:  bl           0x824d2688
0x8259AE1C:  cmplwi       cr6, r3, 1
0x8259AE20:  beq          cr6, 0x8259ae00
0x8259AE24:  srawi        r30, r30, 4
0x8259AE28:  b            0x8259aee0
0x8259AE2C:  lbz          r3, 0(r11)
0x8259AE30:  addi         r4, r11, 6
0x8259AE34:  lbz          r9, 1(r11)
0x8259AE38:  rldicr       r8, r3, 8, 0x3f
0x8259AE3C:  lbz          r6, 3(r11)
0x8259AE40:  lbz          r7, 4(r11)
0x8259AE44:  add          r9, r9, r8
0x8259AE48:  lbz          r5, 2(r11)
0x8259AE4C:  lbz          r8, 5(r11)
0x8259AE50:  sldi         r11, r9, 8
0x8259AE54:  stw          r4, 0xc(r31)
0x8259AE58:  lwz          r10, 8(r31)
0x8259AE5C:  add          r5, r11, r5
0x8259AE60:  ld           r9, 0(r31)
0x8259AE64:  sldi         r11, r5, 8
0x8259AE68:  add          r4, r11, r6
0x8259AE6C:  sldi         r11, r4, 8
0x8259AE70:  add          r3, r11, r7
0x8259AE74:  addi         r7, r10, 0x30
0x8259AE78:  sldi         r11, r3, 8
0x8259AE7C:  add          r11, r11, r8
0x8259AE80:  neg          r8, r10
0x8259AE84:  stw          r7, 8(r31)
0x8259AE88:  extsw        r6, r8
0x8259AE8C:  sld          r11, r11, r6
0x8259AE90:  add          r5, r11, r9
0x8259AE94:  std          r5, 0(r31)
0x8259AE98:  srawi        r30, r30, 4
0x8259AE9C:  b            0x8259aee0
0x8259AEA0:  li           r4, 0xa
0x8259AEA4:  mr           r3, r31
0x8259AEA8:  bl           0x824d2818
0x8259AEAC:  ld           r11, 0(r31)
0x8259AEB0:  li           r4, 1
0x8259AEB4:  mr           r3, r31
0x8259AEB8:  rldicl       r10, r11, 1, 0x3f
0x8259AEBC:  slwi         r29, r10, 0
0x8259AEC0:  bl           0x824d2818
0x8259AEC4:  add          r9, r30, r29
0x8259AEC8:  add          r8, r9, r15
0x8259AECC:  slwi         r7, r8, 1
0x8259AED0:  lhzx         r6, r7, r28
0x8259AED4:  extsh        r30, r6
0x8259AED8:  cmpwi        cr6, r30, 0
0x8259AEDC:  blt          cr6, 0x8259aeac
0x8259AEE0:  addi         r11, r30, 1
0x8259AEE4:  mr           r10, r25
0x8259AEE8:  cmpwi        cr6, r11, 0x25
0x8259AEEC:  bge          cr6, 0x8259aef4
0x8259AEF0:  mr           r10, r14
0x8259AEF4:  extsw        r23, r10
0x8259AEF8:  cmpwi        cr6, r10, 0
0x8259AEFC:  beq          cr6, 0x8259af04
0x8259AF00:  addi         r11, r11, -0x25
0x8259AF04:  mr           r24, r14
0x8259AF08:  cmpwi        cr6, r11, 0
0x8259AF0C:  bne          cr6, 0x8259af18
0x8259AF10:  mr           r11, r14
0x8259AF14:  b            0x8259b184
0x8259AF18:  cmpwi        cr6, r11, 0x23
0x8259AF1C:  bge          cr6, 0x8259b070
0x8259AF20:  lwz          r5, 0xc(r21)
0x8259AF24:  slwi         r4, r11, 2
0x8259AF28:  lwzx         r11, r4, r5
0x8259AF2C:  srawi        r3, r11, 0x18
0x8259AF30:  srawi        r9, r11, 4
0x8259AF34:  clrlwi       r10, r11, 0x1c
0x8259AF38:  clrlwi       r28, r9, 0x1c
0x8259AF3C:  srawi        r8, r11, 8
0x8259AF40:  srawi        r7, r11, 0x10
0x8259AF44:  add          r30, r10, r28
0x8259AF48:  clrlwi       r27, r3, 0x18
0x8259AF4C:  clrlwi       r26, r8, 0x18
0x8259AF50:  clrlwi       r25, r7, 0x18
0x8259AF54:  cmpwi        cr6, r30, 0
0x8259AF58:  ble          cr6, 0x8259b014
0x8259AF5C:  lwz          r9, 8(r31)
0x8259AF60:  mr           r29, r14
0x8259AF64:  cmplwi       cr6, r30, 0
0x8259AF68:  addi         r11, r9, 0x10
0x8259AF6C:  beq          cr6, 0x8259b014
0x8259AF70:  cmplw        cr6, r30, r11
0x8259AF74:  ble          cr6, 0x8259afd0
0x8259AF78:  cmplwi       cr6, r11, 0
0x8259AF7C:  beq          cr6, 0x8259afd0
0x8259AF80:  subfic       r6, r11, 0x40
0x8259AF84:  ld           r10, 0(r31)
0x8259AF88:  clrldi       r5, r11, 0x20
0x8259AF8C:  clrldi       r4, r6, 0x20
0x8259AF90:  subf.        r9, r11, r9
0x8259AF94:  subf         r30, r11, r30
0x8259AF98:  stw          r9, 8(r31)
0x8259AF9C:  sld          r11, r10, r5
0x8259AFA0:  srd          r3, r10, r4
0x8259AFA4:  slwi         r10, r3, 0
0x8259AFA8:  std          r11, 0(r31)
0x8259AFAC:  slw          r11, r10, r30
0x8259AFB0:  add          r29, r11, r29
0x8259AFB4:  bge          0x8259afc0
0x8259AFB8:  mr           r3, r31
0x8259AFBC:  bl           0x824d2748
0x8259AFC0:  lwz          r9, 8(r31)
0x8259AFC4:  addi         r11, r9, 0x10
0x8259AFC8:  cmplw        cr6, r30, r11
0x8259AFCC:  bgt          cr6, 0x8259af78
0x8259AFD0:  subfic       r8, r30, 0x40
0x8259AFD4:  ld           r11, 0(r31)
0x8259AFD8:  clrldi       r7, r30, 0x20
0x8259AFDC:  lwz          r9, 8(r31)
0x8259AFE0:  clrldi       r6, r8, 0x20
0x8259AFE4:  subf.        r10, r30, r9
0x8259AFE8:  stw          r10, 8(r31)
0x8259AFEC:  sld          r4, r11, r7
0x8259AFF0:  srd          r5, r11, r6
0x8259AFF4:  slwi         r11, r5, 0
0x8259AFF8:  add          r30, r11, r29
0x8259AFFC:  std          r4, 0(r31)
0x8259B000:  bge          0x8259b00c
0x8259B004:  mr           r3, r31
0x8259B008:  bl           0x824d2748
0x8259B00C:  mr           r10, r30
0x8259B010:  b            0x8259b018
0x8259B014:  mr           r10, r14
0x8259B018:  and          r11, r10, r27
0x8259B01C:  slwi         r8, r25, 1
0x8259B020:  rlwinm       r9, r11, 0, 0, 0x1e
0x8259B024:  sraw         r10, r10, r28
0x8259B028:  add          r9, r9, r8
0x8259B02C:  clrlwi       r7, r10, 0x1f
0x8259B030:  clrlwi       r3, r11, 0x1f
0x8259B034:  srawi        r10, r10, 1
0x8259B038:  slwi         r8, r9, 0xf
0x8259B03C:  neg          r11, r3
0x8259B040:  neg          r9, r7
0x8259B044:  add          r6, r10, r26
0x8259B048:  slwi         r5, r11, 0x10
0x8259B04C:  xor          r10, r6, r9
0x8259B050:  slwi         r4, r11, 0x10
0x8259B054:  xor          r3, r8, r5
0x8259B058:  subf         r9, r9, r10
0x8259B05C:  subf         r11, r4, r3
0x8259B060:  clrlwi       r8, r9, 0x10
0x8259B064:  li           r25, 1
0x8259B068:  or           r11, r11, r8
0x8259B06C:  b            0x8259b184
0x8259B070:  cmpwi        cr6, r11, 0x24
0x8259B074:  bne          cr6, 0x8259b084
0x8259B078:  mr           r24, r25
0x8259B07C:  mr           r11, r14
0x8259B080:  b            0x8259b184
0x8259B084:  lbz          r11, 0x16(r21)
0x8259B088:  mr           r29, r14
0x8259B08C:  lhz          r7, 0x3e(r21)
0x8259B090:  lhz          r6, 0x40(r21)
0x8259B094:  subf         r27, r11, r7
0x8259B098:  lwz          r10, 8(r31)
0x8259B09C:  subf         r28, r11, r6
0x8259B0A0:  addi         r11, r10, 0x10
0x8259B0A4:  add          r30, r28, r27
0x8259B0A8:  cmplwi       cr6, r30, 0
0x8259B0AC:  bne          cr6, 0x8259b0b8
0x8259B0B0:  mr           r11, r14
0x8259B0B4:  b            0x8259b158
0x8259B0B8:  cmplw        cr6, r30, r11
0x8259B0BC:  ble          cr6, 0x8259b118
0x8259B0C0:  cmplwi       cr6, r11, 0
0x8259B0C4:  beq          cr6, 0x8259b118
0x8259B0C8:  subfic       r5, r11, 0x40
0x8259B0CC:  ld           r9, 0(r31)
0x8259B0D0:  subf.        r10, r11, r10
0x8259B0D4:  clrldi       r3, r5, 0x20
0x8259B0D8:  clrldi       r4, r11, 0x20
0x8259B0DC:  subf         r30, r11, r30
0x8259B0E0:  stw          r10, 8(r31)
0x8259B0E4:  srd          r11, r9, r3
0x8259B0E8:  sld          r9, r9, r4
0x8259B0EC:  slwi         r8, r11, 0
0x8259B0F0:  std          r9, 0(r31)
0x8259B0F4:  slw          r11, r8, r30
0x8259B0F8:  add          r29, r11, r29
0x8259B0FC:  bge          0x8259b108
0x8259B100:  mr           r3, r31
0x8259B104:  bl           0x824d2748
0x8259B108:  lwz          r10, 8(r31)
0x8259B10C:  addi         r11, r10, 0x10
0x8259B110:  cmplw        cr6, r30, r11
0x8259B114:  bgt          cr6, 0x8259b0c0
0x8259B118:  subfic       r6, r30, 0x40
0x8259B11C:  ld           r11, 0(r31)
0x8259B120:  clrldi       r5, r30, 0x20
0x8259B124:  lwz          r7, 8(r31)
0x8259B128:  clrldi       r4, r6, 0x20
0x8259B12C:  subf.        r10, r30, r7
0x8259B130:  stw          r10, 8(r31)
0x8259B134:  sld          r9, r11, r5
0x8259B138:  srd          r3, r11, r4
0x8259B13C:  slwi         r11, r3, 0
0x8259B140:  add          r30, r11, r29
0x8259B144:  std          r9, 0(r31)
0x8259B148:  bge          0x8259b154
0x8259B14C:  mr           r3, r31
0x8259B150:  bl           0x824d2748
0x8259B154:  mr           r11, r30
0x8259B158:  slw          r10, r25, r27
0x8259B15C:  slw          r7, r25, r28
0x8259B160:  sraw         r6, r11, r28
0x8259B164:  addi         r5, r10, -1
0x8259B168:  slwi         r4, r7, 0x10
0x8259B16C:  slwi         r8, r11, 0x10
0x8259B170:  subf         r11, r17, r4
0x8259B174:  and          r3, r6, r5
0x8259B178:  and          r9, r11, r8
0x8259B17C:  clrlwi       r10, r3, 0x10
0x8259B180:  or           r11, r10, r9
0x8259B184:  sldi         r8, r23, 8
0x8259B188:  clrldi       r7, r11, 0x20
0x8259B18C:  or           r6, r8, r24
0x8259B190:  sldi         r5, r6, 0x20
0x8259B194:  or           r11, r5, r7
0x8259B198:  rldicl       r4, r11, 0x20, 0x20
0x8259B19C:  slwi         r19, r11, 0
0x8259B1A0:  clrlwi       r3, r4, 0x1f
0x8259B1A4:  or           r10, r3, r22
0x8259B1A8:  cmpwi        cr6, r10, 0
0x8259B1AC:  bne          cr6, 0x8259b65c
0x8259B1B0:  rldicl       r9, r11, 0x18, 0x28
0x8259B1B4:  clrlwi       r8, r9, 0x1f
0x8259B1B8:  cmpwi        cr6, r8, 0
0x8259B1BC:  bne          cr6, 0x8259b1d0
0x8259B1C0:  lwz          r7, 0(r16)
0x8259B1C4:  oris         r6, r7, 0x4000
0x8259B1C8:  stw          r6, 0(r16)
0x8259B1CC:  b            0x8259b890
0x8259B1D0:  lbz          r5, 0x15(r21)
0x8259B1D4:  lbz          r27, 0x14(r21)
0x8259B1D8:  cmplwi       cr6, r5, 0
0x8259B1DC:  beq          cr6, 0x8259b1f0
0x8259B1E0:  cmpwi        cr6, r22, 0
0x8259B1E4:  bne          cr6, 0x8259b1f0
0x8259B1E8:  mr           r26, r25
0x8259B1EC:  b            0x8259b238
0x8259B1F0:  mr           r26, r14
0x8259B1F4:  cmpwi        cr6, r22, 0
0x8259B1F8:  beq          cr6, 0x8259b238
0x8259B1FC:  lwz          r3, 0(r21)
0x8259B200:  ld           r11, 0(r3)
0x8259B204:  lwz          r4, 8(r3)
0x8259B208:  sldi         r10, r11, 1
0x8259B20C:  rldicl       r9, r11, 1, 0x3f
0x8259B210:  addic.       r11, r4, -1
0x8259B214:  slwi         r31, r9, 0
0x8259B218:  std          r10, 0(r3)
0x8259B21C:  stw          r11, 8(r3)
0x8259B220:  bge          0x8259b228
0x8259B224:  bl           0x824d2748
0x8259B228:  lwz          r7, 0(r16)
0x8259B22C:  clrlwi       r6, r31, 0x18
0x8259B230:  rlwimi       r7, r6, 3, 0x1b, 0x1c
0x8259B234:  stw          r7, 0(r16)
0x8259B238:  lwz          r31, 0(r21)
0x8259B23C:  lwz          r5, 0x70(r21)
0x8259B240:  ld           r10, 0(r31)
0x8259B244:  lwz          r28, 0(r5)
0x8259B248:  rldicl       r4, r10, 8, 0x38
0x8259B24C:  slwi         r3, r4, 1
0x8259B250:  lhzx         r11, r3, r28
0x8259B254:  extsh        r30, r11
0x8259B258:  cmpwi        cr6, r30, 0
0x8259B25C:  blt          cr6, 0x8259b320
0x8259B260:  clrlwi       r11, r30, 0x1c
0x8259B264:  lwz          r9, 8(r31)
0x8259B268:  mr           r8, r11
0x8259B26C:  subf.        r11, r11, r9
0x8259B270:  stw          r11, 8(r31)
0x8259B274:  sld          r7, r10, r8
0x8259B278:  std          r7, 0(r31)
0x8259B27C:  bge          0x8259b318
0x8259B280:  lwz          r10, 0x10(r31)
0x8259B284:  lwz          r11, 0xc(r31)
0x8259B288:  addi         r6, r10, -4
0x8259B28C:  cmplw        cr6, r11, r6
0x8259B290:  blt          cr6, 0x8259b2ac
0x8259B294:  mr           r3, r31
0x8259B298:  bl           0x824d2688
0x8259B29C:  cmplwi       cr6, r3, 1
0x8259B2A0:  beq          cr6, 0x8259b280
0x8259B2A4:  srawi        r30, r30, 4
0x8259B2A8:  b            0x8259b360
0x8259B2AC:  lbz          r3, 0(r11)
0x8259B2B0:  addi         r4, r11, 6
0x8259B2B4:  lbz          r8, 1(r11)
0x8259B2B8:  rldicr       r9, r3, 8, 0x3f
0x8259B2BC:  lbz          r6, 3(r11)
0x8259B2C0:  lbz          r7, 4(r11)
0x8259B2C4:  add          r9, r9, r8
0x8259B2C8:  lbz          r5, 2(r11)
0x8259B2CC:  lbz          r8, 5(r11)
0x8259B2D0:  sldi         r11, r9, 8
0x8259B2D4:  stw          r4, 0xc(r31)
0x8259B2D8:  lwz          r10, 8(r31)
0x8259B2DC:  add          r5, r11, r5
0x8259B2E0:  ld           r9, 0(r31)
0x8259B2E4:  sldi         r11, r5, 8
0x8259B2E8:  add          r4, r11, r6
0x8259B2EC:  sldi         r11, r4, 8
0x8259B2F0:  add          r3, r11, r7
0x8259B2F4:  addi         r7, r10, 0x30
0x8259B2F8:  sldi         r11, r3, 8
0x8259B2FC:  add          r11, r11, r8
0x8259B300:  neg          r8, r10
0x8259B304:  stw          r7, 8(r31)
0x8259B308:  extsw        r6, r8
0x8259B30C:  sld          r11, r11, r6
0x8259B310:  add          r5, r11, r9
0x8259B314:  std          r5, 0(r31)
0x8259B318:  srawi        r30, r30, 4
0x8259B31C:  b            0x8259b360
0x8259B320:  li           r4, 8
0x8259B324:  mr           r3, r31
0x8259B328:  bl           0x824d2818
0x8259B32C:  ld           r11, 0(r31)
0x8259B330:  li           r4, 1
0x8259B334:  mr           r3, r31
0x8259B338:  rldicl       r10, r11, 1, 0x3f
0x8259B33C:  slwi         r29, r10, 0
0x8259B340:  bl           0x824d2818
0x8259B344:  add          r9, r30, r29
0x8259B348:  add          r8, r9, r15
0x8259B34C:  slwi         r7, r8, 1
0x8259B350:  lhzx         r6, r7, r28
0x8259B354:  extsh        r30, r6
0x8259B358:  cmpwi        cr6, r30, 0
0x8259B35C:  blt          cr6, 0x8259b32c
0x8259B360:  lwz          r31, 0(r21)
0x8259B364:  mr           r18, r30
0x8259B368:  lwz          r5, 0x14(r31)
0x8259B36C:  cmpwi        cr6, r5, 0
0x8259B370:  beq          cr6, 0x8259b38c
0x8259B374:  li           r11, 4
0x8259B378:  li           r12, 1
0x8259B37C:  clrldi       r4, r11, 0x20
0x8259B380:  rldicr       r12, r12, 0x3f, 0x3f
0x8259B384:  or           r11, r4, r12
0x8259B388:  b            0x8259b8b4
0x8259B38C:  lbz          r4, 0x13(r21)
0x8259B390:  cmplwi       cr6, r4, 0
0x8259B394:  beq          cr6, 0x8259b674
0x8259B398:  lbz          r11, 0x2fd(r21)
0x8259B39C:  cmplwi       cr6, r11, 0
0x8259B3A0:  beq          cr6, 0x8259b3e4
0x8259B3A4:  lwz          r3, 0(r16)
0x8259B3A8:  srwi         r10, r3, 0xc
0x8259B3AC:  and          r9, r10, r11
0x8259B3B0:  clrlwi       r8, r9, 0x1c
0x8259B3B4:  cmplwi       cr6, r8, 0
0x8259B3B8:  beq          cr6, 0x8259b3c8
0x8259B3BC:  lbz          r7, 0x2fe(r21)
0x8259B3C0:  rotlwi       r11, r7, 1
0x8259B3C4:  b            0x8259b640
0x8259B3C8:  lbz          r4, 0x2fc(r21)
0x8259B3CC:  lbz          r10, 0x301(r21)
0x8259B3D0:  rotlwi       r11, r4, 1
0x8259B3D4:  add          r11, r11, r10
0x8259B3D8:  addi         r3, r11, 0xff
0x8259B3DC:  stb          r3, 4(r16)
0x8259B3E0:  b            0x8259b648
0x8259B3E4:  lbz          r10, 0x302(r21)
0x8259B3E8:  mr           r29, r14
0x8259B3EC:  lwz          r9, 8(r31)
0x8259B3F0:  cmplwi       cr6, r10, 0
0x8259B3F4:  addi         r11, r9, 0x10
0x8259B3F8:  beq          cr6, 0x8259b4d4
0x8259B3FC:  mr           r30, r25
0x8259B400:  cmplwi       cr6, r11, 1
0x8259B404:  bge          cr6, 0x8259b460
0x8259B408:  cmplwi       cr6, r11, 0
0x8259B40C:  beq          cr6, 0x8259b460
0x8259B410:  subfic       r8, r11, 0x40
0x8259B414:  ld           r10, 0(r31)
0x8259B418:  clrldi       r7, r11, 0x20
0x8259B41C:  clrldi       r6, r8, 0x20
0x8259B420:  subf         r30, r11, r30
0x8259B424:  subf.        r9, r11, r9
0x8259B428:  stw          r9, 8(r31)
0x8259B42C:  sld          r4, r10, r7
0x8259B430:  srd          r5, r10, r6
0x8259B434:  slwi         r3, r5, 0
0x8259B438:  std          r4, 0(r31)
0x8259B43C:  slw          r11, r3, r30
0x8259B440:  add          r29, r11, r29
0x8259B444:  bge          0x8259b450
0x8259B448:  mr           r3, r31
0x8259B44C:  bl           0x824d2748
0x8259B450:  lwz          r9, 8(r31)
0x8259B454:  addi         r11, r9, 0x10
0x8259B458:  cmplw        cr6, r30, r11
0x8259B45C:  bgt          cr6, 0x8259b408
0x8259B460:  subfic       r9, r30, 0x40
0x8259B464:  ld           r11, 0(r31)
0x8259B468:  clrldi       r8, r30, 0x20
0x8259B46C:  lwz          r10, 8(r31)
0x8259B470:  clrldi       r7, r9, 0x20
0x8259B474:  subf.        r10, r30, r10
0x8259B478:  stw          r10, 8(r31)
0x8259B47C:  sld          r5, r11, r8
0x8259B480:  srd          r6, r11, r7
0x8259B484:  slwi         r11, r6, 0
0x8259B488:  add          r30, r11, r29
0x8259B48C:  std          r5, 0(r31)
0x8259B490:  bge          0x8259b49c
0x8259B494:  mr           r3, r31
0x8259B498:  bl           0x824d2748
0x8259B49C:  cmplwi       cr6, r30, 0
0x8259B4A0:  beq          cr6, 0x8259b4b8
0x8259B4A4:  lbz          r4, 0x2fe(r21)
0x8259B4A8:  rotlwi       r11, r4, 1
0x8259B4AC:  addi         r11, r11, -1
0x8259B4B0:  stb          r11, 4(r16)
0x8259B4B4:  b            0x8259b648
0x8259B4B8:  lbz          r3, 0x2fc(r21)
0x8259B4BC:  lbz          r10, 0x301(r21)
0x8259B4C0:  rotlwi       r11, r3, 1
0x8259B4C4:  add          r11, r11, r10
0x8259B4C8:  addi         r11, r11, -1
0x8259B4CC:  stb          r11, 4(r16)
0x8259B4D0:  b            0x8259b648
0x8259B4D4:  li           r30, 3
0x8259B4D8:  cmplwi       cr6, r11, 3
0x8259B4DC:  bge          cr6, 0x8259b538
0x8259B4E0:  cmplwi       cr6, r11, 0
0x8259B4E4:  beq          cr6, 0x8259b538
0x8259B4E8:  subfic       r8, r11, 0x40
0x8259B4EC:  ld           r10, 0(r31)
0x8259B4F0:  clrldi       r7, r11, 0x20
0x8259B4F4:  clrldi       r6, r8, 0x20
0x8259B4F8:  subf         r30, r11, r30
0x8259B4FC:  subf.        r9, r11, r9
0x8259B500:  stw          r9, 8(r31)
0x8259B504:  sld          r4, r10, r7
0x8259B508:  srd          r5, r10, r6
0x8259B50C:  slwi         r3, r5, 0
0x8259B510:  std          r4, 0(r31)
0x8259B514:  slw          r11, r3, r30
0x8259B518:  add          r29, r11, r29
0x8259B51C:  bge          0x8259b528
0x8259B520:  mr           r3, r31
0x8259B524:  bl           0x824d2748
0x8259B528:  lwz          r9, 8(r31)
0x8259B52C:  addi         r11, r9, 0x10
0x8259B530:  cmplw        cr6, r30, r11
0x8259B534:  bgt          cr6, 0x8259b4e0
0x8259B538:  subfic       r9, r30, 0x40
0x8259B53C:  ld           r11, 0(r31)
0x8259B540:  clrldi       r8, r30, 0x20
0x8259B544:  lwz          r10, 8(r31)
0x8259B548:  clrldi       r7, r9, 0x20
0x8259B54C:  subf.        r10, r30, r10
0x8259B550:  stw          r10, 8(r31)
0x8259B554:  sld          r5, r11, r8
0x8259B558:  srd          r6, r11, r7
0x8259B55C:  slwi         r11, r6, 0
0x8259B560:  add          r30, r11, r29
0x8259B564:  std          r5, 0(r31)
0x8259B568:  bge          0x8259b574
0x8259B56C:  mr           r3, r31
0x8259B570:  bl           0x824d2748
0x8259B574:  cmpwi        cr6, r30, 7
0x8259B578:  bne          cr6, 0x8259b634
0x8259B57C:  lwz          r31, 0(r21)
0x8259B580:  li           r30, 5
0x8259B584:  mr           r29, r14
0x8259B588:  lwz          r9, 8(r31)
0x8259B58C:  addi         r11, r9, 0x10
0x8259B590:  cmplwi       cr6, r11, 5
0x8259B594:  bge          cr6, 0x8259b5f0
0x8259B598:  cmplwi       cr6, r11, 0
0x8259B59C:  beq          cr6, 0x8259b5f0
0x8259B5A0:  subfic       r4, r11, 0x40
0x8259B5A4:  ld           r10, 0(r31)
0x8259B5A8:  subf.        r9, r11, r9
0x8259B5AC:  clrldi       r3, r11, 0x20
0x8259B5B0:  subf         r30, r11, r30
0x8259B5B4:  clrldi       r11, r4, 0x20
0x8259B5B8:  stw          r9, 8(r31)
0x8259B5BC:  sld          r7, r10, r3
0x8259B5C0:  srd          r8, r10, r11
0x8259B5C4:  slwi         r6, r8, 0
0x8259B5C8:  std          r7, 0(r31)
0x8259B5CC:  slw          r11, r6, r30
0x8259B5D0:  add          r29, r11, r29
0x8259B5D4:  bge          0x8259b5e0
0x8259B5D8:  mr           r3, r31
0x8259B5DC:  bl           0x824d2748
0x8259B5E0:  lwz          r9, 8(r31)
0x8259B5E4:  addi         r11, r9, 0x10
0x8259B5E8:  cmplw        cr6, r30, r11
0x8259B5EC:  bgt          cr6, 0x8259b598
0x8259B5F0:  subfic       r4, r30, 0x40
0x8259B5F4:  ld           r11, 0(r31)
0x8259B5F8:  clrldi       r3, r30, 0x20
0x8259B5FC:  lwz          r5, 8(r31)
0x8259B600:  clrldi       r9, r4, 0x20
0x8259B604:  subf.        r10, r30, r5
0x8259B608:  stw          r10, 8(r31)
0x8259B60C:  sld          r7, r11, r3
0x8259B610:  srd          r8, r11, r9
0x8259B614:  slwi         r11, r8, 0
0x8259B618:  add          r30, r11, r29
0x8259B61C:  std          r7, 0(r31)
0x8259B620:  bge          0x8259b62c
0x8259B624:  mr           r3, r31
0x8259B628:  bl           0x824d2748
0x8259B62C:  mr           r11, r30
0x8259B630:  b            0x8259b63c
0x8259B634:  lbz          r11, 0x2fc(r21)
0x8259B638:  add          r11, r11, r30
0x8259B63C:  slwi         r11, r11, 1
0x8259B640:  addi         r6, r11, 0xff
0x8259B644:  stb          r6, 4(r16)
0x8259B648:  lbz          r11, 4(r16)
0x8259B64C:  cmplwi       cr6, r11, 1
0x8259B650:  blt          cr6, 0x8259b65c
0x8259B654:  cmplwi       cr6, r11, 0x3e
0x8259B658:  ble          cr6, 0x8259b674
0x8259B65C:  mr           r11, r25
0x8259B660:  li           r12, 1
0x8259B664:  clrldi       r4, r11, 0x20
0x8259B668:  rldicr       r12, r12, 0x3f, 0x3f
0x8259B66C:  or           r11, r4, r12
0x8259B670:  b            0x8259b8b4
0x8259B674:  lwz          r3, 0(r16)
0x8259B678:  cmpwi        cr6, r27, 0
0x8259B67C:  rlwinm       r11, r3, 0, 2, 0
0x8259B680:  stw          r11, 0(r16)
0x8259B684:  beq          cr6, 0x8259b6fc
0x8259B688:  lwz          r3, 0(r21)
0x8259B68C:  ld           r11, 0(r3)
0x8259B690:  lwz          r10, 8(r3)
0x8259B694:  sldi         r9, r11, 1
0x8259B698:  rldicl       r8, r11, 1, 0x3f
0x8259B69C:  addic.       r11, r10, -1
0x8259B6A0:  slwi         r30, r8, 0
0x8259B6A4:  std          r9, 0(r3)
0x8259B6A8:  stw          r11, 8(r3)
0x8259B6AC:  bge          0x8259b6b4
0x8259B6B0:  bl           0x824d2748
0x8259B6B4:  mr           r11, r30
0x8259B6B8:  cmpwi        cr6, r30, 0
0x8259B6BC:  beq          cr6, 0x8259b6f0
0x8259B6C0:  lwz          r3, 0(r21)
0x8259B6C4:  ld           r11, 0(r3)
0x8259B6C8:  lwz          r7, 8(r3)
0x8259B6CC:  sldi         r6, r11, 1
0x8259B6D0:  rldicl       r5, r11, 1, 0x3f
0x8259B6D4:  addic.       r11, r7, -1
0x8259B6D8:  slwi         r31, r5, 0
0x8259B6DC:  std          r6, 0(r3)
0x8259B6E0:  stw          r11, 8(r3)
0x8259B6E4:  bge          0x8259b6ec
0x8259B6E8:  bl           0x824d2748
0x8259B6EC:  add          r11, r31, r30
0x8259B6F0:  lwz          r4, 0(r16)
0x8259B6F4:  rlwimi       r4, r11, 0x16, 8, 9
0x8259B6F8:  stw          r4, 0(r16)
0x8259B6FC:  cmpwi        cr6, r26, 0
0x8259B700:  beq          cr6, 0x8259b890
0x8259B704:  lwz          r31, 0(r21)
0x8259B708:  lwz          r3, 0x74(r21)
0x8259B70C:  ld           r10, 0(r31)
0x8259B710:  lwz          r28, 0(r3)
0x8259B714:  rldicl       r11, r10, 8, 0x38
0x8259B718:  slwi         r9, r11, 1
0x8259B71C:  lhzx         r8, r9, r28
0x8259B720:  extsh        r30, r8
0x8259B724:  cmpwi        cr6, r30, 0
0x8259B728:  blt          cr6, 0x8259b7ec
0x8259B72C:  clrlwi       r11, r30, 0x1c
0x8259B730:  lwz          r7, 8(r31)
0x8259B734:  mr           r6, r11
0x8259B738:  subf.        r11, r11, r7
0x8259B73C:  stw          r11, 8(r31)
0x8259B740:  sld          r5, r10, r6
0x8259B744:  std          r5, 0(r31)
0x8259B748:  bge          0x8259b7e4
0x8259B74C:  lwz          r10, 0x10(r31)
0x8259B750:  lwz          r11, 0xc(r31)
0x8259B754:  addi         r4, r10, -4
0x8259B758:  cmplw        cr6, r11, r4
0x8259B75C:  blt          cr6, 0x8259b778
0x8259B760:  mr           r3, r31
0x8259B764:  bl           0x824d2688
0x8259B768:  cmplwi       cr6, r3, 1
0x8259B76C:  beq          cr6, 0x8259b74c
0x8259B770:  srawi        r30, r30, 4
0x8259B774:  b            0x8259b82c
0x8259B778:  lbz          r9, 0(r11)
0x8259B77C:  addi         r3, r11, 6
0x8259B780:  lbz          r8, 1(r11)
0x8259B784:  rldicr       r9, r9, 8, 0x3f
0x8259B788:  lbz          r5, 2(r11)
0x8259B78C:  lbz          r6, 3(r11)
0x8259B790:  add          r4, r9, r8
0x8259B794:  lbz          r7, 4(r11)
0x8259B798:  lbz          r8, 5(r11)
0x8259B79C:  sldi         r11, r4, 8
0x8259B7A0:  stw          r3, 0xc(r31)
0x8259B7A4:  lwz          r10, 8(r31)
0x8259B7A8:  add          r3, r11, r5
0x8259B7AC:  ld           r9, 0(r31)
0x8259B7B0:  neg          r5, r10
0x8259B7B4:  sldi         r11, r3, 8
0x8259B7B8:  extsw        r3, r5
0x8259B7BC:  add          r11, r11, r6
0x8259B7C0:  addi         r4, r10, 0x30
0x8259B7C4:  sldi         r11, r11, 8
0x8259B7C8:  add          r7, r11, r7
0x8259B7CC:  sldi         r11, r7, 8
0x8259B7D0:  stw          r4, 8(r31)
0x8259B7D4:  add          r6, r11, r8
0x8259B7D8:  sld          r11, r6, r3
0x8259B7DC:  add          r11, r11, r9
0x8259B7E0:  std          r11, 0(r31)
0x8259B7E4:  srawi        r30, r30, 4
0x8259B7E8:  b            0x8259b82c
0x8259B7EC:  li           r4, 8
0x8259B7F0:  mr           r3, r31
0x8259B7F4:  bl           0x824d2818
0x8259B7F8:  ld           r10, 0(r31)
0x8259B7FC:  li           r4, 1
0x8259B800:  mr           r3, r31
0x8259B804:  rldicl       r9, r10, 1, 0x3f
0x8259B808:  slwi         r29, r9, 0
0x8259B80C:  bl           0x824d2818
0x8259B810:  add          r8, r30, r29
0x8259B814:  add          r7, r8, r15
0x8259B818:  slwi         r6, r7, 1
0x8259B81C:  lhzx         r5, r6, r28
0x8259B820:  extsh        r30, r5
0x8259B824:  cmpwi        cr6, r30, 0
0x8259B828:  blt          cr6, 0x8259b7f8
0x8259B82C:  lwz          r4, 0(r21)
0x8259B830:  lwz          r3, 0x14(r4)
0x8259B834:  cmpwi        cr6, r3, 0
0x8259B838:  beq          cr6, 0x8259b844
0x8259B83C:  li           r11, 4
0x8259B840:  b            0x8259b8b4
0x8259B844:  cmpwi        cr6, r30, 8
0x8259B848:  mr           r9, r25
0x8259B84C:  blt          cr6, 0x8259b854
0x8259B850:  mr           r9, r14
0x8259B854:  lwz          r11, 0(r16)
0x8259B858:  slwi         r10, r30, 2
0x8259B85C:  rlwimi       r11, r9, 0x1c, 3, 3
0x8259B860:  lwz          r9, 0x68(r1)
0x8259B864:  mr           r7, r11
0x8259B868:  addi         r8, r9, 0x40
0x8259B86C:  stw          r11, 0(r16)
0x8259B870:  lwzx         r6, r10, r9
0x8259B874:  rlwimi       r7, r6, 0x18, 5, 7
0x8259B878:  slwi         r4, r7, 0
0x8259B87C:  stw          r7, 0(r16)
0x8259B880:  lwzx         r5, r10, r8
0x8259B884:  rlwimi       r4, r5, 0x14, 0xa, 0xb
0x8259B888:  rlwinm       r3, r4, 0, 5, 3
0x8259B88C:  stw          r3, 0(r16)
0x8259B890:  stb          r18, 5(r16)
0x8259B894:  cmpwi        cr6, r22, 0
0x8259B898:  beq          cr6, 0x8259b8a8
0x8259B89C:  li           r11, 0x4000
0x8259B8A0:  rldimi       r11, r11, 0x20, 0
0x8259B8A4:  b            0x8259b8b4
0x8259B8A8:  clrldi       r9, r20, 0x20
0x8259B8AC:  sldi         r8, r19, 0x20
0x8259B8B0:  or           r11, r8, r9
0x8259B8B4:  li           r6, 1
0x8259B8B8:  rldicr       r7, r11, 0, 1
0x8259B8BC:  rldicr       r5, r6, 0x3f, 0x3f
0x8259B8C0:  cmpld        cr6, r7, r5
0x8259B8C4:  beq          cr6, 0x8259c3a4
0x8259B8C8:  lwz          r4, 0x50(r1)
0x8259B8CC:  sradi        r8, r11, 0x20
0x8259B8D0:  lwz          r3, 0x3b0(r21)
0x8259B8D4:  li           r7, 0x4000
0x8259B8D8:  slwi         r10, r4, 2
0x8259B8DC:  rldimi       r7, r7, 0x20, 0
0x8259B8E0:  cmpld        cr6, r11, r7
0x8259B8E4:  stwx         r11, r3, r10
0x8259B8E8:  lwz          r5, 0x3b4(r21)
0x8259B8EC:  stwx         r8, r5, r10
0x8259B8F0:  lbz          r19, 5(r16)
0x8259B8F4:  bne          cr6, 0x8259be08
0x8259B8F8:  lbz          r11, 4(r16)
0x8259B8FC:  lwz          r10, 0x90(r21)
0x8259B900:  rotlwi       r9, r11, 2
0x8259B904:  lbz          r4, 0x14(r21)
0x8259B908:  add          r3, r11, r9
0x8259B90C:  cmplwi       cr6, r4, 0
0x8259B910:  slwi         r11, r3, 2
0x8259B914:  add          r23, r11, r10
0x8259B918:  beq          cr6, 0x8259b938
0x8259B91C:  lwz          r11, 0(r16)
0x8259B920:  lwz          r9, 0x98(r21)
0x8259B924:  lwz          r10, 0x9c(r21)
0x8259B928:  rlwinm       r11, r11, 0xc, 0x1c, 0x1d
0x8259B92C:  add          r18, r9, r11
0x8259B930:  add          r17, r10, r11
0x8259B934:  b            0x8259b940
0x8259B938:  lwz          r18, 0xa0(r21)
0x8259B93C:  lwz          r17, 0xa4(r21)
0x8259B940:  lhz          r10, 0xf4(r21)
0x8259B944:  rlwinm       r9, r19, 0xb, 0xd, 0x14
0x8259B948:  lhz          r8, 0xf6(r21)
0x8259B94C:  mr           r22, r14
0x8259B950:  rlwinm       r7, r10, 0, 0, 0x1e
0x8259B954:  lwz          r11, 0x2b4(r21)
0x8259B958:  srwi         r6, r8, 1
0x8259B95C:  or           r5, r7, r9
0x8259B960:  slwi         r4, r5, 0xf
0x8259B964:  or           r3, r4, r6
0x8259B968:  stw          r3, 0(r11)
0x8259B96C:  lwz          r11, 0x2b4(r21)
0x8259B970:  addi         r10, r11, 4
0x8259B974:  stw          r10, 0x2b4(r21)
0x8259B978:  rlwinm       r9, r22, 0, 0, 0x1d
0x8259B97C:  clrlwi       r20, r19, 0x1f
0x8259B980:  cmpwi        cr6, r9, 0
0x8259B984:  bne          cr6, 0x8259b994
0x8259B988:  lwz          r11, 0x2f0(r21)
0x8259B98C:  mr           r24, r17
0x8259B990:  b            0x8259b99c
0x8259B994:  lwz          r11, 0x2f4(r21)
0x8259B998:  mr           r24, r18
0x8259B99C:  lwz          r10, 0x2ac(r21)
0x8259B9A0:  lwz          r26, 0x10(r23)
0x8259B9A4:  addi         r25, r10, -0x80
0x8259B9A8:  stw          r25, 0x2ac(r21)
0x8259B9AC:  dcbzl        0, r25
0x8259B9B0:  mr           r27, r14
0x8259B9B4:  lbz          r4, 8(r11)
0x8259B9B8:  lwz          r31, 0(r21)
0x8259B9BC:  subfic       r8, r4, 0x40
0x8259B9C0:  lwz          r29, 0(r11)
0x8259B9C4:  clrldi       r7, r8, 0x20
0x8259B9C8:  ld           r11, 0(r31)
0x8259B9CC:  srd          r6, r11, r7
0x8259B9D0:  slwi         r5, r6, 1
0x8259B9D4:  lhzx         r3, r5, r29
0x8259B9D8:  extsh        r30, r3
0x8259B9DC:  cmpwi        cr6, r30, 0
0x8259B9E0:  blt          cr6, 0x8259baa4
0x8259B9E4:  clrlwi       r10, r30, 0x1c
0x8259B9E8:  lwz          r9, 8(r31)
0x8259B9EC:  mr           r8, r10
0x8259B9F0:  subf.        r10, r10, r9
0x8259B9F4:  stw          r10, 8(r31)
0x8259B9F8:  sld          r7, r11, r8
0x8259B9FC:  std          r7, 0(r31)
0x8259BA00:  bge          0x8259ba9c
0x8259BA04:  lwz          r10, 0x10(r31)
0x8259BA08:  lwz          r11, 0xc(r31)
0x8259BA0C:  addi         r6, r10, -4
0x8259BA10:  cmplw        cr6, r11, r6
0x8259BA14:  blt          cr6, 0x8259ba30
0x8259BA18:  mr           r3, r31
0x8259BA1C:  bl           0x824d2688
0x8259BA20:  cmplwi       cr6, r3, 1
0x8259BA24:  beq          cr6, 0x8259ba04
0x8259BA28:  srawi        r30, r30, 4
0x8259BA2C:  b            0x8259bae0
0x8259BA30:  lbz          r3, 0(r11)
0x8259BA34:  addi         r4, r11, 6
0x8259BA38:  lbz          r9, 1(r11)
0x8259BA3C:  rldicr       r8, r3, 8, 0x3f
0x8259BA40:  lbz          r6, 3(r11)
0x8259BA44:  lbz          r7, 4(r11)
0x8259BA48:  add          r9, r9, r8
0x8259BA4C:  lbz          r5, 2(r11)
0x8259BA50:  lbz          r8, 5(r11)
0x8259BA54:  sldi         r11, r9, 8
0x8259BA58:  stw          r4, 0xc(r31)
0x8259BA5C:  lwz          r10, 8(r31)
0x8259BA60:  add          r5, r11, r5
0x8259BA64:  ld           r9, 0(r31)
0x8259BA68:  sldi         r11, r5, 8
0x8259BA6C:  add          r4, r11, r6
0x8259BA70:  sldi         r11, r4, 8
0x8259BA74:  add          r3, r11, r7
0x8259BA78:  addi         r7, r10, 0x30
0x8259BA7C:  sldi         r11, r3, 8
0x8259BA80:  add          r11, r11, r8
0x8259BA84:  neg          r8, r10
0x8259BA88:  stw          r7, 8(r31)
0x8259BA8C:  extsw        r6, r8
0x8259BA90:  sld          r11, r11, r6
0x8259BA94:  add          r5, r11, r9
0x8259BA98:  std          r5, 0(r31)
0x8259BA9C:  srawi        r30, r30, 4
0x8259BAA0:  b            0x8259bae0
0x8259BAA4:  mr           r3, r31
0x8259BAA8:  bl           0x824d2818
0x8259BAAC:  ld           r11, 0(r31)
0x8259BAB0:  li           r4, 1
0x8259BAB4:  mr           r3, r31
0x8259BAB8:  rldicl       r10, r11, 1, 0x3f
0x8259BABC:  slwi         r28, r10, 0
0x8259BAC0:  bl           0x824d2818
0x8259BAC4:  add          r9, r30, r28
0x8259BAC8:  add          r8, r9, r15
0x8259BACC:  slwi         r7, r8, 1
0x8259BAD0:  lhzx         r6, r7, r29
0x8259BAD4:  extsh        r30, r6
0x8259BAD8:  cmpwi        cr6, r30, 0
0x8259BADC:  blt          cr6, 0x8259baac
0x8259BAE0:  clrlwi       r31, r30, 0x10
0x8259BAE4:  mr           r28, r31
0x8259BAE8:  cmpwi        cr6, r28, 0x77
0x8259BAEC:  beq          cr6, 0x8259bc54
0x8259BAF0:  cmplwi       cr6, r28, 0
0x8259BAF4:  beq          cr6, 0x8259bd7c
0x8259BAF8:  cmpwi        cr6, r26, 4
0x8259BAFC:  bne          cr6, 0x8259bb44
0x8259BB00:  lwz          r3, 0(r21)
0x8259BB04:  ld           r11, 0(r3)
0x8259BB08:  lwz          r5, 8(r3)
0x8259BB0C:  sldi         r4, r11, 1
0x8259BB10:  rldicl       r10, r11, 1, 0x3f
0x8259BB14:  addic.       r11, r5, -1
0x8259BB18:  slwi         r31, r10, 0
0x8259BB1C:  std          r4, 0(r3)
0x8259BB20:  stw          r11, 8(r3)
0x8259BB24:  bge          0x8259bb2c
0x8259BB28:  bl           0x824d2748
0x8259BB2C:  slwi         r11, r28, 1
0x8259BB30:  add          r9, r11, r31
0x8259BB34:  addis        r8, r9, 1
0x8259BB38:  addi         r8, r8, -1
0x8259BB3C:  clrlwi       r31, r8, 0x10
0x8259BB40:  b            0x8259bc14
0x8259BB44:  cmpwi        cr6, r26, 2
0x8259BB48:  bne          cr6, 0x8259bc14
0x8259BB4C:  lwz          r31, 0(r21)
0x8259BB50:  li           r30, 2
0x8259BB54:  mr           r29, r14
0x8259BB58:  lwz          r11, 8(r31)
0x8259BB5C:  addi         r11, r11, 0x10
0x8259BB60:  cmplwi       cr6, r11, 2
0x8259BB64:  bge          cr6, 0x8259bbc4
0x8259BB68:  cmplwi       cr6, r11, 0
0x8259BB6C:  beq          cr6, 0x8259bbc4
0x8259BB70:  subfic       r6, r11, 0x40
0x8259BB74:  ld           r10, 0(r31)
0x8259BB78:  clrldi       r4, r11, 0x20
0x8259BB7C:  lwz          r7, 8(r31)
0x8259BB80:  clrldi       r5, r6, 0x20
0x8259BB84:  subf.        r9, r11, r7
0x8259BB88:  subf         r30, r11, r30
0x8259BB8C:  stw          r9, 8(r31)
0x8259BB90:  sld          r11, r10, r4
0x8259BB94:  srd          r3, r10, r5
0x8259BB98:  slwi         r10, r3, 0
0x8259BB9C:  std          r11, 0(r31)
0x8259BBA0:  slw          r11, r10, r30
0x8259BBA4:  add          r29, r11, r29
0x8259BBA8:  bge          0x8259bbb4
0x8259BBAC:  mr           r3, r31
0x8259BBB0:  bl           0x824d2748
0x8259BBB4:  lwz          r11, 8(r31)
0x8259BBB8:  addi         r11, r11, 0x10
0x8259BBBC:  cmplw        cr6, r30, r11
0x8259BBC0:  bgt          cr6, 0x8259bb68
0x8259BBC4:  subfic       r8, r30, 0x40
0x8259BBC8:  ld           r11, 0(r31)
0x8259BBCC:  clrldi       r7, r30, 0x20
0x8259BBD0:  lwz          r9, 8(r31)
0x8259BBD4:  clrldi       r6, r8, 0x20
0x8259BBD8:  subf.        r10, r30, r9
0x8259BBDC:  stw          r10, 8(r31)
0x8259BBE0:  sld          r4, r11, r7
0x8259BBE4:  srd          r5, r11, r6
0x8259BBE8:  slwi         r11, r5, 0
0x8259BBEC:  add          r30, r11, r29
0x8259BBF0:  std          r4, 0(r31)
0x8259BBF4:  bge          0x8259bc00
0x8259BBF8:  mr           r3, r31
0x8259BBFC:  bl           0x824d2748
0x8259BC00:  slwi         r11, r28, 2
0x8259BC04:  add          r3, r11, r30
0x8259BC08:  addis        r11, r3, 1
0x8259BC0C:  addi         r11, r11, -3
0x8259BC10:  clrlwi       r31, r11, 0x10
0x8259BC14:  lwz          r3, 0(r21)
0x8259BC18:  ld           r11, 0(r3)
0x8259BC1C:  lwz          r10, 8(r3)
0x8259BC20:  sldi         r9, r11, 1
0x8259BC24:  rldicl       r8, r11, 1, 0x3f
0x8259BC28:  addic.       r11, r10, -1
0x8259BC2C:  slwi         r30, r8, 0
0x8259BC30:  std          r9, 0(r3)
0x8259BC34:  stw          r11, 8(r3)
0x8259BC38:  bge          0x8259bc40
0x8259BC3C:  bl           0x824d2748
0x8259BC40:  cmplwi       cr6, r30, 0
0x8259BC44:  beq          cr6, 0x8259bd74
0x8259BC48:  clrlwi       r7, r31, 0x10
0x8259BC4C:  neg          r11, r7
0x8259BC50:  b            0x8259bd78
0x8259BC54:  mr           r11, r14
0x8259BC58:  cmpwi        cr6, r26, 4
0x8259BC5C:  bgt          cr6, 0x8259bc68
0x8259BC60:  srawi        r6, r26, 1
0x8259BC64:  subfic       r11, r6, 3
0x8259BC68:  lwz          r31, 0(r21)
0x8259BC6C:  addi         r30, r11, 8
0x8259BC70:  mr           r29, r14
0x8259BC74:  cmplwi       cr6, r30, 0
0x8259BC78:  lwz          r11, 8(r31)
0x8259BC7C:  addi         r11, r11, 0x10
0x8259BC80:  bne          cr6, 0x8259bc8c
0x8259BC84:  mr           r11, r14
0x8259BC88:  b            0x8259bd30
0x8259BC8C:  cmplw        cr6, r30, r11
0x8259BC90:  ble          cr6, 0x8259bcf0
0x8259BC94:  cmplwi       cr6, r11, 0
0x8259BC98:  beq          cr6, 0x8259bcf0
0x8259BC9C:  subfic       r4, r11, 0x40
0x8259BCA0:  ld           r10, 0(r31)
0x8259BCA4:  lwz          r5, 8(r31)
0x8259BCA8:  subf         r30, r11, r30
0x8259BCAC:  clrldi       r3, r4, 0x20
0x8259BCB0:  subf.        r9, r11, r5
0x8259BCB4:  clrldi       r11, r11, 0x20
0x8259BCB8:  stw          r9, 8(r31)
0x8259BCBC:  srd          r8, r10, r3
0x8259BCC0:  slwi         r6, r8, 0
0x8259BCC4:  sld          r7, r10, r11
0x8259BCC8:  std          r7, 0(r31)
0x8259BCCC:  slw          r11, r6, r30
0x8259BCD0:  add          r29, r11, r29
0x8259BCD4:  bge          0x8259bce0
0x8259BCD8:  mr           r3, r31
0x8259BCDC:  bl           0x824d2748
0x8259BCE0:  lwz          r11, 8(r31)
0x8259BCE4:  addi         r11, r11, 0x10
0x8259BCE8:  cmplw        cr6, r30, r11
0x8259BCEC:  bgt          cr6, 0x8259bc94
0x8259BCF0:  subfic       r4, r30, 0x40
0x8259BCF4:  ld           r11, 0(r31)
0x8259BCF8:  clrldi       r3, r30, 0x20
0x8259BCFC:  lwz          r5, 8(r31)
0x8259BD00:  clrldi       r9, r4, 0x20
0x8259BD04:  subf.        r10, r30, r5
0x8259BD08:  stw          r10, 8(r31)
0x8259BD0C:  sld          r7, r11, r3
0x8259BD10:  srd          r8, r11, r9
0x8259BD14:  slwi         r11, r8, 0
0x8259BD18:  add          r30, r11, r29
0x8259BD1C:  std          r7, 0(r31)
0x8259BD20:  bge          0x8259bd2c
0x8259BD24:  mr           r3, r31
0x8259BD28:  bl           0x824d2748
0x8259BD2C:  mr           r11, r30
0x8259BD30:  lwz          r3, 0(r21)
0x8259BD34:  clrlwi       r31, r11, 0x10
0x8259BD38:  ld           r11, 0(r3)
0x8259BD3C:  lwz          r6, 8(r3)
0x8259BD40:  sldi         r5, r11, 1
0x8259BD44:  rldicl       r4, r11, 1, 0x3f
0x8259BD48:  addic.       r11, r6, -1
0x8259BD4C:  slwi         r30, r4, 0
0x8259BD50:  std          r5, 0(r3)
0x8259BD54:  stw          r11, 8(r3)
0x8259BD58:  bge          0x8259bd60
0x8259BD5C:  bl           0x824d2748
0x8259BD60:  cmplwi       cr6, r30, 0
0x8259BD64:  beq          cr6, 0x8259bd74
0x8259BD68:  clrlwi       r3, r31, 0x10
0x8259BD6C:  neg          r11, r3
0x8259BD70:  b            0x8259bd78
0x8259BD74:  clrlwi       r11, r31, 0x10
0x8259BD78:  extsh        r27, r11
0x8259BD7C:  sth          r27, 0(r25)
0x8259BD80:  lwz          r11, 0(r21)
0x8259BD84:  lwz          r10, 0x14(r11)
0x8259BD88:  cmpwi        cr6, r10, 0
0x8259BD8C:  bne          cr6, 0x8259c3b0
0x8259BD90:  cmpwi        cr6, r20, 0
0x8259BD94:  beq          cr6, 0x8259bdb4
0x8259BD98:  mr           r5, r25
0x8259BD9C:  lwz          r4, 0(r24)
0x8259BDA0:  mr           r3, r21
0x8259BDA4:  lwz          r6, 0xb8(r21)
0x8259BDA8:  bl           0x8258b9c8
0x8259BDAC:  cmpwi        cr6, r3, 0
0x8259BDB0:  blt          cr6, 0x8259c3b0
0x8259BDB4:  addi         r22, r22, 1
0x8259BDB8:  rlwinm       r19, r19, 0x1f, 0x19, 0x1f
0x8259BDBC:  cmpwi        cr6, r22, 6
0x8259BDC0:  blt          cr6, 0x8259b978
0x8259BDC4:  lwz          r9, 0(r16)
0x8259BDC8:  lbz          r8, 4(r16)
0x8259BDCC:  clrlwi       r11, r9, 1
0x8259BDD0:  lbz          r7, 5(r16)
0x8259BDD4:  rlwinm       r6, r11, 0, 0x1b, 0x1c
0x8259BDD8:  cntlzw       r5, r6
0x8259BDDC:  stw          r11, 0(r16)
0x8259BDE0:  rlwinm       r5, r5, 1, 0x19, 0x19
0x8259BDE4:  lwz          r11, 0x7c(r21)
0x8259BDE8:  lwz          r9, 0x33c(r21)
0x8259BDEC:  or           r4, r5, r8
0x8259BDF0:  slwi         r6, r11, 3
0x8259BDF4:  sldi         r3, r4, 8
0x8259BDF8:  or           r11, r3, r7
0x8259BDFC:  sldi         r10, r11, 0x30
0x8259BE00:  stdx         r10, r6, r9
0x8259BE04:  b            0x8259c2c8
0x8259BE08:  lbz          r10, 0x15(r21)
0x8259BE0C:  mr           r24, r14
0x8259BE10:  lwz          r11, 0(r16)
0x8259BE14:  lbz          r25, 0x1a(r21)
0x8259BE18:  mr           r27, r10
0x8259BE1C:  rlwinm       r23, r11, 0xc, 0x1e, 0x1f
0x8259BE20:  rlwinm       r18, r11, 4, 0x1f, 0x1f
0x8259BE24:  cmplwi       cr6, r10, 0
0x8259BE28:  beq          cr6, 0x8259be30
0x8259BE2C:  rlwinm       r25, r11, 8, 0x1d, 0x1f
0x8259BE30:  lbz          r9, 0x14(r21)
0x8259BE34:  cmplwi       cr6, r9, 0
0x8259BE38:  beq          cr6, 0x8259be4c
0x8259BE3C:  lwz          r10, 0x98(r21)
0x8259BE40:  rlwinm       r11, r11, 0xc, 0x1c, 0x1d
0x8259BE44:  add          r22, r11, r10
0x8259BE48:  b            0x8259be50
0x8259BE4C:  lwz          r22, 0xa0(r21)
0x8259BE50:  mr           r20, r14
0x8259BE54:  clrlwi       r8, r19, 0x1f
0x8259BE58:  cmplwi       cr6, r8, 0
0x8259BE5C:  beq          cr6, 0x8259c24c
0x8259BE60:  addi         r7, r27, -1
0x8259BE64:  and          r6, r7, r18
0x8259BE68:  cmplwi       cr6, r6, 0
0x8259BE6C:  beq          cr6, 0x8259bfb8
0x8259BE70:  lwz          r31, 0(r21)
0x8259BE74:  lwz          r5, 0x110(r21)
0x8259BE78:  ld           r10, 0(r31)
0x8259BE7C:  lwz          r28, 0(r5)
0x8259BE80:  rldicl       r4, r10, 6, 0x3a
0x8259BE84:  slwi         r3, r4, 1
0x8259BE88:  lhzx         r11, r3, r28
0x8259BE8C:  extsh        r30, r11
0x8259BE90:  cmpwi        cr6, r30, 0
0x8259BE94:  blt          cr6, 0x8259bf58
0x8259BE98:  clrlwi       r11, r30, 0x1c
0x8259BE9C:  lwz          r9, 8(r31)
0x8259BEA0:  mr           r8, r11
0x8259BEA4:  subf.        r11, r11, r9
0x8259BEA8:  stw          r11, 8(r31)
0x8259BEAC:  sld          r7, r10, r8
0x8259BEB0:  std          r7, 0(r31)
0x8259BEB4:  bge          0x8259bf50
0x8259BEB8:  lwz          r10, 0x10(r31)
0x8259BEBC:  lwz          r11, 0xc(r31)
0x8259BEC0:  addi         r6, r10, -4
0x8259BEC4:  cmplw        cr6, r11, r6
0x8259BEC8:  blt          cr6, 0x8259bee4
0x8259BECC:  mr           r3, r31
0x8259BED0:  bl           0x824d2688
0x8259BED4:  cmplwi       cr6, r3, 1
0x8259BED8:  beq          cr6, 0x8259beb8
0x8259BEDC:  srawi        r30, r30, 4
0x8259BEE0:  b            0x8259bf98
0x8259BEE4:  lbz          r3, 0(r11)
0x8259BEE8:  addi         r4, r11, 6
0x8259BEEC:  lbz          r8, 1(r11)
0x8259BEF0:  rldicr       r9, r3, 8, 0x3f
0x8259BEF4:  lbz          r6, 3(r11)
0x8259BEF8:  lbz          r7, 4(r11)
0x8259BEFC:  add          r9, r9, r8
0x8259BF00:  lbz          r5, 2(r11)
0x8259BF04:  lbz          r8, 5(r11)
0x8259BF08:  sldi         r11, r9, 8
0x8259BF0C:  stw          r4, 0xc(r31)
0x8259BF10:  lwz          r10, 8(r31)
0x8259BF14:  add          r5, r11, r5
0x8259BF18:  ld           r9, 0(r31)
0x8259BF1C:  sldi         r11, r5, 8
0x8259BF20:  add          r4, r11, r6
0x8259BF24:  sldi         r11, r4, 8
0x8259BF28:  add          r3, r11, r7
0x8259BF2C:  addi         r7, r10, 0x30
0x8259BF30:  sldi         r11, r3, 8
0x8259BF34:  add          r11, r11, r8
0x8259BF38:  neg          r8, r10
0x8259BF3C:  stw          r7, 8(r31)
0x8259BF40:  extsw        r6, r8
0x8259BF44:  sld          r11, r11, r6
0x8259BF48:  add          r5, r11, r9
0x8259BF4C:  std          r5, 0(r31)
0x8259BF50:  srawi        r30, r30, 4
0x8259BF54:  b            0x8259bf98
0x8259BF58:  li           r4, 6
0x8259BF5C:  mr           r3, r31
0x8259BF60:  bl           0x824d2818
0x8259BF64:  ld           r11, 0(r31)
0x8259BF68:  li           r4, 1
0x8259BF6C:  mr           r3, r31
0x8259BF70:  rldicl       r10, r11, 1, 0x3f
0x8259BF74:  slwi         r29, r10, 0
0x8259BF78:  bl           0x824d2818
0x8259BF7C:  add          r9, r29, r30
0x8259BF80:  add          r8, r9, r15
0x8259BF84:  slwi         r7, r8, 1
0x8259BF88:  lhzx         r6, r7, r28
0x8259BF8C:  extsh        r30, r6
0x8259BF90:  cmpwi        cr6, r30, 0
0x8259BF94:  blt          cr6, 0x8259bf64
0x8259BF98:  lwz          r5, 0(r21)
0x8259BF9C:  lwz          r4, 0x14(r5)
0x8259BFA0:  cmpwi        cr6, r4, 0
0x8259BFA4:  bne          cr6, 0x8259c3b0
0x8259BFA8:  add          r3, r30, r21
0x8259BFAC:  add          r11, r30, r21
0x8259BFB0:  lbz          r25, 0x15c(r3)
0x8259BFB4:  lbz          r23, 0x164(r11)
0x8259BFB8:  cmpwi        cr6, r25, 0
0x8259BFBC:  bne          cr6, 0x8259c000
0x8259BFC0:  lwz          r31, 0x2a4(r21)
0x8259BFC4:  mr           r3, r21
0x8259BFC8:  lwz          r4, 0(r22)
0x8259BFCC:  mr           r6, r31
0x8259BFD0:  lbz          r5, 0x1f0(r21)
0x8259BFD4:  bl           0x8258b608
0x8259BFD8:  rlwinm       r11, r3, 1, 0x18, 0x1e
0x8259BFDC:  lwz          r10, 0x2a8(r21)
0x8259BFE0:  ori          r24, r24, 1
0x8259BFE4:  add          r8, r11, r31
0x8259BFE8:  stw          r8, 0x2a4(r21)
0x8259BFEC:  stb          r3, 0(r10)
0x8259BFF0:  lwz          r11, 0x2a8(r21)
0x8259BFF4:  addi         r7, r11, 1
0x8259BFF8:  stw          r7, 0x2a8(r21)
0x8259BFFC:  b            0x8259c248
0x8259C000:  cmpwi        cr6, r25, 2
0x8259C004:  bgt          cr6, 0x8259c08c
0x8259C008:  or           r6, r18, r27
0x8259C00C:  cmplwi       cr6, r6, 0
0x8259C010:  beq          cr6, 0x8259c01c
0x8259C014:  mr           r11, r23
0x8259C018:  b            0x8259c1c8
0x8259C01C:  lwz          r3, 0(r21)
0x8259C020:  ld           r11, 0(r3)
0x8259C024:  lwz          r5, 8(r3)
0x8259C028:  sldi         r4, r11, 1
0x8259C02C:  rldicl       r10, r11, 1, 0x3f
0x8259C030:  addic.       r11, r5, -1
0x8259C034:  slwi         r31, r10, 0
0x8259C038:  std          r4, 0(r3)
0x8259C03C:  stw          r11, 8(r3)
0x8259C040:  bge          0x8259c048
0x8259C044:  bl           0x824d2748
0x8259C048:  cmplwi       cr6, r31, 1
0x8259C04C:  bne          cr6, 0x8259c084
0x8259C050:  lwz          r3, 0(r21)
0x8259C054:  ld           r11, 0(r3)
0x8259C058:  lwz          r9, 8(r3)
0x8259C05C:  sldi         r8, r11, 1
0x8259C060:  rldicl       r7, r11, 1, 0x3f
0x8259C064:  addic.       r11, r9, -1
0x8259C068:  slwi         r31, r7, 0
0x8259C06C:  std          r8, 0(r3)
0x8259C070:  stw          r11, 8(r3)
0x8259C074:  bge          0x8259c07c
0x8259C078:  bl           0x824d2748
0x8259C07C:  addi         r11, r31, 1
0x8259C080:  b            0x8259c1c8
0x8259C084:  li           r11, 3
0x8259C088:  b            0x8259c1c8
0x8259C08C:  lwz          r31, 0(r21)
0x8259C090:  lwz          r6, 0x114(r21)
0x8259C094:  ld           r10, 0(r31)
0x8259C098:  lwz          r28, 0(r6)
0x8259C09C:  rldicl       r5, r10, 6, 0x3a
0x8259C0A0:  slwi         r4, r5, 1
0x8259C0A4:  lhzx         r3, r4, r28
0x8259C0A8:  extsh        r30, r3
0x8259C0AC:  cmpwi        cr6, r30, 0
0x8259C0B0:  blt          cr6, 0x8259c174
0x8259C0B4:  clrlwi       r11, r30, 0x1c
0x8259C0B8:  lwz          r9, 8(r31)
0x8259C0BC:  mr           r8, r11
0x8259C0C0:  subf.        r11, r11, r9
0x8259C0C4:  stw          r11, 8(r31)
0x8259C0C8:  sld          r7, r10, r8
0x8259C0CC:  std          r7, 0(r31)
0x8259C0D0:  bge          0x8259c16c
0x8259C0D4:  lwz          r10, 0x10(r31)
0x8259C0D8:  lwz          r11, 0xc(r31)
0x8259C0DC:  addi         r6, r10, -4
0x8259C0E0:  cmplw        cr6, r11, r6
0x8259C0E4:  blt          cr6, 0x8259c100
0x8259C0E8:  mr           r3, r31
0x8259C0EC:  bl           0x824d2688
0x8259C0F0:  cmplwi       cr6, r3, 1
0x8259C0F4:  beq          cr6, 0x8259c0d4
0x8259C0F8:  srawi        r30, r30, 4
0x8259C0FC:  b            0x8259c1b4
0x8259C100:  lbz          r3, 0(r11)
0x8259C104:  addi         r4, r11, 6
0x8259C108:  lbz          r8, 1(r11)
0x8259C10C:  rldicr       r9, r3, 8, 0x3f
0x8259C110:  lbz          r6, 3(r11)
0x8259C114:  lbz          r7, 4(r11)
0x8259C118:  add          r9, r9, r8
0x8259C11C:  lbz          r5, 2(r11)
0x8259C120:  lbz          r8, 5(r11)
0x8259C124:  sldi         r11, r9, 8
0x8259C128:  stw          r4, 0xc(r31)
0x8259C12C:  lwz          r10, 8(r31)
0x8259C130:  add          r5, r11, r5
0x8259C134:  ld           r9, 0(r31)
0x8259C138:  sldi         r11, r5, 8
0x8259C13C:  add          r4, r11, r6
0x8259C140:  sldi         r11, r4, 8
0x8259C144:  add          r3, r11, r7
0x8259C148:  addi         r7, r10, 0x30
0x8259C14C:  sldi         r11, r3, 8
0x8259C150:  add          r11, r11, r8
0x8259C154:  neg          r8, r10
0x8259C158:  stw          r7, 8(r31)
0x8259C15C:  extsw        r6, r8
0x8259C160:  sld          r11, r11, r6
0x8259C164:  add          r5, r11, r9
0x8259C168:  std          r5, 0(r31)
0x8259C16C:  srawi        r30, r30, 4
0x8259C170:  b            0x8259c1b4
0x8259C174:  li           r4, 6
0x8259C178:  mr           r3, r31
0x8259C17C:  bl           0x824d2818
0x8259C180:  ld           r11, 0(r31)
0x8259C184:  li           r4, 1
0x8259C188:  mr           r3, r31
0x8259C18C:  rldicl       r10, r11, 1, 0x3f
0x8259C190:  slwi         r29, r10, 0
0x8259C194:  bl           0x824d2818
0x8259C198:  add          r9, r29, r30
0x8259C19C:  add          r8, r9, r15
0x8259C1A0:  slwi         r7, r8, 1
0x8259C1A4:  lhzx         r6, r7, r28
0x8259C1A8:  extsh        r30, r6
0x8259C1AC:  cmpwi        cr6, r30, 0
0x8259C1B0:  blt          cr6, 0x8259c180
0x8259C1B4:  lwz          r5, 0(r21)
0x8259C1B8:  addi         r11, r30, 1
0x8259C1BC:  lwz          r4, 0x14(r5)
0x8259C1C0:  cmpwi        cr6, r4, 0
0x8259C1C4:  bne          cr6, 0x8259c3b0
0x8259C1C8:  add          r3, r11, r21
0x8259C1CC:  lwz          r28, 0(r22)
0x8259C1D0:  slwi         r10, r25, 4
0x8259C1D4:  lwz          r26, 0x2a8(r21)
0x8259C1D8:  extsw        r9, r11
0x8259C1DC:  lwz          r30, 0x2a4(r21)
0x8259C1E0:  extsw        r8, r10
0x8259C1E4:  add          r7, r25, r21
0x8259C1E8:  lbz          r29, 0x54(r3)
0x8259C1EC:  or           r6, r8, r9
0x8259C1F0:  mr           r31, r14
0x8259C1F4:  or           r24, r6, r24
0x8259C1F8:  cmpwi        cr6, r29, 0
0x8259C1FC:  lbz          r27, 0x1f0(r7)
0x8259C200:  ble          cr6, 0x8259c230
0x8259C204:  mr           r6, r30
0x8259C208:  mr           r5, r27
0x8259C20C:  mr           r4, r28
0x8259C210:  mr           r3, r21
0x8259C214:  bl           0x8258b608
0x8259C218:  stbx         r3, r31, r26
0x8259C21C:  rlwinm       r11, r3, 1, 0x18, 0x1e
0x8259C220:  addi         r31, r31, 1
0x8259C224:  add          r30, r11, r30
0x8259C228:  cmpw         cr6, r31, r29
0x8259C22C:  blt          cr6, 0x8259c204
0x8259C230:  cmpwi        cr6, r30, -1
0x8259C234:  beq          cr6, 0x8259c3b0
0x8259C238:  lwz          r11, 0x2a8(r21)
0x8259C23C:  stw          r30, 0x2a4(r21)
0x8259C240:  add          r4, r29, r11
0x8259C244:  stw          r4, 0x2a8(r21)
0x8259C248:  mr           r27, r14
0x8259C24C:  addi         r20, r20, 1
0x8259C250:  rlwinm       r19, r19, 0x1f, 0x19, 0x1f
0x8259C254:  sldi         r24, r24, 8
0x8259C258:  cmpwi        cr6, r20, 6
0x8259C25C:  blt          cr6, 0x8259be54
0x8259C260:  lwz          r11, 0(r16)
0x8259C264:  rldicl       r8, r24, 0x38, 8
0x8259C268:  lwz          r5, 0x7c(r21)
0x8259C26C:  rldicr       r6, r11, 0, 0x20
0x8259C270:  lbz          r3, 4(r16)
0x8259C274:  rlwinm       r11, r11, 0x1b, 0x1d, 0x1f
0x8259C278:  lbz          r9, 5(r16)
0x8259C27C:  slwi         r4, r5, 3
0x8259C280:  lwz          r7, 0x33c(r21)
0x8259C284:  rlwinm       r10, r11, 5, 0x13, 0x18
0x8259C288:  slwi         r5, r11, 6
0x8259C28C:  mr           r11, r10
0x8259C290:  mr           r10, r5
0x8259C294:  clrlwi       r5, r11, 0x18
0x8259C298:  clrlwi       r11, r10, 0x18
0x8259C29C:  mr           r10, r5
0x8259C2A0:  mr           r5, r11
0x8259C2A4:  or           r3, r10, r3
0x8259C2A8:  sldi         r11, r3, 8
0x8259C2AC:  or           r10, r11, r5
0x8259C2B0:  or           r9, r10, r9
0x8259C2B4:  sldi         r5, r9, 0x20
0x8259C2B8:  or           r3, r5, r6
0x8259C2BC:  sldi         r11, r3, 0x10
0x8259C2C0:  or           r10, r11, r8
0x8259C2C4:  stdx         r10, r4, r7
0x8259C2C8:  lwz          r9, 0x54(r1)
0x8259C2CC:  addi         r16, r16, 0x14
0x8259C2D0:  lwz          r11, 0x7c(r21)
0x8259C2D4:  lis          r17, 1
0x8259C2D8:  addi         r8, r9, 1
0x8259C2DC:  lwz          r6, 0x50(r1)
0x8259C2E0:  addi         r7, r11, 1
0x8259C2E4:  lhz          r11, 0xf6(r21)
0x8259C2E8:  addi         r5, r6, 1
0x8259C2EC:  lwz          r9, 0x78(r21)
0x8259C2F0:  addi         r4, r11, 2
0x8259C2F4:  lwz          r10, 0xec(r21)
0x8259C2F8:  lwz          r11, 0xf0(r21)
0x8259C2FC:  addi         r3, r9, 2
0x8259C300:  stw          r8, 0x54(r1)
0x8259C304:  stw          r7, 0x7c(r21)
0x8259C308:  addi         r6, r11, 8
0x8259C30C:  stw          r5, 0x50(r1)
0x8259C310:  addi         r7, r10, 0x10
0x8259C314:  sth          r4, 0xf6(r21)
0x8259C318:  stw          r3, 0x78(r21)
0x8259C31C:  stw          r6, 0xf0(r21)
0x8259C320:  stw          r7, 0xec(r21)
0x8259C324:  lwz          r5, 0x124(r1)
0x8259C328:  lwz          r4, 0x84(r5)
0x8259C32C:  cmpw         cr6, r8, r4
0x8259C330:  blt          cr6, 0x8259a4b4
0x8259C334:  lwz          r28, 0x58(r1)
0x8259C338:  slwi         r29, r5, 0
0x8259C33C:  lwz          r26, 0x64(r1)
0x8259C340:  lwz          r27, 0x60(r1)
0x8259C344:  lwz          r3, 0x5c(r1)
0x8259C348:  addi         r28, r28, 1
0x8259C34C:  lwz          r10, 0x78(r21)
0x8259C350:  addi         r11, r3, 0x10
0x8259C354:  stw          r28, 0x58(r1)
0x8259C358:  stw          r11, 0x5c(r1)
0x8259C35C:  lhz          r11, 0xf4(r21)
0x8259C360:  addi         r9, r11, 2
0x8259C364:  sth          r9, 0xf4(r21)
0x8259C368:  lwz          r7, 0x84(r29)
0x8259C36C:  slwi         r11, r7, 1
0x8259C370:  add          r6, r11, r10
0x8259C374:  stw          r6, 0x78(r21)
0x8259C378:  lwz          r10, 0xe0(r29)
0x8259C37C:  lwz          r11, 0xe4(r29)
0x8259C380:  add          r27, r10, r27
0x8259C384:  lwz          r5, 0x88(r29)
0x8259C388:  add          r26, r11, r26
0x8259C38C:  cmpw         cr6, r28, r5
0x8259C390:  stw          r27, 0x60(r1)
0x8259C394:  stw          r26, 0x64(r1)
0x8259C398:  bge          cr6, 0x8259c3bc
0x8259C39C:  li           r25, 1
0x8259C3A0:  b            0x8259a3b4
0x8259C3A4:  extsw        r3, r11
0x8259C3A8:  addi         r1, r1, 0x110
0x8259C3AC:  b            0x8242f8b0
0x8259C3B0:  li           r3, 4
0x8259C3B4:  addi         r1, r1, 0x110
0x8259C3B8:  b            0x8242f8b0
0x8259C3BC:  lwz          r10, 0x2b4(r21)
0x8259C3C0:  li           r11, -1
0x8259C3C4:  lwz          r4, 0x4d9c(r29)
0x8259C3C8:  li           r3, 0
0x8259C3CC:  subf         r9, r4, r10
0x8259C3D0:  srwi         r8, r9, 2
0x8259C3D4:  stw          r8, 0x4da0(r29)
0x8259C3D8:  lwz          r7, 0x2b4(r21)
0x8259C3DC:  stw          r11, 0(r7)
0x8259C3E0:  addi         r1, r1, 0x110
0x8259C3E4:  b            0x8242f8b0
0x8259C3E8:  .align 0                       # padding
0x8259C3EC:  .align 0                       # padding
0x8259C3F0:  .align 0                       # padding
0x8259C3F4:  .align 0                       # padding
0x8259C3F8:  .align 0                       # padding
0x8259C3FC:  .align 0                       # padding
