# Section: BINK
# Base Address: 0x8259C400
# Size: 0x0000FDBC

.section BINK

0x8259C400:  mflr         r12
0x8259C404:  stw          r12, -8(r1)
0x8259C408:  std          r30, -0x18(r1)
0x8259C40C:  std          r31, -0x10(r1)
0x8259C410:  stwu         r1, -0x70(r1)
0x8259C414:  mr           r31, r3
0x8259C418:  mr           r30, r4
0x8259C41C:  bl           0x8259f898
0x8259C420:  mr           r11, r3
0x8259C424:  lwz          r10, 0x290(r31)
0x8259C428:  mr           r3, r30
0x8259C42C:  add          r11, r11, r30
0x8259C430:  add          r11, r11, r10
0x8259C434:  stw          r11, 0x290(r31)
0x8259C438:  bl           0x8259f8a8
0x8259C43C:  addi         r1, r1, 0x70
0x8259C440:  lwz          r12, -8(r1)
0x8259C444:  mtlr         r12
0x8259C448:  ld           r30, -0x18(r1)
0x8259C44C:  ld           r31, -0x10(r1)
0x8259C450:  blr          
0x8259C454:  .align 0                       # padding
0x8259C458:  mflr         r12
0x8259C45C:  stw          r12, -8(r1)
0x8259C460:  std          r31, -0x10(r1)
0x8259C464:  stwu         r1, -0x60(r1)
0x8259C468:  mr           r10, r3
0x8259C46C:  cmplwi       cr6, r10, 0
0x8259C470:  beq          cr6, 0x8259c4d4
0x8259C474:  lis          r11, -0x7d8c
0x8259C478:  addi         r31, r11, -0x6200
0x8259C47C:  lwz          r11, 0(r31)
0x8259C480:  cmplwi       cr6, r11, 0
0x8259C484:  beq          cr6, 0x8259c49c
0x8259C488:  cmplw        cr6, r11, r10
0x8259C48C:  beq          cr6, 0x8259c4a0
0x8259C490:  lwz          r11, 8(r31)
0x8259C494:  cmpwi        cr6, r11, 0
0x8259C498:  bne          cr6, 0x8259c4d4
0x8259C49C:  stw          r10, 0(r31)
0x8259C4A0:  lwz          r11, 0(r31)
0x8259C4A4:  mr           r3, r4
0x8259C4A8:  mtctr        r11
0x8259C4AC:  bctrl        
0x8259C4B0:  mr           r11, r3
0x8259C4B4:  cmplwi       cr6, r11, 0
0x8259C4B8:  beq          cr6, 0x8259c4c4
0x8259C4BC:  stw          r11, 4(r31)
0x8259C4C0:  b            0x8259c4c8
0x8259C4C4:  lwz          r11, 4(r31)
0x8259C4C8:  cmplwi       cr6, r11, 0
0x8259C4CC:  li           r3, 1
0x8259C4D0:  bne          cr6, 0x8259c4d8
0x8259C4D4:  li           r3, 0
0x8259C4D8:  addi         r1, r1, 0x60
0x8259C4DC:  lwz          r12, -8(r1)
0x8259C4E0:  mtlr         r12
0x8259C4E4:  ld           r31, -0x10(r1)
0x8259C4E8:  blr          
0x8259C4EC:  .align 0                       # padding
0x8259C4F0:  mflr         r12
0x8259C4F4:  bl           0x8242f890
0x8259C4F8:  stwu         r1, -0x90(r1)
0x8259C4FC:  mr           r31, r3
0x8259C500:  lwz          r11, 0x2e8(r31)
0x8259C504:  lwz          r10, 0x2f0(r31)
0x8259C508:  mulli        r11, r11, 0x17c
0x8259C50C:  add          r10, r11, r10
0x8259C510:  lwz          r9, -0x17c(r10)
0x8259C514:  cmplwi       cr6, r9, 0
0x8259C518:  beq          cr6, 0x8259c858
0x8259C51C:  lis          r26, -0x7d8c
0x8259C520:  addi         r27, r31, 0x340
0x8259C524:  mr           r4, r27
0x8259C528:  lwz          r3, -0x61f0(r26)
0x8259C52C:  bl           0x8259ff98
0x8259C530:  cmpwi        cr6, r3, 0
0x8259C534:  beq          cr6, 0x8259c858
0x8259C538:  lwz          r8, 0x2e8(r31)
0x8259C53C:  li           r28, 0
0x8259C540:  cmplwi       cr6, r8, 0
0x8259C544:  ble          cr6, 0x8259c84c
0x8259C548:  li           r29, 0
0x8259C54C:  lwz          r11, 0x2f0(r31)
0x8259C550:  add          r3, r29, r11
0x8259C554:  lwz          r7, 0(r3)
0x8259C558:  mtctr        r7
0x8259C55C:  bctrl        
0x8259C560:  cmpwi        cr6, r3, 0
0x8259C564:  beq          cr6, 0x8259c838
0x8259C568:  lwz          r11, 0x2f0(r31)
0x8259C56C:  add          r11, r29, r11
0x8259C570:  lwz          r6, 0x40(r11)
0x8259C574:  lwz          r5, 0x54(r11)
0x8259C578:  cmplw        cr6, r6, r5
0x8259C57C:  bgt          cr6, 0x8259c5a4
0x8259C580:  lwz          r11, 0x2f0(r31)
0x8259C584:  lwz          r4, 0xc(r31)
0x8259C588:  add          r11, r29, r11
0x8259C58C:  lwz          r3, 0x48(r11)
0x8259C590:  cmpw         cr6, r4, r3
0x8259C594:  ble          cr6, 0x8259c838
0x8259C598:  lwz          r11, 0x40(r11)
0x8259C59C:  cmplwi       cr6, r11, 0
0x8259C5A0:  beq          cr6, 0x8259c838
0x8259C5A4:  lwz          r11, 0x2f0(r31)
0x8259C5A8:  addi         r5, r1, 0x50
0x8259C5AC:  addi         r4, r1, 0x54
0x8259C5B0:  add          r3, r29, r11
0x8259C5B4:  lwz          r10, 4(r3)
0x8259C5B8:  mtctr        r10
0x8259C5BC:  bctrl        
0x8259C5C0:  cmpwi        cr6, r3, 0
0x8259C5C4:  beq          cr6, 0x8259c838
0x8259C5C8:  lwz          r11, 0x2f0(r31)
0x8259C5CC:  add          r9, r29, r11
0x8259C5D0:  lwz          r8, 0x44(r9)
0x8259C5D4:  cmplwi       cr6, r8, 0
0x8259C5D8:  beq          cr6, 0x8259c5ec
0x8259C5DC:  lwz          r7, 0x50(r1)
0x8259C5E0:  slwi         r10, r7, 1
0x8259C5E4:  stw          r10, 0x50(r1)
0x8259C5E8:  b            0x8259c5f0
0x8259C5EC:  lwz          r10, 0x50(r1)
0x8259C5F0:  lwz          r11, 0x40(r9)
0x8259C5F4:  cmplw        cr6, r10, r11
0x8259C5F8:  ble          cr6, 0x8259c604
0x8259C5FC:  mr           r10, r11
0x8259C600:  stw          r10, 0x50(r1)
0x8259C604:  lwz          r6, 0xc(r31)
0x8259C608:  lwz          r5, 0x48(r9)
0x8259C60C:  cmpw         cr6, r6, r5
0x8259C610:  bgt          cr6, 0x8259c630
0x8259C614:  lwz          r11, 0x2f0(r31)
0x8259C618:  add          r11, r29, r11
0x8259C61C:  lwz          r4, 0x58(r11)
0x8259C620:  lwz          r3, 0x44(r11)
0x8259C624:  slw          r11, r4, r3
0x8259C628:  and          r10, r11, r10
0x8259C62C:  stw          r10, 0x50(r1)
0x8259C630:  lwz          r11, 0x2f0(r31)
0x8259C634:  add          r11, r29, r11
0x8259C638:  lwz          r9, 0x40(r11)
0x8259C63C:  subf         r8, r10, r9
0x8259C640:  stw          r8, 0x40(r11)
0x8259C644:  lwz          r11, 0x2f0(r31)
0x8259C648:  lwz          r5, 0x50(r1)
0x8259C64C:  add          r11, r29, r11
0x8259C650:  lwz          r7, 0x30(r11)
0x8259C654:  lwz          r6, 0x38(r11)
0x8259C658:  subf         r30, r6, r7
0x8259C65C:  cmplw        cr6, r30, r5
0x8259C660:  bge          cr6, 0x8259c770
0x8259C664:  cmplwi       cr6, r30, 0
0x8259C668:  beq          cr6, 0x8259c6e4
0x8259C66C:  lwz          r11, 0x2f0(r31)
0x8259C670:  add          r10, r29, r11
0x8259C674:  lwz          r5, 0x44(r10)
0x8259C678:  cmplwi       cr6, r5, 0
0x8259C67C:  beq          cr6, 0x8259c6c4
0x8259C680:  srwi         r11, r30, 1
0x8259C684:  lwz          r9, 0x38(r10)
0x8259C688:  lwz          r10, 0x54(r1)
0x8259C68C:  cmplwi       cr6, r11, 0
0x8259C690:  beq          cr6, 0x8259c6b8
0x8259C694:  mr           r4, r9
0x8259C698:  addi         r11, r11, -1
0x8259C69C:  addi         r9, r9, 2
0x8259C6A0:  cmplwi       cr6, r11, 0
0x8259C6A4:  lbz          r8, 0(r4)
0x8259C6A8:  addi         r3, r8, 0x80
0x8259C6AC:  stb          r3, 0(r10)
0x8259C6B0:  addi         r10, r10, 1
0x8259C6B4:  bne          cr6, 0x8259c694
0x8259C6B8:  mr           r9, r10
0x8259C6BC:  stw          r9, 0x54(r1)
0x8259C6C0:  b            0x8259c6e8
0x8259C6C4:  mr           r5, r30
0x8259C6C8:  lwz          r4, 0x38(r10)
0x8259C6CC:  lwz          r3, 0x54(r1)
0x8259C6D0:  bl           0x82434100
0x8259C6D4:  lwz          r7, 0x54(r1)
0x8259C6D8:  add          r9, r30, r7
0x8259C6DC:  stw          r9, 0x54(r1)
0x8259C6E0:  b            0x8259c6e8
0x8259C6E4:  lwz          r9, 0x54(r1)
0x8259C6E8:  lwz          r11, 0x2f0(r31)
0x8259C6EC:  add          r11, r29, r11
0x8259C6F0:  lwz          r6, 0x44(r11)
0x8259C6F4:  cmplwi       cr6, r6, 0
0x8259C6F8:  beq          cr6, 0x8259c73c
0x8259C6FC:  lwz          r5, 0x50(r1)
0x8259C700:  lwz          r10, 0x2c(r11)
0x8259C704:  subf         r4, r30, r5
0x8259C708:  srwi         r11, r4, 1
0x8259C70C:  cmplwi       cr6, r11, 0
0x8259C710:  beq          cr6, 0x8259c750
0x8259C714:  mr           r3, r10
0x8259C718:  addi         r11, r11, -1
0x8259C71C:  addi         r10, r10, 2
0x8259C720:  cmplwi       cr6, r11, 0
0x8259C724:  lbz          r8, 0(r3)
0x8259C728:  addi         r8, r8, 0x80
0x8259C72C:  stb          r8, 0(r9)
0x8259C730:  addi         r9, r9, 1
0x8259C734:  bne          cr6, 0x8259c714
0x8259C738:  b            0x8259c750
0x8259C73C:  lwz          r6, 0x50(r1)
0x8259C740:  mr           r3, r9
0x8259C744:  lwz          r4, 0x2c(r11)
0x8259C748:  subf         r5, r30, r6
0x8259C74C:  bl           0x82434100
0x8259C750:  lwz          r11, 0x2f0(r31)
0x8259C754:  lwz          r4, 0x50(r1)
0x8259C758:  add          r11, r29, r11
0x8259C75C:  lwz          r5, 0x2c(r11)
0x8259C760:  subf         r10, r30, r5
0x8259C764:  add          r3, r10, r4
0x8259C768:  stw          r3, 0x38(r11)
0x8259C76C:  b            0x8259c7e4
0x8259C770:  lwz          r11, 0x2f0(r31)
0x8259C774:  add          r10, r29, r11
0x8259C778:  lwz          r11, 0x44(r10)
0x8259C77C:  cmplwi       cr6, r11, 0
0x8259C780:  beq          cr6, 0x8259c7c0
0x8259C784:  srwi         r11, r5, 1
0x8259C788:  lwz          r10, 0x38(r10)
0x8259C78C:  lwz          r9, 0x54(r1)
0x8259C790:  cmplwi       cr6, r11, 0
0x8259C794:  beq          cr6, 0x8259c7cc
0x8259C798:  mr           r8, r10
0x8259C79C:  addi         r11, r11, -1
0x8259C7A0:  addi         r10, r10, 2
0x8259C7A4:  cmplwi       cr6, r11, 0
0x8259C7A8:  lbz          r8, 0(r8)
0x8259C7AC:  addi         r7, r8, 0x80
0x8259C7B0:  stb          r7, 0(r9)
0x8259C7B4:  addi         r9, r9, 1
0x8259C7B8:  bne          cr6, 0x8259c798
0x8259C7BC:  b            0x8259c7cc
0x8259C7C0:  lwz          r4, 0x38(r10)
0x8259C7C4:  lwz          r3, 0x54(r1)
0x8259C7C8:  bl           0x82434100
0x8259C7CC:  lwz          r11, 0x2f0(r31)
0x8259C7D0:  lwz          r5, 0x50(r1)
0x8259C7D4:  add          r11, r29, r11
0x8259C7D8:  lwz          r10, 0x38(r11)
0x8259C7DC:  add          r4, r10, r5
0x8259C7E0:  stw          r4, 0x38(r11)
0x8259C7E4:  lwz          r11, 0x2f0(r31)
0x8259C7E8:  add          r3, r29, r11
0x8259C7EC:  lwz          r11, 0x44(r3)
0x8259C7F0:  cmplwi       cr6, r11, 0
0x8259C7F4:  beq          cr6, 0x8259c804
0x8259C7F8:  lwz          r10, 0x50(r1)
0x8259C7FC:  srwi         r9, r10, 1
0x8259C800:  stw          r9, 0x50(r1)
0x8259C804:  lwz          r11, 0x2f0(r31)
0x8259C808:  lwz          r4, 0x50(r1)
0x8259C80C:  add          r3, r29, r11
0x8259C810:  lwz          r8, 8(r3)
0x8259C814:  mtctr        r8
0x8259C818:  bctrl        
0x8259C81C:  lwz          r11, 0x2f0(r31)
0x8259C820:  add          r3, r29, r11
0x8259C824:  lwz          r7, 0(r3)
0x8259C828:  mtctr        r7
0x8259C82C:  bctrl        
0x8259C830:  cmpwi        cr6, r3, 0
0x8259C834:  bne          cr6, 0x8259c568
0x8259C838:  lwz          r6, 0x2e8(r31)
0x8259C83C:  addi         r28, r28, 1
0x8259C840:  addi         r29, r29, 0x17c
0x8259C844:  cmplw        cr6, r28, r6
0x8259C848:  blt          cr6, 0x8259c54c
0x8259C84C:  mr           r4, r27
0x8259C850:  lwz          r3, -0x61f0(r26)
0x8259C854:  bl           0x825a0020
0x8259C858:  addi         r1, r1, 0x90
0x8259C85C:  b            0x8242f8e0
0x8259C860:  mflr         r12
0x8259C864:  stw          r12, -8(r1)
0x8259C868:  std          r30, -0x18(r1)
0x8259C86C:  std          r31, -0x10(r1)
0x8259C870:  stwu         r1, -0x70(r1)
0x8259C874:  mr           r30, r4
0x8259C878:  mr           r31, r3
0x8259C87C:  cmplwi       cr6, r30, 0
0x8259C880:  beq          cr6, 0x8259c888
0x8259C884:  addi         r30, r30, -1
0x8259C888:  li           r11, 0
0x8259C88C:  lwz          r10, 0x2e8(r31)
0x8259C890:  cmplwi       cr6, r10, 0
0x8259C894:  stw          r11, 0x2f8(r31)
0x8259C898:  beq          cr6, 0x8259c8a4
0x8259C89C:  mr           r3, r31
0x8259C8A0:  bl           0x8259c4f0
0x8259C8A4:  lwz          r9, 0xc(r31)
0x8259C8A8:  cmplwi       cr6, r30, 0
0x8259C8AC:  stw          r9, 0x10(r31)
0x8259C8B0:  bne          cr6, 0x8259c8ec
0x8259C8B4:  bl           0x825a03b8
0x8259C8B8:  lwz          r8, 0x2d8(r31)
0x8259C8BC:  cmplwi       cr6, r8, 0
0x8259C8C0:  bne          cr6, 0x8259c8c8
0x8259C8C4:  stw          r3, 0x2d8(r31)
0x8259C8C8:  lwz          r7, 0x2d8(r31)
0x8259C8CC:  lis          r6, 4
0x8259C8D0:  ori          r5, r6, 0x93e0
0x8259C8D4:  subf         r4, r7, r3
0x8259C8D8:  cmplw        cr6, r4, r5
0x8259C8DC:  ble          cr6, 0x8259c8ec
0x8259C8E0:  li           r11, 1
0x8259C8E4:  stw          r3, 0x2d8(r31)
0x8259C8E8:  stw          r11, 0x2dc(r31)
0x8259C8EC:  lwz          r10, 0xe8(r31)
0x8259C8F0:  lwz          r11, 0xec(r31)
0x8259C8F4:  cmplwi       cr6, r10, 0
0x8259C8F8:  beq          cr6, 0x8259c920
0x8259C8FC:  slwi         r9, r30, 2
0x8259C900:  lwz          r7, 0(r11)
0x8259C904:  rlwinm       r5, r7, 0, 0, 0x1e
0x8259C908:  lwzx         r8, r9, r11
0x8259C90C:  rlwinm       r6, r8, 0, 0, 0x1e
0x8259C910:  subf         r11, r5, r6
0x8259C914:  add          r4, r11, r10
0x8259C918:  stw          r4, 0xe4(r31)
0x8259C91C:  b            0x8259c958
0x8259C920:  slwi         r10, r30, 2
0x8259C924:  lwz          r6, 0xe4(r31)
0x8259C928:  lwz          r9, 0xf4(r31)
0x8259C92C:  mr           r4, r30
0x8259C930:  add          r11, r11, r10
0x8259C934:  addi         r3, r31, 0xf0
0x8259C938:  lwz          r10, 0(r11)
0x8259C93C:  lwz          r8, 4(r11)
0x8259C940:  rlwinm       r11, r10, 0, 0, 0x1e
0x8259C944:  rlwinm       r7, r8, 0, 0, 0x1e
0x8259C948:  rlwinm       r5, r10, 0, 0, 0x1e
0x8259C94C:  subf         r7, r11, r7
0x8259C950:  mtctr        r9
0x8259C954:  bctrl        
0x8259C958:  lwz          r10, 0x2e8(r31)
0x8259C95C:  cmplwi       cr6, r10, 0
0x8259C960:  beq          cr6, 0x8259c978
0x8259C964:  mr           r3, r31
0x8259C968:  bl           0x8259c4f0
0x8259C96C:  addi         r9, r30, 1
0x8259C970:  stw          r9, 0xc(r31)
0x8259C974:  b            0x8259c980
0x8259C978:  addi         r8, r30, 1
0x8259C97C:  stw          r8, 0xc(r31)
0x8259C980:  addi         r1, r1, 0x70
0x8259C984:  lwz          r12, -8(r1)
0x8259C988:  mtlr         r12
0x8259C98C:  ld           r30, -0x18(r1)
0x8259C990:  ld           r31, -0x10(r1)
0x8259C994:  blr          
0x8259C998:  mr           r10, r3
0x8259C99C:  cmplwi       cr6, r10, 8
0x8259C9A0:  ble          cr6, 0x8259c9a8
0x8259C9A4:  li           r10, 8
0x8259C9A8:  lis          r11, -0x7d8c
0x8259C9AC:  cmplwi       cr6, r10, 0
0x8259C9B0:  addi         r11, r11, -0x61dc
0x8259C9B4:  stw          r10, -4(r11)
0x8259C9B8:  beqlr        cr6
0x8259C9BC:  subf         r9, r11, r4
0x8259C9C0:  lwzx         r8, r9, r11
0x8259C9C4:  addi         r10, r10, -1
0x8259C9C8:  cmplwi       cr6, r10, 0
0x8259C9CC:  stw          r8, 0(r11)
0x8259C9D0:  addi         r11, r11, 4
0x8259C9D4:  bne          cr6, 0x8259c9c0
0x8259C9D8:  blr          
0x8259C9DC:  .align 0                       # padding
0x8259C9E0:  mflr         r12
0x8259C9E4:  bl           0x8242f894
0x8259C9E8:  mr           r30, r3
0x8259C9EC:  li           r3, 0
0x8259C9F0:  subf         r31, r5, r30
0x8259C9F4:  li           r29, 0
0x8259C9F8:  li           r28, 1
0x8259C9FC:  li           r11, 0
0x8259CA00:  cmpwi        cr6, r31, 0
0x8259CA04:  ble          cr6, 0x8259ca4c
0x8259CA08:  slwi         r10, r5, 2
0x8259CA0C:  mr           r8, r4
0x8259CA10:  add          r9, r10, r4
0x8259CA14:  lwz          r10, 0(r9)
0x8259CA18:  lwz          r27, 0(r8)
0x8259CA1C:  subf         r10, r27, r10
0x8259CA20:  cmplw        cr6, r10, r3
0x8259CA24:  ble          cr6, 0x8259ca30
0x8259CA28:  mr           r3, r10
0x8259CA2C:  mr           r29, r11
0x8259CA30:  addi         r11, r11, 1
0x8259CA34:  addi         r9, r9, 4
0x8259CA38:  addi         r8, r8, 4
0x8259CA3C:  cmpw         cr6, r11, r31
0x8259CA40:  blt          cr6, 0x8259ca14
0x8259CA44:  cmplwi       cr6, r3, 0
0x8259CA48:  bne          cr6, 0x8259ca70
0x8259CA4C:  slwi         r9, r30, 2
0x8259CA50:  lwz          r8, 0(r4)
0x8259CA54:  clrldi       r5, r5, 0x20
0x8259CA58:  clrldi       r3, r30, 0x20
0x8259CA5C:  lwzx         r11, r9, r4
0x8259CA60:  subf         r10, r8, r11
0x8259CA64:  mulld        r9, r10, r5
0x8259CA68:  divdu        r8, r9, r3
0x8259CA6C:  slwi         r3, r8, 0
0x8259CA70:  li           r11, 0
0x8259CA74:  cmpwi        cr6, r30, 0
0x8259CA78:  ble          cr6, 0x8259cab0
0x8259CA7C:  mr           r10, r4
0x8259CA80:  lwz          r5, 0(r10)
0x8259CA84:  clrlwi       r4, r5, 0x1f
0x8259CA88:  cmplwi       cr6, r4, 0
0x8259CA8C:  beq          cr6, 0x8259caac
0x8259CA90:  addi         r11, r11, 1
0x8259CA94:  addi         r10, r10, 4
0x8259CA98:  cmpw         cr6, r11, r30
0x8259CA9C:  blt          cr6, 0x8259ca80
0x8259CAA0:  stw          r28, 0(r7)
0x8259CAA4:  stw          r29, 0(r6)
0x8259CAA8:  b            0x8242f8e4
0x8259CAAC:  li           r28, 0
0x8259CAB0:  stw          r28, 0(r7)
0x8259CAB4:  stw          r29, 0(r6)
0x8259CAB8:  b            0x8242f8e4
0x8259CABC:  .align 0                       # padding
0x8259CAC0:  lwz          r10, -0xb8(r3)
0x8259CAC4:  lwz          r11, -0xbc(r3)
0x8259CAC8:  addi         r10, r10, 1
0x8259CACC:  addi         r9, r11, 1
0x8259CAD0:  mulli        r8, r10, 0x64
0x8259CAD4:  divwu        r11, r8, r9
0x8259CAD8:  cmplwi       cr6, r11, 0x32
0x8259CADC:  bge          cr6, 0x8259cae8
0x8259CAE0:  subfic       r3, r11, -1
0x8259CAE4:  blr          
0x8259CAE8:  lwz          r11, 0x144(r3)
0x8259CAEC:  cmplw        cr6, r11, r4
0x8259CAF0:  beq          cr6, 0x8259cb00
0x8259CAF4:  lis          r7, -0x8000
0x8259CAF8:  subf         r3, r11, r7
0x8259CAFC:  blr          
0x8259CB00:  li           r3, 0
0x8259CB04:  blr          
0x8259CB08:  mflr         r12
0x8259CB0C:  stw          r12, -8(r1)
0x8259CB10:  std          r30, -0x18(r1)
0x8259CB14:  std          r31, -0x10(r1)
0x8259CB18:  stwu         r1, -0x70(r1)
0x8259CB1C:  addi         r31, r3, -0x1f4
0x8259CB20:  mr           r30, r4
0x8259CB24:  addi         r3, r31, 0xf0
0x8259CB28:  lwz          r11, 0x100(r31)
0x8259CB2C:  mtctr        r11
0x8259CB30:  bctrl        
0x8259CB34:  stw          r30, 0x338(r31)
0x8259CB38:  addi         r1, r1, 0x70
0x8259CB3C:  lwz          r12, -8(r1)
0x8259CB40:  mtlr         r12
0x8259CB44:  ld           r30, -0x18(r1)
0x8259CB48:  ld           r31, -0x10(r1)
0x8259CB4C:  blr          
0x8259CB50:  lis          r11, -0x7d8c
0x8259CB54:  addi         r4, r3, 0x104
0x8259CB58:  lwz          r3, -0x61f4(r11)
0x8259CB5C:  b            0x8259fff0
0x8259CB60:  lis          r11, -0x7d8c
0x8259CB64:  addi         r4, r3, 0x104
0x8259CB68:  lwz          r3, -0x61f4(r11)
0x8259CB6C:  b            0x8259ff98
0x8259CB70:  lis          r11, -0x7d8c
0x8259CB74:  addi         r4, r3, 0x104
0x8259CB78:  lwz          r3, -0x61f4(r11)
0x8259CB7C:  b            0x825a0020
0x8259CB80:  b            0x825a00f8
0x8259CB84:  .align 0                       # padding
0x8259CB88:  lwz          r11, -4(r3)
0x8259CB8C:  cmplw        cr6, r11, r4
0x8259CB90:  subfic       r3, r11, -1
0x8259CB94:  bnelr        cr6
0x8259CB98:  li           r3, 0
0x8259CB9C:  blr          
0x8259CBA0:  mflr         r12
0x8259CBA4:  stw          r12, -8(r1)
0x8259CBA8:  std          r30, -0x18(r1)
0x8259CBAC:  std          r31, -0x10(r1)
0x8259CBB0:  stwu         r1, -0x70(r1)
0x8259CBB4:  addi         r31, r3, -0x340
0x8259CBB8:  mr           r30, r4
0x8259CBBC:  lwz          r11, 0x2e8(r31)
0x8259CBC0:  cmplwi       cr6, r11, 0
0x8259CBC4:  beq          cr6, 0x8259cbd0
0x8259CBC8:  mr           r3, r31
0x8259CBCC:  bl           0x8259c4f0
0x8259CBD0:  stw          r30, 0x33c(r31)
0x8259CBD4:  addi         r1, r1, 0x70
0x8259CBD8:  lwz          r12, -8(r1)
0x8259CBDC:  mtlr         r12
0x8259CBE0:  ld           r30, -0x18(r1)
0x8259CBE4:  ld           r31, -0x10(r1)
0x8259CBE8:  blr          
0x8259CBEC:  .align 0                       # padding
0x8259CBF0:  li           r11, 0
0x8259CBF4:  li           r10, 1
0x8259CBF8:  addi         r8, r4, 0x18
0x8259CBFC:  stw          r11, 0x14(r4)
0x8259CC00:  lwz          r9, 0x23c(r3)
0x8259CC04:  addi         r9, r9, 7
0x8259CC08:  rlwinm       r7, r9, 0, 0, 0x1c
0x8259CC0C:  stw          r7, 4(r4)
0x8259CC10:  lwz          r9, 0x240(r3)
0x8259CC14:  addi         r6, r9, 7
0x8259CC18:  rlwinm       r5, r6, 0, 0, 0x1c
0x8259CC1C:  stw          r5, 8(r4)
0x8259CC20:  lwz          r9, 0x23c(r3)
0x8259CC24:  addi         r9, r9, 1
0x8259CC28:  srwi         r9, r9, 1
0x8259CC2C:  addi         r7, r9, 7
0x8259CC30:  rlwinm       r6, r7, 0, 0, 0x1c
0x8259CC34:  stw          r6, 0xc(r4)
0x8259CC38:  lwz          r9, 0x240(r3)
0x8259CC3C:  stw          r10, 0(r8)
0x8259CC40:  addi         r5, r9, 1
0x8259CC44:  stw          r10, 0x24(r4)
0x8259CC48:  stw          r10, 0x30(r4)
0x8259CC4C:  srwi         r9, r5, 1
0x8259CC50:  addi         r9, r9, 7
0x8259CC54:  rlwinm       r7, r9, 0, 0, 0x1c
0x8259CC58:  mr           r9, r10
0x8259CC5C:  stw          r7, 0x10(r4)
0x8259CC60:  lwz          r6, 0x20(r3)
0x8259CC64:  rlwinm       r5, r6, 0, 0xb, 0xb
0x8259CC68:  cmplwi       cr6, r5, 0
0x8259CC6C:  bne          cr6, 0x8259cc74
0x8259CC70:  mr           r9, r11
0x8259CC74:  stw          r9, 0x3c(r4)
0x8259CC78:  stw          r11, 0x1c(r4)
0x8259CC7C:  stw          r11, 0x28(r4)
0x8259CC80:  stw          r11, 0x34(r4)
0x8259CC84:  stw          r11, 0x40(r4)
0x8259CC88:  stw          r11, 0x20(r4)
0x8259CC8C:  stw          r11, 0x2c(r4)
0x8259CC90:  stw          r11, 0x38(r4)
0x8259CC94:  stw          r11, 0x44(r4)
0x8259CC98:  lwz          r3, 0x380(r3)
0x8259CC9C:  cmpwi        cr6, r3, 0
0x8259CCA0:  beq          cr6, 0x8259ccac
0x8259CCA4:  stw          r10, 0(r4)
0x8259CCA8:  blr          
0x8259CCAC:  li           r9, 2
0x8259CCB0:  addi         r11, r4, 0x48
0x8259CCB4:  li           r10, 0xc
0x8259CCB8:  stw          r9, 0(r4)
0x8259CCBC:  mtctr        r10
0x8259CCC0:  lwz          r7, 0(r8)
0x8259CCC4:  addi         r8, r8, 4
0x8259CCC8:  stw          r7, 0(r11)
0x8259CCCC:  addi         r11, r11, 4
0x8259CCD0:  bdnz         0x8259ccc0
0x8259CCD4:  blr          
0x8259CCD8:  cmplwi       cr6, r3, 0
0x8259CCDC:  beqlr        cr6
0x8259CCE0:  stw          r4, 0xb8(r3)
0x8259CCE4:  blr          
0x8259CCE8:  mflr         r12
0x8259CCEC:  bl           0x8242f878
0x8259CCF0:  stwu         r1, -0x530(r1)
0x8259CCF4:  lis          r11, -0x7da6
0x8259CCF8:  mr           r20, r4
0x8259CCFC:  addi         r31, r11, 0x5928
0x8259CD00:  rlwinm       r10, r20, 0, 0x11, 0x11
0x8259CD04:  lis          r11, -0x7d8c
0x8259CD08:  mr           r27, r3
0x8259CD0C:  li           r21, 1
0x8259CD10:  li           r22, 0
0x8259CD14:  cmplwi       cr6, r10, 0
0x8259CD18:  addi         r23, r11, -0x61f4
0x8259CD1C:  bne          cr6, 0x8259cd30
0x8259CD20:  mr           r11, r21
0x8259CD24:  stw          r11, 0x14(r23)
0x8259CD28:  mr           r11, r22
0x8259CD2C:  stw          r11, 0x18(r23)
0x8259CD30:  li           r5, 0x388
0x8259CD34:  li           r4, 0
0x8259CD38:  addi         r3, r1, 0x140
0x8259CD3C:  bl           0x8242fed0
0x8259CD40:  bl           0x825a03b8
0x8259CD44:  lis          r11, -0x7da1
0x8259CD48:  rlwinm       r25, r20, 0, 5, 5
0x8259CD4C:  stw          r3, 0x3e0(r1)
0x8259CD50:  addi         r26, r11, -0x56e8
0x8259CD54:  mr           r11, r22
0x8259CD58:  cmplwi       cr6, r25, 0
0x8259CD5C:  stb          r11, 8(r26)
0x8259CD60:  beq          cr6, 0x8259cd78
0x8259CD64:  addi         r3, r1, 0x60
0x8259CD68:  li           r5, 0x2c
0x8259CD6C:  mr           r4, r27
0x8259CD70:  bl           0x82434100
0x8259CD74:  b            0x8259ce18
0x8259CD78:  rlwinm       r9, r20, 0, 6, 6
0x8259CD7C:  cmplwi       cr6, r9, 0
0x8259CD80:  beq          cr6, 0x8259cd94
0x8259CD84:  lwz          r11, 0x38(r23)
0x8259CD88:  cmplwi       cr6, r11, 0
0x8259CD8C:  beq          cr6, 0x8259cd94
0x8259CD90:  mr           r31, r11
0x8259CD94:  mr           r11, r22
0x8259CD98:  mr           r5, r20
0x8259CD9C:  mr           r4, r27
0x8259CDA0:  addi         r3, r1, 0x230
0x8259CDA4:  stw          r11, 0x38(r23)
0x8259CDA8:  mtctr        r31
0x8259CDAC:  bctrl        
0x8259CDB0:  cmpwi        cr6, r3, 0
0x8259CDB4:  bne          cr6, 0x8259cdfc
0x8259CDB8:  lbz          r11, 8(r26)
0x8259CDBC:  cmplwi       cr6, r11, 0
0x8259CDC0:  bne          cr6, 0x8259cdf0
0x8259CDC4:  li           r11, 0x45
0x8259CDC8:  lis          r10, -0x7dfe
0x8259CDCC:  addi         r9, r10, 0x72b8
0x8259CDD0:  stb          r11, 8(r26)
0x8259CDD4:  mr           r11, r22
0x8259CDD8:  addi         r11, r11, 1
0x8259CDDC:  addi         r7, r26, 8
0x8259CDE0:  lbzx         r10, r11, r9
0x8259CDE4:  cmplwi       cr6, r10, 0
0x8259CDE8:  stbx         r10, r11, r7
0x8259CDEC:  bne          cr6, 0x8259cdd8
0x8259CDF0:  li           r3, 0
0x8259CDF4:  addi         r1, r1, 0x530
0x8259CDF8:  b            0x8242f8c8
0x8259CDFC:  lwz          r11, 0x230(r1)
0x8259CE00:  li           r6, 0x2c
0x8259CE04:  addi         r5, r1, 0x60
0x8259CE08:  li           r4, 0
0x8259CE0C:  addi         r3, r1, 0x230
0x8259CE10:  mtctr        r11
0x8259CE14:  bctrl        
0x8259CE18:  lis          r10, 0x664b
0x8259CE1C:  lwz          r6, 0x60(r1)
0x8259CE20:  lis          r9, 0x674b
0x8259CE24:  ori          r4, r10, 0x4942
0x8259CE28:  lis          r8, 0x684b
0x8259CE2C:  ori          r3, r9, 0x4942
0x8259CE30:  ori          r31, r8, 0x4942
0x8259CE34:  cmplw        cr6, r6, r4
0x8259CE38:  beq          cr6, 0x8259ce5c
0x8259CE3C:  cmplw        cr6, r6, r3
0x8259CE40:  beq          cr6, 0x8259ce5c
0x8259CE44:  cmplw        cr6, r6, r31
0x8259CE48:  beq          cr6, 0x8259ce5c
0x8259CE4C:  lis          r7, 0x694b
0x8259CE50:  ori          r5, r7, 0x4942
0x8259CE54:  cmplw        cr6, r6, r5
0x8259CE58:  bne          cr6, 0x8259ce68
0x8259CE5C:  lwz          r11, 0x250(r1)
0x8259CE60:  cmplwi       cr6, r11, 0
0x8259CE64:  beq          cr6, 0x8259ce98
0x8259CE68:  li           r11, 0x4e
0x8259CE6C:  lis          r10, -0x7dfe
0x8259CE70:  addi         r9, r10, 0x72a4
0x8259CE74:  stb          r11, 8(r26)
0x8259CE78:  mr           r11, r22
0x8259CE7C:  addi         r11, r11, 1
0x8259CE80:  addi         r8, r26, 8
0x8259CE84:  lbzx         r10, r11, r9
0x8259CE88:  cmplwi       cr6, r10, 0
0x8259CE8C:  stbx         r10, r11, r8
0x8259CE90:  bne          cr6, 0x8259ce7c
0x8259CE94:  b            0x8259d658
0x8259CE98:  lwz          r29, 0x68(r1)
0x8259CE9C:  cmplwi       cr6, r29, 0
0x8259CEA0:  bne          cr6, 0x8259ced4
0x8259CEA4:  li           r11, 0x54
0x8259CEA8:  lis          r10, -0x7dfe
0x8259CEAC:  addi         r9, r10, 0x7270
0x8259CEB0:  stb          r11, 8(r26)
0x8259CEB4:  mr           r11, r22
0x8259CEB8:  addi         r11, r11, 1
0x8259CEBC:  addi         r6, r26, 8
0x8259CEC0:  lbzx         r10, r11, r9
0x8259CEC4:  cmplwi       cr6, r10, 0
0x8259CEC8:  stbx         r10, r11, r6
0x8259CECC:  bne          cr6, 0x8259ceb8
0x8259CED0:  b            0x8259d658
0x8259CED4:  lwz          r11, 0x84(r1)
0x8259CED8:  lwz          r10, 0x74(r1)
0x8259CEDC:  rlwinm       r9, r11, 0, 0xe, 0xe
0x8259CEE0:  lwz          r8, 0x78(r1)
0x8259CEE4:  addi         r7, r10, 0xf
0x8259CEE8:  or           r9, r9, r20
0x8259CEEC:  srwi         r30, r7, 4
0x8259CEF0:  stw          r11, 0x164(r1)
0x8259CEF4:  mr           r5, r8
0x8259CEF8:  stw          r10, 0x140(r1)
0x8259CEFC:  rlwinm       r7, r9, 0, 4, 0
0x8259CF00:  stw          r10, 0x37c(r1)
0x8259CF04:  rlwinm       r28, r11, 0, 0xb, 0xb
0x8259CF08:  stw          r8, 0x380(r1)
0x8259CF0C:  stw          r30, 0x200(r1)
0x8259CF10:  cmplwi       cr6, r28, 0
0x8259CF14:  stw          r5, 0x144(r1)
0x8259CF18:  stw          r7, 0x160(r1)
0x8259CF1C:  bne          cr6, 0x8259cf28
0x8259CF20:  rlwinm       r7, r7, 0, 0xc, 0xa
0x8259CF24:  stw          r7, 0x160(r1)
0x8259CF28:  rlwinm       r9, r20, 0, 1, 3
0x8259CF2C:  lis          r28, 0x7000
0x8259CF30:  cmplw        cr6, r9, r28
0x8259CF34:  beq          cr6, 0x8259cfb8
0x8259CF38:  cmplwi       cr6, r9, 0
0x8259CF3C:  beq          cr6, 0x8259cf48
0x8259CF40:  or           r7, r9, r7
0x8259CF44:  b            0x8259cf50
0x8259CF48:  rlwinm       r11, r11, 0, 1, 3
0x8259CF4C:  or           r7, r11, r7
0x8259CF50:  rlwinm       r11, r7, 0, 1, 3
0x8259CF54:  stw          r7, 0x160(r1)
0x8259CF58:  lis          r9, 0x3000
0x8259CF5C:  cmplw        cr6, r11, r9
0x8259CF60:  bgt          cr6, 0x8259cf90
0x8259CF64:  beq          cr6, 0x8259cf84
0x8259CF68:  lis          r10, 0x1000
0x8259CF6C:  cmplw        cr6, r11, r10
0x8259CF70:  beq          cr6, 0x8259cfb0
0x8259CF74:  lis          r9, 0x2000
0x8259CF78:  cmplw        cr6, r11, r9
0x8259CF7C:  bne          cr6, 0x8259cfb8
0x8259CF80:  b            0x8259cfb0
0x8259CF84:  slwi         r8, r10, 1
0x8259CF88:  stw          r8, 0x140(r1)
0x8259CF8C:  b            0x8259cfb8
0x8259CF90:  lis          r9, 0x4000
0x8259CF94:  cmplw        cr6, r11, r9
0x8259CF98:  beq          cr6, 0x8259cfa8
0x8259CF9C:  lis          r9, 0x5000
0x8259CFA0:  cmplw        cr6, r11, r9
0x8259CFA4:  bne          cr6, 0x8259cfb8
0x8259CFA8:  slwi         r5, r10, 1
0x8259CFAC:  stw          r5, 0x140(r1)
0x8259CFB0:  slwi         r5, r8, 1
0x8259CFB4:  stw          r5, 0x144(r1)
0x8259CFB8:  cmplw        cr6, r6, r4
0x8259CFBC:  beq          cr6, 0x8259cfdc
0x8259CFC0:  cmplw        cr6, r6, r3
0x8259CFC4:  beq          cr6, 0x8259cfdc
0x8259CFC8:  cmplw        cr6, r6, r31
0x8259CFCC:  bne          cr6, 0x8259cfe8
0x8259CFD0:  ori          r4, r7, 0x8000
0x8259CFD4:  stw          r4, 0x160(r1)
0x8259CFD8:  b            0x8259cfe8
0x8259CFDC:  oris         r3, r7, 1
0x8259CFE0:  ori          r3, r3, 0x8000
0x8259CFE4:  stw          r3, 0x160(r1)
0x8259CFE8:  lwz          r10, 0x70(r1)
0x8259CFEC:  rlwinm       r11, r20, 0, 0x13, 0x13
0x8259CFF0:  lwz          r8, 0x7c(r1)
0x8259CFF4:  stw          r29, 0x148(r1)
0x8259CFF8:  cmplwi       cr6, r11, 0
0x8259CFFC:  stw          r10, 0x20c(r1)
0x8259D000:  lwz          r10, 0x80(r1)
0x8259D004:  beq          cr6, 0x8259d02c
0x8259D008:  lwz          r11, 8(r23)
0x8259D00C:  cmpwi        cr6, r11, -1
0x8259D010:  beq          cr6, 0x8259d02c
0x8259D014:  stw          r11, 0x154(r1)
0x8259D018:  lwz          r11, 0(r26)
0x8259D01C:  stw          r11, 0x158(r1)
0x8259D020:  li           r11, -1
0x8259D024:  stw          r11, 8(r23)
0x8259D028:  b            0x8259d034
0x8259D02C:  stw          r8, 0x154(r1)
0x8259D030:  stw          r10, 0x158(r1)
0x8259D034:  addi         r9, r5, 0xf
0x8259D038:  stw          r10, 0x3e8(r1)
0x8259D03C:  srwi         r11, r10, 1
0x8259D040:  lwz          r5, 0x64(r1)
0x8259D044:  srwi         r7, r9, 4
0x8259D048:  lwz          r4, 0x88(r1)
0x8259D04C:  add          r6, r11, r8
0x8259D050:  lwz          r3, 0x6c(r1)
0x8259D054:  mullw        r11, r7, r30
0x8259D058:  stw          r8, 0x3e4(r1)
0x8259D05C:  stw          r5, 0x168(r1)
0x8259D060:  stw          r4, 0x210(r1)
0x8259D064:  stw          r3, 0x208(r1)
0x8259D068:  stw          r11, 0x204(r1)
0x8259D06C:  divwu        r9, r6, r10
0x8259D070:  srwi         r10, r11, 2
0x8259D074:  cmplwi       cr6, r9, 0
0x8259D078:  stw          r9, 0x3ec(r1)
0x8259D07C:  stw          r10, 0x424(r1)
0x8259D080:  bne          cr6, 0x8259d088
0x8259D084:  stw          r21, 0x3ec(r1)
0x8259D088:  addi         r4, r11, 0x10
0x8259D08C:  addi         r3, r1, 0x1fc
0x8259D090:  bl           0x8259f7b0
0x8259D094:  lwz          r9, 0x3ec(r1)
0x8259D098:  addi         r3, r1, 0x3f4
0x8259D09C:  slwi         r4, r9, 2
0x8259D0A0:  bl           0x8259f7b0
0x8259D0A4:  lwz          r8, 0x3ec(r1)
0x8259D0A8:  addi         r3, r1, 0x3f8
0x8259D0AC:  slwi         r4, r8, 2
0x8259D0B0:  bl           0x8259f7b0
0x8259D0B4:  lwz          r7, 0x3ec(r1)
0x8259D0B8:  addi         r3, r1, 0x3fc
0x8259D0BC:  slwi         r4, r7, 2
0x8259D0C0:  bl           0x8259f7b0
0x8259D0C4:  lwz          r6, 0x3ec(r1)
0x8259D0C8:  addi         r3, r1, 0x400
0x8259D0CC:  slwi         r4, r6, 2
0x8259D0D0:  bl           0x8259f7b0
0x8259D0D4:  lwz          r5, 0x3ec(r1)
0x8259D0D8:  addi         r3, r1, 0x404
0x8259D0DC:  slwi         r4, r5, 2
0x8259D0E0:  bl           0x8259f7b0
0x8259D0E4:  lwz          r4, 0x3ec(r1)
0x8259D0E8:  addi         r3, r1, 0x408
0x8259D0EC:  slwi         r4, r4, 2
0x8259D0F0:  bl           0x8259f7b0
0x8259D0F4:  lwz          r3, 0x3ec(r1)
0x8259D0F8:  slwi         r4, r3, 2
0x8259D0FC:  addi         r3, r1, 0x40c
0x8259D100:  bl           0x8259f7b0
0x8259D104:  lwz          r11, 0x14(r23)
0x8259D108:  cmplwi       cr6, r11, 0
0x8259D10C:  bne          cr6, 0x8259d114
0x8259D110:  mr           r11, r21
0x8259D114:  mulli        r4, r11, 0x17c
0x8259D118:  addi         r3, r1, 0x430
0x8259D11C:  bl           0x8259f7b0
0x8259D120:  lwz          r11, 0x14(r23)
0x8259D124:  addi         r3, r1, 0x384
0x8259D128:  slwi         r4, r11, 2
0x8259D12C:  bl           0x8259f7b0
0x8259D130:  lwz          r11, 0x140(r1)
0x8259D134:  addi         r3, r1, 0x440
0x8259D138:  addi         r10, r11, 0xf
0x8259D13C:  rlwinm       r4, r10, 0, 0, 0x1b
0x8259D140:  bl           0x825a2e58
0x8259D144:  addi         r3, r1, 0x440
0x8259D148:  lwz          r4, 0x440(r1)
0x8259D14C:  bl           0x8259f7b0
0x8259D150:  addi         r3, r1, 0x444
0x8259D154:  lwz          r4, 0x444(r1)
0x8259D158:  bl           0x8259f7b0
0x8259D15C:  addi         r3, r1, 0x448
0x8259D160:  lwz          r4, 0x448(r1)
0x8259D164:  bl           0x8259f7b0
0x8259D168:  addi         r3, r1, 0x44c
0x8259D16C:  lwz          r4, 0x44c(r1)
0x8259D170:  bl           0x8259f7b0
0x8259D174:  addi         r3, r1, 0x450
0x8259D178:  lwz          r4, 0x450(r1)
0x8259D17C:  bl           0x8259f7b0
0x8259D180:  addi         r3, r1, 0x454
0x8259D184:  lwz          r4, 0x454(r1)
0x8259D188:  bl           0x8259f7b0
0x8259D18C:  addi         r3, r1, 0x458
0x8259D190:  lwz          r4, 0x458(r1)
0x8259D194:  bl           0x8259f7b0
0x8259D198:  addi         r3, r1, 0x45c
0x8259D19C:  lwz          r4, 0x45c(r1)
0x8259D1A0:  bl           0x8259f7b0
0x8259D1A4:  addi         r3, r1, 0x460
0x8259D1A8:  lwz          r4, 0x460(r1)
0x8259D1AC:  bl           0x8259f7b0
0x8259D1B0:  cmplwi       cr6, r25, 0
0x8259D1B4:  bne          cr6, 0x8259d1fc
0x8259D1B8:  lwz          r9, 0x20c(r1)
0x8259D1BC:  addi         r3, r1, 0x22c
0x8259D1C0:  addi         r8, r9, 1
0x8259D1C4:  slwi         r4, r8, 2
0x8259D1C8:  bl           0x8259f7b0
0x8259D1CC:  lwz          r7, 0x210(r1)
0x8259D1D0:  addi         r3, r1, 0x388
0x8259D1D4:  slwi         r4, r7, 2
0x8259D1D8:  bl           0x8259f7b0
0x8259D1DC:  lwz          r6, 0x210(r1)
0x8259D1E0:  addi         r3, r1, 0x38c
0x8259D1E4:  slwi         r4, r6, 2
0x8259D1E8:  bl           0x8259f7b0
0x8259D1EC:  lwz          r5, 0x210(r1)
0x8259D1F0:  addi         r3, r1, 0x390
0x8259D1F4:  slwi         r4, r5, 2
0x8259D1F8:  bl           0x8259f7b0
0x8259D1FC:  li           r4, 0x388
0x8259D200:  addi         r3, r1, 0x140
0x8259D204:  bl           0x8259c400
0x8259D208:  mr           r31, r3
0x8259D20C:  cmplwi       cr6, r31, 0
0x8259D210:  beq          cr6, 0x8259d62c
0x8259D214:  addi         r4, r1, 0x140
0x8259D218:  li           r5, 0x388
0x8259D21C:  bl           0x82434100
0x8259D220:  lwz          r4, 0x2b8(r31)
0x8259D224:  stw          r31, 0x10c(r31)
0x8259D228:  cmplwi       cr6, r25, 0
0x8259D22C:  stw          r22, 0(r4)
0x8259D230:  lwz          r3, 0x2bc(r31)
0x8259D234:  stw          r22, 0(r3)
0x8259D238:  lwz          r11, 0x2c0(r31)
0x8259D23C:  stw          r22, 0(r11)
0x8259D240:  lwz          r10, 0x2c4(r31)
0x8259D244:  stw          r22, 0(r10)
0x8259D248:  lwz          r9, 0x2c8(r31)
0x8259D24C:  stw          r22, 0(r9)
0x8259D250:  lwz          r8, 0x2cc(r31)
0x8259D254:  stw          r22, 0(r8)
0x8259D258:  beq          cr6, 0x8259d474
0x8259D25C:  lwz          r11, 0xd0(r31)
0x8259D260:  addi         r7, r27, 0x2c
0x8259D264:  slwi         r10, r11, 1
0x8259D268:  addi         r6, r11, 0xb
0x8259D26C:  add          r5, r11, r10
0x8259D270:  slwi         r10, r11, 3
0x8259D274:  stw          r7, 0x248(r31)
0x8259D278:  slwi         r11, r5, 2
0x8259D27C:  slwi         r9, r6, 2
0x8259D280:  add          r10, r10, r27
0x8259D284:  add          r11, r11, r27
0x8259D288:  add          r4, r9, r27
0x8259D28C:  addi         r3, r10, 0x2c
0x8259D290:  addi         r11, r11, 0x2c
0x8259D294:  stw          r4, 0x24c(r31)
0x8259D298:  stw          r3, 0x250(r31)
0x8259D29C:  stw          r11, 0xec(r31)
0x8259D2A0:  addi         r7, r31, 0x380
0x8259D2A4:  lwz          r5, 0x2ac(r31)
0x8259D2A8:  addi         r6, r31, 0xd8
0x8259D2AC:  lwz          r4, 0xec(r31)
0x8259D2B0:  lwz          r3, 8(r31)
0x8259D2B4:  bl           0x8259c9e0
0x8259D2B8:  rlwinm       r5, r20, 0, 0x15, 0x15
0x8259D2BC:  stw          r3, 0xd4(r31)
0x8259D2C0:  cmplwi       cr6, r5, 0
0x8259D2C4:  bne          cr6, 0x8259d448
0x8259D2C8:  addi         r4, r1, 0xc0
0x8259D2CC:  mr           r3, r31
0x8259D2D0:  bl           0x8259cbf0
0x8259D2D4:  lwz          r4, 0xc0(r1)
0x8259D2D8:  mr           r8, r22
0x8259D2DC:  mr           r29, r22
0x8259D2E0:  cmpwi        cr6, r4, 0
0x8259D2E4:  ble          cr6, 0x8259d3b8
0x8259D2E8:  addi         r30, r1, 0xdc
0x8259D2EC:  lwz          r3, -4(r30)
0x8259D2F0:  cmpwi        cr6, r3, 0
0x8259D2F4:  beq          cr6, 0x8259d318
0x8259D2F8:  lwz          r11, 0xc4(r1)
0x8259D2FC:  mr           r3, r30
0x8259D300:  lwz          r10, 0xc8(r1)
0x8259D304:  mullw        r4, r10, r11
0x8259D308:  bl           0x8259f7b0
0x8259D30C:  lwz          r9, 0xc4(r1)
0x8259D310:  stw          r9, 4(r30)
0x8259D314:  mr           r8, r21
0x8259D318:  lwz          r7, 8(r30)
0x8259D31C:  cmpwi        cr6, r7, 0
0x8259D320:  beq          cr6, 0x8259d344
0x8259D324:  lwz          r6, 0xcc(r1)
0x8259D328:  addi         r3, r30, 0xc
0x8259D32C:  lwz          r5, 0xd0(r1)
0x8259D330:  mullw        r4, r5, r6
0x8259D334:  bl           0x8259f7b0
0x8259D338:  lwz          r4, 0xcc(r1)
0x8259D33C:  stw          r4, 0x10(r30)
0x8259D340:  mr           r8, r21
0x8259D344:  lwz          r3, 0x14(r30)
0x8259D348:  cmpwi        cr6, r3, 0
0x8259D34C:  beq          cr6, 0x8259d370
0x8259D350:  lwz          r11, 0xcc(r1)
0x8259D354:  addi         r3, r30, 0x18
0x8259D358:  lwz          r10, 0xd0(r1)
0x8259D35C:  mullw        r4, r10, r11
0x8259D360:  bl           0x8259f7b0
0x8259D364:  lwz          r9, 0xcc(r1)
0x8259D368:  stw          r9, 0x1c(r30)
0x8259D36C:  mr           r8, r21
0x8259D370:  lwz          r7, 0x20(r30)
0x8259D374:  cmpwi        cr6, r7, 0
0x8259D378:  beq          cr6, 0x8259d39c
0x8259D37C:  lwz          r6, 0xc4(r1)
0x8259D380:  addi         r3, r30, 0x24
0x8259D384:  lwz          r5, 0xc8(r1)
0x8259D388:  mullw        r4, r5, r6
0x8259D38C:  bl           0x8259f7b0
0x8259D390:  lwz          r4, 0xc4(r1)
0x8259D394:  stw          r4, 0x28(r30)
0x8259D398:  mr           r8, r21
0x8259D39C:  lwz          r3, 0xc0(r1)
0x8259D3A0:  addi         r29, r29, 1
0x8259D3A4:  addi         r30, r30, 0x30
0x8259D3A8:  cmpw         cr6, r29, r3
0x8259D3AC:  blt          cr6, 0x8259d2ec
0x8259D3B0:  cmpwi        cr6, r29, 2
0x8259D3B4:  bge          cr6, 0x8259d3ec
0x8259D3B8:  slwi         r11, r29, 1
0x8259D3BC:  addi         r9, r1, 0xf4
0x8259D3C0:  add          r11, r29, r11
0x8259D3C4:  subfic       r10, r29, 2
0x8259D3C8:  slwi         r11, r11, 4
0x8259D3CC:  add          r11, r11, r9
0x8259D3D0:  addi         r10, r10, -1
0x8259D3D4:  stw          r22, -0xc(r11)
0x8259D3D8:  stw          r22, 0(r11)
0x8259D3DC:  stw          r22, -0x18(r11)
0x8259D3E0:  cmplwi       cr6, r10, 0
0x8259D3E4:  addi         r11, r11, 0x30
0x8259D3E8:  bne          cr6, 0x8259d3d0
0x8259D3EC:  cmpwi        cr6, r8, 0
0x8259D3F0:  beq          cr6, 0x8259d448
0x8259D3F4:  bl           0x8259f898
0x8259D3F8:  lwz          r10, 0x290(r31)
0x8259D3FC:  mr           r11, r3
0x8259D400:  li           r3, 0x78
0x8259D404:  add          r11, r10, r11
0x8259D408:  addi         r10, r11, 0x78
0x8259D40C:  stw          r10, 0x290(r31)
0x8259D410:  bl           0x8259f8a8
0x8259D414:  cmplwi       cr6, r3, 0
0x8259D418:  stw          r3, 0x384(r31)
0x8259D41C:  beq          cr6, 0x8259d624
0x8259D420:  addi         r11, r1, 0xc0
0x8259D424:  stw          r3, 0xb8(r31)
0x8259D428:  mr           r10, r3
0x8259D42C:  li           r9, 0x1e
0x8259D430:  mtctr        r9
0x8259D434:  lwz          r9, 0(r11)
0x8259D438:  addi         r11, r11, 4
0x8259D43C:  stw          r9, 0(r10)
0x8259D440:  addi         r10, r10, 4
0x8259D444:  bdnz         0x8259d434
0x8259D448:  rlwinm       r8, r20, 0, 7, 7
0x8259D44C:  cmplwi       cr6, r8, 0
0x8259D450:  beq          cr6, 0x8259d538
0x8259D454:  lwz          r11, 0xc(r23)
0x8259D458:  cmpwi        cr6, r11, -1
0x8259D45C:  beq          cr6, 0x8259d538
0x8259D460:  mr           r7, r11
0x8259D464:  li           r11, -1
0x8259D468:  stw          r7, 0x238(r31)
0x8259D46C:  stw          r11, 0xc(r23)
0x8259D470:  b            0x8259d540
0x8259D474:  addi         r30, r31, 0xf0
0x8259D478:  lwz          r10, 0xd0(r31)
0x8259D47C:  lwz          r5, 0x248(r31)
0x8259D480:  li           r4, -1
0x8259D484:  mr           r3, r30
0x8259D488:  slwi         r6, r10, 2
0x8259D48C:  lwz          r9, 0(r30)
0x8259D490:  mtctr        r9
0x8259D494:  bctrl        
0x8259D498:  lwz          r8, 0xd0(r31)
0x8259D49C:  lwz          r7, 0(r30)
0x8259D4A0:  li           r4, -1
0x8259D4A4:  lwz          r5, 0x24c(r31)
0x8259D4A8:  slwi         r6, r8, 2
0x8259D4AC:  mr           r3, r30
0x8259D4B0:  mtctr        r7
0x8259D4B4:  bctrl        
0x8259D4B8:  lwz          r6, 0xd0(r31)
0x8259D4BC:  lwz          r11, 0(r30)
0x8259D4C0:  li           r4, -1
0x8259D4C4:  lwz          r5, 0x250(r31)
0x8259D4C8:  mr           r3, r30
0x8259D4CC:  slwi         r6, r6, 2
0x8259D4D0:  mtctr        r11
0x8259D4D4:  bctrl        
0x8259D4D8:  lwz          r11, 0xcc(r31)
0x8259D4DC:  lwz          r10, 0(r30)
0x8259D4E0:  li           r4, -1
0x8259D4E4:  addi         r9, r11, 1
0x8259D4E8:  lwz          r5, 0xec(r31)
0x8259D4EC:  mr           r3, r30
0x8259D4F0:  slwi         r6, r9, 2
0x8259D4F4:  mtctr        r10
0x8259D4F8:  bctrl        
0x8259D4FC:  lwz          r8, 0x110(r31)
0x8259D500:  cmplwi       cr6, r8, 0
0x8259D504:  beq          cr6, 0x8259d2a0
0x8259D508:  li           r11, 0x45
0x8259D50C:  lis          r10, -0x7dfe
0x8259D510:  addi         r9, r10, 0x7254
0x8259D514:  stb          r11, 8(r26)
0x8259D518:  mr           r11, r22
0x8259D51C:  addi         r11, r11, 1
0x8259D520:  addi         r7, r26, 8
0x8259D524:  lbzx         r10, r11, r9
0x8259D528:  cmplwi       cr6, r10, 0
0x8259D52C:  stbx         r10, r11, r7
0x8259D530:  bne          cr6, 0x8259d51c
0x8259D534:  b            0x8259d658
0x8259D538:  lwz          r6, 0xd4(r31)
0x8259D53C:  stw          r6, 0x238(r31)
0x8259D540:  rlwinm       r5, r20, 0, 9, 9
0x8259D544:  cmplwi       cr6, r5, 0
0x8259D548:  beq          cr6, 0x8259d568
0x8259D54C:  lwz          r11, 0x10(r23)
0x8259D550:  cmpwi        cr6, r11, -1
0x8259D554:  beq          cr6, 0x8259d568
0x8259D558:  mr           r28, r11
0x8259D55C:  li           r11, -1
0x8259D560:  stw          r11, 0x10(r23)
0x8259D564:  b            0x8259d56c
0x8259D568:  mr           r28, r22
0x8259D56C:  cmplwi       cr6, r25, 0
0x8259D570:  beq          cr6, 0x8259d58c
0x8259D574:  lwz          r4, 0xec(r31)
0x8259D578:  lwz          r3, 0(r4)
0x8259D57C:  rlwinm       r11, r3, 0, 0, 0x1e
0x8259D580:  add          r11, r11, r27
0x8259D584:  stw          r11, 0xe8(r31)
0x8259D588:  b            0x8259d730
0x8259D58C:  addi         r30, r31, 0xf0
0x8259D590:  lwz          r4, 0x238(r31)
0x8259D594:  lwz          r10, 0xf8(r31)
0x8259D598:  mr           r3, r30
0x8259D59C:  mtctr        r10
0x8259D5A0:  bctrl        
0x8259D5A4:  lwz          r11, 0x28(r31)
0x8259D5A8:  lis          r9, -0x3334
0x8259D5AC:  stw          r3, 0x238(r31)
0x8259D5B0:  slwi         r10, r11, 3
0x8259D5B4:  ori          r8, r9, 0xcccd
0x8259D5B8:  add          r7, r11, r10
0x8259D5BC:  mulhwu       r6, r7, r8
0x8259D5C0:  srwi         r5, r6, 3
0x8259D5C4:  cmplw        cr6, r3, r5
0x8259D5C8:  blt          cr6, 0x8259d5dc
0x8259D5CC:  lwz          r4, 0x20(r31)
0x8259D5D0:  ori          r20, r20, 0x2000
0x8259D5D4:  ori          r3, r4, 0x2000
0x8259D5D8:  stw          r3, 0x20(r31)
0x8259D5DC:  rlwinm       r10, r20, 0, 0x12, 0x12
0x8259D5E0:  cmplwi       cr6, r10, 0
0x8259D5E4:  beq          cr6, 0x8259d6e4
0x8259D5E8:  lwz          r9, 0xec(r31)
0x8259D5EC:  mr           r3, r31
0x8259D5F0:  lwz          r8, 0(r9)
0x8259D5F4:  rlwinm       r7, r8, 0, 0, 0x1e
0x8259D5F8:  subf         r11, r7, r11
0x8259D5FC:  addi         r29, r11, 8
0x8259D600:  mr           r4, r29
0x8259D604:  bl           0x8259c400
0x8259D608:  cmplwi       cr6, r3, 0
0x8259D60C:  stw          r3, 0xe8(r31)
0x8259D610:  bne          cr6, 0x8259d680
0x8259D614:  lwz          r3, 0xb8(r31)
0x8259D618:  cmplwi       cr6, r3, 0
0x8259D61C:  beq          cr6, 0x8259d624
0x8259D620:  bl           0x8257b028
0x8259D624:  mr           r3, r31
0x8259D628:  bl           0x8257b028
0x8259D62C:  li           r11, 0x4f
0x8259D630:  lis          r10, -0x7dfe
0x8259D634:  addi         r9, r10, 0x7244
0x8259D638:  stb          r11, 8(r26)
0x8259D63C:  mr           r11, r22
0x8259D640:  addi         r11, r11, 1
0x8259D644:  addi         r6, r26, 8
0x8259D648:  lbzx         r10, r11, r9
0x8259D64C:  cmplwi       cr6, r10, 0
0x8259D650:  stbx         r10, r11, r6
0x8259D654:  bne          cr6, 0x8259d640
0x8259D658:  rlwinm       r4, r20, 0, 5, 5
0x8259D65C:  cmplwi       cr6, r4, 0
0x8259D660:  bne          cr6, 0x8259d674
0x8259D664:  lwz          r11, 0x244(r1)
0x8259D668:  addi         r3, r1, 0x230
0x8259D66C:  mtctr        r11
0x8259D670:  bctrl        
0x8259D674:  li           r3, 0
0x8259D678:  addi         r1, r1, 0x530
0x8259D67C:  b            0x8242f8c8
0x8259D680:  lwz          r11, 0x28(r31)
0x8259D684:  mr           r7, r28
0x8259D688:  lwz          r10, 0xfc(r31)
0x8259D68C:  li           r5, 0
0x8259D690:  addi         r6, r11, 8
0x8259D694:  li           r4, 0
0x8259D698:  mr           r3, r30
0x8259D69C:  mtctr        r10
0x8259D6A0:  bctrl        
0x8259D6A4:  lwz          r9, 0xec(r31)
0x8259D6A8:  lwz          r6, 0xe8(r31)
0x8259D6AC:  mr           r7, r29
0x8259D6B0:  lwz          r8, 0xf4(r31)
0x8259D6B4:  li           r4, 0
0x8259D6B8:  mr           r3, r30
0x8259D6BC:  lwz          r5, 0(r9)
0x8259D6C0:  rlwinm       r5, r5, 0, 0, 0x1e
0x8259D6C4:  mtctr        r8
0x8259D6C8:  bctrl        
0x8259D6CC:  lwz          r4, 0x104(r31)
0x8259D6D0:  mr           r3, r30
0x8259D6D4:  mtctr        r4
0x8259D6D8:  bctrl        
0x8259D6DC:  stw          r22, 0x124(r31)
0x8259D6E0:  b            0x8259d730
0x8259D6E4:  addi         r3, r31, 0xe4
0x8259D6E8:  lwz          r4, 0xc8(r31)
0x8259D6EC:  bl           0x8259f7b0
0x8259D6F0:  mr           r3, r31
0x8259D6F4:  lwz          r4, 0x238(r31)
0x8259D6F8:  bl           0x8259c400
0x8259D6FC:  mr           r4, r3
0x8259D700:  cmplwi       cr6, r4, 0
0x8259D704:  stw          r4, 0x234(r31)
0x8259D708:  bne          cr6, 0x8259d710
0x8259D70C:  stw          r22, 0x238(r31)
0x8259D710:  lwz          r11, 0x28(r31)
0x8259D714:  mr           r7, r28
0x8259D718:  lwz          r5, 0x238(r31)
0x8259D71C:  mr           r3, r30
0x8259D720:  lwz          r10, 0xfc(r31)
0x8259D724:  addi         r6, r11, 8
0x8259D728:  mtctr        r10
0x8259D72C:  bctrl        
0x8259D730:  lwz          r9, 0xc4(r31)
0x8259D734:  li           r24, -1
0x8259D738:  lwz          r8, 0xbc(r31)
0x8259D73C:  stbx         r22, r9, r8
0x8259D740:  lwz          r11, 0x14(r31)
0x8259D744:  stw          r24, 0xc(r31)
0x8259D748:  cmplwi       cr6, r11, 0
0x8259D74C:  beq          cr6, 0x8259d768
0x8259D750:  lwz          r7, 0x18(r31)
0x8259D754:  clrldi       r6, r11, 0x20
0x8259D758:  mulli        r5, r7, 0x7d0
0x8259D75C:  divdu        r4, r5, r6
0x8259D760:  stw          r4, 0x270(r31)
0x8259D764:  b            0x8259d770
0x8259D768:  li           r11, 0x7d0
0x8259D76C:  stw          r11, 0x270(r31)
0x8259D770:  li           r4, 1
0x8259D774:  stw          r21, 0x28c(r31)
0x8259D778:  mr           r3, r31
0x8259D77C:  bl           0x8259c860
0x8259D780:  bl           0x825a03b8
0x8259D784:  lwz          r10, 0x2a0(r31)
0x8259D788:  lwz          r8, 0x2f0(r31)
0x8259D78C:  lis          r25, 1
0x8259D790:  subf         r9, r10, r3
0x8259D794:  stw          r9, 0x2a0(r31)
0x8259D798:  stw          r25, 0x6c(r8)
0x8259D79C:  lwz          r7, 0xd0(r31)
0x8259D7A0:  stw          r22, 0x2e8(r31)
0x8259D7A4:  cmpwi        cr6, r7, 0
0x8259D7A8:  beq          cr6, 0x8259d828
0x8259D7AC:  lwz          r11, 0x14(r23)
0x8259D7B0:  cmplwi       cr6, r11, 0
0x8259D7B4:  beq          cr6, 0x8259d828
0x8259D7B8:  addi         r8, r23, 0x18
0x8259D7BC:  mr           r7, r11
0x8259D7C0:  lwz          r6, 0xd0(r31)
0x8259D7C4:  mr           r10, r22
0x8259D7C8:  cmpwi        cr6, r6, 0
0x8259D7CC:  ble          cr6, 0x8259d818
0x8259D7D0:  lwz          r9, 0(r8)
0x8259D7D4:  lwz          r11, 0x250(r31)
0x8259D7D8:  lwz          r5, 0(r11)
0x8259D7DC:  cmpw         cr6, r5, r9
0x8259D7E0:  beq          cr6, 0x8259d7fc
0x8259D7E4:  lwz          r4, 0xd0(r31)
0x8259D7E8:  addi         r10, r10, 1
0x8259D7EC:  addi         r11, r11, 4
0x8259D7F0:  cmpw         cr6, r10, r4
0x8259D7F4:  blt          cr6, 0x8259d7d8
0x8259D7F8:  b            0x8259d818
0x8259D7FC:  lwz          r3, 0x2e8(r31)
0x8259D800:  lwz          r11, 0x244(r31)
0x8259D804:  slwi         r9, r3, 2
0x8259D808:  stwx         r10, r9, r11
0x8259D80C:  lwz          r11, 0x2e8(r31)
0x8259D810:  addi         r6, r11, 1
0x8259D814:  stw          r6, 0x2e8(r31)
0x8259D818:  addi         r7, r7, -1
0x8259D81C:  addi         r8, r8, 4
0x8259D820:  cmplwi       cr6, r7, 0
0x8259D824:  bne          cr6, 0x8259d7c0
0x8259D828:  mr           r11, r22
0x8259D82C:  mr           r26, r22
0x8259D830:  stw          r11, 0x18(r23)
0x8259D834:  mr           r11, r21
0x8259D838:  lwz          r5, 0x2e8(r31)
0x8259D83C:  cmplwi       cr6, r5, 0
0x8259D840:  stw          r11, 0x14(r23)
0x8259D844:  ble          cr6, 0x8259dc44
0x8259D848:  mr           r28, r22
0x8259D84C:  mr           r30, r22
0x8259D850:  lwz          r11, 0x2f0(r31)
0x8259D854:  add          r4, r30, r11
0x8259D858:  stw          r22, 0x2c(r4)
0x8259D85C:  lwz          r9, 0x244(r31)
0x8259D860:  lwz          r10, 0x24c(r31)
0x8259D864:  lwzx         r3, r28, r9
0x8259D868:  slwi         r11, r3, 2
0x8259D86C:  lwzx         r11, r11, r10
0x8259D870:  rlwinm       r8, r11, 0, 0, 0
0x8259D874:  cmplwi       cr6, r8, 0
0x8259D878:  bne          cr6, 0x8259d888
0x8259D87C:  rlwinm       r7, r11, 0, 3, 3
0x8259D880:  cmplwi       cr6, r7, 0
0x8259D884:  beq          cr6, 0x8259dbd8
0x8259D888:  lwz          r6, -8(r23)
0x8259D88C:  cmplwi       cr6, r6, 0
0x8259D890:  beq          cr6, 0x8259dbd8
0x8259D894:  clrlwi       r27, r11, 0x10
0x8259D898:  lwz          r11, 0x154(r1)
0x8259D89C:  cmplwi       cr6, r11, 0
0x8259D8A0:  stw          r27, 0x50(r1)
0x8259D8A4:  beq          cr6, 0x8259d92c
0x8259D8A8:  lwz          r8, 0x158(r1)
0x8259D8AC:  cmplwi       cr6, r8, 0
0x8259D8B0:  beq          cr6, 0x8259d92c
0x8259D8B4:  lwz          r4, 0x3e8(r1)
0x8259D8B8:  extsw        r5, r27
0x8259D8BC:  lwz          r7, 0x3e4(r1)
0x8259D8C0:  extsw        r11, r11
0x8259D8C4:  extsw        r3, r4
0x8259D8C8:  extsw        r4, r8
0x8259D8CC:  extsw        r6, r7
0x8259D8D0:  std          r5, 0xa8(r1)
0x8259D8D4:  addi         r8, r1, 0x50
0x8259D8D8:  std          r11, 0xb0(r1)
0x8259D8DC:  std          r3, 0xa0(r1)
0x8259D8E0:  std          r4, 0x98(r1)
0x8259D8E4:  std          r6, 0x90(r1)
0x8259D8E8:  lfd          f0, 0xa8(r1)
0x8259D8EC:  fcfid        f12, f0
0x8259D8F0:  lfd          f10, 0xb0(r1)
0x8259D8F4:  lfd          f13, 0xa0(r1)
0x8259D8F8:  fcfid        f8, f10
0x8259D8FC:  fcfid        f11, f13
0x8259D900:  lfd          f7, 0x98(r1)
0x8259D904:  lfd          f9, 0x90(r1)
0x8259D908:  fcfid        f5, f7
0x8259D90C:  fcfid        f6, f9
0x8259D910:  fmul         f4, f12, f11
0x8259D914:  fmul         f3, f6, f5
0x8259D918:  fmul         f2, f4, f8
0x8259D91C:  fdiv         f1, f2, f3
0x8259D920:  fctiwz       f0, f1
0x8259D924:  stfiwx       f0, 0, r8
0x8259D928:  lwz          r27, 0x50(r1)
0x8259D92C:  lwzx         r7, r28, r9
0x8259D930:  addi         r6, r23, -8
0x8259D934:  lwz          r11, 0x2f0(r31)
0x8259D938:  mr           r8, r31
0x8259D93C:  slwi         r5, r7, 2
0x8259D940:  lwz          r7, 0x20(r31)
0x8259D944:  add          r3, r30, r11
0x8259D948:  mr           r4, r27
0x8259D94C:  lwz          r9, 0(r6)
0x8259D950:  lwzx         r11, r5, r10
0x8259D954:  rlwinm       r10, r11, 2, 0x1f, 0x1f
0x8259D958:  rlwinm       r11, r11, 3, 0x1f, 0x1f
0x8259D95C:  addi         r5, r10, 1
0x8259D960:  addi         r6, r11, 1
0x8259D964:  slwi         r5, r5, 3
0x8259D968:  mtctr        r9
0x8259D96C:  bctrl        
0x8259D970:  cmpwi        cr6, r3, 0
0x8259D974:  beq          cr6, 0x8259dbd8
0x8259D978:  lwz          r11, 0x2f0(r31)
0x8259D97C:  add          r11, r30, r11
0x8259D980:  lwz          r4, 0x58(r11)
0x8259D984:  cmplwi       cr6, r4, 0
0x8259D988:  bne          cr6, 0x8259d990
0x8259D98C:  stw          r24, 0x58(r11)
0x8259D990:  lwz          r3, 0x244(r31)
0x8259D994:  lwz          r10, 0x248(r31)
0x8259D998:  lwz          r11, 0x2f0(r31)
0x8259D99C:  add          r9, r30, r11
0x8259D9A0:  lwzx         r8, r28, r3
0x8259D9A4:  slwi         r7, r8, 2
0x8259D9A8:  lwzx         r11, r7, r10
0x8259D9AC:  addi         r6, r11, 0xff
0x8259D9B0:  rlwinm       r5, r6, 0, 0, 0x17
0x8259D9B4:  stw          r5, 0x28(r9)
0x8259D9B8:  lwz          r11, 0x2f0(r31)
0x8259D9BC:  add          r4, r30, r11
0x8259D9C0:  lwz          r29, 0x28(r4)
0x8259D9C4:  bl           0x8259f898
0x8259D9C8:  mr           r11, r3
0x8259D9CC:  lwz          r10, 0x290(r31)
0x8259D9D0:  mr           r3, r29
0x8259D9D4:  add          r11, r29, r11
0x8259D9D8:  add          r11, r11, r10
0x8259D9DC:  stw          r11, 0x290(r31)
0x8259D9E0:  bl           0x8259f8a8
0x8259D9E4:  lwz          r11, 0x2f0(r31)
0x8259D9E8:  add          r10, r30, r11
0x8259D9EC:  stw          r3, 0x2c(r10)
0x8259D9F0:  lwz          r11, 0x2f0(r31)
0x8259D9F4:  add          r3, r30, r11
0x8259D9F8:  lwz          r9, 0x2c(r3)
0x8259D9FC:  cmplwi       cr6, r9, 0
0x8259DA00:  bne          cr6, 0x8259da14
0x8259DA04:  lwz          r8, 0x1c(r3)
0x8259DA08:  mtctr        r8
0x8259DA0C:  bctrl        
0x8259DA10:  b            0x8259dbd8
0x8259DA14:  lwz          r7, 0x244(r31)
0x8259DA18:  lwz          r6, 0x24c(r31)
0x8259DA1C:  lwzx         r5, r28, r7
0x8259DA20:  slwi         r4, r5, 2
0x8259DA24:  lwzx         r11, r4, r6
0x8259DA28:  rlwinm       r11, r11, 5, 0x1c, 0x1c
0x8259DA2C:  addi         r10, r11, 8
0x8259DA30:  mr           r11, r21
0x8259DA34:  cmplwi       cr6, r10, 8
0x8259DA38:  beq          cr6, 0x8259da40
0x8259DA3C:  mr           r11, r22
0x8259DA40:  stw          r11, 0x44(r3)
0x8259DA44:  lis          r7, 0x1a9f
0x8259DA48:  lwz          r11, 0x2f0(r31)
0x8259DA4C:  lis          r5, 0x624
0x8259DA50:  ori          r6, r7, 0xbe77
0x8259DA54:  add          r11, r30, r11
0x8259DA58:  ori          r4, r5, 0xdd2f
0x8259DA5C:  clrldi       r8, r27, 0x20
0x8259DA60:  rldimi       r6, r4, 0x20, 0
0x8259DA64:  lwz          r10, 0x2c(r11)
0x8259DA68:  lwz          r9, 0x28(r11)
0x8259DA6C:  add          r3, r10, r9
0x8259DA70:  stw          r3, 0x30(r11)
0x8259DA74:  lwz          r11, 0x2f0(r31)
0x8259DA78:  add          r11, r30, r11
0x8259DA7C:  lwz          r10, 0x2c(r11)
0x8259DA80:  stw          r10, 0x34(r11)
0x8259DA84:  lwz          r11, 0x2f0(r31)
0x8259DA88:  add          r11, r30, r11
0x8259DA8C:  lwz          r9, 0x2c(r11)
0x8259DA90:  stw          r9, 0x38(r11)
0x8259DA94:  lwz          r7, 0x244(r31)
0x8259DA98:  lwz          r5, 0x24c(r31)
0x8259DA9C:  lwz          r11, 0x2f0(r31)
0x8259DAA0:  add          r11, r30, r11
0x8259DAA4:  lwzx         r4, r28, r7
0x8259DAA8:  slwi         r3, r4, 2
0x8259DAAC:  lwzx         r10, r3, r5
0x8259DAB0:  lwz          r5, 0x68(r11)
0x8259DAB4:  rlwinm       r10, r10, 3, 0x1f, 0x1f
0x8259DAB8:  subfic       r4, r5, 0x2ee
0x8259DABC:  addi         r9, r10, 1
0x8259DAC0:  clrldi       r3, r4, 0x20
0x8259DAC4:  clrldi       r7, r9, 0x20
0x8259DAC8:  mulld        r10, r3, r7
0x8259DACC:  mulld        r9, r10, r8
0x8259DAD0:  sldi         r10, r9, 1
0x8259DAD4:  mulhdu       r9, r10, r6
0x8259DAD8:  subf         r8, r9, r10
0x8259DADC:  rldicl       r10, r8, 0x3f, 1
0x8259DAE0:  add          r7, r10, r9
0x8259DAE4:  rldicl       r6, r7, 0x37, 9
0x8259DAE8:  rlwinm       r5, r6, 0, 0, 0x1d
0x8259DAEC:  stw          r5, 0x4c(r11)
0x8259DAF0:  lwz          r11, 0x2f0(r31)
0x8259DAF4:  add          r11, r30, r11
0x8259DAF8:  lwz          r10, 0x28(r11)
0x8259DAFC:  lwz          r4, 0x4c(r11)
0x8259DB00:  cmplw        cr6, r4, r10
0x8259DB04:  ble          cr6, 0x8259db0c
0x8259DB08:  stw          r10, 0x4c(r11)
0x8259DB0C:  lwz          r10, 0x244(r31)
0x8259DB10:  lwz          r11, 0x24c(r31)
0x8259DB14:  lwzx         r3, r28, r10
0x8259DB18:  slwi         r9, r3, 2
0x8259DB1C:  lwzx         r9, r9, r11
0x8259DB20:  rlwinm       r8, r9, 0, 0, 0
0x8259DB24:  cmplwi       cr6, r8, 0
0x8259DB28:  bne          cr6, 0x8259db3c
0x8259DB2C:  rlwinm       r7, r9, 0, 3, 3
0x8259DB30:  mr           r5, r21
0x8259DB34:  cmplwi       cr6, r7, 0
0x8259DB38:  bne          cr6, 0x8259db40
0x8259DB3C:  mr           r5, r22
0x8259DB40:  lwzx         r6, r28, r10
0x8259DB44:  slwi         r4, r6, 2
0x8259DB48:  lwzx         r11, r4, r11
0x8259DB4C:  rlwinm       r10, r11, 3, 0x1f, 0x1f
0x8259DB50:  clrlwi       r3, r11, 0x10
0x8259DB54:  addi         r4, r10, 1
0x8259DB58:  bl           0x825a0ff0
0x8259DB5C:  lwz          r11, 0x2f0(r31)
0x8259DB60:  add          r11, r30, r11
0x8259DB64:  stw          r3, 0x3c(r11)
0x8259DB68:  lwz          r11, 0x2f0(r31)
0x8259DB6C:  add          r3, r30, r11
0x8259DB70:  lwz          r10, 0x3c(r3)
0x8259DB74:  cmplwi       cr6, r10, 0
0x8259DB78:  bne          cr6, 0x8259db94
0x8259DB7C:  lwz          r9, 0x1c(r3)
0x8259DB80:  mtctr        r9
0x8259DB84:  bctrl        
0x8259DB88:  lwz          r11, 0x2f0(r31)
0x8259DB8C:  add          r8, r30, r11
0x8259DB90:  stw          r22, 0x2c(r8)
0x8259DB94:  lwz          r10, -4(r23)
0x8259DB98:  lwz          r11, 0x2a4(r31)
0x8259DB9C:  addi         r10, r10, 1
0x8259DBA0:  lwz          r7, 0x2a8(r31)
0x8259DBA4:  lwz          r6, 8(r31)
0x8259DBA8:  slwi         r5, r7, 2
0x8259DBAC:  stw          r10, -4(r23)
0x8259DBB0:  lwz          r10, 0x2f0(r31)
0x8259DBB4:  add          r4, r30, r10
0x8259DBB8:  slwi         r10, r11, 1
0x8259DBBC:  add          r3, r11, r10
0x8259DBC0:  divwu        r11, r3, r5
0x8259DBC4:  subf         r10, r11, r6
0x8259DBC8:  stw          r10, 0x48(r4)
0x8259DBCC:  lwz          r11, 0x2f0(r31)
0x8259DBD0:  add          r9, r30, r11
0x8259DBD4:  stw          r22, 0x40(r9)
0x8259DBD8:  lwz          r11, 0x2f0(r31)
0x8259DBDC:  add          r11, r30, r11
0x8259DBE0:  lwz          r8, 0x6c(r11)
0x8259DBE4:  cmplwi       cr6, r8, 0
0x8259DBE8:  bne          cr6, 0x8259dbf0
0x8259DBEC:  stw          r25, 0x6c(r11)
0x8259DBF0:  lwz          r11, 0x2f0(r31)
0x8259DBF4:  add          r7, r30, r11
0x8259DBF8:  lwz          r6, 0x2c(r7)
0x8259DBFC:  cmplwi       cr6, r6, 0
0x8259DC00:  bne          cr6, 0x8259dc2c
0x8259DC04:  lwz          r11, 0x2e8(r31)
0x8259DC08:  lwz          r10, 0x244(r31)
0x8259DC0C:  addi         r11, r11, -1
0x8259DC10:  add          r3, r28, r10
0x8259DC14:  subf         r5, r26, r11
0x8259DC18:  addi         r4, r3, 4
0x8259DC1C:  slwi         r5, r5, 2
0x8259DC20:  stw          r11, 0x2e8(r31)
0x8259DC24:  bl           0x82434100
0x8259DC28:  b            0x8259dc38
0x8259DC2C:  addi         r26, r26, 1
0x8259DC30:  addi         r30, r30, 0x17c
0x8259DC34:  addi         r28, r28, 4
0x8259DC38:  lwz          r4, 0x2e8(r31)
0x8259DC3C:  cmplw        cr6, r26, r4
0x8259DC40:  blt          cr6, 0x8259d850
0x8259DC44:  lwz          r3, 0x2e8(r31)
0x8259DC48:  cmplwi       cr6, r3, 0
0x8259DC4C:  beq          cr6, 0x8259dcac
0x8259DC50:  lwz          r11, 0x2f0(r31)
0x8259DC54:  stw          r21, 0x288(r31)
0x8259DC58:  lwz          r10, 0x60(r11)
0x8259DC5C:  cmpwi        cr6, r10, 0
0x8259DC60:  bne          cr6, 0x8259dcac
0x8259DC64:  lwz          r3, 4(r23)
0x8259DC68:  cmplwi       cr6, r3, 0
0x8259DC6C:  bne          cr6, 0x8259dc94
0x8259DC70:  li           r3, -0xd
0x8259DC74:  bl           0x8259fc88
0x8259DC78:  stw          r3, 4(r23)
0x8259DC7C:  cmplwi       cr6, r3, 0
0x8259DC80:  beq          cr6, 0x8259dcac
0x8259DC84:  bl           0x8259ff10
0x8259DC88:  lwz          r3, 4(r23)
0x8259DC8C:  cmplwi       cr6, r3, 0
0x8259DC90:  beq          cr6, 0x8259dcac
0x8259DC94:  lis          r10, -0x7da6
0x8259DC98:  lis          r11, -0x7da6
0x8259DC9C:  addi         r6, r10, -0x3460
0x8259DCA0:  addi         r5, r11, -0x3478
0x8259DCA4:  addi         r4, r31, 0x340
0x8259DCA8:  bl           0x8259fd80
0x8259DCAC:  lwz          r9, 0x20(r31)
0x8259DCB0:  rlwinm       r8, r9, 0, 4, 4
0x8259DCB4:  cmplwi       cr6, r8, 0
0x8259DCB8:  bne          cr6, 0x8259dd10
0x8259DCBC:  lwz          r7, 0xe8(r31)
0x8259DCC0:  cmplwi       cr6, r7, 0
0x8259DCC4:  bne          cr6, 0x8259dd10
0x8259DCC8:  lwz          r3, 0(r23)
0x8259DCCC:  cmplwi       cr6, r3, 0
0x8259DCD0:  bne          cr6, 0x8259dcf8
0x8259DCD4:  li           r3, 0x50
0x8259DCD8:  bl           0x8259fc88
0x8259DCDC:  stw          r3, 0(r23)
0x8259DCE0:  cmplwi       cr6, r3, 0
0x8259DCE4:  beq          cr6, 0x8259dd10
0x8259DCE8:  bl           0x8259ff10
0x8259DCEC:  lwz          r3, 0(r23)
0x8259DCF0:  cmplwi       cr6, r3, 0
0x8259DCF4:  beq          cr6, 0x8259dd10
0x8259DCF8:  lis          r10, -0x7da6
0x8259DCFC:  lis          r11, -0x7da6
0x8259DD00:  addi         r6, r10, -0x34f8
0x8259DD04:  addi         r5, r11, -0x3540
0x8259DD08:  addi         r4, r31, 0x1f4
0x8259DD0C:  bl           0x8259fd80
0x8259DD10:  lis          r8, -0x7da6
0x8259DD14:  lwz          r6, 0xe8(r31)
0x8259DD18:  lis          r9, -0x7da6
0x8259DD1C:  lis          r10, -0x7da6
0x8259DD20:  lis          r11, -0x7da6
0x8259DD24:  addi         r5, r8, -0x3490
0x8259DD28:  addi         r4, r9, -0x34b0
0x8259DD2C:  addi         r3, r10, -0x34a0
0x8259DD30:  addi         r11, r11, -0x3480
0x8259DD34:  cmplwi       cr6, r6, 0
0x8259DD38:  stw          r5, 0x1ec(r31)
0x8259DD3C:  stw          r4, 0x1e4(r31)
0x8259DD40:  stw          r3, 0x1e8(r31)
0x8259DD44:  stw          r11, 0x1f0(r31)
0x8259DD48:  bne          cr6, 0x8259dd74
0x8259DD4C:  rlwinm       r10, r20, 0, 0xa, 0xa
0x8259DD50:  cmplwi       cr6, r10, 0
0x8259DD54:  bne          cr6, 0x8259dd74
0x8259DD58:  addi         r30, r31, 0xf0
0x8259DD5C:  lwz          r9, 0x100(r31)
0x8259DD60:  mr           r3, r30
0x8259DD64:  mtctr        r9
0x8259DD68:  bctrl        
0x8259DD6C:  cmplwi       cr6, r3, 0
0x8259DD70:  bne          cr6, 0x8259dd5c
0x8259DD74:  mr           r3, r31
0x8259DD78:  addi         r1, r1, 0x530
0x8259DD7C:  b            0x8242f8c8
0x8259DD80:  mflr         r12
0x8259DD84:  bl           0x8242f894
0x8259DD88:  stwu         r1, -0x80(r1)
0x8259DD8C:  mr           r31, r3
0x8259DD90:  li           r30, 0
0x8259DD94:  mr           r28, r4
0x8259DD98:  mr           r27, r30
0x8259DD9C:  cmplwi       cr6, r31, 0
0x8259DDA0:  bne          cr6, 0x8259ddac
0x8259DDA4:  addi         r1, r1, 0x80
0x8259DDA8:  b            0x8242f8e4
0x8259DDAC:  bl           0x825a03b8
0x8259DDB0:  lwz          r11, 0x264(r31)
0x8259DDB4:  cmplwi       cr6, r11, 0
0x8259DDB8:  beq          cr6, 0x8259ddd0
0x8259DDBC:  lwz          r10, 0x29c(r31)
0x8259DDC0:  stw          r30, 0x264(r31)
0x8259DDC4:  subf         r11, r11, r10
0x8259DDC8:  add          r9, r11, r3
0x8259DDCC:  stw          r9, 0x29c(r31)
0x8259DDD0:  lwz          r11, 0x274(r31)
0x8259DDD4:  cmplwi       cr6, r11, 0
0x8259DDD8:  beq          cr6, 0x8259de24
0x8259DDDC:  lwz          r10, 0x278(r31)
0x8259DDE0:  subf         r11, r11, r3
0x8259DDE4:  stw          r30, 0x274(r31)
0x8259DDE8:  cmplw        cr6, r11, r10
0x8259DDEC:  ble          cr6, 0x8259de0c
0x8259DDF0:  lwz          r7, 0x27c(r31)
0x8259DDF4:  lwz          r8, 0xc(r31)
0x8259DDF8:  stw          r10, 0x280(r31)
0x8259DDFC:  stw          r11, 0x278(r31)
0x8259DE00:  stw          r7, 0x284(r31)
0x8259DE04:  stw          r8, 0x27c(r31)
0x8259DE08:  b            0x8259de24
0x8259DE0C:  lwz          r6, 0x280(r31)
0x8259DE10:  cmplw        cr6, r11, r6
0x8259DE14:  ble          cr6, 0x8259de24
0x8259DE18:  lwz          r5, 0xc(r31)
0x8259DE1C:  stw          r11, 0x280(r31)
0x8259DE20:  stw          r5, 0x284(r31)
0x8259DE24:  cmpwi        cr6, r28, 0
0x8259DE28:  bne          cr6, 0x8259de40
0x8259DE2C:  lwz          r4, 0xdc(r31)
0x8259DE30:  cmpwi        cr6, r4, 0
0x8259DE34:  beq          cr6, 0x8259de68
0x8259DE38:  li           r27, 1
0x8259DE3C:  b            0x8259de68
0x8259DE40:  lwz          r11, 0xdc(r31)
0x8259DE44:  cmpwi        cr6, r11, 0
0x8259DE48:  bne          cr6, 0x8259de68
0x8259DE4C:  lwz          r11, 0x268(r31)
0x8259DE50:  cmplwi       cr6, r11, 0
0x8259DE54:  beq          cr6, 0x8259de64
0x8259DE58:  subf         r10, r11, r3
0x8259DE5C:  stw          r10, 0x330(r31)
0x8259DE60:  b            0x8259de68
0x8259DE64:  stw          r30, 0x330(r31)
0x8259DE68:  lwz          r9, 0x2e8(r31)
0x8259DE6C:  stw          r28, 0xdc(r31)
0x8259DE70:  cmplwi       cr6, r9, 0
0x8259DE74:  ble          cr6, 0x8259dea8
0x8259DE78:  mr           r29, r30
0x8259DE7C:  lwz          r11, 0x2f0(r31)
0x8259DE80:  mr           r4, r28
0x8259DE84:  add          r3, r29, r11
0x8259DE88:  lwz          r8, 0x14(r3)
0x8259DE8C:  mtctr        r8
0x8259DE90:  bctrl        
0x8259DE94:  lwz          r7, 0x2e8(r31)
0x8259DE98:  addi         r30, r30, 1
0x8259DE9C:  addi         r29, r29, 0x17c
0x8259DEA0:  cmplw        cr6, r30, r7
0x8259DEA4:  blt          cr6, 0x8259de7c
0x8259DEA8:  lwz          r6, 0x258(r31)
0x8259DEAC:  cmplwi       cr6, r6, 0
0x8259DEB0:  beq          cr6, 0x8259dec8
0x8259DEB4:  lwz          r5, 0x2e8(r31)
0x8259DEB8:  cmplwi       cr6, r5, 0
0x8259DEBC:  beq          cr6, 0x8259dec8
0x8259DEC0:  mr           r3, r31
0x8259DEC4:  bl           0x8259c4f0
0x8259DEC8:  cmpwi        cr6, r27, 0
0x8259DECC:  beq          cr6, 0x8259dee0
0x8259DED0:  bl           0x825a03b8
0x8259DED4:  lwz          r4, 0x330(r31)
0x8259DED8:  subf         r3, r4, r3
0x8259DEDC:  stw          r3, 0x268(r31)
0x8259DEE0:  lwz          r3, 0xdc(r31)
0x8259DEE4:  addi         r1, r1, 0x80
0x8259DEE8:  b            0x8242f8e4
0x8259DEEC:  .align 0                       # padding
0x8259DEF0:  mr           r7, r4
0x8259DEF4:  cmplwi       cr6, r3, 0
0x8259DEF8:  beqlr        cr6
0x8259DEFC:  lwz          r8, 0x2e8(r3)
0x8259DF00:  cmplwi       cr6, r8, 0
0x8259DF04:  beqlr        cr6
0x8259DF08:  li           r10, 0
0x8259DF0C:  cmpwi        cr6, r8, 0
0x8259DF10:  blelr        cr6
0x8259DF14:  lwz          r9, 0x250(r3)
0x8259DF18:  lwz          r11, 0x244(r3)
0x8259DF1C:  lwz          r6, 0(r11)
0x8259DF20:  slwi         r4, r6, 2
0x8259DF24:  lwzx         r6, r4, r9
0x8259DF28:  cmpw         cr6, r6, r7
0x8259DF2C:  beq          cr6, 0x8259df44
0x8259DF30:  addi         r10, r10, 1
0x8259DF34:  addi         r11, r11, 4
0x8259DF38:  cmpw         cr6, r10, r8
0x8259DF3C:  blt          cr6, 0x8259df1c
0x8259DF40:  blr          
0x8259DF44:  cmpwi        cr6, r10, -1
0x8259DF48:  beqlr        cr6
0x8259DF4C:  mulli        r11, r10, 0x17c
0x8259DF50:  lwz          r10, 0x2f0(r3)
0x8259DF54:  add          r4, r11, r10
0x8259DF58:  lwz          r10, 0xc(r4)
0x8259DF5C:  cmplwi       cr6, r10, 0
0x8259DF60:  beqlr        cr6
0x8259DF64:  lwz          r10, 0x2f0(r3)
0x8259DF68:  mr           r4, r5
0x8259DF6C:  add          r3, r11, r10
0x8259DF70:  lwz          r9, 0xc(r3)
0x8259DF74:  mtctr        r9
0x8259DF78:  bctr         
0x8259DF7C:  .align 0                       # padding
0x8259DF80:  mr           r7, r4
0x8259DF84:  mr           r4, r5
0x8259DF88:  cmplwi       cr6, r3, 0
0x8259DF8C:  beqlr        cr6
0x8259DF90:  lwz          r8, 0x2e8(r3)
0x8259DF94:  cmplwi       cr6, r8, 0
0x8259DF98:  beqlr        cr6
0x8259DF9C:  li           r10, 0
0x8259DFA0:  cmpwi        cr6, r8, 0
0x8259DFA4:  blelr        cr6
0x8259DFA8:  lwz          r9, 0x250(r3)
0x8259DFAC:  lwz          r11, 0x244(r3)
0x8259DFB0:  lwz          r5, 0(r11)
0x8259DFB4:  slwi         r5, r5, 2
0x8259DFB8:  lwzx         r5, r5, r9
0x8259DFBC:  cmpw         cr6, r5, r7
0x8259DFC0:  beq          cr6, 0x8259dfd8
0x8259DFC4:  addi         r10, r10, 1
0x8259DFC8:  addi         r11, r11, 4
0x8259DFCC:  cmpw         cr6, r10, r8
0x8259DFD0:  blt          cr6, 0x8259dfb0
0x8259DFD4:  blr          
0x8259DFD8:  cmpwi        cr6, r10, -1
0x8259DFDC:  beqlr        cr6
0x8259DFE0:  mulli        r11, r10, 0x17c
0x8259DFE4:  lwz          r10, 0x2f0(r3)
0x8259DFE8:  add          r10, r11, r10
0x8259DFEC:  lwz          r9, 0x20(r10)
0x8259DFF0:  cmplwi       cr6, r9, 0
0x8259DFF4:  beqlr        cr6
0x8259DFF8:  lwz          r10, 0x2f0(r3)
0x8259DFFC:  mr           r5, r6
0x8259E000:  add          r3, r11, r10
0x8259E004:  lwz          r8, 0x20(r3)
0x8259E008:  mtctr        r8
0x8259E00C:  bctr         
0x8259E010:  mflr         r12
0x8259E014:  bl           0x8242f878
0x8259E018:  stwu         r1, -0xc0(r1)
0x8259E01C:  mr           r30, r3
0x8259E020:  li           r26, 0
0x8259E024:  mr           r20, r4
0x8259E028:  mr           r21, r26
0x8259E02C:  cmplwi       cr6, r30, 0
0x8259E030:  beq          cr6, 0x8259e0f4
0x8259E034:  lwz          r11, 0x2e8(r30)
0x8259E038:  mr           r24, r26
0x8259E03C:  mr           r22, r26
0x8259E040:  cmplwi       cr6, r11, 0
0x8259E044:  ble          cr6, 0x8259e0f4
0x8259E048:  mr           r27, r26
0x8259E04C:  lis          r23, -0x7d8c
0x8259E050:  li           r25, 1
0x8259E054:  lwz          r11, 0x2f0(r30)
0x8259E058:  add          r10, r27, r11
0x8259E05C:  lwz          r9, 0x18(r10)
0x8259E060:  cmplwi       cr6, r9, 0
0x8259E064:  beq          cr6, 0x8259e0cc
0x8259E068:  cmpwi        cr6, r24, 0
0x8259E06C:  bne          cr6, 0x8259e080
0x8259E070:  addi         r4, r30, 0x340
0x8259E074:  lwz          r3, -0x61f0(r23)
0x8259E078:  mr           r24, r25
0x8259E07C:  bl           0x8259fff0
0x8259E080:  cmpwi        cr6, r20, 0
0x8259E084:  mr           r4, r25
0x8259E088:  bne          cr6, 0x8259e090
0x8259E08C:  mr           r4, r26
0x8259E090:  lwz          r11, 0x2f0(r30)
0x8259E094:  add          r3, r27, r11
0x8259E098:  lwz          r8, 0x18(r3)
0x8259E09C:  mtctr        r8
0x8259E0A0:  bctrl        
0x8259E0A4:  cmpwi        cr6, r3, 0
0x8259E0A8:  beq          cr6, 0x8259e100
0x8259E0AC:  lwz          r7, 0x288(r30)
0x8259E0B0:  mr           r21, r25
0x8259E0B4:  cmplwi       cr6, r7, 0
0x8259E0B8:  bne          cr6, 0x8259e0cc
0x8259E0BC:  stw          r25, 0x288(r30)
0x8259E0C0:  stw          r26, 0x268(r30)
0x8259E0C4:  stw          r26, 0x328(r30)
0x8259E0C8:  stw          r26, 0x330(r30)
0x8259E0CC:  lwz          r6, 0x2e8(r30)
0x8259E0D0:  addi         r22, r22, 1
0x8259E0D4:  addi         r27, r27, 0x17c
0x8259E0D8:  cmplw        cr6, r22, r6
0x8259E0DC:  blt          cr6, 0x8259e054
0x8259E0E0:  cmpwi        cr6, r24, 0
0x8259E0E4:  beq          cr6, 0x8259e0f4
0x8259E0E8:  addi         r4, r30, 0x340
0x8259E0EC:  lwz          r3, -0x61f0(r23)
0x8259E0F0:  bl           0x825a0020
0x8259E0F4:  mr           r3, r21
0x8259E0F8:  addi         r1, r1, 0xc0
0x8259E0FC:  b            0x8242f8c8
0x8259E100:  lwz          r7, 0x288(r30)
0x8259E104:  cmplwi       cr6, r7, 0
0x8259E108:  beq          cr6, 0x8259e0cc
0x8259E10C:  lwz          r11, 0x2f0(r30)
0x8259E110:  add          r11, r27, r11
0x8259E114:  lwz          r6, 0x2c(r11)
0x8259E118:  stw          r6, 0x34(r11)
0x8259E11C:  lwz          r11, 0x2f0(r30)
0x8259E120:  add          r11, r27, r11
0x8259E124:  lwz          r5, 0x2c(r11)
0x8259E128:  stw          r5, 0x38(r11)
0x8259E12C:  lwz          r11, 0x2f0(r30)
0x8259E130:  add          r4, r27, r11
0x8259E134:  stw          r26, 0x40(r4)
0x8259E138:  lwz          r3, 0xc(r30)
0x8259E13C:  cmplwi       cr6, r3, 1
0x8259E140:  bne          cr6, 0x8259e150
0x8259E144:  lwz          r11, 0x2d4(r30)
0x8259E148:  cmplwi       cr6, r11, 1
0x8259E14C:  bne          cr6, 0x8259e1c8
0x8259E150:  lwz          r11, 0x2f0(r30)
0x8259E154:  add          r31, r27, r11
0x8259E158:  lwz          r11, 0x4c(r31)
0x8259E15C:  lwz          r10, 0x40(r31)
0x8259E160:  cmplw        cr6, r10, r11
0x8259E164:  bge          cr6, 0x8259e1c8
0x8259E168:  subf         r29, r10, r11
0x8259E16C:  lwz          r10, 0x38(r31)
0x8259E170:  lwz          r3, 0x2c(r31)
0x8259E174:  li           r4, 0
0x8259E178:  subf         r11, r29, r10
0x8259E17C:  cmplw        cr6, r11, r3
0x8259E180:  stw          r11, 0x38(r31)
0x8259E184:  bge          cr6, 0x8259e1b0
0x8259E188:  lwz          r10, 0x28(r31)
0x8259E18C:  subf         r28, r11, r3
0x8259E190:  add          r9, r10, r11
0x8259E194:  subf         r5, r28, r29
0x8259E198:  stw          r9, 0x38(r31)
0x8259E19C:  bl           0x8242fed0
0x8259E1A0:  lwz          r3, 0x38(r31)
0x8259E1A4:  mr           r5, r28
0x8259E1A8:  li           r4, 0
0x8259E1AC:  b            0x8259e1b8
0x8259E1B0:  mr           r5, r29
0x8259E1B4:  mr           r3, r11
0x8259E1B8:  bl           0x8242fed0
0x8259E1BC:  lwz          r11, 0x40(r31)
0x8259E1C0:  add          r8, r11, r29
0x8259E1C4:  stw          r8, 0x40(r31)
0x8259E1C8:  stw          r26, 0x288(r30)
0x8259E1CC:  b            0x8259e0cc
0x8259E1D0:  mflr         r12
0x8259E1D4:  bl           0x8242f87c
0x8259E1D8:  stwu         r1, -0xc0(r1)
0x8259E1DC:  mr           r31, r3
0x8259E1E0:  li           r21, 0
0x8259E1E4:  cmplwi       cr6, r31, 0
0x8259E1E8:  mr           r23, r21
0x8259E1EC:  beq          cr6, 0x8259e764
0x8259E1F0:  lwz          r11, 0x2d4(r31)
0x8259E1F4:  lwz          r10, 0xc(r31)
0x8259E1F8:  cmplw        cr6, r11, r10
0x8259E1FC:  beq          cr6, 0x8259e764
0x8259E200:  lwz          r9, 0x2dc(r31)
0x8259E204:  cmplwi       cr6, r9, 0
0x8259E208:  beq          cr6, 0x8259e23c
0x8259E20C:  lwz          r8, 0x288(r31)
0x8259E210:  stw          r21, 0x2dc(r31)
0x8259E214:  cmplwi       cr6, r8, 0
0x8259E218:  beq          cr6, 0x8259e230
0x8259E21C:  li           r4, 0
0x8259E220:  bl           0x8259e010
0x8259E224:  li           r4, 1
0x8259E228:  mr           r3, r31
0x8259E22C:  bl           0x8259e010
0x8259E230:  stw          r21, 0x268(r31)
0x8259E234:  stw          r21, 0x330(r31)
0x8259E238:  stw          r21, 0x328(r31)
0x8259E23C:  li           r22, 1
0x8259E240:  stw          r21, 0x2f8(r31)
0x8259E244:  stw          r22, 0x11c(r31)
0x8259E248:  bl           0x825a03b8
0x8259E24C:  lwz          r10, 0x2b0(r31)
0x8259E250:  stw          r3, 0x260(r31)
0x8259E254:  addic.       r11, r10, -1
0x8259E258:  stw          r11, 0x2b0(r31)
0x8259E25C:  bge          0x8259e26c
0x8259E260:  lwz          r11, 0x2ac(r31)
0x8259E264:  addi         r7, r11, -1
0x8259E268:  stw          r7, 0x2b0(r31)
0x8259E26C:  lwz          r6, 0x2b0(r31)
0x8259E270:  lwz          r4, 0x2b4(r31)
0x8259E274:  slwi         r5, r6, 2
0x8259E278:  stwx         r3, r5, r4
0x8259E27C:  lwz          r3, 0x2b0(r31)
0x8259E280:  lwz          r11, 0x2bc(r31)
0x8259E284:  lwz          r9, 0x294(r31)
0x8259E288:  slwi         r8, r3, 2
0x8259E28C:  stwx         r9, r11, r8
0x8259E290:  lwz          r7, 0x2b0(r31)
0x8259E294:  lwz          r6, 0x2b8(r31)
0x8259E298:  lwz          r5, 0x298(r31)
0x8259E29C:  slwi         r4, r7, 2
0x8259E2A0:  stwx         r5, r6, r4
0x8259E2A4:  lwz          r3, 0x2b0(r31)
0x8259E2A8:  lwz          r11, 0x2c0(r31)
0x8259E2AC:  lwz          r9, 0x29c(r31)
0x8259E2B0:  slwi         r8, r3, 2
0x8259E2B4:  stwx         r9, r11, r8
0x8259E2B8:  lwz          r7, 0x2b0(r31)
0x8259E2BC:  lwz          r6, 0x124(r31)
0x8259E2C0:  lwz          r5, 0x2c4(r31)
0x8259E2C4:  slwi         r4, r7, 2
0x8259E2C8:  stwx         r6, r5, r4
0x8259E2CC:  lwz          r3, 0x2b0(r31)
0x8259E2D0:  lwz          r11, 0x128(r31)
0x8259E2D4:  lwz          r9, 0x2c8(r31)
0x8259E2D8:  slwi         r8, r3, 2
0x8259E2DC:  stwx         r11, r9, r8
0x8259E2E0:  lwz          r7, 0x2b0(r31)
0x8259E2E4:  lwz          r6, 0x2cc(r31)
0x8259E2E8:  lwz          r5, 0x12c(r31)
0x8259E2EC:  slwi         r4, r7, 2
0x8259E2F0:  stwx         r5, r6, r4
0x8259E2F4:  lwz          r3, 0x25c(r31)
0x8259E2F8:  cmplwi       cr6, r3, 0
0x8259E2FC:  bne          cr6, 0x8259e33c
0x8259E300:  lwz          r11, 0x2a8(r31)
0x8259E304:  slwi         r8, r10, 2
0x8259E308:  lwz          r9, 0x2a4(r31)
0x8259E30C:  mulli        r7, r11, 0x3e8
0x8259E310:  lwz          r6, 0x260(r31)
0x8259E314:  lwz          r5, 0x2b4(r31)
0x8259E318:  divdu        r4, r7, r9
0x8259E31C:  slwi         r3, r4, 0
0x8259E320:  subf         r11, r3, r6
0x8259E324:  stwx         r11, r8, r5
0x8259E328:  lwz          r10, 0x260(r31)
0x8259E32C:  stw          r21, 0x12c(r31)
0x8259E330:  stw          r21, 0x128(r31)
0x8259E334:  stw          r10, 0x25c(r31)
0x8259E338:  b            0x8259e350
0x8259E33C:  lwz          r9, 0x2e8(r31)
0x8259E340:  cmplwi       cr6, r9, 0
0x8259E344:  beq          cr6, 0x8259e350
0x8259E348:  mr           r3, r31
0x8259E34C:  bl           0x8259c4f0
0x8259E350:  lwz          r8, 0x260(r31)
0x8259E354:  lwz          r7, 0x110(r31)
0x8259E358:  lwz          r29, 0xe4(r31)
0x8259E35C:  cmplwi       cr6, r7, 0
0x8259E360:  stw          r8, 0x274(r31)
0x8259E364:  beq          cr6, 0x8259e36c
0x8259E368:  stw          r22, 0x1c(r31)
0x8259E36C:  lwz          r6, 0x1c(r31)
0x8259E370:  cmplwi       cr6, r6, 0
0x8259E374:  beq          cr6, 0x8259e384
0x8259E378:  li           r3, 1
0x8259E37C:  addi         r1, r1, 0xc0
0x8259E380:  b            0x8242f8cc
0x8259E384:  lwz          r5, 0xd0(r31)
0x8259E388:  lis          r11, -0x7d8c
0x8259E38C:  mr           r26, r21
0x8259E390:  cmpwi        cr6, r5, 0
0x8259E394:  addi         r24, r11, -0x61f4
0x8259E398:  ble          cr6, 0x8259e5d0
0x8259E39C:  lwz          r4, 0x2e8(r31)
0x8259E3A0:  mr           r30, r21
0x8259E3A4:  lwz          r25, 0(r29)
0x8259E3A8:  addi         r27, r29, 4
0x8259E3AC:  cmpwi        cr6, r4, 0
0x8259E3B0:  ble          cr6, 0x8259e5a8
0x8259E3B4:  lwz          r11, 0x244(r31)
0x8259E3B8:  lwz          r3, 0(r11)
0x8259E3BC:  cmpw         cr6, r3, r26
0x8259E3C0:  beq          cr6, 0x8259e3dc
0x8259E3C4:  lwz          r10, 0x2e8(r31)
0x8259E3C8:  addi         r30, r30, 1
0x8259E3CC:  addi         r11, r11, 4
0x8259E3D0:  cmpw         cr6, r30, r10
0x8259E3D4:  blt          cr6, 0x8259e3b8
0x8259E3D8:  b            0x8259e5a8
0x8259E3DC:  cmpwi        cr6, r30, -1
0x8259E3E0:  beq          cr6, 0x8259e5a8
0x8259E3E4:  cmplwi       cr6, r25, 0
0x8259E3E8:  beq          cr6, 0x8259e5a8
0x8259E3EC:  addi         r9, r27, 4
0x8259E3F0:  stw          r27, 0x58(r1)
0x8259E3F4:  lwz          r28, 0(r27)
0x8259E3F8:  cmpwi        cr6, r23, 0
0x8259E3FC:  stw          r9, 0x58(r1)
0x8259E400:  bne          cr6, 0x8259e414
0x8259E404:  addi         r4, r31, 0x340
0x8259E408:  lwz          r3, 4(r24)
0x8259E40C:  mr           r23, r22
0x8259E410:  bl           0x8259fff0
0x8259E414:  cmplwi       cr6, r28, 0
0x8259E418:  beq          cr6, 0x8259e5a8
0x8259E41C:  mulli        r30, r30, 0x17c
0x8259E420:  lwz          r11, 0x2f0(r31)
0x8259E424:  addi         r7, r1, 0x58
0x8259E428:  addi         r5, r1, 0x50
0x8259E42C:  lwz          r6, 0x58(r1)
0x8259E430:  add          r11, r30, r11
0x8259E434:  addi         r4, r1, 0x54
0x8259E438:  lwz          r8, 0x28(r11)
0x8259E43C:  lwz          r10, 0x40(r11)
0x8259E440:  lwz          r3, 0x3c(r11)
0x8259E444:  subf         r29, r10, r8
0x8259E448:  bl           0x825a1328
0x8259E44C:  lwz          r11, 0x50(r1)
0x8259E450:  cmplw        cr6, r11, r28
0x8259E454:  ble          cr6, 0x8259e460
0x8259E458:  mr           r11, r28
0x8259E45C:  stw          r11, 0x50(r1)
0x8259E460:  subf         r28, r11, r28
0x8259E464:  cmplw        cr6, r11, r29
0x8259E468:  ble          cr6, 0x8259e4dc
0x8259E46C:  lwz          r10, 0x2f0(r31)
0x8259E470:  subf         r11, r29, r11
0x8259E474:  add          r10, r30, r10
0x8259E478:  neg          r5, r11
0x8259E47C:  lwz          r9, 0x38(r10)
0x8259E480:  add          r4, r9, r11
0x8259E484:  stw          r4, 0x38(r10)
0x8259E488:  lwz          r11, 0x2f0(r31)
0x8259E48C:  add          r11, r30, r11
0x8259E490:  addi         r3, r11, 0x40
0x8259E494:  mfmsr        r6
0x8259E498:  mtmsrd       r13, 1
0x8259E49C:  lwarx        r8, 0, r3
0x8259E4A0:  add          r7, r5, r8
0x8259E4A4:  stwcx.       r7, 0, r3
0x8259E4A8:  mtmsrd       r6, 1
0x8259E4AC:  bne          0x8259e494
0x8259E4B0:  lwz          r11, 0x2f0(r31)
0x8259E4B4:  add          r11, r30, r11
0x8259E4B8:  lwz          r10, 0x30(r11)
0x8259E4BC:  lwz          r9, 0x38(r11)
0x8259E4C0:  cmplw        cr6, r9, r10
0x8259E4C4:  ble          cr6, 0x8259e4d8
0x8259E4C8:  lwz          r7, 0x28(r11)
0x8259E4CC:  slwi         r8, r9, 0
0x8259E4D0:  subf         r6, r7, r8
0x8259E4D4:  stw          r6, 0x38(r11)
0x8259E4D8:  lwz          r11, 0x50(r1)
0x8259E4DC:  lwz          r10, 0x2f0(r31)
0x8259E4E0:  add          r10, r30, r10
0x8259E4E4:  addi         r10, r10, 0x40
0x8259E4E8:  mfmsr        r3
0x8259E4EC:  mtmsrd       r13, 1
0x8259E4F0:  lwarx        r5, 0, r10
0x8259E4F4:  add          r4, r11, r5
0x8259E4F8:  stwcx.       r4, 0, r10
0x8259E4FC:  mtmsrd       r3, 1
0x8259E500:  bne          0x8259e4e8
0x8259E504:  lwz          r11, 0x2f0(r31)
0x8259E508:  lwz          r5, 0x50(r1)
0x8259E50C:  add          r11, r30, r11
0x8259E510:  lwz          r3, 0x34(r11)
0x8259E514:  lwz          r9, 0x30(r11)
0x8259E518:  subf         r29, r3, r9
0x8259E51C:  cmplw        cr6, r29, r5
0x8259E520:  bge          cr6, 0x8259e580
0x8259E524:  cmplwi       cr6, r29, 0
0x8259E528:  beq          cr6, 0x8259e550
0x8259E52C:  mr           r5, r29
0x8259E530:  lwz          r4, 0x54(r1)
0x8259E534:  bl           0x82434100
0x8259E538:  lwz          r8, 0x50(r1)
0x8259E53C:  lwz          r7, 0x54(r1)
0x8259E540:  subf         r5, r29, r8
0x8259E544:  add          r6, r7, r29
0x8259E548:  stw          r5, 0x50(r1)
0x8259E54C:  stw          r6, 0x54(r1)
0x8259E550:  lwz          r11, 0x2f0(r31)
0x8259E554:  lwz          r4, 0x54(r1)
0x8259E558:  add          r3, r30, r11
0x8259E55C:  lwz          r3, 0x2c(r3)
0x8259E560:  bl           0x82434100
0x8259E564:  lwz          r11, 0x2f0(r31)
0x8259E568:  lwz          r9, 0x50(r1)
0x8259E56C:  add          r11, r30, r11
0x8259E570:  lwz          r10, 0x2c(r11)
0x8259E574:  add          r10, r10, r9
0x8259E578:  stw          r10, 0x34(r11)
0x8259E57C:  b            0x8259e5a0
0x8259E580:  lwz          r4, 0x54(r1)
0x8259E584:  bl           0x82434100
0x8259E588:  lwz          r11, 0x2f0(r31)
0x8259E58C:  lwz          r9, 0x50(r1)
0x8259E590:  add          r11, r30, r11
0x8259E594:  lwz          r10, 0x34(r11)
0x8259E598:  add          r9, r10, r9
0x8259E59C:  stw          r9, 0x34(r11)
0x8259E5A0:  cmplwi       cr6, r28, 0
0x8259E5A4:  bne          cr6, 0x8259e420
0x8259E5A8:  lwz          r8, 0xd0(r31)
0x8259E5AC:  addi         r26, r26, 1
0x8259E5B0:  add          r29, r25, r27
0x8259E5B4:  cmpw         cr6, r26, r8
0x8259E5B8:  blt          cr6, 0x8259e39c
0x8259E5BC:  cmpwi        cr6, r23, 0
0x8259E5C0:  beq          cr6, 0x8259e5d0
0x8259E5C4:  addi         r4, r31, 0x340
0x8259E5C8:  lwz          r3, 4(r24)
0x8259E5CC:  bl           0x825a0020
0x8259E5D0:  bl           0x825a03b8
0x8259E5D4:  lwz          r7, 0x28c(r31)
0x8259E5D8:  mr           r28, r3
0x8259E5DC:  cmplwi       cr6, r7, 0
0x8259E5E0:  beq          cr6, 0x8259e6d8
0x8259E5E4:  lwz          r6, 0xb8(r31)
0x8259E5E8:  cmplwi       cr6, r6, 0
0x8259E5EC:  beq          cr6, 0x8259e6d8
0x8259E5F0:  lwz          r5, 0x2f4(r31)
0x8259E5F4:  cmplwi       cr6, r5, 0
0x8259E5F8:  bne          cr6, 0x8259e60c
0x8259E5FC:  li           r4, 0
0x8259E600:  lwz          r5, 0xc4(r31)
0x8259E604:  lwz          r3, 0xbc(r31)
0x8259E608:  bl           0x8242fed0
0x8259E60C:  lwz          r4, 0xe8(r31)
0x8259E610:  cmplwi       cr6, r4, 0
0x8259E614:  bne          cr6, 0x8259e69c
0x8259E618:  lwz          r3, 0x114(r31)
0x8259E61C:  cmplwi       cr6, r3, 0
0x8259E620:  bne          cr6, 0x8259e69c
0x8259E624:  lwz          r11, 0xd4(r31)
0x8259E628:  lwz          r10, 0x138(r31)
0x8259E62C:  cmplw        cr6, r11, r10
0x8259E630:  blt          cr6, 0x8259e638
0x8259E634:  lwz          r11, 0x138(r31)
0x8259E638:  slwi         r10, r11, 1
0x8259E63C:  lwz          r9, 0x13c(r31)
0x8259E640:  add          r8, r11, r10
0x8259E644:  srwi         r11, r8, 2
0x8259E648:  cmplw        cr6, r9, r11
0x8259E64C:  bge          cr6, 0x8259e69c
0x8259E650:  addi         r30, r31, 0x1f4
0x8259E654:  lwz          r3, 0(r24)
0x8259E658:  mr           r4, r30
0x8259E65C:  bl           0x8259ff98
0x8259E660:  cmpwi        cr6, r3, 0
0x8259E664:  beq          cr6, 0x8259e688
0x8259E668:  lwz          r7, 0x100(r31)
0x8259E66C:  addi         r3, r31, 0xf0
0x8259E670:  mtctr        r7
0x8259E674:  bctrl        
0x8259E678:  mr           r4, r30
0x8259E67C:  lwz          r3, 0(r24)
0x8259E680:  bl           0x825a0020
0x8259E684:  b            0x8259e69c
0x8259E688:  lwz          r6, 0x20(r31)
0x8259E68C:  rlwinm       r5, r6, 0, 4, 4
0x8259E690:  cmplwi       cr6, r5, 0
0x8259E694:  bne          cr6, 0x8259e69c
0x8259E698:  bl           0x825a00f8
0x8259E69C:  lwz          r4, 0xc(r31)
0x8259E6A0:  addi         r8, r31, 0x300
0x8259E6A4:  lwz          r7, 0xec(r31)
0x8259E6A8:  mr           r6, r29
0x8259E6AC:  slwi         r11, r4, 2
0x8259E6B0:  lwz          r10, 0x24(r31)
0x8259E6B4:  lwz          r9, 0x20(r31)
0x8259E6B8:  add          r11, r11, r7
0x8259E6BC:  lwz          r5, 0xc0(r31)
0x8259E6C0:  lwz          r4, 0xbc(r31)
0x8259E6C4:  lwz          r3, 0xb8(r31)
0x8259E6C8:  lwz          r7, -4(r11)
0x8259E6CC:  clrlwi       r7, r7, 0x1f
0x8259E6D0:  bl           0x825a4dd8
0x8259E6D4:  stw          r3, 0x2e0(r31)
0x8259E6D8:  lwz          r6, 0x2e8(r31)
0x8259E6DC:  cmplwi       cr6, r6, 0
0x8259E6E0:  beq          cr6, 0x8259e6ec
0x8259E6E4:  mr           r3, r31
0x8259E6E8:  bl           0x8259c4f0
0x8259E6EC:  lwz          r11, 0x258(r31)
0x8259E6F0:  li           r5, -1
0x8259E6F4:  lwz          r4, 0x268(r31)
0x8259E6F8:  addi         r3, r11, 1
0x8259E6FC:  cmplwi       cr6, r4, 0
0x8259E700:  stw          r5, 0xb4(r31)
0x8259E704:  stw          r3, 0x258(r31)
0x8259E708:  bne          cr6, 0x8259e724
0x8259E70C:  bl           0x825a03b8
0x8259E710:  lwz          r11, 0x258(r31)
0x8259E714:  stw          r3, 0x268(r31)
0x8259E718:  addi         r11, r11, -1
0x8259E71C:  stw          r21, 0x330(r31)
0x8259E720:  stw          r11, 0x26c(r31)
0x8259E724:  bl           0x825a03b8
0x8259E728:  lwz          r9, 0x260(r31)
0x8259E72C:  lwz          r7, 0x298(r31)
0x8259E730:  mr           r11, r3
0x8259E734:  subf         r9, r9, r28
0x8259E738:  lwz          r8, 0x294(r31)
0x8259E73C:  lwz          r10, 0xc(r31)
0x8259E740:  add          r7, r9, r7
0x8259E744:  stw          r21, 0x11c(r31)
0x8259E748:  subf         r9, r28, r11
0x8259E74C:  stw          r11, 0x264(r31)
0x8259E750:  add          r6, r9, r8
0x8259E754:  stw          r10, 0x2d4(r31)
0x8259E758:  stw          r7, 0x298(r31)
0x8259E75C:  stw          r10, 0x10(r31)
0x8259E760:  stw          r6, 0x294(r31)
0x8259E764:  li           r3, 0
0x8259E768:  addi         r1, r1, 0xc0
0x8259E76C:  b            0x8242f8cc
0x8259E770:  mflr         r12
0x8259E774:  bl           0x8242f88c
0x8259E778:  stwu         r1, -0x90(r1)
0x8259E77C:  mr           r31, r3
0x8259E780:  li           r26, 0
0x8259E784:  cmplwi       cr6, r31, 0
0x8259E788:  mr           r7, r26
0x8259E78C:  beq          cr6, 0x8259e924
0x8259E790:  li           r25, 1
0x8259E794:  lwz          r11, 0x2e8(r31)
0x8259E798:  mr           r8, r26
0x8259E79C:  stw          r26, 0x2f8(r31)
0x8259E7A0:  cmplwi       cr6, r11, 0
0x8259E7A4:  stw          r25, 0x11c(r31)
0x8259E7A8:  stw          r26, 0x11c(r31)
0x8259E7AC:  ble          cr6, 0x8259e8c4
0x8259E7B0:  mr           r10, r26
0x8259E7B4:  lwz          r11, 0x2f0(r31)
0x8259E7B8:  add          r11, r10, r11
0x8259E7BC:  lwz          r9, 0x5c(r11)
0x8259E7C0:  cmplwi       cr6, r9, 0
0x8259E7C4:  beq          cr6, 0x8259e7f0
0x8259E7C8:  stw          r26, 0x5c(r11)
0x8259E7CC:  lwz          r11, 0xc(r31)
0x8259E7D0:  cmplwi       cr6, r11, 1
0x8259E7D4:  ble          cr6, 0x8259e7f0
0x8259E7D8:  lwz          r9, 0x2f0(r31)
0x8259E7DC:  add          r6, r10, r9
0x8259E7E0:  lwz          r5, 0x48(r6)
0x8259E7E4:  cmpw         cr6, r11, r5
0x8259E7E8:  bgt          cr6, 0x8259e7f0
0x8259E7EC:  mr           r7, r25
0x8259E7F0:  lwz          r4, 0x2e8(r31)
0x8259E7F4:  addi         r8, r8, 1
0x8259E7F8:  addi         r10, r10, 0x17c
0x8259E7FC:  cmplw        cr6, r8, r4
0x8259E800:  blt          cr6, 0x8259e7b4
0x8259E804:  cmpwi        cr6, r7, 0
0x8259E808:  beq          cr6, 0x8259e8c4
0x8259E80C:  lis          r11, -0x7d8c
0x8259E810:  addi         r29, r31, 0x1f4
0x8259E814:  addi         r30, r11, -0x61f0
0x8259E818:  lwz          r11, 0x2ec(r31)
0x8259E81C:  addi         r28, r31, 0x340
0x8259E820:  addi         r11, r11, 1
0x8259E824:  mr           r6, r29
0x8259E828:  mr           r4, r28
0x8259E82C:  lwz          r5, -4(r30)
0x8259E830:  lwz          r3, 0(r30)
0x8259E834:  stw          r11, 0x2ec(r31)
0x8259E838:  bl           0x825a0010
0x8259E83C:  li           r4, 0
0x8259E840:  mr           r3, r31
0x8259E844:  bl           0x8259e010
0x8259E848:  lwz          r10, 0x138(r31)
0x8259E84C:  lwz          r9, 0x13c(r31)
0x8259E850:  mulli        r8, r10, 0x1d
0x8259E854:  srwi         r11, r8, 5
0x8259E858:  cmplw        cr6, r9, r11
0x8259E85C:  bge          cr6, 0x8259e894
0x8259E860:  addi         r27, r31, 0xf0
0x8259E864:  lwz          r7, 0x100(r31)
0x8259E868:  mr           r3, r27
0x8259E86C:  mtctr        r7
0x8259E870:  bctrl        
0x8259E874:  cmplwi       cr6, r3, 0
0x8259E878:  beq          cr6, 0x8259e894
0x8259E87C:  lwz          r6, 0x138(r31)
0x8259E880:  lwz          r5, 0x13c(r31)
0x8259E884:  mulli        r4, r6, 0x1d
0x8259E888:  srwi         r11, r4, 5
0x8259E88C:  cmplw        cr6, r5, r11
0x8259E890:  blt          cr6, 0x8259e864
0x8259E894:  li           r4, 1
0x8259E898:  stw          r26, 0x268(r31)
0x8259E89C:  mr           r3, r31
0x8259E8A0:  stw          r26, 0x328(r31)
0x8259E8A4:  stw          r26, 0x330(r31)
0x8259E8A8:  bl           0x8259e010
0x8259E8AC:  mr           r4, r28
0x8259E8B0:  lwz          r3, 0(r30)
0x8259E8B4:  bl           0x825a0020
0x8259E8B8:  mr           r4, r29
0x8259E8BC:  lwz          r3, -4(r30)
0x8259E8C0:  bl           0x825a0020
0x8259E8C4:  lwz          r3, 0x2e8(r31)
0x8259E8C8:  stw          r25, 0x11c(r31)
0x8259E8CC:  cmplwi       cr6, r3, 0
0x8259E8D0:  beq          cr6, 0x8259e8dc
0x8259E8D4:  mr           r3, r31
0x8259E8D8:  bl           0x8259c4f0
0x8259E8DC:  bl           0x825a03b8
0x8259E8E0:  lwz          r11, 0x264(r31)
0x8259E8E4:  cmplwi       cr6, r11, 0
0x8259E8E8:  beq          cr6, 0x8259e900
0x8259E8EC:  lwz          r10, 0x29c(r31)
0x8259E8F0:  stw          r26, 0x264(r31)
0x8259E8F4:  subf         r11, r11, r10
0x8259E8F8:  add          r9, r11, r3
0x8259E8FC:  stw          r9, 0x29c(r31)
0x8259E900:  lwz          r11, 0xc(r31)
0x8259E904:  mr           r4, r25
0x8259E908:  lwz          r8, 8(r31)
0x8259E90C:  cmplw        cr6, r11, r8
0x8259E910:  bge          cr6, 0x8259e918
0x8259E914:  addi         r4, r11, 1
0x8259E918:  mr           r3, r31
0x8259E91C:  bl           0x8259c860
0x8259E920:  stw          r26, 0x11c(r31)
0x8259E924:  addi         r1, r1, 0x90
0x8259E928:  b            0x8242f8dc
0x8259E92C:  .align 0                       # padding
0x8259E930:  mflr         r12
0x8259E934:  bl           0x8242f89c
0x8259E938:  stwu         r1, -0x70(r1)
0x8259E93C:  mr           r31, r3
0x8259E940:  cmplwi       cr6, r31, 0
0x8259E944:  beq          cr6, 0x8259eaa0
0x8259E948:  li           r4, 1
0x8259E94C:  bl           0x8259dd80
0x8259E950:  lwz          r11, 0x20(r31)
0x8259E954:  rlwinm       r10, r11, 0, 4, 4
0x8259E958:  lis          r11, -0x7d8c
0x8259E95C:  cmplwi       cr6, r10, 0
0x8259E960:  addi         r30, r11, -0x61f0
0x8259E964:  bne          cr6, 0x8259e97c
0x8259E968:  lwz          r9, 0xe8(r31)
0x8259E96C:  cmplwi       cr6, r9, 0
0x8259E970:  bne          cr6, 0x8259e97c
0x8259E974:  lwz          r3, -4(r30)
0x8259E978:  b            0x8259e980
0x8259E97C:  li           r3, 0
0x8259E980:  lwz          r8, 0x2e8(r31)
0x8259E984:  cmplwi       cr6, r8, 0
0x8259E988:  beq          cr6, 0x8259e994
0x8259E98C:  lwz          r5, 0(r30)
0x8259E990:  b            0x8259e998
0x8259E994:  li           r5, 0
0x8259E998:  li           r7, 1
0x8259E99C:  addi         r6, r31, 0x340
0x8259E9A0:  addi         r4, r31, 0x1f4
0x8259E9A4:  bl           0x825a01a0
0x8259E9A8:  cmpwi        cr6, r3, 1
0x8259E9AC:  beq          cr6, 0x8259e9d4
0x8259E9B0:  cmpwi        cr6, r3, 2
0x8259E9B4:  beq          cr6, 0x8259e9c8
0x8259E9B8:  cmpwi        cr6, r3, 3
0x8259E9BC:  bne          cr6, 0x8259e9dc
0x8259E9C0:  li           r11, 0
0x8259E9C4:  stw          r11, -4(r30)
0x8259E9C8:  li           r11, 0
0x8259E9CC:  stw          r11, 0(r30)
0x8259E9D0:  b            0x8259e9dc
0x8259E9D4:  li           r11, 0
0x8259E9D8:  stw          r11, -4(r30)
0x8259E9DC:  lwz          r7, 0x2e8(r31)
0x8259E9E0:  li           r29, 0
0x8259E9E4:  cmplwi       cr6, r7, 0
0x8259E9E8:  ble          cr6, 0x8259ea40
0x8259E9EC:  li           r30, 0
0x8259E9F0:  lwz          r11, 0x2f0(r31)
0x8259E9F4:  add          r3, r30, r11
0x8259E9F8:  lwz          r6, 0x1c(r3)
0x8259E9FC:  mtctr        r6
0x8259EA00:  bctrl        
0x8259EA04:  lwz          r11, 0x2f0(r31)
0x8259EA08:  add          r5, r30, r11
0x8259EA0C:  lwz          r3, 0x3c(r5)
0x8259EA10:  bl           0x825a1450
0x8259EA14:  lwz          r11, 0x2f0(r31)
0x8259EA18:  add          r4, r30, r11
0x8259EA1C:  lwz          r3, 0x2c(r4)
0x8259EA20:  cmplwi       cr6, r3, 0
0x8259EA24:  beq          cr6, 0x8259ea2c
0x8259EA28:  bl           0x8257b028
0x8259EA2C:  lwz          r3, 0x2e8(r31)
0x8259EA30:  addi         r29, r29, 1
0x8259EA34:  addi         r30, r30, 0x17c
0x8259EA38:  cmplw        cr6, r29, r3
0x8259EA3C:  blt          cr6, 0x8259e9f0
0x8259EA40:  lwz          r3, 0xe8(r31)
0x8259EA44:  cmplwi       cr6, r3, 0
0x8259EA48:  beq          cr6, 0x8259ea60
0x8259EA4C:  lwz          r11, 0x20(r31)
0x8259EA50:  rlwinm       r10, r11, 0, 5, 5
0x8259EA54:  cmplwi       cr6, r10, 0
0x8259EA58:  bne          cr6, 0x8259ea78
0x8259EA5C:  b            0x8259ea74
0x8259EA60:  lwz          r9, 0x104(r31)
0x8259EA64:  addi         r3, r31, 0xf0
0x8259EA68:  mtctr        r9
0x8259EA6C:  bctrl        
0x8259EA70:  lwz          r3, 0x234(r31)
0x8259EA74:  bl           0x8257b028
0x8259EA78:  lwz          r3, 0x384(r31)
0x8259EA7C:  cmplwi       cr6, r3, 0
0x8259EA80:  beq          cr6, 0x8259ea88
0x8259EA84:  bl           0x8257b028
0x8259EA88:  li           r5, 0x388
0x8259EA8C:  li           r4, 0
0x8259EA90:  mr           r3, r31
0x8259EA94:  bl           0x8242fed0
0x8259EA98:  mr           r3, r31
0x8259EA9C:  bl           0x8257b028
0x8259EAA0:  addi         r1, r1, 0x70
0x8259EAA4:  b            0x8242f8ec
0x8259EAA8:  mflr         r12
0x8259EAAC:  stw          r12, -8(r1)
0x8259EAB0:  std          r30, -0x18(r1)
0x8259EAB4:  std          r31, -0x10(r1)
0x8259EAB8:  stwu         r1, -0xe0(r1)
0x8259EABC:  mr           r31, r3
0x8259EAC0:  lwz          r11, 0x7c(r31)
0x8259EAC4:  cmplwi       cr6, r11, 0
0x8259EAC8:  beq          cr6, 0x8259ecbc
0x8259EACC:  lwz          r11, 0xe8(r31)
0x8259EAD0:  cmpwi        cr6, r11, 0x7fff
0x8259EAD4:  ble          cr6, 0x8259eadc
0x8259EAD8:  li           r11, 0x7fff
0x8259EADC:  extsw        r10, r11
0x8259EAE0:  lis          r11, -0x7dfe
0x8259EAE4:  std          r10, 0x60(r1)
0x8259EAE8:  lfd          f0, 0x60(r1)
0x8259EAEC:  fcfid        f13, f0
0x8259EAF0:  lfd          f2, 0x72d8(r11)
0x8259EAF4:  lis          r11, -0x7df8
0x8259EAF8:  lfs          f0, -0x6524(r11)
0x8259EAFC:  frsp         f12, f13
0x8259EB00:  fmuls        f1, f12, f0
0x8259EB04:  bl           0x82431308
0x8259EB08:  lwz          r3, 0x7c(r31)
0x8259EB0C:  frsp         f1, f1
0x8259EB10:  bl           0x82460e20
0x8259EB14:  lwz          r9, 0x70(r31)
0x8259EB18:  lwz          r8, 0xe4(r31)
0x8259EB1C:  std          r9, 0x68(r1)
0x8259EB20:  std          r8, 0x70(r1)
0x8259EB24:  lfd          f11, 0x68(r1)
0x8259EB28:  lfd          f10, 0x70(r1)
0x8259EB2C:  fcfid        f9, f11
0x8259EB30:  fcfid        f8, f10
0x8259EB34:  fdiv         f1, f8, f9
0x8259EB38:  bl           0x82432458
0x8259EB3C:  lis          r11, -0x7dfe
0x8259EB40:  lwz          r3, 0x7c(r31)
0x8259EB44:  lfd          f0, 0x72d0(r11)
0x8259EB48:  fmul         f7, f1, f0
0x8259EB4C:  frsp         f1, f7
0x8259EB50:  bl           0x82460e98
0x8259EB54:  li           r10, 1
0x8259EB58:  lwz          r4, 0x118(r31)
0x8259EB5C:  addi         r7, r1, 0x60
0x8259EB60:  li           r30, 0
0x8259EB64:  addi         r6, r1, 0x50
0x8259EB68:  addi         r5, r1, 0x80
0x8259EB6C:  stb          r10, 0x58(r1)
0x8259EB70:  cmpwi        cr6, r4, 0
0x8259EB74:  stw          r7, 0x5c(r1)
0x8259EB78:  stw          r30, 0x60(r1)
0x8259EB7C:  stw          r6, 0x64(r1)
0x8259EB80:  stw          r5, 0x54(r1)
0x8259EB84:  beq          cr6, 0x8259ec58
0x8259EB88:  lwz          r11, 0x120(r31)
0x8259EB8C:  stw          r30, 0x118(r31)
0x8259EB90:  cmplwi       cr6, r11, 0
0x8259EB94:  bne          cr6, 0x8259ec04
0x8259EB98:  lis          r11, -0x7e00
0x8259EB9C:  stb          r10, 0x89(r1)
0x8259EBA0:  li           r3, 6
0x8259EBA4:  stb          r30, 0x80(r1)
0x8259EBA8:  li           r10, 3
0x8259EBAC:  stb          r30, 0x81(r1)
0x8259EBB0:  li           r9, 4
0x8259EBB4:  stb          r30, 0x88(r1)
0x8259EBB8:  li           r8, 5
0x8259EBBC:  stb          r30, 0x90(r1)
0x8259EBC0:  lfs          f0, 0x3dac(r11)
0x8259EBC4:  li           r11, 2
0x8259EBC8:  stfs         f0, 0x84(r1)
0x8259EBCC:  stb          r3, 0x50(r1)
0x8259EBD0:  stfs         f0, 0x8c(r1)
0x8259EBD4:  stb          r30, 0x98(r1)
0x8259EBD8:  stfs         f0, 0x94(r1)
0x8259EBDC:  stb          r10, 0x99(r1)
0x8259EBE0:  stfs         f0, 0x9c(r1)
0x8259EBE4:  stb          r30, 0xa0(r1)
0x8259EBE8:  stfs         f0, 0xa4(r1)
0x8259EBEC:  stb          r11, 0x91(r1)
0x8259EBF0:  stfs         f0, 0xac(r1)
0x8259EBF4:  stb          r9, 0xa1(r1)
0x8259EBF8:  stb          r30, 0xa8(r1)
0x8259EBFC:  stb          r8, 0xa9(r1)
0x8259EC00:  b            0x8259ec4c
0x8259EC04:  clrlwi       r11, r11, 0x18
0x8259EC08:  mr           r10, r11
0x8259EC0C:  cmplwi       cr6, r10, 0
0x8259EC10:  stb          r11, 0x50(r1)
0x8259EC14:  beq          cr6, 0x8259ec4c
0x8259EC18:  lis          r8, -0x7e00
0x8259EC1C:  addi         r11, r1, 0x81
0x8259EC20:  addi         r9, r31, 0x128
0x8259EC24:  lfs          f0, 0x3dac(r8)
0x8259EC28:  lwz          r7, 0(r9)
0x8259EC2C:  addi         r10, r10, -1
0x8259EC30:  stfs         f0, 3(r11)
0x8259EC34:  stb          r30, -1(r11)
0x8259EC38:  addi         r9, r9, 4
0x8259EC3C:  cmplwi       cr6, r10, 0
0x8259EC40:  stb          r7, 0(r11)
0x8259EC44:  addi         r11, r11, 8
0x8259EC48:  bne          cr6, 0x8259ec28
0x8259EC4C:  addi         r4, r1, 0x58
0x8259EC50:  lwz          r3, 0x7c(r31)
0x8259EC54:  bl           0x824609f0
0x8259EC58:  lwz          r5, 0x11c(r31)
0x8259EC5C:  cmpwi        cr6, r5, 0
0x8259EC60:  beq          cr6, 0x8259ecbc
0x8259EC64:  lwz          r4, 0x124(r31)
0x8259EC68:  stw          r30, 0x11c(r31)
0x8259EC6C:  clrlwi       r11, r4, 0x18
0x8259EC70:  mr           r9, r11
0x8259EC74:  cmplwi       cr6, r9, 0
0x8259EC78:  stb          r11, 0x50(r1)
0x8259EC7C:  beq          cr6, 0x8259ecb0
0x8259EC80:  addi         r11, r1, 0x81
0x8259EC84:  addi         r10, r31, 0x168
0x8259EC88:  lwz          r3, -0x20(r10)
0x8259EC8C:  addi         r9, r9, -1
0x8259EC90:  lfs          f6, 0(r10)
0x8259EC94:  stb          r30, -1(r11)
0x8259EC98:  stfs         f6, 3(r11)
0x8259EC9C:  addi         r10, r10, 4
0x8259ECA0:  cmplwi       cr6, r9, 0
0x8259ECA4:  stb          r3, 0(r11)
0x8259ECA8:  addi         r11, r11, 8
0x8259ECAC:  bne          cr6, 0x8259ec88
0x8259ECB0:  addi         r4, r1, 0x58
0x8259ECB4:  lwz          r3, 0x7c(r31)
0x8259ECB8:  bl           0x824609f0
0x8259ECBC:  addi         r1, r1, 0xe0
0x8259ECC0:  lwz          r12, -8(r1)
0x8259ECC4:  mtlr         r12
0x8259ECC8:  ld           r30, -0x18(r1)
0x8259ECCC:  ld           r31, -0x10(r1)
0x8259ECD0:  blr          
0x8259ECD4:  .align 0                       # padding
0x8259ECD8:  mflr         r12
0x8259ECDC:  bl           0x8242f89c
0x8259ECE0:  stwu         r1, -0x70(r1)
0x8259ECE4:  mr           r31, r3
0x8259ECE8:  mr           r29, r5
0x8259ECEC:  mr           r11, r4
0x8259ECF0:  mr           r30, r29
0x8259ECF4:  add          r9, r11, r29
0x8259ECF8:  lwz          r10, 0x60(r31)
0x8259ECFC:  cmplw        cr6, r9, r10
0x8259ED00:  ble          cr6, 0x8259ed08
0x8259ED04:  subf         r30, r11, r10
0x8259ED08:  lwz          r10, 0x5c(r31)
0x8259ED0C:  mr           r5, r30
0x8259ED10:  lwz          r4, 0x98(r31)
0x8259ED14:  add          r3, r10, r11
0x8259ED18:  bl           0x8242fed0
0x8259ED1C:  cmplw        cr6, r30, r29
0x8259ED20:  bge          cr6, 0x8259ed34
0x8259ED24:  subf         r5, r30, r29
0x8259ED28:  lwz          r4, 0x98(r31)
0x8259ED2C:  lwz          r3, 0x5c(r31)
0x8259ED30:  bl           0x8242fed0
0x8259ED34:  addi         r1, r1, 0x70
0x8259ED38:  b            0x8242f8ec
0x8259ED3C:  .align 0                       # padding
0x8259ED40:  mflr         r12
0x8259ED44:  bl           0x8242f88c
0x8259ED48:  stwu         r1, -0xf0(r1)
0x8259ED4C:  mr           r31, r3
0x8259ED50:  lwz          r11, 0xf4(r31)
0x8259ED54:  cmpwi        cr6, r11, 0
0x8259ED58:  bne          cr6, 0x8259ef00
0x8259ED5C:  lwz          r10, 0x64(r31)
0x8259ED60:  cmpwi        cr6, r10, 0
0x8259ED64:  beq          cr6, 0x8259ef00
0x8259ED68:  lwz          r9, 0xf0(r31)
0x8259ED6C:  cmpwi        cr6, r9, 0
0x8259ED70:  bne          cr6, 0x8259ed80
0x8259ED74:  li           r3, 1
0x8259ED78:  addi         r1, r1, 0xf0
0x8259ED7C:  b            0x8242f8dc
0x8259ED80:  lwz          r27, 0x104(r31)
0x8259ED84:  bl           0x825a03b8
0x8259ED88:  lwz          r8, 0x10c(r31)
0x8259ED8C:  li           r26, 1
0x8259ED90:  li           r25, 0
0x8259ED94:  subf         r7, r8, r3
0x8259ED98:  cmplwi       cr6, r7, 0xc
0x8259ED9C:  ble          cr6, 0x8259ee08
0x8259EDA0:  lwz          r30, 0x7c(r31)
0x8259EDA4:  addi         r4, r1, 0x80
0x8259EDA8:  stw          r25, 0x50(r1)
0x8259EDAC:  mr           r3, r30
0x8259EDB0:  stw          r25, 0x80(r1)
0x8259EDB4:  bl           0x82460d18
0x8259EDB8:  addi         r4, r1, 0x50
0x8259EDBC:  mr           r3, r30
0x8259EDC0:  bl           0x82460d18
0x8259EDC4:  addi         r4, r1, 0x50
0x8259EDC8:  mr           r3, r30
0x8259EDCC:  bl           0x82460d18
0x8259EDD0:  lwz          r11, 0x50(r1)
0x8259EDD4:  lwz          r6, 0x80(r1)
0x8259EDD8:  subf         r5, r6, r11
0x8259EDDC:  cmplwi       cr6, r5, 0x400
0x8259EDE0:  bge          cr6, 0x8259eda4
0x8259EDE4:  mr           r27, r11
0x8259EDE8:  bl           0x825a03b8
0x8259EDEC:  lwz          r4, 0x10c(r31)
0x8259EDF0:  lwz          r11, 0x110(r31)
0x8259EDF4:  subf         r10, r4, r3
0x8259EDF8:  stw          r27, 0x104(r31)
0x8259EDFC:  stw          r3, 0x10c(r31)
0x8259EE00:  cmplw        cr6, r10, r11
0x8259EE04:  bge          cr6, 0x8259ee6c
0x8259EE08:  lwz          r11, 0xf8(r31)
0x8259EE0C:  cmplw        cr6, r27, r11
0x8259EE10:  bge          cr6, 0x8259ee24
0x8259EE14:  lwz          r9, 0xdc(r31)
0x8259EE18:  subf         r11, r11, r9
0x8259EE1C:  add          r11, r11, r27
0x8259EE20:  b            0x8259ee28
0x8259EE24:  subf         r11, r11, r27
0x8259EE28:  cmplwi       cr6, r11, 0x400
0x8259EE2C:  bge          cr6, 0x8259ee38
0x8259EE30:  mr           r11, r25
0x8259EE34:  b            0x8259ee40
0x8259EE38:  addi         r8, r11, -0x400
0x8259EE3C:  rlwinm       r11, r8, 0, 0, 0x1d
0x8259EE40:  lwz          r7, 0xe0(r31)
0x8259EE44:  cmplw        cr6, r11, r7
0x8259EE48:  bge          cr6, 0x8259ee60
0x8259EE4C:  lwz          r6, 0xfc(r31)
0x8259EE50:  cmplwi       cr6, r6, 0
0x8259EE54:  beq          cr6, 0x8259ef00
0x8259EE58:  cmplwi       cr6, r11, 0
0x8259EE5C:  beq          cr6, 0x8259ef00
0x8259EE60:  lwz          r5, 0x108(r31)
0x8259EE64:  cmplw        cr6, r11, r5
0x8259EE68:  blt          cr6, 0x8259eef0
0x8259EE6C:  lwz          r4, 0xe0(r31)
0x8259EE70:  addi         r30, r31, 0x7c
0x8259EE74:  lwz          r11, 0x104(r31)
0x8259EE78:  slwi         r29, r4, 1
0x8259EE7C:  stw          r26, 0x5c(r31)
0x8259EE80:  add          r3, r11, r29
0x8259EE84:  lwz          r10, 0x60(r30)
0x8259EE88:  mr           r28, r29
0x8259EE8C:  cmplw        cr6, r3, r10
0x8259EE90:  ble          cr6, 0x8259ee98
0x8259EE94:  subf         r28, r11, r10
0x8259EE98:  lwz          r10, 0x5c(r30)
0x8259EE9C:  mr           r5, r28
0x8259EEA0:  lwz          r4, 0x98(r30)
0x8259EEA4:  add          r3, r10, r11
0x8259EEA8:  bl           0x8242fed0
0x8259EEAC:  cmplw        cr6, r28, r29
0x8259EEB0:  bge          cr6, 0x8259eec4
0x8259EEB4:  subf         r5, r28, r29
0x8259EEB8:  lwz          r4, 0x98(r30)
0x8259EEBC:  lwz          r3, 0x5c(r30)
0x8259EEC0:  bl           0x8242fed0
0x8259EEC4:  lwz          r11, 0xe0(r31)
0x8259EEC8:  lwz          r9, 0x104(r31)
0x8259EECC:  slwi         r11, r11, 1
0x8259EED0:  lwz          r10, 0xdc(r31)
0x8259EED4:  add          r11, r11, r9
0x8259EED8:  cmplw        cr6, r11, r10
0x8259EEDC:  stw          r11, 0xf8(r31)
0x8259EEE0:  blt          cr6, 0x8259ee08
0x8259EEE4:  subf         r10, r10, r11
0x8259EEE8:  stw          r10, 0xf8(r31)
0x8259EEEC:  b            0x8259ee08
0x8259EEF0:  li           r3, 1
0x8259EEF4:  stw          r11, 0xfc(r31)
0x8259EEF8:  addi         r1, r1, 0xf0
0x8259EEFC:  b            0x8242f8dc
0x8259EF00:  li           r3, 0
0x8259EF04:  addi         r1, r1, 0xf0
0x8259EF08:  b            0x8242f8dc
0x8259EF0C:  .align 0                       # padding
0x8259EF10:  lwz          r11, 0xfc(r3)
0x8259EF14:  lwz          r10, 0xf8(r3)
0x8259EF18:  lwz          r9, 0xdc(r3)
0x8259EF1C:  add          r8, r10, r11
0x8259EF20:  cmplw        cr6, r8, r9
0x8259EF24:  ble          cr6, 0x8259ef2c
0x8259EF28:  subf         r11, r10, r9
0x8259EF2C:  lwz          r9, 0xd8(r3)
0x8259EF30:  li           r3, 1
0x8259EF34:  add          r7, r9, r10
0x8259EF38:  stw          r7, 0(r4)
0x8259EF3C:  stw          r11, 0(r5)
0x8259EF40:  blr          
0x8259EF44:  .align 0                       # padding
0x8259EF48:  mflr         r12
0x8259EF4C:  bl           0x8242f894
0x8259EF50:  stwu         r1, -0xb0(r1)
0x8259EF54:  mr           r31, r3
0x8259EF58:  li           r11, 1
0x8259EF5C:  li           r30, 0
0x8259EF60:  lwz          r3, 0(r31)
0x8259EF64:  stw          r11, 0x74(r31)
0x8259EF68:  stw          r30, 0x50(r1)
0x8259EF6C:  bl           0x82460bb0
0x8259EF70:  lwz          r11, 0x60(r31)
0x8259EF74:  addi         r4, r31, 4
0x8259EF78:  lwz          r10, 0x5c(r31)
0x8259EF7C:  li           r29, -1
0x8259EF80:  li           r5, 1
0x8259EF84:  lwz          r3, 0(r31)
0x8259EF88:  stw          r30, 0x10(r31)
0x8259EF8C:  stw          r11, 8(r31)
0x8259EF90:  stw          r29, 0xc(r31)
0x8259EF94:  stw          r10, 0(r4)
0x8259EF98:  stw          r11, 0x14(r31)
0x8259EF9C:  bl           0x82460b50
0x8259EFA0:  mr           r4, r29
0x8259EFA4:  lwz          r3, 0(r31)
0x8259EFA8:  bl           0x82460dc8
0x8259EFAC:  addi         r4, r1, 0x50
0x8259EFB0:  lwz          r3, 0(r31)
0x8259EFB4:  bl           0x82460d70
0x8259EFB8:  li           r4, 0
0x8259EFBC:  lwz          r3, 0(r31)
0x8259EFC0:  bl           0x82460c08
0x8259EFC4:  stw          r30, 0x78(r31)
0x8259EFC8:  bl           0x825a03b8
0x8259EFCC:  addi         r4, r1, 0x50
0x8259EFD0:  mr           r27, r3
0x8259EFD4:  lwz          r3, 0(r31)
0x8259EFD8:  bl           0x82460d18
0x8259EFDC:  lwz          r29, 0x50(r1)
0x8259EFE0:  mr           r28, r30
0x8259EFE4:  addi         r4, r1, 0x50
0x8259EFE8:  lwz          r3, 0(r31)
0x8259EFEC:  stw          r30, 0x50(r1)
0x8259EFF0:  bl           0x82460d18
0x8259EFF4:  lwz          r11, 0x50(r1)
0x8259EFF8:  cmplw        cr6, r11, r29
0x8259EFFC:  stw          r11, 0x88(r31)
0x8259F000:  beq          cr6, 0x8259f00c
0x8259F004:  li           r28, 1
0x8259F008:  b            0x8259f054
0x8259F00C:  cmplwi       cr6, r28, 0
0x8259F010:  bne          cr6, 0x8259f054
0x8259F014:  bl           0x825a03b8
0x8259F018:  subf         r9, r27, r3
0x8259F01C:  cmplwi       cr6, r9, 0x32
0x8259F020:  ble          cr6, 0x8259f054
0x8259F024:  li           r4, 0
0x8259F028:  lwz          r3, 0(r31)
0x8259F02C:  bl           0x82460c08
0x8259F030:  stw          r30, 0x78(r31)
0x8259F034:  bl           0x825a03b8
0x8259F038:  addi         r4, r1, 0x50
0x8259F03C:  mr           r27, r3
0x8259F040:  lwz          r3, 0(r31)
0x8259F044:  stw          r30, 0x50(r1)
0x8259F048:  bl           0x82460d18
0x8259F04C:  lwz          r29, 0x50(r1)
0x8259F050:  mr           r28, r30
0x8259F054:  lwz          r8, 0x88(r31)
0x8259F058:  lwz          r7, 0x64(r31)
0x8259F05C:  cmplw        cr6, r8, r7
0x8259F060:  bgt          cr6, 0x8259efe4
0x8259F064:  bl           0x825a03b8
0x8259F068:  stw          r3, 0x90(r31)
0x8259F06C:  addi         r1, r1, 0xb0
0x8259F070:  b            0x8242f8e4
0x8259F074:  .align 0                       # padding
0x8259F078:  mflr         r12
0x8259F07C:  stw          r12, -8(r1)
0x8259F080:  stwu         r1, -0x60(r1)
0x8259F084:  lwz          r11, 0xf8(r3)
0x8259F088:  lwz          r10, 0xdc(r3)
0x8259F08C:  add          r11, r4, r11
0x8259F090:  cmplw        cr6, r11, r10
0x8259F094:  stw          r11, 0xf8(r3)
0x8259F098:  blt          cr6, 0x8259f0a4
0x8259F09C:  subf         r11, r10, r11
0x8259F0A0:  stw          r11, 0xf8(r3)
0x8259F0A4:  lwz          r9, 0xfc(r3)
0x8259F0A8:  lwz          r10, 0xf0(r3)
0x8259F0AC:  subf         r11, r4, r9
0x8259F0B0:  cmpwi        cr6, r10, 0
0x8259F0B4:  stw          r11, 0xfc(r3)
0x8259F0B8:  bne          cr6, 0x8259f0d8
0x8259F0BC:  cmplwi       cr6, r11, 0
0x8259F0C0:  bne          cr6, 0x8259f0d8
0x8259F0C4:  lwz          r8, 0xf4(r3)
0x8259F0C8:  cmpwi        cr6, r8, 0
0x8259F0CC:  bne          cr6, 0x8259f0d8
0x8259F0D0:  addi         r3, r3, 0x7c
0x8259F0D4:  bl           0x8259ef48
0x8259F0D8:  li           r3, 1
0x8259F0DC:  addi         r1, r1, 0x60
0x8259F0E0:  lwz          r12, -8(r1)
0x8259F0E4:  mtlr         r12
0x8259F0E8:  blr          
0x8259F0EC:  .align 0                       # padding
0x8259F0F0:  cmpwi        cr6, r4, 0x7fff
0x8259F0F4:  ble          cr6, 0x8259f0fc
0x8259F0F8:  li           r4, 0x7fff
0x8259F0FC:  stw          r4, 0xe8(r3)
0x8259F100:  b            0x8259eaa8
0x8259F104:  .align 0                       # padding
0x8259F108:  lis          r11, 1
0x8259F10C:  cmpw         cr6, r4, r11
0x8259F110:  ble          cr6, 0x8259f118
0x8259F114:  lis          r4, 1
0x8259F118:  stw          r4, 0xec(r3)
0x8259F11C:  b            0x8259eaa8
0x8259F120:  mflr         r12
0x8259F124:  stw          r12, -8(r1)
0x8259F128:  std          r31, -0x10(r1)
0x8259F12C:  stwu         r1, -0x60(r1)
0x8259F130:  mr           r11, r5
0x8259F134:  mr           r31, r3
0x8259F138:  cmplwi       cr6, r11, 8
0x8259F13C:  ble          cr6, 0x8259f144
0x8259F140:  li           r11, 8
0x8259F144:  li           r10, 1
0x8259F148:  stw          r11, 0x120(r31)
0x8259F14C:  slwi         r5, r11, 2
0x8259F150:  addi         r3, r31, 0x128
0x8259F154:  stw          r10, 0x118(r31)
0x8259F158:  bl           0x82434100
0x8259F15C:  mr           r3, r31
0x8259F160:  bl           0x8259eaa8
0x8259F164:  addi         r1, r1, 0x60
0x8259F168:  lwz          r12, -8(r1)
0x8259F16C:  mtlr         r12
0x8259F170:  ld           r31, -0x10(r1)
0x8259F174:  blr          
0x8259F178:  mflr         r12
0x8259F17C:  bl           0x8242f89c
0x8259F180:  stwu         r1, -0x90(r1)
0x8259F184:  mr           r31, r6
0x8259F188:  mr           r30, r3
0x8259F18C:  mr           r29, r5
0x8259F190:  cmplwi       cr6, r31, 8
0x8259F194:  ble          cr6, 0x8259f19c
0x8259F198:  li           r31, 8
0x8259F19C:  li           r11, 1
0x8259F1A0:  stw          r31, 0x124(r30)
0x8259F1A4:  slwi         r5, r31, 2
0x8259F1A8:  addi         r3, r30, 0x148
0x8259F1AC:  stw          r11, 0x11c(r30)
0x8259F1B0:  bl           0x82434100
0x8259F1B4:  lis          r11, -0x7df8
0x8259F1B8:  li           r7, 0
0x8259F1BC:  cmpwi        cr6, r31, 4
0x8259F1C0:  lfs          f0, -0x6524(r11)
0x8259F1C4:  blt          cr6, 0x8259f2a4
0x8259F1C8:  addi         r10, r31, -4
0x8259F1CC:  addi         r11, r29, 8
0x8259F1D0:  srwi         r9, r10, 2
0x8259F1D4:  addi         r10, r30, 0x16c
0x8259F1D8:  addi         r8, r9, 1
0x8259F1DC:  slwi         r7, r8, 2
0x8259F1E0:  lwz          r9, -8(r11)
0x8259F1E4:  cmpwi        cr6, r9, 0x7fff
0x8259F1E8:  ble          cr6, 0x8259f1f0
0x8259F1EC:  li           r9, 0x7fff
0x8259F1F0:  extsw        r9, r9
0x8259F1F4:  std          r9, 0x50(r1)
0x8259F1F8:  lfd          f13, 0x50(r1)
0x8259F1FC:  fcfid        f12, f13
0x8259F200:  frsp         f11, f12
0x8259F204:  fmuls        f10, f11, f0
0x8259F208:  stfs         f10, -4(r10)
0x8259F20C:  lwz          r9, -4(r11)
0x8259F210:  cmpwi        cr6, r9, 0x7fff
0x8259F214:  ble          cr6, 0x8259f21c
0x8259F218:  li           r9, 0x7fff
0x8259F21C:  extsw        r6, r9
0x8259F220:  std          r6, 0x58(r1)
0x8259F224:  lfd          f9, 0x58(r1)
0x8259F228:  fcfid        f8, f9
0x8259F22C:  frsp         f7, f8
0x8259F230:  fmuls        f6, f7, f0
0x8259F234:  stfs         f6, 0(r10)
0x8259F238:  lwz          r9, 0(r11)
0x8259F23C:  cmpwi        cr6, r9, 0x7fff
0x8259F240:  ble          cr6, 0x8259f248
0x8259F244:  li           r9, 0x7fff
0x8259F248:  extsw        r5, r9
0x8259F24C:  std          r5, 0x60(r1)
0x8259F250:  lfd          f5, 0x60(r1)
0x8259F254:  fcfid        f4, f5
0x8259F258:  frsp         f3, f4
0x8259F25C:  fmuls        f2, f3, f0
0x8259F260:  stfs         f2, 4(r10)
0x8259F264:  lwz          r9, 4(r11)
0x8259F268:  cmpwi        cr6, r9, 0x7fff
0x8259F26C:  ble          cr6, 0x8259f274
0x8259F270:  li           r9, 0x7fff
0x8259F274:  extsw        r4, r9
0x8259F278:  addi         r8, r8, -1
0x8259F27C:  addi         r11, r11, 0x10
0x8259F280:  cmplwi       cr6, r8, 0
0x8259F284:  std          r4, 0x68(r1)
0x8259F288:  lfd          f1, 0x68(r1)
0x8259F28C:  fcfid        f13, f1
0x8259F290:  frsp         f12, f13
0x8259F294:  fmuls        f11, f12, f0
0x8259F298:  stfs         f11, 8(r10)
0x8259F29C:  addi         r10, r10, 0x10
0x8259F2A0:  bne          cr6, 0x8259f1e0
0x8259F2A4:  cmplw        cr6, r7, r31
0x8259F2A8:  bge          cr6, 0x8259f304
0x8259F2AC:  addi         r3, r7, 0x5a
0x8259F2B0:  slwi         r10, r7, 2
0x8259F2B4:  slwi         r11, r3, 2
0x8259F2B8:  add          r10, r10, r29
0x8259F2BC:  add          r9, r11, r30
0x8259F2C0:  subf         r11, r7, r31
0x8259F2C4:  lwz          r8, 0(r10)
0x8259F2C8:  cmpwi        cr6, r8, 0x7fff
0x8259F2CC:  ble          cr6, 0x8259f2d4
0x8259F2D0:  li           r8, 0x7fff
0x8259F2D4:  extsw        r8, r8
0x8259F2D8:  addi         r11, r11, -1
0x8259F2DC:  addi         r10, r10, 4
0x8259F2E0:  cmplwi       cr6, r11, 0
0x8259F2E4:  std          r8, 0x68(r1)
0x8259F2E8:  lfd          f10, 0x68(r1)
0x8259F2EC:  fcfid        f9, f10
0x8259F2F0:  frsp         f8, f9
0x8259F2F4:  fmuls        f7, f8, f0
0x8259F2F8:  stfs         f7, 0(r9)
0x8259F2FC:  addi         r9, r9, 4
0x8259F300:  bne          cr6, 0x8259f2c4
0x8259F304:  mr           r3, r30
0x8259F308:  bl           0x8259eaa8
0x8259F30C:  addi         r1, r1, 0x90
0x8259F310:  b            0x8242f8ec
0x8259F314:  .align 0                       # padding
0x8259F318:  mflr         r12
0x8259F31C:  bl           0x8242f89c
0x8259F320:  stwu         r1, -0xb0(r1)
0x8259F324:  mr           r31, r3
0x8259F328:  cmpwi        cr6, r4, 0
0x8259F32C:  beq          cr6, 0x8259f364
0x8259F330:  lwz          r11, 0x64(r31)
0x8259F334:  cmpwi        cr6, r11, 0
0x8259F338:  bne          cr6, 0x8259f3cc
0x8259F33C:  lwz          r9, 0x100(r31)
0x8259F340:  li           r10, 1
0x8259F344:  li           r29, 0
0x8259F348:  slwi         r3, r10, 0
0x8259F34C:  stw          r10, 0x64(r31)
0x8259F350:  stw          r29, 0xf0(r31)
0x8259F354:  stw          r29, 0xf8(r31)
0x8259F358:  stw          r9, 0xfc(r31)
0x8259F35C:  addi         r1, r1, 0xb0
0x8259F360:  b            0x8242f8ec
0x8259F364:  lwz          r8, 0x64(r31)
0x8259F368:  cmpwi        cr6, r8, 0
0x8259F36C:  beq          cr6, 0x8259f3cc
0x8259F370:  addi         r30, r31, 0x7c
0x8259F374:  li           r29, 0
0x8259F378:  li           r4, 1
0x8259F37C:  lwz          r3, 0(r30)
0x8259F380:  stw          r29, 0x60(r1)
0x8259F384:  bl           0x82460c58
0x8259F388:  cmpwi        cr6, r3, 0
0x8259F38C:  blt          cr6, 0x8259f3ac
0x8259F390:  addi         r4, r1, 0x50
0x8259F394:  lwz          r3, 0(r30)
0x8259F398:  stb          r29, 0x50(r1)
0x8259F39C:  bl           0x82460aa0
0x8259F3A0:  lbz          r6, 0x50(r1)
0x8259F3A4:  cmplwi       cr6, r6, 1
0x8259F3A8:  beq          cr6, 0x8259f390
0x8259F3AC:  addi         r4, r1, 0x60
0x8259F3B0:  lwz          r3, 0(r30)
0x8259F3B4:  bl           0x82460d70
0x8259F3B8:  li           r4, 0
0x8259F3BC:  mr           r3, r30
0x8259F3C0:  lwz          r5, 0xdc(r31)
0x8259F3C4:  bl           0x8259ecd8
0x8259F3C8:  stw          r29, 0x64(r31)
0x8259F3CC:  lwz          r3, 0x64(r31)
0x8259F3D0:  addi         r1, r1, 0xb0
0x8259F3D4:  b            0x8242f8ec
0x8259F3D8:  mflr         r12
0x8259F3DC:  stw          r12, -8(r1)
0x8259F3E0:  std          r30, -0x18(r1)
0x8259F3E4:  std          r31, -0x10(r1)
0x8259F3E8:  stwu         r1, -0x70(r1)
0x8259F3EC:  mr           r31, r3
0x8259F3F0:  cmpwi        cr6, r4, 0
0x8259F3F4:  beq          cr6, 0x8259f418
0x8259F3F8:  lwz          r11, 0xf0(r31)
0x8259F3FC:  li           r30, 1
0x8259F400:  cmpwi        cr6, r11, 0
0x8259F404:  beq          cr6, 0x8259f450
0x8259F408:  li           r4, 1
0x8259F40C:  lwz          r3, 0x7c(r31)
0x8259F410:  bl           0x82460c58
0x8259F414:  b            0x8259f44c
0x8259F418:  lwz          r10, 0xf0(r31)
0x8259F41C:  li           r30, 0
0x8259F420:  cmpwi        cr6, r10, 0
0x8259F424:  bne          cr6, 0x8259f440
0x8259F428:  lwz          r9, 0xfc(r31)
0x8259F42C:  cmplwi       cr6, r9, 0
0x8259F430:  bne          cr6, 0x8259f450
0x8259F434:  addi         r3, r31, 0x7c
0x8259F438:  bl           0x8259ef48
0x8259F43C:  b            0x8259f450
0x8259F440:  li           r4, 0
0x8259F444:  lwz          r3, 0x7c(r31)
0x8259F448:  bl           0x82460c08
0x8259F44C:  stw          r30, 0xf4(r31)
0x8259F450:  stw          r30, 0xf4(r31)
0x8259F454:  slwi         r3, r30, 0
0x8259F458:  addi         r1, r1, 0x70
0x8259F45C:  lwz          r12, -8(r1)
0x8259F460:  mtlr         r12
0x8259F464:  ld           r30, -0x18(r1)
0x8259F468:  ld           r31, -0x10(r1)
0x8259F46C:  blr          
0x8259F470:  mflr         r12
0x8259F474:  stw          r12, -8(r1)
0x8259F478:  std          r31, -0x10(r1)
0x8259F47C:  stwu         r1, -0x60(r1)
0x8259F480:  mr           r31, r3
0x8259F484:  lwz          r11, 0x7c(r31)
0x8259F488:  cmplwi       cr6, r11, 0
0x8259F48C:  beq          cr6, 0x8259f4cc
0x8259F490:  li           r4, 1
0x8259F494:  slwi         r3, r11, 0
0x8259F498:  bl           0x82460c58
0x8259F49C:  li           r10, 1
0x8259F4A0:  stw          r10, 0xf4(r31)
0x8259F4A4:  lwz          r3, 0x7c(r31)
0x8259F4A8:  bl           0x824608b8
0x8259F4AC:  li           r9, 0
0x8259F4B0:  lwz          r3, 0xd8(r31)
0x8259F4B4:  stw          r9, 0x7c(r31)
0x8259F4B8:  bl           0x8257b028
0x8259F4BC:  lis          r11, -0x7d8c
0x8259F4C0:  lwz          r10, -0x27b8(r11)
0x8259F4C4:  addi         r10, r10, -1
0x8259F4C8:  stw          r10, -0x27b8(r11)
0x8259F4CC:  addi         r1, r1, 0x60
0x8259F4D0:  lwz          r12, -8(r1)
0x8259F4D4:  mtlr         r12
0x8259F4D8:  ld           r31, -0x10(r1)
0x8259F4DC:  blr          
0x8259F4E0:  mflr         r12
0x8259F4E4:  bl           0x8242f888
0x8259F4E8:  stwu         r1, -0xf0(r1)
0x8259F4EC:  mr           r30, r4
0x8259F4F0:  mr           r25, r5
0x8259F4F4:  li           r5, 0x17c
0x8259F4F8:  li           r4, 0
0x8259F4FC:  mr           r31, r3
0x8259F500:  mr           r29, r6
0x8259F504:  mr           r28, r8
0x8259F508:  bl           0x8242fed0
0x8259F50C:  li           r27, 0
0x8259F510:  stw          r30, 0x70(r31)
0x8259F514:  addi         r11, r1, 0x54
0x8259F518:  stw          r25, 0x74(r31)
0x8259F51C:  mr           r9, r27
0x8259F520:  stw          r29, 0x78(r31)
0x8259F524:  li           r10, 0x15
0x8259F528:  stw          r27, 0x5c(r31)
0x8259F52C:  mtctr        r10
0x8259F530:  stw          r9, 0(r11)
0x8259F534:  addi         r11, r11, 4
0x8259F538:  bdnz         0x8259f530
0x8259F53C:  li           r24, 1
0x8259F540:  cmpwi        cr6, r25, 0x10
0x8259F544:  bne          cr6, 0x8259f550
0x8259F548:  stb          r24, 0x50(r1)
0x8259F54C:  b            0x8259f558
0x8259F550:  li           r11, 2
0x8259F554:  stb          r11, 0x50(r1)
0x8259F558:  addi         r26, r31, 0x7c
0x8259F55C:  stb          r29, 0x54(r1)
0x8259F560:  addi         r3, r1, 0x50
0x8259F564:  stw          r30, 0x58(r1)
0x8259F568:  mr           r4, r26
0x8259F56C:  stb          r24, 0x8b(r1)
0x8259F570:  bl           0x82461278
0x8259F574:  cmpwi        cr6, r3, 0
0x8259F578:  bge          cr6, 0x8259f588
0x8259F57C:  li           r3, 0
0x8259F580:  addi         r1, r1, 0xf0
0x8259F584:  b            0x8242f8d8
0x8259F588:  lis          r8, 0x1062
0x8259F58C:  lwz          r9, 0x74(r31)
0x8259F590:  lwz          r6, 0x78(r31)
0x8259F594:  lis          r10, 0x51eb
0x8259F598:  ori          r11, r8, 0x4dd3
0x8259F59C:  lwz          r3, 0x70(r31)
0x8259F5A0:  srawi        r4, r9, 3
0x8259F5A4:  stw          r30, 0xe4(r31)
0x8259F5A8:  mr           r7, r11
0x8259F5AC:  stw          r27, 0x68(r31)
0x8259F5B0:  mr           r5, r11
0x8259F5B4:  mullw        r11, r4, r6
0x8259F5B8:  mullw        r11, r11, r3
0x8259F5BC:  mulli        r4, r11, 0x258
0x8259F5C0:  mulhwu       r3, r4, r7
0x8259F5C4:  li           r6, 0x21c
0x8259F5C8:  ori          r8, r10, 0x851f
0x8259F5CC:  srwi         r10, r3, 6
0x8259F5D0:  mulli        r9, r11, 0x32
0x8259F5D4:  stw          r6, 0x110(r31)
0x8259F5D8:  addi         r7, r10, 0x3ff
0x8259F5DC:  mulhwu       r6, r9, r5
0x8259F5E0:  rlwinm       r3, r7, 0, 0, 0x15
0x8259F5E4:  srwi         r10, r6, 6
0x8259F5E8:  mulli        r4, r3, 0x5a
0x8259F5EC:  stw          r3, 0xdc(r31)
0x8259F5F0:  addi         r5, r10, 0xff
0x8259F5F4:  mulhwu       r10, r4, r8
0x8259F5F8:  srwi         r8, r10, 5
0x8259F5FC:  rlwinm       r9, r5, 0, 0, 0x17
0x8259F600:  stw          r8, 0x108(r31)
0x8259F604:  stw          r9, 0xe0(r31)
0x8259F608:  lwz          r10, 0x14(r28)
0x8259F60C:  cmplwi       cr6, r10, 0
0x8259F610:  beq          cr6, 0x8259f644
0x8259F614:  lwz          r8, 0x18(r28)
0x8259F618:  cmplwi       cr6, r8, 0
0x8259F61C:  beq          cr6, 0x8259f644
0x8259F620:  lwz          r7, 8(r28)
0x8259F624:  clrldi       r6, r8, 0x20
0x8259F628:  clrldi       r5, r11, 0x20
0x8259F62C:  mulld        r4, r7, r6
0x8259F630:  mulld        r11, r4, r5
0x8259F634:  clrldi       r10, r10, 0x20
0x8259F638:  divdu        r9, r11, r10
0x8259F63C:  slwi         r28, r9, 0
0x8259F640:  b            0x8259f648
0x8259F644:  mr           r28, r9
0x8259F648:  bl           0x8257af60
0x8259F64C:  lwz          r11, 0xe0(r31)
0x8259F650:  lwz          r8, 0x74(r31)
0x8259F654:  stw          r3, 0xd8(r31)
0x8259F658:  cmpwi        cr6, r8, 8
0x8259F65C:  stw          r11, 0x54(r31)
0x8259F660:  bne          cr6, 0x8259f66c
0x8259F664:  slwi         r7, r11, 1
0x8259F668:  stw          r7, 0x54(r31)
0x8259F66C:  li           r11, 0x7fff
0x8259F670:  stw          r27, 0x120(r31)
0x8259F674:  cmpwi        cr6, r25, 0x10
0x8259F678:  stw          r27, 0x124(r31)
0x8259F67C:  stw          r11, 0xe8(r31)
0x8259F680:  stw          r11, 0xec(r31)
0x8259F684:  bne          cr6, 0x8259f690
0x8259F688:  mr           r11, r27
0x8259F68C:  b            0x8259f698
0x8259F690:  lis          r11, -0x7f80
0x8259F694:  ori          r11, r11, 0x8080
0x8259F698:  mr           r3, r31
0x8259F69C:  stw          r11, 0x114(r31)
0x8259F6A0:  bl           0x8259eaa8
0x8259F6A4:  lwz          r30, 0xdc(r31)
0x8259F6A8:  lwz          r11, 0x60(r26)
0x8259F6AC:  mr           r29, r30
0x8259F6B0:  cmplw        cr6, r30, r11
0x8259F6B4:  ble          cr6, 0x8259f6bc
0x8259F6B8:  mr           r29, r11
0x8259F6BC:  mr           r5, r29
0x8259F6C0:  lwz          r4, 0x98(r26)
0x8259F6C4:  lwz          r3, 0x5c(r26)
0x8259F6C8:  bl           0x8242fed0
0x8259F6CC:  cmplw        cr6, r29, r30
0x8259F6D0:  bge          cr6, 0x8259f6e4
0x8259F6D4:  subf         r5, r29, r30
0x8259F6D8:  lwz          r4, 0x98(r26)
0x8259F6DC:  lwz          r3, 0x5c(r26)
0x8259F6E0:  bl           0x8242fed0
0x8259F6E4:  lwz          r11, 0xdc(r31)
0x8259F6E8:  stw          r27, 0xf8(r31)
0x8259F6EC:  stw          r27, 0xf0(r31)
0x8259F6F0:  cmplw        cr6, r28, r11
0x8259F6F4:  bge          cr6, 0x8259f6fc
0x8259F6F8:  mr           r11, r28
0x8259F6FC:  lis          r4, -0x7da6
0x8259F700:  stw          r11, 0x100(r31)
0x8259F704:  stw          r11, 0xfc(r31)
0x8259F708:  lis          r6, -0x7da6
0x8259F70C:  addi         r11, r4, -0xf10
0x8259F710:  stw          r27, 0xf4(r31)
0x8259F714:  lis          r8, -0x7da6
0x8259F718:  stw          r24, 0x64(r31)
0x8259F71C:  lis          r10, -0x7da6
0x8259F720:  addi         r4, r6, -0xc28
0x8259F724:  addi         r6, r8, -0xb90
0x8259F728:  stw          r11, 0xc(r31)
0x8259F72C:  lis          r11, -0x7d8c
0x8259F730:  addi         r8, r10, -0xe88
0x8259F734:  lis          r3, -0x7da6
0x8259F738:  lis          r29, -0x7da6
0x8259F73C:  stw          r4, 0x14(r31)
0x8259F740:  lis          r30, -0x7da6
0x8259F744:  stw          r6, 0x1c(r31)
0x8259F748:  lwz          r10, -0x27b8(r11)
0x8259F74C:  lis          r5, -0x7da6
0x8259F750:  lis          r7, -0x7da6
0x8259F754:  stw          r8, 0x24(r31)
0x8259F758:  lis          r9, -0x7da6
0x8259F75C:  addi         r3, r3, -0xf88
0x8259F760:  addi         r29, r29, -0x12c0
0x8259F764:  addi         r30, r30, -0x10f0
0x8259F768:  addi         r5, r5, -0xef8
0x8259F76C:  addi         r7, r7, -0xce8
0x8259F770:  addi         r9, r9, -0xee0
0x8259F774:  stw          r3, 8(r31)
0x8259F778:  addi         r10, r10, 1
0x8259F77C:  stw          r29, 0(r31)
0x8259F780:  stw          r30, 4(r31)
0x8259F784:  li           r3, 1
0x8259F788:  stw          r5, 0x10(r31)
0x8259F78C:  stw          r7, 0x18(r31)
0x8259F790:  stw          r9, 0x20(r31)
0x8259F794:  stw          r10, -0x27b8(r11)
0x8259F798:  addi         r1, r1, 0xf0
0x8259F79C:  b            0x8242f8d8
0x8259F7A0:  lis          r11, -0x7da6
0x8259F7A4:  addi         r3, r11, -0xb20
0x8259F7A8:  blr          
0x8259F7AC:  .align 0                       # padding
0x8259F7B0:  mflr         r12
0x8259F7B4:  bl           0x8242f888
0x8259F7B8:  stwu         r1, -0xa0(r1)
0x8259F7BC:  lis          r11, -0x7d8c
0x8259F7C0:  mr           r25, r3
0x8259F7C4:  addi         r31, r11, -0x27b4
0x8259F7C8:  mr           r24, r4
0x8259F7CC:  lwz          r10, 0x10(r31)
0x8259F7D0:  lwz          r11, 4(r31)
0x8259F7D4:  cmplw        cr6, r10, r11
0x8259F7D8:  bne          cr6, 0x8259f844
0x8259F7DC:  slwi         r3, r10, 4
0x8259F7E0:  bl           0x8257af60
0x8259F7E4:  lwz          r30, 0x10(r31)
0x8259F7E8:  lwz          r26, 8(r31)
0x8259F7EC:  mr           r29, r3
0x8259F7F0:  slwi         r28, r30, 2
0x8259F7F4:  slwi         r11, r30, 3
0x8259F7F8:  mr           r4, r26
0x8259F7FC:  mr           r5, r28
0x8259F800:  add          r27, r11, r29
0x8259F804:  bl           0x82434100
0x8259F808:  mr           r5, r28
0x8259F80C:  mr           r3, r27
0x8259F810:  lwz          r4, 0xc(r31)
0x8259F814:  bl           0x82434100
0x8259F818:  slwi         r11, r30, 1
0x8259F81C:  cmplwi       cr6, r11, 0x40
0x8259F820:  stw          r11, 0x10(r31)
0x8259F824:  beq          cr6, 0x8259f830
0x8259F828:  mr           r3, r26
0x8259F82C:  bl           0x8257b028
0x8259F830:  mr           r7, r27
0x8259F834:  lwz          r11, 4(r31)
0x8259F838:  stw          r29, 8(r31)
0x8259F83C:  stw          r7, 0xc(r31)
0x8259F840:  b            0x8259f84c
0x8259F844:  lwz          r29, 8(r31)
0x8259F848:  lwz          r7, 0xc(r31)
0x8259F84C:  addi         r9, r24, 0x1f
0x8259F850:  lwz          r10, 0(r31)
0x8259F854:  slwi         r8, r11, 2
0x8259F858:  addi         r11, r11, 1
0x8259F85C:  rlwinm       r9, r9, 0, 0, 0x1a
0x8259F860:  srwi         r6, r10, 5
0x8259F864:  srwi         r5, r9, 5
0x8259F868:  stw          r11, 4(r31)
0x8259F86C:  subf         r11, r5, r6
0x8259F870:  addi         r4, r11, 1
0x8259F874:  rlwinm       r11, r4, 5, 0x16, 0x1a
0x8259F878:  add          r11, r11, r9
0x8259F87C:  add          r10, r10, r11
0x8259F880:  stwx         r11, r8, r7
0x8259F884:  stwx         r25, r8, r29
0x8259F888:  stw          r10, 0(r31)
0x8259F88C:  addi         r1, r1, 0xa0
0x8259F890:  b            0x8242f8d8
0x8259F894:  .align 0                       # padding
0x8259F898:  lis          r11, -0x7d8c
0x8259F89C:  lwz          r3, -0x27b4(r11)
0x8259F8A0:  blr          
0x8259F8A4:  .align 0                       # padding
0x8259F8A8:  mflr         r12
0x8259F8AC:  stw          r12, -8(r1)
0x8259F8B0:  std          r30, -0x18(r1)
0x8259F8B4:  std          r31, -0x10(r1)
0x8259F8B8:  stwu         r1, -0x70(r1)
0x8259F8BC:  addi         r11, r3, 0x1f
0x8259F8C0:  rlwinm       r30, r11, 0, 0, 0x1a
0x8259F8C4:  lis          r11, -0x7d8c
0x8259F8C8:  addi         r31, r11, -0x27b4
0x8259F8CC:  lwz          r11, 0(r31)
0x8259F8D0:  add          r3, r11, r30
0x8259F8D4:  bl           0x8257af60
0x8259F8D8:  li           r11, 0
0x8259F8DC:  cmplwi       cr6, r3, 0
0x8259F8E0:  stw          r11, 0(r31)
0x8259F8E4:  beq          cr6, 0x8259f924
0x8259F8E8:  lwz          r10, 4(r31)
0x8259F8EC:  add          r9, r3, r30
0x8259F8F0:  cmplwi       cr6, r10, 0
0x8259F8F4:  beq          cr6, 0x8259f924
0x8259F8F8:  lwz          r11, 0xc(r31)
0x8259F8FC:  lwz          r8, 8(r31)
0x8259F900:  subf         r7, r11, r8
0x8259F904:  lwzx         r8, r7, r11
0x8259F908:  addi         r10, r10, -1
0x8259F90C:  cmplwi       cr6, r10, 0
0x8259F910:  stw          r9, 0(r8)
0x8259F914:  lwz          r8, 0(r11)
0x8259F918:  addi         r11, r11, 4
0x8259F91C:  add          r9, r8, r9
0x8259F920:  bne          cr6, 0x8259f904
0x8259F924:  li           r11, 0
0x8259F928:  stw          r11, 4(r31)
0x8259F92C:  addi         r1, r1, 0x70
0x8259F930:  lwz          r12, -8(r1)
0x8259F934:  mtlr         r12
0x8259F938:  ld           r30, -0x18(r1)
0x8259F93C:  ld           r31, -0x10(r1)
0x8259F940:  blr          
0x8259F944:  .align 0                       # padding
0x8259F948:  mr           r10, r3
0x8259F94C:  cmplwi       cr6, r10, 0
0x8259F950:  beq          cr6, 0x8259f998
0x8259F954:  lwz          r11, 0x18(r10)
0x8259F958:  cmplw        cr6, r11, r4
0x8259F95C:  bne          cr6, 0x8259f970
0x8259F960:  lwz          r11, 0(r11)
0x8259F964:  li           r3, 1
0x8259F968:  stw          r11, 0x18(r10)
0x8259F96C:  blr          
0x8259F970:  lwz          r10, 0(r11)
0x8259F974:  cmplwi       cr6, r10, 0
0x8259F978:  beq          cr6, 0x8259f998
0x8259F97C:  lwz          r10, 0(r11)
0x8259F980:  cmplw        cr6, r10, r4
0x8259F984:  beq          cr6, 0x8259f9a0
0x8259F988:  mr           r11, r10
0x8259F98C:  lwz          r9, 0(r11)
0x8259F990:  cmplwi       cr6, r9, 0
0x8259F994:  bne          cr6, 0x8259f97c
0x8259F998:  li           r3, 0
0x8259F99C:  blr          
0x8259F9A0:  lwz          r8, 0(r4)
0x8259F9A4:  li           r3, 1
0x8259F9A8:  stw          r8, 0(r11)
0x8259F9AC:  blr          
0x8259F9B0:  mflr         r12
0x8259F9B4:  bl           0x8242f89c
0x8259F9B8:  stwu         r1, -0x80(r1)
0x8259F9BC:  mr           r30, r3
0x8259F9C0:  mr           r31, r4
0x8259F9C4:  cmplwi       cr6, r30, 0
0x8259F9C8:  bne          cr6, 0x8259f9e8
0x8259F9CC:  cmplwi       cr6, r31, 0
0x8259F9D0:  beq          cr6, 0x8259fa50
0x8259F9D4:  li           r4, -1
0x8259F9D8:  mr           r3, r31
0x8259F9DC:  bl           0x8242c3b8
0x8259F9E0:  addi         r1, r1, 0x80
0x8259F9E4:  b            0x8242f8ec
0x8259F9E8:  cmplwi       cr6, r31, 0
0x8259F9EC:  bne          cr6, 0x8259fa04
0x8259F9F0:  li           r4, -1
0x8259F9F4:  mr           r3, r30
0x8259F9F8:  bl           0x8242c3b8
0x8259F9FC:  addi         r1, r1, 0x80
0x8259FA00:  b            0x8242f8ec
0x8259FA04:  li           r29, -1
0x8259FA08:  stw          r30, 0x50(r1)
0x8259FA0C:  li           r5, 0
0x8259FA10:  stw          r31, 0x54(r1)
0x8259FA14:  mr           r6, r29
0x8259FA18:  addi         r4, r1, 0x50
0x8259FA1C:  li           r3, 2
0x8259FA20:  bl           0x82566ea8
0x8259FA24:  cmplwi       cr6, r3, 1
0x8259FA28:  blt          cr6, 0x8259fa44
0x8259FA2C:  bne          cr6, 0x8259fa50
0x8259FA30:  mr           r4, r29
0x8259FA34:  mr           r3, r30
0x8259FA38:  bl           0x8242c3b8
0x8259FA3C:  addi         r1, r1, 0x80
0x8259FA40:  b            0x8242f8ec
0x8259FA44:  mr           r4, r29
0x8259FA48:  mr           r3, r31
0x8259FA4C:  bl           0x8242c3b8
0x8259FA50:  addi         r1, r1, 0x80
0x8259FA54:  b            0x8242f8ec
0x8259FA58:  mflr         r12
0x8259FA5C:  bl           0x8242f898
0x8259FA60:  stwu         r1, -0x100(r1)
0x8259FA64:  mr           r29, r3
0x8259FA68:  li           r28, 0
0x8259FA6C:  lwz          r11, 0x1c(r29)
0x8259FA70:  addi         r11, r11, 1
0x8259FA74:  stw          r11, 0x1c(r29)
0x8259FA78:  lwz          r31, 0x18(r29)
0x8259FA7C:  mr           r30, r28
0x8259FA80:  cmplwi       cr6, r31, 0
0x8259FA84:  beq          cr6, 0x8259fb30
0x8259FA88:  lwz          r10, 0(r29)
0x8259FA8C:  cmpwi        cr6, r10, 1
0x8259FA90:  bne          cr6, 0x8259fbd4
0x8259FA94:  lwz          r4, 0x1c(r29)
0x8259FA98:  mr           r3, r31
0x8259FA9C:  lwz          r9, 0x10(r31)
0x8259FAA0:  mtctr        r9
0x8259FAA4:  bctrl        
0x8259FAA8:  cmplwi       cr6, r3, 0
0x8259FAAC:  stw          r3, 0xc(r31)
0x8259FAB0:  beq          cr6, 0x8259fb24
0x8259FAB4:  cmplwi       cr6, r30, 0
0x8259FAB8:  bne          cr6, 0x8259fac8
0x8259FABC:  mr           r30, r31
0x8259FAC0:  stw          r28, 8(r31)
0x8259FAC4:  b            0x8259fb24
0x8259FAC8:  lwz          r8, 0xc(r30)
0x8259FACC:  cmplw        cr6, r3, r8
0x8259FAD0:  blt          cr6, 0x8259fae4
0x8259FAD4:  mr           r7, r30
0x8259FAD8:  mr           r30, r31
0x8259FADC:  stw          r7, 8(r31)
0x8259FAE0:  b            0x8259fb24
0x8259FAE4:  addi         r11, r30, 8
0x8259FAE8:  mr           r10, r30
0x8259FAEC:  lwz          r6, 0(r11)
0x8259FAF0:  cmplwi       cr6, r6, 0
0x8259FAF4:  beq          cr6, 0x8259fb1c
0x8259FAF8:  lwz          r11, 0(r11)
0x8259FAFC:  lwz          r5, 0xc(r11)
0x8259FB00:  cmplw        cr6, r3, r5
0x8259FB04:  bge          cr6, 0x8259fbc8
0x8259FB08:  mr           r10, r11
0x8259FB0C:  addi         r11, r10, 8
0x8259FB10:  lwz          r4, 0(r11)
0x8259FB14:  cmplwi       cr6, r4, 0
0x8259FB18:  bne          cr6, 0x8259faf8
0x8259FB1C:  stw          r28, 8(r31)
0x8259FB20:  stw          r31, 8(r10)
0x8259FB24:  lwz          r31, 0(r31)
0x8259FB28:  cmplwi       cr6, r31, 0
0x8259FB2C:  bne          cr6, 0x8259fa88
0x8259FB30:  mr           r11, r30
0x8259FB34:  mr           r31, r28
0x8259FB38:  cmplwi       cr6, r30, 0
0x8259FB3C:  beq          cr6, 0x8259fbd4
0x8259FB40:  mr           r10, r28
0x8259FB44:  cmplwi       cr6, r10, 0x40
0x8259FB48:  bge          cr6, 0x8259fb78
0x8259FB4C:  lwz          r9, 4(r11)
0x8259FB50:  addi         r8, r1, 0x50
0x8259FB54:  mr           r7, r11
0x8259FB58:  addi         r6, r1, 0x90
0x8259FB5C:  addi         r31, r31, 1
0x8259FB60:  stwx         r9, r10, r8
0x8259FB64:  lwz          r11, 8(r11)
0x8259FB68:  stwx         r7, r10, r6
0x8259FB6C:  addi         r10, r10, 4
0x8259FB70:  cmplwi       cr6, r11, 0
0x8259FB74:  bne          cr6, 0x8259fb44
0x8259FB78:  cmplwi       cr6, r31, 0
0x8259FB7C:  beq          cr6, 0x8259fbd4
0x8259FB80:  li           r6, 1
0x8259FB84:  li           r5, 0
0x8259FB88:  addi         r4, r1, 0x50
0x8259FB8C:  mr           r3, r31
0x8259FB90:  bl           0x82566ea8
0x8259FB94:  cmplw        cr6, r3, r31
0x8259FB98:  bge          cr6, 0x8259fa78
0x8259FB9C:  slwi         r31, r3, 2
0x8259FBA0:  lwz          r4, 0x1c(r29)
0x8259FBA4:  addi         r5, r1, 0x90
0x8259FBA8:  lwzx         r3, r31, r5
0x8259FBAC:  lwz          r11, 0x14(r3)
0x8259FBB0:  mtctr        r11
0x8259FBB4:  bctrl        
0x8259FBB8:  addi         r10, r1, 0x50
0x8259FBBC:  lwzx         r3, r31, r10
0x8259FBC0:  bl           0x8257b670
0x8259FBC4:  b            0x8259fa78
0x8259FBC8:  lwz          r3, 8(r10)
0x8259FBCC:  stw          r3, 8(r31)
0x8259FBD0:  b            0x8259fb20
0x8259FBD4:  addi         r1, r1, 0x100
0x8259FBD8:  b            0x8242f8e8
0x8259FBDC:  .align 0                       # padding
0x8259FBE0:  mflr         r12
0x8259FBE4:  stw          r12, -8(r1)
0x8259FBE8:  std          r30, -0x18(r1)
0x8259FBEC:  std          r31, -0x10(r1)
0x8259FBF0:  stwu         r1, -0x70(r1)
0x8259FBF4:  mr           r31, r3
0x8259FBF8:  lwz          r4, 4(r31)
0x8259FBFC:  lwz          r3, 8(r31)
0x8259FC00:  bl           0x8242c3b8
0x8259FC04:  cmplwi       cr6, r3, 0
0x8259FC08:  beq          cr6, 0x8259fc38
0x8259FC0C:  li           r30, -1
0x8259FC10:  mr           r4, r30
0x8259FC14:  lwz          r3, 0x14(r31)
0x8259FC18:  bl           0x8242c3b8
0x8259FC1C:  lwz          r11, 0(r31)
0x8259FC20:  cmpwi        cr6, r11, 1
0x8259FC24:  beq          cr6, 0x8259fc54
0x8259FC28:  cmpwi        cr6, r11, 3
0x8259FC2C:  bne          cr6, 0x8259fc68
0x8259FC30:  lwz          r3, 0x14(r31)
0x8259FC34:  bl           0x8257b670
0x8259FC38:  li           r3, 0
0x8259FC3C:  addi         r1, r1, 0x70
0x8259FC40:  lwz          r12, -8(r1)
0x8259FC44:  mtlr         r12
0x8259FC48:  ld           r30, -0x18(r1)
0x8259FC4C:  ld           r31, -0x10(r1)
0x8259FC50:  blr          
0x8259FC54:  lwz          r11, 0x18(r31)
0x8259FC58:  cmplwi       cr6, r11, 0
0x8259FC5C:  beq          cr6, 0x8259fc68
0x8259FC60:  mr           r3, r31
0x8259FC64:  bl           0x8259fa58
0x8259FC68:  lwz          r3, 0x14(r31)
0x8259FC6C:  bl           0x8257b670
0x8259FC70:  lwz          r4, 4(r31)
0x8259FC74:  lwz          r3, 8(r31)
0x8259FC78:  bl           0x8242c3b8
0x8259FC7C:  cmplwi       cr6, r3, 0
0x8259FC80:  bne          cr6, 0x8259fc10
0x8259FC84:  b            0x8259fc38
0x8259FC88:  mflr         r12
0x8259FC8C:  stw          r12, -8(r1)
0x8259FC90:  std          r30, -0x18(r1)
0x8259FC94:  std          r31, -0x10(r1)
0x8259FC98:  stwu         r1, -0x70(r1)
0x8259FC9C:  mr           r30, r3
0x8259FCA0:  li           r3, 0x24
0x8259FCA4:  bl           0x8257af60
0x8259FCA8:  mr           r31, r3
0x8259FCAC:  cmplwi       cr6, r31, 0
0x8259FCB0:  beq          cr6, 0x8259fd28
0x8259FCB4:  cmpwi        cr6, r30, 0
0x8259FCB8:  bge          cr6, 0x8259fcc0
0x8259FCBC:  neg          r30, r30
0x8259FCC0:  li           r10, 0x3e8
0x8259FCC4:  li           r5, 1
0x8259FCC8:  li           r11, 0
0x8259FCCC:  divw         r9, r10, r30
0x8259FCD0:  lis          r10, -0x7da6
0x8259FCD4:  lis          r4, 0
0x8259FCD8:  stw          r5, 0x1c(r31)
0x8259FCDC:  addi         r8, r31, 0x10
0x8259FCE0:  li           r7, 4
0x8259FCE4:  stw          r11, 0(r31)
0x8259FCE8:  mr           r6, r31
0x8259FCEC:  stw          r9, 4(r31)
0x8259FCF0:  addi         r5, r10, -0x420
0x8259FCF4:  stw          r11, 0x18(r31)
0x8259FCF8:  ori          r4, r4, 0x8000
0x8259FCFC:  stw          r11, 0x20(r31)
0x8259FD00:  li           r3, 0
0x8259FD04:  bl           0x82566f88
0x8259FD08:  li           r4, 5
0x8259FD0C:  stw          r3, 0xc(r31)
0x8259FD10:  bl           0x8242b8a8
0x8259FD14:  lwz          r4, 0xc(r31)
0x8259FD18:  cmplwi       cr6, r4, 0
0x8259FD1C:  bne          cr6, 0x8259fd30
0x8259FD20:  mr           r3, r31
0x8259FD24:  bl           0x8257b028
0x8259FD28:  li           r3, 0
0x8259FD2C:  b            0x8259fd68
0x8259FD30:  li           r5, 0
0x8259FD34:  li           r4, 0
0x8259FD38:  li           r3, 0
0x8259FD3C:  bl           0x8257b6b0
0x8259FD40:  mr           r11, r3
0x8259FD44:  li           r6, 0
0x8259FD48:  li           r5, 0
0x8259FD4C:  li           r4, 0
0x8259FD50:  li           r3, 0
0x8259FD54:  stw          r11, 0x14(r31)
0x8259FD58:  bl           0x82566c88
0x8259FD5C:  mr           r11, r3
0x8259FD60:  mr           r3, r31
0x8259FD64:  stw          r11, 8(r31)
0x8259FD68:  addi         r1, r1, 0x70
0x8259FD6C:  lwz          r12, -8(r1)
0x8259FD70:  mtlr         r12
0x8259FD74:  ld           r30, -0x18(r1)
0x8259FD78:  ld           r31, -0x10(r1)
0x8259FD7C:  blr          
0x8259FD80:  mflr         r12
0x8259FD84:  stw          r12, -8(r1)
0x8259FD88:  std          r30, -0x18(r1)
0x8259FD8C:  std          r31, -0x10(r1)
0x8259FD90:  stwu         r1, -0x70(r1)
0x8259FD94:  mr           r31, r4
0x8259FD98:  mr           r11, r5
0x8259FD9C:  mr           r30, r3
0x8259FDA0:  li           r5, 0
0x8259FDA4:  li           r4, 0
0x8259FDA8:  li           r3, 0
0x8259FDAC:  stw          r6, 0x14(r31)
0x8259FDB0:  stw          r11, 0x10(r31)
0x8259FDB4:  bl           0x8257b6b0
0x8259FDB8:  stw          r3, 4(r31)
0x8259FDBC:  lwz          r11, 0x18(r30)
0x8259FDC0:  stw          r11, 0(r31)
0x8259FDC4:  lwz          r11, 0x20(r30)
0x8259FDC8:  stw          r31, 0x18(r30)
0x8259FDCC:  addi         r10, r11, 1
0x8259FDD0:  stw          r10, 0x20(r30)
0x8259FDD4:  addi         r1, r1, 0x70
0x8259FDD8:  lwz          r12, -8(r1)
0x8259FDDC:  mtlr         r12
0x8259FDE0:  ld           r30, -0x18(r1)
0x8259FDE4:  ld           r31, -0x10(r1)
0x8259FDE8:  blr          
0x8259FDEC:  .align 0                       # padding
0x8259FDF0:  mflr         r12
0x8259FDF4:  stw          r12, -8(r1)
0x8259FDF8:  std          r30, -0x18(r1)
0x8259FDFC:  std          r31, -0x10(r1)
0x8259FE00:  stwu         r1, -0x70(r1)
0x8259FE04:  mr           r31, r3
0x8259FE08:  lwz          r30, 0(r31)
0x8259FE0C:  lwz          r3, 8(r31)
0x8259FE10:  bl           0x82566c40
0x8259FE14:  li           r11, 3
0x8259FE18:  lwz          r3, 0xc(r31)
0x8259FE1C:  li           r4, 0xf
0x8259FE20:  stw          r11, 0(r31)
0x8259FE24:  bl           0x8242b7d0
0x8259FE28:  cmpwi        cr6, r30, 0
0x8259FE2C:  beq          cr6, 0x8259fe44
0x8259FE30:  cmpwi        cr6, r30, 2
0x8259FE34:  bne          cr6, 0x8259fe4c
0x8259FE38:  lwz          r3, 0x14(r31)
0x8259FE3C:  bl           0x8257b670
0x8259FE40:  b            0x8259fe4c
0x8259FE44:  lwz          r3, 0xc(r31)
0x8259FE48:  bl           0x82566f48
0x8259FE4C:  addi         r1, r1, 0x70
0x8259FE50:  lwz          r12, -8(r1)
0x8259FE54:  mtlr         r12
0x8259FE58:  ld           r30, -0x18(r1)
0x8259FE5C:  ld           r31, -0x10(r1)
0x8259FE60:  blr          
0x8259FE64:  .align 0                       # padding
0x8259FE68:  mflr         r12
0x8259FE6C:  stw          r12, -8(r1)
0x8259FE70:  std          r30, -0x18(r1)
0x8259FE74:  std          r31, -0x10(r1)
0x8259FE78:  stwu         r1, -0x70(r1)
0x8259FE7C:  mr           r30, r3
0x8259FE80:  mr           r31, r4
0x8259FE84:  cmplwi       cr6, r30, 0
0x8259FE88:  beq          cr6, 0x8259fed4
0x8259FE8C:  li           r11, 4
0x8259FE90:  cmplwi       cr6, r31, 0
0x8259FE94:  stw          r11, 0(r30)
0x8259FE98:  beq          cr6, 0x8259febc
0x8259FE9C:  stw          r11, 0(r31)
0x8259FEA0:  lwz          r4, 0x14(r31)
0x8259FEA4:  lwz          r3, 0x14(r30)
0x8259FEA8:  bl           0x8259f9b0
0x8259FEAC:  li           r11, 2
0x8259FEB0:  stw          r11, 0(r30)
0x8259FEB4:  stw          r11, 0(r31)
0x8259FEB8:  b            0x8259fef8
0x8259FEBC:  li           r4, -1
0x8259FEC0:  lwz          r3, 0x14(r30)
0x8259FEC4:  bl           0x8242c3b8
0x8259FEC8:  li           r10, 2
0x8259FECC:  stw          r10, 0(r30)
0x8259FED0:  b            0x8259fef8
0x8259FED4:  cmplwi       cr6, r31, 0
0x8259FED8:  beq          cr6, 0x8259fef8
0x8259FEDC:  li           r9, 4
0x8259FEE0:  lwz          r3, 0x14(r31)
0x8259FEE4:  li           r4, -1
0x8259FEE8:  stw          r9, 0(r31)
0x8259FEEC:  bl           0x8242c3b8
0x8259FEF0:  li           r8, 2
0x8259FEF4:  stw          r8, 0(r31)
0x8259FEF8:  addi         r1, r1, 0x70
0x8259FEFC:  lwz          r12, -8(r1)
0x8259FF00:  mtlr         r12
0x8259FF04:  ld           r30, -0x18(r1)
0x8259FF08:  ld           r31, -0x10(r1)
0x8259FF0C:  blr          
0x8259FF10:  mr           r11, r3
0x8259FF14:  lwz          r10, 0(r11)
0x8259FF18:  cmpwi        cr6, r10, 0
0x8259FF1C:  bne          cr6, 0x8259ff30
0x8259FF20:  li           r9, 1
0x8259FF24:  lwz          r3, 0xc(r11)
0x8259FF28:  stw          r9, 0(r11)
0x8259FF2C:  b            0x82566f48
0x8259FF30:  li           r8, 1
0x8259FF34:  lwz          r3, 0x14(r11)
0x8259FF38:  stw          r8, 0(r11)
0x8259FF3C:  b            0x8257b670
0x8259FF40:  mflr         r12
0x8259FF44:  stw          r12, -8(r1)
0x8259FF48:  std          r31, -0x10(r1)
0x8259FF4C:  stwu         r1, -0x60(r1)
0x8259FF50:  mr           r31, r3
0x8259FF54:  bl           0x8259fdf0
0x8259FF58:  li           r4, -1
0x8259FF5C:  lwz          r3, 0xc(r31)
0x8259FF60:  bl           0x8242c3b8
0x8259FF64:  lwz          r3, 0x14(r31)
0x8259FF68:  bl           0x82566f10
0x8259FF6C:  lwz          r3, 0xc(r31)
0x8259FF70:  bl           0x82566f10
0x8259FF74:  lwz          r3, 8(r31)
0x8259FF78:  bl           0x82566f10
0x8259FF7C:  mr           r3, r31
0x8259FF80:  bl           0x8257b028
0x8259FF84:  addi         r1, r1, 0x60
0x8259FF88:  lwz          r12, -8(r1)
0x8259FF8C:  mtlr         r12
0x8259FF90:  ld           r31, -0x10(r1)
0x8259FF94:  blr          
0x8259FF98:  mflr         r12
0x8259FF9C:  stw          r12, -8(r1)
0x8259FFA0:  stwu         r1, -0x60(r1)
0x8259FFA4:  cmplwi       cr6, r4, 0
0x8259FFA8:  beq          cr6, 0x8259ffd8
0x8259FFAC:  lwz          r3, 4(r4)
0x8259FFB0:  cmplwi       cr6, r3, 0
0x8259FFB4:  beq          cr6, 0x8259ffd8
0x8259FFB8:  li           r4, 0
0x8259FFBC:  bl           0x8242c3b8
0x8259FFC0:  cntlzw       r11, r3
0x8259FFC4:  rlwinm       r3, r11, 0x1b, 0x1f, 0x1f
0x8259FFC8:  addi         r1, r1, 0x60
0x8259FFCC:  lwz          r12, -8(r1)
0x8259FFD0:  mtlr         r12
0x8259FFD4:  blr          
0x8259FFD8:  li           r3, 1
0x8259FFDC:  addi         r1, r1, 0x60
0x8259FFE0:  lwz          r12, -8(r1)
0x8259FFE4:  mtlr         r12
0x8259FFE8:  blr          
0x8259FFEC:  .align 0                       # padding
0x8259FFF0:  cmplwi       cr6, r4, 0
0x8259FFF4:  beqlr        cr6
0x8259FFF8:  lwz          r3, 4(r4)
0x8259FFFC:  cmplwi       cr6, r3, 0
0x825A0000:  beqlr        cr6
0x825A0004:  li           r4, -1
0x825A0008:  b            0x8242c3b8
0x825A000C:  blr          
0x825A0010:  mr           r11, r4
0x825A0014:  lwz          r4, 4(r6)
0x825A0018:  lwz          r3, 4(r11)
0x825A001C:  b            0x8259f9b0
0x825A0020:  cmplwi       cr6, r4, 0
0x825A0024:  beqlr        cr6
0x825A0028:  lwz          r3, 4(r4)
0x825A002C:  cmplwi       cr6, r3, 0
0x825A0030:  beqlr        cr6
0x825A0034:  b            0x8257b670
0x825A0038:  blr          
0x825A003C:  .align 0                       # padding
0x825A0040:  mflr         r12
0x825A0044:  stw          r12, -8(r1)
0x825A0048:  std          r30, -0x18(r1)
0x825A004C:  std          r31, -0x10(r1)
0x825A0050:  stwu         r1, -0x70(r1)
0x825A0054:  mr           r30, r3
0x825A0058:  mr           r31, r4
0x825A005C:  cmplwi       cr6, r30, 0
0x825A0060:  bne          cr6, 0x825a0078
0x825A0064:  cmplwi       cr6, r31, 0
0x825A0068:  beq          cr6, 0x825a00dc
0x825A006C:  mr           r3, r31
0x825A0070:  bl           0x8259ff40
0x825A0074:  b            0x825a00dc
0x825A0078:  cmplwi       cr6, r31, 0
0x825A007C:  mr           r3, r30
0x825A0080:  beq          cr6, 0x825a0070
0x825A0084:  bl           0x8259fdf0
0x825A0088:  mr           r3, r31
0x825A008C:  bl           0x8259fdf0
0x825A0090:  lwz          r4, 0xc(r31)
0x825A0094:  lwz          r3, 0xc(r30)
0x825A0098:  bl           0x8259f9b0
0x825A009C:  lwz          r3, 0x14(r30)
0x825A00A0:  bl           0x82566f10
0x825A00A4:  lwz          r3, 0xc(r30)
0x825A00A8:  bl           0x82566f10
0x825A00AC:  lwz          r3, 8(r30)
0x825A00B0:  bl           0x82566f10
0x825A00B4:  mr           r3, r30
0x825A00B8:  bl           0x8257b028
0x825A00BC:  lwz          r3, 0x14(r31)
0x825A00C0:  bl           0x82566f10
0x825A00C4:  lwz          r3, 0xc(r31)
0x825A00C8:  bl           0x82566f10
0x825A00CC:  lwz          r3, 8(r31)
0x825A00D0:  bl           0x82566f10
0x825A00D4:  mr           r3, r31
0x825A00D8:  bl           0x8257b028
0x825A00DC:  addi         r1, r1, 0x70
0x825A00E0:  lwz          r12, -8(r1)
0x825A00E4:  mtlr         r12
0x825A00E8:  ld           r30, -0x18(r1)
0x825A00EC:  ld           r31, -0x10(r1)
0x825A00F0:  blr          
0x825A00F4:  .align 0                       # padding
0x825A00F8:  li           r3, 0
0x825A00FC:  b            0x82566c80
0x825A0100:  mflr         r12
0x825A0104:  stw          r12, -8(r1)
0x825A0108:  std          r30, -0x18(r1)
0x825A010C:  std          r31, -0x10(r1)
0x825A0110:  stwu         r1, -0x70(r1)
0x825A0114:  mr           r31, r3
0x825A0118:  clrlwi       r11, r5, 0x1f
0x825A011C:  mr           r30, r4
0x825A0120:  cmplwi       cr6, r11, 0
0x825A0124:  lwz          r11, 0x20(r31)
0x825A0128:  addi         r10, r11, -1
0x825A012C:  stw          r10, 0x20(r31)
0x825A0130:  beq          cr6, 0x825a014c
0x825A0134:  lwz          r9, 0x18(r31)
0x825A0138:  cmplwi       cr6, r9, 0
0x825A013C:  bne          cr6, 0x825a014c
0x825A0140:  bl           0x8259ff40
0x825A0144:  li           r31, 1
0x825A0148:  b            0x825a0174
0x825A014C:  li           r8, 4
0x825A0150:  lwz          r3, 0x14(r31)
0x825A0154:  li           r4, -1
0x825A0158:  stw          r8, 0(r31)
0x825A015C:  bl           0x8242c3b8
0x825A0160:  li           r7, 1
0x825A0164:  lwz          r3, 0x14(r31)
0x825A0168:  stw          r7, 0(r31)
0x825A016C:  bl           0x8257b670
0x825A0170:  li           r31, 0
0x825A0174:  lwz          r3, 4(r30)
0x825A0178:  bl           0x82566f10
0x825A017C:  li           r6, 0
0x825A0180:  mr           r3, r31
0x825A0184:  stw          r6, 4(r30)
0x825A0188:  addi         r1, r1, 0x70
0x825A018C:  lwz          r12, -8(r1)
0x825A0190:  mtlr         r12
0x825A0194:  ld           r30, -0x18(r1)
0x825A0198:  ld           r31, -0x10(r1)
0x825A019C:  blr          
0x825A01A0:  mflr         r12
0x825A01A4:  bl           0x8242f890
0x825A01A8:  stwu         r1, -0x90(r1)
0x825A01AC:  li           r26, 0
0x825A01B0:  mr           r30, r5
0x825A01B4:  mr           r27, r6
0x825A01B8:  mr           r31, r3
0x825A01BC:  mr           r28, r4
0x825A01C0:  mr           r5, r7
0x825A01C4:  mr           r6, r26
0x825A01C8:  bl           0x8259f948
0x825A01CC:  li           r29, 1
0x825A01D0:  cmpwi        cr6, r3, 0
0x825A01D4:  beq          cr6, 0x825a01dc
0x825A01D8:  mr           r6, r29
0x825A01DC:  mr           r4, r27
0x825A01E0:  mr           r3, r30
0x825A01E4:  bl           0x8259f948
0x825A01E8:  cmpwi        cr6, r3, 0
0x825A01EC:  beq          cr6, 0x825a01f4
0x825A01F0:  ori          r6, r6, 2
0x825A01F4:  cmpwi        cr6, r6, 1
0x825A01F8:  beq          cr6, 0x825a03a4
0x825A01FC:  cmpwi        cr6, r6, 2
0x825A0200:  beq          cr6, 0x825a038c
0x825A0204:  cmpwi        cr6, r6, 3
0x825A0208:  beq          cr6, 0x825a0218
0x825A020C:  li           r3, 0
0x825A0210:  addi         r1, r1, 0x90
0x825A0214:  b            0x8242f8e0
0x825A0218:  clrlwi       r11, r5, 0x1f
0x825A021C:  cmplwi       cr6, r11, 0
0x825A0220:  lwz          r11, 0x20(r31)
0x825A0224:  addi         r10, r11, -1
0x825A0228:  stw          r10, 0x20(r31)
0x825A022C:  lwz          r11, 0x20(r30)
0x825A0230:  addi         r9, r11, -1
0x825A0234:  stw          r9, 0x20(r30)
0x825A0238:  beq          cr6, 0x825a0310
0x825A023C:  lwz          r8, 0x18(r31)
0x825A0240:  cmplwi       cr6, r8, 0
0x825A0244:  bne          cr6, 0x825a02b4
0x825A0248:  lwz          r7, 0x18(r30)
0x825A024C:  mr           r3, r31
0x825A0250:  cmplwi       cr6, r7, 0
0x825A0254:  bne          cr6, 0x825a0268
0x825A0258:  mr           r4, r30
0x825A025C:  bl           0x825a0040
0x825A0260:  li           r31, 3
0x825A0264:  b            0x825a0368
0x825A0268:  bl           0x8259fdf0
0x825A026C:  li           r6, 4
0x825A0270:  lwz          r4, 0x14(r30)
0x825A0274:  stw          r6, 0(r30)
0x825A0278:  lwz          r3, 0xc(r31)
0x825A027C:  bl           0x8259f9b0
0x825A0280:  lwz          r3, 0x14(r31)
0x825A0284:  bl           0x82566f10
0x825A0288:  lwz          r3, 0xc(r31)
0x825A028C:  bl           0x82566f10
0x825A0290:  lwz          r3, 8(r31)
0x825A0294:  bl           0x82566f10
0x825A0298:  mr           r3, r31
0x825A029C:  bl           0x8257b028
0x825A02A0:  lwz          r3, 0x14(r30)
0x825A02A4:  stw          r29, 0(r30)
0x825A02A8:  bl           0x8257b670
0x825A02AC:  mr           r31, r29
0x825A02B0:  b            0x825a0368
0x825A02B4:  lwz          r5, 0x18(r30)
0x825A02B8:  cmplwi       cr6, r5, 0
0x825A02BC:  bne          cr6, 0x825a0310
0x825A02C0:  mr           r3, r30
0x825A02C4:  bl           0x8259fdf0
0x825A02C8:  li           r4, 4
0x825A02CC:  stw          r4, 0(r31)
0x825A02D0:  lwz          r4, 0x14(r31)
0x825A02D4:  lwz          r3, 0xc(r30)
0x825A02D8:  bl           0x8259f9b0
0x825A02DC:  lwz          r3, 0x14(r30)
0x825A02E0:  bl           0x82566f10
0x825A02E4:  lwz          r3, 0xc(r30)
0x825A02E8:  bl           0x82566f10
0x825A02EC:  lwz          r3, 8(r30)
0x825A02F0:  bl           0x82566f10
0x825A02F4:  mr           r3, r30
0x825A02F8:  bl           0x8257b028
0x825A02FC:  lwz          r3, 0x14(r31)
0x825A0300:  stw          r29, 0(r31)
0x825A0304:  bl           0x8257b670
0x825A0308:  li           r31, 2
0x825A030C:  b            0x825a0368
0x825A0310:  mr           r4, r30
0x825A0314:  mr           r3, r31
0x825A0318:  bl           0x8259fe68
0x825A031C:  lwz          r3, 0(r31)
0x825A0320:  stw          r29, 0(r31)
0x825A0324:  cmpwi        cr6, r3, 0
0x825A0328:  bne          cr6, 0x825a0338
0x825A032C:  lwz          r3, 0xc(r31)
0x825A0330:  bl           0x82566f48
0x825A0334:  b            0x825a0340
0x825A0338:  lwz          r3, 0x14(r31)
0x825A033C:  bl           0x8257b670
0x825A0340:  lwz          r11, 0(r30)
0x825A0344:  stw          r29, 0(r30)
0x825A0348:  cmpwi        cr6, r11, 0
0x825A034C:  bne          cr6, 0x825a035c
0x825A0350:  lwz          r3, 0xc(r30)
0x825A0354:  bl           0x82566f48
0x825A0358:  b            0x825a0364
0x825A035C:  lwz          r3, 0x14(r30)
0x825A0360:  bl           0x8257b670
0x825A0364:  mr           r31, r26
0x825A0368:  lwz          r3, 4(r28)
0x825A036C:  bl           0x82566f10
0x825A0370:  lwz          r3, 4(r27)
0x825A0374:  bl           0x82566f10
0x825A0378:  stw          r26, 4(r28)
0x825A037C:  mr           r3, r31
0x825A0380:  stw          r26, 4(r27)
0x825A0384:  addi         r1, r1, 0x90
0x825A0388:  b            0x8242f8e0
0x825A038C:  mr           r4, r27
0x825A0390:  mr           r3, r30
0x825A0394:  bl           0x825a0100
0x825A0398:  slwi         r3, r3, 1
0x825A039C:  addi         r1, r1, 0x90
0x825A03A0:  b            0x8242f8e0
0x825A03A4:  mr           r4, r28
0x825A03A8:  mr           r3, r31
0x825A03AC:  bl           0x825a0100
0x825A03B0:  addi         r1, r1, 0x90
0x825A03B4:  b            0x8242f8e0
0x825A03B8:  mflr         r12
0x825A03BC:  stw          r12, -8(r1)
0x825A03C0:  stwu         r1, -0x60(r1)
0x825A03C4:  bl           0x82566f98
0x825A03C8:  lis          r11, -0x7d8c
0x825A03CC:  addi         r10, r11, -0x27a0
0x825A03D0:  lwz          r11, 0(r10)
0x825A03D4:  cmplwi       cr6, r11, 0
0x825A03D8:  bne          cr6, 0x825a03e4
0x825A03DC:  mr           r11, r3
0x825A03E0:  stw          r11, 0(r10)
0x825A03E4:  subf         r11, r11, r3
0x825A03E8:  lwz          r3, 4(r10)
0x825A03EC:  lis          r9, -0x4000
0x825A03F0:  subf         r8, r3, r11
0x825A03F4:  cmplw        cr6, r8, r9
0x825A03F8:  bgt          cr6, 0x825a0404
0x825A03FC:  stw          r11, 4(r10)
0x825A0400:  mr           r3, r11
0x825A0404:  addi         r1, r1, 0x60
0x825A0408:  lwz          r12, -8(r1)
0x825A040C:  mtlr         r12
0x825A0410:  blr          
0x825A0414:  .align 0                       # padding
0x825A0418:  mflr         r12
0x825A041C:  bl           0x8242f86c
0x825A0420:  stfd         f29, -0x98(r1)
0x825A0424:  stfd         f30, -0x90(r1)
0x825A0428:  stfd         f31, -0x88(r1)
0x825A042C:  stwu         r1, -0x110(r1)
0x825A0430:  mr           r22, r7
0x825A0434:  mr           r19, r5
0x825A0438:  mr           r18, r4
0x825A043C:  mr           r21, r6
0x825A0440:  li           r26, 0
0x825A0444:  lwz          r11, 0(r22)
0x825A0448:  lwz          r29, 0(r19)
0x825A044C:  slwi         r10, r11, 1
0x825A0450:  lwz          r30, 4(r19)
0x825A0454:  lis          r11, -0x7e00
0x825A0458:  lwz          r31, 8(r19)
0x825A045C:  cmplwi       cr6, r10, 2
0x825A0460:  lfs          f29, 0x3da8(r11)
0x825A0464:  fmr          f31, f29
0x825A0468:  fmr          f30, f29
0x825A046C:  bge          cr6, 0x825a0494
0x825A0470:  li           r11, 0
0x825A0474:  addi         r26, r26, 1
0x825A0478:  lfsx         f31, r11, r21
0x825A047C:  fneg         f30, f31
0x825A0480:  slwi         r11, r26, 2
0x825A0484:  lwzx         r9, r11, r22
0x825A0488:  slwi         r8, r9, 1
0x825A048C:  cmplwi       cr6, r8, 2
0x825A0490:  blt          cr6, 0x825a0474
0x825A0494:  addi         r27, r3, 8
0x825A0498:  li           r23, 2
0x825A049C:  cmplwi       cr6, r18, 2
0x825A04A0:  ble          cr6, 0x825a0a58
0x825A04A4:  lis          r11, -0x7d8c
0x825A04A8:  li           r20, -1
0x825A04AC:  addi         r17, r11, -0x2798
0x825A04B0:  cmplwi       cr6, r31, 0
0x825A04B4:  bne          cr6, 0x825a04cc
0x825A04B8:  lwz          r11, 0(r30)
0x825A04BC:  li           r10, 0x1f
0x825A04C0:  addi         r30, r30, 4
0x825A04C4:  srwi         r9, r11, 1
0x825A04C8:  b            0x825a04d8
0x825A04CC:  mr           r11, r29
0x825A04D0:  srwi         r9, r29, 1
0x825A04D4:  addi         r10, r31, -1
0x825A04D8:  clrlwi       r7, r11, 0x1f
0x825A04DC:  cmplwi       cr6, r7, 0
0x825A04E0:  beq          cr6, 0x825a0538
0x825A04E4:  cmplwi       cr6, r10, 4
0x825A04E8:  bge          cr6, 0x825a051c
0x825A04EC:  lwz          r11, 0(r30)
0x825A04F0:  subfic       r6, r10, 4
0x825A04F4:  addi         r30, r30, 4
0x825A04F8:  slw          r5, r11, r10
0x825A04FC:  addi         r10, r10, 0x1c
0x825A0500:  or           r4, r5, r9
0x825A0504:  srw          r9, r11, r6
0x825A0508:  clrlwi       r8, r4, 0x1c
0x825A050C:  lbzx         r3, r8, r17
0x825A0510:  rotlwi       r11, r3, 3
0x825A0514:  add          r24, r11, r23
0x825A0518:  b            0x825a053c
0x825A051C:  clrlwi       r8, r9, 0x1c
0x825A0520:  srwi         r9, r9, 4
0x825A0524:  addi         r10, r10, -4
0x825A0528:  lbzx         r3, r8, r17
0x825A052C:  rotlwi       r11, r3, 3
0x825A0530:  add          r24, r11, r23
0x825A0534:  b            0x825a053c
0x825A0538:  addi         r24, r23, 8
0x825A053C:  cmplw        cr6, r24, r18
0x825A0540:  ble          cr6, 0x825a0548
0x825A0544:  mr           r24, r18
0x825A0548:  cmplwi       cr6, r10, 4
0x825A054C:  bge          cr6, 0x825a0574
0x825A0550:  lwz          r8, 0(r30)
0x825A0554:  subfic       r7, r10, 4
0x825A0558:  addi         r30, r30, 4
0x825A055C:  addi         r31, r10, 0x1c
0x825A0560:  slw          r6, r8, r10
0x825A0564:  srw          r29, r8, r7
0x825A0568:  or           r5, r6, r9
0x825A056C:  clrlwi       r11, r5, 0x1c
0x825A0570:  b            0x825a0580
0x825A0574:  clrlwi       r11, r9, 0x1c
0x825A0578:  srwi         r29, r9, 4
0x825A057C:  addi         r31, r10, -4
0x825A0580:  cmplwi       cr6, r11, 0
0x825A0584:  bne          cr6, 0x825a05e0
0x825A0588:  subf         r11, r23, r24
0x825A058C:  li           r4, 0
0x825A0590:  slwi         r28, r11, 2
0x825A0594:  mr           r3, r27
0x825A0598:  mr           r5, r28
0x825A059C:  bl           0x8242fed0
0x825A05A0:  slwi         r11, r26, 2
0x825A05A4:  add          r27, r28, r27
0x825A05A8:  mr           r23, r24
0x825A05AC:  lwzx         r4, r11, r22
0x825A05B0:  slwi         r3, r4, 1
0x825A05B4:  cmplw        cr6, r24, r3
0x825A05B8:  ble          cr6, 0x825a0a50
0x825A05BC:  addi         r26, r26, 1
0x825A05C0:  lfsx         f31, r11, r21
0x825A05C4:  fneg         f30, f31
0x825A05C8:  slwi         r11, r26, 2
0x825A05CC:  lwzx         r10, r11, r22
0x825A05D0:  slwi         r9, r10, 1
0x825A05D4:  cmplw        cr6, r24, r9
0x825A05D8:  bgt          cr6, 0x825a05bc
0x825A05DC:  b            0x825a0a50
0x825A05E0:  subf         r8, r23, r24
0x825A05E4:  cmpwi        cr6, r8, 4
0x825A05E8:  blt          cr6, 0x825a095c
0x825A05EC:  subfic       r7, r11, 0x20
0x825A05F0:  slwi         r10, r26, 2
0x825A05F4:  addi         r25, r24, -3
0x825A05F8:  addi         r28, r23, 2
0x825A05FC:  add          r3, r10, r21
0x825A0600:  add          r4, r10, r22
0x825A0604:  srw          r5, r20, r7
0x825A0608:  lwz          r6, 0(r4)
0x825A060C:  slwi         r10, r6, 1
0x825A0610:  cmplw        cr6, r23, r10
0x825A0614:  bne          cr6, 0x825a062c
0x825A0618:  lfs          f31, 0(r3)
0x825A061C:  addi         r26, r26, 1
0x825A0620:  addi         r4, r4, 4
0x825A0624:  fneg         f30, f31
0x825A0628:  addi         r3, r3, 4
0x825A062C:  cmplw        cr6, r31, r11
0x825A0630:  subf         r10, r11, r31
0x825A0634:  bge          cr6, 0x825a065c
0x825A0638:  lwz          r9, 0(r30)
0x825A063C:  subf         r8, r31, r11
0x825A0640:  addi         r30, r30, 4
0x825A0644:  addi         r10, r10, 0x20
0x825A0648:  slw          r7, r9, r31
0x825A064C:  srw          r9, r9, r8
0x825A0650:  or           r6, r7, r29
0x825A0654:  and          r7, r6, r5
0x825A0658:  b            0x825a0664
0x825A065C:  and          r7, r5, r29
0x825A0660:  srw          r9, r29, r11
0x825A0664:  cmpwi        cr6, r7, 0
0x825A0668:  beq          cr6, 0x825a06cc
0x825A066C:  cmplwi       cr6, r10, 0
0x825A0670:  bne          cr6, 0x825a0688
0x825A0674:  lwz          r8, 0(r30)
0x825A0678:  li           r10, 0x1f
0x825A067C:  addi         r30, r30, 4
0x825A0680:  srwi         r9, r8, 1
0x825A0684:  b            0x825a0694
0x825A0688:  mr           r8, r9
0x825A068C:  srwi         r9, r9, 1
0x825A0690:  addi         r10, r10, -1
0x825A0694:  clrlwi       r8, r8, 0x1f
0x825A0698:  cmplwi       cr6, r8, 0
0x825A069C:  beq          cr6, 0x825a06a8
0x825A06A0:  fmr          f0, f30
0x825A06A4:  b            0x825a06ac
0x825A06A8:  fmr          f0, f31
0x825A06AC:  extsw        r7, r7
0x825A06B0:  std          r7, 0x50(r1)
0x825A06B4:  lfd          f13, 0x50(r1)
0x825A06B8:  fcfid        f12, f13
0x825A06BC:  frsp         f11, f12
0x825A06C0:  fmuls        f10, f11, f0
0x825A06C4:  stfs         f10, 0(r27)
0x825A06C8:  b            0x825a06d0
0x825A06CC:  stfs         f29, 0(r27)
0x825A06D0:  lwz          r8, 0(r4)
0x825A06D4:  addi         r7, r28, -1
0x825A06D8:  addi         r6, r27, 4
0x825A06DC:  slwi         r8, r8, 1
0x825A06E0:  cmplw        cr6, r7, r8
0x825A06E4:  bne          cr6, 0x825a06fc
0x825A06E8:  lfs          f31, 0(r3)
0x825A06EC:  addi         r26, r26, 1
0x825A06F0:  addi         r4, r4, 4
0x825A06F4:  fneg         f30, f31
0x825A06F8:  addi         r3, r3, 4
0x825A06FC:  cmplw        cr6, r10, r11
0x825A0700:  bge          cr6, 0x825a072c
0x825A0704:  lwz          r8, 0(r30)
0x825A0708:  subf         r31, r10, r11
0x825A070C:  subf         r7, r11, r10
0x825A0710:  addi         r30, r30, 4
0x825A0714:  slw          r29, r8, r10
0x825A0718:  addi         r10, r7, 0x20
0x825A071C:  or           r7, r29, r9
0x825A0720:  srw          r9, r8, r31
0x825A0724:  and          r7, r7, r5
0x825A0728:  b            0x825a0738
0x825A072C:  and          r7, r5, r9
0x825A0730:  subf         r10, r11, r10
0x825A0734:  srw          r9, r9, r11
0x825A0738:  cmpwi        cr6, r7, 0
0x825A073C:  beq          cr6, 0x825a07a0
0x825A0740:  cmplwi       cr6, r10, 0
0x825A0744:  bne          cr6, 0x825a075c
0x825A0748:  lwz          r8, 0(r30)
0x825A074C:  li           r10, 0x1f
0x825A0750:  addi         r30, r30, 4
0x825A0754:  srwi         r9, r8, 1
0x825A0758:  b            0x825a0768
0x825A075C:  mr           r8, r9
0x825A0760:  srwi         r9, r9, 1
0x825A0764:  addi         r10, r10, -1
0x825A0768:  clrlwi       r8, r8, 0x1f
0x825A076C:  cmplwi       cr6, r8, 0
0x825A0770:  beq          cr6, 0x825a077c
0x825A0774:  fmr          f0, f30
0x825A0778:  b            0x825a0780
0x825A077C:  fmr          f0, f31
0x825A0780:  extsw        r7, r7
0x825A0784:  std          r7, 0x58(r1)
0x825A0788:  lfd          f9, 0x58(r1)
0x825A078C:  fcfid        f8, f9
0x825A0790:  frsp         f7, f8
0x825A0794:  fmuls        f6, f7, f0
0x825A0798:  stfs         f6, 0(r6)
0x825A079C:  b            0x825a07a4
0x825A07A0:  stfs         f29, 0(r6)
0x825A07A4:  lwz          r8, 0(r4)
0x825A07A8:  addi         r6, r6, 4
0x825A07AC:  slwi         r7, r8, 1
0x825A07B0:  cmplw        cr6, r28, r7
0x825A07B4:  bne          cr6, 0x825a07cc
0x825A07B8:  lfs          f31, 0(r3)
0x825A07BC:  addi         r26, r26, 1
0x825A07C0:  addi         r4, r4, 4
0x825A07C4:  fneg         f30, f31
0x825A07C8:  addi         r3, r3, 4
0x825A07CC:  cmplw        cr6, r10, r11
0x825A07D0:  bge          cr6, 0x825a07fc
0x825A07D4:  lwz          r8, 0(r30)
0x825A07D8:  subf         r31, r10, r11
0x825A07DC:  subf         r7, r11, r10
0x825A07E0:  addi         r30, r30, 4
0x825A07E4:  slw          r29, r8, r10
0x825A07E8:  addi         r10, r7, 0x20
0x825A07EC:  or           r9, r29, r9
0x825A07F0:  and          r7, r9, r5
0x825A07F4:  srw          r9, r8, r31
0x825A07F8:  b            0x825a0808
0x825A07FC:  and          r7, r5, r9
0x825A0800:  subf         r10, r11, r10
0x825A0804:  srw          r9, r9, r11
0x825A0808:  cmpwi        cr6, r7, 0
0x825A080C:  beq          cr6, 0x825a0870
0x825A0810:  cmplwi       cr6, r10, 0
0x825A0814:  bne          cr6, 0x825a082c
0x825A0818:  lwz          r8, 0(r30)
0x825A081C:  li           r10, 0x1f
0x825A0820:  addi         r30, r30, 4
0x825A0824:  srwi         r9, r8, 1
0x825A0828:  b            0x825a0838
0x825A082C:  mr           r8, r9
0x825A0830:  srwi         r9, r9, 1
0x825A0834:  addi         r10, r10, -1
0x825A0838:  clrlwi       r8, r8, 0x1f
0x825A083C:  cmplwi       cr6, r8, 0
0x825A0840:  beq          cr6, 0x825a084c
0x825A0844:  fmr          f0, f30
0x825A0848:  b            0x825a0850
0x825A084C:  fmr          f0, f31
0x825A0850:  extsw        r7, r7
0x825A0854:  std          r7, 0x60(r1)
0x825A0858:  lfd          f5, 0x60(r1)
0x825A085C:  fcfid        f4, f5
0x825A0860:  frsp         f3, f4
0x825A0864:  fmuls        f2, f3, f0
0x825A0868:  stfs         f2, 0(r6)
0x825A086C:  b            0x825a0874
0x825A0870:  stfs         f29, 0(r6)
0x825A0874:  lwz          r8, 0(r4)
0x825A0878:  addi         r7, r28, 1
0x825A087C:  addi         r6, r6, 4
0x825A0880:  slwi         r8, r8, 1
0x825A0884:  cmplw        cr6, r7, r8
0x825A0888:  bne          cr6, 0x825a08a0
0x825A088C:  lfs          f31, 0(r3)
0x825A0890:  addi         r26, r26, 1
0x825A0894:  addi         r4, r4, 4
0x825A0898:  fneg         f30, f31
0x825A089C:  addi         r3, r3, 4
0x825A08A0:  cmplw        cr6, r10, r11
0x825A08A4:  bge          cr6, 0x825a08d0
0x825A08A8:  lwz          r8, 0(r30)
0x825A08AC:  subf         r29, r10, r11
0x825A08B0:  subf         r7, r11, r10
0x825A08B4:  addi         r30, r30, 4
0x825A08B8:  addi         r31, r7, 0x20
0x825A08BC:  slw          r10, r8, r10
0x825A08C0:  srw          r29, r8, r29
0x825A08C4:  or           r9, r10, r9
0x825A08C8:  and          r7, r9, r5
0x825A08CC:  b            0x825a08dc
0x825A08D0:  subf         r31, r11, r10
0x825A08D4:  and          r7, r5, r9
0x825A08D8:  srw          r29, r9, r11
0x825A08DC:  cmpwi        cr6, r7, 0
0x825A08E0:  beq          cr6, 0x825a0944
0x825A08E4:  cmplwi       cr6, r31, 0
0x825A08E8:  bne          cr6, 0x825a0900
0x825A08EC:  lwz          r10, 0(r30)
0x825A08F0:  li           r31, 0x1f
0x825A08F4:  addi         r30, r30, 4
0x825A08F8:  srwi         r29, r10, 1
0x825A08FC:  b            0x825a090c
0x825A0900:  mr           r10, r29
0x825A0904:  srwi         r29, r29, 1
0x825A0908:  addi         r31, r31, -1
0x825A090C:  clrlwi       r8, r10, 0x1f
0x825A0910:  cmplwi       cr6, r8, 0
0x825A0914:  beq          cr6, 0x825a0920
0x825A0918:  fmr          f0, f30
0x825A091C:  b            0x825a0924
0x825A0920:  fmr          f0, f31
0x825A0924:  extsw        r7, r7
0x825A0928:  std          r7, 0x68(r1)
0x825A092C:  lfd          f1, 0x68(r1)
0x825A0930:  fcfid        f13, f1
0x825A0934:  frsp         f12, f13
0x825A0938:  fmuls        f11, f12, f0
0x825A093C:  stfs         f11, 0(r6)
0x825A0940:  b            0x825a0948
0x825A0944:  stfs         f29, 0(r6)
0x825A0948:  addi         r23, r23, 4
0x825A094C:  addi         r27, r6, 4
0x825A0950:  addi         r28, r28, 4
0x825A0954:  cmplw        cr6, r23, r25
0x825A0958:  blt          cr6, 0x825a0608
0x825A095C:  cmplw        cr6, r23, r24
0x825A0960:  bge          cr6, 0x825a0a50
0x825A0964:  subfic       r5, r11, 0x20
0x825A0968:  slwi         r10, r26, 2
0x825A096C:  subf         r6, r21, r22
0x825A0970:  add          r7, r10, r21
0x825A0974:  srw          r8, r20, r5
0x825A0978:  lwzx         r4, r6, r7
0x825A097C:  slwi         r3, r4, 1
0x825A0980:  cmplw        cr6, r23, r3
0x825A0984:  bne          cr6, 0x825a0998
0x825A0988:  lfs          f31, 0(r7)
0x825A098C:  addi         r26, r26, 1
0x825A0990:  addi         r7, r7, 4
0x825A0994:  fneg         f30, f31
0x825A0998:  cmplw        cr6, r31, r11
0x825A099C:  bge          cr6, 0x825a09c8
0x825A09A0:  lwz          r10, 0(r30)
0x825A09A4:  subf         r5, r31, r11
0x825A09A8:  subf         r9, r11, r31
0x825A09AC:  addi         r30, r30, 4
0x825A09B0:  slw          r4, r10, r31
0x825A09B4:  addi         r31, r9, 0x20
0x825A09B8:  or           r3, r4, r29
0x825A09BC:  srw          r29, r10, r5
0x825A09C0:  and          r9, r3, r8
0x825A09C4:  b            0x825a09d4
0x825A09C8:  and          r9, r8, r29
0x825A09CC:  subf         r31, r11, r31
0x825A09D0:  srw          r29, r29, r11
0x825A09D4:  cmpwi        cr6, r9, 0
0x825A09D8:  beq          cr6, 0x825a0a3c
0x825A09DC:  cmplwi       cr6, r31, 0
0x825A09E0:  bne          cr6, 0x825a09f8
0x825A09E4:  lwz          r10, 0(r30)
0x825A09E8:  li           r31, 0x1f
0x825A09EC:  addi         r30, r30, 4
0x825A09F0:  srwi         r29, r10, 1
0x825A09F4:  b            0x825a0a04
0x825A09F8:  mr           r10, r29
0x825A09FC:  srwi         r29, r29, 1
0x825A0A00:  addi         r31, r31, -1
0x825A0A04:  clrlwi       r10, r10, 0x1f
0x825A0A08:  cmplwi       cr6, r10, 0
0x825A0A0C:  beq          cr6, 0x825a0a18
0x825A0A10:  fmr          f0, f30
0x825A0A14:  b            0x825a0a1c
0x825A0A18:  fmr          f0, f31
0x825A0A1C:  extsw        r9, r9
0x825A0A20:  std          r9, 0x70(r1)
0x825A0A24:  lfd          f10, 0x70(r1)
0x825A0A28:  fcfid        f9, f10
0x825A0A2C:  frsp         f8, f9
0x825A0A30:  fmuls        f7, f8, f0
0x825A0A34:  stfs         f7, 0(r27)
0x825A0A38:  b            0x825a0a40
0x825A0A3C:  stfs         f29, 0(r27)
0x825A0A40:  addi         r23, r23, 1
0x825A0A44:  addi         r27, r27, 4
0x825A0A48:  cmplw        cr6, r23, r24
0x825A0A4C:  blt          cr6, 0x825a0978
0x825A0A50:  cmplw        cr6, r23, r18
0x825A0A54:  blt          cr6, 0x825a04b0
0x825A0A58:  stw          r30, 4(r19)
0x825A0A5C:  stw          r29, 0(r19)
0x825A0A60:  stw          r31, 8(r19)
0x825A0A64:  addi         r1, r1, 0x110
0x825A0A68:  lfd          f29, -0x98(r1)
0x825A0A6C:  lfd          f30, -0x90(r1)
0x825A0A70:  lfd          f31, -0x88(r1)
0x825A0A74:  b            0x8242f8bc
0x825A0A78:  mflr         r12
0x825A0A7C:  bl           0x8242f878
0x825A0A80:  stfd         f31, -0x70(r1)
0x825A0A84:  ld           r12, -0x1000(r1)
0x825A0A88:  ld           r12, -0x2000(r1)
0x825A0A8C:  ld           r12, -0x3000(r1)
0x825A0A90:  ld           r12, -0x4000(r1)
0x825A0A94:  stwu         r1, -0x4150(r1)
0x825A0A98:  mr           r21, r5
0x825A0A9C:  fmr          f31, f1
0x825A0AA0:  mr           r20, r9
0x825A0AA4:  mr           r5, r10
0x825A0AA8:  mr           r9, r10
0x825A0AAC:  clrlwi       r25, r6, 0x1f
0x825A0AB0:  mr           r26, r8
0x825A0AB4:  mr           r29, r3
0x825A0AB8:  mr           r27, r7
0x825A0ABC:  stw          r5, 0x5c(r1)
0x825A0AC0:  stw          r9, 0x54(r1)
0x825A0AC4:  li           r11, 0
0x825A0AC8:  li           r8, 0
0x825A0ACC:  cmplwi       cr6, r25, 0
0x825A0AD0:  beq          cr6, 0x825a0ae8
0x825A0AD4:  lwz          r9, 0(r10)
0x825A0AD8:  li           r11, 0x1e
0x825A0ADC:  srwi         r8, r9, 2
0x825A0AE0:  addi         r9, r10, 4
0x825A0AE4:  stw          r9, 0x54(r1)
0x825A0AE8:  addi         r30, r1, 0xe0
0x825A0AEC:  cmplwi       cr6, r21, 0
0x825A0AF0:  beq          cr6, 0x825a0e80
0x825A0AF4:  lis          r10, -0x7d8c
0x825A0AF8:  lwz          r23, 0x41ac(r1)
0x825A0AFC:  lwz          r28, 0x41a4(r1)
0x825A0B00:  slwi         r22, r29, 2
0x825A0B04:  mr           r24, r21
0x825A0B08:  addi         r31, r10, -0x25a0
0x825A0B0C:  b            0x825a0b18
0x825A0B10:  lwz          r11, 0x58(r1)
0x825A0B14:  lwz          r8, 0x50(r1)
0x825A0B18:  cmplwi       cr6, r11, 0x1d
0x825A0B1C:  bge          cr6, 0x825a0b48
0x825A0B20:  lwz          r10, 0(r9)
0x825A0B24:  subfic       r6, r11, 0x1d
0x825A0B28:  addi         r9, r9, 4
0x825A0B2C:  stw          r9, 0x54(r1)
0x825A0B30:  slw          r5, r10, r11
0x825A0B34:  addi         r11, r11, 3
0x825A0B38:  or           r4, r5, r8
0x825A0B3C:  srw          r10, r10, r6
0x825A0B40:  clrlwi       r7, r4, 3
0x825A0B44:  b            0x825a0b54
0x825A0B48:  clrlwi       r7, r8, 3
0x825A0B4C:  srwi         r10, r8, 0x1d
0x825A0B50:  addi         r11, r11, -0x1d
0x825A0B54:  srwi         r8, r7, 5
0x825A0B58:  rlwinm       r6, r7, 2, 0x19, 0x1d
0x825A0B5C:  rlwinm       r8, r8, 0, 9, 7
0x825A0B60:  rlwinm       r5, r7, 0, 3, 3
0x825A0B64:  clrldi       r8, r8, 0x20
0x825A0B68:  cmplwi       cr6, r5, 0
0x825A0B6C:  lfsx         f0, r6, r31
0x825A0B70:  std          r8, 0x60(r1)
0x825A0B74:  lfd          f13, 0x60(r1)
0x825A0B78:  fcfid        f12, f13
0x825A0B7C:  frsp         f11, f12
0x825A0B80:  fmuls        f0, f11, f0
0x825A0B84:  beq          cr6, 0x825a0b8c
0x825A0B88:  fneg         f0, f0
0x825A0B8C:  stfs         f0, 0(r30)
0x825A0B90:  cmplwi       cr6, r11, 0x1d
0x825A0B94:  bge          cr6, 0x825a0bc0
0x825A0B98:  lwz          r8, 0(r9)
0x825A0B9C:  subfic       r4, r11, 0x1d
0x825A0BA0:  addi         r9, r9, 4
0x825A0BA4:  stw          r9, 0x54(r1)
0x825A0BA8:  slw          r3, r8, r11
0x825A0BAC:  addi         r11, r11, 3
0x825A0BB0:  or           r10, r3, r10
0x825A0BB4:  clrlwi       r7, r10, 3
0x825A0BB8:  srw          r10, r8, r4
0x825A0BBC:  b            0x825a0bcc
0x825A0BC0:  clrlwi       r7, r10, 3
0x825A0BC4:  srwi         r10, r10, 0x1d
0x825A0BC8:  addi         r11, r11, -0x1d
0x825A0BCC:  srwi         r6, r7, 5
0x825A0BD0:  stw          r11, 0x58(r1)
0x825A0BD4:  rlwinm       r5, r7, 2, 0x19, 0x1d
0x825A0BD8:  stw          r10, 0x50(r1)
0x825A0BDC:  rlwinm       r6, r6, 0, 9, 7
0x825A0BE0:  rlwinm       r4, r7, 0, 3, 3
0x825A0BE4:  clrldi       r6, r6, 0x20
0x825A0BE8:  cmplwi       cr6, r4, 0
0x825A0BEC:  lfsx         f10, r5, r31
0x825A0BF0:  std          r6, 0x68(r1)
0x825A0BF4:  lfd          f9, 0x68(r1)
0x825A0BF8:  fcfid        f8, f9
0x825A0BFC:  frsp         f7, f8
0x825A0C00:  fmuls        f0, f7, f10
0x825A0C04:  beq          cr6, 0x825a0c0c
0x825A0C08:  fneg         f0, f0
0x825A0C0C:  stfs         f0, 4(r30)
0x825A0C10:  li           r4, 0
0x825A0C14:  cmpwi        cr6, r28, 4
0x825A0C18:  blt          cr6, 0x825a0d9c
0x825A0C1C:  addi         r3, r28, -4
0x825A0C20:  addi         r6, r1, 0x74
0x825A0C24:  srwi         r8, r3, 2
0x825A0C28:  addi         r5, r8, 1
0x825A0C2C:  slwi         r4, r5, 2
0x825A0C30:  cmplwi       cr6, r11, 8
0x825A0C34:  bge          cr6, 0x825a0c5c
0x825A0C38:  lwz          r8, 0(r9)
0x825A0C3C:  subfic       r3, r11, 8
0x825A0C40:  addi         r9, r9, 4
0x825A0C44:  slw          r7, r8, r11
0x825A0C48:  addi         r11, r11, 0x18
0x825A0C4C:  or           r10, r7, r10
0x825A0C50:  clrlwi       r7, r10, 0x18
0x825A0C54:  srw          r10, r8, r3
0x825A0C58:  b            0x825a0c68
0x825A0C5C:  clrlwi       r7, r10, 0x18
0x825A0C60:  srwi         r10, r10, 8
0x825A0C64:  addi         r11, r11, -8
0x825A0C68:  cmplwi       cr6, r7, 0x5f
0x825A0C6C:  ble          cr6, 0x825a0c74
0x825A0C70:  li           r7, 0x5f
0x825A0C74:  addi         r8, r31, -0x180
0x825A0C78:  slwi         r7, r7, 2
0x825A0C7C:  cmplwi       cr6, r11, 8
0x825A0C80:  lfsx         f6, r7, r8
0x825A0C84:  stfs         f6, -4(r6)
0x825A0C88:  bge          cr6, 0x825a0cb0
0x825A0C8C:  lwz          r8, 0(r9)
0x825A0C90:  subfic       r3, r11, 8
0x825A0C94:  addi         r9, r9, 4
0x825A0C98:  slw          r7, r8, r11
0x825A0C9C:  addi         r11, r11, 0x18
0x825A0CA0:  or           r10, r7, r10
0x825A0CA4:  clrlwi       r7, r10, 0x18
0x825A0CA8:  srw          r10, r8, r3
0x825A0CAC:  b            0x825a0cbc
0x825A0CB0:  clrlwi       r7, r10, 0x18
0x825A0CB4:  srwi         r10, r10, 8
0x825A0CB8:  addi         r11, r11, -8
0x825A0CBC:  cmplwi       cr6, r7, 0x5f
0x825A0CC0:  ble          cr6, 0x825a0cc8
0x825A0CC4:  li           r7, 0x5f
0x825A0CC8:  addi         r8, r31, -0x180
0x825A0CCC:  slwi         r7, r7, 2
0x825A0CD0:  cmplwi       cr6, r11, 8
0x825A0CD4:  lfsx         f5, r7, r8
0x825A0CD8:  stfs         f5, 0(r6)
0x825A0CDC:  bge          cr6, 0x825a0d04
0x825A0CE0:  lwz          r8, 0(r9)
0x825A0CE4:  subfic       r3, r11, 8
0x825A0CE8:  addi         r9, r9, 4
0x825A0CEC:  slw          r7, r8, r11
0x825A0CF0:  addi         r11, r11, 0x18
0x825A0CF4:  or           r10, r7, r10
0x825A0CF8:  clrlwi       r7, r10, 0x18
0x825A0CFC:  srw          r10, r8, r3
0x825A0D00:  b            0x825a0d10
0x825A0D04:  clrlwi       r7, r10, 0x18
0x825A0D08:  srwi         r10, r10, 8
0x825A0D0C:  addi         r11, r11, -8
0x825A0D10:  cmplwi       cr6, r7, 0x5f
0x825A0D14:  ble          cr6, 0x825a0d1c
0x825A0D18:  li           r7, 0x5f
0x825A0D1C:  addi         r8, r31, -0x180
0x825A0D20:  slwi         r7, r7, 2
0x825A0D24:  cmplwi       cr6, r11, 8
0x825A0D28:  lfsx         f4, r7, r8
0x825A0D2C:  stfs         f4, 4(r6)
0x825A0D30:  bge          cr6, 0x825a0d58
0x825A0D34:  lwz          r8, 0(r9)
0x825A0D38:  subfic       r3, r11, 8
0x825A0D3C:  addi         r9, r9, 4
0x825A0D40:  slw          r7, r8, r11
0x825A0D44:  addi         r11, r11, 0x18
0x825A0D48:  or           r10, r7, r10
0x825A0D4C:  clrlwi       r7, r10, 0x18
0x825A0D50:  srw          r10, r8, r3
0x825A0D54:  b            0x825a0d64
0x825A0D58:  clrlwi       r7, r10, 0x18
0x825A0D5C:  srwi         r10, r10, 8
0x825A0D60:  addi         r11, r11, -8
0x825A0D64:  cmplwi       cr6, r7, 0x5f
0x825A0D68:  ble          cr6, 0x825a0d70
0x825A0D6C:  li           r7, 0x5f
0x825A0D70:  addi         r8, r31, -0x180
0x825A0D74:  slwi         r7, r7, 2
0x825A0D78:  addi         r5, r5, -1
0x825A0D7C:  cmplwi       cr6, r5, 0
0x825A0D80:  lfsx         f3, r7, r8
0x825A0D84:  stfs         f3, 8(r6)
0x825A0D88:  addi         r6, r6, 0x10
0x825A0D8C:  bne          cr6, 0x825a0c30
0x825A0D90:  stw          r10, 0x50(r1)
0x825A0D94:  stw          r11, 0x58(r1)
0x825A0D98:  stw          r9, 0x54(r1)
0x825A0D9C:  cmplw        cr6, r4, r28
0x825A0DA0:  bge          cr6, 0x825a0e24
0x825A0DA4:  slwi         r8, r4, 2
0x825A0DA8:  addi         r7, r1, 0x70
0x825A0DAC:  subf         r6, r4, r28
0x825A0DB0:  add          r5, r8, r7
0x825A0DB4:  cmplwi       cr6, r11, 8
0x825A0DB8:  bge          cr6, 0x825a0de0
0x825A0DBC:  lwz          r7, 0(r9)
0x825A0DC0:  subfic       r4, r11, 8
0x825A0DC4:  addi         r9, r9, 4
0x825A0DC8:  slw          r3, r7, r11
0x825A0DCC:  addi         r11, r11, 0x18
0x825A0DD0:  or           r10, r3, r10
0x825A0DD4:  clrlwi       r8, r10, 0x18
0x825A0DD8:  srw          r10, r7, r4
0x825A0DDC:  b            0x825a0dec
0x825A0DE0:  clrlwi       r8, r10, 0x18
0x825A0DE4:  srwi         r10, r10, 8
0x825A0DE8:  addi         r11, r11, -8
0x825A0DEC:  cmplwi       cr6, r8, 0x5f
0x825A0DF0:  ble          cr6, 0x825a0df8
0x825A0DF4:  li           r8, 0x5f
0x825A0DF8:  addi         r7, r31, -0x180
0x825A0DFC:  slwi         r4, r8, 2
0x825A0E00:  addi         r6, r6, -1
0x825A0E04:  cmplwi       cr6, r6, 0
0x825A0E08:  lfsx         f2, r4, r7
0x825A0E0C:  stfs         f2, 0(r5)
0x825A0E10:  addi         r5, r5, 4
0x825A0E14:  bne          cr6, 0x825a0db4
0x825A0E18:  stw          r10, 0x50(r1)
0x825A0E1C:  stw          r11, 0x58(r1)
0x825A0E20:  stw          r9, 0x54(r1)
0x825A0E24:  mr           r7, r23
0x825A0E28:  addi         r6, r1, 0x70
0x825A0E2C:  addi         r5, r1, 0x50
0x825A0E30:  mr           r4, r29
0x825A0E34:  mr           r3, r30
0x825A0E38:  bl           0x825a0418
0x825A0E3C:  cmplwi       cr6, r25, 0
0x825A0E40:  mr           r7, r26
0x825A0E44:  mr           r6, r27
0x825A0E48:  mr           r5, r30
0x825A0E4C:  mr           r3, r29
0x825A0E50:  beq          cr6, 0x825a0e60
0x825A0E54:  li           r4, 1
0x825A0E58:  bl           0x825aa1c8
0x825A0E5C:  b            0x825a0e68
0x825A0E60:  li           r4, -1
0x825A0E64:  bl           0x825aa068
0x825A0E68:  addi         r24, r24, -1
0x825A0E6C:  lwz          r9, 0x54(r1)
0x825A0E70:  add          r30, r22, r30
0x825A0E74:  cmplwi       cr6, r24, 0
0x825A0E78:  bne          cr6, 0x825a0b10
0x825A0E7C:  lwz          r5, 0x5c(r1)
0x825A0E80:  cmplwi       cr6, r21, 1
0x825A0E84:  addi         r8, r1, 0xe0
0x825A0E88:  bne          cr6, 0x825a0f04
0x825A0E8C:  mr           r7, r20
0x825A0E90:  mr           r10, r29
0x825A0E94:  cmplwi       cr6, r29, 0
0x825A0E98:  beq          cr6, 0x825a0fd8
0x825A0E9C:  lfs          f1, 0(r8)
0x825A0EA0:  addi         r3, r1, 0x60
0x825A0EA4:  fmuls        f0, f1, f31
0x825A0EA8:  fctiwz       f13, f0
0x825A0EAC:  stfiwx       f13, 0, r3
0x825A0EB0:  lwz          r11, 0x60(r1)
0x825A0EB4:  cmpwi        cr6, r11, 0x7fff
0x825A0EB8:  blt          cr6, 0x825a0ec4
0x825A0EBC:  li           r11, 0x7fff
0x825A0EC0:  b            0x825a0ed8
0x825A0EC4:  cmpwi        cr6, r11, -0x8000
0x825A0EC8:  bgt          cr6, 0x825a0ed4
0x825A0ECC:  li           r11, -0x8000
0x825A0ED0:  b            0x825a0ed8
0x825A0ED4:  extsh        r11, r11
0x825A0ED8:  addi         r10, r10, -1
0x825A0EDC:  sth          r11, 0(r7)
0x825A0EE0:  addi         r8, r8, 4
0x825A0EE4:  addi         r7, r7, 2
0x825A0EE8:  cmplwi       cr6, r10, 0
0x825A0EEC:  bne          cr6, 0x825a0e9c
0x825A0EF0:  subf         r5, r5, r9
0x825A0EF4:  clrlwi       r3, r5, 3
0x825A0EF8:  addi         r1, r1, 0x4150
0x825A0EFC:  lfd          f31, -0x70(r1)
0x825A0F00:  b            0x8242f8c8
0x825A0F04:  mr           r10, r20
0x825A0F08:  mr           r7, r29
0x825A0F0C:  cmplwi       cr6, r29, 0
0x825A0F10:  beq          cr6, 0x825a0fc4
0x825A0F14:  slwi         r6, r29, 2
0x825A0F18:  lfs          f12, 0(r8)
0x825A0F1C:  addi         r4, r1, 0x60
0x825A0F20:  fmuls        f11, f12, f31
0x825A0F24:  addi         r7, r7, -1
0x825A0F28:  fctiwz       f10, f11
0x825A0F2C:  stfiwx       f10, 0, r4
0x825A0F30:  lwz          r11, 0x60(r1)
0x825A0F34:  cmpwi        cr6, r11, 0x7fff
0x825A0F38:  blt          cr6, 0x825a0f44
0x825A0F3C:  li           r11, 0x7fff
0x825A0F40:  b            0x825a0f58
0x825A0F44:  cmpwi        cr6, r11, -0x8000
0x825A0F48:  bgt          cr6, 0x825a0f54
0x825A0F4C:  li           r11, -0x8000
0x825A0F50:  b            0x825a0f58
0x825A0F54:  extsh        r11, r11
0x825A0F58:  lfsx         f9, r6, r8
0x825A0F5C:  addi         r4, r1, 0x60
0x825A0F60:  fmuls        f8, f9, f31
0x825A0F64:  sth          r11, 0(r10)
0x825A0F68:  addi         r10, r10, 2
0x825A0F6C:  fctiwz       f7, f8
0x825A0F70:  stfiwx       f7, 0, r4
0x825A0F74:  lwz          r11, 0x60(r1)
0x825A0F78:  cmpwi        cr6, r11, 0x7fff
0x825A0F7C:  blt          cr6, 0x825a0f88
0x825A0F80:  li           r11, 0x7fff
0x825A0F84:  b            0x825a0f9c
0x825A0F88:  cmpwi        cr6, r11, -0x8000
0x825A0F8C:  bgt          cr6, 0x825a0f98
0x825A0F90:  li           r11, -0x8000
0x825A0F94:  b            0x825a0f9c
0x825A0F98:  extsh        r11, r11
0x825A0F9C:  sth          r11, 0(r10)
0x825A0FA0:  addi         r8, r8, 4
0x825A0FA4:  cmplwi       cr6, r7, 0
0x825A0FA8:  addi         r10, r10, 2
0x825A0FAC:  bne          cr6, 0x825a0f18
0x825A0FB0:  subf         r10, r5, r9
0x825A0FB4:  clrlwi       r3, r10, 3
0x825A0FB8:  addi         r1, r1, 0x4150
0x825A0FBC:  lfd          f31, -0x70(r1)
0x825A0FC0:  b            0x8242f8c8
0x825A0FC4:  subf         r9, r5, r9
0x825A0FC8:  clrlwi       r3, r9, 3
0x825A0FCC:  addi         r1, r1, 0x4150
0x825A0FD0:  lfd          f31, -0x70(r1)
0x825A0FD4:  b            0x8242f8c8
0x825A0FD8:  subf         r8, r5, r9
0x825A0FDC:  clrlwi       r3, r8, 3
0x825A0FE0:  addi         r1, r1, 0x4150
0x825A0FE4:  lfd          f31, -0x70(r1)
0x825A0FE8:  b            0x8242f8c8
0x825A0FEC:  .align 0                       # padding
0x825A0FF0:  mflr         r12
0x825A0FF4:  bl           0x8242f880
0x825A0FF8:  stfd         f29, -0x70(r1)
0x825A0FFC:  stfd         f30, -0x68(r1)
0x825A1000:  stfd         f31, -0x60(r1)
0x825A1004:  stwu         r1, -0xe0(r1)
0x825A1008:  mr           r26, r4
0x825A100C:  mr           r23, r5
0x825A1010:  cmplwi       cr6, r3, 0xac44
0x825A1014:  blt          cr6, 0x825a1020
0x825A1018:  li           r30, 0x800
0x825A101C:  b            0x825a1030
0x825A1020:  cmplwi       cr6, r3, 0x5622
0x825A1024:  li           r30, 0x400
0x825A1028:  bge          cr6, 0x825a1030
0x825A102C:  li           r30, 0x200
0x825A1030:  mullw        r11, r30, r26
0x825A1034:  clrlwi       r28, r23, 0x1f
0x825A1038:  slwi         r25, r11, 1
0x825A103C:  li           r22, 1
0x825A1040:  cmplwi       cr6, r28, 0
0x825A1044:  bne          cr6, 0x825a1054
0x825A1048:  mullw        r3, r3, r26
0x825A104C:  mr           r30, r11
0x825A1050:  mr           r26, r22
0x825A1054:  lis          r11, -0x7d8c
0x825A1058:  srwi         r27, r30, 1
0x825A105C:  addi         r24, r11, -0x2788
0x825A1060:  addi         r11, r3, 1
0x825A1064:  li           r31, 0
0x825A1068:  srwi         r29, r11, 1
0x825A106C:  addi         r11, r24, 4
0x825A1070:  lwz          r10, -4(r11)
0x825A1074:  cmplw        cr6, r10, r29
0x825A1078:  bge          cr6, 0x825a10dc
0x825A107C:  lwz          r9, 0(r11)
0x825A1080:  cmplw        cr6, r9, r29
0x825A1084:  bge          cr6, 0x825a10c0
0x825A1088:  lwz          r8, 4(r11)
0x825A108C:  cmplw        cr6, r8, r29
0x825A1090:  bge          cr6, 0x825a10c8
0x825A1094:  lwz          r7, 8(r11)
0x825A1098:  cmplw        cr6, r7, r29
0x825A109C:  bge          cr6, 0x825a10d0
0x825A10A0:  lwz          r6, 0xc(r11)
0x825A10A4:  cmplw        cr6, r6, r29
0x825A10A8:  bge          cr6, 0x825a10d8
0x825A10AC:  addi         r31, r31, 5
0x825A10B0:  addi         r11, r11, 0x14
0x825A10B4:  cmplwi       cr6, r31, 0x19
0x825A10B8:  blt          cr6, 0x825a1070
0x825A10BC:  b            0x825a10dc
0x825A10C0:  addi         r31, r31, 1
0x825A10C4:  b            0x825a10dc
0x825A10C8:  addi         r31, r31, 2
0x825A10CC:  b            0x825a10dc
0x825A10D0:  addi         r31, r31, 3
0x825A10D4:  b            0x825a10dc
0x825A10D8:  addi         r31, r31, 4
0x825A10DC:  addi         r5, r31, 1
0x825A10E0:  addi         r3, r1, 0x5c
0x825A10E4:  slwi         r4, r5, 2
0x825A10E8:  bl           0x8259f7b0
0x825A10EC:  clrldi       r4, r27, 0x20
0x825A10F0:  lis          r11, -0x7e00
0x825A10F4:  lis          r10, -0x7df8
0x825A10F8:  std          r4, 0x50(r1)
0x825A10FC:  lfs          f29, 0x3da8(r11)
0x825A1100:  lis          r11, -0x7df8
0x825A1104:  lfd          f30, -0x6250(r10)
0x825A1108:  lfd          f31, -0x62e0(r11)
0x825A110C:  lfd          f0, 0x50(r1)
0x825A1110:  fcfid        f13, f0
0x825A1114:  frsp         f0, f13
0x825A1118:  fcmpu        cr6, f0, f29
0x825A111C:  ble          cr6, 0x825a115c
0x825A1120:  frsqrte      f13, f0
0x825A1124:  fmul         f12, f0, f13
0x825A1128:  fnmsub       f11, f12, f13, f31
0x825A112C:  fmul         f10, f11, f13
0x825A1130:  fmul         f13, f10, f30
0x825A1134:  fmul         f9, f0, f13
0x825A1138:  fnmsub       f8, f9, f13, f31
0x825A113C:  fmul         f7, f8, f13
0x825A1140:  fmul         f13, f7, f30
0x825A1144:  fmul         f6, f0, f13
0x825A1148:  fnmsub       f5, f6, f13, f31
0x825A114C:  fmul         f4, f5, f13
0x825A1150:  fmul         f3, f4, f30
0x825A1154:  fmul         f2, f3, f0
0x825A1158:  frsp         f0, f2
0x825A115C:  addi         r11, r1, 0x50
0x825A1160:  fctiwz       f1, f0
0x825A1164:  addi         r3, r1, 0x60
0x825A1168:  stfiwx       f1, 0, r11
0x825A116C:  lwz          r11, 0x50(r1)
0x825A1170:  addi         r10, r11, 2
0x825A1174:  slwi         r4, r10, 2
0x825A1178:  bl           0x8259f7b0
0x825A117C:  cmplwi       cr6, r28, 0
0x825A1180:  addi         r3, r1, 0x58
0x825A1184:  beq          cr6, 0x825a1198
0x825A1188:  slwi         r11, r30, 2
0x825A118C:  add          r9, r30, r11
0x825A1190:  clrlwi       r4, r9, 2
0x825A1194:  b            0x825a11a0
0x825A1198:  slwi         r11, r27, 2
0x825A119C:  addi         r4, r11, -1
0x825A11A0:  bl           0x8259f7b0
0x825A11A4:  srwi         r4, r25, 1
0x825A11A8:  addi         r3, r1, 0x64
0x825A11AC:  bl           0x8259f7b0
0x825A11B0:  mr           r4, r25
0x825A11B4:  addi         r3, r1, 0x68
0x825A11B8:  bl           0x8259f7b0
0x825A11BC:  li           r3, 0x34
0x825A11C0:  bl           0x8259f8a8
0x825A11C4:  cmplwi       cr6, r3, 0
0x825A11C8:  bne          cr6, 0x825a11e0
0x825A11CC:  addi         r1, r1, 0xe0
0x825A11D0:  lfd          f29, -0x70(r1)
0x825A11D4:  lfd          f30, -0x68(r1)
0x825A11D8:  lfd          f31, -0x60(r1)
0x825A11DC:  b            0x8242f8d0
0x825A11E0:  mr           r11, r3
0x825A11E4:  li           r9, 0
0x825A11E8:  li           r10, 0xd
0x825A11EC:  mtctr        r10
0x825A11F0:  stw          r9, 0(r11)
0x825A11F4:  addi         r11, r11, 4
0x825A11F8:  bdnz         0x825a11f0
0x825A11FC:  clrldi       r8, r30, 0x20
0x825A1200:  lwz          r7, 0x5c(r1)
0x825A1204:  srwi         r6, r25, 4
0x825A1208:  std          r8, 0x50(r1)
0x825A120C:  stw          r7, 0x20(r3)
0x825A1210:  lwz          r5, 0x60(r1)
0x825A1214:  stw          r5, 0x24(r3)
0x825A1218:  lwz          r4, 0x58(r1)
0x825A121C:  stw          r4, 0x28(r3)
0x825A1220:  lwz          r11, 0x64(r1)
0x825A1224:  stw          r11, 0x2c(r3)
0x825A1228:  lwz          r10, 0x68(r1)
0x825A122C:  stw          r23, 0x30(r3)
0x825A1230:  stw          r26, 0x10(r3)
0x825A1234:  stw          r31, 0x1c(r3)
0x825A1238:  stw          r30, 0(r3)
0x825A123C:  stw          r10, 0x14(r3)
0x825A1240:  stw          r25, 8(r3)
0x825A1244:  stw          r6, 0xc(r3)
0x825A1248:  lfd          f0, 0x50(r1)
0x825A124C:  fcfid        f13, f0
0x825A1250:  frsp         f0, f13
0x825A1254:  fcmpu        cr6, f0, f29
0x825A1258:  ble          cr6, 0x825a1298
0x825A125C:  frsqrte      f13, f0
0x825A1260:  fmul         f12, f0, f13
0x825A1264:  fnmsub       f11, f12, f13, f31
0x825A1268:  fmul         f10, f11, f13
0x825A126C:  fmul         f13, f10, f30
0x825A1270:  fmul         f9, f0, f13
0x825A1274:  fnmsub       f8, f9, f13, f31
0x825A1278:  fmul         f7, f8, f13
0x825A127C:  fmul         f13, f7, f30
0x825A1280:  fmul         f6, f0, f13
0x825A1284:  fnmsub       f5, f6, f13, f31
0x825A1288:  fmul         f4, f5, f13
0x825A128C:  fmul         f3, f4, f30
0x825A1290:  fmul         f2, f3, f0
0x825A1294:  frsp         f0, f2
0x825A1298:  lis          r11, -0x7e00
0x825A129C:  li           r8, 0
0x825A12A0:  cmplwi       cr6, r31, 0
0x825A12A4:  lfs          f13, 0x3eb8(r11)
0x825A12A8:  fdivs        f1, f13, f0
0x825A12AC:  stfs         f1, 4(r3)
0x825A12B0:  beq          cr6, 0x825a12f8
0x825A12B4:  li           r11, 0
0x825A12B8:  mr           r9, r31
0x825A12BC:  mr           r8, r31
0x825A12C0:  lwzx         r6, r11, r24
0x825A12C4:  lwz          r7, 0x20(r3)
0x825A12C8:  mullw        r5, r6, r27
0x825A12CC:  divwu        r4, r5, r29
0x825A12D0:  stwx         r4, r11, r7
0x825A12D4:  lwz          r10, 0x20(r3)
0x825A12D8:  lwzx         r7, r11, r10
0x825A12DC:  cmplwi       cr6, r7, 0
0x825A12E0:  bne          cr6, 0x825a12e8
0x825A12E4:  stwx         r22, r11, r10
0x825A12E8:  addi         r9, r9, -1
0x825A12EC:  addi         r11, r11, 4
0x825A12F0:  cmplwi       cr6, r9, 0
0x825A12F4:  bne          cr6, 0x825a12c0
0x825A12F8:  lwz          r6, 0x20(r3)
0x825A12FC:  slwi         r5, r8, 2
0x825A1300:  li           r11, 0
0x825A1304:  stwx         r27, r5, r6
0x825A1308:  lwz          r4, 0x24(r3)
0x825A130C:  stw          r11, 0(r4)
0x825A1310:  stw          r22, 0x18(r3)
0x825A1314:  addi         r1, r1, 0xe0
0x825A1318:  lfd          f29, -0x70(r1)
0x825A131C:  lfd          f30, -0x68(r1)
0x825A1320:  lfd          f31, -0x60(r1)
0x825A1324:  b            0x8242f8d0
0x825A1328:  mflr         r12
0x825A132C:  bl           0x8242f890
0x825A1330:  stwu         r1, -0xa0(r1)
0x825A1334:  mr           r31, r3
0x825A1338:  mr           r29, r6
0x825A133C:  mr           r30, r5
0x825A1340:  mr           r10, r29
0x825A1344:  mr           r27, r7
0x825A1348:  lwz          r9, 0x1c(r31)
0x825A134C:  lfs          f1, 4(r31)
0x825A1350:  lwz          r11, 0x20(r31)
0x825A1354:  mr           r28, r4
0x825A1358:  lwz          r8, 0x28(r31)
0x825A135C:  lwz          r7, 0x24(r31)
0x825A1360:  lwz          r6, 0x30(r31)
0x825A1364:  stw          r9, 0x54(r1)
0x825A1368:  lwz          r5, 0x10(r31)
0x825A136C:  lwz          r3, 0(r31)
0x825A1370:  lwz          r9, 0x14(r31)
0x825A1374:  stw          r11, 0x5c(r1)
0x825A1378:  bl           0x825a0a78
0x825A137C:  lwz          r8, 0x18(r31)
0x825A1380:  mr           r26, r3
0x825A1384:  cmpwi        cr6, r8, 0
0x825A1388:  beq          cr6, 0x825a1398
0x825A138C:  li           r7, 0
0x825A1390:  stw          r7, 0x18(r31)
0x825A1394:  b            0x825a13f4
0x825A1398:  lwz          r6, 0xc(r31)
0x825A139C:  li           r11, 0
0x825A13A0:  srwi         r9, r6, 1
0x825A13A4:  cmplwi       cr6, r9, 0
0x825A13A8:  beq          cr6, 0x825a13f4
0x825A13AC:  li           r10, 0
0x825A13B0:  lwz          r4, 0x2c(r31)
0x825A13B4:  subf         r5, r11, r9
0x825A13B8:  lwz          r8, 0x14(r31)
0x825A13BC:  add          r8, r8, r10
0x825A13C0:  lhzx         r3, r4, r10
0x825A13C4:  addi         r10, r10, 2
0x825A13C8:  extsh        r7, r3
0x825A13CC:  lhz          r6, 0(r8)
0x825A13D0:  mullw        r7, r7, r5
0x825A13D4:  extsh        r5, r6
0x825A13D8:  mullw        r6, r5, r11
0x825A13DC:  add          r4, r7, r6
0x825A13E0:  addi         r11, r11, 1
0x825A13E4:  divwu        r3, r4, r9
0x825A13E8:  cmplw        cr6, r11, r9
0x825A13EC:  sth          r3, 0(r8)
0x825A13F0:  blt          cr6, 0x825a13b0
0x825A13F4:  lwz          r6, 0x14(r31)
0x825A13F8:  lwz          r5, 0xc(r31)
0x825A13FC:  lwz          r10, 8(r31)
0x825A1400:  subf         r11, r5, r6
0x825A1404:  lwz          r3, 0x2c(r31)
0x825A1408:  add          r4, r11, r10
0x825A140C:  bl           0x82434100
0x825A1410:  cmplwi       cr6, r30, 0
0x825A1414:  beq          cr6, 0x825a1428
0x825A1418:  lwz          r5, 8(r31)
0x825A141C:  lwz          r4, 0xc(r31)
0x825A1420:  subf         r3, r4, r5
0x825A1424:  stw          r3, 0(r30)
0x825A1428:  cmplwi       cr6, r28, 0
0x825A142C:  beq          cr6, 0x825a1438
0x825A1430:  lwz          r11, 0x14(r31)
0x825A1434:  stw          r11, 0(r28)
0x825A1438:  cmplwi       cr6, r27, 0
0x825A143C:  beq          cr6, 0x825a1448
0x825A1440:  add          r10, r26, r29
0x825A1444:  stw          r10, 0(r27)
0x825A1448:  addi         r1, r1, 0xa0
0x825A144C:  b            0x8242f8e0
0x825A1450:  b            0x8257b028
0x825A1454:  .align 0                       # padding
0x825A1458:  mr           r10, r3
0x825A145C:  mr           r11, r7
0x825A1460:  li           r3, 0
0x825A1464:  clrlwi       r9, r10, 0x1f
0x825A1468:  addi         r3, r3, 1
0x825A146C:  cmplwi       cr6, r9, 0
0x825A1470:  beq          cr6, 0x825a1488
0x825A1474:  lbz          r8, 0(r6)
0x825A1478:  addi         r11, r11, -1
0x825A147C:  addi         r6, r6, 1
0x825A1480:  stb          r8, 0(r4)
0x825A1484:  b            0x825a1498
0x825A1488:  lbz          r9, 0(r5)
0x825A148C:  addi         r7, r7, -1
0x825A1490:  addi         r5, r5, 1
0x825A1494:  stb          r9, 0(r4)
0x825A1498:  addi         r4, r4, 1
0x825A149C:  srwi         r10, r10, 1
0x825A14A0:  cmplwi       cr6, r7, 0
0x825A14A4:  beq          cr6, 0x825a14d0
0x825A14A8:  cmplwi       cr6, r11, 0
0x825A14AC:  bne          cr6, 0x825a1464
0x825A14B0:  lbz          r8, 0(r5)
0x825A14B4:  addi         r7, r7, -1
0x825A14B8:  addi         r5, r5, 1
0x825A14BC:  cmplwi       cr6, r7, 0
0x825A14C0:  stb          r8, 0(r4)
0x825A14C4:  addi         r4, r4, 1
0x825A14C8:  bne          cr6, 0x825a14b0
0x825A14CC:  blr          
0x825A14D0:  cmplwi       cr6, r11, 0
0x825A14D4:  beqlr        cr6
0x825A14D8:  lbz          r7, 0(r6)
0x825A14DC:  addi         r11, r11, -1
0x825A14E0:  addi         r6, r6, 1
0x825A14E4:  cmplwi       cr6, r11, 0
0x825A14E8:  stb          r7, 0(r4)
0x825A14EC:  addi         r4, r4, 1
0x825A14F0:  bne          cr6, 0x825a14d8
0x825A14F4:  blr          
0x825A14F8:  mflr         r12
0x825A14FC:  bl           0x8242f890
0x825A1500:  stwu         r1, -0xb0(r1)
0x825A1504:  mr           r26, r3
0x825A1508:  mr           r27, r6
0x825A150C:  lwz          r11, 8(r26)
0x825A1510:  lwz          r9, 0(r26)
0x825A1514:  lwz          r31, 4(r26)
0x825A1518:  cmplwi       cr6, r11, 4
0x825A151C:  bge          cr6, 0x825a1544
0x825A1520:  lwz          r8, 0(r31)
0x825A1524:  subfic       r7, r11, 4
0x825A1528:  addi         r31, r31, 4
0x825A152C:  addi         r30, r11, 0x1c
0x825A1530:  slw          r6, r8, r11
0x825A1534:  srw          r29, r8, r7
0x825A1538:  or           r3, r6, r9
0x825A153C:  clrlwi       r10, r3, 0x1c
0x825A1540:  b            0x825a1550
0x825A1544:  clrlwi       r10, r9, 0x1c
0x825A1548:  srwi         r29, r9, 4
0x825A154C:  addi         r30, r11, -4
0x825A1550:  lis          r11, -0x7d8c
0x825A1554:  slwi         r8, r10, 2
0x825A1558:  addi         r11, r11, -0x2540
0x825A155C:  lis          r9, -0x7df5
0x825A1560:  cmplwi       cr6, r10, 0
0x825A1564:  addi         r9, r9, 0x1be0
0x825A1568:  lwzx         r7, r8, r11
0x825A156C:  stw          r7, 0(r4)
0x825A1570:  lbzx         r6, r10, r9
0x825A1574:  stw          r6, 0(r5)
0x825A1578:  bne          cr6, 0x825a15a4
0x825A157C:  li           r11, 0
0x825A1580:  stbx         r11, r11, r27
0x825A1584:  addi         r11, r11, 1
0x825A1588:  cmplwi       cr6, r11, 0x10
0x825A158C:  blt          cr6, 0x825a1580
0x825A1590:  stw          r31, 4(r26)
0x825A1594:  stw          r29, 0(r26)
0x825A1598:  stw          r30, 8(r26)
0x825A159C:  addi         r1, r1, 0xb0
0x825A15A0:  b            0x8242f8e0
0x825A15A4:  cmplwi       cr6, r30, 0
0x825A15A8:  bne          cr6, 0x825a15c0
0x825A15AC:  lwz          r9, 0(r31)
0x825A15B0:  li           r11, 0x1f
0x825A15B4:  addi         r31, r31, 4
0x825A15B8:  srwi         r10, r9, 1
0x825A15BC:  b            0x825a15cc
0x825A15C0:  mr           r9, r29
0x825A15C4:  srwi         r10, r29, 1
0x825A15C8:  addi         r11, r30, -1
0x825A15CC:  clrlwi       r4, r9, 0x1f
0x825A15D0:  cmplwi       cr6, r4, 0
0x825A15D4:  bne          cr6, 0x825a1c24
0x825A15D8:  cmplwi       cr6, r11, 2
0x825A15DC:  bge          cr6, 0x825a1604
0x825A15E0:  lwz          r9, 0(r31)
0x825A15E4:  subfic       r3, r11, 2
0x825A15E8:  addi         r31, r31, 4
0x825A15EC:  addi         r30, r11, 0x1e
0x825A15F0:  slw          r8, r9, r11
0x825A15F4:  srw          r29, r9, r3
0x825A15F8:  or           r7, r8, r10
0x825A15FC:  clrlwi       r28, r7, 0x1e
0x825A1600:  b            0x825a1610
0x825A1604:  clrlwi       r28, r10, 0x1e
0x825A1608:  srwi         r29, r10, 2
0x825A160C:  addi         r30, r11, -2
0x825A1610:  cmplwi       cr6, r28, 0
0x825A1614:  li           r11, 1
0x825A1618:  bne          cr6, 0x825a1694
0x825A161C:  addi         r9, r27, -1
0x825A1620:  cmplwi       cr6, r30, 0
0x825A1624:  bne          cr6, 0x825a163c
0x825A1628:  lwz          r10, 0(r31)
0x825A162C:  li           r30, 0x1f
0x825A1630:  addi         r31, r31, 4
0x825A1634:  srwi         r29, r10, 1
0x825A1638:  b            0x825a1648
0x825A163C:  mr           r10, r29
0x825A1640:  srwi         r29, r29, 1
0x825A1644:  addi         r30, r30, -1
0x825A1648:  clrlwi       r6, r10, 0x1f
0x825A164C:  cmplwi       cr6, r6, 0
0x825A1650:  beq          cr6, 0x825a1664
0x825A1654:  addi         r5, r11, 0xff
0x825A1658:  stbx         r11, r9, r11
0x825A165C:  stbx         r5, r11, r27
0x825A1660:  b            0x825a1670
0x825A1664:  addi         r10, r11, 0xff
0x825A1668:  stbx         r11, r11, r27
0x825A166C:  stbx         r10, r9, r11
0x825A1670:  addi         r11, r11, 2
0x825A1674:  addi         r6, r11, -1
0x825A1678:  cmplwi       cr6, r6, 0x10
0x825A167C:  blt          cr6, 0x825a1620
0x825A1680:  stw          r31, 4(r26)
0x825A1684:  stw          r29, 0(r26)
0x825A1688:  stw          r30, 8(r26)
0x825A168C:  addi         r1, r1, 0xb0
0x825A1690:  b            0x8242f8e0
0x825A1694:  addi         r10, r1, 0x50
0x825A1698:  li           r9, 0
0x825A169C:  addi         r8, r10, -1
0x825A16A0:  cmplwi       cr6, r30, 0
0x825A16A4:  bne          cr6, 0x825a16bc
0x825A16A8:  lwz          r10, 0(r31)
0x825A16AC:  li           r30, 0x1f
0x825A16B0:  addi         r31, r31, 4
0x825A16B4:  srwi         r29, r10, 1
0x825A16B8:  b            0x825a16c8
0x825A16BC:  mr           r10, r29
0x825A16C0:  srwi         r29, r29, 1
0x825A16C4:  addi         r30, r30, -1
0x825A16C8:  clrlwi       r5, r10, 0x1f
0x825A16CC:  cmplwi       cr6, r5, 0
0x825A16D0:  beq          cr6, 0x825a16e4
0x825A16D4:  addi         r10, r1, 0x50
0x825A16D8:  stbx         r11, r8, r11
0x825A16DC:  stbx         r9, r11, r10
0x825A16E0:  b            0x825a16f0
0x825A16E4:  addi         r5, r1, 0x50
0x825A16E8:  stbx         r9, r8, r11
0x825A16EC:  stbx         r11, r11, r5
0x825A16F0:  addi         r11, r11, 2
0x825A16F4:  addi         r9, r9, 2
0x825A16F8:  cmplwi       cr6, r11, 0x11
0x825A16FC:  blt          cr6, 0x825a16a0
0x825A1700:  cmplwi       cr6, r28, 1
0x825A1704:  bne          cr6, 0x825a18ac
0x825A1708:  cmplwi       cr6, r30, 3
0x825A170C:  bge          cr6, 0x825a1724
0x825A1710:  lwz          r4, 0(r31)
0x825A1714:  slw          r3, r4, r30
0x825A1718:  or           r11, r3, r29
0x825A171C:  clrlwi       r3, r11, 0x1d
0x825A1720:  b            0x825a1728
0x825A1724:  clrlwi       r3, r29, 0x1d
0x825A1728:  li           r7, 2
0x825A172C:  addi         r6, r1, 0x52
0x825A1730:  addi         r5, r1, 0x50
0x825A1734:  mr           r4, r27
0x825A1738:  bl           0x825a1458
0x825A173C:  cmplw        cr6, r30, r3
0x825A1740:  bge          cr6, 0x825a1760
0x825A1744:  lwz          r10, 0(r31)
0x825A1748:  subf         r9, r30, r3
0x825A174C:  subf         r11, r3, r30
0x825A1750:  addi         r31, r31, 4
0x825A1754:  addi         r30, r11, 0x20
0x825A1758:  srw          r29, r10, r9
0x825A175C:  b            0x825a1768
0x825A1760:  subf         r30, r3, r30
0x825A1764:  srw          r29, r29, r3
0x825A1768:  cmplwi       cr6, r30, 3
0x825A176C:  bge          cr6, 0x825a1784
0x825A1770:  lwz          r8, 0(r31)
0x825A1774:  slw          r7, r8, r30
0x825A1778:  or           r6, r7, r29
0x825A177C:  clrlwi       r3, r6, 0x1d
0x825A1780:  b            0x825a1788
0x825A1784:  clrlwi       r3, r29, 0x1d
0x825A1788:  li           r7, 2
0x825A178C:  addi         r6, r1, 0x56
0x825A1790:  addi         r5, r1, 0x54
0x825A1794:  addi         r4, r27, 4
0x825A1798:  bl           0x825a1458
0x825A179C:  cmplw        cr6, r30, r3
0x825A17A0:  bge          cr6, 0x825a17c0
0x825A17A4:  lwz          r5, 0(r31)
0x825A17A8:  subf         r4, r30, r3
0x825A17AC:  subf         r11, r3, r30
0x825A17B0:  addi         r31, r31, 4
0x825A17B4:  addi         r30, r11, 0x20
0x825A17B8:  srw          r29, r5, r4
0x825A17BC:  b            0x825a17c8
0x825A17C0:  subf         r30, r3, r30
0x825A17C4:  srw          r29, r29, r3
0x825A17C8:  cmplwi       cr6, r30, 3
0x825A17CC:  bge          cr6, 0x825a17e4
0x825A17D0:  lwz          r3, 0(r31)
0x825A17D4:  slw          r11, r3, r30
0x825A17D8:  or           r10, r11, r29
0x825A17DC:  clrlwi       r3, r10, 0x1d
0x825A17E0:  b            0x825a17e8
0x825A17E4:  clrlwi       r3, r29, 0x1d
0x825A17E8:  li           r7, 2
0x825A17EC:  addi         r6, r1, 0x5a
0x825A17F0:  addi         r5, r1, 0x58
0x825A17F4:  addi         r4, r27, 8
0x825A17F8:  bl           0x825a1458
0x825A17FC:  cmplw        cr6, r30, r3
0x825A1800:  bge          cr6, 0x825a1820
0x825A1804:  lwz          r9, 0(r31)
0x825A1808:  subf         r8, r30, r3
0x825A180C:  subf         r11, r3, r30
0x825A1810:  addi         r31, r31, 4
0x825A1814:  addi         r30, r11, 0x20
0x825A1818:  srw          r29, r9, r8
0x825A181C:  b            0x825a1828
0x825A1820:  subf         r30, r3, r30
0x825A1824:  srw          r29, r29, r3
0x825A1828:  cmplwi       cr6, r30, 3
0x825A182C:  bge          cr6, 0x825a1844
0x825A1830:  lwz          r7, 0(r31)
0x825A1834:  slw          r6, r7, r30
0x825A1838:  or           r5, r6, r29
0x825A183C:  clrlwi       r3, r5, 0x1d
0x825A1840:  b            0x825a1848
0x825A1844:  clrlwi       r3, r29, 0x1d
0x825A1848:  li           r7, 2
0x825A184C:  addi         r6, r1, 0x5e
0x825A1850:  addi         r5, r1, 0x5c
0x825A1854:  addi         r4, r27, 0xc
0x825A1858:  bl           0x825a1458
0x825A185C:  cmplw        cr6, r30, r3
0x825A1860:  bge          cr6, 0x825a1890
0x825A1864:  lwz          r4, 0(r31)
0x825A1868:  subf         r10, r30, r3
0x825A186C:  subf         r11, r3, r30
0x825A1870:  addi         r31, r31, 4
0x825A1874:  addi         r30, r11, 0x20
0x825A1878:  stw          r31, 4(r26)
0x825A187C:  stw          r30, 8(r26)
0x825A1880:  srw          r29, r4, r10
0x825A1884:  stw          r29, 0(r26)
0x825A1888:  addi         r1, r1, 0xb0
0x825A188C:  b            0x8242f8e0
0x825A1890:  subf         r30, r3, r30
0x825A1894:  stw          r31, 4(r26)
0x825A1898:  srw          r29, r29, r3
0x825A189C:  stw          r30, 8(r26)
0x825A18A0:  stw          r29, 0(r26)
0x825A18A4:  addi         r1, r1, 0xb0
0x825A18A8:  b            0x8242f8e0
0x825A18AC:  cmplwi       cr6, r30, 3
0x825A18B0:  bge          cr6, 0x825a18c8
0x825A18B4:  lwz          r9, 0(r31)
0x825A18B8:  slw          r8, r9, r30
0x825A18BC:  or           r7, r8, r29
0x825A18C0:  clrlwi       r3, r7, 0x1d
0x825A18C4:  b            0x825a18cc
0x825A18C8:  clrlwi       r3, r29, 0x1d
0x825A18CC:  li           r7, 2
0x825A18D0:  addi         r6, r1, 0x52
0x825A18D4:  addi         r5, r1, 0x50
0x825A18D8:  addi         r4, r1, 0x60
0x825A18DC:  bl           0x825a1458
0x825A18E0:  cmplw        cr6, r30, r3
0x825A18E4:  bge          cr6, 0x825a1904
0x825A18E8:  lwz          r6, 0(r31)
0x825A18EC:  subf         r5, r30, r3
0x825A18F0:  subf         r11, r3, r30
0x825A18F4:  addi         r31, r31, 4
0x825A18F8:  addi         r30, r11, 0x20
0x825A18FC:  srw          r29, r6, r5
0x825A1900:  b            0x825a190c
0x825A1904:  subf         r30, r3, r30
0x825A1908:  srw          r29, r29, r3
0x825A190C:  cmplwi       cr6, r30, 3
0x825A1910:  bge          cr6, 0x825a1928
0x825A1914:  lwz          r4, 0(r31)
0x825A1918:  slw          r3, r4, r30
0x825A191C:  or           r11, r3, r29
0x825A1920:  clrlwi       r3, r11, 0x1d
0x825A1924:  b            0x825a192c
0x825A1928:  clrlwi       r3, r29, 0x1d
0x825A192C:  li           r7, 2
0x825A1930:  addi         r6, r1, 0x56
0x825A1934:  addi         r5, r1, 0x54
0x825A1938:  addi         r4, r1, 0x64
0x825A193C:  bl           0x825a1458
0x825A1940:  cmplw        cr6, r30, r3
0x825A1944:  bge          cr6, 0x825a1964
0x825A1948:  lwz          r10, 0(r31)
0x825A194C:  subf         r9, r30, r3
0x825A1950:  subf         r11, r3, r30
0x825A1954:  addi         r31, r31, 4
0x825A1958:  addi         r30, r11, 0x20
0x825A195C:  srw          r29, r10, r9
0x825A1960:  b            0x825a196c
0x825A1964:  subf         r30, r3, r30
0x825A1968:  srw          r29, r29, r3
0x825A196C:  cmplwi       cr6, r30, 3
0x825A1970:  bge          cr6, 0x825a1988
0x825A1974:  lwz          r8, 0(r31)
0x825A1978:  slw          r7, r8, r30
0x825A197C:  or           r6, r7, r29
0x825A1980:  clrlwi       r3, r6, 0x1d
0x825A1984:  b            0x825a198c
0x825A1988:  clrlwi       r3, r29, 0x1d
0x825A198C:  li           r7, 2
0x825A1990:  addi         r6, r1, 0x5a
0x825A1994:  addi         r5, r1, 0x58
0x825A1998:  addi         r4, r1, 0x68
0x825A199C:  bl           0x825a1458
0x825A19A0:  cmplw        cr6, r30, r3
0x825A19A4:  bge          cr6, 0x825a19c4
0x825A19A8:  lwz          r5, 0(r31)
0x825A19AC:  subf         r4, r30, r3
0x825A19B0:  subf         r11, r3, r30
0x825A19B4:  addi         r31, r31, 4
0x825A19B8:  addi         r30, r11, 0x20
0x825A19BC:  srw          r29, r5, r4
0x825A19C0:  b            0x825a19cc
0x825A19C4:  subf         r30, r3, r30
0x825A19C8:  srw          r29, r29, r3
0x825A19CC:  cmplwi       cr6, r30, 3
0x825A19D0:  bge          cr6, 0x825a19e8
0x825A19D4:  lwz          r3, 0(r31)
0x825A19D8:  slw          r11, r3, r30
0x825A19DC:  or           r10, r11, r29
0x825A19E0:  clrlwi       r3, r10, 0x1d
0x825A19E4:  b            0x825a19ec
0x825A19E8:  clrlwi       r3, r29, 0x1d
0x825A19EC:  li           r7, 2
0x825A19F0:  addi         r6, r1, 0x5e
0x825A19F4:  addi         r5, r1, 0x5c
0x825A19F8:  addi         r4, r1, 0x6c
0x825A19FC:  bl           0x825a1458
0x825A1A00:  cmplw        cr6, r30, r3
0x825A1A04:  bge          cr6, 0x825a1a24
0x825A1A08:  lwz          r9, 0(r31)
0x825A1A0C:  subf         r8, r30, r3
0x825A1A10:  subf         r11, r3, r30
0x825A1A14:  addi         r31, r31, 4
0x825A1A18:  addi         r30, r11, 0x20
0x825A1A1C:  srw          r29, r9, r8
0x825A1A20:  b            0x825a1a2c
0x825A1A24:  subf         r30, r3, r30
0x825A1A28:  srw          r29, r29, r3
0x825A1A2C:  cmplwi       cr6, r28, 2
0x825A1A30:  bne          cr6, 0x825a1afc
0x825A1A34:  cmplwi       cr6, r30, 7
0x825A1A38:  bge          cr6, 0x825a1a50
0x825A1A3C:  lwz          r7, 0(r31)
0x825A1A40:  slw          r6, r7, r30
0x825A1A44:  or           r5, r6, r29
0x825A1A48:  clrlwi       r3, r5, 0x19
0x825A1A4C:  b            0x825a1a54
0x825A1A50:  clrlwi       r3, r29, 0x19
0x825A1A54:  li           r7, 4
0x825A1A58:  addi         r6, r1, 0x64
0x825A1A5C:  addi         r5, r1, 0x60
0x825A1A60:  mr           r4, r27
0x825A1A64:  bl           0x825a1458
0x825A1A68:  cmplw        cr6, r30, r3
0x825A1A6C:  bge          cr6, 0x825a1a8c
0x825A1A70:  lwz          r4, 0(r31)
0x825A1A74:  subf         r10, r30, r3
0x825A1A78:  subf         r11, r3, r30
0x825A1A7C:  addi         r31, r31, 4
0x825A1A80:  addi         r30, r11, 0x20
0x825A1A84:  srw          r29, r4, r10
0x825A1A88:  b            0x825a1a94
0x825A1A8C:  subf         r30, r3, r30
0x825A1A90:  srw          r29, r29, r3
0x825A1A94:  cmplwi       cr6, r30, 7
0x825A1A98:  bge          cr6, 0x825a1ab0
0x825A1A9C:  lwz          r9, 0(r31)
0x825A1AA0:  slw          r8, r9, r30
0x825A1AA4:  or           r7, r8, r29
0x825A1AA8:  clrlwi       r3, r7, 0x19
0x825A1AAC:  b            0x825a1ab4
0x825A1AB0:  clrlwi       r3, r29, 0x19
0x825A1AB4:  li           r7, 4
0x825A1AB8:  addi         r6, r1, 0x6c
0x825A1ABC:  addi         r5, r1, 0x68
0x825A1AC0:  addi         r4, r27, 8
0x825A1AC4:  bl           0x825a1458
0x825A1AC8:  cmplw        cr6, r30, r3
0x825A1ACC:  bge          cr6, 0x825a1890
0x825A1AD0:  lwz          r6, 0(r31)
0x825A1AD4:  subf         r5, r30, r3
0x825A1AD8:  subf         r11, r3, r30
0x825A1ADC:  addi         r31, r31, 4
0x825A1AE0:  addi         r30, r11, 0x20
0x825A1AE4:  stw          r31, 4(r26)
0x825A1AE8:  stw          r30, 8(r26)
0x825A1AEC:  srw          r29, r6, r5
0x825A1AF0:  stw          r29, 0(r26)
0x825A1AF4:  addi         r1, r1, 0xb0
0x825A1AF8:  b            0x8242f8e0
0x825A1AFC:  cmplwi       cr6, r30, 7
0x825A1B00:  bge          cr6, 0x825a1b18
0x825A1B04:  lwz          r4, 0(r31)
0x825A1B08:  slw          r3, r4, r30
0x825A1B0C:  or           r11, r3, r29
0x825A1B10:  clrlwi       r3, r11, 0x19
0x825A1B14:  b            0x825a1b1c
0x825A1B18:  clrlwi       r3, r29, 0x19
0x825A1B1C:  li           r7, 4
0x825A1B20:  addi         r6, r1, 0x64
0x825A1B24:  addi         r5, r1, 0x60
0x825A1B28:  addi         r4, r1, 0x50
0x825A1B2C:  bl           0x825a1458
0x825A1B30:  cmplw        cr6, r30, r3
0x825A1B34:  bge          cr6, 0x825a1b54
0x825A1B38:  lwz          r10, 0(r31)
0x825A1B3C:  subf         r9, r30, r3
0x825A1B40:  subf         r11, r3, r30
0x825A1B44:  addi         r31, r31, 4
0x825A1B48:  addi         r30, r11, 0x20
0x825A1B4C:  srw          r29, r10, r9
0x825A1B50:  b            0x825a1b5c
0x825A1B54:  subf         r30, r3, r30
0x825A1B58:  srw          r29, r29, r3
0x825A1B5C:  cmplwi       cr6, r30, 7
0x825A1B60:  bge          cr6, 0x825a1b78
0x825A1B64:  lwz          r8, 0(r31)
0x825A1B68:  slw          r7, r8, r30
0x825A1B6C:  or           r6, r7, r29
0x825A1B70:  clrlwi       r3, r6, 0x19
0x825A1B74:  b            0x825a1b7c
0x825A1B78:  clrlwi       r3, r29, 0x19
0x825A1B7C:  li           r7, 4
0x825A1B80:  addi         r6, r1, 0x6c
0x825A1B84:  addi         r5, r1, 0x68
0x825A1B88:  addi         r4, r1, 0x58
0x825A1B8C:  bl           0x825a1458
0x825A1B90:  cmplw        cr6, r30, r3
0x825A1B94:  bge          cr6, 0x825a1bb4
0x825A1B98:  lwz          r5, 0(r31)
0x825A1B9C:  subf         r4, r30, r3
0x825A1BA0:  subf         r11, r3, r30
0x825A1BA4:  addi         r31, r31, 4
0x825A1BA8:  addi         r30, r11, 0x20
0x825A1BAC:  srw          r29, r5, r4
0x825A1BB0:  b            0x825a1bbc
0x825A1BB4:  subf         r30, r3, r30
0x825A1BB8:  srw          r29, r29, r3
0x825A1BBC:  cmplwi       cr6, r30, 0xf
0x825A1BC0:  bge          cr6, 0x825a1bd8
0x825A1BC4:  lwz          r3, 0(r31)
0x825A1BC8:  slw          r11, r3, r30
0x825A1BCC:  or           r10, r11, r29
0x825A1BD0:  clrlwi       r3, r10, 0x11
0x825A1BD4:  b            0x825a1bdc
0x825A1BD8:  clrlwi       r3, r29, 0x11
0x825A1BDC:  li           r7, 8
0x825A1BE0:  addi         r6, r1, 0x58
0x825A1BE4:  addi         r5, r1, 0x50
0x825A1BE8:  mr           r4, r27
0x825A1BEC:  bl           0x825a1458
0x825A1BF0:  cmplw        cr6, r30, r3
0x825A1BF4:  bge          cr6, 0x825a1890
0x825A1BF8:  lwz          r9, 0(r31)
0x825A1BFC:  subf         r8, r30, r3
0x825A1C00:  subf         r11, r3, r30
0x825A1C04:  addi         r31, r31, 4
0x825A1C08:  addi         r30, r11, 0x20
0x825A1C0C:  stw          r31, 4(r26)
0x825A1C10:  stw          r30, 8(r26)
0x825A1C14:  srw          r29, r9, r8
0x825A1C18:  stw          r29, 0(r26)
0x825A1C1C:  addi         r1, r1, 0xb0
0x825A1C20:  b            0x8242f8e0
0x825A1C24:  cmplwi       cr6, r11, 3
0x825A1C28:  bge          cr6, 0x825a1c50
0x825A1C2C:  lwz          r9, 0(r31)
0x825A1C30:  subfic       r6, r11, 3
0x825A1C34:  addi         r31, r31, 4
0x825A1C38:  addi         r30, r11, 0x1d
0x825A1C3C:  slw          r5, r9, r11
0x825A1C40:  srw          r29, r9, r6
0x825A1C44:  or           r4, r5, r10
0x825A1C48:  clrlwi       r7, r4, 0x1d
0x825A1C4C:  b            0x825a1c5c
0x825A1C50:  clrlwi       r7, r10, 0x1d
0x825A1C54:  srwi         r29, r10, 3
0x825A1C58:  addi         r30, r11, -3
0x825A1C5C:  lis          r8, 0
0x825A1C60:  li           r10, 0
0x825A1C64:  ori          r8, r8, 0xffff
0x825A1C68:  li           r6, 1
0x825A1C6C:  cmplwi       cr6, r30, 4
0x825A1C70:  bge          cr6, 0x825a1c98
0x825A1C74:  lwz          r9, 0(r31)
0x825A1C78:  subfic       r3, r30, 4
0x825A1C7C:  addi         r31, r31, 4
0x825A1C80:  slw          r11, r9, r30
0x825A1C84:  addi         r30, r30, 0x1c
0x825A1C88:  or           r5, r11, r29
0x825A1C8C:  srw          r29, r9, r3
0x825A1C90:  clrlwi       r11, r5, 0x1c
0x825A1C94:  b            0x825a1ca4
0x825A1C98:  clrlwi       r11, r29, 0x1c
0x825A1C9C:  srwi         r29, r29, 4
0x825A1CA0:  addi         r30, r30, -4
0x825A1CA4:  stbx         r11, r10, r27
0x825A1CA8:  slw          r3, r6, r11
0x825A1CAC:  addi         r10, r10, 1
0x825A1CB0:  andc         r8, r8, r3
0x825A1CB4:  cmplw        cr6, r10, r7
0x825A1CB8:  ble          cr6, 0x825a1c6c
0x825A1CBC:  li           r11, 0
0x825A1CC0:  add          r10, r7, r27
0x825A1CC4:  clrlwi       r9, r8, 0x1f
0x825A1CC8:  cmplwi       cr6, r9, 0
0x825A1CCC:  beq          cr6, 0x825a1cd8
0x825A1CD0:  addi         r10, r10, 1
0x825A1CD4:  stb          r11, 0(r10)
0x825A1CD8:  srwi         r8, r8, 1
0x825A1CDC:  addi         r11, r11, 1
0x825A1CE0:  cmplwi       cr6, r8, 0
0x825A1CE4:  bne          cr6, 0x825a1cc4
0x825A1CE8:  stw          r31, 4(r26)
0x825A1CEC:  stw          r29, 0(r26)
0x825A1CF0:  stw          r30, 8(r26)
0x825A1CF4:  addi         r1, r1, 0xb0
0x825A1CF8:  b            0x8242f8e0
0x825A1CFC:  .align 0                       # padding
0x825A1D00:  mflr         r12
0x825A1D04:  bl           0x8242f88c
0x825A1D08:  stwu         r1, -0x90(r1)
0x825A1D0C:  mr           r8, r3
0x825A1D10:  mr           r25, r4
0x825A1D14:  lwz          r11, 0(r8)
0x825A1D18:  lwz          r10, 4(r8)
0x825A1D1C:  cmplw        cr6, r11, r10
0x825A1D20:  bne          cr6, 0x825a1fd0
0x825A1D24:  lwz          r11, 0x28(r8)
0x825A1D28:  li           r9, -1
0x825A1D2C:  lwz          r10, 8(r25)
0x825A1D30:  lwz          r6, 0(r25)
0x825A1D34:  subfic       r5, r11, 0x20
0x825A1D38:  lwz          r29, 4(r25)
0x825A1D3C:  cmplw        cr6, r10, r11
0x825A1D40:  bge          cr6, 0x825a1d70
0x825A1D44:  lwz          r7, 0(r29)
0x825A1D48:  subf         r4, r10, r11
0x825A1D4C:  subf         r11, r11, r10
0x825A1D50:  srw          r3, r9, r5
0x825A1D54:  addi         r29, r29, 4
0x825A1D58:  addi         r31, r11, 0x20
0x825A1D5C:  slw          r10, r7, r10
0x825A1D60:  srw          r30, r7, r4
0x825A1D64:  or           r6, r10, r6
0x825A1D68:  and          r5, r3, r6
0x825A1D6C:  b            0x825a1d80
0x825A1D70:  srw          r4, r9, r5
0x825A1D74:  subf         r31, r11, r10
0x825A1D78:  and          r5, r4, r6
0x825A1D7C:  srw          r30, r6, r11
0x825A1D80:  lwz          r11, 0x2c(r8)
0x825A1D84:  cmplwi       cr6, r5, 0
0x825A1D88:  beq          cr6, 0x825a1fb8
0x825A1D8C:  add          r3, r11, r5
0x825A1D90:  stw          r11, 0(r8)
0x825A1D94:  cmplwi       cr6, r31, 0
0x825A1D98:  stw          r3, 4(r8)
0x825A1D9C:  bne          cr6, 0x825a1db4
0x825A1DA0:  lwz          r10, 0(r29)
0x825A1DA4:  li           r31, 0x1f
0x825A1DA8:  addi         r29, r29, 4
0x825A1DAC:  srwi         r30, r10, 1
0x825A1DB0:  b            0x825a1dc0
0x825A1DB4:  mr           r10, r30
0x825A1DB8:  srwi         r30, r30, 1
0x825A1DBC:  addi         r31, r31, -1
0x825A1DC0:  clrlwi       r10, r10, 0x1f
0x825A1DC4:  cmplwi       cr6, r10, 0
0x825A1DC8:  bne          cr6, 0x825a1f4c
0x825A1DCC:  lwz          r26, 0x20(r8)
0x825A1DD0:  addi         r3, r8, 0x10
0x825A1DD4:  lwz          r4, 0x24(r8)
0x825A1DD8:  lis          r10, -0x7df5
0x825A1DDC:  subfic       r8, r26, 0x20
0x825A1DE0:  li           r6, 0
0x825A1DE4:  addi         r27, r10, 0x1bf0
0x825A1DE8:  srw          r28, r9, r8
0x825A1DEC:  cmplw        cr6, r31, r26
0x825A1DF0:  blt          cr6, 0x825a1e14
0x825A1DF4:  and          r7, r28, r30
0x825A1DF8:  lbzx         r10, r7, r4
0x825A1DFC:  clrlwi       r9, r10, 0x1c
0x825A1E00:  srwi         r10, r10, 4
0x825A1E04:  subf         r31, r10, r31
0x825A1E08:  lbzx         r8, r9, r3
0x825A1E0C:  srw          r30, r30, r10
0x825A1E10:  b            0x825a1e5c
0x825A1E14:  lwz          r9, 0(r29)
0x825A1E18:  slw          r8, r9, r31
0x825A1E1C:  or           r7, r8, r30
0x825A1E20:  and          r10, r7, r28
0x825A1E24:  lbzx         r10, r10, r4
0x825A1E28:  clrlwi       r8, r10, 0x1c
0x825A1E2C:  srwi         r10, r10, 4
0x825A1E30:  cmplw        cr6, r31, r10
0x825A1E34:  lbzx         r8, r8, r3
0x825A1E38:  blt          cr6, 0x825a1e48
0x825A1E3C:  subf         r31, r10, r31
0x825A1E40:  srw          r30, r30, r10
0x825A1E44:  b            0x825a1e5c
0x825A1E48:  subf         r7, r31, r10
0x825A1E4C:  subf         r10, r10, r31
0x825A1E50:  addi         r29, r29, 4
0x825A1E54:  addi         r31, r10, 0x20
0x825A1E58:  srw          r30, r9, r7
0x825A1E5C:  clrlwi       r9, r8, 0x18
0x825A1E60:  cmplwi       cr6, r9, 0xc
0x825A1E64:  blt          cr6, 0x825a1f20
0x825A1E68:  slwi         r10, r6, 8
0x825A1E6C:  add          r9, r9, r27
0x825A1E70:  or           r10, r10, r6
0x825A1E74:  clrlwi       r8, r11, 0x1e
0x825A1E78:  slwi         r7, r10, 0x10
0x825A1E7C:  cmplwi       cr6, r8, 0
0x825A1E80:  lbz          r9, -0xc(r9)
0x825A1E84:  or           r8, r7, r10
0x825A1E88:  mr           r10, r9
0x825A1E8C:  subf         r5, r10, r5
0x825A1E90:  beq          cr6, 0x825a1f04
0x825A1E94:  clrlwi       r7, r8, 0x18
0x825A1E98:  addi         r10, r10, 0xff
0x825A1E9C:  stb          r7, 0(r11)
0x825A1EA0:  addi         r11, r11, 1
0x825A1EA4:  clrlwi       r9, r10, 0x18
0x825A1EA8:  mr           r10, r9
0x825A1EAC:  cmplwi       cr6, r10, 0
0x825A1EB0:  bne          cr6, 0x825a1e98
0x825A1EB4:  clrlwi       r10, r9, 0x18
0x825A1EB8:  cmplwi       cr6, r10, 4
0x825A1EBC:  blt          cr6, 0x825a1edc
0x825A1EC0:  addi         r9, r10, 0xfc
0x825A1EC4:  stw          r8, 0(r11)
0x825A1EC8:  addi         r11, r11, 4
0x825A1ECC:  clrlwi       r9, r9, 0x18
0x825A1ED0:  mr           r10, r9
0x825A1ED4:  cmplwi       cr6, r10, 4
0x825A1ED8:  bge          cr6, 0x825a1ec0
0x825A1EDC:  clrlwi       r10, r9, 0x18
0x825A1EE0:  cmplwi       cr6, r10, 0
0x825A1EE4:  beq          cr6, 0x825a1f30
0x825A1EE8:  addi         r8, r10, 0xff
0x825A1EEC:  stb          r7, 0(r11)
0x825A1EF0:  addi         r11, r11, 1
0x825A1EF4:  clrlwi       r10, r8, 0x18
0x825A1EF8:  cmplwi       cr6, r10, 0
0x825A1EFC:  bne          cr6, 0x825a1ee8
0x825A1F00:  b            0x825a1f30
0x825A1F04:  addi         r7, r10, 0xfc
0x825A1F08:  stw          r8, 0(r11)
0x825A1F0C:  addi         r11, r11, 4
0x825A1F10:  clrlwi       r10, r7, 0x18
0x825A1F14:  cmplwi       cr6, r10, 0
0x825A1F18:  bne          cr6, 0x825a1f04
0x825A1F1C:  b            0x825a1f30
0x825A1F20:  stb          r8, 0(r11)
0x825A1F24:  mr           r6, r9
0x825A1F28:  addi         r5, r5, -1
0x825A1F2C:  addi         r11, r11, 1
0x825A1F30:  cmplwi       cr6, r5, 0
0x825A1F34:  bne          cr6, 0x825a1dec
0x825A1F38:  stw          r29, 4(r25)
0x825A1F3C:  stw          r30, 0(r25)
0x825A1F40:  stw          r31, 8(r25)
0x825A1F44:  addi         r1, r1, 0x90
0x825A1F48:  b            0x8242f8dc
0x825A1F4C:  cmplwi       cr6, r31, 4
0x825A1F50:  bge          cr6, 0x825a1f90
0x825A1F54:  lwz          r10, 0(r29)
0x825A1F58:  subfic       r6, r31, 4
0x825A1F5C:  addi         r29, r29, 4
0x825A1F60:  slw          r4, r10, r31
0x825A1F64:  addi         r31, r31, 0x1c
0x825A1F68:  or           r3, r4, r30
0x825A1F6C:  srw          r30, r10, r6
0x825A1F70:  clrlwi       r4, r3, 0x1c
0x825A1F74:  mr           r3, r11
0x825A1F78:  bl           0x8242fed0
0x825A1F7C:  stw          r29, 4(r25)
0x825A1F80:  stw          r30, 0(r25)
0x825A1F84:  stw          r31, 8(r25)
0x825A1F88:  addi         r1, r1, 0x90
0x825A1F8C:  b            0x8242f8dc
0x825A1F90:  clrlwi       r4, r30, 0x1c
0x825A1F94:  mr           r3, r11
0x825A1F98:  srwi         r30, r30, 4
0x825A1F9C:  addi         r31, r31, -4
0x825A1FA0:  bl           0x8242fed0
0x825A1FA4:  stw          r29, 4(r25)
0x825A1FA8:  stw          r30, 0(r25)
0x825A1FAC:  stw          r31, 8(r25)
0x825A1FB0:  addi         r1, r1, 0x90
0x825A1FB4:  b            0x8242f8dc
0x825A1FB8:  addi         r10, r11, 4
0x825A1FBC:  stw          r11, 4(r8)
0x825A1FC0:  stw          r10, 0(r8)
0x825A1FC4:  stw          r29, 4(r25)
0x825A1FC8:  stw          r30, 0(r25)
0x825A1FCC:  stw          r31, 8(r25)
0x825A1FD0:  addi         r1, r1, 0x90
0x825A1FD4:  b            0x8242f8dc
0x825A1FD8:  mflr         r12
0x825A1FDC:  bl           0x8242f880
0x825A1FE0:  stwu         r1, -0xb0(r1)
0x825A1FE4:  lwz          r11, 0(r3)
0x825A1FE8:  mr           r22, r4
0x825A1FEC:  lwz          r10, 4(r3)
0x825A1FF0:  mr           r27, r5
0x825A1FF4:  cmplw        cr6, r11, r10
0x825A1FF8:  bne          cr6, 0x825a2268
0x825A1FFC:  lwz          r11, 0x28(r3)
0x825A2000:  li           r6, -1
0x825A2004:  lwz          r10, 8(r22)
0x825A2008:  lwz          r8, 0(r22)
0x825A200C:  subfic       r7, r11, 0x20
0x825A2010:  lwz          r28, 4(r22)
0x825A2014:  cmplw        cr6, r10, r11
0x825A2018:  bge          cr6, 0x825a2048
0x825A201C:  lwz          r9, 0(r28)
0x825A2020:  subf         r5, r10, r11
0x825A2024:  subf         r11, r11, r10
0x825A2028:  srw          r4, r6, r7
0x825A202C:  addi         r28, r28, 4
0x825A2030:  addi         r31, r11, 0x20
0x825A2034:  slw          r10, r9, r10
0x825A2038:  srw          r30, r9, r5
0x825A203C:  or           r8, r10, r8
0x825A2040:  and          r25, r4, r8
0x825A2044:  b            0x825a2058
0x825A2048:  srw          r5, r6, r7
0x825A204C:  subf         r31, r11, r10
0x825A2050:  and          r25, r5, r8
0x825A2054:  srw          r30, r8, r11
0x825A2058:  lwz          r11, 0x2c(r3)
0x825A205C:  cmpwi        cr6, r25, 0
0x825A2060:  beq          cr6, 0x825a2250
0x825A2064:  add          r10, r11, r25
0x825A2068:  stw          r11, 0(r3)
0x825A206C:  lwz          r23, 0x20(r3)
0x825A2070:  addi         r26, r3, 0x10
0x825A2074:  lwz          r4, 0x24(r3)
0x825A2078:  cmplwi       cr6, r31, 0
0x825A207C:  mr           r24, r11
0x825A2080:  stw          r10, 4(r3)
0x825A2084:  lwz          r29, 0x180(r27)
0x825A2088:  bne          cr6, 0x825a20a0
0x825A208C:  lwz          r11, 0(r28)
0x825A2090:  li           r31, 0x1f
0x825A2094:  addi         r28, r28, 4
0x825A2098:  srwi         r30, r11, 1
0x825A209C:  b            0x825a20ac
0x825A20A0:  mr           r11, r30
0x825A20A4:  srwi         r30, r30, 1
0x825A20A8:  addi         r31, r31, -1
0x825A20AC:  clrlwi       r9, r11, 0x1f
0x825A20B0:  cmplwi       cr6, r9, 0
0x825A20B4:  beq          cr6, 0x825a20bc
0x825A20B8:  subfic       r25, r25, -0x14
0x825A20BC:  subfic       r8, r23, 0x20
0x825A20C0:  srw          r5, r6, r8
0x825A20C4:  addi         r7, r29, 0x40
0x825A20C8:  addi         r25, r25, -1
0x825A20CC:  slwi         r11, r7, 2
0x825A20D0:  lwzx         r11, r11, r27
0x825A20D4:  cmplw        cr6, r31, r11
0x825A20D8:  blt          cr6, 0x825a2118
0x825A20DC:  subfic       r10, r11, 0x20
0x825A20E0:  addi         r9, r29, 0x50
0x825A20E4:  slwi         r8, r9, 2
0x825A20E8:  slwi         r9, r29, 4
0x825A20EC:  lwzx         r7, r8, r27
0x825A20F0:  srw          r11, r6, r10
0x825A20F4:  and          r10, r11, r30
0x825A20F8:  lbzx         r11, r10, r7
0x825A20FC:  clrlwi       r10, r11, 0x1c
0x825A2100:  srwi         r11, r11, 4
0x825A2104:  add          r9, r10, r9
0x825A2108:  lbzx         r29, r9, r27
0x825A210C:  srw          r10, r30, r11
0x825A2110:  subf         r11, r11, r31
0x825A2114:  b            0x825a2178
0x825A2118:  subfic       r8, r11, 0x20
0x825A211C:  lwz          r9, 0(r28)
0x825A2120:  addi         r11, r29, 0x50
0x825A2124:  slwi         r7, r29, 4
0x825A2128:  slwi         r10, r11, 2
0x825A212C:  lwzx         r11, r10, r27
0x825A2130:  srw          r10, r6, r8
0x825A2134:  slw          r8, r9, r31
0x825A2138:  or           r8, r8, r30
0x825A213C:  and          r10, r10, r8
0x825A2140:  lbzx         r11, r10, r11
0x825A2144:  clrlwi       r8, r11, 0x1c
0x825A2148:  srwi         r10, r11, 4
0x825A214C:  add          r8, r8, r7
0x825A2150:  cmplw        cr6, r31, r10
0x825A2154:  subf         r11, r10, r31
0x825A2158:  lbzx         r29, r8, r27
0x825A215C:  blt          cr6, 0x825a2168
0x825A2160:  srw          r10, r30, r10
0x825A2164:  b            0x825a2178
0x825A2168:  subf         r7, r31, r10
0x825A216C:  addi         r28, r28, 4
0x825A2170:  addi         r11, r11, 0x20
0x825A2174:  srw          r10, r9, r7
0x825A2178:  cmplw        cr6, r11, r23
0x825A217C:  blt          cr6, 0x825a21a0
0x825A2180:  and          r9, r5, r10
0x825A2184:  lbzx         r9, r9, r4
0x825A2188:  clrlwi       r8, r9, 0x1c
0x825A218C:  srwi         r9, r9, 4
0x825A2190:  subf         r31, r9, r11
0x825A2194:  lbzx         r7, r8, r26
0x825A2198:  srw          r30, r10, r9
0x825A219C:  b            0x825a21e8
0x825A21A0:  lwz          r8, 0(r28)
0x825A21A4:  slw          r7, r8, r11
0x825A21A8:  or           r9, r7, r10
0x825A21AC:  and          r7, r9, r5
0x825A21B0:  lbzx         r9, r7, r4
0x825A21B4:  clrlwi       r7, r9, 0x1c
0x825A21B8:  srwi         r9, r9, 4
0x825A21BC:  cmplw        cr6, r11, r9
0x825A21C0:  lbzx         r7, r7, r26
0x825A21C4:  blt          cr6, 0x825a21d4
0x825A21C8:  subf         r31, r9, r11
0x825A21CC:  srw          r30, r10, r9
0x825A21D0:  b            0x825a21e8
0x825A21D4:  subf         r10, r11, r9
0x825A21D8:  subf         r11, r9, r11
0x825A21DC:  addi         r28, r28, 4
0x825A21E0:  addi         r31, r11, 0x20
0x825A21E4:  srw          r30, r8, r10
0x825A21E8:  slwi         r9, r29, 4
0x825A21EC:  or           r11, r9, r7
0x825A21F0:  rlwinm       r8, r11, 0, 0x18, 0x18
0x825A21F4:  cmplwi       cr6, r8, 0
0x825A21F8:  beq          cr6, 0x825a2208
0x825A21FC:  clrlwi       r10, r11, 0x19
0x825A2200:  neg          r11, r10
0x825A2204:  b            0x825a220c
0x825A2208:  clrlwi       r11, r11, 0x19
0x825A220C:  addi         r9, r11, 0x80
0x825A2210:  cmpwi        cr6, r25, 0
0x825A2214:  stb          r9, 0(r24)
0x825A2218:  addi         r24, r24, 1
0x825A221C:  bgt          cr6, 0x825a20c4
0x825A2220:  cmpwi        cr6, r25, -0x16
0x825A2224:  bge          cr6, 0x825a2238
0x825A2228:  lwz          r3, 0x2c(r3)
0x825A222C:  subfic       r5, r25, -0x15
0x825A2230:  lbz          r4, 0(r3)
0x825A2234:  bl           0x8242fed0
0x825A2238:  stw          r29, 0x180(r27)
0x825A223C:  stw          r28, 4(r22)
0x825A2240:  stw          r30, 0(r22)
0x825A2244:  stw          r31, 8(r22)
0x825A2248:  addi         r1, r1, 0xb0
0x825A224C:  b            0x8242f8d0
0x825A2250:  addi         r7, r11, 4
0x825A2254:  stw          r11, 4(r3)
0x825A2258:  stw          r7, 0(r3)
0x825A225C:  stw          r28, 4(r22)
0x825A2260:  stw          r30, 0(r22)
0x825A2264:  stw          r31, 8(r22)
0x825A2268:  addi         r1, r1, 0xb0
0x825A226C:  b            0x8242f8d0
0x825A2270:  mflr         r12
0x825A2274:  bl           0x8242f87c
0x825A2278:  stwu         r1, -0xb0(r1)
0x825A227C:  lwz          r11, 0(r3)
0x825A2280:  mr           r21, r4
0x825A2284:  lwz          r10, 4(r3)
0x825A2288:  mr           r28, r5
0x825A228C:  cmplw        cr6, r11, r10
0x825A2290:  bne          cr6, 0x825a24e4
0x825A2294:  lwz          r11, 0x28(r3)
0x825A2298:  li           r5, -1
0x825A229C:  lwz          r10, 8(r21)
0x825A22A0:  lwz          r8, 0(r21)
0x825A22A4:  subfic       r7, r11, 0x20
0x825A22A8:  lwz          r29, 4(r21)
0x825A22AC:  cmplw        cr6, r10, r11
0x825A22B0:  bge          cr6, 0x825a22e0
0x825A22B4:  lwz          r9, 0(r29)
0x825A22B8:  subf         r6, r10, r11
0x825A22BC:  subf         r11, r11, r10
0x825A22C0:  srw          r4, r5, r7
0x825A22C4:  addi         r29, r29, 4
0x825A22C8:  addi         r31, r11, 0x20
0x825A22CC:  slw          r10, r9, r10
0x825A22D0:  srw          r30, r9, r6
0x825A22D4:  or           r8, r10, r8
0x825A22D8:  and          r24, r4, r8
0x825A22DC:  b            0x825a22f0
0x825A22E0:  srw          r6, r5, r7
0x825A22E4:  subf         r31, r11, r10
0x825A22E8:  and          r24, r6, r8
0x825A22EC:  srw          r30, r8, r11
0x825A22F0:  lwz          r11, 0x2c(r3)
0x825A22F4:  cmpwi        cr6, r24, 0
0x825A22F8:  beq          cr6, 0x825a24cc
0x825A22FC:  add          r4, r11, r24
0x825A2300:  stw          r11, 0(r3)
0x825A2304:  lwz          r22, 0x20(r3)
0x825A2308:  addi         r25, r3, 0x10
0x825A230C:  lwz          r26, 0x24(r3)
0x825A2310:  cmplwi       cr6, r31, 0
0x825A2314:  mr           r23, r11
0x825A2318:  stw          r4, 4(r3)
0x825A231C:  lwz          r7, 0x180(r28)
0x825A2320:  bne          cr6, 0x825a2338
0x825A2324:  lwz          r11, 0(r29)
0x825A2328:  li           r31, 0x1f
0x825A232C:  addi         r29, r29, 4
0x825A2330:  srwi         r30, r11, 1
0x825A2334:  b            0x825a2344
0x825A2338:  mr           r11, r30
0x825A233C:  srwi         r30, r30, 1
0x825A2340:  addi         r31, r31, -1
0x825A2344:  clrlwi       r11, r11, 0x1f
0x825A2348:  cmplwi       cr6, r11, 0
0x825A234C:  beq          cr6, 0x825a2354
0x825A2350:  subfic       r24, r24, -0x14
0x825A2354:  subfic       r10, r22, 0x20
0x825A2358:  srw          r4, r5, r10
0x825A235C:  addi         r9, r7, 0x40
0x825A2360:  addi         r24, r24, -1
0x825A2364:  slwi         r8, r9, 2
0x825A2368:  lwzx         r11, r8, r28
0x825A236C:  cmplw        cr6, r31, r11
0x825A2370:  blt          cr6, 0x825a23b0
0x825A2374:  subfic       r6, r11, 0x20
0x825A2378:  addi         r11, r7, 0x50
0x825A237C:  slwi         r9, r7, 4
0x825A2380:  slwi         r10, r11, 2
0x825A2384:  lwzx         r8, r10, r28
0x825A2388:  srw          r7, r5, r6
0x825A238C:  and          r6, r7, r30
0x825A2390:  lbzx         r11, r6, r8
0x825A2394:  clrlwi       r10, r11, 0x1c
0x825A2398:  srwi         r11, r11, 4
0x825A239C:  add          r10, r10, r9
0x825A23A0:  lbzx         r27, r10, r28
0x825A23A4:  srw          r10, r30, r11
0x825A23A8:  subf         r11, r11, r31
0x825A23AC:  b            0x825a2410
0x825A23B0:  lwz          r9, 0(r29)
0x825A23B4:  addi         r6, r7, 0x50
0x825A23B8:  subfic       r8, r11, 0x20
0x825A23BC:  slwi         r11, r6, 2
0x825A23C0:  slwi         r7, r7, 4
0x825A23C4:  lwzx         r10, r11, r28
0x825A23C8:  slw          r6, r9, r31
0x825A23CC:  srw          r8, r5, r8
0x825A23D0:  or           r11, r6, r30
0x825A23D4:  and          r8, r8, r11
0x825A23D8:  lbzx         r11, r8, r10
0x825A23DC:  clrlwi       r8, r11, 0x1c
0x825A23E0:  srwi         r10, r11, 4
0x825A23E4:  add          r7, r8, r7
0x825A23E8:  cmplw        cr6, r31, r10
0x825A23EC:  subf         r11, r10, r31
0x825A23F0:  lbzx         r27, r7, r28
0x825A23F4:  blt          cr6, 0x825a2400
0x825A23F8:  srw          r10, r30, r10
0x825A23FC:  b            0x825a2410
0x825A2400:  subf         r6, r31, r10
0x825A2404:  addi         r29, r29, 4
0x825A2408:  addi         r11, r11, 0x20
0x825A240C:  srw          r10, r9, r6
0x825A2410:  mr           r7, r27
0x825A2414:  cmplw        cr6, r11, r22
0x825A2418:  blt          cr6, 0x825a243c
0x825A241C:  and          r9, r4, r10
0x825A2420:  lbzx         r9, r9, r26
0x825A2424:  clrlwi       r8, r9, 0x1c
0x825A2428:  srwi         r9, r9, 4
0x825A242C:  subf         r31, r9, r11
0x825A2430:  lbzx         r6, r8, r25
0x825A2434:  srw          r30, r10, r9
0x825A2438:  b            0x825a2484
0x825A243C:  lwz          r8, 0(r29)
0x825A2440:  slw          r6, r8, r11
0x825A2444:  or           r9, r6, r10
0x825A2448:  and          r6, r9, r4
0x825A244C:  lbzx         r9, r6, r26
0x825A2450:  clrlwi       r6, r9, 0x1c
0x825A2454:  srwi         r9, r9, 4
0x825A2458:  cmplw        cr6, r11, r9
0x825A245C:  lbzx         r6, r6, r25
0x825A2460:  blt          cr6, 0x825a2470
0x825A2464:  subf         r31, r9, r11
0x825A2468:  srw          r30, r10, r9
0x825A246C:  b            0x825a2484
0x825A2470:  subf         r10, r11, r9
0x825A2474:  subf         r11, r9, r11
0x825A2478:  addi         r29, r29, 4
0x825A247C:  addi         r31, r11, 0x20
0x825A2480:  srw          r30, r8, r10
0x825A2484:  slwi         r9, r27, 4
0x825A2488:  cmpwi        cr6, r24, 0
0x825A248C:  or           r8, r9, r6
0x825A2490:  stb          r8, 0(r23)
0x825A2494:  addi         r23, r23, 1
0x825A2498:  bgt          cr6, 0x825a235c
0x825A249C:  cmpwi        cr6, r24, -0x16
0x825A24A0:  bge          cr6, 0x825a24b4
0x825A24A4:  lwz          r3, 0x2c(r3)
0x825A24A8:  subfic       r5, r24, -0x15
0x825A24AC:  lbz          r4, 0(r3)
0x825A24B0:  bl           0x8242fed0
0x825A24B4:  stw          r27, 0x180(r28)
0x825A24B8:  stw          r29, 4(r21)
0x825A24BC:  stw          r30, 0(r21)
0x825A24C0:  stw          r31, 8(r21)
0x825A24C4:  addi         r1, r1, 0xb0
0x825A24C8:  b            0x8242f8cc
0x825A24CC:  addi         r5, r11, 4
0x825A24D0:  stw          r11, 4(r3)
0x825A24D4:  stw          r5, 0(r3)
0x825A24D8:  stw          r29, 4(r21)
0x825A24DC:  stw          r30, 0(r21)
0x825A24E0:  stw          r31, 8(r21)
0x825A24E4:  addi         r1, r1, 0xb0
0x825A24E8:  b            0x8242f8cc
0x825A24EC:  .align 0                       # padding
0x825A24F0:  mflr         r12
0x825A24F4:  bl           0x8242f898
0x825A24F8:  stwu         r1, -0x80(r1)
0x825A24FC:  mr           r6, r3
0x825A2500:  mr           r28, r4
0x825A2504:  lwz          r11, 0(r6)
0x825A2508:  lwz          r10, 4(r6)
0x825A250C:  cmplw        cr6, r11, r10
0x825A2510:  bne          cr6, 0x825a26f4
0x825A2514:  lwz          r11, 0x28(r6)
0x825A2518:  li           r9, -1
0x825A251C:  lwz          r10, 8(r28)
0x825A2520:  lwz          r7, 0(r28)
0x825A2524:  subfic       r5, r11, 0x20
0x825A2528:  lwz          r29, 4(r28)
0x825A252C:  cmplw        cr6, r10, r11
0x825A2530:  bge          cr6, 0x825a2560
0x825A2534:  lwz          r8, 0(r29)
0x825A2538:  subf         r4, r10, r11
0x825A253C:  subf         r11, r11, r10
0x825A2540:  srw          r3, r9, r5
0x825A2544:  addi         r29, r29, 4
0x825A2548:  addi         r31, r11, 0x20
0x825A254C:  slw          r10, r8, r10
0x825A2550:  srw          r30, r8, r4
0x825A2554:  or           r7, r10, r7
0x825A2558:  and          r5, r3, r7
0x825A255C:  b            0x825a2570
0x825A2560:  srw          r4, r9, r5
0x825A2564:  subf         r31, r11, r10
0x825A2568:  and          r5, r4, r7
0x825A256C:  srw          r30, r7, r11
0x825A2570:  lwz          r11, 0x2c(r6)
0x825A2574:  cmplwi       cr6, r5, 0
0x825A2578:  beq          cr6, 0x825a26dc
0x825A257C:  add          r3, r11, r5
0x825A2580:  stw          r11, 0(r6)
0x825A2584:  cmplwi       cr6, r31, 0
0x825A2588:  stw          r3, 4(r6)
0x825A258C:  bne          cr6, 0x825a25a4
0x825A2590:  lwz          r10, 0(r29)
0x825A2594:  li           r31, 0x1f
0x825A2598:  addi         r29, r29, 4
0x825A259C:  srwi         r30, r10, 1
0x825A25A0:  b            0x825a25b0
0x825A25A4:  mr           r10, r30
0x825A25A8:  srwi         r30, r30, 1
0x825A25AC:  addi         r31, r31, -1
0x825A25B0:  clrlwi       r10, r10, 0x1f
0x825A25B4:  cmplwi       cr6, r10, 0
0x825A25B8:  bne          cr6, 0x825a2670
0x825A25BC:  lwz          r4, 0x20(r6)
0x825A25C0:  addi         r7, r6, 0x10
0x825A25C4:  lwz          r8, 0x24(r6)
0x825A25C8:  subfic       r6, r4, 0x20
0x825A25CC:  srw          r6, r9, r6
0x825A25D0:  addi         r5, r5, -1
0x825A25D4:  cmplw        cr6, r31, r4
0x825A25D8:  blt          cr6, 0x825a2604
0x825A25DC:  and          r3, r6, r30
0x825A25E0:  lbzx         r10, r3, r8
0x825A25E4:  clrlwi       r9, r10, 0x1c
0x825A25E8:  srwi         r10, r10, 4
0x825A25EC:  subf         r31, r10, r31
0x825A25F0:  lbzx         r3, r9, r7
0x825A25F4:  stb          r3, 0(r11)
0x825A25F8:  addi         r11, r11, 1
0x825A25FC:  srw          r30, r30, r10
0x825A2600:  b            0x825a2654
0x825A2604:  lwz          r9, 0(r29)
0x825A2608:  slw          r10, r9, r31
0x825A260C:  or           r3, r10, r30
0x825A2610:  and          r10, r3, r6
0x825A2614:  lbzx         r10, r10, r8
0x825A2618:  clrlwi       r3, r10, 0x1c
0x825A261C:  srwi         r10, r10, 4
0x825A2620:  cmplw        cr6, r31, r10
0x825A2624:  lbzx         r3, r3, r7
0x825A2628:  stb          r3, 0(r11)
0x825A262C:  addi         r11, r11, 1
0x825A2630:  blt          cr6, 0x825a2640
0x825A2634:  subf         r31, r10, r31
0x825A2638:  srw          r30, r30, r10
0x825A263C:  b            0x825a2654
0x825A2640:  subf         r3, r31, r10
0x825A2644:  subf         r10, r10, r31
0x825A2648:  addi         r29, r29, 4
0x825A264C:  addi         r31, r10, 0x20
0x825A2650:  srw          r30, r9, r3
0x825A2654:  cmplwi       cr6, r5, 0
0x825A2658:  bne          cr6, 0x825a25d0
0x825A265C:  stw          r29, 4(r28)
0x825A2660:  stw          r30, 0(r28)
0x825A2664:  stw          r31, 8(r28)
0x825A2668:  addi         r1, r1, 0x80
0x825A266C:  b            0x8242f8e8
0x825A2670:  cmplwi       cr6, r31, 4
0x825A2674:  bge          cr6, 0x825a26b4
0x825A2678:  lwz          r10, 0(r29)
0x825A267C:  subfic       r9, r31, 4
0x825A2680:  mr           r3, r11
0x825A2684:  addi         r29, r29, 4
0x825A2688:  slw          r8, r10, r31
0x825A268C:  addi         r31, r31, 0x1c
0x825A2690:  or           r7, r8, r30
0x825A2694:  srw          r30, r10, r9
0x825A2698:  clrlwi       r4, r7, 0x1c
0x825A269C:  bl           0x8242fed0
0x825A26A0:  stw          r29, 4(r28)
0x825A26A4:  stw          r30, 0(r28)
0x825A26A8:  stw          r31, 8(r28)
0x825A26AC:  addi         r1, r1, 0x80
0x825A26B0:  b            0x8242f8e8
0x825A26B4:  clrlwi       r4, r30, 0x1c
0x825A26B8:  mr           r3, r11
0x825A26BC:  srwi         r30, r30, 4
0x825A26C0:  addi         r31, r31, -4
0x825A26C4:  bl           0x8242fed0
0x825A26C8:  stw          r29, 4(r28)
0x825A26CC:  stw          r30, 0(r28)
0x825A26D0:  stw          r31, 8(r28)
0x825A26D4:  addi         r1, r1, 0x80
0x825A26D8:  b            0x8242f8e8
0x825A26DC:  addi         r5, r11, 4
0x825A26E0:  stw          r11, 4(r6)
0x825A26E4:  stw          r5, 0(r6)
0x825A26E8:  stw          r29, 4(r28)
0x825A26EC:  stw          r30, 0(r28)
0x825A26F0:  stw          r31, 8(r28)
0x825A26F4:  addi         r1, r1, 0x80
0x825A26F8:  b            0x8242f8e8
0x825A26FC:  .align 0                       # padding
0x825A2700:  mflr         r12
0x825A2704:  bl           0x8242f894
0x825A2708:  lwz          r11, 0(r3)
0x825A270C:  lwz          r10, 4(r3)
0x825A2710:  cmplw        cr6, r11, r10
0x825A2714:  bne          cr6, 0x825a28d0
0x825A2718:  lwz          r10, 0x28(r3)
0x825A271C:  li           r9, -1
0x825A2720:  lwz          r11, 8(r4)
0x825A2724:  lwz          r6, 0(r4)
0x825A2728:  lwz          r7, 4(r4)
0x825A272C:  cmplw        cr6, r11, r10
0x825A2730:  bge          cr6, 0x825a2764
0x825A2734:  lwz          r8, 0(r7)
0x825A2738:  subfic       r5, r10, 0x20
0x825A273C:  subf         r31, r11, r10
0x825A2740:  subf         r10, r10, r11
0x825A2744:  addi         r7, r7, 4
0x825A2748:  slw          r30, r8, r11
0x825A274C:  addi         r11, r10, 0x20
0x825A2750:  or           r10, r30, r6
0x825A2754:  srw          r5, r9, r5
0x825A2758:  and          r29, r5, r10
0x825A275C:  srw          r10, r8, r31
0x825A2760:  b            0x825a2778
0x825A2764:  subfic       r8, r10, 0x20
0x825A2768:  subf         r11, r10, r11
0x825A276C:  srw          r10, r6, r10
0x825A2770:  srw          r5, r9, r8
0x825A2774:  and          r29, r5, r6
0x825A2778:  cmplwi       cr6, r29, 0
0x825A277C:  beq          cr6, 0x825a28b4
0x825A2780:  lwz          r27, 0x20(r3)
0x825A2784:  addi         r31, r3, 0x10
0x825A2788:  lwz          r8, 0x2c(r3)
0x825A278C:  subfic       r6, r27, 0x20
0x825A2790:  lwz          r5, 0x24(r3)
0x825A2794:  add          r30, r8, r29
0x825A2798:  mr           r28, r8
0x825A279C:  stw          r8, 0(r3)
0x825A27A0:  stw          r30, 4(r3)
0x825A27A4:  srw          r3, r9, r6
0x825A27A8:  addi         r29, r29, -1
0x825A27AC:  cmplw        cr6, r11, r27
0x825A27B0:  blt          cr6, 0x825a27d4
0x825A27B4:  and          r9, r3, r10
0x825A27B8:  lbzx         r9, r9, r5
0x825A27BC:  clrlwi       r8, r9, 0x1c
0x825A27C0:  srwi         r9, r9, 4
0x825A27C4:  subf         r11, r9, r11
0x825A27C8:  lbzx         r30, r8, r31
0x825A27CC:  srw          r10, r10, r9
0x825A27D0:  b            0x825a281c
0x825A27D4:  lwz          r8, 0(r7)
0x825A27D8:  slw          r6, r8, r11
0x825A27DC:  or           r9, r6, r10
0x825A27E0:  and          r6, r9, r3
0x825A27E4:  lbzx         r9, r6, r5
0x825A27E8:  clrlwi       r6, r9, 0x1c
0x825A27EC:  srwi         r9, r9, 4
0x825A27F0:  cmplw        cr6, r11, r9
0x825A27F4:  lbzx         r30, r6, r31
0x825A27F8:  blt          cr6, 0x825a2808
0x825A27FC:  subf         r11, r9, r11
0x825A2800:  srw          r10, r10, r9
0x825A2804:  b            0x825a281c
0x825A2808:  subf         r10, r11, r9
0x825A280C:  subf         r11, r9, r11
0x825A2810:  addi         r7, r7, 4
0x825A2814:  addi         r11, r11, 0x20
0x825A2818:  srw          r10, r8, r10
0x825A281C:  cmplw        cr6, r11, r27
0x825A2820:  blt          cr6, 0x825a2844
0x825A2824:  and          r9, r3, r10
0x825A2828:  lbzx         r9, r9, r5
0x825A282C:  clrlwi       r8, r9, 0x1c
0x825A2830:  srwi         r9, r9, 4
0x825A2834:  subf         r11, r9, r11
0x825A2838:  lbzx         r6, r8, r31
0x825A283C:  srw          r10, r10, r9
0x825A2840:  b            0x825a288c
0x825A2844:  lwz          r8, 0(r7)
0x825A2848:  slw          r6, r8, r11
0x825A284C:  or           r9, r6, r10
0x825A2850:  and          r6, r9, r3
0x825A2854:  lbzx         r9, r6, r5
0x825A2858:  clrlwi       r6, r9, 0x1c
0x825A285C:  srwi         r9, r9, 4
0x825A2860:  cmplw        cr6, r11, r9
0x825A2864:  lbzx         r6, r6, r31
0x825A2868:  blt          cr6, 0x825a2878
0x825A286C:  subf         r11, r9, r11
0x825A2870:  srw          r10, r10, r9
0x825A2874:  b            0x825a288c
0x825A2878:  subf         r10, r11, r9
0x825A287C:  subf         r11, r9, r11
0x825A2880:  addi         r7, r7, 4
0x825A2884:  addi         r11, r11, 0x20
0x825A2888:  srw          r10, r8, r10
0x825A288C:  slwi         r9, r6, 4
0x825A2890:  cmplwi       cr6, r29, 0
0x825A2894:  or           r8, r9, r30
0x825A2898:  stb          r8, 0(r28)
0x825A289C:  addi         r28, r28, 1
0x825A28A0:  bne          cr6, 0x825a27a8
0x825A28A4:  stw          r7, 4(r4)
0x825A28A8:  stw          r10, 0(r4)
0x825A28AC:  stw          r11, 8(r4)
0x825A28B0:  b            0x8242f8e4
0x825A28B4:  lwz          r9, 0x2c(r3)
0x825A28B8:  addi         r5, r9, 4
0x825A28BC:  stw          r9, 4(r3)
0x825A28C0:  stw          r5, 0(r3)
0x825A28C4:  stw          r7, 4(r4)
0x825A28C8:  stw          r10, 0(r4)
0x825A28CC:  stw          r11, 8(r4)
0x825A28D0:  b            0x8242f8e4
0x825A28D4:  .align 0                       # padding
0x825A28D8:  mflr         r12
0x825A28DC:  bl           0x8242f898
0x825A28E0:  stwu         r1, -0x80(r1)
0x825A28E4:  mr           r6, r3
0x825A28E8:  mr           r28, r4
0x825A28EC:  lwz          r11, 0(r6)
0x825A28F0:  lwz          r10, 4(r6)
0x825A28F4:  cmplw        cr6, r11, r10
0x825A28F8:  bne          cr6, 0x825a2b3c
0x825A28FC:  lwz          r11, 0x28(r6)
0x825A2900:  li           r9, -1
0x825A2904:  lwz          r10, 8(r28)
0x825A2908:  lwz          r7, 0(r28)
0x825A290C:  subfic       r5, r11, 0x20
0x825A2910:  lwz          r29, 4(r28)
0x825A2914:  cmplw        cr6, r10, r11
0x825A2918:  bge          cr6, 0x825a2948
0x825A291C:  lwz          r8, 0(r29)
0x825A2920:  subf         r4, r10, r11
0x825A2924:  subf         r11, r11, r10
0x825A2928:  srw          r3, r9, r5
0x825A292C:  addi         r29, r29, 4
0x825A2930:  addi         r31, r11, 0x20
0x825A2934:  slw          r10, r8, r10
0x825A2938:  srw          r30, r8, r4
0x825A293C:  or           r7, r10, r7
0x825A2940:  and          r5, r3, r7
0x825A2944:  b            0x825a2958
0x825A2948:  srw          r4, r9, r5
0x825A294C:  subf         r31, r11, r10
0x825A2950:  and          r5, r4, r7
0x825A2954:  srw          r30, r7, r11
0x825A2958:  cmplwi       cr6, r5, 0
0x825A295C:  beq          cr6, 0x825a2b20
0x825A2960:  lwz          r3, 0x2c(r6)
0x825A2964:  cmplwi       cr6, r31, 0
0x825A2968:  add          r11, r3, r5
0x825A296C:  stw          r3, 0(r6)
0x825A2970:  stw          r11, 4(r6)
0x825A2974:  bne          cr6, 0x825a298c
0x825A2978:  lwz          r11, 0(r29)
0x825A297C:  li           r31, 0x1f
0x825A2980:  addi         r29, r29, 4
0x825A2984:  srwi         r30, r11, 1
0x825A2988:  b            0x825a2998
0x825A298C:  mr           r11, r30
0x825A2990:  srwi         r30, r30, 1
0x825A2994:  addi         r31, r31, -1
0x825A2998:  clrlwi       r10, r11, 0x1f
0x825A299C:  cmplwi       cr6, r10, 0
0x825A29A0:  bne          cr6, 0x825a2a90
0x825A29A4:  lwz          r4, 0x20(r6)
0x825A29A8:  addi         r7, r6, 0x10
0x825A29AC:  lwz          r8, 0x24(r6)
0x825A29B0:  subfic       r6, r4, 0x20
0x825A29B4:  srw          r6, r9, r6
0x825A29B8:  addi         r5, r5, -1
0x825A29BC:  cmplw        cr6, r31, r4
0x825A29C0:  blt          cr6, 0x825a29e4
0x825A29C4:  and          r11, r6, r30
0x825A29C8:  lbzx         r11, r11, r8
0x825A29CC:  clrlwi       r10, r11, 0x1c
0x825A29D0:  srwi         r11, r11, 4
0x825A29D4:  subf         r31, r11, r31
0x825A29D8:  lbzx         r9, r10, r7
0x825A29DC:  srw          r30, r30, r11
0x825A29E0:  b            0x825a2a2c
0x825A29E4:  lwz          r10, 0(r29)
0x825A29E8:  slw          r9, r10, r31
0x825A29EC:  or           r11, r9, r30
0x825A29F0:  and          r9, r11, r6
0x825A29F4:  lbzx         r11, r9, r8
0x825A29F8:  clrlwi       r9, r11, 0x1c
0x825A29FC:  srwi         r11, r11, 4
0x825A2A00:  cmplw        cr6, r31, r11
0x825A2A04:  lbzx         r9, r9, r7
0x825A2A08:  blt          cr6, 0x825a2a18
0x825A2A0C:  subf         r31, r11, r31
0x825A2A10:  srw          r30, r30, r11
0x825A2A14:  b            0x825a2a2c
0x825A2A18:  subf         r30, r31, r11
0x825A2A1C:  subf         r11, r11, r31
0x825A2A20:  addi         r29, r29, 4
0x825A2A24:  addi         r31, r11, 0x20
0x825A2A28:  srw          r30, r10, r30
0x825A2A2C:  cmpwi        cr6, r9, 0
0x825A2A30:  beq          cr6, 0x825a2a6c
0x825A2A34:  cmplwi       cr6, r31, 0
0x825A2A38:  bne          cr6, 0x825a2a50
0x825A2A3C:  lwz          r11, 0(r29)
0x825A2A40:  li           r31, 0x1f
0x825A2A44:  addi         r29, r29, 4
0x825A2A48:  srwi         r30, r11, 1
0x825A2A4C:  b            0x825a2a5c
0x825A2A50:  mr           r11, r30
0x825A2A54:  srwi         r30, r30, 1
0x825A2A58:  addi         r31, r31, -1
0x825A2A5C:  clrlwi       r11, r11, 0x1f
0x825A2A60:  cmplwi       cr6, r11, 0
0x825A2A64:  beq          cr6, 0x825a2a6c
0x825A2A68:  neg          r9, r9
0x825A2A6C:  stb          r9, 0(r3)
0x825A2A70:  cmplwi       cr6, r5, 0
0x825A2A74:  addi         r3, r3, 1
0x825A2A78:  bne          cr6, 0x825a29b8
0x825A2A7C:  stw          r29, 4(r28)
0x825A2A80:  stw          r30, 0(r28)
0x825A2A84:  stw          r31, 8(r28)
0x825A2A88:  addi         r1, r1, 0x80
0x825A2A8C:  b            0x8242f8e8
0x825A2A90:  cmplwi       cr6, r31, 4
0x825A2A94:  bge          cr6, 0x825a2abc
0x825A2A98:  lwz          r11, 0(r29)
0x825A2A9C:  subfic       r9, r31, 4
0x825A2AA0:  addi         r29, r29, 4
0x825A2AA4:  slw          r8, r11, r31
0x825A2AA8:  addi         r31, r31, 0x1c
0x825A2AAC:  or           r7, r8, r30
0x825A2AB0:  srw          r30, r11, r9
0x825A2AB4:  clrlwi       r4, r7, 0x1c
0x825A2AB8:  b            0x825a2ac8
0x825A2ABC:  clrlwi       r4, r30, 0x1c
0x825A2AC0:  srwi         r30, r30, 4
0x825A2AC4:  addi         r31, r31, -4
0x825A2AC8:  cmpwi        cr6, r4, 0
0x825A2ACC:  beq          cr6, 0x825a2b08
0x825A2AD0:  cmplwi       cr6, r31, 0
0x825A2AD4:  bne          cr6, 0x825a2aec
0x825A2AD8:  lwz          r11, 0(r29)
0x825A2ADC:  li           r31, 0x1f
0x825A2AE0:  addi         r29, r29, 4
0x825A2AE4:  srwi         r30, r11, 1
0x825A2AE8:  b            0x825a2af8
0x825A2AEC:  mr           r11, r30
0x825A2AF0:  srwi         r30, r30, 1
0x825A2AF4:  addi         r31, r31, -1
0x825A2AF8:  clrlwi       r6, r11, 0x1f
0x825A2AFC:  cmplwi       cr6, r6, 0
0x825A2B00:  beq          cr6, 0x825a2b08
0x825A2B04:  neg          r4, r4
0x825A2B08:  bl           0x8242fed0
0x825A2B0C:  stw          r29, 4(r28)
0x825A2B10:  stw          r30, 0(r28)
0x825A2B14:  stw          r31, 8(r28)
0x825A2B18:  addi         r1, r1, 0x80
0x825A2B1C:  b            0x8242f8e8
0x825A2B20:  lwz          r11, 0x2c(r6)
0x825A2B24:  addi         r5, r11, 4
0x825A2B28:  stw          r11, 4(r6)
0x825A2B2C:  stw          r5, 0(r6)
0x825A2B30:  stw          r29, 4(r28)
0x825A2B34:  stw          r30, 0(r28)
0x825A2B38:  stw          r31, 8(r28)
0x825A2B3C:  addi         r1, r1, 0x80
0x825A2B40:  b            0x8242f8e8
0x825A2B44:  .align 0                       # padding
0x825A2B48:  mflr         r12
0x825A2B4C:  bl           0x8242f87c
0x825A2B50:  stwu         r1, -0xb0(r1)
0x825A2B54:  lwz          r11, 0(r3)
0x825A2B58:  mr           r21, r4
0x825A2B5C:  lwz          r10, 4(r3)
0x825A2B60:  cmplw        cr6, r11, r10
0x825A2B64:  bne          cr6, 0x825a2e4c
0x825A2B68:  lwz          r11, 0x28(r3)
0x825A2B6C:  li           r24, -1
0x825A2B70:  lwz          r10, 8(r21)
0x825A2B74:  lwz          r8, 0(r21)
0x825A2B78:  lwz          r29, 4(r21)
0x825A2B7C:  cmplw        cr6, r10, r11
0x825A2B80:  bge          cr6, 0x825a2bb4
0x825A2B84:  lwz          r9, 0(r29)
0x825A2B88:  subfic       r7, r11, 0x20
0x825A2B8C:  subf         r6, r10, r11
0x825A2B90:  subf         r11, r11, r10
0x825A2B94:  addi         r29, r29, 4
0x825A2B98:  addi         r31, r11, 0x20
0x825A2B9C:  slw          r4, r9, r10
0x825A2BA0:  srw          r5, r24, r7
0x825A2BA4:  or           r11, r4, r8
0x825A2BA8:  srw          r30, r9, r6
0x825A2BAC:  and          r7, r5, r11
0x825A2BB0:  b            0x825a2bc8
0x825A2BB4:  subfic       r9, r11, 0x20
0x825A2BB8:  subf         r31, r11, r10
0x825A2BBC:  srw          r30, r8, r11
0x825A2BC0:  srw          r7, r24, r9
0x825A2BC4:  and          r7, r7, r8
0x825A2BC8:  cmplwi       cr6, r7, 0
0x825A2BCC:  beq          cr6, 0x825a2e30
0x825A2BD0:  lwz          r6, 0xc(r3)
0x825A2BD4:  lwz          r8, 0x2c(r3)
0x825A2BD8:  lwz          r11, 8(r3)
0x825A2BDC:  cmpwi        cr6, r6, 0
0x825A2BE0:  beq          cr6, 0x825a2c80
0x825A2BE4:  addi         r10, r11, -1
0x825A2BE8:  cmplw        cr6, r31, r10
0x825A2BEC:  bge          cr6, 0x825a2c24
0x825A2BF0:  lwz          r10, 0(r29)
0x825A2BF4:  subfic       r5, r11, 0x21
0x825A2BF8:  subf         r9, r31, r11
0x825A2BFC:  subf         r11, r11, r31
0x825A2C00:  addi         r4, r9, -1
0x825A2C04:  addi         r29, r29, 4
0x825A2C08:  slw          r6, r10, r31
0x825A2C0C:  srw          r9, r24, r5
0x825A2C10:  or           r5, r6, r30
0x825A2C14:  addi         r31, r11, 0x21
0x825A2C18:  and          r25, r9, r5
0x825A2C1C:  srw          r30, r10, r4
0x825A2C20:  b            0x825a2c3c
0x825A2C24:  subfic       r4, r11, 0x21
0x825A2C28:  subf         r11, r11, r31
0x825A2C2C:  addi         r31, r11, 1
0x825A2C30:  srw          r11, r24, r4
0x825A2C34:  and          r25, r11, r30
0x825A2C38:  srw          r30, r30, r10
0x825A2C3C:  cmpwi        cr6, r25, 0
0x825A2C40:  beq          cr6, 0x825a2ccc
0x825A2C44:  cmplwi       cr6, r31, 0
0x825A2C48:  bne          cr6, 0x825a2c60
0x825A2C4C:  lwz          r11, 0(r29)
0x825A2C50:  li           r31, 0x1f
0x825A2C54:  addi         r29, r29, 4
0x825A2C58:  srwi         r30, r11, 1
0x825A2C5C:  b            0x825a2c6c
0x825A2C60:  mr           r11, r30
0x825A2C64:  srwi         r30, r30, 1
0x825A2C68:  addi         r31, r31, -1
0x825A2C6C:  clrlwi       r10, r11, 0x1f
0x825A2C70:  cmplwi       cr6, r10, 0
0x825A2C74:  beq          cr6, 0x825a2ccc
0x825A2C78:  neg          r25, r25
0x825A2C7C:  b            0x825a2ccc
0x825A2C80:  cmplw        cr6, r31, r11
0x825A2C84:  bge          cr6, 0x825a2cb8
0x825A2C88:  lwz          r10, 0(r29)
0x825A2C8C:  subfic       r9, r11, 0x20
0x825A2C90:  subf         r6, r31, r11
0x825A2C94:  subf         r11, r11, r31
0x825A2C98:  addi         r29, r29, 4
0x825A2C9C:  slw          r4, r10, r31
0x825A2CA0:  addi         r31, r11, 0x20
0x825A2CA4:  or           r11, r4, r30
0x825A2CA8:  srw          r5, r24, r9
0x825A2CAC:  srw          r30, r10, r6
0x825A2CB0:  and          r25, r5, r11
0x825A2CB4:  b            0x825a2ccc
0x825A2CB8:  subfic       r10, r11, 0x20
0x825A2CBC:  subf         r31, r11, r31
0x825A2CC0:  srw          r9, r24, r10
0x825A2CC4:  and          r25, r9, r30
0x825A2CC8:  srw          r30, r30, r11
0x825A2CCC:  clrlwi       r28, r25, 0x10
0x825A2CD0:  slwi         r10, r7, 1
0x825A2CD4:  addi         r26, r8, 2
0x825A2CD8:  addi         r22, r7, -1
0x825A2CDC:  sth          r28, 0(r8)
0x825A2CE0:  cmplwi       cr6, r22, 0
0x825A2CE4:  lwz          r11, 0x2c(r3)
0x825A2CE8:  add          r8, r10, r11
0x825A2CEC:  stw          r11, 0(r3)
0x825A2CF0:  stw          r8, 4(r3)
0x825A2CF4:  beq          cr6, 0x825a2e40
0x825A2CF8:  cmplwi       cr6, r22, 8
0x825A2CFC:  li           r23, 8
0x825A2D00:  bgt          cr6, 0x825a2d08
0x825A2D04:  mr           r23, r22
0x825A2D08:  cmplwi       cr6, r31, 4
0x825A2D0C:  bge          cr6, 0x825a2d34
0x825A2D10:  lwz          r11, 0(r29)
0x825A2D14:  subfic       r7, r31, 4
0x825A2D18:  addi         r29, r29, 4
0x825A2D1C:  slw          r6, r11, r31
0x825A2D20:  addi         r31, r31, 0x1c
0x825A2D24:  or           r5, r6, r30
0x825A2D28:  srw          r30, r11, r7
0x825A2D2C:  clrlwi       r10, r5, 0x1c
0x825A2D30:  b            0x825a2d40
0x825A2D34:  clrlwi       r10, r30, 0x1c
0x825A2D38:  srwi         r30, r30, 4
0x825A2D3C:  addi         r31, r31, -4
0x825A2D40:  cmplwi       cr6, r10, 0
0x825A2D44:  beq          cr6, 0x825a2df8
0x825A2D48:  mr           r7, r23
0x825A2D4C:  cmpwi        cr6, r23, 0
0x825A2D50:  beq          cr6, 0x825a2e10
0x825A2D54:  subfic       r4, r10, 0x20
0x825A2D58:  srw          r8, r24, r4
0x825A2D5C:  addi         r7, r7, -1
0x825A2D60:  cmplw        cr6, r31, r10
0x825A2D64:  bge          cr6, 0x825a2d90
0x825A2D68:  lwz          r11, 0(r29)
0x825A2D6C:  subf         r3, r31, r10
0x825A2D70:  subf         r9, r10, r31
0x825A2D74:  addi         r29, r29, 4
0x825A2D78:  slw          r6, r11, r31
0x825A2D7C:  addi         r31, r9, 0x20
0x825A2D80:  or           r5, r6, r30
0x825A2D84:  srw          r30, r11, r3
0x825A2D88:  and          r9, r5, r8
0x825A2D8C:  b            0x825a2d9c
0x825A2D90:  and          r9, r8, r30
0x825A2D94:  subf         r31, r10, r31
0x825A2D98:  srw          r30, r30, r10
0x825A2D9C:  cmpwi        cr6, r9, 0
0x825A2DA0:  beq          cr6, 0x825a2ddc
0x825A2DA4:  cmplwi       cr6, r31, 0
0x825A2DA8:  bne          cr6, 0x825a2dc0
0x825A2DAC:  lwz          r11, 0(r29)
0x825A2DB0:  li           r31, 0x1f
0x825A2DB4:  addi         r29, r29, 4
0x825A2DB8:  srwi         r30, r11, 1
0x825A2DBC:  b            0x825a2dcc
0x825A2DC0:  mr           r11, r30
0x825A2DC4:  srwi         r30, r30, 1
0x825A2DC8:  addi         r31, r31, -1
0x825A2DCC:  clrlwi       r4, r11, 0x1f
0x825A2DD0:  cmplwi       cr6, r4, 0
0x825A2DD4:  beq          cr6, 0x825a2ddc
0x825A2DD8:  neg          r9, r9
0x825A2DDC:  add          r25, r9, r25
0x825A2DE0:  cmpwi        cr6, r7, 0
0x825A2DE4:  clrlwi       r28, r25, 0x10
0x825A2DE8:  sth          r28, 0(r26)
0x825A2DEC:  addi         r26, r26, 2
0x825A2DF0:  bne          cr6, 0x825a2d5c
0x825A2DF4:  b            0x825a2e10
0x825A2DF8:  slwi         r27, r23, 1
0x825A2DFC:  mr           r4, r28
0x825A2E00:  mr           r3, r26
0x825A2E04:  mr           r5, r27
0x825A2E08:  bl           0x825aa3c0
0x825A2E0C:  add          r26, r27, r26
0x825A2E10:  subf         r22, r23, r22
0x825A2E14:  cmplwi       cr6, r22, 0
0x825A2E18:  bne          cr6, 0x825a2cf8
0x825A2E1C:  stw          r29, 4(r21)
0x825A2E20:  stw          r30, 0(r21)
0x825A2E24:  stw          r31, 8(r21)
0x825A2E28:  addi         r1, r1, 0xb0
0x825A2E2C:  b            0x8242f8cc
0x825A2E30:  lwz          r11, 0x2c(r3)
0x825A2E34:  addi         r10, r11, 4
0x825A2E38:  stw          r11, 4(r3)
0x825A2E3C:  stw          r10, 0(r3)
0x825A2E40:  stw          r29, 4(r21)
0x825A2E44:  stw          r30, 0(r21)
0x825A2E48:  stw          r31, 8(r21)
0x825A2E4C:  addi         r1, r1, 0xb0
0x825A2E50:  b            0x8242f8cc
0x825A2E54:  .align 0                       # padding
0x825A2E58:  srwi         r11, r4, 4
0x825A2E5C:  srwi         r10, r4, 3
0x825A2E60:  addi         r11, r11, 0x203
0x825A2E64:  rlwinm       r9, r11, 0, 0, 0x1d
0x825A2E68:  slwi         r11, r10, 3
0x825A2E6C:  rlwinm       r10, r10, 1, 3, 0x1e
0x825A2E70:  addi         r8, r10, 0x403
0x825A2E74:  srwi         r10, r11, 3
0x825A2E78:  stw          r9, 4(r3)
0x825A2E7C:  rlwinm       r9, r8, 0, 0, 0x1d
0x825A2E80:  slwi         r8, r11, 1
0x825A2E84:  addi         r7, r10, 0x203
0x825A2E88:  add          r6, r11, r8
0x825A2E8C:  rlwinm       r10, r7, 0, 0, 0x1d
0x825A2E90:  rlwinm       r7, r11, 3, 3, 0x1c
0x825A2E94:  stw          r9, 0x18(r3)
0x825A2E98:  clrlwi       r8, r11, 3
0x825A2E9C:  stw          r9, 0x1c(r3)
0x825A2EA0:  rlwinm       r11, r6, 1, 3, 0x1e
0x825A2EA4:  addi         r5, r7, 0x203
0x825A2EA8:  addi         r4, r8, 0x203
0x825A2EAC:  stw          r10, 0(r3)
0x825A2EB0:  addi         r11, r11, 0x203
0x825A2EB4:  stw          r10, 0x10(r3)
0x825A2EB8:  rlwinm       r8, r5, 0, 0, 0x1d
0x825A2EBC:  stw          r10, 0x14(r3)
0x825A2EC0:  rlwinm       r7, r4, 0, 0, 0x1d
0x825A2EC4:  rlwinm       r6, r11, 0, 0, 0x1d
0x825A2EC8:  stw          r8, 8(r3)
0x825A2ECC:  stw          r7, 0xc(r3)
0x825A2ED0:  stw          r6, 0x20(r3)
0x825A2ED4:  blr          
0x825A2ED8:  mflr         r12
0x825A2EDC:  bl           0x8242f860
0x825A2EE0:  stwu         r1, -0x520(r1)
0x825A2EE4:  mr           r30, r7
0x825A2EE8:  lwz          r9, 0x58c(r1)
0x825A2EEC:  li           r19, 0
0x825A2EF0:  stw          r8, 0x5c(r1)
0x825A2EF4:  mr           r17, r5
0x825A2EF8:  stw          r8, 0x54(r1)
0x825A2EFC:  slwi         r11, r30, 3
0x825A2F00:  mr           r25, r6
0x825A2F04:  mr           r20, r4
0x825A2F08:  mr           r23, r10
0x825A2F0C:  stw          r19, 0x58(r1)
0x825A2F10:  subf         r27, r17, r11
0x825A2F14:  stw          r17, 0x544(r1)
0x825A2F18:  mr           r24, r19
0x825A2F1C:  stw          r19, 0x50(r1)
0x825A2F20:  rlwinm       r7, r9, 0, 0x10, 0x10
0x825A2F24:  stw          r25, 0x54c(r1)
0x825A2F28:  mr           r26, r3
0x825A2F2C:  stw          r20, 0x60(r1)
0x825A2F30:  stw          r23, 0x70(r1)
0x825A2F34:  cmplwi       cr6, r7, 0
0x825A2F38:  stw          r27, 0xb0(r1)
0x825A2F3C:  stw          r24, 0x6c(r1)
0x825A2F40:  stw          r19, 0x64(r1)
0x825A2F44:  beq          cr6, 0x825a2f58
0x825A2F48:  lis          r11, -0x7da6
0x825A2F4C:  addi         r6, r11, 0x1fd8
0x825A2F50:  stw          r6, 0x78(r1)
0x825A2F54:  b            0x825a2f64
0x825A2F58:  lis          r11, -0x7da6
0x825A2F5C:  addi         r5, r11, 0x2270
0x825A2F60:  stw          r5, 0x78(r1)
0x825A2F64:  lwz          r11, 0x584(r1)
0x825A2F68:  srwi         r9, r17, 4
0x825A2F6C:  srwi         r10, r17, 3
0x825A2F70:  stw          r19, 0x1e0(r1)
0x825A2F74:  addi         r7, r9, 0x1ff
0x825A2F78:  stw          r19, 0x1e4(r1)
0x825A2F7C:  li           r29, 0x10
0x825A2F80:  stw          r19, 0x1ec(r1)
0x825A2F84:  stw          r19, 0x150(r1)
0x825A2F88:  lwz          r4, 0(r11)
0x825A2F8C:  lwz          r6, 0xc(r11)
0x825A2F90:  lwz          r3, 4(r11)
0x825A2F94:  lwz          r8, 8(r11)
0x825A2F98:  lwz          r9, 0x18(r11)
0x825A2F9C:  stw          r4, 0x20c(r1)
0x825A2FA0:  clrlwi       r4, r7, 0x10
0x825A2FA4:  lwz          r7, 0x1c(r11)
0x825A2FA8:  stw          r6, 0x1dc(r1)
0x825A2FAC:  li           r6, 0x400
0x825A2FB0:  stw          r3, 0x17c(r1)
0x825A2FB4:  lwz          r3, 0x14(r11)
0x825A2FB8:  lwz          r5, 0x10(r11)
0x825A2FBC:  stw          r7, 0x27c(r1)
0x825A2FC0:  li           r7, 8
0x825A2FC4:  stw          r9, 0x1ac(r1)
0x825A2FC8:  li           r9, 4
0x825A2FCC:  stw          r8, 0xac(r1)
0x825A2FD0:  cntlzw       r8, r4
0x825A2FD4:  addi         r4, r10, 0x1ff
0x825A2FD8:  stw          r6, 0x25c(r1)
0x825A2FDC:  slwi         r6, r10, 1
0x825A2FE0:  stw          r3, 0xec(r1)
0x825A2FE4:  stw          r7, 0x88(r1)
0x825A2FE8:  clrlwi       r3, r4, 0x10
0x825A2FEC:  stw          r7, 0x1b8(r1)
0x825A2FF0:  li           r7, 5
0x825A2FF4:  stw          r5, 0x11c(r1)
0x825A2FF8:  slwi         r5, r10, 6
0x825A2FFC:  add          r4, r10, r6
0x825A3000:  stw          r9, 0x1e8(r1)
0x825A3004:  stw          r9, 0x158(r1)
0x825A3008:  addi         r5, r5, 0x1ff
0x825A300C:  stw          r9, 0x128(r1)
0x825A3010:  cntlzw       r9, r3
0x825A3014:  stw          r7, 0xf8(r1)
0x825A3018:  stw          r7, 0xc8(r1)
0x825A301C:  li           r7, 0xb
0x825A3020:  stw          r19, 0x154(r1)
0x825A3024:  stw          r19, 0x15c(r1)
0x825A3028:  stw          r19, 0x80(r1)
0x825A302C:  stw          r19, 0x84(r1)
0x825A3030:  stw          r7, 0x188(r1)
0x825A3034:  stw          r7, 0x258(r1)
0x825A3038:  slwi         r7, r10, 3
0x825A303C:  clrlwi       r10, r5, 0x10
0x825A3040:  stw          r19, 0x8c(r1)
0x825A3044:  addi         r3, r7, 0x1ff
0x825A3048:  stw          r19, 0x1b0(r1)
0x825A304C:  slwi         r7, r4, 4
0x825A3050:  stw          r19, 0x1b4(r1)
0x825A3054:  clrlwi       r6, r3, 0x10
0x825A3058:  stw          r19, 0x1bc(r1)
0x825A305C:  addi         r5, r7, 0x1ff
0x825A3060:  stw          r19, 0xf0(r1)
0x825A3064:  cntlzw       r7, r6
0x825A3068:  stw          r19, 0xf4(r1)
0x825A306C:  clrlwi       r4, r5, 0x10
0x825A3070:  stw          r29, 0xfc(r1)
0x825A3074:  stw          r19, 0xc0(r1)
0x825A3078:  cntlzw       r10, r10
0x825A307C:  stw          r19, 0xc4(r1)
0x825A3080:  cntlzw       r6, r4
0x825A3084:  stw          r29, 0xcc(r1)
0x825A3088:  stw          r19, 0x180(r1)
0x825A308C:  stw          r19, 0x184(r1)
0x825A3090:  stw          r19, 0x18c(r1)
0x825A3094:  stw          r19, 0x250(r1)
0x825A3098:  stw          r19, 0x254(r1)
0x825A309C:  stw          r19, 0x120(r1)
0x825A30A0:  stw          r19, 0x124(r1)
0x825A30A4:  stw          r19, 0x12c(r1)
0x825A30A8:  subfic       r3, r9, 0x20
0x825A30AC:  subfic       r8, r8, 0x20
0x825A30B0:  subfic       r5, r10, 0x20
0x825A30B4:  subfic       r4, r7, 0x20
0x825A30B8:  stw          r3, 0x208(r1)
0x825A30BC:  subfic       r3, r9, 0x20
0x825A30C0:  subfic       r10, r9, 0x20
0x825A30C4:  stw          r8, 0x178(r1)
0x825A30C8:  subfic       r8, r9, 0x20
0x825A30CC:  stw          r5, 0xa8(r1)
0x825A30D0:  subfic       r7, r9, 0x20
0x825A30D4:  lwz          r5, 0x20(r11)
0x825A30D8:  subfic       r6, r6, 0x20
0x825A30DC:  stw          r4, 0x1d8(r1)
0x825A30E0:  stw          r3, 0x118(r1)
0x825A30E4:  addi         r4, r1, 0x204
0x825A30E8:  addi         r3, r1, 0x50
0x825A30EC:  stw          r10, 0xe8(r1)
0x825A30F0:  stw          r8, 0x1a8(r1)
0x825A30F4:  stw          r5, 0x14c(r1)
0x825A30F8:  addi         r5, r1, 0x200
0x825A30FC:  stw          r6, 0x148(r1)
0x825A3100:  addi         r6, r1, 0x1f0
0x825A3104:  stw          r7, 0x278(r1)
0x825A3108:  bl           0x825a14f8
0x825A310C:  addi         r6, r1, 0x160
0x825A3110:  addi         r5, r1, 0x170
0x825A3114:  addi         r4, r1, 0x174
0x825A3118:  addi         r3, r1, 0x50
0x825A311C:  bl           0x825a14f8
0x825A3120:  addi         r31, r1, 0x440
0x825A3124:  addi         r28, r1, 0x300
0x825A3128:  mr           r6, r28
0x825A312C:  addi         r5, r31, -0x40
0x825A3130:  mr           r4, r31
0x825A3134:  addi         r3, r1, 0x50
0x825A3138:  bl           0x825a14f8
0x825A313C:  addi         r29, r29, -1
0x825A3140:  addi         r28, r28, 0x10
0x825A3144:  addi         r31, r31, 4
0x825A3148:  cmplwi       cr6, r29, 0
0x825A314C:  bne          cr6, 0x825a3128
0x825A3150:  addi         r6, r1, 0x90
0x825A3154:  addi         r5, r1, 0xa0
0x825A3158:  addi         r4, r1, 0xa4
0x825A315C:  addi         r3, r1, 0x50
0x825A3160:  bl           0x825a14f8
0x825A3164:  addi         r6, r1, 0x1c0
0x825A3168:  addi         r5, r1, 0x1d0
0x825A316C:  stw          r19, 0x480(r1)
0x825A3170:  addi         r4, r1, 0x1d4
0x825A3174:  addi         r3, r1, 0x50
0x825A3178:  bl           0x825a14f8
0x825A317C:  addi         r6, r1, 0x100
0x825A3180:  addi         r5, r1, 0x110
0x825A3184:  addi         r4, r1, 0x114
0x825A3188:  addi         r3, r1, 0x50
0x825A318C:  bl           0x825a14f8
0x825A3190:  addi         r6, r1, 0xd0
0x825A3194:  addi         r5, r1, 0xe0
0x825A3198:  addi         r4, r1, 0xe4
0x825A319C:  addi         r3, r1, 0x50
0x825A31A0:  bl           0x825a14f8
0x825A31A4:  addi         r6, r1, 0x130
0x825A31A8:  addi         r5, r1, 0x140
0x825A31AC:  addi         r4, r1, 0x144
0x825A31B0:  addi         r3, r1, 0x50
0x825A31B4:  bl           0x825a14f8
0x825A31B8:  mr           r14, r19
0x825A31BC:  cmplwi       cr6, r25, 0
0x825A31C0:  stw          r14, 0x68(r1)
0x825A31C4:  beq          cr6, 0x825a4dbc
0x825A31C8:  lis          r11, -0x7df5
0x825A31CC:  lwz          r15, 0x57c(r1)
0x825A31D0:  lwz          r29, 0x574(r1)
0x825A31D4:  li           r28, 1
0x825A31D8:  addi         r31, r11, 0x1bf8
0x825A31DC:  lis          r11, -0x7df5
0x825A31E0:  addi         r4, r11, 0x5d08
0x825A31E4:  stw          r4, 0x7c(r1)
0x825A31E8:  addi         r4, r1, 0x50
0x825A31EC:  addi         r3, r1, 0x1e0
0x825A31F0:  bl           0x825a1d00
0x825A31F4:  addi         r4, r1, 0x50
0x825A31F8:  addi         r3, r1, 0x150
0x825A31FC:  bl           0x825a1d00
0x825A3200:  lwz          r11, 0x78(r1)
0x825A3204:  addi         r5, r1, 0x300
0x825A3208:  addi         r4, r1, 0x50
0x825A320C:  addi         r3, r1, 0x80
0x825A3210:  mtctr        r11
0x825A3214:  bctrl        
0x825A3218:  addi         r4, r1, 0x50
0x825A321C:  addi         r3, r1, 0x1b0
0x825A3220:  bl           0x825a2700
0x825A3224:  addi         r4, r1, 0x50
0x825A3228:  addi         r3, r1, 0xf0
0x825A322C:  bl           0x825a28d8
0x825A3230:  addi         r4, r1, 0x50
0x825A3234:  addi         r3, r1, 0xc0
0x825A3238:  bl           0x825a28d8
0x825A323C:  addi         r4, r1, 0x50
0x825A3240:  addi         r3, r1, 0x180
0x825A3244:  bl           0x825a2b48
0x825A3248:  addi         r4, r1, 0x50
0x825A324C:  addi         r3, r1, 0x250
0x825A3250:  bl           0x825a2b48
0x825A3254:  addi         r4, r1, 0x50
0x825A3258:  addi         r3, r1, 0x120
0x825A325C:  bl           0x825a24f0
0x825A3260:  lwz          r3, 0x54(r1)
0x825A3264:  stw          r17, 0x74(r1)
0x825A3268:  cmplwi       cr6, r17, 0
0x825A326C:  beq          cr6, 0x825a4d68
0x825A3270:  lwz          r25, 0x58(r1)
0x825A3274:  lwz          r29, 0x50(r1)
0x825A3278:  lwz          r11, 0x80(r1)
0x825A327C:  lwz          r27, 0x1b0(r1)
0x825A3280:  lwz          r21, 0xf0(r1)
0x825A3284:  lwz          r22, 0xc0(r1)
0x825A3288:  lwz          r16, 0x180(r1)
0x825A328C:  lwz          r18, 0x120(r1)
0x825A3290:  lwz          r9, 0x1e0(r1)
0x825A3294:  lbz          r10, 0(r9)
0x825A3298:  addi         r9, r9, 1
0x825A329C:  cmplwi       cr6, r10, 9
0x825A32A0:  stw          r9, 0x1e0(r1)
0x825A32A4:  bgt          cr6, 0x825a4d2c
0x825A32A8:  lis          r12, -0x7da6
0x825A32AC:  addi         r12, r12, 0x32c0
0x825A32B0:  slwi         r0, r10, 2
0x825A32B4:  lwzx         r0, r12, r0
0x825A32B8:  mtctr        r0
0x825A32BC:  bctr         
0x825A32C0:  lwz          r18, 0x32e8(r26)
0x825A32C4:  lwz          r18, 0x3378(r26)
0x825A32C8:  lwz          r18, 0x3f50(r26)
0x825A32CC:  lwz          r18, 0x42f8(r26)
0x825A32D0:  lwz          r18, 0x4498(r26)
0x825A32D4:  lwz          r18, 0x4540(r26)
0x825A32D8:  lwz          r18, 0x45e4(r26)
0x825A32DC:  lwz          r18, 0x4664(r26)
0x825A32E0:  lwz          r18, 0x4740(r26)
0x825A32E4:  lwz          r18, 0x49f4(r26)
0x825A32E8:  add          r10, r20, r30
0x825A32EC:  lfd          f0, 0(r20)
0x825A32F0:  add          r9, r26, r30
0x825A32F4:  stfd         f0, 0(r26)
0x825A32F8:  add          r8, r10, r30
0x825A32FC:  srwi         r7, r24, 1
0x825A3300:  lfd          f13, 0(r10)
0x825A3304:  add          r10, r9, r30
0x825A3308:  stfd         f13, 0(r9)
0x825A330C:  add          r9, r8, r30
0x825A3310:  lfd          f12, 0(r8)
0x825A3314:  add          r8, r10, r30
0x825A3318:  stfd         f12, 0(r10)
0x825A331C:  add          r10, r9, r30
0x825A3320:  lfd          f11, 0(r9)
0x825A3324:  add          r9, r8, r30
0x825A3328:  stfd         f11, 0(r8)
0x825A332C:  add          r8, r10, r30
0x825A3330:  lfd          f10, 0(r10)
0x825A3334:  add          r10, r9, r30
0x825A3338:  stfd         f10, 0(r9)
0x825A333C:  add          r9, r8, r30
0x825A3340:  lfd          f9, 0(r8)
0x825A3344:  add          r8, r10, r30
0x825A3348:  stfd         f9, 0(r10)
0x825A334C:  lfd          f8, 0(r9)
0x825A3350:  stfd         f8, 0(r8)
0x825A3354:  lfdx         f7, r9, r30
0x825A3358:  stfdx        f7, r8, r30
0x825A335C:  lbzx         r6, r7, r23
0x825A3360:  cmplwi       cr6, r6, 0
0x825A3364:  bne          cr6, 0x825a4d2c
0x825A3368:  lwz          r5, 0x64(r1)
0x825A336C:  addi         r4, r5, 1
0x825A3370:  stw          r4, 0x64(r1)
0x825A3374:  b            0x825a4d2c
0x825A3378:  rlwinm       r10, r14, 0, 0x1c, 0x1c
0x825A337C:  cmplwi       cr6, r10, 0
0x825A3380:  bne          cr6, 0x825a3f2c
0x825A3384:  lwz          r9, 0x150(r1)
0x825A3388:  lbz          r10, 0(r9)
0x825A338C:  addi         r9, r9, 1
0x825A3390:  addi         r10, r10, -3
0x825A3394:  cmplwi       cr6, r10, 6
0x825A3398:  stw          r9, 0x150(r1)
0x825A339C:  bgt          cr6, 0x825a3f2c
0x825A33A0:  lis          r12, -0x7da6
0x825A33A4:  addi         r12, r12, 0x33b8
0x825A33A8:  slwi         r0, r10, 2
0x825A33AC:  lwzx         r0, r12, r0
0x825A33B0:  mtctr        r0
0x825A33B4:  bctr         
0x825A33B8:  lwz          r18, 0x33d4(r26)
0x825A33BC:  lwz          r18, 0x3f2c(r26)
0x825A33C0:  lwz          r18, 0x3740(r26)
0x825A33C4:  lwz          r18, 0x37dc(r26)
0x825A33C8:  lwz          r18, 0x3f2c(r26)
0x825A33CC:  lwz          r18, 0x3938(r26)
0x825A33D0:  lwz          r18, 0x3c7c(r26)
0x825A33D4:  cmplwi       cr6, r25, 4
0x825A33D8:  bge          cr6, 0x825a3400
0x825A33DC:  lwz          r10, 0(r3)
0x825A33E0:  subfic       r8, r25, 4
0x825A33E4:  addi         r4, r25, 0x1c
0x825A33E8:  addi         r3, r3, 4
0x825A33EC:  slw          r7, r10, r25
0x825A33F0:  srw          r5, r10, r8
0x825A33F4:  or           r6, r7, r29
0x825A33F8:  clrlwi       r9, r6, 0x1c
0x825A33FC:  b            0x825a340c
0x825A3400:  clrlwi       r9, r29, 0x1c
0x825A3404:  srwi         r5, r29, 4
0x825A3408:  addi         r4, r25, -4
0x825A340C:  lwz          r8, 0x7c(r1)
0x825A3410:  slwi         r9, r9, 6
0x825A3414:  mr           r10, r19
0x825A3418:  add          r7, r9, r8
0x825A341C:  cmplwi       cr6, r4, 0
0x825A3420:  bne          cr6, 0x825a3438
0x825A3424:  lwz          r9, 0(r3)
0x825A3428:  li           r4, 0x1f
0x825A342C:  addi         r3, r3, 4
0x825A3430:  srwi         r5, r9, 1
0x825A3434:  b            0x825a3444
0x825A3438:  mr           r9, r5
0x825A343C:  srwi         r5, r5, 1
0x825A3440:  addi         r4, r4, -1
0x825A3444:  clrlwi       r6, r9, 0x1f
0x825A3448:  cmplwi       cr6, r6, 0
0x825A344C:  beq          cr6, 0x825a3490
0x825A3450:  lbz          r8, 0(r11)
0x825A3454:  addi         r11, r11, 1
0x825A3458:  stw          r11, 0x80(r1)
0x825A345C:  lbz          r9, 0(r18)
0x825A3460:  addi         r18, r18, 1
0x825A3464:  addi         r9, r9, 1
0x825A3468:  cmpwi        cr6, r9, 0
0x825A346C:  beq          cr6, 0x825a34cc
0x825A3470:  lbzx         r6, r10, r7
0x825A3474:  addi         r29, r1, 0x210
0x825A3478:  addi         r9, r9, -1
0x825A347C:  addi         r10, r10, 1
0x825A3480:  cmpwi        cr6, r9, 0
0x825A3484:  stbx         r8, r6, r29
0x825A3488:  bne          cr6, 0x825a3470
0x825A348C:  b            0x825a34cc
0x825A3490:  lbz          r9, 0(r18)
0x825A3494:  addi         r18, r18, 1
0x825A3498:  addi         r9, r9, 1
0x825A349C:  cmpwi        cr6, r9, 0
0x825A34A0:  beq          cr6, 0x825a34cc
0x825A34A4:  lbz          r6, 0(r11)
0x825A34A8:  addi         r29, r1, 0x210
0x825A34AC:  lbzx         r8, r10, r7
0x825A34B0:  addi         r11, r11, 1
0x825A34B4:  addi         r9, r9, -1
0x825A34B8:  addi         r10, r10, 1
0x825A34BC:  cmpwi        cr6, r9, 0
0x825A34C0:  stw          r11, 0x80(r1)
0x825A34C4:  stbx         r6, r8, r29
0x825A34C8:  bne          cr6, 0x825a34a4
0x825A34CC:  cmpwi        cr6, r10, 0x3f
0x825A34D0:  blt          cr6, 0x825a341c
0x825A34D4:  stw          r18, 0x120(r1)
0x825A34D8:  bne          cr6, 0x825a34f4
0x825A34DC:  lbz          r10, 0(r11)
0x825A34E0:  addi         r8, r1, 0x210
0x825A34E4:  lbz          r9, 0x3f(r7)
0x825A34E8:  addi         r11, r11, 1
0x825A34EC:  stw          r11, 0x80(r1)
0x825A34F0:  stbx         r10, r9, r8
0x825A34F4:  mr           r9, r26
0x825A34F8:  add          r8, r26, r30
0x825A34FC:  addi         r10, r1, 0x214
0x825A3500:  li           r6, 2
0x825A3504:  lhz          r7, -4(r10)
0x825A3508:  rlwinm       r29, r7, 8, 0, 0xf
0x825A350C:  or           r29, r29, r7
0x825A3510:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3514:  mr           r29, r7
0x825A3518:  mr           r25, r7
0x825A351C:  lhz          r7, -2(r10)
0x825A3520:  stw          r29, 0(r9)
0x825A3524:  rlwinm       r29, r7, 8, 0, 0xf
0x825A3528:  stw          r25, 0(r8)
0x825A352C:  or           r29, r29, r7
0x825A3530:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3534:  mr           r29, r7
0x825A3538:  mr           r25, r7
0x825A353C:  lhz          r7, 0(r10)
0x825A3540:  stw          r29, 4(r9)
0x825A3544:  rlwinm       r29, r7, 8, 0, 0xf
0x825A3548:  stw          r25, 4(r8)
0x825A354C:  or           r29, r29, r7
0x825A3550:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3554:  mr           r29, r7
0x825A3558:  mr           r25, r7
0x825A355C:  lhz          r7, 2(r10)
0x825A3560:  stw          r29, 8(r9)
0x825A3564:  rlwinm       r29, r7, 8, 0, 0xf
0x825A3568:  stw          r25, 8(r8)
0x825A356C:  or           r29, r29, r7
0x825A3570:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3574:  mr           r29, r7
0x825A3578:  mr           r25, r7
0x825A357C:  lhz          r7, 4(r10)
0x825A3580:  stw          r29, 0xc(r9)
0x825A3584:  add          r9, r8, r30
0x825A3588:  stw          r25, 0xc(r8)
0x825A358C:  rlwinm       r8, r7, 8, 0, 0xf
0x825A3590:  or           r8, r8, r7
0x825A3594:  rlwimi       r7, r8, 8, 0, 0x17
0x825A3598:  add          r8, r9, r30
0x825A359C:  mr           r29, r7
0x825A35A0:  mr           r25, r7
0x825A35A4:  lhz          r7, 6(r10)
0x825A35A8:  stw          r29, 0(r9)
0x825A35AC:  rlwinm       r29, r7, 8, 0, 0xf
0x825A35B0:  stw          r25, 0(r8)
0x825A35B4:  or           r29, r29, r7
0x825A35B8:  rlwimi       r7, r29, 8, 0, 0x17
0x825A35BC:  mr           r29, r7
0x825A35C0:  mr           r25, r7
0x825A35C4:  lhz          r7, 8(r10)
0x825A35C8:  stw          r29, 4(r9)
0x825A35CC:  rlwinm       r29, r7, 8, 0, 0xf
0x825A35D0:  stw          r25, 4(r8)
0x825A35D4:  or           r29, r29, r7
0x825A35D8:  rlwimi       r7, r29, 8, 0, 0x17
0x825A35DC:  mr           r29, r7
0x825A35E0:  mr           r25, r7
0x825A35E4:  lhz          r7, 0xa(r10)
0x825A35E8:  stw          r29, 8(r9)
0x825A35EC:  rlwinm       r29, r7, 8, 0, 0xf
0x825A35F0:  stw          r25, 8(r8)
0x825A35F4:  or           r29, r29, r7
0x825A35F8:  rlwimi       r7, r29, 8, 0, 0x17
0x825A35FC:  mr           r29, r7
0x825A3600:  stw          r7, 0xc(r9)
0x825A3604:  add          r9, r8, r30
0x825A3608:  lhz          r7, 0xc(r10)
0x825A360C:  stw          r29, 0xc(r8)
0x825A3610:  rlwinm       r8, r7, 8, 0, 0xf
0x825A3614:  or           r29, r8, r7
0x825A3618:  add          r8, r9, r30
0x825A361C:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3620:  mr           r29, r7
0x825A3624:  mr           r25, r7
0x825A3628:  lhz          r7, 0xe(r10)
0x825A362C:  stw          r29, 0(r9)
0x825A3630:  rlwinm       r29, r7, 8, 0, 0xf
0x825A3634:  stw          r25, 0(r8)
0x825A3638:  or           r29, r29, r7
0x825A363C:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3640:  mr           r29, r7
0x825A3644:  mr           r25, r7
0x825A3648:  lhz          r7, 0x10(r10)
0x825A364C:  addi         r6, r6, -1
0x825A3650:  stw          r29, 4(r9)
0x825A3654:  rlwinm       r29, r7, 8, 0, 0xf
0x825A3658:  stw          r25, 4(r8)
0x825A365C:  cmplwi       cr6, r6, 0
0x825A3660:  or           r29, r29, r7
0x825A3664:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3668:  mr           r29, r7
0x825A366C:  mr           r25, r7
0x825A3670:  lhz          r7, 0x12(r10)
0x825A3674:  stw          r29, 8(r9)
0x825A3678:  rlwinm       r29, r7, 8, 0, 0xf
0x825A367C:  stw          r25, 8(r8)
0x825A3680:  or           r29, r29, r7
0x825A3684:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3688:  mr           r29, r7
0x825A368C:  mr           r25, r7
0x825A3690:  lhz          r7, 0x14(r10)
0x825A3694:  stw          r29, 0xc(r9)
0x825A3698:  add          r9, r8, r30
0x825A369C:  stw          r25, 0xc(r8)
0x825A36A0:  rlwinm       r8, r7, 8, 0, 0xf
0x825A36A4:  or           r8, r8, r7
0x825A36A8:  rlwimi       r7, r8, 8, 0, 0x17
0x825A36AC:  add          r8, r9, r30
0x825A36B0:  mr           r29, r7
0x825A36B4:  mr           r25, r7
0x825A36B8:  lhz          r7, 0x16(r10)
0x825A36BC:  stw          r29, 0(r9)
0x825A36C0:  rlwinm       r29, r7, 8, 0, 0xf
0x825A36C4:  stw          r25, 0(r8)
0x825A36C8:  or           r29, r29, r7
0x825A36CC:  rlwimi       r7, r29, 8, 0, 0x17
0x825A36D0:  mr           r29, r7
0x825A36D4:  mr           r25, r7
0x825A36D8:  lhz          r7, 0x18(r10)
0x825A36DC:  stw          r29, 4(r9)
0x825A36E0:  rlwinm       r29, r7, 8, 0, 0xf
0x825A36E4:  stw          r25, 4(r8)
0x825A36E8:  or           r29, r29, r7
0x825A36EC:  rlwimi       r7, r29, 8, 0, 0x17
0x825A36F0:  mr           r29, r7
0x825A36F4:  mr           r25, r7
0x825A36F8:  lhz          r7, 0x1a(r10)
0x825A36FC:  addi         r10, r10, 0x20
0x825A3700:  stw          r29, 8(r9)
0x825A3704:  rlwinm       r29, r7, 8, 0, 0xf
0x825A3708:  stw          r25, 8(r8)
0x825A370C:  or           r29, r29, r7
0x825A3710:  rlwimi       r7, r29, 8, 0, 0x17
0x825A3714:  stw          r7, 0xc(r9)
0x825A3718:  add          r9, r8, r30
0x825A371C:  stw          r7, 0xc(r8)
0x825A3720:  add          r8, r9, r30
0x825A3724:  bne          cr6, 0x825a3504
0x825A3728:  mr           r29, r5
0x825A372C:  stw          r3, 0x54(r1)
0x825A3730:  mr           r25, r4
0x825A3734:  stw          r29, 0x50(r1)
0x825A3738:  stw          r25, 0x58(r1)
0x825A373C:  b            0x825a3f2c
0x825A3740:  lhz          r7, 0(r16)
0x825A3744:  addi         r16, r16, 2
0x825A3748:  addi         r4, r1, 0x50
0x825A374C:  addi         r3, r1, 0x280
0x825A3750:  sth          r7, 0x280(r1)
0x825A3754:  stw          r16, 0x180(r1)
0x825A3758:  bl           0x825aae80
0x825A375C:  lwz          r11, 0x58(r1)
0x825A3760:  cmplwi       cr6, r11, 4
0x825A3764:  bge          cr6, 0x825a37a0
0x825A3768:  lwz          r9, 0x54(r1)
0x825A376C:  subfic       r5, r11, 4
0x825A3770:  addi         r4, r9, 4
0x825A3774:  lwz          r10, 0(r9)
0x825A3778:  lwz          r9, 0x50(r1)
0x825A377C:  stw          r4, 0x54(r1)
0x825A3780:  slw          r3, r10, r11
0x825A3784:  addi         r11, r11, 0x1c
0x825A3788:  srw          r7, r10, r5
0x825A378C:  or           r8, r3, r9
0x825A3790:  clrlwi       r6, r8, 0x1c
0x825A3794:  stw          r11, 0x58(r1)
0x825A3798:  stw          r7, 0x50(r1)
0x825A379C:  b            0x825a37b8
0x825A37A0:  lwz          r10, 0x50(r1)
0x825A37A4:  addi         r4, r11, -4
0x825A37A8:  srwi         r5, r10, 4
0x825A37AC:  clrlwi       r6, r10, 0x1c
0x825A37B0:  stw          r4, 0x58(r1)
0x825A37B4:  stw          r5, 0x50(r1)
0x825A37B8:  addi         r5, r1, 0x280
0x825A37BC:  mr           r4, r30
0x825A37C0:  mr           r3, r26
0x825A37C4:  bl           0x825aaae0
0x825A37C8:  lwz          r25, 0x58(r1)
0x825A37CC:  lwz          r3, 0x54(r1)
0x825A37D0:  lwz          r29, 0x50(r1)
0x825A37D4:  lwz          r11, 0x80(r1)
0x825A37D8:  b            0x825a3f2c
0x825A37DC:  lbz          r10, 0(r11)
0x825A37E0:  add          r9, r26, r30
0x825A37E4:  addi         r11, r11, 1
0x825A37E8:  rotlwi       r7, r10, 8
0x825A37EC:  add          r8, r9, r30
0x825A37F0:  or           r10, r7, r10
0x825A37F4:  slwi         r6, r10, 0x10
0x825A37F8:  stw          r11, 0x80(r1)
0x825A37FC:  or           r10, r6, r10
0x825A3800:  stw          r10, 0(r26)
0x825A3804:  stw          r10, 4(r26)
0x825A3808:  stw          r10, 8(r26)
0x825A380C:  stw          r10, 0xc(r26)
0x825A3810:  stw          r10, 0(r9)
0x825A3814:  stw          r10, 4(r9)
0x825A3818:  stw          r10, 8(r9)
0x825A381C:  stw          r10, 0xc(r9)
0x825A3820:  add          r9, r8, r30
0x825A3824:  stw          r10, 0(r8)
0x825A3828:  stw          r10, 4(r8)
0x825A382C:  stw          r10, 8(r8)
0x825A3830:  stw          r10, 0xc(r8)
0x825A3834:  add          r8, r9, r30
0x825A3838:  stw          r10, 0(r9)
0x825A383C:  stw          r10, 4(r9)
0x825A3840:  stw          r10, 8(r9)
0x825A3844:  stw          r10, 0xc(r9)
0x825A3848:  add          r9, r8, r30
0x825A384C:  stw          r10, 0(r8)
0x825A3850:  stw          r10, 4(r8)
0x825A3854:  stw          r10, 8(r8)
0x825A3858:  stw          r10, 0xc(r8)
0x825A385C:  add          r8, r9, r30
0x825A3860:  stw          r10, 0(r9)
0x825A3864:  stw          r10, 4(r9)
0x825A3868:  stw          r10, 8(r9)
0x825A386C:  stw          r10, 0xc(r9)
0x825A3870:  add          r9, r8, r30
0x825A3874:  stw          r10, 0(r8)
0x825A3878:  stw          r10, 4(r8)
0x825A387C:  stw          r10, 8(r8)
0x825A3880:  stw          r10, 0xc(r8)
0x825A3884:  add          r8, r9, r30
0x825A3888:  stw          r10, 0(r9)
0x825A388C:  stw          r10, 4(r9)
0x825A3890:  stw          r10, 8(r9)
0x825A3894:  stw          r10, 0xc(r9)
0x825A3898:  add          r9, r8, r30
0x825A389C:  stw          r10, 0(r8)
0x825A38A0:  stw          r10, 4(r8)
0x825A38A4:  stw          r10, 8(r8)
0x825A38A8:  stw          r10, 0xc(r8)
0x825A38AC:  add          r8, r9, r30
0x825A38B0:  stw          r10, 0(r9)
0x825A38B4:  stw          r10, 4(r9)
0x825A38B8:  stw          r10, 8(r9)
0x825A38BC:  stw          r10, 0xc(r9)
0x825A38C0:  add          r9, r8, r30
0x825A38C4:  stw          r10, 0(r8)
0x825A38C8:  stw          r10, 4(r8)
0x825A38CC:  stw          r10, 8(r8)
0x825A38D0:  stw          r10, 0xc(r8)
0x825A38D4:  add          r8, r9, r30
0x825A38D8:  stw          r10, 0(r9)
0x825A38DC:  stw          r10, 4(r9)
0x825A38E0:  stw          r10, 8(r9)
0x825A38E4:  stw          r10, 0xc(r9)
0x825A38E8:  add          r9, r8, r30
0x825A38EC:  stw          r10, 0(r8)
0x825A38F0:  stw          r10, 4(r8)
0x825A38F4:  stw          r10, 8(r8)
0x825A38F8:  stw          r10, 0xc(r8)
0x825A38FC:  add          r8, r9, r30
0x825A3900:  stw          r10, 0(r9)
0x825A3904:  stw          r10, 4(r9)
0x825A3908:  stw          r10, 8(r9)
0x825A390C:  stw          r10, 0xc(r9)
0x825A3910:  add          r9, r8, r30
0x825A3914:  stw          r10, 0(r8)
0x825A3918:  stw          r10, 4(r8)
0x825A391C:  stw          r10, 8(r8)
0x825A3920:  stw          r10, 0xc(r8)
0x825A3924:  stw          r10, 0(r9)
0x825A3928:  stw          r10, 4(r9)
0x825A392C:  stw          r10, 8(r9)
0x825A3930:  stw          r10, 0xc(r9)
0x825A3934:  b            0x825a3f2c
0x825A3938:  lbz          r10, 0(r11)
0x825A393C:  addi         r11, r11, 1
0x825A3940:  mr           r8, r26
0x825A3944:  rotlwi       r4, r10, 8
0x825A3948:  add          r7, r26, r30
0x825A394C:  or           r10, r4, r10
0x825A3950:  stw          r11, 0x80(r1)
0x825A3954:  li           r5, 2
0x825A3958:  lbz          r9, 0(r11)
0x825A395C:  addi         r11, r11, 1
0x825A3960:  slwi         r4, r10, 0x10
0x825A3964:  rotlwi       r6, r9, 8
0x825A3968:  or           r10, r4, r10
0x825A396C:  or           r9, r6, r9
0x825A3970:  stw          r11, 0x80(r1)
0x825A3974:  slwi         r6, r9, 0x10
0x825A3978:  or           r9, r6, r9
0x825A397C:  lbz          r6, 0(r27)
0x825A3980:  addi         r24, r31, 0x80
0x825A3984:  addi         r23, r31, 0x90
0x825A3988:  rlwinm       r29, r6, 2, 0x1c, 0x1d
0x825A398C:  rlwinm       r28, r6, 0, 0x1c, 0x1d
0x825A3990:  addi         r22, r31, 0x80
0x825A3994:  addi         r21, r31, 0x90
0x825A3998:  addi         r4, r27, 1
0x825A399C:  lwzx         r24, r29, r24
0x825A39A0:  rlwinm       r27, r6, 0x1e, 0x1c, 0x1d
0x825A39A4:  lwzx         r29, r29, r23
0x825A39A8:  addi         r20, r31, 0x80
0x825A39AC:  and          r24, r24, r9
0x825A39B0:  and          r29, r29, r10
0x825A39B4:  addi         r19, r31, 0x90
0x825A39B8:  or           r29, r24, r29
0x825A39BC:  rlwinm       r6, r6, 0x1c, 4, 0x1d
0x825A39C0:  addi         r18, r31, 0x80
0x825A39C4:  addi         r17, r31, 0x90
0x825A39C8:  addi         r16, r31, 0x80
0x825A39CC:  stw          r29, 0(r8)
0x825A39D0:  addi         r15, r31, 0x90
0x825A39D4:  stw          r29, 0(r7)
0x825A39D8:  addi         r14, r31, 0x80
0x825A39DC:  lwzx         r29, r28, r22
0x825A39E0:  addi         r24, r31, 0x90
0x825A39E4:  lwzx         r28, r28, r21
0x825A39E8:  addi         r23, r31, 0x80
0x825A39EC:  and          r29, r29, r9
0x825A39F0:  and          r28, r28, r10
0x825A39F4:  addi         r22, r31, 0x90
0x825A39F8:  or           r29, r29, r28
0x825A39FC:  addi         r21, r31, 0x90
0x825A3A00:  stw          r29, 4(r8)
0x825A3A04:  stw          r29, 4(r7)
0x825A3A08:  lwzx         r29, r27, r20
0x825A3A0C:  addi         r20, r31, 0x80
0x825A3A10:  lwzx         r28, r27, r19
0x825A3A14:  addi         r19, r31, 0x90
0x825A3A18:  and          r29, r29, r9
0x825A3A1C:  and          r28, r28, r10
0x825A3A20:  or           r29, r29, r28
0x825A3A24:  stw          r29, 8(r8)
0x825A3A28:  stw          r29, 8(r7)
0x825A3A2C:  lwzx         r29, r6, r18
0x825A3A30:  addi         r18, r31, 0x80
0x825A3A34:  lwzx         r6, r6, r17
0x825A3A38:  addi         r17, r31, 0x90
0x825A3A3C:  and          r29, r29, r9
0x825A3A40:  and          r6, r6, r10
0x825A3A44:  or           r6, r29, r6
0x825A3A48:  stw          r6, 0xc(r8)
0x825A3A4C:  add          r8, r7, r30
0x825A3A50:  stw          r6, 0xc(r7)
0x825A3A54:  lbz          r6, 0(r4)
0x825A3A58:  add          r7, r8, r30
0x825A3A5C:  addi         r4, r4, 1
0x825A3A60:  rlwinm       r29, r6, 2, 0x1c, 0x1d
0x825A3A64:  rlwinm       r28, r6, 0, 0x1c, 0x1d
0x825A3A68:  lwzx         r27, r29, r16
0x825A3A6C:  addi         r16, r31, 0x80
0x825A3A70:  lwzx         r29, r29, r15
0x825A3A74:  addi         r15, r31, 0x90
0x825A3A78:  and          r27, r27, r9
0x825A3A7C:  and          r29, r29, r10
0x825A3A80:  or           r29, r27, r29
0x825A3A84:  stw          r29, 0(r8)
0x825A3A88:  stw          r29, 0(r7)
0x825A3A8C:  lwzx         r29, r28, r14
0x825A3A90:  lwzx         r28, r28, r24
0x825A3A94:  addi         r24, r31, 0x80
0x825A3A98:  and          r27, r29, r9
0x825A3A9C:  and          r28, r28, r10
0x825A3AA0:  rlwinm       r29, r6, 0x1e, 0x1c, 0x1d
0x825A3AA4:  or           r28, r27, r28
0x825A3AA8:  rlwinm       r6, r6, 0x1c, 4, 0x1d
0x825A3AAC:  stw          r28, 4(r8)
0x825A3AB0:  stw          r28, 4(r7)
0x825A3AB4:  lwzx         r28, r29, r23
0x825A3AB8:  lwzx         r29, r29, r22
0x825A3ABC:  and          r28, r28, r9
0x825A3AC0:  and          r29, r29, r10
0x825A3AC4:  addi         r23, r31, 0x90
0x825A3AC8:  or           r29, r28, r29
0x825A3ACC:  addi         r22, r31, 0x80
0x825A3AD0:  addi         r5, r5, -1
0x825A3AD4:  cmplwi       cr6, r5, 0
0x825A3AD8:  stw          r29, 8(r8)
0x825A3ADC:  stw          r29, 8(r7)
0x825A3AE0:  lwzx         r29, r6, r24
0x825A3AE4:  lwzx         r6, r6, r21
0x825A3AE8:  addi         r21, r31, 0x90
0x825A3AEC:  and          r29, r29, r9
0x825A3AF0:  and          r6, r6, r10
0x825A3AF4:  or           r6, r29, r6
0x825A3AF8:  stw          r6, 0xc(r8)
0x825A3AFC:  add          r8, r7, r30
0x825A3B00:  stw          r6, 0xc(r7)
0x825A3B04:  lbz          r6, 0(r4)
0x825A3B08:  add          r7, r8, r30
0x825A3B0C:  addi         r4, r4, 1
0x825A3B10:  rlwinm       r29, r6, 2, 0x1c, 0x1d
0x825A3B14:  rlwinm       r28, r6, 0, 0x1c, 0x1d
0x825A3B18:  rlwinm       r27, r6, 0x1e, 0x1c, 0x1d
0x825A3B1C:  rlwinm       r6, r6, 0x1c, 4, 0x1d
0x825A3B20:  lwzx         r24, r29, r20
0x825A3B24:  addi         r20, r31, 0x80
0x825A3B28:  lwzx         r29, r29, r19
0x825A3B2C:  addi         r19, r31, 0x90
0x825A3B30:  and          r24, r24, r9
0x825A3B34:  and          r29, r29, r10
0x825A3B38:  or           r29, r24, r29
0x825A3B3C:  addi         r24, r31, 0x80
0x825A3B40:  stw          r29, 0(r8)
0x825A3B44:  stw          r29, 0(r7)
0x825A3B48:  lwzx         r29, r28, r18
0x825A3B4C:  addi         r18, r31, 0x80
0x825A3B50:  lwzx         r28, r28, r17
0x825A3B54:  addi         r17, r31, 0x90
0x825A3B58:  and          r29, r29, r9
0x825A3B5C:  and          r28, r28, r10
0x825A3B60:  or           r29, r29, r28
0x825A3B64:  stw          r29, 4(r8)
0x825A3B68:  stw          r29, 4(r7)
0x825A3B6C:  lwzx         r29, r27, r16
0x825A3B70:  lwzx         r28, r27, r15
0x825A3B74:  addi         r27, r31, 0x90
0x825A3B78:  and          r29, r29, r9
0x825A3B7C:  and          r28, r28, r10
0x825A3B80:  or           r29, r29, r28
0x825A3B84:  addi         r28, r31, 0x80
0x825A3B88:  stw          r29, 8(r8)
0x825A3B8C:  stw          r29, 8(r7)
0x825A3B90:  lwzx         r29, r6, r28
0x825A3B94:  lwzx         r6, r6, r27
0x825A3B98:  addi         r27, r4, 1
0x825A3B9C:  and          r29, r29, r9
0x825A3BA0:  and          r6, r6, r10
0x825A3BA4:  or           r6, r29, r6
0x825A3BA8:  stw          r6, 0xc(r8)
0x825A3BAC:  add          r8, r7, r30
0x825A3BB0:  stw          r6, 0xc(r7)
0x825A3BB4:  lbz          r6, 0(r4)
0x825A3BB8:  add          r7, r8, r30
0x825A3BBC:  rlwinm       r4, r6, 2, 0x1c, 0x1d
0x825A3BC0:  rlwinm       r29, r6, 0, 0x1c, 0x1d
0x825A3BC4:  rlwinm       r28, r6, 0x1e, 0x1c, 0x1d
0x825A3BC8:  rlwinm       r6, r6, 0x1c, 4, 0x1d
0x825A3BCC:  lwzx         r24, r4, r24
0x825A3BD0:  lwzx         r4, r4, r23
0x825A3BD4:  and          r24, r24, r9
0x825A3BD8:  and          r4, r4, r10
0x825A3BDC:  or           r4, r24, r4
0x825A3BE0:  stw          r4, 0(r8)
0x825A3BE4:  stw          r4, 0(r7)
0x825A3BE8:  lwzx         r4, r29, r22
0x825A3BEC:  lwzx         r29, r29, r21
0x825A3BF0:  and          r4, r4, r9
0x825A3BF4:  and          r29, r29, r10
0x825A3BF8:  or           r4, r4, r29
0x825A3BFC:  stw          r4, 4(r8)
0x825A3C00:  stw          r4, 4(r7)
0x825A3C04:  lwzx         r4, r28, r20
0x825A3C08:  lwzx         r29, r28, r19
0x825A3C0C:  and          r4, r4, r9
0x825A3C10:  and          r29, r29, r10
0x825A3C14:  or           r4, r4, r29
0x825A3C18:  stw          r4, 8(r8)
0x825A3C1C:  stw          r4, 8(r7)
0x825A3C20:  lwzx         r4, r6, r18
0x825A3C24:  lwzx         r6, r6, r17
0x825A3C28:  and          r4, r4, r9
0x825A3C2C:  and          r6, r6, r10
0x825A3C30:  or           r6, r4, r6
0x825A3C34:  stw          r6, 0xc(r8)
0x825A3C38:  add          r8, r7, r30
0x825A3C3C:  stw          r6, 0xc(r7)
0x825A3C40:  add          r7, r8, r30
0x825A3C44:  bne          cr6, 0x825a397c
0x825A3C48:  lwz          r15, 0x57c(r1)
0x825A3C4C:  li           r28, 1
0x825A3C50:  lwz          r23, 0x70(r1)
0x825A3C54:  li           r19, 0
0x825A3C58:  lwz          r20, 0x60(r1)
0x825A3C5C:  lwz          r24, 0x6c(r1)
0x825A3C60:  lwz          r29, 0x50(r1)
0x825A3C64:  lwz          r17, 0x74(r1)
0x825A3C68:  lwz          r18, 0x120(r1)
0x825A3C6C:  lwz          r16, 0x180(r1)
0x825A3C70:  lwz          r22, 0xc0(r1)
0x825A3C74:  lwz          r21, 0xf0(r1)
0x825A3C78:  b            0x825a3f2c
0x825A3C7C:  clrlwi       r5, r11, 0x1e
0x825A3C80:  add          r8, r26, r30
0x825A3C84:  mr           r10, r26
0x825A3C88:  slwi         r9, r30, 1
0x825A3C8C:  cmplwi       cr6, r5, 0
0x825A3C90:  beq          cr6, 0x825a3d5c
0x825A3C94:  subf         r4, r26, r8
0x825A3C98:  li           r7, 8
0x825A3C9C:  lbz          r6, 0(r11)
0x825A3CA0:  add          r8, r4, r10
0x825A3CA4:  lbz          r5, 1(r11)
0x825A3CA8:  addi         r7, r7, -1
0x825A3CAC:  rotlwi       r14, r6, 8
0x825A3CB0:  cmplwi       cr6, r7, 0
0x825A3CB4:  or           r6, r14, r6
0x825A3CB8:  slwi         r6, r6, 8
0x825A3CBC:  or           r6, r6, r5
0x825A3CC0:  slwi         r6, r6, 8
0x825A3CC4:  or           r6, r6, r5
0x825A3CC8:  stw          r6, 0(r10)
0x825A3CCC:  stw          r6, 0(r8)
0x825A3CD0:  lbz          r6, 2(r11)
0x825A3CD4:  lbz          r5, 3(r11)
0x825A3CD8:  rotlwi       r14, r6, 8
0x825A3CDC:  or           r6, r14, r6
0x825A3CE0:  slwi         r6, r6, 8
0x825A3CE4:  or           r6, r6, r5
0x825A3CE8:  slwi         r6, r6, 8
0x825A3CEC:  or           r6, r6, r5
0x825A3CF0:  stw          r6, 4(r10)
0x825A3CF4:  stw          r6, 4(r8)
0x825A3CF8:  lbz          r6, 4(r11)
0x825A3CFC:  lbz          r5, 5(r11)
0x825A3D00:  rotlwi       r14, r6, 8
0x825A3D04:  or           r6, r14, r6
0x825A3D08:  slwi         r6, r6, 8
0x825A3D0C:  or           r6, r6, r5
0x825A3D10:  slwi         r6, r6, 8
0x825A3D14:  or           r6, r6, r5
0x825A3D18:  stw          r6, 8(r10)
0x825A3D1C:  stw          r6, 8(r8)
0x825A3D20:  lbz          r6, 6(r11)
0x825A3D24:  lbz          r5, 7(r11)
0x825A3D28:  addi         r11, r11, 8
0x825A3D2C:  rotlwi       r14, r6, 8
0x825A3D30:  or           r6, r14, r6
0x825A3D34:  slwi         r6, r6, 8
0x825A3D38:  stw          r11, 0x80(r1)
0x825A3D3C:  or           r6, r6, r5
0x825A3D40:  slwi         r6, r6, 8
0x825A3D44:  or           r6, r6, r5
0x825A3D48:  stw          r6, 0xc(r10)
0x825A3D4C:  add          r10, r9, r10
0x825A3D50:  stw          r6, 0xc(r8)
0x825A3D54:  bne          cr6, 0x825a3c9c
0x825A3D58:  b            0x825a3f2c
0x825A3D5C:  li           r6, 2
0x825A3D60:  lhz          r7, 0(r11)
0x825A3D64:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3D68:  or           r4, r5, r7
0x825A3D6C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3D70:  stw          r7, 0(r10)
0x825A3D74:  stw          r7, 0(r8)
0x825A3D78:  lhz          r7, 2(r11)
0x825A3D7C:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3D80:  or           r4, r5, r7
0x825A3D84:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3D88:  stw          r7, 4(r10)
0x825A3D8C:  stw          r7, 4(r8)
0x825A3D90:  lhz          r7, 4(r11)
0x825A3D94:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3D98:  or           r4, r5, r7
0x825A3D9C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3DA0:  stw          r7, 8(r10)
0x825A3DA4:  stw          r7, 8(r8)
0x825A3DA8:  lhz          r7, 6(r11)
0x825A3DAC:  addi         r11, r11, 8
0x825A3DB0:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3DB4:  or           r4, r5, r7
0x825A3DB8:  stw          r11, 0x80(r1)
0x825A3DBC:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3DC0:  stw          r7, 0xc(r10)
0x825A3DC4:  add          r10, r9, r10
0x825A3DC8:  stw          r7, 0xc(r8)
0x825A3DCC:  add          r8, r9, r8
0x825A3DD0:  lhz          r7, 0(r11)
0x825A3DD4:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3DD8:  or           r4, r5, r7
0x825A3DDC:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3DE0:  stw          r7, 0(r10)
0x825A3DE4:  stw          r7, 0(r8)
0x825A3DE8:  lhz          r7, 2(r11)
0x825A3DEC:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3DF0:  or           r4, r5, r7
0x825A3DF4:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3DF8:  stw          r7, 4(r10)
0x825A3DFC:  stw          r7, 4(r8)
0x825A3E00:  lhz          r7, 4(r11)
0x825A3E04:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3E08:  or           r4, r5, r7
0x825A3E0C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3E10:  stw          r7, 8(r10)
0x825A3E14:  stw          r7, 8(r8)
0x825A3E18:  lhz          r7, 6(r11)
0x825A3E1C:  addi         r11, r11, 8
0x825A3E20:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3E24:  or           r4, r5, r7
0x825A3E28:  stw          r11, 0x80(r1)
0x825A3E2C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3E30:  stw          r7, 0xc(r10)
0x825A3E34:  add          r10, r9, r10
0x825A3E38:  stw          r7, 0xc(r8)
0x825A3E3C:  add          r8, r9, r8
0x825A3E40:  lhz          r7, 0(r11)
0x825A3E44:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3E48:  or           r4, r5, r7
0x825A3E4C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3E50:  stw          r7, 0(r10)
0x825A3E54:  stw          r7, 0(r8)
0x825A3E58:  lhz          r7, 2(r11)
0x825A3E5C:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3E60:  or           r4, r5, r7
0x825A3E64:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3E68:  stw          r7, 4(r10)
0x825A3E6C:  stw          r7, 4(r8)
0x825A3E70:  lhz          r7, 4(r11)
0x825A3E74:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3E78:  or           r4, r5, r7
0x825A3E7C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3E80:  stw          r7, 8(r10)
0x825A3E84:  stw          r7, 8(r8)
0x825A3E88:  lhz          r7, 6(r11)
0x825A3E8C:  addi         r11, r11, 8
0x825A3E90:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3E94:  or           r4, r5, r7
0x825A3E98:  stw          r11, 0x80(r1)
0x825A3E9C:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3EA0:  stw          r7, 0xc(r10)
0x825A3EA4:  stw          r7, 0xc(r8)
0x825A3EA8:  add          r10, r9, r10
0x825A3EAC:  lhz          r7, 0(r11)
0x825A3EB0:  add          r8, r9, r8
0x825A3EB4:  addi         r6, r6, -1
0x825A3EB8:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3EBC:  cmplwi       cr6, r6, 0
0x825A3EC0:  or           r4, r5, r7
0x825A3EC4:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3EC8:  stw          r7, 0(r10)
0x825A3ECC:  stw          r7, 0(r8)
0x825A3ED0:  lhz          r7, 2(r11)
0x825A3ED4:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3ED8:  or           r4, r5, r7
0x825A3EDC:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3EE0:  stw          r7, 4(r10)
0x825A3EE4:  stw          r7, 4(r8)
0x825A3EE8:  lhz          r7, 4(r11)
0x825A3EEC:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3EF0:  or           r4, r5, r7
0x825A3EF4:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3EF8:  stw          r7, 8(r10)
0x825A3EFC:  stw          r7, 8(r8)
0x825A3F00:  lhz          r7, 6(r11)
0x825A3F04:  addi         r11, r11, 8
0x825A3F08:  rlwinm       r5, r7, 8, 0, 0xf
0x825A3F0C:  or           r4, r5, r7
0x825A3F10:  stw          r11, 0x80(r1)
0x825A3F14:  rlwimi       r7, r4, 8, 0, 0x17
0x825A3F18:  stw          r7, 0xc(r10)
0x825A3F1C:  add          r10, r9, r10
0x825A3F20:  stw          r7, 0xc(r8)
0x825A3F24:  add          r8, r9, r8
0x825A3F28:  bne          cr6, 0x825a3d60
0x825A3F2C:  srwi         r10, r24, 1
0x825A3F30:  add          r24, r24, r15
0x825A3F34:  addi         r26, r26, 8
0x825A3F38:  srwi         r9, r24, 1
0x825A3F3C:  addi         r20, r20, 8
0x825A3F40:  stbx         r28, r10, r23
0x825A3F44:  addi         r17, r17, -8
0x825A3F48:  stbx         r28, r9, r23
0x825A3F4C:  b            0x825a4d2c
0x825A3F50:  srwi         r8, r24, 1
0x825A3F54:  add          r10, r26, r30
0x825A3F58:  stbx         r28, r8, r23
0x825A3F5C:  lbz          r7, 0(r22)
0x825A3F60:  addi         r22, r22, 1
0x825A3F64:  lbz          r6, 0(r21)
0x825A3F68:  addi         r21, r21, 1
0x825A3F6C:  extsb        r5, r7
0x825A3F70:  extsb        r8, r6
0x825A3F74:  mullw        r9, r5, r30
0x825A3F78:  stw          r22, 0xc0(r1)
0x825A3F7C:  stw          r21, 0xf0(r1)
0x825A3F80:  add          r9, r9, r8
0x825A3F84:  add          r8, r9, r20
0x825A3F88:  or           r7, r8, r26
0x825A3F8C:  add          r9, r8, r30
0x825A3F90:  clrlwi       r4, r7, 0x1d
0x825A3F94:  cmplwi       cr6, r4, 0
0x825A3F98:  bne          cr6, 0x825a4008
0x825A3F9C:  lfd          f6, 0(r8)
0x825A3FA0:  add          r8, r9, r30
0x825A3FA4:  stfd         f6, 0(r26)
0x825A3FA8:  add          r7, r10, r30
0x825A3FAC:  lfd          f5, 0(r9)
0x825A3FB0:  stfd         f5, 0(r10)
0x825A3FB4:  add          r10, r8, r30
0x825A3FB8:  add          r9, r7, r30
0x825A3FBC:  lfd          f4, 0(r8)
0x825A3FC0:  add          r8, r10, r30
0x825A3FC4:  stfd         f4, 0(r7)
0x825A3FC8:  lfd          f3, 0(r10)
0x825A3FCC:  add          r10, r9, r30
0x825A3FD0:  stfd         f3, 0(r9)
0x825A3FD4:  add          r9, r8, r30
0x825A3FD8:  lfd          f2, 0(r8)
0x825A3FDC:  add          r8, r10, r30
0x825A3FE0:  stfd         f2, 0(r10)
0x825A3FE4:  add          r10, r9, r30
0x825A3FE8:  lfd          f1, 0(r9)
0x825A3FEC:  add          r9, r8, r30
0x825A3FF0:  stfd         f1, 0(r8)
0x825A3FF4:  lfd          f0, 0(r10)
0x825A3FF8:  stfd         f0, 0(r9)
0x825A3FFC:  lfdx         f13, r10, r30
0x825A4000:  stfdx        f13, r9, r30
0x825A4004:  b            0x825a4d2c
0x825A4008:  clrlwi       r7, r7, 0x1e
0x825A400C:  cmplwi       cr6, r7, 0
0x825A4010:  add          r7, r10, r30
0x825A4014:  bne          cr6, 0x825a40c8
0x825A4018:  lwz          r6, 0(r8)
0x825A401C:  stw          r6, 0(r26)
0x825A4020:  lwz          r5, 4(r8)
0x825A4024:  add          r8, r9, r30
0x825A4028:  stw          r5, 4(r26)
0x825A402C:  lwz          r4, 0(r9)
0x825A4030:  stw          r4, 0(r10)
0x825A4034:  lwz          r9, 4(r9)
0x825A4038:  stw          r9, 4(r10)
0x825A403C:  add          r9, r8, r30
0x825A4040:  lwz          r6, 0(r8)
0x825A4044:  add          r10, r7, r30
0x825A4048:  stw          r6, 0(r7)
0x825A404C:  lwz          r5, 4(r8)
0x825A4050:  add          r8, r10, r30
0x825A4054:  stw          r5, 4(r7)
0x825A4058:  add          r7, r9, r30
0x825A405C:  lwz          r4, 0(r9)
0x825A4060:  stw          r4, 0(r10)
0x825A4064:  lwz          r9, 4(r9)
0x825A4068:  stw          r9, 4(r10)
0x825A406C:  add          r9, r7, r30
0x825A4070:  lwz          r6, 0(r7)
0x825A4074:  add          r10, r8, r30
0x825A4078:  stw          r6, 0(r8)
0x825A407C:  lwz          r5, 4(r7)
0x825A4080:  add          r7, r9, r30
0x825A4084:  stw          r5, 4(r8)
0x825A4088:  add          r8, r10, r30
0x825A408C:  lwz          r4, 0(r9)
0x825A4090:  stw          r4, 0(r10)
0x825A4094:  lwz          r9, 4(r9)
0x825A4098:  stw          r9, 4(r10)
0x825A409C:  add          r9, r7, r30
0x825A40A0:  lwz          r6, 0(r7)
0x825A40A4:  add          r10, r8, r30
0x825A40A8:  stw          r6, 0(r8)
0x825A40AC:  lwz          r5, 4(r7)
0x825A40B0:  stw          r5, 4(r8)
0x825A40B4:  lwz          r4, 0(r9)
0x825A40B8:  stw          r4, 0(r10)
0x825A40BC:  lwz          r9, 4(r9)
0x825A40C0:  stw          r9, 4(r10)
0x825A40C4:  b            0x825a4d2c
0x825A40C8:  lbz          r6, 0(r8)
0x825A40CC:  stb          r6, 0(r26)
0x825A40D0:  lbz          r5, 1(r8)
0x825A40D4:  stb          r5, 1(r26)
0x825A40D8:  lbz          r4, 2(r8)
0x825A40DC:  stb          r4, 2(r26)
0x825A40E0:  lbz          r6, 3(r8)
0x825A40E4:  stb          r6, 3(r26)
0x825A40E8:  lbz          r5, 4(r8)
0x825A40EC:  stb          r5, 4(r26)
0x825A40F0:  lbz          r4, 5(r8)
0x825A40F4:  stb          r4, 5(r26)
0x825A40F8:  lbz          r6, 6(r8)
0x825A40FC:  stb          r6, 6(r26)
0x825A4100:  lbz          r5, 7(r8)
0x825A4104:  add          r8, r9, r30
0x825A4108:  stb          r5, 7(r26)
0x825A410C:  lbz          r4, 0(r9)
0x825A4110:  stb          r4, 0(r10)
0x825A4114:  lbz          r6, 1(r9)
0x825A4118:  stb          r6, 1(r10)
0x825A411C:  lbz          r5, 2(r9)
0x825A4120:  stb          r5, 2(r10)
0x825A4124:  lbz          r4, 3(r9)
0x825A4128:  stb          r4, 3(r10)
0x825A412C:  lbz          r6, 4(r9)
0x825A4130:  stb          r6, 4(r10)
0x825A4134:  lbz          r5, 5(r9)
0x825A4138:  stb          r5, 5(r10)
0x825A413C:  lbz          r4, 6(r9)
0x825A4140:  stb          r4, 6(r10)
0x825A4144:  lbz          r6, 7(r9)
0x825A4148:  add          r9, r8, r30
0x825A414C:  stb          r6, 7(r10)
0x825A4150:  add          r10, r7, r30
0x825A4154:  lbz          r5, 0(r8)
0x825A4158:  stb          r5, 0(r7)
0x825A415C:  lbz          r4, 1(r8)
0x825A4160:  stb          r4, 1(r7)
0x825A4164:  lbz          r6, 2(r8)
0x825A4168:  stb          r6, 2(r7)
0x825A416C:  lbz          r5, 3(r8)
0x825A4170:  stb          r5, 3(r7)
0x825A4174:  lbz          r4, 4(r8)
0x825A4178:  stb          r4, 4(r7)
0x825A417C:  lbz          r6, 5(r8)
0x825A4180:  stb          r6, 5(r7)
0x825A4184:  lbz          r5, 6(r8)
0x825A4188:  stb          r5, 6(r7)
0x825A418C:  lbz          r4, 7(r8)
0x825A4190:  add          r8, r9, r30
0x825A4194:  stb          r4, 7(r7)
0x825A4198:  add          r7, r10, r30
0x825A419C:  lbz          r6, 0(r9)
0x825A41A0:  stb          r6, 0(r10)
0x825A41A4:  lbz          r5, 1(r9)
0x825A41A8:  stb          r5, 1(r10)
0x825A41AC:  lbz          r4, 2(r9)
0x825A41B0:  stb          r4, 2(r10)
0x825A41B4:  lbz          r6, 3(r9)
0x825A41B8:  stb          r6, 3(r10)
0x825A41BC:  lbz          r5, 4(r9)
0x825A41C0:  stb          r5, 4(r10)
0x825A41C4:  lbz          r4, 5(r9)
0x825A41C8:  stb          r4, 5(r10)
0x825A41CC:  lbz          r6, 6(r9)
0x825A41D0:  stb          r6, 6(r10)
0x825A41D4:  lbz          r5, 7(r9)
0x825A41D8:  add          r9, r7, r30
0x825A41DC:  stb          r5, 7(r10)
0x825A41E0:  add          r10, r8, r30
0x825A41E4:  lbz          r4, 0(r8)
0x825A41E8:  stb          r4, 0(r7)
0x825A41EC:  lbz          r6, 1(r8)
0x825A41F0:  stb          r6, 1(r7)
0x825A41F4:  lbz          r5, 2(r8)
0x825A41F8:  stb          r5, 2(r7)
0x825A41FC:  lbz          r4, 3(r8)
0x825A4200:  stb          r4, 3(r7)
0x825A4204:  lbz          r6, 4(r8)
0x825A4208:  stb          r6, 4(r7)
0x825A420C:  lbz          r5, 5(r8)
0x825A4210:  stb          r5, 5(r7)
0x825A4214:  lbz          r4, 6(r8)
0x825A4218:  stb          r4, 6(r7)
0x825A421C:  lbz          r6, 7(r8)
0x825A4220:  add          r8, r10, r30
0x825A4224:  stb          r6, 7(r7)
0x825A4228:  add          r7, r9, r30
0x825A422C:  lbz          r5, 0(r10)
0x825A4230:  stb          r5, 0(r9)
0x825A4234:  lbz          r4, 1(r10)
0x825A4238:  stb          r4, 1(r9)
0x825A423C:  lbz          r6, 2(r10)
0x825A4240:  stb          r6, 2(r9)
0x825A4244:  lbz          r5, 3(r10)
0x825A4248:  stb          r5, 3(r9)
0x825A424C:  lbz          r4, 4(r10)
0x825A4250:  stb          r4, 4(r9)
0x825A4254:  lbz          r6, 5(r10)
0x825A4258:  stb          r6, 5(r9)
0x825A425C:  lbz          r5, 6(r10)
0x825A4260:  stb          r5, 6(r9)
0x825A4264:  lbz          r4, 7(r10)
0x825A4268:  add          r10, r8, r30
0x825A426C:  stb          r4, 7(r9)
0x825A4270:  add          r9, r7, r30
0x825A4274:  lbz          r6, 0(r8)
0x825A4278:  stb          r6, 0(r7)
0x825A427C:  lbz          r5, 1(r8)
0x825A4280:  stb          r5, 1(r7)
0x825A4284:  lbz          r4, 2(r8)
0x825A4288:  stb          r4, 2(r7)
0x825A428C:  lbz          r6, 3(r8)
0x825A4290:  stb          r6, 3(r7)
0x825A4294:  lbz          r5, 4(r8)
0x825A4298:  stb          r5, 4(r7)
0x825A429C:  lbz          r4, 5(r8)
0x825A42A0:  stb          r4, 5(r7)
0x825A42A4:  lbz          r6, 6(r8)
0x825A42A8:  stb          r6, 6(r7)
0x825A42AC:  lbz          r5, 7(r8)
0x825A42B0:  stb          r5, 7(r7)
0x825A42B4:  lbz          r4, 0(r10)
0x825A42B8:  stb          r4, 0(r9)
0x825A42BC:  lbz          r8, 1(r10)
0x825A42C0:  stb          r8, 1(r9)
0x825A42C4:  lbz          r7, 2(r10)
0x825A42C8:  stb          r7, 2(r9)
0x825A42CC:  lbz          r6, 3(r10)
0x825A42D0:  stb          r6, 3(r9)
0x825A42D4:  lbz          r5, 4(r10)
0x825A42D8:  stb          r5, 4(r9)
0x825A42DC:  lbz          r4, 5(r10)
0x825A42E0:  stb          r4, 5(r9)
0x825A42E4:  lbz          r8, 6(r10)
0x825A42E8:  stb          r8, 6(r9)
0x825A42EC:  lbz          r7, 7(r10)
0x825A42F0:  stb          r7, 7(r9)
0x825A42F4:  b            0x825a4d2c
0x825A42F8:  mr           r4, r3
0x825A42FC:  cmplwi       cr6, r25, 4
0x825A4300:  bge          cr6, 0x825a4328
0x825A4304:  lwz          r10, 0(r3)
0x825A4308:  subfic       r6, r25, 4
0x825A430C:  addi         r4, r3, 4
0x825A4310:  addi         r5, r25, 0x1c
0x825A4314:  slw          r3, r10, r25
0x825A4318:  srw          r6, r10, r6
0x825A431C:  or           r9, r3, r29
0x825A4320:  clrlwi       r9, r9, 0x1c
0x825A4324:  b            0x825a4334
0x825A4328:  clrlwi       r9, r29, 0x1c
0x825A432C:  srwi         r6, r29, 4
0x825A4330:  addi         r5, r25, -4
0x825A4334:  srwi         r8, r24, 1
0x825A4338:  lwz          r7, 0x7c(r1)
0x825A433C:  slwi         r9, r9, 6
0x825A4340:  mr           r10, r19
0x825A4344:  add          r7, r9, r7
0x825A4348:  stbx         r28, r8, r23
0x825A434C:  cmplwi       cr6, r5, 0
0x825A4350:  bne          cr6, 0x825a4368
0x825A4354:  lwz          r9, 0(r4)
0x825A4358:  li           r5, 0x1f
0x825A435C:  addi         r4, r4, 4
0x825A4360:  srwi         r6, r9, 1
0x825A4364:  b            0x825a4374
0x825A4368:  mr           r9, r6
0x825A436C:  srwi         r6, r6, 1
0x825A4370:  addi         r5, r5, -1
0x825A4374:  clrlwi       r3, r9, 0x1f
0x825A4378:  cmplwi       cr6, r3, 0
0x825A437C:  beq          cr6, 0x825a43c0
0x825A4380:  lbz          r8, 0(r11)
0x825A4384:  addi         r11, r11, 1
0x825A4388:  stw          r11, 0x80(r1)
0x825A438C:  lbz          r9, 0(r18)
0x825A4390:  addi         r18, r18, 1
0x825A4394:  addi         r9, r9, 1
0x825A4398:  cmpwi        cr6, r9, 0
0x825A439C:  beq          cr6, 0x825a43fc
0x825A43A0:  lbzx         r3, r10, r7
0x825A43A4:  addi         r29, r1, 0x210
0x825A43A8:  addi         r9, r9, -1
0x825A43AC:  addi         r10, r10, 1
0x825A43B0:  cmpwi        cr6, r9, 0
0x825A43B4:  stbx         r8, r3, r29
0x825A43B8:  bne          cr6, 0x825a43a0
0x825A43BC:  b            0x825a43fc
0x825A43C0:  lbz          r9, 0(r18)
0x825A43C4:  addi         r18, r18, 1
0x825A43C8:  addi         r9, r9, 1
0x825A43CC:  cmpwi        cr6, r9, 0
0x825A43D0:  beq          cr6, 0x825a43fc
0x825A43D4:  lbz          r3, 0(r11)
0x825A43D8:  addi         r29, r1, 0x210
0x825A43DC:  lbzx         r8, r10, r7
0x825A43E0:  addi         r11, r11, 1
0x825A43E4:  addi         r9, r9, -1
0x825A43E8:  addi         r10, r10, 1
0x825A43EC:  cmpwi        cr6, r9, 0
0x825A43F0:  stw          r11, 0x80(r1)
0x825A43F4:  stbx         r3, r8, r29
0x825A43F8:  bne          cr6, 0x825a43d4
0x825A43FC:  cmpwi        cr6, r10, 0x3f
0x825A4400:  blt          cr6, 0x825a434c
0x825A4404:  stw          r18, 0x120(r1)
0x825A4408:  bne          cr6, 0x825a4424
0x825A440C:  lbz          r10, 0(r11)
0x825A4410:  addi         r8, r1, 0x210
0x825A4414:  lbz          r9, 0x3f(r7)
0x825A4418:  addi         r11, r11, 1
0x825A441C:  stw          r11, 0x80(r1)
0x825A4420:  stbx         r10, r9, r8
0x825A4424:  add          r10, r26, r30
0x825A4428:  lfd          f12, 0x210(r1)
0x825A442C:  stfd         f12, 0(r26)
0x825A4430:  mr           r3, r4
0x825A4434:  add          r9, r10, r30
0x825A4438:  lfd          f11, 0x218(r1)
0x825A443C:  lfd          f10, 0x220(r1)
0x825A4440:  mr           r29, r6
0x825A4444:  lfd          f9, 0x228(r1)
0x825A4448:  mr           r25, r5
0x825A444C:  stfd         f11, 0(r10)
0x825A4450:  add          r10, r9, r30
0x825A4454:  lfd          f8, 0x230(r1)
0x825A4458:  stw          r3, 0x54(r1)
0x825A445C:  stfd         f10, 0(r9)
0x825A4460:  add          r9, r10, r30
0x825A4464:  lfd          f7, 0x238(r1)
0x825A4468:  stw          r29, 0x50(r1)
0x825A446C:  lfd          f6, 0x240(r1)
0x825A4470:  stw          r25, 0x58(r1)
0x825A4474:  stfd         f9, 0(r10)
0x825A4478:  add          r10, r9, r30
0x825A447C:  lfd          f5, 0x248(r1)
0x825A4480:  stfd         f8, 0(r9)
0x825A4484:  add          r9, r10, r30
0x825A4488:  stfd         f7, 0(r10)
0x825A448C:  stfd         f6, 0(r9)
0x825A4490:  stfdx        f5, r9, r30
0x825A4494:  b            0x825a4d2c
0x825A4498:  srwi         r7, r24, 1
0x825A449C:  cmplwi       cr6, r25, 7
0x825A44A0:  stbx         r28, r7, r23
0x825A44A4:  lbz          r6, 0(r22)
0x825A44A8:  addi         r22, r22, 1
0x825A44AC:  lbz          r5, 0(r21)
0x825A44B0:  addi         r21, r21, 1
0x825A44B4:  extsb        r4, r6
0x825A44B8:  extsb        r10, r5
0x825A44BC:  mullw        r11, r4, r30
0x825A44C0:  stw          r22, 0xc0(r1)
0x825A44C4:  stw          r21, 0xf0(r1)
0x825A44C8:  add          r11, r11, r10
0x825A44CC:  add          r7, r11, r20
0x825A44D0:  bge          cr6, 0x825a4504
0x825A44D4:  lwz          r11, 0(r3)
0x825A44D8:  subfic       r10, r25, 7
0x825A44DC:  addi         r6, r25, 0x19
0x825A44E0:  addi         r9, r3, 4
0x825A44E4:  stw          r6, 0x58(r1)
0x825A44E8:  stw          r9, 0x54(r1)
0x825A44EC:  slw          r8, r11, r25
0x825A44F0:  srw          r4, r11, r10
0x825A44F4:  or           r5, r8, r29
0x825A44F8:  clrlwi       r6, r5, 0x19
0x825A44FC:  stw          r4, 0x50(r1)
0x825A4500:  b            0x825a4518
0x825A4504:  srwi         r3, r29, 7
0x825A4508:  addi         r11, r25, -7
0x825A450C:  clrlwi       r6, r29, 0x19
0x825A4510:  stw          r3, 0x50(r1)
0x825A4514:  stw          r11, 0x58(r1)
0x825A4518:  mr           r8, r30
0x825A451C:  addi         r5, r1, 0x50
0x825A4520:  mr           r4, r30
0x825A4524:  mr           r3, r26
0x825A4528:  bl           0x825abd30
0x825A452C:  lwz          r25, 0x58(r1)
0x825A4530:  lwz          r3, 0x54(r1)
0x825A4534:  lwz          r29, 0x50(r1)
0x825A4538:  lwz          r11, 0x80(r1)
0x825A453C:  b            0x825a4d2c
0x825A4540:  srwi         r10, r24, 1
0x825A4544:  addi         r4, r1, 0x50
0x825A4548:  addi         r3, r1, 0x280
0x825A454C:  stbx         r28, r10, r23
0x825A4550:  lhz          r9, 0(r16)
0x825A4554:  addi         r16, r16, 2
0x825A4558:  sth          r9, 0x280(r1)
0x825A455C:  stw          r16, 0x180(r1)
0x825A4560:  bl           0x825aae80
0x825A4564:  lwz          r11, 0x58(r1)
0x825A4568:  cmplwi       cr6, r11, 4
0x825A456C:  bge          cr6, 0x825a45a8
0x825A4570:  lwz          r9, 0x54(r1)
0x825A4574:  subfic       r8, r11, 4
0x825A4578:  addi         r5, r11, 0x1c
0x825A457C:  lwz          r4, 0x50(r1)
0x825A4580:  addi         r7, r9, 4
0x825A4584:  lwz          r10, 0(r9)
0x825A4588:  stw          r5, 0x58(r1)
0x825A458C:  stw          r7, 0x54(r1)
0x825A4590:  slw          r6, r10, r11
0x825A4594:  srw          r11, r10, r8
0x825A4598:  or           r3, r6, r4
0x825A459C:  clrlwi       r6, r3, 0x1c
0x825A45A0:  stw          r11, 0x50(r1)
0x825A45A4:  b            0x825a45c0
0x825A45A8:  lwz          r10, 0x50(r1)
0x825A45AC:  addi         r9, r11, -4
0x825A45B0:  clrlwi       r6, r10, 0x1c
0x825A45B4:  srwi         r10, r10, 4
0x825A45B8:  stw          r9, 0x58(r1)
0x825A45BC:  stw          r10, 0x50(r1)
0x825A45C0:  addi         r5, r1, 0x280
0x825A45C4:  mr           r4, r30
0x825A45C8:  mr           r3, r26
0x825A45CC:  bl           0x825aaac8
0x825A45D0:  lwz          r25, 0x58(r1)
0x825A45D4:  lwz          r3, 0x54(r1)
0x825A45D8:  lwz          r29, 0x50(r1)
0x825A45DC:  lwz          r11, 0x80(r1)
0x825A45E0:  b            0x825a4d2c
0x825A45E4:  lbz          r10, 0(r11)
0x825A45E8:  srwi         r8, r24, 1
0x825A45EC:  add          r9, r26, r30
0x825A45F0:  rotlwi       r7, r10, 8
0x825A45F4:  addi         r11, r11, 1
0x825A45F8:  or           r10, r7, r10
0x825A45FC:  stbx         r28, r8, r23
0x825A4600:  add          r8, r9, r30
0x825A4604:  slwi         r6, r10, 0x10
0x825A4608:  or           r10, r6, r10
0x825A460C:  stw          r10, 0(r26)
0x825A4610:  stw          r10, 4(r26)
0x825A4614:  stw          r10, 0(r9)
0x825A4618:  stw          r10, 4(r9)
0x825A461C:  add          r9, r8, r30
0x825A4620:  stw          r10, 0(r8)
0x825A4624:  stw          r10, 4(r8)
0x825A4628:  add          r8, r9, r30
0x825A462C:  stw          r10, 0(r9)
0x825A4630:  stw          r10, 4(r9)
0x825A4634:  add          r9, r8, r30
0x825A4638:  stw          r10, 0(r8)
0x825A463C:  stw          r10, 4(r8)
0x825A4640:  add          r8, r9, r30
0x825A4644:  stw          r10, 0(r9)
0x825A4648:  stw          r10, 4(r9)
0x825A464C:  add          r9, r8, r30
0x825A4650:  stw          r10, 0(r8)
0x825A4654:  stw          r10, 4(r8)
0x825A4658:  stw          r10, 0(r9)
0x825A465C:  stw          r10, 4(r9)
0x825A4660:  b            0x825a4d28
0x825A4664:  srwi         r5, r24, 1
0x825A4668:  lwz          r9, 0x250(r1)
0x825A466C:  addi         r4, r1, 0x50
0x825A4670:  addi         r6, r9, 2
0x825A4674:  addi         r3, r1, 0x280
0x825A4678:  stbx         r28, r5, r23
0x825A467C:  lbz          r11, 0(r22)
0x825A4680:  addi         r22, r22, 1
0x825A4684:  lbz          r10, 0(r21)
0x825A4688:  addi         r21, r21, 1
0x825A468C:  extsb        r8, r11
0x825A4690:  lhz          r7, 0(r9)
0x825A4694:  extsb        r10, r10
0x825A4698:  stw          r6, 0x250(r1)
0x825A469C:  mullw        r11, r8, r30
0x825A46A0:  stw          r22, 0xc0(r1)
0x825A46A4:  stw          r21, 0xf0(r1)
0x825A46A8:  sth          r7, 0x280(r1)
0x825A46AC:  add          r11, r11, r10
0x825A46B0:  add          r29, r11, r20
0x825A46B4:  bl           0x825aae80
0x825A46B8:  lwz          r11, 0x58(r1)
0x825A46BC:  cmplwi       cr6, r11, 4
0x825A46C0:  bge          cr6, 0x825a46fc
0x825A46C4:  lwz          r9, 0x54(r1)
0x825A46C8:  subfic       r5, r11, 4
0x825A46CC:  addi         r4, r9, 4
0x825A46D0:  lwz          r10, 0(r9)
0x825A46D4:  lwz          r9, 0x50(r1)
0x825A46D8:  stw          r4, 0x54(r1)
0x825A46DC:  slw          r3, r10, r11
0x825A46E0:  addi         r11, r11, 0x1c
0x825A46E4:  srw          r7, r10, r5
0x825A46E8:  or           r8, r3, r9
0x825A46EC:  clrlwi       r6, r8, 0x1c
0x825A46F0:  stw          r11, 0x58(r1)
0x825A46F4:  stw          r7, 0x50(r1)
0x825A46F8:  b            0x825a4714
0x825A46FC:  lwz          r10, 0x50(r1)
0x825A4700:  addi         r4, r11, -4
0x825A4704:  srwi         r5, r10, 4
0x825A4708:  clrlwi       r6, r10, 0x1c
0x825A470C:  stw          r4, 0x58(r1)
0x825A4710:  stw          r5, 0x50(r1)
0x825A4714:  mr           r8, r30
0x825A4718:  mr           r7, r29
0x825A471C:  addi         r5, r1, 0x280
0x825A4720:  mr           r4, r30
0x825A4724:  mr           r3, r26
0x825A4728:  bl           0x825aaaf8
0x825A472C:  lwz          r25, 0x58(r1)
0x825A4730:  lwz          r3, 0x54(r1)
0x825A4734:  lwz          r29, 0x50(r1)
0x825A4738:  lwz          r11, 0x80(r1)
0x825A473C:  b            0x825a4d2c
0x825A4740:  lbz          r10, 0(r11)
0x825A4744:  addi         r11, r11, 1
0x825A4748:  srwi         r8, r24, 1
0x825A474C:  rotlwi       r5, r10, 8
0x825A4750:  addi         r19, r31, 0x40
0x825A4754:  or           r10, r5, r10
0x825A4758:  stw          r11, 0x80(r1)
0x825A475C:  addi         r18, r31, 0x40
0x825A4760:  lbz          r9, 0(r11)
0x825A4764:  addi         r11, r11, 1
0x825A4768:  stbx         r28, r8, r23
0x825A476C:  slwi         r15, r10, 0x10
0x825A4770:  rotlwi       r4, r9, 8
0x825A4774:  or           r10, r15, r10
0x825A4778:  or           r9, r4, r9
0x825A477C:  stw          r11, 0x80(r1)
0x825A4780:  addi         r6, r27, 1
0x825A4784:  lbz          r7, 0(r27)
0x825A4788:  slwi         r14, r9, 0x10
0x825A478C:  addi         r27, r31, 0x40
0x825A4790:  rlwinm       r5, r7, 2, 0x1a, 0x1d
0x825A4794:  or           r9, r14, r9
0x825A4798:  rlwinm       r4, r7, 0x1e, 2, 0x1d
0x825A479C:  add          r8, r26, r30
0x825A47A0:  addi         r17, r31, 0x40
0x825A47A4:  lwzx         r15, r5, r31
0x825A47A8:  addi         r16, r31, 0x40
0x825A47AC:  lwzx         r5, r5, r19
0x825A47B0:  add          r7, r8, r30
0x825A47B4:  and          r19, r15, r9
0x825A47B8:  and          r5, r5, r10
0x825A47BC:  or           r5, r19, r5
0x825A47C0:  stw          r5, 0(r26)
0x825A47C4:  lwzx         r5, r4, r31
0x825A47C8:  lwzx         r4, r4, r18
0x825A47CC:  addi         r18, r31, 0x40
0x825A47D0:  and          r5, r5, r9
0x825A47D4:  and          r4, r4, r10
0x825A47D8:  or           r5, r5, r4
0x825A47DC:  stw          r5, 4(r26)
0x825A47E0:  lbz          r5, 0(r6)
0x825A47E4:  addi         r6, r6, 1
0x825A47E8:  rlwinm       r4, r5, 2, 0x1a, 0x1d
0x825A47EC:  rlwinm       r5, r5, 0x1e, 2, 0x1d
0x825A47F0:  lwzx         r19, r4, r31
0x825A47F4:  lwzx         r4, r4, r27
0x825A47F8:  and          r27, r19, r9
0x825A47FC:  and          r4, r4, r10
0x825A4800:  or           r4, r27, r4
0x825A4804:  stw          r4, 0(r8)
0x825A4808:  lwzx         r4, r5, r31
0x825A480C:  lwzx         r5, r5, r17
0x825A4810:  addi         r17, r31, 0x40
0x825A4814:  and          r4, r4, r9
0x825A4818:  and          r5, r5, r10
0x825A481C:  or           r4, r4, r5
0x825A4820:  stw          r4, 4(r8)
0x825A4824:  add          r8, r7, r30
0x825A4828:  lbz          r5, 0(r6)
0x825A482C:  addi         r6, r6, 1
0x825A4830:  rlwinm       r4, r5, 2, 0x1a, 0x1d
0x825A4834:  rlwinm       r5, r5, 0x1e, 2, 0x1d
0x825A4838:  lwzx         r27, r4, r31
0x825A483C:  lwzx         r4, r4, r16
0x825A4840:  addi         r16, r31, 0x40
0x825A4844:  and          r27, r27, r9
0x825A4848:  and          r4, r4, r10
0x825A484C:  or           r4, r27, r4
0x825A4850:  addi         r27, r31, 0x40
0x825A4854:  stw          r4, 0(r7)
0x825A4858:  addi         r4, r31, 0x40
0x825A485C:  lwzx         r19, r5, r31
0x825A4860:  lwzx         r5, r5, r4
0x825A4864:  and          r4, r19, r9
0x825A4868:  addi         r19, r31, 0x40
0x825A486C:  and          r5, r5, r10
0x825A4870:  or           r4, r4, r5
0x825A4874:  stw          r4, 4(r7)
0x825A4878:  add          r7, r8, r30
0x825A487C:  lbz          r5, 0(r6)
0x825A4880:  addi         r6, r6, 1
0x825A4884:  rlwinm       r4, r5, 2, 0x1a, 0x1d
0x825A4888:  rlwinm       r5, r5, 0x1e, 2, 0x1d
0x825A488C:  addi         r15, r31, 0x40
0x825A4890:  lwzx         r14, r4, r31
0x825A4894:  lwzx         r4, r4, r27
0x825A4898:  and          r27, r14, r9
0x825A489C:  and          r4, r4, r10
0x825A48A0:  or           r4, r27, r4
0x825A48A4:  stw          r4, 0(r8)
0x825A48A8:  lwzx         r4, r5, r31
0x825A48AC:  lwzx         r5, r5, r19
0x825A48B0:  addi         r19, r31, 0x40
0x825A48B4:  and          r4, r4, r9
0x825A48B8:  and          r5, r5, r10
0x825A48BC:  or           r4, r4, r5
0x825A48C0:  stw          r4, 4(r8)
0x825A48C4:  add          r8, r7, r30
0x825A48C8:  lbz          r5, 0(r6)
0x825A48CC:  addi         r6, r6, 1
0x825A48D0:  rlwinm       r4, r5, 2, 0x1a, 0x1d
0x825A48D4:  rlwinm       r5, r5, 0x1e, 2, 0x1d
0x825A48D8:  lwzx         r27, r4, r31
0x825A48DC:  lwzx         r4, r4, r18
0x825A48E0:  addi         r18, r31, 0x40
0x825A48E4:  and          r27, r27, r9
0x825A48E8:  and          r4, r4, r10
0x825A48EC:  or           r4, r27, r4
0x825A48F0:  stw          r4, 0(r7)
0x825A48F4:  lwzx         r4, r5, r31
0x825A48F8:  lwzx         r5, r5, r17
0x825A48FC:  addi         r17, r31, 0x40
0x825A4900:  and          r4, r4, r9
0x825A4904:  and          r5, r5, r10
0x825A4908:  or           r4, r4, r5
0x825A490C:  stw          r4, 4(r7)
0x825A4910:  add          r7, r8, r30
0x825A4914:  lbz          r5, 0(r6)
0x825A4918:  addi         r6, r6, 1
0x825A491C:  rlwinm       r4, r5, 2, 0x1a, 0x1d
0x825A4920:  rlwinm       r5, r5, 0x1e, 2, 0x1d
0x825A4924:  lwzx         r27, r4, r31
0x825A4928:  lwzx         r4, r4, r16
0x825A492C:  and          r27, r27, r9
0x825A4930:  and          r4, r4, r10
0x825A4934:  or           r4, r27, r4
0x825A4938:  addi         r27, r31, 0x40
0x825A493C:  stw          r4, 0(r8)
0x825A4940:  lwzx         r4, r5, r31
0x825A4944:  lwzx         r5, r5, r15
0x825A4948:  and          r4, r4, r9
0x825A494C:  and          r5, r5, r10
0x825A4950:  or           r4, r4, r5
0x825A4954:  add          r5, r7, r30
0x825A4958:  stw          r4, 4(r8)
0x825A495C:  lbz          r8, 0(r6)
0x825A4960:  addi         r6, r6, 1
0x825A4964:  rlwinm       r4, r8, 2, 0x1a, 0x1d
0x825A4968:  rlwinm       r8, r8, 0x1e, 2, 0x1d
0x825A496C:  lwzx         r16, r4, r31
0x825A4970:  lwzx         r4, r4, r27
0x825A4974:  and          r27, r16, r9
0x825A4978:  and          r4, r4, r10
0x825A497C:  or           r4, r27, r4
0x825A4980:  addi         r27, r6, 1
0x825A4984:  stw          r4, 0(r7)
0x825A4988:  lwzx         r4, r8, r31
0x825A498C:  lwzx         r8, r8, r19
0x825A4990:  and          r4, r4, r9
0x825A4994:  and          r8, r8, r10
0x825A4998:  or           r4, r4, r8
0x825A499C:  stw          r4, 4(r7)
0x825A49A0:  lbz          r8, 0(r6)
0x825A49A4:  rlwinm       r7, r8, 2, 0x1a, 0x1d
0x825A49A8:  rlwinm       r8, r8, 0x1e, 2, 0x1d
0x825A49AC:  lwzx         r6, r7, r31
0x825A49B0:  lwzx         r4, r7, r18
0x825A49B4:  and          r7, r6, r9
0x825A49B8:  and          r6, r4, r10
0x825A49BC:  or           r4, r7, r6
0x825A49C0:  stw          r4, 0(r5)
0x825A49C4:  lwzx         r7, r8, r31
0x825A49C8:  lwzx         r6, r8, r17
0x825A49CC:  and          r4, r7, r9
0x825A49D0:  lwz          r16, 0x180(r1)
0x825A49D4:  li           r19, 0
0x825A49D8:  and          r10, r6, r10
0x825A49DC:  lwz          r18, 0x120(r1)
0x825A49E0:  lwz          r17, 0x74(r1)
0x825A49E4:  or           r9, r4, r10
0x825A49E8:  lwz          r15, 0x57c(r1)
0x825A49EC:  stw          r9, 4(r5)
0x825A49F0:  b            0x825a4d2c
0x825A49F4:  or           r9, r11, r26
0x825A49F8:  srwi         r8, r24, 1
0x825A49FC:  clrlwi       r7, r9, 0x1d
0x825A4A00:  add          r10, r26, r30
0x825A4A04:  cmplwi       cr6, r7, 0
0x825A4A08:  stbx         r28, r8, r23
0x825A4A0C:  bne          cr6, 0x825a4a68
0x825A4A10:  lfd          f4, 0(r11)
0x825A4A14:  add          r9, r10, r30
0x825A4A18:  stfd         f4, 0(r26)
0x825A4A1C:  lfd          f3, 8(r11)
0x825A4A20:  stfd         f3, 0(r10)
0x825A4A24:  add          r10, r9, r30
0x825A4A28:  lfd          f2, 0x10(r11)
0x825A4A2C:  stfd         f2, 0(r9)
0x825A4A30:  add          r9, r10, r30
0x825A4A34:  lfd          f1, 0x18(r11)
0x825A4A38:  stfd         f1, 0(r10)
0x825A4A3C:  add          r10, r9, r30
0x825A4A40:  lfd          f0, 0x20(r11)
0x825A4A44:  stfd         f0, 0(r9)
0x825A4A48:  add          r9, r10, r30
0x825A4A4C:  lfd          f13, 0x28(r11)
0x825A4A50:  stfd         f13, 0(r10)
0x825A4A54:  lfd          f12, 0x30(r11)
0x825A4A58:  stfd         f12, 0(r9)
0x825A4A5C:  lfd          f11, 0x38(r11)
0x825A4A60:  stfdx        f11, r9, r30
0x825A4A64:  b            0x825a4d24
0x825A4A68:  clrlwi       r6, r9, 0x1e
0x825A4A6C:  add          r9, r10, r30
0x825A4A70:  cmplwi       cr6, r6, 0
0x825A4A74:  bne          cr6, 0x825a4b10
0x825A4A78:  lwz          r5, 0(r11)
0x825A4A7C:  stw          r5, 0(r26)
0x825A4A80:  lwz          r4, 4(r11)
0x825A4A84:  stw          r4, 4(r26)
0x825A4A88:  lwz          r8, 8(r11)
0x825A4A8C:  stw          r8, 0(r10)
0x825A4A90:  lwz          r7, 0xc(r11)
0x825A4A94:  stw          r7, 4(r10)
0x825A4A98:  add          r10, r9, r30
0x825A4A9C:  lwz          r6, 0x10(r11)
0x825A4AA0:  stw          r6, 0(r9)
0x825A4AA4:  lwz          r5, 0x14(r11)
0x825A4AA8:  stw          r5, 4(r9)
0x825A4AAC:  add          r9, r10, r30
0x825A4AB0:  lwz          r4, 0x18(r11)
0x825A4AB4:  stw          r4, 0(r10)
0x825A4AB8:  lwz          r8, 0x1c(r11)
0x825A4ABC:  stw          r8, 4(r10)
0x825A4AC0:  add          r10, r9, r30
0x825A4AC4:  lwz          r7, 0x20(r11)
0x825A4AC8:  stw          r7, 0(r9)
0x825A4ACC:  lwz          r6, 0x24(r11)
0x825A4AD0:  stw          r6, 4(r9)
0x825A4AD4:  add          r9, r10, r30
0x825A4AD8:  lwz          r5, 0x28(r11)
0x825A4ADC:  stw          r5, 0(r10)
0x825A4AE0:  lwz          r4, 0x2c(r11)
0x825A4AE4:  stw          r4, 4(r10)
0x825A4AE8:  add          r10, r9, r30
0x825A4AEC:  lwz          r8, 0x30(r11)
0x825A4AF0:  stw          r8, 0(r9)
0x825A4AF4:  lwz          r7, 0x34(r11)
0x825A4AF8:  stw          r7, 4(r9)
0x825A4AFC:  lwz          r6, 0x38(r11)
0x825A4B00:  stw          r6, 0(r10)
0x825A4B04:  lwz          r5, 0x3c(r11)
0x825A4B08:  stw          r5, 4(r10)
0x825A4B0C:  b            0x825a4d24
0x825A4B10:  lbz          r4, 0(r11)
0x825A4B14:  stb          r4, 0(r26)
0x825A4B18:  lbz          r8, 1(r11)
0x825A4B1C:  stb          r8, 1(r26)
0x825A4B20:  lbz          r7, 2(r11)
0x825A4B24:  stb          r7, 2(r26)
0x825A4B28:  lbz          r6, 3(r11)
0x825A4B2C:  stb          r6, 3(r26)
0x825A4B30:  lbz          r5, 4(r11)
0x825A4B34:  stb          r5, 4(r26)
0x825A4B38:  lbz          r4, 5(r11)
0x825A4B3C:  stb          r4, 5(r26)
0x825A4B40:  lbz          r8, 6(r11)
0x825A4B44:  stb          r8, 6(r26)
0x825A4B48:  lbz          r7, 7(r11)
0x825A4B4C:  stb          r7, 7(r26)
0x825A4B50:  lbz          r6, 8(r11)
0x825A4B54:  stb          r6, 0(r10)
0x825A4B58:  lbz          r5, 9(r11)
0x825A4B5C:  stb          r5, 1(r10)
0x825A4B60:  lbz          r4, 0xa(r11)
0x825A4B64:  stb          r4, 2(r10)
0x825A4B68:  lbz          r8, 0xb(r11)
0x825A4B6C:  stb          r8, 3(r10)
0x825A4B70:  lbz          r7, 0xc(r11)
0x825A4B74:  stb          r7, 4(r10)
0x825A4B78:  lbz          r6, 0xd(r11)
0x825A4B7C:  stb          r6, 5(r10)
0x825A4B80:  lbz          r5, 0xe(r11)
0x825A4B84:  stb          r5, 6(r10)
0x825A4B88:  lbz          r4, 0xf(r11)
0x825A4B8C:  stb          r4, 7(r10)
0x825A4B90:  add          r10, r9, r30
0x825A4B94:  lbz          r8, 0x10(r11)
0x825A4B98:  stb          r8, 0(r9)
0x825A4B9C:  lbz          r7, 0x11(r11)
0x825A4BA0:  stb          r7, 1(r9)
0x825A4BA4:  lbz          r6, 0x12(r11)
0x825A4BA8:  stb          r6, 2(r9)
0x825A4BAC:  lbz          r5, 0x13(r11)
0x825A4BB0:  stb          r5, 3(r9)
0x825A4BB4:  lbz          r4, 0x14(r11)
0x825A4BB8:  stb          r4, 4(r9)
0x825A4BBC:  lbz          r8, 0x15(r11)
0x825A4BC0:  stb          r8, 5(r9)
0x825A4BC4:  lbz          r7, 0x16(r11)
0x825A4BC8:  stb          r7, 6(r9)
0x825A4BCC:  lbz          r6, 0x17(r11)
0x825A4BD0:  stb          r6, 7(r9)
0x825A4BD4:  add          r9, r10, r30
0x825A4BD8:  lbz          r5, 0x18(r11)
0x825A4BDC:  stb          r5, 0(r10)
0x825A4BE0:  lbz          r4, 0x19(r11)
0x825A4BE4:  stb          r4, 1(r10)
0x825A4BE8:  lbz          r8, 0x1a(r11)
0x825A4BEC:  stb          r8, 2(r10)
0x825A4BF0:  lbz          r7, 0x1b(r11)
0x825A4BF4:  stb          r7, 3(r10)
0x825A4BF8:  lbz          r6, 0x1c(r11)
0x825A4BFC:  stb          r6, 4(r10)
0x825A4C00:  lbz          r5, 0x1d(r11)
0x825A4C04:  stb          r5, 5(r10)
0x825A4C08:  lbz          r4, 0x1e(r11)
0x825A4C0C:  stb          r4, 6(r10)
0x825A4C10:  lbz          r8, 0x1f(r11)
0x825A4C14:  stb          r8, 7(r10)
0x825A4C18:  add          r10, r9, r30
0x825A4C1C:  lbz          r7, 0x20(r11)
0x825A4C20:  stb          r7, 0(r9)
0x825A4C24:  lbz          r6, 0x21(r11)
0x825A4C28:  stb          r6, 1(r9)
0x825A4C2C:  lbz          r5, 0x22(r11)
0x825A4C30:  stb          r5, 2(r9)
0x825A4C34:  lbz          r4, 0x23(r11)
0x825A4C38:  stb          r4, 3(r9)
0x825A4C3C:  lbz          r8, 0x24(r11)
0x825A4C40:  stb          r8, 4(r9)
0x825A4C44:  lbz          r7, 0x25(r11)
0x825A4C48:  stb          r7, 5(r9)
0x825A4C4C:  lbz          r6, 0x26(r11)
0x825A4C50:  stb          r6, 6(r9)
0x825A4C54:  lbz          r5, 0x27(r11)
0x825A4C58:  stb          r5, 7(r9)
0x825A4C5C:  add          r9, r10, r30
0x825A4C60:  lbz          r4, 0x28(r11)
0x825A4C64:  stb          r4, 0(r10)
0x825A4C68:  lbz          r8, 0x29(r11)
0x825A4C6C:  stb          r8, 1(r10)
0x825A4C70:  lbz          r7, 0x2a(r11)
0x825A4C74:  stb          r7, 2(r10)
0x825A4C78:  lbz          r6, 0x2b(r11)
0x825A4C7C:  stb          r6, 3(r10)
0x825A4C80:  lbz          r5, 0x2c(r11)
0x825A4C84:  stb          r5, 4(r10)
0x825A4C88:  lbz          r4, 0x2d(r11)
0x825A4C8C:  stb          r4, 5(r10)
0x825A4C90:  lbz          r8, 0x2e(r11)
0x825A4C94:  stb          r8, 6(r10)
0x825A4C98:  lbz          r7, 0x2f(r11)
0x825A4C9C:  stb          r7, 7(r10)
0x825A4CA0:  add          r10, r9, r30
0x825A4CA4:  lbz          r6, 0x30(r11)
0x825A4CA8:  stb          r6, 0(r9)
0x825A4CAC:  lbz          r5, 0x31(r11)
0x825A4CB0:  stb          r5, 1(r9)
0x825A4CB4:  lbz          r4, 0x32(r11)
0x825A4CB8:  stb          r4, 2(r9)
0x825A4CBC:  lbz          r8, 0x33(r11)
0x825A4CC0:  stb          r8, 3(r9)
0x825A4CC4:  lbz          r7, 0x34(r11)
0x825A4CC8:  stb          r7, 4(r9)
0x825A4CCC:  lbz          r6, 0x35(r11)
0x825A4CD0:  stb          r6, 5(r9)
0x825A4CD4:  lbz          r5, 0x36(r11)
0x825A4CD8:  stb          r5, 6(r9)
0x825A4CDC:  lbz          r4, 0x37(r11)
0x825A4CE0:  stb          r4, 7(r9)
0x825A4CE4:  lbz          r9, 0x38(r11)
0x825A4CE8:  stb          r9, 0(r10)
0x825A4CEC:  lbz          r8, 0x39(r11)
0x825A4CF0:  stb          r8, 1(r10)
0x825A4CF4:  lbz          r7, 0x3a(r11)
0x825A4CF8:  stb          r7, 2(r10)
0x825A4CFC:  lbz          r6, 0x3b(r11)
0x825A4D00:  stb          r6, 3(r10)
0x825A4D04:  lbz          r5, 0x3c(r11)
0x825A4D08:  stb          r5, 4(r10)
0x825A4D0C:  lbz          r4, 0x3d(r11)
0x825A4D10:  stb          r4, 5(r10)
0x825A4D14:  lbz          r9, 0x3e(r11)
0x825A4D18:  stb          r9, 6(r10)
0x825A4D1C:  lbz          r8, 0x3f(r11)
0x825A4D20:  stb          r8, 7(r10)
0x825A4D24:  addi         r11, r11, 0x40
0x825A4D28:  stw          r11, 0x80(r1)
0x825A4D2C:  addi         r17, r17, -8
0x825A4D30:  lwz          r14, 0x68(r1)
0x825A4D34:  add          r24, r24, r15
0x825A4D38:  addi         r20, r20, 8
0x825A4D3C:  addi         r26, r26, 8
0x825A4D40:  cmplwi       cr6, r17, 0
0x825A4D44:  stw          r17, 0x74(r1)
0x825A4D48:  stw          r24, 0x6c(r1)
0x825A4D4C:  stw          r20, 0x60(r1)
0x825A4D50:  bne          cr6, 0x825a3290
0x825A4D54:  stw          r27, 0x1b0(r1)
0x825A4D58:  lwz          r27, 0xb0(r1)
0x825A4D5C:  lwz          r17, 0x544(r1)
0x825A4D60:  lwz          r25, 0x54c(r1)
0x825A4D64:  lwz          r29, 0x574(r1)
0x825A4D68:  cmplwi       cr6, r15, 1
0x825A4D6C:  bne          cr6, 0x825a4d7c
0x825A4D70:  rlwinm       r7, r14, 0, 0x1c, 0x1c
0x825A4D74:  cmplwi       cr6, r7, 0
0x825A4D78:  beq          cr6, 0x825a4d84
0x825A4D7C:  add          r23, r23, r29
0x825A4D80:  stw          r23, 0x70(r1)
0x825A4D84:  addi         r14, r14, 8
0x825A4D88:  mr           r24, r19
0x825A4D8C:  add          r20, r20, r27
0x825A4D90:  add          r26, r26, r27
0x825A4D94:  cmplw        cr6, r14, r25
0x825A4D98:  stw          r14, 0x68(r1)
0x825A4D9C:  stw          r24, 0x6c(r1)
0x825A4DA0:  stw          r20, 0x60(r1)
0x825A4DA4:  blt          cr6, 0x825a31e8
0x825A4DA8:  lwz          r6, 0x594(r1)
0x825A4DAC:  lwz          r5, 0x64(r1)
0x825A4DB0:  stw          r5, 0(r6)
0x825A4DB4:  addi         r1, r1, 0x520
0x825A4DB8:  b            0x8242f8b0
0x825A4DBC:  lwz          r6, 0x594(r1)
0x825A4DC0:  lwz          r5, 0x64(r1)
0x825A4DC4:  lwz          r3, 0x54(r1)
0x825A4DC8:  stw          r5, 0(r6)
0x825A4DCC:  addi         r1, r1, 0x520
0x825A4DD0:  b            0x8242f8b0
0x825A4DD4:  .align 0                       # padding
0x825A4DD8:  mflr         r12
0x825A4DDC:  bl           0x8242f878
0x825A4DE0:  stwu         r1, -0xf0(r1)
0x825A4DE4:  mr           r31, r3
0x825A4DE8:  mr           r28, r9
0x825A4DEC:  mr           r23, r8
0x825A4DF0:  mr           r24, r7
0x825A4DF4:  mr           r26, r4
0x825A4DF8:  lwz          r30, 0x14(r31)
0x825A4DFC:  mr           r25, r5
0x825A4E00:  mr           r27, r6
0x825A4E04:  xori         r11, r30, 1
0x825A4E08:  slwi         r9, r11, 1
0x825A4E0C:  add          r9, r11, r9
0x825A4E10:  slwi         r9, r9, 4
0x825A4E14:  add          r8, r9, r31
0x825A4E18:  lwz          r7, 0x1c(r8)
0x825A4E1C:  cmplwi       cr6, r7, 0
0x825A4E20:  beq          cr6, 0x825a4e28
0x825A4E24:  stw          r11, 0x14(r31)
0x825A4E28:  rlwinm       r6, r10, 0, 0xb, 0xb
0x825A4E2C:  lwz          r29, 0x14(r31)
0x825A4E30:  li           r22, 1
0x825A4E34:  cmplwi       cr6, r6, 0
0x825A4E38:  beq          cr6, 0x825a4eac
0x825A4E3C:  rlwinm       r5, r28, 0, 0xb, 0xb
0x825A4E40:  cmplwi       cr6, r5, 0
0x825A4E44:  beq          cr6, 0x825a4ea4
0x825A4E48:  slwi         r7, r29, 1
0x825A4E4C:  lwz          r6, 8(r31)
0x825A4E50:  slwi         r11, r30, 1
0x825A4E54:  lwz          r5, 4(r31)
0x825A4E58:  addi         r4, r1, 0x80
0x825A4E5C:  stw          r28, 0x6c(r1)
0x825A4E60:  add          r7, r29, r7
0x825A4E64:  stw          r23, 0x64(r1)
0x825A4E68:  add          r3, r30, r11
0x825A4E6C:  stw          r25, 0x54(r1)
0x825A4E70:  slwi         r7, r7, 4
0x825A4E74:  stw          r22, 0x5c(r1)
0x825A4E78:  slwi         r11, r3, 4
0x825A4E7C:  stw          r4, 0x74(r1)
0x825A4E80:  add          r4, r7, r31
0x825A4E84:  add          r11, r11, r31
0x825A4E88:  mr           r10, r26
0x825A4E8C:  mr           r9, r24
0x825A4E90:  addi         r8, r27, 4
0x825A4E94:  lwz          r3, 0x40(r4)
0x825A4E98:  lwz          r7, 0x44(r11)
0x825A4E9C:  lwz          r4, 0x40(r11)
0x825A4EA0:  bl           0x825a2ed8
0x825A4EA4:  lwz          r11, 0(r27)
0x825A4EA8:  add          r27, r11, r27
0x825A4EAC:  li           r20, 0
0x825A4EB0:  rlwinm       r21, r28, 0, 0x10, 0x10
0x825A4EB4:  cmplwi       cr6, r21, 0
0x825A4EB8:  stw          r20, 0x80(r1)
0x825A4EBC:  bne          cr6, 0x825a4ec4
0x825A4EC0:  addi         r27, r27, 4
0x825A4EC4:  slwi         r11, r29, 1
0x825A4EC8:  lwz          r6, 8(r31)
0x825A4ECC:  slwi         r7, r30, 1
0x825A4ED0:  lwz          r5, 4(r31)
0x825A4ED4:  add          r4, r29, r11
0x825A4ED8:  stw          r28, 0x6c(r1)
0x825A4EDC:  add          r7, r30, r7
0x825A4EE0:  stw          r23, 0x64(r1)
0x825A4EE4:  slwi         r11, r4, 4
0x825A4EE8:  stw          r25, 0x54(r1)
0x825A4EEC:  slwi         r7, r7, 4
0x825A4EF0:  stw          r22, 0x5c(r1)
0x825A4EF4:  addi         r3, r1, 0x80
0x825A4EF8:  add          r30, r7, r31
0x825A4EFC:  add          r29, r11, r31
0x825A4F00:  mr           r10, r26
0x825A4F04:  mr           r9, r24
0x825A4F08:  mr           r8, r27
0x825A4F0C:  stw          r3, 0x74(r1)
0x825A4F10:  lwz          r7, 0x20(r30)
0x825A4F14:  lwz          r4, 0x1c(r30)
0x825A4F18:  lwz          r3, 0x1c(r29)
0x825A4F1C:  bl           0x825a2ed8
0x825A4F20:  mr           r8, r3
0x825A4F24:  cmplwi       cr6, r21, 0
0x825A4F28:  bne          cr6, 0x825a4f38
0x825A4F2C:  lwz          r11, -4(r27)
0x825A4F30:  add          r11, r11, r27
0x825A4F34:  addi         r8, r11, -4
0x825A4F38:  lwz          r10, 0x80(r1)
0x825A4F3C:  rlwinm       r11, r28, 0, 0xe, 0xe
0x825A4F40:  srwi         r3, r10, 2
0x825A4F44:  cmplwi       cr6, r11, 0
0x825A4F48:  bne          cr6, 0x825a4fcc
0x825A4F4C:  addi         r6, r1, 0x80
0x825A4F50:  lwz          r7, 0x2c(r30)
0x825A4F54:  li           r27, 2
0x825A4F58:  lwz          r4, 0x28(r30)
0x825A4F5C:  mr           r10, r26
0x825A4F60:  lwz          r3, 0x28(r29)
0x825A4F64:  mr           r9, r24
0x825A4F68:  lwz          r5, 0xc(r31)
0x825A4F6C:  stw          r28, 0x6c(r1)
0x825A4F70:  stw          r6, 0x74(r1)
0x825A4F74:  lwz          r6, 0x10(r31)
0x825A4F78:  stw          r23, 0x64(r1)
0x825A4F7C:  stw          r25, 0x54(r1)
0x825A4F80:  stw          r27, 0x5c(r1)
0x825A4F84:  bl           0x825a2ed8
0x825A4F88:  addi         r11, r1, 0x80
0x825A4F8C:  lwz          r7, 0x38(r30)
0x825A4F90:  mr           r8, r3
0x825A4F94:  lwz          r6, 0x10(r31)
0x825A4F98:  mr           r10, r26
0x825A4F9C:  lwz          r5, 0xc(r31)
0x825A4FA0:  mr           r9, r24
0x825A4FA4:  lwz          r4, 0x34(r30)
0x825A4FA8:  lwz          r3, 0x34(r29)
0x825A4FAC:  stw          r20, 0x80(r1)
0x825A4FB0:  stw          r28, 0x6c(r1)
0x825A4FB4:  stw          r11, 0x74(r1)
0x825A4FB8:  stw          r23, 0x64(r1)
0x825A4FBC:  stw          r27, 0x5c(r1)
0x825A4FC0:  stw          r25, 0x54(r1)
0x825A4FC4:  bl           0x825a2ed8
0x825A4FC8:  lwz          r3, 0x80(r1)
0x825A4FCC:  addi         r1, r1, 0xf0
0x825A4FD0:  b            0x8242f8c8
0x825A4FD4:  .align 0                       # padding
0x825A4FD8:  mflr         r12
0x825A4FDC:  bl           0x8242f898
0x825A4FE0:  stwu         r1, -0x90(r1)
0x825A4FE4:  mr           r31, r3
0x825A4FE8:  mr           r30, r4
0x825A4FEC:  mr           r28, r5
0x825A4FF0:  mr           r29, r6
0x825A4FF4:  lwz          r11, 0xf4(r31)
0x825A4FF8:  cmplwi       cr6, r11, 0
0x825A4FFC:  beq          cr6, 0x825a5008
0x825A5000:  mtctr        r11
0x825A5004:  bctrl        
0x825A5008:  cmpwi        cr6, r30, -1
0x825A500C:  beq          cr6, 0x825a5038
0x825A5010:  lwz          r11, 0x58(r31)
0x825A5014:  cmplw        cr6, r11, r30
0x825A5018:  beq          cr6, 0x825a5038
0x825A501C:  lwz          r11, 0x7c(r31)
0x825A5020:  li           r6, 0
0x825A5024:  li           r5, 0
0x825A5028:  lwz          r3, 0x54(r31)
0x825A502C:  add          r4, r11, r30
0x825A5030:  bl           0x825679e0
0x825A5034:  stw          r30, 0x58(r31)
0x825A5038:  li           r7, 0
0x825A503C:  lwz          r3, 0x54(r31)
0x825A5040:  addi         r6, r1, 0x50
0x825A5044:  mr           r5, r29
0x825A5048:  mr           r4, r28
0x825A504C:  bl           0x82567738
0x825A5050:  lwz          r3, 0x50(r1)
0x825A5054:  cmplw        cr6, r3, r29
0x825A5058:  beq          cr6, 0x825a5064
0x825A505C:  li           r10, 1
0x825A5060:  stw          r10, 0x20(r31)
0x825A5064:  lwz          r11, 0x58(r31)
0x825A5068:  add          r9, r11, r3
0x825A506C:  stw          r9, 0x58(r31)
0x825A5070:  lwz          r8, 0x58(r31)
0x825A5074:  stw          r8, 0x5c(r31)
0x825A5078:  lwz          r7, 0x80(r31)
0x825A507C:  lwz          r6, 0x5c(r31)
0x825A5080:  lwz          r5, 0x40(r31)
0x825A5084:  subf         r11, r6, r7
0x825A5088:  cmplw        cr6, r11, r5
0x825A508C:  blt          cr6, 0x825a5094
0x825A5090:  lwz          r11, 0x40(r31)
0x825A5094:  lwz          r10, 0xfc(r31)
0x825A5098:  stw          r11, 0x48(r31)
0x825A509C:  cmplwi       cr6, r10, 0
0x825A50A0:  beq          cr6, 0x825a50b4
0x825A50A4:  mr           r3, r31
0x825A50A8:  mtctr        r10
0x825A50AC:  bctrl        
0x825A50B0:  lwz          r3, 0x50(r1)
0x825A50B4:  addi         r4, r3, 3
0x825A50B8:  mr           r11, r28
0x825A50BC:  srwi         r10, r4, 2
0x825A50C0:  cmplwi       cr6, r10, 0
0x825A50C4:  stw          r11, 0x54(r1)
0x825A50C8:  beq          cr6, 0x825a50ec
0x825A50CC:  lwbrx        r9, 0, r11
0x825A50D0:  addi         r10, r10, -1
0x825A50D4:  cmplwi       cr6, r10, 0
0x825A50D8:  stw          r9, 0(r11)
0x825A50DC:  addi         r11, r11, 4
0x825A50E0:  stw          r9, 0x58(r1)
0x825A50E4:  stw          r11, 0x54(r1)
0x825A50E8:  bne          cr6, 0x825a50cc
0x825A50EC:  addi         r1, r1, 0x90
0x825A50F0:  b            0x8242f8e8
0x825A50F4:  .align 0                       # padding
0x825A50F8:  mflr         r12
0x825A50FC:  bl           0x8242f898
0x825A5100:  stwu         r1, -0x80(r1)
0x825A5104:  mr           r31, r3
0x825A5108:  clrldi       r11, r4, 0x20
0x825A510C:  mr           r29, r5
0x825A5110:  mulli        r9, r11, 0x3e8
0x825A5114:  lwz          r10, 0x84(r31)
0x825A5118:  divdu        r8, r9, r10
0x825A511C:  slwi         r28, r8, 0
0x825A5120:  bl           0x825a03b8
0x825A5124:  mr           r30, r3
0x825A5128:  lwz          r10, 0x88(r31)
0x825A512C:  subf         r11, r30, r28
0x825A5130:  add          r11, r11, r10
0x825A5134:  add          r7, r11, r29
0x825A5138:  stw          r7, 0x88(r31)
0x825A513C:  lwz          r6, 0x88(r31)
0x825A5140:  cmpwi        cr6, r6, 0
0x825A5144:  ble          cr6, 0x825a517c
0x825A5148:  bl           0x825a03b8
0x825A514C:  lwz          r5, 0x88(r31)
0x825A5150:  subf         r4, r30, r3
0x825A5154:  cmpw         cr6, r4, r5
0x825A5158:  blt          cr6, 0x825a5148
0x825A515C:  lwz          r10, 0x88(r31)
0x825A5160:  subf         r11, r3, r30
0x825A5164:  mr           r30, r3
0x825A5168:  add          r3, r11, r10
0x825A516C:  stw          r3, 0x88(r31)
0x825A5170:  lwz          r11, 0x88(r31)
0x825A5174:  cmpwi        cr6, r11, 0
0x825A5178:  bgt          cr6, 0x825a5148
0x825A517C:  addi         r1, r1, 0x80
0x825A5180:  b            0x8242f8e8
0x825A5184:  .align 0                       # padding
0x825A5188:  mflr         r12
0x825A518C:  bl           0x8242f880
0x825A5190:  stwu         r1, -0xc0(r1)
0x825A5194:  mr           r31, r3
0x825A5198:  li           r25, 0
0x825A519C:  mr           r28, r6
0x825A51A0:  mr           r30, r5
0x825A51A4:  mr           r27, r7
0x825A51A8:  lwz          r11, 0x20(r31)
0x825A51AC:  mr           r23, r25
0x825A51B0:  stw          r25, 0x50(r1)
0x825A51B4:  mr           r22, r28
0x825A51B8:  cmplwi       cr6, r11, 0
0x825A51BC:  beq          cr6, 0x825a51cc
0x825A51C0:  li           r3, 0
0x825A51C4:  addi         r1, r1, 0xc0
0x825A51C8:  b            0x8242f8d0
0x825A51CC:  bl           0x825a03b8
0x825A51D0:  mr           r24, r3
0x825A51D4:  cmpwi        cr6, r30, -1
0x825A51D8:  beq          cr6, 0x825a52b0
0x825A51DC:  lwz          r10, 0x5c(r31)
0x825A51E0:  cmplw        cr6, r10, r30
0x825A51E4:  beq          cr6, 0x825a52b0
0x825A51E8:  lwz          r11, 0xf4(r31)
0x825A51EC:  li           r23, 1
0x825A51F0:  cmplwi       cr6, r11, 0
0x825A51F4:  beq          cr6, 0x825a5204
0x825A51F8:  mr           r3, r31
0x825A51FC:  mtctr        r11
0x825A5200:  bctrl        
0x825A5204:  lwz          r9, 0x5c(r31)
0x825A5208:  cmplw        cr6, r30, r9
0x825A520C:  ble          cr6, 0x825a5274
0x825A5210:  lwz          r8, 0x58(r31)
0x825A5214:  cmplw        cr6, r30, r8
0x825A5218:  bgt          cr6, 0x825a5274
0x825A521C:  lwz          r7, 0x5c(r31)
0x825A5220:  stw          r30, 0x5c(r31)
0x825A5224:  lwz          r10, 0x64(r31)
0x825A5228:  subf         r11, r7, r30
0x825A522C:  add          r6, r10, r11
0x825A5230:  stw          r11, 0x54(r1)
0x825A5234:  stw          r6, 0x64(r31)
0x825A5238:  lwz          r5, 0x4c(r31)
0x825A523C:  subf         r4, r11, r5
0x825A5240:  stw          r4, 0x4c(r31)
0x825A5244:  lwz          r10, 0x60(r31)
0x825A5248:  add          r3, r10, r11
0x825A524C:  stw          r3, 0x60(r31)
0x825A5250:  lwz          r11, 0x70(r31)
0x825A5254:  lwz          r10, 0x60(r31)
0x825A5258:  cmplw        cr6, r10, r11
0x825A525C:  ble          cr6, 0x825a52b0
0x825A5260:  lwz          r9, 0x40(r31)
0x825A5264:  lwz          r8, 0x60(r31)
0x825A5268:  subf         r7, r9, r8
0x825A526C:  stw          r7, 0x60(r31)
0x825A5270:  b            0x825a52b0
0x825A5274:  lwz          r11, 0x7c(r31)
0x825A5278:  li           r6, 0
0x825A527C:  li           r5, 0
0x825A5280:  lwz          r3, 0x54(r31)
0x825A5284:  add          r4, r11, r30
0x825A5288:  bl           0x825679e0
0x825A528C:  stw          r30, 0x58(r31)
0x825A5290:  stw          r30, 0x5c(r31)
0x825A5294:  lwz          r6, 0x40(r31)
0x825A5298:  stw          r6, 0x64(r31)
0x825A529C:  stw          r25, 0x4c(r31)
0x825A52A0:  lwz          r5, 0x6c(r31)
0x825A52A4:  stw          r5, 0x60(r31)
0x825A52A8:  lwz          r4, 0x6c(r31)
0x825A52AC:  stw          r4, 0x74(r31)
0x825A52B0:  addi         r26, r31, 0x4c
0x825A52B4:  lwz          r30, 0(r26)
0x825A52B8:  cmplwi       cr6, r30, 0
0x825A52BC:  beq          cr6, 0x825a53bc
0x825A52C0:  cmplw        cr6, r30, r27
0x825A52C4:  ble          cr6, 0x825a52cc
0x825A52C8:  mr           r30, r27
0x825A52CC:  lwz          r11, 0x5c(r31)
0x825A52D0:  subf         r27, r30, r27
0x825A52D4:  add          r25, r30, r25
0x825A52D8:  add          r3, r11, r30
0x825A52DC:  stw          r3, 0x5c(r31)
0x825A52E0:  lwz          r11, 0x70(r31)
0x825A52E4:  lwz          r10, 0x60(r31)
0x825A52E8:  subf         r29, r10, r11
0x825A52EC:  cmplw        cr6, r29, r30
0x825A52F0:  bgt          cr6, 0x825a535c
0x825A52F4:  mr           r5, r29
0x825A52F8:  lwz          r4, 0x60(r31)
0x825A52FC:  mr           r3, r28
0x825A5300:  bl           0x82434100
0x825A5304:  lwz          r9, 0x6c(r31)
0x825A5308:  add          r28, r29, r28
0x825A530C:  subf         r30, r29, r30
0x825A5310:  neg          r6, r29
0x825A5314:  stw          r9, 0x60(r31)
0x825A5318:  mfmsr        r5
0x825A531C:  mtmsrd       r13, 1
0x825A5320:  lwarx        r8, 0, r26
0x825A5324:  add          r7, r6, r8
0x825A5328:  stwcx.       r7, 0, r26
0x825A532C:  mtmsrd       r5, 1
0x825A5330:  bne          0x825a5318
0x825A5334:  addi         r4, r31, 0x64
0x825A5338:  mfmsr        r10
0x825A533C:  mtmsrd       r13, 1
0x825A5340:  lwarx        r3, 0, r4
0x825A5344:  add          r11, r29, r3
0x825A5348:  stwcx.       r11, 0, r4
0x825A534C:  mtmsrd       r10, 1
0x825A5350:  bne          0x825a5338
0x825A5354:  cmplwi       cr6, r30, 0
0x825A5358:  beq          cr6, 0x825a53bc
0x825A535C:  mr           r5, r30
0x825A5360:  lwz          r4, 0x60(r31)
0x825A5364:  mr           r3, r28
0x825A5368:  bl           0x82434100
0x825A536C:  lwz          r11, 0x60(r31)
0x825A5370:  add          r28, r30, r28
0x825A5374:  add          r8, r11, r30
0x825A5378:  neg          r6, r30
0x825A537C:  stw          r8, 0x60(r31)
0x825A5380:  mfmsr        r5
0x825A5384:  mtmsrd       r13, 1
0x825A5388:  lwarx        r9, 0, r26
0x825A538C:  add          r7, r6, r9
0x825A5390:  stwcx.       r7, 0, r26
0x825A5394:  mtmsrd       r5, 1
0x825A5398:  bne          0x825a5380
0x825A539C:  addi         r4, r31, 0x64
0x825A53A0:  mfmsr        r10
0x825A53A4:  mtmsrd       r13, 1
0x825A53A8:  lwarx        r3, 0, r4
0x825A53AC:  add          r11, r30, r3
0x825A53B0:  stwcx.       r11, 0, r4
0x825A53B4:  mtmsrd       r10, 1
0x825A53B8:  bne          0x825a53a0
0x825A53BC:  cmplwi       cr6, r27, 0
0x825A53C0:  beq          cr6, 0x825a5490
0x825A53C4:  cmpwi        cr6, r23, 0
0x825A53C8:  bne          cr6, 0x825a53ec
0x825A53CC:  lwz          r11, 0xf4(r31)
0x825A53D0:  li           r23, 1
0x825A53D4:  cmplwi       cr6, r11, 0
0x825A53D8:  beq          cr6, 0x825a52b4
0x825A53DC:  mr           r3, r31
0x825A53E0:  mtctr        r11
0x825A53E4:  bctrl        
0x825A53E8:  b            0x825a52b4
0x825A53EC:  stw          r25, 0x50(r1)
0x825A53F0:  bl           0x825a03b8
0x825A53F4:  li           r7, 0
0x825A53F8:  addi         r6, r1, 0x54
0x825A53FC:  mr           r5, r27
0x825A5400:  mr           r4, r28
0x825A5404:  mr           r30, r3
0x825A5408:  lwz          r3, 0x54(r31)
0x825A540C:  bl           0x82567738
0x825A5410:  lwz          r4, 0x54(r1)
0x825A5414:  cmplw        cr6, r4, r27
0x825A5418:  bge          cr6, 0x825a5424
0x825A541C:  li           r9, 1
0x825A5420:  stw          r9, 0x20(r31)
0x825A5424:  lwz          r11, 0x58(r31)
0x825A5428:  add          r25, r4, r25
0x825A542C:  add          r8, r11, r4
0x825A5430:  stw          r25, 0x50(r1)
0x825A5434:  stw          r8, 0x58(r31)
0x825A5438:  lwz          r11, 0x5c(r31)
0x825A543C:  add          r7, r11, r4
0x825A5440:  stw          r7, 0x5c(r31)
0x825A5444:  lwz          r11, 0x28(r31)
0x825A5448:  add          r6, r11, r4
0x825A544C:  stw          r6, 0x28(r31)
0x825A5450:  lwz          r5, 0x84(r31)
0x825A5454:  cmplwi       cr6, r5, 0
0x825A5458:  beq          cr6, 0x825a5468
0x825A545C:  mr           r5, r30
0x825A5460:  mr           r3, r31
0x825A5464:  bl           0x825a50f8
0x825A5468:  bl           0x825a03b8
0x825A546C:  lwz          r9, 0x30(r31)
0x825A5470:  subf         r10, r30, r3
0x825A5474:  subf         r11, r24, r3
0x825A5478:  add          r4, r10, r9
0x825A547C:  stw          r4, 0x30(r31)
0x825A5480:  lwz          r10, 0x34(r31)
0x825A5484:  add          r3, r11, r10
0x825A5488:  stw          r3, 0x34(r31)
0x825A548C:  b            0x825a54a8
0x825A5490:  stw          r25, 0x50(r1)
0x825A5494:  bl           0x825a03b8
0x825A5498:  lwz          r10, 0x34(r31)
0x825A549C:  subf         r11, r24, r3
0x825A54A0:  add          r11, r11, r10
0x825A54A4:  stw          r11, 0x34(r31)
0x825A54A8:  lwz          r10, 0x80(r31)
0x825A54AC:  lwz          r9, 0x5c(r31)
0x825A54B0:  lwz          r8, 0x40(r31)
0x825A54B4:  subf         r11, r9, r10
0x825A54B8:  cmplw        cr6, r11, r8
0x825A54BC:  stw          r11, 0x54(r1)
0x825A54C0:  blt          cr6, 0x825a54c8
0x825A54C4:  lwz          r11, 0x40(r31)
0x825A54C8:  stw          r11, 0x48(r31)
0x825A54CC:  lwz          r7, 0(r26)
0x825A54D0:  lwz          r6, 0x48(r31)
0x825A54D4:  addis        r5, r7, 2
0x825A54D8:  cmplw        cr6, r5, r6
0x825A54DC:  ble          cr6, 0x825a54e8
0x825A54E0:  lwz          r4, 0(r26)
0x825A54E4:  stw          r4, 0x48(r31)
0x825A54E8:  cmpwi        cr6, r23, 0
0x825A54EC:  beq          cr6, 0x825a5508
0x825A54F0:  lwz          r11, 0xfc(r31)
0x825A54F4:  cmplwi       cr6, r11, 0
0x825A54F8:  beq          cr6, 0x825a5508
0x825A54FC:  mr           r3, r31
0x825A5500:  mtctr        r11
0x825A5504:  bctrl        
0x825A5508:  addi         r3, r25, 3
0x825A550C:  mr           r11, r22
0x825A5510:  srwi         r10, r3, 2
0x825A5514:  cmplwi       cr6, r10, 0
0x825A5518:  stw          r11, 0x58(r1)
0x825A551C:  beq          cr6, 0x825a5540
0x825A5520:  lwbrx        r9, 0, r11
0x825A5524:  addi         r10, r10, -1
0x825A5528:  cmplwi       cr6, r10, 0
0x825A552C:  stw          r9, 0(r11)
0x825A5530:  addi         r11, r11, 4
0x825A5534:  stw          r9, 0x5c(r1)
0x825A5538:  stw          r11, 0x58(r1)
0x825A553C:  bne          cr6, 0x825a5520
0x825A5540:  mr           r3, r25
0x825A5544:  addi         r1, r1, 0xc0
0x825A5548:  b            0x8242f8d0
0x825A554C:  .align 0                       # padding
0x825A5550:  addis        r11, r4, 2
0x825A5554:  addi         r11, r11, -1
0x825A5558:  rlwinm       r3, r11, 0, 0, 0xe
0x825A555C:  blr          
0x825A5560:  mflr         r12
0x825A5564:  bl           0x8242f894
0x825A5568:  stwu         r1, -0x80(r1)
0x825A556C:  mr           r31, r3
0x825A5570:  mr           r30, r4
0x825A5574:  mr           r29, r5
0x825A5578:  mr           r28, r6
0x825A557C:  mr           r27, r7
0x825A5580:  lwz          r11, 0xf4(r31)
0x825A5584:  cmplwi       cr6, r11, 0
0x825A5588:  beq          cr6, 0x825a5594
0x825A558C:  mtctr        r11
0x825A5590:  bctrl        
0x825A5594:  rlwinm       r11, r29, 0, 0, 0xe
0x825A5598:  stw          r30, 0x6c(r31)
0x825A559C:  li           r9, 0
0x825A55A0:  stw          r30, 0x60(r31)
0x825A55A4:  add          r8, r30, r11
0x825A55A8:  stw          r30, 0x74(r31)
0x825A55AC:  lwz          r10, 0xfc(r31)
0x825A55B0:  cmplwi       cr6, r10, 0
0x825A55B4:  stw          r8, 0x70(r31)
0x825A55B8:  stw          r11, 0x40(r31)
0x825A55BC:  stw          r11, 0x64(r31)
0x825A55C0:  stw          r9, 0x4c(r31)
0x825A55C4:  stw          r28, 0x80(r31)
0x825A55C8:  stw          r27, 0x84(r31)
0x825A55CC:  beq          cr6, 0x825a55dc
0x825A55D0:  mr           r3, r31
0x825A55D4:  mtctr        r10
0x825A55D8:  bctrl        
0x825A55DC:  addi         r1, r1, 0x80
0x825A55E0:  b            0x8242f8e4
0x825A55E4:  .align 0                       # padding
0x825A55E8:  mflr         r12
0x825A55EC:  stw          r12, -8(r1)
0x825A55F0:  std          r31, -0x10(r1)
0x825A55F4:  stwu         r1, -0x60(r1)
0x825A55F8:  mr           r31, r3
0x825A55FC:  lwz          r11, 0xf4(r31)
0x825A5600:  cmplwi       cr6, r11, 0
0x825A5604:  beq          cr6, 0x825a5610
0x825A5608:  mtctr        r11
0x825A560C:  bctrl        
0x825A5610:  lwz          r11, 0x78(r31)
0x825A5614:  cmplwi       cr6, r11, 0
0x825A5618:  bne          cr6, 0x825a5624
0x825A561C:  lwz          r3, 0x54(r31)
0x825A5620:  bl           0x82566f10
0x825A5624:  lwz          r11, 0xfc(r31)
0x825A5628:  cmplwi       cr6, r11, 0
0x825A562C:  beq          cr6, 0x825a563c
0x825A5630:  mr           r3, r31
0x825A5634:  mtctr        r11
0x825A5638:  bctrl        
0x825A563C:  addi         r1, r1, 0x60
0x825A5640:  lwz          r12, -8(r1)
0x825A5644:  mtlr         r12
0x825A5648:  ld           r31, -0x10(r1)
0x825A564C:  blr          
0x825A5650:  mflr         r12
0x825A5654:  bl           0x8242f88c
0x825A5658:  stwu         r1, -0xa0(r1)
0x825A565C:  mr           r31, r3
0x825A5660:  li           r27, 0
0x825A5664:  lwz          r25, 0x2c(r31)
0x825A5668:  lwz          r11, 0x20(r31)
0x825A566C:  stw          r27, 0x50(r1)
0x825A5670:  cmplwi       cr6, r11, 0
0x825A5674:  beq          cr6, 0x825a5684
0x825A5678:  li           r3, 0
0x825A567C:  addi         r1, r1, 0xa0
0x825A5680:  b            0x8242f8dc
0x825A5684:  lwz          r10, 0x50(r31)
0x825A5688:  cmplwi       cr6, r10, 0
0x825A568C:  bne          cr6, 0x825a5678
0x825A5690:  lwz          r11, 0xf8(r31)
0x825A5694:  cmplwi       cr6, r11, 0
0x825A5698:  beq          cr6, 0x825a583c
0x825A569C:  mr           r3, r31
0x825A56A0:  mtctr        r11
0x825A56A4:  bctrl        
0x825A56A8:  cmpwi        cr6, r3, 0
0x825A56AC:  beq          cr6, 0x825a583c
0x825A56B0:  addi         r26, r31, 0x64
0x825A56B4:  lwz          r9, 0x80(r31)
0x825A56B8:  lwz          r8, 0x58(r31)
0x825A56BC:  lis          r30, 2
0x825A56C0:  subf         r11, r8, r9
0x825A56C4:  lwz          r7, 0(r26)
0x825A56C8:  cmplw        cr6, r7, r30
0x825A56CC:  blt          cr6, 0x825a5810
0x825A56D0:  cmplw        cr6, r11, r30
0x825A56D4:  blt          cr6, 0x825a5810
0x825A56D8:  bl           0x825a03b8
0x825A56DC:  li           r29, 1
0x825A56E0:  li           r7, 0
0x825A56E4:  addi         r6, r1, 0x50
0x825A56E8:  lis          r5, 2
0x825A56EC:  mr           r28, r3
0x825A56F0:  stw          r29, 0x24(r31)
0x825A56F4:  lwz          r4, 0x74(r31)
0x825A56F8:  lwz          r3, 0x54(r31)
0x825A56FC:  bl           0x82567738
0x825A5700:  lwz          r11, 0x50(r1)
0x825A5704:  stw          r27, 0x24(r31)
0x825A5708:  cmplw        cr6, r11, r30
0x825A570C:  bge          cr6, 0x825a5714
0x825A5710:  stw          r29, 0x20(r31)
0x825A5714:  lwz          r10, 0x28(r31)
0x825A5718:  add          r6, r10, r11
0x825A571C:  stw          r6, 0x28(r31)
0x825A5720:  lwz          r10, 0x58(r31)
0x825A5724:  add          r5, r10, r11
0x825A5728:  stw          r5, 0x58(r31)
0x825A572C:  lwz          r10, 0x74(r31)
0x825A5730:  add          r4, r10, r11
0x825A5734:  stw          r4, 0x74(r31)
0x825A5738:  lwz          r3, 0x70(r31)
0x825A573C:  lwz          r10, 0x74(r31)
0x825A5740:  cmplw        cr6, r10, r3
0x825A5744:  blt          cr6, 0x825a5750
0x825A5748:  lwz          r9, 0x6c(r31)
0x825A574C:  stw          r9, 0x74(r31)
0x825A5750:  neg          r8, r11
0x825A5754:  mfmsr        r5
0x825A5758:  mtmsrd       r13, 1
0x825A575C:  lwarx        r7, 0, r26
0x825A5760:  add          r6, r8, r7
0x825A5764:  stwcx.       r6, 0, r26
0x825A5768:  mtmsrd       r5, 1
0x825A576C:  bne          0x825a5754
0x825A5770:  lwz          r9, 0x50(r1)
0x825A5774:  addi         r11, r31, 0x4c
0x825A5778:  mfmsr        r10
0x825A577C:  mtmsrd       r13, 1
0x825A5780:  lwarx        r4, 0, r11
0x825A5784:  add          r3, r9, r4
0x825A5788:  stwcx.       r3, 0, r11
0x825A578C:  mtmsrd       r10, 1
0x825A5790:  bne          0x825a5778
0x825A5794:  lwz          r8, 0x44(r31)
0x825A5798:  lwz          r7, 0(r11)
0x825A579C:  cmplw        cr6, r7, r8
0x825A57A0:  ble          cr6, 0x825a57ac
0x825A57A4:  lwz          r6, 0(r11)
0x825A57A8:  stw          r6, 0x44(r31)
0x825A57AC:  lwz          r5, 0x84(r31)
0x825A57B0:  cmplwi       cr6, r5, 0
0x825A57B4:  beq          cr6, 0x825a57c8
0x825A57B8:  mr           r5, r28
0x825A57BC:  lwz          r4, 0x50(r1)
0x825A57C0:  mr           r3, r31
0x825A57C4:  bl           0x825a50f8
0x825A57C8:  bl           0x825a03b8
0x825A57CC:  lwz          r10, 0x30(r31)
0x825A57D0:  subf         r11, r28, r3
0x825A57D4:  cmpwi        cr6, r25, 0
0x825A57D8:  add          r4, r10, r11
0x825A57DC:  stw          r4, 0x30(r31)
0x825A57E0:  bne          cr6, 0x825a5800
0x825A57E4:  lwz          r3, 0x2c(r31)
0x825A57E8:  cmplwi       cr6, r3, 0
0x825A57EC:  bne          cr6, 0x825a5800
0x825A57F0:  lwz          r10, 0x38(r31)
0x825A57F4:  add          r11, r10, r11
0x825A57F8:  stw          r11, 0x38(r31)
0x825A57FC:  b            0x825a5818
0x825A5800:  lwz          r10, 0x3c(r31)
0x825A5804:  add          r10, r10, r11
0x825A5808:  stw          r10, 0x3c(r31)
0x825A580C:  b            0x825a5818
0x825A5810:  lwz          r9, 0x4c(r31)
0x825A5814:  stw          r9, 0x48(r31)
0x825A5818:  lwz          r11, 0xfc(r31)
0x825A581C:  cmplwi       cr6, r11, 0
0x825A5820:  beq          cr6, 0x825a585c
0x825A5824:  mr           r3, r31
0x825A5828:  mtctr        r11
0x825A582C:  bctrl        
0x825A5830:  lwz          r3, 0x50(r1)
0x825A5834:  addi         r1, r1, 0xa0
0x825A5838:  b            0x8242f8dc
0x825A583C:  lwz          r11, 0x100(r31)
0x825A5840:  cmplwi       cr6, r11, 0
0x825A5844:  beq          cr6, 0x825a5854
0x825A5848:  mr           r3, r31
0x825A584C:  mtctr        r11
0x825A5850:  bctrl        
0x825A5854:  li           r8, -1
0x825A5858:  stw          r8, 0x50(r1)
0x825A585C:  lwz          r3, 0x50(r1)
0x825A5860:  addi         r1, r1, 0xa0
0x825A5864:  b            0x8242f8dc
0x825A5868:  mflr         r12
0x825A586C:  stw          r12, -8(r1)
0x825A5870:  std          r31, -0x10(r1)
0x825A5874:  stwu         r1, -0x60(r1)
0x825A5878:  clrlwi       r11, r4, 0x1f
0x825A587C:  mr           r31, r3
0x825A5880:  cmplwi       cr6, r11, 0
0x825A5884:  beq          cr6, 0x825a58dc
0x825A5888:  lwz          r10, 0x50(r31)
0x825A588C:  cmplwi       cr6, r10, 0
0x825A5890:  bne          cr6, 0x825a589c
0x825A5894:  li           r9, 1
0x825A5898:  stw          r9, 0x50(r31)
0x825A589C:  rlwinm       r8, r4, 0, 0, 0
0x825A58A0:  cmplwi       cr6, r8, 0
0x825A58A4:  beq          cr6, 0x825a5910
0x825A58A8:  lwz          r11, 0xf4(r31)
0x825A58AC:  cmplwi       cr6, r11, 0
0x825A58B0:  beq          cr6, 0x825a58c0
0x825A58B4:  mr           r3, r31
0x825A58B8:  mtctr        r11
0x825A58BC:  bctrl        
0x825A58C0:  lwz          r11, 0xfc(r31)
0x825A58C4:  cmplwi       cr6, r11, 0
0x825A58C8:  beq          cr6, 0x825a5910
0x825A58CC:  mr           r3, r31
0x825A58D0:  mtctr        r11
0x825A58D4:  bctrl        
0x825A58D8:  b            0x825a5910
0x825A58DC:  rlwinm       r7, r4, 0, 0x1e, 0x1e
0x825A58E0:  cmplwi       cr6, r7, 0
0x825A58E4:  beq          cr6, 0x825a5910
0x825A58E8:  lwz          r6, 0x50(r31)
0x825A58EC:  cmplwi       cr6, r6, 1
0x825A58F0:  bne          cr6, 0x825a58fc
0x825A58F4:  li           r5, 0
0x825A58F8:  stw          r5, 0x50(r31)
0x825A58FC:  rlwinm       r4, r4, 0, 0, 0
0x825A5900:  cmplwi       cr6, r4, 0
0x825A5904:  beq          cr6, 0x825a5910
0x825A5908:  mr           r3, r31
0x825A590C:  bl           0x825a5650
0x825A5910:  lwz          r3, 0x50(r31)
0x825A5914:  addi         r1, r1, 0x60
0x825A5918:  lwz          r12, -8(r1)
0x825A591C:  mtlr         r12
0x825A5920:  ld           r31, -0x10(r1)
0x825A5924:  blr          
0x825A5928:  mflr         r12
0x825A592C:  bl           0x8242f89c
0x825A5930:  stwu         r1, -0x70(r1)
0x825A5934:  mr           r30, r4
0x825A5938:  mr           r29, r5
0x825A593C:  li           r5, 0x144
0x825A5940:  li           r4, 0
0x825A5944:  mr           r31, r3
0x825A5948:  bl           0x8242fed0
0x825A594C:  rlwinm       r11, r29, 0, 8, 8
0x825A5950:  mr           r3, r30
0x825A5954:  cmplwi       cr6, r11, 0
0x825A5958:  beq          cr6, 0x825a59dc
0x825A595C:  li           r10, 1
0x825A5960:  stw          r30, 0x54(r31)
0x825A5964:  li           r6, 1
0x825A5968:  li           r5, 0
0x825A596C:  li           r4, 0
0x825A5970:  stw          r10, 0x78(r31)
0x825A5974:  bl           0x825679e0
0x825A5978:  stw          r3, 0x7c(r31)
0x825A597C:  lis          r5, -0x7da6
0x825A5980:  lis          r6, -0x7da6
0x825A5984:  addi         r5, r5, 0x4fd8
0x825A5988:  lis          r7, -0x7da6
0x825A598C:  lis          r8, -0x7da6
0x825A5990:  lis          r9, -0x7da6
0x825A5994:  lis          r10, -0x7da6
0x825A5998:  lis          r11, -0x7da6
0x825A599C:  stw          r5, 0(r31)
0x825A59A0:  addi         r3, r7, 0x5550
0x825A59A4:  addi         r4, r6, 0x5188
0x825A59A8:  addi         r8, r8, 0x5560
0x825A59AC:  addi         r7, r9, 0x5650
0x825A59B0:  addi         r6, r10, 0x55e8
0x825A59B4:  addi         r5, r11, 0x5868
0x825A59B8:  stw          r3, 8(r31)
0x825A59BC:  stw          r4, 4(r31)
0x825A59C0:  li           r3, 1
0x825A59C4:  stw          r8, 0xc(r31)
0x825A59C8:  stw          r7, 0x10(r31)
0x825A59CC:  stw          r6, 0x14(r31)
0x825A59D0:  stw          r5, 0x18(r31)
0x825A59D4:  addi         r1, r1, 0x70
0x825A59D8:  b            0x8242f8ec
0x825A59DC:  lis          r8, 0x800
0x825A59E0:  lis          r29, -0x8000
0x825A59E4:  li           r9, 0
0x825A59E8:  ori          r8, r8, 0x80
0x825A59EC:  li           r7, 3
0x825A59F0:  li           r6, 0
0x825A59F4:  li           r5, 1
0x825A59F8:  mr           r4, r29
0x825A59FC:  bl           0x82567368
0x825A5A00:  stw          r3, 0x54(r31)
0x825A5A04:  lwz          r9, 0x54(r31)
0x825A5A08:  cmpwi        cr6, r9, -1
0x825A5A0C:  bne          cr6, 0x825a5a38
0x825A5A10:  lis          r8, 0x800
0x825A5A14:  li           r9, 0
0x825A5A18:  ori          r8, r8, 0x80
0x825A5A1C:  li           r7, 3
0x825A5A20:  li           r6, 0
0x825A5A24:  li           r5, 3
0x825A5A28:  mr           r4, r29
0x825A5A2C:  mr           r3, r30
0x825A5A30:  bl           0x82567368
0x825A5A34:  stw          r3, 0x54(r31)
0x825A5A38:  lwz          r8, 0x54(r31)
0x825A5A3C:  cmpwi        cr6, r8, -1
0x825A5A40:  bne          cr6, 0x825a597c
0x825A5A44:  li           r3, 0
0x825A5A48:  addi         r1, r1, 0x70
0x825A5A4C:  b            0x8242f8ec
0x825A5A50:  mflr         r12
0x825A5A54:  bl           0x8242f898
0x825A5A58:  addi         r12, r1, -0x28
0x825A5A5C:  bl           0x82436610
0x825A5A60:  stwu         r1, -0xb0(r1)
0x825A5A64:  li           r11, 1
0x825A5A68:  stw          r3, 0(r4)
0x825A5A6C:  mr           r30, r5
0x825A5A70:  cmpwi        cr6, r3, 2
0x825A5A74:  stw          r11, 4(r4)
0x825A5A78:  ble          cr6, 0x825a5c60
0x825A5A7C:  srawi        r31, r3, 1
0x825A5A80:  lis          r11, -0x7df8
0x825A5A84:  extsw        r10, r31
0x825A5A88:  std          r10, 0x50(r1)
0x825A5A8C:  lfd          f0, 0x50(r1)
0x825A5A90:  fcfid        f13, f0
0x825A5A94:  frsp         f0, f13
0x825A5A98:  lfs          f13, -0x6108(r11)
0x825A5A9C:  fdivs        f30, f13, f0
0x825A5AA0:  fmuls        f1, f0, f30
0x825A5AA4:  bl           0x824302b0
0x825A5AA8:  lis          r11, -0x7e00
0x825A5AAC:  frsp         f26, f1
0x825A5AB0:  stfs         f26, 4(r30)
0x825A5AB4:  cmpwi        cr6, r31, 4
0x825A5AB8:  lfs          f27, 0x3dac(r11)
0x825A5ABC:  lis          r11, -0x7e00
0x825A5AC0:  stfs         f27, 0(r30)
0x825A5AC4:  lfs          f28, 0x6a40(r11)
0x825A5AC8:  blt          cr6, 0x825a5b08
0x825A5ACC:  lis          r11, -0x7e00
0x825A5AD0:  lfs          f0, 0x3eb8(r11)
0x825A5AD4:  fmuls        f1, f30, f0
0x825A5AD8:  bl           0x824302b0
0x825A5ADC:  fmr          f0, f1
0x825A5AE0:  lis          r11, -0x7dfe
0x825A5AE4:  lfs          f13, 0x72e0(r11)
0x825A5AE8:  fmuls        f1, f30, f13
0x825A5AEC:  frsp         f12, f0
0x825A5AF0:  fdivs        f11, f28, f12
0x825A5AF4:  stfs         f11, 8(r30)
0x825A5AF8:  bl           0x824302b0
0x825A5AFC:  frsp         f10, f1
0x825A5B00:  fdivs        f9, f28, f10
0x825A5B04:  stfs         f9, 0xc(r30)
0x825A5B08:  li           r28, 4
0x825A5B0C:  cmpwi        cr6, r31, 4
0x825A5B10:  ble          cr6, 0x825a5b94
0x825A5B14:  lis          r11, -0x7e00
0x825A5B18:  addi         r29, r30, 0x18
0x825A5B1C:  lfs          f29, 0x58f8(r11)
0x825A5B20:  extsw        r9, r28
0x825A5B24:  std          r9, 0x50(r1)
0x825A5B28:  lfd          f8, 0x50(r1)
0x825A5B2C:  fcfid        f7, f8
0x825A5B30:  frsp         f6, f7
0x825A5B34:  fmuls        f31, f6, f30
0x825A5B38:  fmr          f1, f31
0x825A5B3C:  bl           0x824302b0
0x825A5B40:  fmr          f0, f1
0x825A5B44:  fmr          f1, f31
0x825A5B48:  frsp         f5, f0
0x825A5B4C:  stfs         f5, -8(r29)
0x825A5B50:  bl           0x824301d8
0x825A5B54:  fmuls        f31, f31, f29
0x825A5B58:  frsp         f4, f1
0x825A5B5C:  stfs         f4, -4(r29)
0x825A5B60:  fmr          f1, f31
0x825A5B64:  bl           0x824302b0
0x825A5B68:  fmr          f0, f1
0x825A5B6C:  fmr          f1, f31
0x825A5B70:  frsp         f3, f0
0x825A5B74:  stfs         f3, 0(r29)
0x825A5B78:  bl           0x824301d8
0x825A5B7C:  addi         r28, r28, 4
0x825A5B80:  frsp         f2, f1
0x825A5B84:  stfs         f2, 4(r29)
0x825A5B88:  addi         r29, r29, 0x10
0x825A5B8C:  cmpw         cr6, r28, r31
0x825A5B90:  blt          cr6, 0x825a5b20
0x825A5B94:  li           r11, 0
0x825A5B98:  cmpwi        cr6, r31, 2
0x825A5B9C:  ble          cr6, 0x825a5c60
0x825A5BA0:  add          r8, r11, r31
0x825A5BA4:  srawi        r31, r31, 1
0x825A5BA8:  slwi         r10, r8, 2
0x825A5BAC:  cmpwi        cr6, r31, 4
0x825A5BB0:  add          r10, r10, r30
0x825A5BB4:  stfs         f27, 0(r10)
0x825A5BB8:  stfs         f26, 4(r10)
0x825A5BBC:  blt          cr6, 0x825a5c54
0x825A5BC0:  addi         r7, r11, 6
0x825A5BC4:  addi         r6, r11, 4
0x825A5BC8:  slwi         r5, r7, 2
0x825A5BCC:  slwi         r4, r6, 2
0x825A5BD0:  addi         r3, r8, 2
0x825A5BD4:  addi         r10, r8, 3
0x825A5BD8:  slwi         r9, r3, 2
0x825A5BDC:  slwi         r7, r10, 2
0x825A5BE0:  lfsx         f0, r5, r30
0x825A5BE4:  lfsx         f1, r4, r30
0x825A5BE8:  fdivs        f12, f28, f0
0x825A5BEC:  fdivs        f13, f28, f1
0x825A5BF0:  stfsx        f13, r9, r30
0x825A5BF4:  stfsx        f12, r7, r30
0x825A5BF8:  ble          cr6, 0x825a5c54
0x825A5BFC:  addi         r5, r11, 0xa
0x825A5C00:  addi         r6, r8, 4
0x825A5C04:  addi         r4, r31, -5
0x825A5C08:  slwi         r10, r6, 2
0x825A5C0C:  slwi         r11, r5, 2
0x825A5C10:  srwi         r9, r4, 2
0x825A5C14:  add          r10, r10, r30
0x825A5C18:  add          r11, r11, r30
0x825A5C1C:  addi         r9, r9, 1
0x825A5C20:  addi         r9, r9, -1
0x825A5C24:  lfs          f0, -4(r11)
0x825A5C28:  lfs          f13, 0(r11)
0x825A5C2C:  lfs          f12, 4(r11)
0x825A5C30:  cmplwi       cr6, r9, 0
0x825A5C34:  lfs          f11, -8(r11)
0x825A5C38:  addi         r11, r11, 0x20
0x825A5C3C:  stfs         f11, 0(r10)
0x825A5C40:  stfs         f0, 4(r10)
0x825A5C44:  stfs         f13, 8(r10)
0x825A5C48:  stfs         f12, 0xc(r10)
0x825A5C4C:  addi         r10, r10, 0x10
0x825A5C50:  bne          cr6, 0x825a5c20
0x825A5C54:  mr           r11, r8
0x825A5C58:  cmpwi        cr6, r31, 2
0x825A5C5C:  bgt          cr6, 0x825a5ba0
0x825A5C60:  addi         r1, r1, 0xb0
0x825A5C64:  addi         r12, r1, -0x28
0x825A5C68:  bl           0x8243665c
0x825A5C6C:  b            0x8242f8e8
0x825A5C70:  mflr         r12
0x825A5C74:  bl           0x8242f898
0x825A5C78:  stfd         f29, -0x40(r1)
0x825A5C7C:  stfd         f30, -0x38(r1)
0x825A5C80:  stfd         f31, -0x30(r1)
0x825A5C84:  stwu         r1, -0xa0(r1)
0x825A5C88:  mr           r30, r3
0x825A5C8C:  mr           r29, r5
0x825A5C90:  cmpwi        cr6, r30, 1
0x825A5C94:  stw          r30, 4(r4)
0x825A5C98:  ble          cr6, 0x825a5d58
0x825A5C9C:  srawi        r28, r30, 1
0x825A5CA0:  extsw        r11, r28
0x825A5CA4:  std          r11, 0x50(r1)
0x825A5CA8:  lis          r11, -0x7df8
0x825A5CAC:  lfd          f0, 0x50(r1)
0x825A5CB0:  fcfid        f13, f0
0x825A5CB4:  frsp         f0, f13
0x825A5CB8:  lfs          f13, -0x6108(r11)
0x825A5CBC:  fdivs        f29, f13, f0
0x825A5CC0:  fmuls        f1, f0, f29
0x825A5CC4:  bl           0x824302b0
0x825A5CC8:  frsp         f0, f1
0x825A5CCC:  lis          r11, -0x7e00
0x825A5CD0:  slwi         r10, r28, 2
0x825A5CD4:  stfs         f0, 0(r29)
0x825A5CD8:  li           r31, 1
0x825A5CDC:  cmpwi        cr6, r28, 1
0x825A5CE0:  lfs          f31, 0x6a40(r11)
0x825A5CE4:  fmuls        f12, f0, f31
0x825A5CE8:  stfsx        f12, r10, r29
0x825A5CEC:  ble          cr6, 0x825a5d58
0x825A5CF0:  slwi         r11, r30, 2
0x825A5CF4:  addi         r30, r29, 4
0x825A5CF8:  add          r11, r11, r29
0x825A5CFC:  addi         r29, r11, -4
0x825A5D00:  extsw        r9, r31
0x825A5D04:  std          r9, 0x50(r1)
0x825A5D08:  lfd          f11, 0x50(r1)
0x825A5D0C:  fcfid        f10, f11
0x825A5D10:  frsp         f9, f10
0x825A5D14:  fmuls        f30, f9, f29
0x825A5D18:  fmr          f1, f30
0x825A5D1C:  bl           0x824302b0
0x825A5D20:  fmr          f0, f1
0x825A5D24:  fmr          f1, f30
0x825A5D28:  frsp         f8, f0
0x825A5D2C:  fmuls        f7, f8, f31
0x825A5D30:  stfs         f7, 0(r30)
0x825A5D34:  bl           0x824301d8
0x825A5D38:  frsp         f6, f1
0x825A5D3C:  addi         r31, r31, 1
0x825A5D40:  addi         r30, r30, 4
0x825A5D44:  cmpw         cr6, r31, r28
0x825A5D48:  fmuls        f5, f6, f31
0x825A5D4C:  stfs         f5, 0(r29)
0x825A5D50:  addi         r29, r29, -4
0x825A5D54:  blt          cr6, 0x825a5d00
0x825A5D58:  addi         r1, r1, 0xa0
0x825A5D5C:  lfd          f29, -0x40(r1)
0x825A5D60:  lfd          f30, -0x38(r1)
0x825A5D64:  lfd          f31, -0x30(r1)
0x825A5D68:  b            0x8242f8e8
0x825A5D6C:  .align 0                       # padding
0x825A5D70:  mflr         r12
0x825A5D74:  bl           0x8242f878
0x825A5D78:  li           r11, 0
0x825A5D7C:  mr           r7, r3
0x825A5D80:  li           r20, 1
0x825A5D84:  cmpwi        cr6, r7, 8
0x825A5D88:  stw          r11, 0(r4)
0x825A5D8C:  ble          cr6, 0x825a5ddc
0x825A5D90:  cmpwi        cr6, r20, 0
0x825A5D94:  srawi        r7, r7, 1
0x825A5D98:  ble          cr6, 0x825a5dcc
0x825A5D9C:  slwi         r11, r20, 2
0x825A5DA0:  mr           r10, r4
0x825A5DA4:  add          r9, r11, r4
0x825A5DA8:  mr           r11, r20
0x825A5DAC:  lwz          r8, 0(r10)
0x825A5DB0:  addi         r11, r11, -1
0x825A5DB4:  addi         r10, r10, 4
0x825A5DB8:  add          r8, r7, r8
0x825A5DBC:  cmplwi       cr6, r11, 0
0x825A5DC0:  stw          r8, 0(r9)
0x825A5DC4:  addi         r9, r9, 4
0x825A5DC8:  bne          cr6, 0x825a5dac
0x825A5DCC:  slwi         r20, r20, 1
0x825A5DD0:  slwi         r6, r20, 3
0x825A5DD4:  cmpw         cr6, r6, r7
0x825A5DD8:  blt          cr6, 0x825a5d90
0x825A5DDC:  slwi         r3, r20, 3
0x825A5DE0:  slwi         r11, r20, 1
0x825A5DE4:  cmpw         cr6, r3, r7
0x825A5DE8:  bne          cr6, 0x825a5f84
0x825A5DEC:  li           r21, 0
0x825A5DF0:  cmpwi        cr6, r20, 0
0x825A5DF4:  ble          cr6, 0x825a629c
0x825A5DF8:  li           r22, 0
0x825A5DFC:  mr           r23, r4
0x825A5E00:  cmpwi        cr6, r21, 0
0x825A5E04:  ble          cr6, 0x825a5f2c
0x825A5E08:  slwi         r28, r11, 1
0x825A5E0C:  li           r3, 0
0x825A5E10:  mr           r6, r4
0x825A5E14:  mr           r7, r21
0x825A5E18:  lwz          r9, 0(r6)
0x825A5E1C:  addi         r7, r7, -1
0x825A5E20:  lwz          r10, 0(r23)
0x825A5E24:  addi         r6, r6, 4
0x825A5E28:  add          r8, r22, r9
0x825A5E2C:  add          r31, r3, r10
0x825A5E30:  slwi         r10, r8, 2
0x825A5E34:  slwi         r9, r31, 2
0x825A5E38:  add          r10, r10, r5
0x825A5E3C:  add          r9, r9, r5
0x825A5E40:  addi         r30, r10, 4
0x825A5E44:  addi         r29, r9, 4
0x825A5E48:  add          r31, r31, r11
0x825A5E4C:  add          r8, r28, r8
0x825A5E50:  lfs          f11, 0(r10)
0x825A5E54:  slwi         r27, r31, 2
0x825A5E58:  lfs          f0, 0(r9)
0x825A5E5C:  slwi         r24, r8, 2
0x825A5E60:  lfs          f13, 0(r30)
0x825A5E64:  subf         r8, r11, r8
0x825A5E68:  stfs         f11, 0(r9)
0x825A5E6C:  add          r31, r31, r11
0x825A5E70:  lfs          f12, 0(r29)
0x825A5E74:  add          r9, r27, r5
0x825A5E78:  stfs         f13, 0(r29)
0x825A5E7C:  slwi         r25, r31, 2
0x825A5E80:  stfs         f0, 0(r10)
0x825A5E84:  add          r27, r28, r8
0x825A5E88:  stfs         f12, 0(r30)
0x825A5E8C:  add          r31, r31, r11
0x825A5E90:  add          r10, r24, r5
0x825A5E94:  slwi         r29, r27, 2
0x825A5E98:  lfs          f0, 0(r9)
0x825A5E9C:  slwi         r27, r31, 2
0x825A5EA0:  slwi         r26, r8, 2
0x825A5EA4:  addi         r8, r9, 4
0x825A5EA8:  addi         r31, r10, 4
0x825A5EAC:  lfs          f10, 0(r10)
0x825A5EB0:  addi         r3, r3, 2
0x825A5EB4:  cmplwi       cr6, r7, 0
0x825A5EB8:  lfs          f13, 0(r8)
0x825A5EBC:  lfs          f12, 0(r31)
0x825A5EC0:  stfs         f12, 0(r8)
0x825A5EC4:  add          r8, r26, r5
0x825A5EC8:  stfs         f10, 0(r9)
0x825A5ECC:  add          r9, r25, r5
0x825A5ED0:  addi         r30, r8, 4
0x825A5ED4:  stfs         f13, 0(r31)
0x825A5ED8:  stfs         f0, 0(r10)
0x825A5EDC:  addi         r31, r9, 4
0x825A5EE0:  add          r10, r27, r5
0x825A5EE4:  lfs          f9, 0(r8)
0x825A5EE8:  lfs          f0, 0(r9)
0x825A5EEC:  lfs          f12, 0(r30)
0x825A5EF0:  stfs         f9, 0(r9)
0x825A5EF4:  add          r9, r29, r5
0x825A5EF8:  lfs          f13, 0(r31)
0x825A5EFC:  stfs         f12, 0(r31)
0x825A5F00:  stfs         f0, 0(r8)
0x825A5F04:  stfs         f13, 0(r30)
0x825A5F08:  lfs          f0, 0(r10)
0x825A5F0C:  lfs          f8, 0(r9)
0x825A5F10:  lfs          f13, 4(r10)
0x825A5F14:  lfs          f12, 4(r9)
0x825A5F18:  stfs         f8, 0(r10)
0x825A5F1C:  stfs         f12, 4(r10)
0x825A5F20:  stfs         f0, 0(r9)
0x825A5F24:  stfs         f13, 4(r9)
0x825A5F28:  bne          cr6, 0x825a5e18
0x825A5F2C:  lwz          r10, 0(r23)
0x825A5F30:  addi         r21, r21, 1
0x825A5F34:  addi         r23, r23, 4
0x825A5F38:  add          r10, r22, r10
0x825A5F3C:  addi         r22, r22, 2
0x825A5F40:  add          r10, r10, r11
0x825A5F44:  cmpw         cr6, r21, r20
0x825A5F48:  add          r9, r10, r11
0x825A5F4C:  slwi         r10, r10, 2
0x825A5F50:  slwi         r9, r9, 2
0x825A5F54:  add          r10, r10, r5
0x825A5F58:  add          r9, r9, r5
0x825A5F5C:  lfs          f0, 0(r10)
0x825A5F60:  lfs          f7, 0(r9)
0x825A5F64:  lfs          f13, 4(r10)
0x825A5F68:  lfs          f12, 4(r9)
0x825A5F6C:  stfs         f7, 0(r10)
0x825A5F70:  stfs         f12, 4(r10)
0x825A5F74:  stfs         f0, 0(r9)
0x825A5F78:  stfs         f13, 4(r9)
0x825A5F7C:  blt          cr6, 0x825a5e00
0x825A5F80:  b            0x8242f8c8
0x825A5F84:  li           r24, 1
0x825A5F88:  cmpwi        cr6, r20, 1
0x825A5F8C:  ble          cr6, 0x825a629c
0x825A5F90:  li           r3, 2
0x825A5F94:  addi         r6, r4, 4
0x825A5F98:  li           r25, 0
0x825A5F9C:  cmpwi        cr6, r24, 4
0x825A5FA0:  blt          cr6, 0x825a61dc
0x825A5FA4:  addi         r10, r24, -4
0x825A5FA8:  li           r9, 0
0x825A5FAC:  srwi         r8, r10, 2
0x825A5FB0:  addi         r10, r4, 8
0x825A5FB4:  addi         r31, r8, 1
0x825A5FB8:  slwi         r25, r31, 2
0x825A5FBC:  lwz          r7, -8(r10)
0x825A5FC0:  lwz          r8, 0(r6)
0x825A5FC4:  add          r30, r7, r3
0x825A5FC8:  add          r29, r9, r8
0x825A5FCC:  slwi         r7, r30, 2
0x825A5FD0:  slwi         r8, r29, 2
0x825A5FD4:  add          r7, r7, r5
0x825A5FD8:  add          r8, r8, r5
0x825A5FDC:  add          r28, r30, r11
0x825A5FE0:  add          r27, r29, r11
0x825A5FE4:  addi         r29, r8, 4
0x825A5FE8:  addi         r30, r7, 4
0x825A5FEC:  lfs          f6, 0(r7)
0x825A5FF0:  lfs          f0, 0(r8)
0x825A5FF4:  slwi         r28, r28, 2
0x825A5FF8:  slwi         r27, r27, 2
0x825A5FFC:  lfs          f12, 0(r29)
0x825A6000:  lfs          f13, 0(r30)
0x825A6004:  stfs         f6, 0(r8)
0x825A6008:  add          r8, r27, r5
0x825A600C:  stfs         f13, 0(r29)
0x825A6010:  stfs         f0, 0(r7)
0x825A6014:  add          r7, r28, r5
0x825A6018:  stfs         f12, 0(r30)
0x825A601C:  lfs          f0, 0(r8)
0x825A6020:  lfs          f13, 4(r8)
0x825A6024:  lfs          f12, 4(r7)
0x825A6028:  lfs          f5, 0(r7)
0x825A602C:  stfs         f5, 0(r8)
0x825A6030:  stfs         f12, 4(r8)
0x825A6034:  stfs         f0, 0(r7)
0x825A6038:  stfs         f13, 4(r7)
0x825A603C:  lwz          r7, 0(r6)
0x825A6040:  lwz          r8, -4(r10)
0x825A6044:  add          r7, r9, r7
0x825A6048:  add          r8, r8, r3
0x825A604C:  addi         r30, r7, 2
0x825A6050:  add          r29, r8, r11
0x825A6054:  slwi         r7, r8, 2
0x825A6058:  slwi         r8, r30, 2
0x825A605C:  add          r7, r7, r5
0x825A6060:  add          r8, r8, r5
0x825A6064:  slwi         r27, r29, 2
0x825A6068:  add          r28, r30, r11
0x825A606C:  addi         r29, r8, 4
0x825A6070:  addi         r30, r7, 4
0x825A6074:  lfs          f4, 0(r7)
0x825A6078:  lfs          f0, 0(r8)
0x825A607C:  slwi         r28, r28, 2
0x825A6080:  lfs          f12, 0(r29)
0x825A6084:  lfs          f13, 0(r30)
0x825A6088:  stfs         f4, 0(r8)
0x825A608C:  add          r8, r28, r5
0x825A6090:  stfs         f13, 0(r29)
0x825A6094:  stfs         f0, 0(r7)
0x825A6098:  add          r7, r27, r5
0x825A609C:  stfs         f12, 0(r30)
0x825A60A0:  addi         r30, r9, 6
0x825A60A4:  lfs          f0, 0(r8)
0x825A60A8:  lfs          f13, 4(r8)
0x825A60AC:  lfs          f3, 0(r7)
0x825A60B0:  lfs          f12, 4(r7)
0x825A60B4:  stfs         f3, 0(r8)
0x825A60B8:  stfs         f12, 4(r8)
0x825A60BC:  stfs         f0, 0(r7)
0x825A60C0:  stfs         f13, 4(r7)
0x825A60C4:  lwz          r8, 0(r10)
0x825A60C8:  lwz          r7, 0(r6)
0x825A60CC:  add          r8, r3, r8
0x825A60D0:  add          r7, r30, r7
0x825A60D4:  slwi         r28, r8, 2
0x825A60D8:  addi         r29, r7, -2
0x825A60DC:  add          r7, r28, r5
0x825A60E0:  add          r27, r8, r11
0x825A60E4:  addi         r28, r7, 4
0x825A60E8:  slwi         r8, r29, 2
0x825A60EC:  add          r26, r29, r11
0x825A60F0:  lfs          f2, 0(r7)
0x825A60F4:  add          r8, r8, r5
0x825A60F8:  slwi         r27, r27, 2
0x825A60FC:  lfs          f0, 0(r28)
0x825A6100:  addi         r29, r8, 4
0x825A6104:  lfs          f13, 0(r8)
0x825A6108:  stfs         f2, 0(r8)
0x825A610C:  slwi         r8, r26, 2
0x825A6110:  addi         r31, r31, -1
0x825A6114:  add          r8, r8, r5
0x825A6118:  addi         r9, r9, 8
0x825A611C:  lfs          f12, 0(r29)
0x825A6120:  cmplwi       cr6, r31, 0
0x825A6124:  stfs         f0, 0(r29)
0x825A6128:  stfs         f13, 0(r7)
0x825A612C:  add          r7, r27, r5
0x825A6130:  stfs         f12, 0(r28)
0x825A6134:  lfs          f0, 0(r8)
0x825A6138:  lfs          f13, 4(r8)
0x825A613C:  lfs          f12, 4(r7)
0x825A6140:  lfs          f1, 0(r7)
0x825A6144:  stfs         f1, 0(r8)
0x825A6148:  stfs         f12, 4(r8)
0x825A614C:  stfs         f0, 0(r7)
0x825A6150:  stfs         f13, 4(r7)
0x825A6154:  lwz          r7, 4(r10)
0x825A6158:  lwz          r8, 0(r6)
0x825A615C:  addi         r10, r10, 0x10
0x825A6160:  add          r29, r7, r3
0x825A6164:  add          r30, r30, r8
0x825A6168:  slwi         r7, r29, 2
0x825A616C:  slwi         r8, r30, 2
0x825A6170:  add          r7, r7, r5
0x825A6174:  add          r8, r8, r5
0x825A6178:  add          r28, r29, r11
0x825A617C:  add          r27, r30, r11
0x825A6180:  addi         r30, r7, 4
0x825A6184:  addi         r29, r8, 4
0x825A6188:  lfs          f11, 0(r7)
0x825A618C:  lfs          f0, 0(r8)
0x825A6190:  slwi         r28, r28, 2
0x825A6194:  slwi         r27, r27, 2
0x825A6198:  lfs          f13, 0(r30)
0x825A619C:  lfs          f12, 0(r29)
0x825A61A0:  stfs         f11, 0(r8)
0x825A61A4:  add          r8, r27, r5
0x825A61A8:  stfs         f13, 0(r29)
0x825A61AC:  stfs         f0, 0(r7)
0x825A61B0:  add          r7, r28, r5
0x825A61B4:  stfs         f12, 0(r30)
0x825A61B8:  lfs          f0, 0(r8)
0x825A61BC:  lfs          f13, 4(r8)
0x825A61C0:  lfs          f10, 0(r7)
0x825A61C4:  lfs          f12, 4(r7)
0x825A61C8:  stfs         f10, 0(r8)
0x825A61CC:  stfs         f12, 4(r8)
0x825A61D0:  stfs         f0, 0(r7)
0x825A61D4:  stfs         f13, 4(r7)
0x825A61D8:  bne          cr6, 0x825a5fbc
0x825A61DC:  cmpw         cr6, r25, r24
0x825A61E0:  bge          cr6, 0x825a6288
0x825A61E4:  slwi         r10, r25, 2
0x825A61E8:  slwi         r31, r25, 1
0x825A61EC:  add          r7, r10, r4
0x825A61F0:  subf         r8, r25, r24
0x825A61F4:  lwz          r9, 0(r7)
0x825A61F8:  addi         r8, r8, -1
0x825A61FC:  lwz          r10, 0(r6)
0x825A6200:  addi         r7, r7, 4
0x825A6204:  add          r30, r3, r9
0x825A6208:  add          r29, r31, r10
0x825A620C:  slwi         r9, r30, 2
0x825A6210:  slwi         r10, r29, 2
0x825A6214:  add          r9, r9, r5
0x825A6218:  add          r10, r10, r5
0x825A621C:  add          r28, r30, r11
0x825A6220:  add          r27, r29, r11
0x825A6224:  addi         r30, r9, 4
0x825A6228:  addi         r29, r10, 4
0x825A622C:  lfs          f9, 0(r9)
0x825A6230:  lfs          f0, 0(r10)
0x825A6234:  slwi         r28, r28, 2
0x825A6238:  slwi         r27, r27, 2
0x825A623C:  addi         r31, r31, 2
0x825A6240:  lfs          f13, 0(r30)
0x825A6244:  cmplwi       cr6, r8, 0
0x825A6248:  lfs          f12, 0(r29)
0x825A624C:  stfs         f9, 0(r10)
0x825A6250:  add          r10, r27, r5
0x825A6254:  stfs         f13, 0(r29)
0x825A6258:  stfs         f0, 0(r9)
0x825A625C:  add          r9, r28, r5
0x825A6260:  stfs         f12, 0(r30)
0x825A6264:  lfs          f0, 0(r10)
0x825A6268:  lfs          f13, 4(r10)
0x825A626C:  lfs          f8, 0(r9)
0x825A6270:  lfs          f12, 4(r9)
0x825A6274:  stfs         f8, 0(r10)
0x825A6278:  stfs         f12, 4(r10)
0x825A627C:  stfs         f0, 0(r9)
0x825A6280:  stfs         f13, 4(r9)
0x825A6284:  bne          cr6, 0x825a61f4
0x825A6288:  addi         r24, r24, 1
0x825A628C:  addi         r6, r6, 4
0x825A6290:  addi         r3, r3, 2
0x825A6294:  cmpw         cr6, r24, r20
0x825A6298:  blt          cr6, 0x825a5f98
0x825A629C:  b            0x8242f8c8
0x825A62A0:  mflr         r12
0x825A62A4:  bl           0x8242f878
0x825A62A8:  li           r11, 0
0x825A62AC:  mr           r7, r3
0x825A62B0:  li           r20, 1
0x825A62B4:  cmpwi        cr6, r7, 8
0x825A62B8:  stw          r11, 0(r4)
0x825A62BC:  ble          cr6, 0x825a630c
0x825A62C0:  cmpwi        cr6, r20, 0
0x825A62C4:  srawi        r7, r7, 1
0x825A62C8:  ble          cr6, 0x825a62fc
0x825A62CC:  slwi         r11, r20, 2
0x825A62D0:  mr           r10, r4
0x825A62D4:  add          r9, r11, r4
0x825A62D8:  mr           r11, r20
0x825A62DC:  lwz          r8, 0(r10)
0x825A62E0:  addi         r11, r11, -1
0x825A62E4:  addi         r10, r10, 4
0x825A62E8:  add          r8, r8, r7
0x825A62EC:  cmplwi       cr6, r11, 0
0x825A62F0:  stw          r8, 0(r9)
0x825A62F4:  addi         r9, r9, 4
0x825A62F8:  bne          cr6, 0x825a62dc
0x825A62FC:  slwi         r20, r20, 1
0x825A6300:  slwi         r6, r20, 3
0x825A6304:  cmpw         cr6, r6, r7
0x825A6308:  blt          cr6, 0x825a62c0
0x825A630C:  slwi         r3, r20, 3
0x825A6310:  slwi         r11, r20, 1
0x825A6314:  cmpw         cr6, r3, r7
0x825A6318:  bne          cr6, 0x825a6508
0x825A631C:  li           r21, 0
0x825A6320:  cmpwi        cr6, r20, 0
0x825A6324:  ble          cr6, 0x825a68c0
0x825A6328:  li           r22, 0
0x825A632C:  mr           r23, r4
0x825A6330:  cmpwi        cr6, r21, 0
0x825A6334:  ble          cr6, 0x825a647c
0x825A6338:  slwi         r28, r11, 1
0x825A633C:  li           r6, 0
0x825A6340:  mr           r3, r4
0x825A6344:  mr           r7, r21
0x825A6348:  lwz          r9, 0(r3)
0x825A634C:  addi         r7, r7, -1
0x825A6350:  lwz          r10, 0(r23)
0x825A6354:  addi         r3, r3, 4
0x825A6358:  add          r8, r22, r9
0x825A635C:  add          r31, r6, r10
0x825A6360:  slwi         r10, r8, 2
0x825A6364:  slwi         r9, r31, 2
0x825A6368:  add          r10, r10, r5
0x825A636C:  add          r9, r9, r5
0x825A6370:  addi         r30, r10, 4
0x825A6374:  addi         r29, r9, 4
0x825A6378:  add          r31, r31, r11
0x825A637C:  add          r8, r28, r8
0x825A6380:  lfs          f12, 0(r10)
0x825A6384:  slwi         r27, r31, 2
0x825A6388:  lfs          f0, 0(r9)
0x825A638C:  slwi         r24, r8, 2
0x825A6390:  lfs          f11, 0(r30)
0x825A6394:  subf         r8, r11, r8
0x825A6398:  stfs         f12, 0(r9)
0x825A639C:  add          r31, r31, r11
0x825A63A0:  lfs          f10, 0(r29)
0x825A63A4:  add          r9, r27, r5
0x825A63A8:  fneg         f13, f11
0x825A63AC:  stfs         f13, 0(r29)
0x825A63B0:  slwi         r25, r31, 2
0x825A63B4:  add          r27, r28, r8
0x825A63B8:  stfs         f0, 0(r10)
0x825A63BC:  add          r31, r31, r11
0x825A63C0:  fneg         f12, f10
0x825A63C4:  add          r10, r24, r5
0x825A63C8:  stfs         f12, 0(r30)
0x825A63CC:  slwi         r29, r27, 2
0x825A63D0:  lfs          f0, 0(r9)
0x825A63D4:  slwi         r27, r31, 2
0x825A63D8:  addi         r31, r10, 4
0x825A63DC:  slwi         r26, r8, 2
0x825A63E0:  addi         r8, r9, 4
0x825A63E4:  lfs          f9, 0(r10)
0x825A63E8:  addi         r6, r6, 2
0x825A63EC:  cmplwi       cr6, r7, 0
0x825A63F0:  lfs          f7, 0(r31)
0x825A63F4:  fneg         f12, f7
0x825A63F8:  stfs         f9, 0(r9)
0x825A63FC:  lfs          f8, 0(r8)
0x825A6400:  add          r9, r25, r5
0x825A6404:  stfs         f12, 0(r8)
0x825A6408:  add          r8, r26, r5
0x825A640C:  fneg         f13, f8
0x825A6410:  stfs         f13, 0(r31)
0x825A6414:  addi         r30, r8, 4
0x825A6418:  stfs         f0, 0(r10)
0x825A641C:  addi         r31, r9, 4
0x825A6420:  lfs          f0, 0(r9)
0x825A6424:  add          r10, r27, r5
0x825A6428:  lfs          f6, 0(r8)
0x825A642C:  lfs          f4, 0(r30)
0x825A6430:  stfs         f6, 0(r9)
0x825A6434:  add          r9, r29, r5
0x825A6438:  lfs          f5, 0(r31)
0x825A643C:  fneg         f12, f4
0x825A6440:  stfs         f12, 0(r31)
0x825A6444:  fneg         f13, f5
0x825A6448:  stfs         f13, 0(r30)
0x825A644C:  stfs         f0, 0(r8)
0x825A6450:  lfs          f1, 4(r9)
0x825A6454:  lfs          f2, 4(r10)
0x825A6458:  fneg         f12, f1
0x825A645C:  lfs          f0, 0(r10)
0x825A6460:  fneg         f13, f2
0x825A6464:  lfs          f3, 0(r9)
0x825A6468:  stfs         f3, 0(r10)
0x825A646C:  stfs         f12, 4(r10)
0x825A6470:  stfs         f0, 0(r9)
0x825A6474:  stfs         f13, 4(r9)
0x825A6478:  bne          cr6, 0x825a6348
0x825A647C:  lwz          r10, 0(r23)
0x825A6480:  addi         r21, r21, 1
0x825A6484:  addi         r23, r23, 4
0x825A6488:  add          r10, r22, r10
0x825A648C:  addi         r22, r22, 2
0x825A6490:  add          r9, r10, r11
0x825A6494:  addi         r10, r10, 1
0x825A6498:  add          r8, r9, r11
0x825A649C:  slwi         r7, r10, 2
0x825A64A0:  slwi         r10, r9, 2
0x825A64A4:  slwi         r9, r8, 2
0x825A64A8:  add          r8, r8, r11
0x825A64AC:  add          r9, r9, r5
0x825A64B0:  add          r10, r10, r5
0x825A64B4:  lfsx         f0, r7, r5
0x825A64B8:  addi         r8, r8, 1
0x825A64BC:  fneg         f13, f0
0x825A64C0:  stfsx        f13, r7, r5
0x825A64C4:  cmpw         cr6, r21, r20
0x825A64C8:  slwi         r8, r8, 2
0x825A64CC:  lfs          f9, 4(r9)
0x825A64D0:  lfs          f10, 4(r10)
0x825A64D4:  fneg         f12, f9
0x825A64D8:  lfs          f0, 0(r10)
0x825A64DC:  fneg         f13, f10
0x825A64E0:  lfs          f11, 0(r9)
0x825A64E4:  stfs         f11, 0(r10)
0x825A64E8:  stfs         f12, 4(r10)
0x825A64EC:  stfs         f13, 4(r9)
0x825A64F0:  stfs         f0, 0(r9)
0x825A64F4:  lfsx         f8, r8, r5
0x825A64F8:  fneg         f7, f8
0x825A64FC:  stfsx        f7, r8, r5
0x825A6500:  blt          cr6, 0x825a6330
0x825A6504:  b            0x8242f8c8
0x825A6508:  addi         r7, r11, 1
0x825A650C:  lfs          f6, 4(r5)
0x825A6510:  fneg         f5, f6
0x825A6514:  stfs         f5, 4(r5)
0x825A6518:  slwi         r10, r7, 2
0x825A651C:  li           r24, 1
0x825A6520:  cmpwi        cr6, r20, 1
0x825A6524:  lfsx         f4, r10, r5
0x825A6528:  fneg         f3, f4
0x825A652C:  stfsx        f3, r10, r5
0x825A6530:  ble          cr6, 0x825a68c0
0x825A6534:  li           r3, 2
0x825A6538:  addi         r6, r4, 4
0x825A653C:  li           r25, 0
0x825A6540:  cmpwi        cr6, r24, 4
0x825A6544:  blt          cr6, 0x825a67bc
0x825A6548:  addi         r10, r24, -4
0x825A654C:  li           r9, 0
0x825A6550:  srwi         r8, r10, 2
0x825A6554:  addi         r10, r4, 8
0x825A6558:  addi         r31, r8, 1
0x825A655C:  slwi         r25, r31, 2
0x825A6560:  lwz          r7, -8(r10)
0x825A6564:  lwz          r8, 0(r6)
0x825A6568:  add          r30, r7, r3
0x825A656C:  add          r29, r9, r8
0x825A6570:  slwi         r7, r30, 2
0x825A6574:  slwi         r8, r29, 2
0x825A6578:  add          r7, r7, r5
0x825A657C:  add          r8, r8, r5
0x825A6580:  add          r28, r30, r11
0x825A6584:  addi         r30, r7, 4
0x825A6588:  add          r27, r29, r11
0x825A658C:  addi         r29, r8, 4
0x825A6590:  lfs          f2, 0(r7)
0x825A6594:  lfs          f0, 0(r8)
0x825A6598:  slwi         r28, r28, 2
0x825A659C:  slwi         r27, r27, 2
0x825A65A0:  lfs          f1, 0(r30)
0x825A65A4:  stfs         f2, 0(r8)
0x825A65A8:  fneg         f13, f1
0x825A65AC:  lfs          f12, 0(r29)
0x825A65B0:  add          r8, r27, r5
0x825A65B4:  stfs         f13, 0(r29)
0x825A65B8:  fneg         f12, f12
0x825A65BC:  stfs         f0, 0(r7)
0x825A65C0:  add          r7, r28, r5
0x825A65C4:  stfs         f12, 0(r30)
0x825A65C8:  lfs          f10, 4(r8)
0x825A65CC:  lfs          f0, 0(r8)
0x825A65D0:  fneg         f13, f10
0x825A65D4:  lfs          f9, 4(r7)
0x825A65D8:  lfs          f11, 0(r7)
0x825A65DC:  fneg         f12, f9
0x825A65E0:  stfs         f12, 4(r8)
0x825A65E4:  stfs         f11, 0(r8)
0x825A65E8:  stfs         f13, 4(r7)
0x825A65EC:  stfs         f0, 0(r7)
0x825A65F0:  lwz          r7, 0(r6)
0x825A65F4:  lwz          r8, -4(r10)
0x825A65F8:  add          r7, r9, r7
0x825A65FC:  add          r8, r8, r3
0x825A6600:  addi         r30, r7, 2
0x825A6604:  slwi         r7, r8, 2
0x825A6608:  add          r29, r8, r11
0x825A660C:  slwi         r8, r30, 2
0x825A6610:  add          r7, r7, r5
0x825A6614:  add          r8, r8, r5
0x825A6618:  add          r28, r30, r11
0x825A661C:  addi         r30, r7, 4
0x825A6620:  slwi         r27, r29, 2
0x825A6624:  addi         r29, r8, 4
0x825A6628:  lfs          f8, 0(r7)
0x825A662C:  lfs          f0, 0(r8)
0x825A6630:  slwi         r28, r28, 2
0x825A6634:  lfs          f7, 0(r30)
0x825A6638:  fneg         f13, f7
0x825A663C:  stfs         f8, 0(r8)
0x825A6640:  lfs          f6, 0(r29)
0x825A6644:  add          r8, r28, r5
0x825A6648:  stfs         f13, 0(r29)
0x825A664C:  fneg         f12, f6
0x825A6650:  stfs         f0, 0(r7)
0x825A6654:  add          r7, r27, r5
0x825A6658:  stfs         f12, 0(r30)
0x825A665C:  addi         r30, r9, 6
0x825A6660:  lfs          f5, 4(r8)
0x825A6664:  lfs          f0, 0(r8)
0x825A6668:  fneg         f13, f5
0x825A666C:  lfs          f4, 4(r7)
0x825A6670:  lfs          f3, 0(r7)
0x825A6674:  fneg         f12, f4
0x825A6678:  stfs         f3, 0(r8)
0x825A667C:  stfs         f12, 4(r8)
0x825A6680:  stfs         f0, 0(r7)
0x825A6684:  stfs         f13, 4(r7)
0x825A6688:  lwz          r7, 0(r6)
0x825A668C:  lwz          r8, 0(r10)
0x825A6690:  add          r7, r30, r7
0x825A6694:  add          r8, r3, r8
0x825A6698:  addi         r29, r7, -2
0x825A669C:  slwi         r7, r8, 2
0x825A66A0:  add          r28, r8, r11
0x825A66A4:  slwi         r8, r29, 2
0x825A66A8:  add          r7, r7, r5
0x825A66AC:  add          r8, r8, r5
0x825A66B0:  add          r27, r29, r11
0x825A66B4:  addi         r29, r7, 4
0x825A66B8:  slwi         r26, r28, 2
0x825A66BC:  addi         r28, r8, 4
0x825A66C0:  lfs          f2, 0(r7)
0x825A66C4:  lfs          f0, 0(r8)
0x825A66C8:  slwi         r27, r27, 2
0x825A66CC:  addi         r31, r31, -1
0x825A66D0:  lfs          f1, 0(r29)
0x825A66D4:  addi         r9, r9, 8
0x825A66D8:  fneg         f13, f1
0x825A66DC:  stfs         f2, 0(r8)
0x825A66E0:  lfs          f12, 0(r28)
0x825A66E4:  add          r8, r27, r5
0x825A66E8:  stfs         f13, 0(r28)
0x825A66EC:  fneg         f12, f12
0x825A66F0:  stfs         f0, 0(r7)
0x825A66F4:  add          r7, r26, r5
0x825A66F8:  stfs         f12, 0(r29)
0x825A66FC:  cmplwi       cr6, r31, 0
0x825A6700:  lfs          f10, 4(r8)
0x825A6704:  lfs          f0, 0(r8)
0x825A6708:  fneg         f13, f10
0x825A670C:  lfs          f9, 4(r7)
0x825A6710:  lfs          f11, 0(r7)
0x825A6714:  fneg         f12, f9
0x825A6718:  stfs         f12, 4(r8)
0x825A671C:  stfs         f11, 0(r8)
0x825A6720:  stfs         f13, 4(r7)
0x825A6724:  stfs         f0, 0(r7)
0x825A6728:  lwz          r8, 0(r6)
0x825A672C:  lwz          r7, 4(r10)
0x825A6730:  addi         r10, r10, 0x10
0x825A6734:  add          r30, r30, r8
0x825A6738:  add          r29, r7, r3
0x825A673C:  slwi         r7, r30, 2
0x825A6740:  slwi         r8, r29, 2
0x825A6744:  add          r7, r7, r5
0x825A6748:  add          r8, r8, r5
0x825A674C:  add          r28, r30, r11
0x825A6750:  addi         r30, r7, 4
0x825A6754:  add          r29, r29, r11
0x825A6758:  lfs          f0, 0(r7)
0x825A675C:  slwi         r28, r28, 2
0x825A6760:  lfs          f7, 4(r8)
0x825A6764:  lfs          f8, 0(r8)
0x825A6768:  fneg         f13, f7
0x825A676C:  lfs          f6, 0(r30)
0x825A6770:  stfs         f8, 0(r7)
0x825A6774:  fneg         f12, f6
0x825A6778:  stfs         f13, 0(r30)
0x825A677C:  add          r7, r28, r5
0x825A6780:  stfs         f12, 4(r8)
0x825A6784:  stfs         f0, 0(r8)
0x825A6788:  slwi         r8, r29, 2
0x825A678C:  add          r8, r8, r5
0x825A6790:  lfs          f5, 4(r7)
0x825A6794:  lfs          f0, 0(r7)
0x825A6798:  fneg         f13, f5
0x825A679C:  lfs          f3, 4(r8)
0x825A67A0:  lfs          f4, 0(r8)
0x825A67A4:  fneg         f12, f3
0x825A67A8:  stfs         f4, 0(r7)
0x825A67AC:  stfs         f12, 4(r7)
0x825A67B0:  stfs         f0, 0(r8)
0x825A67B4:  stfs         f13, 4(r8)
0x825A67B8:  bne          cr6, 0x825a6560
0x825A67BC:  cmpw         cr6, r25, r24
0x825A67C0:  bge          cr6, 0x825a6878
0x825A67C4:  slwi         r10, r25, 2
0x825A67C8:  slwi         r31, r25, 1
0x825A67CC:  add          r7, r10, r4
0x825A67D0:  subf         r8, r25, r24
0x825A67D4:  lwz          r9, 0(r7)
0x825A67D8:  addi         r8, r8, -1
0x825A67DC:  lwz          r10, 0(r6)
0x825A67E0:  addi         r7, r7, 4
0x825A67E4:  add          r30, r3, r9
0x825A67E8:  add          r29, r31, r10
0x825A67EC:  slwi         r9, r30, 2
0x825A67F0:  slwi         r10, r29, 2
0x825A67F4:  add          r9, r9, r5
0x825A67F8:  add          r10, r10, r5
0x825A67FC:  add          r28, r30, r11
0x825A6800:  addi         r30, r9, 4
0x825A6804:  add          r27, r29, r11
0x825A6808:  addi         r29, r10, 4
0x825A680C:  lfs          f2, 0(r9)
0x825A6810:  lfs          f0, 0(r10)
0x825A6814:  slwi         r28, r28, 2
0x825A6818:  slwi         r27, r27, 2
0x825A681C:  lfs          f1, 0(r30)
0x825A6820:  addi         r31, r31, 2
0x825A6824:  fneg         f13, f1
0x825A6828:  stfs         f2, 0(r10)
0x825A682C:  lfs          f12, 0(r29)
0x825A6830:  add          r10, r27, r5
0x825A6834:  stfs         f13, 0(r29)
0x825A6838:  fneg         f12, f12
0x825A683C:  stfs         f0, 0(r9)
0x825A6840:  add          r9, r28, r5
0x825A6844:  stfs         f12, 0(r30)
0x825A6848:  cmplwi       cr6, r8, 0
0x825A684C:  lfs          f10, 4(r10)
0x825A6850:  lfs          f0, 0(r10)
0x825A6854:  fneg         f13, f10
0x825A6858:  lfs          f9, 4(r9)
0x825A685C:  lfs          f11, 0(r9)
0x825A6860:  fneg         f12, f9
0x825A6864:  stfs         f11, 0(r10)
0x825A6868:  stfs         f12, 4(r10)
0x825A686C:  stfs         f0, 0(r9)
0x825A6870:  stfs         f13, 4(r9)
0x825A6874:  bne          cr6, 0x825a67d4
0x825A6878:  lwz          r10, 0(r6)
0x825A687C:  addi         r24, r24, 1
0x825A6880:  addi         r6, r6, 4
0x825A6884:  add          r10, r3, r10
0x825A6888:  addi         r3, r3, 2
0x825A688C:  addi         r9, r10, 1
0x825A6890:  add          r10, r10, r11
0x825A6894:  slwi         r9, r9, 2
0x825A6898:  addi         r8, r10, 1
0x825A689C:  cmpw         cr6, r24, r20
0x825A68A0:  slwi         r10, r8, 2
0x825A68A4:  lfsx         f8, r9, r5
0x825A68A8:  fneg         f7, f8
0x825A68AC:  stfsx        f7, r9, r5
0x825A68B0:  lfsx         f6, r10, r5
0x825A68B4:  fneg         f5, f6
0x825A68B8:  stfsx        f5, r10, r5
0x825A68BC:  blt          cr6, 0x825a653c
0x825A68C0:  b            0x8242f8c8
0x825A68C4:  .align 0                       # padding
0x825A68C8:  mflr         r12
0x825A68CC:  stw          r12, -8(r1)
0x825A68D0:  addi         r12, r1, -8
0x825A68D4:  bl           0x824365ec
0x825A68D8:  lfs          f0, 8(r3)
0x825A68DC:  lfs          f2, 0x40(r3)
0x825A68E0:  stfs         f0, 0x40(r3)
0x825A68E4:  lfs          f13, 0xc(r3)
0x825A68E8:  lfs          f12, 0x18(r3)
0x825A68EC:  lfs          f11, 0x1c(r3)
0x825A68F0:  lfs          f10, 0x28(r3)
0x825A68F4:  lfs          f9, 0x2c(r3)
0x825A68F8:  lfs          f8, 0x10(r3)
0x825A68FC:  lfs          f7, 0x14(r3)
0x825A6900:  lfs          f6, 0x20(r3)
0x825A6904:  lfs          f5, 0x24(r3)
0x825A6908:  lfs          f4, 0x30(r3)
0x825A690C:  lfs          f3, 0x34(r3)
0x825A6910:  lfs          f1, 0x44(r3)
0x825A6914:  lfs          f27, 0x38(r3)
0x825A6918:  lfs          f26, 0x3c(r3)
0x825A691C:  lfs          f25, 0x48(r3)
0x825A6920:  lfs          f24, 0x78(r3)
0x825A6924:  lfs          f23, 0x7c(r3)
0x825A6928:  lfs          f22, 0x58(r3)
0x825A692C:  lfs          f21, 0x5c(r3)
0x825A6930:  lfs          f20, 0x68(r3)
0x825A6934:  lfs          f19, 0x6c(r3)
0x825A6938:  lfs          f18, 0x4c(r3)
0x825A693C:  lfs          f17, 0x70(r3)
0x825A6940:  lfs          f31, 0x50(r3)
0x825A6944:  lfs          f30, 0x54(r3)
0x825A6948:  lfs          f29, 0x60(r3)
0x825A694C:  lfs          f28, 0x64(r3)
0x825A6950:  lfs          f0, 0x74(r3)
0x825A6954:  stfs         f24, 8(r3)
0x825A6958:  stfs         f23, 0xc(r3)
0x825A695C:  stfs         f27, 0x10(r3)
0x825A6960:  stfs         f26, 0x14(r3)
0x825A6964:  stfs         f22, 0x18(r3)
0x825A6968:  stfs         f21, 0x1c(r3)
0x825A696C:  stfs         f20, 0x28(r3)
0x825A6970:  stfs         f19, 0x2c(r3)
0x825A6974:  stfs         f25, 0x38(r3)
0x825A6978:  stfs         f18, 0x3c(r3)
0x825A697C:  stfs         f17, 0x48(r3)
0x825A6980:  stfs         f13, 0x44(r3)
0x825A6984:  stfs         f12, 0x20(r3)
0x825A6988:  stfs         f11, 0x24(r3)
0x825A698C:  stfs         f10, 0x30(r3)
0x825A6990:  stfs         f9, 0x34(r3)
0x825A6994:  stfs         f4, 0x50(r3)
0x825A6998:  stfs         f3, 0x54(r3)
0x825A699C:  stfs         f31, 0x58(r3)
0x825A69A0:  stfs         f30, 0x5c(r3)
0x825A69A4:  stfs         f8, 0x60(r3)
0x825A69A8:  stfs         f7, 0x64(r3)
0x825A69AC:  stfs         f29, 0x68(r3)
0x825A69B0:  stfs         f28, 0x6c(r3)
0x825A69B4:  stfs         f6, 0x70(r3)
0x825A69B8:  stfs         f2, 0x78(r3)
0x825A69BC:  stfs         f1, 0x7c(r3)
0x825A69C0:  stfs         f0, 0x4c(r3)
0x825A69C4:  stfs         f5, 0x74(r3)
0x825A69C8:  addi         r12, r1, -8
0x825A69CC:  bl           0x82436638
0x825A69D0:  lwz          r12, -8(r1)
0x825A69D4:  mtlr         r12
0x825A69D8:  blr          
0x825A69DC:  .align 0                       # padding
0x825A69E0:  mflr         r12
0x825A69E4:  bl           0x8242f87c
0x825A69E8:  addi         r12, r1, -0x60
0x825A69EC:  bl           0x824365e0
0x825A69F0:  lis          r11, -0x7e00
0x825A69F4:  lfs          f10, 0(r4)
0x825A69F8:  srawi        r28, r3, 3
0x825A69FC:  lfs          f9, 4(r4)
0x825A6A00:  addi         r30, r5, 8
0x825A6A04:  slwi         r3, r28, 1
0x825A6A08:  addi         r21, r28, -2
0x825A6A0C:  lfs          f0, 0x3dac(r11)
0x825A6A10:  lis          r11, -0x7e00
0x825A6A14:  slwi         r10, r3, 1
0x825A6A18:  fmr          f12, f0
0x825A6A1C:  cmpwi        cr6, r21, 2
0x825A6A20:  add          r9, r10, r3
0x825A6A24:  slwi         r10, r10, 2
0x825A6A28:  lfs          f13, 0x3da8(r11)
0x825A6A2C:  slwi         r9, r9, 2
0x825A6A30:  slwi         r11, r3, 2
0x825A6A34:  fmr          f11, f13
0x825A6A38:  add          r10, r10, r4
0x825A6A3C:  add          r11, r11, r4
0x825A6A40:  add          r9, r9, r4
0x825A6A44:  lfs          f7, 0(r10)
0x825A6A48:  lfs          f4, 4(r10)
0x825A6A4C:  fadds        f3, f10, f7
0x825A6A50:  lfs          f5, 0(r9)
0x825A6A54:  fsubs        f10, f10, f7
0x825A6A58:  lfs          f8, 0(r11)
0x825A6A5C:  fadds        f1, f9, f4
0x825A6A60:  lfs          f6, 4(r11)
0x825A6A64:  fadds        f2, f8, f5
0x825A6A68:  lfs          f7, 4(r9)
0x825A6A6C:  fsubs        f8, f8, f5
0x825A6A70:  fsubs        f9, f9, f4
0x825A6A74:  fadds        f5, f6, f7
0x825A6A78:  fsubs        f7, f6, f7
0x825A6A7C:  fadds        f6, f2, f3
0x825A6A80:  stfs         f6, 0(r4)
0x825A6A84:  fsubs        f4, f3, f2
0x825A6A88:  fadds        f3, f8, f9
0x825A6A8C:  fsubs        f2, f9, f8
0x825A6A90:  fadds        f9, f5, f1
0x825A6A94:  stfs         f9, 4(r4)
0x825A6A98:  fsubs        f8, f1, f5
0x825A6A9C:  stfs         f4, 0(r11)
0x825A6AA0:  stfs         f8, 4(r11)
0x825A6AA4:  fsubs        f6, f10, f7
0x825A6AA8:  stfs         f6, 0(r10)
0x825A6AAC:  fadds        f5, f7, f10
0x825A6AB0:  stfs         f3, 4(r10)
0x825A6AB4:  stfs         f5, 0(r9)
0x825A6AB8:  stfs         f2, 4(r9)
0x825A6ABC:  lfs          f15, 4(r5)
0x825A6AC0:  lfs          f17, 0(r30)
0x825A6AC4:  lfs          f16, 0xc(r5)
0x825A6AC8:  ble          cr6, 0x825a6e60
0x825A6ACC:  addi         r10, r3, 4
0x825A6AD0:  addi         r6, r3, -2
0x825A6AD4:  addi         r7, r3, 2
0x825A6AD8:  slwi         r11, r3, 4
0x825A6ADC:  addi         r31, r21, -3
0x825A6AE0:  slwi         r9, r10, 2
0x825A6AE4:  slwi         r6, r6, 3
0x825A6AE8:  add          r11, r11, r4
0x825A6AEC:  slwi         r10, r7, 3
0x825A6AF0:  srwi         r7, r31, 2
0x825A6AF4:  add          r31, r6, r4
0x825A6AF8:  addi         r6, r11, -0x10
0x825A6AFC:  slwi         r11, r3, 1
0x825A6B00:  addi         r8, r3, -4
0x825A6B04:  add          r11, r3, r11
0x825A6B08:  addi         r29, r7, 1
0x825A6B0C:  slwi         r11, r11, 2
0x825A6B10:  slwi         r5, r8, 2
0x825A6B14:  add          r7, r11, r4
0x825A6B18:  addi         r8, r4, 0x10
0x825A6B1C:  addi         r11, r7, 0x10
0x825A6B20:  add          r9, r9, r4
0x825A6B24:  add          r5, r5, r4
0x825A6B28:  add          r10, r10, r4
0x825A6B2C:  addi         r7, r7, -0x10
0x825A6B30:  addi         r30, r30, 0x10
0x825A6B34:  lfs          f9, 0(r11)
0x825A6B38:  lfs          f7, 0(r9)
0x825A6B3C:  addi         r27, r5, 0xc
0x825A6B40:  lfs          f10, 0(r10)
0x825A6B44:  fadds        f1, f7, f9
0x825A6B48:  lfs          f8, 0(r8)
0x825A6B4C:  fsubs        f29, f7, f9
0x825A6B50:  fadds        f3, f10, f8
0x825A6B54:  lfs          f4, -8(r10)
0x825A6B58:  lfs          f9, -8(r30)
0x825A6B5C:  fsubs        f30, f8, f10
0x825A6B60:  fadds        f0, f9, f0
0x825A6B64:  lfs          f6, -8(r8)
0x825A6B68:  lfs          f10, -4(r11)
0x825A6B6C:  fadds        f21, f6, f4
0x825A6B70:  lfs          f31, -4(r9)
0x825A6B74:  fsubs        f6, f6, f4
0x825A6B78:  fsubs        f4, f31, f10
0x825A6B7C:  lfs          f2, -8(r11)
0x825A6B80:  lfs          f5, -8(r9)
0x825A6B84:  fadds        f31, f31, f10
0x825A6B88:  lfs          f26, 4(r10)
0x825A6B8C:  fadds        f20, f5, f2
0x825A6B90:  lfs          f25, 4(r11)
0x825A6B94:  fsubs        f5, f5, f2
0x825A6B98:  lfs          f24, -4(r30)
0x825A6B9C:  addi         r26, r5, 4
0x825A6BA0:  lfs          f27, -4(r10)
0x825A6BA4:  fadds        f10, f1, f3
0x825A6BA8:  lfs          f23, 0(r30)
0x825A6BAC:  addi         r25, r7, 4
0x825A6BB0:  lfs          f22, 4(r30)
0x825A6BB4:  fadds        f14, f23, f12
0x825A6BB8:  lfs          f28, 4(r9)
0x825A6BBC:  fmr          f12, f23
0x825A6BC0:  lfs          f8, -4(r8)
0x825A6BC4:  fadds        f18, f28, f25
0x825A6BC8:  stfs         f10, 0(r8)
0x825A6BCC:  fmuls        f10, f0, f17
0x825A6BD0:  fadds        f2, f8, f27
0x825A6BD4:  lfs          f7, 4(r8)
0x825A6BD8:  fsubs        f0, f11, f22
0x825A6BDC:  stfs         f0, -0x100(r1)
0x825A6BE0:  fsubs        f27, f8, f27
0x825A6BE4:  addi         r24, r31, 0xc
0x825A6BE8:  fadds        f8, f24, f13
0x825A6BEC:  fsubs        f13, f3, f1
0x825A6BF0:  stfs         f13, -0xfc(r1)
0x825A6BF4:  fadds        f19, f7, f26
0x825A6BF8:  fsubs        f1, f28, f25
0x825A6BFC:  fadds        f28, f20, f21
0x825A6C00:  stfs         f28, -8(r8)
0x825A6C04:  fmr          f0, f9
0x825A6C08:  fsubs        f3, f7, f26
0x825A6C0C:  fadds        f25, f31, f2
0x825A6C10:  stfs         f25, -4(r8)
0x825A6C14:  fsubs        f2, f2, f31
0x825A6C18:  fsubs        f26, f21, f20
0x825A6C1C:  fmuls        f9, f8, f17
0x825A6C20:  fsubs        f28, f6, f4
0x825A6C24:  fadds        f31, f18, f19
0x825A6C28:  stfs         f31, 4(r8)
0x825A6C2C:  stfs         f2, -4(r9)
0x825A6C30:  fadds        f2, f5, f27
0x825A6C34:  stfs         f26, -8(r9)
0x825A6C38:  fmuls        f8, f14, f16
0x825A6C3C:  fsubs        f31, f19, f18
0x825A6C40:  stfs         f31, 4(r9)
0x825A6C44:  fmr          f13, f24
0x825A6C48:  fneg         f11, f22
0x825A6C4C:  fadds        f31, f29, f3
0x825A6C50:  fmuls        f24, f9, f28
0x825A6C54:  fmuls        f26, f9, f2
0x825A6C58:  fmr          f25, f2
0x825A6C5C:  fsubs        f2, f30, f1
0x825A6C60:  fmsubs       f28, f10, f28, f26
0x825A6C64:  lfs          f14, -0x100(r1)
0x825A6C68:  fmuls        f7, f14, f16
0x825A6C6C:  lfs          f14, -0xfc(r1)
0x825A6C70:  stfs         f14, 0(r9)
0x825A6C74:  stfs         f28, -8(r10)
0x825A6C78:  fmadds       f28, f10, f25, f24
0x825A6C7C:  stfs         f28, -4(r10)
0x825A6C80:  fmuls        f28, f13, f31
0x825A6C84:  fmuls        f26, f13, f2
0x825A6C88:  addi         r23, r6, 4
0x825A6C8C:  fadds        f6, f4, f6
0x825A6C90:  addi         r22, r31, 4
0x825A6C94:  fsubs        f5, f27, f5
0x825A6C98:  addi         r29, r29, -1
0x825A6C9C:  addi         r9, r9, 0x10
0x825A6CA0:  addi         r8, r8, 0x10
0x825A6CA4:  cmplwi       cr6, r29, 0
0x825A6CA8:  fmsubs       f4, f0, f2, f28
0x825A6CAC:  stfs         f4, 0(r10)
0x825A6CB0:  fmadds       f2, f0, f31, f26
0x825A6CB4:  stfs         f2, 4(r10)
0x825A6CB8:  fmuls        f4, f8, f6
0x825A6CBC:  addi         r10, r10, 0x10
0x825A6CC0:  fmr          f2, f5
0x825A6CC4:  fmuls        f28, f7, f6
0x825A6CC8:  fadds        f6, f1, f30
0x825A6CCC:  fmr          f31, f5
0x825A6CD0:  fsubs        f5, f3, f29
0x825A6CD4:  fmadds       f1, f7, f2, f4
0x825A6CD8:  stfs         f1, -8(r11)
0x825A6CDC:  fmuls        f3, f12, f6
0x825A6CE0:  fmuls        f2, f11, f6
0x825A6CE4:  fmsubs       f4, f8, f31, f28
0x825A6CE8:  stfs         f4, -4(r11)
0x825A6CEC:  fmadds       f1, f11, f5, f3
0x825A6CF0:  stfs         f1, 0(r11)
0x825A6CF4:  fmsubs       f6, f12, f5, f2
0x825A6CF8:  stfs         f6, 4(r11)
0x825A6CFC:  lfs          f5, 8(r7)
0x825A6D00:  addi         r11, r11, 0x10
0x825A6D04:  lfs          f6, 8(r5)
0x825A6D08:  lfs          f3, 0xc(r7)
0x825A6D0C:  fadds        f23, f6, f5
0x825A6D10:  lfs          f4, 0(r27)
0x825A6D14:  fsubs        f6, f6, f5
0x825A6D18:  lfs          f1, 0(r7)
0x825A6D1C:  fadds        f5, f3, f4
0x825A6D20:  lfs          f2, 0(r5)
0x825A6D24:  fsubs        f4, f4, f3
0x825A6D28:  lfs          f30, 0(r25)
0x825A6D2C:  fadds        f3, f1, f2
0x825A6D30:  lfs          f31, 0(r26)
0x825A6D34:  fsubs        f2, f2, f1
0x825A6D38:  lfs          f28, 8(r31)
0x825A6D3C:  fadds        f1, f30, f31
0x825A6D40:  lfs          f29, 8(r6)
0x825A6D44:  fsubs        f31, f31, f30
0x825A6D48:  fadds        f30, f29, f28
0x825A6D4C:  lfs          f26, 0(r24)
0x825A6D50:  fsubs        f29, f28, f29
0x825A6D54:  lfs          f27, 0xc(r6)
0x825A6D58:  lfs          f25, 0(r6)
0x825A6D5C:  fadds        f28, f26, f27
0x825A6D60:  lfs          f24, 0(r31)
0x825A6D64:  fsubs        f27, f26, f27
0x825A6D68:  fadds        f26, f25, f24
0x825A6D6C:  fadds        f22, f30, f23
0x825A6D70:  fsubs        f23, f23, f30
0x825A6D74:  fadds        f30, f29, f4
0x825A6D78:  fadds        f21, f28, f5
0x825A6D7C:  fsubs        f20, f5, f28
0x825A6D80:  fsubs        f28, f24, f25
0x825A6D84:  fadds        f25, f26, f3
0x825A6D88:  fsubs        f24, f3, f26
0x825A6D8C:  lfs          f3, 0(r23)
0x825A6D90:  lfs          f26, 0(r22)
0x825A6D94:  fsubs        f5, f6, f27
0x825A6D98:  stfs         f25, 0(r5)
0x825A6D9C:  stfs         f22, 8(r5)
0x825A6DA0:  addi         r5, r5, -0x10
0x825A6DA4:  stfs         f21, 0(r27)
0x825A6DA8:  fmuls        f25, f10, f30
0x825A6DAC:  fmuls        f22, f10, f5
0x825A6DB0:  fadds        f10, f28, f31
0x825A6DB4:  fmsubs       f25, f9, f5, f25
0x825A6DB8:  fmadds       f30, f9, f30, f22
0x825A6DBC:  fsubs        f9, f26, f3
0x825A6DC0:  fadds        f5, f3, f26
0x825A6DC4:  fmuls        f3, f0, f10
0x825A6DC8:  fmr          f26, f10
0x825A6DCC:  fsubs        f10, f4, f29
0x825A6DD0:  fadds        f6, f27, f6
0x825A6DD4:  fsubs        f4, f2, f9
0x825A6DD8:  fadds        f29, f5, f1
0x825A6DDC:  stfs         f29, 0(r26)
0x825A6DE0:  fsubs        f1, f1, f5
0x825A6DE4:  stfs         f1, 0(r22)
0x825A6DE8:  stfs         f24, 0(r31)
0x825A6DEC:  fadds        f9, f9, f2
0x825A6DF0:  fmr          f5, f10
0x825A6DF4:  stfs         f23, 8(r31)
0x825A6DF8:  fmr          f1, f10
0x825A6DFC:  stfs         f20, 0(r24)
0x825A6E00:  fsubs        f10, f31, f28
0x825A6E04:  stfs         f25, 8(r7)
0x825A6E08:  stfs         f30, 0xc(r7)
0x825A6E0C:  addi         r31, r31, -0x10
0x825A6E10:  fmuls        f31, f0, f4
0x825A6E14:  fmsubs       f4, f13, f4, f3
0x825A6E18:  stfs         f4, 0(r7)
0x825A6E1C:  fmuls        f4, f7, f6
0x825A6E20:  addi         r7, r7, -0x10
0x825A6E24:  fmadds       f3, f13, f26, f31
0x825A6E28:  stfs         f3, 0(r25)
0x825A6E2C:  fmuls        f3, f8, f6
0x825A6E30:  fmadds       f2, f8, f5, f4
0x825A6E34:  stfs         f2, 8(r6)
0x825A6E38:  fmuls        f8, f11, f9
0x825A6E3C:  fmsubs       f1, f7, f1, f3
0x825A6E40:  stfs         f1, 0xc(r6)
0x825A6E44:  fmuls        f7, f12, f9
0x825A6E48:  fmadds       f6, f12, f10, f8
0x825A6E4C:  stfs         f6, 0(r6)
0x825A6E50:  addi         r6, r6, -0x10
0x825A6E54:  fmsubs       f5, f11, f10, f7
0x825A6E58:  stfs         f5, 0(r23)
0x825A6E5C:  bne          cr6, 0x825a6b30
0x825A6E60:  add          r7, r3, r28
0x825A6E64:  fadds        f3, f0, f15
0x825A6E68:  fadds        f4, f13, f15
0x825A6E6C:  slwi         r11, r28, 2
0x825A6E70:  add          r6, r7, r3
0x825A6E74:  fsubs        f2, f12, f15
0x825A6E78:  addi         r9, r7, -2
0x825A6E7C:  fsubs        f1, f11, f15
0x825A6E80:  add          r5, r6, r3
0x825A6E84:  addi         r8, r6, -2
0x825A6E88:  slwi         r30, r9, 2
0x825A6E8C:  slwi         r9, r6, 2
0x825A6E90:  slwi         r31, r8, 2
0x825A6E94:  slwi         r29, r21, 2
0x825A6E98:  addi         r3, r5, -2
0x825A6E9C:  fmuls        f13, f3, f17
0x825A6EA0:  slwi         r10, r7, 2
0x825A6EA4:  fmuls        f0, f4, f17
0x825A6EA8:  slwi         r8, r5, 2
0x825A6EAC:  lfsx         f8, r30, r4
0x825A6EB0:  add          r11, r11, r4
0x825A6EB4:  lfsx         f6, r31, r4
0x825A6EB8:  add          r9, r9, r4
0x825A6EBC:  lfsx         f10, r29, r4
0x825A6EC0:  slwi         r3, r3, 2
0x825A6EC4:  fadds        f3, f10, f6
0x825A6EC8:  add          r10, r10, r4
0x825A6ECC:  fsubs        f10, f10, f6
0x825A6ED0:  add          r8, r8, r4
0x825A6ED4:  fmuls        f12, f2, f16
0x825A6ED8:  lfs          f9, -4(r11)
0x825A6EDC:  fmuls        f11, f1, f16
0x825A6EE0:  lfs          f5, -4(r9)
0x825A6EE4:  addi         r26, r5, 2
0x825A6EE8:  lfsx         f4, r3, r4
0x825A6EEC:  fadds        f2, f9, f5
0x825A6EF0:  lfs          f7, -4(r10)
0x825A6EF4:  fadds        f1, f8, f4
0x825A6EF8:  lfs          f6, -4(r8)
0x825A6EFC:  fsubs        f9, f9, f5
0x825A6F00:  fadds        f5, f7, f6
0x825A6F04:  addi         r25, r7, 2
0x825A6F08:  fsubs        f8, f8, f4
0x825A6F0C:  fsubs        f7, f7, f6
0x825A6F10:  fadds        f6, f1, f3
0x825A6F14:  stfsx        f6, r29, r4
0x825A6F18:  fsubs        f4, f3, f1
0x825A6F1C:  fadds        f3, f5, f2
0x825A6F20:  stfs         f3, -4(r11)
0x825A6F24:  fsubs        f2, f2, f5
0x825A6F28:  stfsx        f4, r30, r4
0x825A6F2C:  fadds        f5, f8, f9
0x825A6F30:  stfs         f2, -4(r10)
0x825A6F34:  fsubs        f6, f10, f7
0x825A6F38:  addi         r30, r28, 3
0x825A6F3C:  fadds        f10, f7, f10
0x825A6F40:  fsubs        f9, f9, f8
0x825A6F44:  slwi         r29, r30, 2
0x825A6F48:  fmuls        f1, f0, f5
0x825A6F4C:  fmuls        f4, f0, f6
0x825A6F50:  fmsubs       f3, f13, f6, f1
0x825A6F54:  stfsx        f3, r31, r4
0x825A6F58:  fmuls        f1, f12, f10
0x825A6F5C:  addi         r31, r6, 2
0x825A6F60:  fmuls        f10, f11, f10
0x825A6F64:  addi         r6, r6, 3
0x825A6F68:  fmadds       f2, f13, f5, f4
0x825A6F6C:  stfs         f2, -4(r9)
0x825A6F70:  fneg         f6, f15
0x825A6F74:  slwi         r30, r6, 2
0x825A6F78:  fmadds       f8, f11, f9, f1
0x825A6F7C:  stfsx        f8, r3, r4
0x825A6F80:  addi         r3, r28, 2
0x825A6F84:  fmsubs       f7, f12, f9, f10
0x825A6F88:  stfs         f7, -4(r8)
0x825A6F8C:  slwi         r28, r31, 2
0x825A6F90:  lfs          f10, 0(r11)
0x825A6F94:  slwi         r27, r3, 2
0x825A6F98:  lfs          f9, 0(r9)
0x825A6F9C:  lfs          f8, 0(r8)
0x825A6FA0:  lfs          f7, 0(r10)
0x825A6FA4:  fadds        f2, f10, f9
0x825A6FA8:  lfs          f4, 4(r9)
0x825A6FAC:  fsubs        f10, f10, f9
0x825A6FB0:  lfs          f5, 4(r11)
0x825A6FB4:  fadds        f9, f7, f8
0x825A6FB8:  lfs          f3, 4(r8)
0x825A6FBC:  fsubs        f8, f7, f8
0x825A6FC0:  lfs          f1, 4(r10)
0x825A6FC4:  fadds        f7, f5, f4
0x825A6FC8:  slwi         r31, r26, 2
0x825A6FCC:  fsubs        f5, f5, f4
0x825A6FD0:  slwi         r3, r25, 2
0x825A6FD4:  fadds        f4, f1, f3
0x825A6FD8:  addi         r5, r5, 3
0x825A6FDC:  fsubs        f3, f1, f3
0x825A6FE0:  addi         r7, r7, 3
0x825A6FE4:  slwi         r6, r5, 2
0x825A6FE8:  slwi         r7, r7, 2
0x825A6FEC:  fadds        f1, f9, f2
0x825A6FF0:  stfs         f1, 0(r11)
0x825A6FF4:  fsubs        f9, f2, f9
0x825A6FF8:  fadds        f2, f4, f7
0x825A6FFC:  stfs         f2, 4(r11)
0x825A7000:  stfs         f9, 0(r10)
0x825A7004:  fsubs        f1, f7, f4
0x825A7008:  fadds        f7, f8, f5
0x825A700C:  stfs         f1, 4(r10)
0x825A7010:  fsubs        f9, f10, f3
0x825A7014:  fadds        f10, f3, f10
0x825A7018:  fsubs        f4, f9, f7
0x825A701C:  fadds        f2, f7, f9
0x825A7020:  fsubs        f9, f5, f8
0x825A7024:  fmuls        f1, f4, f15
0x825A7028:  stfs         f1, 0(r9)
0x825A702C:  fmuls        f8, f2, f15
0x825A7030:  stfs         f8, 4(r9)
0x825A7034:  fadds        f7, f9, f10
0x825A7038:  lfsx         f8, r31, r4
0x825A703C:  fsubs        f5, f9, f10
0x825A7040:  fmuls        f4, f7, f6
0x825A7044:  stfs         f4, 0(r8)
0x825A7048:  fmuls        f3, f5, f6
0x825A704C:  stfs         f3, 4(r8)
0x825A7050:  lfsx         f9, r28, r4
0x825A7054:  lfsx         f10, r27, r4
0x825A7058:  lfsx         f6, r30, r4
0x825A705C:  fadds        f4, f10, f9
0x825A7060:  lfsx         f7, r29, r4
0x825A7064:  fsubs        f10, f10, f9
0x825A7068:  lfsx         f5, r3, r4
0x825A706C:  fadds        f9, f7, f6
0x825A7070:  fsubs        f7, f7, f6
0x825A7074:  lfsx         f3, r6, r4
0x825A7078:  fadds        f6, f5, f8
0x825A707C:  fsubs        f8, f5, f8
0x825A7080:  lfsx         f5, r7, r4
0x825A7084:  fadds        f2, f6, f4
0x825A7088:  stfsx        f2, r27, r4
0x825A708C:  fsubs        f1, f4, f6
0x825A7090:  fadds        f6, f5, f3
0x825A7094:  fsubs        f5, f5, f3
0x825A7098:  fadds        f4, f6, f9
0x825A709C:  stfsx        f4, r29, r4
0x825A70A0:  fsubs        f3, f9, f6
0x825A70A4:  stfsx        f1, r3, r4
0x825A70A8:  fsubs        f9, f10, f5
0x825A70AC:  stfsx        f3, r7, r4
0x825A70B0:  fadds        f6, f8, f7
0x825A70B4:  fmuls        f1, f13, f9
0x825A70B8:  fmuls        f2, f13, f6
0x825A70BC:  fadds        f13, f5, f10
0x825A70C0:  fsubs        f10, f7, f8
0x825A70C4:  fmadds       f8, f0, f6, f1
0x825A70C8:  stfsx        f8, r30, r4
0x825A70CC:  fmsubs       f9, f0, f9, f2
0x825A70D0:  stfsx        f9, r28, r4
0x825A70D4:  fmuls        f7, f11, f13
0x825A70D8:  fmuls        f6, f12, f13
0x825A70DC:  fmadds       f5, f12, f10, f7
0x825A70E0:  stfsx        f5, r31, r4
0x825A70E4:  fmsubs       f4, f11, f10, f6
0x825A70E8:  stfsx        f4, r6, r4
0x825A70EC:  addi         r12, r1, -0x60
0x825A70F0:  bl           0x8243662c
0x825A70F4:  b            0x8242f8cc
0x825A70F8:  mflr         r12
0x825A70FC:  bl           0x8242f88c
0x825A7100:  addi         r12, r1, -0x40
0x825A7104:  bl           0x824365e0
0x825A7108:  lis          r11, -0x7e00
0x825A710C:  lfs          f9, 4(r4)
0x825A7110:  srawi        r29, r3, 3
0x825A7114:  fneg         f1, f9
0x825A7118:  lfs          f10, 0(r4)
0x825A711C:  addi         r30, r5, 8
0x825A7120:  addi         r26, r29, -2
0x825A7124:  lfs          f0, 0x3dac(r11)
0x825A7128:  lis          r11, -0x7e00
0x825A712C:  fmr          f12, f0
0x825A7130:  cmpwi        cr6, r26, 2
0x825A7134:  lfs          f13, 0x3da8(r11)
0x825A7138:  slwi         r11, r29, 1
0x825A713C:  fmr          f11, f13
0x825A7140:  slwi         r9, r11, 1
0x825A7144:  slwi         r10, r11, 2
0x825A7148:  add          r8, r9, r11
0x825A714C:  slwi         r9, r9, 2
0x825A7150:  slwi         r8, r8, 2
0x825A7154:  add          r9, r9, r4
0x825A7158:  add          r10, r10, r4
0x825A715C:  add          r8, r8, r4
0x825A7160:  lfs          f7, 0(r9)
0x825A7164:  lfs          f8, 0(r10)
0x825A7168:  fadds        f3, f10, f7
0x825A716C:  lfs          f5, 0(r8)
0x825A7170:  fsubs        f10, f10, f7
0x825A7174:  lfs          f4, 4(r9)
0x825A7178:  fadds        f2, f8, f5
0x825A717C:  lfs          f6, 4(r10)
0x825A7180:  fsubs        f8, f8, f5
0x825A7184:  lfs          f7, 4(r8)
0x825A7188:  fsubs        f9, f4, f9
0x825A718C:  fadds        f5, f6, f7
0x825A7190:  fsubs        f1, f1, f4
0x825A7194:  fsubs        f7, f6, f7
0x825A7198:  fadds        f6, f2, f3
0x825A719C:  stfs         f6, 0(r4)
0x825A71A0:  fsubs        f4, f3, f2
0x825A71A4:  fadds        f3, f8, f9
0x825A71A8:  fsubs        f2, f9, f8
0x825A71AC:  fsubs        f9, f1, f5
0x825A71B0:  stfs         f9, 4(r4)
0x825A71B4:  fadds        f8, f5, f1
0x825A71B8:  stfs         f4, 0(r10)
0x825A71BC:  stfs         f8, 4(r10)
0x825A71C0:  fadds        f6, f7, f10
0x825A71C4:  stfs         f6, 0(r9)
0x825A71C8:  fsubs        f5, f10, f7
0x825A71CC:  stfs         f3, 4(r9)
0x825A71D0:  stfs         f5, 0(r8)
0x825A71D4:  stfs         f2, 4(r8)
0x825A71D8:  lfs          f15, 4(r5)
0x825A71DC:  lfs          f17, 0(r30)
0x825A71E0:  lfs          f16, 0xc(r5)
0x825A71E4:  ble          cr6, 0x825a757c
0x825A71E8:  addi         r9, r11, 4
0x825A71EC:  slwi         r10, r11, 4
0x825A71F0:  addi         r6, r11, 2
0x825A71F4:  addi         r31, r26, -3
0x825A71F8:  add          r10, r10, r4
0x825A71FC:  slwi         r8, r9, 2
0x825A7200:  slwi         r9, r6, 3
0x825A7204:  srwi         r6, r31, 2
0x825A7208:  addi         r31, r10, -0x10
0x825A720C:  slwi         r10, r11, 1
0x825A7210:  addi         r7, r11, -4
0x825A7214:  add          r10, r11, r10
0x825A7218:  addi         r5, r11, -2
0x825A721C:  slwi         r10, r10, 2
0x825A7220:  addi         r28, r6, 1
0x825A7224:  add          r6, r10, r4
0x825A7228:  slwi         r3, r7, 2
0x825A722C:  slwi         r5, r5, 3
0x825A7230:  addi         r10, r6, 0x10
0x825A7234:  addi         r7, r4, 0x10
0x825A7238:  add          r8, r8, r4
0x825A723C:  add          r3, r3, r4
0x825A7240:  add          r9, r9, r4
0x825A7244:  add          r5, r5, r4
0x825A7248:  addi         r6, r6, -0x10
0x825A724C:  lfs          f9, 0(r10)
0x825A7250:  addi         r30, r30, 0x10
0x825A7254:  lfs          f10, 0(r9)
0x825A7258:  lfs          f8, 0(r7)
0x825A725C:  lfs          f7, 0(r8)
0x825A7260:  fadds        f3, f8, f10
0x825A7264:  fadds        f1, f7, f9
0x825A7268:  lfs          f4, -8(r7)
0x825A726C:  fsubs        f29, f7, f9
0x825A7270:  lfs          f9, -8(r9)
0x825A7274:  lfs          f25, -8(r30)
0x825A7278:  fsubs        f30, f8, f10
0x825A727C:  lfs          f22, 4(r30)
0x825A7280:  lfs          f6, -4(r7)
0x825A7284:  fneg         f21, f6
0x825A7288:  lfs          f5, 4(r7)
0x825A728C:  lfs          f28, -8(r10)
0x825A7290:  fneg         f20, f5
0x825A7294:  lfs          f27, -4(r10)
0x825A7298:  lfs          f26, 4(r10)
0x825A729C:  lfs          f24, -4(r30)
0x825A72A0:  fadds        f19, f1, f3
0x825A72A4:  lfs          f8, -4(r9)
0x825A72A8:  fsubs        f14, f3, f1
0x825A72AC:  lfs          f23, 0(r30)
0x825A72B0:  fadds        f3, f9, f4
0x825A72B4:  lfs          f7, 4(r9)
0x825A72B8:  fsubs        f4, f4, f9
0x825A72BC:  lfs          f10, 4(r8)
0x825A72C0:  fadds        f9, f0, f25
0x825A72C4:  lfs          f2, -8(r8)
0x825A72C8:  fsubs        f0, f11, f22
0x825A72CC:  stfs         f0, -0xe0(r1)
0x825A72D0:  lfs          f31, -4(r8)
0x825A72D4:  fsubs        f21, f21, f8
0x825A72D8:  stfs         f19, 0(r7)
0x825A72DC:  fadds        f19, f27, f31
0x825A72E0:  fsubs        f6, f8, f6
0x825A72E4:  fsubs        f31, f31, f27
0x825A72E8:  fadds        f8, f13, f24
0x825A72EC:  fadds        f27, f23, f12
0x825A72F0:  fadds        f1, f28, f2
0x825A72F4:  fsubs        f2, f2, f28
0x825A72F8:  fadds        f18, f26, f10
0x825A72FC:  fsubs        f20, f20, f7
0x825A7300:  fsubs        f28, f10, f26
0x825A7304:  fmuls        f10, f9, f17
0x825A7308:  fsubs        f5, f7, f5
0x825A730C:  fmr          f0, f25
0x825A7310:  fmuls        f9, f8, f17
0x825A7314:  fmuls        f8, f27, f16
0x825A7318:  fmr          f13, f24
0x825A731C:  fmr          f12, f23
0x825A7320:  fneg         f11, f22
0x825A7324:  lfs          f27, -0xe0(r1)
0x825A7328:  fmuls        f7, f27, f16
0x825A732C:  fadds        f27, f1, f3
0x825A7330:  stfs         f27, -8(r7)
0x825A7334:  fsubs        f3, f3, f1
0x825A7338:  fsubs        f1, f21, f19
0x825A733C:  stfs         f1, -4(r7)
0x825A7340:  fsubs        f1, f20, f18
0x825A7344:  stfs         f1, 4(r7)
0x825A7348:  fadds        f1, f19, f21
0x825A734C:  stfs         f3, -8(r8)
0x825A7350:  stfs         f1, -4(r8)
0x825A7354:  fadds        f3, f31, f4
0x825A7358:  fadds        f1, f2, f6
0x825A735C:  stfs         f14, 0(r8)
0x825A7360:  fadds        f27, f18, f20
0x825A7364:  stfs         f27, 4(r8)
0x825A7368:  fmr          f27, f3
0x825A736C:  fmuls        f26, f9, f1
0x825A7370:  fmr          f25, f1
0x825A7374:  fmuls        f24, f9, f3
0x825A7378:  fadds        f3, f28, f30
0x825A737C:  fadds        f1, f29, f5
0x825A7380:  fmsubs       f27, f10, f27, f26
0x825A7384:  stfs         f27, -8(r9)
0x825A7388:  fmadds       f27, f10, f25, f24
0x825A738C:  stfs         f27, -4(r9)
0x825A7390:  fmuls        f26, f13, f3
0x825A7394:  addi         r27, r31, 4
0x825A7398:  fmuls        f27, f13, f1
0x825A739C:  addi         r8, r8, 0x10
0x825A73A0:  fsubs        f6, f6, f2
0x825A73A4:  addi         r7, r7, 0x10
0x825A73A8:  fsubs        f4, f4, f31
0x825A73AC:  fsubs        f5, f5, f29
0x825A73B0:  fmadds       f2, f0, f1, f26
0x825A73B4:  stfs         f2, 4(r9)
0x825A73B8:  fmsubs       f3, f0, f3, f27
0x825A73BC:  stfs         f3, 0(r9)
0x825A73C0:  fmr          f1, f6
0x825A73C4:  addi         r9, r9, 0x10
0x825A73C8:  fmuls        f3, f8, f4
0x825A73CC:  fmr          f2, f6
0x825A73D0:  fsubs        f6, f30, f28
0x825A73D4:  fmuls        f4, f7, f4
0x825A73D8:  fmadds       f3, f7, f1, f3
0x825A73DC:  stfs         f3, -8(r10)
0x825A73E0:  fmuls        f1, f12, f6
0x825A73E4:  fmuls        f6, f11, f6
0x825A73E8:  fmsubs       f2, f8, f2, f4
0x825A73EC:  stfs         f2, -4(r10)
0x825A73F0:  fmadds       f4, f11, f5, f1
0x825A73F4:  stfs         f4, 0(r10)
0x825A73F8:  fmsubs       f3, f12, f5, f6
0x825A73FC:  stfs         f3, 4(r10)
0x825A7400:  lfs          f6, 0xc(r3)
0x825A7404:  addi         r10, r10, 0x10
0x825A7408:  fneg         f24, f6
0x825A740C:  lfs          f3, 8(r6)
0x825A7410:  lfs          f4, 8(r3)
0x825A7414:  lfs          f31, 0(r6)
0x825A7418:  fadds        f25, f3, f4
0x825A741C:  lfs          f2, 0xc(r6)
0x825A7420:  fsubs        f4, f4, f3
0x825A7424:  lfs          f1, 0(r3)
0x825A7428:  fsubs        f6, f2, f6
0x825A742C:  lfs          f28, 8(r5)
0x825A7430:  fadds        f3, f31, f1
0x825A7434:  lfs          f27, 0xc(r31)
0x825A7438:  fsubs        f1, f1, f31
0x825A743C:  lfs          f26, 0xc(r5)
0x825A7440:  lfs          f29, 8(r31)
0x825A7444:  fadds        f31, f28, f29
0x825A7448:  lfs          f5, 4(r3)
0x825A744C:  fsubs        f2, f24, f2
0x825A7450:  lfs          f30, 4(r6)
0x825A7454:  fadds        f24, f26, f27
0x825A7458:  fsubs        f29, f28, f29
0x825A745C:  fsubs        f28, f26, f27
0x825A7460:  lfs          f26, 0(r5)
0x825A7464:  fneg         f23, f5
0x825A7468:  lfs          f27, 0(r31)
0x825A746C:  fsubs        f5, f30, f5
0x825A7470:  fadds        f22, f31, f25
0x825A7474:  fsubs        f20, f25, f31
0x825A7478:  lfs          f31, 0(r27)
0x825A747C:  fsubs        f21, f2, f24
0x825A7480:  fadds        f19, f24, f2
0x825A7484:  lfs          f24, 4(r5)
0x825A7488:  fadds        f2, f29, f6
0x825A748C:  stfs         f22, 8(r3)
0x825A7490:  fadds        f25, f28, f4
0x825A7494:  stfs         f21, 0xc(r3)
0x825A7498:  fsubs        f30, f23, f30
0x825A749C:  fsubs        f23, f26, f27
0x825A74A0:  fadds        f27, f27, f26
0x825A74A4:  fmuls        f26, f10, f2
0x825A74A8:  fmr          f22, f2
0x825A74AC:  fmuls        f21, f10, f25
0x825A74B0:  fsubs        f2, f24, f31
0x825A74B4:  fadds        f31, f24, f31
0x825A74B8:  fadds        f24, f27, f3
0x825A74BC:  stfs         f24, 0(r3)
0x825A74C0:  fsubs        f3, f3, f27
0x825A74C4:  fadds        f10, f23, f5
0x825A74C8:  fmsubs       f27, f9, f25, f26
0x825A74CC:  fmadds       f26, f9, f22, f21
0x825A74D0:  fadds        f9, f2, f1
0x825A74D4:  fmuls        f25, f0, f10
0x825A74D8:  addi         r28, r28, -1
0x825A74DC:  fmr          f24, f10
0x825A74E0:  fsubs        f10, f6, f29
0x825A74E4:  cmplwi       cr6, r28, 0
0x825A74E8:  fsubs        f6, f30, f31
0x825A74EC:  stfs         f6, 4(r3)
0x825A74F0:  stfs         f3, 0(r5)
0x825A74F4:  fadds        f6, f31, f30
0x825A74F8:  stfs         f6, 4(r5)
0x825A74FC:  fmr          f3, f9
0x825A7500:  fmuls        f6, f0, f9
0x825A7504:  stfs         f20, 8(r5)
0x825A7508:  fsubs        f9, f4, f28
0x825A750C:  stfs         f19, 0xc(r5)
0x825A7510:  stfs         f27, 8(r6)
0x825A7514:  addi         r5, r5, -0x10
0x825A7518:  stfs         f26, 0xc(r6)
0x825A751C:  addi         r3, r3, -0x10
0x825A7520:  fmr          f4, f10
0x825A7524:  fmr          f31, f10
0x825A7528:  fsubs        f10, f5, f23
0x825A752C:  fmsubs       f3, f13, f3, f25
0x825A7530:  stfs         f3, 0(r6)
0x825A7534:  fmadds       f6, f13, f24, f6
0x825A7538:  stfs         f6, 4(r6)
0x825A753C:  fmuls        f5, f7, f9
0x825A7540:  addi         r6, r6, -0x10
0x825A7544:  fmuls        f3, f8, f9
0x825A7548:  fsubs        f9, f1, f2
0x825A754C:  fmadds       f2, f8, f4, f5
0x825A7550:  stfs         f2, 8(r31)
0x825A7554:  fmsubs       f1, f7, f31, f3
0x825A7558:  stfs         f1, 0xc(r31)
0x825A755C:  fmuls        f8, f11, f9
0x825A7560:  fmuls        f7, f12, f9
0x825A7564:  fmadds       f6, f12, f10, f8
0x825A7568:  stfs         f6, 0(r31)
0x825A756C:  fmsubs       f5, f11, f10, f7
0x825A7570:  addi         r31, r31, -0x10
0x825A7574:  stfs         f5, 0(r27)
0x825A7578:  bne          cr6, 0x825a724c
0x825A757C:  fsubs        f2, f12, f15
0x825A7580:  slwi         r10, r29, 2
0x825A7584:  add          r7, r11, r29
0x825A7588:  fadds        f3, f0, f15
0x825A758C:  add          r10, r10, r4
0x825A7590:  fadds        f4, f13, f15
0x825A7594:  add          r6, r7, r11
0x825A7598:  fsubs        f1, f11, f15
0x825A759C:  addi         r8, r7, -2
0x825A75A0:  addi         r3, r6, -2
0x825A75A4:  add          r5, r6, r11
0x825A75A8:  lfs          f9, -4(r10)
0x825A75AC:  slwi         r31, r3, 2
0x825A75B0:  slwi         r28, r26, 2
0x825A75B4:  slwi         r30, r8, 2
0x825A75B8:  fmuls        f12, f2, f16
0x825A75BC:  slwi         r11, r6, 2
0x825A75C0:  fneg         f2, f9
0x825A75C4:  addi         r3, r5, -2
0x825A75C8:  slwi         r9, r7, 2
0x825A75CC:  lfsx         f6, r31, r4
0x825A75D0:  slwi         r8, r5, 2
0x825A75D4:  lfsx         f10, r28, r4
0x825A75D8:  slwi         r3, r3, 2
0x825A75DC:  fmuls        f13, f3, f17
0x825A75E0:  add          r11, r11, r4
0x825A75E4:  fadds        f3, f10, f6
0x825A75E8:  add          r9, r9, r4
0x825A75EC:  fmuls        f0, f4, f17
0x825A75F0:  add          r8, r8, r4
0x825A75F4:  lfsx         f8, r30, r4
0x825A75F8:  fsubs        f10, f10, f6
0x825A75FC:  addi         r26, r5, 2
0x825A7600:  lfsx         f4, r3, r4
0x825A7604:  fmuls        f11, f1, f16
0x825A7608:  lfs          f5, -4(r11)
0x825A760C:  fadds        f1, f8, f4
0x825A7610:  lfs          f7, -4(r9)
0x825A7614:  fsubs        f2, f2, f5
0x825A7618:  lfs          f6, -4(r8)
0x825A761C:  fsubs        f9, f5, f9
0x825A7620:  fadds        f5, f7, f6
0x825A7624:  addi         r25, r7, 2
0x825A7628:  fsubs        f8, f8, f4
0x825A762C:  fsubs        f7, f7, f6
0x825A7630:  fadds        f6, f1, f3
0x825A7634:  stfsx        f6, r28, r4
0x825A7638:  fsubs        f4, f3, f1
0x825A763C:  fsubs        f3, f2, f5
0x825A7640:  stfs         f3, -4(r10)
0x825A7644:  fadds        f2, f5, f2
0x825A7648:  stfsx        f4, r30, r4
0x825A764C:  fadds        f5, f8, f9
0x825A7650:  stfs         f2, -4(r9)
0x825A7654:  fadds        f6, f7, f10
0x825A7658:  fsubs        f10, f10, f7
0x825A765C:  fsubs        f9, f9, f8
0x825A7660:  fmuls        f1, f0, f5
0x825A7664:  fmuls        f4, f0, f6
0x825A7668:  fmsubs       f3, f13, f6, f1
0x825A766C:  stfsx        f3, r31, r4
0x825A7670:  fmuls        f1, f12, f10
0x825A7674:  addi         r31, r6, 2
0x825A7678:  fmuls        f10, f11, f10
0x825A767C:  addi         r6, r6, 3
0x825A7680:  fmadds       f2, f13, f5, f4
0x825A7684:  stfs         f2, -4(r11)
0x825A7688:  fneg         f6, f15
0x825A768C:  fmadds       f8, f11, f9, f1
0x825A7690:  stfsx        f8, r3, r4
0x825A7694:  addi         r3, r29, 3
0x825A7698:  fmsubs       f7, f12, f9, f10
0x825A769C:  stfs         f7, -4(r8)
0x825A76A0:  slwi         r27, r3, 2
0x825A76A4:  lfs          f10, 0(r10)
0x825A76A8:  addi         r3, r29, 2
0x825A76AC:  lfs          f9, 0(r11)
0x825A76B0:  lfs          f8, 0(r8)
0x825A76B4:  slwi         r29, r31, 2
0x825A76B8:  lfs          f7, 0(r9)
0x825A76BC:  slwi         r28, r3, 2
0x825A76C0:  fadds        f1, f10, f9
0x825A76C4:  lfs          f5, 4(r10)
0x825A76C8:  fsubs        f10, f10, f9
0x825A76CC:  lfs          f4, 4(r11)
0x825A76D0:  fadds        f9, f7, f8
0x825A76D4:  lfs          f3, 4(r8)
0x825A76D8:  fneg         f31, f5
0x825A76DC:  lfs          f2, 4(r9)
0x825A76E0:  fsubs        f8, f7, f8
0x825A76E4:  slwi         r3, r25, 2
0x825A76E8:  fsubs        f7, f4, f5
0x825A76EC:  slwi         r31, r26, 2
0x825A76F0:  fadds        f5, f2, f3
0x825A76F4:  addi         r5, r5, 3
0x825A76F8:  fsubs        f3, f2, f3
0x825A76FC:  slwi         r30, r6, 2
0x825A7700:  slwi         r6, r5, 2
0x825A7704:  fadds        f2, f9, f1
0x825A7708:  stfs         f2, 0(r10)
0x825A770C:  fsubs        f1, f1, f9
0x825A7710:  fsubs        f9, f31, f4
0x825A7714:  fadds        f4, f3, f10
0x825A7718:  fsubs        f10, f10, f3
0x825A771C:  fsubs        f2, f9, f5
0x825A7720:  stfs         f2, 4(r10)
0x825A7724:  fadds        f5, f5, f9
0x825A7728:  stfs         f1, 0(r9)
0x825A772C:  fadds        f9, f8, f7
0x825A7730:  stfs         f5, 4(r9)
0x825A7734:  fsubs        f2, f4, f9
0x825A7738:  fadds        f1, f9, f4
0x825A773C:  fsubs        f9, f7, f8
0x825A7740:  fmuls        f8, f2, f15
0x825A7744:  stfs         f8, 0(r11)
0x825A7748:  fmuls        f7, f1, f15
0x825A774C:  stfs         f7, 4(r11)
0x825A7750:  fadds        f5, f9, f10
0x825A7754:  lfsx         f8, r31, r4
0x825A7758:  fsubs        f4, f9, f10
0x825A775C:  addi         r11, r7, 3
0x825A7760:  slwi         r11, r11, 2
0x825A7764:  fmuls        f3, f5, f6
0x825A7768:  stfs         f3, 0(r8)
0x825A776C:  fmuls        f2, f4, f6
0x825A7770:  stfs         f2, 4(r8)
0x825A7774:  lfsx         f7, r29, r4
0x825A7778:  lfsx         f9, r28, r4
0x825A777C:  lfsx         f5, r3, r4
0x825A7780:  fadds        f4, f9, f7
0x825A7784:  fsubs        f9, f9, f7
0x825A7788:  lfsx         f10, r27, r4
0x825A778C:  fadds        f7, f5, f8
0x825A7790:  lfsx         f6, r30, r4
0x825A7794:  fneg         f1, f10
0x825A7798:  lfsx         f3, r6, r4
0x825A779C:  fsubs        f8, f5, f8
0x825A77A0:  fsubs        f10, f6, f10
0x825A77A4:  fadds        f5, f7, f4
0x825A77A8:  fsubs        f4, f4, f7
0x825A77AC:  lfsx         f7, r11, r4
0x825A77B0:  stfsx        f5, r28, r4
0x825A77B4:  fsubs        f6, f1, f6
0x825A77B8:  fadds        f5, f7, f3
0x825A77BC:  fsubs        f7, f7, f3
0x825A77C0:  fsubs        f3, f6, f5
0x825A77C4:  stfsx        f3, r27, r4
0x825A77C8:  fadds        f2, f5, f6
0x825A77CC:  stfsx        f4, r3, r4
0x825A77D0:  fadds        f5, f8, f10
0x825A77D4:  stfsx        f2, r11, r4
0x825A77D8:  fadds        f6, f7, f9
0x825A77DC:  fsubs        f10, f10, f8
0x825A77E0:  fmuls        f1, f13, f5
0x825A77E4:  fmuls        f4, f13, f6
0x825A77E8:  fsubs        f13, f9, f7
0x825A77EC:  fmsubs       f3, f0, f6, f1
0x825A77F0:  stfsx        f3, r29, r4
0x825A77F4:  fmadds       f2, f0, f5, f4
0x825A77F8:  stfsx        f2, r30, r4
0x825A77FC:  fmuls        f1, f11, f13
0x825A7800:  fmuls        f0, f12, f13
0x825A7804:  fmadds       f13, f12, f10, f1
0x825A7808:  stfsx        f13, r31, r4
0x825A780C:  fmsubs       f12, f11, f10, f0
0x825A7810:  stfsx        f12, r6, r4
0x825A7814:  addi         r12, r1, -0x40
0x825A7818:  bl           0x8243662c
0x825A781C:  b            0x8242f8dc
0x825A7820:  mflr         r12
0x825A7824:  bl           0x8242f890
0x825A7828:  stfd         f31, -0x40(r1)
0x825A782C:  srawi        r26, r3, 3
0x825A7830:  lfs          f0, 0(r4)
0x825A7834:  lfs          f13, 4(r4)
0x825A7838:  slwi         r11, r26, 1
0x825A783C:  cmpwi        cr6, r26, 2
0x825A7840:  slwi         r9, r11, 1
0x825A7844:  slwi         r10, r11, 2
0x825A7848:  add          r8, r9, r11
0x825A784C:  slwi         r9, r9, 2
0x825A7850:  slwi         r8, r8, 2
0x825A7854:  add          r9, r9, r4
0x825A7858:  add          r10, r10, r4
0x825A785C:  add          r8, r8, r4
0x825A7860:  lfs          f11, 0(r9)
0x825A7864:  lfs          f12, 0(r10)
0x825A7868:  fadds        f7, f0, f11
0x825A786C:  lfs          f9, 0(r8)
0x825A7870:  fsubs        f0, f0, f11
0x825A7874:  lfs          f10, 4(r10)
0x825A7878:  fadds        f6, f12, f9
0x825A787C:  lfs          f8, 4(r9)
0x825A7880:  fsubs        f12, f12, f9
0x825A7884:  lfs          f11, 4(r8)
0x825A7888:  fadds        f5, f13, f8
0x825A788C:  fadds        f9, f10, f11
0x825A7890:  fsubs        f13, f13, f8
0x825A7894:  fsubs        f11, f10, f11
0x825A7898:  fadds        f10, f6, f7
0x825A789C:  stfs         f10, 0(r4)
0x825A78A0:  fsubs        f8, f7, f6
0x825A78A4:  fadds        f4, f9, f5
0x825A78A8:  stfs         f4, 4(r4)
0x825A78AC:  fsubs        f3, f5, f9
0x825A78B0:  stfs         f8, 0(r10)
0x825A78B4:  stfs         f3, 4(r10)
0x825A78B8:  fadds        f7, f12, f13
0x825A78BC:  fsubs        f2, f0, f11
0x825A78C0:  stfs         f7, 4(r9)
0x825A78C4:  stfs         f2, 0(r9)
0x825A78C8:  fsubs        f6, f13, f12
0x825A78CC:  fadds        f1, f11, f0
0x825A78D0:  stfs         f6, 4(r8)
0x825A78D4:  stfs         f1, 0(r8)
0x825A78D8:  lfs          f31, 4(r5)
0x825A78DC:  ble          cr6, 0x825a7ad0
0x825A78E0:  slwi         r8, r11, 4
0x825A78E4:  slwi         r10, r11, 3
0x825A78E8:  addi         r7, r11, 2
0x825A78EC:  add          r8, r8, r4
0x825A78F0:  add          r9, r10, r4
0x825A78F4:  slwi         r10, r7, 2
0x825A78F8:  addi         r30, r8, -8
0x825A78FC:  addi         r31, r26, -3
0x825A7900:  slwi         r8, r11, 1
0x825A7904:  add          r7, r10, r4
0x825A7908:  srwi         r31, r31, 1
0x825A790C:  addi         r10, r5, 8
0x825A7910:  add          r5, r11, r8
0x825A7914:  addi         r29, r31, 1
0x825A7918:  slwi         r31, r5, 2
0x825A791C:  addi         r6, r11, -2
0x825A7920:  add          r31, r31, r4
0x825A7924:  slwi         r3, r6, 2
0x825A7928:  addi         r8, r9, 8
0x825A792C:  addi         r5, r9, -8
0x825A7930:  addi         r9, r31, 8
0x825A7934:  addi         r6, r4, 8
0x825A7938:  add          r3, r3, r4
0x825A793C:  addi         r31, r31, -8
0x825A7940:  lfs          f0, 0(r6)
0x825A7944:  addi         r10, r10, 0x10
0x825A7948:  lfs          f13, 0(r8)
0x825A794C:  addi         r28, r31, 4
0x825A7950:  lfs          f12, 0(r9)
0x825A7954:  fadds        f8, f13, f0
0x825A7958:  lfs          f11, 0(r7)
0x825A795C:  fsubs        f6, f0, f13
0x825A7960:  lfs          f9, 4(r8)
0x825A7964:  fadds        f4, f12, f11
0x825A7968:  lfs          f10, 4(r6)
0x825A796C:  fsubs        f3, f11, f12
0x825A7970:  lfs          f5, 4(r7)
0x825A7974:  fadds        f2, f9, f10
0x825A7978:  lfs          f7, 4(r9)
0x825A797C:  fsubs        f10, f10, f9
0x825A7980:  fadds        f9, f7, f5
0x825A7984:  lfs          f0, -4(r10)
0x825A7988:  fsubs        f7, f5, f7
0x825A798C:  lfs          f13, -8(r10)
0x825A7990:  lfs          f11, 0(r10)
0x825A7994:  addi         r27, r30, 4
0x825A7998:  lfs          f12, 4(r10)
0x825A799C:  addi         r29, r29, -1
0x825A79A0:  fneg         f12, f12
0x825A79A4:  fadds        f5, f4, f8
0x825A79A8:  stfs         f5, 0(r6)
0x825A79AC:  fsubs        f4, f8, f4
0x825A79B0:  fadds        f1, f9, f2
0x825A79B4:  stfs         f1, 4(r6)
0x825A79B8:  fsubs        f8, f2, f9
0x825A79BC:  stfs         f8, 4(r7)
0x825A79C0:  fsubs        f9, f6, f7
0x825A79C4:  stfs         f4, 0(r7)
0x825A79C8:  fadds        f8, f3, f10
0x825A79CC:  addi         r7, r7, 8
0x825A79D0:  fsubs        f10, f10, f3
0x825A79D4:  addi         r6, r6, 8
0x825A79D8:  fmr          f5, f9
0x825A79DC:  fmuls        f2, f0, f9
0x825A79E0:  fadds        f9, f7, f6
0x825A79E4:  fmuls        f4, f0, f8
0x825A79E8:  fmadds       f8, f13, f8, f2
0x825A79EC:  stfs         f8, 4(r8)
0x825A79F0:  fmuls        f7, f11, f9
0x825A79F4:  fmuls        f6, f12, f9
0x825A79F8:  fmsubs       f1, f13, f5, f4
0x825A79FC:  stfs         f1, 0(r8)
0x825A7A00:  addi         r8, r8, 8
0x825A7A04:  fmadds       f5, f12, f10, f7
0x825A7A08:  stfs         f5, 0(r9)
0x825A7A0C:  fmsubs       f4, f11, f10, f6
0x825A7A10:  stfs         f4, 4(r9)
0x825A7A14:  lfs          f9, 0(r31)
0x825A7A18:  addi         r9, r9, 8
0x825A7A1C:  lfs          f10, 0(r3)
0x825A7A20:  lfs          f7, 0(r5)
0x825A7A24:  fadds        f2, f9, f10
0x825A7A28:  lfs          f8, 0(r30)
0x825A7A2C:  fsubs        f10, f10, f9
0x825A7A30:  lfs          f4, 4(r5)
0x825A7A34:  fadds        f9, f8, f7
0x825A7A38:  lfs          f6, 0(r28)
0x825A7A3C:  fsubs        f8, f7, f8
0x825A7A40:  lfs          f5, 0(r27)
0x825A7A44:  lfs          f3, 4(r3)
0x825A7A48:  fadds        f7, f5, f4
0x825A7A4C:  fadds        f1, f6, f3
0x825A7A50:  fsubs        f6, f3, f6
0x825A7A54:  fsubs        f5, f4, f5
0x825A7A58:  fadds        f3, f9, f2
0x825A7A5C:  stfs         f3, 0(r3)
0x825A7A60:  fsubs        f3, f2, f9
0x825A7A64:  fadds        f4, f7, f1
0x825A7A68:  stfs         f4, 4(r3)
0x825A7A6C:  fsubs        f2, f1, f7
0x825A7A70:  stfs         f3, 0(r5)
0x825A7A74:  stfs         f2, 4(r5)
0x825A7A78:  fsubs        f9, f10, f5
0x825A7A7C:  fadds        f7, f8, f6
0x825A7A80:  addi         r5, r5, -8
0x825A7A84:  fmuls        f1, f13, f7
0x825A7A88:  addi         r3, r3, -8
0x825A7A8C:  fmuls        f4, f13, f9
0x825A7A90:  cmplwi       cr6, r29, 0
0x825A7A94:  fadds        f13, f5, f10
0x825A7A98:  fsubs        f10, f6, f8
0x825A7A9C:  fmsubs       f3, f0, f9, f1
0x825A7AA0:  stfs         f3, 0(r31)
0x825A7AA4:  fmadds       f2, f0, f7, f4
0x825A7AA8:  stfs         f2, 0(r28)
0x825A7AAC:  fmuls        f1, f12, f13
0x825A7AB0:  addi         r31, r31, -8
0x825A7AB4:  fmuls        f0, f11, f13
0x825A7AB8:  fmadds       f13, f11, f10, f1
0x825A7ABC:  stfs         f13, 0(r30)
0x825A7AC0:  fmsubs       f12, f12, f10, f0
0x825A7AC4:  addi         r30, r30, -8
0x825A7AC8:  stfs         f12, 0(r27)
0x825A7ACC:  bne          cr6, 0x825a7940
0x825A7AD0:  add          r9, r11, r26
0x825A7AD4:  fneg         f0, f31
0x825A7AD8:  slwi         r10, r26, 2
0x825A7ADC:  add          r8, r9, r11
0x825A7AE0:  slwi         r9, r9, 2
0x825A7AE4:  add          r7, r8, r11
0x825A7AE8:  slwi         r11, r8, 2
0x825A7AEC:  slwi         r8, r7, 2
0x825A7AF0:  add          r10, r10, r4
0x825A7AF4:  add          r11, r11, r4
0x825A7AF8:  add          r9, r9, r4
0x825A7AFC:  add          r8, r8, r4
0x825A7B00:  lfs          f13, 0(r10)
0x825A7B04:  lfs          f10, 0(r11)
0x825A7B08:  lfs          f12, 0(r9)
0x825A7B0C:  fadds        f6, f13, f10
0x825A7B10:  lfs          f8, 0(r8)
0x825A7B14:  fsubs        f13, f13, f10
0x825A7B18:  lfs          f9, 4(r9)
0x825A7B1C:  fadds        f5, f12, f8
0x825A7B20:  lfs          f7, 4(r11)
0x825A7B24:  fsubs        f12, f12, f8
0x825A7B28:  lfs          f11, 4(r10)
0x825A7B2C:  lfs          f10, 4(r8)
0x825A7B30:  fadds        f4, f11, f7
0x825A7B34:  fadds        f8, f9, f10
0x825A7B38:  fsubs        f10, f9, f10
0x825A7B3C:  fsubs        f11, f11, f7
0x825A7B40:  fadds        f9, f5, f6
0x825A7B44:  stfs         f9, 0(r10)
0x825A7B48:  fsubs        f7, f6, f5
0x825A7B4C:  fadds        f6, f8, f4
0x825A7B50:  stfs         f6, 4(r10)
0x825A7B54:  fsubs        f5, f4, f8
0x825A7B58:  stfs         f7, 0(r9)
0x825A7B5C:  fsubs        f9, f13, f10
0x825A7B60:  stfs         f5, 4(r9)
0x825A7B64:  fadds        f8, f12, f11
0x825A7B68:  fadds        f13, f10, f13
0x825A7B6C:  fsubs        f12, f11, f12
0x825A7B70:  fsubs        f4, f9, f8
0x825A7B74:  fadds        f3, f8, f9
0x825A7B78:  fadds        f11, f12, f13
0x825A7B7C:  fsubs        f10, f12, f13
0x825A7B80:  fmuls        f2, f4, f31
0x825A7B84:  stfs         f2, 0(r11)
0x825A7B88:  fmuls        f1, f3, f31
0x825A7B8C:  stfs         f1, 4(r11)
0x825A7B90:  fmuls        f9, f11, f0
0x825A7B94:  stfs         f9, 0(r8)
0x825A7B98:  fmuls        f8, f10, f0
0x825A7B9C:  stfs         f8, 4(r8)
0x825A7BA0:  lfd          f31, -0x40(r1)
0x825A7BA4:  b            0x8242f8e0
0x825A7BA8:  mflr         r12
0x825A7BAC:  bl           0x8242f884
0x825A7BB0:  addi         r12, r1, -0x50
0x825A7BB4:  bl           0x8243660c
0x825A7BB8:  srawi        r24, r3, 3
0x825A7BBC:  lfs          f12, 4(r4)
0x825A7BC0:  lfs          f13, 0(r4)
0x825A7BC4:  slwi         r11, r24, 1
0x825A7BC8:  lfs          f0, 4(r5)
0x825A7BCC:  cmpwi        cr6, r24, 2
0x825A7BD0:  slwi         r9, r11, 1
0x825A7BD4:  slwi         r23, r11, 2
0x825A7BD8:  add          r8, r9, r11
0x825A7BDC:  slwi         r9, r9, 2
0x825A7BE0:  slwi         r8, r8, 2
0x825A7BE4:  add          r10, r23, r4
0x825A7BE8:  add          r9, r9, r4
0x825A7BEC:  add          r8, r8, r4
0x825A7BF0:  slwi         r7, r11, 1
0x825A7BF4:  lfs          f9, 4(r10)
0x825A7BF8:  lfs          f10, 0(r9)
0x825A7BFC:  lfs          f8, 0(r8)
0x825A7C00:  fadds        f6, f12, f10
0x825A7C04:  lfs          f7, 4(r9)
0x825A7C08:  fadds        f5, f8, f9
0x825A7C0C:  fsubs        f12, f12, f10
0x825A7C10:  lfs          f10, 4(r8)
0x825A7C14:  fsubs        f9, f9, f8
0x825A7C18:  lfs          f11, 0(r10)
0x825A7C1C:  fsubs        f8, f13, f7
0x825A7C20:  fadds        f13, f7, f13
0x825A7C24:  fsubs        f7, f11, f10
0x825A7C28:  fadds        f11, f10, f11
0x825A7C2C:  fsubs        f10, f7, f5
0x825A7C30:  fadds        f7, f5, f7
0x825A7C34:  fadds        f4, f9, f11
0x825A7C38:  fsubs        f5, f11, f9
0x825A7C3C:  fmuls        f11, f10, f0
0x825A7C40:  fmuls        f10, f7, f0
0x825A7C44:  fadds        f3, f11, f8
0x825A7C48:  stfs         f3, 0(r4)
0x825A7C4C:  fsubs        f1, f8, f11
0x825A7C50:  fadds        f2, f10, f6
0x825A7C54:  stfs         f2, 4(r4)
0x825A7C58:  fsubs        f11, f6, f10
0x825A7C5C:  stfs         f11, 4(r10)
0x825A7C60:  fmuls        f11, f5, f0
0x825A7C64:  stfs         f1, 0(r10)
0x825A7C68:  fmuls        f0, f4, f0
0x825A7C6C:  fadds        f10, f11, f12
0x825A7C70:  stfs         f10, 4(r9)
0x825A7C74:  fsubs        f9, f13, f0
0x825A7C78:  stfs         f9, 0(r9)
0x825A7C7C:  fadds        f8, f0, f13
0x825A7C80:  stfs         f8, 0(r8)
0x825A7C84:  fsubs        f7, f12, f11
0x825A7C88:  stfs         f7, 4(r8)
0x825A7C8C:  ble          cr6, 0x825a7ee0
0x825A7C90:  addi         r6, r7, 2
0x825A7C94:  slwi         r7, r11, 4
0x825A7C98:  addi         r31, r24, -3
0x825A7C9C:  add          r7, r7, r4
0x825A7CA0:  srwi         r31, r31, 1
0x825A7CA4:  addi         r3, r11, -2
0x825A7CA8:  addi         r28, r7, -8
0x825A7CAC:  slwi         r7, r11, 1
0x825A7CB0:  slwi         r30, r3, 2
0x825A7CB4:  addi         r27, r31, 1
0x825A7CB8:  add          r31, r11, r7
0x825A7CBC:  add          r29, r30, r4
0x825A7CC0:  slwi         r10, r11, 3
0x825A7CC4:  addi         r9, r11, 2
0x825A7CC8:  slwi         r30, r31, 2
0x825A7CCC:  add          r8, r10, r4
0x825A7CD0:  slwi         r9, r9, 2
0x825A7CD4:  slwi         r10, r6, 2
0x825A7CD8:  add          r30, r30, r4
0x825A7CDC:  add          r6, r9, r4
0x825A7CE0:  addi         r7, r8, 8
0x825A7CE4:  addi         r31, r8, -8
0x825A7CE8:  add          r9, r10, r5
0x825A7CEC:  addi         r8, r30, 8
0x825A7CF0:  addi         r3, r4, 8
0x825A7CF4:  addi         r10, r5, 8
0x825A7CF8:  addi         r30, r30, -8
0x825A7CFC:  lfs          f1, 4(r3)
0x825A7D00:  addi         r10, r10, 0x10
0x825A7D04:  lfs          f5, 0(r7)
0x825A7D08:  addi         r9, r9, -0x10
0x825A7D0C:  lfs          f2, 4(r7)
0x825A7D10:  fadds        f28, f5, f1
0x825A7D14:  lfs          f6, 0(r3)
0x825A7D18:  fsubs        f5, f1, f5
0x825A7D1C:  lfs          f30, 4(r6)
0x825A7D20:  fsubs        f29, f6, f2
0x825A7D24:  lfs          f3, 0(r8)
0x825A7D28:  fadds        f6, f2, f6
0x825A7D2C:  lfs          f31, 4(r8)
0x825A7D30:  fadds        f26, f3, f30
0x825A7D34:  lfs          f4, 0(r6)
0x825A7D38:  fsubs        f3, f30, f3
0x825A7D3C:  fsubs        f27, f4, f31
0x825A7D40:  lfs          f13, -4(r10)
0x825A7D44:  lfs          f10, 4(r10)
0x825A7D48:  fadds        f4, f31, f4
0x825A7D4C:  lfs          f12, -8(r9)
0x825A7D50:  fneg         f10, f10
0x825A7D54:  lfs          f8, 4(r9)
0x825A7D58:  addi         r26, r29, 4
0x825A7D5C:  fmuls        f2, f13, f28
0x825A7D60:  lfs          f0, -8(r10)
0x825A7D64:  fneg         f8, f8
0x825A7D68:  lfs          f9, 0(r10)
0x825A7D6C:  fmuls        f1, f13, f29
0x825A7D70:  lfs          f11, -4(r9)
0x825A7D74:  fmuls        f25, f9, f6
0x825A7D78:  lfs          f7, 0(r9)
0x825A7D7C:  fmuls        f31, f12, f26
0x825A7D80:  addi         r25, r28, 4
0x825A7D84:  addi         r27, r27, -1
0x825A7D88:  fmuls        f30, f12, f27
0x825A7D8C:  cmplwi       cr6, r27, 0
0x825A7D90:  fmsubs       f2, f0, f29, f2
0x825A7D94:  fmuls        f29, f10, f6
0x825A7D98:  fmadds       f6, f0, f28, f1
0x825A7D9C:  fmuls        f28, f8, f4
0x825A7DA0:  fmsubs       f1, f11, f27, f31
0x825A7DA4:  fmadds       f31, f11, f26, f30
0x825A7DA8:  fmuls        f30, f7, f4
0x825A7DAC:  fadds        f4, f1, f2
0x825A7DB0:  stfs         f4, 0(r3)
0x825A7DB4:  fsubs        f2, f2, f1
0x825A7DB8:  fadds        f1, f31, f6
0x825A7DBC:  stfs         f1, 4(r3)
0x825A7DC0:  fsubs        f4, f6, f31
0x825A7DC4:  stfs         f4, 4(r6)
0x825A7DC8:  fmadds       f4, f7, f3, f28
0x825A7DCC:  stfs         f2, 0(r6)
0x825A7DD0:  fmadds       f6, f10, f5, f25
0x825A7DD4:  addi         r6, r6, 8
0x825A7DD8:  fmsubs       f5, f9, f5, f29
0x825A7DDC:  addi         r3, r3, 8
0x825A7DE0:  fmsubs       f3, f8, f3, f30
0x825A7DE4:  fadds        f2, f4, f6
0x825A7DE8:  stfs         f2, 0(r7)
0x825A7DEC:  fsubs        f6, f6, f4
0x825A7DF0:  fadds        f1, f3, f5
0x825A7DF4:  stfs         f1, 4(r7)
0x825A7DF8:  fsubs        f5, f5, f3
0x825A7DFC:  stfs         f6, 0(r8)
0x825A7E00:  stfs         f5, 4(r8)
0x825A7E04:  addi         r7, r7, 8
0x825A7E08:  lfs          f6, 0(r29)
0x825A7E0C:  addi         r8, r8, 8
0x825A7E10:  lfs          f2, 4(r30)
0x825A7E14:  lfs          f5, 0(r30)
0x825A7E18:  fsubs        f29, f6, f2
0x825A7E1C:  lfs          f4, 0(r31)
0x825A7E20:  fadds        f6, f2, f6
0x825A7E24:  lfs          f3, 0(r28)
0x825A7E28:  lfs          f1, 0(r26)
0x825A7E2C:  lfs          f31, 0(r25)
0x825A7E30:  fadds        f28, f5, f1
0x825A7E34:  lfs          f30, 4(r31)
0x825A7E38:  fsubs        f27, f4, f31
0x825A7E3C:  fadds        f26, f3, f30
0x825A7E40:  fsubs        f5, f1, f5
0x825A7E44:  fmuls        f2, f11, f28
0x825A7E48:  fmuls        f1, f11, f29
0x825A7E4C:  fmuls        f11, f0, f26
0x825A7E50:  fadds        f4, f31, f4
0x825A7E54:  fmuls        f31, f0, f27
0x825A7E58:  fsubs        f3, f30, f3
0x825A7E5C:  fmuls        f30, f7, f6
0x825A7E60:  fmuls        f6, f8, f6
0x825A7E64:  fmsubs       f0, f12, f29, f2
0x825A7E68:  fmadds       f12, f12, f28, f1
0x825A7E6C:  fmsubs       f11, f13, f27, f11
0x825A7E70:  fmuls        f2, f10, f4
0x825A7E74:  fmadds       f13, f13, f26, f31
0x825A7E78:  fmuls        f1, f9, f4
0x825A7E7C:  fadds        f4, f11, f0
0x825A7E80:  stfs         f4, 0(r29)
0x825A7E84:  fsubs        f0, f0, f11
0x825A7E88:  addi         r29, r29, -8
0x825A7E8C:  fadds        f11, f13, f12
0x825A7E90:  stfs         f11, 0(r26)
0x825A7E94:  stfs         f0, 0(r31)
0x825A7E98:  fsubs        f4, f12, f13
0x825A7E9C:  fmsubs       f11, f10, f3, f1
0x825A7EA0:  stfs         f4, 4(r31)
0x825A7EA4:  fmadds       f0, f8, f5, f30
0x825A7EA8:  addi         r31, r31, -8
0x825A7EAC:  fmadds       f12, f9, f3, f2
0x825A7EB0:  fmsubs       f13, f7, f5, f6
0x825A7EB4:  fadds        f3, f12, f0
0x825A7EB8:  stfs         f3, 0(r30)
0x825A7EBC:  fadds        f2, f11, f13
0x825A7EC0:  stfs         f2, 4(r30)
0x825A7EC4:  fsubs        f1, f0, f12
0x825A7EC8:  stfs         f1, 0(r28)
0x825A7ECC:  fsubs        f0, f13, f11
0x825A7ED0:  addi         r28, r28, -8
0x825A7ED4:  stfs         f0, 0(r25)
0x825A7ED8:  addi         r30, r30, -8
0x825A7EDC:  bne          cr6, 0x825a7cfc
0x825A7EE0:  add          r8, r23, r5
0x825A7EE4:  add          r9, r11, r24
0x825A7EE8:  slwi         r10, r24, 2
0x825A7EEC:  add          r7, r9, r11
0x825A7EF0:  add          r10, r10, r4
0x825A7EF4:  lfs          f0, 0(r8)
0x825A7EF8:  slwi         r9, r9, 2
0x825A7EFC:  lfs          f13, 4(r8)
0x825A7F00:  add          r8, r7, r11
0x825A7F04:  slwi         r11, r7, 2
0x825A7F08:  slwi         r8, r8, 2
0x825A7F0C:  add          r11, r11, r4
0x825A7F10:  lfs          f12, 0(r10)
0x825A7F14:  lfs          f10, 4(r10)
0x825A7F18:  add          r9, r9, r4
0x825A7F1C:  add          r8, r8, r4
0x825A7F20:  lfs          f6, 4(r11)
0x825A7F24:  lfs          f9, 0(r11)
0x825A7F28:  fsubs        f3, f12, f6
0x825A7F2C:  fadds        f5, f9, f10
0x825A7F30:  lfs          f7, 0(r8)
0x825A7F34:  lfs          f8, 4(r9)
0x825A7F38:  fsubs        f10, f10, f9
0x825A7F3C:  lfs          f9, 4(r8)
0x825A7F40:  fadds        f4, f8, f7
0x825A7F44:  lfs          f11, 0(r9)
0x825A7F48:  fadds        f12, f6, f12
0x825A7F4C:  fsubs        f8, f8, f7
0x825A7F50:  fsubs        f7, f11, f9
0x825A7F54:  fadds        f11, f9, f11
0x825A7F58:  fmuls        f6, f13, f3
0x825A7F5C:  fmuls        f2, f13, f5
0x825A7F60:  fmuls        f1, f0, f10
0x825A7F64:  fmuls        f31, f0, f4
0x825A7F68:  fmuls        f30, f0, f12
0x825A7F6C:  fmuls        f29, f13, f8
0x825A7F70:  fmadds       f6, f0, f5, f6
0x825A7F74:  fmuls        f5, f0, f7
0x825A7F78:  fmsubs       f9, f0, f3, f2
0x825A7F7C:  fmuls        f3, f13, f11
0x825A7F80:  fmsubs       f7, f13, f7, f31
0x825A7F84:  fmsubs       f12, f13, f12, f1
0x825A7F88:  fmsubs       f11, f0, f11, f29
0x825A7F8C:  fmadds       f5, f13, f4, f5
0x825A7F90:  fmadds       f13, f13, f10, f30
0x825A7F94:  fmadds       f0, f0, f8, f3
0x825A7F98:  fadds        f2, f7, f9
0x825A7F9C:  stfs         f2, 0(r10)
0x825A7FA0:  fsubs        f2, f9, f7
0x825A7FA4:  fsubs        f8, f12, f11
0x825A7FA8:  fadds        f4, f5, f6
0x825A7FAC:  stfs         f4, 4(r10)
0x825A7FB0:  fsubs        f9, f6, f5
0x825A7FB4:  stfs         f2, 0(r9)
0x825A7FB8:  stfs         f9, 4(r9)
0x825A7FBC:  fsubs        f7, f13, f0
0x825A7FC0:  stfs         f8, 0(r11)
0x825A7FC4:  fadds        f6, f11, f12
0x825A7FC8:  stfs         f7, 4(r11)
0x825A7FCC:  fadds        f5, f0, f13
0x825A7FD0:  stfs         f6, 0(r8)
0x825A7FD4:  stfs         f5, 4(r8)
0x825A7FD8:  addi         r12, r1, -0x50
0x825A7FDC:  bl           0x82436658
0x825A7FE0:  b            0x8242f8d4
0x825A7FE4:  .align 0                       # padding
0x825A7FE8:  mflr         r12
0x825A7FEC:  stw          r12, -8(r1)
0x825A7FF0:  addi         r12, r1, -8
0x825A7FF4:  bl           0x824365e0
0x825A7FF8:  lfs          f8, 4(r3)
0x825A7FFC:  lfs          f9, 0x44(r3)
0x825A8000:  lfs          f11, 8(r4)
0x825A8004:  fadds        f25, f8, f9
0x825A8008:  lfs          f0, 4(r4)
0x825A800C:  fsubs        f9, f8, f9
0x825A8010:  lfs          f12, 0(r3)
0x825A8014:  fmuls        f13, f11, f0
0x825A8018:  lfs          f6, 0x20(r3)
0x825A801C:  lfs          f10, 0x40(r3)
0x825A8020:  lfs          f7, 0x60(r3)
0x825A8024:  fadds        f26, f12, f10
0x825A8028:  lfs          f4, 0x24(r3)
0x825A802C:  fadds        f8, f6, f7
0x825A8030:  lfs          f5, 0x64(r3)
0x825A8034:  fsubs        f7, f6, f7
0x825A8038:  fadds        f6, f4, f5
0x825A803C:  lfs          f2, 8(r3)
0x825A8040:  fsubs        f10, f12, f10
0x825A8044:  lfs          f3, 0x48(r3)
0x825A8048:  fsubs        f5, f4, f5
0x825A804C:  lfs          f1, 0x4c(r3)
0x825A8050:  lfs          f31, 0xc(r3)
0x825A8054:  fadds        f12, f11, f13
0x825A8058:  lfs          f28, 0x6c(r3)
0x825A805C:  lfs          f27, 0x2c(r3)
0x825A8060:  lfs          f30, 0x68(r3)
0x825A8064:  lfs          f29, 0x28(r3)
0x825A8068:  fadds        f11, f8, f26
0x825A806C:  lfs          f22, 0x70(r3)
0x825A8070:  fsubs        f8, f26, f8
0x825A8074:  lfs          f21, 0x30(r3)
0x825A8078:  fadds        f26, f2, f3
0x825A807C:  lfs          f20, 0x74(r3)
0x825A8080:  fadds        f4, f6, f25
0x825A8084:  lfs          f19, 0x34(r3)
0x825A8088:  fsubs        f3, f2, f3
0x825A808C:  lfs          f18, 0x58(r3)
0x825A8090:  fsubs        f6, f25, f6
0x825A8094:  lfs          f17, 0x18(r3)
0x825A8098:  fadds        f25, f31, f1
0x825A809C:  lfs          f16, 0x5c(r3)
0x825A80A0:  fsubs        f2, f31, f1
0x825A80A4:  lfs          f15, 0x1c(r3)
0x825A80A8:  fsubs        f31, f27, f28
0x825A80AC:  fsubs        f24, f10, f5
0x825A80B0:  fadds        f10, f5, f10
0x825A80B4:  fadds        f5, f27, f28
0x825A80B8:  fsubs        f1, f29, f30
0x825A80BC:  fadds        f23, f7, f9
0x825A80C0:  fsubs        f9, f9, f7
0x825A80C4:  fadds        f7, f29, f30
0x825A80C8:  fsubs        f27, f3, f31
0x825A80CC:  fadds        f3, f31, f3
0x825A80D0:  fadds        f29, f5, f25
0x825A80D4:  fsubs        f5, f25, f5
0x825A80D8:  fadds        f28, f1, f2
0x825A80DC:  fsubs        f2, f2, f1
0x825A80E0:  fadds        f30, f7, f26
0x825A80E4:  fsubs        f7, f26, f7
0x825A80E8:  fmuls        f25, f27, f13
0x825A80EC:  fmuls        f26, f28, f13
0x825A80F0:  fmadds       f31, f28, f12, f25
0x825A80F4:  lfs          f25, 0x14(r3)
0x825A80F8:  fmuls        f28, f2, f12
0x825A80FC:  fmuls        f2, f2, f13
0x825A8100:  fmsubs       f1, f27, f12, f26
0x825A8104:  lfs          f27, 0x10(r3)
0x825A8108:  lfs          f26, 0x54(r3)
0x825A810C:  fmsubs       f28, f3, f13, f28
0x825A8110:  fmadds       f3, f3, f12, f2
0x825A8114:  lfs          f2, 0x50(r3)
0x825A8118:  fadds        f14, f27, f2
0x825A811C:  fsubs        f27, f27, f2
0x825A8120:  stfs         f27, -0xbc(r1)
0x825A8124:  fadds        f2, f25, f26
0x825A8128:  fsubs        f26, f25, f26
0x825A812C:  stfs         f26, -0xc0(r1)
0x825A8130:  fadds        f26, f21, f22
0x825A8134:  fsubs        f22, f21, f22
0x825A8138:  fadds        f25, f19, f20
0x825A813C:  fsubs        f21, f19, f20
0x825A8140:  fadds        f20, f26, f14
0x825A8144:  stfs         f20, -0xa8(r1)
0x825A8148:  fsubs        f26, f14, f26
0x825A814C:  stfs         f26, -0xac(r1)
0x825A8150:  fadds        f26, f25, f2
0x825A8154:  stfs         f26, -0xa4(r1)
0x825A8158:  fsubs        f2, f2, f25
0x825A815C:  stfs         f2, -0xb0(r1)
0x825A8160:  lfs          f2, -0xc0(r1)
0x825A8164:  fadds        f19, f22, f2
0x825A8168:  lfs          f2, 0x78(r3)
0x825A816C:  lfs          f25, 0x3c(r3)
0x825A8170:  fsubs        f20, f27, f21
0x825A8174:  lfs          f27, 0x38(r3)
0x825A8178:  fadds        f26, f27, f2
0x825A817C:  stfs         f26, -0xb8(r1)
0x825A8180:  lfs          f26, 0x7c(r3)
0x825A8184:  fsubs        f2, f27, f2
0x825A8188:  fadds        f14, f25, f26
0x825A818C:  stfs         f14, -0xb4(r1)
0x825A8190:  fsubs        f26, f25, f26
0x825A8194:  fsubs        f25, f15, f16
0x825A8198:  fsubs        f14, f20, f19
0x825A819C:  fadds        f19, f19, f20
0x825A81A0:  lfs          f20, -0xbc(r1)
0x825A81A4:  fadds        f21, f21, f20
0x825A81A8:  lfs          f20, -0xc0(r1)
0x825A81AC:  fsubs        f22, f20, f22
0x825A81B0:  fsubs        f20, f17, f18
0x825A81B4:  fmuls        f27, f14, f0
0x825A81B8:  fmuls        f19, f19, f0
0x825A81BC:  fadds        f14, f22, f21
0x825A81C0:  fsubs        f22, f22, f21
0x825A81C4:  stfs         f22, -0xbc(r1)
0x825A81C8:  fadds        f22, f17, f18
0x825A81CC:  fadds        f21, f15, f16
0x825A81D0:  fmuls        f18, f14, f0
0x825A81D4:  lfs          f15, -0xb8(r1)
0x825A81D8:  fadds        f16, f15, f22
0x825A81DC:  fsubs        f22, f22, f15
0x825A81E0:  stfs         f22, -0xb8(r1)
0x825A81E4:  lfs          f22, -0xb4(r1)
0x825A81E8:  fadds        f15, f22, f21
0x825A81EC:  fsubs        f22, f21, f22
0x825A81F0:  stfs         f22, -0xb4(r1)
0x825A81F4:  fadds        f21, f2, f25
0x825A81F8:  fsubs        f22, f20, f26
0x825A81FC:  fadds        f26, f26, f20
0x825A8200:  fsubs        f2, f25, f2
0x825A8204:  fmr          f20, f26
0x825A8208:  lfs          f14, -0xbc(r1)
0x825A820C:  fmuls        f17, f14, f0
0x825A8210:  stfs         f15, -0xbc(r1)
0x825A8214:  fmuls        f14, f21, f12
0x825A8218:  fmuls        f21, f21, f13
0x825A821C:  fmr          f15, f22
0x825A8220:  fadds        f25, f17, f9
0x825A8224:  fmsubs       f22, f22, f13, f14
0x825A8228:  fmadds       f21, f15, f12, f21
0x825A822C:  fmuls        f15, f26, f13
0x825A8230:  fsubs        f26, f10, f18
0x825A8234:  fadds        f10, f18, f10
0x825A8238:  fmuls        f18, f2, f13
0x825A823C:  fsubs        f13, f9, f17
0x825A8240:  fmsubs       f9, f20, f12, f18
0x825A8244:  fmadds       f12, f2, f12, f15
0x825A8248:  fsubs        f2, f28, f9
0x825A824C:  fsubs        f20, f3, f12
0x825A8250:  fadds        f12, f12, f3
0x825A8254:  fadds        f9, f9, f28
0x825A8258:  fadds        f3, f2, f26
0x825A825C:  stfs         f3, 0x60(r3)
0x825A8260:  fadds        f3, f20, f13
0x825A8264:  stfs         f3, 0x64(r3)
0x825A8268:  fsubs        f13, f13, f20
0x825A826C:  stfs         f13, 0x6c(r3)
0x825A8270:  fsubs        f2, f26, f2
0x825A8274:  stfs         f2, 0x68(r3)
0x825A8278:  fadds        f13, f27, f24
0x825A827C:  fadds        f3, f19, f23
0x825A8280:  fadds        f2, f22, f1
0x825A8284:  fadds        f28, f21, f31
0x825A8288:  fsubs        f26, f10, f12
0x825A828C:  stfs         f26, 0x70(r3)
0x825A8290:  fadds        f12, f12, f10
0x825A8294:  stfs         f12, 0x78(r3)
0x825A8298:  fsubs        f10, f25, f9
0x825A829C:  stfs         f10, 0x7c(r3)
0x825A82A0:  fadds        f26, f9, f25
0x825A82A4:  stfs         f26, 0x74(r3)
0x825A82A8:  fsubs        f9, f1, f22
0x825A82AC:  fsubs        f1, f31, f21
0x825A82B0:  fsubs        f10, f23, f19
0x825A82B4:  fsubs        f12, f24, f27
0x825A82B8:  fadds        f31, f2, f13
0x825A82BC:  stfs         f31, 0x40(r3)
0x825A82C0:  fsubs        f2, f13, f2
0x825A82C4:  stfs         f2, 0x48(r3)
0x825A82C8:  fadds        f31, f28, f3
0x825A82CC:  stfs         f31, 0x44(r3)
0x825A82D0:  fsubs        f13, f3, f28
0x825A82D4:  lfs          f31, -0xb8(r1)
0x825A82D8:  lfs          f28, -0xb4(r1)
0x825A82DC:  fadds        f3, f31, f5
0x825A82E0:  stfs         f13, 0x4c(r3)
0x825A82E4:  fsubs        f13, f7, f28
0x825A82E8:  fadds        f2, f9, f10
0x825A82EC:  stfs         f2, 0x54(r3)
0x825A82F0:  fsubs        f2, f12, f1
0x825A82F4:  stfs         f2, 0x50(r3)
0x825A82F8:  fadds        f1, f1, f12
0x825A82FC:  stfs         f1, 0x58(r3)
0x825A8300:  fsubs        f12, f10, f9
0x825A8304:  lfs          f1, -0xb0(r1)
0x825A8308:  fsubs        f9, f5, f31
0x825A830C:  lfs          f2, -0xac(r1)
0x825A8310:  stfs         f12, 0x5c(r3)
0x825A8314:  fadds        f12, f1, f8
0x825A8318:  fsubs        f10, f6, f2
0x825A831C:  fsubs        f27, f13, f3
0x825A8320:  fadds        f3, f3, f13
0x825A8324:  fadds        f13, f28, f7
0x825A8328:  fmuls        f7, f27, f0
0x825A832C:  fmuls        f5, f3, f0
0x825A8330:  fsubs        f3, f13, f9
0x825A8334:  fadds        f31, f9, f13
0x825A8338:  fsubs        f13, f8, f1
0x825A833C:  fadds        f9, f2, f6
0x825A8340:  fmuls        f8, f3, f0
0x825A8344:  fmuls        f0, f31, f0
0x825A8348:  fadds        f2, f7, f13
0x825A834C:  stfs         f2, 0x20(r3)
0x825A8350:  fadds        f1, f5, f9
0x825A8354:  stfs         f1, 0x24(r3)
0x825A8358:  fsubs        f13, f13, f7
0x825A835C:  stfs         f13, 0x28(r3)
0x825A8360:  fsubs        f9, f9, f5
0x825A8364:  stfs         f9, 0x2c(r3)
0x825A8368:  fadds        f6, f8, f10
0x825A836C:  stfs         f6, 0x34(r3)
0x825A8370:  fsubs        f7, f12, f0
0x825A8374:  stfs         f7, 0x30(r3)
0x825A8378:  fadds        f5, f0, f12
0x825A837C:  lfs          f12, -0xa8(r1)
0x825A8380:  fsubs        f3, f10, f8
0x825A8384:  lfs          f10, -0xa4(r1)
0x825A8388:  fadds        f0, f12, f11
0x825A838C:  lfs          f7, -0xbc(r1)
0x825A8390:  fsubs        f12, f11, f12
0x825A8394:  stfs         f5, 0x38(r3)
0x825A8398:  fadds        f13, f10, f4
0x825A839C:  stfs         f3, 0x3c(r3)
0x825A83A0:  fsubs        f11, f4, f10
0x825A83A4:  fadds        f9, f7, f29
0x825A83A8:  fadds        f10, f16, f30
0x825A83AC:  fsubs        f8, f30, f16
0x825A83B0:  fsubs        f7, f29, f7
0x825A83B4:  fadds        f1, f9, f13
0x825A83B8:  stfs         f1, 4(r3)
0x825A83BC:  fadds        f2, f10, f0
0x825A83C0:  stfs         f2, 0(r3)
0x825A83C4:  fsubs        f0, f0, f10
0x825A83C8:  stfs         f0, 8(r3)
0x825A83CC:  fsubs        f13, f13, f9
0x825A83D0:  stfs         f13, 0xc(r3)
0x825A83D4:  fsubs        f10, f12, f7
0x825A83D8:  stfs         f10, 0x10(r3)
0x825A83DC:  fadds        f9, f8, f11
0x825A83E0:  stfs         f9, 0x14(r3)
0x825A83E4:  fadds        f7, f7, f12
0x825A83E8:  stfs         f7, 0x18(r3)
0x825A83EC:  fsubs        f6, f11, f8
0x825A83F0:  stfs         f6, 0x1c(r3)
0x825A83F4:  addi         r12, r1, -8
0x825A83F8:  bl           0x8243662c
0x825A83FC:  lwz          r12, -8(r1)
0x825A8400:  mtlr         r12
0x825A8404:  blr          
0x825A8408:  mflr         r12
0x825A840C:  stw          r12, -8(r1)
0x825A8410:  addi         r12, r1, -8
0x825A8414:  bl           0x824365e0
0x825A8418:  lfs          f5, 0x64(r3)
0x825A841C:  lfs          f4, 0x60(r3)
0x825A8420:  lfs          f3, 0x24(r3)
0x825A8424:  lfs          f6, 0x20(r3)
0x825A8428:  fadds        f21, f3, f4
0x825A842C:  fsubs        f22, f6, f5
0x825A8430:  lfs          f0, 4(r4)
0x825A8434:  fadds        f6, f5, f6
0x825A8438:  lfs          f28, 0x40(r3)
0x825A843C:  fsubs        f5, f3, f4
0x825A8440:  lfs          f27, 4(r3)
0x825A8444:  fadds        f19, f27, f28
0x825A8448:  lfs          f2, 8(r3)
0x825A844C:  lfs          f1, 0x4c(r3)
0x825A8450:  lfs          f31, 0x48(r3)
0x825A8454:  lfs          f30, 0xc(r3)
0x825A8458:  lfs          f29, 0x44(r3)
0x825A845C:  lfs          f7, 0(r3)
0x825A8460:  fsubs        f20, f7, f29
0x825A8464:  lfs          f12, 0x14(r4)
0x825A8468:  fsubs        f18, f22, f21
0x825A846C:  lfs          f13, 0x10(r4)
0x825A8470:  fadds        f22, f21, f22
0x825A8474:  lfs          f25, 0x6c(r3)
0x825A8478:  fadds        f17, f5, f6
0x825A847C:  lfs          f23, 0x2c(r3)
0x825A8480:  fadds        f7, f29, f7
0x825A8484:  lfs          f26, 0x28(r3)
0x825A8488:  fsubs        f29, f27, f28
0x825A848C:  lfs          f24, 0x68(r3)
0x825A8490:  lfs          f11, 0x18(r4)
0x825A8494:  lfs          f10, 0x1c(r4)
0x825A8498:  lfs          f9, 0x20(r4)
0x825A849C:  lfs          f8, 0x24(r4)
0x825A84A0:  fmuls        f4, f18, f0
0x825A84A4:  fsubs        f18, f6, f5
0x825A84A8:  fmuls        f3, f22, f0
0x825A84AC:  fsubs        f6, f2, f1
0x825A84B0:  fadds        f5, f30, f31
0x825A84B4:  fmuls        f27, f17, f0
0x825A84B8:  fadds        f22, f4, f20
0x825A84BC:  fmuls        f28, f18, f0
0x825A84C0:  fadds        f21, f3, f19
0x825A84C4:  fsubs        f3, f19, f3
0x825A84C8:  fmr          f19, f6
0x825A84CC:  fmuls        f18, f5, f12
0x825A84D0:  fmr          f17, f6
0x825A84D4:  fmuls        f16, f5, f13
0x825A84D8:  fadds        f5, f23, f24
0x825A84DC:  fsubs        f6, f26, f25
0x825A84E0:  fsubs        f4, f20, f4
0x825A84E4:  fadds        f20, f28, f29
0x825A84E8:  fsubs        f29, f29, f28
0x825A84EC:  fsubs        f28, f7, f27
0x825A84F0:  fadds        f7, f27, f7
0x825A84F4:  fmsubs       f27, f19, f13, f18
0x825A84F8:  fadds        f26, f25, f26
0x825A84FC:  lfs          f25, 0x30(r3)
0x825A8500:  fmadds       f19, f17, f12, f16
0x825A8504:  fmuls        f18, f5, f11
0x825A8508:  fmr          f17, f5
0x825A850C:  fmuls        f16, f6, f11
0x825A8510:  fadds        f5, f1, f2
0x825A8514:  fsubs        f2, f30, f31
0x825A8518:  lfs          f30, 0x10(r3)
0x825A851C:  fsubs        f24, f23, f24
0x825A8520:  lfs          f23, 0x74(r3)
0x825A8524:  fmsubs       f6, f6, f10, f18
0x825A8528:  fmadds       f1, f17, f10, f16
0x825A852C:  fmr          f17, f5
0x825A8530:  fmuls        f16, f2, f10
0x825A8534:  fmr          f15, f5
0x825A8538:  lfs          f5, 0x50(r3)
0x825A853C:  fmuls        f14, f2, f11
0x825A8540:  lfs          f2, 0x14(r3)
0x825A8544:  fadds        f31, f6, f27
0x825A8548:  fsubs        f6, f27, f6
0x825A854C:  lfs          f27, 0x54(r3)
0x825A8550:  fadds        f18, f1, f19
0x825A8554:  stfs         f18, -0xc0(r1)
0x825A8558:  lfs          f18, 0x34(r3)
0x825A855C:  fmsubs       f17, f17, f11, f16
0x825A8560:  stfs         f17, -0xcc(r1)
0x825A8564:  fsubs        f19, f19, f1
0x825A8568:  fmadds       f1, f15, f10, f14
0x825A856C:  stfs         f1, -0xd0(r1)
0x825A8570:  fmuls        f16, f26, f13
0x825A8574:  fmr          f15, f24
0x825A8578:  fmuls        f14, f26, f12
0x825A857C:  fsubs        f1, f30, f27
0x825A8580:  fadds        f26, f2, f5
0x825A8584:  fsubs        f5, f2, f5
0x825A8588:  fadds        f30, f27, f30
0x825A858C:  fmadds       f24, f24, f12, f16
0x825A8590:  fmsubs       f16, f15, f13, f14
0x825A8594:  stfs         f16, -0xc8(r1)
0x825A8598:  fmr          f14, f1
0x825A859C:  fmr          f16, f1
0x825A85A0:  fmuls        f1, f26, f9
0x825A85A4:  stfs         f1, -0xc4(r1)
0x825A85A8:  fmuls        f15, f26, f8
0x825A85AC:  lfs          f1, 0x70(r3)
0x825A85B0:  fsubs        f26, f17, f24
0x825A85B4:  stfs         f26, -0xb4(r1)
0x825A85B8:  fmr          f26, f17
0x825A85BC:  fmsubs       f16, f16, f9, f15
0x825A85C0:  fadds        f26, f24, f26
0x825A85C4:  stfs         f26, -0xbc(r1)
0x825A85C8:  lfs          f24, -0xd0(r1)
0x825A85CC:  lfs          f26, -0xc8(r1)
0x825A85D0:  fsubs        f24, f24, f26
0x825A85D4:  stfs         f24, -0xb0(r1)
0x825A85D8:  lfs          f24, -0xd0(r1)
0x825A85DC:  lfs          f15, -0xc4(r1)
0x825A85E0:  fadds        f17, f26, f24
0x825A85E4:  fmadds       f26, f14, f8, f15
0x825A85E8:  stfs         f26, -0xd0(r1)
0x825A85EC:  fsubs        f26, f25, f23
0x825A85F0:  fadds        f24, f18, f1
0x825A85F4:  fsubs        f1, f18, f1
0x825A85F8:  lfs          f18, 0x1c(r3)
0x825A85FC:  fmuls        f14, f26, f9
0x825A8600:  fmuls        f15, f24, f9
0x825A8604:  fmadds       f27, f24, f8, f14
0x825A8608:  fmsubs       f2, f26, f8, f15
0x825A860C:  fmuls        f15, f5, f9
0x825A8610:  fmr          f14, f5
0x825A8614:  fmuls        f5, f30, f9
0x825A8618:  stfs         f5, -0xc4(r1)
0x825A861C:  fadds        f5, f23, f25
0x825A8620:  fmr          f24, f30
0x825A8624:  fmuls        f23, f1, f8
0x825A8628:  fmuls        f1, f1, f9
0x825A862C:  fadds        f30, f2, f16
0x825A8630:  fsubs        f2, f16, f2
0x825A8634:  lfs          f16, 0x38(r3)
0x825A8638:  fmsubs       f9, f5, f9, f23
0x825A863C:  lfs          f23, 0x58(r3)
0x825A8640:  lfs          f25, -0xd0(r1)
0x825A8644:  fadds        f26, f27, f25
0x825A8648:  fsubs        f27, f25, f27
0x825A864C:  fmsubs       f25, f24, f8, f15
0x825A8650:  lfs          f15, 0x78(r3)
0x825A8654:  lfs          f24, -0xc4(r1)
0x825A8658:  fmadds       f24, f14, f8, f24
0x825A865C:  fmadds       f8, f5, f8, f1
0x825A8660:  lfs          f5, 0x18(r3)
0x825A8664:  fsubs        f14, f25, f9
0x825A8668:  lfs          f1, 0x5c(r3)
0x825A866C:  fadds        f9, f9, f25
0x825A8670:  stfs         f9, -0xa8(r1)
0x825A8674:  stfs         f14, -0xc8(r1)
0x825A8678:  fsubs        f9, f24, f8
0x825A867C:  stfs         f9, -0xb8(r1)
0x825A8680:  fadds        f8, f8, f24
0x825A8684:  stfs         f8, -0xac(r1)
0x825A8688:  fsubs        f9, f5, f1
0x825A868C:  fadds        f8, f18, f23
0x825A8690:  fmr          f24, f9
0x825A8694:  fmr          f14, f9
0x825A8698:  fmuls        f9, f8, f10
0x825A869C:  fmuls        f8, f8, f11
0x825A86A0:  stfs         f9, -0xc4(r1)
0x825A86A4:  fmadds       f8, f14, f10, f8
0x825A86A8:  lfs          f9, 0x3c(r3)
0x825A86AC:  fadds        f25, f9, f15
0x825A86B0:  stfs         f8, -0xd0(r1)
0x825A86B4:  fmuls        f14, f25, f13
0x825A86B8:  lfs          f9, -0xc4(r1)
0x825A86BC:  fmsubs       f9, f24, f11, f9
0x825A86C0:  stfs         f9, -0xcc(r1)
0x825A86C4:  lfs          f9, 0x7c(r3)
0x825A86C8:  fsubs        f8, f16, f9
0x825A86CC:  fmr          f24, f8
0x825A86D0:  fmuls        f8, f8, f13
0x825A86D4:  stfs         f8, -0xc4(r1)
0x825A86D8:  fadds        f8, f1, f5
0x825A86DC:  fsubs        f5, f18, f23
0x825A86E0:  fmsubs       f1, f24, f12, f14
0x825A86E4:  fmr          f23, f8
0x825A86E8:  fmuls        f18, f5, f13
0x825A86EC:  fmuls        f13, f8, f13
0x825A86F0:  fmr          f14, f5
0x825A86F4:  lfs          f5, -0xcc(r1)
0x825A86F8:  fadds        f8, f1, f5
0x825A86FC:  fsubs        f5, f5, f1
0x825A8700:  lfs          f24, -0xc4(r1)
0x825A8704:  stfs         f13, -0xc4(r1)
0x825A8708:  fmadds       f25, f25, f12, f24
0x825A870C:  fadds        f13, f9, f16
0x825A8710:  lfs          f9, 0x3c(r3)
0x825A8714:  fsubs        f9, f9, f15
0x825A8718:  lfs          f24, -0xd0(r1)
0x825A871C:  fadds        f1, f25, f24
0x825A8720:  fsubs        f25, f24, f25
0x825A8724:  fmadds       f24, f23, f12, f18
0x825A8728:  fsubs        f16, f6, f25
0x825A872C:  fadds        f6, f25, f6
0x825A8730:  lfs          f23, -0xc4(r1)
0x825A8734:  fmsubs       f12, f14, f12, f23
0x825A8738:  lfs          f14, -0xc0(r1)
0x825A873C:  fmuls        f23, f9, f11
0x825A8740:  fmuls        f11, f13, f11
0x825A8744:  fadds        f18, f1, f14
0x825A8748:  fsubs        f1, f14, f1
0x825A874C:  fmsubs       f13, f13, f10, f23
0x825A8750:  fmadds       f11, f9, f10, f11
0x825A8754:  fadds        f9, f30, f22
0x825A8758:  fadds        f10, f13, f24
0x825A875C:  fsubs        f13, f24, f13
0x825A8760:  fadds        f24, f8, f31
0x825A8764:  fadds        f23, f11, f12
0x825A8768:  fsubs        f12, f12, f11
0x825A876C:  fadds        f11, f26, f21
0x825A8770:  fsubs        f8, f31, f8
0x825A8774:  fadds        f15, f24, f9
0x825A8778:  stfs         f15, 0(r3)
0x825A877C:  fadds        f15, f5, f19
0x825A8780:  fsubs        f9, f9, f24
0x825A8784:  stfs         f9, 8(r3)
0x825A8788:  fadds        f9, f18, f11
0x825A878C:  stfs         f9, 4(r3)
0x825A8790:  fsubs        f11, f11, f18
0x825A8794:  stfs         f11, 0xc(r3)
0x825A8798:  fsubs        f9, f21, f26
0x825A879C:  fsubs        f11, f22, f30
0x825A87A0:  fsubs        f5, f19, f5
0x825A87A4:  fadds        f30, f15, f16
0x825A87A8:  fsubs        f31, f16, f15
0x825A87AC:  fadds        f26, f8, f9
0x825A87B0:  stfs         f26, 0x14(r3)
0x825A87B4:  fsubs        f9, f9, f8
0x825A87B8:  stfs         f9, 0x1c(r3)
0x825A87BC:  fsubs        f8, f11, f1
0x825A87C0:  stfs         f8, 0x10(r3)
0x825A87C4:  fadds        f1, f1, f11
0x825A87C8:  stfs         f1, 0x18(r3)
0x825A87CC:  fsubs        f11, f4, f27
0x825A87D0:  fadds        f9, f2, f3
0x825A87D4:  fmuls        f1, f30, f0
0x825A87D8:  fmuls        f8, f31, f0
0x825A87DC:  fsubs        f30, f6, f5
0x825A87E0:  fadds        f26, f5, f6
0x825A87E4:  fadds        f25, f8, f11
0x825A87E8:  lfs          f31, -0xbc(r1)
0x825A87EC:  fsubs        f11, f11, f8
0x825A87F0:  stfs         f11, 0x28(r3)
0x825A87F4:  fadds        f8, f1, f9
0x825A87F8:  stfs         f8, 0x24(r3)
0x825A87FC:  fsubs        f1, f9, f1
0x825A8800:  stfs         f1, 0x2c(r3)
0x825A8804:  fadds        f11, f27, f4
0x825A8808:  stfs         f25, 0x20(r3)
0x825A880C:  fsubs        f9, f3, f2
0x825A8810:  fmuls        f8, f30, f0
0x825A8814:  lfs          f30, -0xb0(r1)
0x825A8818:  fmuls        f4, f26, f0
0x825A881C:  fsubs        f5, f17, f13
0x825A8820:  fadds        f6, f12, f31
0x825A8824:  fsubs        f12, f31, f12
0x825A8828:  fadds        f13, f13, f17
0x825A882C:  fadds        f3, f8, f9
0x825A8830:  stfs         f3, 0x34(r3)
0x825A8834:  fsubs        f1, f11, f4
0x825A8838:  stfs         f1, 0x30(r3)
0x825A883C:  fsubs        f2, f9, f8
0x825A8840:  lfs          f3, -0xc8(r1)
0x825A8844:  fadds        f11, f4, f11
0x825A8848:  lfs          f1, -0xb4(r1)
0x825A884C:  stfs         f11, 0x38(r3)
0x825A8850:  fsubs        f8, f1, f10
0x825A8854:  stfs         f2, 0x3c(r3)
0x825A8858:  fadds        f11, f3, f28
0x825A885C:  lfs          f2, -0xb8(r1)
0x825A8860:  fsubs        f4, f30, f23
0x825A8864:  fadds        f9, f2, f20
0x825A8868:  fadds        f10, f10, f1
0x825A886C:  fsubs        f27, f6, f5
0x825A8870:  fadds        f6, f5, f6
0x825A8874:  fadds        f26, f8, f11
0x825A8878:  stfs         f26, 0x40(r3)
0x825A887C:  fsubs        f8, f11, f8
0x825A8880:  stfs         f8, 0x48(r3)
0x825A8884:  fadds        f11, f4, f9
0x825A8888:  stfs         f11, 0x44(r3)
0x825A888C:  fsubs        f9, f9, f4
0x825A8890:  stfs         f9, 0x4c(r3)
0x825A8894:  fsubs        f9, f20, f2
0x825A8898:  fsubs        f11, f28, f3
0x825A889C:  fadds        f8, f23, f30
0x825A88A0:  fadds        f4, f10, f9
0x825A88A4:  stfs         f4, 0x54(r3)
0x825A88A8:  lfs          f4, -0xac(r1)
0x825A88AC:  fsubs        f3, f9, f10
0x825A88B0:  fsubs        f2, f11, f8
0x825A88B4:  stfs         f3, 0x5c(r3)
0x825A88B8:  fadds        f1, f8, f11
0x825A88BC:  lfs          f8, -0xa8(r1)
0x825A88C0:  fmuls        f9, f27, f0
0x825A88C4:  stfs         f2, 0x50(r3)
0x825A88C8:  fsubs        f11, f7, f4
0x825A88CC:  stfs         f1, 0x58(r3)
0x825A88D0:  fadds        f10, f8, f29
0x825A88D4:  fsubs        f2, f12, f13
0x825A88D8:  fadds        f1, f13, f12
0x825A88DC:  fadds        f5, f9, f11
0x825A88E0:  stfs         f5, 0x60(r3)
0x825A88E4:  fsubs        f3, f11, f9
0x825A88E8:  stfs         f3, 0x68(r3)
0x825A88EC:  fmuls        f9, f6, f0
0x825A88F0:  fadds        f11, f4, f7
0x825A88F4:  fsubs        f12, f10, f9
0x825A88F8:  stfs         f12, 0x6c(r3)
0x825A88FC:  fadds        f13, f9, f10
0x825A8900:  stfs         f13, 0x64(r3)
0x825A8904:  fmuls        f12, f2, f0
0x825A8908:  fsubs        f13, f29, f8
0x825A890C:  fmuls        f0, f1, f0
0x825A8910:  fadds        f10, f12, f13
0x825A8914:  stfs         f10, 0x74(r3)
0x825A8918:  fsubs        f9, f11, f0
0x825A891C:  stfs         f9, 0x70(r3)
0x825A8920:  fadds        f8, f0, f11
0x825A8924:  stfs         f8, 0x78(r3)
0x825A8928:  fsubs        f7, f13, f12
0x825A892C:  stfs         f7, 0x7c(r3)
0x825A8930:  addi         r12, r1, -8
0x825A8934:  bl           0x8243662c
0x825A8938:  lwz          r12, -8(r1)
0x825A893C:  mtlr         r12
0x825A8940:  blr          
0x825A8944:  .align 0                       # padding
0x825A8948:  mflr         r12
0x825A894C:  stw          r12, -8(r1)
0x825A8950:  addi         r12, r1, -8
0x825A8954:  bl           0x82436608
0x825A8958:  lfs          f12, 0x20(r3)
0x825A895C:  lfs          f13, 0(r3)
0x825A8960:  lfs          f10, 4(r3)
0x825A8964:  fadds        f28, f13, f12
0x825A8968:  lfs          f11, 0x24(r3)
0x825A896C:  fsubs        f13, f13, f12
0x825A8970:  lfs          f9, 0x30(r3)
0x825A8974:  fadds        f12, f10, f11
0x825A8978:  lfs          f8, 0x10(r3)
0x825A897C:  fsubs        f11, f10, f11
0x825A8980:  fadds        f10, f8, f9
0x825A8984:  lfs          f7, 0x34(r3)
0x825A8988:  lfs          f6, 0x14(r3)
0x825A898C:  fsubs        f9, f8, f9
0x825A8990:  fadds        f8, f6, f7
0x825A8994:  lfs          f5, 0x28(r3)
0x825A8998:  fsubs        f7, f6, f7
0x825A899C:  lfs          f3, 8(r3)
0x825A89A0:  lfs          f4, 0x2c(r3)
0x825A89A4:  lfs          f2, 0xc(r3)
0x825A89A8:  lfs          f1, 0x38(r3)
0x825A89AC:  fadds        f26, f2, f4
0x825A89B0:  lfs          f30, 0x3c(r3)
0x825A89B4:  lfs          f31, 0x18(r3)
0x825A89B8:  lfs          f29, 0x1c(r3)
0x825A89BC:  fadds        f25, f31, f1
0x825A89C0:  fadds        f6, f10, f28
0x825A89C4:  lfs          f0, 4(r4)
0x825A89C8:  fsubs        f10, f28, f10
0x825A89CC:  fadds        f28, f8, f12
0x825A89D0:  fsubs        f12, f12, f8
0x825A89D4:  fadds        f27, f9, f11
0x825A89D8:  fsubs        f8, f13, f7
0x825A89DC:  fsubs        f11, f11, f9
0x825A89E0:  fadds        f13, f7, f13
0x825A89E4:  fadds        f7, f3, f5
0x825A89E8:  fsubs        f9, f3, f5
0x825A89EC:  fsubs        f5, f2, f4
0x825A89F0:  fsubs        f3, f29, f30
0x825A89F4:  fsubs        f4, f31, f1
0x825A89F8:  fadds        f24, f29, f30
0x825A89FC:  fadds        f2, f25, f7
0x825A8A00:  fsubs        f7, f7, f25
0x825A8A04:  fsubs        f29, f9, f3
0x825A8A08:  fadds        f30, f4, f5
0x825A8A0C:  fadds        f9, f3, f9
0x825A8A10:  fsubs        f5, f5, f4
0x825A8A14:  fadds        f1, f24, f26
0x825A8A18:  fsubs        f31, f26, f24
0x825A8A1C:  fsubs        f4, f29, f30
0x825A8A20:  fadds        f3, f30, f29
0x825A8A24:  fadds        f30, f5, f9
0x825A8A28:  fsubs        f29, f9, f5
0x825A8A2C:  fmuls        f9, f4, f0
0x825A8A30:  fmuls        f5, f3, f0
0x825A8A34:  fmuls        f4, f30, f0
0x825A8A38:  fmuls        f0, f29, f0
0x825A8A3C:  fadds        f3, f9, f8
0x825A8A40:  stfs         f3, 0x20(r3)
0x825A8A44:  fadds        f3, f5, f27
0x825A8A48:  stfs         f3, 0x24(r3)
0x825A8A4C:  fsubs        f9, f8, f9
0x825A8A50:  stfs         f9, 0x28(r3)
0x825A8A54:  fsubs        f8, f27, f5
0x825A8A58:  stfs         f8, 0x2c(r3)
0x825A8A5C:  fsubs        f5, f13, f4
0x825A8A60:  stfs         f5, 0x30(r3)
0x825A8A64:  fadds        f3, f0, f11
0x825A8A68:  stfs         f3, 0x34(r3)
0x825A8A6C:  fadds        f9, f2, f6
0x825A8A70:  stfs         f9, 0(r3)
0x825A8A74:  fadds        f13, f4, f13
0x825A8A78:  stfs         f13, 0x38(r3)
0x825A8A7C:  fsubs        f11, f11, f0
0x825A8A80:  stfs         f11, 0x3c(r3)
0x825A8A84:  fadds        f8, f1, f28
0x825A8A88:  stfs         f8, 4(r3)
0x825A8A8C:  fsubs        f6, f6, f2
0x825A8A90:  stfs         f6, 8(r3)
0x825A8A94:  fsubs        f5, f28, f1
0x825A8A98:  fsubs        f4, f10, f31
0x825A8A9C:  fadds        f3, f7, f12
0x825A8AA0:  stfs         f5, 0xc(r3)
0x825A8AA4:  fadds        f2, f31, f10
0x825A8AA8:  stfs         f4, 0x10(r3)
0x825A8AAC:  fsubs        f1, f12, f7
0x825A8AB0:  stfs         f3, 0x14(r3)
0x825A8AB4:  stfs         f2, 0x18(r3)
0x825A8AB8:  stfs         f1, 0x1c(r3)
0x825A8ABC:  addi         r12, r1, -8
0x825A8AC0:  bl           0x82436654
0x825A8AC4:  lwz          r12, -8(r1)
0x825A8AC8:  mtlr         r12
0x825A8ACC:  blr          
0x825A8AD0:  mflr         r12
0x825A8AD4:  stw          r12, -8(r1)
0x825A8AD8:  addi         r12, r1, -8
0x825A8ADC:  bl           0x824365fc
0x825A8AE0:  lfs          f9, 0x34(r3)
0x825A8AE4:  lfs          f8, 0x30(r3)
0x825A8AE8:  lfs          f7, 0x14(r3)
0x825A8AEC:  lfs          f10, 0x10(r3)
0x825A8AF0:  fadds        f25, f7, f8
0x825A8AF4:  fsubs        f26, f10, f9
0x825A8AF8:  lfs          f11, 0(r3)
0x825A8AFC:  lfs          f29, 0x24(r3)
0x825A8B00:  fadds        f10, f9, f10
0x825A8B04:  lfs          f27, 4(r3)
0x825A8B08:  fsubs        f24, f11, f29
0x825A8B0C:  lfs          f28, 0x20(r3)
0x825A8B10:  fadds        f11, f29, f11
0x825A8B14:  fadds        f29, f27, f28
0x825A8B18:  lfs          f12, 4(r4)
0x825A8B1C:  fsubs        f28, f27, f28
0x825A8B20:  lfs          f6, 8(r3)
0x825A8B24:  fsubs        f9, f7, f8
0x825A8B28:  lfs          f3, 0xc(r3)
0x825A8B2C:  lfs          f5, 0x2c(r3)
0x825A8B30:  lfs          f4, 0x28(r3)
0x825A8B34:  lfs          f0, 0x10(r4)
0x825A8B38:  fsubs        f27, f26, f25
0x825A8B3C:  lfs          f13, 0x14(r4)
0x825A8B40:  fadds        f26, f25, f26
0x825A8B44:  lfs          f2, 0x18(r3)
0x825A8B48:  lfs          f1, 0x3c(r3)
0x825A8B4C:  lfs          f31, 0x38(r3)
0x825A8B50:  lfs          f30, 0x1c(r3)
0x825A8B54:  fmuls        f8, f27, f12
0x825A8B58:  fsubs        f27, f10, f9
0x825A8B5C:  fmuls        f7, f26, f12
0x825A8B60:  fadds        f26, f9, f10
0x825A8B64:  fadds        f9, f3, f4
0x825A8B68:  fsubs        f10, f6, f5
0x825A8B6C:  fmuls        f27, f27, f12
0x825A8B70:  fmuls        f12, f26, f12
0x825A8B74:  fmuls        f23, f9, f13
0x825A8B78:  fmr          f26, f10
0x825A8B7C:  fmr          f25, f10
0x825A8B80:  fmuls        f22, f9, f0
0x825A8B84:  fsubs        f9, f3, f4
0x825A8B88:  fadds        f10, f5, f6
0x825A8B8C:  fmsubs       f6, f26, f0, f23
0x825A8B90:  fmadds       f5, f25, f13, f22
0x825A8B94:  fmuls        f26, f9, f0
0x825A8B98:  fmuls        f3, f10, f0
0x825A8B9C:  fmr          f4, f10
0x825A8BA0:  fmr          f25, f9
0x825A8BA4:  fsubs        f10, f2, f1
0x825A8BA8:  fadds        f9, f30, f31
0x825A8BAC:  fmsubs       f4, f4, f13, f26
0x825A8BB0:  fmadds       f3, f25, f13, f3
0x825A8BB4:  fmr          f26, f10
0x825A8BB8:  fmuls        f23, f9, f0
0x825A8BBC:  fmuls        f25, f10, f0
0x825A8BC0:  fadds        f10, f1, f2
0x825A8BC4:  fsubs        f1, f30, f31
0x825A8BC8:  fmsubs       f2, f26, f13, f23
0x825A8BCC:  fmadds       f9, f9, f13, f25
0x825A8BD0:  fmr          f26, f10
0x825A8BD4:  fmr          f25, f10
0x825A8BD8:  fadds        f10, f8, f24
0x825A8BDC:  fmuls        f23, f1, f13
0x825A8BE0:  fmuls        f22, f1, f0
0x825A8BE4:  fadds        f1, f7, f29
0x825A8BE8:  fsubs        f7, f29, f7
0x825A8BEC:  fadds        f31, f2, f6
0x825A8BF0:  fadds        f30, f9, f5
0x825A8BF4:  fmsubs       f0, f26, f0, f23
0x825A8BF8:  fmadds       f13, f25, f13, f22
0x825A8BFC:  fadds        f21, f31, f10
0x825A8C00:  stfs         f21, 0(r3)
0x825A8C04:  fsubs        f10, f10, f31
0x825A8C08:  stfs         f10, 8(r3)
0x825A8C0C:  fsubs        f10, f24, f8
0x825A8C10:  fsubs        f8, f6, f2
0x825A8C14:  fadds        f6, f30, f1
0x825A8C18:  stfs         f6, 4(r3)
0x825A8C1C:  fsubs        f2, f1, f30
0x825A8C20:  stfs         f2, 0xc(r3)
0x825A8C24:  fsubs        f9, f5, f9
0x825A8C28:  fadds        f1, f8, f7
0x825A8C2C:  stfs         f1, 0x14(r3)
0x825A8C30:  fsubs        f2, f7, f8
0x825A8C34:  stfs         f2, 0x1c(r3)
0x825A8C38:  fsubs        f8, f4, f0
0x825A8C3C:  fsubs        f7, f3, f13
0x825A8C40:  fadds        f0, f0, f4
0x825A8C44:  fadds        f13, f13, f3
0x825A8C48:  fsubs        f6, f10, f9
0x825A8C4C:  stfs         f6, 0x10(r3)
0x825A8C50:  fadds        f5, f9, f10
0x825A8C54:  stfs         f5, 0x18(r3)
0x825A8C58:  fsubs        f10, f11, f12
0x825A8C5C:  fadds        f9, f27, f28
0x825A8C60:  fadds        f12, f12, f11
0x825A8C64:  fsubs        f11, f28, f27
0x825A8C68:  fadds        f1, f8, f10
0x825A8C6C:  stfs         f1, 0x20(r3)
0x825A8C70:  fsubs        f5, f10, f8
0x825A8C74:  stfs         f5, 0x28(r3)
0x825A8C78:  fadds        f6, f7, f9
0x825A8C7C:  stfs         f6, 0x24(r3)
0x825A8C80:  fsubs        f2, f9, f7
0x825A8C84:  stfs         f2, 0x2c(r3)
0x825A8C88:  fadds        f1, f0, f11
0x825A8C8C:  stfs         f1, 0x34(r3)
0x825A8C90:  fsubs        f10, f12, f13
0x825A8C94:  stfs         f10, 0x30(r3)
0x825A8C98:  fadds        f9, f13, f12
0x825A8C9C:  stfs         f9, 0x38(r3)
0x825A8CA0:  fsubs        f8, f11, f0
0x825A8CA4:  stfs         f8, 0x3c(r3)
0x825A8CA8:  addi         r12, r1, -8
0x825A8CAC:  bl           0x82436648
0x825A8CB0:  lwz          r12, -8(r1)
0x825A8CB4:  mtlr         r12
0x825A8CB8:  blr          
0x825A8CBC:  .align 0                       # padding
0x825A8CC0:  srawi        r10, r3, 1
0x825A8CC4:  slwi         r11, r5, 1
0x825A8CC8:  mr           r8, r6
0x825A8CCC:  divw         r11, r11, r10
0x825A8CD0:  cmpwi        cr6, r10, 2
0x825A8CD4:  blelr        cr6
0x825A8CD8:  addi         r9, r3, -2
0x825A8CDC:  neg          r3, r11
0x825A8CE0:  slwi         r6, r9, 2
0x825A8CE4:  slwi         r9, r5, 2
0x825A8CE8:  slwi         r5, r11, 2
0x825A8CEC:  add          r11, r6, r4
0x825A8CF0:  addi         r7, r10, -3
0x825A8CF4:  lis          r6, -0x7e00
0x825A8CF8:  srwi         r7, r7, 1
0x825A8CFC:  addi         r10, r4, 8
0x825A8D00:  slwi         r3, r3, 2
0x825A8D04:  add          r9, r9, r8
0x825A8D08:  lfs          f8, 0x6a40(r6)
0x825A8D0C:  addi         r7, r7, 1
0x825A8D10:  add          r9, r3, r9
0x825A8D14:  lfs          f13, 4(r10)
0x825A8D18:  lfs          f9, 4(r11)
0x825A8D1C:  add          r8, r5, r8
0x825A8D20:  fadds        f9, f9, f13
0x825A8D24:  lfs          f0, 0(r10)
0x825A8D28:  lfs          f12, 0(r11)
0x825A8D2C:  addi         r7, r7, -1
0x825A8D30:  fsubs        f12, f0, f12
0x825A8D34:  lfs          f7, 0(r9)
0x825A8D38:  cmplwi       cr6, r7, 0
0x825A8D3C:  fsubs        f10, f8, f7
0x825A8D40:  lfs          f11, 0(r8)
0x825A8D44:  fmuls        f6, f9, f11
0x825A8D48:  fmuls        f5, f9, f10
0x825A8D4C:  fmsubs       f10, f12, f10, f6
0x825A8D50:  fmadds       f12, f12, f11, f5
0x825A8D54:  fsubs        f4, f0, f10
0x825A8D58:  stfs         f4, 0(r10)
0x825A8D5C:  fsubs        f3, f13, f12
0x825A8D60:  stfs         f3, 4(r10)
0x825A8D64:  lfs          f2, 0(r11)
0x825A8D68:  addi         r10, r10, 8
0x825A8D6C:  lfs          f1, 4(r11)
0x825A8D70:  fadds        f0, f2, f10
0x825A8D74:  fsubs        f13, f1, f12
0x825A8D78:  stfs         f0, 0(r11)
0x825A8D7C:  stfs         f13, 4(r11)
0x825A8D80:  addi         r11, r11, -8
0x825A8D84:  bne          cr6, 0x825a8d10
0x825A8D88:  blr          
0x825A8D8C:  .align 0                       # padding
0x825A8D90:  srawi        r10, r3, 1
0x825A8D94:  slwi         r11, r5, 1
0x825A8D98:  mr           r8, r6
0x825A8D9C:  divw         r11, r11, r10
0x825A8DA0:  cmpwi        cr6, r10, 2
0x825A8DA4:  blelr        cr6
0x825A8DA8:  addi         r9, r3, -2
0x825A8DAC:  neg          r3, r11
0x825A8DB0:  slwi         r6, r9, 2
0x825A8DB4:  slwi         r9, r5, 2
0x825A8DB8:  slwi         r5, r11, 2
0x825A8DBC:  add          r11, r6, r4
0x825A8DC0:  addi         r7, r10, -3
0x825A8DC4:  lis          r6, -0x7e00
0x825A8DC8:  srwi         r7, r7, 1
0x825A8DCC:  addi         r10, r4, 8
0x825A8DD0:  slwi         r3, r3, 2
0x825A8DD4:  add          r9, r9, r8
0x825A8DD8:  lfs          f8, 0x6a40(r6)
0x825A8DDC:  addi         r7, r7, 1
0x825A8DE0:  add          r9, r3, r9
0x825A8DE4:  lfs          f0, 0(r10)
0x825A8DE8:  lfs          f12, 0(r11)
0x825A8DEC:  add          r8, r5, r8
0x825A8DF0:  fsubs        f12, f0, f12
0x825A8DF4:  lfs          f13, 4(r10)
0x825A8DF8:  lfs          f9, 4(r11)
0x825A8DFC:  addi         r7, r7, -1
0x825A8E00:  fadds        f9, f9, f13
0x825A8E04:  lfs          f7, 0(r9)
0x825A8E08:  cmplwi       cr6, r7, 0
0x825A8E0C:  fsubs        f10, f8, f7
0x825A8E10:  lfs          f11, 0(r8)
0x825A8E14:  fmuls        f5, f12, f11
0x825A8E18:  fmuls        f6, f12, f10
0x825A8E1C:  fmadds       f12, f9, f11, f6
0x825A8E20:  fmsubs       f11, f9, f10, f5
0x825A8E24:  fsubs        f4, f0, f12
0x825A8E28:  stfs         f4, 0(r10)
0x825A8E2C:  fsubs        f3, f13, f11
0x825A8E30:  stfs         f3, 4(r10)
0x825A8E34:  lfs          f2, 0(r11)
0x825A8E38:  addi         r10, r10, 8
0x825A8E3C:  lfs          f1, 4(r11)
0x825A8E40:  fadds        f0, f2, f12
0x825A8E44:  fsubs        f13, f1, f11
0x825A8E48:  stfs         f0, 0(r11)
0x825A8E4C:  stfs         f13, 4(r11)
0x825A8E50:  addi         r11, r11, -8
0x825A8E54:  bne          cr6, 0x825a8de0
0x825A8E58:  blr          
0x825A8E5C:  .align 0                       # padding
0x825A8E60:  mflr         r12
0x825A8E64:  bl           0x8242f888
0x825A8E68:  srawi        r24, r3, 1
0x825A8E6C:  divw         r9, r5, r3
0x825A8E70:  addi         r11, r24, -1
0x825A8E74:  li           r26, 0
0x825A8E78:  li           r25, 1
0x825A8E7C:  cmpwi        cr6, r11, 4
0x825A8E80:  blt          cr6, 0x825a8fcc
0x825A8E84:  addi         r10, r24, -5
0x825A8E88:  addi         r8, r3, -3
0x825A8E8C:  srwi         r11, r10, 2
0x825A8E90:  slwi         r10, r8, 2
0x825A8E94:  addi         r31, r11, 1
0x825A8E98:  neg          r7, r9
0x825A8E9C:  slwi         r29, r5, 2
0x825A8EA0:  slwi         r28, r31, 2
0x825A8EA4:  slwi         r8, r9, 2
0x825A8EA8:  mr           r30, r6
0x825A8EAC:  slwi         r7, r7, 2
0x825A8EB0:  addi         r11, r4, 0xc
0x825A8EB4:  add          r29, r29, r6
0x825A8EB8:  add          r10, r10, r4
0x825A8EBC:  addi         r25, r28, 1
0x825A8EC0:  add          r29, r7, r29
0x825A8EC4:  lfs          f11, 8(r10)
0x825A8EC8:  add          r30, r8, r30
0x825A8ECC:  lfs          f12, -8(r11)
0x825A8ED0:  addi         r27, r10, -4
0x825A8ED4:  addi         r28, r11, 4
0x825A8ED8:  add          r26, r26, r9
0x825A8EDC:  lfs          f0, 0(r29)
0x825A8EE0:  add          r29, r7, r29
0x825A8EE4:  lfs          f13, 0(r30)
0x825A8EE8:  add          r30, r8, r30
0x825A8EEC:  fsubs        f10, f13, f0
0x825A8EF0:  add          r26, r26, r9
0x825A8EF4:  fadds        f0, f0, f13
0x825A8EF8:  addi         r31, r31, -1
0x825A8EFC:  add          r26, r26, r9
0x825A8F00:  cmplwi       cr6, r31, 0
0x825A8F04:  add          r26, r26, r9
0x825A8F08:  fmuls        f13, f11, f10
0x825A8F0C:  fmuls        f11, f11, f0
0x825A8F10:  fmsubs       f0, f12, f0, f13
0x825A8F14:  fmadds       f10, f12, f10, f11
0x825A8F18:  stfs         f10, -8(r11)
0x825A8F1C:  stfs         f0, 8(r10)
0x825A8F20:  lfs          f13, 0(r29)
0x825A8F24:  add          r29, r7, r29
0x825A8F28:  lfs          f0, 0(r30)
0x825A8F2C:  add          r30, r8, r30
0x825A8F30:  fsubs        f10, f0, f13
0x825A8F34:  lfs          f12, 4(r10)
0x825A8F38:  fadds        f0, f13, f0
0x825A8F3C:  lfs          f11, -4(r11)
0x825A8F40:  fmuls        f9, f12, f10
0x825A8F44:  fmuls        f8, f12, f0
0x825A8F48:  fmsubs       f0, f11, f0, f9
0x825A8F4C:  fmadds       f7, f11, f10, f8
0x825A8F50:  stfs         f7, -4(r11)
0x825A8F54:  stfs         f0, 4(r10)
0x825A8F58:  lfs          f13, 0(r29)
0x825A8F5C:  add          r29, r7, r29
0x825A8F60:  lfs          f0, 0(r30)
0x825A8F64:  add          r30, r8, r30
0x825A8F68:  fsubs        f10, f0, f13
0x825A8F6C:  lfs          f12, 0(r11)
0x825A8F70:  lfs          f11, 0(r10)
0x825A8F74:  fadds        f0, f13, f0
0x825A8F78:  fmuls        f5, f12, f10
0x825A8F7C:  fmuls        f6, f11, f10
0x825A8F80:  fmadds       f4, f11, f0, f5
0x825A8F84:  stfs         f4, 0(r11)
0x825A8F88:  fmsubs       f13, f12, f0, f6
0x825A8F8C:  stfs         f13, 0(r10)
0x825A8F90:  lfs          f13, 0(r29)
0x825A8F94:  addi         r11, r11, 0x10
0x825A8F98:  lfs          f0, 0(r30)
0x825A8F9C:  addi         r10, r10, -0x10
0x825A8FA0:  fsubs        f10, f0, f13
0x825A8FA4:  lfs          f11, 0(r27)
0x825A8FA8:  fadds        f0, f13, f0
0x825A8FAC:  lfs          f12, 0(r28)
0x825A8FB0:  fmuls        f3, f11, f10
0x825A8FB4:  fmuls        f2, f11, f0
0x825A8FB8:  fmsubs       f13, f12, f0, f3
0x825A8FBC:  fmadds       f1, f12, f10, f2
0x825A8FC0:  stfs         f1, 0(r28)
0x825A8FC4:  stfs         f13, 0(r27)
0x825A8FC8:  bne          cr6, 0x825a8ec0
0x825A8FCC:  cmpw         cr6, r25, r24
0x825A8FD0:  bge          cr6, 0x825a9058
0x825A8FD4:  subf         r7, r26, r5
0x825A8FD8:  subf         r3, r25, r3
0x825A8FDC:  slwi         r7, r7, 2
0x825A8FE0:  neg          r31, r9
0x825A8FE4:  slwi         r10, r3, 2
0x825A8FE8:  slwi         r11, r25, 2
0x825A8FEC:  slwi         r8, r26, 2
0x825A8FF0:  slwi         r5, r9, 2
0x825A8FF4:  add          r9, r7, r6
0x825A8FF8:  add          r11, r11, r4
0x825A8FFC:  add          r8, r8, r6
0x825A9000:  slwi         r3, r31, 2
0x825A9004:  add          r10, r10, r4
0x825A9008:  subf         r7, r25, r24
0x825A900C:  add          r9, r9, r3
0x825A9010:  lfs          f13, 0(r10)
0x825A9014:  add          r8, r8, r5
0x825A9018:  lfs          f0, 0(r11)
0x825A901C:  addi         r7, r7, -1
0x825A9020:  cmplwi       cr6, r7, 0
0x825A9024:  lfs          f12, 0(r9)
0x825A9028:  lfs          f11, 0(r8)
0x825A902C:  fsubs        f10, f11, f12
0x825A9030:  fadds        f12, f12, f11
0x825A9034:  fmuls        f11, f13, f10
0x825A9038:  fmuls        f10, f0, f10
0x825A903C:  fmsubs       f0, f0, f12, f11
0x825A9040:  fmadds       f9, f13, f12, f10
0x825A9044:  stfs         f9, 0(r11)
0x825A9048:  stfs         f0, 0(r10)
0x825A904C:  addi         r11, r11, 4
0x825A9050:  addi         r10, r10, -4
0x825A9054:  bne          cr6, 0x825a900c
0x825A9058:  slwi         r11, r24, 2
0x825A905C:  lfs          f8, 0(r6)
0x825A9060:  lfsx         f7, r11, r4
0x825A9064:  fmuls        f6, f8, f7
0x825A9068:  stfsx        f6, r11, r4
0x825A906C:  b            0x8242f8d8
0x825A9070:  mflr         r12
0x825A9074:  stw          r12, -8(r1)
0x825A9078:  stwu         r1, -0x60(r1)
0x825A907C:  mr           r10, r4
0x825A9080:  cmpwi        cr6, r3, 0x80
0x825A9084:  mr           r3, r10
0x825A9088:  bne          cr6, 0x825a90d8
0x825A908C:  addi         r11, r5, -8
0x825A9090:  slwi         r11, r11, 2
0x825A9094:  add          r9, r11, r6
0x825A9098:  mr           r4, r9
0x825A909C:  bl           0x825a7fe8
0x825A90A0:  addi         r8, r5, -0x20
0x825A90A4:  addi         r3, r10, 0x80
0x825A90A8:  slwi         r11, r8, 2
0x825A90AC:  add          r4, r11, r6
0x825A90B0:  bl           0x825a8408
0x825A90B4:  mr           r4, r9
0x825A90B8:  addi         r3, r10, 0x100
0x825A90BC:  bl           0x825a7fe8
0x825A90C0:  addi         r3, r10, 0x180
0x825A90C4:  bl           0x825a7fe8
0x825A90C8:  addi         r1, r1, 0x60
0x825A90CC:  lwz          r12, -8(r1)
0x825A90D0:  mtlr         r12
0x825A90D4:  blr          
0x825A90D8:  addi         r7, r5, -0x10
0x825A90DC:  slwi         r11, r7, 2
0x825A90E0:  add          r4, r11, r6
0x825A90E4:  bl           0x825a8948
0x825A90E8:  addi         r3, r10, 0x40
0x825A90EC:  bl           0x825a8ad0
0x825A90F0:  addi         r3, r10, 0x80
0x825A90F4:  bl           0x825a8948
0x825A90F8:  addi         r3, r10, 0xc0
0x825A90FC:  bl           0x825a8948
0x825A9100:  addi         r1, r1, 0x60
0x825A9104:  lwz          r12, -8(r1)
0x825A9108:  mtlr         r12
0x825A910C:  blr          
0x825A9110:  mflr         r12
0x825A9114:  bl           0x8242f870
0x825A9118:  stwu         r1, -0xd0(r1)
0x825A911C:  mr           r18, r3
0x825A9120:  mr           r19, r4
0x825A9124:  srawi        r31, r18, 2
0x825A9128:  mr           r29, r5
0x825A912C:  mr           r28, r6
0x825A9130:  cmpwi        cr6, r31, 0x80
0x825A9134:  ble          cr6, 0x825a921c
0x825A9138:  mr           r23, r31
0x825A913C:  cmpw         cr6, r31, r18
0x825A9140:  bge          cr6, 0x825a91ec
0x825A9144:  subf         r30, r31, r23
0x825A9148:  cmpw         cr6, r30, r18
0x825A914C:  bge          cr6, 0x825a91e0
0x825A9150:  slwi         r11, r23, 1
0x825A9154:  srawi        r10, r31, 1
0x825A9158:  subf         r9, r31, r29
0x825A915C:  subf         r8, r10, r29
0x825A9160:  add          r6, r11, r30
0x825A9164:  add          r5, r30, r23
0x825A9168:  slwi         r7, r9, 2
0x825A916C:  slwi         r8, r8, 2
0x825A9170:  slwi         r9, r30, 2
0x825A9174:  slwi         r10, r6, 2
0x825A9178:  slwi         r11, r5, 2
0x825A917C:  add          r21, r7, r28
0x825A9180:  add          r22, r8, r28
0x825A9184:  slwi         r20, r23, 2
0x825A9188:  slwi         r24, r23, 4
0x825A918C:  add          r27, r9, r19
0x825A9190:  add          r25, r10, r19
0x825A9194:  add          r26, r11, r19
0x825A9198:  mr           r5, r22
0x825A919C:  mr           r4, r27
0x825A91A0:  mr           r3, r31
0x825A91A4:  bl           0x825a7820
0x825A91A8:  mr           r5, r21
0x825A91AC:  mr           r4, r26
0x825A91B0:  mr           r3, r31
0x825A91B4:  bl           0x825a7ba8
0x825A91B8:  mr           r5, r22
0x825A91BC:  mr           r4, r25
0x825A91C0:  mr           r3, r31
0x825A91C4:  bl           0x825a7820
0x825A91C8:  add          r30, r20, r30
0x825A91CC:  add          r27, r24, r27
0x825A91D0:  add          r26, r24, r26
0x825A91D4:  add          r25, r24, r25
0x825A91D8:  cmpw         cr6, r30, r18
0x825A91DC:  blt          cr6, 0x825a9198
0x825A91E0:  slwi         r23, r23, 2
0x825A91E4:  cmpw         cr6, r23, r18
0x825A91E8:  blt          cr6, 0x825a9144
0x825A91EC:  subf         r4, r31, r18
0x825A91F0:  srawi        r3, r31, 1
0x825A91F4:  slwi         r11, r4, 2
0x825A91F8:  subf         r10, r3, r29
0x825A91FC:  add          r4, r11, r19
0x825A9200:  slwi         r11, r10, 2
0x825A9204:  mr           r3, r31
0x825A9208:  add          r5, r11, r28
0x825A920C:  bl           0x825a7820
0x825A9210:  srawi        r31, r31, 2
0x825A9214:  cmpwi        cr6, r31, 0x80
0x825A9218:  bgt          cr6, 0x825a9138
0x825A921C:  mr           r24, r31
0x825A9220:  cmpw         cr6, r31, r18
0x825A9224:  bge          cr6, 0x825a9440
0x825A9228:  subf         r25, r31, r24
0x825A922C:  cmpw         cr6, r25, r18
0x825A9230:  bge          cr6, 0x825a9434
0x825A9234:  addi         r9, r24, 0x30
0x825A9238:  srawi        r8, r31, 1
0x825A923C:  slwi         r10, r9, 1
0x825A9240:  add          r11, r25, r24
0x825A9244:  subf         r7, r31, r29
0x825A9248:  subf         r6, r8, r29
0x825A924C:  addi         r5, r25, 0x60
0x825A9250:  add          r4, r10, r25
0x825A9254:  addi         r3, r11, 0x60
0x825A9258:  slwi         r7, r7, 2
0x825A925C:  slwi         r8, r6, 2
0x825A9260:  slwi         r9, r5, 2
0x825A9264:  slwi         r10, r4, 2
0x825A9268:  slwi         r11, r3, 2
0x825A926C:  add          r20, r7, r28
0x825A9270:  add          r22, r8, r28
0x825A9274:  slwi         r21, r24, 2
0x825A9278:  slwi         r23, r24, 4
0x825A927C:  add          r27, r9, r19
0x825A9280:  add          r26, r10, r19
0x825A9284:  add          r30, r11, r19
0x825A9288:  addi         r4, r27, -0x180
0x825A928C:  mr           r5, r22
0x825A9290:  mr           r3, r31
0x825A9294:  bl           0x825a7820
0x825A9298:  cmpwi        cr6, r31, 0x80
0x825A929C:  mr           r3, r4
0x825A92A0:  bne          cr6, 0x825a92e4
0x825A92A4:  addi         r11, r29, -8
0x825A92A8:  slwi         r11, r11, 2
0x825A92AC:  add          r10, r11, r28
0x825A92B0:  mr           r4, r10
0x825A92B4:  bl           0x825a7fe8
0x825A92B8:  addi         r9, r29, -0x20
0x825A92BC:  addi         r3, r27, -0x100
0x825A92C0:  slwi         r11, r9, 2
0x825A92C4:  add          r4, r11, r28
0x825A92C8:  bl           0x825a8408
0x825A92CC:  mr           r4, r10
0x825A92D0:  addi         r3, r27, -0x80
0x825A92D4:  bl           0x825a7fe8
0x825A92D8:  mr           r3, r27
0x825A92DC:  bl           0x825a7fe8
0x825A92E0:  b            0x825a930c
0x825A92E4:  addi         r8, r29, -0x10
0x825A92E8:  slwi         r11, r8, 2
0x825A92EC:  add          r4, r11, r28
0x825A92F0:  bl           0x825a8948
0x825A92F4:  addi         r3, r27, -0x140
0x825A92F8:  bl           0x825a8ad0
0x825A92FC:  addi         r3, r27, -0x100
0x825A9300:  bl           0x825a8948
0x825A9304:  addi         r3, r27, -0xc0
0x825A9308:  bl           0x825a8948
0x825A930C:  addi         r4, r30, -0x180
0x825A9310:  mr           r5, r20
0x825A9314:  mr           r3, r31
0x825A9318:  bl           0x825a7ba8
0x825A931C:  cmpwi        cr6, r31, 0x80
0x825A9320:  mr           r3, r4
0x825A9324:  bne          cr6, 0x825a9370
0x825A9328:  addi         r7, r29, -8
0x825A932C:  slwi         r11, r7, 2
0x825A9330:  add          r10, r11, r28
0x825A9334:  mr           r4, r10
0x825A9338:  bl           0x825a7fe8
0x825A933C:  addi         r6, r29, -0x20
0x825A9340:  addi         r3, r30, -0x100
0x825A9344:  slwi         r11, r6, 2
0x825A9348:  add          r9, r11, r28
0x825A934C:  mr           r4, r9
0x825A9350:  bl           0x825a8408
0x825A9354:  mr           r4, r10
0x825A9358:  addi         r3, r30, -0x80
0x825A935C:  bl           0x825a7fe8
0x825A9360:  mr           r4, r9
0x825A9364:  mr           r3, r30
0x825A9368:  bl           0x825a8408
0x825A936C:  b            0x825a9398
0x825A9370:  addi         r5, r29, -0x10
0x825A9374:  slwi         r11, r5, 2
0x825A9378:  add          r4, r11, r28
0x825A937C:  bl           0x825a8948
0x825A9380:  addi         r3, r30, -0x140
0x825A9384:  bl           0x825a8ad0
0x825A9388:  addi         r3, r30, -0x100
0x825A938C:  bl           0x825a8948
0x825A9390:  addi         r3, r30, -0xc0
0x825A9394:  bl           0x825a8ad0
0x825A9398:  addi         r4, r26, -0x180
0x825A939C:  mr           r5, r22
0x825A93A0:  mr           r3, r31
0x825A93A4:  bl           0x825a7820
0x825A93A8:  cmpwi        cr6, r31, 0x80
0x825A93AC:  mr           r3, r4
0x825A93B0:  bne          cr6, 0x825a93f4
0x825A93B4:  addi         r11, r29, -8
0x825A93B8:  slwi         r11, r11, 2
0x825A93BC:  add          r10, r11, r28
0x825A93C0:  mr           r4, r10
0x825A93C4:  bl           0x825a7fe8
0x825A93C8:  addi         r9, r29, -0x20
0x825A93CC:  addi         r3, r26, -0x100
0x825A93D0:  slwi         r11, r9, 2
0x825A93D4:  add          r4, r11, r28
0x825A93D8:  bl           0x825a8408
0x825A93DC:  mr           r4, r10
0x825A93E0:  addi         r3, r26, -0x80
0x825A93E4:  bl           0x825a7fe8
0x825A93E8:  mr           r3, r26
0x825A93EC:  bl           0x825a7fe8
0x825A93F0:  b            0x825a941c
0x825A93F4:  addi         r8, r29, -0x10
0x825A93F8:  slwi         r11, r8, 2
0x825A93FC:  add          r4, r11, r28
0x825A9400:  bl           0x825a8948
0x825A9404:  addi         r3, r26, -0x140
0x825A9408:  bl           0x825a8ad0
0x825A940C:  addi         r3, r26, -0x100
0x825A9410:  bl           0x825a8948
0x825A9414:  addi         r3, r26, -0xc0
0x825A9418:  bl           0x825a8948
0x825A941C:  add          r25, r21, r25
0x825A9420:  add          r27, r27, r23
0x825A9424:  add          r30, r30, r23
0x825A9428:  add          r26, r26, r23
0x825A942C:  cmpw         cr6, r25, r18
0x825A9430:  blt          cr6, 0x825a9288
0x825A9434:  slwi         r24, r24, 2
0x825A9438:  cmpw         cr6, r24, r18
0x825A943C:  blt          cr6, 0x825a9228
0x825A9440:  subf         r7, r31, r18
0x825A9444:  srawi        r6, r31, 1
0x825A9448:  slwi         r11, r7, 2
0x825A944C:  subf         r5, r6, r29
0x825A9450:  add          r30, r11, r19
0x825A9454:  slwi         r11, r5, 2
0x825A9458:  mr           r3, r31
0x825A945C:  add          r5, r11, r28
0x825A9460:  mr           r4, r30
0x825A9464:  bl           0x825a7820
0x825A9468:  cmpwi        cr6, r31, 0x80
0x825A946C:  mr           r3, r30
0x825A9470:  bne          cr6, 0x825a94b8
0x825A9474:  addi         r4, r29, -8
0x825A9478:  slwi         r11, r4, 2
0x825A947C:  add          r10, r11, r28
0x825A9480:  mr           r4, r10
0x825A9484:  bl           0x825a7fe8
0x825A9488:  addi         r11, r29, -0x20
0x825A948C:  addi         r3, r30, 0x80
0x825A9490:  slwi         r11, r11, 2
0x825A9494:  add          r4, r11, r28
0x825A9498:  bl           0x825a8408
0x825A949C:  mr           r4, r10
0x825A94A0:  addi         r3, r30, 0x100
0x825A94A4:  bl           0x825a7fe8
0x825A94A8:  addi         r3, r30, 0x180
0x825A94AC:  bl           0x825a7fe8
0x825A94B0:  addi         r1, r1, 0xd0
0x825A94B4:  b            0x8242f8c0
0x825A94B8:  addi         r10, r29, -0x10
0x825A94BC:  slwi         r11, r10, 2
0x825A94C0:  add          r4, r11, r28
0x825A94C4:  bl           0x825a8948
0x825A94C8:  addi         r3, r30, 0x40
0x825A94CC:  bl           0x825a8ad0
0x825A94D0:  addi         r3, r30, 0x80
0x825A94D4:  bl           0x825a8948
0x825A94D8:  addi         r3, r30, 0xc0
0x825A94DC:  bl           0x825a8948
0x825A94E0:  addi         r1, r1, 0xd0
0x825A94E4:  b            0x8242f8c0
0x825A94E8:  mflr         r12
0x825A94EC:  bl           0x8242f878
0x825A94F0:  stwu         r1, -0xc0(r1)
0x825A94F4:  srawi        r22, r3, 1
0x825A94F8:  srawi        r31, r3, 2
0x825A94FC:  mr           r20, r4
0x825A9500:  mr           r27, r5
0x825A9504:  mr           r26, r6
0x825A9508:  cmpwi        cr6, r31, 0x80
0x825A950C:  ble          cr6, 0x825a9604
0x825A9510:  mr           r23, r31
0x825A9514:  cmpw         cr6, r31, r22
0x825A9518:  bge          cr6, 0x825a95f8
0x825A951C:  subf         r30, r31, r23
0x825A9520:  cmpw         cr6, r30, r22
0x825A9524:  bge          cr6, 0x825a9588
0x825A9528:  srawi        r9, r31, 1
0x825A952C:  add          r11, r30, r22
0x825A9530:  subf         r8, r9, r27
0x825A9534:  slwi         r10, r30, 2
0x825A9538:  slwi         r11, r11, 2
0x825A953C:  slwi         r9, r8, 2
0x825A9540:  slwi         r21, r23, 1
0x825A9544:  add          r25, r9, r26
0x825A9548:  slwi         r24, r23, 3
0x825A954C:  add          r29, r10, r20
0x825A9550:  add          r28, r11, r20
0x825A9554:  mr           r5, r25
0x825A9558:  mr           r4, r29
0x825A955C:  mr           r3, r31
0x825A9560:  bl           0x825a7820
0x825A9564:  mr           r5, r25
0x825A9568:  mr           r4, r28
0x825A956C:  mr           r3, r31
0x825A9570:  bl           0x825a7820
0x825A9574:  add          r30, r21, r30
0x825A9578:  add          r29, r24, r29
0x825A957C:  add          r28, r24, r28
0x825A9580:  cmpw         cr6, r30, r22
0x825A9584:  blt          cr6, 0x825a9554
0x825A9588:  slwi         r7, r23, 1
0x825A958C:  subf         r30, r31, r7
0x825A9590:  cmpw         cr6, r30, r22
0x825A9594:  bge          cr6, 0x825a95ec
0x825A9598:  add          r5, r30, r22
0x825A959C:  subf         r6, r31, r27
0x825A95A0:  slwi         r11, r5, 2
0x825A95A4:  slwi         r9, r6, 2
0x825A95A8:  slwi         r10, r30, 2
0x825A95AC:  add          r5, r9, r26
0x825A95B0:  slwi         r24, r23, 2
0x825A95B4:  slwi         r25, r23, 4
0x825A95B8:  add          r29, r10, r20
0x825A95BC:  add          r28, r11, r20
0x825A95C0:  mr           r4, r29
0x825A95C4:  mr           r3, r31
0x825A95C8:  bl           0x825a7ba8
0x825A95CC:  mr           r4, r28
0x825A95D0:  mr           r3, r31
0x825A95D4:  bl           0x825a7ba8
0x825A95D8:  add          r30, r24, r30
0x825A95DC:  add          r29, r25, r29
0x825A95E0:  add          r28, r25, r28
0x825A95E4:  cmpw         cr6, r30, r22
0x825A95E8:  blt          cr6, 0x825a95c0
0x825A95EC:  slwi         r23, r23, 2
0x825A95F0:  cmpw         cr6, r23, r22
0x825A95F4:  blt          cr6, 0x825a951c
0x825A95F8:  srawi        r31, r31, 2
0x825A95FC:  cmpwi        cr6, r31, 0x80
0x825A9600:  bgt          cr6, 0x825a9510
0x825A9604:  mr           r21, r31
0x825A9608:  cmpw         cr6, r31, r22
0x825A960C:  bge          cr6, 0x825a98dc
0x825A9610:  subf         r28, r31, r21
0x825A9614:  cmpw         cr6, r28, r22
0x825A9618:  bge          cr6, 0x825a976c
0x825A961C:  srawi        r4, r31, 1
0x825A9620:  add          r11, r28, r22
0x825A9624:  subf         r10, r4, r27
0x825A9628:  addi         r3, r28, 0x60
0x825A962C:  addi         r8, r11, 0x60
0x825A9630:  slwi         r9, r10, 2
0x825A9634:  slwi         r10, r3, 2
0x825A9638:  slwi         r11, r8, 2
0x825A963C:  add          r24, r9, r26
0x825A9640:  slwi         r23, r21, 1
0x825A9644:  slwi         r25, r21, 3
0x825A9648:  add          r30, r10, r20
0x825A964C:  add          r29, r11, r20
0x825A9650:  addi         r4, r30, -0x180
0x825A9654:  mr           r5, r24
0x825A9658:  mr           r3, r31
0x825A965C:  bl           0x825a7820
0x825A9660:  cmpwi        cr6, r31, 0x80
0x825A9664:  mr           r3, r4
0x825A9668:  bne          cr6, 0x825a96ac
0x825A966C:  addi         r7, r27, -8
0x825A9670:  slwi         r11, r7, 2
0x825A9674:  add          r10, r11, r26
0x825A9678:  mr           r4, r10
0x825A967C:  bl           0x825a7fe8
0x825A9680:  addi         r6, r27, -0x20
0x825A9684:  addi         r3, r30, -0x100
0x825A9688:  slwi         r11, r6, 2
0x825A968C:  add          r4, r11, r26
0x825A9690:  bl           0x825a8408
0x825A9694:  mr           r4, r10
0x825A9698:  addi         r3, r30, -0x80
0x825A969C:  bl           0x825a7fe8
0x825A96A0:  mr           r3, r30
0x825A96A4:  bl           0x825a7fe8
0x825A96A8:  b            0x825a96d4
0x825A96AC:  addi         r5, r27, -0x10
0x825A96B0:  slwi         r11, r5, 2
0x825A96B4:  add          r4, r11, r26
0x825A96B8:  bl           0x825a8948
0x825A96BC:  addi         r3, r30, -0x140
0x825A96C0:  bl           0x825a8ad0
0x825A96C4:  addi         r3, r30, -0x100
0x825A96C8:  bl           0x825a8948
0x825A96CC:  addi         r3, r30, -0xc0
0x825A96D0:  bl           0x825a8948
0x825A96D4:  addi         r4, r29, -0x180
0x825A96D8:  mr           r5, r24
0x825A96DC:  mr           r3, r31
0x825A96E0:  bl           0x825a7820
0x825A96E4:  cmpwi        cr6, r31, 0x80
0x825A96E8:  mr           r3, r4
0x825A96EC:  bne          cr6, 0x825a9730
0x825A96F0:  addi         r11, r27, -8
0x825A96F4:  slwi         r11, r11, 2
0x825A96F8:  add          r10, r11, r26
0x825A96FC:  mr           r4, r10
0x825A9700:  bl           0x825a7fe8
0x825A9704:  addi         r9, r27, -0x20
0x825A9708:  addi         r3, r29, -0x100
0x825A970C:  slwi         r11, r9, 2
0x825A9710:  add          r4, r11, r26
0x825A9714:  bl           0x825a8408
0x825A9718:  mr           r4, r10
0x825A971C:  addi         r3, r29, -0x80
0x825A9720:  bl           0x825a7fe8
0x825A9724:  mr           r3, r29
0x825A9728:  bl           0x825a7fe8
0x825A972C:  b            0x825a9758
0x825A9730:  addi         r8, r27, -0x10
0x825A9734:  slwi         r11, r8, 2
0x825A9738:  add          r4, r11, r26
0x825A973C:  bl           0x825a8948
0x825A9740:  addi         r3, r29, -0x140
0x825A9744:  bl           0x825a8ad0
0x825A9748:  addi         r3, r29, -0x100
0x825A974C:  bl           0x825a8948
0x825A9750:  addi         r3, r29, -0xc0
0x825A9754:  bl           0x825a8948
0x825A9758:  add          r28, r23, r28
0x825A975C:  add          r30, r30, r25
0x825A9760:  add          r29, r29, r25
0x825A9764:  cmpw         cr6, r28, r22
0x825A9768:  blt          cr6, 0x825a9650
0x825A976C:  slwi         r7, r21, 1
0x825A9770:  subf         r28, r31, r7
0x825A9774:  cmpw         cr6, r28, r22
0x825A9778:  bge          cr6, 0x825a98d0
0x825A977C:  add          r11, r28, r22
0x825A9780:  addi         r5, r28, 0x60
0x825A9784:  subf         r6, r31, r27
0x825A9788:  addi         r4, r11, 0x60
0x825A978C:  slwi         r10, r5, 2
0x825A9790:  slwi         r9, r6, 2
0x825A9794:  slwi         r11, r4, 2
0x825A9798:  add          r5, r9, r26
0x825A979C:  slwi         r24, r21, 2
0x825A97A0:  slwi         r25, r21, 4
0x825A97A4:  add          r30, r10, r20
0x825A97A8:  add          r29, r11, r20
0x825A97AC:  addi         r4, r30, -0x180
0x825A97B0:  mr           r3, r31
0x825A97B4:  bl           0x825a7ba8
0x825A97B8:  cmpwi        cr6, r31, 0x80
0x825A97BC:  mr           r3, r4
0x825A97C0:  bne          cr6, 0x825a980c
0x825A97C4:  addi         r11, r27, -8
0x825A97C8:  slwi         r11, r11, 2
0x825A97CC:  add          r10, r11, r26
0x825A97D0:  mr           r4, r10
0x825A97D4:  bl           0x825a7fe8
0x825A97D8:  addi         r9, r27, -0x20
0x825A97DC:  addi         r3, r30, -0x100
0x825A97E0:  slwi         r11, r9, 2
0x825A97E4:  add          r9, r11, r26
0x825A97E8:  mr           r4, r9
0x825A97EC:  bl           0x825a8408
0x825A97F0:  mr           r4, r10
0x825A97F4:  addi         r3, r30, -0x80
0x825A97F8:  bl           0x825a7fe8
0x825A97FC:  mr           r4, r9
0x825A9800:  mr           r3, r30
0x825A9804:  bl           0x825a8408
0x825A9808:  b            0x825a9834
0x825A980C:  addi         r8, r27, -0x10
0x825A9810:  slwi         r11, r8, 2
0x825A9814:  add          r4, r11, r26
0x825A9818:  bl           0x825a8948
0x825A981C:  addi         r3, r30, -0x140
0x825A9820:  bl           0x825a8ad0
0x825A9824:  addi         r3, r30, -0x100
0x825A9828:  bl           0x825a8948
0x825A982C:  addi         r3, r30, -0xc0
0x825A9830:  bl           0x825a8ad0
0x825A9834:  addi         r4, r29, -0x180
0x825A9838:  mr           r3, r31
0x825A983C:  bl           0x825a7ba8
0x825A9840:  cmpwi        cr6, r31, 0x80
0x825A9844:  mr           r3, r4
0x825A9848:  bne          cr6, 0x825a9894
0x825A984C:  addi         r7, r27, -8
0x825A9850:  slwi         r11, r7, 2
0x825A9854:  add          r10, r11, r26
0x825A9858:  mr           r4, r10
0x825A985C:  bl           0x825a7fe8
0x825A9860:  addi         r6, r27, -0x20
0x825A9864:  addi         r3, r29, -0x100
0x825A9868:  slwi         r11, r6, 2
0x825A986C:  add          r9, r11, r26
0x825A9870:  mr           r4, r9
0x825A9874:  bl           0x825a8408
0x825A9878:  mr           r4, r10
0x825A987C:  addi         r3, r29, -0x80
0x825A9880:  bl           0x825a7fe8
0x825A9884:  mr           r4, r9
0x825A9888:  mr           r3, r29
0x825A988C:  bl           0x825a8408
0x825A9890:  b            0x825a98bc
0x825A9894:  addi         r11, r27, -0x10
0x825A9898:  slwi         r11, r11, 2
0x825A989C:  add          r4, r11, r26
0x825A98A0:  bl           0x825a8948
0x825A98A4:  addi         r3, r29, -0x140
0x825A98A8:  bl           0x825a8ad0
0x825A98AC:  addi         r3, r29, -0x100
0x825A98B0:  bl           0x825a8948
0x825A98B4:  addi         r3, r29, -0xc0
0x825A98B8:  bl           0x825a8ad0
0x825A98BC:  add          r28, r24, r28
0x825A98C0:  add          r30, r25, r30
0x825A98C4:  add          r29, r25, r29
0x825A98C8:  cmpw         cr6, r28, r22
0x825A98CC:  blt          cr6, 0x825a97ac
0x825A98D0:  slwi         r21, r21, 2
0x825A98D4:  cmpw         cr6, r21, r22
0x825A98D8:  blt          cr6, 0x825a9610
0x825A98DC:  addi         r1, r1, 0xc0
0x825A98E0:  b            0x8242f8c8
0x825A98E4:  .align 0                       # padding
0x825A98E8:  mflr         r12
0x825A98EC:  bl           0x8242f894
0x825A98F0:  stwu         r1, -0x80(r1)
0x825A98F4:  mr           r27, r3
0x825A98F8:  mr           r29, r5
0x825A98FC:  srawi        r31, r27, 2
0x825A9900:  mr           r28, r6
0x825A9904:  slwi         r11, r31, 1
0x825A9908:  mr           r30, r4
0x825A990C:  subf         r10, r11, r29
0x825A9910:  slwi         r11, r10, 2
0x825A9914:  add          r5, r11, r28
0x825A9918:  bl           0x825a7820
0x825A991C:  cmpwi        cr6, r27, 0x200
0x825A9920:  ble          cr6, 0x825a99a8
0x825A9924:  mr           r6, r28
0x825A9928:  mr           r5, r29
0x825A992C:  mr           r4, r30
0x825A9930:  mr           r3, r31
0x825A9934:  bl           0x825a98e8
0x825A9938:  slwi         r11, r31, 2
0x825A993C:  mr           r6, r28
0x825A9940:  mr           r5, r29
0x825A9944:  add          r4, r11, r30
0x825A9948:  mr           r3, r31
0x825A994C:  bl           0x825a99c8
0x825A9950:  slwi         r11, r31, 3
0x825A9954:  mr           r6, r28
0x825A9958:  mr           r5, r29
0x825A995C:  add          r4, r11, r30
0x825A9960:  mr           r3, r31
0x825A9964:  bl           0x825a98e8
0x825A9968:  mr           r11, r31
0x825A996C:  mr           r27, r31
0x825A9970:  slwi         r10, r11, 1
0x825A9974:  srawi        r31, r31, 2
0x825A9978:  add          r9, r11, r10
0x825A997C:  slwi         r8, r31, 1
0x825A9980:  slwi         r11, r9, 2
0x825A9984:  subf         r7, r8, r29
0x825A9988:  add          r30, r11, r30
0x825A998C:  slwi         r11, r7, 2
0x825A9990:  mr           r3, r27
0x825A9994:  add          r5, r11, r28
0x825A9998:  mr           r4, r30
0x825A999C:  bl           0x825a7820
0x825A99A0:  cmpwi        cr6, r27, 0x200
0x825A99A4:  bgt          cr6, 0x825a9924
0x825A99A8:  mr           r6, r28
0x825A99AC:  mr           r5, r29
0x825A99B0:  mr           r4, r30
0x825A99B4:  mr           r3, r27
0x825A99B8:  bl           0x825a9110
0x825A99BC:  addi         r1, r1, 0x80
0x825A99C0:  b            0x8242f8e4
0x825A99C4:  .align 0                       # padding
0x825A99C8:  mflr         r12
0x825A99CC:  bl           0x8242f894
0x825A99D0:  stwu         r1, -0x80(r1)
0x825A99D4:  mr           r29, r3
0x825A99D8:  mr           r28, r5
0x825A99DC:  mr           r27, r6
0x825A99E0:  subf         r11, r29, r28
0x825A99E4:  mr           r30, r4
0x825A99E8:  slwi         r11, r11, 2
0x825A99EC:  srawi        r31, r29, 2
0x825A99F0:  add          r5, r11, r27
0x825A99F4:  bl           0x825a7ba8
0x825A99F8:  cmpwi        cr6, r29, 0x200
0x825A99FC:  ble          cr6, 0x825a9a80
0x825A9A00:  mr           r6, r27
0x825A9A04:  mr           r5, r28
0x825A9A08:  mr           r4, r30
0x825A9A0C:  mr           r3, r31
0x825A9A10:  bl           0x825a98e8
0x825A9A14:  slwi         r11, r31, 2
0x825A9A18:  mr           r6, r27
0x825A9A1C:  mr           r5, r28
0x825A9A20:  add          r4, r11, r30
0x825A9A24:  mr           r3, r31
0x825A9A28:  bl           0x825a99c8
0x825A9A2C:  slwi         r11, r31, 3
0x825A9A30:  mr           r6, r27
0x825A9A34:  mr           r5, r28
0x825A9A38:  add          r4, r11, r30
0x825A9A3C:  mr           r3, r31
0x825A9A40:  bl           0x825a98e8
0x825A9A44:  mr           r11, r31
0x825A9A48:  mr           r29, r31
0x825A9A4C:  slwi         r10, r11, 1
0x825A9A50:  subf         r9, r29, r28
0x825A9A54:  add          r8, r11, r10
0x825A9A58:  slwi         r10, r9, 2
0x825A9A5C:  slwi         r11, r8, 2
0x825A9A60:  add          r5, r10, r27
0x825A9A64:  add          r30, r11, r30
0x825A9A68:  mr           r3, r29
0x825A9A6C:  mr           r4, r30
0x825A9A70:  srawi        r31, r31, 2
0x825A9A74:  bl           0x825a7ba8
0x825A9A78:  cmpwi        cr6, r29, 0x200
0x825A9A7C:  bgt          cr6, 0x825a9a00
0x825A9A80:  mr           r6, r27
0x825A9A84:  mr           r5, r28
0x825A9A88:  mr           r4, r30
0x825A9A8C:  mr           r3, r29
0x825A9A90:  bl           0x825a94e8
0x825A9A94:  addi         r1, r1, 0x80
0x825A9A98:  b            0x8242f8e4
0x825A9A9C:  .align 0                       # padding
0x825A9AA0:  mflr         r12
0x825A9AA4:  bl           0x8242f890
0x825A9AA8:  addi         r12, r1, -0x38
0x825A9AAC:  bl           0x82436600
0x825A9AB0:  stwu         r1, -0xe0(r1)
0x825A9AB4:  mr           r27, r3
0x825A9AB8:  mr           r31, r4
0x825A9ABC:  mr           r26, r5
0x825A9AC0:  mr           r28, r6
0x825A9AC4:  mr           r29, r7
0x825A9AC8:  cmpwi        cr6, r27, 0x20
0x825A9ACC:  ble          cr6, 0x825a9bc0
0x825A9AD0:  srawi        r30, r27, 2
0x825A9AD4:  subf         r11, r30, r28
0x825A9AD8:  slwi         r11, r11, 2
0x825A9ADC:  add          r5, r11, r29
0x825A9AE0:  bl           0x825a69e0
0x825A9AE4:  cmpwi        cr6, r27, 0x200
0x825A9AE8:  mr           r6, r29
0x825A9AEC:  mr           r5, r28
0x825A9AF0:  ble          cr6, 0x825a9b6c
0x825A9AF4:  mr           r3, r30
0x825A9AF8:  bl           0x825a98e8
0x825A9AFC:  slwi         r11, r30, 2
0x825A9B00:  mr           r6, r29
0x825A9B04:  mr           r5, r28
0x825A9B08:  add          r4, r11, r31
0x825A9B0C:  mr           r3, r30
0x825A9B10:  bl           0x825a99c8
0x825A9B14:  slwi         r11, r30, 3
0x825A9B18:  mr           r6, r29
0x825A9B1C:  mr           r5, r28
0x825A9B20:  add          r4, r11, r31
0x825A9B24:  mr           r3, r30
0x825A9B28:  bl           0x825a98e8
0x825A9B2C:  slwi         r11, r30, 1
0x825A9B30:  mr           r6, r29
0x825A9B34:  add          r10, r30, r11
0x825A9B38:  mr           r5, r28
0x825A9B3C:  slwi         r11, r10, 2
0x825A9B40:  mr           r3, r30
0x825A9B44:  add          r4, r11, r31
0x825A9B48:  bl           0x825a98e8
0x825A9B4C:  mr           r5, r31
0x825A9B50:  mr           r4, r26
0x825A9B54:  mr           r3, r27
0x825A9B58:  bl           0x825a5d70
0x825A9B5C:  addi         r1, r1, 0xe0
0x825A9B60:  addi         r12, r1, -0x38
0x825A9B64:  bl           0x8243664c
0x825A9B68:  b            0x8242f8e0
0x825A9B6C:  cmpwi        cr6, r30, 0x20
0x825A9B70:  mr           r3, r27
0x825A9B74:  ble          cr6, 0x825a9b9c
0x825A9B78:  bl           0x825a9110
0x825A9B7C:  mr           r5, r31
0x825A9B80:  mr           r4, r26
0x825A9B84:  mr           r3, r27
0x825A9B88:  bl           0x825a5d70
0x825A9B8C:  addi         r1, r1, 0xe0
0x825A9B90:  addi         r12, r1, -0x38
0x825A9B94:  bl           0x8243664c
0x825A9B98:  b            0x8242f8e0
0x825A9B9C:  bl           0x825a9070
0x825A9BA0:  mr           r5, r31
0x825A9BA4:  mr           r4, r26
0x825A9BA8:  mr           r3, r27
0x825A9BAC:  bl           0x825a5d70
0x825A9BB0:  addi         r1, r1, 0xe0
0x825A9BB4:  addi         r12, r1, -0x38
0x825A9BB8:  bl           0x8243664c
0x825A9BBC:  b            0x8242f8e0
0x825A9BC0:  cmpwi        cr6, r27, 8
0x825A9BC4:  ble          cr6, 0x825a9d0c
0x825A9BC8:  cmpwi        cr6, r27, 0x20
0x825A9BCC:  mr           r3, r31
0x825A9BD0:  bne          cr6, 0x825a9cb4
0x825A9BD4:  addi         r9, r28, -8
0x825A9BD8:  slwi         r11, r9, 2
0x825A9BDC:  add          r4, r11, r29
0x825A9BE0:  bl           0x825a7fe8
0x825A9BE4:  lfs          f0, 8(r31)
0x825A9BE8:  lfs          f13, 0xc(r31)
0x825A9BEC:  lfs          f12, 0x10(r31)
0x825A9BF0:  lfs          f11, 0x14(r31)
0x825A9BF4:  lfs          f10, 0x18(r31)
0x825A9BF8:  lfs          f9, 0x28(r31)
0x825A9BFC:  lfs          f8, 0x2c(r31)
0x825A9C00:  lfs          f7, 0x1c(r31)
0x825A9C04:  lfs          f6, 0x38(r31)
0x825A9C08:  lfs          f5, 0x3c(r31)
0x825A9C0C:  lfs          f4, 0x58(r31)
0x825A9C10:  lfs          f3, 0x5c(r31)
0x825A9C14:  lfs          f2, 0x40(r31)
0x825A9C18:  lfs          f1, 0x44(r31)
0x825A9C1C:  lfs          f31, 0x20(r31)
0x825A9C20:  lfs          f30, 0x24(r31)
0x825A9C24:  lfs          f29, 0x60(r31)
0x825A9C28:  lfs          f28, 0x50(r31)
0x825A9C2C:  lfs          f27, 0x54(r31)
0x825A9C30:  lfs          f26, 0x64(r31)
0x825A9C34:  lfs          f25, 0x70(r31)
0x825A9C38:  lfs          f24, 0x74(r31)
0x825A9C3C:  lfs          f23, 0x68(r31)
0x825A9C40:  lfs          f22, 0x6c(r31)
0x825A9C44:  stfs         f2, 8(r31)
0x825A9C48:  stfs         f1, 0xc(r31)
0x825A9C4C:  stfs         f31, 0x10(r31)
0x825A9C50:  stfs         f30, 0x14(r31)
0x825A9C54:  stfs         f29, 0x18(r31)
0x825A9C58:  stfs         f26, 0x1c(r31)
0x825A9C5C:  stfs         f28, 0x28(r31)
0x825A9C60:  stfs         f27, 0x2c(r31)
0x825A9C64:  stfs         f25, 0x38(r31)
0x825A9C68:  stfs         f24, 0x3c(r31)
0x825A9C6C:  stfs         f23, 0x58(r31)
0x825A9C70:  stfs         f22, 0x5c(r31)
0x825A9C74:  stfs         f0, 0x40(r31)
0x825A9C78:  stfs         f13, 0x44(r31)
0x825A9C7C:  stfs         f12, 0x20(r31)
0x825A9C80:  stfs         f11, 0x24(r31)
0x825A9C84:  stfs         f10, 0x60(r31)
0x825A9C88:  stfs         f9, 0x50(r31)
0x825A9C8C:  stfs         f8, 0x54(r31)
0x825A9C90:  stfs         f7, 0x64(r31)
0x825A9C94:  stfs         f4, 0x68(r31)
0x825A9C98:  stfs         f3, 0x6c(r31)
0x825A9C9C:  stfs         f6, 0x70(r31)
0x825A9CA0:  stfs         f5, 0x74(r31)
0x825A9CA4:  addi         r1, r1, 0xe0
0x825A9CA8:  addi         r12, r1, -0x38
0x825A9CAC:  bl           0x8243664c
0x825A9CB0:  b            0x8242f8e0
0x825A9CB4:  mr           r4, r29
0x825A9CB8:  bl           0x825a8948
0x825A9CBC:  lfs          f0, 8(r31)
0x825A9CC0:  lfs          f13, 0xc(r31)
0x825A9CC4:  lfs          f12, 0x18(r31)
0x825A9CC8:  lfs          f11, 0x1c(r31)
0x825A9CCC:  lfs          f10, 0x20(r31)
0x825A9CD0:  lfs          f9, 0x24(r31)
0x825A9CD4:  lfs          f8, 0x30(r31)
0x825A9CD8:  lfs          f7, 0x34(r31)
0x825A9CDC:  stfs         f9, 0xc(r31)
0x825A9CE0:  stfs         f8, 0x18(r31)
0x825A9CE4:  stfs         f7, 0x1c(r31)
0x825A9CE8:  stfs         f0, 0x20(r31)
0x825A9CEC:  stfs         f13, 0x24(r31)
0x825A9CF0:  stfs         f12, 0x30(r31)
0x825A9CF4:  stfs         f11, 0x34(r31)
0x825A9CF8:  stfs         f10, 8(r31)
0x825A9CFC:  addi         r1, r1, 0xe0
0x825A9D00:  addi         r12, r1, -0x38
0x825A9D04:  bl           0x8243664c
0x825A9D08:  b            0x8242f8e0
0x825A9D0C:  bne          cr6, 0x825a9da0
0x825A9D10:  lfs          f13, 0x10(r31)
0x825A9D14:  lfs          f0, 0(r31)
0x825A9D18:  lfs          f11, 4(r31)
0x825A9D1C:  fadds        f6, f0, f13
0x825A9D20:  lfs          f12, 0x14(r31)
0x825A9D24:  fsubs        f0, f0, f13
0x825A9D28:  lfs          f9, 8(r31)
0x825A9D2C:  fadds        f13, f11, f12
0x825A9D30:  lfs          f10, 0x18(r31)
0x825A9D34:  fsubs        f12, f11, f12
0x825A9D38:  lfs          f8, 0x1c(r31)
0x825A9D3C:  fadds        f11, f10, f9
0x825A9D40:  lfs          f7, 0xc(r31)
0x825A9D44:  fsubs        f10, f9, f10
0x825A9D48:  fadds        f9, f8, f7
0x825A9D4C:  fsubs        f8, f7, f8
0x825A9D50:  fadds        f5, f11, f6
0x825A9D54:  stfs         f5, 0(r31)
0x825A9D58:  fsubs        f4, f6, f11
0x825A9D5C:  stfs         f4, 0x10(r31)
0x825A9D60:  fadds        f3, f9, f13
0x825A9D64:  stfs         f3, 4(r31)
0x825A9D68:  fsubs        f2, f13, f9
0x825A9D6C:  stfs         f2, 0x14(r31)
0x825A9D70:  fadds        f13, f10, f12
0x825A9D74:  stfs         f13, 0xc(r31)
0x825A9D78:  fsubs        f1, f0, f8
0x825A9D7C:  stfs         f1, 8(r31)
0x825A9D80:  fadds        f11, f8, f0
0x825A9D84:  stfs         f11, 0x18(r31)
0x825A9D88:  fsubs        f10, f12, f10
0x825A9D8C:  stfs         f10, 0x1c(r31)
0x825A9D90:  addi         r1, r1, 0xe0
0x825A9D94:  addi         r12, r1, -0x38
0x825A9D98:  bl           0x8243664c
0x825A9D9C:  b            0x8242f8e0
0x825A9DA0:  cmpwi        cr6, r27, 4
0x825A9DA4:  bne          cr6, 0x825a9dd8
0x825A9DA8:  lfs          f0, 0(r31)
0x825A9DAC:  lfs          f13, 8(r31)
0x825A9DB0:  lfs          f12, 4(r31)
0x825A9DB4:  fsubs        f10, f0, f13
0x825A9DB8:  lfs          f11, 0xc(r31)
0x825A9DBC:  fadds        f9, f0, f13
0x825A9DC0:  fsubs        f0, f12, f11
0x825A9DC4:  stfs         f9, 0(r31)
0x825A9DC8:  fadds        f8, f12, f11
0x825A9DCC:  stfs         f0, 0xc(r31)
0x825A9DD0:  stfs         f8, 4(r31)
0x825A9DD4:  stfs         f10, 8(r31)
0x825A9DD8:  addi         r1, r1, 0xe0
0x825A9DDC:  addi         r12, r1, -0x38
0x825A9DE0:  bl           0x8243664c
0x825A9DE4:  b            0x8242f8e0
0x825A9DE8:  mflr         r12
0x825A9DEC:  bl           0x8242f890
0x825A9DF0:  stwu         r1, -0x90(r1)
0x825A9DF4:  mr           r27, r3
0x825A9DF8:  mr           r31, r4
0x825A9DFC:  mr           r26, r5
0x825A9E00:  mr           r28, r6
0x825A9E04:  mr           r29, r7
0x825A9E08:  cmpwi        cr6, r27, 0x20
0x825A9E0C:  ble          cr6, 0x825a9ee8
0x825A9E10:  srawi        r30, r27, 2
0x825A9E14:  subf         r11, r30, r28
0x825A9E18:  slwi         r11, r11, 2
0x825A9E1C:  add          r5, r11, r29
0x825A9E20:  bl           0x825a70f8
0x825A9E24:  cmpwi        cr6, r27, 0x200
0x825A9E28:  mr           r6, r29
0x825A9E2C:  mr           r5, r28
0x825A9E30:  ble          cr6, 0x825a9ea4
0x825A9E34:  mr           r3, r30
0x825A9E38:  bl           0x825a98e8
0x825A9E3C:  slwi         r11, r30, 2
0x825A9E40:  mr           r6, r29
0x825A9E44:  mr           r5, r28
0x825A9E48:  add          r4, r11, r31
0x825A9E4C:  mr           r3, r30
0x825A9E50:  bl           0x825a99c8
0x825A9E54:  slwi         r11, r30, 3
0x825A9E58:  mr           r6, r29
0x825A9E5C:  mr           r5, r28
0x825A9E60:  add          r4, r11, r31
0x825A9E64:  mr           r3, r30
0x825A9E68:  bl           0x825a98e8
0x825A9E6C:  slwi         r11, r30, 1
0x825A9E70:  mr           r6, r29
0x825A9E74:  add          r10, r30, r11
0x825A9E78:  mr           r5, r28
0x825A9E7C:  slwi         r11, r10, 2
0x825A9E80:  mr           r3, r30
0x825A9E84:  add          r4, r11, r31
0x825A9E88:  bl           0x825a98e8
0x825A9E8C:  mr           r5, r31
0x825A9E90:  mr           r4, r26
0x825A9E94:  mr           r3, r27
0x825A9E98:  bl           0x825a62a0
0x825A9E9C:  addi         r1, r1, 0x90
0x825A9EA0:  b            0x8242f8e0
0x825A9EA4:  cmpwi        cr6, r30, 0x20
0x825A9EA8:  mr           r3, r27
0x825A9EAC:  ble          cr6, 0x825a9ecc
0x825A9EB0:  bl           0x825a9110
0x825A9EB4:  mr           r5, r31
0x825A9EB8:  mr           r4, r26
0x825A9EBC:  mr           r3, r27
0x825A9EC0:  bl           0x825a62a0
0x825A9EC4:  addi         r1, r1, 0x90
0x825A9EC8:  b            0x8242f8e0
0x825A9ECC:  bl           0x825a9070
0x825A9ED0:  mr           r5, r31
0x825A9ED4:  mr           r4, r26
0x825A9ED8:  mr           r3, r27
0x825A9EDC:  bl           0x825a62a0
0x825A9EE0:  addi         r1, r1, 0x90
0x825A9EE4:  b            0x8242f8e0
0x825A9EE8:  cmpwi        cr6, r27, 8
0x825A9EEC:  ble          cr6, 0x825a9f98
0x825A9EF0:  cmpwi        cr6, r27, 0x20
0x825A9EF4:  mr           r3, r31
0x825A9EF8:  bne          cr6, 0x825a9f18
0x825A9EFC:  addi         r9, r28, -8
0x825A9F00:  slwi         r11, r9, 2
0x825A9F04:  add          r4, r11, r29
0x825A9F08:  bl           0x825a7fe8
0x825A9F0C:  bl           0x825a68c8
0x825A9F10:  addi         r1, r1, 0x90
0x825A9F14:  b            0x8242f8e0
0x825A9F18:  mr           r4, r29
0x825A9F1C:  bl           0x825a8948
0x825A9F20:  lfs          f0, 8(r31)
0x825A9F24:  lfs          f13, 0xc(r31)
0x825A9F28:  lfs          f12, 0x10(r31)
0x825A9F2C:  lfs          f11, 0x14(r31)
0x825A9F30:  lfs          f10, 0x20(r31)
0x825A9F34:  lfs          f9, 0x24(r31)
0x825A9F38:  lfs          f8, 0x38(r31)
0x825A9F3C:  lfs          f7, 0x3c(r31)
0x825A9F40:  lfs          f6, 0x18(r31)
0x825A9F44:  lfs          f5, 0x1c(r31)
0x825A9F48:  lfs          f4, 0x28(r31)
0x825A9F4C:  lfs          f3, 0x2c(r31)
0x825A9F50:  lfs          f2, 0x30(r31)
0x825A9F54:  lfs          f1, 0x34(r31)
0x825A9F58:  stfs         f8, 8(r31)
0x825A9F5C:  stfs         f7, 0xc(r31)
0x825A9F60:  stfs         f6, 0x10(r31)
0x825A9F64:  stfs         f5, 0x14(r31)
0x825A9F68:  stfs         f4, 0x18(r31)
0x825A9F6C:  stfs         f3, 0x1c(r31)
0x825A9F70:  stfs         f2, 0x28(r31)
0x825A9F74:  stfs         f1, 0x2c(r31)
0x825A9F78:  stfs         f0, 0x20(r31)
0x825A9F7C:  stfs         f13, 0x24(r31)
0x825A9F80:  stfs         f12, 0x30(r31)
0x825A9F84:  stfs         f11, 0x34(r31)
0x825A9F88:  stfs         f10, 0x38(r31)
0x825A9F8C:  stfs         f9, 0x3c(r31)
0x825A9F90:  addi         r1, r1, 0x90
0x825A9F94:  b            0x8242f8e0
0x825A9F98:  bne          cr6, 0x825aa024
0x825A9F9C:  lfs          f13, 0x10(r31)
0x825A9FA0:  lfs          f0, 0(r31)
0x825A9FA4:  lfs          f11, 4(r31)
0x825A9FA8:  fadds        f6, f0, f13
0x825A9FAC:  lfs          f12, 0x14(r31)
0x825A9FB0:  fsubs        f0, f0, f13
0x825A9FB4:  lfs          f9, 8(r31)
0x825A9FB8:  fadds        f13, f11, f12
0x825A9FBC:  lfs          f10, 0x18(r31)
0x825A9FC0:  fsubs        f12, f11, f12
0x825A9FC4:  lfs          f7, 0xc(r31)
0x825A9FC8:  fadds        f11, f10, f9
0x825A9FCC:  lfs          f8, 0x1c(r31)
0x825A9FD0:  fsubs        f10, f9, f10
0x825A9FD4:  fadds        f9, f8, f7
0x825A9FD8:  fsubs        f8, f7, f8
0x825A9FDC:  fadds        f7, f11, f6
0x825A9FE0:  stfs         f7, 0(r31)
0x825A9FE4:  fsubs        f6, f6, f11
0x825A9FE8:  stfs         f6, 0x10(r31)
0x825A9FEC:  fadds        f5, f9, f13
0x825A9FF0:  stfs         f5, 4(r31)
0x825A9FF4:  fadds        f3, f8, f0
0x825A9FF8:  stfs         f3, 8(r31)
0x825A9FFC:  fsubs        f1, f0, f8
0x825AA000:  stfs         f1, 0x18(r31)
0x825AA004:  fsubs        f4, f13, f9
0x825AA008:  stfs         f4, 0x14(r31)
0x825AA00C:  fsubs        f2, f12, f10
0x825AA010:  stfs         f2, 0xc(r31)
0x825AA014:  fadds        f0, f10, f12
0x825AA018:  stfs         f0, 0x1c(r31)
0x825AA01C:  addi         r1, r1, 0x90
0x825AA020:  b            0x8242f8e0
0x825AA024:  cmpwi        cr6, r27, 4
0x825AA028:  bne          cr6, 0x825aa05c
0x825AA02C:  lfs          f0, 0(r31)
0x825AA030:  lfs          f13, 8(r31)
0x825AA034:  lfs          f12, 4(r31)
0x825AA038:  fsubs        f10, f0, f13
0x825AA03C:  lfs          f11, 0xc(r31)
0x825AA040:  fadds        f13, f0, f13
0x825AA044:  fsubs        f0, f12, f11
0x825AA048:  stfs         f13, 0(r31)
0x825AA04C:  fadds        f12, f12, f11
0x825AA050:  stfs         f10, 8(r31)
0x825AA054:  stfs         f12, 4(r31)
0x825AA058:  stfs         f0, 0xc(r31)
0x825AA05C:  addi         r1, r1, 0x90
0x825AA060:  b            0x8242f8e0
0x825AA064:  .align 0                       # padding
0x825AA068:  mflr         r12
0x825AA06C:  bl           0x8242f88c
0x825AA070:  stwu         r1, -0x90(r1)
0x825AA074:  mr           r27, r6
0x825AA078:  mr           r31, r3
0x825AA07C:  mr           r25, r4
0x825AA080:  mr           r28, r5
0x825AA084:  mr           r29, r7
0x825AA088:  lwz          r30, 0(r27)
0x825AA08C:  slwi         r11, r30, 2
0x825AA090:  cmpw         cr6, r31, r11
0x825AA094:  ble          cr6, 0x825aa0ac
0x825AA098:  srawi        r30, r31, 2
0x825AA09C:  mr           r5, r29
0x825AA0A0:  mr           r4, r27
0x825AA0A4:  mr           r3, r30
0x825AA0A8:  bl           0x825a5a50
0x825AA0AC:  lwz          r26, 4(r27)
0x825AA0B0:  slwi         r10, r26, 2
0x825AA0B4:  cmpw         cr6, r31, r10
0x825AA0B8:  ble          cr6, 0x825aa0d4
0x825AA0BC:  slwi         r11, r30, 2
0x825AA0C0:  srawi        r26, r31, 2
0x825AA0C4:  add          r5, r11, r29
0x825AA0C8:  mr           r4, r27
0x825AA0CC:  mr           r3, r26
0x825AA0D0:  bl           0x825a5c70
0x825AA0D4:  cmpwi        cr6, r25, 0
0x825AA0D8:  blt          cr6, 0x825aa154
0x825AA0DC:  cmpwi        cr6, r31, 4
0x825AA0E0:  ble          cr6, 0x825aa118
0x825AA0E4:  mr           r7, r29
0x825AA0E8:  mr           r6, r30
0x825AA0EC:  addi         r5, r27, 8
0x825AA0F0:  mr           r4, r28
0x825AA0F4:  mr           r3, r31
0x825AA0F8:  bl           0x825a9aa0
0x825AA0FC:  slwi         r11, r30, 2
0x825AA100:  mr           r5, r26
0x825AA104:  add          r6, r11, r29
0x825AA108:  mr           r4, r28
0x825AA10C:  mr           r3, r31
0x825AA110:  bl           0x825a8cc0
0x825AA114:  b            0x825aa134
0x825AA118:  bne          cr6, 0x825aa134
0x825AA11C:  mr           r7, r29
0x825AA120:  mr           r6, r30
0x825AA124:  addi         r5, r27, 8
0x825AA128:  mr           r4, r28
0x825AA12C:  li           r3, 4
0x825AA130:  bl           0x825a9aa0
0x825AA134:  lfs          f0, 0(r28)
0x825AA138:  lfs          f13, 4(r28)
0x825AA13C:  fsubs        f12, f0, f13
0x825AA140:  stfs         f12, 4(r28)
0x825AA144:  fadds        f0, f13, f0
0x825AA148:  stfs         f0, 0(r28)
0x825AA14C:  addi         r1, r1, 0x90
0x825AA150:  b            0x8242f8dc
0x825AA154:  lfs          f0, 0(r28)
0x825AA158:  lis          r11, -0x7e00
0x825AA15C:  lfs          f13, 4(r28)
0x825AA160:  cmpwi        cr6, r31, 4
0x825AA164:  fsubs        f12, f0, f13
0x825AA168:  lfs          f13, 0x6a40(r11)
0x825AA16C:  fmuls        f13, f12, f13
0x825AA170:  stfs         f13, 4(r28)
0x825AA174:  fsubs        f11, f0, f13
0x825AA178:  stfs         f11, 0(r28)
0x825AA17C:  ble          cr6, 0x825aa1a0
0x825AA180:  slwi         r11, r30, 2
0x825AA184:  mr           r5, r26
0x825AA188:  add          r6, r11, r29
0x825AA18C:  mr           r4, r28
0x825AA190:  mr           r3, r31
0x825AA194:  bl           0x825a8d90
0x825AA198:  mr           r3, r31
0x825AA19C:  b            0x825aa1a8
0x825AA1A0:  bne          cr6, 0x825aa1bc
0x825AA1A4:  li           r3, 4
0x825AA1A8:  mr           r7, r29
0x825AA1AC:  mr           r6, r30
0x825AA1B0:  addi         r5, r27, 8
0x825AA1B4:  mr           r4, r28
0x825AA1B8:  bl           0x825a9de8
0x825AA1BC:  addi         r1, r1, 0x90
0x825AA1C0:  b            0x8242f8dc
0x825AA1C4:  .align 0                       # padding
0x825AA1C8:  mflr         r12
0x825AA1CC:  bl           0x8242f888
0x825AA1D0:  stwu         r1, -0xa0(r1)
0x825AA1D4:  mr           r26, r6
0x825AA1D8:  mr           r31, r3
0x825AA1DC:  mr           r24, r4
0x825AA1E0:  mr           r30, r5
0x825AA1E4:  mr           r28, r7
0x825AA1E8:  lwz          r29, 0(r26)
0x825AA1EC:  slwi         r11, r29, 2
0x825AA1F0:  cmpw         cr6, r31, r11
0x825AA1F4:  ble          cr6, 0x825aa20c
0x825AA1F8:  srawi        r29, r31, 2
0x825AA1FC:  mr           r5, r28
0x825AA200:  mr           r4, r26
0x825AA204:  mr           r3, r29
0x825AA208:  bl           0x825a5a50
0x825AA20C:  lwz          r25, 4(r26)
0x825AA210:  cmpw         cr6, r31, r25
0x825AA214:  ble          cr6, 0x825aa230
0x825AA218:  slwi         r11, r29, 2
0x825AA21C:  mr           r4, r26
0x825AA220:  add          r5, r11, r28
0x825AA224:  mr           r3, r31
0x825AA228:  mr           r25, r31
0x825AA22C:  bl           0x825a5c70
0x825AA230:  cmpwi        cr6, r24, 0
0x825AA234:  bge          cr6, 0x825aa2e0
0x825AA238:  slwi         r10, r31, 2
0x825AA23C:  addi         r11, r31, -2
0x825AA240:  add          r10, r10, r30
0x825AA244:  cmpwi        cr6, r11, 2
0x825AA248:  lfs          f12, -4(r10)
0x825AA24C:  blt          cr6, 0x825aa288
0x825AA250:  slwi         r9, r11, 2
0x825AA254:  srwi         r10, r11, 1
0x825AA258:  add          r11, r9, r30
0x825AA25C:  addi         r9, r11, -4
0x825AA260:  lfs          f0, 0(r11)
0x825AA264:  addi         r10, r10, -1
0x825AA268:  cmplwi       cr6, r10, 0
0x825AA26C:  lfs          f13, 0(r9)
0x825AA270:  fadds        f11, f13, f0
0x825AA274:  stfs         f11, 0(r11)
0x825AA278:  fsubs        f10, f0, f13
0x825AA27C:  addi         r11, r11, -8
0x825AA280:  stfs         f10, 8(r9)
0x825AA284:  bne          cr6, 0x825aa25c
0x825AA288:  lfs          f0, 0(r30)
0x825AA28C:  cmpwi        cr6, r31, 4
0x825AA290:  fsubs        f9, f0, f12
0x825AA294:  stfs         f9, 4(r30)
0x825AA298:  fadds        f8, f0, f12
0x825AA29C:  stfs         f8, 0(r30)
0x825AA2A0:  ble          cr6, 0x825aa2c4
0x825AA2A4:  slwi         r11, r29, 2
0x825AA2A8:  mr           r5, r25
0x825AA2AC:  add          r6, r11, r28
0x825AA2B0:  mr           r4, r30
0x825AA2B4:  mr           r3, r31
0x825AA2B8:  bl           0x825a8d90
0x825AA2BC:  mr           r3, r31
0x825AA2C0:  b            0x825aa2cc
0x825AA2C4:  bne          cr6, 0x825aa2e0
0x825AA2C8:  li           r3, 4
0x825AA2CC:  mr           r7, r28
0x825AA2D0:  mr           r6, r29
0x825AA2D4:  addi         r5, r26, 8
0x825AA2D8:  mr           r4, r30
0x825AA2DC:  bl           0x825a9de8
0x825AA2E0:  slwi         r11, r29, 2
0x825AA2E4:  mr           r5, r25
0x825AA2E8:  add          r27, r11, r28
0x825AA2EC:  mr           r4, r30
0x825AA2F0:  mr           r3, r31
0x825AA2F4:  mr           r6, r27
0x825AA2F8:  bl           0x825a8e60
0x825AA2FC:  cmpwi        cr6, r24, 0
0x825AA300:  blt          cr6, 0x825aa3b8
0x825AA304:  cmpwi        cr6, r31, 4
0x825AA308:  ble          cr6, 0x825aa338
0x825AA30C:  mr           r7, r28
0x825AA310:  mr           r6, r29
0x825AA314:  addi         r5, r26, 8
0x825AA318:  mr           r3, r31
0x825AA31C:  bl           0x825a9aa0
0x825AA320:  mr           r6, r27
0x825AA324:  mr           r5, r25
0x825AA328:  mr           r4, r30
0x825AA32C:  mr           r3, r31
0x825AA330:  bl           0x825a8cc0
0x825AA334:  b            0x825aa354
0x825AA338:  bne          cr6, 0x825aa354
0x825AA33C:  mr           r7, r28
0x825AA340:  mr           r6, r29
0x825AA344:  addi         r5, r26, 8
0x825AA348:  mr           r4, r30
0x825AA34C:  li           r3, 4
0x825AA350:  bl           0x825a9aa0
0x825AA354:  lfs          f0, 0(r30)
0x825AA358:  cmpwi        cr6, r31, 2
0x825AA35C:  lfs          f13, 4(r30)
0x825AA360:  fadds        f7, f13, f0
0x825AA364:  stfs         f7, 0(r30)
0x825AA368:  fsubs        f12, f0, f13
0x825AA36C:  ble          cr6, 0x825aa3ac
0x825AA370:  addi         r9, r31, -3
0x825AA374:  addi         r11, r30, 8
0x825AA378:  srwi         r10, r9, 1
0x825AA37C:  addi         r10, r10, 1
0x825AA380:  addi         r9, r11, 4
0x825AA384:  lfs          f0, 0(r11)
0x825AA388:  addi         r10, r10, -1
0x825AA38C:  cmplwi       cr6, r10, 0
0x825AA390:  lfs          f13, 0(r9)
0x825AA394:  fadds        f6, f13, f0
0x825AA398:  stfs         f6, 0(r11)
0x825AA39C:  fsubs        f5, f0, f13
0x825AA3A0:  addi         r11, r11, 8
0x825AA3A4:  stfs         f5, -8(r9)
0x825AA3A8:  bne          cr6, 0x825aa380
0x825AA3AC:  slwi         r11, r31, 2
0x825AA3B0:  add          r8, r11, r30
0x825AA3B4:  stfs         f12, -4(r8)
0x825AA3B8:  addi         r1, r1, 0xa0
0x825AA3BC:  b            0x8242f8d8
0x825AA3C0:  clrlwi       r11, r4, 0x10
0x825AA3C4:  srwi         r10, r5, 1
0x825AA3C8:  slwi         r8, r11, 0x10
0x825AA3CC:  mr           r9, r3
0x825AA3D0:  cmplwi       cr6, r10, 0
0x825AA3D4:  or           r8, r8, r11
0x825AA3D8:  beq          cr6, 0x825aa3f8
0x825AA3DC:  mr           r11, r3
0x825AA3E0:  mtctr        r10
0x825AA3E4:  stw          r8, 0(r11)
0x825AA3E8:  addi         r11, r11, 4
0x825AA3EC:  bdnz         0x825aa3e4
0x825AA3F0:  slwi         r11, r10, 2
0x825AA3F4:  add          r9, r11, r3
0x825AA3F8:  clrlwi       r7, r5, 0x1f
0x825AA3FC:  cmplwi       cr6, r7, 0
0x825AA400:  beqlr        cr6
0x825AA404:  sth          r8, 0(r9)
0x825AA408:  blr          
0x825AA40C:  .align 0                       # padding
0x825AA410:  mflr         r12
0x825AA414:  bl           0x8242f878
0x825AA418:  mr           r10, r5
0x825AA41C:  addi         r11, r1, -0x170
0x825AA420:  li           r25, 8
0x825AA424:  lhz          r8, 0x10(r10)
0x825AA428:  lhz          r7, 0x30(r10)
0x825AA42C:  lhz          r5, 0x40(r10)
0x825AA430:  extsh        r31, r8
0x825AA434:  lhz          r9, 0x20(r10)
0x825AA438:  extsh        r30, r7
0x825AA43C:  extsh        r8, r5
0x825AA440:  lhz          r7, 0x50(r10)
0x825AA444:  lhz          r5, 0x60(r10)
0x825AA448:  extsh        r9, r9
0x825AA44C:  extsh        r29, r7
0x825AA450:  lhz          r28, 0x70(r10)
0x825AA454:  extsh        r7, r5
0x825AA458:  or           r5, r31, r9
0x825AA45C:  extsh        r28, r28
0x825AA460:  or           r5, r5, r30
0x825AA464:  or           r5, r5, r8
0x825AA468:  or           r5, r5, r29
0x825AA46C:  or           r5, r5, r7
0x825AA470:  or           r5, r5, r28
0x825AA474:  extsh        r5, r5
0x825AA478:  cmpwi        cr6, r5, 0
0x825AA47C:  bne          cr6, 0x825aa4c0
0x825AA480:  lhz          r9, 0(r10)
0x825AA484:  addi         r10, r10, 2
0x825AA488:  lwz          r8, 0(r6)
0x825AA48C:  addi         r6, r6, 4
0x825AA490:  extsh        r7, r9
0x825AA494:  mullw        r5, r7, r8
0x825AA498:  srawi        r9, r5, 0xb
0x825AA49C:  stw          r9, 0(r11)
0x825AA4A0:  stw          r9, 0x20(r11)
0x825AA4A4:  stw          r9, 0x40(r11)
0x825AA4A8:  stw          r9, 0x60(r11)
0x825AA4AC:  stw          r9, 0x80(r11)
0x825AA4B0:  stw          r9, 0xa0(r11)
0x825AA4B4:  stw          r9, 0xc0(r11)
0x825AA4B8:  stw          r9, 0xe0(r11)
0x825AA4BC:  b            0x825aa5ec
0x825AA4C0:  lhz          r5, 0(r10)
0x825AA4C4:  addi         r10, r10, 2
0x825AA4C8:  lwz          r27, 0(r6)
0x825AA4CC:  extsh        r5, r5
0x825AA4D0:  lwz          r26, 0x40(r6)
0x825AA4D4:  lwz          r24, 0x80(r6)
0x825AA4D8:  mullw        r5, r5, r27
0x825AA4DC:  lwz          r23, 0xc0(r6)
0x825AA4E0:  lwz          r27, 0x20(r6)
0x825AA4E4:  lwz          r22, 0x60(r6)
0x825AA4E8:  lwz          r21, 0xa0(r6)
0x825AA4EC:  lwz          r20, 0xe0(r6)
0x825AA4F0:  mullw        r9, r26, r9
0x825AA4F4:  mullw        r26, r24, r8
0x825AA4F8:  srawi        r8, r5, 0xb
0x825AA4FC:  mullw        r5, r23, r7
0x825AA500:  srawi        r9, r9, 0xb
0x825AA504:  srawi        r7, r26, 0xb
0x825AA508:  srawi        r5, r5, 0xb
0x825AA50C:  mullw        r27, r27, r31
0x825AA510:  subf         r26, r5, r9
0x825AA514:  add          r9, r5, r9
0x825AA518:  mulli        r5, r26, 0xb50
0x825AA51C:  mullw        r30, r22, r30
0x825AA520:  srawi        r26, r5, 0xb
0x825AA524:  mullw        r29, r21, r29
0x825AA528:  add          r31, r7, r8
0x825AA52C:  mullw        r28, r20, r28
0x825AA530:  srawi        r5, r27, 0xb
0x825AA534:  srawi        r30, r30, 0xb
0x825AA538:  srawi        r29, r29, 0xb
0x825AA53C:  subf         r8, r7, r8
0x825AA540:  add          r27, r9, r31
0x825AA544:  srawi        r28, r28, 0xb
0x825AA548:  subf         r7, r9, r26
0x825AA54C:  subf         r31, r9, r31
0x825AA550:  subf         r9, r30, r29
0x825AA554:  subf         r26, r28, r5
0x825AA558:  add          r30, r29, r30
0x825AA55C:  add          r29, r7, r8
0x825AA560:  add          r5, r28, r5
0x825AA564:  subf         r7, r7, r8
0x825AA568:  add          r8, r26, r9
0x825AA56C:  mulli        r28, r9, -0x14e8
0x825AA570:  add          r9, r5, r30
0x825AA574:  subf         r5, r30, r5
0x825AA578:  mulli        r8, r8, 0xec8
0x825AA57C:  srawi        r8, r8, 0xb
0x825AA580:  mulli        r5, r5, 0xb50
0x825AA584:  srawi        r28, r28, 0xb
0x825AA588:  mulli        r30, r26, 0x8a9
0x825AA58C:  srawi        r26, r5, 0xb
0x825AA590:  subf         r5, r9, r28
0x825AA594:  add          r28, r9, r27
0x825AA598:  subf         r9, r9, r27
0x825AA59C:  srawi        r30, r30, 0xb
0x825AA5A0:  addi         r6, r6, 4
0x825AA5A4:  stw          r28, 0(r11)
0x825AA5A8:  stw          r9, 0xe0(r11)
0x825AA5AC:  add          r9, r5, r8
0x825AA5B0:  subf         r5, r8, r30
0x825AA5B4:  subf         r8, r9, r26
0x825AA5B8:  add          r30, r9, r29
0x825AA5BC:  subf         r9, r9, r29
0x825AA5C0:  stw          r30, 0x20(r11)
0x825AA5C4:  stw          r9, 0xc0(r11)
0x825AA5C8:  add          r9, r5, r8
0x825AA5CC:  add          r5, r8, r7
0x825AA5D0:  subf         r8, r8, r7
0x825AA5D4:  add          r7, r9, r31
0x825AA5D8:  stw          r5, 0x40(r11)
0x825AA5DC:  subf         r5, r9, r31
0x825AA5E0:  stw          r8, 0xa0(r11)
0x825AA5E4:  stw          r7, 0x80(r11)
0x825AA5E8:  stw          r5, 0x60(r11)
0x825AA5EC:  addi         r25, r25, -1
0x825AA5F0:  addi         r11, r11, 4
0x825AA5F4:  cmpwi        cr6, r25, 0
0x825AA5F8:  bgt          cr6, 0x825aa424
0x825AA5FC:  addi         r10, r3, 1
0x825AA600:  addi         r11, r1, -0x158
0x825AA604:  li           r7, 8
0x825AA608:  lwz          r9, -4(r11)
0x825AA60C:  addi         r7, r7, -1
0x825AA610:  lwz          r8, -0xc(r11)
0x825AA614:  lwz          r3, 0(r11)
0x825AA618:  cmplwi       cr6, r7, 0
0x825AA61C:  lwz          r31, -0x10(r11)
0x825AA620:  subf         r28, r8, r9
0x825AA624:  lwz          r5, -0x14(r11)
0x825AA628:  add          r27, r8, r9
0x825AA62C:  lwz          r6, 4(r11)
0x825AA630:  subf         r8, r3, r31
0x825AA634:  add          r9, r3, r31
0x825AA638:  lwz          r30, -8(r11)
0x825AA63C:  subf         r26, r6, r5
0x825AA640:  lwz          r29, -0x18(r11)
0x825AA644:  add          r6, r5, r6
0x825AA648:  mulli        r5, r8, 0xb50
0x825AA64C:  add          r31, r26, r28
0x825AA650:  srawi        r25, r5, 0xb
0x825AA654:  subf         r3, r30, r29
0x825AA658:  add          r5, r29, r30
0x825AA65C:  mulli        r31, r31, 0xec8
0x825AA660:  mulli        r30, r28, -0x14e8
0x825AA664:  srawi        r31, r31, 0xb
0x825AA668:  srawi        r29, r30, 0xb
0x825AA66C:  subf         r30, r27, r6
0x825AA670:  add          r8, r6, r27
0x825AA674:  mulli        r30, r30, 0xb50
0x825AA678:  srawi        r28, r30, 0xb
0x825AA67C:  mulli        r27, r26, 0x8a9
0x825AA680:  add          r30, r9, r5
0x825AA684:  subf         r6, r9, r25
0x825AA688:  subf         r5, r9, r5
0x825AA68C:  subf         r9, r8, r29
0x825AA690:  srawi        r27, r27, 0xb
0x825AA694:  add          r9, r9, r31
0x825AA698:  subf         r29, r31, r27
0x825AA69C:  add          r31, r6, r3
0x825AA6A0:  subf         r6, r6, r3
0x825AA6A4:  add          r3, r8, r30
0x825AA6A8:  subf         r8, r8, r30
0x825AA6AC:  addi         r3, r3, 0x7f
0x825AA6B0:  addi         r8, r8, 0x7f
0x825AA6B4:  srawi        r3, r3, 8
0x825AA6B8:  srawi        r8, r8, 8
0x825AA6BC:  addi         r11, r11, 0x20
0x825AA6C0:  stb          r3, -1(r10)
0x825AA6C4:  add          r3, r9, r31
0x825AA6C8:  stb          r8, 6(r10)
0x825AA6CC:  subf         r8, r9, r28
0x825AA6D0:  subf         r9, r9, r31
0x825AA6D4:  addi         r3, r3, 0x7f
0x825AA6D8:  addi         r9, r9, 0x7f
0x825AA6DC:  srawi        r3, r3, 8
0x825AA6E0:  srawi        r9, r9, 8
0x825AA6E4:  stb          r3, 0(r10)
0x825AA6E8:  add          r3, r8, r6
0x825AA6EC:  stb          r9, 5(r10)
0x825AA6F0:  add          r9, r29, r8
0x825AA6F4:  subf         r8, r8, r6
0x825AA6F8:  addi         r6, r3, 0x7f
0x825AA6FC:  addi         r3, r8, 0x7f
0x825AA700:  srawi        r8, r6, 8
0x825AA704:  srawi        r6, r3, 8
0x825AA708:  stb          r8, 1(r10)
0x825AA70C:  add          r8, r9, r5
0x825AA710:  subf         r9, r9, r5
0x825AA714:  stb          r6, 4(r10)
0x825AA718:  addi         r6, r8, 0x7f
0x825AA71C:  addi         r5, r9, 0x7f
0x825AA720:  srawi        r3, r6, 8
0x825AA724:  srawi        r9, r5, 8
0x825AA728:  stb          r3, 3(r10)
0x825AA72C:  stb          r9, 2(r10)
0x825AA730:  add          r10, r10, r4
0x825AA734:  bne          cr6, 0x825aa608
0x825AA738:  b            0x8242f8c8
0x825AA73C:  .align 0                       # padding
0x825AA740:  mflr         r12
0x825AA744:  bl           0x8242f874
0x825AA748:  mr           r10, r5
0x825AA74C:  add          r9, r3, r4
0x825AA750:  slwi         r25, r4, 1
0x825AA754:  addi         r11, r1, -0x170
0x825AA758:  li           r24, 8
0x825AA75C:  lhz          r7, 0x10(r10)
0x825AA760:  lhz          r5, 0x30(r10)
0x825AA764:  lhz          r4, 0x40(r10)
0x825AA768:  extsh        r31, r7
0x825AA76C:  lhz          r8, 0x20(r10)
0x825AA770:  extsh        r30, r5
0x825AA774:  extsh        r7, r4
0x825AA778:  lhz          r5, 0x50(r10)
0x825AA77C:  lhz          r4, 0x60(r10)
0x825AA780:  extsh        r8, r8
0x825AA784:  extsh        r29, r5
0x825AA788:  lhz          r28, 0x70(r10)
0x825AA78C:  extsh        r5, r4
0x825AA790:  or           r4, r31, r8
0x825AA794:  extsh        r28, r28
0x825AA798:  or           r4, r4, r30
0x825AA79C:  or           r4, r4, r7
0x825AA7A0:  or           r4, r4, r29
0x825AA7A4:  or           r4, r4, r5
0x825AA7A8:  or           r4, r4, r28
0x825AA7AC:  extsh        r4, r4
0x825AA7B0:  cmpwi        cr6, r4, 0
0x825AA7B4:  bne          cr6, 0x825aa7f8
0x825AA7B8:  lhz          r8, 0(r10)
0x825AA7BC:  addi         r10, r10, 2
0x825AA7C0:  lwz          r7, 0(r6)
0x825AA7C4:  addi         r6, r6, 4
0x825AA7C8:  extsh        r5, r8
0x825AA7CC:  mullw        r4, r5, r7
0x825AA7D0:  srawi        r8, r4, 0xb
0x825AA7D4:  stw          r8, 0(r11)
0x825AA7D8:  stw          r8, 0x20(r11)
0x825AA7DC:  stw          r8, 0x40(r11)
0x825AA7E0:  stw          r8, 0x60(r11)
0x825AA7E4:  stw          r8, 0x80(r11)
0x825AA7E8:  stw          r8, 0xa0(r11)
0x825AA7EC:  stw          r8, 0xc0(r11)
0x825AA7F0:  stw          r8, 0xe0(r11)
0x825AA7F4:  b            0x825aa924
0x825AA7F8:  lhz          r4, 0(r10)
0x825AA7FC:  addi         r10, r10, 2
0x825AA800:  lwz          r27, 0(r6)
0x825AA804:  extsh        r4, r4
0x825AA808:  lwz          r26, 0x40(r6)
0x825AA80C:  lwz          r23, 0x80(r6)
0x825AA810:  mullw        r4, r4, r27
0x825AA814:  lwz          r22, 0xc0(r6)
0x825AA818:  lwz          r27, 0x20(r6)
0x825AA81C:  lwz          r21, 0x60(r6)
0x825AA820:  lwz          r20, 0xa0(r6)
0x825AA824:  lwz          r19, 0xe0(r6)
0x825AA828:  mullw        r8, r26, r8
0x825AA82C:  mullw        r26, r23, r7
0x825AA830:  srawi        r7, r4, 0xb
0x825AA834:  mullw        r4, r22, r5
0x825AA838:  srawi        r8, r8, 0xb
0x825AA83C:  srawi        r5, r26, 0xb
0x825AA840:  srawi        r4, r4, 0xb
0x825AA844:  mullw        r27, r27, r31
0x825AA848:  subf         r26, r4, r8
0x825AA84C:  add          r8, r4, r8
0x825AA850:  mulli        r4, r26, 0xb50
0x825AA854:  mullw        r30, r21, r30
0x825AA858:  srawi        r26, r4, 0xb
0x825AA85C:  mullw        r29, r20, r29
0x825AA860:  add          r31, r5, r7
0x825AA864:  mullw        r28, r19, r28
0x825AA868:  srawi        r4, r27, 0xb
0x825AA86C:  srawi        r30, r30, 0xb
0x825AA870:  srawi        r29, r29, 0xb
0x825AA874:  subf         r7, r5, r7
0x825AA878:  add          r27, r8, r31
0x825AA87C:  srawi        r28, r28, 0xb
0x825AA880:  subf         r5, r8, r26
0x825AA884:  subf         r31, r8, r31
0x825AA888:  subf         r8, r30, r29
0x825AA88C:  subf         r26, r28, r4
0x825AA890:  add          r30, r29, r30
0x825AA894:  add          r29, r5, r7
0x825AA898:  add          r4, r28, r4
0x825AA89C:  subf         r5, r5, r7
0x825AA8A0:  add          r7, r26, r8
0x825AA8A4:  mulli        r28, r8, -0x14e8
0x825AA8A8:  add          r8, r4, r30
0x825AA8AC:  subf         r4, r30, r4
0x825AA8B0:  mulli        r7, r7, 0xec8
0x825AA8B4:  srawi        r7, r7, 0xb
0x825AA8B8:  mulli        r4, r4, 0xb50
0x825AA8BC:  srawi        r28, r28, 0xb
0x825AA8C0:  mulli        r30, r26, 0x8a9
0x825AA8C4:  srawi        r26, r4, 0xb
0x825AA8C8:  subf         r4, r8, r28
0x825AA8CC:  add          r28, r8, r27
0x825AA8D0:  subf         r8, r8, r27
0x825AA8D4:  srawi        r30, r30, 0xb
0x825AA8D8:  addi         r6, r6, 4
0x825AA8DC:  stw          r28, 0(r11)
0x825AA8E0:  stw          r8, 0xe0(r11)
0x825AA8E4:  add          r8, r4, r7
0x825AA8E8:  subf         r4, r7, r30
0x825AA8EC:  subf         r7, r8, r26
0x825AA8F0:  add          r30, r8, r29
0x825AA8F4:  subf         r8, r8, r29
0x825AA8F8:  stw          r30, 0x20(r11)
0x825AA8FC:  stw          r8, 0xc0(r11)
0x825AA900:  add          r8, r4, r7
0x825AA904:  add          r4, r7, r5
0x825AA908:  subf         r7, r7, r5
0x825AA90C:  add          r5, r8, r31
0x825AA910:  stw          r4, 0x40(r11)
0x825AA914:  subf         r4, r8, r31
0x825AA918:  stw          r7, 0xa0(r11)
0x825AA91C:  stw          r5, 0x80(r11)
0x825AA920:  stw          r4, 0x60(r11)
0x825AA924:  addi         r24, r24, -1
0x825AA928:  addi         r11, r11, 4
0x825AA92C:  cmpwi        cr6, r24, 0
0x825AA930:  bgt          cr6, 0x825aa75c
0x825AA934:  addi         r11, r1, -0x158
0x825AA938:  addi         r10, r3, 8
0x825AA93C:  li           r6, 8
0x825AA940:  lwz          r8, -4(r11)
0x825AA944:  lwz          r7, -0xc(r11)
0x825AA948:  lwz          r3, 0(r11)
0x825AA94C:  lwz          r31, -0x10(r11)
0x825AA950:  subf         r28, r7, r8
0x825AA954:  lwz          r4, -0x14(r11)
0x825AA958:  add          r27, r7, r8
0x825AA95C:  lwz          r5, 4(r11)
0x825AA960:  subf         r7, r3, r31
0x825AA964:  add          r8, r31, r3
0x825AA968:  lwz          r30, -8(r11)
0x825AA96C:  subf         r26, r5, r4
0x825AA970:  lwz          r29, -0x18(r11)
0x825AA974:  add          r5, r4, r5
0x825AA978:  mulli        r4, r7, 0xb50
0x825AA97C:  add          r31, r26, r28
0x825AA980:  srawi        r24, r4, 0xb
0x825AA984:  subf         r3, r30, r29
0x825AA988:  add          r4, r29, r30
0x825AA98C:  mulli        r31, r31, 0xec8
0x825AA990:  mulli        r30, r28, -0x14e8
0x825AA994:  srawi        r31, r31, 0xb
0x825AA998:  srawi        r29, r30, 0xb
0x825AA99C:  subf         r30, r27, r5
0x825AA9A0:  add          r7, r5, r27
0x825AA9A4:  mulli        r30, r30, 0xb50
0x825AA9A8:  srawi        r28, r30, 0xb
0x825AA9AC:  mulli        r30, r26, 0x8a9
0x825AA9B0:  srawi        r27, r30, 0xb
0x825AA9B4:  add          r30, r8, r4
0x825AA9B8:  subf         r4, r8, r4
0x825AA9BC:  subf         r5, r8, r24
0x825AA9C0:  subf         r8, r7, r29
0x825AA9C4:  subf         r29, r31, r27
0x825AA9C8:  add          r8, r8, r31
0x825AA9CC:  add          r31, r5, r3
0x825AA9D0:  subf         r5, r5, r3
0x825AA9D4:  add          r3, r7, r30
0x825AA9D8:  subf         r7, r7, r30
0x825AA9DC:  addi         r3, r3, 0x7f
0x825AA9E0:  addi         r30, r7, 0x7f
0x825AA9E4:  srawi        r7, r3, 8
0x825AA9E8:  add          r3, r8, r31
0x825AA9EC:  rlwinm       r27, r7, 0x10, 8, 0xf
0x825AA9F0:  subf         r7, r8, r28
0x825AA9F4:  subf         r8, r8, r31
0x825AA9F8:  addi         r3, r3, 0x7f
0x825AA9FC:  addi         r8, r8, 0x7f
0x825AAA00:  srawi        r3, r3, 8
0x825AAA04:  srawi        r8, r8, 8
0x825AAA08:  srawi        r31, r30, 8
0x825AAA0C:  clrlwi       r3, r3, 0x18
0x825AAA10:  rlwinm       r30, r8, 0x10, 8, 0xf
0x825AAA14:  clrlwi       r31, r31, 0x18
0x825AAA18:  or           r8, r27, r3
0x825AAA1C:  or           r3, r30, r31
0x825AAA20:  subf         r30, r7, r5
0x825AAA24:  add          r31, r29, r7
0x825AAA28:  add          r7, r7, r5
0x825AAA2C:  add          r5, r31, r4
0x825AAA30:  addi         r29, r7, 0x7f
0x825AAA34:  subf         r7, r31, r4
0x825AAA38:  addi         r5, r5, 0x7f
0x825AAA3C:  addi         r30, r30, 0x7f
0x825AAA40:  addi         r4, r7, 0x7f
0x825AAA44:  srawi        r7, r5, 8
0x825AAA48:  srawi        r5, r30, 8
0x825AAA4C:  srawi        r31, r29, 8
0x825AAA50:  clrlwi       r5, r5, 0x18
0x825AAA54:  rlwinm       r7, r7, 0x10, 8, 0xf
0x825AAA58:  srawi        r4, r4, 8
0x825AAA5C:  or           r7, r7, r5
0x825AAA60:  clrlwi       r4, r4, 0x18
0x825AAA64:  rlwinm       r5, r31, 0x10, 8, 0xf
0x825AAA68:  slwi         r31, r8, 8
0x825AAA6C:  or           r5, r5, r4
0x825AAA70:  slwi         r4, r3, 8
0x825AAA74:  slwi         r30, r7, 8
0x825AAA78:  or           r8, r31, r8
0x825AAA7C:  or           r4, r4, r3
0x825AAA80:  or           r7, r30, r7
0x825AAA84:  slwi         r3, r5, 8
0x825AAA88:  stw          r8, -8(r10)
0x825AAA8C:  addi         r6, r6, -1
0x825AAA90:  stw          r4, 4(r10)
0x825AAA94:  or           r5, r3, r5
0x825AAA98:  stw          r7, 0(r10)
0x825AAA9C:  addi         r11, r11, 0x20
0x825AAAA0:  cmplwi       cr6, r6, 0
0x825AAAA4:  stw          r5, -4(r10)
0x825AAAA8:  add          r10, r10, r25
0x825AAAAC:  stw          r8, 0(r9)
0x825AAAB0:  stw          r5, 4(r9)
0x825AAAB4:  stw          r7, 8(r9)
0x825AAAB8:  stw          r4, 0xc(r9)
0x825AAABC:  add          r9, r25, r9
0x825AAAC0:  bne          cr6, 0x825aa940
0x825AAAC4:  b            0x8242f8c4
0x825AAAC8:  lis          r11, -0x7df5
0x825AAACC:  slwi         r10, r6, 8
0x825AAAD0:  addi         r11, r11, 0x1ca0
0x825AAAD4:  add          r6, r10, r11
0x825AAAD8:  b            0x825aa410
0x825AAADC:  .align 0                       # padding
0x825AAAE0:  lis          r11, -0x7df5
0x825AAAE4:  slwi         r10, r6, 8
0x825AAAE8:  addi         r11, r11, 0x1ca0
0x825AAAEC:  add          r6, r10, r11
0x825AAAF0:  b            0x825aa740
0x825AAAF4:  .align 0                       # padding
0x825AAAF8:  mflr         r12
0x825AAAFC:  bl           0x8242f870
0x825AAB00:  lis          r11, -0x7df5
0x825AAB04:  slwi         r10, r6, 8
0x825AAB08:  addi         r11, r11, 0x2ca8
0x825AAB0C:  mr           r9, r5
0x825AAB10:  add          r10, r10, r11
0x825AAB14:  addi         r11, r1, -0x180
0x825AAB18:  li           r23, 8
0x825AAB1C:  lhz          r31, 0x30(r9)
0x825AAB20:  lhz          r5, 0x10(r9)
0x825AAB24:  lhz          r6, 0x20(r9)
0x825AAB28:  extsh        r28, r31
0x825AAB2C:  lhz          r31, 0x50(r9)
0x825AAB30:  extsh        r29, r5
0x825AAB34:  lhz          r30, 0x60(r9)
0x825AAB38:  extsh        r6, r6
0x825AAB3C:  extsh        r27, r31
0x825AAB40:  lhz          r5, 0x40(r9)
0x825AAB44:  extsh        r31, r30
0x825AAB48:  lhz          r26, 0x70(r9)
0x825AAB4C:  or           r30, r29, r6
0x825AAB50:  extsh        r5, r5
0x825AAB54:  or           r30, r30, r28
0x825AAB58:  extsh        r26, r26
0x825AAB5C:  or           r30, r30, r5
0x825AAB60:  or           r30, r30, r27
0x825AAB64:  or           r30, r30, r31
0x825AAB68:  or           r30, r30, r26
0x825AAB6C:  extsh        r30, r30
0x825AAB70:  cmpwi        cr6, r30, 0
0x825AAB74:  bne          cr6, 0x825aabb4
0x825AAB78:  lhz          r6, 0(r9)
0x825AAB7C:  addi         r9, r9, 2
0x825AAB80:  lwz          r5, 0(r10)
0x825AAB84:  addi         r10, r10, 4
0x825AAB88:  extsh        r6, r6
0x825AAB8C:  mullw        r5, r6, r5
0x825AAB90:  srawi        r6, r5, 0xb
0x825AAB94:  stw          r6, 0(r11)
0x825AAB98:  stw          r6, 0x20(r11)
0x825AAB9C:  stw          r6, 0x40(r11)
0x825AABA0:  stw          r6, 0x80(r11)
0x825AABA4:  stw          r6, 0xa0(r11)
0x825AABA8:  stw          r6, 0xc0(r11)
0x825AABAC:  stw          r6, 0xe0(r11)
0x825AABB0:  b            0x825aacdc
0x825AABB4:  lhz          r30, 0(r9)
0x825AABB8:  addi         r9, r9, 2
0x825AABBC:  lwz          r25, 0(r10)
0x825AABC0:  extsh        r30, r30
0x825AABC4:  lwz          r24, 0x40(r10)
0x825AABC8:  lwz          r22, 0x80(r10)
0x825AABCC:  mullw        r30, r30, r25
0x825AABD0:  lwz          r21, 0xc0(r10)
0x825AABD4:  lwz          r25, 0x20(r10)
0x825AABD8:  lwz          r20, 0x60(r10)
0x825AABDC:  lwz          r19, 0xa0(r10)
0x825AABE0:  lwz          r18, 0xe0(r10)
0x825AABE4:  mullw        r6, r24, r6
0x825AABE8:  mullw        r24, r22, r5
0x825AABEC:  srawi        r5, r30, 0xb
0x825AABF0:  mullw        r30, r21, r31
0x825AABF4:  srawi        r6, r6, 0xb
0x825AABF8:  srawi        r31, r24, 0xb
0x825AABFC:  srawi        r30, r30, 0xb
0x825AAC00:  mullw        r25, r25, r29
0x825AAC04:  subf         r24, r30, r6
0x825AAC08:  add          r6, r30, r6
0x825AAC0C:  mulli        r30, r24, 0xb50
0x825AAC10:  mullw        r28, r20, r28
0x825AAC14:  srawi        r24, r30, 0xb
0x825AAC18:  mullw        r27, r19, r27
0x825AAC1C:  add          r29, r31, r5
0x825AAC20:  mullw        r26, r18, r26
0x825AAC24:  srawi        r30, r25, 0xb
0x825AAC28:  srawi        r28, r28, 0xb
0x825AAC2C:  srawi        r27, r27, 0xb
0x825AAC30:  subf         r5, r31, r5
0x825AAC34:  add          r25, r6, r29
0x825AAC38:  srawi        r26, r26, 0xb
0x825AAC3C:  subf         r31, r6, r24
0x825AAC40:  subf         r29, r6, r29
0x825AAC44:  subf         r6, r28, r27
0x825AAC48:  subf         r24, r26, r30
0x825AAC4C:  add          r28, r27, r28
0x825AAC50:  add          r27, r31, r5
0x825AAC54:  add          r30, r26, r30
0x825AAC58:  subf         r31, r31, r5
0x825AAC5C:  add          r5, r24, r6
0x825AAC60:  mulli        r26, r6, -0x14e8
0x825AAC64:  add          r6, r30, r28
0x825AAC68:  subf         r30, r28, r30
0x825AAC6C:  mulli        r5, r5, 0xec8
0x825AAC70:  srawi        r5, r5, 0xb
0x825AAC74:  mulli        r30, r30, 0xb50
0x825AAC78:  srawi        r26, r26, 0xb
0x825AAC7C:  mulli        r28, r24, 0x8a9
0x825AAC80:  srawi        r24, r30, 0xb
0x825AAC84:  subf         r30, r6, r26
0x825AAC88:  add          r26, r6, r25
0x825AAC8C:  subf         r6, r6, r25
0x825AAC90:  srawi        r28, r28, 0xb
0x825AAC94:  addi         r10, r10, 4
0x825AAC98:  stw          r26, 0(r11)
0x825AAC9C:  stw          r6, 0xe0(r11)
0x825AACA0:  add          r6, r30, r5
0x825AACA4:  subf         r30, r5, r28
0x825AACA8:  subf         r5, r6, r24
0x825AACAC:  add          r28, r6, r27
0x825AACB0:  subf         r6, r6, r27
0x825AACB4:  stw          r28, 0x20(r11)
0x825AACB8:  stw          r6, 0xc0(r11)
0x825AACBC:  add          r6, r30, r5
0x825AACC0:  add          r30, r5, r31
0x825AACC4:  subf         r5, r5, r31
0x825AACC8:  stw          r30, 0x40(r11)
0x825AACCC:  stw          r5, 0xa0(r11)
0x825AACD0:  add          r5, r6, r29
0x825AACD4:  subf         r6, r6, r29
0x825AACD8:  stw          r5, 0x80(r11)
0x825AACDC:  addi         r23, r23, -1
0x825AACE0:  stw          r6, 0x60(r11)
0x825AACE4:  addi         r11, r11, 4
0x825AACE8:  cmpwi        cr6, r23, 0
0x825AACEC:  bgt          cr6, 0x825aab1c
0x825AACF0:  addi         r9, r3, 1
0x825AACF4:  addi         r10, r7, 1
0x825AACF8:  addi         r11, r1, -0x168
0x825AACFC:  li           r5, 8
0x825AAD00:  lwz          r7, -4(r11)
0x825AAD04:  addi         r5, r5, -1
0x825AAD08:  lwz          r6, -0xc(r11)
0x825AAD0C:  lwz          r30, 0(r11)
0x825AAD10:  lwz          r29, -0x10(r11)
0x825AAD14:  subf         r26, r6, r7
0x825AAD18:  add          r25, r6, r7
0x825AAD1C:  lwz          r31, -0x14(r11)
0x825AAD20:  subf         r6, r30, r29
0x825AAD24:  lwz          r3, 4(r11)
0x825AAD28:  add          r7, r29, r30
0x825AAD2C:  lwz          r28, -8(r11)
0x825AAD30:  subf         r24, r3, r31
0x825AAD34:  lwz          r27, -0x18(r11)
0x825AAD38:  mulli        r6, r6, 0xb50
0x825AAD3C:  lbz          r23, -1(r10)
0x825AAD40:  add          r3, r31, r3
0x825AAD44:  srawi        r22, r6, 0xb
0x825AAD48:  add          r29, r24, r26
0x825AAD4C:  add          r6, r3, r25
0x825AAD50:  subf         r3, r25, r3
0x825AAD54:  mulli        r29, r29, 0xec8
0x825AAD58:  mulli        r21, r26, -0x14e8
0x825AAD5C:  add          r31, r27, r28
0x825AAD60:  mulli        r3, r3, 0xb50
0x825AAD64:  srawi        r29, r29, 0xb
0x825AAD68:  subf         r30, r28, r27
0x825AAD6C:  srawi        r26, r21, 0xb
0x825AAD70:  mulli        r25, r24, 0x8a9
0x825AAD74:  add          r27, r7, r31
0x825AAD78:  srawi        r24, r3, 0xb
0x825AAD7C:  subf         r3, r7, r31
0x825AAD80:  subf         r28, r7, r22
0x825AAD84:  subf         r7, r6, r26
0x825AAD88:  add          r26, r6, r27
0x825AAD8C:  add          r31, r28, r30
0x825AAD90:  addi         r26, r26, 0x7f
0x825AAD94:  subf         r30, r28, r30
0x825AAD98:  subf         r28, r6, r27
0x825AAD9C:  srawi        r25, r25, 0xb
0x825AADA0:  srawi        r6, r26, 8
0x825AADA4:  addi         r28, r28, 0x7f
0x825AADA8:  add          r27, r6, r23
0x825AADAC:  srawi        r6, r28, 8
0x825AADB0:  add          r7, r7, r29
0x825AADB4:  subf         r29, r29, r25
0x825AADB8:  addi         r11, r11, 0x20
0x825AADBC:  stb          r27, -1(r9)
0x825AADC0:  add          r27, r7, r31
0x825AADC4:  lbz          r28, 6(r10)
0x825AADC8:  add          r6, r6, r28
0x825AADCC:  stb          r6, 6(r9)
0x825AADD0:  subf         r6, r7, r24
0x825AADD4:  subf         r7, r7, r31
0x825AADD8:  lbz          r28, 0(r10)
0x825AADDC:  addi         r31, r27, 0x7f
0x825AADE0:  addi         r7, r7, 0x7f
0x825AADE4:  srawi        r31, r31, 8
0x825AADE8:  srawi        r7, r7, 8
0x825AADEC:  add          r31, r31, r28
0x825AADF0:  stb          r31, 0(r9)
0x825AADF4:  lbz          r31, 5(r10)
0x825AADF8:  add          r7, r7, r31
0x825AADFC:  stb          r7, 5(r9)
0x825AAE00:  add          r7, r6, r30
0x825AAE04:  subf         r30, r6, r30
0x825AAE08:  lbz          r31, 1(r10)
0x825AAE0C:  addi         r28, r7, 0x7f
0x825AAE10:  add          r7, r29, r6
0x825AAE14:  srawi        r6, r28, 8
0x825AAE18:  addi         r30, r30, 0x7f
0x825AAE1C:  add          r6, r6, r31
0x825AAE20:  srawi        r31, r30, 8
0x825AAE24:  stb          r6, 1(r9)
0x825AAE28:  lbz          r6, 4(r10)
0x825AAE2C:  mr           r30, r6
0x825AAE30:  subf         r6, r7, r3
0x825AAE34:  add          r31, r31, r30
0x825AAE38:  addi         r6, r6, 0x7f
0x825AAE3C:  add          r7, r7, r3
0x825AAE40:  srawi        r6, r6, 8
0x825AAE44:  stb          r31, 4(r9)
0x825AAE48:  addi         r3, r7, 0x7f
0x825AAE4C:  lbz          r31, 2(r10)
0x825AAE50:  cmplwi       cr6, r5, 0
0x825AAE54:  srawi        r7, r3, 8
0x825AAE58:  add          r6, r6, r31
0x825AAE5C:  stb          r6, 2(r9)
0x825AAE60:  lbz          r6, 3(r10)
0x825AAE64:  add          r10, r10, r8
0x825AAE68:  add          r3, r7, r6
0x825AAE6C:  stb          r3, 3(r9)
0x825AAE70:  add          r9, r9, r4
0x825AAE74:  bne          cr6, 0x825aad00
0x825AAE78:  b            0x8242f8c0
0x825AAE7C:  .align 0                       # padding
0x825AAE80:  mflr         r12
0x825AAE84:  bl           0x8242f870
0x825AAE88:  li           r21, 0
0x825AAE8C:  addi         r11, r1, -0x178
0x825AAE90:  mr           r9, r21
0x825AAE94:  li           r10, 0xf
0x825AAE98:  std          r21, -0x180(r1)
0x825AAE9C:  mtctr        r10
0x825AAEA0:  std          r9, 0(r11)
0x825AAEA4:  addi         r11, r11, 8
0x825AAEA8:  bdnz         0x825aaea0
0x825AAEAC:  lwz          r11, 8(r4)
0x825AAEB0:  lwz          r9, 0(r4)
0x825AAEB4:  lwz          r5, 4(r4)
0x825AAEB8:  cmplwi       cr6, r11, 4
0x825AAEBC:  bge          cr6, 0x825aaeec
0x825AAEC0:  lwz          r10, 0(r5)
0x825AAEC4:  subfic       r8, r11, 4
0x825AAEC8:  clrlwi       r6, r9, 0x18
0x825AAECC:  addi         r5, r5, 4
0x825AAED0:  slw          r9, r10, r11
0x825AAED4:  addi         r11, r11, 0x1c
0x825AAED8:  clrlwi       r9, r9, 0x18
0x825AAEDC:  srw          r10, r10, r8
0x825AAEE0:  or           r7, r9, r6
0x825AAEE4:  clrlwi       r25, r7, 0x1c
0x825AAEE8:  b            0x825aaef8
0x825AAEEC:  srwi         r10, r9, 4
0x825AAEF0:  clrlwi       r25, r9, 0x1c
0x825AAEF4:  addi         r11, r11, -4
0x825AAEF8:  addi         r9, r25, -1
0x825AAEFC:  li           r22, 1
0x825AAF00:  li           r7, 0x60
0x825AAF04:  li           r8, 0x10
0x825AAF08:  li           r6, 0xb0
0x825AAF0C:  addi         r24, r1, -0xbc
0x825AAF10:  addi         r23, r1, -0xb6
0x825AAF14:  stb          r7, -0xbb(r1)
0x825AAF18:  li           r7, 0xb
0x825AAF1C:  stb          r8, -0xbc(r1)
0x825AAF20:  li           r8, 7
0x825AAF24:  stb          r6, -0xba(r1)
0x825AAF28:  li           r6, 0xf
0x825AAF2C:  cmplwi       cr6, r25, 1
0x825AAF30:  stb          r7, -0xb8(r1)
0x825AAF34:  mr           r7, r25
0x825AAF38:  stb          r8, -0xb9(r1)
0x825AAF3C:  stb          r6, -0xb7(r1)
0x825AAF40:  slw          r27, r22, r9
0x825AAF44:  ble          cr6, 0x825ab398
0x825AAF48:  li           r26, -1
0x825AAF4C:  addi         r31, r7, 1
0x825AAF50:  mr           r30, r24
0x825AAF54:  addi         r9, r31, -2
0x825AAF58:  cmplw        cr6, r24, r23
0x825AAF5C:  slw          r29, r22, r9
0x825AAF60:  addi         r28, r29, -1
0x825AAF64:  bge          cr6, 0x825ab388
0x825AAF68:  lbz          r8, 0(r30)
0x825AAF6C:  cmplwi       cr6, r8, 0
0x825AAF70:  beq          cr6, 0x825ab37c
0x825AAF74:  cmplwi       cr6, r11, 0
0x825AAF78:  bne          cr6, 0x825aaf90
0x825AAF7C:  lwz          r9, 0(r5)
0x825AAF80:  li           r11, 0x1f
0x825AAF84:  addi         r5, r5, 4
0x825AAF88:  srwi         r10, r9, 1
0x825AAF8C:  b            0x825aaf9c
0x825AAF90:  mr           r9, r10
0x825AAF94:  srwi         r10, r10, 1
0x825AAF98:  addi         r11, r11, -1
0x825AAF9C:  clrlwi       r6, r9, 0x1f
0x825AAFA0:  cmplwi       cr6, r6, 0
0x825AAFA4:  beq          cr6, 0x825ab37c
0x825AAFA8:  clrlwi       r9, r8, 0x1e
0x825AAFAC:  cmplwi       cr6, r9, 3
0x825AAFB0:  bgt          cr6, 0x825ab37c
0x825AAFB4:  lis          r12, -0x7da5
0x825AAFB8:  addi         r12, r12, -0x5034
0x825AAFBC:  slwi         r0, r9, 2
0x825AAFC0:  lwzx         r0, r12, r0
0x825AAFC4:  mtctr        r0
0x825AAFC8:  bctr         
0x825AAFCC:  lwz          r18, -0x5024(r26)
0x825AAFD0:  lwz          r18, -0x5010(r26)
0x825AAFD4:  lwz          r18, -0x4fd8(r26)
0x825AAFD8:  lwz          r18, -0x4cfc(r26)
0x825AAFDC:  srwi         r6, r8, 2
0x825AAFE0:  slwi         r9, r6, 2
0x825AAFE4:  addi         r9, r9, 0x11
0x825AAFE8:  stb          r9, 0(r30)
0x825AAFEC:  b            0x825ab034
0x825AAFF0:  rlwinm       r9, r8, 0, 0, 0x1d
0x825AAFF4:  addi         r6, r9, 2
0x825AAFF8:  addi         r8, r9, 0x12
0x825AAFFC:  addi         r20, r9, 0x22
0x825AB000:  addi         r9, r9, 0x32
0x825AB004:  stb          r6, 0(r30)
0x825AB008:  mr           r6, r9
0x825AB00C:  addi         r9, r23, 1
0x825AB010:  stb          r8, 0(r23)
0x825AB014:  stb          r20, 0(r9)
0x825AB018:  addi         r9, r9, 1
0x825AB01C:  addi         r23, r9, 1
0x825AB020:  stb          r6, 0(r9)
0x825AB024:  b            0x825ab380
0x825AB028:  stb          r21, 0(r30)
0x825AB02C:  srwi         r6, r8, 2
0x825AB030:  addi         r30, r30, 1
0x825AB034:  cmplwi       cr6, r11, 0
0x825AB038:  bne          cr6, 0x825ab190
0x825AB03C:  lwz          r10, 0(r5)
0x825AB040:  addi         r5, r5, 4
0x825AB044:  clrlwi       r11, r10, 0x1f
0x825AB048:  cmplwi       cr6, r11, 0
0x825AB04C:  bne          cr6, 0x825ab174
0x825AB050:  srwi         r9, r10, 1
0x825AB054:  subfic       r11, r7, 0x1f
0x825AB058:  srw          r10, r10, r31
0x825AB05C:  and          r20, r9, r29
0x825AB060:  and          r8, r9, r28
0x825AB064:  cmplwi       cr6, r20, 0
0x825AB068:  or           r9, r8, r27
0x825AB06C:  beq          cr6, 0x825ab074
0x825AB070:  neg          r9, r9
0x825AB074:  slwi         r8, r6, 1
0x825AB078:  addi         r20, r1, -0x180
0x825AB07C:  sthx         r9, r8, r20
0x825AB080:  addi         r6, r6, 1
0x825AB084:  cmplwi       cr6, r11, 0
0x825AB088:  bne          cr6, 0x825ab1f4
0x825AB08C:  lwz          r10, 0(r5)
0x825AB090:  addi         r5, r5, 4
0x825AB094:  clrlwi       r11, r10, 0x1f
0x825AB098:  cmplwi       cr6, r11, 0
0x825AB09C:  bne          cr6, 0x825ab1d8
0x825AB0A0:  srwi         r9, r10, 1
0x825AB0A4:  subfic       r11, r7, 0x1f
0x825AB0A8:  srw          r10, r10, r31
0x825AB0AC:  and          r20, r9, r29
0x825AB0B0:  and          r8, r9, r28
0x825AB0B4:  cmplwi       cr6, r20, 0
0x825AB0B8:  or           r9, r8, r27
0x825AB0BC:  beq          cr6, 0x825ab0c4
0x825AB0C0:  neg          r9, r9
0x825AB0C4:  slwi         r8, r6, 1
0x825AB0C8:  addi         r20, r1, -0x180
0x825AB0CC:  sthx         r9, r8, r20
0x825AB0D0:  addi         r6, r6, 1
0x825AB0D4:  cmplwi       cr6, r11, 0
0x825AB0D8:  bne          cr6, 0x825ab258
0x825AB0DC:  lwz          r10, 0(r5)
0x825AB0E0:  addi         r5, r5, 4
0x825AB0E4:  clrlwi       r11, r10, 0x1f
0x825AB0E8:  cmplwi       cr6, r11, 0
0x825AB0EC:  bne          cr6, 0x825ab23c
0x825AB0F0:  srwi         r9, r10, 1
0x825AB0F4:  subfic       r11, r7, 0x1f
0x825AB0F8:  srw          r10, r10, r31
0x825AB0FC:  and          r20, r9, r29
0x825AB100:  and          r8, r9, r28
0x825AB104:  cmplwi       cr6, r20, 0
0x825AB108:  or           r9, r8, r27
0x825AB10C:  beq          cr6, 0x825ab114
0x825AB110:  neg          r9, r9
0x825AB114:  slwi         r8, r6, 1
0x825AB118:  addi         r20, r1, -0x180
0x825AB11C:  sthx         r9, r8, r20
0x825AB120:  addi         r6, r6, 1
0x825AB124:  cmplwi       cr6, r11, 0
0x825AB128:  bne          cr6, 0x825ab2bc
0x825AB12C:  lwz          r10, 0(r5)
0x825AB130:  addi         r5, r5, 4
0x825AB134:  clrlwi       r11, r10, 0x1f
0x825AB138:  cmplwi       cr6, r11, 0
0x825AB13C:  bne          cr6, 0x825ab2a0
0x825AB140:  srwi         r9, r10, 1
0x825AB144:  subfic       r11, r7, 0x1f
0x825AB148:  srw          r10, r10, r31
0x825AB14C:  and          r20, r9, r29
0x825AB150:  and          r8, r9, r28
0x825AB154:  cmplwi       cr6, r20, 0
0x825AB158:  or           r9, r8, r27
0x825AB15C:  beq          cr6, 0x825ab164
0x825AB160:  neg          r9, r9
0x825AB164:  slwi         r8, r6, 1
0x825AB168:  addi         r6, r1, -0x180
0x825AB16C:  sthx         r9, r8, r6
0x825AB170:  b            0x825ab380
0x825AB174:  li           r11, 0x1f
0x825AB178:  slwi         r9, r6, 2
0x825AB17C:  addi         r24, r24, -1
0x825AB180:  addi         r8, r9, 3
0x825AB184:  srwi         r10, r10, 1
0x825AB188:  stb          r8, 0(r24)
0x825AB18C:  b            0x825ab080
0x825AB190:  clrlwi       r9, r10, 0x1f
0x825AB194:  addi         r11, r11, -1
0x825AB198:  cmplwi       cr6, r9, 0
0x825AB19C:  bne          cr6, 0x825ab178
0x825AB1A0:  cmplw        cr6, r11, r7
0x825AB1A4:  srwi         r9, r10, 1
0x825AB1A8:  bge          cr6, 0x825ab1d0
0x825AB1AC:  lwz          r8, 0(r5)
0x825AB1B0:  subf         r20, r11, r7
0x825AB1B4:  subf         r10, r7, r11
0x825AB1B8:  addi         r5, r5, 4
0x825AB1BC:  slw          r19, r8, r11
0x825AB1C0:  addi         r11, r10, 0x20
0x825AB1C4:  or           r9, r19, r9
0x825AB1C8:  srw          r10, r8, r20
0x825AB1CC:  b            0x825ab05c
0x825AB1D0:  subf         r11, r7, r11
0x825AB1D4:  b            0x825ab058
0x825AB1D8:  li           r11, 0x1f
0x825AB1DC:  slwi         r9, r6, 2
0x825AB1E0:  addi         r24, r24, -1
0x825AB1E4:  addi         r8, r9, 3
0x825AB1E8:  srwi         r10, r10, 1
0x825AB1EC:  stb          r8, 0(r24)
0x825AB1F0:  b            0x825ab0d0
0x825AB1F4:  clrlwi       r9, r10, 0x1f
0x825AB1F8:  addi         r11, r11, -1
0x825AB1FC:  cmplwi       cr6, r9, 0
0x825AB200:  bne          cr6, 0x825ab1dc
0x825AB204:  cmplw        cr6, r11, r7
0x825AB208:  srwi         r9, r10, 1
0x825AB20C:  bge          cr6, 0x825ab234
0x825AB210:  lwz          r8, 0(r5)
0x825AB214:  subf         r20, r11, r7
0x825AB218:  subf         r10, r7, r11
0x825AB21C:  addi         r5, r5, 4
0x825AB220:  slw          r19, r8, r11
0x825AB224:  addi         r11, r10, 0x20
0x825AB228:  or           r9, r19, r9
0x825AB22C:  srw          r10, r8, r20
0x825AB230:  b            0x825ab0ac
0x825AB234:  subf         r11, r7, r11
0x825AB238:  b            0x825ab0a8
0x825AB23C:  li           r11, 0x1f
0x825AB240:  slwi         r9, r6, 2
0x825AB244:  addi         r24, r24, -1
0x825AB248:  addi         r8, r9, 3
0x825AB24C:  srwi         r10, r10, 1
0x825AB250:  stb          r8, 0(r24)
0x825AB254:  b            0x825ab120
0x825AB258:  clrlwi       r9, r10, 0x1f
0x825AB25C:  addi         r11, r11, -1
0x825AB260:  cmplwi       cr6, r9, 0
0x825AB264:  bne          cr6, 0x825ab240
0x825AB268:  cmplw        cr6, r11, r7
0x825AB26C:  srwi         r9, r10, 1
0x825AB270:  bge          cr6, 0x825ab298
0x825AB274:  lwz          r8, 0(r5)
0x825AB278:  subf         r20, r11, r7
0x825AB27C:  subf         r10, r7, r11
0x825AB280:  addi         r5, r5, 4
0x825AB284:  slw          r19, r8, r11
0x825AB288:  addi         r11, r10, 0x20
0x825AB28C:  or           r9, r19, r9
0x825AB290:  srw          r10, r8, r20
0x825AB294:  b            0x825ab0fc
0x825AB298:  subf         r11, r7, r11
0x825AB29C:  b            0x825ab0f8
0x825AB2A0:  li           r11, 0x1f
0x825AB2A4:  slwi         r9, r6, 2
0x825AB2A8:  addi         r24, r24, -1
0x825AB2AC:  addi         r8, r9, 3
0x825AB2B0:  srwi         r10, r10, 1
0x825AB2B4:  stb          r8, 0(r24)
0x825AB2B8:  b            0x825ab380
0x825AB2BC:  clrlwi       r9, r10, 0x1f
0x825AB2C0:  addi         r11, r11, -1
0x825AB2C4:  cmplwi       cr6, r9, 0
0x825AB2C8:  bne          cr6, 0x825ab2a4
0x825AB2CC:  cmplw        cr6, r11, r7
0x825AB2D0:  srwi         r9, r10, 1
0x825AB2D4:  bge          cr6, 0x825ab2fc
0x825AB2D8:  lwz          r8, 0(r5)
0x825AB2DC:  subf         r20, r11, r7
0x825AB2E0:  subf         r10, r7, r11
0x825AB2E4:  addi         r5, r5, 4
0x825AB2E8:  slw          r19, r8, r11
0x825AB2EC:  addi         r11, r10, 0x20
0x825AB2F0:  or           r9, r19, r9
0x825AB2F4:  srw          r10, r8, r20
0x825AB2F8:  b            0x825ab14c
0x825AB2FC:  subf         r11, r7, r11
0x825AB300:  b            0x825ab148
0x825AB304:  srwi         r6, r8, 2
0x825AB308:  cmplw        cr6, r11, r7
0x825AB30C:  bge          cr6, 0x825ab340
0x825AB310:  lwz          r9, 0(r5)
0x825AB314:  subfic       r20, r7, 0x20
0x825AB318:  subf         r19, r11, r7
0x825AB31C:  subf         r8, r7, r11
0x825AB320:  addi         r5, r5, 4
0x825AB324:  slw          r18, r9, r11
0x825AB328:  srw          r20, r26, r20
0x825AB32C:  or           r10, r18, r10
0x825AB330:  addi         r11, r8, 0x20
0x825AB334:  and          r8, r20, r10
0x825AB338:  srw          r10, r9, r19
0x825AB33C:  b            0x825ab354
0x825AB340:  subfic       r9, r7, 0x20
0x825AB344:  subf         r11, r7, r11
0x825AB348:  srw          r8, r26, r9
0x825AB34C:  and          r8, r8, r10
0x825AB350:  srw          r10, r10, r7
0x825AB354:  and          r9, r8, r28
0x825AB358:  and          r8, r8, r29
0x825AB35C:  or           r9, r9, r27
0x825AB360:  cmplwi       cr6, r8, 0
0x825AB364:  beq          cr6, 0x825ab36c
0x825AB368:  neg          r9, r9
0x825AB36C:  slwi         r8, r6, 1
0x825AB370:  stb          r21, 0(r30)
0x825AB374:  addi         r6, r1, -0x180
0x825AB378:  sthx         r9, r8, r6
0x825AB37C:  addi         r30, r30, 1
0x825AB380:  cmplw        cr6, r30, r23
0x825AB384:  blt          cr6, 0x825aaf68
0x825AB388:  addi         r7, r7, -1
0x825AB38C:  srawi        r27, r27, 1
0x825AB390:  cmplwi       cr6, r7, 1
0x825AB394:  bgt          cr6, 0x825aaf4c
0x825AB398:  cmplwi       cr6, r25, 0
0x825AB39C:  beq          cr6, 0x825ab6fc
0x825AB3A0:  mr           r6, r24
0x825AB3A4:  cmplw        cr6, r24, r23
0x825AB3A8:  bge          cr6, 0x825ab6fc
0x825AB3AC:  lbz          r8, 0(r6)
0x825AB3B0:  cmplwi       cr6, r8, 0
0x825AB3B4:  beq          cr6, 0x825ab6f0
0x825AB3B8:  cmplwi       cr6, r11, 0
0x825AB3BC:  bne          cr6, 0x825ab3d4
0x825AB3C0:  lwz          r9, 0(r5)
0x825AB3C4:  li           r11, 0x1f
0x825AB3C8:  addi         r5, r5, 4
0x825AB3CC:  srwi         r10, r9, 1
0x825AB3D0:  b            0x825ab3e0
0x825AB3D4:  mr           r9, r10
0x825AB3D8:  srwi         r10, r10, 1
0x825AB3DC:  addi         r11, r11, -1
0x825AB3E0:  clrlwi       r9, r9, 0x1f
0x825AB3E4:  cmplwi       cr6, r9, 0
0x825AB3E8:  beq          cr6, 0x825ab6f0
0x825AB3EC:  clrlwi       r9, r8, 0x1e
0x825AB3F0:  cmplwi       cr6, r9, 3
0x825AB3F4:  bgt          cr6, 0x825ab6f0
0x825AB3F8:  lis          r12, -0x7da5
0x825AB3FC:  addi         r12, r12, -0x4bf0
0x825AB400:  slwi         r0, r9, 2
0x825AB404:  lwzx         r0, r12, r0
0x825AB408:  mtctr        r0
0x825AB40C:  bctr         
0x825AB410:  lwz          r18, -0x4be0(r26)
0x825AB414:  lwz          r18, -0x4bcc(r26)
0x825AB418:  lwz          r18, -0x4b94(r26)
0x825AB41C:  lwz          r18, -0x4958(r26)
0x825AB420:  srwi         r7, r8, 2
0x825AB424:  slwi         r9, r7, 2
0x825AB428:  addi         r8, r9, 0x11
0x825AB42C:  stb          r8, 0(r6)
0x825AB430:  b            0x825ab478
0x825AB434:  rlwinm       r9, r8, 0, 0, 0x1d
0x825AB438:  addi         r8, r9, 2
0x825AB43C:  addi         r7, r9, 0x12
0x825AB440:  addi         r31, r9, 0x22
0x825AB444:  addi         r9, r9, 0x32
0x825AB448:  stb          r8, 0(r6)
0x825AB44C:  mr           r8, r9
0x825AB450:  addi         r9, r23, 1
0x825AB454:  stb          r7, 0(r23)
0x825AB458:  stb          r31, 0(r9)
0x825AB45C:  addi         r9, r9, 1
0x825AB460:  addi         r23, r9, 1
0x825AB464:  stb          r8, 0(r9)
0x825AB468:  b            0x825ab6f4
0x825AB46C:  stb          r21, 0(r6)
0x825AB470:  srwi         r7, r8, 2
0x825AB474:  addi         r6, r6, 1
0x825AB478:  cmplwi       cr6, r11, 0
0x825AB47C:  bne          cr6, 0x825ab494
0x825AB480:  lwz          r8, 0(r5)
0x825AB484:  li           r9, 0x1f
0x825AB488:  addi         r5, r5, 4
0x825AB48C:  srwi         r10, r8, 1
0x825AB490:  b            0x825ab4a0
0x825AB494:  mr           r8, r10
0x825AB498:  srwi         r10, r10, 1
0x825AB49C:  addi         r9, r11, -1
0x825AB4A0:  clrlwi       r11, r8, 0x1f
0x825AB4A4:  cmplwi       cr6, r11, 0
0x825AB4A8:  beq          cr6, 0x825ab4c0
0x825AB4AC:  slwi         r11, r7, 2
0x825AB4B0:  addi         r24, r24, -1
0x825AB4B4:  addi         r8, r11, 3
0x825AB4B8:  stb          r8, 0(r24)
0x825AB4BC:  b            0x825ab500
0x825AB4C0:  cmplwi       cr6, r9, 0
0x825AB4C4:  bne          cr6, 0x825ab4dc
0x825AB4C8:  lwz          r11, 0(r5)
0x825AB4CC:  li           r9, 0x1f
0x825AB4D0:  addi         r5, r5, 4
0x825AB4D4:  srwi         r10, r11, 1
0x825AB4D8:  b            0x825ab4e8
0x825AB4DC:  mr           r11, r10
0x825AB4E0:  srwi         r10, r10, 1
0x825AB4E4:  addi         r9, r9, -1
0x825AB4E8:  clrlwi       r8, r11, 0x1f
0x825AB4EC:  addi         r31, r1, -0x180
0x825AB4F0:  neg          r11, r8
0x825AB4F4:  slwi         r8, r7, 1
0x825AB4F8:  rlwimi       r11, r22, 0, 0x1f, 0xf
0x825AB4FC:  sthx         r11, r8, r31
0x825AB500:  addi         r7, r7, 1
0x825AB504:  cmplwi       cr6, r9, 0
0x825AB508:  bne          cr6, 0x825ab520
0x825AB50C:  lwz          r8, 0(r5)
0x825AB510:  li           r10, 0x1f
0x825AB514:  addi         r5, r5, 4
0x825AB518:  srwi         r11, r8, 1
0x825AB51C:  b            0x825ab52c
0x825AB520:  mr           r8, r10
0x825AB524:  srwi         r11, r10, 1
0x825AB528:  addi         r10, r9, -1
0x825AB52C:  clrlwi       r9, r8, 0x1f
0x825AB530:  cmplwi       cr6, r9, 0
0x825AB534:  beq          cr6, 0x825ab54c
0x825AB538:  slwi         r9, r7, 2
0x825AB53C:  addi         r24, r24, -1
0x825AB540:  addi         r8, r9, 3
0x825AB544:  stb          r8, 0(r24)
0x825AB548:  b            0x825ab58c
0x825AB54C:  cmplwi       cr6, r10, 0
0x825AB550:  bne          cr6, 0x825ab568
0x825AB554:  lwz          r9, 0(r5)
0x825AB558:  li           r10, 0x1f
0x825AB55C:  addi         r5, r5, 4
0x825AB560:  srwi         r11, r9, 1
0x825AB564:  b            0x825ab574
0x825AB568:  mr           r9, r11
0x825AB56C:  srwi         r11, r11, 1
0x825AB570:  addi         r10, r10, -1
0x825AB574:  clrlwi       r8, r9, 0x1f
0x825AB578:  addi         r31, r1, -0x180
0x825AB57C:  neg          r9, r8
0x825AB580:  slwi         r8, r7, 1
0x825AB584:  rlwimi       r9, r22, 0, 0x1f, 0xf
0x825AB588:  sthx         r9, r8, r31
0x825AB58C:  addi         r7, r7, 1
0x825AB590:  cmplwi       cr6, r10, 0
0x825AB594:  bne          cr6, 0x825ab5ac
0x825AB598:  lwz          r8, 0(r5)
0x825AB59C:  li           r9, 0x1f
0x825AB5A0:  addi         r5, r5, 4
0x825AB5A4:  srwi         r11, r8, 1
0x825AB5A8:  b            0x825ab5b8
0x825AB5AC:  mr           r8, r11
0x825AB5B0:  srwi         r11, r11, 1
0x825AB5B4:  addi         r9, r10, -1
0x825AB5B8:  clrlwi       r10, r8, 0x1f
0x825AB5BC:  cmplwi       cr6, r10, 0
0x825AB5C0:  beq          cr6, 0x825ab5d8
0x825AB5C4:  slwi         r10, r7, 2
0x825AB5C8:  addi         r24, r24, -1
0x825AB5CC:  addi         r8, r10, 3
0x825AB5D0:  stb          r8, 0(r24)
0x825AB5D4:  b            0x825ab618
0x825AB5D8:  cmplwi       cr6, r9, 0
0x825AB5DC:  bne          cr6, 0x825ab5f4
0x825AB5E0:  lwz          r10, 0(r5)
0x825AB5E4:  li           r9, 0x1f
0x825AB5E8:  addi         r5, r5, 4
0x825AB5EC:  srwi         r11, r10, 1
0x825AB5F0:  b            0x825ab600
0x825AB5F4:  mr           r10, r11
0x825AB5F8:  srwi         r11, r11, 1
0x825AB5FC:  addi         r9, r9, -1
0x825AB600:  clrlwi       r8, r10, 0x1f
0x825AB604:  addi         r31, r1, -0x180
0x825AB608:  neg          r10, r8
0x825AB60C:  slwi         r8, r7, 1
0x825AB610:  rlwimi       r10, r22, 0, 0x1f, 0xf
0x825AB614:  sthx         r10, r8, r31
0x825AB618:  addi         r7, r7, 1
0x825AB61C:  cmplwi       cr6, r9, 0
0x825AB620:  bne          cr6, 0x825ab638
0x825AB624:  lwz          r8, 0(r5)
0x825AB628:  li           r11, 0x1f
0x825AB62C:  addi         r5, r5, 4
0x825AB630:  srwi         r10, r8, 1
0x825AB634:  b            0x825ab644
0x825AB638:  mr           r8, r11
0x825AB63C:  srwi         r10, r11, 1
0x825AB640:  addi         r11, r9, -1
0x825AB644:  clrlwi       r9, r8, 0x1f
0x825AB648:  cmplwi       cr6, r9, 0
0x825AB64C:  beq          cr6, 0x825ab664
0x825AB650:  slwi         r9, r7, 2
0x825AB654:  addi         r24, r24, -1
0x825AB658:  addi         r8, r9, 3
0x825AB65C:  stb          r8, 0(r24)
0x825AB660:  b            0x825ab6f4
0x825AB664:  cmplwi       cr6, r11, 0
0x825AB668:  bne          cr6, 0x825ab680
0x825AB66C:  lwz          r9, 0(r5)
0x825AB670:  li           r11, 0x1f
0x825AB674:  addi         r5, r5, 4
0x825AB678:  srwi         r10, r9, 1
0x825AB67C:  b            0x825ab68c
0x825AB680:  mr           r9, r10
0x825AB684:  srwi         r10, r10, 1
0x825AB688:  addi         r11, r11, -1
0x825AB68C:  clrlwi       r9, r9, 0x1f
0x825AB690:  slwi         r7, r7, 1
0x825AB694:  neg          r8, r9
0x825AB698:  addi         r9, r1, -0x180
0x825AB69C:  rlwimi       r8, r22, 0, 0x1f, 0xf
0x825AB6A0:  sthx         r8, r7, r9
0x825AB6A4:  b            0x825ab6f4
0x825AB6A8:  srwi         r8, r8, 2
0x825AB6AC:  cmplwi       cr6, r11, 0
0x825AB6B0:  bne          cr6, 0x825ab6c8
0x825AB6B4:  lwz          r9, 0(r5)
0x825AB6B8:  li           r11, 0x1f
0x825AB6BC:  addi         r5, r5, 4
0x825AB6C0:  srwi         r10, r9, 1
0x825AB6C4:  b            0x825ab6d4
0x825AB6C8:  mr           r9, r10
0x825AB6CC:  srwi         r10, r10, 1
0x825AB6D0:  addi         r11, r11, -1
0x825AB6D4:  clrlwi       r7, r9, 0x1f
0x825AB6D8:  stb          r21, 0(r6)
0x825AB6DC:  slwi         r8, r8, 1
0x825AB6E0:  neg          r9, r7
0x825AB6E4:  addi         r7, r1, -0x180
0x825AB6E8:  rlwimi       r9, r22, 0, 0x1f, 0xf
0x825AB6EC:  sthx         r9, r8, r7
0x825AB6F0:  addi         r6, r6, 1
0x825AB6F4:  cmplw        cr6, r6, r23
0x825AB6F8:  blt          cr6, 0x825ab3ac
0x825AB6FC:  stw          r5, 4(r4)
0x825AB700:  stw          r10, 0(r4)
0x825AB704:  stw          r11, 8(r4)
0x825AB708:  lhz          r6, -0x17e(r1)
0x825AB70C:  lwz          r9, -0x178(r1)
0x825AB710:  lwz          r8, -0x170(r1)
0x825AB714:  lwz          r7, -0x168(r1)
0x825AB718:  lwz          r5, -0x17c(r1)
0x825AB71C:  lwz          r4, -0x174(r1)
0x825AB720:  lwz          r11, -0x16c(r1)
0x825AB724:  lwz          r10, -0x164(r1)
0x825AB728:  sth          r6, 2(r3)
0x825AB72C:  stw          r9, 4(r3)
0x825AB730:  stw          r8, 8(r3)
0x825AB734:  stw          r7, 0xc(r3)
0x825AB738:  stw          r5, 0x10(r3)
0x825AB73C:  stw          r4, 0x14(r3)
0x825AB740:  stw          r11, 0x18(r3)
0x825AB744:  stw          r10, 0x1c(r3)
0x825AB748:  lwz          r9, -0x150(r1)
0x825AB74C:  lwz          r8, -0x128(r1)
0x825AB750:  lwz          r7, -0x160(r1)
0x825AB754:  lwz          r6, -0x158(r1)
0x825AB758:  lwz          r5, -0x14c(r1)
0x825AB75C:  lwz          r4, -0x124(r1)
0x825AB760:  lwz          r11, -0x15c(r1)
0x825AB764:  lwz          r10, -0x154(r1)
0x825AB768:  stw          r9, 0x20(r3)
0x825AB76C:  stw          r8, 0x24(r3)
0x825AB770:  stw          r7, 0x28(r3)
0x825AB774:  stw          r6, 0x2c(r3)
0x825AB778:  stw          r5, 0x30(r3)
0x825AB77C:  stw          r4, 0x34(r3)
0x825AB780:  stw          r11, 0x38(r3)
0x825AB784:  stw          r10, 0x3c(r3)
0x825AB788:  lwz          r9, -0x148(r1)
0x825AB78C:  lwz          r8, -0x140(r1)
0x825AB790:  lwz          r7, -0x120(r1)
0x825AB794:  lwz          r6, -0x118(r1)
0x825AB798:  lwz          r5, -0x144(r1)
0x825AB79C:  lwz          r4, -0x13c(r1)
0x825AB7A0:  lwz          r11, -0x11c(r1)
0x825AB7A4:  lwz          r10, -0x114(r1)
0x825AB7A8:  stw          r9, 0x40(r3)
0x825AB7AC:  stw          r8, 0x44(r3)
0x825AB7B0:  stw          r7, 0x48(r3)
0x825AB7B4:  stw          r6, 0x4c(r3)
0x825AB7B8:  stw          r5, 0x50(r3)
0x825AB7BC:  stw          r4, 0x54(r3)
0x825AB7C0:  stw          r11, 0x58(r3)
0x825AB7C4:  stw          r10, 0x5c(r3)
0x825AB7C8:  lwz          r9, -0x138(r1)
0x825AB7CC:  lwz          r8, -0x130(r1)
0x825AB7D0:  lwz          r7, -0x110(r1)
0x825AB7D4:  lwz          r6, -0x108(r1)
0x825AB7D8:  lwz          r5, -0x134(r1)
0x825AB7DC:  lwz          r4, -0x12c(r1)
0x825AB7E0:  lwz          r11, -0x10c(r1)
0x825AB7E4:  lwz          r10, -0x104(r1)
0x825AB7E8:  stw          r9, 0x60(r3)
0x825AB7EC:  stw          r8, 0x64(r3)
0x825AB7F0:  stw          r7, 0x68(r3)
0x825AB7F4:  stw          r6, 0x6c(r3)
0x825AB7F8:  stw          r5, 0x70(r3)
0x825AB7FC:  stw          r4, 0x74(r3)
0x825AB800:  stw          r11, 0x78(r3)
0x825AB804:  stw          r10, 0x7c(r3)
0x825AB808:  b            0x8242f8c0
0x825AB80C:  .align 0                       # padding
0x825AB810:  mflr         r12
0x825AB814:  bl           0x8242f880
0x825AB818:  lwz          r11, 8(r4)
0x825AB81C:  li           r25, 0
0x825AB820:  lwz          r10, 0(r4)
0x825AB824:  lwz          r31, 4(r4)
0x825AB828:  mr           r30, r25
0x825AB82C:  cmplwi       cr6, r11, 3
0x825AB830:  bge          cr6, 0x825ab860
0x825AB834:  lwz          r9, 0(r31)
0x825AB838:  clrlwi       r6, r10, 0x18
0x825AB83C:  subfic       r8, r11, 3
0x825AB840:  addi         r31, r31, 4
0x825AB844:  slw          r10, r9, r11
0x825AB848:  addi         r11, r11, 0x1d
0x825AB84C:  clrlwi       r10, r10, 0x18
0x825AB850:  or           r7, r10, r6
0x825AB854:  clrlwi       r10, r7, 0x1d
0x825AB858:  srw          r7, r9, r8
0x825AB85C:  b            0x825ab86c
0x825AB860:  srwi         r7, r10, 3
0x825AB864:  clrlwi       r10, r10, 0x1d
0x825AB868:  addi         r11, r11, -3
0x825AB86C:  li           r8, 0x10
0x825AB870:  addi         r10, r10, 1
0x825AB874:  li           r9, 1
0x825AB878:  li           r6, 0x60
0x825AB87C:  addi         r28, r1, -0xec
0x825AB880:  stb          r8, -0xec(r1)
0x825AB884:  li           r8, 0xb0
0x825AB888:  addi         r26, r1, -0xe8
0x825AB88C:  mr           r29, r25
0x825AB890:  stb          r6, -0xeb(r1)
0x825AB894:  li           r6, 2
0x825AB898:  mr           r24, r10
0x825AB89C:  stb          r8, -0xea(r1)
0x825AB8A0:  addi         r8, r10, -1
0x825AB8A4:  cmplwi       cr6, r10, 0
0x825AB8A8:  stb          r6, -0xe9(r1)
0x825AB8AC:  slw          r27, r9, r8
0x825AB8B0:  beq          cr6, 0x825abd20
0x825AB8B4:  mr           r6, r25
0x825AB8B8:  cmpwi        cr6, r29, 0
0x825AB8BC:  ble          cr6, 0x825ab934
0x825AB8C0:  cmplwi       cr6, r11, 0
0x825AB8C4:  bne          cr6, 0x825ab8dc
0x825AB8C8:  lwz          r10, 0(r31)
0x825AB8CC:  li           r11, 0x1f
0x825AB8D0:  addi         r31, r31, 4
0x825AB8D4:  srwi         r7, r10, 1
0x825AB8D8:  b            0x825ab8e8
0x825AB8DC:  mr           r10, r7
0x825AB8E0:  srwi         r7, r7, 1
0x825AB8E4:  addi         r11, r11, -1
0x825AB8E8:  clrlwi       r10, r10, 0x1f
0x825AB8EC:  cmplwi       cr6, r10, 0
0x825AB8F0:  beq          cr6, 0x825ab928
0x825AB8F4:  addi         r9, r1, -0xa0
0x825AB8F8:  lbzx         r10, r6, r9
0x825AB8FC:  lbzx         r8, r10, r3
0x825AB900:  cmplwi       cr6, r8, 0x80
0x825AB904:  neg          r8, r27
0x825AB908:  bge          cr6, 0x825ab910
0x825AB90C:  mr           r8, r27
0x825AB910:  lbzx         r9, r10, r3
0x825AB914:  cmplw        cr6, r30, r5
0x825AB918:  addi         r30, r30, 1
0x825AB91C:  add          r8, r9, r8
0x825AB920:  stbx         r8, r10, r3
0x825AB924:  beq          cr6, 0x825abd20
0x825AB928:  addi         r6, r6, 1
0x825AB92C:  cmpw         cr6, r6, r29
0x825AB930:  blt          cr6, 0x825ab8c0
0x825AB934:  mr           r6, r28
0x825AB938:  cmplw        cr6, r28, r26
0x825AB93C:  bge          cr6, 0x825abd10
0x825AB940:  addi         r10, r1, -0xa0
0x825AB944:  add          r8, r29, r10
0x825AB948:  lbz          r10, 0(r6)
0x825AB94C:  cmplwi       cr6, r10, 0
0x825AB950:  beq          cr6, 0x825abd04
0x825AB954:  cmplwi       cr6, r11, 0
0x825AB958:  bne          cr6, 0x825ab970
0x825AB95C:  lwz          r9, 0(r31)
0x825AB960:  li           r11, 0x1f
0x825AB964:  addi         r31, r31, 4
0x825AB968:  srwi         r7, r9, 1
0x825AB96C:  b            0x825ab97c
0x825AB970:  mr           r9, r7
0x825AB974:  srwi         r7, r7, 1
0x825AB978:  addi         r11, r11, -1
0x825AB97C:  clrlwi       r9, r9, 0x1f
0x825AB980:  cmplwi       cr6, r9, 0
0x825AB984:  beq          cr6, 0x825abd04
0x825AB988:  clrlwi       r9, r10, 0x1e
0x825AB98C:  cmplwi       cr6, r9, 3
0x825AB990:  bgt          cr6, 0x825abd04
0x825AB994:  lis          r12, -0x7da5
0x825AB998:  addi         r12, r12, -0x4654
0x825AB99C:  slwi         r0, r9, 2
0x825AB9A0:  lwzx         r0, r12, r0
0x825AB9A4:  mtctr        r0
0x825AB9A8:  bctr         
0x825AB9AC:  lwz          r18, -0x4644(r26)
0x825AB9B0:  lwz          r18, -0x4630(r26)
0x825AB9B4:  lwz          r18, -0x45f8(r26)
0x825AB9B8:  lwz          r18, -0x435c(r26)
0x825AB9BC:  srwi         r9, r10, 2
0x825AB9C0:  slwi         r10, r9, 2
0x825AB9C4:  addi         r10, r10, 0x11
0x825AB9C8:  stb          r10, 0(r6)
0x825AB9CC:  b            0x825aba14
0x825AB9D0:  rlwinm       r10, r10, 0, 0, 0x1d
0x825AB9D4:  addi         r9, r10, 2
0x825AB9D8:  addi         r23, r10, 0x12
0x825AB9DC:  addi         r22, r10, 0x22
0x825AB9E0:  addi         r10, r10, 0x32
0x825AB9E4:  stb          r9, 0(r6)
0x825AB9E8:  mr           r9, r10
0x825AB9EC:  addi         r10, r26, 1
0x825AB9F0:  stb          r23, 0(r26)
0x825AB9F4:  stb          r22, 0(r10)
0x825AB9F8:  addi         r10, r10, 1
0x825AB9FC:  addi         r26, r10, 1
0x825ABA00:  stb          r9, 0(r10)
0x825ABA04:  b            0x825abd08
0x825ABA08:  stb          r25, 0(r6)
0x825ABA0C:  srwi         r9, r10, 2
0x825ABA10:  addi         r6, r6, 1
0x825ABA14:  cmplwi       cr6, r11, 0
0x825ABA18:  bne          cr6, 0x825aba30
0x825ABA1C:  lwz          r10, 0(r31)
0x825ABA20:  li           r11, 0x1f
0x825ABA24:  addi         r31, r31, 4
0x825ABA28:  srwi         r7, r10, 1
0x825ABA2C:  b            0x825aba3c
0x825ABA30:  mr           r10, r7
0x825ABA34:  srwi         r7, r7, 1
0x825ABA38:  addi         r11, r11, -1
0x825ABA3C:  clrlwi       r10, r10, 0x1f
0x825ABA40:  cmplwi       cr6, r10, 0
0x825ABA44:  beq          cr6, 0x825aba5c
0x825ABA48:  slwi         r10, r9, 2
0x825ABA4C:  addi         r28, r28, -1
0x825ABA50:  addi         r10, r10, 3
0x825ABA54:  stb          r10, 0(r28)
0x825ABA58:  b            0x825abab4
0x825ABA5C:  stb          r9, 0(r8)
0x825ABA60:  addi         r29, r29, 1
0x825ABA64:  cmplwi       cr6, r11, 0
0x825ABA68:  addi         r8, r8, 1
0x825ABA6C:  bne          cr6, 0x825aba84
0x825ABA70:  lwz          r10, 0(r31)
0x825ABA74:  li           r11, 0x1f
0x825ABA78:  addi         r31, r31, 4
0x825ABA7C:  srwi         r7, r10, 1
0x825ABA80:  b            0x825aba90
0x825ABA84:  mr           r10, r7
0x825ABA88:  srwi         r7, r7, 1
0x825ABA8C:  addi         r11, r11, -1
0x825ABA90:  clrlwi       r10, r10, 0x1f
0x825ABA94:  cmplwi       cr6, r10, 0
0x825ABA98:  neg          r10, r27
0x825ABA9C:  bne          cr6, 0x825abaa4
0x825ABAA0:  mr           r10, r27
0x825ABAA4:  cmplw        cr6, r30, r5
0x825ABAA8:  stbx         r10, r9, r3
0x825ABAAC:  addi         r30, r30, 1
0x825ABAB0:  beq          cr6, 0x825abd20
0x825ABAB4:  addi         r9, r9, 1
0x825ABAB8:  cmplwi       cr6, r11, 0
0x825ABABC:  bne          cr6, 0x825abad4
0x825ABAC0:  lwz          r10, 0(r31)
0x825ABAC4:  li           r11, 0x1f
0x825ABAC8:  addi         r31, r31, 4
0x825ABACC:  srwi         r7, r10, 1
0x825ABAD0:  b            0x825abae0
0x825ABAD4:  mr           r10, r7
0x825ABAD8:  srwi         r7, r7, 1
0x825ABADC:  addi         r11, r11, -1
0x825ABAE0:  clrlwi       r10, r10, 0x1f
0x825ABAE4:  cmplwi       cr6, r10, 0
0x825ABAE8:  beq          cr6, 0x825abb00
0x825ABAEC:  slwi         r10, r9, 2
0x825ABAF0:  addi         r28, r28, -1
0x825ABAF4:  addi         r10, r10, 3
0x825ABAF8:  stb          r10, 0(r28)
0x825ABAFC:  b            0x825abb58
0x825ABB00:  stb          r9, 0(r8)
0x825ABB04:  addi         r29, r29, 1
0x825ABB08:  cmplwi       cr6, r11, 0
0x825ABB0C:  addi         r8, r8, 1
0x825ABB10:  bne          cr6, 0x825abb28
0x825ABB14:  lwz          r10, 0(r31)
0x825ABB18:  li           r11, 0x1f
0x825ABB1C:  addi         r31, r31, 4
0x825ABB20:  srwi         r7, r10, 1
0x825ABB24:  b            0x825abb34
0x825ABB28:  mr           r10, r7
0x825ABB2C:  srwi         r7, r7, 1
0x825ABB30:  addi         r11, r11, -1
0x825ABB34:  clrlwi       r10, r10, 0x1f
0x825ABB38:  cmplwi       cr6, r10, 0
0x825ABB3C:  neg          r10, r27
0x825ABB40:  bne          cr6, 0x825abb48
0x825ABB44:  mr           r10, r27
0x825ABB48:  cmplw        cr6, r30, r5
0x825ABB4C:  stbx         r10, r9, r3
0x825ABB50:  addi         r30, r30, 1
0x825ABB54:  beq          cr6, 0x825abd20
0x825ABB58:  addi         r9, r9, 1
0x825ABB5C:  cmplwi       cr6, r11, 0
0x825ABB60:  bne          cr6, 0x825abb78
0x825ABB64:  lwz          r10, 0(r31)
0x825ABB68:  li           r11, 0x1f
0x825ABB6C:  addi         r31, r31, 4
0x825ABB70:  srwi         r7, r10, 1
0x825ABB74:  b            0x825abb84
0x825ABB78:  mr           r10, r7
0x825ABB7C:  srwi         r7, r7, 1
0x825ABB80:  addi         r11, r11, -1
0x825ABB84:  clrlwi       r10, r10, 0x1f
0x825ABB88:  cmplwi       cr6, r10, 0
0x825ABB8C:  beq          cr6, 0x825abba4
0x825ABB90:  slwi         r10, r9, 2
0x825ABB94:  addi         r28, r28, -1
0x825ABB98:  addi         r10, r10, 3
0x825ABB9C:  stb          r10, 0(r28)
0x825ABBA0:  b            0x825abbfc
0x825ABBA4:  stb          r9, 0(r8)
0x825ABBA8:  addi         r29, r29, 1
0x825ABBAC:  cmplwi       cr6, r11, 0
0x825ABBB0:  addi         r8, r8, 1
0x825ABBB4:  bne          cr6, 0x825abbcc
0x825ABBB8:  lwz          r10, 0(r31)
0x825ABBBC:  li           r11, 0x1f
0x825ABBC0:  addi         r31, r31, 4
0x825ABBC4:  srwi         r7, r10, 1
0x825ABBC8:  b            0x825abbd8
0x825ABBCC:  mr           r10, r7
0x825ABBD0:  srwi         r7, r7, 1
0x825ABBD4:  addi         r11, r11, -1
0x825ABBD8:  clrlwi       r10, r10, 0x1f
0x825ABBDC:  cmplwi       cr6, r10, 0
0x825ABBE0:  neg          r10, r27
0x825ABBE4:  bne          cr6, 0x825abbec
0x825ABBE8:  mr           r10, r27
0x825ABBEC:  cmplw        cr6, r30, r5
0x825ABBF0:  stbx         r10, r9, r3
0x825ABBF4:  addi         r30, r30, 1
0x825ABBF8:  beq          cr6, 0x825abd20
0x825ABBFC:  addi         r9, r9, 1
0x825ABC00:  cmplwi       cr6, r11, 0
0x825ABC04:  bne          cr6, 0x825abc1c
0x825ABC08:  lwz          r10, 0(r31)
0x825ABC0C:  li           r11, 0x1f
0x825ABC10:  addi         r31, r31, 4
0x825ABC14:  srwi         r7, r10, 1
0x825ABC18:  b            0x825abc28
0x825ABC1C:  mr           r10, r7
0x825ABC20:  srwi         r7, r7, 1
0x825ABC24:  addi         r11, r11, -1
0x825ABC28:  clrlwi       r10, r10, 0x1f
0x825ABC2C:  cmplwi       cr6, r10, 0
0x825ABC30:  beq          cr6, 0x825abc48
0x825ABC34:  slwi         r10, r9, 2
0x825ABC38:  addi         r28, r28, -1
0x825ABC3C:  addi         r9, r10, 3
0x825ABC40:  stb          r9, 0(r28)
0x825ABC44:  b            0x825abd08
0x825ABC48:  stb          r9, 0(r8)
0x825ABC4C:  addi         r29, r29, 1
0x825ABC50:  cmplwi       cr6, r11, 0
0x825ABC54:  addi         r8, r8, 1
0x825ABC58:  bne          cr6, 0x825abc70
0x825ABC5C:  lwz          r10, 0(r31)
0x825ABC60:  li           r11, 0x1f
0x825ABC64:  addi         r31, r31, 4
0x825ABC68:  srwi         r7, r10, 1
0x825ABC6C:  b            0x825abc7c
0x825ABC70:  mr           r10, r7
0x825ABC74:  srwi         r7, r7, 1
0x825ABC78:  addi         r11, r11, -1
0x825ABC7C:  clrlwi       r10, r10, 0x1f
0x825ABC80:  cmplwi       cr6, r10, 0
0x825ABC84:  neg          r10, r27
0x825ABC88:  bne          cr6, 0x825abc90
0x825ABC8C:  mr           r10, r27
0x825ABC90:  cmplw        cr6, r30, r5
0x825ABC94:  stbx         r10, r9, r3
0x825ABC98:  addi         r30, r30, 1
0x825ABC9C:  beq          cr6, 0x825abd20
0x825ABCA0:  b            0x825abd08
0x825ABCA4:  srwi         r9, r10, 2
0x825ABCA8:  addi         r29, r29, 1
0x825ABCAC:  cmplwi       cr6, r11, 0
0x825ABCB0:  stb          r9, 0(r8)
0x825ABCB4:  addi         r8, r8, 1
0x825ABCB8:  bne          cr6, 0x825abcd0
0x825ABCBC:  lwz          r10, 0(r31)
0x825ABCC0:  li           r11, 0x1f
0x825ABCC4:  addi         r31, r31, 4
0x825ABCC8:  srwi         r7, r10, 1
0x825ABCCC:  b            0x825abcdc
0x825ABCD0:  mr           r10, r7
0x825ABCD4:  srwi         r7, r7, 1
0x825ABCD8:  addi         r11, r11, -1
0x825ABCDC:  clrlwi       r10, r10, 0x1f
0x825ABCE0:  cmplwi       cr6, r10, 0
0x825ABCE4:  neg          r10, r27
0x825ABCE8:  bne          cr6, 0x825abcf0
0x825ABCEC:  mr           r10, r27
0x825ABCF0:  cmplw        cr6, r30, r5
0x825ABCF4:  stbx         r10, r9, r3
0x825ABCF8:  addi         r30, r30, 1
0x825ABCFC:  beq          cr6, 0x825abd20
0x825ABD00:  stb          r25, 0(r6)
0x825ABD04:  addi         r6, r6, 1
0x825ABD08:  cmplw        cr6, r6, r26
0x825ABD0C:  blt          cr6, 0x825ab948
0x825ABD10:  addi         r24, r24, -1
0x825ABD14:  srawi        r27, r27, 1
0x825ABD18:  cmplwi       cr6, r24, 0
0x825ABD1C:  bne          cr6, 0x825ab8b4
0x825ABD20:  stw          r31, 4(r4)
0x825ABD24:  stw          r7, 0(r4)
0x825ABD28:  stw          r11, 8(r4)
0x825ABD2C:  b            0x8242f8d0
0x825ABD30:  mflr         r12
0x825ABD34:  bl           0x8242f888
0x825ABD38:  stwu         r1, -0xe0(r1)
0x825ABD3C:  li           r9, 0
0x825ABD40:  mr           r29, r4
0x825ABD44:  mr           r31, r3
0x825ABD48:  mr           r4, r5
0x825ABD4C:  mr           r30, r7
0x825ABD50:  mr           r28, r8
0x825ABD54:  std          r9, 0x50(r1)
0x825ABD58:  addi         r11, r1, 0x58
0x825ABD5C:  li           r10, 7
0x825ABD60:  mtctr        r10
0x825ABD64:  std          r9, 0(r11)
0x825ABD68:  addi         r11, r11, 8
0x825ABD6C:  bdnz         0x825abd64
0x825ABD70:  mr           r5, r6
0x825ABD74:  addi         r3, r1, 0x50
0x825ABD78:  bl           0x825ab810
0x825ABD7C:  lbz          r11, 0(r30)
0x825ABD80:  lbz          r10, 0x50(r1)
0x825ABD84:  lbz          r7, 0x51(r1)
0x825ABD88:  add          r8, r11, r10
0x825ABD8C:  lbz          r24, 0x54(r1)
0x825ABD90:  lbz          r25, 0x55(r1)
0x825ABD94:  add          r11, r30, r28
0x825ABD98:  lbz          r26, 0x58(r1)
0x825ABD9C:  add          r10, r31, r29
0x825ABDA0:  lbz          r27, 0x59(r1)
0x825ABDA4:  lbz          r3, 0x5c(r1)
0x825ABDA8:  stb          r8, 0(r31)
0x825ABDAC:  lbz          r8, 1(r30)
0x825ABDB0:  lbz          r4, 0x5d(r1)
0x825ABDB4:  add          r5, r8, r7
0x825ABDB8:  lbz          r9, 0x52(r1)
0x825ABDBC:  lbz          r6, 0x53(r1)
0x825ABDC0:  lbz          r7, 0x56(r1)
0x825ABDC4:  lbz          r8, 0x57(r1)
0x825ABDC8:  stb          r5, 1(r31)
0x825ABDCC:  lbz          r5, 2(r30)
0x825ABDD0:  add          r5, r5, r24
0x825ABDD4:  stb          r5, 2(r31)
0x825ABDD8:  lbz          r5, 3(r30)
0x825ABDDC:  add          r5, r5, r25
0x825ABDE0:  lbz          r25, 0x5e(r1)
0x825ABDE4:  stb          r5, 3(r31)
0x825ABDE8:  lbz          r5, 4(r30)
0x825ABDEC:  add          r5, r5, r26
0x825ABDF0:  lbz          r26, 0x5f(r1)
0x825ABDF4:  stb          r5, 4(r31)
0x825ABDF8:  lbz          r5, 5(r30)
0x825ABDFC:  add          r5, r5, r27
0x825ABE00:  stb          r5, 5(r31)
0x825ABE04:  lbz          r5, 6(r30)
0x825ABE08:  add          r3, r5, r3
0x825ABE0C:  stb          r3, 6(r31)
0x825ABE10:  lbz          r3, 7(r30)
0x825ABE14:  lbz          r30, 0x69(r1)
0x825ABE18:  add          r5, r3, r4
0x825ABE1C:  stb          r5, 7(r31)
0x825ABE20:  lbz          r5, 0(r11)
0x825ABE24:  lbz          r31, 0x7c(r1)
0x825ABE28:  add          r9, r9, r5
0x825ABE2C:  stb          r9, 0(r10)
0x825ABE30:  lbz          r9, 1(r11)
0x825ABE34:  add          r3, r9, r6
0x825ABE38:  lbz          r6, 0x64(r1)
0x825ABE3C:  stb          r3, 1(r10)
0x825ABE40:  lbz          r9, 2(r11)
0x825ABE44:  add          r5, r9, r7
0x825ABE48:  lbz          r7, 0x5b(r1)
0x825ABE4C:  stb          r5, 2(r10)
0x825ABE50:  lbz          r3, 3(r11)
0x825ABE54:  lbz          r5, 0x61(r1)
0x825ABE58:  add          r9, r3, r8
0x825ABE5C:  lbz          r8, 0x5a(r1)
0x825ABE60:  lbz          r3, 0x7d(r1)
0x825ABE64:  stb          r9, 3(r10)
0x825ABE68:  lbz          r9, 4(r11)
0x825ABE6C:  add          r4, r9, r8
0x825ABE70:  lbz          r8, 0x68(r1)
0x825ABE74:  stb          r4, 4(r10)
0x825ABE78:  lbz          r9, 5(r11)
0x825ABE7C:  lbz          r4, 0x60(r1)
0x825ABE80:  add          r27, r9, r7
0x825ABE84:  lbz          r7, 0x65(r1)
0x825ABE88:  lbz          r9, 0x6a(r1)
0x825ABE8C:  stb          r27, 5(r10)
0x825ABE90:  lbz          r27, 6(r11)
0x825ABE94:  add          r27, r27, r25
0x825ABE98:  stb          r27, 6(r10)
0x825ABE9C:  lbz          r27, 7(r11)
0x825ABEA0:  add          r11, r11, r28
0x825ABEA4:  add          r27, r27, r26
0x825ABEA8:  stb          r27, 7(r10)
0x825ABEAC:  add          r10, r10, r29
0x825ABEB0:  lbz          r27, 0(r11)
0x825ABEB4:  add          r8, r8, r27
0x825ABEB8:  lbz          r25, 0x7f(r1)
0x825ABEBC:  lbz          r26, 0x62(r1)
0x825ABEC0:  lbz          r27, 0x63(r1)
0x825ABEC4:  stb          r8, 0(r10)
0x825ABEC8:  lbz          r8, 1(r11)
0x825ABECC:  add          r8, r8, r30
0x825ABED0:  lbz          r30, 0x66(r1)
0x825ABED4:  stb          r8, 1(r10)
0x825ABED8:  lbz          r8, 2(r11)
0x825ABEDC:  add          r8, r8, r31
0x825ABEE0:  lbz          r31, 0x67(r1)
0x825ABEE4:  stb          r8, 2(r10)
0x825ABEE8:  lbz          r8, 3(r11)
0x825ABEEC:  add          r3, r8, r3
0x825ABEF0:  stb          r3, 3(r10)
0x825ABEF4:  lbz          r3, 4(r11)
0x825ABEF8:  add          r8, r3, r4
0x825ABEFC:  lbz          r4, 0x6d(r1)
0x825ABF00:  stb          r8, 4(r10)
0x825ABF04:  lbz          r3, 5(r11)
0x825ABF08:  add          r8, r3, r5
0x825ABF0C:  stb          r8, 5(r10)
0x825ABF10:  lbz          r8, 6(r11)
0x825ABF14:  add          r3, r8, r6
0x825ABF18:  lbz          r6, 0x71(r1)
0x825ABF1C:  stb          r3, 6(r10)
0x825ABF20:  lbz          r8, 7(r11)
0x825ABF24:  add          r11, r11, r28
0x825ABF28:  lbz          r3, 0x7e(r1)
0x825ABF2C:  add          r5, r8, r7
0x825ABF30:  stb          r5, 7(r10)
0x825ABF34:  add          r10, r10, r29
0x825ABF38:  lbz          r8, 0(r11)
0x825ABF3C:  lbz          r5, 0x70(r1)
0x825ABF40:  add          r9, r9, r8
0x825ABF44:  lbz          r8, 0x6b(r1)
0x825ABF48:  stb          r9, 0(r10)
0x825ABF4C:  lbz          r7, 1(r11)
0x825ABF50:  lbz          r9, 0x6c(r1)
0x825ABF54:  add          r7, r7, r8
0x825ABF58:  stb          r7, 1(r10)
0x825ABF5C:  lbz          r8, 2(r11)
0x825ABF60:  lbz          r7, 0x80(r1)
0x825ABF64:  add          r3, r8, r3
0x825ABF68:  lbz          r8, 0x81(r1)
0x825ABF6C:  stb          r3, 2(r10)
0x825ABF70:  lbz          r3, 3(r11)
0x825ABF74:  add          r3, r3, r25
0x825ABF78:  stb          r3, 3(r10)
0x825ABF7C:  lbz          r3, 4(r11)
0x825ABF80:  add          r3, r3, r26
0x825ABF84:  stb          r3, 4(r10)
0x825ABF88:  lbz          r3, 5(r11)
0x825ABF8C:  add          r3, r3, r27
0x825ABF90:  stb          r3, 5(r10)
0x825ABF94:  lbz          r3, 6(r11)
0x825ABF98:  add          r3, r3, r30
0x825ABF9C:  stb          r3, 6(r10)
0x825ABFA0:  lbz          r3, 7(r11)
0x825ABFA4:  add          r11, r11, r28
0x825ABFA8:  add          r3, r3, r31
0x825ABFAC:  stb          r3, 7(r10)
0x825ABFB0:  add          r10, r10, r29
0x825ABFB4:  lbz          r3, 0(r11)
0x825ABFB8:  add          r9, r9, r3
0x825ABFBC:  stb          r9, 0(r10)
0x825ABFC0:  lbz          r9, 1(r11)
0x825ABFC4:  add          r4, r9, r4
0x825ABFC8:  stb          r4, 1(r10)
0x825ABFCC:  lbz          r9, 2(r11)
0x825ABFD0:  add          r5, r9, r5
0x825ABFD4:  stb          r5, 2(r10)
0x825ABFD8:  lbz          r3, 3(r11)
0x825ABFDC:  add          r9, r3, r6
0x825ABFE0:  stb          r9, 3(r10)
0x825ABFE4:  lbz          r9, 4(r11)
0x825ABFE8:  add          r4, r9, r7
0x825ABFEC:  stb          r4, 4(r10)
0x825ABFF0:  lbz          r9, 5(r11)
0x825ABFF4:  add          r8, r9, r8
0x825ABFF8:  stb          r8, 5(r10)
0x825ABFFC:  lbz          r9, 6(r11)
0x825AC000:  lbz          r7, 0x84(r1)
0x825AC004:  lbz          r8, 0x85(r1)
0x825AC008:  add          r3, r9, r7
0x825AC00C:  lbz          r6, 0x6e(r1)
0x825AC010:  lbz          r26, 0x6f(r1)
0x825AC014:  lbz          r27, 0x72(r1)
0x825AC018:  lbz          r30, 0x73(r1)
0x825AC01C:  lbz          r31, 0x82(r1)
0x825AC020:  stb          r3, 6(r10)
0x825AC024:  lbz          r9, 7(r11)
0x825AC028:  add          r11, r11, r28
0x825AC02C:  lbz          r3, 0x83(r1)
0x825AC030:  add          r25, r9, r8
0x825AC034:  lbz          r4, 0x86(r1)
0x825AC038:  lbz          r5, 0x87(r1)
0x825AC03C:  lbz          r9, 0x74(r1)
0x825AC040:  lbz          r7, 0x75(r1)
0x825AC044:  lbz          r8, 0x78(r1)
0x825AC048:  stb          r25, 7(r10)
0x825AC04C:  add          r10, r10, r29
0x825AC050:  lbz          r25, 0(r11)
0x825AC054:  add          r6, r6, r25
0x825AC058:  lbz          r25, 0x89(r1)
0x825AC05C:  stb          r6, 0(r10)
0x825AC060:  lbz          r6, 1(r11)
0x825AC064:  add          r6, r6, r26
0x825AC068:  lbz          r26, 0x8c(r1)
0x825AC06C:  stb          r6, 1(r10)
0x825AC070:  lbz          r6, 2(r11)
0x825AC074:  add          r6, r6, r27
0x825AC078:  lbz          r27, 0x8d(r1)
0x825AC07C:  stb          r6, 2(r10)
0x825AC080:  lbz          r6, 3(r11)
0x825AC084:  add          r6, r6, r30
0x825AC088:  lbz          r30, 0x88(r1)
0x825AC08C:  stb          r6, 3(r10)
0x825AC090:  lbz          r6, 4(r11)
0x825AC094:  add          r6, r6, r31
0x825AC098:  lbz          r31, 0x77(r1)
0x825AC09C:  stb          r6, 4(r10)
0x825AC0A0:  lbz          r6, 5(r11)
0x825AC0A4:  add          r3, r6, r3
0x825AC0A8:  stb          r3, 5(r10)
0x825AC0AC:  lbz          r3, 6(r11)
0x825AC0B0:  add          r6, r3, r4
0x825AC0B4:  lbz          r4, 0x7b(r1)
0x825AC0B8:  stb          r6, 6(r10)
0x825AC0BC:  lbz          r3, 7(r11)
0x825AC0C0:  add          r11, r11, r28
0x825AC0C4:  add          r6, r3, r5
0x825AC0C8:  stb          r6, 7(r10)
0x825AC0CC:  add          r10, r10, r29
0x825AC0D0:  lbz          r6, 0(r11)
0x825AC0D4:  add          r3, r9, r6
0x825AC0D8:  lbz          r6, 0x8b(r1)
0x825AC0DC:  stb          r3, 0(r10)
0x825AC0E0:  lbz          r9, 1(r11)
0x825AC0E4:  add          r5, r9, r7
0x825AC0E8:  stb          r5, 1(r10)
0x825AC0EC:  lbz          r3, 2(r11)
0x825AC0F0:  lbz          r5, 0x8a(r1)
0x825AC0F4:  add          r9, r3, r8
0x825AC0F8:  lbz          r8, 0x79(r1)
0x825AC0FC:  lbz          r3, 0x7a(r1)
0x825AC100:  stb          r9, 2(r10)
0x825AC104:  lbz          r9, 3(r11)
0x825AC108:  add          r7, r9, r8
0x825AC10C:  lbz          r8, 0x76(r1)
0x825AC110:  stb          r7, 3(r10)
0x825AC114:  lbz          r9, 4(r11)
0x825AC118:  lbz          r7, 0x8e(r1)
0x825AC11C:  add          r30, r9, r30
0x825AC120:  lbz          r9, 0x8f(r1)
0x825AC124:  stb          r30, 4(r10)
0x825AC128:  lbz          r30, 5(r11)
0x825AC12C:  add          r30, r30, r25
0x825AC130:  stb          r30, 5(r10)
0x825AC134:  lbz          r30, 6(r11)
0x825AC138:  add          r30, r30, r26
0x825AC13C:  stb          r30, 6(r10)
0x825AC140:  lbz          r30, 7(r11)
0x825AC144:  add          r11, r11, r28
0x825AC148:  add          r30, r30, r27
0x825AC14C:  stb          r30, 7(r10)
0x825AC150:  add          r10, r10, r29
0x825AC154:  lbz          r30, 0(r11)
0x825AC158:  add          r8, r8, r30
0x825AC15C:  stb          r8, 0(r10)
0x825AC160:  lbz          r8, 1(r11)
0x825AC164:  add          r8, r8, r31
0x825AC168:  stb          r8, 1(r10)
0x825AC16C:  lbz          r8, 2(r11)
0x825AC170:  add          r3, r8, r3
0x825AC174:  stb          r3, 2(r10)
0x825AC178:  lbz          r3, 3(r11)
0x825AC17C:  add          r8, r3, r4
0x825AC180:  stb          r8, 3(r10)
0x825AC184:  lbz          r3, 4(r11)
0x825AC188:  add          r8, r3, r5
0x825AC18C:  stb          r8, 4(r10)
0x825AC190:  lbz          r8, 5(r11)
0x825AC194:  add          r3, r8, r6
0x825AC198:  stb          r3, 5(r10)
0x825AC19C:  lbz          r8, 6(r11)
0x825AC1A0:  add          r5, r8, r7
0x825AC1A4:  stb          r5, 6(r10)
0x825AC1A8:  lbz          r3, 7(r11)
0x825AC1AC:  add          r11, r3, r9
0x825AC1B0:  stb          r11, 7(r10)
0x825AC1B4:  addi         r1, r1, 0xe0
0x825AC1B8:  b            0x8242f8d8
0x825AC1BC:  .align 0                       # padding
0x825AC1C0:  .align 0                       # padding
0x825AC1C4:  .align 0                       # padding
0x825AC1C8:  .align 0                       # padding
0x825AC1CC:  .align 0                       # padding
0x825AC1D0:  .align 0                       # padding
0x825AC1D4:  .align 0                       # padding
0x825AC1D8:  .align 0                       # padding
0x825AC1DC:  .align 0                       # padding
0x825AC1E0:  .align 0                       # padding
0x825AC1E4:  .align 0                       # padding
0x825AC1E8:  .align 0                       # padding
0x825AC1EC:  .align 0                       # padding
0x825AC1F0:  .align 0                       # padding
0x825AC1F4:  .align 0                       # padding
0x825AC1F8:  .align 0                       # padding
0x825AC1FC:  .align 0                       # padding
