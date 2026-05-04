/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"


/// Macros

#define VSNIP_RSV           : : [operands] "r" (operands), [m_11] "r" (m_11), [m_12] "r" (m_12), [m_13] "r" (m_13), [m_14] "r" (m_14), [m_15] "r" (m_15) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
#define TSNIP_RSV          : : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x31"
#define RESET_FCSR            __asm__ __volatile__ ( "csrw    fcsr, zero\n" VSNIP_RSV);
#define RESET_TENSOR_COOP     __asm__ __volatile__ ( "csrw    0x804, zero\n" VSNIP_RSV);

#define ECALL                 __asm__ __volatile__ ( "ecall\n" VSNIP_RSV);
#define MRET                  __asm__ __volatile__ ( "mret\n" VSNIP_RSV);

#define TENSOR_WAIT_TL0       __asm__ __volatile__ ( "csrwi   0x830, 0x0\n" VSNIP_RSV);
#define TENSOR_WAIT_TL1       __asm__ __volatile__ ( "csrwi   0x830, 0x1\n" VSNIP_RSV);
#define TENSOR_WAIT_COP       __asm__ __volatile__ ( "csrwi   0x830, 0x6\n" VSNIP_RSV);
#define TENSOR_WAIT_FMA       __asm__ __volatile__ ( "csrwi   0x830, 0x7\n" VSNIP_RSV);
#define TENSOR_WAIT_STR       __asm__ __volatile__ ( "csrwi   0x830, 0x8\n" VSNIP_RSV);
#define TENSOR_WAIT_RED       __asm__ __volatile__ ( "csrwi   0x830, 0x9\n" VSNIP_RSV);
#define TENSOR_WAIT_QNT       __asm__ __volatile__ ( "csrwi   0x830, 0xa\n" VSNIP_RSV);
#define SET_TENSOR_MASK    \
   __asm__ __volatile__ ( \
         "li x5, 0xFFFF\n" \
         "csrrw zero, 0x805, x5\n" \
         VSNIP_RSV);

#define SET_SCP_CTL    \
   __asm__ __volatile__ ( \
         "csrrs	x0, 0x7e0, 3\n" \
         VSNIP_RSV);

int main() {

	uint64_t mid, tid;
	mid = get_minion_id();
	tid = get_thread_id();
	if (mid != 0 && tid != 0) { C_TEST_FAIL; return -1;};
	volatile uint32_t m_11[1024] __attribute__ ((aligned (4096))) = {
			 0xf06f2661, /// 0x0
			 0x4eb21c9c, /// 0x4
			 0xbe944082, /// 0x8
			 0x017fce88, /// 0xc
			 0x21106050, /// 0x10
			 0xf3117c77, /// 0x14
			 0x94b2ddec, /// 0x18
			 0xe95bab2e, /// 0x1c
			 0x96ae74e9, /// 0x20
			 0x7887dd7f, /// 0x24
			 0x0b1e0e8a, /// 0x28
			 0x35dc566b, /// 0x2c
			 0x4e0a800a, /// 0x30
			 0x73942e00, /// 0x34
			 0xf6d2f673, /// 0x38
			 0x7b043574, /// 0x3c
			 0x81f066a8, /// 0x40
			 0xd2cb2b3b, /// 0x44
			 0x02bbd443, /// 0x48
			 0x9bad9621, /// 0x4c
			 0x942ad9c1, /// 0x50
			 0x176f2798, /// 0x54
			 0x86814af2, /// 0x58
			 0x126da365, /// 0x5c
			 0x4a882032, /// 0x60
			 0x8ed2c170, /// 0x64
			 0x5add2036, /// 0x68
			 0xf291b11d, /// 0x6c
			 0x835491c5, /// 0x70
			 0xb49986cb, /// 0x74
			 0xae901cac, /// 0x78
			 0xa7b4f064, /// 0x7c
			 0x01aacdc8, /// 0x80
			 0xc57d7488, /// 0x84
			 0xce206a13, /// 0x88
			 0xdc5adf45, /// 0x8c
			 0x6c08e00c, /// 0x90
			 0xf806cd61, /// 0x94
			 0x83a6a5fd, /// 0x98
			 0x2dea7e27, /// 0x9c
			 0x52b4fe64, /// 0xa0
			 0x6d9c9458, /// 0xa4
			 0x0bac4b39, /// 0xa8
			 0x913d8f87, /// 0xac
			 0x65b3e2a8, /// 0xb0
			 0xa549384d, /// 0xb4
			 0x07e79998, /// 0xb8
			 0xa17f9cf2, /// 0xbc
			 0x8ae1041c, /// 0xc0
			 0x5e9777b2, /// 0xc4
			 0xb77d3676, /// 0xc8
			 0xb18de5b9, /// 0xcc
			 0x9b7c5446, /// 0xd0
			 0x6a6f292d, /// 0xd4
			 0xb7d39d30, /// 0xd8
			 0xc78df614, /// 0xdc
			 0x86516394, /// 0xe0
			 0x056c448f, /// 0xe4
			 0xd950f58d, /// 0xe8
			 0x645490a2, /// 0xec
			 0xaa1f0f52, /// 0xf0
			 0xb6143cb2, /// 0xf4
			 0x4b230999, /// 0xf8
			 0xf7d7e82b, /// 0xfc
			 0xeefb8a0e, /// 0x100
			 0x0a31dbe7, /// 0x104
			 0x5e24baf9, /// 0x108
			 0x77dacc08, /// 0x10c
			 0x809e72e0, /// 0x110
			 0xa3b25b6c, /// 0x114
			 0xb79c7a5e, /// 0x118
			 0x50f7b245, /// 0x11c
			 0x931ef10c, /// 0x120
			 0xecc0b961, /// 0x124
			 0x60fcd92b, /// 0x128
			 0x4ccdbf6b, /// 0x12c
			 0x86f2224a, /// 0x130
			 0xb42a1ab6, /// 0x134
			 0xaf386814, /// 0x138
			 0x31067dcd, /// 0x13c
			 0xaada1179, /// 0x140
			 0x417b3708, /// 0x144
			 0x38a8b753, /// 0x148
			 0x780cac11, /// 0x14c
			 0xaa886e3a, /// 0x150
			 0x6a257e5b, /// 0x154
			 0xfb8f92f8, /// 0x158
			 0x07e7d114, /// 0x15c
			 0x8daa08a2, /// 0x160
			 0x4f6686c7, /// 0x164
			 0xca6395bf, /// 0x168
			 0x65cf7968, /// 0x16c
			 0x664d079f, /// 0x170
			 0x3250150a, /// 0x174
			 0xb7b3b736, /// 0x178
			 0xe18f0edf, /// 0x17c
			 0x1217dca3, /// 0x180
			 0xf7f84a24, /// 0x184
			 0x929a6882, /// 0x188
			 0xeb918267, /// 0x18c
			 0x2f8dcd2a, /// 0x190
			 0x32160b76, /// 0x194
			 0xa14217e0, /// 0x198
			 0xac76407c, /// 0x19c
			 0x0de44eef, /// 0x1a0
			 0x49338d14, /// 0x1a4
			 0x14bfdf45, /// 0x1a8
			 0x47408df5, /// 0x1ac
			 0x71ffef70, /// 0x1b0
			 0x910d8b4e, /// 0x1b4
			 0xb8b56c3c, /// 0x1b8
			 0x606f9153, /// 0x1bc
			 0xa175cc1e, /// 0x1c0
			 0xefa1ca47, /// 0x1c4
			 0xd9700760, /// 0x1c8
			 0x22dec20e, /// 0x1cc
			 0x14c3d9f2, /// 0x1d0
			 0x0823c193, /// 0x1d4
			 0xd67565c1, /// 0x1d8
			 0xd6a043a4, /// 0x1dc
			 0xc482752f, /// 0x1e0
			 0x265ca546, /// 0x1e4
			 0x8c1b53cd, /// 0x1e8
			 0x5ffc450f, /// 0x1ec
			 0x64b9e3e9, /// 0x1f0
			 0xd18ad1dc, /// 0x1f4
			 0x50f5b242, /// 0x1f8
			 0xeb4b4563, /// 0x1fc
			 0xb53e829b, /// 0x200
			 0xc667026f, /// 0x204
			 0x5c1c2f1c, /// 0x208
			 0x189b5767, /// 0x20c
			 0xed2249db, /// 0x210
			 0x9aada67a, /// 0x214
			 0x0a2974e6, /// 0x218
			 0xef37df89, /// 0x21c
			 0x089f9537, /// 0x220
			 0xbc53169b, /// 0x224
			 0xa9acc641, /// 0x228
			 0x5b3cdbe9, /// 0x22c
			 0x7a232d29, /// 0x230
			 0xc0e9965d, /// 0x234
			 0x9bb26ab5, /// 0x238
			 0x50adbbd3, /// 0x23c
			 0x6403a584, /// 0x240
			 0xc2dc8674, /// 0x244
			 0xd7c28f98, /// 0x248
			 0x3504d3ff, /// 0x24c
			 0xa585d867, /// 0x250
			 0x7ffad397, /// 0x254
			 0x520dbe28, /// 0x258
			 0x7132607e, /// 0x25c
			 0x43ba7cd5, /// 0x260
			 0x761afbc1, /// 0x264
			 0xc3b74e68, /// 0x268
			 0x60505633, /// 0x26c
			 0x6bf2c9ab, /// 0x270
			 0xaba2354f, /// 0x274
			 0x09519291, /// 0x278
			 0xecf09a82, /// 0x27c
			 0xb11dd11d, /// 0x280
			 0x811bea3f, /// 0x284
			 0xf0fd7b27, /// 0x288
			 0x2cbf1de8, /// 0x28c
			 0x1eefa5f2, /// 0x290
			 0x730d9918, /// 0x294
			 0x847ca65e, /// 0x298
			 0xeb0054ce, /// 0x29c
			 0x38808431, /// 0x2a0
			 0x22a71d32, /// 0x2a4
			 0x416a6f96, /// 0x2a8
			 0xf763634a, /// 0x2ac
			 0x33edc85c, /// 0x2b0
			 0x3237c8fd, /// 0x2b4
			 0x3ad419ee, /// 0x2b8
			 0xc16fd2af, /// 0x2bc
			 0xc30e66b7, /// 0x2c0
			 0x6f44d0ac, /// 0x2c4
			 0x53e8466d, /// 0x2c8
			 0x168e80f1, /// 0x2cc
			 0x5bedc4b3, /// 0x2d0
			 0x505d0087, /// 0x2d4
			 0x944c7940, /// 0x2d8
			 0x3d9a7ba9, /// 0x2dc
			 0xa9441d40, /// 0x2e0
			 0x4ba61073, /// 0x2e4
			 0x7d344bac, /// 0x2e8
			 0x184e2769, /// 0x2ec
			 0xaee1b51f, /// 0x2f0
			 0x2adf4dde, /// 0x2f4
			 0xe86040d4, /// 0x2f8
			 0xa37e6803, /// 0x2fc
			 0xe63bca0f, /// 0x300
			 0xfccee41c, /// 0x304
			 0xc2f64b8d, /// 0x308
			 0xdd74acf8, /// 0x30c
			 0x3eff8f9a, /// 0x310
			 0x20938730, /// 0x314
			 0x611db298, /// 0x318
			 0xf6cf3763, /// 0x31c
			 0x1949b1cb, /// 0x320
			 0x6a94f176, /// 0x324
			 0xcc55e584, /// 0x328
			 0x6cb25a4c, /// 0x32c
			 0xcfbab56c, /// 0x330
			 0xc3ce89fe, /// 0x334
			 0x398e3bdf, /// 0x338
			 0xad14e119, /// 0x33c
			 0xe8c3a93a, /// 0x340
			 0xdbe655cf, /// 0x344
			 0x7ecae963, /// 0x348
			 0x08514975, /// 0x34c
			 0x06e66b88, /// 0x350
			 0x1286f1d5, /// 0x354
			 0x3c2ddfd6, /// 0x358
			 0x7b3c9381, /// 0x35c
			 0x452dd608, /// 0x360
			 0xb02381dc, /// 0x364
			 0xc59bffe6, /// 0x368
			 0x8a5f04f9, /// 0x36c
			 0xcb367e26, /// 0x370
			 0x66f79fb8, /// 0x374
			 0x83d70ce8, /// 0x378
			 0x8695983c, /// 0x37c
			 0xfd3a3391, /// 0x380
			 0xc2163341, /// 0x384
			 0x9d937d0a, /// 0x388
			 0x32b70a18, /// 0x38c
			 0x39b741cd, /// 0x390
			 0x901b5dad, /// 0x394
			 0xacf07531, /// 0x398
			 0x2914d8f7, /// 0x39c
			 0x8499f82f, /// 0x3a0
			 0x3393a14f, /// 0x3a4
			 0xb7ab5e32, /// 0x3a8
			 0x9e0c5cc9, /// 0x3ac
			 0xd3533d6c, /// 0x3b0
			 0x9bf5c14c, /// 0x3b4
			 0x09b97677, /// 0x3b8
			 0x5f21814e, /// 0x3bc
			 0x585302b7, /// 0x3c0
			 0x16c0db4a, /// 0x3c4
			 0x4b585de9, /// 0x3c8
			 0x0a7555be, /// 0x3cc
			 0x63a20b2f, /// 0x3d0
			 0x0ec3f7f6, /// 0x3d4
			 0x4ce1e097, /// 0x3d8
			 0x21f84d71, /// 0x3dc
			 0x7380207a, /// 0x3e0
			 0x0707a88e, /// 0x3e4
			 0x80688a2c, /// 0x3e8
			 0xaa219741, /// 0x3ec
			 0x242eec1c, /// 0x3f0
			 0x100195f2, /// 0x3f4
			 0xe5f0d731, /// 0x3f8
			 0xcda2d42a, /// 0x3fc
			 0x23701208, /// 0x400
			 0x470b7d33, /// 0x404
			 0x1a1c0dcf, /// 0x408
			 0x7d4b69df, /// 0x40c
			 0x1d0ed2d7, /// 0x410
			 0x91274d9f, /// 0x414
			 0x35568ea8, /// 0x418
			 0xe496be6b, /// 0x41c
			 0x9a3cb84f, /// 0x420
			 0x8718f1ab, /// 0x424
			 0x042673b3, /// 0x428
			 0xc6ac6ddd, /// 0x42c
			 0x75ad8458, /// 0x430
			 0xba1eb87b, /// 0x434
			 0x602837c8, /// 0x438
			 0xd3be15f7, /// 0x43c
			 0xfeee3573, /// 0x440
			 0x495e7abd, /// 0x444
			 0x1d1017bc, /// 0x448
			 0x7b8765a1, /// 0x44c
			 0x6dae1887, /// 0x450
			 0xce053458, /// 0x454
			 0xd939df2a, /// 0x458
			 0x9702e26c, /// 0x45c
			 0x23edba26, /// 0x460
			 0x7b014bff, /// 0x464
			 0x3ad3b89a, /// 0x468
			 0x068198d8, /// 0x46c
			 0x4a88cf14, /// 0x470
			 0x355360b0, /// 0x474
			 0xe6df9755, /// 0x478
			 0x4c4e74bb, /// 0x47c
			 0xe77ce82a, /// 0x480
			 0x29866b57, /// 0x484
			 0xcb152b91, /// 0x488
			 0x6d8cb81f, /// 0x48c
			 0x7ff0fb1a, /// 0x490
			 0xd81bc1ed, /// 0x494
			 0x0b34f7a5, /// 0x498
			 0x27b91de2, /// 0x49c
			 0x22ba01e7, /// 0x4a0
			 0x6780f686, /// 0x4a4
			 0x08e1b2e0, /// 0x4a8
			 0xde15a288, /// 0x4ac
			 0x34fb9579, /// 0x4b0
			 0x7c08d150, /// 0x4b4
			 0xc963f947, /// 0x4b8
			 0xa48f7515, /// 0x4bc
			 0xdf3fca62, /// 0x4c0
			 0xc9a223ba, /// 0x4c4
			 0xe58e2e4f, /// 0x4c8
			 0xa536664e, /// 0x4cc
			 0x05052c0e, /// 0x4d0
			 0xf17d2dcc, /// 0x4d4
			 0xd315e9f7, /// 0x4d8
			 0x1776779d, /// 0x4dc
			 0x542492a6, /// 0x4e0
			 0x31313b54, /// 0x4e4
			 0x3a03b47e, /// 0x4e8
			 0x9320d65b, /// 0x4ec
			 0xefb94c23, /// 0x4f0
			 0x6d86ae79, /// 0x4f4
			 0xf1c8a177, /// 0x4f8
			 0x91b2886d, /// 0x4fc
			 0x7d738770, /// 0x500
			 0x282c671c, /// 0x504
			 0x0b983433, /// 0x508
			 0x3829ac65, /// 0x50c
			 0x12ca8fcf, /// 0x510
			 0x2f752577, /// 0x514
			 0x62e0a78d, /// 0x518
			 0x3b593faf, /// 0x51c
			 0x6a13039d, /// 0x520
			 0xcbf2ce4c, /// 0x524
			 0xbd15f328, /// 0x528
			 0xbeb06b50, /// 0x52c
			 0xe20785b6, /// 0x530
			 0xc9ddcd18, /// 0x534
			 0xfaa7f9cb, /// 0x538
			 0x9bbb1114, /// 0x53c
			 0x77277705, /// 0x540
			 0x79621899, /// 0x544
			 0xfff97394, /// 0x548
			 0x99f98651, /// 0x54c
			 0xfb4f68d0, /// 0x550
			 0x5ce8580b, /// 0x554
			 0xb774f96a, /// 0x558
			 0x39496842, /// 0x55c
			 0x558301a3, /// 0x560
			 0x9a6cd343, /// 0x564
			 0xce21f280, /// 0x568
			 0xd603a302, /// 0x56c
			 0x24f101b9, /// 0x570
			 0xf8bd910f, /// 0x574
			 0xe55376e0, /// 0x578
			 0x70e90b9b, /// 0x57c
			 0x488f284e, /// 0x580
			 0x04ac48ba, /// 0x584
			 0x7194f2ba, /// 0x588
			 0x49e41e69, /// 0x58c
			 0xadc995e1, /// 0x590
			 0x31368e56, /// 0x594
			 0x4bf4a713, /// 0x598
			 0x80f9162d, /// 0x59c
			 0x3dc44b19, /// 0x5a0
			 0xdb64b2b8, /// 0x5a4
			 0x722b8a6b, /// 0x5a8
			 0xedc27520, /// 0x5ac
			 0x8134b3f2, /// 0x5b0
			 0x96e9980c, /// 0x5b4
			 0x5334a5c4, /// 0x5b8
			 0xf7c3078f, /// 0x5bc
			 0x4be0ea20, /// 0x5c0
			 0x87ac7dec, /// 0x5c4
			 0xbc67ed1f, /// 0x5c8
			 0x4970846c, /// 0x5cc
			 0x1ca469cd, /// 0x5d0
			 0xfda7d5f0, /// 0x5d4
			 0xbdba17bc, /// 0x5d8
			 0x8fd3fc5f, /// 0x5dc
			 0x3e450195, /// 0x5e0
			 0xfd579bd8, /// 0x5e4
			 0xf77c72e1, /// 0x5e8
			 0x63bd06ae, /// 0x5ec
			 0xf068bcb2, /// 0x5f0
			 0x9d95b155, /// 0x5f4
			 0xd8800bf6, /// 0x5f8
			 0x2180ea13, /// 0x5fc
			 0xd71e2d89, /// 0x600
			 0xba2abbd2, /// 0x604
			 0xfa8c3c8a, /// 0x608
			 0xfc4f9b31, /// 0x60c
			 0xa4ea834a, /// 0x610
			 0xd9bcd181, /// 0x614
			 0x2fa87d65, /// 0x618
			 0x646f74cc, /// 0x61c
			 0x38f945fd, /// 0x620
			 0xdd7ad05e, /// 0x624
			 0x7046ba36, /// 0x628
			 0xa9991ee0, /// 0x62c
			 0x376f7794, /// 0x630
			 0x70b869f7, /// 0x634
			 0x95b13889, /// 0x638
			 0x676491f7, /// 0x63c
			 0x8b42f618, /// 0x640
			 0xbe1e643c, /// 0x644
			 0xd33b1d3b, /// 0x648
			 0x90b14a32, /// 0x64c
			 0xf73e5c1a, /// 0x650
			 0x382f87e1, /// 0x654
			 0xca2fbd7b, /// 0x658
			 0x039e0c1d, /// 0x65c
			 0xbc444bc7, /// 0x660
			 0x9d45f7d6, /// 0x664
			 0xe429f84c, /// 0x668
			 0xb97a60c2, /// 0x66c
			 0xc7aaf39a, /// 0x670
			 0x733d9cb9, /// 0x674
			 0x8ef3caec, /// 0x678
			 0xc996c43a, /// 0x67c
			 0xd021a926, /// 0x680
			 0x4b7bf6f1, /// 0x684
			 0xe4f7b822, /// 0x688
			 0x22b9854e, /// 0x68c
			 0x2e3fb82a, /// 0x690
			 0x9757ad2e, /// 0x694
			 0xff3dd4df, /// 0x698
			 0xd5d5eec6, /// 0x69c
			 0xbb48d786, /// 0x6a0
			 0xbfb0cee0, /// 0x6a4
			 0xd87d8219, /// 0x6a8
			 0xea6d961c, /// 0x6ac
			 0x2d87ac6d, /// 0x6b0
			 0x23357fe4, /// 0x6b4
			 0x7af8a0a0, /// 0x6b8
			 0x1d11b211, /// 0x6bc
			 0xaa88cee5, /// 0x6c0
			 0xb98b8465, /// 0x6c4
			 0xdede2ee1, /// 0x6c8
			 0x9503070c, /// 0x6cc
			 0x540f257b, /// 0x6d0
			 0x026743a7, /// 0x6d4
			 0xe541abb8, /// 0x6d8
			 0xba5f4f10, /// 0x6dc
			 0x5f002578, /// 0x6e0
			 0x52a8e27a, /// 0x6e4
			 0x799f4759, /// 0x6e8
			 0xb09a5879, /// 0x6ec
			 0x85397410, /// 0x6f0
			 0xce60633c, /// 0x6f4
			 0x299151a6, /// 0x6f8
			 0x29699406, /// 0x6fc
			 0x25262dad, /// 0x700
			 0x733ddf70, /// 0x704
			 0xde7c5b88, /// 0x708
			 0x9b541b43, /// 0x70c
			 0x4a49a13b, /// 0x710
			 0x8b226c4b, /// 0x714
			 0x9d5ab268, /// 0x718
			 0x744568d9, /// 0x71c
			 0x4f5848cf, /// 0x720
			 0xdfa2606e, /// 0x724
			 0xeb3c3ff5, /// 0x728
			 0xa86b3f9d, /// 0x72c
			 0xf9e913a4, /// 0x730
			 0x7e42f2d0, /// 0x734
			 0xdb687f9c, /// 0x738
			 0x32a57a96, /// 0x73c
			 0xde37bd12, /// 0x740
			 0xe907039d, /// 0x744
			 0x9d0fa4c3, /// 0x748
			 0xa122c424, /// 0x74c
			 0x874984ce, /// 0x750
			 0x4d8b517f, /// 0x754
			 0x9f2e13bd, /// 0x758
			 0x9e7836f8, /// 0x75c
			 0x4b3a4f08, /// 0x760
			 0x1f05d8ef, /// 0x764
			 0xe21779c4, /// 0x768
			 0x42d08b97, /// 0x76c
			 0x2db0ad89, /// 0x770
			 0x67717f22, /// 0x774
			 0x1703ca09, /// 0x778
			 0xa4a3ab0a, /// 0x77c
			 0xfe4b79ba, /// 0x780
			 0xaf42ae0d, /// 0x784
			 0xc02cbb2b, /// 0x788
			 0x75bd39b8, /// 0x78c
			 0xaaefa6eb, /// 0x790
			 0x84e77950, /// 0x794
			 0x7a3bd0b3, /// 0x798
			 0xb578a93d, /// 0x79c
			 0x626bd269, /// 0x7a0
			 0x63e479b9, /// 0x7a4
			 0x00a30185, /// 0x7a8
			 0x092aab00, /// 0x7ac
			 0x4b908a23, /// 0x7b0
			 0x598ab2ae, /// 0x7b4
			 0x48af91f2, /// 0x7b8
			 0xb89bac34, /// 0x7bc
			 0xb1275703, /// 0x7c0
			 0x2a378388, /// 0x7c4
			 0x31e4edf1, /// 0x7c8
			 0x3e25d667, /// 0x7cc
			 0x286f5295, /// 0x7d0
			 0xf1523a69, /// 0x7d4
			 0x8a4b8578, /// 0x7d8
			 0x78e6ec9f, /// 0x7dc
			 0x163aee05, /// 0x7e0
			 0x69071348, /// 0x7e4
			 0x08dd1bf5, /// 0x7e8
			 0x7148593d, /// 0x7ec
			 0xdc58ef10, /// 0x7f0
			 0x1801de16, /// 0x7f4
			 0x44cec555, /// 0x7f8
			 0x7e45fcd0, /// 0x7fc
			 0x38e5dbfe, /// 0x800
			 0xd7ecbd50, /// 0x804
			 0xe0814659, /// 0x808
			 0x04f0f401, /// 0x80c
			 0x27d2fd8b, /// 0x810
			 0xf046b731, /// 0x814
			 0xed738ce1, /// 0x818
			 0x58412ca9, /// 0x81c
			 0x484901c5, /// 0x820
			 0xeeaab009, /// 0x824
			 0xc38bf1cf, /// 0x828
			 0xbf6fbfd6, /// 0x82c
			 0x1fb55e67, /// 0x830
			 0x98dd4dae, /// 0x834
			 0x1f5f0473, /// 0x838
			 0xfdfc5375, /// 0x83c
			 0x7118fd25, /// 0x840
			 0x34a02b07, /// 0x844
			 0xb6bdb032, /// 0x848
			 0x356a33b3, /// 0x84c
			 0x0d0b3d23, /// 0x850
			 0x4842da1b, /// 0x854
			 0xf55daafb, /// 0x858
			 0xf899d746, /// 0x85c
			 0x77a41609, /// 0x860
			 0x25938b72, /// 0x864
			 0xd1421179, /// 0x868
			 0xb2850717, /// 0x86c
			 0x81469a5f, /// 0x870
			 0x8e532d3f, /// 0x874
			 0x8038da79, /// 0x878
			 0x7f1575ba, /// 0x87c
			 0x7068cc9b, /// 0x880
			 0x46034a61, /// 0x884
			 0x37a0bd3a, /// 0x888
			 0x4dca6ac6, /// 0x88c
			 0x6eb9ec89, /// 0x890
			 0xd9e211e8, /// 0x894
			 0x5cfa5fbd, /// 0x898
			 0x54f5c065, /// 0x89c
			 0xcfe01f25, /// 0x8a0
			 0x6f68b41f, /// 0x8a4
			 0xb001b119, /// 0x8a8
			 0x8e681301, /// 0x8ac
			 0xd28e6111, /// 0x8b0
			 0xa5afd931, /// 0x8b4
			 0x36a5ce14, /// 0x8b8
			 0x06ef2955, /// 0x8bc
			 0x4f846af3, /// 0x8c0
			 0x7bb65b97, /// 0x8c4
			 0x6aacfe67, /// 0x8c8
			 0x7224c80c, /// 0x8cc
			 0x2119037f, /// 0x8d0
			 0xc5a1d9bf, /// 0x8d4
			 0xcceed2c1, /// 0x8d8
			 0x96974be1, /// 0x8dc
			 0xb35a0722, /// 0x8e0
			 0x5ea41408, /// 0x8e4
			 0x72210a3a, /// 0x8e8
			 0xad32b0a5, /// 0x8ec
			 0x0139315a, /// 0x8f0
			 0x201f8000, /// 0x8f4
			 0x1c439270, /// 0x8f8
			 0x0db91b65, /// 0x8fc
			 0x1ce57249, /// 0x900
			 0xf58b2d74, /// 0x904
			 0x60edd362, /// 0x908
			 0x118986aa, /// 0x90c
			 0x5c106d67, /// 0x910
			 0x4419148d, /// 0x914
			 0x3f805a08, /// 0x918
			 0xceff8a94, /// 0x91c
			 0x0366ef13, /// 0x920
			 0x8e2a5bcf, /// 0x924
			 0x085646eb, /// 0x928
			 0x9165f8a2, /// 0x92c
			 0x72950e30, /// 0x930
			 0x7f878dc9, /// 0x934
			 0xda650432, /// 0x938
			 0xed7aeb51, /// 0x93c
			 0x116dfb08, /// 0x940
			 0x6898b7c7, /// 0x944
			 0x62d3e03d, /// 0x948
			 0x9f048aba, /// 0x94c
			 0xba5f568b, /// 0x950
			 0xacd42097, /// 0x954
			 0x270ecc1e, /// 0x958
			 0xb86f6e51, /// 0x95c
			 0xf3b1a8c6, /// 0x960
			 0x4635a679, /// 0x964
			 0x517de366, /// 0x968
			 0xf0835d09, /// 0x96c
			 0x4be31a63, /// 0x970
			 0x2e5c922d, /// 0x974
			 0x523dad0a, /// 0x978
			 0x159be3f1, /// 0x97c
			 0xaf3826d0, /// 0x980
			 0xd9592bf7, /// 0x984
			 0x59fd36d5, /// 0x988
			 0x02927fad, /// 0x98c
			 0xcb03f4c2, /// 0x990
			 0xf9b47d8d, /// 0x994
			 0xdfe8e558, /// 0x998
			 0xedd09369, /// 0x99c
			 0xe78cf550, /// 0x9a0
			 0x2154668a, /// 0x9a4
			 0xb639f34b, /// 0x9a8
			 0x4dfdbaad, /// 0x9ac
			 0x95af308b, /// 0x9b0
			 0xa643e41a, /// 0x9b4
			 0x2b2a9a58, /// 0x9b8
			 0xed55af82, /// 0x9bc
			 0xb90cd395, /// 0x9c0
			 0x04497eb7, /// 0x9c4
			 0x9aa3d0cf, /// 0x9c8
			 0x6cb993dc, /// 0x9cc
			 0xaece9f6d, /// 0x9d0
			 0x0aab32c8, /// 0x9d4
			 0x25c6fd6e, /// 0x9d8
			 0xcb9ed6fc, /// 0x9dc
			 0x92878349, /// 0x9e0
			 0xe634d108, /// 0x9e4
			 0xfa639f0b, /// 0x9e8
			 0xa81f7e0b, /// 0x9ec
			 0x614c6297, /// 0x9f0
			 0x0342a9cd, /// 0x9f4
			 0xd4972ee5, /// 0x9f8
			 0xb2263ca7, /// 0x9fc
			 0xe10c79ae, /// 0xa00
			 0xcc3fc3b2, /// 0xa04
			 0x043be081, /// 0xa08
			 0x4feec92e, /// 0xa0c
			 0xc48ccd18, /// 0xa10
			 0x397e608b, /// 0xa14
			 0x7abe4d7a, /// 0xa18
			 0x6597688d, /// 0xa1c
			 0xb86ff2ee, /// 0xa20
			 0x689fa617, /// 0xa24
			 0x4c986e06, /// 0xa28
			 0xc1ba91fd, /// 0xa2c
			 0xe44cf5c5, /// 0xa30
			 0xe0b6ac54, /// 0xa34
			 0xf63b3f3d, /// 0xa38
			 0xd66c5a34, /// 0xa3c
			 0x54ef4c6b, /// 0xa40
			 0x5a84fe46, /// 0xa44
			 0x96e02b37, /// 0xa48
			 0x11df91d1, /// 0xa4c
			 0x33d19e32, /// 0xa50
			 0x6ac5683e, /// 0xa54
			 0x663a3337, /// 0xa58
			 0x9bb5ec4d, /// 0xa5c
			 0x77899a6b, /// 0xa60
			 0xba8b2521, /// 0xa64
			 0x43d5481b, /// 0xa68
			 0x1689d5fa, /// 0xa6c
			 0x804a1053, /// 0xa70
			 0x03af98c7, /// 0xa74
			 0xb3d8b348, /// 0xa78
			 0x10dbe442, /// 0xa7c
			 0x0518fa48, /// 0xa80
			 0x504695ab, /// 0xa84
			 0xe704dcb8, /// 0xa88
			 0x85f640de, /// 0xa8c
			 0xf18c6e5d, /// 0xa90
			 0x7a5d03d3, /// 0xa94
			 0x93574412, /// 0xa98
			 0x7f85b906, /// 0xa9c
			 0x5aa7717a, /// 0xaa0
			 0xb9f65f40, /// 0xaa4
			 0x7c1b0d01, /// 0xaa8
			 0x116e79b7, /// 0xaac
			 0x84e38b66, /// 0xab0
			 0x5817e614, /// 0xab4
			 0x7b630d05, /// 0xab8
			 0x9f5a4139, /// 0xabc
			 0x15674b0e, /// 0xac0
			 0x3c3a2c55, /// 0xac4
			 0xc32bef8c, /// 0xac8
			 0xa7430a05, /// 0xacc
			 0xd7ffb924, /// 0xad0
			 0x917f6255, /// 0xad4
			 0x14499de8, /// 0xad8
			 0xb5f19133, /// 0xadc
			 0x886b179f, /// 0xae0
			 0x73049b77, /// 0xae4
			 0xa2b15065, /// 0xae8
			 0x82e0a3a1, /// 0xaec
			 0xb81ce3dc, /// 0xaf0
			 0xb10efbed, /// 0xaf4
			 0x2a7522e5, /// 0xaf8
			 0x8803a7ea, /// 0xafc
			 0xc9f8a6ef, /// 0xb00
			 0x1c810ac8, /// 0xb04
			 0x4d1ec499, /// 0xb08
			 0x3c19bcac, /// 0xb0c
			 0xe029664b, /// 0xb10
			 0xb7adb1f0, /// 0xb14
			 0x1c9d391a, /// 0xb18
			 0x659f904a, /// 0xb1c
			 0x76e89a94, /// 0xb20
			 0xd50b3019, /// 0xb24
			 0x19d553b0, /// 0xb28
			 0x2c16a35b, /// 0xb2c
			 0xede2f6bd, /// 0xb30
			 0x25a2a70f, /// 0xb34
			 0x7be00cc6, /// 0xb38
			 0xe0c5ef35, /// 0xb3c
			 0xf84e6ce6, /// 0xb40
			 0xea85f54e, /// 0xb44
			 0x59c75034, /// 0xb48
			 0x6f3d8ddd, /// 0xb4c
			 0xe5b85f49, /// 0xb50
			 0x90d1e6b3, /// 0xb54
			 0x8037dd07, /// 0xb58
			 0x8340e8d2, /// 0xb5c
			 0xe0c1ab9f, /// 0xb60
			 0xa50c56ed, /// 0xb64
			 0xc116b82a, /// 0xb68
			 0xc23bf98f, /// 0xb6c
			 0xc7f57157, /// 0xb70
			 0xf35dbb3b, /// 0xb74
			 0x543ac94e, /// 0xb78
			 0xc302856b, /// 0xb7c
			 0x6d9a4935, /// 0xb80
			 0x06b933bb, /// 0xb84
			 0xd4250780, /// 0xb88
			 0xf592e9de, /// 0xb8c
			 0x65be332a, /// 0xb90
			 0x037e50e1, /// 0xb94
			 0x83f81d99, /// 0xb98
			 0xfde33bb0, /// 0xb9c
			 0x93505070, /// 0xba0
			 0xd7d25209, /// 0xba4
			 0x55299727, /// 0xba8
			 0xca08fab6, /// 0xbac
			 0xe9f6d95c, /// 0xbb0
			 0xdbd47e0d, /// 0xbb4
			 0xc9b9e41d, /// 0xbb8
			 0x860eb626, /// 0xbbc
			 0x00087c7a, /// 0xbc0
			 0x3c46a60f, /// 0xbc4
			 0x54bec256, /// 0xbc8
			 0x2cfc0137, /// 0xbcc
			 0xbb58eb99, /// 0xbd0
			 0xa59e52b8, /// 0xbd4
			 0x8239899d, /// 0xbd8
			 0x8b3e3383, /// 0xbdc
			 0x52a9f5ad, /// 0xbe0
			 0x9d292b86, /// 0xbe4
			 0x8d08e445, /// 0xbe8
			 0x6be6a4e9, /// 0xbec
			 0x6c00904d, /// 0xbf0
			 0x2d9365d8, /// 0xbf4
			 0xf402b230, /// 0xbf8
			 0xffe48a91, /// 0xbfc
			 0x346d8833, /// 0xc00
			 0xb5cbfc4d, /// 0xc04
			 0x46de799f, /// 0xc08
			 0x8eb9cbec, /// 0xc0c
			 0x7a2aaadc, /// 0xc10
			 0x24d5fcd1, /// 0xc14
			 0x95bf9d9b, /// 0xc18
			 0xb7b7e093, /// 0xc1c
			 0x2659080d, /// 0xc20
			 0x81ffc34c, /// 0xc24
			 0x7bf33272, /// 0xc28
			 0x5f8d334e, /// 0xc2c
			 0x658243e1, /// 0xc30
			 0xe612440c, /// 0xc34
			 0x74a684c5, /// 0xc38
			 0x3672cdee, /// 0xc3c
			 0x4d568a83, /// 0xc40
			 0x815517a5, /// 0xc44
			 0xf20907fb, /// 0xc48
			 0xef1881c2, /// 0xc4c
			 0xe42edf97, /// 0xc50
			 0x70bd265d, /// 0xc54
			 0x3d323716, /// 0xc58
			 0x045a5fa5, /// 0xc5c
			 0x8a9650ae, /// 0xc60
			 0x1f316c94, /// 0xc64
			 0xd7219594, /// 0xc68
			 0x304e9833, /// 0xc6c
			 0x8d8e9d3b, /// 0xc70
			 0x6f5cb694, /// 0xc74
			 0xefdf6b81, /// 0xc78
			 0x6c8fd77b, /// 0xc7c
			 0x74e96f3f, /// 0xc80
			 0x4f0ec5b5, /// 0xc84
			 0x891e8549, /// 0xc88
			 0x53d3dc3b, /// 0xc8c
			 0x23b22d76, /// 0xc90
			 0xac417ea5, /// 0xc94
			 0x48056bba, /// 0xc98
			 0x9bd1713b, /// 0xc9c
			 0xda968911, /// 0xca0
			 0x2025f017, /// 0xca4
			 0xa2538901, /// 0xca8
			 0xb95507e3, /// 0xcac
			 0xf6f9e02a, /// 0xcb0
			 0x615754fd, /// 0xcb4
			 0x934509f8, /// 0xcb8
			 0xea09a3cc, /// 0xcbc
			 0x573d4f82, /// 0xcc0
			 0xf1031644, /// 0xcc4
			 0xac09cf2c, /// 0xcc8
			 0x9acbcc85, /// 0xccc
			 0xf5c59ad2, /// 0xcd0
			 0xa60a7083, /// 0xcd4
			 0xd31ecc8d, /// 0xcd8
			 0x553ffe13, /// 0xcdc
			 0x45aa0cea, /// 0xce0
			 0x77d93687, /// 0xce4
			 0x587babab, /// 0xce8
			 0xae079aaa, /// 0xcec
			 0x26417674, /// 0xcf0
			 0xf00c4d01, /// 0xcf4
			 0xe6443b6a, /// 0xcf8
			 0x75e409bf, /// 0xcfc
			 0x17ca22c7, /// 0xd00
			 0x87121859, /// 0xd04
			 0xc8a86afd, /// 0xd08
			 0x95a2ec10, /// 0xd0c
			 0x2bc1622f, /// 0xd10
			 0xba53e003, /// 0xd14
			 0x6eb0d03c, /// 0xd18
			 0x7c05c48f, /// 0xd1c
			 0x0e47460c, /// 0xd20
			 0x36d92e61, /// 0xd24
			 0x7eec6ff6, /// 0xd28
			 0x67432b25, /// 0xd2c
			 0xa044b26a, /// 0xd30
			 0xf233fffd, /// 0xd34
			 0x82d2b196, /// 0xd38
			 0x4eaad7c2, /// 0xd3c
			 0xf2a06f9a, /// 0xd40
			 0x77acc18a, /// 0xd44
			 0x3409ce20, /// 0xd48
			 0x2a2887da, /// 0xd4c
			 0x9d24794d, /// 0xd50
			 0x3e6dadf8, /// 0xd54
			 0x1364d1f3, /// 0xd58
			 0x33089bc6, /// 0xd5c
			 0xc8d150a6, /// 0xd60
			 0xb63b9624, /// 0xd64
			 0x0c4e9b07, /// 0xd68
			 0xd630c86c, /// 0xd6c
			 0x61338974, /// 0xd70
			 0x794bae8a, /// 0xd74
			 0xa79da244, /// 0xd78
			 0xf36734b8, /// 0xd7c
			 0x9fdc68de, /// 0xd80
			 0x84c851da, /// 0xd84
			 0x200fc730, /// 0xd88
			 0xa470a060, /// 0xd8c
			 0x19ab5ba4, /// 0xd90
			 0xabc0a54e, /// 0xd94
			 0x45e9fff9, /// 0xd98
			 0xaa45d487, /// 0xd9c
			 0x3728dd6a, /// 0xda0
			 0xd00ec7f5, /// 0xda4
			 0x1fd01be1, /// 0xda8
			 0xc54abcd5, /// 0xdac
			 0x4f035f82, /// 0xdb0
			 0xd2e5df4f, /// 0xdb4
			 0x319dd8e4, /// 0xdb8
			 0x83745774, /// 0xdbc
			 0xef64e711, /// 0xdc0
			 0xe370cf3f, /// 0xdc4
			 0x2af1ec04, /// 0xdc8
			 0x7be1efdf, /// 0xdcc
			 0xa6fa6349, /// 0xdd0
			 0x5f4159f3, /// 0xdd4
			 0xa9c4afd5, /// 0xdd8
			 0x21f16bc4, /// 0xddc
			 0x75c9f53e, /// 0xde0
			 0xa6b53d5b, /// 0xde4
			 0x41aa9ff8, /// 0xde8
			 0x17555d74, /// 0xdec
			 0x50590298, /// 0xdf0
			 0x72a973aa, /// 0xdf4
			 0x7ee14ed1, /// 0xdf8
			 0xfd0f3b33, /// 0xdfc
			 0x21640e98, /// 0xe00
			 0x2c897a88, /// 0xe04
			 0x4cedb8b4, /// 0xe08
			 0x19bde2ca, /// 0xe0c
			 0x04e419da, /// 0xe10
			 0x4ad896b9, /// 0xe14
			 0xc4000542, /// 0xe18
			 0x26223631, /// 0xe1c
			 0x7424d30a, /// 0xe20
			 0x17495088, /// 0xe24
			 0x306a3327, /// 0xe28
			 0xc17fe49c, /// 0xe2c
			 0xad09d598, /// 0xe30
			 0xb7bc3e6c, /// 0xe34
			 0xe78e9efd, /// 0xe38
			 0xf76484a0, /// 0xe3c
			 0xef5d7be7, /// 0xe40
			 0xe06913f7, /// 0xe44
			 0x3393155b, /// 0xe48
			 0x61744d86, /// 0xe4c
			 0x94d5df87, /// 0xe50
			 0x4f958818, /// 0xe54
			 0x5cefc93d, /// 0xe58
			 0x04029463, /// 0xe5c
			 0xc956e8b5, /// 0xe60
			 0xe9124f67, /// 0xe64
			 0x9abda95f, /// 0xe68
			 0x299e1ff2, /// 0xe6c
			 0x62f27133, /// 0xe70
			 0xb0d67e52, /// 0xe74
			 0x36905316, /// 0xe78
			 0xef23d023, /// 0xe7c
			 0xa36683a0, /// 0xe80
			 0xec5112dc, /// 0xe84
			 0x84ffc051, /// 0xe88
			 0x79e07250, /// 0xe8c
			 0xe5a20e7c, /// 0xe90
			 0x8ac858b7, /// 0xe94
			 0xb2b7cff1, /// 0xe98
			 0x309fc47b, /// 0xe9c
			 0x65672830, /// 0xea0
			 0x106654d4, /// 0xea4
			 0x024359c9, /// 0xea8
			 0x63b5446a, /// 0xeac
			 0x8f6ee4f9, /// 0xeb0
			 0xb7510abd, /// 0xeb4
			 0xf12a06e1, /// 0xeb8
			 0x79e271e7, /// 0xebc
			 0x9fc1c76b, /// 0xec0
			 0x66fb59fc, /// 0xec4
			 0xf75d6997, /// 0xec8
			 0xf1ea67d6, /// 0xecc
			 0xb3b09e5c, /// 0xed0
			 0x9b55b8a2, /// 0xed4
			 0x146e765c, /// 0xed8
			 0xd344166c, /// 0xedc
			 0x44b6de8e, /// 0xee0
			 0xcfed40dd, /// 0xee4
			 0x55d55799, /// 0xee8
			 0x022daf0b, /// 0xeec
			 0x2da6811e, /// 0xef0
			 0xe5ad2da7, /// 0xef4
			 0xbe80d6bb, /// 0xef8
			 0x834a2b9a, /// 0xefc
			 0x48dd4994, /// 0xf00
			 0xada943fc, /// 0xf04
			 0x22f79b43, /// 0xf08
			 0x7ffe2876, /// 0xf0c
			 0xaf7fa9e6, /// 0xf10
			 0xb25b0e30, /// 0xf14
			 0x1411e64c, /// 0xf18
			 0x55d612a7, /// 0xf1c
			 0xa72c2487, /// 0xf20
			 0x47ade070, /// 0xf24
			 0x0124b477, /// 0xf28
			 0xca082a93, /// 0xf2c
			 0xfccab219, /// 0xf30
			 0x8e23e3be, /// 0xf34
			 0xd34c77ce, /// 0xf38
			 0x70078c74, /// 0xf3c
			 0x7a7a08d0, /// 0xf40
			 0xa85b436f, /// 0xf44
			 0x45f76d0b, /// 0xf48
			 0x691f0009, /// 0xf4c
			 0x626b67d7, /// 0xf50
			 0x9b284f25, /// 0xf54
			 0x23d00407, /// 0xf58
			 0x58d238a9, /// 0xf5c
			 0x7db554b8, /// 0xf60
			 0x545788af, /// 0xf64
			 0xd6be7b24, /// 0xf68
			 0x71ab1ccb, /// 0xf6c
			 0x7fbeaaaa, /// 0xf70
			 0xf3a24ec8, /// 0xf74
			 0x635fdaa5, /// 0xf78
			 0xbf85607d, /// 0xf7c
			 0xd5f9c2d6, /// 0xf80
			 0x2a9edd35, /// 0xf84
			 0xc2ca558b, /// 0xf88
			 0xcb992b78, /// 0xf8c
			 0x71ef9616, /// 0xf90
			 0xf841c024, /// 0xf94
			 0xbc42304e, /// 0xf98
			 0x12cca535, /// 0xf9c
			 0xcfc0ff73, /// 0xfa0
			 0x4d4c4c25, /// 0xfa4
			 0x4a4a85f0, /// 0xfa8
			 0xf65e8ea3, /// 0xfac
			 0x2e4a7178, /// 0xfb0
			 0x986fbb87, /// 0xfb4
			 0xe8b74699, /// 0xfb8
			 0x024d0621, /// 0xfbc
			 0x9c19ff9f, /// 0xfc0
			 0x605a3981, /// 0xfc4
			 0x1f748bb9, /// 0xfc8
			 0x9a4f2315, /// 0xfcc
			 0xb5a1d2ae, /// 0xfd0
			 0xdc869bfc, /// 0xfd4
			 0xabd15c00, /// 0xfd8
			 0x12165480, /// 0xfdc
			 0xa3041804, /// 0xfe0
			 0x53dd0c8d, /// 0xfe4
			 0x205ed123, /// 0xfe8
			 0x47ea8adb, /// 0xfec
			 0x1728d96c, /// 0xff0
			 0x11dd8ef7, /// 0xff4
			 0x6b7d72fd, /// 0xff8
			 0x391762eb /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00010
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00018
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0001c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0xcb000000,                                                  // -8388608.0                                  /// 00024
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x33333333,                                                  // 4 random values                             /// 0002c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x00011111,                                                  // 9.7958E-41                                  /// 00034
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00038
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00044
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0004c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x00011111,                                                  // 9.7958E-41                                  /// 00058
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00060
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00064
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0006c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00074
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00084
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00088
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0008c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00090
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00094
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00098
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0009c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xffc00001,                                                  // -signaling NaN                              /// 000d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xffc00001,                                                  // -signaling NaN                              /// 000e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00100
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00104
			 0x7fc00001,                                                  // signaling NaN                               /// 00108
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00110
			 0x3f028f5c,                                                  // 0.51                                        /// 00114
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0011c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00120
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00128
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00130
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00134
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00138
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0013c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00140
			 0xbf028f5c,                                                  // -0.51                                       /// 00144
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00148
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00150
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00154
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00158
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0015c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00160
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00168
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0016c
			 0x7fc00001,                                                  // signaling NaN                               /// 00170
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00178
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00184
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00188
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0019c
			 0xbf028f5c,                                                  // -0.51                                       /// 001a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001b8
			 0xbf028f5c,                                                  // -0.51                                       /// 001bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x7fc00001,                                                  // signaling NaN                               /// 001d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00200
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00208
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00220
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00224
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00234
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0023c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00240
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00244
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0024c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00250
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00254
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00258
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0025c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00268
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00274
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00278
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0027c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00280
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00288
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0028c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00290
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00294
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002bc
			 0xff800000,                                                  // -inf                                        /// 002c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002f8
			 0x55555555,                                                  // 4 random values                             /// 002fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00300
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0030c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00310
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00314
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00318
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0031c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00320
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00324
			 0xcb000000,                                                  // -8388608.0                                  /// 00328
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x3f028f5c,                                                  // 0.51                                        /// 00338
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0033c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00340
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00344
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00348
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00350
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00358
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00364
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00368
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0c780000,                                                  // Leading 1s:                                 /// 00374
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00378
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00380
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00384
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00390
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00394
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00398
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x4b000000,                                                  // 8388608.0                                   /// 003b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x33333333,                                                  // 4 random values                             /// 003f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00400
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00404
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00410
			 0x3f028f5c,                                                  // 0.51                                        /// 00414
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0041c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00424
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00428
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0042c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00430
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0043c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00440
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x7fc00001,                                                  // signaling NaN                               /// 00448
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0044c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00450
			 0x00011111,                                                  // 9.7958E-41                                  /// 00454
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00458
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00474
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0047c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00484
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00488
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0048c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00494
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00498
			 0xffc00001,                                                  // -signaling NaN                              /// 0049c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00504
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00508
			 0x00000000,                                                  // zero                                        /// 0050c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00510
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00514
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0051c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0c400000,                                                  // Leading 1s:                                 /// 0052c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00534
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00538
			 0x0c700000,                                                  // Leading 1s:                                 /// 0053c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00540
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00548
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0054c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0055c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00560
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00564
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00568
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0056c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00574
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00578
			 0x80011111,                                                  // -9.7958E-41                                 /// 0057c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00580
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00588
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00590
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00594
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00598
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005b8
			 0x3f028f5c,                                                  // 0.51                                        /// 005bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00600
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00604
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00608
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0060c
			 0xbf028f5c,                                                  // -0.51                                       /// 00610
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00614
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00618
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0061c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00620
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00624
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00628
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00630
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00634
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00638
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00640
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00648
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00654
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00658
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0065c
			 0x3f028f5c,                                                  // 0.51                                        /// 00660
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00664
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00668
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0066c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00670
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x7fc00001,                                                  // signaling NaN                               /// 0067c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00680
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00684
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00688
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00690
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00694
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0069c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x55555555,                                                  // 4 random values                             /// 006c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0xbf028f5c,                                                  // -0.51                                       /// 00708
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0070c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00710
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00714
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00718
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0071c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00720
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00724
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00728
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0072c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00734
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0073c
			 0x33333333,                                                  // 4 random values                             /// 00740
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00744
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00748
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00750
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0075c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00768
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0076c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00778
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00780
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00784
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00788
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x00000000,                                                  // zero                                        /// 00798
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xffc00001,                                                  // -signaling NaN                              /// 007b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0xcb000000,                                                  // -8388608.0                                  /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00804
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0080c
			 0xff800000,                                                  // -inf                                        /// 00810
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00818
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00820
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00824
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00828
			 0x7fc00001,                                                  // signaling NaN                               /// 0082c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00830
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0083c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00840
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00844
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0084c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00854
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00858
			 0x4b000000,                                                  // 8388608.0                                   /// 0085c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00860
			 0x0c400000,                                                  // Leading 1s:                                 /// 00864
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00868
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00870
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0xcb000000,                                                  // -8388608.0                                  /// 00878
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0087c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00880
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00884
			 0x0c780000,                                                  // Leading 1s:                                 /// 00888
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00890
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00894
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00898
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x00000000,                                                  // zero                                        /// 008d8
			 0xff800000,                                                  // -inf                                        /// 008dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00904
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00908
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0090c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00910
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00000000,                                                  // zero                                        /// 0091c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0092c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00930
			 0x55555555,                                                  // 4 random values                             /// 00934
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00938
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00940
			 0xbf028f5c,                                                  // -0.51                                       /// 00944
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00950
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x0e000003,                                                  // Trailing 1s:                                /// 00958
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0095c
			 0x3f028f5c,                                                  // 0.51                                        /// 00960
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x0c780000,                                                  // Leading 1s:                                 /// 00968
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x4b000000,                                                  // 8388608.0                                   /// 00970
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x55555555,                                                  // 4 random values                             /// 00978
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0097c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00980
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00984
			 0x00011111,                                                  // 9.7958E-41                                  /// 00988
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00990
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00994
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00998
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0099c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80000000,                                                  // -zero                                       /// 009ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xff800000,                                                  // -inf                                        /// 009fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a64
			 0xff800000,                                                  // -inf                                        /// 00a68
			 0xff800000,                                                  // -inf                                        /// 00a6c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a94
			 0xff800000,                                                  // -inf                                        /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xbf028f5c,                                                  // -0.51                                       /// 00aa8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ab0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ac0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00acc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x4b000000,                                                  // 8388608.0                                   /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b2c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b64
			 0x33333333,                                                  // 4 random values                             /// 00b68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b8c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bb8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bcc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00be4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00be8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c8c
			 0x80000000,                                                  // -zero                                       /// 00c90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c9c
			 0x33333333,                                                  // 4 random values                             /// 00ca0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ce8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0xffc00001,                                                  // -signaling NaN                              /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x55555555,                                                  // 4 random values                             /// 00d14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x33333333,                                                  // 4 random values                             /// 00d30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x4b000000,                                                  // 8388608.0                                   /// 00d4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d68
			 0x80000000,                                                  // -zero                                       /// 00d6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00da0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00da8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dac
			 0x7f800000,                                                  // inf                                         /// 00db0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00db8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00de4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df0
			 0x4b000000,                                                  // 8388608.0                                   /// 00df4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e88
			 0x4b000000,                                                  // 8388608.0                                   /// 00e8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ec4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ecc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed4
			 0x80000000,                                                  // -zero                                       /// 00ed8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ef0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xbf028f5c,                                                  // -0.51                                       /// 00efc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f5c
			 0x00000000,                                                  // zero                                        /// 00f60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fb4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00000000,                                                  // zero                                        /// 00fdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fe4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff8
			 0x0e000007                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x7f800000,                                                  // inf                                         /// 00004
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00008
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0000c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00010
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00018
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00020
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00024
			 0x0c400000,                                                  // Leading 1s:                                 /// 00028
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00030
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0003c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00040
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00050
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00054
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00058
			 0x7fc00001,                                                  // signaling NaN                               /// 0005c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00060
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0006c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00078
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00080
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0c400000,                                                  // Leading 1s:                                 /// 00088
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0008c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00090
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00098
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0009c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c8
			 0x3f028f5c,                                                  // 0.51                                        /// 000cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e4
			 0x7f800000,                                                  // inf                                         /// 000e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7f800000,                                                  // inf                                         /// 00100
			 0x80000000,                                                  // -zero                                       /// 00104
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0011c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00120
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00128
			 0x0c400000,                                                  // Leading 1s:                                 /// 0012c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00130
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00138
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0013c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00144
			 0x55555555,                                                  // 4 random values                             /// 00148
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0014c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00158
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00160
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00164
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00168
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0016c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00174
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0017c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0019c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b4
			 0xff800000,                                                  // -inf                                        /// 001b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001c8
			 0x00000000,                                                  // zero                                        /// 001cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00200
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80011111,                                                  // -9.7958E-41                                 /// 00208
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0020c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00210
			 0x0e000001,                                                  // Trailing 1s:                                /// 00214
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00220
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x3f028f5c,                                                  // 0.51                                        /// 00228
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0022c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0023c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00240
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00244
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0024c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0026c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00270
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00274
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00278
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0027c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00280
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00284
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00288
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00290
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00294
			 0x7f800000,                                                  // inf                                         /// 00298
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0029c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002b4
			 0x3f028f5c,                                                  // 0.51                                        /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c8
			 0x4b000000,                                                  // 8388608.0                                   /// 002cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x80011111,                                                  // -9.7958E-41                                 /// 00304
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0030c
			 0xff800000,                                                  // -inf                                        /// 00310
			 0x0c780000,                                                  // Leading 1s:                                 /// 00314
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00318
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0031c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00320
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00324
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0xcb000000,                                                  // -8388608.0                                  /// 0032c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0033c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00340
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0034c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00350
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00354
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00358
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00360
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00364
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00368
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0036c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00374
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00378
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0037c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00380
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00384
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0038c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00390
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00394
			 0x33333333,                                                  // 4 random values                             /// 00398
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80000000,                                                  // -zero                                       /// 00404
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00408
			 0x00011111,                                                  // 9.7958E-41                                  /// 0040c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00418
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00430
			 0x55555555,                                                  // 4 random values                             /// 00434
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00438
			 0xbf028f5c,                                                  // -0.51                                       /// 0043c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00444
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00448
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00450
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00454
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00458
			 0x0e000003,                                                  // Trailing 1s:                                /// 0045c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0046c
			 0x3f028f5c,                                                  // 0.51                                        /// 00470
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00474
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00478
			 0x0e000003,                                                  // Trailing 1s:                                /// 0047c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00488
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00490
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00494
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00498
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0049c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x4b000000,                                                  // 8388608.0                                   /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x55555555,                                                  // 4 random values                             /// 004c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c4
			 0x3f028f5c,                                                  // 0.51                                        /// 004c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d8
			 0x33333333,                                                  // 4 random values                             /// 004dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ec
			 0x7f800000,                                                  // inf                                         /// 004f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00500
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00504
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00510
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00514
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00518
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0051c
			 0x33333333,                                                  // 4 random values                             /// 00520
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00524
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00528
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00530
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00534
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00538
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0053c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00540
			 0x7f800000,                                                  // inf                                         /// 00544
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00548
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00558
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0055c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00560
			 0xbf028f5c,                                                  // -0.51                                       /// 00564
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00568
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0056c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00574
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00584
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0058c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00590
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00594
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00598
			 0x0c780000,                                                  // Leading 1s:                                 /// 0059c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005cc
			 0x3f028f5c,                                                  // 0.51                                        /// 005d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00604
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00608
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0060c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00610
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00614
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00620
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0062c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0e000003,                                                  // Trailing 1s:                                /// 00638
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00640
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00644
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00650
			 0x0c700000,                                                  // Leading 1s:                                 /// 00654
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00660
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x0e000007,                                                  // Trailing 1s:                                /// 00668
			 0x7fc00001,                                                  // signaling NaN                               /// 0066c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00670
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00674
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0067c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00680
			 0x0e000007,                                                  // Trailing 1s:                                /// 00684
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00688
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0068c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x55555555,                                                  // 4 random values                             /// 00698
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ec
			 0x3f028f5c,                                                  // 0.51                                        /// 006f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00700
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00708
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0070c
			 0x7f800000,                                                  // inf                                         /// 00710
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00718
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00724
			 0x00000000,                                                  // zero                                        /// 00728
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0072c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00730
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00734
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00738
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00740
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00744
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0074c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00750
			 0xffc00001,                                                  // -signaling NaN                              /// 00754
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00758
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0075c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00760
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00764
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00768
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x0c600000,                                                  // Leading 1s:                                 /// 00774
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00778
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00780
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00784
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x4b000000,                                                  // 8388608.0                                   /// 0078c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00790
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00794
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00798
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0079c
			 0x7fc00001,                                                  // signaling NaN                               /// 007a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00800
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00804
			 0x3f028f5c,                                                  // 0.51                                        /// 00808
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00814
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0081c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00820
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00828
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00830
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00834
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00838
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0083c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00840
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00844
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00848
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0084c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00858
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00864
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00868
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0086c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00870
			 0x0c700000,                                                  // Leading 1s:                                 /// 00874
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0e000007,                                                  // Trailing 1s:                                /// 00888
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0088c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x4b000000,                                                  // 8388608.0                                   /// 00894
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xbf028f5c,                                                  // -0.51                                       /// 0089c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b0
			 0x7fc00001,                                                  // signaling NaN                               /// 008b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c8
			 0x3f028f5c,                                                  // 0.51                                        /// 008cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 008f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00900
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00910
			 0x80011111,                                                  // -9.7958E-41                                 /// 00914
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00924
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00938
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0093c
			 0x33333333,                                                  // 4 random values                             /// 00940
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x4b000000,                                                  // 8388608.0                                   /// 0094c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00950
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00954
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0095c
			 0x7f800000,                                                  // inf                                         /// 00960
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00964
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00968
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00970
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00978
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0097c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00980
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00984
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00988
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00994
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0099c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009a0
			 0xcb000000,                                                  // -8388608.0                                  /// 009a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009dc
			 0xcb000000,                                                  // -8388608.0                                  /// 009e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a3c
			 0x55555555,                                                  // 4 random values                             /// 00a40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a58
			 0x4b000000,                                                  // 8388608.0                                   /// 00a5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a90
			 0x33333333,                                                  // 4 random values                             /// 00a94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ab8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00abc
			 0x00000000,                                                  // zero                                        /// 00ac0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00acc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ad8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x55555555,                                                  // 4 random values                             /// 00af0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00afc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x7f800000,                                                  // inf                                         /// 00b54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b60
			 0xffc00001,                                                  // -signaling NaN                              /// 00b64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x4b000000,                                                  // 8388608.0                                   /// 00b84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ba0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ba8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb0
			 0x7f800000,                                                  // inf                                         /// 00bb4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0xffc00001,                                                  // -signaling NaN                              /// 00be8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bf8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bfc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0xff800000,                                                  // -inf                                        /// 00c04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cb8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ccc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd8
			 0x00000000,                                                  // zero                                        /// 00cdc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce4
			 0x00000000,                                                  // zero                                        /// 00ce8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7f800000,                                                  // inf                                         /// 00cf4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d08
			 0xcb000000,                                                  // -8388608.0                                  /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d50
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00db0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00db4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dfc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x33333333,                                                  // 4 random values                             /// 00e58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ea4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ebc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ec0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00eec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00efc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x7f800000,                                                  // inf                                         /// 00f38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f6c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00fa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000000,                                                  // -zero                                       /// 00fc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fcc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fe0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff8
			 0x0e000003                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xb9eeb09f, /// 0x0
			 0xc0a862c0, /// 0x4
			 0xa52646a4, /// 0x8
			 0x7d001466, /// 0xc
			 0xf66b4e94, /// 0x10
			 0x4d0531f6, /// 0x14
			 0x3a4d8286, /// 0x18
			 0xea1c7163, /// 0x1c
			 0xaa3c4468, /// 0x20
			 0xf6e3b89b, /// 0x24
			 0x25913148, /// 0x28
			 0xdf06e256, /// 0x2c
			 0x8885551c, /// 0x30
			 0x175f7b04, /// 0x34
			 0x55bc4e70, /// 0x38
			 0x16c3dde4, /// 0x3c
			 0xb424f42b, /// 0x40
			 0xa590031d, /// 0x44
			 0x308c75e0, /// 0x48
			 0x66cf9ba9, /// 0x4c
			 0xe8b5c922, /// 0x50
			 0x37aeb71c, /// 0x54
			 0x94640cb6, /// 0x58
			 0x2e177c82, /// 0x5c
			 0x5bc363dd, /// 0x60
			 0x755369e7, /// 0x64
			 0x56067e82, /// 0x68
			 0xa0f25166, /// 0x6c
			 0x6f18a81f, /// 0x70
			 0x2fc9c274, /// 0x74
			 0x232d386c, /// 0x78
			 0xb6c797ad, /// 0x7c
			 0x3152b8ed, /// 0x80
			 0xe4265a3e, /// 0x84
			 0x3377137c, /// 0x88
			 0x2d1e9c3f, /// 0x8c
			 0x1a3f26da, /// 0x90
			 0x499f2122, /// 0x94
			 0xdaab42d5, /// 0x98
			 0x4804b052, /// 0x9c
			 0xcb559d5a, /// 0xa0
			 0xb4ad7e77, /// 0xa4
			 0x9270baa1, /// 0xa8
			 0xb95db599, /// 0xac
			 0xfd9dd080, /// 0xb0
			 0x652111d2, /// 0xb4
			 0xbf0b4ca1, /// 0xb8
			 0xd3a650c0, /// 0xbc
			 0xec67d86c, /// 0xc0
			 0x3139d69f, /// 0xc4
			 0x0cfd9be0, /// 0xc8
			 0x9596dd4e, /// 0xcc
			 0xbdcd661e, /// 0xd0
			 0x9cda4268, /// 0xd4
			 0xe36538d7, /// 0xd8
			 0x0fbaa039, /// 0xdc
			 0x072d67b6, /// 0xe0
			 0x5abbaa17, /// 0xe4
			 0x15ec776b, /// 0xe8
			 0x4cae7641, /// 0xec
			 0x9323b394, /// 0xf0
			 0x358e3827, /// 0xf4
			 0x85905574, /// 0xf8
			 0xa5eee028, /// 0xfc
			 0x4e845e52, /// 0x100
			 0x143ff0ed, /// 0x104
			 0xa911f972, /// 0x108
			 0x0ab47108, /// 0x10c
			 0x03900a55, /// 0x110
			 0xbe1a22f2, /// 0x114
			 0x17a44076, /// 0x118
			 0x72914b48, /// 0x11c
			 0xefa0bc83, /// 0x120
			 0xdba34199, /// 0x124
			 0xf9675f02, /// 0x128
			 0x4e98ae0d, /// 0x12c
			 0x774615d2, /// 0x130
			 0x1fac5f88, /// 0x134
			 0x5c618033, /// 0x138
			 0x5d5996a5, /// 0x13c
			 0x6a18cc55, /// 0x140
			 0xbdfcb77a, /// 0x144
			 0x9de51b0a, /// 0x148
			 0x424d5d83, /// 0x14c
			 0x17cd7c71, /// 0x150
			 0x6c303558, /// 0x154
			 0x38b82d4b, /// 0x158
			 0xc0e548d5, /// 0x15c
			 0xec11db42, /// 0x160
			 0x0a75bd03, /// 0x164
			 0xae6ad8f2, /// 0x168
			 0x71672e59, /// 0x16c
			 0x0402a4c4, /// 0x170
			 0x3ccb49a3, /// 0x174
			 0x0ced4ac8, /// 0x178
			 0x2a23545d, /// 0x17c
			 0x72f5eb99, /// 0x180
			 0xb179bbe6, /// 0x184
			 0xfa269ddb, /// 0x188
			 0x51276c72, /// 0x18c
			 0xc9b15f47, /// 0x190
			 0xcad7b009, /// 0x194
			 0x65c35ad6, /// 0x198
			 0x78944320, /// 0x19c
			 0x3f37a7f5, /// 0x1a0
			 0xeffceffb, /// 0x1a4
			 0xcdaaf15e, /// 0x1a8
			 0xdb168dc3, /// 0x1ac
			 0x79b2d2d5, /// 0x1b0
			 0x587b6fe9, /// 0x1b4
			 0xda4e33e1, /// 0x1b8
			 0x37469f17, /// 0x1bc
			 0xfbc1612e, /// 0x1c0
			 0x3fb072e8, /// 0x1c4
			 0xe75878a2, /// 0x1c8
			 0x2616c03c, /// 0x1cc
			 0xb48ea804, /// 0x1d0
			 0x747d0032, /// 0x1d4
			 0x4c5f47a4, /// 0x1d8
			 0x2519a5ab, /// 0x1dc
			 0x54e65877, /// 0x1e0
			 0xcaac8b4e, /// 0x1e4
			 0xd70cc2aa, /// 0x1e8
			 0x15a7f36f, /// 0x1ec
			 0x31fb901a, /// 0x1f0
			 0xa6d80c9a, /// 0x1f4
			 0xa6ffc6e8, /// 0x1f8
			 0x2899e04c, /// 0x1fc
			 0x68fe7c04, /// 0x200
			 0x9fecb03c, /// 0x204
			 0x4982516e, /// 0x208
			 0x60d9e246, /// 0x20c
			 0x89e1a098, /// 0x210
			 0x618c5bfc, /// 0x214
			 0x4af298a0, /// 0x218
			 0xb52f4363, /// 0x21c
			 0x871a4557, /// 0x220
			 0xb3c8a0bb, /// 0x224
			 0xb6d5ee5f, /// 0x228
			 0xefeeab2e, /// 0x22c
			 0x34820b31, /// 0x230
			 0x85b7bb24, /// 0x234
			 0x3124f1f6, /// 0x238
			 0x7ce6e474, /// 0x23c
			 0x9a30ebdf, /// 0x240
			 0xabc20ce0, /// 0x244
			 0xa0d16909, /// 0x248
			 0x06bd5939, /// 0x24c
			 0x2096a7c2, /// 0x250
			 0xaaa143ab, /// 0x254
			 0xe43c27c2, /// 0x258
			 0x89d1246d, /// 0x25c
			 0xcaf6abdd, /// 0x260
			 0xa4257453, /// 0x264
			 0xd25908c9, /// 0x268
			 0xbdadb25c, /// 0x26c
			 0x948f2082, /// 0x270
			 0x4628e8fd, /// 0x274
			 0xb17afefe, /// 0x278
			 0x7f5c4adc, /// 0x27c
			 0x4687c4ea, /// 0x280
			 0x76cc9df7, /// 0x284
			 0xd4bd1f56, /// 0x288
			 0xa2d1bd81, /// 0x28c
			 0x4182214b, /// 0x290
			 0x91b3668d, /// 0x294
			 0x12fdd250, /// 0x298
			 0xa36f53a6, /// 0x29c
			 0x2323873e, /// 0x2a0
			 0xaefc7d12, /// 0x2a4
			 0x7b9be6cb, /// 0x2a8
			 0xc67d0f3f, /// 0x2ac
			 0x18d89872, /// 0x2b0
			 0x4bfe1cc7, /// 0x2b4
			 0x4db74904, /// 0x2b8
			 0x20b8bd23, /// 0x2bc
			 0x6220769c, /// 0x2c0
			 0x179e2a57, /// 0x2c4
			 0xf556c292, /// 0x2c8
			 0xeada39f4, /// 0x2cc
			 0xb0226cdc, /// 0x2d0
			 0x5deb8bfc, /// 0x2d4
			 0x108a4483, /// 0x2d8
			 0x1804abc8, /// 0x2dc
			 0xdafc5d97, /// 0x2e0
			 0x8c6cdf2a, /// 0x2e4
			 0x278e48b5, /// 0x2e8
			 0xd4f239af, /// 0x2ec
			 0x49e5b8d2, /// 0x2f0
			 0x2b6ca553, /// 0x2f4
			 0xf647a34d, /// 0x2f8
			 0x46c3d261, /// 0x2fc
			 0xb3584db6, /// 0x300
			 0x2bbaa640, /// 0x304
			 0xaa235e34, /// 0x308
			 0xd418989a, /// 0x30c
			 0x27b21e22, /// 0x310
			 0x91f90088, /// 0x314
			 0xbf71c250, /// 0x318
			 0xfe45bf04, /// 0x31c
			 0x859561f0, /// 0x320
			 0xd8d05071, /// 0x324
			 0xc2c444fb, /// 0x328
			 0x89638f3a, /// 0x32c
			 0xc7d03dd5, /// 0x330
			 0x86446453, /// 0x334
			 0x7f7dde3d, /// 0x338
			 0x6b4e6ffd, /// 0x33c
			 0x3e646e89, /// 0x340
			 0x7fc88341, /// 0x344
			 0xc964803d, /// 0x348
			 0xedeb6511, /// 0x34c
			 0xf8cb92ce, /// 0x350
			 0x054ce782, /// 0x354
			 0x89229a13, /// 0x358
			 0x9fcee868, /// 0x35c
			 0x2f5a1b81, /// 0x360
			 0x488ac1c0, /// 0x364
			 0xe03cde45, /// 0x368
			 0x1f59a228, /// 0x36c
			 0xd39a7a50, /// 0x370
			 0xd5660281, /// 0x374
			 0x7d98344f, /// 0x378
			 0xb64e8bfa, /// 0x37c
			 0x5b84954f, /// 0x380
			 0x73afda58, /// 0x384
			 0xad764959, /// 0x388
			 0x84d3a4b5, /// 0x38c
			 0x82e87ec4, /// 0x390
			 0x9fbe89b0, /// 0x394
			 0x3f777f4a, /// 0x398
			 0x1a1d9b19, /// 0x39c
			 0x292e1113, /// 0x3a0
			 0x0128c991, /// 0x3a4
			 0x4cff884d, /// 0x3a8
			 0x5405c432, /// 0x3ac
			 0x51e5a4c3, /// 0x3b0
			 0xcfb40ff8, /// 0x3b4
			 0x342c325c, /// 0x3b8
			 0x6bba7f39, /// 0x3bc
			 0xa0e48047, /// 0x3c0
			 0x941ec8a2, /// 0x3c4
			 0x7cebb53e, /// 0x3c8
			 0xb29ff0f0, /// 0x3cc
			 0x191a86ae, /// 0x3d0
			 0x8a31d9b9, /// 0x3d4
			 0x0a767b15, /// 0x3d8
			 0x886c150c, /// 0x3dc
			 0xfc70dca1, /// 0x3e0
			 0x51854756, /// 0x3e4
			 0x5f75f118, /// 0x3e8
			 0x78cc8bcb, /// 0x3ec
			 0x9ca9dccc, /// 0x3f0
			 0xee7c9c77, /// 0x3f4
			 0x6e62c6b0, /// 0x3f8
			 0x5008cb3a, /// 0x3fc
			 0xaeea6b59, /// 0x400
			 0x700e90fc, /// 0x404
			 0x7cf4675e, /// 0x408
			 0x1d69e3ed, /// 0x40c
			 0xc76fe98e, /// 0x410
			 0x84f55798, /// 0x414
			 0xa33bf19f, /// 0x418
			 0x226daff1, /// 0x41c
			 0x7c1115b5, /// 0x420
			 0xdcd372e1, /// 0x424
			 0x941d1968, /// 0x428
			 0x3fb3a1e6, /// 0x42c
			 0x4534e4f2, /// 0x430
			 0xa5381110, /// 0x434
			 0xdf41464a, /// 0x438
			 0xc26021f5, /// 0x43c
			 0x128c7d65, /// 0x440
			 0xf65761ff, /// 0x444
			 0x8db88117, /// 0x448
			 0x5a06c615, /// 0x44c
			 0xd5898541, /// 0x450
			 0x71458e6f, /// 0x454
			 0xa8273da2, /// 0x458
			 0x7a73b18b, /// 0x45c
			 0xd3252ee1, /// 0x460
			 0x5810eb1e, /// 0x464
			 0x232c86c2, /// 0x468
			 0x2ab913ac, /// 0x46c
			 0xba0588d8, /// 0x470
			 0x530c9141, /// 0x474
			 0xee248113, /// 0x478
			 0x653fdf3f, /// 0x47c
			 0xc7bc01ed, /// 0x480
			 0x4e742dd8, /// 0x484
			 0x2a721f42, /// 0x488
			 0xf02bf482, /// 0x48c
			 0x8c8532a6, /// 0x490
			 0xf7c0591d, /// 0x494
			 0x6a920218, /// 0x498
			 0xeeb93daf, /// 0x49c
			 0x826b40cc, /// 0x4a0
			 0x293a3225, /// 0x4a4
			 0xaa55fbd6, /// 0x4a8
			 0xf645e316, /// 0x4ac
			 0x02ac5cc7, /// 0x4b0
			 0x2b87d735, /// 0x4b4
			 0x3859b31c, /// 0x4b8
			 0xa01f5b6d, /// 0x4bc
			 0xb092ebd4, /// 0x4c0
			 0x25916ff0, /// 0x4c4
			 0xbd4aa6f8, /// 0x4c8
			 0xe90931fb, /// 0x4cc
			 0x210102ad, /// 0x4d0
			 0x9c62daf5, /// 0x4d4
			 0x3859ada1, /// 0x4d8
			 0x46a35621, /// 0x4dc
			 0xe7bb4ab0, /// 0x4e0
			 0xe5d98c89, /// 0x4e4
			 0x2db95466, /// 0x4e8
			 0x709ce88a, /// 0x4ec
			 0x21b1562d, /// 0x4f0
			 0x1c1c0fe3, /// 0x4f4
			 0x8675f87f, /// 0x4f8
			 0xba904c25, /// 0x4fc
			 0x61ae79b6, /// 0x500
			 0x760801ce, /// 0x504
			 0xea5a004e, /// 0x508
			 0xe1d03527, /// 0x50c
			 0xeb7c2db2, /// 0x510
			 0xfd113396, /// 0x514
			 0x2df31fb6, /// 0x518
			 0x3741a2df, /// 0x51c
			 0x407275e3, /// 0x520
			 0xa1689c5f, /// 0x524
			 0x8066b868, /// 0x528
			 0x36eb60b1, /// 0x52c
			 0x8fd4a8c0, /// 0x530
			 0x4e995e1f, /// 0x534
			 0x20940779, /// 0x538
			 0x208766fb, /// 0x53c
			 0x4b346565, /// 0x540
			 0xc35bde06, /// 0x544
			 0xdb78ea84, /// 0x548
			 0x28d054ad, /// 0x54c
			 0x3f0e84c7, /// 0x550
			 0x91ea29d8, /// 0x554
			 0x0ea19f1e, /// 0x558
			 0xe65dc534, /// 0x55c
			 0x7fd24d1c, /// 0x560
			 0xaa520351, /// 0x564
			 0xf6a85849, /// 0x568
			 0xb61a66c5, /// 0x56c
			 0x65f8972d, /// 0x570
			 0x18643c6b, /// 0x574
			 0x38d673ea, /// 0x578
			 0xdfa4be26, /// 0x57c
			 0x61f2fba5, /// 0x580
			 0x7bc405f1, /// 0x584
			 0xb5e40cde, /// 0x588
			 0x322969e5, /// 0x58c
			 0xe947488e, /// 0x590
			 0xddecbac2, /// 0x594
			 0x27741ea4, /// 0x598
			 0x397023b1, /// 0x59c
			 0xfb5fb776, /// 0x5a0
			 0x104615ac, /// 0x5a4
			 0x3a83e112, /// 0x5a8
			 0x66919c89, /// 0x5ac
			 0x429a787c, /// 0x5b0
			 0x386dfddb, /// 0x5b4
			 0xfde89902, /// 0x5b8
			 0x2e4ada3c, /// 0x5bc
			 0xdbf5e905, /// 0x5c0
			 0xf3d49103, /// 0x5c4
			 0x777fa228, /// 0x5c8
			 0xbcb6263f, /// 0x5cc
			 0xbcc68491, /// 0x5d0
			 0x4807a30a, /// 0x5d4
			 0x0a30a9f6, /// 0x5d8
			 0x99d071ce, /// 0x5dc
			 0x390def0e, /// 0x5e0
			 0xfccbae7d, /// 0x5e4
			 0xe4caa046, /// 0x5e8
			 0x99bd1276, /// 0x5ec
			 0xfb4dbd6e, /// 0x5f0
			 0xc3ac00a1, /// 0x5f4
			 0x75139443, /// 0x5f8
			 0x3e89ea37, /// 0x5fc
			 0x453ebc12, /// 0x600
			 0x936f37bb, /// 0x604
			 0x27fb4de3, /// 0x608
			 0xaa414796, /// 0x60c
			 0x45d778a3, /// 0x610
			 0x1d45e820, /// 0x614
			 0xc7200ae7, /// 0x618
			 0xdcef1161, /// 0x61c
			 0x0980e4ac, /// 0x620
			 0xf3fa851a, /// 0x624
			 0xe3348087, /// 0x628
			 0xfb68ae0f, /// 0x62c
			 0xcc68e489, /// 0x630
			 0x77e65a60, /// 0x634
			 0x2c04ac2d, /// 0x638
			 0xc61a092a, /// 0x63c
			 0x92e330d9, /// 0x640
			 0xe64423a7, /// 0x644
			 0x50700f5d, /// 0x648
			 0x32c304b5, /// 0x64c
			 0x2845e0d5, /// 0x650
			 0x3fca09d1, /// 0x654
			 0xe14b2f7f, /// 0x658
			 0x5aa6fc06, /// 0x65c
			 0xbf09ac8a, /// 0x660
			 0x242ecd69, /// 0x664
			 0x5b206a5e, /// 0x668
			 0xfbcb717c, /// 0x66c
			 0xfc36f838, /// 0x670
			 0x1b1f6162, /// 0x674
			 0xe1130cf2, /// 0x678
			 0xaf74c11d, /// 0x67c
			 0x99337d9a, /// 0x680
			 0x4bf13ff6, /// 0x684
			 0x46f3c2fe, /// 0x688
			 0xbe6955ab, /// 0x68c
			 0x9ecdf1e8, /// 0x690
			 0x327e2690, /// 0x694
			 0x65ff4e42, /// 0x698
			 0x8846ac04, /// 0x69c
			 0xeff69312, /// 0x6a0
			 0xbbb2642b, /// 0x6a4
			 0xc196067d, /// 0x6a8
			 0xe970aa14, /// 0x6ac
			 0xd70bd4fe, /// 0x6b0
			 0xb9701e5e, /// 0x6b4
			 0xe0202ae0, /// 0x6b8
			 0x7ae3fc97, /// 0x6bc
			 0xddfeb54d, /// 0x6c0
			 0xc2fa04d5, /// 0x6c4
			 0xd4bb56fc, /// 0x6c8
			 0xd6d5080e, /// 0x6cc
			 0xa12c0280, /// 0x6d0
			 0x200d2862, /// 0x6d4
			 0x376ba74a, /// 0x6d8
			 0x3c70a979, /// 0x6dc
			 0xda4b5a36, /// 0x6e0
			 0x210f9d7d, /// 0x6e4
			 0x7edc0c0d, /// 0x6e8
			 0xde960843, /// 0x6ec
			 0xa082b5e7, /// 0x6f0
			 0x4ef0e649, /// 0x6f4
			 0x6bd14ca3, /// 0x6f8
			 0x2ac6271a, /// 0x6fc
			 0xdd2f34bf, /// 0x700
			 0x9bb10a74, /// 0x704
			 0x938bc8bb, /// 0x708
			 0xb75db3e9, /// 0x70c
			 0x3c9e630b, /// 0x710
			 0x31a14046, /// 0x714
			 0x5824164e, /// 0x718
			 0xf8c5cfa4, /// 0x71c
			 0xb8eba88e, /// 0x720
			 0x7502e651, /// 0x724
			 0xe09354bb, /// 0x728
			 0x9cde7ea8, /// 0x72c
			 0x916acd2f, /// 0x730
			 0x0b598c9c, /// 0x734
			 0x37925007, /// 0x738
			 0x8c7bc12c, /// 0x73c
			 0xbd51cfbb, /// 0x740
			 0xd6c2a2ec, /// 0x744
			 0x8eaa4680, /// 0x748
			 0x34badee3, /// 0x74c
			 0xb4cbec15, /// 0x750
			 0xe52c828f, /// 0x754
			 0x1d84a156, /// 0x758
			 0xc1da4304, /// 0x75c
			 0x2302fe87, /// 0x760
			 0xc0aa2252, /// 0x764
			 0xa4788b9b, /// 0x768
			 0x20c14ecf, /// 0x76c
			 0xe811a852, /// 0x770
			 0x36256bea, /// 0x774
			 0xa3a7a552, /// 0x778
			 0x178a69bd, /// 0x77c
			 0x21f6ae7b, /// 0x780
			 0x56010ff8, /// 0x784
			 0x7c5550fc, /// 0x788
			 0xb026f6e4, /// 0x78c
			 0x63cef2f7, /// 0x790
			 0x4986d6f1, /// 0x794
			 0xbaa5a1fc, /// 0x798
			 0xc3cc1161, /// 0x79c
			 0x50b7654d, /// 0x7a0
			 0x2aa66c83, /// 0x7a4
			 0x1bd1bb71, /// 0x7a8
			 0x932b9d7b, /// 0x7ac
			 0xdd48346c, /// 0x7b0
			 0x9422b996, /// 0x7b4
			 0xf108fd54, /// 0x7b8
			 0x89aa7b62, /// 0x7bc
			 0xfd0a3138, /// 0x7c0
			 0x2f54810d, /// 0x7c4
			 0x21f4224e, /// 0x7c8
			 0x73d6cfc8, /// 0x7cc
			 0xa559994f, /// 0x7d0
			 0x3c23ec6d, /// 0x7d4
			 0x744de7ce, /// 0x7d8
			 0x108e3b60, /// 0x7dc
			 0x25b0e78b, /// 0x7e0
			 0xefff3cdc, /// 0x7e4
			 0x15f54227, /// 0x7e8
			 0x60f53bfa, /// 0x7ec
			 0x1748f54b, /// 0x7f0
			 0x1e2bc744, /// 0x7f4
			 0xaaf91421, /// 0x7f8
			 0x3fca0003, /// 0x7fc
			 0x9c541486, /// 0x800
			 0x95951915, /// 0x804
			 0x247375e9, /// 0x808
			 0x687ff284, /// 0x80c
			 0x4435c8f9, /// 0x810
			 0x8c698d2c, /// 0x814
			 0xc90a83fb, /// 0x818
			 0x28ce7097, /// 0x81c
			 0xa36a1d6f, /// 0x820
			 0x70d725b3, /// 0x824
			 0x21487c7f, /// 0x828
			 0x477f546b, /// 0x82c
			 0x1bc7156d, /// 0x830
			 0x34d805f5, /// 0x834
			 0xd71658b5, /// 0x838
			 0x1f18b300, /// 0x83c
			 0x2d5e6c21, /// 0x840
			 0x6938b6e0, /// 0x844
			 0xf64b47ec, /// 0x848
			 0x1393258f, /// 0x84c
			 0x3f954cec, /// 0x850
			 0x41314e1d, /// 0x854
			 0x2c6e441c, /// 0x858
			 0x53812b6c, /// 0x85c
			 0x55240de8, /// 0x860
			 0xa3bf5af4, /// 0x864
			 0x1a0a6a76, /// 0x868
			 0xd880f9e9, /// 0x86c
			 0xd8c2e778, /// 0x870
			 0xe2be013a, /// 0x874
			 0xe1b925ab, /// 0x878
			 0xdfb0ec8a, /// 0x87c
			 0x83b82682, /// 0x880
			 0xd9b74d3b, /// 0x884
			 0xbba98cab, /// 0x888
			 0x25e991f0, /// 0x88c
			 0x8b774c82, /// 0x890
			 0x922df58a, /// 0x894
			 0x6f052a7f, /// 0x898
			 0x3c65cea1, /// 0x89c
			 0x51726c2d, /// 0x8a0
			 0xb9cbc709, /// 0x8a4
			 0xc0ffa7c7, /// 0x8a8
			 0x769f2343, /// 0x8ac
			 0xd7acdba3, /// 0x8b0
			 0x22a94d5f, /// 0x8b4
			 0x04257469, /// 0x8b8
			 0x2add6bd8, /// 0x8bc
			 0x25554b60, /// 0x8c0
			 0x05978997, /// 0x8c4
			 0x5ccd3de7, /// 0x8c8
			 0x43c34aaf, /// 0x8cc
			 0x43f65535, /// 0x8d0
			 0xa6c5e0b2, /// 0x8d4
			 0x5ca00f1b, /// 0x8d8
			 0x389da25b, /// 0x8dc
			 0xdee24930, /// 0x8e0
			 0x454d6dbe, /// 0x8e4
			 0x7a20c7a0, /// 0x8e8
			 0xc540b439, /// 0x8ec
			 0x71ad111d, /// 0x8f0
			 0x341aaf4b, /// 0x8f4
			 0xeb9e0d68, /// 0x8f8
			 0xe5dab7d4, /// 0x8fc
			 0xf3998a95, /// 0x900
			 0xabd3e2d7, /// 0x904
			 0xd419d9d2, /// 0x908
			 0x85777e1e, /// 0x90c
			 0x5ccf1c7f, /// 0x910
			 0x861119b9, /// 0x914
			 0xdee980f9, /// 0x918
			 0xfa046ed4, /// 0x91c
			 0xe4ae186b, /// 0x920
			 0xfe87cfbf, /// 0x924
			 0x09b730fd, /// 0x928
			 0xe28a77a9, /// 0x92c
			 0xe8178dc3, /// 0x930
			 0x8811f407, /// 0x934
			 0xe27e4761, /// 0x938
			 0x4a8bbc29, /// 0x93c
			 0x9319377d, /// 0x940
			 0x71f53e2d, /// 0x944
			 0xb7decde0, /// 0x948
			 0x505bbb41, /// 0x94c
			 0x28859b21, /// 0x950
			 0x49551d11, /// 0x954
			 0x223183f5, /// 0x958
			 0x2a74a8b0, /// 0x95c
			 0x0a06b3a2, /// 0x960
			 0xe44cb1bc, /// 0x964
			 0x07d87970, /// 0x968
			 0xf42aaea3, /// 0x96c
			 0x0f0ee55c, /// 0x970
			 0x49c474d0, /// 0x974
			 0xf44a6f43, /// 0x978
			 0x60d23ac2, /// 0x97c
			 0x7b74c01f, /// 0x980
			 0xf5dc22a0, /// 0x984
			 0xc14a5dd7, /// 0x988
			 0xf78fd03b, /// 0x98c
			 0x8e0471bb, /// 0x990
			 0x4a344108, /// 0x994
			 0x4a90c0ba, /// 0x998
			 0x5a15057c, /// 0x99c
			 0xa14928de, /// 0x9a0
			 0xe112d357, /// 0x9a4
			 0x4fbf481b, /// 0x9a8
			 0x9db25ba8, /// 0x9ac
			 0xba1403f4, /// 0x9b0
			 0x0f6dd2b8, /// 0x9b4
			 0x6f502ea8, /// 0x9b8
			 0x5f201bb2, /// 0x9bc
			 0x3a909069, /// 0x9c0
			 0xbe4a1187, /// 0x9c4
			 0x55646f68, /// 0x9c8
			 0xbbe316ff, /// 0x9cc
			 0xc01b07a1, /// 0x9d0
			 0x4a2bc2f5, /// 0x9d4
			 0x9f0b8a29, /// 0x9d8
			 0xaf042ab2, /// 0x9dc
			 0xe6737e49, /// 0x9e0
			 0x034fd618, /// 0x9e4
			 0x91d20f8e, /// 0x9e8
			 0xe55b5fec, /// 0x9ec
			 0x05b3212e, /// 0x9f0
			 0xe58263f9, /// 0x9f4
			 0x2338ce39, /// 0x9f8
			 0x8056c251, /// 0x9fc
			 0x314a7a01, /// 0xa00
			 0x42916dc3, /// 0xa04
			 0x7a3fff87, /// 0xa08
			 0xf5eb3ee5, /// 0xa0c
			 0x5e1c731e, /// 0xa10
			 0xe95f181a, /// 0xa14
			 0x6303d694, /// 0xa18
			 0x3bf69443, /// 0xa1c
			 0xdf4eec46, /// 0xa20
			 0xec09c0e5, /// 0xa24
			 0xe100c5a3, /// 0xa28
			 0xdb0cceb5, /// 0xa2c
			 0x4d524f6b, /// 0xa30
			 0x546773ca, /// 0xa34
			 0xf29de488, /// 0xa38
			 0xec47ab5d, /// 0xa3c
			 0xe5cf656c, /// 0xa40
			 0x5c808023, /// 0xa44
			 0x4bff011d, /// 0xa48
			 0x989a36cf, /// 0xa4c
			 0x7a5d5662, /// 0xa50
			 0x48208384, /// 0xa54
			 0xdde6c705, /// 0xa58
			 0x456a8115, /// 0xa5c
			 0xb4670835, /// 0xa60
			 0x3be9ab41, /// 0xa64
			 0xc636ed85, /// 0xa68
			 0x3c3a45e1, /// 0xa6c
			 0xf568b96f, /// 0xa70
			 0x3ee1e6b8, /// 0xa74
			 0xe9667f3d, /// 0xa78
			 0x15e4b799, /// 0xa7c
			 0xb973c19a, /// 0xa80
			 0x3031bcc4, /// 0xa84
			 0x3bae8534, /// 0xa88
			 0x0538576b, /// 0xa8c
			 0xc901c186, /// 0xa90
			 0x593eb31b, /// 0xa94
			 0x1ed2d4b7, /// 0xa98
			 0xac3277e8, /// 0xa9c
			 0x1340c920, /// 0xaa0
			 0xb21d3ed3, /// 0xaa4
			 0x239d1611, /// 0xaa8
			 0xbabe8fac, /// 0xaac
			 0x8242c2b1, /// 0xab0
			 0x85f4f32c, /// 0xab4
			 0x2835476f, /// 0xab8
			 0x45784a9f, /// 0xabc
			 0x454081b0, /// 0xac0
			 0xf19d8c45, /// 0xac4
			 0xdd6476b4, /// 0xac8
			 0xb46e25ca, /// 0xacc
			 0xbdfda612, /// 0xad0
			 0x613b2d57, /// 0xad4
			 0x5660e506, /// 0xad8
			 0xa12abfae, /// 0xadc
			 0xa034cd89, /// 0xae0
			 0xe944f617, /// 0xae4
			 0x0190290f, /// 0xae8
			 0x39a07765, /// 0xaec
			 0xf162a508, /// 0xaf0
			 0xb4a98223, /// 0xaf4
			 0x992cf836, /// 0xaf8
			 0x7150c57c, /// 0xafc
			 0xe4eaa3b1, /// 0xb00
			 0x824ef183, /// 0xb04
			 0xfdcd04f8, /// 0xb08
			 0x85aace4f, /// 0xb0c
			 0x5cca8162, /// 0xb10
			 0x7faa1780, /// 0xb14
			 0x05212fdb, /// 0xb18
			 0x1acac64b, /// 0xb1c
			 0xfabf9007, /// 0xb20
			 0x381761d9, /// 0xb24
			 0xe314d7e9, /// 0xb28
			 0x7e25a720, /// 0xb2c
			 0x2e0ebc92, /// 0xb30
			 0x0ae0b598, /// 0xb34
			 0xa98502dc, /// 0xb38
			 0xb43893fd, /// 0xb3c
			 0xd647d748, /// 0xb40
			 0x55ad1b60, /// 0xb44
			 0x8ee38ae9, /// 0xb48
			 0xe4fd2924, /// 0xb4c
			 0x28f1a020, /// 0xb50
			 0x31b4004c, /// 0xb54
			 0xe339b247, /// 0xb58
			 0x53bdff6a, /// 0xb5c
			 0x601acbb0, /// 0xb60
			 0xf40816bd, /// 0xb64
			 0x30a91646, /// 0xb68
			 0xcc29d983, /// 0xb6c
			 0xb85c43ec, /// 0xb70
			 0xca6e2f01, /// 0xb74
			 0x01d6cc5d, /// 0xb78
			 0x8db1d1fa, /// 0xb7c
			 0xe325dbdd, /// 0xb80
			 0x25e5ff29, /// 0xb84
			 0x2f5854f0, /// 0xb88
			 0x158810eb, /// 0xb8c
			 0xcb9ca2a2, /// 0xb90
			 0xb69e48cf, /// 0xb94
			 0xaf43c1f0, /// 0xb98
			 0x84cae7e6, /// 0xb9c
			 0xb4ed6668, /// 0xba0
			 0xfd990b36, /// 0xba4
			 0x6f03d621, /// 0xba8
			 0xdc53b5ea, /// 0xbac
			 0xaf9839c1, /// 0xbb0
			 0xc36ba768, /// 0xbb4
			 0x1f6fdc69, /// 0xbb8
			 0xe41aa061, /// 0xbbc
			 0xecdacb10, /// 0xbc0
			 0xa5862fd4, /// 0xbc4
			 0x5fad762f, /// 0xbc8
			 0x5533e951, /// 0xbcc
			 0x741827ee, /// 0xbd0
			 0xbd422afb, /// 0xbd4
			 0x9a08f932, /// 0xbd8
			 0x7f178de9, /// 0xbdc
			 0xaea38284, /// 0xbe0
			 0x1259871c, /// 0xbe4
			 0x62bb311e, /// 0xbe8
			 0x51bd8605, /// 0xbec
			 0xb6fe099e, /// 0xbf0
			 0x96d613ca, /// 0xbf4
			 0x30e4172e, /// 0xbf8
			 0x7064f1a2, /// 0xbfc
			 0x88ea5c35, /// 0xc00
			 0x64e68987, /// 0xc04
			 0xacd03d97, /// 0xc08
			 0x8a3bff0d, /// 0xc0c
			 0x374c6f14, /// 0xc10
			 0x08a7430b, /// 0xc14
			 0xdac981ec, /// 0xc18
			 0xd4a7ad69, /// 0xc1c
			 0x303cd3c5, /// 0xc20
			 0x917db36f, /// 0xc24
			 0x497c63f4, /// 0xc28
			 0x80f065fb, /// 0xc2c
			 0x2277ad60, /// 0xc30
			 0x9245968a, /// 0xc34
			 0x4a5712d4, /// 0xc38
			 0x13aaada2, /// 0xc3c
			 0xd6b67606, /// 0xc40
			 0xa6a92543, /// 0xc44
			 0xc6202d71, /// 0xc48
			 0xbb55d212, /// 0xc4c
			 0xec884eae, /// 0xc50
			 0x62613798, /// 0xc54
			 0x4a971c04, /// 0xc58
			 0xce0f560d, /// 0xc5c
			 0xbf3d2dd7, /// 0xc60
			 0xb1cde420, /// 0xc64
			 0xae47bfd6, /// 0xc68
			 0x58473eed, /// 0xc6c
			 0x29400dc5, /// 0xc70
			 0x34655e40, /// 0xc74
			 0x30d866a1, /// 0xc78
			 0x54f4115a, /// 0xc7c
			 0x821b48b3, /// 0xc80
			 0x230104ab, /// 0xc84
			 0x8148ba52, /// 0xc88
			 0x1f51b2d4, /// 0xc8c
			 0x080db9ad, /// 0xc90
			 0x61fd07ac, /// 0xc94
			 0xd5d1eb04, /// 0xc98
			 0x43e7b87d, /// 0xc9c
			 0xc4e227e8, /// 0xca0
			 0x4da3f2ab, /// 0xca4
			 0xec3eca42, /// 0xca8
			 0x3c2c8aee, /// 0xcac
			 0x68852c94, /// 0xcb0
			 0xad24ad6c, /// 0xcb4
			 0x127654a4, /// 0xcb8
			 0xa2c9afc4, /// 0xcbc
			 0x213bdb76, /// 0xcc0
			 0x40e85e8e, /// 0xcc4
			 0x3522ff0a, /// 0xcc8
			 0x24be8026, /// 0xccc
			 0x74bd50e4, /// 0xcd0
			 0x1e90cb73, /// 0xcd4
			 0xf1dfbe1a, /// 0xcd8
			 0x6cee5a02, /// 0xcdc
			 0x5a71a3fb, /// 0xce0
			 0xe559b377, /// 0xce4
			 0xf0e134cc, /// 0xce8
			 0xcd7b418f, /// 0xcec
			 0xed073d60, /// 0xcf0
			 0x83391e61, /// 0xcf4
			 0x204d2ab2, /// 0xcf8
			 0xc74710a0, /// 0xcfc
			 0x5badb254, /// 0xd00
			 0xb9dea9d4, /// 0xd04
			 0xf32375fa, /// 0xd08
			 0x7f0a117c, /// 0xd0c
			 0xab8e2f6a, /// 0xd10
			 0x6a1e3d94, /// 0xd14
			 0x04b139f4, /// 0xd18
			 0x60eed275, /// 0xd1c
			 0x1234a656, /// 0xd20
			 0x71d86694, /// 0xd24
			 0x0890e4ab, /// 0xd28
			 0x398bd930, /// 0xd2c
			 0x81d9db27, /// 0xd30
			 0x8f7284d3, /// 0xd34
			 0x83b99b01, /// 0xd38
			 0xc1a45012, /// 0xd3c
			 0x7907f42b, /// 0xd40
			 0x5272fd95, /// 0xd44
			 0x6867af0b, /// 0xd48
			 0x226bc512, /// 0xd4c
			 0x507b61f7, /// 0xd50
			 0x384aade5, /// 0xd54
			 0xc40c247c, /// 0xd58
			 0x8c116333, /// 0xd5c
			 0x7be0e238, /// 0xd60
			 0x69329943, /// 0xd64
			 0x2eba7194, /// 0xd68
			 0x98f2430e, /// 0xd6c
			 0xc090db57, /// 0xd70
			 0x7382ff21, /// 0xd74
			 0x004c4cfa, /// 0xd78
			 0x5293449d, /// 0xd7c
			 0xe9e69633, /// 0xd80
			 0xcd7fac97, /// 0xd84
			 0x7b703913, /// 0xd88
			 0x9a41decf, /// 0xd8c
			 0xa9c82a8e, /// 0xd90
			 0x1d189ab9, /// 0xd94
			 0x238b5f47, /// 0xd98
			 0xe2e061ed, /// 0xd9c
			 0x26b76dba, /// 0xda0
			 0x4109f63e, /// 0xda4
			 0x42c10fa1, /// 0xda8
			 0x6f5858b7, /// 0xdac
			 0xd54e9058, /// 0xdb0
			 0x8ec1f9c2, /// 0xdb4
			 0xe45f11aa, /// 0xdb8
			 0x69cb4a98, /// 0xdbc
			 0xac8f31c1, /// 0xdc0
			 0x3d542d00, /// 0xdc4
			 0x79d8c027, /// 0xdc8
			 0x61d670d8, /// 0xdcc
			 0x44419417, /// 0xdd0
			 0x4a6199e5, /// 0xdd4
			 0x6bf7592a, /// 0xdd8
			 0x30d71738, /// 0xddc
			 0x832c132a, /// 0xde0
			 0x659f79ec, /// 0xde4
			 0xb3516ef2, /// 0xde8
			 0x7b625f9c, /// 0xdec
			 0xb1731247, /// 0xdf0
			 0xc4d8d2d6, /// 0xdf4
			 0xa5c8dc7b, /// 0xdf8
			 0x298a5d3a, /// 0xdfc
			 0xe33ffc94, /// 0xe00
			 0x269dcf66, /// 0xe04
			 0x8821bdfc, /// 0xe08
			 0x91c9d52d, /// 0xe0c
			 0x09519f26, /// 0xe10
			 0xcc2b7976, /// 0xe14
			 0x16eb7fd7, /// 0xe18
			 0x241cf6e8, /// 0xe1c
			 0xd2c2b65e, /// 0xe20
			 0xa006dae7, /// 0xe24
			 0xd6bd313f, /// 0xe28
			 0x9cc71977, /// 0xe2c
			 0xb8fb7977, /// 0xe30
			 0xbbc39185, /// 0xe34
			 0xe12273cd, /// 0xe38
			 0x44075bba, /// 0xe3c
			 0x9699a88c, /// 0xe40
			 0xe886deb0, /// 0xe44
			 0x929c512a, /// 0xe48
			 0x088a60f2, /// 0xe4c
			 0x726cd174, /// 0xe50
			 0x04567e95, /// 0xe54
			 0xf44d2d0a, /// 0xe58
			 0xda894a41, /// 0xe5c
			 0xb447c910, /// 0xe60
			 0xb13c617b, /// 0xe64
			 0x8031d0f2, /// 0xe68
			 0x9940f908, /// 0xe6c
			 0x63256630, /// 0xe70
			 0x36a980bb, /// 0xe74
			 0xd02cb307, /// 0xe78
			 0xa1d18fd2, /// 0xe7c
			 0x8ec741d8, /// 0xe80
			 0x11cf3276, /// 0xe84
			 0xc2976f8a, /// 0xe88
			 0xdff2c4f0, /// 0xe8c
			 0x4b6f5405, /// 0xe90
			 0xd5458dc9, /// 0xe94
			 0xc241a572, /// 0xe98
			 0x0e212d59, /// 0xe9c
			 0x4e2bc1a9, /// 0xea0
			 0xb56a8f73, /// 0xea4
			 0xe181c962, /// 0xea8
			 0x1580af94, /// 0xeac
			 0x45da8619, /// 0xeb0
			 0x1a61ba4f, /// 0xeb4
			 0xabe9c6d6, /// 0xeb8
			 0x1e3ee7ab, /// 0xebc
			 0x5e391390, /// 0xec0
			 0x9a46e586, /// 0xec4
			 0x39e1b397, /// 0xec8
			 0x9c6fad26, /// 0xecc
			 0x7b9cb2b3, /// 0xed0
			 0xea299d8e, /// 0xed4
			 0xd3514305, /// 0xed8
			 0xcfb38495, /// 0xedc
			 0xe5ee04bc, /// 0xee0
			 0xb044223c, /// 0xee4
			 0x074e86f4, /// 0xee8
			 0x3021fa03, /// 0xeec
			 0x0f75d8e6, /// 0xef0
			 0x42f4ad1c, /// 0xef4
			 0x36471b1c, /// 0xef8
			 0x799315c4, /// 0xefc
			 0xdd267987, /// 0xf00
			 0x8cb090ff, /// 0xf04
			 0x82efdda3, /// 0xf08
			 0xe75b4483, /// 0xf0c
			 0xea39fd9f, /// 0xf10
			 0xd7e1a25b, /// 0xf14
			 0x4006ffdf, /// 0xf18
			 0x5a7b1b02, /// 0xf1c
			 0xf1e12e8f, /// 0xf20
			 0x43c630af, /// 0xf24
			 0x4fb25fa7, /// 0xf28
			 0x4c246089, /// 0xf2c
			 0xf29c575a, /// 0xf30
			 0xb14c3edc, /// 0xf34
			 0xa72384b2, /// 0xf38
			 0xd4fa1076, /// 0xf3c
			 0xe5a8cbc5, /// 0xf40
			 0x284d063b, /// 0xf44
			 0x74f87244, /// 0xf48
			 0x16e55d9d, /// 0xf4c
			 0x0723d735, /// 0xf50
			 0xb93c7818, /// 0xf54
			 0x6c95bc0b, /// 0xf58
			 0xbc81c794, /// 0xf5c
			 0xaecab36c, /// 0xf60
			 0x1acec897, /// 0xf64
			 0x351b20d4, /// 0xf68
			 0x490e6381, /// 0xf6c
			 0xf1272b25, /// 0xf70
			 0xafa4b534, /// 0xf74
			 0xea4d67aa, /// 0xf78
			 0x888e4920, /// 0xf7c
			 0xe499e2a6, /// 0xf80
			 0x440e5eba, /// 0xf84
			 0x644d1153, /// 0xf88
			 0x6014b124, /// 0xf8c
			 0x9dd5efbc, /// 0xf90
			 0xb6d3bf66, /// 0xf94
			 0xfea7b0e2, /// 0xf98
			 0x50574be1, /// 0xf9c
			 0x6be05833, /// 0xfa0
			 0xe300abc3, /// 0xfa4
			 0x56e5f1a4, /// 0xfa8
			 0x01333a5a, /// 0xfac
			 0xcc25fbcd, /// 0xfb0
			 0x33077c37, /// 0xfb4
			 0x65e3b64a, /// 0xfb8
			 0xa0337572, /// 0xfbc
			 0x03752243, /// 0xfc0
			 0xf96512c6, /// 0xfc4
			 0x8cd50351, /// 0xfc8
			 0x40abe229, /// 0xfcc
			 0x01fa523a, /// 0xfd0
			 0xca0d8e45, /// 0xfd4
			 0xccbeed15, /// 0xfd8
			 0x9c175b81, /// 0xfdc
			 0x6712a334, /// 0xfe0
			 0x7c4ac63d, /// 0xfe4
			 0xf67a163c, /// 0xfe8
			 0x83114cb9, /// 0xfec
			 0x09475ac4, /// 0xff0
			 0xb337e818, /// 0xff4
			 0xfaed42f7, /// 0xff8
			 0x711e17f9 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xd6eb73c4, /// 0x0
			 0x53a9a808, /// 0x4
			 0x2dfeb148, /// 0x8
			 0xa9db8d2e, /// 0xc
			 0x1c1de977, /// 0x10
			 0xc914790b, /// 0x14
			 0x12fea6d6, /// 0x18
			 0x26b8625f, /// 0x1c
			 0x798520f1, /// 0x20
			 0xaf3becc7, /// 0x24
			 0x714b09bb, /// 0x28
			 0xf1a76e96, /// 0x2c
			 0xc27a578c, /// 0x30
			 0xadcbcbc8, /// 0x34
			 0xaefe9d90, /// 0x38
			 0xa353f345, /// 0x3c
			 0xac7ecbf6, /// 0x40
			 0x5a647f42, /// 0x44
			 0xc8219dbe, /// 0x48
			 0x7a1950a7, /// 0x4c
			 0x90e3483d, /// 0x50
			 0xed2ab779, /// 0x54
			 0x34666fc3, /// 0x58
			 0xc4e43334, /// 0x5c
			 0xd050092b, /// 0x60
			 0x9ee8e3f9, /// 0x64
			 0x2e19b61b, /// 0x68
			 0x64d2a9bc, /// 0x6c
			 0xe3862066, /// 0x70
			 0x6c28a611, /// 0x74
			 0xd5ec206c, /// 0x78
			 0xde90d02a, /// 0x7c
			 0x10379e45, /// 0x80
			 0xb89a20b4, /// 0x84
			 0x304bf29f, /// 0x88
			 0x8825af7d, /// 0x8c
			 0x5fb00781, /// 0x90
			 0x665790d1, /// 0x94
			 0x4a4f97ac, /// 0x98
			 0xf608a5d1, /// 0x9c
			 0x36d9fb75, /// 0xa0
			 0x54e0fd51, /// 0xa4
			 0xf44c53df, /// 0xa8
			 0x03a000d8, /// 0xac
			 0x40b53e5d, /// 0xb0
			 0x46d519c5, /// 0xb4
			 0x1fa13f4d, /// 0xb8
			 0x3ca62f5f, /// 0xbc
			 0x9f2fc0e5, /// 0xc0
			 0x528fab78, /// 0xc4
			 0xecba5359, /// 0xc8
			 0xa2d402d8, /// 0xcc
			 0xa6a87811, /// 0xd0
			 0x1b7cd64b, /// 0xd4
			 0x7e224a1b, /// 0xd8
			 0x2c561995, /// 0xdc
			 0x18794129, /// 0xe0
			 0x1a188b54, /// 0xe4
			 0xb7f3975f, /// 0xe8
			 0xe5fa7d68, /// 0xec
			 0x78045d53, /// 0xf0
			 0x54047fbe, /// 0xf4
			 0x8c3fad6b, /// 0xf8
			 0xa7144e7e, /// 0xfc
			 0xba966818, /// 0x100
			 0x04bff01d, /// 0x104
			 0x6906d603, /// 0x108
			 0x1041b707, /// 0x10c
			 0xce3d147d, /// 0x110
			 0x34c8b9b0, /// 0x114
			 0x6a58fa84, /// 0x118
			 0xe84eeee5, /// 0x11c
			 0xd519f6e7, /// 0x120
			 0xc5ce0af2, /// 0x124
			 0xd535211b, /// 0x128
			 0xb2d4d1cd, /// 0x12c
			 0x5a70b52b, /// 0x130
			 0x69c71e49, /// 0x134
			 0xe72ddc70, /// 0x138
			 0x1124ed64, /// 0x13c
			 0x032de91a, /// 0x140
			 0xc7bbc744, /// 0x144
			 0x08c52e01, /// 0x148
			 0x4562aa23, /// 0x14c
			 0x864af797, /// 0x150
			 0xdbcece8d, /// 0x154
			 0x0d1ea70f, /// 0x158
			 0x54370c9a, /// 0x15c
			 0x65d09443, /// 0x160
			 0x85937a19, /// 0x164
			 0x80e5c56d, /// 0x168
			 0xedadae4a, /// 0x16c
			 0xa3498884, /// 0x170
			 0x7f5444c0, /// 0x174
			 0x7417bc49, /// 0x178
			 0xdf995108, /// 0x17c
			 0x4f88b910, /// 0x180
			 0x3630bb0c, /// 0x184
			 0x25b3c708, /// 0x188
			 0x8b70b19e, /// 0x18c
			 0xd98d2690, /// 0x190
			 0x995a6b3b, /// 0x194
			 0x179294eb, /// 0x198
			 0xadfc86d1, /// 0x19c
			 0x71273d59, /// 0x1a0
			 0xc999e291, /// 0x1a4
			 0xf53700ed, /// 0x1a8
			 0x9fc6d768, /// 0x1ac
			 0x80ae2b10, /// 0x1b0
			 0x7bb86700, /// 0x1b4
			 0xbc82b1bc, /// 0x1b8
			 0xfcea6f3f, /// 0x1bc
			 0x0337105c, /// 0x1c0
			 0x426c3d3e, /// 0x1c4
			 0x8a1e64e2, /// 0x1c8
			 0x2d96fd19, /// 0x1cc
			 0x66a7d34d, /// 0x1d0
			 0x12a62b78, /// 0x1d4
			 0x16d5f9a9, /// 0x1d8
			 0x9d809c01, /// 0x1dc
			 0xbd6f6066, /// 0x1e0
			 0xe9a28fc0, /// 0x1e4
			 0x9515a152, /// 0x1e8
			 0xf9f53fac, /// 0x1ec
			 0xc3a4dd4c, /// 0x1f0
			 0x7df87eb5, /// 0x1f4
			 0x62c760fc, /// 0x1f8
			 0x4c905d54, /// 0x1fc
			 0x1e3757a0, /// 0x200
			 0xf44b44f2, /// 0x204
			 0xb7d10482, /// 0x208
			 0x7d10f061, /// 0x20c
			 0xc13fe2f6, /// 0x210
			 0x6763d834, /// 0x214
			 0x48a7ea8d, /// 0x218
			 0xfe0ee4f3, /// 0x21c
			 0x6e993cca, /// 0x220
			 0x2f4113a7, /// 0x224
			 0x5b6b5a1a, /// 0x228
			 0x23c52f1c, /// 0x22c
			 0xcdd27de4, /// 0x230
			 0x625f2af1, /// 0x234
			 0xd9a70173, /// 0x238
			 0x348102f3, /// 0x23c
			 0x5f5cf1b6, /// 0x240
			 0x2037246b, /// 0x244
			 0x13352bbd, /// 0x248
			 0xa3a83e56, /// 0x24c
			 0xc40fa9f2, /// 0x250
			 0xbe7a294b, /// 0x254
			 0xeb1b03d0, /// 0x258
			 0x05864a4a, /// 0x25c
			 0xc40bc975, /// 0x260
			 0x27020a7f, /// 0x264
			 0x07ef0122, /// 0x268
			 0x2d689c87, /// 0x26c
			 0x500ee00f, /// 0x270
			 0x4efe1395, /// 0x274
			 0x88cc9858, /// 0x278
			 0x7c96b317, /// 0x27c
			 0x04ae0ef9, /// 0x280
			 0xf1683d13, /// 0x284
			 0x287a012c, /// 0x288
			 0x8d75c9c8, /// 0x28c
			 0x5ebff392, /// 0x290
			 0x120b9416, /// 0x294
			 0x847e9ff4, /// 0x298
			 0x107e2d0d, /// 0x29c
			 0xe8ef395f, /// 0x2a0
			 0xa81d47b1, /// 0x2a4
			 0x2ccc11f4, /// 0x2a8
			 0x25615446, /// 0x2ac
			 0x4e81de01, /// 0x2b0
			 0xbf7e7830, /// 0x2b4
			 0x535a6a51, /// 0x2b8
			 0xa9383f1b, /// 0x2bc
			 0xc715944a, /// 0x2c0
			 0x8e3b66ba, /// 0x2c4
			 0xfaf07cb5, /// 0x2c8
			 0x2f637ce8, /// 0x2cc
			 0xd8128859, /// 0x2d0
			 0x46f2b028, /// 0x2d4
			 0xc34bb59d, /// 0x2d8
			 0xb6ea612b, /// 0x2dc
			 0x032ad39a, /// 0x2e0
			 0x34dfaeff, /// 0x2e4
			 0xc4315c04, /// 0x2e8
			 0xb2dd4060, /// 0x2ec
			 0x3943d673, /// 0x2f0
			 0x8a8e55d9, /// 0x2f4
			 0x9f37c70c, /// 0x2f8
			 0xf5425eae, /// 0x2fc
			 0x4366c1ba, /// 0x300
			 0x4e570b2a, /// 0x304
			 0xc9a21f37, /// 0x308
			 0x46679024, /// 0x30c
			 0x454f228c, /// 0x310
			 0xd5e31b6f, /// 0x314
			 0x7a8df33f, /// 0x318
			 0xaf8873ad, /// 0x31c
			 0x6c096c52, /// 0x320
			 0xe1d111cc, /// 0x324
			 0xd9f683ce, /// 0x328
			 0x114f2554, /// 0x32c
			 0x8361872a, /// 0x330
			 0x153eac3c, /// 0x334
			 0x66e6d138, /// 0x338
			 0x22146f20, /// 0x33c
			 0xb6f39fef, /// 0x340
			 0x84ad6fd1, /// 0x344
			 0xaa871984, /// 0x348
			 0x253cead9, /// 0x34c
			 0x877b3d35, /// 0x350
			 0xc94fba5e, /// 0x354
			 0x27633ac6, /// 0x358
			 0x48a54190, /// 0x35c
			 0x73ae77ae, /// 0x360
			 0x8aa6e260, /// 0x364
			 0xc8420e40, /// 0x368
			 0x614c9819, /// 0x36c
			 0x5ff1c560, /// 0x370
			 0x77684da9, /// 0x374
			 0xa1713387, /// 0x378
			 0x4447fd63, /// 0x37c
			 0x7079c8ec, /// 0x380
			 0x79edb3f8, /// 0x384
			 0xdc0db061, /// 0x388
			 0xf54b526b, /// 0x38c
			 0x0402e5d1, /// 0x390
			 0xb0bf3946, /// 0x394
			 0x125f691b, /// 0x398
			 0x7b2ab420, /// 0x39c
			 0x32a72954, /// 0x3a0
			 0xac8ac282, /// 0x3a4
			 0xffeaef52, /// 0x3a8
			 0xd19a734e, /// 0x3ac
			 0x5ef7f0e9, /// 0x3b0
			 0x96843088, /// 0x3b4
			 0x5e418b46, /// 0x3b8
			 0x2b00d07e, /// 0x3bc
			 0x991fad06, /// 0x3c0
			 0x458c3fa6, /// 0x3c4
			 0xc130935b, /// 0x3c8
			 0xf19fe244, /// 0x3cc
			 0x4463bd89, /// 0x3d0
			 0xc7663026, /// 0x3d4
			 0xd7468a6f, /// 0x3d8
			 0x909e572d, /// 0x3dc
			 0xb2e5d0f1, /// 0x3e0
			 0xce1ccf2c, /// 0x3e4
			 0x0a9942f3, /// 0x3e8
			 0x897e478d, /// 0x3ec
			 0x18af0f64, /// 0x3f0
			 0xa0492330, /// 0x3f4
			 0x575b1b66, /// 0x3f8
			 0x7c72f898, /// 0x3fc
			 0x2c19012c, /// 0x400
			 0xc96522f5, /// 0x404
			 0x4fd257f6, /// 0x408
			 0xd1446993, /// 0x40c
			 0x73dd6317, /// 0x410
			 0x4eeaed5c, /// 0x414
			 0x25f4d51a, /// 0x418
			 0xd16e970b, /// 0x41c
			 0xd5aa66e2, /// 0x420
			 0xf539920c, /// 0x424
			 0xf4784415, /// 0x428
			 0xf3b0677f, /// 0x42c
			 0xd006f7c6, /// 0x430
			 0x39057897, /// 0x434
			 0xb7806757, /// 0x438
			 0x563f8836, /// 0x43c
			 0xdf8ed9c3, /// 0x440
			 0xaf08b0b2, /// 0x444
			 0x936daf74, /// 0x448
			 0x85f2d5b8, /// 0x44c
			 0x9104364e, /// 0x450
			 0xe0f2b005, /// 0x454
			 0x181e2da0, /// 0x458
			 0x28dfc2d0, /// 0x45c
			 0x8bba6853, /// 0x460
			 0x49dc4f22, /// 0x464
			 0x7c290bc8, /// 0x468
			 0xd09177f7, /// 0x46c
			 0x9a1515c5, /// 0x470
			 0xf1b0e898, /// 0x474
			 0xc2540759, /// 0x478
			 0xe71c2632, /// 0x47c
			 0x84c3a79f, /// 0x480
			 0xd1a8ceef, /// 0x484
			 0x5e9bc7b2, /// 0x488
			 0xdba68e20, /// 0x48c
			 0x18db2220, /// 0x490
			 0xcfb178ef, /// 0x494
			 0xf050d6b6, /// 0x498
			 0x4f9e5e2c, /// 0x49c
			 0x3f5da99c, /// 0x4a0
			 0x9b33cca3, /// 0x4a4
			 0xaa39a617, /// 0x4a8
			 0x31f2adf7, /// 0x4ac
			 0x2ca06aae, /// 0x4b0
			 0x87eb8bb4, /// 0x4b4
			 0x21e80cac, /// 0x4b8
			 0xa5b7353a, /// 0x4bc
			 0xba0add90, /// 0x4c0
			 0x34705d99, /// 0x4c4
			 0x14672970, /// 0x4c8
			 0xabdcdb1c, /// 0x4cc
			 0x83602e1d, /// 0x4d0
			 0x998b0a87, /// 0x4d4
			 0x7c89bcbd, /// 0x4d8
			 0xbfe02bfd, /// 0x4dc
			 0xd6857b27, /// 0x4e0
			 0x6c754da1, /// 0x4e4
			 0x6669505e, /// 0x4e8
			 0xa3bffc2b, /// 0x4ec
			 0x88c20726, /// 0x4f0
			 0x08ead080, /// 0x4f4
			 0x1e16d34b, /// 0x4f8
			 0x73796104, /// 0x4fc
			 0xcef4ebb9, /// 0x500
			 0x1fa30d76, /// 0x504
			 0x18e8b3eb, /// 0x508
			 0xa038705a, /// 0x50c
			 0xc4686fc5, /// 0x510
			 0x40cfc33a, /// 0x514
			 0xb6b8bdff, /// 0x518
			 0x19124f05, /// 0x51c
			 0x8682272a, /// 0x520
			 0x7c9ec00d, /// 0x524
			 0x9b80c1b5, /// 0x528
			 0xd9cb86e5, /// 0x52c
			 0x810ce54d, /// 0x530
			 0x72d6daff, /// 0x534
			 0x03e4af01, /// 0x538
			 0x57934532, /// 0x53c
			 0xbaede881, /// 0x540
			 0x505966a9, /// 0x544
			 0xed4ccde3, /// 0x548
			 0xb72ccf89, /// 0x54c
			 0xa97c438b, /// 0x550
			 0x84f91e9b, /// 0x554
			 0x4967768b, /// 0x558
			 0x3bac500e, /// 0x55c
			 0x6c5dde37, /// 0x560
			 0xf29815f1, /// 0x564
			 0x32ff6034, /// 0x568
			 0xfe1867c2, /// 0x56c
			 0xeba512da, /// 0x570
			 0xb90df958, /// 0x574
			 0xad4c8590, /// 0x578
			 0xd259c839, /// 0x57c
			 0x85633932, /// 0x580
			 0xe08f0a84, /// 0x584
			 0xe00fecb2, /// 0x588
			 0x4961d5b4, /// 0x58c
			 0xbe690de9, /// 0x590
			 0xdd890aed, /// 0x594
			 0x32ba6c7c, /// 0x598
			 0x40c44dcb, /// 0x59c
			 0xaf1a6f73, /// 0x5a0
			 0x25b1212d, /// 0x5a4
			 0x7713fcda, /// 0x5a8
			 0xb001f79e, /// 0x5ac
			 0xa50bf3e9, /// 0x5b0
			 0x1c370b5a, /// 0x5b4
			 0x0c4dc550, /// 0x5b8
			 0xd74f0e6c, /// 0x5bc
			 0x00ab7d80, /// 0x5c0
			 0x7e810497, /// 0x5c4
			 0xbd5689c1, /// 0x5c8
			 0x1ccc90a1, /// 0x5cc
			 0x449aa283, /// 0x5d0
			 0x2f86067e, /// 0x5d4
			 0x59c672c2, /// 0x5d8
			 0x87841672, /// 0x5dc
			 0x0a44fcc9, /// 0x5e0
			 0xaf4ec636, /// 0x5e4
			 0xa039ae53, /// 0x5e8
			 0xa51e528c, /// 0x5ec
			 0x19d7fdba, /// 0x5f0
			 0xf098102c, /// 0x5f4
			 0xf5a45c3c, /// 0x5f8
			 0xde149c4b, /// 0x5fc
			 0x79cc56f6, /// 0x600
			 0xfa4d5a34, /// 0x604
			 0x681b7355, /// 0x608
			 0xa172df8f, /// 0x60c
			 0x7fef530f, /// 0x610
			 0x5a0d168a, /// 0x614
			 0x2bf3648c, /// 0x618
			 0x18b3d4cb, /// 0x61c
			 0x26180e69, /// 0x620
			 0x12a1e223, /// 0x624
			 0xf9a12381, /// 0x628
			 0x8c7fa5b4, /// 0x62c
			 0x062df048, /// 0x630
			 0x751d0847, /// 0x634
			 0xf530d969, /// 0x638
			 0x890eb87b, /// 0x63c
			 0x61eb00d3, /// 0x640
			 0xe20f973e, /// 0x644
			 0x90a6fa63, /// 0x648
			 0xf0e3fcdd, /// 0x64c
			 0x2c205539, /// 0x650
			 0x688bb251, /// 0x654
			 0xd3b11420, /// 0x658
			 0x90f32e8c, /// 0x65c
			 0xbf34f875, /// 0x660
			 0x2fe0f780, /// 0x664
			 0x97d13854, /// 0x668
			 0x229454be, /// 0x66c
			 0xe515ff88, /// 0x670
			 0x83e093a8, /// 0x674
			 0xf9eee54e, /// 0x678
			 0xa9c30e7a, /// 0x67c
			 0xe0971506, /// 0x680
			 0xbc766e2d, /// 0x684
			 0xf1ca3cef, /// 0x688
			 0x5ca746d5, /// 0x68c
			 0x6a5523af, /// 0x690
			 0xa9f78cb9, /// 0x694
			 0x04a33391, /// 0x698
			 0x5ed4e72d, /// 0x69c
			 0xf863aab9, /// 0x6a0
			 0xf0dfbbae, /// 0x6a4
			 0x8e8fe2a0, /// 0x6a8
			 0x2dc9e6fd, /// 0x6ac
			 0xf2faacf2, /// 0x6b0
			 0xf3b285d2, /// 0x6b4
			 0xcac6b870, /// 0x6b8
			 0xd9dba2ec, /// 0x6bc
			 0x50ae8559, /// 0x6c0
			 0x6f05f098, /// 0x6c4
			 0x20fadac6, /// 0x6c8
			 0x9603026e, /// 0x6cc
			 0x9b78a761, /// 0x6d0
			 0x3fe0698e, /// 0x6d4
			 0xb118cc1e, /// 0x6d8
			 0xc91fdd64, /// 0x6dc
			 0x7f584bd7, /// 0x6e0
			 0x06560328, /// 0x6e4
			 0x3a914eb7, /// 0x6e8
			 0x3fb5ff33, /// 0x6ec
			 0xda8c3357, /// 0x6f0
			 0x44fbe730, /// 0x6f4
			 0x42f52d20, /// 0x6f8
			 0x5eac5301, /// 0x6fc
			 0x34124042, /// 0x700
			 0x4cc79fcd, /// 0x704
			 0x61baac4c, /// 0x708
			 0x7fd38d3d, /// 0x70c
			 0x485b56a3, /// 0x710
			 0x13905ea9, /// 0x714
			 0x305b6591, /// 0x718
			 0xf03e2cd3, /// 0x71c
			 0xff55a43d, /// 0x720
			 0x90cfcd8c, /// 0x724
			 0xeb62b01a, /// 0x728
			 0x0edde325, /// 0x72c
			 0x35f8b6c1, /// 0x730
			 0x7e739bf5, /// 0x734
			 0xada86317, /// 0x738
			 0xc911aace, /// 0x73c
			 0xe3e97d4a, /// 0x740
			 0x75ef2a53, /// 0x744
			 0x7e432807, /// 0x748
			 0x77dfda65, /// 0x74c
			 0x4230485d, /// 0x750
			 0xfc6447ea, /// 0x754
			 0xe92516e6, /// 0x758
			 0x17db9218, /// 0x75c
			 0x5064ba32, /// 0x760
			 0x8e10398f, /// 0x764
			 0x8b8d6f28, /// 0x768
			 0x4fa9e3f1, /// 0x76c
			 0x3c07b9f0, /// 0x770
			 0x64a380a4, /// 0x774
			 0x535e2c56, /// 0x778
			 0xd6c05e78, /// 0x77c
			 0x0fd01db2, /// 0x780
			 0x2b8fd258, /// 0x784
			 0x900fae75, /// 0x788
			 0x96bdc171, /// 0x78c
			 0x54a6468e, /// 0x790
			 0x23f2581a, /// 0x794
			 0xcaf7fb85, /// 0x798
			 0x92f5f4f6, /// 0x79c
			 0x2564aba5, /// 0x7a0
			 0x6917443c, /// 0x7a4
			 0xcb9bd87a, /// 0x7a8
			 0x5f52883a, /// 0x7ac
			 0xe83b2caa, /// 0x7b0
			 0x54d6680a, /// 0x7b4
			 0x98e20ae4, /// 0x7b8
			 0x18e9cdd5, /// 0x7bc
			 0xc7058a73, /// 0x7c0
			 0x9127f715, /// 0x7c4
			 0x61cbaf5f, /// 0x7c8
			 0x15490250, /// 0x7cc
			 0x181b26f3, /// 0x7d0
			 0x6e0a09c9, /// 0x7d4
			 0x601c2b0d, /// 0x7d8
			 0x5dedcf9b, /// 0x7dc
			 0x680f7a30, /// 0x7e0
			 0x1dad7156, /// 0x7e4
			 0x3e5c16a7, /// 0x7e8
			 0x80344352, /// 0x7ec
			 0x924bb980, /// 0x7f0
			 0xfd19f5cb, /// 0x7f4
			 0x9f238cfd, /// 0x7f8
			 0x70bbc87d, /// 0x7fc
			 0x702369bb, /// 0x800
			 0x336a6010, /// 0x804
			 0xaaecbcf3, /// 0x808
			 0x68d85983, /// 0x80c
			 0x36caf3a2, /// 0x810
			 0x98c95fdc, /// 0x814
			 0x1617ec83, /// 0x818
			 0x53e4a32e, /// 0x81c
			 0x8badb1ae, /// 0x820
			 0x6ae67d21, /// 0x824
			 0x24a583da, /// 0x828
			 0xa22b1839, /// 0x82c
			 0x6d295c4a, /// 0x830
			 0xf27591b3, /// 0x834
			 0x03c30be2, /// 0x838
			 0x3660b35b, /// 0x83c
			 0x57ca7a1f, /// 0x840
			 0x5223edb4, /// 0x844
			 0x4b29ed5f, /// 0x848
			 0x9297f061, /// 0x84c
			 0xd4a76a3e, /// 0x850
			 0xa2493cd9, /// 0x854
			 0xc43144e7, /// 0x858
			 0x36fe0856, /// 0x85c
			 0x65973302, /// 0x860
			 0x224f21b8, /// 0x864
			 0xa4c51213, /// 0x868
			 0xd0507ebe, /// 0x86c
			 0x12579ed4, /// 0x870
			 0xf56b2ac7, /// 0x874
			 0x3783b7f4, /// 0x878
			 0xb0cedffc, /// 0x87c
			 0x85a429b3, /// 0x880
			 0x97c5f1d4, /// 0x884
			 0xdc4e0c34, /// 0x888
			 0x085b0c0c, /// 0x88c
			 0x4025d253, /// 0x890
			 0x410ee876, /// 0x894
			 0xbd20c6bc, /// 0x898
			 0x6da543db, /// 0x89c
			 0x69ac6c55, /// 0x8a0
			 0xcc006f4c, /// 0x8a4
			 0x41d64f82, /// 0x8a8
			 0xe09b38e0, /// 0x8ac
			 0x9ebf2eb4, /// 0x8b0
			 0x5556c7d1, /// 0x8b4
			 0xb07f5668, /// 0x8b8
			 0x8babf24d, /// 0x8bc
			 0x3827caf4, /// 0x8c0
			 0xb5ea76ca, /// 0x8c4
			 0xa9e31334, /// 0x8c8
			 0xe3cb66d8, /// 0x8cc
			 0x032daabd, /// 0x8d0
			 0x33262308, /// 0x8d4
			 0x2b038438, /// 0x8d8
			 0xbe7d80a5, /// 0x8dc
			 0x91e26f5a, /// 0x8e0
			 0x3959b7be, /// 0x8e4
			 0x5093c27c, /// 0x8e8
			 0x79e661a5, /// 0x8ec
			 0x7b77d568, /// 0x8f0
			 0xebb16b4f, /// 0x8f4
			 0xb34cb2a2, /// 0x8f8
			 0xc9059254, /// 0x8fc
			 0x32edadf2, /// 0x900
			 0x8870f94e, /// 0x904
			 0x2e29d045, /// 0x908
			 0xdcc1a48f, /// 0x90c
			 0x6ce413fb, /// 0x910
			 0xff336a93, /// 0x914
			 0x2f37b2ab, /// 0x918
			 0x76d936c9, /// 0x91c
			 0x448f887f, /// 0x920
			 0x55db180b, /// 0x924
			 0x7f8c52c1, /// 0x928
			 0x11ada0f6, /// 0x92c
			 0x827a98f8, /// 0x930
			 0xb75d3693, /// 0x934
			 0xd4615dd7, /// 0x938
			 0xd5de40d2, /// 0x93c
			 0x653343ed, /// 0x940
			 0x94c88208, /// 0x944
			 0xf0adb3ed, /// 0x948
			 0xf0159c52, /// 0x94c
			 0x5a24ab1b, /// 0x950
			 0x0eb4e054, /// 0x954
			 0x8b7440df, /// 0x958
			 0x1bc417ae, /// 0x95c
			 0x19874e54, /// 0x960
			 0xba91acdc, /// 0x964
			 0x4da66183, /// 0x968
			 0xf37e8b76, /// 0x96c
			 0xe00899dc, /// 0x970
			 0xe1bf326a, /// 0x974
			 0xe375a160, /// 0x978
			 0x0936094d, /// 0x97c
			 0x52e46087, /// 0x980
			 0x852a5298, /// 0x984
			 0x42408b73, /// 0x988
			 0xb98ec412, /// 0x98c
			 0x176a4bb8, /// 0x990
			 0xf2212a63, /// 0x994
			 0x190ca411, /// 0x998
			 0x0a55503b, /// 0x99c
			 0x18a1933e, /// 0x9a0
			 0xa23240f2, /// 0x9a4
			 0x75e2e176, /// 0x9a8
			 0xf470127c, /// 0x9ac
			 0x2cdc4321, /// 0x9b0
			 0xad5fb2f6, /// 0x9b4
			 0xd3374388, /// 0x9b8
			 0x8f4c5ea9, /// 0x9bc
			 0x4e30114e, /// 0x9c0
			 0x89e937b4, /// 0x9c4
			 0xb691bbdf, /// 0x9c8
			 0x6b7af626, /// 0x9cc
			 0x921e0cc2, /// 0x9d0
			 0x8db632f9, /// 0x9d4
			 0x120e270d, /// 0x9d8
			 0xfa2cea6b, /// 0x9dc
			 0x11a3bdc0, /// 0x9e0
			 0xe6b1b1c6, /// 0x9e4
			 0xfaf3a85a, /// 0x9e8
			 0x6fa57073, /// 0x9ec
			 0x16867af8, /// 0x9f0
			 0x7b9893bb, /// 0x9f4
			 0x14490ccc, /// 0x9f8
			 0x0f17864a, /// 0x9fc
			 0xbc1ac4dc, /// 0xa00
			 0xdb1a4135, /// 0xa04
			 0x26e90d44, /// 0xa08
			 0xfec170dc, /// 0xa0c
			 0x5d4acd5c, /// 0xa10
			 0x021e718c, /// 0xa14
			 0x58ac19b2, /// 0xa18
			 0x4fba7c1d, /// 0xa1c
			 0x9c9dcde6, /// 0xa20
			 0x872bb8d8, /// 0xa24
			 0x52a1f3bc, /// 0xa28
			 0x9cf15d29, /// 0xa2c
			 0xcc8a16a6, /// 0xa30
			 0xbbaae133, /// 0xa34
			 0x380ee2ac, /// 0xa38
			 0x6228e135, /// 0xa3c
			 0xac6674e9, /// 0xa40
			 0xd4485e84, /// 0xa44
			 0x66d68be0, /// 0xa48
			 0x1921b643, /// 0xa4c
			 0xa2e0e8c3, /// 0xa50
			 0xbc904aa1, /// 0xa54
			 0x569c3685, /// 0xa58
			 0x1454f251, /// 0xa5c
			 0xb2f022af, /// 0xa60
			 0x66123013, /// 0xa64
			 0xe36268fb, /// 0xa68
			 0x027ebf11, /// 0xa6c
			 0x73cac262, /// 0xa70
			 0xa667ecf8, /// 0xa74
			 0x85b6b58d, /// 0xa78
			 0x22d011e5, /// 0xa7c
			 0xc133f2f5, /// 0xa80
			 0x3268baee, /// 0xa84
			 0x2a5d603e, /// 0xa88
			 0x490b6776, /// 0xa8c
			 0x9ddb16d1, /// 0xa90
			 0x03413a16, /// 0xa94
			 0x1d3d9f6e, /// 0xa98
			 0x2372cde2, /// 0xa9c
			 0x4973131a, /// 0xaa0
			 0xd828a22c, /// 0xaa4
			 0x8dd85185, /// 0xaa8
			 0xae91c8d6, /// 0xaac
			 0x025906fa, /// 0xab0
			 0xd15a3158, /// 0xab4
			 0x72b8a78d, /// 0xab8
			 0x650233d5, /// 0xabc
			 0x3e3f23dc, /// 0xac0
			 0xa7e00f80, /// 0xac4
			 0xccde6201, /// 0xac8
			 0x997a7d98, /// 0xacc
			 0x6179a0cd, /// 0xad0
			 0x0271185e, /// 0xad4
			 0x4ca68a82, /// 0xad8
			 0x38f07973, /// 0xadc
			 0xdb9c347a, /// 0xae0
			 0x58abd732, /// 0xae4
			 0x6557f92c, /// 0xae8
			 0xcf1a9e8d, /// 0xaec
			 0x769584f9, /// 0xaf0
			 0xecf9cb73, /// 0xaf4
			 0x40955aad, /// 0xaf8
			 0xaf4c3108, /// 0xafc
			 0x5d10aec2, /// 0xb00
			 0x98f26d24, /// 0xb04
			 0x67767aa8, /// 0xb08
			 0x540dcd02, /// 0xb0c
			 0x74b15231, /// 0xb10
			 0x4820ac0f, /// 0xb14
			 0xb9cadbd1, /// 0xb18
			 0x36f9d3d8, /// 0xb1c
			 0x07916b38, /// 0xb20
			 0xb8764834, /// 0xb24
			 0x89309f5d, /// 0xb28
			 0x943b0a03, /// 0xb2c
			 0x650afa61, /// 0xb30
			 0xe073fa4d, /// 0xb34
			 0xa2836c1c, /// 0xb38
			 0x9aea404f, /// 0xb3c
			 0xf96a0c90, /// 0xb40
			 0xf90dbb68, /// 0xb44
			 0x7c374325, /// 0xb48
			 0xa162a725, /// 0xb4c
			 0x1d20f28f, /// 0xb50
			 0xcbb63f4d, /// 0xb54
			 0xa9881b48, /// 0xb58
			 0xf459ae58, /// 0xb5c
			 0x99d93a82, /// 0xb60
			 0xb8be3b36, /// 0xb64
			 0x9a195c85, /// 0xb68
			 0xe7d6af35, /// 0xb6c
			 0xadfe4b0e, /// 0xb70
			 0xbe4ad9f7, /// 0xb74
			 0x7486e32a, /// 0xb78
			 0x3c0b6587, /// 0xb7c
			 0x44d006db, /// 0xb80
			 0x23c4d45d, /// 0xb84
			 0x96890f94, /// 0xb88
			 0x8940b403, /// 0xb8c
			 0xf5cff6df, /// 0xb90
			 0xfa31e2fe, /// 0xb94
			 0xea4863d5, /// 0xb98
			 0x5da7b47e, /// 0xb9c
			 0xe9eb6a3c, /// 0xba0
			 0x32fbcfe7, /// 0xba4
			 0x88ea476e, /// 0xba8
			 0xc13c5832, /// 0xbac
			 0x1fb27939, /// 0xbb0
			 0x25a96888, /// 0xbb4
			 0xc486b280, /// 0xbb8
			 0xcff68aec, /// 0xbbc
			 0xaa96b24a, /// 0xbc0
			 0x78b6b64c, /// 0xbc4
			 0x8b138cbf, /// 0xbc8
			 0xeabe6487, /// 0xbcc
			 0x909e422a, /// 0xbd0
			 0xd55e7dfe, /// 0xbd4
			 0x588bb61b, /// 0xbd8
			 0x8f230a81, /// 0xbdc
			 0xc0c4bc63, /// 0xbe0
			 0x7e10a845, /// 0xbe4
			 0xaa13031c, /// 0xbe8
			 0xc3fb3096, /// 0xbec
			 0x5a14ad44, /// 0xbf0
			 0xcb715a79, /// 0xbf4
			 0x2abf5048, /// 0xbf8
			 0x0c5c7cf3, /// 0xbfc
			 0xf7b76593, /// 0xc00
			 0x10355dd0, /// 0xc04
			 0xae97cbbd, /// 0xc08
			 0x03f4ae4b, /// 0xc0c
			 0x13cd4a7d, /// 0xc10
			 0xd517374b, /// 0xc14
			 0x0e821fdd, /// 0xc18
			 0x7e02747c, /// 0xc1c
			 0x34dd6f6f, /// 0xc20
			 0x2a1aaab5, /// 0xc24
			 0x0b756fff, /// 0xc28
			 0x04f898b2, /// 0xc2c
			 0xe7e39f30, /// 0xc30
			 0xaf0863b8, /// 0xc34
			 0x6d406cdb, /// 0xc38
			 0x184e2ad5, /// 0xc3c
			 0x3f3d74fa, /// 0xc40
			 0x6a3da87b, /// 0xc44
			 0xa6254089, /// 0xc48
			 0xb54e4375, /// 0xc4c
			 0xe37b17f0, /// 0xc50
			 0x276ab612, /// 0xc54
			 0x08c98d61, /// 0xc58
			 0x077d0662, /// 0xc5c
			 0xae22519c, /// 0xc60
			 0x0aebee0a, /// 0xc64
			 0x9f62a54e, /// 0xc68
			 0xa6333f32, /// 0xc6c
			 0xd5a567e1, /// 0xc70
			 0xfcf7e6d7, /// 0xc74
			 0x03ea9c97, /// 0xc78
			 0x5795431b, /// 0xc7c
			 0xd51ca470, /// 0xc80
			 0x76b6ffeb, /// 0xc84
			 0xd9cd61cb, /// 0xc88
			 0xfe4a5e87, /// 0xc8c
			 0xba8f8d8f, /// 0xc90
			 0x77a9f976, /// 0xc94
			 0xdfd72911, /// 0xc98
			 0xaef04614, /// 0xc9c
			 0x3dba6e32, /// 0xca0
			 0x5557ed6e, /// 0xca4
			 0x03c91a0e, /// 0xca8
			 0x535812e0, /// 0xcac
			 0x21fda0a7, /// 0xcb0
			 0xb85adf78, /// 0xcb4
			 0xdbf75d1e, /// 0xcb8
			 0xc875679a, /// 0xcbc
			 0xb47832ac, /// 0xcc0
			 0x65f160af, /// 0xcc4
			 0x2d4e62f1, /// 0xcc8
			 0x505c033b, /// 0xccc
			 0x070e6487, /// 0xcd0
			 0x082b2948, /// 0xcd4
			 0x188fbcf1, /// 0xcd8
			 0x0b4bb321, /// 0xcdc
			 0x1778e010, /// 0xce0
			 0x34d8c5b5, /// 0xce4
			 0xb097051b, /// 0xce8
			 0x7113dd77, /// 0xcec
			 0x9841f37e, /// 0xcf0
			 0x447a5b07, /// 0xcf4
			 0xbf3510d8, /// 0xcf8
			 0xa54218d0, /// 0xcfc
			 0x5ee7ff3f, /// 0xd00
			 0x0f3cd216, /// 0xd04
			 0x481abc0c, /// 0xd08
			 0x3381e468, /// 0xd0c
			 0xda59282f, /// 0xd10
			 0x15bdee78, /// 0xd14
			 0x6106c4dd, /// 0xd18
			 0x18d4b94d, /// 0xd1c
			 0x9df6655a, /// 0xd20
			 0x1e4bc624, /// 0xd24
			 0x7e064f5b, /// 0xd28
			 0x2ae396d7, /// 0xd2c
			 0x090375f0, /// 0xd30
			 0x99521268, /// 0xd34
			 0x02ef8a32, /// 0xd38
			 0xbd7f97e2, /// 0xd3c
			 0x7fe68b09, /// 0xd40
			 0x7a3e35c0, /// 0xd44
			 0x9c0c33b1, /// 0xd48
			 0xd2bcc9b8, /// 0xd4c
			 0xb8ec931a, /// 0xd50
			 0x1dfcef37, /// 0xd54
			 0x0c0dd4ea, /// 0xd58
			 0x46753c92, /// 0xd5c
			 0x5fa15ef2, /// 0xd60
			 0x382bb541, /// 0xd64
			 0x26cc4516, /// 0xd68
			 0x924c9ad9, /// 0xd6c
			 0x347bce69, /// 0xd70
			 0x8f9a7884, /// 0xd74
			 0x0d9cdef3, /// 0xd78
			 0x3eb7bcd4, /// 0xd7c
			 0xb911b093, /// 0xd80
			 0xcfb7a190, /// 0xd84
			 0x5b961b5d, /// 0xd88
			 0x4205deed, /// 0xd8c
			 0x8a03dc36, /// 0xd90
			 0xfa48ae43, /// 0xd94
			 0xbdfd376d, /// 0xd98
			 0xe2ca8aa2, /// 0xd9c
			 0x5e035b0b, /// 0xda0
			 0xa3b3f5de, /// 0xda4
			 0x0a4264dc, /// 0xda8
			 0x12eceb6f, /// 0xdac
			 0x37c7f017, /// 0xdb0
			 0xe2ab1db2, /// 0xdb4
			 0xa6a2abc5, /// 0xdb8
			 0xe1f3cbf6, /// 0xdbc
			 0x9952b840, /// 0xdc0
			 0x092afa06, /// 0xdc4
			 0x435a5dda, /// 0xdc8
			 0x87a3fc32, /// 0xdcc
			 0xcd19dded, /// 0xdd0
			 0xc98ab30e, /// 0xdd4
			 0x570c8b1f, /// 0xdd8
			 0x082e884c, /// 0xddc
			 0x3ca558cd, /// 0xde0
			 0x7b715cdf, /// 0xde4
			 0x1c28d5bf, /// 0xde8
			 0x8af4f42b, /// 0xdec
			 0x47085bbd, /// 0xdf0
			 0x9ebdb0ad, /// 0xdf4
			 0x3c2859db, /// 0xdf8
			 0x55344ccf, /// 0xdfc
			 0xe1f0e0c1, /// 0xe00
			 0x37f82c8e, /// 0xe04
			 0x9416af28, /// 0xe08
			 0x8f8bc5aa, /// 0xe0c
			 0xfae0b212, /// 0xe10
			 0x94a43ae1, /// 0xe14
			 0x9b935056, /// 0xe18
			 0x5c7cfa37, /// 0xe1c
			 0xb2f7e4c7, /// 0xe20
			 0xb794eebc, /// 0xe24
			 0xdf2b8032, /// 0xe28
			 0xcbbda236, /// 0xe2c
			 0x980d86c7, /// 0xe30
			 0xecf4f11f, /// 0xe34
			 0x1aee7049, /// 0xe38
			 0x1953c46b, /// 0xe3c
			 0xd5d67338, /// 0xe40
			 0x2ade6cf1, /// 0xe44
			 0x77d4c546, /// 0xe48
			 0x03ed8b17, /// 0xe4c
			 0x037b6b52, /// 0xe50
			 0x5e4d108b, /// 0xe54
			 0x130b5adc, /// 0xe58
			 0x52ddf8e9, /// 0xe5c
			 0xa829f470, /// 0xe60
			 0x1c472c40, /// 0xe64
			 0xa0928db1, /// 0xe68
			 0x8be85764, /// 0xe6c
			 0x5287c1ca, /// 0xe70
			 0x0e7b3293, /// 0xe74
			 0x5ac1aa77, /// 0xe78
			 0x2ddb8ca6, /// 0xe7c
			 0xeec4bffd, /// 0xe80
			 0xee4ec7fd, /// 0xe84
			 0xffe3b907, /// 0xe88
			 0x784b0b91, /// 0xe8c
			 0x2511e632, /// 0xe90
			 0xcdaf29d9, /// 0xe94
			 0x95f8f87a, /// 0xe98
			 0xe34fc355, /// 0xe9c
			 0x5418f2db, /// 0xea0
			 0xecb89818, /// 0xea4
			 0x735c8963, /// 0xea8
			 0xd7bed550, /// 0xeac
			 0x7ce8793f, /// 0xeb0
			 0x18fb7bc9, /// 0xeb4
			 0x6506a953, /// 0xeb8
			 0x64740a55, /// 0xebc
			 0x87861737, /// 0xec0
			 0x02d7f205, /// 0xec4
			 0xfb2fd6c7, /// 0xec8
			 0xcb5f4ef8, /// 0xecc
			 0x0110497d, /// 0xed0
			 0xe1f4dcda, /// 0xed4
			 0x8d66c410, /// 0xed8
			 0x1ddc60a2, /// 0xedc
			 0xd45e8894, /// 0xee0
			 0x2654e115, /// 0xee4
			 0xa57da04e, /// 0xee8
			 0x6154da5c, /// 0xeec
			 0x40ea3f23, /// 0xef0
			 0x81737df3, /// 0xef4
			 0xa3d2d648, /// 0xef8
			 0x7cff3ddf, /// 0xefc
			 0x41f34683, /// 0xf00
			 0xeaf8edcf, /// 0xf04
			 0x1fd2ed8f, /// 0xf08
			 0xc18f65d5, /// 0xf0c
			 0xc70143a8, /// 0xf10
			 0x4bf467c1, /// 0xf14
			 0xb4c563d0, /// 0xf18
			 0xa54339cc, /// 0xf1c
			 0x8c8776b8, /// 0xf20
			 0x71062ac8, /// 0xf24
			 0x6e655609, /// 0xf28
			 0x284d1073, /// 0xf2c
			 0x4451763e, /// 0xf30
			 0x11b3053e, /// 0xf34
			 0xd66c6743, /// 0xf38
			 0xa9caf3af, /// 0xf3c
			 0xb3a7db99, /// 0xf40
			 0x622c0ecc, /// 0xf44
			 0x827f39ea, /// 0xf48
			 0xe4fe169d, /// 0xf4c
			 0x3cd7e004, /// 0xf50
			 0xe2d16ebb, /// 0xf54
			 0x4dd71640, /// 0xf58
			 0xee439e63, /// 0xf5c
			 0x50104132, /// 0xf60
			 0xb10039f5, /// 0xf64
			 0xb4cb61c4, /// 0xf68
			 0xdacf8eab, /// 0xf6c
			 0xbdd47a7c, /// 0xf70
			 0x327846a2, /// 0xf74
			 0x61154dd2, /// 0xf78
			 0x62b702b5, /// 0xf7c
			 0x872484b3, /// 0xf80
			 0x2a201ef3, /// 0xf84
			 0x33a6df70, /// 0xf88
			 0xe1a1b86c, /// 0xf8c
			 0xe8ca0154, /// 0xf90
			 0xe0340bb6, /// 0xf94
			 0x76cb70ad, /// 0xf98
			 0x558d76c5, /// 0xf9c
			 0x9de3164f, /// 0xfa0
			 0x23f5d3a5, /// 0xfa4
			 0x121aa1a6, /// 0xfa8
			 0xf64353a4, /// 0xfac
			 0xad43c0df, /// 0xfb0
			 0xed58134c, /// 0xfb4
			 0xac953657, /// 0xfb8
			 0xd5bba1fc, /// 0xfbc
			 0xfc7f868b, /// 0xfc0
			 0x7c00683e, /// 0xfc4
			 0x7cd04d3d, /// 0xfc8
			 0x836f8d60, /// 0xfcc
			 0x00f2b754, /// 0xfd0
			 0x9af0c85a, /// 0xfd4
			 0x146126a2, /// 0xfd8
			 0x2866c598, /// 0xfdc
			 0x506936b1, /// 0xfe0
			 0xeadae3d9, /// 0xfe4
			 0x60b9855d, /// 0xfe8
			 0xdf140913, /// 0xfec
			 0x9e00bb15, /// 0xff0
			 0x53bff1c6, /// 0xff4
			 0x98a2577c, /// 0xff8
			 0xf7e434c3 /// last
	};
	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			 // vpu register f0 is designated 0 register
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,

			 // vpu register f1 is designated offset register for memory access ops
			 0x00000574,
			 0x000006e0,
			 0x000001d4,
			 0x000001e8,
			 0x00000404,
			 0x000007cc,
			 0x00000208,
			 0x000002b4,

			 /// vpu register f2
			 0x41a80000,
			 0x41f80000,
			 0x40400000,
			 0x41000000,
			 0x41f00000,
			 0x3f800000,
			 0x41e80000,
			 0x41200000,

			 /// vpu register f3
			 0x41e80000,
			 0x41000000,
			 0x41880000,
			 0x41200000,
			 0x41000000,
			 0x41b00000,
			 0x41100000,
			 0x41f00000,

			 /// vpu register f4
			 0x41900000,
			 0x41300000,
			 0x40c00000,
			 0x40e00000,
			 0x41800000,
			 0x40a00000,
			 0x40a00000,
			 0x3f800000,

			 /// vpu register f5
			 0x40400000,
			 0x41d80000,
			 0x40e00000,
			 0x41d80000,
			 0x41200000,
			 0x41500000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f6
			 0x41100000,
			 0x41c00000,
			 0x41d80000,
			 0x40c00000,
			 0x41800000,
			 0x41800000,
			 0x41d00000,
			 0x41a80000,

			 /// vpu register f7
			 0x41d00000,
			 0x41a80000,
			 0x41100000,
			 0x41d80000,
			 0x41100000,
			 0x41b80000,
			 0x41e00000,
			 0x41d00000,

			 /// vpu register f8
			 0x41200000,
			 0x41500000,
			 0x40e00000,
			 0x41900000,
			 0x40800000,
			 0x3f800000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f9
			 0x40400000,
			 0x41500000,
			 0x41300000,
			 0x41f80000,
			 0x41500000,
			 0x41200000,
			 0x41a80000,
			 0x41c80000,

			 /// vpu register f10
			 0x41000000,
			 0x41c00000,
			 0x41d00000,
			 0x41600000,
			 0x41c00000,
			 0x40400000,
			 0x41d80000,
			 0x40800000,

			 /// vpu register f11
			 0x41d80000,
			 0x41400000,
			 0x41900000,
			 0x41880000,
			 0x41b00000,
			 0x42000000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f12
			 0x41600000,
			 0x41980000,
			 0x40000000,
			 0x41c00000,
			 0x41600000,
			 0x41d00000,
			 0x41b80000,
			 0x41300000,

			 /// vpu register f13
			 0x41e80000,
			 0x41e80000,
			 0x41500000,
			 0x41800000,
			 0x41700000,
			 0x41400000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f14
			 0x41c80000,
			 0x41c80000,
			 0x40800000,
			 0x41f80000,
			 0x41a00000,
			 0x41c80000,
			 0x41500000,
			 0x40000000,

			 /// vpu register f15
			 0x41100000,
			 0x41c80000,
			 0x41c00000,
			 0x41e80000,
			 0x41f00000,
			 0x41e80000,
			 0x41b80000,
			 0x41e00000,

			 /// vpu register f16
			 0x41a00000,
			 0x41b00000,
			 0x40e00000,
			 0x41100000,
			 0x40800000,
			 0x41b00000,
			 0x41980000,
			 0x3f800000,

			 /// vpu register f17
			 0x41980000,
			 0x3f800000,
			 0x41300000,
			 0x40c00000,
			 0x41800000,
			 0x41d00000,
			 0x3f800000,
			 0x41b80000,

			 /// vpu register f18
			 0x41c00000,
			 0x40400000,
			 0x40800000,
			 0x3f800000,
			 0x41880000,
			 0x41400000,
			 0x40000000,
			 0x41700000,

			 /// vpu register f19
			 0x41b80000,
			 0x41f80000,
			 0x41e80000,
			 0x42000000,
			 0x41c00000,
			 0x41500000,
			 0x41980000,
			 0x41e80000,

			 /// vpu register f20
			 0x41f00000,
			 0x41300000,
			 0x41a80000,
			 0x41600000,
			 0x3f800000,
			 0x41b80000,
			 0x41d80000,
			 0x41c00000,

			 /// vpu register f21
			 0x41700000,
			 0x41e80000,
			 0x40e00000,
			 0x40400000,
			 0x41500000,
			 0x41200000,
			 0x41400000,
			 0x41500000,

			 /// vpu register f22
			 0x41a00000,
			 0x40a00000,
			 0x41700000,
			 0x41d80000,
			 0x41300000,
			 0x41000000,
			 0x41000000,
			 0x41980000,

			 /// vpu register f23
			 0x41a00000,
			 0x41c80000,
			 0x41b00000,
			 0x41880000,
			 0x40a00000,
			 0x41100000,
			 0x41b00000,
			 0x40800000,

			 /// vpu register f24
			 0x41a00000,
			 0x40c00000,
			 0x41700000,
			 0x40800000,
			 0x3f800000,
			 0x41400000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f25
			 0x41a00000,
			 0x41c00000,
			 0x40c00000,
			 0x41980000,
			 0x41200000,
			 0x41300000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f26
			 0x41500000,
			 0x41400000,
			 0x41100000,
			 0x41300000,
			 0x3f800000,
			 0x40000000,
			 0x41e80000,
			 0x41e80000,

			 /// vpu register f27
			 0x41b80000,
			 0x41800000,
			 0x41980000,
			 0x41a80000,
			 0x3f800000,
			 0x40000000,
			 0x41f00000,
			 0x40000000,

			 /// vpu register f28
			 0x41e80000,
			 0x41300000,
			 0x40c00000,
			 0x41500000,
			 0x41d00000,
			 0x41a00000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f29
			 0x41000000,
			 0x41e80000,
			 0x41e00000,
			 0x3f800000,
			 0x41000000,
			 0x40c00000,
			 0x3f800000,
			 0x41e00000,

			 /// vpu register f30
			 0x41400000,
			 0x41100000,
			 0x41e80000,
			 0x41b80000,
			 0x41e80000,
			 0x40c00000,
			 0x41d00000,
			 0x42000000,

			 /// vpu register f31
			 0x41980000,
			 0x41100000,
			 0x41e00000,
			 0x41000000,
			 0x40400000,
			 0x41900000,
			 0x40000000,
			 0x3f800000     /// 1.0                   
	};


	C_TEST_START;

	__asm__ __volatile__ (
		"LBL_C_TEST_START:\n"
		"j LBL_SETUP_EV\n"
		".align 12\n"
		"M_MODE_E_H:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR\n"
		"LBL_SETUP_SCP:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
		"LBL_RET_TO_CLR:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
		"LBL_S_MODE_E_H:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
		"LBL_SETUP_EV:\n"
		"la x5, M_MODE_E_H\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"mv x11, %[m_11]\n"
		"addi x11, x11, 2047\n"
		"addi x11, x11, 1\n"
		"mv x12, %[m_12]\n"
		"addi x12, x12, 2047\n"
		"addi x12, x12, 1\n"
		"mv x13, %[m_13]\n"
		"addi x13, x13, 2047\n"
		"addi x13, x13, 1\n"
		"mv x14, %[m_14]\n"
		"addi x14, x14, 2047\n"
		"addi x14, x14, 1\n"
		"mv x15, %[m_15]\n"
		"addi x15, x15, 2047\n"
		"addi x15, x15, 1\n"
		"mv x4, %[operands]\n"
		"flw.ps f0,  0 (x4)\n"
		"flw.ps f1,  32 (x4)\n"
		"flw.ps f2,  64 (x4)\n"
		"flw.ps f3,  96 (x4)\n"
		"flw.ps f4,  128 (x4)\n"
		"flw.ps f5,  160 (x4)\n"
		"flw.ps f6,  192 (x4)\n"
		"flw.ps f7,  224 (x4)\n"
		"flw.ps f8,  256 (x4)\n"
		"flw.ps f9,  288 (x4)\n"
		"flw.ps f10,  320 (x4)\n"
		"flw.ps f11,  352 (x4)\n"
		"flw.ps f12,  384 (x4)\n"
		"flw.ps f13,  416 (x4)\n"
		"flw.ps f14,  448 (x4)\n"
		"flw.ps f15,  480 (x4)\n"
		"flw.ps f16,  512 (x4)\n"
		"flw.ps f17,  544 (x4)\n"
		"flw.ps f18,  576 (x4)\n"
		"flw.ps f19,  608 (x4)\n"
		"flw.ps f20,  640 (x4)\n"
		"flw.ps f21,  672 (x4)\n"
		"flw.ps f22,  704 (x4)\n"
		"flw.ps f23,  736 (x4)\n"
		"flw.ps f24,  768 (x4)\n"
		"flw.ps f25,  800 (x4)\n"
		"flw.ps f26,  832 (x4)\n"
		"flw.ps f27,  864 (x4)\n"
		"flw.ps f28,  896 (x4)\n"
		"flw.ps f29,  928 (x4)\n"
		"flw.ps f30,  960 (x4)\n"
		"flw.ps f31,  992 (x4)\n"
		//RTLMIN-5479"fsin.ps f0, f0\n"
		"fexp.ps f0, f0\n"
		"frcp.ps f0, f0\n"
		//RTLMIN-5479"frsq.ps f0, f0\n"
		"flog.ps f0, f0\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_VPU_SEQ_START:\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f14, f1 (x12)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f9, f1 (x11)\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f16, f1 (x11)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f8, f1 (x12)\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f28, f1 (x15)\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f27, f1 (x12)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f16, f1 (x13)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x12)\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f9, f1 (x14)\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f24, f1 (x12)\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x13)\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f21, f1 (x14)\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f3, f1 (x11)\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f12, f1 (x11)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f13, f1 (x14)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x15)\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f22, f1 (x15)\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f7, f1 (x14)\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f22, f1 (x15)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x14)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f25, f1 (x11)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f18, f1 (x13)\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x13)\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f0, f1 (x15)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f0, f1 (x15)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f23, f1 (x15)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f3, f1 (x13)\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f23, f1 (x13)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f5, f1 (x13)\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f9, f1 (x15)\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f2, f1 (x15)\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f4, f1 (x14)\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f26, f1 (x11)\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f15, f1 (x15)\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f19, f1 (x12)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f17, f1 (x13)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x11)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f25, f1 (x15)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f22, f1 (x15)\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f1, f1 (x14)\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f20, f1 (x12)\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x13)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f5, f1 (x13)\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f20, f1 (x14)\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f17, f1 (x14)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f24, f1 (x11)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f13, f1 (x11)\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f15, f1 (x11)\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f20, f1 (x11)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f11, f1 (x11)\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f15, f1 (x15)\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f17, f1 (x11)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f3, f1 (x14)\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f11, f1 (x11)\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f16, f1 (x14)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f15, f1 (x15)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f18, f1 (x15)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f8, f1 (x13)\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f11, f1 (x13)\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f9, f1 (x12)\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f3, f1 (x11)\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f17, f1 (x14)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f8, f1 (x15)\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f5, f1 (x11)\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f8, f1 (x14)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f4, f1 (x13)\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f8, f1 (x14)\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f18, f1 (x14)\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f26, f1 (x14)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f21, f1 (x13)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f8, f1 (x12)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f19, f1 (x12)\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x15)\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f25, f1 (x12)\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f27, f1 (x13)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f14, f1 (x11)\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f18, f1 (x14)\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f0, f1 (x14)\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f14, f1 (x12)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f23, f1 (x13)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f30, f1 (x15)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f30, f1 (x14)\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f24, f1 (x12)\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f10, f1 (x14)\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f26, f1 (x15)\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f18, f1 (x13)\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f7, f1 (x15)\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x13)\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f5, f1 (x14)\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f28, f1 (x13)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f14, f1 (x12)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x13)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f4, f1 (x11)\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f27, f1 (x13)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f13, f1 (x13)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f14, f1 (x14)\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f25, f1 (x12)\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f2, f1 (x13)\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscb.ps f29, f1 (x12)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
