
fexp_ps:     file format elf64-littleriscv
fexp_ps
architecture: riscv, flags 0x00000112:
EXEC_P, HAS_SYMS, D_PAGED
start address 0x00000000000101bc

Program Header:
    LOAD off    0x0000000000000000 vaddr 0x0000000000010000 paddr 0x0000000000010000 align 2**12
         filesz 0x000000000000072c memsz 0x000000000000072c flags r-x
    LOAD off    0x0000000000000730 vaddr 0x0000000000011730 paddr 0x0000000000011730 align 2**12
         filesz 0x0000000000000770 memsz 0x00000000000007a8 flags rw-

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000678  00000000000100b0  00000000000100b0  000000b0  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .eh_frame     00000004  0000000000010728  0000000000010728  00000728  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .init_array   00000008  0000000000011730  0000000000011730  00000730  2**3
                  CONTENTS, ALLOC, LOAD, DATA
  3 .fini_array   00000008  0000000000011738  0000000000011738  00000738  2**3
                  CONTENTS, ALLOC, LOAD, DATA
  4 .data         00000748  0000000000011740  0000000000011740  00000740  2**3
                  CONTENTS, ALLOC, LOAD, DATA
  5 .sdata        00000018  0000000000011e88  0000000000011e88  00000e88  2**3
                  CONTENTS, ALLOC, LOAD, DATA
  6 .bss          00000038  0000000000011ea0  0000000000011ea0  00000ea0  2**3
                  ALLOC
  7 .comment      00000011  0000000000000000  0000000000000000  00000ea0  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
00000000000100b0 l    d  .text	0000000000000000 .text
0000000000010728 l    d  .eh_frame	0000000000000000 .eh_frame
0000000000011730 l    d  .init_array	0000000000000000 .init_array
0000000000011738 l    d  .fini_array	0000000000000000 .fini_array
0000000000011740 l    d  .data	0000000000000000 .data
0000000000011e88 l    d  .sdata	0000000000000000 .sdata
0000000000011ea0 l    d  .bss	0000000000000000 .bss
0000000000000000 l    d  .comment	0000000000000000 .comment
0000000000000000 l    df *ABS*	0000000000000000 fexp_ps.cc
00000000000100f0 l       .text	0000000000000000 loop
0000000000000000 l    df *ABS*	0000000000000000 crtstuff.c
0000000000010728 l     O .eh_frame	0000000000000000 __EH_FRAME_BEGIN__
0000000000010208 l     F .text	0000000000000000 deregister_tm_clones
0000000000010234 l     F .text	0000000000000000 register_tm_clones
0000000000010270 l     F .text	0000000000000000 __do_global_dtors_aux
0000000000011ea0 l     O .bss	0000000000000001 completed.5212
0000000000011738 l     O .fini_array	0000000000000000 __do_global_dtors_aux_fini_array_entry
00000000000102c8 l     F .text	0000000000000000 frame_dummy
0000000000011ea8 l     O .bss	0000000000000030 object.5217
0000000000011730 l     O .init_array	0000000000000000 __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*	0000000000000000 atexit.c
0000000000000000 l    df *ABS*	0000000000000000 exit.c
0000000000000000 l    df *ABS*	0000000000000000 fini.c
0000000000000000 l    df *ABS*	0000000000000000 impure.c
0000000000011740 l     O .data	0000000000000748 impure_data
0000000000000000 l    df *ABS*	0000000000000000 init.c
0000000000000000 l    df *ABS*	0000000000000000 __atexit.c
0000000000000000 l    df *ABS*	0000000000000000 __call_atexit.c
0000000000000000 l    df *ABS*	0000000000000000 sys_exit.c
0000000000000000 l    df *ABS*	0000000000000000 errno.c
0000000000000000 l    df *ABS*	0000000000000000 crtstuff.c
0000000000010728 l     O .eh_frame	0000000000000000 __FRAME_END__
0000000000000000 l    df *ABS*	0000000000000000 
0000000000011740 l       .fini_array	0000000000000000 __fini_array_end
0000000000011738 l       .fini_array	0000000000000000 __fini_array_start
0000000000011738 l       .init_array	0000000000000000 __init_array_end
000000000001172c l       .init_array	0000000000000000 __preinit_array_end
0000000000011730 l       .init_array	0000000000000000 __init_array_start
000000000001172c l       .init_array	0000000000000000 __preinit_array_start
0000000000012688 g       .sdata	0000000000000000 __global_pointer$
000000000001071c g     F .text	000000000000000c __errno
0000000000011e88 g     O .sdata	0000000000000000 .hidden __TMC_END__
0000000000011e88 g     O .sdata	0000000000000000 .hidden __dso_handle
0000000000011e90 g     O .sdata	0000000000000008 _global_impure_ptr
00000000000103bc g     F .text	00000000000000a0 __libc_init_array
0000000000010204 g     F .text	0000000000000000 _init
0000000000010348 g     F .text	0000000000000074 __libc_fini_array
00000000000105c0 g     F .text	0000000000000118 __call_exitprocs
00000000000101bc g     F .text	0000000000000048 _start
0000000000010538 g     F .text	0000000000000088 __register_exitproc
0000000000011ea0 g       .bss	0000000000000000 __bss_start
000000000001045c g     F .text	00000000000000dc memset
00000000000100b0 g     F .text	000000000000010c main
0000000000010204 g     F .text	0000000000000000 _fini
0000000000010300 g     F .text	0000000000000014 atexit
0000000000011e98 g     O .sdata	0000000000000008 _impure_ptr
0000000000011ea0 g       .sdata	0000000000000000 _edata
0000000000011ed8 g       .bss	0000000000000000 _end
0000000000010314 g     F .text	0000000000000034 exit
00000000000106d8 g     F .text	0000000000000044 _exit



Disassembly of section .text:

00000000000100b0 <main>:
   100b0:	ff010113          	addi	sp,sp,-16
   100b4:	00813023          	sd	s0,0(sp)
   100b8:	00113423          	sd	ra,8(sp)
   100bc:	01010413          	addi	s0,sp,16
   100c0:	dead08b7          	lui	a7,0xdead0
   100c4:	57f0707b          	mov.m.x	mt0,zero,0xff
   100c8:	00800113          	li	sp,8
   100cc:	0e700093          	li	ra,231
   100d0:	002090b3          	sll	ra,ra,sp
   100d4:	08008093          	addi	ra,ra,128
   100d8:	002090b3          	sll	ra,ra,sp
   100dc:	00008093          	mv	ra,ra
   100e0:	002090b3          	sll	ra,ra,sp
   100e4:	00008093          	mv	ra,ra
   100e8:	10000113          	li	sp,256
   100ec:	0000b08b          	fbcx.ps	ft1,ra

00000000000100f0 <loop>:
   100f0:	584080fb          	fexp.ps	ft1,ft1
   100f4:	00108093          	addi	ra,ra,1
   100f8:	fff10113          	addi	sp,sp,-1
   100fc:	fff10113          	addi	sp,sp,-1
   10100:	fff10113          	addi	sp,sp,-1
   10104:	fff10113          	addi	sp,sp,-1
   10108:	fff10113          	addi	sp,sp,-1
   1010c:	fff10113          	addi	sp,sp,-1
   10110:	fff10113          	addi	sp,sp,-1
   10114:	fff10113          	addi	sp,sp,-1
   10118:	fff10113          	addi	sp,sp,-1
   1011c:	fff10113          	addi	sp,sp,-1
   10120:	fff10113          	addi	sp,sp,-1
   10124:	fff10113          	addi	sp,sp,-1
   10128:	fff10113          	addi	sp,sp,-1
   1012c:	fff10113          	addi	sp,sp,-1
   10130:	fff10113          	addi	sp,sp,-1
   10134:	fff10113          	addi	sp,sp,-1
   10138:	fff10113          	addi	sp,sp,-1
   1013c:	fff10113          	addi	sp,sp,-1
   10140:	fff10113          	addi	sp,sp,-1
   10144:	fff10113          	addi	sp,sp,-1
   10148:	fff10113          	addi	sp,sp,-1
   1014c:	fff10113          	addi	sp,sp,-1
   10150:	fff10113          	addi	sp,sp,-1
   10154:	fff10113          	addi	sp,sp,-1
   10158:	fff10113          	addi	sp,sp,-1
   1015c:	fff10113          	addi	sp,sp,-1
   10160:	fff10113          	addi	sp,sp,-1
   10164:	fff10113          	addi	sp,sp,-1
   10168:	fff10113          	addi	sp,sp,-1
   1016c:	fff10113          	addi	sp,sp,-1
   10170:	fff10113          	addi	sp,sp,-1
   10174:	fff10113          	addi	sp,sp,-1
   10178:	fff10113          	addi	sp,sp,-1
   1017c:	fff10113          	addi	sp,sp,-1
   10180:	fff10113          	addi	sp,sp,-1
   10184:	fff10113          	addi	sp,sp,-1
   10188:	fff10113          	addi	sp,sp,-1
   1018c:	fff10113          	addi	sp,sp,-1
   10190:	fff10113          	addi	sp,sp,-1
   10194:	fff10113          	addi	sp,sp,-1
   10198:	fff10113          	addi	sp,sp,-1
   1019c:	fff10113          	addi	sp,sp,-1
   101a0:	fff10113          	addi	sp,sp,-1
   101a4:	10500073          	wfi
   101a8:	00813083          	ld	ra,8(sp)
   101ac:	00013403          	ld	s0,0(sp)
   101b0:	00000513          	li	a0,0
   101b4:	01010113          	addi	sp,sp,16
   101b8:	00008067          	ret

00000000000101bc <_start>:
   101bc:	00002197          	auipc	gp,0x2
   101c0:	4cc18193          	addi	gp,gp,1228 # 12688 <__global_pointer$>
   101c4:	00002517          	auipc	a0,0x2
   101c8:	cdc50513          	addi	a0,a0,-804 # 11ea0 <_edata>
   101cc:	00002617          	auipc	a2,0x2
   101d0:	d0c60613          	addi	a2,a2,-756 # 11ed8 <_end>
   101d4:	40a60633          	sub	a2,a2,a0
   101d8:	00000593          	li	a1,0
   101dc:	280000ef          	jal	ra,1045c <memset>
   101e0:	00000517          	auipc	a0,0x0
   101e4:	16850513          	addi	a0,a0,360 # 10348 <__libc_fini_array>
   101e8:	118000ef          	jal	ra,10300 <atexit>
   101ec:	1d0000ef          	jal	ra,103bc <__libc_init_array>
   101f0:	00012503          	lw	a0,0(sp)
   101f4:	00810593          	addi	a1,sp,8
   101f8:	00000613          	li	a2,0
   101fc:	eb5ff0ef          	jal	ra,100b0 <main>
   10200:	1140006f          	j	10314 <exit>

0000000000010204 <_fini>:
   10204:	00008067          	ret

0000000000010208 <deregister_tm_clones>:
   10208:	00012537          	lui	a0,0x12
   1020c:	000127b7          	lui	a5,0x12
   10210:	e8850713          	addi	a4,a0,-376 # 11e88 <__TMC_END__>
   10214:	e8878793          	addi	a5,a5,-376 # 11e88 <__TMC_END__>
   10218:	00e78c63          	beq	a5,a4,10230 <deregister_tm_clones+0x28>
   1021c:	00000337          	lui	t1,0x0
   10220:	00030313          	mv	t1,t1
   10224:	00030663          	beqz	t1,10230 <deregister_tm_clones+0x28>
   10228:	e8850513          	addi	a0,a0,-376
   1022c:	00030067          	jr	t1
   10230:	00008067          	ret

0000000000010234 <register_tm_clones>:
   10234:	000127b7          	lui	a5,0x12
   10238:	00012537          	lui	a0,0x12
   1023c:	e8878593          	addi	a1,a5,-376 # 11e88 <__TMC_END__>
   10240:	e8850793          	addi	a5,a0,-376 # 11e88 <__TMC_END__>
   10244:	40f585b3          	sub	a1,a1,a5
   10248:	4035d593          	srai	a1,a1,0x3
   1024c:	00200793          	li	a5,2
   10250:	02f5c5b3          	div	a1,a1,a5
   10254:	00058c63          	beqz	a1,1026c <register_tm_clones+0x38>
   10258:	00000337          	lui	t1,0x0
   1025c:	00030313          	mv	t1,t1
   10260:	00030663          	beqz	t1,1026c <register_tm_clones+0x38>
   10264:	e8850513          	addi	a0,a0,-376
   10268:	00030067          	jr	t1
   1026c:	00008067          	ret

0000000000010270 <__do_global_dtors_aux>:
   10270:	000127b7          	lui	a5,0x12
   10274:	ea07c703          	lbu	a4,-352(a5) # 11ea0 <_edata>
   10278:	04071663          	bnez	a4,102c4 <__do_global_dtors_aux+0x54>
   1027c:	ff010113          	addi	sp,sp,-16
   10280:	00813023          	sd	s0,0(sp)
   10284:	00113423          	sd	ra,8(sp)
   10288:	00078413          	mv	s0,a5
   1028c:	f7dff0ef          	jal	ra,10208 <deregister_tm_clones>
   10290:	000007b7          	lui	a5,0x0
   10294:	00078793          	mv	a5,a5
   10298:	00078a63          	beqz	a5,102ac <__do_global_dtors_aux+0x3c>
   1029c:	00010537          	lui	a0,0x10
   102a0:	72850513          	addi	a0,a0,1832 # 10728 <__EH_FRAME_BEGIN__>
   102a4:	ffff0317          	auipc	t1,0xffff0
   102a8:	d5c300e7          	jalr	-676(t1) # 0 <main-0x100b0>
   102ac:	00100793          	li	a5,1
   102b0:	eaf40023          	sb	a5,-352(s0)
   102b4:	00813083          	ld	ra,8(sp)
   102b8:	00013403          	ld	s0,0(sp)
   102bc:	01010113          	addi	sp,sp,16
   102c0:	00008067          	ret
   102c4:	00008067          	ret

00000000000102c8 <frame_dummy>:
   102c8:	000007b7          	lui	a5,0x0
   102cc:	00078793          	mv	a5,a5
   102d0:	02078663          	beqz	a5,102fc <frame_dummy+0x34>
   102d4:	000125b7          	lui	a1,0x12
   102d8:	00010537          	lui	a0,0x10
   102dc:	ff010113          	addi	sp,sp,-16
   102e0:	ea858593          	addi	a1,a1,-344 # 11ea8 <object.5217>
   102e4:	72850513          	addi	a0,a0,1832 # 10728 <__EH_FRAME_BEGIN__>
   102e8:	00113423          	sd	ra,8(sp)
   102ec:	ffff0317          	auipc	t1,0xffff0
   102f0:	d14300e7          	jalr	-748(t1) # 0 <main-0x100b0>
   102f4:	00813083          	ld	ra,8(sp)
   102f8:	01010113          	addi	sp,sp,16
   102fc:	f39ff06f          	j	10234 <register_tm_clones>

0000000000010300 <atexit>:
   10300:	00050593          	mv	a1,a0
   10304:	00000693          	li	a3,0
   10308:	00000613          	li	a2,0
   1030c:	00000513          	li	a0,0
   10310:	2280006f          	j	10538 <__register_exitproc>

0000000000010314 <exit>:
   10314:	ff010113          	addi	sp,sp,-16
   10318:	00000593          	li	a1,0
   1031c:	00813023          	sd	s0,0(sp)
   10320:	00113423          	sd	ra,8(sp)
   10324:	00050413          	mv	s0,a0
   10328:	298000ef          	jal	ra,105c0 <__call_exitprocs>
   1032c:	000127b7          	lui	a5,0x12
   10330:	e907b503          	ld	a0,-368(a5) # 11e90 <_global_impure_ptr>
   10334:	05853783          	ld	a5,88(a0)
   10338:	00078463          	beqz	a5,10340 <exit+0x2c>
   1033c:	000780e7          	jalr	a5
   10340:	00040513          	mv	a0,s0
   10344:	394000ef          	jal	ra,106d8 <_exit>

0000000000010348 <__libc_fini_array>:
   10348:	fd010113          	addi	sp,sp,-48
   1034c:	000117b7          	lui	a5,0x11
   10350:	00011737          	lui	a4,0x11
   10354:	02813023          	sd	s0,32(sp)
   10358:	73870713          	addi	a4,a4,1848 # 11738 <__init_array_end>
   1035c:	74078413          	addi	s0,a5,1856 # 11740 <__fini_array_end>
   10360:	40e40433          	sub	s0,s0,a4
   10364:	00913c23          	sd	s1,24(sp)
   10368:	01213823          	sd	s2,16(sp)
   1036c:	01313423          	sd	s3,8(sp)
   10370:	02113423          	sd	ra,40(sp)
   10374:	40345413          	srai	s0,s0,0x3
   10378:	00000493          	li	s1,0
   1037c:	74078913          	addi	s2,a5,1856
   10380:	ff800993          	li	s3,-8
   10384:	02941063          	bne	s0,s1,103a4 <__libc_fini_array+0x5c>
   10388:	02013403          	ld	s0,32(sp)
   1038c:	02813083          	ld	ra,40(sp)
   10390:	01813483          	ld	s1,24(sp)
   10394:	01013903          	ld	s2,16(sp)
   10398:	00813983          	ld	s3,8(sp)
   1039c:	03010113          	addi	sp,sp,48
   103a0:	e65ff06f          	j	10204 <_fini>
   103a4:	033487b3          	mul	a5,s1,s3
   103a8:	00148493          	addi	s1,s1,1
   103ac:	00f907b3          	add	a5,s2,a5
   103b0:	ff87b783          	ld	a5,-8(a5)
   103b4:	000780e7          	jalr	a5
   103b8:	fcdff06f          	j	10384 <__libc_fini_array+0x3c>

00000000000103bc <__libc_init_array>:
   103bc:	fe010113          	addi	sp,sp,-32
   103c0:	00813823          	sd	s0,16(sp)
   103c4:	00913423          	sd	s1,8(sp)
   103c8:	00011437          	lui	s0,0x11
   103cc:	000114b7          	lui	s1,0x11
   103d0:	72c48793          	addi	a5,s1,1836 # 1172c <__preinit_array_end>
   103d4:	72c40413          	addi	s0,s0,1836 # 1172c <__preinit_array_end>
   103d8:	40f40433          	sub	s0,s0,a5
   103dc:	01213023          	sd	s2,0(sp)
   103e0:	00113c23          	sd	ra,24(sp)
   103e4:	40345413          	srai	s0,s0,0x3
   103e8:	72c48493          	addi	s1,s1,1836
   103ec:	00000913          	li	s2,0
   103f0:	04891263          	bne	s2,s0,10434 <__libc_init_array+0x78>
   103f4:	000114b7          	lui	s1,0x11
   103f8:	e0dff0ef          	jal	ra,10204 <_fini>
   103fc:	00011437          	lui	s0,0x11
   10400:	73048793          	addi	a5,s1,1840 # 11730 <__frame_dummy_init_array_entry>
   10404:	73840413          	addi	s0,s0,1848 # 11738 <__init_array_end>
   10408:	40f40433          	sub	s0,s0,a5
   1040c:	40345413          	srai	s0,s0,0x3
   10410:	73048493          	addi	s1,s1,1840
   10414:	00000913          	li	s2,0
   10418:	02891863          	bne	s2,s0,10448 <__libc_init_array+0x8c>
   1041c:	01813083          	ld	ra,24(sp)
   10420:	01013403          	ld	s0,16(sp)
   10424:	00813483          	ld	s1,8(sp)
   10428:	00013903          	ld	s2,0(sp)
   1042c:	02010113          	addi	sp,sp,32
   10430:	00008067          	ret
   10434:	0004b783          	ld	a5,0(s1)
   10438:	00190913          	addi	s2,s2,1
   1043c:	00848493          	addi	s1,s1,8
   10440:	000780e7          	jalr	a5
   10444:	fadff06f          	j	103f0 <__libc_init_array+0x34>
   10448:	0004b783          	ld	a5,0(s1)
   1044c:	00190913          	addi	s2,s2,1
   10450:	00848493          	addi	s1,s1,8
   10454:	000780e7          	jalr	a5
   10458:	fc1ff06f          	j	10418 <__libc_init_array+0x5c>

000000000001045c <memset>:
   1045c:	00f00313          	li	t1,15
   10460:	00050713          	mv	a4,a0
   10464:	02c37a63          	bleu	a2,t1,10498 <memset+0x3c>
   10468:	00f77793          	andi	a5,a4,15
   1046c:	0a079063          	bnez	a5,1050c <memset+0xb0>
   10470:	06059e63          	bnez	a1,104ec <memset+0x90>
   10474:	ff067693          	andi	a3,a2,-16
   10478:	00f67613          	andi	a2,a2,15
   1047c:	00e686b3          	add	a3,a3,a4
   10480:	00b73023          	sd	a1,0(a4)
   10484:	00b73423          	sd	a1,8(a4)
   10488:	01070713          	addi	a4,a4,16
   1048c:	fed76ae3          	bltu	a4,a3,10480 <memset+0x24>
   10490:	00061463          	bnez	a2,10498 <memset+0x3c>
   10494:	00008067          	ret
   10498:	40c306b3          	sub	a3,t1,a2
   1049c:	00269693          	slli	a3,a3,0x2
   104a0:	00000297          	auipc	t0,0x0
   104a4:	005686b3          	add	a3,a3,t0
   104a8:	00c68067          	jr	12(a3)
   104ac:	00b70723          	sb	a1,14(a4)
   104b0:	00b706a3          	sb	a1,13(a4)
   104b4:	00b70623          	sb	a1,12(a4)
   104b8:	00b705a3          	sb	a1,11(a4)
   104bc:	00b70523          	sb	a1,10(a4)
   104c0:	00b704a3          	sb	a1,9(a4)
   104c4:	00b70423          	sb	a1,8(a4)
   104c8:	00b703a3          	sb	a1,7(a4)
   104cc:	00b70323          	sb	a1,6(a4)
   104d0:	00b702a3          	sb	a1,5(a4)
   104d4:	00b70223          	sb	a1,4(a4)
   104d8:	00b701a3          	sb	a1,3(a4)
   104dc:	00b70123          	sb	a1,2(a4)
   104e0:	00b700a3          	sb	a1,1(a4)
   104e4:	00b70023          	sb	a1,0(a4)
   104e8:	00008067          	ret
   104ec:	0ff5f593          	andi	a1,a1,255
   104f0:	00859693          	slli	a3,a1,0x8
   104f4:	00d5e5b3          	or	a1,a1,a3
   104f8:	01059693          	slli	a3,a1,0x10
   104fc:	00d5e5b3          	or	a1,a1,a3
   10500:	02059693          	slli	a3,a1,0x20
   10504:	00d5e5b3          	or	a1,a1,a3
   10508:	f6dff06f          	j	10474 <memset+0x18>
   1050c:	00279693          	slli	a3,a5,0x2
   10510:	00000297          	auipc	t0,0x0
   10514:	005686b3          	add	a3,a3,t0
   10518:	00008293          	mv	t0,ra
   1051c:	f98680e7          	jalr	-104(a3)
   10520:	00028093          	mv	ra,t0
   10524:	ff078793          	addi	a5,a5,-16
   10528:	40f70733          	sub	a4,a4,a5
   1052c:	00f60633          	add	a2,a2,a5
   10530:	f6c374e3          	bleu	a2,t1,10498 <memset+0x3c>
   10534:	f3dff06f          	j	10470 <memset+0x14>

0000000000010538 <__register_exitproc>:
   10538:	000127b7          	lui	a5,0x12
   1053c:	e907b703          	ld	a4,-368(a5) # 11e90 <_global_impure_ptr>
   10540:	00050313          	mv	t1,a0
   10544:	1f873783          	ld	a5,504(a4)
   10548:	00079663          	bnez	a5,10554 <__register_exitproc+0x1c>
   1054c:	20070793          	addi	a5,a4,512
   10550:	1ef73c23          	sd	a5,504(a4)
   10554:	0087a703          	lw	a4,8(a5)
   10558:	01f00813          	li	a6,31
   1055c:	fff00513          	li	a0,-1
   10560:	04e84e63          	blt	a6,a4,105bc <__register_exitproc+0x84>
   10564:	02030e63          	beqz	t1,105a0 <__register_exitproc+0x68>
   10568:	00371813          	slli	a6,a4,0x3
   1056c:	01078833          	add	a6,a5,a6
   10570:	10c83823          	sd	a2,272(a6)
   10574:	3107a883          	lw	a7,784(a5)
   10578:	00100613          	li	a2,1
   1057c:	00e6163b          	sllw	a2,a2,a4
   10580:	00c8e8b3          	or	a7,a7,a2
   10584:	3117a823          	sw	a7,784(a5)
   10588:	20d83823          	sd	a3,528(a6)
   1058c:	00200693          	li	a3,2
   10590:	00d31863          	bne	t1,a3,105a0 <__register_exitproc+0x68>
   10594:	3147a683          	lw	a3,788(a5)
   10598:	00c6e633          	or	a2,a3,a2
   1059c:	30c7aa23          	sw	a2,788(a5)
   105a0:	0017069b          	addiw	a3,a4,1
   105a4:	00270713          	addi	a4,a4,2
   105a8:	00371713          	slli	a4,a4,0x3
   105ac:	00d7a423          	sw	a3,8(a5)
   105b0:	00e787b3          	add	a5,a5,a4
   105b4:	00b7b023          	sd	a1,0(a5)
   105b8:	00000513          	li	a0,0
   105bc:	00008067          	ret

00000000000105c0 <__call_exitprocs>:
   105c0:	fb010113          	addi	sp,sp,-80
   105c4:	000127b7          	lui	a5,0x12
   105c8:	03313423          	sd	s3,40(sp)
   105cc:	e907b983          	ld	s3,-368(a5) # 11e90 <_global_impure_ptr>
   105d0:	03413023          	sd	s4,32(sp)
   105d4:	01513c23          	sd	s5,24(sp)
   105d8:	01613823          	sd	s6,16(sp)
   105dc:	04113423          	sd	ra,72(sp)
   105e0:	04813023          	sd	s0,64(sp)
   105e4:	02913c23          	sd	s1,56(sp)
   105e8:	03213823          	sd	s2,48(sp)
   105ec:	01713423          	sd	s7,8(sp)
   105f0:	00050a93          	mv	s5,a0
   105f4:	00058a13          	mv	s4,a1
   105f8:	00100b13          	li	s6,1
   105fc:	1f89b403          	ld	s0,504(s3)
   10600:	00040c63          	beqz	s0,10618 <__call_exitprocs+0x58>
   10604:	00842483          	lw	s1,8(s0)
   10608:	fff4849b          	addiw	s1,s1,-1
   1060c:	00349913          	slli	s2,s1,0x3
   10610:	01240933          	add	s2,s0,s2
   10614:	0204d863          	bgez	s1,10644 <__call_exitprocs+0x84>
   10618:	04813083          	ld	ra,72(sp)
   1061c:	04013403          	ld	s0,64(sp)
   10620:	03813483          	ld	s1,56(sp)
   10624:	03013903          	ld	s2,48(sp)
   10628:	02813983          	ld	s3,40(sp)
   1062c:	02013a03          	ld	s4,32(sp)
   10630:	01813a83          	ld	s5,24(sp)
   10634:	01013b03          	ld	s6,16(sp)
   10638:	00813b83          	ld	s7,8(sp)
   1063c:	05010113          	addi	sp,sp,80
   10640:	00008067          	ret
   10644:	000a0c63          	beqz	s4,1065c <__call_exitprocs+0x9c>
   10648:	21093783          	ld	a5,528(s2)
   1064c:	01478863          	beq	a5,s4,1065c <__call_exitprocs+0x9c>
   10650:	fff4849b          	addiw	s1,s1,-1
   10654:	ff890913          	addi	s2,s2,-8
   10658:	fbdff06f          	j	10614 <__call_exitprocs+0x54>
   1065c:	00842783          	lw	a5,8(s0)
   10660:	01093683          	ld	a3,16(s2)
   10664:	fff7879b          	addiw	a5,a5,-1
   10668:	02979e63          	bne	a5,s1,106a4 <__call_exitprocs+0xe4>
   1066c:	00942423          	sw	s1,8(s0)
   10670:	fe0680e3          	beqz	a3,10650 <__call_exitprocs+0x90>
   10674:	31042703          	lw	a4,784(s0)
   10678:	009b163b          	sllw	a2,s6,s1
   1067c:	00842b83          	lw	s7,8(s0)
   10680:	00c77733          	and	a4,a4,a2
   10684:	0007071b          	sext.w	a4,a4
   10688:	02071263          	bnez	a4,106ac <__call_exitprocs+0xec>
   1068c:	000680e7          	jalr	a3
   10690:	00842703          	lw	a4,8(s0)
   10694:	1f89b783          	ld	a5,504(s3)
   10698:	f77712e3          	bne	a4,s7,105fc <__call_exitprocs+0x3c>
   1069c:	faf40ae3          	beq	s0,a5,10650 <__call_exitprocs+0x90>
   106a0:	f5dff06f          	j	105fc <__call_exitprocs+0x3c>
   106a4:	00093823          	sd	zero,16(s2)
   106a8:	fc9ff06f          	j	10670 <__call_exitprocs+0xb0>
   106ac:	31442783          	lw	a5,788(s0)
   106b0:	11093583          	ld	a1,272(s2)
   106b4:	00c7f7b3          	and	a5,a5,a2
   106b8:	0007879b          	sext.w	a5,a5
   106bc:	00079863          	bnez	a5,106cc <__call_exitprocs+0x10c>
   106c0:	000a8513          	mv	a0,s5
   106c4:	000680e7          	jalr	a3
   106c8:	fc9ff06f          	j	10690 <__call_exitprocs+0xd0>
   106cc:	00058513          	mv	a0,a1
   106d0:	000680e7          	jalr	a3
   106d4:	fbdff06f          	j	10690 <__call_exitprocs+0xd0>

00000000000106d8 <_exit>:
   106d8:	00000593          	li	a1,0
   106dc:	00000613          	li	a2,0
   106e0:	00000693          	li	a3,0
   106e4:	00000713          	li	a4,0
   106e8:	00000793          	li	a5,0
   106ec:	05d00893          	li	a7,93
   106f0:	00000073          	ecall
   106f4:	02055263          	bgez	a0,10718 <_exit+0x40>
   106f8:	ff010113          	addi	sp,sp,-16
   106fc:	00813023          	sd	s0,0(sp)
   10700:	00050413          	mv	s0,a0
   10704:	00113423          	sd	ra,8(sp)
   10708:	4080043b          	negw	s0,s0
   1070c:	010000ef          	jal	ra,1071c <__errno>
   10710:	00852023          	sw	s0,0(a0)
   10714:	0000006f          	j	10714 <_exit+0x3c>
   10718:	0000006f          	j	10718 <_exit+0x40>

000000000001071c <__errno>:
   1071c:	000127b7          	lui	a5,0x12
   10720:	e987b503          	ld	a0,-360(a5) # 11e98 <_impure_ptr>
   10724:	00008067          	ret
