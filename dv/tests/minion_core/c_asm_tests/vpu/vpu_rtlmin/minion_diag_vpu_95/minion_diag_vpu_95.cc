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
			 0x28268523, /// 0x0
			 0x6b4aa24c, /// 0x4
			 0x724a65b3, /// 0x8
			 0x2b74e3c5, /// 0xc
			 0x8a29f0ac, /// 0x10
			 0x409a7a01, /// 0x14
			 0x79a3606a, /// 0x18
			 0x68a721b7, /// 0x1c
			 0x3752ec12, /// 0x20
			 0x5236ac01, /// 0x24
			 0x02141290, /// 0x28
			 0x8bcbe85e, /// 0x2c
			 0x4fcd1244, /// 0x30
			 0xa5ebf63d, /// 0x34
			 0x9760094c, /// 0x38
			 0x1ce50d9a, /// 0x3c
			 0x50b7b33f, /// 0x40
			 0x555b2867, /// 0x44
			 0xe07f4d73, /// 0x48
			 0xe50b4cfc, /// 0x4c
			 0xc1235bdf, /// 0x50
			 0xea783bd9, /// 0x54
			 0x568d45ed, /// 0x58
			 0xaf92206d, /// 0x5c
			 0xddc9e4f6, /// 0x60
			 0x1c60982e, /// 0x64
			 0xc1284917, /// 0x68
			 0xf68d835b, /// 0x6c
			 0x05b7ec47, /// 0x70
			 0x8d9e65f7, /// 0x74
			 0xa2e23bc5, /// 0x78
			 0x11aecfc2, /// 0x7c
			 0x2744129f, /// 0x80
			 0x7d048578, /// 0x84
			 0x8651c838, /// 0x88
			 0xc5882766, /// 0x8c
			 0xda8b0a88, /// 0x90
			 0xbe654d76, /// 0x94
			 0xe180b77e, /// 0x98
			 0x5f4b999b, /// 0x9c
			 0x1bd00790, /// 0xa0
			 0x0604008e, /// 0xa4
			 0x74b43dee, /// 0xa8
			 0xeb6d80e6, /// 0xac
			 0x0d8af3c5, /// 0xb0
			 0xe70ba339, /// 0xb4
			 0x3cac8031, /// 0xb8
			 0x4b7a3608, /// 0xbc
			 0x6a379395, /// 0xc0
			 0x9f2a026e, /// 0xc4
			 0x7e5fd1fa, /// 0xc8
			 0x0d3fe063, /// 0xcc
			 0x39e93e25, /// 0xd0
			 0x305ae1eb, /// 0xd4
			 0x28b1fbcb, /// 0xd8
			 0x6bb23220, /// 0xdc
			 0xbdc4bf1c, /// 0xe0
			 0xfd2cca1a, /// 0xe4
			 0x89b32bbb, /// 0xe8
			 0xc343f853, /// 0xec
			 0x9340e42d, /// 0xf0
			 0xbe92ef66, /// 0xf4
			 0x93265f29, /// 0xf8
			 0x36def19e, /// 0xfc
			 0x9d690987, /// 0x100
			 0x6d061780, /// 0x104
			 0x4fc58723, /// 0x108
			 0x27602060, /// 0x10c
			 0xf59c6d24, /// 0x110
			 0xa44b4783, /// 0x114
			 0x6ca4f18d, /// 0x118
			 0x7c0e4341, /// 0x11c
			 0x0be69a8f, /// 0x120
			 0x8b9fc8ee, /// 0x124
			 0x59e9400d, /// 0x128
			 0x11725e8b, /// 0x12c
			 0x8105eafd, /// 0x130
			 0xa23f5429, /// 0x134
			 0x08dd9413, /// 0x138
			 0x0ba906ea, /// 0x13c
			 0xcd516c50, /// 0x140
			 0x6a2b9e89, /// 0x144
			 0xabf30b81, /// 0x148
			 0x7d5fa426, /// 0x14c
			 0xd34791e2, /// 0x150
			 0x156b4497, /// 0x154
			 0x83ea7ce5, /// 0x158
			 0x2a2d0005, /// 0x15c
			 0x1207ae15, /// 0x160
			 0xb2b1d81e, /// 0x164
			 0xcd684360, /// 0x168
			 0xc07ec4ef, /// 0x16c
			 0x85c5f346, /// 0x170
			 0xd1fc53f2, /// 0x174
			 0x403a4a8f, /// 0x178
			 0xd8b700de, /// 0x17c
			 0x20d00b1a, /// 0x180
			 0x461cd877, /// 0x184
			 0xcd5297db, /// 0x188
			 0xc4bc18ff, /// 0x18c
			 0xb83e23c5, /// 0x190
			 0xad1b850b, /// 0x194
			 0x6b1dbe40, /// 0x198
			 0xefe15cfa, /// 0x19c
			 0x480cafac, /// 0x1a0
			 0x4ed4dd39, /// 0x1a4
			 0x0ef286ca, /// 0x1a8
			 0xb9cdc8a3, /// 0x1ac
			 0x326b65e0, /// 0x1b0
			 0x2deb2108, /// 0x1b4
			 0xf96c288c, /// 0x1b8
			 0x859bcc0d, /// 0x1bc
			 0x7bbed62b, /// 0x1c0
			 0xca3e2547, /// 0x1c4
			 0x15fdba0c, /// 0x1c8
			 0xc14d933e, /// 0x1cc
			 0x632bc101, /// 0x1d0
			 0xe8326c50, /// 0x1d4
			 0x4b512d93, /// 0x1d8
			 0xec09e816, /// 0x1dc
			 0xcca8ffac, /// 0x1e0
			 0x371e2d0f, /// 0x1e4
			 0x3ca20162, /// 0x1e8
			 0x65c811bc, /// 0x1ec
			 0x6cedcc05, /// 0x1f0
			 0xc029eebf, /// 0x1f4
			 0x933badde, /// 0x1f8
			 0x2b5c360d, /// 0x1fc
			 0x2bd06298, /// 0x200
			 0xc4b6d4b4, /// 0x204
			 0xb2518862, /// 0x208
			 0xcd01c0d3, /// 0x20c
			 0x3d82e6cd, /// 0x210
			 0x59afbcbe, /// 0x214
			 0x7b2b5410, /// 0x218
			 0x9645d899, /// 0x21c
			 0xd7989cc8, /// 0x220
			 0x5fe6e578, /// 0x224
			 0x9005ceb6, /// 0x228
			 0xcad2b6b1, /// 0x22c
			 0x0ec77495, /// 0x230
			 0x06ddc09f, /// 0x234
			 0x915566cd, /// 0x238
			 0xcabf2d80, /// 0x23c
			 0xc3d08173, /// 0x240
			 0x6cd2c884, /// 0x244
			 0x683a8889, /// 0x248
			 0xf2938593, /// 0x24c
			 0x7a9215fa, /// 0x250
			 0x621b04c6, /// 0x254
			 0x3853c278, /// 0x258
			 0xf071ac24, /// 0x25c
			 0xb91af04b, /// 0x260
			 0xfa1b42f7, /// 0x264
			 0x873e4776, /// 0x268
			 0x1ca048e0, /// 0x26c
			 0x44ce9b85, /// 0x270
			 0xddb5e007, /// 0x274
			 0x56daf815, /// 0x278
			 0x699849b4, /// 0x27c
			 0x51daf3a5, /// 0x280
			 0xb57bbc14, /// 0x284
			 0x19b2a983, /// 0x288
			 0x574dfff3, /// 0x28c
			 0xaf1c89c7, /// 0x290
			 0x487c9ff1, /// 0x294
			 0x06f1c931, /// 0x298
			 0x95ef33ef, /// 0x29c
			 0xcf09ef60, /// 0x2a0
			 0x8b26c515, /// 0x2a4
			 0x7702f361, /// 0x2a8
			 0xbc27aa19, /// 0x2ac
			 0x2d9354b9, /// 0x2b0
			 0x294f7e9b, /// 0x2b4
			 0x71bc718b, /// 0x2b8
			 0x2ebce3db, /// 0x2bc
			 0x7b193e4d, /// 0x2c0
			 0xc81c7f52, /// 0x2c4
			 0xaad6aa6d, /// 0x2c8
			 0x99c1c940, /// 0x2cc
			 0x49701d22, /// 0x2d0
			 0x51ae528d, /// 0x2d4
			 0x21830775, /// 0x2d8
			 0x4f749a4a, /// 0x2dc
			 0x1548b7a2, /// 0x2e0
			 0xd696679f, /// 0x2e4
			 0x237b9d05, /// 0x2e8
			 0x7cf9ff3b, /// 0x2ec
			 0x954ff9c3, /// 0x2f0
			 0x1d6141a2, /// 0x2f4
			 0x530e6ec2, /// 0x2f8
			 0x38429ef9, /// 0x2fc
			 0x7b92dfd1, /// 0x300
			 0xe764d58f, /// 0x304
			 0x1d48c06a, /// 0x308
			 0x0b0ee206, /// 0x30c
			 0xfb9ed293, /// 0x310
			 0x6abc3187, /// 0x314
			 0x3f4d7fa6, /// 0x318
			 0x64e7a8e8, /// 0x31c
			 0x7214780c, /// 0x320
			 0xda6786e0, /// 0x324
			 0x01a6204a, /// 0x328
			 0x54f343cc, /// 0x32c
			 0x7e904dfa, /// 0x330
			 0xdab3d53d, /// 0x334
			 0xf4ed2e67, /// 0x338
			 0xf6b564f1, /// 0x33c
			 0x8c1f3c18, /// 0x340
			 0x5bcd3265, /// 0x344
			 0x4d3881c7, /// 0x348
			 0xaba32605, /// 0x34c
			 0xb06865e6, /// 0x350
			 0x4803a961, /// 0x354
			 0xdad2cde3, /// 0x358
			 0xf9bb5fb5, /// 0x35c
			 0x4ef444be, /// 0x360
			 0xae855c6e, /// 0x364
			 0x73330a0f, /// 0x368
			 0x55a1e601, /// 0x36c
			 0x490d9244, /// 0x370
			 0xa97c4418, /// 0x374
			 0xd745f3bf, /// 0x378
			 0xdcf0c758, /// 0x37c
			 0x17d394ba, /// 0x380
			 0x2c1c0f8d, /// 0x384
			 0x8d107357, /// 0x388
			 0x5ab369af, /// 0x38c
			 0x30d58287, /// 0x390
			 0x9c83002a, /// 0x394
			 0x5fc22a00, /// 0x398
			 0x1cff6474, /// 0x39c
			 0xc7284780, /// 0x3a0
			 0xe8286c40, /// 0x3a4
			 0x42c833d9, /// 0x3a8
			 0xab0d2505, /// 0x3ac
			 0x44403b8c, /// 0x3b0
			 0xe49cd996, /// 0x3b4
			 0x4518eddd, /// 0x3b8
			 0x4d57b675, /// 0x3bc
			 0xf678919a, /// 0x3c0
			 0x5bef076c, /// 0x3c4
			 0x27787a8d, /// 0x3c8
			 0x1b60a294, /// 0x3cc
			 0xf8018e13, /// 0x3d0
			 0xaa063f11, /// 0x3d4
			 0x3f774ebb, /// 0x3d8
			 0x00adbf9f, /// 0x3dc
			 0xd795f921, /// 0x3e0
			 0xcbc72597, /// 0x3e4
			 0xc0f13c92, /// 0x3e8
			 0xb3f15401, /// 0x3ec
			 0x99171fd2, /// 0x3f0
			 0xa78e8de6, /// 0x3f4
			 0xfb20b62d, /// 0x3f8
			 0x8f820481, /// 0x3fc
			 0x4d5d4acc, /// 0x400
			 0x4559066c, /// 0x404
			 0xf89545d0, /// 0x408
			 0x12972f6a, /// 0x40c
			 0x654f12af, /// 0x410
			 0x8661aa7e, /// 0x414
			 0x0346a58f, /// 0x418
			 0x36f875f5, /// 0x41c
			 0xd2b4f427, /// 0x420
			 0x3afa3e72, /// 0x424
			 0x351e4c3e, /// 0x428
			 0xe8bab7aa, /// 0x42c
			 0xfd9a01fa, /// 0x430
			 0xb58107b6, /// 0x434
			 0x697611c8, /// 0x438
			 0x4ec171d3, /// 0x43c
			 0xa5258aa0, /// 0x440
			 0x1240ad53, /// 0x444
			 0x48fcff91, /// 0x448
			 0xf15e5791, /// 0x44c
			 0x3fd16e10, /// 0x450
			 0xf7972e5b, /// 0x454
			 0xbaa1ab20, /// 0x458
			 0x26802b9e, /// 0x45c
			 0xeb2003f0, /// 0x460
			 0x0d4c83f4, /// 0x464
			 0x2f36b209, /// 0x468
			 0x65f2a8d2, /// 0x46c
			 0xa6f7800d, /// 0x470
			 0x13227bc8, /// 0x474
			 0xd81c0d04, /// 0x478
			 0x83d27154, /// 0x47c
			 0xe778970a, /// 0x480
			 0x8a243187, /// 0x484
			 0x54a6ff54, /// 0x488
			 0x90690eb0, /// 0x48c
			 0xb105b2cc, /// 0x490
			 0xd0c378f0, /// 0x494
			 0x239928e5, /// 0x498
			 0x45594346, /// 0x49c
			 0xcf9d8d8e, /// 0x4a0
			 0x7bf3fe68, /// 0x4a4
			 0xcfd4b1db, /// 0x4a8
			 0x012a4012, /// 0x4ac
			 0xc3b8b828, /// 0x4b0
			 0x29f70de2, /// 0x4b4
			 0xed8553ef, /// 0x4b8
			 0x1734fbdb, /// 0x4bc
			 0x65ed49f8, /// 0x4c0
			 0x3407df96, /// 0x4c4
			 0xaa07c5d8, /// 0x4c8
			 0x285f6878, /// 0x4cc
			 0xe94ec0f9, /// 0x4d0
			 0x80a2fce2, /// 0x4d4
			 0x5878c2c4, /// 0x4d8
			 0xf8513138, /// 0x4dc
			 0x196c420a, /// 0x4e0
			 0xe5c38e75, /// 0x4e4
			 0x5ba73bcf, /// 0x4e8
			 0x3bf30758, /// 0x4ec
			 0x8a567b26, /// 0x4f0
			 0x0d2d5ab1, /// 0x4f4
			 0x6725942c, /// 0x4f8
			 0x5f498358, /// 0x4fc
			 0x82033671, /// 0x500
			 0x050f8e79, /// 0x504
			 0xa5ee07c6, /// 0x508
			 0xd5b72850, /// 0x50c
			 0xd02c744b, /// 0x510
			 0x899545ee, /// 0x514
			 0xc2d31ca4, /// 0x518
			 0x2c035b8a, /// 0x51c
			 0x799f812a, /// 0x520
			 0xdd171598, /// 0x524
			 0xf1823a87, /// 0x528
			 0x0afff23e, /// 0x52c
			 0xbd06be01, /// 0x530
			 0xa8674293, /// 0x534
			 0x754c2160, /// 0x538
			 0x3bdf975f, /// 0x53c
			 0xc0b4943c, /// 0x540
			 0x78436621, /// 0x544
			 0xc1d4c353, /// 0x548
			 0x1e8b4da6, /// 0x54c
			 0xed90e25e, /// 0x550
			 0x6a01a0c6, /// 0x554
			 0x20f81e98, /// 0x558
			 0xc7b47f29, /// 0x55c
			 0xf4e6c4c0, /// 0x560
			 0xf7dd0413, /// 0x564
			 0xef05b3ca, /// 0x568
			 0x99533e75, /// 0x56c
			 0x020c77b5, /// 0x570
			 0xe281e4b3, /// 0x574
			 0x87f136ab, /// 0x578
			 0x16ee40ff, /// 0x57c
			 0x9af15c00, /// 0x580
			 0x57f7dded, /// 0x584
			 0xb655c543, /// 0x588
			 0x69e0aece, /// 0x58c
			 0x8b4bfdc8, /// 0x590
			 0x7a215509, /// 0x594
			 0x9c5027b4, /// 0x598
			 0x543c43aa, /// 0x59c
			 0xafe74baf, /// 0x5a0
			 0xd4b1dc74, /// 0x5a4
			 0x4989c20d, /// 0x5a8
			 0x3e278cd2, /// 0x5ac
			 0xd60b2ea6, /// 0x5b0
			 0xf5a8bab9, /// 0x5b4
			 0xfd1124ff, /// 0x5b8
			 0x770ad117, /// 0x5bc
			 0xf5cd6d4b, /// 0x5c0
			 0x3a3d61f8, /// 0x5c4
			 0x1d1f4dc8, /// 0x5c8
			 0x4c209062, /// 0x5cc
			 0xbd308576, /// 0x5d0
			 0x7e049d63, /// 0x5d4
			 0x7c23833d, /// 0x5d8
			 0xdafb2c61, /// 0x5dc
			 0x01d55188, /// 0x5e0
			 0x73849452, /// 0x5e4
			 0x9eb27363, /// 0x5e8
			 0x36bfb77a, /// 0x5ec
			 0x796916eb, /// 0x5f0
			 0x533e6306, /// 0x5f4
			 0xd9283642, /// 0x5f8
			 0x301939fb, /// 0x5fc
			 0xa9b7d724, /// 0x600
			 0xf61dd0b5, /// 0x604
			 0x339d7e4b, /// 0x608
			 0x9eea32f7, /// 0x60c
			 0xb318cfe1, /// 0x610
			 0x23089d09, /// 0x614
			 0x6a2854bd, /// 0x618
			 0xba7569e0, /// 0x61c
			 0xc933638e, /// 0x620
			 0x0f61ce1d, /// 0x624
			 0x13aaf036, /// 0x628
			 0x99617865, /// 0x62c
			 0xc06b78b2, /// 0x630
			 0x59a65be8, /// 0x634
			 0xcae86429, /// 0x638
			 0xeddabff9, /// 0x63c
			 0x8fb27032, /// 0x640
			 0xb93ca29f, /// 0x644
			 0x5805a306, /// 0x648
			 0x3bcb661f, /// 0x64c
			 0xdd342834, /// 0x650
			 0xa06e7bdb, /// 0x654
			 0x89269d93, /// 0x658
			 0x7945d6b4, /// 0x65c
			 0x217b91bb, /// 0x660
			 0xc4dc5496, /// 0x664
			 0xe067f19e, /// 0x668
			 0x34d4b917, /// 0x66c
			 0x15d88d42, /// 0x670
			 0xc7472bb8, /// 0x674
			 0x828a310a, /// 0x678
			 0xc837f07f, /// 0x67c
			 0xbe42a0ba, /// 0x680
			 0x08730d75, /// 0x684
			 0x16c007dc, /// 0x688
			 0xfe13bfbd, /// 0x68c
			 0x5bb4c04e, /// 0x690
			 0x14f42002, /// 0x694
			 0x1daaccea, /// 0x698
			 0x1a0a5d31, /// 0x69c
			 0x01d3fb37, /// 0x6a0
			 0x06c29fb1, /// 0x6a4
			 0xfeca7b8c, /// 0x6a8
			 0xbabd4d51, /// 0x6ac
			 0x986b6706, /// 0x6b0
			 0x043fa3c3, /// 0x6b4
			 0x9062accb, /// 0x6b8
			 0xcab93fff, /// 0x6bc
			 0x609a446e, /// 0x6c0
			 0x3f5a18fc, /// 0x6c4
			 0x7733434d, /// 0x6c8
			 0xe0c0ee77, /// 0x6cc
			 0xf590057d, /// 0x6d0
			 0xe7a2870b, /// 0x6d4
			 0x0a7f96b0, /// 0x6d8
			 0x67f2bd6f, /// 0x6dc
			 0x6755a403, /// 0x6e0
			 0xa7ce9878, /// 0x6e4
			 0x66ba6e9d, /// 0x6e8
			 0x5a2cd41a, /// 0x6ec
			 0x3a8ce6d9, /// 0x6f0
			 0x5e03b183, /// 0x6f4
			 0x4554165d, /// 0x6f8
			 0x8879f9ea, /// 0x6fc
			 0x5e967e3a, /// 0x700
			 0x24c6ccae, /// 0x704
			 0xcdc3ec93, /// 0x708
			 0xba7ae601, /// 0x70c
			 0xcb161467, /// 0x710
			 0x7734ac4e, /// 0x714
			 0xb9f3c574, /// 0x718
			 0xd178f491, /// 0x71c
			 0x621cd7c4, /// 0x720
			 0x1618244d, /// 0x724
			 0x158a3d7b, /// 0x728
			 0xfae90a9a, /// 0x72c
			 0xdd8e4ebf, /// 0x730
			 0x62c8fc01, /// 0x734
			 0x48f53fd9, /// 0x738
			 0xe588c23e, /// 0x73c
			 0x40aa7be4, /// 0x740
			 0xe29cf7a4, /// 0x744
			 0x4ad3b03d, /// 0x748
			 0xe2257d7a, /// 0x74c
			 0xc970aa1b, /// 0x750
			 0x69c70cd9, /// 0x754
			 0x01a14dce, /// 0x758
			 0x7ff729f1, /// 0x75c
			 0xb6c76716, /// 0x760
			 0x99b02bac, /// 0x764
			 0xca635c1d, /// 0x768
			 0xd23c51ba, /// 0x76c
			 0x17803a92, /// 0x770
			 0x9bddecd9, /// 0x774
			 0xf7a672f9, /// 0x778
			 0xc62a8d1b, /// 0x77c
			 0x67e0af06, /// 0x780
			 0xed4c1a34, /// 0x784
			 0xd64621bc, /// 0x788
			 0x84f8ee0b, /// 0x78c
			 0x3dd945b2, /// 0x790
			 0xef44b2da, /// 0x794
			 0x629bd82b, /// 0x798
			 0xede76b54, /// 0x79c
			 0x83c17bd9, /// 0x7a0
			 0x91d5faf0, /// 0x7a4
			 0x0eaf7c97, /// 0x7a8
			 0x5d6a59f0, /// 0x7ac
			 0x36ed978a, /// 0x7b0
			 0xa65d569c, /// 0x7b4
			 0xddc76c62, /// 0x7b8
			 0x042789eb, /// 0x7bc
			 0x0f2036fd, /// 0x7c0
			 0x00bf3c5d, /// 0x7c4
			 0x6a529b33, /// 0x7c8
			 0x6312bb52, /// 0x7cc
			 0x81841ee7, /// 0x7d0
			 0x4d9a53ce, /// 0x7d4
			 0x2244924b, /// 0x7d8
			 0x49eaff62, /// 0x7dc
			 0x925f3e2a, /// 0x7e0
			 0xe8ead378, /// 0x7e4
			 0xe3fede43, /// 0x7e8
			 0x282f5dd0, /// 0x7ec
			 0xd3171023, /// 0x7f0
			 0x1d80407a, /// 0x7f4
			 0x94ba01a2, /// 0x7f8
			 0x9d03fe5e, /// 0x7fc
			 0xe9359063, /// 0x800
			 0xb4ea430d, /// 0x804
			 0x66528222, /// 0x808
			 0x3f864e1f, /// 0x80c
			 0xb9117d08, /// 0x810
			 0xa1625faf, /// 0x814
			 0x1c5a5f0f, /// 0x818
			 0xfbe15acf, /// 0x81c
			 0x28b74164, /// 0x820
			 0x8d821b11, /// 0x824
			 0x3a9de3c1, /// 0x828
			 0x91465bc4, /// 0x82c
			 0x2d546b97, /// 0x830
			 0xba27632a, /// 0x834
			 0xc7f2470d, /// 0x838
			 0x1cf13d71, /// 0x83c
			 0x84f6a634, /// 0x840
			 0x2939ac8b, /// 0x844
			 0xf9e3fb8b, /// 0x848
			 0x313f30b3, /// 0x84c
			 0xe207b240, /// 0x850
			 0xb4648e20, /// 0x854
			 0xb9a2284e, /// 0x858
			 0xbac844a7, /// 0x85c
			 0x488691e1, /// 0x860
			 0x8756f1e8, /// 0x864
			 0x107b1027, /// 0x868
			 0xe872aea3, /// 0x86c
			 0x6dbe7663, /// 0x870
			 0x31bc4018, /// 0x874
			 0x5affc35c, /// 0x878
			 0xa9f6af46, /// 0x87c
			 0x8b4142c5, /// 0x880
			 0x689f85b2, /// 0x884
			 0x44cb7158, /// 0x888
			 0x14c4753d, /// 0x88c
			 0x4714ea92, /// 0x890
			 0xea37179f, /// 0x894
			 0xb338c1f5, /// 0x898
			 0x7b93c9f5, /// 0x89c
			 0x031ec647, /// 0x8a0
			 0x5db00e94, /// 0x8a4
			 0x3df51e45, /// 0x8a8
			 0x7ed95e70, /// 0x8ac
			 0x6e384f60, /// 0x8b0
			 0x27130975, /// 0x8b4
			 0xcb4b5616, /// 0x8b8
			 0x6feb30bf, /// 0x8bc
			 0x2f8fa03f, /// 0x8c0
			 0xa467b8c4, /// 0x8c4
			 0x93b3b528, /// 0x8c8
			 0xd69d04d2, /// 0x8cc
			 0xe0ca9b9f, /// 0x8d0
			 0x9f376933, /// 0x8d4
			 0x68bacb89, /// 0x8d8
			 0xd253df9b, /// 0x8dc
			 0x5229208f, /// 0x8e0
			 0xe0aacca4, /// 0x8e4
			 0x587eec20, /// 0x8e8
			 0x7003c03b, /// 0x8ec
			 0xda45f2c9, /// 0x8f0
			 0x8550ab35, /// 0x8f4
			 0xff320c74, /// 0x8f8
			 0xadafc575, /// 0x8fc
			 0x54d826af, /// 0x900
			 0x315f5da8, /// 0x904
			 0x877b7c72, /// 0x908
			 0xf132897a, /// 0x90c
			 0x8b465528, /// 0x910
			 0xeb226b94, /// 0x914
			 0x634f511d, /// 0x918
			 0x197a53ea, /// 0x91c
			 0x0c1a9ace, /// 0x920
			 0xbb6c7091, /// 0x924
			 0x175848c2, /// 0x928
			 0x61ded7b1, /// 0x92c
			 0xfe401e6d, /// 0x930
			 0x7676e6b2, /// 0x934
			 0x54577407, /// 0x938
			 0x5f04cc57, /// 0x93c
			 0x08a41c44, /// 0x940
			 0x71560fff, /// 0x944
			 0x5cd8b849, /// 0x948
			 0x14761059, /// 0x94c
			 0x4b81d60f, /// 0x950
			 0xdcc1fb97, /// 0x954
			 0xacd66f39, /// 0x958
			 0xb39b4977, /// 0x95c
			 0x10511784, /// 0x960
			 0x2525cb73, /// 0x964
			 0x81c325dc, /// 0x968
			 0x2b269061, /// 0x96c
			 0x10c32e64, /// 0x970
			 0xf1df0e2a, /// 0x974
			 0x479875d2, /// 0x978
			 0x76e9d04b, /// 0x97c
			 0x1531c84f, /// 0x980
			 0x15bf6db1, /// 0x984
			 0x106a2e0e, /// 0x988
			 0xd713bf7e, /// 0x98c
			 0xa9d8ddd8, /// 0x990
			 0xc8d5d530, /// 0x994
			 0x0505f029, /// 0x998
			 0x4608d33c, /// 0x99c
			 0x11997e43, /// 0x9a0
			 0x8deaebd3, /// 0x9a4
			 0x38a11c72, /// 0x9a8
			 0x3dfc4af1, /// 0x9ac
			 0x756d9d95, /// 0x9b0
			 0xa4fd49a7, /// 0x9b4
			 0x346ddd13, /// 0x9b8
			 0x0c41f3ea, /// 0x9bc
			 0xf2712883, /// 0x9c0
			 0x17c07249, /// 0x9c4
			 0x01f37533, /// 0x9c8
			 0x684ff571, /// 0x9cc
			 0x406d28c7, /// 0x9d0
			 0xe9f1ffb7, /// 0x9d4
			 0x31f51495, /// 0x9d8
			 0x8c24a159, /// 0x9dc
			 0xda381046, /// 0x9e0
			 0xb6bdc350, /// 0x9e4
			 0x815ab3f2, /// 0x9e8
			 0x132b1c6e, /// 0x9ec
			 0x4d9a89c3, /// 0x9f0
			 0x34c362e6, /// 0x9f4
			 0x218c933c, /// 0x9f8
			 0x8a24516b, /// 0x9fc
			 0x18408a41, /// 0xa00
			 0x082a6860, /// 0xa04
			 0xb4b8574d, /// 0xa08
			 0x5ee1cbb0, /// 0xa0c
			 0x194493a8, /// 0xa10
			 0x6d5ae3e4, /// 0xa14
			 0x0bc1b7ef, /// 0xa18
			 0x32873c39, /// 0xa1c
			 0xa30f9afe, /// 0xa20
			 0x059c91f0, /// 0xa24
			 0x33a0adfa, /// 0xa28
			 0xa3f0c9da, /// 0xa2c
			 0xabd042cb, /// 0xa30
			 0xba325bdc, /// 0xa34
			 0xe075be00, /// 0xa38
			 0xa40f7639, /// 0xa3c
			 0x1ecfe952, /// 0xa40
			 0x4a626b89, /// 0xa44
			 0x9d3830aa, /// 0xa48
			 0x711e47d6, /// 0xa4c
			 0xe5c373b8, /// 0xa50
			 0x73e19974, /// 0xa54
			 0x9a853288, /// 0xa58
			 0x09a0ea81, /// 0xa5c
			 0x8742d986, /// 0xa60
			 0x1b356aae, /// 0xa64
			 0xfbd35186, /// 0xa68
			 0x090e4a12, /// 0xa6c
			 0xcbd677f1, /// 0xa70
			 0xed10c527, /// 0xa74
			 0x376e284a, /// 0xa78
			 0x85870e79, /// 0xa7c
			 0x6ce75451, /// 0xa80
			 0x9fcde75a, /// 0xa84
			 0xce6a5913, /// 0xa88
			 0xdeaa61d2, /// 0xa8c
			 0xc021b7d9, /// 0xa90
			 0xf0ed6164, /// 0xa94
			 0x5ce29533, /// 0xa98
			 0x49226742, /// 0xa9c
			 0xba22f09d, /// 0xaa0
			 0xe57f74f2, /// 0xaa4
			 0x0c4ad3d3, /// 0xaa8
			 0x61334cdb, /// 0xaac
			 0xfc21deb2, /// 0xab0
			 0x5b37000b, /// 0xab4
			 0x005b0cb0, /// 0xab8
			 0xb64d592b, /// 0xabc
			 0x163c832b, /// 0xac0
			 0x684a6815, /// 0xac4
			 0x51906bb9, /// 0xac8
			 0x31879efb, /// 0xacc
			 0x6f8414ac, /// 0xad0
			 0x3ecf405c, /// 0xad4
			 0x3c0aa30e, /// 0xad8
			 0xb7684f64, /// 0xadc
			 0x3879796c, /// 0xae0
			 0x7164464a, /// 0xae4
			 0x2e9bf076, /// 0xae8
			 0xbaa7928b, /// 0xaec
			 0xcefc50d6, /// 0xaf0
			 0x06eaed8d, /// 0xaf4
			 0x45b1aa64, /// 0xaf8
			 0x0a38093a, /// 0xafc
			 0x3e24f2dd, /// 0xb00
			 0x0eca3d87, /// 0xb04
			 0x14ad0650, /// 0xb08
			 0xd527e093, /// 0xb0c
			 0x52b9d723, /// 0xb10
			 0x86ed41fa, /// 0xb14
			 0x4a06ac93, /// 0xb18
			 0x4096a5e0, /// 0xb1c
			 0xed0f64b4, /// 0xb20
			 0xcb18f8b3, /// 0xb24
			 0xc2d5fab2, /// 0xb28
			 0xac2faa78, /// 0xb2c
			 0xf39b5c2e, /// 0xb30
			 0x4d255cb7, /// 0xb34
			 0x9e279349, /// 0xb38
			 0x7de6113b, /// 0xb3c
			 0xfb952fab, /// 0xb40
			 0xcc3c7c48, /// 0xb44
			 0x7e281baa, /// 0xb48
			 0x9a4bf139, /// 0xb4c
			 0x07221f47, /// 0xb50
			 0xe9a86529, /// 0xb54
			 0xba5590fc, /// 0xb58
			 0x22ffed81, /// 0xb5c
			 0xea4248be, /// 0xb60
			 0xcffbc056, /// 0xb64
			 0xa37bcb4c, /// 0xb68
			 0x56a85eee, /// 0xb6c
			 0x34203f18, /// 0xb70
			 0x53576c94, /// 0xb74
			 0x4400e7ed, /// 0xb78
			 0x019defd4, /// 0xb7c
			 0xf69613d9, /// 0xb80
			 0xa447a5f0, /// 0xb84
			 0x874d6f99, /// 0xb88
			 0xd5aa24fa, /// 0xb8c
			 0xaedffd3c, /// 0xb90
			 0x70a9c894, /// 0xb94
			 0x084c8614, /// 0xb98
			 0x7489b5dd, /// 0xb9c
			 0x0edb8e90, /// 0xba0
			 0xa0554b3f, /// 0xba4
			 0x3a328d91, /// 0xba8
			 0xc763cad0, /// 0xbac
			 0xd6d3356b, /// 0xbb0
			 0x84d54130, /// 0xbb4
			 0xcabab581, /// 0xbb8
			 0x6c61ff87, /// 0xbbc
			 0x8eef912b, /// 0xbc0
			 0xc875771a, /// 0xbc4
			 0xb444edfe, /// 0xbc8
			 0x5ad8271c, /// 0xbcc
			 0xa6375430, /// 0xbd0
			 0x6c41651b, /// 0xbd4
			 0x02de30a8, /// 0xbd8
			 0x70f0a77b, /// 0xbdc
			 0x3c345250, /// 0xbe0
			 0x45d9e6f2, /// 0xbe4
			 0xec0922df, /// 0xbe8
			 0x9c734fde, /// 0xbec
			 0x7e36764e, /// 0xbf0
			 0xafb1848a, /// 0xbf4
			 0xd0004aa5, /// 0xbf8
			 0x13cd8af8, /// 0xbfc
			 0x02c66a6e, /// 0xc00
			 0x47b6d6a9, /// 0xc04
			 0x4c683763, /// 0xc08
			 0xc993b53a, /// 0xc0c
			 0x755981ca, /// 0xc10
			 0x52e315dc, /// 0xc14
			 0x3ca14701, /// 0xc18
			 0x7774274a, /// 0xc1c
			 0xcfb57d08, /// 0xc20
			 0x8cb12ee4, /// 0xc24
			 0x0f178d35, /// 0xc28
			 0x5569c9df, /// 0xc2c
			 0x84783e67, /// 0xc30
			 0xb70278f1, /// 0xc34
			 0xa55ac568, /// 0xc38
			 0xe1b07dc8, /// 0xc3c
			 0x8b8e62ce, /// 0xc40
			 0x76f81893, /// 0xc44
			 0x01467cb0, /// 0xc48
			 0x0556bf56, /// 0xc4c
			 0x5230bf92, /// 0xc50
			 0xbb490723, /// 0xc54
			 0x9ae93569, /// 0xc58
			 0xc326ca5c, /// 0xc5c
			 0x588500aa, /// 0xc60
			 0x425904f0, /// 0xc64
			 0x8761ba5d, /// 0xc68
			 0x0382ee6b, /// 0xc6c
			 0x89875d76, /// 0xc70
			 0x09a9abfc, /// 0xc74
			 0x284f67ee, /// 0xc78
			 0xdb7b22da, /// 0xc7c
			 0xd14e3263, /// 0xc80
			 0xc7fe9128, /// 0xc84
			 0xe8bc14ed, /// 0xc88
			 0xff52193c, /// 0xc8c
			 0x7f648436, /// 0xc90
			 0x21c26db9, /// 0xc94
			 0xb71235e3, /// 0xc98
			 0x89d2fc48, /// 0xc9c
			 0xdaebdfac, /// 0xca0
			 0xc2b546e8, /// 0xca4
			 0xc2458787, /// 0xca8
			 0x77c0845b, /// 0xcac
			 0x0a39704b, /// 0xcb0
			 0x94397c32, /// 0xcb4
			 0x18b5e559, /// 0xcb8
			 0x27a36e1f, /// 0xcbc
			 0xffa59ce1, /// 0xcc0
			 0xeeaee34b, /// 0xcc4
			 0x945748d1, /// 0xcc8
			 0xc7e08ad2, /// 0xccc
			 0x8430a0ef, /// 0xcd0
			 0x8e3bf8af, /// 0xcd4
			 0xde935fe9, /// 0xcd8
			 0x460aa174, /// 0xcdc
			 0x6e8c102a, /// 0xce0
			 0x26d51b4f, /// 0xce4
			 0xd20b333c, /// 0xce8
			 0xd336e515, /// 0xcec
			 0xd2bcf59f, /// 0xcf0
			 0x624fcd16, /// 0xcf4
			 0x5d3498d8, /// 0xcf8
			 0x4dfab487, /// 0xcfc
			 0xcf2ad02b, /// 0xd00
			 0xa7bd5809, /// 0xd04
			 0x6b015a41, /// 0xd08
			 0x55224804, /// 0xd0c
			 0xfa3c4dea, /// 0xd10
			 0x3bcfad88, /// 0xd14
			 0x9bb498d1, /// 0xd18
			 0xcc016b31, /// 0xd1c
			 0xa67c9a64, /// 0xd20
			 0xcc6d8687, /// 0xd24
			 0xf5db3974, /// 0xd28
			 0xc17f57a8, /// 0xd2c
			 0x2a50eaf1, /// 0xd30
			 0xd89a5ecc, /// 0xd34
			 0x9fa52c0b, /// 0xd38
			 0xc3ebb284, /// 0xd3c
			 0x5e4d2dfa, /// 0xd40
			 0x45a43abd, /// 0xd44
			 0x363cade2, /// 0xd48
			 0xfe2cfeb9, /// 0xd4c
			 0x44e01bda, /// 0xd50
			 0x42e660f7, /// 0xd54
			 0xcdab35e4, /// 0xd58
			 0xfcfa916a, /// 0xd5c
			 0xde3c4de0, /// 0xd60
			 0xf1b60aca, /// 0xd64
			 0x165661c6, /// 0xd68
			 0xa05b2df3, /// 0xd6c
			 0xf5f9e262, /// 0xd70
			 0xdd1a900b, /// 0xd74
			 0xfd00c291, /// 0xd78
			 0xb6dcd654, /// 0xd7c
			 0xf47fecd5, /// 0xd80
			 0xd2b52593, /// 0xd84
			 0x51a20bed, /// 0xd88
			 0xd3dbd7dc, /// 0xd8c
			 0xb4ff8305, /// 0xd90
			 0x308da7c2, /// 0xd94
			 0xc667a145, /// 0xd98
			 0xe6ed8356, /// 0xd9c
			 0x43f46fee, /// 0xda0
			 0x73a5f62d, /// 0xda4
			 0xa0eb95f9, /// 0xda8
			 0xc08a55c2, /// 0xdac
			 0x0deb8717, /// 0xdb0
			 0xff06816a, /// 0xdb4
			 0x5219d444, /// 0xdb8
			 0x6d5b77b6, /// 0xdbc
			 0xe5bbaf56, /// 0xdc0
			 0xd12debe9, /// 0xdc4
			 0x840ad5ac, /// 0xdc8
			 0x60e079d0, /// 0xdcc
			 0x13446df0, /// 0xdd0
			 0xfc824e22, /// 0xdd4
			 0x5360a748, /// 0xdd8
			 0x33e543e5, /// 0xddc
			 0x2bb6072b, /// 0xde0
			 0xbf040395, /// 0xde4
			 0x11708062, /// 0xde8
			 0x2f8da525, /// 0xdec
			 0xf6138cba, /// 0xdf0
			 0x1a91e0fc, /// 0xdf4
			 0xb0094840, /// 0xdf8
			 0x08c7df32, /// 0xdfc
			 0x106f07a6, /// 0xe00
			 0x3f93563d, /// 0xe04
			 0x83c72148, /// 0xe08
			 0x85023e81, /// 0xe0c
			 0x5d148753, /// 0xe10
			 0x7ad34dd8, /// 0xe14
			 0x8a72e701, /// 0xe18
			 0xaefe5b27, /// 0xe1c
			 0x213b4262, /// 0xe20
			 0x9f093730, /// 0xe24
			 0x8f61db80, /// 0xe28
			 0x836f8838, /// 0xe2c
			 0xeafe9997, /// 0xe30
			 0xa49b3e2e, /// 0xe34
			 0xc2e8a95c, /// 0xe38
			 0xe6c1342e, /// 0xe3c
			 0x6a9a43d0, /// 0xe40
			 0xc20481c2, /// 0xe44
			 0x7e6b636f, /// 0xe48
			 0x4263c29c, /// 0xe4c
			 0x1494d374, /// 0xe50
			 0x1ef8b8fc, /// 0xe54
			 0x59f34ff4, /// 0xe58
			 0x3d1a50ff, /// 0xe5c
			 0x4fcc3027, /// 0xe60
			 0x1e47d607, /// 0xe64
			 0x599a78d3, /// 0xe68
			 0xb170a2e4, /// 0xe6c
			 0xb50db4ae, /// 0xe70
			 0x9c9621e1, /// 0xe74
			 0xe58b1afe, /// 0xe78
			 0x1d17c63c, /// 0xe7c
			 0xa5f45886, /// 0xe80
			 0x6ab53d07, /// 0xe84
			 0x74b7297c, /// 0xe88
			 0xc7843adc, /// 0xe8c
			 0x069168fb, /// 0xe90
			 0xea72d109, /// 0xe94
			 0xfb529052, /// 0xe98
			 0xded6a063, /// 0xe9c
			 0xf9b6b0c5, /// 0xea0
			 0x5007e337, /// 0xea4
			 0x935e7b85, /// 0xea8
			 0x6fc641e1, /// 0xeac
			 0x21ca675f, /// 0xeb0
			 0x52bb33c2, /// 0xeb4
			 0x57127b2f, /// 0xeb8
			 0xda917b64, /// 0xebc
			 0x357c15d7, /// 0xec0
			 0xf30affaf, /// 0xec4
			 0x60a8dee8, /// 0xec8
			 0x464e0d5e, /// 0xecc
			 0x780b4263, /// 0xed0
			 0x180e027f, /// 0xed4
			 0x8e43a3fa, /// 0xed8
			 0xa2cb40da, /// 0xedc
			 0xcbf45eff, /// 0xee0
			 0x0a4b5c56, /// 0xee4
			 0xcf0f7005, /// 0xee8
			 0xcd1a53cf, /// 0xeec
			 0x05ed37b5, /// 0xef0
			 0x738b2d7e, /// 0xef4
			 0xaf788587, /// 0xef8
			 0xb10086e3, /// 0xefc
			 0x783474bb, /// 0xf00
			 0xb640a96a, /// 0xf04
			 0x8ef65114, /// 0xf08
			 0xb184d422, /// 0xf0c
			 0x66d1fdfd, /// 0xf10
			 0x977f9f26, /// 0xf14
			 0x055eb694, /// 0xf18
			 0x145b1865, /// 0xf1c
			 0xcdd4ef6a, /// 0xf20
			 0x07f237db, /// 0xf24
			 0x4f392b7b, /// 0xf28
			 0x4816d70e, /// 0xf2c
			 0x69978862, /// 0xf30
			 0xf86fdc45, /// 0xf34
			 0x1a71deef, /// 0xf38
			 0x21cb1183, /// 0xf3c
			 0x7f00fe44, /// 0xf40
			 0x38af0ad6, /// 0xf44
			 0x4025803b, /// 0xf48
			 0x18cf6142, /// 0xf4c
			 0x48bcf8a6, /// 0xf50
			 0x3dbd5584, /// 0xf54
			 0xb17273d1, /// 0xf58
			 0x87c5737d, /// 0xf5c
			 0x682f4dba, /// 0xf60
			 0x614346fc, /// 0xf64
			 0x910b67d8, /// 0xf68
			 0x2d1793e9, /// 0xf6c
			 0x11a4d216, /// 0xf70
			 0xde251da2, /// 0xf74
			 0xdbe299a6, /// 0xf78
			 0x3a5c3b68, /// 0xf7c
			 0xa5997f72, /// 0xf80
			 0xd97b2da2, /// 0xf84
			 0x45c270a4, /// 0xf88
			 0xd70e6d73, /// 0xf8c
			 0xf36c1da6, /// 0xf90
			 0x3faca5b4, /// 0xf94
			 0x46ffc32f, /// 0xf98
			 0x0d387c03, /// 0xf9c
			 0x799773c8, /// 0xfa0
			 0x3416ae5f, /// 0xfa4
			 0x50e04917, /// 0xfa8
			 0xca3e7e0f, /// 0xfac
			 0x41df1cae, /// 0xfb0
			 0x547586e0, /// 0xfb4
			 0x5b97182f, /// 0xfb8
			 0xd864d6e6, /// 0xfbc
			 0x24d6d9fd, /// 0xfc0
			 0x4909bdca, /// 0xfc4
			 0x44e512f5, /// 0xfc8
			 0x95280a8b, /// 0xfcc
			 0x023fd4a8, /// 0xfd0
			 0x445bef33, /// 0xfd4
			 0x05633920, /// 0xfd8
			 0xc0caf4bf, /// 0xfdc
			 0xbcf7e341, /// 0xfe0
			 0xb2058711, /// 0xfe4
			 0xbad205cb, /// 0xfe8
			 0x1a183b20, /// 0xfec
			 0x156ef60e, /// 0xff0
			 0xc772c6a9, /// 0xff4
			 0x03560ba9, /// 0xff8
			 0xce912749 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00000
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00004
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00008
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0000c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00020
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00028
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0002c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00034
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00044
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00050
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00058
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0006c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0xffc00001,                                                  // -signaling NaN                              /// 00074
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0007c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00080
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00084
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00088
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00090
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00094
			 0x0c600000,                                                  // Leading 1s:                                 /// 00098
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0009c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a0
			 0xbf028f5c,                                                  // -0.51                                       /// 000a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 000f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x7fc00001,                                                  // signaling NaN                               /// 000f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00108
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0010c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00118
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00120
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x7f800000,                                                  // inf                                         /// 00128
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0012c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00130
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00134
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00138
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0013c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00140
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00144
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00150
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00154
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00160
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00168
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0016c
			 0x7f800000,                                                  // inf                                         /// 00170
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00174
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00178
			 0xffc00001,                                                  // -signaling NaN                              /// 0017c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00180
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00188
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0xff800000,                                                  // -inf                                        /// 001a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0c700000,                                                  // Leading 1s:                                 /// 00204
			 0x0c600000,                                                  // Leading 1s:                                 /// 00208
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00210
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00214
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00218
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00220
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00228
			 0x80000000,                                                  // -zero                                       /// 0022c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00230
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00234
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0023c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00240
			 0x0e000001,                                                  // Trailing 1s:                                /// 00244
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00248
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0024c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00250
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00254
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00258
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0025c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00260
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00268
			 0xcb000000,                                                  // -8388608.0                                  /// 0026c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00274
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0027c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00280
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0028c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00290
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00294
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00298
			 0x0c600000,                                                  // Leading 1s:                                 /// 0029c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x7f800000,                                                  // inf                                         /// 002f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00300
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00304
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0030c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00310
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00314
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00318
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00324
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00328
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0032c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00330
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00334
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00338
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0033c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00348
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00350
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00354
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00358
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00360
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00370
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00374
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00378
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0037c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00380
			 0x55555555,                                                  // 4 random values                             /// 00384
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00388
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0038c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00390
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d8
			 0xbf028f5c,                                                  // -0.51                                       /// 003dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00400
			 0x4b000000,                                                  // 8388608.0                                   /// 00404
			 0xcb000000,                                                  // -8388608.0                                  /// 00408
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xbf028f5c,                                                  // -0.51                                       /// 00410
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00414
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00418
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0041c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00424
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00428
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0042c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00430
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0043c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00440
			 0x80011111,                                                  // -9.7958E-41                                 /// 00444
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00448
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0044c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00458
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00460
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00468
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0046c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00474
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00478
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00484
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0xffc00001,                                                  // -signaling NaN                              /// 0048c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xbf028f5c,                                                  // -0.51                                       /// 00494
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00498
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0049c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x3f028f5c,                                                  // 0.51                                        /// 004cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00500
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00504
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00508
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0050c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00510
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00520
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00524
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00530
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00534
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00540
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00544
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00548
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00550
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00554
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00558
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00560
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00564
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00568
			 0x0c700000,                                                  // Leading 1s:                                 /// 0056c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00574
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00578
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00580
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00588
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0058c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00598
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005bc
			 0x55555555,                                                  // 4 random values                             /// 005c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00604
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00608
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00610
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00618
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00620
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00624
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00628
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x55555555,                                                  // 4 random values                             /// 00638
			 0x3f028f5c,                                                  // 0.51                                        /// 0063c
			 0x3f028f5c,                                                  // 0.51                                        /// 00640
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0064c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00650
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00654
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00658
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00660
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00664
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00668
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00670
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00678
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0067c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00684
			 0x3f028f5c,                                                  // 0.51                                        /// 00688
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0068c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00690
			 0x4b000000,                                                  // 8388608.0                                   /// 00694
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0069c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006c4
			 0xcb000000,                                                  // -8388608.0                                  /// 006c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00700
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00704
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00710
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00720
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0072c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00734
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00738
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0073c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00744
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x4b000000,                                                  // 8388608.0                                   /// 00750
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00754
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00758
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0075c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00760
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00768
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0076c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00774
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00778
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0077c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00780
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00784
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00788
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00790
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00794
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00798
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0079c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b8
			 0x00000000,                                                  // zero                                        /// 007bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00804
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00808
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0080c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00810
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00814
			 0x0e000007,                                                  // Trailing 1s:                                /// 00818
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00824
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00838
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0083c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00840
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00844
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000000,                                                  // -zero                                       /// 00850
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00854
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0085c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00860
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00874
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0087c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0c780000,                                                  // Leading 1s:                                 /// 00884
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00888
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00890
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00898
			 0xff800000,                                                  // -inf                                        /// 0089c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008c4
			 0x55555555,                                                  // 4 random values                             /// 008c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008d4
			 0xcb000000,                                                  // -8388608.0                                  /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00900
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00904
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00914
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00918
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00924
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00928
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0092c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00930
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0093c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00944
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00948
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0094c
			 0xbf028f5c,                                                  // -0.51                                       /// 00950
			 0xff800000,                                                  // -inf                                        /// 00954
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00968
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0096c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00970
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00974
			 0x7f800000,                                                  // inf                                         /// 00978
			 0x0c600000,                                                  // Leading 1s:                                 /// 0097c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00980
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00984
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00988
			 0xff800000,                                                  // -inf                                        /// 0098c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00990
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00998
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0099c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009bc
			 0xbf028f5c,                                                  // -0.51                                       /// 009c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c4
			 0xbf028f5c,                                                  // -0.51                                       /// 009c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d8
			 0xcb000000,                                                  // -8388608.0                                  /// 009dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0xff800000,                                                  // -inf                                        /// 00a1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a3c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a5c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x7f800000,                                                  // inf                                         /// 00a74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a90
			 0x4b000000,                                                  // 8388608.0                                   /// 00a94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00aa4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aa8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ac4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ac8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00adc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00afc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0xffc00001,                                                  // -signaling NaN                              /// 00b04
			 0x00000000,                                                  // zero                                        /// 00b08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x80000000,                                                  // -zero                                       /// 00b24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b64
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ba4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ba8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bf8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0xffc00001,                                                  // -signaling NaN                              /// 00c1c
			 0x33333333,                                                  // 4 random values                             /// 00c20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c44
			 0x55555555,                                                  // 4 random values                             /// 00c48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c80
			 0xff800000,                                                  // -inf                                        /// 00c84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x33333333,                                                  // 4 random values                             /// 00cdc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ce0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cf8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x7f800000,                                                  // inf                                         /// 00d28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d34
			 0xcb000000,                                                  // -8388608.0                                  /// 00d38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00da0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00db0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00db4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00db8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00de0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00de8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00df4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0xcb000000,                                                  // -8388608.0                                  /// 00e1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e40
			 0x4b000000,                                                  // 8388608.0                                   /// 00e44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e68
			 0x4b000000,                                                  // 8388608.0                                   /// 00e6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ecc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00edc
			 0x7f800000,                                                  // inf                                         /// 00ee0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ee4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f00
			 0xffc00001,                                                  // -signaling NaN                              /// 00f04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f58
			 0xffc00001,                                                  // -signaling NaN                              /// 00f5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x80000000,                                                  // -zero                                       /// 00f8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fa4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00000000,                                                  // zero                                        /// 00fdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ff4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0c7fff00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x55555555,                                                  // 4 random values                             /// 00004
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x7fc00001,                                                  // signaling NaN                               /// 0000c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00014
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00020
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00024
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00028
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0002c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00030
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c700000,                                                  // Leading 1s:                                 /// 00038
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xff800000,                                                  // -inf                                        /// 00044
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0xcb000000,                                                  // -8388608.0                                  /// 0004c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00054
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00060
			 0x33333333,                                                  // 4 random values                             /// 00064
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0006c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00070
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00078
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0007c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00080
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00088
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00090
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x7f800000,                                                  // inf                                         /// 000b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0xbf028f5c,                                                  // -0.51                                       /// 000f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00100
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00104
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00108
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0010c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00110
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00114
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00118
			 0x55555555,                                                  // 4 random values                             /// 0011c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00124
			 0xff800000,                                                  // -inf                                        /// 00128
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00134
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00138
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00144
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00154
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00160
			 0x0c780000,                                                  // Leading 1s:                                 /// 00164
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0016c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00170
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00174
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00178
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00184
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00188
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0018c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00190
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00194
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00198
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0xffc00001,                                                  // -signaling NaN                              /// 001e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00200
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00204
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00214
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x4b000000,                                                  // 8388608.0                                   /// 00220
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00228
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0022c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00230
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00234
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00238
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00248
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0024c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00250
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00254
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00258
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00264
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00270
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00274
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00280
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00284
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00288
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0028c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00298
			 0xffc00001,                                                  // -signaling NaN                              /// 0029c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x7f800000,                                                  // inf                                         /// 002b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000000,                                                  // zero                                        /// 002d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002d8
			 0x4b000000,                                                  // 8388608.0                                   /// 002dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00300
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00310
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00314
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00318
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0031c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00320
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00324
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00334
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00338
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00344
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0034c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00354
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00358
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0035c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00360
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00364
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0xffc00001,                                                  // -signaling NaN                              /// 00374
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00378
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0037c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00384
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00390
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00394
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0039c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00408
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0040c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00410
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00414
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00418
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00420
			 0x3f028f5c,                                                  // 0.51                                        /// 00424
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00428
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00430
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0043c
			 0x00000000,                                                  // zero                                        /// 00440
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00444
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00454
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0045c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xffc00001,                                                  // -signaling NaN                              /// 00464
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00468
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0046c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00474
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00478
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00484
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00488
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00490
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x3f028f5c,                                                  // 0.51                                        /// 004a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004d8
			 0x33333333,                                                  // 4 random values                             /// 004dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00500
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00504
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00508
			 0x7f800000,                                                  // inf                                         /// 0050c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00514
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00518
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0051c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00520
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0e000007,                                                  // Trailing 1s:                                /// 00528
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00534
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00538
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0053c
			 0x80000000,                                                  // -zero                                       /// 00540
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00544
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00548
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0054c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00550
			 0x0e000001,                                                  // Trailing 1s:                                /// 00554
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0055c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00564
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00568
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0056c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00570
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0057c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00580
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00584
			 0x0c400000,                                                  // Leading 1s:                                 /// 00588
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00594
			 0x7fc00001,                                                  // signaling NaN                               /// 00598
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0059c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005a0
			 0xffc00001,                                                  // -signaling NaN                              /// 005a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005dc
			 0x55555555,                                                  // 4 random values                             /// 005e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f4
			 0x80000000,                                                  // -zero                                       /// 005f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00618
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00620
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00628
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0062c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00630
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00638
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0xff800000,                                                  // -inf                                        /// 00644
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00650
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00654
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00664
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0066c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00670
			 0x0c600000,                                                  // Leading 1s:                                 /// 00674
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x4b000000,                                                  // 8388608.0                                   /// 00680
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00684
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0068c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0c780000,                                                  // Leading 1s:                                 /// 00694
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00698
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0069c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x4b000000,                                                  // 8388608.0                                   /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0xffc00001,                                                  // -signaling NaN                              /// 006ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x7fc00001,                                                  // signaling NaN                               /// 006b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x7f800000,                                                  // inf                                         /// 006f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x33333333,                                                  // 4 random values                             /// 006fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00708
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0070c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00710
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00714
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00720
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00724
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00734
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00738
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00748
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00754
			 0x00011111,                                                  // 9.7958E-41                                  /// 00758
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0075c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00768
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00770
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00778
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00784
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00790
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00800
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00804
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00814
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0081c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00820
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00828
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0082c
			 0x00000000,                                                  // zero                                        /// 00830
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00834
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00840
			 0xbf028f5c,                                                  // -0.51                                       /// 00844
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00848
			 0x00000000,                                                  // zero                                        /// 0084c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00850
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00854
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00858
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0085c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00864
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0086c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00878
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0087c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00880
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00884
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00888
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00898
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0089c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008c0
			 0x7fc00001,                                                  // signaling NaN                               /// 008c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008c8
			 0xffc00001,                                                  // -signaling NaN                              /// 008cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d0
			 0xff800000,                                                  // -inf                                        /// 008d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00900
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00904
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00910
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00918
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00920
			 0x0c400000,                                                  // Leading 1s:                                 /// 00924
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00928
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00930
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00934
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00940
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00950
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00958
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0095c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00964
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00968
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00974
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00978
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0097c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00980
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00984
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0098c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0099c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009a0
			 0xff800000,                                                  // -inf                                        /// 009a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009a8
			 0xbf028f5c,                                                  // -0.51                                       /// 009ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a10
			 0xcb000000,                                                  // -8388608.0                                  /// 00a14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a8c
			 0x55555555,                                                  // 4 random values                             /// 00a90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00abc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ad8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ae4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00af4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00afc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b2c
			 0xff800000,                                                  // -inf                                        /// 00b30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0xffc00001,                                                  // -signaling NaN                              /// 00b58
			 0xcb000000,                                                  // -8388608.0                                  /// 00b5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b94
			 0x4b000000,                                                  // 8388608.0                                   /// 00b98
			 0x00000000,                                                  // zero                                        /// 00b9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x7f800000,                                                  // inf                                         /// 00bac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bc0
			 0x00000000,                                                  // zero                                        /// 00bc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bfc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cac
			 0x00000000,                                                  // zero                                        /// 00cb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cbc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cc8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ccc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cdc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf4
			 0x00000000,                                                  // zero                                        /// 00cf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d2c
			 0x55555555,                                                  // 4 random values                             /// 00d30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00db0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x7f800000,                                                  // inf                                         /// 00db8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00df4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dfc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e60
			 0x4b000000,                                                  // 8388608.0                                   /// 00e64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000000,                                                  // -zero                                       /// 00e8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ea4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000000,                                                  // zero                                        /// 00eac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eb8
			 0x55555555,                                                  // 4 random values                             /// 00ebc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ec0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ee0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xbf028f5c,                                                  // -0.51                                       /// 00f08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f18
			 0x00000000,                                                  // zero                                        /// 00f1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0xcb000000,                                                  // -8388608.0                                  /// 00f48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f64
			 0x00000000,                                                  // zero                                        /// 00f68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f70
			 0x7f800000,                                                  // inf                                         /// 00f74
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fa4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fb0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe0
			 0x33333333,                                                  // 4 random values                             /// 00fe4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x00000008                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00008
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0000c
			 0xff800000,                                                  // -inf                                        /// 00010
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00014
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00018
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0001c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00020
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00024
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00028
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00030
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00034
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0003c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00040
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00044
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00048
			 0x4b000000,                                                  // 8388608.0                                   /// 0004c
			 0x4b000000,                                                  // 8388608.0                                   /// 00050
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x3f028f5c,                                                  // 0.51                                        /// 0005c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0006c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00070
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00074
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00078
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00080
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00088
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0008c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00090
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00098
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0009c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00100
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0010c
			 0x00000000,                                                  // zero                                        /// 00110
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00114
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00118
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0011c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00120
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00124
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0012c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00130
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00138
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0013c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00140
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00144
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x7fc00001,                                                  // signaling NaN                               /// 00150
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00160
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00164
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00168
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00170
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00174
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00178
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0017c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x00000000,                                                  // zero                                        /// 00188
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00198
			 0xffc00001,                                                  // -signaling NaN                              /// 0019c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001cc
			 0xcb000000,                                                  // -8388608.0                                  /// 001d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00208
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0020c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00218
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0021c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00224
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00238
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0023c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00240
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0024c
			 0xffc00001,                                                  // -signaling NaN                              /// 00250
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00258
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0025c
			 0x80000000,                                                  // -zero                                       /// 00260
			 0x7fc00001,                                                  // signaling NaN                               /// 00264
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00268
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0026c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0027c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00280
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00284
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00288
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0028c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00290
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00294
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0029c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00304
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00308
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0030c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00310
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00320
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00324
			 0x3f028f5c,                                                  // 0.51                                        /// 00328
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00330
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0033c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00340
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00344
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00348
			 0x80000000,                                                  // -zero                                       /// 0034c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00358
			 0x0e000001,                                                  // Trailing 1s:                                /// 0035c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00360
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00364
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00370
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00374
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00378
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0037c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00384
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0038c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00390
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00394
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00398
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0039c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b0
			 0xcb000000,                                                  // -8388608.0                                  /// 003b4
			 0x7f800000,                                                  // inf                                         /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003e0
			 0xcb000000,                                                  // -8388608.0                                  /// 003e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00410
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00414
			 0x0e000007,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0041c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00420
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00424
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00428
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0042c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00434
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00438
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0043c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00440
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00444
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00448
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0044c
			 0xcb000000,                                                  // -8388608.0                                  /// 00450
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00454
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00458
			 0x80000000,                                                  // -zero                                       /// 0045c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00460
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00468
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0046c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00478
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00480
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x4b000000,                                                  // 8388608.0                                   /// 00488
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0048c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00490
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00494
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0c780000,                                                  // Leading 1s:                                 /// 0049c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00504
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00508
			 0x3f028f5c,                                                  // 0.51                                        /// 0050c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00514
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00518
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00520
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00524
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00528
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0052c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00534
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00538
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0053c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00540
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00544
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0054c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00550
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00554
			 0x0e000003,                                                  // Trailing 1s:                                /// 00558
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0055c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00568
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00570
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00574
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0057c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x7f800000,                                                  // inf                                         /// 00588
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00594
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00598
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0059c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0xffc00001,                                                  // -signaling NaN                              /// 005c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00600
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00604
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00608
			 0x0e000001,                                                  // Trailing 1s:                                /// 0060c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00614
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00618
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0061c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x3f028f5c,                                                  // 0.51                                        /// 00624
			 0x33333333,                                                  // 4 random values                             /// 00628
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0062c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00630
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00634
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0063c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00640
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0064c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00650
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0c400000,                                                  // Leading 1s:                                 /// 00658
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00660
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00668
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00670
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00674
			 0xcb000000,                                                  // -8388608.0                                  /// 00678
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0067c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00680
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00684
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00688
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0068c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00694
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00698
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0069c
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00708
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0070c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00710
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00714
			 0x7f800000,                                                  // inf                                         /// 00718
			 0xcb000000,                                                  // -8388608.0                                  /// 0071c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00720
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00724
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00728
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0072c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e000007,                                                  // Trailing 1s:                                /// 00734
			 0xffc00001,                                                  // -signaling NaN                              /// 00738
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00740
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0074c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00750
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00758
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0075c
			 0xffc00001,                                                  // -signaling NaN                              /// 00760
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xffc00001,                                                  // -signaling NaN                              /// 00768
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0076c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00774
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0077c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00784
			 0xbf028f5c,                                                  // -0.51                                       /// 00788
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00790
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00794
			 0x0c600000,                                                  // Leading 1s:                                 /// 00798
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0079c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0080c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00814
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0081c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0082c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x7f800000,                                                  // inf                                         /// 00834
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00838
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0083c
			 0x55555555,                                                  // 4 random values                             /// 00840
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00848
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0084c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00850
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c700000,                                                  // Leading 1s:                                 /// 00858
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0086c
			 0x33333333,                                                  // 4 random values                             /// 00870
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00878
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0087c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00880
			 0x3f028f5c,                                                  // 0.51                                        /// 00884
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00894
			 0x33333333,                                                  // 4 random values                             /// 00898
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0089c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0xffc00001,                                                  // -signaling NaN                              /// 008d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008f4
			 0x33333333,                                                  // 4 random values                             /// 008f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00900
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00910
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00914
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0091c
			 0xbf028f5c,                                                  // -0.51                                       /// 00920
			 0x7fc00001,                                                  // signaling NaN                               /// 00924
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00934
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00938
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0093c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00944
			 0x80011111,                                                  // -9.7958E-41                                 /// 00948
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00958
			 0x7fc00001,                                                  // signaling NaN                               /// 0095c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00964
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00968
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00974
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00980
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00984
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00988
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x80011111,                                                  // -9.7958E-41                                 /// 00994
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000000,                                                  // zero                                        /// 009c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000000,                                                  // zero                                        /// 009e0
			 0x33333333,                                                  // 4 random values                             /// 009e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a14
			 0x80000000,                                                  // -zero                                       /// 00a18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a20
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x00000000,                                                  // zero                                        /// 00a70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a90
			 0x7fc00001,                                                  // signaling NaN                               /// 00a94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00aa4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ab8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ac8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00acc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad8
			 0x55555555,                                                  // 4 random values                             /// 00adc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00afc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b48
			 0x00000000,                                                  // zero                                        /// 00b4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bdc
			 0x7f800000,                                                  // inf                                         /// 00be0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00be8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bf4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c50
			 0xffc00001,                                                  // -signaling NaN                              /// 00c54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x55555555,                                                  // 4 random values                             /// 00c5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c84
			 0x00000000,                                                  // zero                                        /// 00c88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ca8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000000,                                                  // -zero                                       /// 00cb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0x33333333,                                                  // 4 random values                             /// 00d68
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d84
			 0xff800000,                                                  // -inf                                        /// 00d88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x33333333,                                                  // 4 random values                             /// 00d90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00db4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ddc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00de8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00df0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dfc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x55555555,                                                  // 4 random values                             /// 00e2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80000000,                                                  // -zero                                       /// 00e54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e64
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ea4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ea8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ebc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ec0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ec8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00edc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee4
			 0x55555555,                                                  // 4 random values                             /// 00ee8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ef0
			 0x80000000,                                                  // -zero                                       /// 00ef4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ef8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f10
			 0x3f028f5c,                                                  // 0.51                                        /// 00f14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x4b000000,                                                  // 8388608.0                                   /// 00f6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fa0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fe8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80000000                                                  // -zero                                       /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x34df69a8, /// 0x0
			 0x394ca8b0, /// 0x4
			 0xc858eae1, /// 0x8
			 0x2e00355c, /// 0xc
			 0x7de51159, /// 0x10
			 0x86e22aa5, /// 0x14
			 0x392612f1, /// 0x18
			 0x82971262, /// 0x1c
			 0x0ae7d0b9, /// 0x20
			 0x5a2e518f, /// 0x24
			 0xbf7e5ab2, /// 0x28
			 0x0b8d8b4b, /// 0x2c
			 0xf97b1b36, /// 0x30
			 0xaedabf0a, /// 0x34
			 0x97976fd8, /// 0x38
			 0xd67c6711, /// 0x3c
			 0xbbdcaa74, /// 0x40
			 0xbd11bc47, /// 0x44
			 0x43d3557d, /// 0x48
			 0x8917e624, /// 0x4c
			 0xee2b5700, /// 0x50
			 0x5136a441, /// 0x54
			 0x2f52e2e4, /// 0x58
			 0xa1a17b32, /// 0x5c
			 0x7a7f01eb, /// 0x60
			 0x02bee09f, /// 0x64
			 0x0bca6144, /// 0x68
			 0xa4f1b563, /// 0x6c
			 0xe4b9c9cf, /// 0x70
			 0x8a37c087, /// 0x74
			 0xfefc6b76, /// 0x78
			 0xf7d6d42d, /// 0x7c
			 0xb63de971, /// 0x80
			 0x68f747da, /// 0x84
			 0x2bf5a57d, /// 0x88
			 0x96433b15, /// 0x8c
			 0xfb6f6be5, /// 0x90
			 0x45bce184, /// 0x94
			 0xe2f03f6a, /// 0x98
			 0x77f1b922, /// 0x9c
			 0x02a3f9a1, /// 0xa0
			 0x568eb891, /// 0xa4
			 0x08c87819, /// 0xa8
			 0xa44d2f2d, /// 0xac
			 0xd1d43bee, /// 0xb0
			 0xd2efca40, /// 0xb4
			 0x2abfa4ab, /// 0xb8
			 0x85312d87, /// 0xbc
			 0xd39d5ec4, /// 0xc0
			 0x017ff83d, /// 0xc4
			 0xfbf276ee, /// 0xc8
			 0xbda2bf6d, /// 0xcc
			 0x26f9ceac, /// 0xd0
			 0x8a643c1c, /// 0xd4
			 0x29eb215c, /// 0xd8
			 0x81d13216, /// 0xdc
			 0xf1249152, /// 0xe0
			 0x478a29a2, /// 0xe4
			 0xce66d18d, /// 0xe8
			 0xa77ad81e, /// 0xec
			 0xa8a3f1ce, /// 0xf0
			 0xc10b1438, /// 0xf4
			 0xae10c84d, /// 0xf8
			 0xfc9b6e3e, /// 0xfc
			 0xa6c0cc41, /// 0x100
			 0xfc14fe1a, /// 0x104
			 0xdf3a4296, /// 0x108
			 0x7b9d74bd, /// 0x10c
			 0xd99ceec9, /// 0x110
			 0x83561830, /// 0x114
			 0x2802a4bf, /// 0x118
			 0xacead5bc, /// 0x11c
			 0xe3407d37, /// 0x120
			 0xdf7a92e5, /// 0x124
			 0x30c90dee, /// 0x128
			 0xb62340ce, /// 0x12c
			 0x7ed0bab9, /// 0x130
			 0x53981757, /// 0x134
			 0x1df62a79, /// 0x138
			 0x548eecc5, /// 0x13c
			 0xdd732126, /// 0x140
			 0x71dbb57b, /// 0x144
			 0xb4ff3074, /// 0x148
			 0xe7c345b2, /// 0x14c
			 0x07bf9006, /// 0x150
			 0xc244a903, /// 0x154
			 0xc906e528, /// 0x158
			 0xe5a47b85, /// 0x15c
			 0x4cb5b20d, /// 0x160
			 0x09ccc90d, /// 0x164
			 0x670a9583, /// 0x168
			 0xec8ccc5e, /// 0x16c
			 0xdf3aee1c, /// 0x170
			 0x7b075faa, /// 0x174
			 0x15871ebe, /// 0x178
			 0xd960c297, /// 0x17c
			 0x00e5c528, /// 0x180
			 0x3d191c45, /// 0x184
			 0x7d6d2686, /// 0x188
			 0xae941e15, /// 0x18c
			 0xae7d3a5d, /// 0x190
			 0x4946dce8, /// 0x194
			 0xf32b252f, /// 0x198
			 0xfc3e35a5, /// 0x19c
			 0xe6083cc0, /// 0x1a0
			 0x09bcea0d, /// 0x1a4
			 0x38f7b498, /// 0x1a8
			 0xd9b24d9d, /// 0x1ac
			 0xd52c668f, /// 0x1b0
			 0x0265de9a, /// 0x1b4
			 0xcde4c854, /// 0x1b8
			 0xab4808c6, /// 0x1bc
			 0xb9a3ec12, /// 0x1c0
			 0x45f311a4, /// 0x1c4
			 0xb154a39f, /// 0x1c8
			 0x9db86a18, /// 0x1cc
			 0xe9405b46, /// 0x1d0
			 0x261e4724, /// 0x1d4
			 0x2a8023e7, /// 0x1d8
			 0xd6ccdf32, /// 0x1dc
			 0x1525df2b, /// 0x1e0
			 0xdab49df3, /// 0x1e4
			 0x8373b4ce, /// 0x1e8
			 0xd768ba7b, /// 0x1ec
			 0x9411ba7b, /// 0x1f0
			 0xc3e50adb, /// 0x1f4
			 0x062b7560, /// 0x1f8
			 0x6b690e1a, /// 0x1fc
			 0x7362b5ce, /// 0x200
			 0x8705940e, /// 0x204
			 0xedc40bb5, /// 0x208
			 0x33a850ae, /// 0x20c
			 0xf7e309b7, /// 0x210
			 0xe4f8ed78, /// 0x214
			 0x45e10d80, /// 0x218
			 0x27bacf10, /// 0x21c
			 0x013f2f01, /// 0x220
			 0x79032c19, /// 0x224
			 0x780997b5, /// 0x228
			 0x146e1b73, /// 0x22c
			 0x010be94c, /// 0x230
			 0xf19bfdf6, /// 0x234
			 0x4af68723, /// 0x238
			 0x6d827890, /// 0x23c
			 0x387a2326, /// 0x240
			 0x41f945ef, /// 0x244
			 0x128976d8, /// 0x248
			 0x019f6af4, /// 0x24c
			 0xf22b7a1a, /// 0x250
			 0x201330da, /// 0x254
			 0xd725a972, /// 0x258
			 0x558dbc4b, /// 0x25c
			 0xcf635482, /// 0x260
			 0x01620023, /// 0x264
			 0x8ffdc67a, /// 0x268
			 0x58f3bc92, /// 0x26c
			 0xee28afbe, /// 0x270
			 0x23620a87, /// 0x274
			 0x78d3d1b5, /// 0x278
			 0xfc1a28cd, /// 0x27c
			 0xd1f7245c, /// 0x280
			 0x4f96fb3c, /// 0x284
			 0x11a9db22, /// 0x288
			 0x19920529, /// 0x28c
			 0x7a6bbe65, /// 0x290
			 0x8214bb5b, /// 0x294
			 0xdd2fa95d, /// 0x298
			 0xaeb081b5, /// 0x29c
			 0x2eaba92c, /// 0x2a0
			 0xb6f6dcf2, /// 0x2a4
			 0x2f48024e, /// 0x2a8
			 0x4c0bd0b6, /// 0x2ac
			 0xb18dc273, /// 0x2b0
			 0x68618d31, /// 0x2b4
			 0x1c0bef7f, /// 0x2b8
			 0x34e4ecb6, /// 0x2bc
			 0xedd94afc, /// 0x2c0
			 0x25d7d98d, /// 0x2c4
			 0xe546f903, /// 0x2c8
			 0xfa231c7f, /// 0x2cc
			 0x6bc77911, /// 0x2d0
			 0x45f15e81, /// 0x2d4
			 0xa4fc36bf, /// 0x2d8
			 0x64e7d10d, /// 0x2dc
			 0x62749f97, /// 0x2e0
			 0x15c72f5b, /// 0x2e4
			 0x158646e5, /// 0x2e8
			 0x8d321e82, /// 0x2ec
			 0x6202f68b, /// 0x2f0
			 0xe8b040b5, /// 0x2f4
			 0x3570fccc, /// 0x2f8
			 0x8de9c978, /// 0x2fc
			 0xba4d7628, /// 0x300
			 0x21f10019, /// 0x304
			 0xd1b3e4f0, /// 0x308
			 0xd0533e00, /// 0x30c
			 0x85b622cd, /// 0x310
			 0xe7ab57ae, /// 0x314
			 0x11dfabd9, /// 0x318
			 0x33324d58, /// 0x31c
			 0xaedfd27f, /// 0x320
			 0xe8b11470, /// 0x324
			 0x4c8cce24, /// 0x328
			 0x80202d80, /// 0x32c
			 0x44ca8a27, /// 0x330
			 0xd891cf69, /// 0x334
			 0x3e4c2431, /// 0x338
			 0xa7bd4367, /// 0x33c
			 0x9abf99da, /// 0x340
			 0x1d56d958, /// 0x344
			 0x4117bdbb, /// 0x348
			 0xfbda4c00, /// 0x34c
			 0x30db6112, /// 0x350
			 0x6e057534, /// 0x354
			 0x55ee2250, /// 0x358
			 0xc9c302b6, /// 0x35c
			 0xb2ac0e26, /// 0x360
			 0xc749d3f2, /// 0x364
			 0xff29e6f6, /// 0x368
			 0xd55df269, /// 0x36c
			 0xd04cf5b0, /// 0x370
			 0x06b911e2, /// 0x374
			 0x38c9d6fb, /// 0x378
			 0xda5d9d40, /// 0x37c
			 0x7e23d9a9, /// 0x380
			 0xb0f60d3a, /// 0x384
			 0x764f2153, /// 0x388
			 0xa39eac83, /// 0x38c
			 0x3a3e4d59, /// 0x390
			 0x24de88e0, /// 0x394
			 0xf9d47e28, /// 0x398
			 0x29a770ec, /// 0x39c
			 0x6e69b095, /// 0x3a0
			 0x0b1501f5, /// 0x3a4
			 0xd4ce06a2, /// 0x3a8
			 0x8546ffed, /// 0x3ac
			 0x377fdc54, /// 0x3b0
			 0x07a486ce, /// 0x3b4
			 0x4cf2f331, /// 0x3b8
			 0xa94d243e, /// 0x3bc
			 0x7acc21cf, /// 0x3c0
			 0x58ac1775, /// 0x3c4
			 0x8c4f434d, /// 0x3c8
			 0x25dae093, /// 0x3cc
			 0x4af48a42, /// 0x3d0
			 0xc2c7721d, /// 0x3d4
			 0xf81d5cea, /// 0x3d8
			 0x3af3dcf5, /// 0x3dc
			 0x5967bdaa, /// 0x3e0
			 0x30817662, /// 0x3e4
			 0xf477840e, /// 0x3e8
			 0x605950b6, /// 0x3ec
			 0x5bcfd247, /// 0x3f0
			 0xfdedeaaa, /// 0x3f4
			 0x67478a20, /// 0x3f8
			 0xc4c4d7a9, /// 0x3fc
			 0x479f0325, /// 0x400
			 0xbd808771, /// 0x404
			 0x389e36b6, /// 0x408
			 0x8eca5690, /// 0x40c
			 0xeeaa18af, /// 0x410
			 0x971b9b5d, /// 0x414
			 0x54379bf6, /// 0x418
			 0xc99ecd58, /// 0x41c
			 0xd1973816, /// 0x420
			 0x529bcb41, /// 0x424
			 0x18ca81a2, /// 0x428
			 0x4e3d4018, /// 0x42c
			 0xb2f67a7d, /// 0x430
			 0xbb7a9a85, /// 0x434
			 0x4174de82, /// 0x438
			 0x3f73286c, /// 0x43c
			 0x4e15e83f, /// 0x440
			 0x47e895c3, /// 0x444
			 0x604992cd, /// 0x448
			 0xe87b3856, /// 0x44c
			 0x845a3a8a, /// 0x450
			 0xd1124d8a, /// 0x454
			 0x7493c056, /// 0x458
			 0xfb346573, /// 0x45c
			 0x5cda82f0, /// 0x460
			 0x0e4e528c, /// 0x464
			 0x22ebcfe7, /// 0x468
			 0x032d7a62, /// 0x46c
			 0x12503b91, /// 0x470
			 0xfda4fe47, /// 0x474
			 0x09230da5, /// 0x478
			 0x1aa38465, /// 0x47c
			 0x51ad2397, /// 0x480
			 0x774851e0, /// 0x484
			 0xa8b8aeae, /// 0x488
			 0xc6d72410, /// 0x48c
			 0x81d39e83, /// 0x490
			 0xab45c75f, /// 0x494
			 0x6ced9c60, /// 0x498
			 0xe8de7fce, /// 0x49c
			 0x140ad61d, /// 0x4a0
			 0x00bccad2, /// 0x4a4
			 0xdf8460fd, /// 0x4a8
			 0x2ca9b34c, /// 0x4ac
			 0xc922693f, /// 0x4b0
			 0x03117152, /// 0x4b4
			 0x3e1e2169, /// 0x4b8
			 0x32a639b8, /// 0x4bc
			 0x4a43bc73, /// 0x4c0
			 0xcd27aba0, /// 0x4c4
			 0xebb31c58, /// 0x4c8
			 0x29396390, /// 0x4cc
			 0xf04ec677, /// 0x4d0
			 0x4d4dbfdf, /// 0x4d4
			 0x9c681af8, /// 0x4d8
			 0x683fc390, /// 0x4dc
			 0xab668eaf, /// 0x4e0
			 0x5222dc12, /// 0x4e4
			 0x7abb28b1, /// 0x4e8
			 0x24ccf614, /// 0x4ec
			 0x00c40060, /// 0x4f0
			 0x95178b26, /// 0x4f4
			 0xec874943, /// 0x4f8
			 0xdfce9f6a, /// 0x4fc
			 0xc60395c4, /// 0x500
			 0x67f2b954, /// 0x504
			 0x8639cc38, /// 0x508
			 0xebb22134, /// 0x50c
			 0x560dd0da, /// 0x510
			 0x2f13f9c3, /// 0x514
			 0x10b44a29, /// 0x518
			 0x3eb30ca7, /// 0x51c
			 0xa9cf996d, /// 0x520
			 0x91c9da12, /// 0x524
			 0xcea96af5, /// 0x528
			 0xb94b97fb, /// 0x52c
			 0x3b43ce77, /// 0x530
			 0x2f570224, /// 0x534
			 0x3f47c526, /// 0x538
			 0x65d5e150, /// 0x53c
			 0x2803708e, /// 0x540
			 0x80aca3b7, /// 0x544
			 0x59f8b345, /// 0x548
			 0x1847f0fe, /// 0x54c
			 0x67da4227, /// 0x550
			 0xef94afba, /// 0x554
			 0x0bde7c90, /// 0x558
			 0x6a2dc7fa, /// 0x55c
			 0xd9dd7f25, /// 0x560
			 0x55de287a, /// 0x564
			 0x1915b267, /// 0x568
			 0x9c0a1d6a, /// 0x56c
			 0x4c2d7cdf, /// 0x570
			 0xbf590589, /// 0x574
			 0x56f767bc, /// 0x578
			 0x7cbc7b45, /// 0x57c
			 0x6c9d0c46, /// 0x580
			 0xeac0f3d3, /// 0x584
			 0x5ac99dc7, /// 0x588
			 0x82ea2653, /// 0x58c
			 0xaca34725, /// 0x590
			 0xf269ae93, /// 0x594
			 0x82de9349, /// 0x598
			 0x04181f3f, /// 0x59c
			 0xdbe754fe, /// 0x5a0
			 0x58cec35c, /// 0x5a4
			 0x8891d958, /// 0x5a8
			 0xd242606f, /// 0x5ac
			 0xd508d935, /// 0x5b0
			 0x3e071414, /// 0x5b4
			 0x448ec232, /// 0x5b8
			 0x80c26638, /// 0x5bc
			 0x449cb063, /// 0x5c0
			 0x5076a665, /// 0x5c4
			 0xf9f14c3d, /// 0x5c8
			 0x9d557850, /// 0x5cc
			 0xeaef667b, /// 0x5d0
			 0x03c567aa, /// 0x5d4
			 0x2b29e06c, /// 0x5d8
			 0x69c99b65, /// 0x5dc
			 0x5e67fefe, /// 0x5e0
			 0x3606d789, /// 0x5e4
			 0xaadfca86, /// 0x5e8
			 0x621d3239, /// 0x5ec
			 0x92b07ae0, /// 0x5f0
			 0x433eb8ea, /// 0x5f4
			 0x14ef6e0d, /// 0x5f8
			 0x1c6afd9c, /// 0x5fc
			 0xa2b155cb, /// 0x600
			 0x7e1c3778, /// 0x604
			 0xb203343c, /// 0x608
			 0xdcec261c, /// 0x60c
			 0x576ae96e, /// 0x610
			 0xf81082f4, /// 0x614
			 0x1cd4a232, /// 0x618
			 0x3843ab40, /// 0x61c
			 0x3c4c5e19, /// 0x620
			 0x4c52636e, /// 0x624
			 0x68ebf0b4, /// 0x628
			 0xa1af0074, /// 0x62c
			 0xcf72fca1, /// 0x630
			 0x02a7fde7, /// 0x634
			 0x642e7aed, /// 0x638
			 0xaa6ef57f, /// 0x63c
			 0x1df2793f, /// 0x640
			 0x65bc86fa, /// 0x644
			 0xb6c7500e, /// 0x648
			 0x8d75762a, /// 0x64c
			 0xe161fe62, /// 0x650
			 0x03498fd7, /// 0x654
			 0x482fead9, /// 0x658
			 0xfc7abdd6, /// 0x65c
			 0x33c06980, /// 0x660
			 0x4ef62405, /// 0x664
			 0x609c23df, /// 0x668
			 0xc6729569, /// 0x66c
			 0xbc2221c2, /// 0x670
			 0xf8d4fd78, /// 0x674
			 0x86a0cbb3, /// 0x678
			 0x995b5589, /// 0x67c
			 0x32740964, /// 0x680
			 0xd0199373, /// 0x684
			 0x6a77ba42, /// 0x688
			 0x50d23871, /// 0x68c
			 0x6bdc0595, /// 0x690
			 0x22f5baac, /// 0x694
			 0x074e6727, /// 0x698
			 0x8c62ea80, /// 0x69c
			 0xaedc3a3e, /// 0x6a0
			 0x6756c258, /// 0x6a4
			 0xbcd74b0b, /// 0x6a8
			 0xc3d933e9, /// 0x6ac
			 0x138a11af, /// 0x6b0
			 0x89f48059, /// 0x6b4
			 0xc2b97de4, /// 0x6b8
			 0xc4cdf11b, /// 0x6bc
			 0xac88265b, /// 0x6c0
			 0xecdcfd3d, /// 0x6c4
			 0x43bb4603, /// 0x6c8
			 0xfee0abfd, /// 0x6cc
			 0x9b3eada9, /// 0x6d0
			 0x5bbac1ff, /// 0x6d4
			 0x775db959, /// 0x6d8
			 0xbc9c4d55, /// 0x6dc
			 0x1dd365c4, /// 0x6e0
			 0xd42af173, /// 0x6e4
			 0x9c024b3c, /// 0x6e8
			 0x7ad10dbb, /// 0x6ec
			 0x89cca8b3, /// 0x6f0
			 0x70f59cf3, /// 0x6f4
			 0x9a419e82, /// 0x6f8
			 0x00f95ce9, /// 0x6fc
			 0x90403473, /// 0x700
			 0xdb9622b2, /// 0x704
			 0xa2a4c056, /// 0x708
			 0x6981bf8a, /// 0x70c
			 0x258edaae, /// 0x710
			 0xc9946a66, /// 0x714
			 0xe4e8a61b, /// 0x718
			 0xec05ecb3, /// 0x71c
			 0xf1296eba, /// 0x720
			 0xce773611, /// 0x724
			 0xa24994de, /// 0x728
			 0x7eca0537, /// 0x72c
			 0xeadb8cc5, /// 0x730
			 0x313e9eb3, /// 0x734
			 0x640492fb, /// 0x738
			 0x7ec7938f, /// 0x73c
			 0x04bd54cb, /// 0x740
			 0x1b85ff3b, /// 0x744
			 0xe0ccefd4, /// 0x748
			 0x2a3317bb, /// 0x74c
			 0x9fba1445, /// 0x750
			 0xc2e48e09, /// 0x754
			 0xe3f53413, /// 0x758
			 0x3c1ea711, /// 0x75c
			 0x3a072ab3, /// 0x760
			 0xd1036dd1, /// 0x764
			 0xd2a9190b, /// 0x768
			 0x18c7e161, /// 0x76c
			 0x1d424e53, /// 0x770
			 0x6371d28a, /// 0x774
			 0xe7fb3796, /// 0x778
			 0x0465ec1e, /// 0x77c
			 0x4f302db9, /// 0x780
			 0x4041eb2f, /// 0x784
			 0xec8ec343, /// 0x788
			 0x1173abfe, /// 0x78c
			 0xf78881d0, /// 0x790
			 0xa13df08f, /// 0x794
			 0xb3f2202c, /// 0x798
			 0x5c97f9d5, /// 0x79c
			 0xb27b5def, /// 0x7a0
			 0xe3f1a668, /// 0x7a4
			 0x3cd14587, /// 0x7a8
			 0xd0ab85c2, /// 0x7ac
			 0xe3e39e68, /// 0x7b0
			 0x00ccdf22, /// 0x7b4
			 0xe6e27adf, /// 0x7b8
			 0xe0f6ceee, /// 0x7bc
			 0x7cb1d1b4, /// 0x7c0
			 0xdb7a566e, /// 0x7c4
			 0xfdb47cbd, /// 0x7c8
			 0x522ef762, /// 0x7cc
			 0x9b85a8d3, /// 0x7d0
			 0x845f9486, /// 0x7d4
			 0xfc127ce8, /// 0x7d8
			 0x5ca544b1, /// 0x7dc
			 0xe8bfa85e, /// 0x7e0
			 0x1645ebaf, /// 0x7e4
			 0xf1efcc6a, /// 0x7e8
			 0x89787cd6, /// 0x7ec
			 0xdde9d8fb, /// 0x7f0
			 0xa0299760, /// 0x7f4
			 0x03f1daa0, /// 0x7f8
			 0xadd3a06e, /// 0x7fc
			 0xa724c8f6, /// 0x800
			 0xc2538aed, /// 0x804
			 0xf3d6704b, /// 0x808
			 0x749a2f3a, /// 0x80c
			 0x1aa64aad, /// 0x810
			 0x70a8bcfa, /// 0x814
			 0xc51d2788, /// 0x818
			 0x2aa4b8b1, /// 0x81c
			 0x5b5061a2, /// 0x820
			 0x5092e42f, /// 0x824
			 0x7696c9ee, /// 0x828
			 0x2ed608d1, /// 0x82c
			 0xb64ae5fd, /// 0x830
			 0x30a69eb0, /// 0x834
			 0x166a609b, /// 0x838
			 0x1ee3e3ff, /// 0x83c
			 0xf890f73f, /// 0x840
			 0x47124a81, /// 0x844
			 0x0d6f460e, /// 0x848
			 0x13570325, /// 0x84c
			 0xabef1da0, /// 0x850
			 0xc6b33d75, /// 0x854
			 0x0b86dbd5, /// 0x858
			 0x1208a23d, /// 0x85c
			 0xc2d62d5b, /// 0x860
			 0x62a9cc99, /// 0x864
			 0x665b962c, /// 0x868
			 0x01e41bfa, /// 0x86c
			 0x1bb76962, /// 0x870
			 0xc320a78e, /// 0x874
			 0x4e1a5de8, /// 0x878
			 0xc82d4ab2, /// 0x87c
			 0x7db6f08c, /// 0x880
			 0x3dd2473b, /// 0x884
			 0x2307ad6a, /// 0x888
			 0x28e39974, /// 0x88c
			 0x0fcc22c3, /// 0x890
			 0x80dc9e66, /// 0x894
			 0x3fbd7f4e, /// 0x898
			 0x98af2929, /// 0x89c
			 0x8160b21a, /// 0x8a0
			 0xbc1c1605, /// 0x8a4
			 0x2de899ae, /// 0x8a8
			 0x1f841086, /// 0x8ac
			 0x02e8c767, /// 0x8b0
			 0x6f7a5b31, /// 0x8b4
			 0xe0ba6c58, /// 0x8b8
			 0xa5b8c8ba, /// 0x8bc
			 0x5f15947b, /// 0x8c0
			 0x1f4e9cb9, /// 0x8c4
			 0x2ad52bd1, /// 0x8c8
			 0x99e7f534, /// 0x8cc
			 0xda970c4e, /// 0x8d0
			 0xf33555f8, /// 0x8d4
			 0x089c6665, /// 0x8d8
			 0xc93ea85c, /// 0x8dc
			 0xf075ccdc, /// 0x8e0
			 0x9dd5bbcc, /// 0x8e4
			 0xe07d07c9, /// 0x8e8
			 0xb0c4e7ca, /// 0x8ec
			 0x163c2745, /// 0x8f0
			 0x4a244f76, /// 0x8f4
			 0x4822adbf, /// 0x8f8
			 0x68e4e56c, /// 0x8fc
			 0x9c8e34df, /// 0x900
			 0xdfa40b73, /// 0x904
			 0xf35ec98f, /// 0x908
			 0x185fbeb3, /// 0x90c
			 0x1909af47, /// 0x910
			 0x55735372, /// 0x914
			 0x0b1e965f, /// 0x918
			 0xe035dd9b, /// 0x91c
			 0xdcd05189, /// 0x920
			 0xd3745b75, /// 0x924
			 0xbec84dda, /// 0x928
			 0xdf095efb, /// 0x92c
			 0x0297dd1e, /// 0x930
			 0x1ecb7bf7, /// 0x934
			 0x3a42eab6, /// 0x938
			 0xd9c8c4b4, /// 0x93c
			 0x282e879c, /// 0x940
			 0xed3b6f34, /// 0x944
			 0x3481e188, /// 0x948
			 0x10aaeecb, /// 0x94c
			 0x8e0d30a8, /// 0x950
			 0x6d871117, /// 0x954
			 0x74e53d85, /// 0x958
			 0x5bac1b20, /// 0x95c
			 0x5eaa8c90, /// 0x960
			 0xe4859947, /// 0x964
			 0xc62c706c, /// 0x968
			 0x878fe35d, /// 0x96c
			 0x106e37c1, /// 0x970
			 0xc0098d3f, /// 0x974
			 0x69c30dff, /// 0x978
			 0xbdf3fb2b, /// 0x97c
			 0x99832358, /// 0x980
			 0xb9016ff4, /// 0x984
			 0x54964d9c, /// 0x988
			 0x3f65d2ec, /// 0x98c
			 0x2d77b619, /// 0x990
			 0xcb1f0eb9, /// 0x994
			 0x29baeb43, /// 0x998
			 0x2454d5f7, /// 0x99c
			 0xa5be176d, /// 0x9a0
			 0x19410e9a, /// 0x9a4
			 0xcc590c84, /// 0x9a8
			 0xc2512d54, /// 0x9ac
			 0x71d02e2d, /// 0x9b0
			 0x5f8ace2c, /// 0x9b4
			 0x53f6f0e3, /// 0x9b8
			 0xf5c51e9a, /// 0x9bc
			 0x9e965808, /// 0x9c0
			 0x8b7b4417, /// 0x9c4
			 0xd0f930df, /// 0x9c8
			 0x34eacacd, /// 0x9cc
			 0x0e5cfb81, /// 0x9d0
			 0x8ba9eb7e, /// 0x9d4
			 0x31b74c9f, /// 0x9d8
			 0xe0bfee63, /// 0x9dc
			 0xaf6ad331, /// 0x9e0
			 0xdf64dd82, /// 0x9e4
			 0x101e114a, /// 0x9e8
			 0xc3e7d035, /// 0x9ec
			 0x36224176, /// 0x9f0
			 0x33e5924c, /// 0x9f4
			 0xda371c33, /// 0x9f8
			 0x48802689, /// 0x9fc
			 0xabf4bcfe, /// 0xa00
			 0x0889dbbb, /// 0xa04
			 0xa69270f2, /// 0xa08
			 0x20fbb02a, /// 0xa0c
			 0x96138ce6, /// 0xa10
			 0xefdf109f, /// 0xa14
			 0x2fe702e3, /// 0xa18
			 0x4779167d, /// 0xa1c
			 0x575a0fd9, /// 0xa20
			 0xa162863a, /// 0xa24
			 0x8475c8f8, /// 0xa28
			 0x01c86c2d, /// 0xa2c
			 0xc04d5d02, /// 0xa30
			 0x9a20c207, /// 0xa34
			 0x30f9d47d, /// 0xa38
			 0xa6a16543, /// 0xa3c
			 0x5223177d, /// 0xa40
			 0xdc6b9a90, /// 0xa44
			 0x7c1e08e1, /// 0xa48
			 0x7c3b2995, /// 0xa4c
			 0x9dfd15a9, /// 0xa50
			 0x8e84fadd, /// 0xa54
			 0xfa66ab42, /// 0xa58
			 0xae2cd2d0, /// 0xa5c
			 0xb5f7d443, /// 0xa60
			 0xd11d181c, /// 0xa64
			 0xd942417e, /// 0xa68
			 0x3c3e4b80, /// 0xa6c
			 0xec5acff5, /// 0xa70
			 0x1a4f21e2, /// 0xa74
			 0x5c4ad629, /// 0xa78
			 0x481fe2c9, /// 0xa7c
			 0x720a1eba, /// 0xa80
			 0x19bf6a1c, /// 0xa84
			 0x862d3264, /// 0xa88
			 0xb998217f, /// 0xa8c
			 0x65c276b8, /// 0xa90
			 0x804f35e4, /// 0xa94
			 0x2e69531f, /// 0xa98
			 0x7b5c931d, /// 0xa9c
			 0xabd2ea6c, /// 0xaa0
			 0xab107ce5, /// 0xaa4
			 0x358833ea, /// 0xaa8
			 0x5d933a6f, /// 0xaac
			 0xd9a40947, /// 0xab0
			 0xc77be579, /// 0xab4
			 0x1c92494b, /// 0xab8
			 0xda7645ab, /// 0xabc
			 0x008720ba, /// 0xac0
			 0xbc1017a9, /// 0xac4
			 0xf21fda0f, /// 0xac8
			 0xd20c7a33, /// 0xacc
			 0xc45071b8, /// 0xad0
			 0xd8151860, /// 0xad4
			 0x4af0b900, /// 0xad8
			 0x149fb3e5, /// 0xadc
			 0xc776d523, /// 0xae0
			 0xd6933c78, /// 0xae4
			 0x12d22483, /// 0xae8
			 0x0b4807a6, /// 0xaec
			 0x0a62f3ee, /// 0xaf0
			 0x584bcb3b, /// 0xaf4
			 0x1b9af4d9, /// 0xaf8
			 0x5b271f4c, /// 0xafc
			 0xc580a03e, /// 0xb00
			 0x570fb6fb, /// 0xb04
			 0x679bf70c, /// 0xb08
			 0x4dfa33d3, /// 0xb0c
			 0x64438a8b, /// 0xb10
			 0x6c018f83, /// 0xb14
			 0x9379e646, /// 0xb18
			 0x01e0d629, /// 0xb1c
			 0x23964088, /// 0xb20
			 0xf974fba3, /// 0xb24
			 0xe59e5ad7, /// 0xb28
			 0x06e0e3da, /// 0xb2c
			 0xf52ccb73, /// 0xb30
			 0xb64d4e48, /// 0xb34
			 0x47278d7b, /// 0xb38
			 0x14c219dc, /// 0xb3c
			 0x437f96a2, /// 0xb40
			 0x879acb3a, /// 0xb44
			 0x79e88c6d, /// 0xb48
			 0x25d73050, /// 0xb4c
			 0xacaea061, /// 0xb50
			 0x39f06325, /// 0xb54
			 0x77825f46, /// 0xb58
			 0x6901922c, /// 0xb5c
			 0xe6eb5b5d, /// 0xb60
			 0x4e78d9a2, /// 0xb64
			 0x35de1983, /// 0xb68
			 0xcd73e98e, /// 0xb6c
			 0x7c0afb2c, /// 0xb70
			 0x12f1114f, /// 0xb74
			 0xec877858, /// 0xb78
			 0x0e1c0925, /// 0xb7c
			 0x00e2a114, /// 0xb80
			 0x93fb7c69, /// 0xb84
			 0x2ae49620, /// 0xb88
			 0x8c4245b8, /// 0xb8c
			 0x45a4a1e7, /// 0xb90
			 0xba716eea, /// 0xb94
			 0x2a478c62, /// 0xb98
			 0x7c56fea8, /// 0xb9c
			 0xcb1d5034, /// 0xba0
			 0x45b784ad, /// 0xba4
			 0x4bc21082, /// 0xba8
			 0xe7239692, /// 0xbac
			 0x75a836fc, /// 0xbb0
			 0xa00ba414, /// 0xbb4
			 0xd6f1b207, /// 0xbb8
			 0xf6421cbc, /// 0xbbc
			 0xd62134a5, /// 0xbc0
			 0x7c00098d, /// 0xbc4
			 0xdd517539, /// 0xbc8
			 0x90c7f78e, /// 0xbcc
			 0x55ef4ae0, /// 0xbd0
			 0xbfc86060, /// 0xbd4
			 0xb2add2bb, /// 0xbd8
			 0xd074ecfe, /// 0xbdc
			 0x0b106974, /// 0xbe0
			 0x81201137, /// 0xbe4
			 0x00021421, /// 0xbe8
			 0xcf2c6f9e, /// 0xbec
			 0xa70bee2e, /// 0xbf0
			 0x3ec002f4, /// 0xbf4
			 0xa950d7a3, /// 0xbf8
			 0x41012945, /// 0xbfc
			 0x60981ec0, /// 0xc00
			 0x5c95bf5c, /// 0xc04
			 0xa66c45eb, /// 0xc08
			 0xd4c1ee89, /// 0xc0c
			 0x90c5a4d4, /// 0xc10
			 0x9c237940, /// 0xc14
			 0xa6721bc2, /// 0xc18
			 0x21489838, /// 0xc1c
			 0x645670e7, /// 0xc20
			 0x9d86bfa0, /// 0xc24
			 0x0eee3951, /// 0xc28
			 0x66c7fe7e, /// 0xc2c
			 0x83e2c6cf, /// 0xc30
			 0xe33cced1, /// 0xc34
			 0xbacf7313, /// 0xc38
			 0xff2bdd37, /// 0xc3c
			 0x698a2c18, /// 0xc40
			 0xe8ba3788, /// 0xc44
			 0x635aee35, /// 0xc48
			 0xf834575c, /// 0xc4c
			 0xf3cf08bc, /// 0xc50
			 0xd6896188, /// 0xc54
			 0x7221ea1d, /// 0xc58
			 0xda608d0a, /// 0xc5c
			 0x402c02c5, /// 0xc60
			 0xb875afd8, /// 0xc64
			 0xcf870d13, /// 0xc68
			 0x956b5923, /// 0xc6c
			 0x5f4b3d18, /// 0xc70
			 0x0665c220, /// 0xc74
			 0xb84d9409, /// 0xc78
			 0xa82e3256, /// 0xc7c
			 0x2bf44e9b, /// 0xc80
			 0x7ad41b01, /// 0xc84
			 0xcdc1fd47, /// 0xc88
			 0x6cd78123, /// 0xc8c
			 0xd37efd08, /// 0xc90
			 0x1d1b51e4, /// 0xc94
			 0x00431797, /// 0xc98
			 0xc691f8bd, /// 0xc9c
			 0x8c346813, /// 0xca0
			 0x70ac3d44, /// 0xca4
			 0xc0ff35a9, /// 0xca8
			 0x78cec056, /// 0xcac
			 0xb207da33, /// 0xcb0
			 0xad8d194f, /// 0xcb4
			 0xdc79d120, /// 0xcb8
			 0x1d51228b, /// 0xcbc
			 0xc1c5c557, /// 0xcc0
			 0xe31f913e, /// 0xcc4
			 0xa391344c, /// 0xcc8
			 0x300b1da8, /// 0xccc
			 0x7a8c38ea, /// 0xcd0
			 0x5e068ca1, /// 0xcd4
			 0x466c8a93, /// 0xcd8
			 0xf72ad1a5, /// 0xcdc
			 0x3703a1e6, /// 0xce0
			 0xe5c1503c, /// 0xce4
			 0xe47bbe45, /// 0xce8
			 0x5d6af63d, /// 0xcec
			 0x79db00b7, /// 0xcf0
			 0x6dc32434, /// 0xcf4
			 0x097ca771, /// 0xcf8
			 0x20ec1b2a, /// 0xcfc
			 0xb6484c0f, /// 0xd00
			 0x1cea39a8, /// 0xd04
			 0x3c0a935e, /// 0xd08
			 0x3577996f, /// 0xd0c
			 0x226cae75, /// 0xd10
			 0x1b4cf062, /// 0xd14
			 0x91ef8b34, /// 0xd18
			 0x71b41224, /// 0xd1c
			 0x4ac2e939, /// 0xd20
			 0xa30cd09c, /// 0xd24
			 0x680b3080, /// 0xd28
			 0x62e51387, /// 0xd2c
			 0x8825cfc3, /// 0xd30
			 0x419f0a59, /// 0xd34
			 0x9dde4a15, /// 0xd38
			 0x4bb4be26, /// 0xd3c
			 0x690ebdcf, /// 0xd40
			 0xb16fa9e4, /// 0xd44
			 0x11edc0e3, /// 0xd48
			 0x254d1e20, /// 0xd4c
			 0xf370a3e6, /// 0xd50
			 0x5b9d7e05, /// 0xd54
			 0x6e1405f9, /// 0xd58
			 0x6531cd3e, /// 0xd5c
			 0x8de85411, /// 0xd60
			 0xfd19cbe6, /// 0xd64
			 0x6aace8c5, /// 0xd68
			 0x1ba267fc, /// 0xd6c
			 0xef08c9ec, /// 0xd70
			 0x2b91b004, /// 0xd74
			 0xf95f9705, /// 0xd78
			 0x1ebbb5ef, /// 0xd7c
			 0x1d6c7aab, /// 0xd80
			 0xcc94fb41, /// 0xd84
			 0x2ca54894, /// 0xd88
			 0x18dbfe09, /// 0xd8c
			 0xaf718fa4, /// 0xd90
			 0x3f1e8e48, /// 0xd94
			 0xe69a0eea, /// 0xd98
			 0xe16a5614, /// 0xd9c
			 0x3af19315, /// 0xda0
			 0xb1193649, /// 0xda4
			 0xb9b07fa8, /// 0xda8
			 0xb6ae5a5e, /// 0xdac
			 0x9ff6fd97, /// 0xdb0
			 0x0e422dd7, /// 0xdb4
			 0x2a5a40be, /// 0xdb8
			 0x2b8aadc6, /// 0xdbc
			 0x262cee4b, /// 0xdc0
			 0x65459408, /// 0xdc4
			 0xf69ef6b6, /// 0xdc8
			 0x3897bceb, /// 0xdcc
			 0xe4527c19, /// 0xdd0
			 0xa6765cc9, /// 0xdd4
			 0xaaa7ec8d, /// 0xdd8
			 0xde0eb182, /// 0xddc
			 0x393c241a, /// 0xde0
			 0xc6cd7ca1, /// 0xde4
			 0x0fe5e2ed, /// 0xde8
			 0xd2450f30, /// 0xdec
			 0xd34f26c5, /// 0xdf0
			 0x988899e4, /// 0xdf4
			 0x3faa4957, /// 0xdf8
			 0xf98290b8, /// 0xdfc
			 0x8ab8e3ff, /// 0xe00
			 0xbc29605e, /// 0xe04
			 0xe80633ba, /// 0xe08
			 0x1c04084e, /// 0xe0c
			 0x13a1397b, /// 0xe10
			 0x6d8458e3, /// 0xe14
			 0x50591aa6, /// 0xe18
			 0x8d81f896, /// 0xe1c
			 0x4a4a4833, /// 0xe20
			 0x1c42bbf6, /// 0xe24
			 0x2f8f3954, /// 0xe28
			 0x15f82e10, /// 0xe2c
			 0xfec0fd2a, /// 0xe30
			 0xa15db979, /// 0xe34
			 0xb272e4ac, /// 0xe38
			 0x2d113b3f, /// 0xe3c
			 0x72eb405d, /// 0xe40
			 0xd91eab8d, /// 0xe44
			 0xd22e3c96, /// 0xe48
			 0xb473e71a, /// 0xe4c
			 0xb893de5f, /// 0xe50
			 0xcc9794b9, /// 0xe54
			 0x71fb8e0d, /// 0xe58
			 0x0d4b00a7, /// 0xe5c
			 0xf845af66, /// 0xe60
			 0xe142f285, /// 0xe64
			 0x25a14457, /// 0xe68
			 0x8ba138c5, /// 0xe6c
			 0x05253aa6, /// 0xe70
			 0x8fd58917, /// 0xe74
			 0x9ebbb145, /// 0xe78
			 0xe8de4c9f, /// 0xe7c
			 0xb53d9f7d, /// 0xe80
			 0x8bce552e, /// 0xe84
			 0x10c6b24f, /// 0xe88
			 0xd450cff1, /// 0xe8c
			 0x63d153f4, /// 0xe90
			 0x36c1a6e1, /// 0xe94
			 0xa2ffc262, /// 0xe98
			 0xde45c365, /// 0xe9c
			 0x785c52a1, /// 0xea0
			 0xd8f748d6, /// 0xea4
			 0x60513587, /// 0xea8
			 0x2603e89c, /// 0xeac
			 0xd86861a6, /// 0xeb0
			 0x0cabeffd, /// 0xeb4
			 0x69ac56d9, /// 0xeb8
			 0xf79d8a90, /// 0xebc
			 0xbbbae330, /// 0xec0
			 0xff8aefc3, /// 0xec4
			 0xf0ba2949, /// 0xec8
			 0x2d697f79, /// 0xecc
			 0x70a9811c, /// 0xed0
			 0x379e28d0, /// 0xed4
			 0xfdcf53b2, /// 0xed8
			 0x733a92f5, /// 0xedc
			 0x94a99441, /// 0xee0
			 0xb0cccded, /// 0xee4
			 0xb64403b5, /// 0xee8
			 0x53ec6956, /// 0xeec
			 0x911e5c7b, /// 0xef0
			 0xf672a484, /// 0xef4
			 0x64f70f0e, /// 0xef8
			 0x2da4c2cf, /// 0xefc
			 0xa3bc8fe0, /// 0xf00
			 0x1a4a1cde, /// 0xf04
			 0x5b8dd891, /// 0xf08
			 0x7e442b85, /// 0xf0c
			 0xd1f446f7, /// 0xf10
			 0xc4e63f20, /// 0xf14
			 0x9d3fc873, /// 0xf18
			 0x9a3a7e5f, /// 0xf1c
			 0x678ccc40, /// 0xf20
			 0x0c1dbf67, /// 0xf24
			 0x40531563, /// 0xf28
			 0xe13c5eb7, /// 0xf2c
			 0x8dd323ff, /// 0xf30
			 0x5ea71be4, /// 0xf34
			 0x6d42605e, /// 0xf38
			 0x71e0850f, /// 0xf3c
			 0xc72d9cd4, /// 0xf40
			 0x2c228d30, /// 0xf44
			 0x2f047db8, /// 0xf48
			 0x460d541d, /// 0xf4c
			 0x56b92642, /// 0xf50
			 0xc2597ff2, /// 0xf54
			 0xbcb820c8, /// 0xf58
			 0x2f82001d, /// 0xf5c
			 0x7e358641, /// 0xf60
			 0x3db30b1a, /// 0xf64
			 0x6cfea29d, /// 0xf68
			 0xeb66ba89, /// 0xf6c
			 0x122410ad, /// 0xf70
			 0x8c263bb2, /// 0xf74
			 0xf836a1f0, /// 0xf78
			 0xd798cb72, /// 0xf7c
			 0x3612c404, /// 0xf80
			 0x0a22d56b, /// 0xf84
			 0x10f97983, /// 0xf88
			 0xf53e1923, /// 0xf8c
			 0xa86e4079, /// 0xf90
			 0xeda899e0, /// 0xf94
			 0x7897ff28, /// 0xf98
			 0xe612dedf, /// 0xf9c
			 0xd0c6e47a, /// 0xfa0
			 0x0fdcf968, /// 0xfa4
			 0xcb895cd8, /// 0xfa8
			 0xbd8bed7f, /// 0xfac
			 0x559bedfd, /// 0xfb0
			 0x79adb1cb, /// 0xfb4
			 0x0a6e4790, /// 0xfb8
			 0x5e9a29c0, /// 0xfbc
			 0x3175a142, /// 0xfc0
			 0x0f189110, /// 0xfc4
			 0x3bb6222d, /// 0xfc8
			 0xc12a060b, /// 0xfcc
			 0xa90ab86c, /// 0xfd0
			 0xb5d9bc36, /// 0xfd4
			 0x4caa099a, /// 0xfd8
			 0x67823274, /// 0xfdc
			 0x4adee09b, /// 0xfe0
			 0x685c3e27, /// 0xfe4
			 0x99c3168c, /// 0xfe8
			 0x39f61b1d, /// 0xfec
			 0xca8bc583, /// 0xff0
			 0x75999fe0, /// 0xff4
			 0xec4528d1, /// 0xff8
			 0xeb48f2e7 /// last
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
			 0x00000608,
			 0x000004c0,
			 0x00000154,
			 0x000003a0,
			 0x0000028c,
			 0x000004f8,
			 0x000002a0,
			 0x00000600,

			 /// vpu register f2
			 0x40a00000,
			 0x41000000,
			 0x41980000,
			 0x41e80000,
			 0x41f80000,
			 0x41d00000,
			 0x41b00000,
			 0x41200000,

			 /// vpu register f3
			 0x41b00000,
			 0x41e00000,
			 0x41e00000,
			 0x40c00000,
			 0x41900000,
			 0x41a80000,
			 0x41300000,
			 0x41f00000,

			 /// vpu register f4
			 0x41100000,
			 0x41c00000,
			 0x41b80000,
			 0x41500000,
			 0x40e00000,
			 0x41880000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f5
			 0x40e00000,
			 0x41900000,
			 0x41880000,
			 0x41f80000,
			 0x41200000,
			 0x40e00000,
			 0x41700000,
			 0x41800000,

			 /// vpu register f6
			 0x41000000,
			 0x41800000,
			 0x41600000,
			 0x41500000,
			 0x41d00000,
			 0x41600000,
			 0x40000000,
			 0x41c80000,

			 /// vpu register f7
			 0x41980000,
			 0x3f800000,
			 0x40c00000,
			 0x40400000,
			 0x41e80000,
			 0x40400000,
			 0x42000000,
			 0x40400000,

			 /// vpu register f8
			 0x40400000,
			 0x41f00000,
			 0x41d80000,
			 0x41c80000,
			 0x41a00000,
			 0x41d80000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f9
			 0x41c80000,
			 0x41c00000,
			 0x41f00000,
			 0x40e00000,
			 0x40e00000,
			 0x41300000,
			 0x41e00000,
			 0x41980000,

			 /// vpu register f10
			 0x41e00000,
			 0x41b80000,
			 0x41d00000,
			 0x40400000,
			 0x41c80000,
			 0x40a00000,
			 0x41800000,
			 0x41e80000,

			 /// vpu register f11
			 0x41c00000,
			 0x3f800000,
			 0x40e00000,
			 0x40000000,
			 0x40a00000,
			 0x41000000,
			 0x40e00000,
			 0x41a80000,

			 /// vpu register f12
			 0x41900000,
			 0x41880000,
			 0x41f80000,
			 0x40400000,
			 0x42000000,
			 0x41c00000,
			 0x41b80000,
			 0x41900000,

			 /// vpu register f13
			 0x41300000,
			 0x41c80000,
			 0x40e00000,
			 0x41900000,
			 0x41400000,
			 0x40000000,
			 0x41200000,
			 0x41c00000,

			 /// vpu register f14
			 0x41200000,
			 0x41100000,
			 0x41000000,
			 0x41b80000,
			 0x3f800000,
			 0x41700000,
			 0x40000000,
			 0x41b00000,

			 /// vpu register f15
			 0x41700000,
			 0x40a00000,
			 0x41880000,
			 0x41400000,
			 0x41f00000,
			 0x41300000,
			 0x41a80000,
			 0x41e80000,

			 /// vpu register f16
			 0x40000000,
			 0x41900000,
			 0x41e80000,
			 0x41200000,
			 0x41000000,
			 0x40800000,
			 0x41700000,
			 0x41000000,

			 /// vpu register f17
			 0x41d00000,
			 0x40c00000,
			 0x41e00000,
			 0x41000000,
			 0x41000000,
			 0x41700000,
			 0x41500000,
			 0x41400000,

			 /// vpu register f18
			 0x40a00000,
			 0x41f80000,
			 0x41400000,
			 0x41200000,
			 0x40a00000,
			 0x41500000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f19
			 0x40000000,
			 0x41f80000,
			 0x41300000,
			 0x41800000,
			 0x41d00000,
			 0x41800000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f20
			 0x41400000,
			 0x41800000,
			 0x41880000,
			 0x41800000,
			 0x41980000,
			 0x41000000,
			 0x41800000,
			 0x41b00000,

			 /// vpu register f21
			 0x41000000,
			 0x40e00000,
			 0x41c00000,
			 0x41c80000,
			 0x41d00000,
			 0x41d80000,
			 0x41600000,
			 0x41700000,

			 /// vpu register f22
			 0x40400000,
			 0x41f80000,
			 0x40400000,
			 0x40a00000,
			 0x41900000,
			 0x41f80000,
			 0x40c00000,
			 0x41000000,

			 /// vpu register f23
			 0x41500000,
			 0x41300000,
			 0x41100000,
			 0x41d80000,
			 0x41900000,
			 0x41000000,
			 0x41100000,
			 0x41600000,

			 /// vpu register f24
			 0x41600000,
			 0x42000000,
			 0x41a80000,
			 0x41c80000,
			 0x41f80000,
			 0x41200000,
			 0x42000000,
			 0x41a00000,

			 /// vpu register f25
			 0x42000000,
			 0x41200000,
			 0x41b00000,
			 0x41200000,
			 0x41f80000,
			 0x41200000,
			 0x41980000,
			 0x41a80000,

			 /// vpu register f26
			 0x40000000,
			 0x40a00000,
			 0x42000000,
			 0x41000000,
			 0x40e00000,
			 0x41b00000,
			 0x41e00000,
			 0x40400000,

			 /// vpu register f27
			 0x41c00000,
			 0x40c00000,
			 0x41c00000,
			 0x3f800000,
			 0x41b00000,
			 0x41c80000,
			 0x41400000,
			 0x41c80000,

			 /// vpu register f28
			 0x41f00000,
			 0x41500000,
			 0x41c00000,
			 0x41400000,
			 0x41400000,
			 0x41000000,
			 0x41b00000,
			 0x41500000,

			 /// vpu register f29
			 0x41600000,
			 0x40e00000,
			 0x41600000,
			 0x40e00000,
			 0x40a00000,
			 0x40e00000,
			 0x40a00000,
			 0x41700000,

			 /// vpu register f30
			 0x41980000,
			 0x42000000,
			 0x41f00000,
			 0x41e80000,
			 0x41b00000,
			 0x3f800000,
			 0x41200000,
			 0x41900000,

			 /// vpu register f31
			 0x42000000,
			 0x40e00000,
			 0x41b80000,
			 0x3f800000,
			 0x41d00000,
			 0x41800000,
			 0x41880000,
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
		"fexp.ps f9, f18\n"                                   ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f6, f6\n"                                    ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f24, f30\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f28, f10\n"                                  ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f6, f20\n"                                   ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f14\n"                                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f16, f25\n"                                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f29, f7\n"                                   ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f28\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f2, f13\n"                                   ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f3, f2\n"                                    ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f7\n"                                    ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f10, f26\n"                                  ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f1\n"                                   ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f9, f4\n"                                    ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f14, f15\n"                                  ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f14, f6\n"                                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f19, f22\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f6, f3\n"                                    ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f14, f14\n"                                  ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f30, f30\n"                                  ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f23, f15\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f22, f22\n"                                  ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f17, f8\n"                                   ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f8, f16\n"                                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f3, f3\n"                                    ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f23, f23\n"                                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f5, f12\n"                                   ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f3, f28\n"                                   ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f30, f27\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f10\n"                                  ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f17, f27\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f15\n"                                   ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f17, f17\n"                                  ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f30, f19\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f6, f11\n"                                   ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f3, f0\n"                                    ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f12, f24\n"                                  ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f13, f20\n"                                  ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f10\n"                                   ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f24, f27\n"                                  ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f11, f14\n"                                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f24, f29\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f5, f0\n"                                    ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f5, f29\n"                                   ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f11, f27\n"                                  ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f3, f20\n"                                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f9, f8\n"                                    ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f17, f24\n"                                  ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f6, f6\n"                                    ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f20, f19\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f5, f29\n"                                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f29, f8\n"                                   ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f25, f28\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f8, f24\n"                                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f8, f3\n"                                    ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f5\n"                                    ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f26, f26\n"                                  ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f6, f12\n"                                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f19, f18\n"                                  ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f28\n"                                  ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f28, f19\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f17, f9\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f4, f1\n"                                    ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f12, f21\n"                                  ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f11\n"                                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f25, f5\n"                                   ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f6\n"                                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f20, f18\n"                                  ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f26\n"                                   ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f14, f12\n"                                  ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f11, f20\n"                                  ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f23, f11\n"                                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f30, f24\n"                                  ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f5, f14\n"                                   ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f11, f1\n"                                   ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f23\n"                                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f23, f16\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f20, f17\n"                                  ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f4, f24\n"                                   ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f25\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f18, f6\n"                                   ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f28, f8\n"                                   ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f22, f8\n"                                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f4, f1\n"                                    ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f30, f30\n"                                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f20, f23\n"                                  ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f19, f9\n"                                   ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f17, f4\n"                                   ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f7, f18\n"                                   ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f28, f6\n"                                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f3, f25\n"                                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f30\n"                                  ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f29\n"                                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f26, f29\n"                                  ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f29, f10\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f8, f10\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f9, f27\n"                                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fexp.ps f21, f3\n"                                   ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
