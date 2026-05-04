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
			 0x042dcaaa, /// 0x0
			 0x26a13a54, /// 0x4
			 0x16496536, /// 0x8
			 0x8dbcee8e, /// 0xc
			 0x1ef64f68, /// 0x10
			 0xd350979f, /// 0x14
			 0x5586fa94, /// 0x18
			 0x0e55153b, /// 0x1c
			 0x9c5795bc, /// 0x20
			 0x0459eace, /// 0x24
			 0xdc3294e0, /// 0x28
			 0x5cba2541, /// 0x2c
			 0x7b33b605, /// 0x30
			 0xe3775f31, /// 0x34
			 0xcb5815e6, /// 0x38
			 0xed9696c2, /// 0x3c
			 0x61521fa6, /// 0x40
			 0xc32a5d87, /// 0x44
			 0x9b9de886, /// 0x48
			 0x43053a37, /// 0x4c
			 0x5a6a8acc, /// 0x50
			 0x1118fc19, /// 0x54
			 0x11047966, /// 0x58
			 0xd91fb549, /// 0x5c
			 0x86176f6f, /// 0x60
			 0x2c969074, /// 0x64
			 0xd574e4ef, /// 0x68
			 0x047706c7, /// 0x6c
			 0xb0489736, /// 0x70
			 0xf70ed956, /// 0x74
			 0xf732000f, /// 0x78
			 0x1382738e, /// 0x7c
			 0xfee42ea6, /// 0x80
			 0x79a9fedf, /// 0x84
			 0x3f26b6c8, /// 0x88
			 0x919e86f6, /// 0x8c
			 0xb8d0b712, /// 0x90
			 0x43e22ee6, /// 0x94
			 0x9a0bd5f0, /// 0x98
			 0xd4e5768c, /// 0x9c
			 0xf305d7ec, /// 0xa0
			 0xd789ccb9, /// 0xa4
			 0xc4af8c1d, /// 0xa8
			 0xc7f72157, /// 0xac
			 0x0992137b, /// 0xb0
			 0xa5eebdbc, /// 0xb4
			 0xf37016b7, /// 0xb8
			 0x79e4e0b0, /// 0xbc
			 0xb7fc0098, /// 0xc0
			 0x8362b57f, /// 0xc4
			 0x2009cf4b, /// 0xc8
			 0x6b05b13e, /// 0xcc
			 0xf6fc3a5b, /// 0xd0
			 0x97e84677, /// 0xd4
			 0x648e4c08, /// 0xd8
			 0xc10de8c5, /// 0xdc
			 0xb1b9d52c, /// 0xe0
			 0x83cc8685, /// 0xe4
			 0xdd79a597, /// 0xe8
			 0xf35a045e, /// 0xec
			 0x940ddda3, /// 0xf0
			 0xf7f2bc4a, /// 0xf4
			 0x7b79e006, /// 0xf8
			 0xf5b5c404, /// 0xfc
			 0xfbc241a3, /// 0x100
			 0xbbac1989, /// 0x104
			 0x4bd51f66, /// 0x108
			 0xb0f6b7c3, /// 0x10c
			 0x7e6638f4, /// 0x110
			 0x56cbd497, /// 0x114
			 0xd1f1a3ed, /// 0x118
			 0x285bcd67, /// 0x11c
			 0x459c3fea, /// 0x120
			 0x87899a11, /// 0x124
			 0xae198f26, /// 0x128
			 0x94e4adae, /// 0x12c
			 0xbdc8df46, /// 0x130
			 0xb47c848e, /// 0x134
			 0xb46aeb4d, /// 0x138
			 0x135f6746, /// 0x13c
			 0xcbe1cf37, /// 0x140
			 0x42213ca1, /// 0x144
			 0x04c52c86, /// 0x148
			 0x0f97cae4, /// 0x14c
			 0xf266e6fb, /// 0x150
			 0x188a9380, /// 0x154
			 0x708bb4dd, /// 0x158
			 0x0894579a, /// 0x15c
			 0xfe28c3b5, /// 0x160
			 0x8009a904, /// 0x164
			 0x046b46b5, /// 0x168
			 0x7fec0424, /// 0x16c
			 0xaebd877e, /// 0x170
			 0xb20f8533, /// 0x174
			 0xf7108302, /// 0x178
			 0xdf5f9442, /// 0x17c
			 0x0af00557, /// 0x180
			 0xbcc25fcb, /// 0x184
			 0xb665ab40, /// 0x188
			 0x29e42ff3, /// 0x18c
			 0xce33163f, /// 0x190
			 0xa0f2e651, /// 0x194
			 0xc53dc0d1, /// 0x198
			 0xd74de54a, /// 0x19c
			 0xabb418ac, /// 0x1a0
			 0x1f2568d4, /// 0x1a4
			 0x58b31a63, /// 0x1a8
			 0x99e50abe, /// 0x1ac
			 0x7319ec62, /// 0x1b0
			 0xc38f0af4, /// 0x1b4
			 0x3cd519ca, /// 0x1b8
			 0xe30213c2, /// 0x1bc
			 0xdf7fa7bd, /// 0x1c0
			 0x787fb5cf, /// 0x1c4
			 0xc002fc6b, /// 0x1c8
			 0xaa46c2a4, /// 0x1cc
			 0x893cd45a, /// 0x1d0
			 0x45e9ed2e, /// 0x1d4
			 0xbef63ee7, /// 0x1d8
			 0x1e4f82cb, /// 0x1dc
			 0xbb3564c9, /// 0x1e0
			 0xa779e9c0, /// 0x1e4
			 0x9215ecb0, /// 0x1e8
			 0x9f95788a, /// 0x1ec
			 0xc9c5d46f, /// 0x1f0
			 0xeeae419f, /// 0x1f4
			 0x0a1f1de1, /// 0x1f8
			 0x28b9860a, /// 0x1fc
			 0x334e533b, /// 0x200
			 0xfe28d48a, /// 0x204
			 0x6f7bd587, /// 0x208
			 0x280b2920, /// 0x20c
			 0x7d850049, /// 0x210
			 0x15d4b511, /// 0x214
			 0x8fef28ba, /// 0x218
			 0x78eec5a9, /// 0x21c
			 0x857d57f1, /// 0x220
			 0x7fe3cfb7, /// 0x224
			 0x6c2fe839, /// 0x228
			 0x59efe02c, /// 0x22c
			 0xc27d3725, /// 0x230
			 0x38d0cae2, /// 0x234
			 0x11980c5d, /// 0x238
			 0xdeaa5e6c, /// 0x23c
			 0x6913ae61, /// 0x240
			 0x2e405737, /// 0x244
			 0x55a20afb, /// 0x248
			 0x9892e8ab, /// 0x24c
			 0x1f566619, /// 0x250
			 0x54c3b522, /// 0x254
			 0xaabf9eea, /// 0x258
			 0xa521451c, /// 0x25c
			 0x0fb0fc10, /// 0x260
			 0xc2611d36, /// 0x264
			 0x935d4e53, /// 0x268
			 0xdbd68e5c, /// 0x26c
			 0x734c8a41, /// 0x270
			 0x55573692, /// 0x274
			 0xfff7b728, /// 0x278
			 0x10a3399f, /// 0x27c
			 0xa78e080b, /// 0x280
			 0x0b9e28be, /// 0x284
			 0xb86e6b6e, /// 0x288
			 0x8cb0c9e9, /// 0x28c
			 0xa71da051, /// 0x290
			 0x439bce93, /// 0x294
			 0xf19bc572, /// 0x298
			 0x9085e781, /// 0x29c
			 0xbf529978, /// 0x2a0
			 0xecea8b54, /// 0x2a4
			 0x5475fd86, /// 0x2a8
			 0x2df278ec, /// 0x2ac
			 0x5d37de65, /// 0x2b0
			 0xc79041cd, /// 0x2b4
			 0xee7d3833, /// 0x2b8
			 0x884f8f5e, /// 0x2bc
			 0x920e6785, /// 0x2c0
			 0x07013814, /// 0x2c4
			 0x59596e91, /// 0x2c8
			 0xa36229ee, /// 0x2cc
			 0x0ae4dc17, /// 0x2d0
			 0x684d2fcd, /// 0x2d4
			 0xe826af05, /// 0x2d8
			 0x5678dc07, /// 0x2dc
			 0x8bc0c5ab, /// 0x2e0
			 0xee38f638, /// 0x2e4
			 0x95e5387f, /// 0x2e8
			 0x0eae0866, /// 0x2ec
			 0x1428aace, /// 0x2f0
			 0x066822b6, /// 0x2f4
			 0x528efe6f, /// 0x2f8
			 0x19e79dfc, /// 0x2fc
			 0xd3dca42d, /// 0x300
			 0x9dfc3426, /// 0x304
			 0xa2933b40, /// 0x308
			 0x2a50c6ff, /// 0x30c
			 0x6833b6fd, /// 0x310
			 0xf8fed3dc, /// 0x314
			 0x019e734e, /// 0x318
			 0x9a9b0c10, /// 0x31c
			 0x53bbc120, /// 0x320
			 0xfb271029, /// 0x324
			 0xfe333b80, /// 0x328
			 0x1183d3f8, /// 0x32c
			 0xf5e4d642, /// 0x330
			 0x3841d8ca, /// 0x334
			 0xbb3df891, /// 0x338
			 0xcb267a2e, /// 0x33c
			 0x474b0a68, /// 0x340
			 0xcd0a0ff1, /// 0x344
			 0xa2f5f677, /// 0x348
			 0x28cb8641, /// 0x34c
			 0xa27e7e0d, /// 0x350
			 0xff208634, /// 0x354
			 0x27d66611, /// 0x358
			 0x4281b09c, /// 0x35c
			 0x32958760, /// 0x360
			 0x76d62f52, /// 0x364
			 0xb23fa0ab, /// 0x368
			 0x2adebcf5, /// 0x36c
			 0x44c82b1b, /// 0x370
			 0xbdc5126c, /// 0x374
			 0xf67ef0fc, /// 0x378
			 0x445d7cd3, /// 0x37c
			 0x33b1486b, /// 0x380
			 0xf97ca065, /// 0x384
			 0xae1f3667, /// 0x388
			 0x6b39a0f6, /// 0x38c
			 0xaa8de25d, /// 0x390
			 0xe53302a9, /// 0x394
			 0xa01ffdd0, /// 0x398
			 0x1d24f7be, /// 0x39c
			 0x557437fa, /// 0x3a0
			 0x63570d07, /// 0x3a4
			 0x640b3b9b, /// 0x3a8
			 0x5c9732c0, /// 0x3ac
			 0xc3ffb414, /// 0x3b0
			 0xc6093930, /// 0x3b4
			 0x8e5edae1, /// 0x3b8
			 0x5a3e9f82, /// 0x3bc
			 0x4358ed26, /// 0x3c0
			 0x85422e94, /// 0x3c4
			 0xa4fa3a6a, /// 0x3c8
			 0x06b3a4d7, /// 0x3cc
			 0x609916b9, /// 0x3d0
			 0x0e005df5, /// 0x3d4
			 0x76ad5b14, /// 0x3d8
			 0x849cb086, /// 0x3dc
			 0x466580fd, /// 0x3e0
			 0x27b0af97, /// 0x3e4
			 0x28b1e0b4, /// 0x3e8
			 0xfb5c942c, /// 0x3ec
			 0x7bc10c3c, /// 0x3f0
			 0x5746e8e3, /// 0x3f4
			 0x7db4b4c1, /// 0x3f8
			 0xf3093f6c, /// 0x3fc
			 0xc7dc044b, /// 0x400
			 0x400d8c81, /// 0x404
			 0x5a4dfefb, /// 0x408
			 0xef222bfd, /// 0x40c
			 0x26aa0a11, /// 0x410
			 0xe671f92f, /// 0x414
			 0x3287c272, /// 0x418
			 0xc1040a63, /// 0x41c
			 0x70045fd2, /// 0x420
			 0x2d254b2a, /// 0x424
			 0x072859d3, /// 0x428
			 0x57e851dc, /// 0x42c
			 0xec153ce6, /// 0x430
			 0xf48ef34a, /// 0x434
			 0x78aee815, /// 0x438
			 0xabcf5548, /// 0x43c
			 0x1524e5dc, /// 0x440
			 0xd54a3d1e, /// 0x444
			 0x12d61a75, /// 0x448
			 0x74b31fec, /// 0x44c
			 0x81fb7449, /// 0x450
			 0xc18ac87a, /// 0x454
			 0x426e7ecd, /// 0x458
			 0xfb6567a2, /// 0x45c
			 0x65c42b70, /// 0x460
			 0x1b73cb92, /// 0x464
			 0xc3566d70, /// 0x468
			 0xad6008bc, /// 0x46c
			 0xa32014eb, /// 0x470
			 0xab0bb0cb, /// 0x474
			 0x0f1214a9, /// 0x478
			 0xfdf1c40e, /// 0x47c
			 0x64f859e1, /// 0x480
			 0xd2a4043e, /// 0x484
			 0x184026c5, /// 0x488
			 0x4da86f3d, /// 0x48c
			 0x7290298b, /// 0x490
			 0xf8b0c4c3, /// 0x494
			 0xc7b15725, /// 0x498
			 0x8fe35497, /// 0x49c
			 0xc9f57a02, /// 0x4a0
			 0x0e1cec1e, /// 0x4a4
			 0xe79306f8, /// 0x4a8
			 0xe637f61e, /// 0x4ac
			 0x7ab952b4, /// 0x4b0
			 0x4a9e2d4b, /// 0x4b4
			 0xca63433c, /// 0x4b8
			 0x3c2b3b7b, /// 0x4bc
			 0x5b567e7a, /// 0x4c0
			 0x4330e821, /// 0x4c4
			 0x9d8372ed, /// 0x4c8
			 0x1d852b7e, /// 0x4cc
			 0xd933b551, /// 0x4d0
			 0x6afd03bb, /// 0x4d4
			 0xdec3266f, /// 0x4d8
			 0x5612dae0, /// 0x4dc
			 0xe2c6ee75, /// 0x4e0
			 0x453a7424, /// 0x4e4
			 0xcc2dafe6, /// 0x4e8
			 0xc11f69e7, /// 0x4ec
			 0xaa61ff72, /// 0x4f0
			 0x012ce15f, /// 0x4f4
			 0xb057310e, /// 0x4f8
			 0x8b0c081f, /// 0x4fc
			 0x414968d2, /// 0x500
			 0x358fb54e, /// 0x504
			 0x4f97219e, /// 0x508
			 0x41063c5f, /// 0x50c
			 0xe020585a, /// 0x510
			 0xab610e23, /// 0x514
			 0x1a09998e, /// 0x518
			 0x9a18057e, /// 0x51c
			 0xad20672a, /// 0x520
			 0xc42b9350, /// 0x524
			 0xdf1dd36f, /// 0x528
			 0x73d9e5d5, /// 0x52c
			 0xa3e0b9a8, /// 0x530
			 0x1aa58014, /// 0x534
			 0x1ebcfeea, /// 0x538
			 0xc9180024, /// 0x53c
			 0xb0de8cd5, /// 0x540
			 0xbc7f54c0, /// 0x544
			 0x7a392282, /// 0x548
			 0xd9351cbd, /// 0x54c
			 0xf2ca6aa4, /// 0x550
			 0x15bc1293, /// 0x554
			 0x9ad05707, /// 0x558
			 0xcd895c01, /// 0x55c
			 0x53179504, /// 0x560
			 0xbb501eb5, /// 0x564
			 0x3b1edbc1, /// 0x568
			 0x528cdf23, /// 0x56c
			 0x1818f3c4, /// 0x570
			 0x714cddf4, /// 0x574
			 0xf9ca0f32, /// 0x578
			 0x03d0b3d1, /// 0x57c
			 0x554ee320, /// 0x580
			 0x045ca29e, /// 0x584
			 0x54c1f67a, /// 0x588
			 0xa93cc793, /// 0x58c
			 0x5caab7ce, /// 0x590
			 0x971fa6be, /// 0x594
			 0xbc859ffb, /// 0x598
			 0xeebe7059, /// 0x59c
			 0x4c9e842a, /// 0x5a0
			 0xdf25af63, /// 0x5a4
			 0x2ada4dbf, /// 0x5a8
			 0x08981da3, /// 0x5ac
			 0xef402eb0, /// 0x5b0
			 0x7714d9f6, /// 0x5b4
			 0x0e3791d1, /// 0x5b8
			 0x80d6b668, /// 0x5bc
			 0x13f245ef, /// 0x5c0
			 0x40774f6e, /// 0x5c4
			 0xf1d7228c, /// 0x5c8
			 0xb3cb0449, /// 0x5cc
			 0x81f2053f, /// 0x5d0
			 0x44fd585c, /// 0x5d4
			 0x64e43fab, /// 0x5d8
			 0x556f414a, /// 0x5dc
			 0xcc789f07, /// 0x5e0
			 0x6e685e8a, /// 0x5e4
			 0xf49e5a80, /// 0x5e8
			 0x10becb9a, /// 0x5ec
			 0xc285c578, /// 0x5f0
			 0xb3789af2, /// 0x5f4
			 0xeb96c58d, /// 0x5f8
			 0x21de5136, /// 0x5fc
			 0x36e00584, /// 0x600
			 0xdcab85e3, /// 0x604
			 0x3ed6a0e2, /// 0x608
			 0x8242a2dc, /// 0x60c
			 0xa74d7e41, /// 0x610
			 0x1186a531, /// 0x614
			 0x3e46a31a, /// 0x618
			 0x7fd45ed9, /// 0x61c
			 0xb359f324, /// 0x620
			 0x423f2016, /// 0x624
			 0x27c5793d, /// 0x628
			 0x6d3c952e, /// 0x62c
			 0xbffd4dae, /// 0x630
			 0x2fff95bb, /// 0x634
			 0x24d048fc, /// 0x638
			 0x5bb63d73, /// 0x63c
			 0xb019c3ad, /// 0x640
			 0x1b5332ef, /// 0x644
			 0xc26cd8b2, /// 0x648
			 0xef39ffac, /// 0x64c
			 0x153073ff, /// 0x650
			 0xeb920a78, /// 0x654
			 0x85b3d6cf, /// 0x658
			 0x18064685, /// 0x65c
			 0x0e165b30, /// 0x660
			 0xe13a2bb8, /// 0x664
			 0x5559d90a, /// 0x668
			 0x1e7f2871, /// 0x66c
			 0x7c51b1d2, /// 0x670
			 0x830aab12, /// 0x674
			 0xc02faf46, /// 0x678
			 0x79c0f2f6, /// 0x67c
			 0xc28a6c28, /// 0x680
			 0x4ca9e7b8, /// 0x684
			 0x20f16966, /// 0x688
			 0x11cfa3ff, /// 0x68c
			 0xb99397b5, /// 0x690
			 0x8165264e, /// 0x694
			 0xfe2b8e3b, /// 0x698
			 0x406f8e15, /// 0x69c
			 0x8cc44d4f, /// 0x6a0
			 0xf662c0c3, /// 0x6a4
			 0x09591278, /// 0x6a8
			 0xcde86ebb, /// 0x6ac
			 0x86a2b780, /// 0x6b0
			 0x6abc6224, /// 0x6b4
			 0xf55a871b, /// 0x6b8
			 0x4a7f5c29, /// 0x6bc
			 0x972c5fb4, /// 0x6c0
			 0x141b6092, /// 0x6c4
			 0xed852d7f, /// 0x6c8
			 0xcea7ac35, /// 0x6cc
			 0xfc07fc6a, /// 0x6d0
			 0x238c9da7, /// 0x6d4
			 0x6068b23a, /// 0x6d8
			 0x4442cc10, /// 0x6dc
			 0x518ca277, /// 0x6e0
			 0xd7736693, /// 0x6e4
			 0x4e386023, /// 0x6e8
			 0x9df1d024, /// 0x6ec
			 0x8b97338f, /// 0x6f0
			 0x207f2d43, /// 0x6f4
			 0xa8cace36, /// 0x6f8
			 0x9a5d307a, /// 0x6fc
			 0xb8dd207a, /// 0x700
			 0x9f86a4a1, /// 0x704
			 0xd1a96370, /// 0x708
			 0xa158aaff, /// 0x70c
			 0x3db7453f, /// 0x710
			 0x40401d05, /// 0x714
			 0x3e593984, /// 0x718
			 0xe78c347b, /// 0x71c
			 0x3558f002, /// 0x720
			 0x3ebb5aaf, /// 0x724
			 0x5d650f74, /// 0x728
			 0x88ef60ee, /// 0x72c
			 0x2574182f, /// 0x730
			 0x60ba8077, /// 0x734
			 0xabdbbd32, /// 0x738
			 0xfd584476, /// 0x73c
			 0x2fa910a9, /// 0x740
			 0x04e0fea2, /// 0x744
			 0x3b47dd61, /// 0x748
			 0xdd2d86c9, /// 0x74c
			 0xee0730e6, /// 0x750
			 0xe8046894, /// 0x754
			 0x1bb69b4e, /// 0x758
			 0xccf8c58e, /// 0x75c
			 0xfcfb047d, /// 0x760
			 0xe789d7a2, /// 0x764
			 0xde3e15c7, /// 0x768
			 0x1dda657c, /// 0x76c
			 0x5ee1afe2, /// 0x770
			 0x4490a131, /// 0x774
			 0x3ada9bc4, /// 0x778
			 0xd84070f1, /// 0x77c
			 0x0966ef11, /// 0x780
			 0x9799f62b, /// 0x784
			 0xeaa1b128, /// 0x788
			 0x68ebab28, /// 0x78c
			 0xf3dae366, /// 0x790
			 0x9f4d4340, /// 0x794
			 0x834b5e27, /// 0x798
			 0xec3d4b46, /// 0x79c
			 0x901d56bc, /// 0x7a0
			 0xeada7a28, /// 0x7a4
			 0x5e9f72c9, /// 0x7a8
			 0xbf94e635, /// 0x7ac
			 0x0502ad9c, /// 0x7b0
			 0x5a51d15b, /// 0x7b4
			 0xcbe5d63d, /// 0x7b8
			 0x9a28a0ad, /// 0x7bc
			 0x4179a93e, /// 0x7c0
			 0x104ce7dd, /// 0x7c4
			 0x911656f2, /// 0x7c8
			 0x194e6f75, /// 0x7cc
			 0xd13ca39e, /// 0x7d0
			 0x77cbac56, /// 0x7d4
			 0xfc262889, /// 0x7d8
			 0x92216edc, /// 0x7dc
			 0x3f0038b1, /// 0x7e0
			 0x73b60a5b, /// 0x7e4
			 0x78ff6a4c, /// 0x7e8
			 0xb1558562, /// 0x7ec
			 0xec9efb3b, /// 0x7f0
			 0xd54ae152, /// 0x7f4
			 0x0ee37b22, /// 0x7f8
			 0xac1f9ec8, /// 0x7fc
			 0xd005d1e8, /// 0x800
			 0xc51a3403, /// 0x804
			 0xb621ad53, /// 0x808
			 0x2cb689a2, /// 0x80c
			 0xfb3acf39, /// 0x810
			 0xa2447811, /// 0x814
			 0x4408f8d5, /// 0x818
			 0x6e187584, /// 0x81c
			 0x6f00a935, /// 0x820
			 0xaf4eae9a, /// 0x824
			 0xdb3bcd2a, /// 0x828
			 0x0e444177, /// 0x82c
			 0x7f17a596, /// 0x830
			 0x89625ee2, /// 0x834
			 0xe8922d4f, /// 0x838
			 0xcf6cdf8f, /// 0x83c
			 0x4812d456, /// 0x840
			 0xb0e9e56d, /// 0x844
			 0xeda5d8fe, /// 0x848
			 0xcb53f49d, /// 0x84c
			 0xe7fe75b7, /// 0x850
			 0x1edbd051, /// 0x854
			 0x3b767792, /// 0x858
			 0xd7ec4523, /// 0x85c
			 0xd0a185a8, /// 0x860
			 0x1bffea95, /// 0x864
			 0x5cdc1839, /// 0x868
			 0x81e57d12, /// 0x86c
			 0x95c9309f, /// 0x870
			 0x40658149, /// 0x874
			 0x05110d3d, /// 0x878
			 0xbf9ff9b2, /// 0x87c
			 0xf2ebdd10, /// 0x880
			 0x88d74c54, /// 0x884
			 0xf8e9d2d0, /// 0x888
			 0x9c87e08a, /// 0x88c
			 0x23773f07, /// 0x890
			 0x41e64647, /// 0x894
			 0x43f4d2fb, /// 0x898
			 0xa0453e79, /// 0x89c
			 0xe293c59d, /// 0x8a0
			 0xa30bc552, /// 0x8a4
			 0x3d996f6f, /// 0x8a8
			 0x79a73497, /// 0x8ac
			 0x018276e0, /// 0x8b0
			 0x0766c9d3, /// 0x8b4
			 0x1624ec98, /// 0x8b8
			 0xddcc93fc, /// 0x8bc
			 0x7cb9466a, /// 0x8c0
			 0xc4c95a9b, /// 0x8c4
			 0x1e38b93a, /// 0x8c8
			 0x808e7fa7, /// 0x8cc
			 0x4cf26dc5, /// 0x8d0
			 0xa91dc065, /// 0x8d4
			 0x311dc130, /// 0x8d8
			 0x0f0757c7, /// 0x8dc
			 0x7160fe66, /// 0x8e0
			 0x6902b430, /// 0x8e4
			 0x53cf08b9, /// 0x8e8
			 0x4514097a, /// 0x8ec
			 0xcc37ce0b, /// 0x8f0
			 0x2341ae43, /// 0x8f4
			 0x419bbdae, /// 0x8f8
			 0xfdd9635c, /// 0x8fc
			 0x0e44c872, /// 0x900
			 0xb9740fae, /// 0x904
			 0x8be86ed7, /// 0x908
			 0x16336ded, /// 0x90c
			 0xbc7b2397, /// 0x910
			 0xdd0ac851, /// 0x914
			 0xd0e3912a, /// 0x918
			 0x2baf000e, /// 0x91c
			 0x9e8e3b8b, /// 0x920
			 0x8e2e2ccf, /// 0x924
			 0xe75b4e30, /// 0x928
			 0x5b36d81c, /// 0x92c
			 0xd4549b37, /// 0x930
			 0x2bb9ba8e, /// 0x934
			 0x326595d6, /// 0x938
			 0xad28acb5, /// 0x93c
			 0xe212843f, /// 0x940
			 0x06c2069b, /// 0x944
			 0xd67d3828, /// 0x948
			 0x9d0ce6d6, /// 0x94c
			 0x51ec999d, /// 0x950
			 0xe0202296, /// 0x954
			 0xd0de143a, /// 0x958
			 0x4372ec14, /// 0x95c
			 0x3039e3ed, /// 0x960
			 0xa3146f1b, /// 0x964
			 0x9fc4312c, /// 0x968
			 0xcfa8d9d7, /// 0x96c
			 0x13d8d8fd, /// 0x970
			 0xc78132e7, /// 0x974
			 0x533383db, /// 0x978
			 0xa14481f6, /// 0x97c
			 0xb29086ea, /// 0x980
			 0x1d919039, /// 0x984
			 0x76d82544, /// 0x988
			 0xc87aec91, /// 0x98c
			 0x7f76d676, /// 0x990
			 0x3ef104dc, /// 0x994
			 0x4e83c85d, /// 0x998
			 0xd1b0d802, /// 0x99c
			 0x5989c677, /// 0x9a0
			 0xe3cea69e, /// 0x9a4
			 0x6c05bf47, /// 0x9a8
			 0xbadf6aa5, /// 0x9ac
			 0xcce1ae95, /// 0x9b0
			 0xdef4fca6, /// 0x9b4
			 0x2f15e970, /// 0x9b8
			 0x229b4fd8, /// 0x9bc
			 0x87e7d0f6, /// 0x9c0
			 0x98f45817, /// 0x9c4
			 0x779e52b1, /// 0x9c8
			 0x74041ca0, /// 0x9cc
			 0x95514fb1, /// 0x9d0
			 0x34d61a49, /// 0x9d4
			 0x6363b446, /// 0x9d8
			 0xa9c17649, /// 0x9dc
			 0x3c7130e6, /// 0x9e0
			 0x2a7dca29, /// 0x9e4
			 0xa1bf070f, /// 0x9e8
			 0xf2488df8, /// 0x9ec
			 0x31cc161d, /// 0x9f0
			 0x3534d098, /// 0x9f4
			 0xe1a3117a, /// 0x9f8
			 0x02e2563c, /// 0x9fc
			 0x9c260a4b, /// 0xa00
			 0x3d0eaceb, /// 0xa04
			 0xbb066ed8, /// 0xa08
			 0x5cde760e, /// 0xa0c
			 0xd57fa8cb, /// 0xa10
			 0xc01ea405, /// 0xa14
			 0x8d2e8b67, /// 0xa18
			 0x3a0b29cf, /// 0xa1c
			 0xb2430a05, /// 0xa20
			 0x2e80b8d5, /// 0xa24
			 0xb588b1f1, /// 0xa28
			 0x3e83df88, /// 0xa2c
			 0x1f7ac601, /// 0xa30
			 0x2c02c69b, /// 0xa34
			 0xec091c99, /// 0xa38
			 0xd70de70b, /// 0xa3c
			 0xdf9a7b54, /// 0xa40
			 0xa68e9461, /// 0xa44
			 0xcd6e32e9, /// 0xa48
			 0xa9dc859d, /// 0xa4c
			 0xc7ceb5c7, /// 0xa50
			 0x6874ad91, /// 0xa54
			 0xf69a39b3, /// 0xa58
			 0xe9a5561c, /// 0xa5c
			 0x856d82d2, /// 0xa60
			 0x4787bf48, /// 0xa64
			 0x975f696f, /// 0xa68
			 0x97c7e473, /// 0xa6c
			 0x0c68fa5a, /// 0xa70
			 0x1bff82f4, /// 0xa74
			 0x897d67cc, /// 0xa78
			 0xb90e22e5, /// 0xa7c
			 0xe907e409, /// 0xa80
			 0xc0205a9c, /// 0xa84
			 0x9abad3f4, /// 0xa88
			 0xc4df4577, /// 0xa8c
			 0xa760b803, /// 0xa90
			 0xb80481ce, /// 0xa94
			 0x57b6d541, /// 0xa98
			 0x60f10ac1, /// 0xa9c
			 0xacd5a9be, /// 0xaa0
			 0xa356e79c, /// 0xaa4
			 0x5e024f04, /// 0xaa8
			 0x97de1a35, /// 0xaac
			 0x7539caf8, /// 0xab0
			 0x61b47951, /// 0xab4
			 0x06399a13, /// 0xab8
			 0x049d5f50, /// 0xabc
			 0xd1e5e893, /// 0xac0
			 0xf66eef22, /// 0xac4
			 0xe7ab369d, /// 0xac8
			 0xc8851f98, /// 0xacc
			 0x9f587e2f, /// 0xad0
			 0xa0da2063, /// 0xad4
			 0x290f035b, /// 0xad8
			 0x9a0a93ca, /// 0xadc
			 0x60663e0c, /// 0xae0
			 0x7f69c3bc, /// 0xae4
			 0x69524f7d, /// 0xae8
			 0xff3154b9, /// 0xaec
			 0x6eac9a01, /// 0xaf0
			 0x3acd7ac7, /// 0xaf4
			 0x128012d2, /// 0xaf8
			 0xcd1bc6b9, /// 0xafc
			 0xe1e6dfca, /// 0xb00
			 0x65002158, /// 0xb04
			 0x5c9a9545, /// 0xb08
			 0xc84dc44e, /// 0xb0c
			 0x0795e364, /// 0xb10
			 0x7ff56742, /// 0xb14
			 0x177d3f11, /// 0xb18
			 0x85053da8, /// 0xb1c
			 0xc5925a10, /// 0xb20
			 0xf93bda16, /// 0xb24
			 0xe7dfd9e7, /// 0xb28
			 0x0d132c67, /// 0xb2c
			 0xf8ad88f8, /// 0xb30
			 0xb35cf17a, /// 0xb34
			 0xfee3e69f, /// 0xb38
			 0x331c76e7, /// 0xb3c
			 0x15c72985, /// 0xb40
			 0x32fe1d0c, /// 0xb44
			 0x90d9ec6f, /// 0xb48
			 0x1df4a191, /// 0xb4c
			 0x82c54d7f, /// 0xb50
			 0x2806ad28, /// 0xb54
			 0xd8d672ab, /// 0xb58
			 0x4f757e28, /// 0xb5c
			 0x7df3ddbb, /// 0xb60
			 0xbc701e82, /// 0xb64
			 0x1cd3a0f9, /// 0xb68
			 0x342be047, /// 0xb6c
			 0x86e7b40e, /// 0xb70
			 0x26f05b73, /// 0xb74
			 0xeb646584, /// 0xb78
			 0x3923fddc, /// 0xb7c
			 0x8e43d248, /// 0xb80
			 0x092cb8d3, /// 0xb84
			 0xa8ffc897, /// 0xb88
			 0x55371a31, /// 0xb8c
			 0x4f1c2525, /// 0xb90
			 0xa2c75406, /// 0xb94
			 0x4c86db3a, /// 0xb98
			 0x6a4349ba, /// 0xb9c
			 0x9d9fa554, /// 0xba0
			 0x959e74ff, /// 0xba4
			 0xd7c43ca4, /// 0xba8
			 0x11bfce89, /// 0xbac
			 0xca5a30a1, /// 0xbb0
			 0x257e3cab, /// 0xbb4
			 0xad085ff1, /// 0xbb8
			 0xd30b7fda, /// 0xbbc
			 0x17ed5ac0, /// 0xbc0
			 0xc94932d3, /// 0xbc4
			 0xdc807216, /// 0xbc8
			 0x43f873b1, /// 0xbcc
			 0xfcf11afa, /// 0xbd0
			 0xa7b7885a, /// 0xbd4
			 0xa42bdd7b, /// 0xbd8
			 0xbf66e26b, /// 0xbdc
			 0xa611ef87, /// 0xbe0
			 0xdf6793bc, /// 0xbe4
			 0x20404352, /// 0xbe8
			 0x8b54498a, /// 0xbec
			 0x896fff15, /// 0xbf0
			 0xa50214a5, /// 0xbf4
			 0x5348b67f, /// 0xbf8
			 0xfbee75a3, /// 0xbfc
			 0xb83a85f3, /// 0xc00
			 0x09a115a7, /// 0xc04
			 0xe0ea4576, /// 0xc08
			 0x392fa16f, /// 0xc0c
			 0xcf8f1c35, /// 0xc10
			 0x23e1a311, /// 0xc14
			 0xf6174494, /// 0xc18
			 0x3cefaf4c, /// 0xc1c
			 0x82064fbd, /// 0xc20
			 0x439de44a, /// 0xc24
			 0x8d1b2f20, /// 0xc28
			 0x14898135, /// 0xc2c
			 0x3ec00298, /// 0xc30
			 0x70a63588, /// 0xc34
			 0xc4d2196b, /// 0xc38
			 0x87e7c644, /// 0xc3c
			 0xf12c196e, /// 0xc40
			 0x7f020583, /// 0xc44
			 0xfe81b54d, /// 0xc48
			 0x9cc26339, /// 0xc4c
			 0xcd807404, /// 0xc50
			 0xb6bae98d, /// 0xc54
			 0x7f851b7f, /// 0xc58
			 0x2f673f63, /// 0xc5c
			 0x0a915182, /// 0xc60
			 0x810841c9, /// 0xc64
			 0x3bb7661d, /// 0xc68
			 0x36429d51, /// 0xc6c
			 0x7c88e95a, /// 0xc70
			 0x1d1576d5, /// 0xc74
			 0xfc53bc1c, /// 0xc78
			 0x2970b1fb, /// 0xc7c
			 0x07637fef, /// 0xc80
			 0x219ab221, /// 0xc84
			 0x0b828cd5, /// 0xc88
			 0xa2fd7f44, /// 0xc8c
			 0xc7c43b6d, /// 0xc90
			 0x9854e0e1, /// 0xc94
			 0xe1dc7d11, /// 0xc98
			 0xfada86b1, /// 0xc9c
			 0x21b29768, /// 0xca0
			 0xf11993a2, /// 0xca4
			 0x5aa8db8e, /// 0xca8
			 0x624f057f, /// 0xcac
			 0x6c4a4a39, /// 0xcb0
			 0x28a3b7cf, /// 0xcb4
			 0xa70cdcde, /// 0xcb8
			 0xe32393fd, /// 0xcbc
			 0x37784905, /// 0xcc0
			 0x06f76f59, /// 0xcc4
			 0xcfde96ec, /// 0xcc8
			 0x68b7b60d, /// 0xccc
			 0x4fa81f05, /// 0xcd0
			 0xf8016056, /// 0xcd4
			 0x977128a0, /// 0xcd8
			 0x8cb7a6ce, /// 0xcdc
			 0x5e38ca5e, /// 0xce0
			 0x598afb00, /// 0xce4
			 0xe94d54e9, /// 0xce8
			 0xc0e5b504, /// 0xcec
			 0xb3867bbc, /// 0xcf0
			 0xbd8db9e3, /// 0xcf4
			 0x08e08476, /// 0xcf8
			 0xa772fbba, /// 0xcfc
			 0xf7b9bd8c, /// 0xd00
			 0xa79bed3e, /// 0xd04
			 0x071c519a, /// 0xd08
			 0xfce296d5, /// 0xd0c
			 0x6840eac0, /// 0xd10
			 0x4996cd2e, /// 0xd14
			 0xf6af0931, /// 0xd18
			 0x2b997189, /// 0xd1c
			 0xe43a7cf5, /// 0xd20
			 0x910c948f, /// 0xd24
			 0x9188918a, /// 0xd28
			 0xfb6d61a5, /// 0xd2c
			 0xdbb6cdc1, /// 0xd30
			 0x9bfdcfb4, /// 0xd34
			 0x38492cd9, /// 0xd38
			 0x25c476e3, /// 0xd3c
			 0x721ee74c, /// 0xd40
			 0x72e30a7a, /// 0xd44
			 0x503c97a4, /// 0xd48
			 0x939b7ff9, /// 0xd4c
			 0xed7021b2, /// 0xd50
			 0x7e589e1e, /// 0xd54
			 0xaac69a22, /// 0xd58
			 0x5897d918, /// 0xd5c
			 0xc929f7e7, /// 0xd60
			 0x0f7937be, /// 0xd64
			 0xfee33c57, /// 0xd68
			 0xe8c6d5ec, /// 0xd6c
			 0xe7f26350, /// 0xd70
			 0x673c211e, /// 0xd74
			 0x9c70ec3a, /// 0xd78
			 0x256f9560, /// 0xd7c
			 0xd9260810, /// 0xd80
			 0x7a80b4b1, /// 0xd84
			 0x8e37d0d8, /// 0xd88
			 0x0b76c251, /// 0xd8c
			 0x46da18c1, /// 0xd90
			 0x817e845e, /// 0xd94
			 0x11a38c3d, /// 0xd98
			 0x1e4cf58f, /// 0xd9c
			 0x1f2c3743, /// 0xda0
			 0xab4cc9a2, /// 0xda4
			 0x7a440ddb, /// 0xda8
			 0x04edf2ed, /// 0xdac
			 0xd137be69, /// 0xdb0
			 0x2e29e77b, /// 0xdb4
			 0x241017ff, /// 0xdb8
			 0x695114a6, /// 0xdbc
			 0xd9807c6e, /// 0xdc0
			 0x704259db, /// 0xdc4
			 0xb40eea50, /// 0xdc8
			 0x02760267, /// 0xdcc
			 0x56591c0c, /// 0xdd0
			 0x426b9983, /// 0xdd4
			 0x4da925a5, /// 0xdd8
			 0x3e88974f, /// 0xddc
			 0xa59d1f52, /// 0xde0
			 0xd55f773d, /// 0xde4
			 0xc66dbc2a, /// 0xde8
			 0xd17c4488, /// 0xdec
			 0xff853602, /// 0xdf0
			 0xadaeb03b, /// 0xdf4
			 0x4b7774d0, /// 0xdf8
			 0xf8503d3a, /// 0xdfc
			 0xce057da7, /// 0xe00
			 0x6473238d, /// 0xe04
			 0xc990c59c, /// 0xe08
			 0xb7601505, /// 0xe0c
			 0x8c2574dd, /// 0xe10
			 0xac3ed764, /// 0xe14
			 0xe00def15, /// 0xe18
			 0x97b709aa, /// 0xe1c
			 0x2473f584, /// 0xe20
			 0x47b8607a, /// 0xe24
			 0x64b02257, /// 0xe28
			 0x18174487, /// 0xe2c
			 0x0e4c210a, /// 0xe30
			 0xbb2f5714, /// 0xe34
			 0x222ef833, /// 0xe38
			 0x3754c48b, /// 0xe3c
			 0xd57325ae, /// 0xe40
			 0x2fa09de6, /// 0xe44
			 0xb817e679, /// 0xe48
			 0x3c999173, /// 0xe4c
			 0x15404cc0, /// 0xe50
			 0x530bb001, /// 0xe54
			 0x87ae03e9, /// 0xe58
			 0xeb343fe5, /// 0xe5c
			 0xce77e73f, /// 0xe60
			 0x1729e421, /// 0xe64
			 0x3c4ef051, /// 0xe68
			 0x812f168f, /// 0xe6c
			 0x2ea462f1, /// 0xe70
			 0xc49ef370, /// 0xe74
			 0x838f1868, /// 0xe78
			 0x3b6479f6, /// 0xe7c
			 0x36fb5caa, /// 0xe80
			 0x98092d6a, /// 0xe84
			 0x144c4f4e, /// 0xe88
			 0x5e7a01c2, /// 0xe8c
			 0x8e94201e, /// 0xe90
			 0xa053d860, /// 0xe94
			 0xd889cd22, /// 0xe98
			 0xad328a40, /// 0xe9c
			 0xc2786f2e, /// 0xea0
			 0x54ecade1, /// 0xea4
			 0xc3e0494b, /// 0xea8
			 0xbd4430f6, /// 0xeac
			 0x525fdabe, /// 0xeb0
			 0x21d16fe9, /// 0xeb4
			 0x37c01174, /// 0xeb8
			 0xe4d20623, /// 0xebc
			 0xbe2e1ef1, /// 0xec0
			 0xcc322877, /// 0xec4
			 0x2c5eadbd, /// 0xec8
			 0xd14ff1f2, /// 0xecc
			 0x9a68c6be, /// 0xed0
			 0x019c4a76, /// 0xed4
			 0x6e300f35, /// 0xed8
			 0x7886d8e8, /// 0xedc
			 0xe007622f, /// 0xee0
			 0x83ef3917, /// 0xee4
			 0x717d2b17, /// 0xee8
			 0x074a5416, /// 0xeec
			 0x96aa9210, /// 0xef0
			 0xaf76c8e7, /// 0xef4
			 0xbdff28c7, /// 0xef8
			 0x714fb87f, /// 0xefc
			 0xa0174d7f, /// 0xf00
			 0xf3ba62c7, /// 0xf04
			 0xcf304c79, /// 0xf08
			 0x8e03f739, /// 0xf0c
			 0xb04ae280, /// 0xf10
			 0xff0edd3a, /// 0xf14
			 0x58f00ef9, /// 0xf18
			 0xe8384d04, /// 0xf1c
			 0x0a62e78a, /// 0xf20
			 0x9a1ab98d, /// 0xf24
			 0x09d4189e, /// 0xf28
			 0xa64d54a6, /// 0xf2c
			 0x9315e7c3, /// 0xf30
			 0x92a43bfa, /// 0xf34
			 0x86a2c0f7, /// 0xf38
			 0xe800ad8d, /// 0xf3c
			 0x5f8ebcbd, /// 0xf40
			 0x3c1cf6fa, /// 0xf44
			 0xe5ede08e, /// 0xf48
			 0xa3141f3f, /// 0xf4c
			 0x75a01200, /// 0xf50
			 0xa849cef2, /// 0xf54
			 0x81080f0c, /// 0xf58
			 0x8490473d, /// 0xf5c
			 0xff78c324, /// 0xf60
			 0x5448f7e0, /// 0xf64
			 0xcc79fe4f, /// 0xf68
			 0x888fc7bf, /// 0xf6c
			 0xfd584713, /// 0xf70
			 0x168c0e5a, /// 0xf74
			 0xc682f394, /// 0xf78
			 0x318dd31f, /// 0xf7c
			 0x57f0ce89, /// 0xf80
			 0xb8b88f0c, /// 0xf84
			 0x731693d7, /// 0xf88
			 0xbc734853, /// 0xf8c
			 0x4d5e825d, /// 0xf90
			 0x26796910, /// 0xf94
			 0x3e4f1437, /// 0xf98
			 0x95c9abe8, /// 0xf9c
			 0x5bfc48ef, /// 0xfa0
			 0xba978f09, /// 0xfa4
			 0x6b44973f, /// 0xfa8
			 0x3fcbf65d, /// 0xfac
			 0x70c6a8c6, /// 0xfb0
			 0x0cc8af4c, /// 0xfb4
			 0xe4340222, /// 0xfb8
			 0xa8637c7c, /// 0xfbc
			 0x2b216060, /// 0xfc0
			 0xa45bf15b, /// 0xfc4
			 0x60fad654, /// 0xfc8
			 0xcee2fd77, /// 0xfcc
			 0x24fec9fb, /// 0xfd0
			 0x0e8ec002, /// 0xfd4
			 0x51c45fd7, /// 0xfd8
			 0x93ece203, /// 0xfdc
			 0x0cae41d7, /// 0xfe0
			 0x02a7c0e1, /// 0xfe4
			 0x8ab691ae, /// 0xfe8
			 0x0aa6704c, /// 0xfec
			 0xf65f7fb1, /// 0xff0
			 0x03c45ac2, /// 0xff4
			 0x3c3f2a30, /// 0xff8
			 0x72e18ad3 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00000
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00004
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00010
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00014
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00018
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00020
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00024
			 0x00011111,                                                  // 9.7958E-41                                  /// 00028
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0002c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00030
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00038
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00040
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00044
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00048
			 0x7f800000,                                                  // inf                                         /// 0004c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00050
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00058
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0005c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00060
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00070
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00074
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00078
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0007c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00080
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00084
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0008c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00090
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00094
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00098
			 0x0c400000,                                                  // Leading 1s:                                 /// 0009c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00100
			 0x0e000001,                                                  // Trailing 1s:                                /// 00104
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0010c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00110
			 0x0e000001,                                                  // Trailing 1s:                                /// 00114
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00118
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00120
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00124
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0012c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00130
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00134
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00140
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00144
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00148
			 0xff800000,                                                  // -inf                                        /// 0014c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00150
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00154
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00158
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00160
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00164
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00180
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0018c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00190
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00194
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00198
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 001c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x7fc00001,                                                  // signaling NaN                               /// 001fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00200
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00214
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0021c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00220
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00228
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00238
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00240
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0024c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00250
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00258
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0025c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00260
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00264
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00268
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00280
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0028c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00290
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00294
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0029c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x7f800000,                                                  // inf                                         /// 00300
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00308
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00314
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0031c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00324
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00328
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0032c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00330
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00334
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0033c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f800000,                                                  // inf                                         /// 00344
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00348
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0034c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00354
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00358
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00360
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00364
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00368
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0036c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00370
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00378
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0037c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00388
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0038c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00390
			 0x0c780000,                                                  // Leading 1s:                                 /// 00394
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00398
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x55555555,                                                  // 4 random values                             /// 003a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00404
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00408
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00414
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00418
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0041c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00420
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00424
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00428
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0042c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00430
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0043c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0c600000,                                                  // Leading 1s:                                 /// 00444
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00448
			 0x80000000,                                                  // -zero                                       /// 0044c
			 0x00000000,                                                  // zero                                        /// 00450
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00454
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00458
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00464
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00468
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0046c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00470
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0047c
			 0xcb000000,                                                  // -8388608.0                                  /// 00480
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00484
			 0x80011111,                                                  // -9.7958E-41                                 /// 00488
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0048c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00490
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00494
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00498
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0049c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 004c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x4b000000,                                                  // 8388608.0                                   /// 004d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00500
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00504
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00508
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0050c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00514
			 0x80011111,                                                  // -9.7958E-41                                 /// 00518
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00520
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00524
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00528
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0052c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00530
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00540
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0054c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00550
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00554
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00558
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0055c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00570
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00574
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00580
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00584
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0058c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00598
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x7fc00001,                                                  // signaling NaN                               /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005dc
			 0x3f028f5c,                                                  // 0.51                                        /// 005e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00600
			 0x0c600000,                                                  // Leading 1s:                                 /// 00604
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00608
			 0x0c700000,                                                  // Leading 1s:                                 /// 0060c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00610
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00614
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00618
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0061c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00624
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00628
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0062c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00630
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00634
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00638
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0063c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00644
			 0x0c600000,                                                  // Leading 1s:                                 /// 00648
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00658
			 0x3f028f5c,                                                  // 0.51                                        /// 0065c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00664
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00668
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0066c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00670
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0c400000,                                                  // Leading 1s:                                 /// 0067c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00680
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00684
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00688
			 0xbf028f5c,                                                  // -0.51                                       /// 0068c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00690
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00694
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0xffc00001,                                                  // -signaling NaN                              /// 006a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00704
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00710
			 0x7fc00001,                                                  // signaling NaN                               /// 00714
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00728
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0072c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00730
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00734
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x4b000000,                                                  // 8388608.0                                   /// 0073c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00744
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00748
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0074c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00750
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00754
			 0x7fc00001,                                                  // signaling NaN                               /// 00758
			 0x0c700000,                                                  // Leading 1s:                                 /// 0075c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00760
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00764
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00778
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00780
			 0x33333333,                                                  // 4 random values                             /// 00784
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00788
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0078c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xff800000,                                                  // -inf                                        /// 007c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f8
			 0x55555555,                                                  // 4 random values                             /// 007fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00800
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00804
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00814
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00820
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00824
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00838
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0083c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00840
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0084c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00850
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00858
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0085c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00860
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00874
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0087c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00880
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00890
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00894
			 0x0c700000,                                                  // Leading 1s:                                 /// 00898
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xffc00001,                                                  // -signaling NaN                              /// 008a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b4
			 0x33333333,                                                  // 4 random values                             /// 008b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80000000,                                                  // -zero                                       /// 008c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f0
			 0x33333333,                                                  // 4 random values                             /// 008f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xbf028f5c,                                                  // -0.51                                       /// 008fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00900
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00910
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00914
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00918
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0091c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00920
			 0x0c700000,                                                  // Leading 1s:                                 /// 00924
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e000001,                                                  // Trailing 1s:                                /// 0092c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00934
			 0x0c700000,                                                  // Leading 1s:                                 /// 00938
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0093c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0094c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00950
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00954
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0095c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00960
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0xbf028f5c,                                                  // -0.51                                       /// 00968
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00974
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00978
			 0x0c700000,                                                  // Leading 1s:                                 /// 0097c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00984
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00988
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00998
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0099c
			 0x55555555,                                                  // 4 random values                             /// 009a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x3f028f5c,                                                  // 0.51                                        /// 009d8
			 0xffc00001,                                                  // -signaling NaN                              /// 009dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xffc00001,                                                  // -signaling NaN                              /// 009f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a38
			 0xffc00001,                                                  // -signaling NaN                              /// 00a3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a64
			 0x3f028f5c,                                                  // 0.51                                        /// 00a68
			 0x3f028f5c,                                                  // 0.51                                        /// 00a6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a9c
			 0x80000000,                                                  // -zero                                       /// 00aa0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x7f800000,                                                  // inf                                         /// 00aac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00abc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x80000000,                                                  // -zero                                       /// 00ac8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00acc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00adc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae8
			 0x80000000,                                                  // -zero                                       /// 00aec
			 0xffc00001,                                                  // -signaling NaN                              /// 00af0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00af8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00afc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0xff800000,                                                  // -inf                                        /// 00b18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b4c
			 0x7f800000,                                                  // inf                                         /// 00b50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x3f028f5c,                                                  // 0.51                                        /// 00b74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ba8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bc8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00be0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00be8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bf0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bfc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c18
			 0x7f800000,                                                  // inf                                         /// 00c1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0xbf028f5c,                                                  // -0.51                                       /// 00c2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c58
			 0xbf028f5c,                                                  // -0.51                                       /// 00c5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c7c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ca0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ccc
			 0x7f800000,                                                  // inf                                         /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cdc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d1c
			 0xff800000,                                                  // -inf                                        /// 00d20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x4b000000,                                                  // 8388608.0                                   /// 00d48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x33333333,                                                  // 4 random values                             /// 00d50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00da8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00db4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00dc4
			 0x33333333,                                                  // 4 random values                             /// 00dc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x7f800000,                                                  // inf                                         /// 00ddc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x80000000,                                                  // -zero                                       /// 00dec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x33333333,                                                  // 4 random values                             /// 00e70
			 0xcb000000,                                                  // -8388608.0                                  /// 00e74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e98
			 0xbf028f5c,                                                  // -0.51                                       /// 00e9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ebc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ec4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ecc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ed8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ee0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ef4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ef8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f44
			 0xffc00001,                                                  // -signaling NaN                              /// 00f48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fb0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fcc
			 0xffc00001,                                                  // -signaling NaN                              /// 00fd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fdc
			 0x33333333,                                                  // 4 random values                             /// 00fe0
			 0xff800000,                                                  // -inf                                        /// 00fe4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x7f800000,                                                  // inf                                         /// 00ff4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xffffffff // QNan                                          // SP - ve numbers                             /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f6801b0, /// 0x0
			 0xc4252d0e, /// 0x4
			 0x6aa6c89e, /// 0x8
			 0xf5a18564, /// 0xc
			 0x6c54f07e, /// 0x10
			 0xeae86b0a, /// 0x14
			 0xe83ba575, /// 0x18
			 0x8462d754, /// 0x1c
			 0xc1cc062d, /// 0x20
			 0xc2be8e61, /// 0x24
			 0x1302f4d2, /// 0x28
			 0xd4e6a8a0, /// 0x2c
			 0x7aef599a, /// 0x30
			 0x9b16cf21, /// 0x34
			 0x07ccb7b0, /// 0x38
			 0x388832f3, /// 0x3c
			 0x831a9387, /// 0x40
			 0x2f755203, /// 0x44
			 0x1a740cc6, /// 0x48
			 0xef93fda3, /// 0x4c
			 0x385f2647, /// 0x50
			 0x88ed554a, /// 0x54
			 0x59dcf4c7, /// 0x58
			 0xe25af770, /// 0x5c
			 0x90c68e6b, /// 0x60
			 0x52d8c28c, /// 0x64
			 0x3410ce1f, /// 0x68
			 0xce3a3ad9, /// 0x6c
			 0xc94fed9a, /// 0x70
			 0x893854df, /// 0x74
			 0x474d4def, /// 0x78
			 0x5446edbe, /// 0x7c
			 0x1e1dca8b, /// 0x80
			 0xf2bfbf3c, /// 0x84
			 0x434829c7, /// 0x88
			 0xa809d9c0, /// 0x8c
			 0xddb67ee4, /// 0x90
			 0xeef2f0ec, /// 0x94
			 0x976afdaf, /// 0x98
			 0x9d09330d, /// 0x9c
			 0xfde6092f, /// 0xa0
			 0x054e7eae, /// 0xa4
			 0x4ec08591, /// 0xa8
			 0xa164ce8a, /// 0xac
			 0x9790cd02, /// 0xb0
			 0x2eed09eb, /// 0xb4
			 0x82131698, /// 0xb8
			 0x94ed035b, /// 0xbc
			 0x497e8e53, /// 0xc0
			 0x981b7e32, /// 0xc4
			 0x6d10271b, /// 0xc8
			 0x4d155ddd, /// 0xcc
			 0x0eff3341, /// 0xd0
			 0x178d9d1c, /// 0xd4
			 0x3362fa86, /// 0xd8
			 0xa84973ad, /// 0xdc
			 0x0c1fd596, /// 0xe0
			 0xef3dc1c4, /// 0xe4
			 0xf76e36df, /// 0xe8
			 0xb9b2c9e0, /// 0xec
			 0x623897b2, /// 0xf0
			 0xcff323f0, /// 0xf4
			 0x8387be8c, /// 0xf8
			 0xfe4d3bb7, /// 0xfc
			 0x1c0ac2f0, /// 0x100
			 0xae968188, /// 0x104
			 0x66f953b1, /// 0x108
			 0x629835de, /// 0x10c
			 0x737235b0, /// 0x110
			 0xccb79dbb, /// 0x114
			 0x7659b639, /// 0x118
			 0xbd16755b, /// 0x11c
			 0xa4d49176, /// 0x120
			 0x605f02ae, /// 0x124
			 0xb5c950d9, /// 0x128
			 0xe36304d4, /// 0x12c
			 0xc2d2abf4, /// 0x130
			 0x732f1b8e, /// 0x134
			 0x05c92a46, /// 0x138
			 0x7086475f, /// 0x13c
			 0x6bd1e529, /// 0x140
			 0x291f0bc0, /// 0x144
			 0xcc2caa46, /// 0x148
			 0x6e0d986e, /// 0x14c
			 0x40b90aff, /// 0x150
			 0x579cf44f, /// 0x154
			 0x5b2924dc, /// 0x158
			 0x79f30224, /// 0x15c
			 0xd6cfddfb, /// 0x160
			 0xea7229e9, /// 0x164
			 0x25983388, /// 0x168
			 0x1f98fbd0, /// 0x16c
			 0x9de3cb79, /// 0x170
			 0x28d3aeee, /// 0x174
			 0x315828e6, /// 0x178
			 0x4f8d2781, /// 0x17c
			 0xb9d2ff49, /// 0x180
			 0x076ce560, /// 0x184
			 0x6def2843, /// 0x188
			 0x2aebbe49, /// 0x18c
			 0x74ad0371, /// 0x190
			 0x0b420027, /// 0x194
			 0xa99355bd, /// 0x198
			 0xbf4178e9, /// 0x19c
			 0xbd279524, /// 0x1a0
			 0xac2e5c72, /// 0x1a4
			 0x35cbe8a5, /// 0x1a8
			 0x7fb4930b, /// 0x1ac
			 0x3b9eaa38, /// 0x1b0
			 0x05d5409f, /// 0x1b4
			 0x00808313, /// 0x1b8
			 0x84c601c9, /// 0x1bc
			 0x80110e07, /// 0x1c0
			 0x400c896c, /// 0x1c4
			 0x199c427b, /// 0x1c8
			 0x55acdd81, /// 0x1cc
			 0x60a26b3f, /// 0x1d0
			 0x15b420e3, /// 0x1d4
			 0x9c155db2, /// 0x1d8
			 0xa20b9023, /// 0x1dc
			 0x01cf541b, /// 0x1e0
			 0x2af0e3c0, /// 0x1e4
			 0x293575c0, /// 0x1e8
			 0x7e91372a, /// 0x1ec
			 0xe785210c, /// 0x1f0
			 0x24799f17, /// 0x1f4
			 0xa433024c, /// 0x1f8
			 0x1277053b, /// 0x1fc
			 0x548cf4ee, /// 0x200
			 0x91d223e3, /// 0x204
			 0x939e7761, /// 0x208
			 0x09a9d81d, /// 0x20c
			 0x60664b6c, /// 0x210
			 0xe84f3d67, /// 0x214
			 0x80eb034c, /// 0x218
			 0xb26a2876, /// 0x21c
			 0x5851981f, /// 0x220
			 0xd0591c9d, /// 0x224
			 0x996a02cb, /// 0x228
			 0x08710dde, /// 0x22c
			 0xa3d19e4f, /// 0x230
			 0x20adfeb1, /// 0x234
			 0x19cfc4b3, /// 0x238
			 0x614d313f, /// 0x23c
			 0x4884866b, /// 0x240
			 0xbdc4a6c3, /// 0x244
			 0xf4410325, /// 0x248
			 0x6f24d509, /// 0x24c
			 0x8118292e, /// 0x250
			 0x6102656f, /// 0x254
			 0x3f58af82, /// 0x258
			 0xb1982964, /// 0x25c
			 0x2c6c8150, /// 0x260
			 0xcf4d8d36, /// 0x264
			 0x76b47989, /// 0x268
			 0xe79da60e, /// 0x26c
			 0xf9f125d7, /// 0x270
			 0x2df295fa, /// 0x274
			 0xddb849d7, /// 0x278
			 0xddf95e08, /// 0x27c
			 0x56dee14a, /// 0x280
			 0x758f5c60, /// 0x284
			 0x3eae4e57, /// 0x288
			 0x0b4a9566, /// 0x28c
			 0xdcf50a4f, /// 0x290
			 0xf9468145, /// 0x294
			 0x15eed6d8, /// 0x298
			 0x6c4cc32c, /// 0x29c
			 0x79759ccd, /// 0x2a0
			 0x225d28cd, /// 0x2a4
			 0x169b7e53, /// 0x2a8
			 0xdaad9b42, /// 0x2ac
			 0x9b359790, /// 0x2b0
			 0x02ae86de, /// 0x2b4
			 0xe0455bfd, /// 0x2b8
			 0x1c17fe6a, /// 0x2bc
			 0xc631b6bd, /// 0x2c0
			 0xa327cd70, /// 0x2c4
			 0x7ef90305, /// 0x2c8
			 0xd3da3c3c, /// 0x2cc
			 0xa625ac98, /// 0x2d0
			 0x494f05a2, /// 0x2d4
			 0xb85d671d, /// 0x2d8
			 0xb7b1afef, /// 0x2dc
			 0x7cce675b, /// 0x2e0
			 0x46b902b7, /// 0x2e4
			 0x4723f7f5, /// 0x2e8
			 0xfd3a58e5, /// 0x2ec
			 0xc292a45e, /// 0x2f0
			 0x50591a28, /// 0x2f4
			 0x37bf3fc1, /// 0x2f8
			 0xa66655cf, /// 0x2fc
			 0x6011dc8a, /// 0x300
			 0x396b150a, /// 0x304
			 0x67c6d0b5, /// 0x308
			 0x119ec2a2, /// 0x30c
			 0x6f5c1545, /// 0x310
			 0xef9431fb, /// 0x314
			 0xc53acbb5, /// 0x318
			 0xe1888f91, /// 0x31c
			 0x748d45b0, /// 0x320
			 0x75a933d4, /// 0x324
			 0xc1f3d427, /// 0x328
			 0x7c982bcf, /// 0x32c
			 0x2c63d408, /// 0x330
			 0x3dfde1e4, /// 0x334
			 0x94bb4caa, /// 0x338
			 0xa8e9f42b, /// 0x33c
			 0x5c07b2b7, /// 0x340
			 0x183697e2, /// 0x344
			 0x437715cb, /// 0x348
			 0x4360da52, /// 0x34c
			 0x154ad33b, /// 0x350
			 0x6b4fc5c8, /// 0x354
			 0x7f511825, /// 0x358
			 0xf0454292, /// 0x35c
			 0xe41e98e9, /// 0x360
			 0x28321209, /// 0x364
			 0xdf94c8b4, /// 0x368
			 0x43a48af9, /// 0x36c
			 0x701eac59, /// 0x370
			 0x3249530e, /// 0x374
			 0xf6dcc4a1, /// 0x378
			 0xd17b2a32, /// 0x37c
			 0x0254519f, /// 0x380
			 0x099f7deb, /// 0x384
			 0x6be57fca, /// 0x388
			 0xb4887e9c, /// 0x38c
			 0x9c21038a, /// 0x390
			 0xe04f10e4, /// 0x394
			 0xa03fa4f6, /// 0x398
			 0xf595ee38, /// 0x39c
			 0x0d36ce00, /// 0x3a0
			 0xd90028a7, /// 0x3a4
			 0x88efedb7, /// 0x3a8
			 0x40062f19, /// 0x3ac
			 0xc136fb1d, /// 0x3b0
			 0xcf1b025c, /// 0x3b4
			 0x66672a9e, /// 0x3b8
			 0x7ef9c6d1, /// 0x3bc
			 0x964aaf58, /// 0x3c0
			 0x0ff51a37, /// 0x3c4
			 0x0184549f, /// 0x3c8
			 0xa98aba45, /// 0x3cc
			 0x492f23ae, /// 0x3d0
			 0xae275b7b, /// 0x3d4
			 0x16d17ae5, /// 0x3d8
			 0xb136ede5, /// 0x3dc
			 0x56218dd7, /// 0x3e0
			 0x6716b223, /// 0x3e4
			 0x212b9cc4, /// 0x3e8
			 0x24cb8e50, /// 0x3ec
			 0x11cbc523, /// 0x3f0
			 0x54136f08, /// 0x3f4
			 0x8e55f2f6, /// 0x3f8
			 0xbb21ef24, /// 0x3fc
			 0xc8633c78, /// 0x400
			 0x10a9e953, /// 0x404
			 0x5a0f7bc1, /// 0x408
			 0xb322b313, /// 0x40c
			 0x61e837df, /// 0x410
			 0xf3763f13, /// 0x414
			 0x14679c0b, /// 0x418
			 0x9a7afd17, /// 0x41c
			 0xeaf0388b, /// 0x420
			 0xe553ecb2, /// 0x424
			 0xd5610ede, /// 0x428
			 0xa29979b7, /// 0x42c
			 0xc6135122, /// 0x430
			 0x1163896c, /// 0x434
			 0xea3476d4, /// 0x438
			 0xba732630, /// 0x43c
			 0x3eb65655, /// 0x440
			 0x3f5ea2b2, /// 0x444
			 0x0940baa0, /// 0x448
			 0x9c767c4d, /// 0x44c
			 0xfdc0635d, /// 0x450
			 0x02bdc8b5, /// 0x454
			 0x219cd919, /// 0x458
			 0xa35bb1db, /// 0x45c
			 0xa4a86b9c, /// 0x460
			 0xc25d7d77, /// 0x464
			 0x05f938d4, /// 0x468
			 0xb738b340, /// 0x46c
			 0xde531dd0, /// 0x470
			 0x80225e9c, /// 0x474
			 0xdbc0bc44, /// 0x478
			 0xb52f0436, /// 0x47c
			 0x18ea90b5, /// 0x480
			 0x0affd21c, /// 0x484
			 0xcaa6252d, /// 0x488
			 0x9d94d8d3, /// 0x48c
			 0xe8982a93, /// 0x490
			 0x55fb4907, /// 0x494
			 0x8db661fb, /// 0x498
			 0xc28b57c5, /// 0x49c
			 0x065f233d, /// 0x4a0
			 0xea93edae, /// 0x4a4
			 0x93c07b02, /// 0x4a8
			 0x4a3bd404, /// 0x4ac
			 0x24897fe7, /// 0x4b0
			 0x088cd7e8, /// 0x4b4
			 0x4bdb5313, /// 0x4b8
			 0xca489aee, /// 0x4bc
			 0xcc3d858b, /// 0x4c0
			 0x734ba2ef, /// 0x4c4
			 0x9ab3f8b2, /// 0x4c8
			 0x794d49cd, /// 0x4cc
			 0x68402647, /// 0x4d0
			 0x04ff686e, /// 0x4d4
			 0x78e114ec, /// 0x4d8
			 0x96faae4a, /// 0x4dc
			 0x3e82337c, /// 0x4e0
			 0xa4b4e48f, /// 0x4e4
			 0xcfa50449, /// 0x4e8
			 0x36cba36b, /// 0x4ec
			 0x64a5a137, /// 0x4f0
			 0x193f4ad4, /// 0x4f4
			 0x3a4af1b6, /// 0x4f8
			 0x359d4c81, /// 0x4fc
			 0xcc2e2115, /// 0x500
			 0x2952ff3c, /// 0x504
			 0xd5c586be, /// 0x508
			 0x3315232e, /// 0x50c
			 0xc066c5fb, /// 0x510
			 0x22659bf0, /// 0x514
			 0xe917892d, /// 0x518
			 0x2cc0448b, /// 0x51c
			 0xd060fc0f, /// 0x520
			 0x76bca639, /// 0x524
			 0x2f20773c, /// 0x528
			 0x9b1954db, /// 0x52c
			 0x9160928f, /// 0x530
			 0x5dee6d00, /// 0x534
			 0x2022a623, /// 0x538
			 0x7152a23a, /// 0x53c
			 0xd674a56a, /// 0x540
			 0xa9208829, /// 0x544
			 0xe5a194a9, /// 0x548
			 0xdaa5c0e2, /// 0x54c
			 0x00424461, /// 0x550
			 0x77df38b2, /// 0x554
			 0x50d50d29, /// 0x558
			 0x086f4521, /// 0x55c
			 0x584d0032, /// 0x560
			 0x055cdd91, /// 0x564
			 0x80be7217, /// 0x568
			 0xc2007edc, /// 0x56c
			 0xfcd0e23e, /// 0x570
			 0x09547941, /// 0x574
			 0x2eee2fad, /// 0x578
			 0x81ad7054, /// 0x57c
			 0x86267731, /// 0x580
			 0xef22f474, /// 0x584
			 0xd4b61196, /// 0x588
			 0x3c6dbbca, /// 0x58c
			 0x718b0cff, /// 0x590
			 0x09b214f8, /// 0x594
			 0x5d11fc98, /// 0x598
			 0xe775211c, /// 0x59c
			 0xa8adb586, /// 0x5a0
			 0x86e0a3f4, /// 0x5a4
			 0x18644c5e, /// 0x5a8
			 0xef063c9c, /// 0x5ac
			 0xec9b2d59, /// 0x5b0
			 0x14718ebc, /// 0x5b4
			 0x60ce8308, /// 0x5b8
			 0x30694525, /// 0x5bc
			 0x556ac24f, /// 0x5c0
			 0x51c911b3, /// 0x5c4
			 0xeb28da22, /// 0x5c8
			 0x15635d20, /// 0x5cc
			 0xd8ad7acd, /// 0x5d0
			 0x39cf83d1, /// 0x5d4
			 0xfd5d8889, /// 0x5d8
			 0x6a451c48, /// 0x5dc
			 0x740eeb5f, /// 0x5e0
			 0xb36db65d, /// 0x5e4
			 0x5d034b97, /// 0x5e8
			 0x416df109, /// 0x5ec
			 0x69c660e5, /// 0x5f0
			 0x3dc3f876, /// 0x5f4
			 0xdf252c28, /// 0x5f8
			 0x4dc1001f, /// 0x5fc
			 0xbe60534d, /// 0x600
			 0xfc772059, /// 0x604
			 0x3141b502, /// 0x608
			 0xae530539, /// 0x60c
			 0x38984ec9, /// 0x610
			 0x10a49895, /// 0x614
			 0xebe59dad, /// 0x618
			 0xe87f8738, /// 0x61c
			 0x24061be8, /// 0x620
			 0x8b6291dd, /// 0x624
			 0x28c3c66b, /// 0x628
			 0x68cbcf97, /// 0x62c
			 0x5d9e0deb, /// 0x630
			 0x00de9585, /// 0x634
			 0x0c126a2d, /// 0x638
			 0x48611073, /// 0x63c
			 0x5d2e9aa3, /// 0x640
			 0x408f3364, /// 0x644
			 0xd5031324, /// 0x648
			 0xd8744674, /// 0x64c
			 0xa263d5a6, /// 0x650
			 0xf21a12cc, /// 0x654
			 0x351f7167, /// 0x658
			 0x88c0c829, /// 0x65c
			 0x8120db5e, /// 0x660
			 0xd4e098eb, /// 0x664
			 0xc45d6dc6, /// 0x668
			 0x8792ee8e, /// 0x66c
			 0x439df60e, /// 0x670
			 0x8223cd89, /// 0x674
			 0x28599b5e, /// 0x678
			 0x32f6cf52, /// 0x67c
			 0x84ef56bd, /// 0x680
			 0x1fc69e0a, /// 0x684
			 0x7c626e5d, /// 0x688
			 0x39093dde, /// 0x68c
			 0xcbee0550, /// 0x690
			 0x36106f70, /// 0x694
			 0x64c43665, /// 0x698
			 0x23de58ab, /// 0x69c
			 0x47e97aa1, /// 0x6a0
			 0x4bdf21f8, /// 0x6a4
			 0x1cbe7aac, /// 0x6a8
			 0x3396fe4e, /// 0x6ac
			 0x695347e1, /// 0x6b0
			 0xac4cf834, /// 0x6b4
			 0x22df1b01, /// 0x6b8
			 0xc4e80067, /// 0x6bc
			 0x951bbef1, /// 0x6c0
			 0x495092f4, /// 0x6c4
			 0xc9e63144, /// 0x6c8
			 0x5195b456, /// 0x6cc
			 0x3c025d10, /// 0x6d0
			 0x842a172a, /// 0x6d4
			 0x608da46d, /// 0x6d8
			 0x98639f2e, /// 0x6dc
			 0x24edbcf6, /// 0x6e0
			 0x84b0012d, /// 0x6e4
			 0x8043e521, /// 0x6e8
			 0xc1615394, /// 0x6ec
			 0xc70e41fe, /// 0x6f0
			 0xd98c1247, /// 0x6f4
			 0x532a0cec, /// 0x6f8
			 0x22441c29, /// 0x6fc
			 0x69e9444b, /// 0x700
			 0x301fd1bc, /// 0x704
			 0x6fa1b372, /// 0x708
			 0xfdfffeb7, /// 0x70c
			 0xc3754752, /// 0x710
			 0x0ab34748, /// 0x714
			 0x4a1b6663, /// 0x718
			 0xf2e723de, /// 0x71c
			 0x731209ed, /// 0x720
			 0x93471d9b, /// 0x724
			 0x2098237f, /// 0x728
			 0x4712fd82, /// 0x72c
			 0x8179d6a3, /// 0x730
			 0x6b040a13, /// 0x734
			 0xd8d89638, /// 0x738
			 0x8c9d0b84, /// 0x73c
			 0xaa176b8f, /// 0x740
			 0xd2571dab, /// 0x744
			 0x9554926c, /// 0x748
			 0x7e07ac12, /// 0x74c
			 0xdfb9bb6d, /// 0x750
			 0xaee9dc80, /// 0x754
			 0xf945c232, /// 0x758
			 0x724e8d43, /// 0x75c
			 0xceeb554f, /// 0x760
			 0x464cdff7, /// 0x764
			 0x1a0a6f5f, /// 0x768
			 0xe8191164, /// 0x76c
			 0xf22bf20f, /// 0x770
			 0x5fbb6fa5, /// 0x774
			 0x38199e23, /// 0x778
			 0xb66c163d, /// 0x77c
			 0xf58c553a, /// 0x780
			 0x38008785, /// 0x784
			 0x958ddcb1, /// 0x788
			 0x23679117, /// 0x78c
			 0xc22fda81, /// 0x790
			 0xf04ee186, /// 0x794
			 0x25268921, /// 0x798
			 0x1f494b81, /// 0x79c
			 0x24e16bf3, /// 0x7a0
			 0xbb766b93, /// 0x7a4
			 0x94177d2b, /// 0x7a8
			 0x2d7d9c4d, /// 0x7ac
			 0xd57e72b8, /// 0x7b0
			 0x80d13c13, /// 0x7b4
			 0xb32b17a7, /// 0x7b8
			 0x8b9e3bcd, /// 0x7bc
			 0x3759e8b1, /// 0x7c0
			 0x8007f414, /// 0x7c4
			 0xaa1c054d, /// 0x7c8
			 0xf5a60b3d, /// 0x7cc
			 0xcf337c1d, /// 0x7d0
			 0x1c4b96e4, /// 0x7d4
			 0xb9e5d46f, /// 0x7d8
			 0x6c1cfca1, /// 0x7dc
			 0xe21bcd10, /// 0x7e0
			 0xd5573ccc, /// 0x7e4
			 0x65d56017, /// 0x7e8
			 0x2e82eebc, /// 0x7ec
			 0x5fe964e4, /// 0x7f0
			 0x19447061, /// 0x7f4
			 0x2432f7b5, /// 0x7f8
			 0xa2311da4, /// 0x7fc
			 0x65e1e68e, /// 0x800
			 0xb0e515ee, /// 0x804
			 0x6e2a90ec, /// 0x808
			 0xf249fc18, /// 0x80c
			 0xd6f943ca, /// 0x810
			 0x880ef768, /// 0x814
			 0x57d6c089, /// 0x818
			 0x1f3b5b0f, /// 0x81c
			 0x40e13ab6, /// 0x820
			 0xfec2299f, /// 0x824
			 0x180282ca, /// 0x828
			 0xa2399808, /// 0x82c
			 0xb97b0540, /// 0x830
			 0xf51937ed, /// 0x834
			 0xaad3544b, /// 0x838
			 0xf0f726a3, /// 0x83c
			 0x8de6d594, /// 0x840
			 0x6c36d2f7, /// 0x844
			 0x85b8b663, /// 0x848
			 0x77bc63c0, /// 0x84c
			 0x6cd7bf78, /// 0x850
			 0x4faa40b8, /// 0x854
			 0x0b4580fb, /// 0x858
			 0x532bce32, /// 0x85c
			 0xa902f610, /// 0x860
			 0x11ee51dc, /// 0x864
			 0x776bb4a6, /// 0x868
			 0x8f3a683c, /// 0x86c
			 0x2321c721, /// 0x870
			 0xf91dd19a, /// 0x874
			 0x82f87e63, /// 0x878
			 0xc08b07d4, /// 0x87c
			 0x423a0b06, /// 0x880
			 0x2fd5f4ca, /// 0x884
			 0x5a4acf28, /// 0x888
			 0xd4dbd268, /// 0x88c
			 0xa4f78fce, /// 0x890
			 0xdc7322a7, /// 0x894
			 0xa3d6e178, /// 0x898
			 0x5857879f, /// 0x89c
			 0xbe107792, /// 0x8a0
			 0xc91e1133, /// 0x8a4
			 0x19063423, /// 0x8a8
			 0x76fb56f9, /// 0x8ac
			 0xfef9b4a9, /// 0x8b0
			 0xe7907589, /// 0x8b4
			 0xef389494, /// 0x8b8
			 0x58d83d6d, /// 0x8bc
			 0x6f41cc06, /// 0x8c0
			 0x70b6d906, /// 0x8c4
			 0x433a1080, /// 0x8c8
			 0x9d0f0db8, /// 0x8cc
			 0xbbbf727f, /// 0x8d0
			 0x7ac9e127, /// 0x8d4
			 0xb36cc1a7, /// 0x8d8
			 0x941d3684, /// 0x8dc
			 0x08d7e3cd, /// 0x8e0
			 0xf92c3f7b, /// 0x8e4
			 0xe4738b00, /// 0x8e8
			 0xf66f782c, /// 0x8ec
			 0xbcdec018, /// 0x8f0
			 0xef02c5f2, /// 0x8f4
			 0x78226b8c, /// 0x8f8
			 0x13733afb, /// 0x8fc
			 0x3760c1f7, /// 0x900
			 0x40e32c0e, /// 0x904
			 0xb662549c, /// 0x908
			 0xbcc46176, /// 0x90c
			 0x5fd1abbb, /// 0x910
			 0xd74f673a, /// 0x914
			 0x24a76fda, /// 0x918
			 0x6ff3b7fb, /// 0x91c
			 0x814217d2, /// 0x920
			 0xff2c7670, /// 0x924
			 0xdd017334, /// 0x928
			 0x6fa141c0, /// 0x92c
			 0xb7070823, /// 0x930
			 0xffc1494f, /// 0x934
			 0xdde89fd9, /// 0x938
			 0x9e98249a, /// 0x93c
			 0xd761c13c, /// 0x940
			 0x677b3252, /// 0x944
			 0xcd8bd549, /// 0x948
			 0xb51a34b0, /// 0x94c
			 0x864c21d2, /// 0x950
			 0x26947320, /// 0x954
			 0x0d0ff98e, /// 0x958
			 0xcfacab38, /// 0x95c
			 0x3f7334b6, /// 0x960
			 0xeca03c9c, /// 0x964
			 0x3b65d190, /// 0x968
			 0xa43c40ae, /// 0x96c
			 0x31bcb1ea, /// 0x970
			 0x4a819cbe, /// 0x974
			 0x2602688f, /// 0x978
			 0x5a65bcac, /// 0x97c
			 0xf5a828e1, /// 0x980
			 0xc4c032f2, /// 0x984
			 0x0ec4d14f, /// 0x988
			 0x831cd16e, /// 0x98c
			 0xd3bacef1, /// 0x990
			 0x737270ff, /// 0x994
			 0xb3146f81, /// 0x998
			 0xb4a780c5, /// 0x99c
			 0xc27d9c05, /// 0x9a0
			 0xdecf0cd0, /// 0x9a4
			 0x291eaf8c, /// 0x9a8
			 0x10f42786, /// 0x9ac
			 0xde7ec889, /// 0x9b0
			 0x310fab17, /// 0x9b4
			 0x955f9f16, /// 0x9b8
			 0x59e294b1, /// 0x9bc
			 0x967dbe89, /// 0x9c0
			 0x1aadf0e0, /// 0x9c4
			 0x8e52776c, /// 0x9c8
			 0x357fec60, /// 0x9cc
			 0xf39042ea, /// 0x9d0
			 0x3c2a2df7, /// 0x9d4
			 0x64e2285c, /// 0x9d8
			 0x12f49acf, /// 0x9dc
			 0x96e0535e, /// 0x9e0
			 0x4476b535, /// 0x9e4
			 0xe3dfb54e, /// 0x9e8
			 0xbc4178a8, /// 0x9ec
			 0xd86d4c85, /// 0x9f0
			 0x20937f39, /// 0x9f4
			 0xe22b9402, /// 0x9f8
			 0x25ae0fc9, /// 0x9fc
			 0x8538840f, /// 0xa00
			 0xfffe1945, /// 0xa04
			 0x1875a41f, /// 0xa08
			 0xae83a748, /// 0xa0c
			 0xc7bc5dfb, /// 0xa10
			 0x7c2c21a9, /// 0xa14
			 0x37990e40, /// 0xa18
			 0x5c297fdb, /// 0xa1c
			 0x1202a032, /// 0xa20
			 0xf3b20ab4, /// 0xa24
			 0xc37bbf15, /// 0xa28
			 0x4560990d, /// 0xa2c
			 0x66eeb42a, /// 0xa30
			 0xc6068050, /// 0xa34
			 0x4173e47e, /// 0xa38
			 0x4939d9be, /// 0xa3c
			 0x39f01e01, /// 0xa40
			 0x92011c81, /// 0xa44
			 0x19006d51, /// 0xa48
			 0xc1c264d9, /// 0xa4c
			 0xffb5ee53, /// 0xa50
			 0x820e874f, /// 0xa54
			 0x738d9191, /// 0xa58
			 0x682cf958, /// 0xa5c
			 0x30935f36, /// 0xa60
			 0x22fcb6b8, /// 0xa64
			 0xdc760555, /// 0xa68
			 0x9c0eb463, /// 0xa6c
			 0x7c413c54, /// 0xa70
			 0x321db101, /// 0xa74
			 0xdeccf5e6, /// 0xa78
			 0x3199e0da, /// 0xa7c
			 0xa77ff9a1, /// 0xa80
			 0x472f0c64, /// 0xa84
			 0x36e705ab, /// 0xa88
			 0x3af9abdd, /// 0xa8c
			 0x5a34af4c, /// 0xa90
			 0x39775564, /// 0xa94
			 0x6f39ff1b, /// 0xa98
			 0xdba16748, /// 0xa9c
			 0xc3bd60ac, /// 0xaa0
			 0x6ffe3766, /// 0xaa4
			 0x82c4a551, /// 0xaa8
			 0x4bbf3cb3, /// 0xaac
			 0xa5ee709c, /// 0xab0
			 0x1c34cabf, /// 0xab4
			 0xffa120c9, /// 0xab8
			 0xa9de79b4, /// 0xabc
			 0xcf4623ee, /// 0xac0
			 0xead1864a, /// 0xac4
			 0x13fe47a0, /// 0xac8
			 0xdb8266b4, /// 0xacc
			 0xd5bbc94d, /// 0xad0
			 0x1e9bacaf, /// 0xad4
			 0x0445ba1d, /// 0xad8
			 0xe7f9c1fc, /// 0xadc
			 0x148a94c2, /// 0xae0
			 0x51364113, /// 0xae4
			 0x6c119e94, /// 0xae8
			 0xca76edea, /// 0xaec
			 0xcf5d24ed, /// 0xaf0
			 0x375b7073, /// 0xaf4
			 0x5a67f14f, /// 0xaf8
			 0xd9a84f86, /// 0xafc
			 0xa074dcd2, /// 0xb00
			 0x2cec8cad, /// 0xb04
			 0x65569573, /// 0xb08
			 0xbe60d9a4, /// 0xb0c
			 0x050758d4, /// 0xb10
			 0x84a6ebd2, /// 0xb14
			 0x2c6e2078, /// 0xb18
			 0x10114d8e, /// 0xb1c
			 0x82510608, /// 0xb20
			 0x7940354a, /// 0xb24
			 0x47cc8c7e, /// 0xb28
			 0x42163a40, /// 0xb2c
			 0x0fe17929, /// 0xb30
			 0x0e5de918, /// 0xb34
			 0x5af702d7, /// 0xb38
			 0x83a726c6, /// 0xb3c
			 0x0f26915b, /// 0xb40
			 0xcdc4a5b9, /// 0xb44
			 0x16c752bc, /// 0xb48
			 0x3209175e, /// 0xb4c
			 0x96207588, /// 0xb50
			 0x34f87e31, /// 0xb54
			 0xc5afab3d, /// 0xb58
			 0x04d2e017, /// 0xb5c
			 0xf68697c7, /// 0xb60
			 0xa28d7c1f, /// 0xb64
			 0x806548c8, /// 0xb68
			 0xebbf457f, /// 0xb6c
			 0x4eaea158, /// 0xb70
			 0x6c85d343, /// 0xb74
			 0x8c79041e, /// 0xb78
			 0xe326b8c2, /// 0xb7c
			 0x607cc398, /// 0xb80
			 0xa772f731, /// 0xb84
			 0x0582a6a0, /// 0xb88
			 0x7e08f771, /// 0xb8c
			 0x048356b6, /// 0xb90
			 0x4cfbfd4d, /// 0xb94
			 0x0f652a18, /// 0xb98
			 0xbd9c1418, /// 0xb9c
			 0xb0def9c5, /// 0xba0
			 0xcaea6291, /// 0xba4
			 0xb0b279e2, /// 0xba8
			 0xc8777308, /// 0xbac
			 0x082f2b95, /// 0xbb0
			 0x09b2d41f, /// 0xbb4
			 0x399c5c8f, /// 0xbb8
			 0xc3616f01, /// 0xbbc
			 0x9c8fa488, /// 0xbc0
			 0xeef4e69c, /// 0xbc4
			 0xa9bdee6c, /// 0xbc8
			 0x8a110e91, /// 0xbcc
			 0xfc5ac780, /// 0xbd0
			 0xfbd821f9, /// 0xbd4
			 0x2175a3e3, /// 0xbd8
			 0x8ca1938e, /// 0xbdc
			 0xf26b2122, /// 0xbe0
			 0x93286aeb, /// 0xbe4
			 0xb8b20dfd, /// 0xbe8
			 0x1984c3bf, /// 0xbec
			 0x663c9032, /// 0xbf0
			 0x8ca33898, /// 0xbf4
			 0x7acaa87c, /// 0xbf8
			 0xaf8f17f5, /// 0xbfc
			 0x67a16016, /// 0xc00
			 0x7a94c9e3, /// 0xc04
			 0x1e833b6f, /// 0xc08
			 0xb9217a14, /// 0xc0c
			 0x0f99046e, /// 0xc10
			 0xb0c9997e, /// 0xc14
			 0x001afbce, /// 0xc18
			 0xeefd658a, /// 0xc1c
			 0xe4de18cd, /// 0xc20
			 0x26c32b30, /// 0xc24
			 0x9e8c2774, /// 0xc28
			 0x4a8e2e13, /// 0xc2c
			 0x6226dee0, /// 0xc30
			 0xe7da90e0, /// 0xc34
			 0x5e23d934, /// 0xc38
			 0x581d74c4, /// 0xc3c
			 0x8131d4ed, /// 0xc40
			 0xc12bd4b2, /// 0xc44
			 0x6f22d232, /// 0xc48
			 0x347d336f, /// 0xc4c
			 0xa29eb7d4, /// 0xc50
			 0x026b334f, /// 0xc54
			 0x0378168c, /// 0xc58
			 0xcbbb78f4, /// 0xc5c
			 0x48687f40, /// 0xc60
			 0x1c7e4d08, /// 0xc64
			 0x2eb94c6a, /// 0xc68
			 0x5e755050, /// 0xc6c
			 0xc4c1e2e2, /// 0xc70
			 0x6def3192, /// 0xc74
			 0x36f63e94, /// 0xc78
			 0xccc9bad0, /// 0xc7c
			 0xf0116b05, /// 0xc80
			 0xbec25ac2, /// 0xc84
			 0xc6d219e4, /// 0xc88
			 0xf4d83298, /// 0xc8c
			 0x4a235d50, /// 0xc90
			 0x1338f194, /// 0xc94
			 0x74672a0f, /// 0xc98
			 0x5b18baf9, /// 0xc9c
			 0xb2cd5e82, /// 0xca0
			 0x85553a8c, /// 0xca4
			 0x38e02e12, /// 0xca8
			 0x73843914, /// 0xcac
			 0x5b603d9b, /// 0xcb0
			 0x99ffea59, /// 0xcb4
			 0xc9a745fe, /// 0xcb8
			 0xccb257fc, /// 0xcbc
			 0x8f579005, /// 0xcc0
			 0x4565ce06, /// 0xcc4
			 0x54ac753f, /// 0xcc8
			 0x8c9d33b4, /// 0xccc
			 0x2eb3ea81, /// 0xcd0
			 0x8b6fd9b5, /// 0xcd4
			 0xff5f63c7, /// 0xcd8
			 0xe5d2fe1e, /// 0xcdc
			 0xf6e6f008, /// 0xce0
			 0x3c337e22, /// 0xce4
			 0x0763ef5a, /// 0xce8
			 0x574eceed, /// 0xcec
			 0xf3e3eddb, /// 0xcf0
			 0x5a04dcbe, /// 0xcf4
			 0x243f8653, /// 0xcf8
			 0xca089817, /// 0xcfc
			 0x7e940e24, /// 0xd00
			 0xde39f0d1, /// 0xd04
			 0xa10eb028, /// 0xd08
			 0xbbf96d57, /// 0xd0c
			 0x5192a24a, /// 0xd10
			 0x043eae52, /// 0xd14
			 0xc2a559e0, /// 0xd18
			 0x98c4d299, /// 0xd1c
			 0x2dff30d8, /// 0xd20
			 0x8c59f043, /// 0xd24
			 0x3292305a, /// 0xd28
			 0xd8056b61, /// 0xd2c
			 0xf51008be, /// 0xd30
			 0x2feab693, /// 0xd34
			 0x80beb582, /// 0xd38
			 0xd8a75d12, /// 0xd3c
			 0x5832b888, /// 0xd40
			 0x0b4808a0, /// 0xd44
			 0xa18d3e4d, /// 0xd48
			 0x0429ead6, /// 0xd4c
			 0xbba9b53f, /// 0xd50
			 0x1d67bbdb, /// 0xd54
			 0x465d00af, /// 0xd58
			 0x15cbbcb4, /// 0xd5c
			 0x0716194c, /// 0xd60
			 0x5d6587c9, /// 0xd64
			 0x48491910, /// 0xd68
			 0x08f46074, /// 0xd6c
			 0xd3d4da87, /// 0xd70
			 0x166e1315, /// 0xd74
			 0xd43b9ca8, /// 0xd78
			 0xbcf6e175, /// 0xd7c
			 0x8a409245, /// 0xd80
			 0x1137fdd7, /// 0xd84
			 0x81006a81, /// 0xd88
			 0x71f80503, /// 0xd8c
			 0x0dcfa7d1, /// 0xd90
			 0x3fb0a8d7, /// 0xd94
			 0x04754739, /// 0xd98
			 0xad552853, /// 0xd9c
			 0x6fc5a5fa, /// 0xda0
			 0x1deaefea, /// 0xda4
			 0xedec3652, /// 0xda8
			 0x290e71d3, /// 0xdac
			 0x25104bdf, /// 0xdb0
			 0xea33adb1, /// 0xdb4
			 0xc5a5da65, /// 0xdb8
			 0xd915811b, /// 0xdbc
			 0xca57f1f5, /// 0xdc0
			 0xc1a2dd2e, /// 0xdc4
			 0xb7e62ebd, /// 0xdc8
			 0x50e7d3b3, /// 0xdcc
			 0xcdddf293, /// 0xdd0
			 0xcf0859d8, /// 0xdd4
			 0x2c88e04d, /// 0xdd8
			 0x42ff96d4, /// 0xddc
			 0xe58e0900, /// 0xde0
			 0xdf444d7c, /// 0xde4
			 0xd0b6d881, /// 0xde8
			 0xddc37f32, /// 0xdec
			 0x2c1120dc, /// 0xdf0
			 0xa73bd2e4, /// 0xdf4
			 0xeb0b44c3, /// 0xdf8
			 0x44336015, /// 0xdfc
			 0x19e22b92, /// 0xe00
			 0xb4bf5683, /// 0xe04
			 0x00b032e1, /// 0xe08
			 0x6b44ac4b, /// 0xe0c
			 0xc60cfbb8, /// 0xe10
			 0x55fc20bc, /// 0xe14
			 0xb751b7cf, /// 0xe18
			 0xf64fe37d, /// 0xe1c
			 0x92b35d33, /// 0xe20
			 0x6cf0c5be, /// 0xe24
			 0xd4e16712, /// 0xe28
			 0x1de2d2cf, /// 0xe2c
			 0x3f3c28d9, /// 0xe30
			 0x179b3473, /// 0xe34
			 0xb0b652b2, /// 0xe38
			 0xdc31722c, /// 0xe3c
			 0x710b2eb1, /// 0xe40
			 0x2f66b6c9, /// 0xe44
			 0x17b9e22f, /// 0xe48
			 0xca550e0b, /// 0xe4c
			 0x0f051613, /// 0xe50
			 0x89dd4768, /// 0xe54
			 0x9d1119a9, /// 0xe58
			 0x4b930b48, /// 0xe5c
			 0x9b58aa4f, /// 0xe60
			 0x7098bfb0, /// 0xe64
			 0xf90f5c83, /// 0xe68
			 0xaf48007f, /// 0xe6c
			 0xb939e9f2, /// 0xe70
			 0x8d709116, /// 0xe74
			 0x46708826, /// 0xe78
			 0xa54587ee, /// 0xe7c
			 0xe2da0bed, /// 0xe80
			 0x3772a525, /// 0xe84
			 0x23dcb5a9, /// 0xe88
			 0x8a2c5b2a, /// 0xe8c
			 0x3c207f3b, /// 0xe90
			 0xb2cf3d2b, /// 0xe94
			 0x70400cbe, /// 0xe98
			 0x8f734639, /// 0xe9c
			 0x0b953697, /// 0xea0
			 0x22494440, /// 0xea4
			 0x518d0f81, /// 0xea8
			 0xf5f0a7c9, /// 0xeac
			 0xfe6b0fec, /// 0xeb0
			 0x8b03e94e, /// 0xeb4
			 0x4bc0431a, /// 0xeb8
			 0xd5420eb1, /// 0xebc
			 0x578b16a2, /// 0xec0
			 0x6d7513e2, /// 0xec4
			 0x3637b19e, /// 0xec8
			 0xaf06037c, /// 0xecc
			 0x00785388, /// 0xed0
			 0xa58ff7b6, /// 0xed4
			 0x3027515e, /// 0xed8
			 0xb0fc05fa, /// 0xedc
			 0x9c33ee5e, /// 0xee0
			 0xe3601265, /// 0xee4
			 0x2f7b6c0b, /// 0xee8
			 0x7a8ccf80, /// 0xeec
			 0xc881decd, /// 0xef0
			 0x8f1b45d6, /// 0xef4
			 0x75fd024b, /// 0xef8
			 0x7c85144a, /// 0xefc
			 0xa2adb1df, /// 0xf00
			 0x6ef562a6, /// 0xf04
			 0xd935f842, /// 0xf08
			 0xa59e6e4d, /// 0xf0c
			 0xb33b7521, /// 0xf10
			 0x0fc1aeec, /// 0xf14
			 0x1dd90706, /// 0xf18
			 0x98a870f7, /// 0xf1c
			 0xde0eddf1, /// 0xf20
			 0xf9ba6f00, /// 0xf24
			 0x5458d684, /// 0xf28
			 0x5ed198ec, /// 0xf2c
			 0x6b8bb5e2, /// 0xf30
			 0xb7c055af, /// 0xf34
			 0xbc785d0c, /// 0xf38
			 0x7d52bea0, /// 0xf3c
			 0xd0b89fdb, /// 0xf40
			 0xff5d8544, /// 0xf44
			 0x8595eeb1, /// 0xf48
			 0x6cfc748a, /// 0xf4c
			 0x04d8ab2e, /// 0xf50
			 0x24919714, /// 0xf54
			 0x82c05a56, /// 0xf58
			 0xe60980e6, /// 0xf5c
			 0xc5d18d8c, /// 0xf60
			 0x9bdd81a6, /// 0xf64
			 0x93bfc3fc, /// 0xf68
			 0x6fa72def, /// 0xf6c
			 0xe3c1dc38, /// 0xf70
			 0xd88d5779, /// 0xf74
			 0x57deb4b3, /// 0xf78
			 0x0d881ebd, /// 0xf7c
			 0x605baa5f, /// 0xf80
			 0xba611e67, /// 0xf84
			 0xa0ef65ce, /// 0xf88
			 0xbfbf7861, /// 0xf8c
			 0x0dcaf8a0, /// 0xf90
			 0xb2ad32a8, /// 0xf94
			 0xab8fd157, /// 0xf98
			 0x6ead86c9, /// 0xf9c
			 0x5b2689fc, /// 0xfa0
			 0x60d60c1d, /// 0xfa4
			 0x4cedfc06, /// 0xfa8
			 0x087c1544, /// 0xfac
			 0x4130a9df, /// 0xfb0
			 0x08a8df67, /// 0xfb4
			 0x3d5712c1, /// 0xfb8
			 0x98127da3, /// 0xfbc
			 0x36e91e3a, /// 0xfc0
			 0x14bb1ed9, /// 0xfc4
			 0x4e01a48b, /// 0xfc8
			 0x26e28b2d, /// 0xfcc
			 0xfb0b2b59, /// 0xfd0
			 0x1713f436, /// 0xfd4
			 0xf9da3698, /// 0xfd8
			 0xd45ada16, /// 0xfdc
			 0x96bccc97, /// 0xfe0
			 0xf3c53a95, /// 0xfe4
			 0xf5d2f31e, /// 0xfe8
			 0xd3e499aa, /// 0xfec
			 0x7ab16390, /// 0xff0
			 0xf62a3d8f, /// 0xff4
			 0xe5707deb, /// 0xff8
			 0x2f444839 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00004
			 0x4b000000,                                                  // 8388608.0                                   /// 00008
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00018
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0001c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00020
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00028
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00030
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00034
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00038
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00040
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0004c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00054
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00058
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0005c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00060
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00064
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00068
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0006c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00070
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00074
			 0x55555555,                                                  // 4 random values                             /// 00078
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0007c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00084
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00088
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0008c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00090
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00098
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0009c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000dc
			 0x55555555,                                                  // 4 random values                             /// 000e0
			 0xcb000000,                                                  // -8388608.0                                  /// 000e4
			 0x4b000000,                                                  // 8388608.0                                   /// 000e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00100
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00104
			 0xbf028f5c,                                                  // -0.51                                       /// 00108
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0010c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00118
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0011c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00130
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00138
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0013c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00144
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0014c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00150
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00154
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00158
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0015c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00164
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00168
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00170
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00178
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00180
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00188
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0018c
			 0xbf028f5c,                                                  // -0.51                                       /// 00190
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c400000,                                                  // Leading 1s:                                 /// 00204
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00208
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0020c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00210
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00214
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00220
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00228
			 0x3f028f5c,                                                  // 0.51                                        /// 0022c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xff800000,                                                  // -inf                                        /// 00238
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00240
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00244
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00254
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00258
			 0x0e000003,                                                  // Trailing 1s:                                /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00264
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0026c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0e000003,                                                  // Trailing 1s:                                /// 00274
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00278
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00284
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00290
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00294
			 0x00011111,                                                  // 9.7958E-41                                  /// 00298
			 0x0c600000,                                                  // Leading 1s:                                 /// 0029c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0xcb000000,                                                  // -8388608.0                                  /// 002ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d4
			 0x55555555,                                                  // 4 random values                             /// 002d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00300
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00308
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00310
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00314
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00318
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00320
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00328
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0032c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00334
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00338
			 0x4b000000,                                                  // 8388608.0                                   /// 0033c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00340
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00348
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0034c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00354
			 0x3f028f5c,                                                  // 0.51                                        /// 00358
			 0x0c600000,                                                  // Leading 1s:                                 /// 0035c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00360
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0c600000,                                                  // Leading 1s:                                 /// 00368
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00374
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00378
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00380
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00384
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0038c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00394
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00398
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0039c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x80000000,                                                  // -zero                                       /// 003bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000000,                                                  // -zero                                       /// 003d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 003f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00404
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00410
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00414
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00420
			 0xffc00001,                                                  // -signaling NaN                              /// 00424
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00430
			 0x7f800000,                                                  // inf                                         /// 00434
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00438
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0043c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00440
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00448
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00454
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00460
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00464
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xcb000000,                                                  // -8388608.0                                  /// 0046c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00470
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00474
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00478
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00480
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0e000003,                                                  // Trailing 1s:                                /// 00488
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0048c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00494
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0049c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x3f028f5c,                                                  // 0.51                                        /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004cc
			 0x00000000,                                                  // zero                                        /// 004d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x4b000000,                                                  // 8388608.0                                   /// 004e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00500
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0050c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00510
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00514
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00520
			 0xbf028f5c,                                                  // -0.51                                       /// 00524
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00528
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0052c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00534
			 0x33333333,                                                  // 4 random values                             /// 00538
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0053c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00540
			 0xbf028f5c,                                                  // -0.51                                       /// 00544
			 0xffc00001,                                                  // -signaling NaN                              /// 00548
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00554
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00558
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0055c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00560
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00564
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00568
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0056c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00570
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00578
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0057c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00580
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00588
			 0x0c600000,                                                  // Leading 1s:                                 /// 0058c
			 0x7fc00001,                                                  // signaling NaN                               /// 00590
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00594
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00598
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a0
			 0x55555555,                                                  // 4 random values                             /// 005a4
			 0xffc00001,                                                  // -signaling NaN                              /// 005a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00604
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00610
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00614
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00624
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00628
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0062c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00634
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00638
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0063c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00640
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00644
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00648
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0064c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00658
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00664
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00668
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00670
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00674
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x33333333,                                                  // 4 random values                             /// 0067c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00680
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00684
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0068c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00690
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00698
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0069c
			 0x4b000000,                                                  // 8388608.0                                   /// 006a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00708
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0070c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00710
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00714
			 0x4b000000,                                                  // 8388608.0                                   /// 00718
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0071c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00720
			 0x00000000,                                                  // zero                                        /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00738
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0073c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00740
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00744
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0074c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00750
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00754
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00758
			 0x0e000003,                                                  // Trailing 1s:                                /// 0075c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00770
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00778
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00780
			 0x80011111,                                                  // -9.7958E-41                                 /// 00784
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00788
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00790
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00794
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a4
			 0x55555555,                                                  // 4 random values                             /// 007a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007e4
			 0xff800000,                                                  // -inf                                        /// 007e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00800
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00804
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00808
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00810
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00814
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00820
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00824
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00830
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00838
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00840
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00848
			 0x00000000,                                                  // zero                                        /// 0084c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00850
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00858
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0085c
			 0x80000000,                                                  // -zero                                       /// 00860
			 0x00000000,                                                  // zero                                        /// 00864
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00884
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00888
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00890
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00898
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xcb000000,                                                  // -8388608.0                                  /// 008b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0xcb000000,                                                  // -8388608.0                                  /// 008c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7f800000,                                                  // inf                                         /// 008d8
			 0x33333333,                                                  // 4 random values                             /// 008dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x7f800000,                                                  // inf                                         /// 00908
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00910
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x33333333,                                                  // 4 random values                             /// 00918
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00924
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00928
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0092c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00930
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0094c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00950
			 0x4b000000,                                                  // 8388608.0                                   /// 00954
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00960
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00964
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00980
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x4b000000,                                                  // 8388608.0                                   /// 0098c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00990
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00994
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00998
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0099c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x4b000000,                                                  // 8388608.0                                   /// 009dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a78
			 0x7f800000,                                                  // inf                                         /// 00a7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a94
			 0xff800000,                                                  // -inf                                        /// 00a98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aa0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aa4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00aac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ac4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00acc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00adc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ae0
			 0xff800000,                                                  // -inf                                        /// 00ae4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b04
			 0x80000000,                                                  // -zero                                       /// 00b08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x80000000,                                                  // -zero                                       /// 00b50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b58
			 0xcb000000,                                                  // -8388608.0                                  /// 00b5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b60
			 0x3f028f5c,                                                  // 0.51                                        /// 00b64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bdc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bf4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c10
			 0x80000000,                                                  // -zero                                       /// 00c14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x00000000,                                                  // zero                                        /// 00c54
			 0x55555555,                                                  // 4 random values                             /// 00c58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c84
			 0xbf028f5c,                                                  // -0.51                                       /// 00c88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c90
			 0xcb000000,                                                  // -8388608.0                                  /// 00c94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cd4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cdc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cfc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00000000,                                                  // zero                                        /// 00d18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d20
			 0xffc00001,                                                  // -signaling NaN                              /// 00d24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d30
			 0xcb000000,                                                  // -8388608.0                                  /// 00d34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d7c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x7f800000,                                                  // inf                                         /// 00da0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x33333333,                                                  // 4 random values                             /// 00db4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd0
			 0x33333333,                                                  // 4 random values                             /// 00dd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ddc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e30
			 0x4b000000,                                                  // 8388608.0                                   /// 00e34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e40
			 0x7f800000,                                                  // inf                                         /// 00e44
			 0x7f800000,                                                  // inf                                         /// 00e48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0xbf028f5c,                                                  // -0.51                                       /// 00e58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e5c
			 0x7f800000,                                                  // inf                                         /// 00e60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00000000,                                                  // zero                                        /// 00e6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eb4
			 0x33333333,                                                  // 4 random values                             /// 00eb8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ebc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ec0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ecc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00edc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ef0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x7f800000,                                                  // inf                                         /// 00f0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f20
			 0xcb000000,                                                  // -8388608.0                                  /// 00f24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0xffc00001,                                                  // -signaling NaN                              /// 00f8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fa4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x0d00fff0                                                  // Set of 1s                                   /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00000
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00004
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00008
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00010
			 0x0c600000,                                                  // Leading 1s:                                 /// 00014
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00018
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00020
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00028
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0003c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00044
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00048
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0004c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00050
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00054
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00058
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00060
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00064
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00068
			 0x55555555,                                                  // 4 random values                             /// 0006c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00070
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0007c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00080
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00084
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00088
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0008c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c8
			 0x00000000,                                                  // zero                                        /// 000cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00104
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0010c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00114
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0011c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00128
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0012c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00134
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0013c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00140
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0014c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0015c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00160
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00168
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00174
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0017c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00180
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00184
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00190
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00194
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00198
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0019c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00200
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00208
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00210
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00218
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0021c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00224
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00228
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00230
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00234
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00238
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0023c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00240
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00244
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00250
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00254
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xcb000000,                                                  // -8388608.0                                  /// 00260
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00264
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00268
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0026c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00270
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00274
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00278
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00280
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00284
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00288
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00298
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0029c
			 0xff800000,                                                  // -inf                                        /// 002a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002dc
			 0x7fc00001,                                                  // signaling NaN                               /// 002e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xbf028f5c,                                                  // -0.51                                       /// 002f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0xbf028f5c,                                                  // -0.51                                       /// 00308
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00310
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x4b000000,                                                  // 8388608.0                                   /// 00318
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00328
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0032c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00338
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0033c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00348
			 0x4b000000,                                                  // 8388608.0                                   /// 0034c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00350
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00358
			 0x0c780000,                                                  // Leading 1s:                                 /// 0035c
			 0xcb000000,                                                  // -8388608.0                                  /// 00360
			 0x0e000001,                                                  // Trailing 1s:                                /// 00364
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00368
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00378
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00380
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00388
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0038c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00390
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00394
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00398
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0039c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00400
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00408
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0040c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00410
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00414
			 0x80011111,                                                  // -9.7958E-41                                 /// 00418
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00420
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00424
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00428
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0042c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00430
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00434
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00438
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00440
			 0x7fc00001,                                                  // signaling NaN                               /// 00444
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00448
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0044c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00450
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00454
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x7fc00001,                                                  // signaling NaN                               /// 00460
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00470
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00474
			 0xbf028f5c,                                                  // -0.51                                       /// 00478
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0047c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00480
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00484
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00488
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0048c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00498
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0049c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0xbf028f5c,                                                  // -0.51                                       /// 004a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b0
			 0x33333333,                                                  // 4 random values                             /// 004b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x4b000000,                                                  // 8388608.0                                   /// 004f0
			 0x55555555,                                                  // 4 random values                             /// 004f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00500
			 0xffc00001,                                                  // -signaling NaN                              /// 00504
			 0x3f028f5c,                                                  // 0.51                                        /// 00508
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0050c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00510
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00514
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0051c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00524
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00528
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00540
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00548
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0054c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00550
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00554
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00558
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0055c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00568
			 0x4b000000,                                                  // 8388608.0                                   /// 0056c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00570
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00578
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0057c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00588
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0058c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00590
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00598
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0059c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005f0
			 0x00000000,                                                  // zero                                        /// 005f4
			 0x80000000,                                                  // -zero                                       /// 005f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00600
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00604
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xff800000,                                                  // -inf                                        /// 00614
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00618
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00620
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00624
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00638
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0063c
			 0xcb000000,                                                  // -8388608.0                                  /// 00640
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x7f800000,                                                  // inf                                         /// 00648
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00650
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00658
			 0x80000000,                                                  // -zero                                       /// 0065c
			 0x80000000,                                                  // -zero                                       /// 00660
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00670
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00674
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00678
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0067c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0c400000,                                                  // Leading 1s:                                 /// 00684
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x3f028f5c,                                                  // 0.51                                        /// 0068c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00694
			 0x33333333,                                                  // 4 random values                             /// 00698
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d8
			 0x33333333,                                                  // 4 random values                             /// 006dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006e8
			 0xff800000,                                                  // -inf                                        /// 006ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00708
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0070c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0071c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00720
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00728
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00730
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00738
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0073c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00748
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0074c
			 0x7f800000,                                                  // inf                                         /// 00750
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00754
			 0x00011111,                                                  // 9.7958E-41                                  /// 00758
			 0x0e000007,                                                  // Trailing 1s:                                /// 0075c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00760
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00764
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00768
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00770
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00778
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0077c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00780
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00784
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00798
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0079c
			 0x55555555,                                                  // 4 random values                             /// 007a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007ac
			 0x80000000,                                                  // -zero                                       /// 007b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x7f800000,                                                  // inf                                         /// 007c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d8
			 0x7fc00001,                                                  // signaling NaN                               /// 007dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00800
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00804
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00808
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0080c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00820
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00828
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00834
			 0x80000000,                                                  // -zero                                       /// 00838
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0083c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00840
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00848
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0084c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00850
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00858
			 0xcb000000,                                                  // -8388608.0                                  /// 0085c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0xcb000000,                                                  // -8388608.0                                  /// 00864
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0086c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00870
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00878
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0087c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00880
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00884
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00888
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0088c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00890
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00894
			 0x7f800000,                                                  // inf                                         /// 00898
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0089c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a8
			 0x7fc00001,                                                  // signaling NaN                               /// 008ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xffc00001,                                                  // -signaling NaN                              /// 008cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008d4
			 0x33333333,                                                  // 4 random values                             /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e0
			 0xff800000,                                                  // -inf                                        /// 008e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00900
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00904
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00908
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0090c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00910
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00914
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0091c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00924
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00930
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00938
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0093c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00940
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00944
			 0x7f800000,                                                  // inf                                         /// 00948
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00950
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00954
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00960
			 0x0e000007,                                                  // Trailing 1s:                                /// 00964
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0096c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00974
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00980
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0098c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00994
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00998
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x7fc00001,                                                  // signaling NaN                               /// 009a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000000,                                                  // zero                                        /// 00a20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a88
			 0x7f800000,                                                  // inf                                         /// 00a8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aa4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ab0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ab4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ab8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ac8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00acc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00adc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00af8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x55555555,                                                  // 4 random values                             /// 00b18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b40
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xff800000,                                                  // -inf                                        /// 00b60
			 0x3f028f5c,                                                  // 0.51                                        /// 00b64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bb4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bf4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bfc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x7f800000,                                                  // inf                                         /// 00c28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0xcb000000,                                                  // -8388608.0                                  /// 00c44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c48
			 0xcb000000,                                                  // -8388608.0                                  /// 00c4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ca0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ccc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ce8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d30
			 0xff800000,                                                  // -inf                                        /// 00d34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d48
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x33333333,                                                  // 4 random values                             /// 00d7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dac
			 0x33333333,                                                  // 4 random values                             /// 00db0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00db4
			 0x7f800000,                                                  // inf                                         /// 00db8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00de8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0xcb000000,                                                  // -8388608.0                                  /// 00df4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e10
			 0xbf028f5c,                                                  // -0.51                                       /// 00e14
			 0x4b000000,                                                  // 8388608.0                                   /// 00e18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e90
			 0xbf028f5c,                                                  // -0.51                                       /// 00e94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eb0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eb4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ec0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ec8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ecc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed8
			 0x33333333,                                                  // 4 random values                             /// 00edc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00efc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x55555555,                                                  // 4 random values                             /// 00f24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xcb000000,                                                  // -8388608.0                                  /// 00f84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fa8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb8
			 0xff800000,                                                  // -inf                                        /// 00fbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc8
			 0x55555555,                                                  // 4 random values                             /// 00fcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fe0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fe8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ff8
			 0x0e000003                                                  // Trailing 1s:                                /// last
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
			 0x00000714,
			 0x00000130,
			 0x00000594,
			 0x000005f0,
			 0x00000008,
			 0x00000674,
			 0x0000016c,
			 0x00000738,

			 /// vpu register f2
			 0x40400000,
			 0x41e80000,
			 0x41800000,
			 0x40000000,
			 0x41200000,
			 0x41300000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f3
			 0x40000000,
			 0x41500000,
			 0x41900000,
			 0x41f00000,
			 0x41b80000,
			 0x41800000,
			 0x41600000,
			 0x41f00000,

			 /// vpu register f4
			 0x41c80000,
			 0x41100000,
			 0x41800000,
			 0x41a80000,
			 0x41d80000,
			 0x40800000,
			 0x40400000,
			 0x41200000,

			 /// vpu register f5
			 0x41b00000,
			 0x40400000,
			 0x40a00000,
			 0x41d00000,
			 0x42000000,
			 0x41400000,
			 0x41f80000,
			 0x41f80000,

			 /// vpu register f6
			 0x41a80000,
			 0x41500000,
			 0x42000000,
			 0x41a80000,
			 0x41d80000,
			 0x41880000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f7
			 0x41800000,
			 0x40400000,
			 0x40e00000,
			 0x41400000,
			 0x41880000,
			 0x41300000,
			 0x40e00000,
			 0x40000000,

			 /// vpu register f8
			 0x41e80000,
			 0x40a00000,
			 0x41c00000,
			 0x41400000,
			 0x40800000,
			 0x41d80000,
			 0x3f800000,
			 0x40c00000,

			 /// vpu register f9
			 0x41200000,
			 0x3f800000,
			 0x40a00000,
			 0x42000000,
			 0x41200000,
			 0x41800000,
			 0x41a80000,
			 0x41f80000,

			 /// vpu register f10
			 0x41400000,
			 0x41200000,
			 0x3f800000,
			 0x40c00000,
			 0x3f800000,
			 0x41400000,
			 0x41f00000,
			 0x41a00000,

			 /// vpu register f11
			 0x41c00000,
			 0x41e00000,
			 0x41f00000,
			 0x40000000,
			 0x40400000,
			 0x40a00000,
			 0x41700000,
			 0x41800000,

			 /// vpu register f12
			 0x41800000,
			 0x40e00000,
			 0x40c00000,
			 0x41a80000,
			 0x41d80000,
			 0x40e00000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f13
			 0x41c80000,
			 0x40a00000,
			 0x41a00000,
			 0x41c00000,
			 0x41a00000,
			 0x41000000,
			 0x41980000,
			 0x41700000,

			 /// vpu register f14
			 0x41400000,
			 0x41300000,
			 0x41e00000,
			 0x40800000,
			 0x41980000,
			 0x42000000,
			 0x41b00000,
			 0x40000000,

			 /// vpu register f15
			 0x41f80000,
			 0x41400000,
			 0x41e00000,
			 0x41200000,
			 0x41e80000,
			 0x40a00000,
			 0x41300000,
			 0x41880000,

			 /// vpu register f16
			 0x41e80000,
			 0x41500000,
			 0x40800000,
			 0x41100000,
			 0x41800000,
			 0x41600000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f17
			 0x41c80000,
			 0x41400000,
			 0x3f800000,
			 0x40e00000,
			 0x40000000,
			 0x41000000,
			 0x41000000,
			 0x41400000,

			 /// vpu register f18
			 0x41c00000,
			 0x40a00000,
			 0x41400000,
			 0x41300000,
			 0x41b80000,
			 0x41980000,
			 0x41700000,
			 0x41800000,

			 /// vpu register f19
			 0x41f00000,
			 0x41b00000,
			 0x41c00000,
			 0x42000000,
			 0x3f800000,
			 0x3f800000,
			 0x41300000,
			 0x41e00000,

			 /// vpu register f20
			 0x41c80000,
			 0x41f80000,
			 0x41900000,
			 0x41000000,
			 0x41980000,
			 0x41900000,
			 0x42000000,
			 0x41880000,

			 /// vpu register f21
			 0x41880000,
			 0x41c80000,
			 0x41e80000,
			 0x40e00000,
			 0x41a80000,
			 0x41500000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f22
			 0x41100000,
			 0x41100000,
			 0x41400000,
			 0x40800000,
			 0x40400000,
			 0x41c80000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f23
			 0x41600000,
			 0x41b00000,
			 0x40800000,
			 0x41500000,
			 0x41400000,
			 0x41000000,
			 0x40800000,
			 0x41f00000,

			 /// vpu register f24
			 0x41800000,
			 0x41d00000,
			 0x41500000,
			 0x41400000,
			 0x40a00000,
			 0x42000000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f25
			 0x41500000,
			 0x41900000,
			 0x41300000,
			 0x40000000,
			 0x40c00000,
			 0x41600000,
			 0x41000000,
			 0x41b00000,

			 /// vpu register f26
			 0x41a80000,
			 0x42000000,
			 0x41e80000,
			 0x41c80000,
			 0x41800000,
			 0x41c80000,
			 0x41c80000,
			 0x41c00000,

			 /// vpu register f27
			 0x41e00000,
			 0x41880000,
			 0x41f80000,
			 0x41000000,
			 0x41000000,
			 0x41d00000,
			 0x40400000,
			 0x41200000,

			 /// vpu register f28
			 0x40e00000,
			 0x41900000,
			 0x41e80000,
			 0x41700000,
			 0x40400000,
			 0x41d80000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f29
			 0x41b00000,
			 0x41100000,
			 0x40400000,
			 0x41500000,
			 0x41980000,
			 0x41f00000,
			 0x41900000,
			 0x41800000,

			 /// vpu register f30
			 0x40400000,
			 0x41100000,
			 0x41a80000,
			 0x40a00000,
			 0x41b00000,
			 0x40c00000,
			 0x40800000,
			 0x40a00000,

			 /// vpu register f31
			 0x41400000,
			 0x41300000,
			 0x3f800000,
			 0x40c00000,
			 0x41c80000,
			 0x41300000,
			 0x41500000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f16, f16\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f15, f3\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f2, f3\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f6, f10\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f6\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f23, f24\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f6, f18\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f29, f12\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f22, f26\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f3\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f23, f12\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f5, f24\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f30, f23\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f23, f8\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f11, f26\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f18, f10\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f3, f28\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f29, f27\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f29, f21\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f27, f9\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f20, f13\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f23, f27\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f5, f26\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f23, f2\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f17\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f20, f8\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f23\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f21, f6\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f9, f18\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f6, f30\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f14, f4\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f22, f1\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f8, f14\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f10, f14\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f22, f1\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f29, f7\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f23, f19\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f21, f15\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f25, f28\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f11, f28\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f11, f30\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f10, f10\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f8, f29\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f15\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f27, f26\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f22, f0\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f12, f11\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f7, f24\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f30, f17\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f18\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f29, f19\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f20\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f28, f30\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f20, f0\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f12\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f7, f19\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f15\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f11, f24\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f2, f24\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f7, f24\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f14\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f2, f1\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f27, f0\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f11, f16\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f14, f26\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f10, f12\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f17, f7\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f21, f24\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f9, f24\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f9, f5\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f15, f21\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f14\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f6, f6\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f25, f29\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f13, f3\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f7, f12\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f25, f14\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f26, f14\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f2, f17\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f6, f30\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f10, f21\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f22, f5\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f28, f11\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f28, f12\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f18, f25\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f8, f30\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f27, f11\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f25, f10\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f24, f10\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f29\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f12, f4\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f13, f22\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f2, f2\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f25, f23\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f13, f22\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f16\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f21, f6\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f26, f25\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un10 f19, f21\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
