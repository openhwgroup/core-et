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
			 0x63b49a55, /// 0x0
			 0x80feb6e0, /// 0x4
			 0x1cd366c8, /// 0x8
			 0xacbf88e6, /// 0xc
			 0x4570b555, /// 0x10
			 0xb4cbfd63, /// 0x14
			 0xb6dff3fd, /// 0x18
			 0xfdb153df, /// 0x1c
			 0xb324856c, /// 0x20
			 0x6ea9d74d, /// 0x24
			 0xa3c48875, /// 0x28
			 0xf5aeeeab, /// 0x2c
			 0x0a340ce2, /// 0x30
			 0xa9b035ef, /// 0x34
			 0x3cd460c9, /// 0x38
			 0x9e63d942, /// 0x3c
			 0xd95c2243, /// 0x40
			 0xf71da1c9, /// 0x44
			 0x9bef9dbe, /// 0x48
			 0x75372b96, /// 0x4c
			 0x10604943, /// 0x50
			 0x9593192b, /// 0x54
			 0xf41d1edf, /// 0x58
			 0x2115cdcd, /// 0x5c
			 0x241fed53, /// 0x60
			 0x9a6e6ac2, /// 0x64
			 0x8234deff, /// 0x68
			 0xed682106, /// 0x6c
			 0xe1137580, /// 0x70
			 0xb03d0577, /// 0x74
			 0x59d95ec0, /// 0x78
			 0x84badb19, /// 0x7c
			 0x027d10ac, /// 0x80
			 0xdd8e63eb, /// 0x84
			 0xbda00de7, /// 0x88
			 0xf7db520d, /// 0x8c
			 0x584392f8, /// 0x90
			 0x5b833e11, /// 0x94
			 0x611fa7c2, /// 0x98
			 0x432a1808, /// 0x9c
			 0x16e23353, /// 0xa0
			 0x13bc9d10, /// 0xa4
			 0x2667f055, /// 0xa8
			 0x4558ee66, /// 0xac
			 0xbd80bf1b, /// 0xb0
			 0xaa76c4cd, /// 0xb4
			 0xac9863ef, /// 0xb8
			 0x23746af1, /// 0xbc
			 0x4e7c88fe, /// 0xc0
			 0xc381d3b7, /// 0xc4
			 0x1fccb732, /// 0xc8
			 0x91f1c717, /// 0xcc
			 0x5282c807, /// 0xd0
			 0xc50c333e, /// 0xd4
			 0xade1d138, /// 0xd8
			 0xaab01d48, /// 0xdc
			 0xc87a5792, /// 0xe0
			 0x76926ef8, /// 0xe4
			 0x93b7d8a3, /// 0xe8
			 0x6cc4c5f9, /// 0xec
			 0x1b339b61, /// 0xf0
			 0x41a2867f, /// 0xf4
			 0xe666ea21, /// 0xf8
			 0xa9d95ad9, /// 0xfc
			 0xc8904688, /// 0x100
			 0x01952dcd, /// 0x104
			 0x7663651f, /// 0x108
			 0xd0c364c1, /// 0x10c
			 0xb230f1f6, /// 0x110
			 0x5fb1ca9b, /// 0x114
			 0x034dec51, /// 0x118
			 0xef07e188, /// 0x11c
			 0x4e219bf4, /// 0x120
			 0xe1840b32, /// 0x124
			 0xab853d93, /// 0x128
			 0x8b11648e, /// 0x12c
			 0x0d680a5d, /// 0x130
			 0xcb5b104d, /// 0x134
			 0xd40e3e73, /// 0x138
			 0x6166182c, /// 0x13c
			 0x1c48ad68, /// 0x140
			 0xa8d50ce6, /// 0x144
			 0x39eb07ba, /// 0x148
			 0xa648a401, /// 0x14c
			 0x9ab63c87, /// 0x150
			 0xac2b4609, /// 0x154
			 0xff197f39, /// 0x158
			 0x24e01950, /// 0x15c
			 0x564de025, /// 0x160
			 0xef61173d, /// 0x164
			 0xb2e6a5f8, /// 0x168
			 0x1f5a28cf, /// 0x16c
			 0x4cfc599b, /// 0x170
			 0xae7cd7fb, /// 0x174
			 0x8201cce2, /// 0x178
			 0xdec16c0d, /// 0x17c
			 0x88db4cc7, /// 0x180
			 0x787f9eb5, /// 0x184
			 0x5f11ae86, /// 0x188
			 0xc3048993, /// 0x18c
			 0x7d65090a, /// 0x190
			 0x535be84e, /// 0x194
			 0x721c97d0, /// 0x198
			 0xe532a91a, /// 0x19c
			 0xf68cb4c2, /// 0x1a0
			 0xda2c674b, /// 0x1a4
			 0xad7cf834, /// 0x1a8
			 0x520629df, /// 0x1ac
			 0xe65f6fe1, /// 0x1b0
			 0x9c70a55b, /// 0x1b4
			 0xdf172f6c, /// 0x1b8
			 0x8257aeb4, /// 0x1bc
			 0xb1acf380, /// 0x1c0
			 0x46801b17, /// 0x1c4
			 0x59be6aec, /// 0x1c8
			 0xe660649c, /// 0x1cc
			 0xf6fd8a35, /// 0x1d0
			 0xfb61858b, /// 0x1d4
			 0x1d5ea37f, /// 0x1d8
			 0xd03d60c7, /// 0x1dc
			 0x99fbd80b, /// 0x1e0
			 0x92ce5473, /// 0x1e4
			 0x8d5ab50c, /// 0x1e8
			 0x3f7e5cdc, /// 0x1ec
			 0x5d36d0d4, /// 0x1f0
			 0xed916aec, /// 0x1f4
			 0x47cfab94, /// 0x1f8
			 0x0070d0e8, /// 0x1fc
			 0x514be296, /// 0x200
			 0x717fb8fa, /// 0x204
			 0x40acfd98, /// 0x208
			 0xa1ec7ec3, /// 0x20c
			 0x32069634, /// 0x210
			 0x7e75aaa9, /// 0x214
			 0x066961cc, /// 0x218
			 0xd8b50252, /// 0x21c
			 0x9e4d078f, /// 0x220
			 0x3028d01b, /// 0x224
			 0x0e4faeb1, /// 0x228
			 0xc1b12a2d, /// 0x22c
			 0xfca6e280, /// 0x230
			 0x92e131b8, /// 0x234
			 0x63610774, /// 0x238
			 0xd34d3f4e, /// 0x23c
			 0xed1860e1, /// 0x240
			 0x7b7924b1, /// 0x244
			 0x5e3bc77f, /// 0x248
			 0x77764b66, /// 0x24c
			 0xae6480b8, /// 0x250
			 0x48fe8c0d, /// 0x254
			 0x2b307ff4, /// 0x258
			 0x5dc784e2, /// 0x25c
			 0x8a21b47e, /// 0x260
			 0x09578099, /// 0x264
			 0x6032f443, /// 0x268
			 0x569564be, /// 0x26c
			 0xcb0b935c, /// 0x270
			 0xf7cfb363, /// 0x274
			 0xa3141a98, /// 0x278
			 0xc21cb264, /// 0x27c
			 0x5aa54a04, /// 0x280
			 0xdbec419d, /// 0x284
			 0xdd440700, /// 0x288
			 0xc95ac23b, /// 0x28c
			 0xa2cb1cf6, /// 0x290
			 0xdc1bc2c8, /// 0x294
			 0x642455e9, /// 0x298
			 0xa6fd56d9, /// 0x29c
			 0x14f22b0f, /// 0x2a0
			 0x6e0e9eb2, /// 0x2a4
			 0xaaacaede, /// 0x2a8
			 0xab5c080d, /// 0x2ac
			 0x23b553da, /// 0x2b0
			 0x4fae89b2, /// 0x2b4
			 0xc40ed519, /// 0x2b8
			 0x5938bec1, /// 0x2bc
			 0xe27fd214, /// 0x2c0
			 0x31cc1279, /// 0x2c4
			 0x07fd702a, /// 0x2c8
			 0x9f50ea11, /// 0x2cc
			 0x8b3abf9e, /// 0x2d0
			 0x63d5cdcf, /// 0x2d4
			 0xac61ebf8, /// 0x2d8
			 0xd456904b, /// 0x2dc
			 0xa006ab5a, /// 0x2e0
			 0x1099ba33, /// 0x2e4
			 0xbb3290ce, /// 0x2e8
			 0xcb889414, /// 0x2ec
			 0xd64a5572, /// 0x2f0
			 0x59d70dbe, /// 0x2f4
			 0xe286e1c8, /// 0x2f8
			 0x973148be, /// 0x2fc
			 0xbb5d6f59, /// 0x300
			 0x96f63c2c, /// 0x304
			 0x1731431a, /// 0x308
			 0x952cbbae, /// 0x30c
			 0xe8dc6983, /// 0x310
			 0x72840906, /// 0x314
			 0x8cd6e984, /// 0x318
			 0x7cca8850, /// 0x31c
			 0xb532cb2f, /// 0x320
			 0x9856f9ea, /// 0x324
			 0xefbc0d77, /// 0x328
			 0xdea2b4ac, /// 0x32c
			 0xbb85d5e2, /// 0x330
			 0x728479ed, /// 0x334
			 0xfd58ad83, /// 0x338
			 0xac2df9aa, /// 0x33c
			 0xc6e805e2, /// 0x340
			 0x5c7b15c1, /// 0x344
			 0x957743d8, /// 0x348
			 0xb5907625, /// 0x34c
			 0x6b8d45fd, /// 0x350
			 0xaca65a3d, /// 0x354
			 0x60616f8f, /// 0x358
			 0x1e5a7337, /// 0x35c
			 0xc2ffe1b7, /// 0x360
			 0x232094ab, /// 0x364
			 0xdffc954e, /// 0x368
			 0xdfc574c8, /// 0x36c
			 0x9ce168c8, /// 0x370
			 0x58c60f16, /// 0x374
			 0x91a2d600, /// 0x378
			 0x41c2345f, /// 0x37c
			 0xffcd67b5, /// 0x380
			 0xae120304, /// 0x384
			 0x79c73ce2, /// 0x388
			 0xbe8b3e58, /// 0x38c
			 0xda4cea07, /// 0x390
			 0x13fc83c9, /// 0x394
			 0x5aa886be, /// 0x398
			 0xaeb22062, /// 0x39c
			 0x6f292e82, /// 0x3a0
			 0x4b6fea53, /// 0x3a4
			 0x416ed98e, /// 0x3a8
			 0x1ed57427, /// 0x3ac
			 0xf4326147, /// 0x3b0
			 0x169dfbea, /// 0x3b4
			 0x5ab94ded, /// 0x3b8
			 0x2c2eee2c, /// 0x3bc
			 0x17f7f079, /// 0x3c0
			 0x287c956f, /// 0x3c4
			 0x92f3eaca, /// 0x3c8
			 0x237ba069, /// 0x3cc
			 0x8b7f0020, /// 0x3d0
			 0x8f6b3c6e, /// 0x3d4
			 0xab9ccf15, /// 0x3d8
			 0xb6531bd1, /// 0x3dc
			 0xf5aaa62d, /// 0x3e0
			 0x24206727, /// 0x3e4
			 0x117b7910, /// 0x3e8
			 0xf6f17f80, /// 0x3ec
			 0xa1480dde, /// 0x3f0
			 0x1de62da5, /// 0x3f4
			 0xd6e496a9, /// 0x3f8
			 0xc5705fb5, /// 0x3fc
			 0x651928ca, /// 0x400
			 0x513a7dfc, /// 0x404
			 0x451f91b7, /// 0x408
			 0x5e36a5aa, /// 0x40c
			 0xdf1627e7, /// 0x410
			 0x8f2693f4, /// 0x414
			 0xf8679c98, /// 0x418
			 0xbd7cc8da, /// 0x41c
			 0x3195fb51, /// 0x420
			 0x6193692e, /// 0x424
			 0x92c46b31, /// 0x428
			 0x9fa22714, /// 0x42c
			 0x0a645b73, /// 0x430
			 0xd210057a, /// 0x434
			 0x4e4a236c, /// 0x438
			 0x72867163, /// 0x43c
			 0x3d1125ef, /// 0x440
			 0x8827e570, /// 0x444
			 0x8c1c51da, /// 0x448
			 0x677823ae, /// 0x44c
			 0x20617747, /// 0x450
			 0xcd42b830, /// 0x454
			 0x15609c9b, /// 0x458
			 0xe0e800e6, /// 0x45c
			 0x3629756c, /// 0x460
			 0x51ccb2c7, /// 0x464
			 0x7d9a75fb, /// 0x468
			 0x660319c6, /// 0x46c
			 0xb45ec638, /// 0x470
			 0x27ff9f48, /// 0x474
			 0xe6079914, /// 0x478
			 0xf4a50183, /// 0x47c
			 0x340d2c6c, /// 0x480
			 0x86c370dd, /// 0x484
			 0x1841ece0, /// 0x488
			 0xc82ac796, /// 0x48c
			 0x61706bf5, /// 0x490
			 0xce8213a1, /// 0x494
			 0xbf1402a5, /// 0x498
			 0xfbcec789, /// 0x49c
			 0xc04871af, /// 0x4a0
			 0x075ec5db, /// 0x4a4
			 0xd428d252, /// 0x4a8
			 0x5d5f9c35, /// 0x4ac
			 0xa5159d75, /// 0x4b0
			 0x280307dc, /// 0x4b4
			 0xf5e7136b, /// 0x4b8
			 0x33da1ae0, /// 0x4bc
			 0x3f68bd16, /// 0x4c0
			 0x9c90b3b3, /// 0x4c4
			 0x56980fa0, /// 0x4c8
			 0x8fcb9abd, /// 0x4cc
			 0xa6f2bc30, /// 0x4d0
			 0x63aefda2, /// 0x4d4
			 0x2a148d88, /// 0x4d8
			 0xb6e29a1f, /// 0x4dc
			 0x18a3b859, /// 0x4e0
			 0xcbaaaabb, /// 0x4e4
			 0xb40972a1, /// 0x4e8
			 0x7ec23e63, /// 0x4ec
			 0xc1367ea4, /// 0x4f0
			 0x9f87f2ac, /// 0x4f4
			 0xf775c41d, /// 0x4f8
			 0x527a4432, /// 0x4fc
			 0xea984c64, /// 0x500
			 0xb1593cf4, /// 0x504
			 0xc6a5f481, /// 0x508
			 0x3596d968, /// 0x50c
			 0x498a25e4, /// 0x510
			 0x2e41784c, /// 0x514
			 0x4061866f, /// 0x518
			 0x8a334826, /// 0x51c
			 0x11f969c0, /// 0x520
			 0x949808cc, /// 0x524
			 0x023e8b97, /// 0x528
			 0xbd15b0e5, /// 0x52c
			 0x35b9bfeb, /// 0x530
			 0xa4697866, /// 0x534
			 0xc40ce4ac, /// 0x538
			 0x7c3c9ba6, /// 0x53c
			 0xfece9adb, /// 0x540
			 0xf00f3821, /// 0x544
			 0x2ac0e690, /// 0x548
			 0xec62d08a, /// 0x54c
			 0x023fc910, /// 0x550
			 0x584bb780, /// 0x554
			 0xc2f5125b, /// 0x558
			 0x77f9b3fd, /// 0x55c
			 0x4408166c, /// 0x560
			 0x7fb98d15, /// 0x564
			 0x174c0fd7, /// 0x568
			 0xed60a3a1, /// 0x56c
			 0xd068cbe8, /// 0x570
			 0x88134cd5, /// 0x574
			 0xe4ba400e, /// 0x578
			 0xfb1e1651, /// 0x57c
			 0xe57bb994, /// 0x580
			 0xacec7e1c, /// 0x584
			 0xb79243bc, /// 0x588
			 0x042fcf55, /// 0x58c
			 0x2407cd83, /// 0x590
			 0xfcc0ad69, /// 0x594
			 0x7fd241d4, /// 0x598
			 0xab797891, /// 0x59c
			 0xdb618ec6, /// 0x5a0
			 0xda3dd62e, /// 0x5a4
			 0x7e78b628, /// 0x5a8
			 0xf38435b2, /// 0x5ac
			 0xfeda9fa5, /// 0x5b0
			 0xd811dbfb, /// 0x5b4
			 0x75a23b69, /// 0x5b8
			 0x09824740, /// 0x5bc
			 0xfa74b39d, /// 0x5c0
			 0x4c2fff0a, /// 0x5c4
			 0x273749c4, /// 0x5c8
			 0x59c8aa21, /// 0x5cc
			 0xd0628465, /// 0x5d0
			 0xa11914e2, /// 0x5d4
			 0x96cce612, /// 0x5d8
			 0x1cd13d4a, /// 0x5dc
			 0x831bebe5, /// 0x5e0
			 0xa25bf19e, /// 0x5e4
			 0xee06e385, /// 0x5e8
			 0x6712a52f, /// 0x5ec
			 0x5325d732, /// 0x5f0
			 0x64086078, /// 0x5f4
			 0x93c7cc23, /// 0x5f8
			 0x9ce0cd2f, /// 0x5fc
			 0x37b93e1e, /// 0x600
			 0x33db4358, /// 0x604
			 0x5f94b4c8, /// 0x608
			 0x099ad98c, /// 0x60c
			 0xe01a30f9, /// 0x610
			 0xd2d5ad5d, /// 0x614
			 0xc17f4e18, /// 0x618
			 0x7ae5cbf3, /// 0x61c
			 0xa0e4c146, /// 0x620
			 0x5e43a1bf, /// 0x624
			 0xe0b527ec, /// 0x628
			 0xf39409db, /// 0x62c
			 0xf2490c7b, /// 0x630
			 0xdabe36bc, /// 0x634
			 0x38ea9339, /// 0x638
			 0xcd097fcd, /// 0x63c
			 0x0d237396, /// 0x640
			 0x24918eef, /// 0x644
			 0x7d65e46d, /// 0x648
			 0x37da3d34, /// 0x64c
			 0x31ce57b4, /// 0x650
			 0x092a016d, /// 0x654
			 0x579c52c9, /// 0x658
			 0x3cf13e59, /// 0x65c
			 0x5cd8072e, /// 0x660
			 0x5a6d2299, /// 0x664
			 0x38bd4efe, /// 0x668
			 0x4ba43f52, /// 0x66c
			 0xdc42f825, /// 0x670
			 0x85e64f97, /// 0x674
			 0x30f60e9b, /// 0x678
			 0xe4897e39, /// 0x67c
			 0x2c6f20e1, /// 0x680
			 0x8e4bd8cd, /// 0x684
			 0x8d042d18, /// 0x688
			 0x15205a9f, /// 0x68c
			 0x27383c36, /// 0x690
			 0x79c6a148, /// 0x694
			 0xb57d6ded, /// 0x698
			 0x9bebe25c, /// 0x69c
			 0xdd7a0c95, /// 0x6a0
			 0xc01e9714, /// 0x6a4
			 0x81d64018, /// 0x6a8
			 0x457fc599, /// 0x6ac
			 0x05484a14, /// 0x6b0
			 0x29b43b90, /// 0x6b4
			 0x6292ec08, /// 0x6b8
			 0x0bffcfb4, /// 0x6bc
			 0x571f547d, /// 0x6c0
			 0xc07055fd, /// 0x6c4
			 0xba2fd754, /// 0x6c8
			 0x8fbeb4af, /// 0x6cc
			 0x5e910ce1, /// 0x6d0
			 0xf5893a98, /// 0x6d4
			 0x6b81b89f, /// 0x6d8
			 0xf733d9cb, /// 0x6dc
			 0xef65924a, /// 0x6e0
			 0x0e6d3611, /// 0x6e4
			 0x2ea6b543, /// 0x6e8
			 0xb784a357, /// 0x6ec
			 0xe3b19ba3, /// 0x6f0
			 0x2078b085, /// 0x6f4
			 0xe30765b1, /// 0x6f8
			 0xe83c3f15, /// 0x6fc
			 0x52f66e5d, /// 0x700
			 0x99414f65, /// 0x704
			 0x7c3fd6db, /// 0x708
			 0x2dad110a, /// 0x70c
			 0xd7663f46, /// 0x710
			 0x35bf960c, /// 0x714
			 0xcb033509, /// 0x718
			 0xf59f9d11, /// 0x71c
			 0x29c1ec9b, /// 0x720
			 0xf1bbd3de, /// 0x724
			 0x1353c02e, /// 0x728
			 0xca1772ed, /// 0x72c
			 0xf9c6c651, /// 0x730
			 0x1580fa23, /// 0x734
			 0x2ada3d1a, /// 0x738
			 0xdbc1b72d, /// 0x73c
			 0x4d8c60ac, /// 0x740
			 0x2dc38ff1, /// 0x744
			 0xaf548d0f, /// 0x748
			 0x4a8ec84b, /// 0x74c
			 0xdc674f17, /// 0x750
			 0xc1a9c765, /// 0x754
			 0xbda9bcb5, /// 0x758
			 0x00afe4b9, /// 0x75c
			 0x4563dedf, /// 0x760
			 0x27421d37, /// 0x764
			 0x4a9c01fe, /// 0x768
			 0x25ac2253, /// 0x76c
			 0xb0e62da2, /// 0x770
			 0x185f0396, /// 0x774
			 0x8b84c4d6, /// 0x778
			 0xa5597835, /// 0x77c
			 0x5dea1a33, /// 0x780
			 0xab39ca7f, /// 0x784
			 0xbf71d810, /// 0x788
			 0xe677652c, /// 0x78c
			 0xd7e183a5, /// 0x790
			 0xcf4e2fc5, /// 0x794
			 0xcd6e67a0, /// 0x798
			 0xfed168c1, /// 0x79c
			 0x715bbd0b, /// 0x7a0
			 0x632a2b1b, /// 0x7a4
			 0x06127ceb, /// 0x7a8
			 0xd4a2b6a7, /// 0x7ac
			 0xb668e9de, /// 0x7b0
			 0x97b7d9e9, /// 0x7b4
			 0x610a577f, /// 0x7b8
			 0x7772cc8d, /// 0x7bc
			 0x39c9f25d, /// 0x7c0
			 0xcd821502, /// 0x7c4
			 0xa4759958, /// 0x7c8
			 0x55a5ccb9, /// 0x7cc
			 0x1c751122, /// 0x7d0
			 0x5ccf17b9, /// 0x7d4
			 0x72596ff4, /// 0x7d8
			 0x56fecad4, /// 0x7dc
			 0x8ef59233, /// 0x7e0
			 0xb53cbcdd, /// 0x7e4
			 0xca8739a1, /// 0x7e8
			 0x34f8bdc6, /// 0x7ec
			 0x4b5a1e1a, /// 0x7f0
			 0x616cd465, /// 0x7f4
			 0xa2a3c88a, /// 0x7f8
			 0x278e5aec, /// 0x7fc
			 0x5ea8e127, /// 0x800
			 0x3572dbbf, /// 0x804
			 0x9a65222d, /// 0x808
			 0x6a597cf5, /// 0x80c
			 0xb08702c7, /// 0x810
			 0x17298a2a, /// 0x814
			 0x8fc4da76, /// 0x818
			 0x69cc152a, /// 0x81c
			 0x1f95e19c, /// 0x820
			 0x43173789, /// 0x824
			 0x4d4f09a2, /// 0x828
			 0xde0913d1, /// 0x82c
			 0x92f35754, /// 0x830
			 0x8a7d4fec, /// 0x834
			 0x00fba683, /// 0x838
			 0xc7e45e2f, /// 0x83c
			 0x5dea71f6, /// 0x840
			 0xaffc197a, /// 0x844
			 0x79b23299, /// 0x848
			 0x9bcc3d34, /// 0x84c
			 0x796a7ae7, /// 0x850
			 0xfa2b38fa, /// 0x854
			 0x2a90ca39, /// 0x858
			 0xb78c16a8, /// 0x85c
			 0xa69869c6, /// 0x860
			 0xc1ee3a79, /// 0x864
			 0xca0c0eef, /// 0x868
			 0xf34c49dc, /// 0x86c
			 0x0e676e30, /// 0x870
			 0x5e613ed7, /// 0x874
			 0xf37f5461, /// 0x878
			 0xa8338d78, /// 0x87c
			 0xbf35b845, /// 0x880
			 0xff8a717d, /// 0x884
			 0x0e2eea7a, /// 0x888
			 0xac183806, /// 0x88c
			 0x8f33929b, /// 0x890
			 0x1e0d84a8, /// 0x894
			 0xa08b9ea5, /// 0x898
			 0x15a8d91d, /// 0x89c
			 0xbd2e0ab9, /// 0x8a0
			 0x15538200, /// 0x8a4
			 0xfa6d3f25, /// 0x8a8
			 0xd5a66e95, /// 0x8ac
			 0x3ceb02a3, /// 0x8b0
			 0x11fe7aad, /// 0x8b4
			 0x99f56db1, /// 0x8b8
			 0xa3eaaf28, /// 0x8bc
			 0xf6041bb4, /// 0x8c0
			 0x32632f69, /// 0x8c4
			 0x9a8f2cba, /// 0x8c8
			 0xfe72e766, /// 0x8cc
			 0x793eff61, /// 0x8d0
			 0xb8737bd2, /// 0x8d4
			 0x73ee3463, /// 0x8d8
			 0x2418f820, /// 0x8dc
			 0x26279ba6, /// 0x8e0
			 0x373fc4e8, /// 0x8e4
			 0x88e8b39c, /// 0x8e8
			 0x133b7778, /// 0x8ec
			 0xd91a62fd, /// 0x8f0
			 0x0900b55a, /// 0x8f4
			 0xcbfec2f0, /// 0x8f8
			 0x3f0d19c2, /// 0x8fc
			 0x023c0d56, /// 0x900
			 0xe7c9a181, /// 0x904
			 0xc4f8b260, /// 0x908
			 0xdf79633c, /// 0x90c
			 0x4bcf48a1, /// 0x910
			 0xaa78e9d6, /// 0x914
			 0x0a211dc3, /// 0x918
			 0x5866bd81, /// 0x91c
			 0xbc92cc94, /// 0x920
			 0x00cd6d3b, /// 0x924
			 0x61d40408, /// 0x928
			 0x0e2cbdda, /// 0x92c
			 0x9c519ef6, /// 0x930
			 0xe53f921f, /// 0x934
			 0xff2b819e, /// 0x938
			 0x9ddb79e7, /// 0x93c
			 0x21e1da73, /// 0x940
			 0xbefd9135, /// 0x944
			 0xa8260de4, /// 0x948
			 0x9ebe6821, /// 0x94c
			 0x51ae9abd, /// 0x950
			 0xbc8c45aa, /// 0x954
			 0x637a7c43, /// 0x958
			 0x516a8fc6, /// 0x95c
			 0x15c5390a, /// 0x960
			 0x7d023e9f, /// 0x964
			 0xf4ec933a, /// 0x968
			 0xfeff1409, /// 0x96c
			 0x0180f635, /// 0x970
			 0xfb551078, /// 0x974
			 0xb265e96f, /// 0x978
			 0x4e31ab99, /// 0x97c
			 0xcd5d3096, /// 0x980
			 0x37b99ed0, /// 0x984
			 0xf021f72f, /// 0x988
			 0xa6c7c339, /// 0x98c
			 0xdb22c38e, /// 0x990
			 0x86039e79, /// 0x994
			 0x8198e2c2, /// 0x998
			 0xe60e4e6e, /// 0x99c
			 0x06076229, /// 0x9a0
			 0x507a13dc, /// 0x9a4
			 0x268572a7, /// 0x9a8
			 0x53d2c2ca, /// 0x9ac
			 0x867b7180, /// 0x9b0
			 0x12a2e1c0, /// 0x9b4
			 0x7a99bc17, /// 0x9b8
			 0x6591e7b4, /// 0x9bc
			 0xe2a6bd2e, /// 0x9c0
			 0xcfafd222, /// 0x9c4
			 0x25cc3223, /// 0x9c8
			 0xaf4a36a8, /// 0x9cc
			 0x1f0e9115, /// 0x9d0
			 0xcd977a62, /// 0x9d4
			 0x1afb2f93, /// 0x9d8
			 0x731786c4, /// 0x9dc
			 0x0dde9b1f, /// 0x9e0
			 0xdf430b9a, /// 0x9e4
			 0x439a36f4, /// 0x9e8
			 0xa86bf7a5, /// 0x9ec
			 0x5de79107, /// 0x9f0
			 0x88beff5a, /// 0x9f4
			 0xfcd015a7, /// 0x9f8
			 0x24a02d48, /// 0x9fc
			 0x60495cf1, /// 0xa00
			 0xf3754ef4, /// 0xa04
			 0x345ad041, /// 0xa08
			 0x713a4d0b, /// 0xa0c
			 0x40a409c4, /// 0xa10
			 0x0c4f2dd9, /// 0xa14
			 0x9d9e2429, /// 0xa18
			 0x2bae0789, /// 0xa1c
			 0x353a5ab3, /// 0xa20
			 0x098a5f46, /// 0xa24
			 0x6e882c38, /// 0xa28
			 0xaec4c343, /// 0xa2c
			 0x20c3a835, /// 0xa30
			 0x632e57a7, /// 0xa34
			 0xa6ae0d7e, /// 0xa38
			 0xc5a13daf, /// 0xa3c
			 0xa214fb69, /// 0xa40
			 0xf3292c73, /// 0xa44
			 0x75936d7b, /// 0xa48
			 0xf386e449, /// 0xa4c
			 0x11c2c8c1, /// 0xa50
			 0xd6339bdc, /// 0xa54
			 0x21de9a91, /// 0xa58
			 0x7e21e82b, /// 0xa5c
			 0x2c2eb260, /// 0xa60
			 0xf51c147f, /// 0xa64
			 0x57a12435, /// 0xa68
			 0xf6ac1172, /// 0xa6c
			 0xad73341c, /// 0xa70
			 0xfdfb56a2, /// 0xa74
			 0xaacb42b3, /// 0xa78
			 0x64606e64, /// 0xa7c
			 0x57a114c4, /// 0xa80
			 0x26509c99, /// 0xa84
			 0x3e5480d6, /// 0xa88
			 0x7d969766, /// 0xa8c
			 0x13780b6e, /// 0xa90
			 0x12667a19, /// 0xa94
			 0xb2502e5c, /// 0xa98
			 0x24083811, /// 0xa9c
			 0x4ccc5b07, /// 0xaa0
			 0x4b558864, /// 0xaa4
			 0x1fb30b0f, /// 0xaa8
			 0xb4ef660d, /// 0xaac
			 0x3d42dc82, /// 0xab0
			 0x97fed691, /// 0xab4
			 0xd427fd9f, /// 0xab8
			 0xc0ccaa7a, /// 0xabc
			 0x965b3e5b, /// 0xac0
			 0xc515acef, /// 0xac4
			 0xca9f3bcf, /// 0xac8
			 0xb6f8367c, /// 0xacc
			 0x2ade21f7, /// 0xad0
			 0x1fecb67e, /// 0xad4
			 0x244c70e6, /// 0xad8
			 0xcd14ff69, /// 0xadc
			 0x87628dfd, /// 0xae0
			 0xa445a25b, /// 0xae4
			 0x197438a2, /// 0xae8
			 0xa53af76e, /// 0xaec
			 0xe43c6b3c, /// 0xaf0
			 0xb6e9d930, /// 0xaf4
			 0xbfe59ebe, /// 0xaf8
			 0xd95c4ba9, /// 0xafc
			 0x7801cff2, /// 0xb00
			 0x8e5178fb, /// 0xb04
			 0x331176e3, /// 0xb08
			 0x2b9adbae, /// 0xb0c
			 0xd3bdac81, /// 0xb10
			 0x7cdbecad, /// 0xb14
			 0x838fa9c8, /// 0xb18
			 0x69e90601, /// 0xb1c
			 0x516a0979, /// 0xb20
			 0x528ec9ff, /// 0xb24
			 0xf879a314, /// 0xb28
			 0xbd322a39, /// 0xb2c
			 0x65c286d6, /// 0xb30
			 0xd4b50bd9, /// 0xb34
			 0x955648c1, /// 0xb38
			 0x1fc84b4a, /// 0xb3c
			 0xd5c22e12, /// 0xb40
			 0x1e3059b7, /// 0xb44
			 0xe4cb6fd1, /// 0xb48
			 0x987df38f, /// 0xb4c
			 0xd23a8457, /// 0xb50
			 0x460f7a87, /// 0xb54
			 0xb5d3ed9d, /// 0xb58
			 0x9c9824e7, /// 0xb5c
			 0x9abd957d, /// 0xb60
			 0xb776ef9b, /// 0xb64
			 0x5d0a4ca4, /// 0xb68
			 0x60f548b1, /// 0xb6c
			 0x9d884b9d, /// 0xb70
			 0xe2046309, /// 0xb74
			 0xd0f8fc9b, /// 0xb78
			 0xb22037c0, /// 0xb7c
			 0x26f4858b, /// 0xb80
			 0x281bc2ad, /// 0xb84
			 0x03cc997d, /// 0xb88
			 0xbdc2b3c8, /// 0xb8c
			 0x2518f10e, /// 0xb90
			 0x31055a62, /// 0xb94
			 0x0fa2e6ae, /// 0xb98
			 0x7661664b, /// 0xb9c
			 0xc726812a, /// 0xba0
			 0x41ec4020, /// 0xba4
			 0x0e1a48e8, /// 0xba8
			 0x054cb83b, /// 0xbac
			 0x2d296d83, /// 0xbb0
			 0x4f7c9efc, /// 0xbb4
			 0xe448ab95, /// 0xbb8
			 0x864f2161, /// 0xbbc
			 0x4770581d, /// 0xbc0
			 0xf855bc1c, /// 0xbc4
			 0x5017ee83, /// 0xbc8
			 0xdb83445e, /// 0xbcc
			 0xd39db80c, /// 0xbd0
			 0x78f14740, /// 0xbd4
			 0xb05ab2fb, /// 0xbd8
			 0xd26473da, /// 0xbdc
			 0xb3ca5e18, /// 0xbe0
			 0x63ed0e80, /// 0xbe4
			 0xef7a204b, /// 0xbe8
			 0xc1340f3f, /// 0xbec
			 0x7d4166b7, /// 0xbf0
			 0x9cdc6e6d, /// 0xbf4
			 0x7854c2cf, /// 0xbf8
			 0x5a69355e, /// 0xbfc
			 0x67024c73, /// 0xc00
			 0x5d0d083a, /// 0xc04
			 0x8b66af4a, /// 0xc08
			 0x7210a19c, /// 0xc0c
			 0xdedac4f5, /// 0xc10
			 0x039275d3, /// 0xc14
			 0xdf98a4c1, /// 0xc18
			 0xe544f6bb, /// 0xc1c
			 0x2593ec95, /// 0xc20
			 0x86e7b4c7, /// 0xc24
			 0x11b9182f, /// 0xc28
			 0xdd021087, /// 0xc2c
			 0x4ff8f4e6, /// 0xc30
			 0x60b4042a, /// 0xc34
			 0xb18ec702, /// 0xc38
			 0x709e24e9, /// 0xc3c
			 0x710412bd, /// 0xc40
			 0x57fbbcdb, /// 0xc44
			 0x1baa0198, /// 0xc48
			 0x0a43e2a6, /// 0xc4c
			 0x6dc5fb3a, /// 0xc50
			 0x5ff4102a, /// 0xc54
			 0x83a00157, /// 0xc58
			 0x2ce25409, /// 0xc5c
			 0x7681f567, /// 0xc60
			 0xfaff0600, /// 0xc64
			 0x936fc5ad, /// 0xc68
			 0x94fe3f05, /// 0xc6c
			 0x315116d1, /// 0xc70
			 0x06fe0b18, /// 0xc74
			 0x6ac19a69, /// 0xc78
			 0x4417c7ff, /// 0xc7c
			 0xd23b5a4a, /// 0xc80
			 0xa32969b4, /// 0xc84
			 0x7908b2d2, /// 0xc88
			 0xfc11a1b5, /// 0xc8c
			 0x8edd2b66, /// 0xc90
			 0xfe86fe6c, /// 0xc94
			 0x110b5814, /// 0xc98
			 0x2023a883, /// 0xc9c
			 0x9d9096df, /// 0xca0
			 0xa2b539c4, /// 0xca4
			 0x3876558b, /// 0xca8
			 0x6360a236, /// 0xcac
			 0x0281671c, /// 0xcb0
			 0x68131356, /// 0xcb4
			 0xb014170f, /// 0xcb8
			 0xc0553263, /// 0xcbc
			 0x278e96a6, /// 0xcc0
			 0x982bb181, /// 0xcc4
			 0xcd60bffd, /// 0xcc8
			 0x918b965a, /// 0xccc
			 0x37f50e75, /// 0xcd0
			 0x3e964c29, /// 0xcd4
			 0xebd34bb0, /// 0xcd8
			 0x8f2e278e, /// 0xcdc
			 0xf02ff17b, /// 0xce0
			 0x6fd50440, /// 0xce4
			 0x2252d08b, /// 0xce8
			 0x7d85e092, /// 0xcec
			 0xf713e66f, /// 0xcf0
			 0xcdc58316, /// 0xcf4
			 0x3f831a77, /// 0xcf8
			 0xc45a1bb8, /// 0xcfc
			 0x24550e88, /// 0xd00
			 0x121d5b88, /// 0xd04
			 0x3165df37, /// 0xd08
			 0x74e28e33, /// 0xd0c
			 0x8fb05d16, /// 0xd10
			 0x1f9a4fd3, /// 0xd14
			 0xe07e9f85, /// 0xd18
			 0x5a0af632, /// 0xd1c
			 0xf05b7d9d, /// 0xd20
			 0xea32b3e4, /// 0xd24
			 0x92b61385, /// 0xd28
			 0x2f896445, /// 0xd2c
			 0xf6e253d4, /// 0xd30
			 0xc689b8b4, /// 0xd34
			 0xaa54ebfb, /// 0xd38
			 0x7e7257f8, /// 0xd3c
			 0x3cbd1a19, /// 0xd40
			 0x506e5bfd, /// 0xd44
			 0x861d5be0, /// 0xd48
			 0xf7b4785a, /// 0xd4c
			 0x90d330a9, /// 0xd50
			 0xd8869ac1, /// 0xd54
			 0x736a979a, /// 0xd58
			 0x86ea1958, /// 0xd5c
			 0xcb25cb02, /// 0xd60
			 0xe92ad8a5, /// 0xd64
			 0x89bd85b6, /// 0xd68
			 0xf390b58f, /// 0xd6c
			 0x5e897876, /// 0xd70
			 0x04433b99, /// 0xd74
			 0xc0623a71, /// 0xd78
			 0xb4144fdb, /// 0xd7c
			 0x8e8bdf14, /// 0xd80
			 0x6cc6ca02, /// 0xd84
			 0xd34dfcc6, /// 0xd88
			 0x1ad9230e, /// 0xd8c
			 0x17b1e882, /// 0xd90
			 0xaaa030e9, /// 0xd94
			 0x5d2de3d3, /// 0xd98
			 0x29f3ca85, /// 0xd9c
			 0x85bd3156, /// 0xda0
			 0x5316d479, /// 0xda4
			 0xa780ed19, /// 0xda8
			 0x3ed52cad, /// 0xdac
			 0x577ff76f, /// 0xdb0
			 0x0516028b, /// 0xdb4
			 0x25bc03cb, /// 0xdb8
			 0x54140462, /// 0xdbc
			 0xc9e6c897, /// 0xdc0
			 0xdb1bbedc, /// 0xdc4
			 0x189f6556, /// 0xdc8
			 0x26f60c7e, /// 0xdcc
			 0x3688d848, /// 0xdd0
			 0xe6ddd029, /// 0xdd4
			 0x017f3b72, /// 0xdd8
			 0x5c3186fd, /// 0xddc
			 0x4c5cd486, /// 0xde0
			 0xc89df38d, /// 0xde4
			 0x86fda56d, /// 0xde8
			 0x1440916b, /// 0xdec
			 0x50b7568f, /// 0xdf0
			 0x7855dfba, /// 0xdf4
			 0xb095e48b, /// 0xdf8
			 0x5aeb43ce, /// 0xdfc
			 0x2ddd6377, /// 0xe00
			 0xd2a53caf, /// 0xe04
			 0x107037f8, /// 0xe08
			 0x2aeb26e6, /// 0xe0c
			 0xb376aa0e, /// 0xe10
			 0xa42342fd, /// 0xe14
			 0xe656202e, /// 0xe18
			 0xf0b7e829, /// 0xe1c
			 0x55d2bcfb, /// 0xe20
			 0x641afa63, /// 0xe24
			 0x43f567d8, /// 0xe28
			 0xa3c64a21, /// 0xe2c
			 0xc856f71b, /// 0xe30
			 0x71ed2446, /// 0xe34
			 0x90e2adb0, /// 0xe38
			 0xee0521f0, /// 0xe3c
			 0x6b3323ad, /// 0xe40
			 0xa218ee0f, /// 0xe44
			 0x86ff4175, /// 0xe48
			 0x53f660f2, /// 0xe4c
			 0xa1258839, /// 0xe50
			 0x16d9db3a, /// 0xe54
			 0x9a71c21b, /// 0xe58
			 0xc9b273e0, /// 0xe5c
			 0xc29d61b6, /// 0xe60
			 0x6cb593f9, /// 0xe64
			 0x82dea1b6, /// 0xe68
			 0xb696b4bb, /// 0xe6c
			 0x35d172c2, /// 0xe70
			 0x4cf0daff, /// 0xe74
			 0x68d291cc, /// 0xe78
			 0x175195ee, /// 0xe7c
			 0x8c15706a, /// 0xe80
			 0x02221777, /// 0xe84
			 0x433ce70b, /// 0xe88
			 0x829e8673, /// 0xe8c
			 0x39447bad, /// 0xe90
			 0x0a23a463, /// 0xe94
			 0x73c335f0, /// 0xe98
			 0xb0cfeed2, /// 0xe9c
			 0x969f65fa, /// 0xea0
			 0x5b58efdd, /// 0xea4
			 0x7ded5247, /// 0xea8
			 0x3379a1ba, /// 0xeac
			 0xfac171fd, /// 0xeb0
			 0x1233ea48, /// 0xeb4
			 0x00c8248b, /// 0xeb8
			 0x1a068f44, /// 0xebc
			 0x45b22c60, /// 0xec0
			 0x127181f8, /// 0xec4
			 0x35cf5a9b, /// 0xec8
			 0x0975d0f0, /// 0xecc
			 0xb6c5c3c0, /// 0xed0
			 0xaa04a310, /// 0xed4
			 0x318a2be6, /// 0xed8
			 0xeb10be9d, /// 0xedc
			 0xebbf291d, /// 0xee0
			 0xcf2c4d3e, /// 0xee4
			 0x833d3886, /// 0xee8
			 0x3f87af19, /// 0xeec
			 0xffef7b8e, /// 0xef0
			 0x28063d3f, /// 0xef4
			 0xc151ad7c, /// 0xef8
			 0x37683888, /// 0xefc
			 0xf0241609, /// 0xf00
			 0x2de26d3d, /// 0xf04
			 0x91cd395c, /// 0xf08
			 0xe3caf3bb, /// 0xf0c
			 0x0f7c6d7d, /// 0xf10
			 0x22dd5357, /// 0xf14
			 0xa3d1c049, /// 0xf18
			 0xa1068e4a, /// 0xf1c
			 0xd3a9ed75, /// 0xf20
			 0x1585c7b8, /// 0xf24
			 0x1af8c3f4, /// 0xf28
			 0x7a4adc8b, /// 0xf2c
			 0x04444b3e, /// 0xf30
			 0x4d73f73f, /// 0xf34
			 0xb3fd41c2, /// 0xf38
			 0x30245384, /// 0xf3c
			 0x645d364a, /// 0xf40
			 0x7d6b79df, /// 0xf44
			 0xf782bcfd, /// 0xf48
			 0x76af5061, /// 0xf4c
			 0x3751e045, /// 0xf50
			 0x789befde, /// 0xf54
			 0x60faf9da, /// 0xf58
			 0x40ea80ff, /// 0xf5c
			 0xfc3cd8e9, /// 0xf60
			 0x89312111, /// 0xf64
			 0x8ed7af04, /// 0xf68
			 0xdc124337, /// 0xf6c
			 0x7f01ec68, /// 0xf70
			 0x3dd6c496, /// 0xf74
			 0x3d354286, /// 0xf78
			 0x3d08c013, /// 0xf7c
			 0x1aefecf9, /// 0xf80
			 0xb392fd13, /// 0xf84
			 0xf0ab1689, /// 0xf88
			 0x9652744b, /// 0xf8c
			 0x5dd2f952, /// 0xf90
			 0xc896148c, /// 0xf94
			 0xaeff9cbd, /// 0xf98
			 0xfb9fe60a, /// 0xf9c
			 0x557c4ca5, /// 0xfa0
			 0xdd77750e, /// 0xfa4
			 0x90d5506e, /// 0xfa8
			 0xbe908ce0, /// 0xfac
			 0x120730e9, /// 0xfb0
			 0xe80a86bb, /// 0xfb4
			 0x7e7fd4e6, /// 0xfb8
			 0x22569c6a, /// 0xfbc
			 0x2abc3535, /// 0xfc0
			 0xcb948506, /// 0xfc4
			 0xa9a7d95c, /// 0xfc8
			 0x095792c2, /// 0xfcc
			 0x3a81ddc6, /// 0xfd0
			 0xcde5d073, /// 0xfd4
			 0x88752314, /// 0xfd8
			 0xabdfb164, /// 0xfdc
			 0x6ddbacaf, /// 0xfe0
			 0x2bd6dd53, /// 0xfe4
			 0x164d08fd, /// 0xfe8
			 0x17217f8d, /// 0xfec
			 0x2a7ae038, /// 0xff0
			 0x0c05232b, /// 0xff4
			 0x21a61723, /// 0xff8
			 0xd361a420 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00004
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0000c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00010
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00014
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00018
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00020
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00024
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00028
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00030
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00034
			 0x0c400000,                                                  // Leading 1s:                                 /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0003c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00044
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0xcb000000,                                                  // -8388608.0                                  /// 0004c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00050
			 0xbf028f5c,                                                  // -0.51                                       /// 00054
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00058
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0005c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00060
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00064
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00068
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0006c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00070
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00074
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0007c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00080
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00084
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00090
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00094
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00098
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a0
			 0x33333333,                                                  // 4 random values                             /// 000a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00108
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0010c
			 0x7fc00001,                                                  // signaling NaN                               /// 00110
			 0xcb000000,                                                  // -8388608.0                                  /// 00114
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0011c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00128
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0012c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00134
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00144
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0014c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00150
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00154
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00158
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0015c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0016c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00170
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00178
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0017c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00180
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00184
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00188
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0018c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001a4
			 0x00000000,                                                  // zero                                        /// 001a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00204
			 0x00000000,                                                  // zero                                        /// 00208
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0020c
			 0x7f800000,                                                  // inf                                         /// 00210
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00218
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00220
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00228
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0022c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00230
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00234
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0023c
			 0xcb000000,                                                  // -8388608.0                                  /// 00240
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0024c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0c600000,                                                  // Leading 1s:                                 /// 00254
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00258
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00260
			 0x55555555,                                                  // 4 random values                             /// 00264
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00268
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0026c
			 0x55555555,                                                  // 4 random values                             /// 00270
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00274
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00278
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00280
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00288
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0028c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00290
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00294
			 0x80011111,                                                  // -9.7958E-41                                 /// 00298
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0029c
			 0x80000000,                                                  // -zero                                       /// 002a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002b4
			 0xffc00001,                                                  // -signaling NaN                              /// 002b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00000000,                                                  // zero                                        /// 002d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x4b000000,                                                  // 8388608.0                                   /// 002ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00300
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00304
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00310
			 0x7f800000,                                                  // inf                                         /// 00314
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e000007,                                                  // Trailing 1s:                                /// 0031c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00320
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00328
			 0xbf028f5c,                                                  // -0.51                                       /// 0032c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00330
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00338
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x55555555,                                                  // 4 random values                             /// 00348
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0034c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00350
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00358
			 0x0c400000,                                                  // Leading 1s:                                 /// 0035c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0036c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00370
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00374
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0038c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00390
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00394
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00398
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0039c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f0
			 0x3f028f5c,                                                  // 0.51                                        /// 003f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00400
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00408
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0x0e000007,                                                  // Trailing 1s:                                /// 00418
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00428
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0042c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00430
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00434
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0043c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00440
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00448
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00460
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0046c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00470
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0047c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00480
			 0x0c400000,                                                  // Leading 1s:                                 /// 00484
			 0xff800000,                                                  // -inf                                        /// 00488
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0048c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00490
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00494
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0xcb000000,                                                  // -8388608.0                                  /// 0049c
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x7fc00001,                                                  // signaling NaN                               /// 004b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ec
			 0xbf028f5c,                                                  // -0.51                                       /// 004f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80000000,                                                  // -zero                                       /// 004fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00504
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00508
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00510
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00514
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00518
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0051c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00520
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00524
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0052c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00530
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00534
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00548
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00550
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0055c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00560
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00564
			 0xffc00001,                                                  // -signaling NaN                              /// 00568
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0056c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00570
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00574
			 0x0c400000,                                                  // Leading 1s:                                 /// 00578
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00580
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00590
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00594
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00598
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0059c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80000000,                                                  // -zero                                       /// 005c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x4b000000,                                                  // 8388608.0                                   /// 005d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00600
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00608
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0060c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00614
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00618
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0061c
			 0x7fc00001,                                                  // signaling NaN                               /// 00620
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00624
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00628
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00630
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0063c
			 0x33333333,                                                  // 4 random values                             /// 00640
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00644
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0064c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00650
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00654
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00658
			 0x4b000000,                                                  // 8388608.0                                   /// 0065c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00660
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00668
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0066c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00670
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00674
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x00000000,                                                  // zero                                        /// 00684
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00688
			 0xff800000,                                                  // -inf                                        /// 0068c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00690
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00698
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0069c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x55555555,                                                  // 4 random values                             /// 006b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d4
			 0xcb000000,                                                  // -8388608.0                                  /// 006d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00700
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00708
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0070c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00710
			 0x0e000003,                                                  // Trailing 1s:                                /// 00714
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00718
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00724
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00728
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0072c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00730
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00734
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0073c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00740
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00744
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0074c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00760
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00768
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0076c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00788
			 0x0e000001,                                                  // Trailing 1s:                                /// 0078c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00790
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00794
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00804
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0080c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00814
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0081c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00820
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00824
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00828
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0082c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00834
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00838
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0083c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00840
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00844
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0xffc00001,                                                  // -signaling NaN                              /// 0084c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0085c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00860
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00864
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0086c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00870
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00874
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0087c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0088c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00890
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00898
			 0xffc00001,                                                  // -signaling NaN                              /// 0089c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x3f028f5c,                                                  // 0.51                                        /// 008b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008cc
			 0x7f800000,                                                  // inf                                         /// 008d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00900
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00904
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00908
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0090c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00910
			 0x0c700000,                                                  // Leading 1s:                                 /// 00914
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0091c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00924
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00928
			 0x0c700000,                                                  // Leading 1s:                                 /// 0092c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00930
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00934
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00938
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0093c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00940
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00950
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00954
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00958
			 0x80011111,                                                  // -9.7958E-41                                 /// 0095c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00960
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00968
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0096c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00970
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x55555555,                                                  // 4 random values                             /// 0097c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00980
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00988
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0098c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00994
			 0x55555555,                                                  // 4 random values                             /// 00998
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0099c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c4
			 0xbf028f5c,                                                  // -0.51                                       /// 009c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a6c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a94
			 0x00000000,                                                  // zero                                        /// 00a98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aa4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aa8
			 0x55555555,                                                  // 4 random values                             /// 00aac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00abc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00acc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ae0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ae8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00af0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00afc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b18
			 0xcb000000,                                                  // -8388608.0                                  /// 00b1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b40
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000000,                                                  // zero                                        /// 00b5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000000,                                                  // zero                                        /// 00b90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00bd0
			 0x7f800000,                                                  // inf                                         /// 00bd4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00be8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bf0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bfc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0xff800000,                                                  // -inf                                        /// 00c08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c70
			 0x55555555,                                                  // 4 random values                             /// 00c74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x55555555,                                                  // 4 random values                             /// 00c88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ca8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cb8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cc0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ccc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cfc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xffc00001,                                                  // -signaling NaN                              /// 00d54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x4b000000,                                                  // 8388608.0                                   /// 00d94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00da0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00da4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dac
			 0x3f028f5c,                                                  // 0.51                                        /// 00db0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00db8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e20
			 0x33333333,                                                  // 4 random values                             /// 00e24
			 0x7f800000,                                                  // inf                                         /// 00e28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ec4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ec8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ed4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00edc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ee8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00efc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f50
			 0x3f028f5c,                                                  // 0.51                                        /// 00f54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f5c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80000000,                                                  // -zero                                       /// 00f68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xff800000,                                                  // -inf                                        /// 00fd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fdc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fe0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fe4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe8
			 0x7f800000,                                                  // inf                                         /// 00fec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ff4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00004000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x2a82e290, /// 0x0
			 0x65ff07dc, /// 0x4
			 0x8abbf44c, /// 0x8
			 0xb936a0f9, /// 0xc
			 0xa075745f, /// 0x10
			 0x88dd1034, /// 0x14
			 0x2763bf27, /// 0x18
			 0xb713bbb0, /// 0x1c
			 0x5de4b3d7, /// 0x20
			 0x69a5fb9e, /// 0x24
			 0xb0eefa57, /// 0x28
			 0x5febb0d0, /// 0x2c
			 0xc5310d92, /// 0x30
			 0x82dc620a, /// 0x34
			 0x2a079c97, /// 0x38
			 0x986ede15, /// 0x3c
			 0xf3be3377, /// 0x40
			 0x7e7c5e50, /// 0x44
			 0x76a88584, /// 0x48
			 0x866f9506, /// 0x4c
			 0x7faf16ce, /// 0x50
			 0x4e3ea9a7, /// 0x54
			 0xed3b8629, /// 0x58
			 0x95d6b0e3, /// 0x5c
			 0x9ac5c1e4, /// 0x60
			 0x9ee59b9f, /// 0x64
			 0xeb8dc137, /// 0x68
			 0x1f17409b, /// 0x6c
			 0xcd79dff6, /// 0x70
			 0x420fc771, /// 0x74
			 0x862fa18d, /// 0x78
			 0x0e1e235b, /// 0x7c
			 0xbd3c01e7, /// 0x80
			 0xf209f276, /// 0x84
			 0x5be21f66, /// 0x88
			 0x71872f23, /// 0x8c
			 0x007213d8, /// 0x90
			 0x367d9874, /// 0x94
			 0x3c7e768d, /// 0x98
			 0x1a66f164, /// 0x9c
			 0x29508ed3, /// 0xa0
			 0xde887e2d, /// 0xa4
			 0x19572beb, /// 0xa8
			 0x8ffba211, /// 0xac
			 0x7a0f43a3, /// 0xb0
			 0xcc449236, /// 0xb4
			 0xe762cc18, /// 0xb8
			 0x96b14e3a, /// 0xbc
			 0x1ea8a301, /// 0xc0
			 0x8162be81, /// 0xc4
			 0x32623fed, /// 0xc8
			 0xd68b1b4f, /// 0xcc
			 0x38dabfdd, /// 0xd0
			 0x0b7cbe27, /// 0xd4
			 0x9ed5a2ad, /// 0xd8
			 0xe40186d2, /// 0xdc
			 0xb3866d6c, /// 0xe0
			 0xdca486f7, /// 0xe4
			 0x91755b4b, /// 0xe8
			 0xa645b29e, /// 0xec
			 0x1de0e064, /// 0xf0
			 0xc4c8c071, /// 0xf4
			 0xac317d12, /// 0xf8
			 0x5abfd9a9, /// 0xfc
			 0xf238ebbc, /// 0x100
			 0xb1d25263, /// 0x104
			 0x2f11148f, /// 0x108
			 0x2d4a16a9, /// 0x10c
			 0xb92a084c, /// 0x110
			 0x6df43de4, /// 0x114
			 0xa5c14c42, /// 0x118
			 0x9728a026, /// 0x11c
			 0x22c49c1a, /// 0x120
			 0x2e2c1899, /// 0x124
			 0x8e755480, /// 0x128
			 0x9bb4c754, /// 0x12c
			 0x505171d1, /// 0x130
			 0x7cca5197, /// 0x134
			 0x0adb2b8e, /// 0x138
			 0x07b86ccd, /// 0x13c
			 0xdbf6efe8, /// 0x140
			 0x4f3d2763, /// 0x144
			 0xdf50265b, /// 0x148
			 0xe8998a3f, /// 0x14c
			 0x80329a08, /// 0x150
			 0x22854125, /// 0x154
			 0xd0ccae75, /// 0x158
			 0xc900669c, /// 0x15c
			 0x8b1b8c7a, /// 0x160
			 0xf0a937a9, /// 0x164
			 0xe4dca256, /// 0x168
			 0xad4919a8, /// 0x16c
			 0xe98b415e, /// 0x170
			 0x82e3aabc, /// 0x174
			 0x364ce48b, /// 0x178
			 0x21346ecc, /// 0x17c
			 0xd6b7d4a6, /// 0x180
			 0xc8de3685, /// 0x184
			 0x6c2e5701, /// 0x188
			 0x0b714f1f, /// 0x18c
			 0xfdcdb0df, /// 0x190
			 0x55202ffe, /// 0x194
			 0x700254b2, /// 0x198
			 0x8391801f, /// 0x19c
			 0xad342489, /// 0x1a0
			 0x32352651, /// 0x1a4
			 0x1c404632, /// 0x1a8
			 0xb5fef44f, /// 0x1ac
			 0xda2b77a1, /// 0x1b0
			 0xde73c873, /// 0x1b4
			 0x7bc0bd69, /// 0x1b8
			 0xc5b001d1, /// 0x1bc
			 0x77f63fe9, /// 0x1c0
			 0x49854aff, /// 0x1c4
			 0x92926f11, /// 0x1c8
			 0x6e7b9863, /// 0x1cc
			 0x14a251d2, /// 0x1d0
			 0x6b6b348b, /// 0x1d4
			 0xaf56bc42, /// 0x1d8
			 0xd747ab3c, /// 0x1dc
			 0x86ff119e, /// 0x1e0
			 0x4189489b, /// 0x1e4
			 0xa1d86583, /// 0x1e8
			 0x8a971044, /// 0x1ec
			 0x28b3e9a7, /// 0x1f0
			 0x4958d5fe, /// 0x1f4
			 0x5c979f77, /// 0x1f8
			 0xd03fbe5a, /// 0x1fc
			 0xe3959420, /// 0x200
			 0x63cee521, /// 0x204
			 0x0246a55f, /// 0x208
			 0xbed2f361, /// 0x20c
			 0x3eefb53f, /// 0x210
			 0x85c806c2, /// 0x214
			 0xdc136f6a, /// 0x218
			 0x003d5086, /// 0x21c
			 0x3adb9e06, /// 0x220
			 0xa7211b4d, /// 0x224
			 0xb5da58b3, /// 0x228
			 0x13a2e01f, /// 0x22c
			 0x0bf4c934, /// 0x230
			 0xc8ac7b11, /// 0x234
			 0x11b54b44, /// 0x238
			 0x428ec920, /// 0x23c
			 0x962a1089, /// 0x240
			 0xb8a0ef48, /// 0x244
			 0x5b7e41dc, /// 0x248
			 0x84fd94c9, /// 0x24c
			 0x30118db6, /// 0x250
			 0x6c4885f7, /// 0x254
			 0xbe4e9ad1, /// 0x258
			 0xf82fb36a, /// 0x25c
			 0x708bb724, /// 0x260
			 0x4e9bfb78, /// 0x264
			 0xbd8bf9b9, /// 0x268
			 0x45ae1ece, /// 0x26c
			 0x587a02be, /// 0x270
			 0x218d8fcd, /// 0x274
			 0x4adf9690, /// 0x278
			 0x36154b13, /// 0x27c
			 0x8204c38f, /// 0x280
			 0xb8f872ae, /// 0x284
			 0x94d4707e, /// 0x288
			 0xd0c8bac7, /// 0x28c
			 0xf85a9b2a, /// 0x290
			 0xa95c6869, /// 0x294
			 0xa1df0e12, /// 0x298
			 0xd150aaa2, /// 0x29c
			 0xe5b37441, /// 0x2a0
			 0x1dfaa248, /// 0x2a4
			 0x64fbc90f, /// 0x2a8
			 0xa7f9d0ba, /// 0x2ac
			 0x5cdb2eca, /// 0x2b0
			 0x9f5ed896, /// 0x2b4
			 0x5c7af756, /// 0x2b8
			 0x32739fc5, /// 0x2bc
			 0xba68f675, /// 0x2c0
			 0x184b04cb, /// 0x2c4
			 0xf6e46650, /// 0x2c8
			 0xf0eaf5b2, /// 0x2cc
			 0x68c3403c, /// 0x2d0
			 0x0cc11b26, /// 0x2d4
			 0xe99e11a8, /// 0x2d8
			 0x96dd1ef4, /// 0x2dc
			 0x3078b9d9, /// 0x2e0
			 0x298d5de5, /// 0x2e4
			 0x1290ec17, /// 0x2e8
			 0x7fc1c98b, /// 0x2ec
			 0xb4f26021, /// 0x2f0
			 0x6835367c, /// 0x2f4
			 0x15417051, /// 0x2f8
			 0x09187035, /// 0x2fc
			 0x1064ee2d, /// 0x300
			 0x93d72d5b, /// 0x304
			 0x37381031, /// 0x308
			 0x0de717ea, /// 0x30c
			 0x77c7f6d5, /// 0x310
			 0x085a2ca4, /// 0x314
			 0x2bdf66ff, /// 0x318
			 0x55d7f219, /// 0x31c
			 0x6f4aa9f0, /// 0x320
			 0x41490d42, /// 0x324
			 0x7e6d11a3, /// 0x328
			 0x8696ce6f, /// 0x32c
			 0x94989bda, /// 0x330
			 0xbfa64c5f, /// 0x334
			 0x64161b0b, /// 0x338
			 0x4e91f7a0, /// 0x33c
			 0x08ff5457, /// 0x340
			 0x1e7f6bb9, /// 0x344
			 0xc0400b47, /// 0x348
			 0xc7ee9495, /// 0x34c
			 0x126b6dc6, /// 0x350
			 0x6ce9cbac, /// 0x354
			 0x3876197e, /// 0x358
			 0xcdb966d1, /// 0x35c
			 0xbed57621, /// 0x360
			 0xf854444e, /// 0x364
			 0xfd7f84d8, /// 0x368
			 0xccfd97f6, /// 0x36c
			 0x9e9fe95c, /// 0x370
			 0xf39d463e, /// 0x374
			 0xa951ef34, /// 0x378
			 0x9757e32a, /// 0x37c
			 0xd7c76723, /// 0x380
			 0x162588e8, /// 0x384
			 0x2094330d, /// 0x388
			 0xe0bc957c, /// 0x38c
			 0xa270d0db, /// 0x390
			 0x73a3a120, /// 0x394
			 0x392bc031, /// 0x398
			 0xa6103236, /// 0x39c
			 0x4c7ccaf9, /// 0x3a0
			 0xdca4f3be, /// 0x3a4
			 0xb9203859, /// 0x3a8
			 0xb30659d3, /// 0x3ac
			 0x329cd260, /// 0x3b0
			 0x53c78305, /// 0x3b4
			 0xb0399a24, /// 0x3b8
			 0x485f6874, /// 0x3bc
			 0xf73f5721, /// 0x3c0
			 0x300bbdf4, /// 0x3c4
			 0xbbaafc6a, /// 0x3c8
			 0x7e8cc6a9, /// 0x3cc
			 0x918fbc7b, /// 0x3d0
			 0xfd8b77b2, /// 0x3d4
			 0x90112efd, /// 0x3d8
			 0xd0420381, /// 0x3dc
			 0xf8af3301, /// 0x3e0
			 0x963b9e9c, /// 0x3e4
			 0xf5dad82e, /// 0x3e8
			 0xfd1afada, /// 0x3ec
			 0xacfd4843, /// 0x3f0
			 0x78a6ec50, /// 0x3f4
			 0xbd7a9842, /// 0x3f8
			 0xe366430c, /// 0x3fc
			 0x121272d6, /// 0x400
			 0x372ce83f, /// 0x404
			 0xb8cc101b, /// 0x408
			 0x9b3788f0, /// 0x40c
			 0xdc75d325, /// 0x410
			 0x305b09c0, /// 0x414
			 0xec908600, /// 0x418
			 0x3881f195, /// 0x41c
			 0x1c0c41f6, /// 0x420
			 0x50443f1d, /// 0x424
			 0xc912d415, /// 0x428
			 0x52562fed, /// 0x42c
			 0xa9c5bcc3, /// 0x430
			 0xb1d17293, /// 0x434
			 0x51c12639, /// 0x438
			 0xa2da4987, /// 0x43c
			 0x53d0c7fe, /// 0x440
			 0x209524ee, /// 0x444
			 0x092ec22b, /// 0x448
			 0xc084870e, /// 0x44c
			 0x680b65c5, /// 0x450
			 0xb29e9793, /// 0x454
			 0xaff0d7bd, /// 0x458
			 0x81d9a63f, /// 0x45c
			 0xe4f5dd3c, /// 0x460
			 0x6ea6fbdf, /// 0x464
			 0x07f2718b, /// 0x468
			 0x3ce32b77, /// 0x46c
			 0x18138679, /// 0x470
			 0xe45438e3, /// 0x474
			 0xa472cca8, /// 0x478
			 0xddc0a59e, /// 0x47c
			 0x6b9b51ec, /// 0x480
			 0xd9cb9e08, /// 0x484
			 0xb521ca93, /// 0x488
			 0x1018ec59, /// 0x48c
			 0x361a50ef, /// 0x490
			 0x4646b67f, /// 0x494
			 0x0ed2ba6c, /// 0x498
			 0x41f84e1a, /// 0x49c
			 0x82d9c579, /// 0x4a0
			 0x32347e0d, /// 0x4a4
			 0x1f2d06ae, /// 0x4a8
			 0x80612ca0, /// 0x4ac
			 0x0ce96d5e, /// 0x4b0
			 0xce2d2903, /// 0x4b4
			 0x9700e77f, /// 0x4b8
			 0xb1acfa0c, /// 0x4bc
			 0xbadbdac2, /// 0x4c0
			 0x7ff40350, /// 0x4c4
			 0x7db5e08b, /// 0x4c8
			 0x19d9afde, /// 0x4cc
			 0x678b9edf, /// 0x4d0
			 0x213c429d, /// 0x4d4
			 0xf7bdcd63, /// 0x4d8
			 0xc96b6955, /// 0x4dc
			 0x09fe3c61, /// 0x4e0
			 0x02e10dcd, /// 0x4e4
			 0xb990a041, /// 0x4e8
			 0xe89b7ee6, /// 0x4ec
			 0xf6a87752, /// 0x4f0
			 0x016b6f90, /// 0x4f4
			 0xbb2eea90, /// 0x4f8
			 0xcd994b56, /// 0x4fc
			 0x9576e727, /// 0x500
			 0xa59bedf9, /// 0x504
			 0x7d369e5d, /// 0x508
			 0x74889a1d, /// 0x50c
			 0x76b8612c, /// 0x510
			 0x8dfd72c0, /// 0x514
			 0xe4501214, /// 0x518
			 0xbfc08495, /// 0x51c
			 0xeb1713ba, /// 0x520
			 0xc7f09674, /// 0x524
			 0x68dc8992, /// 0x528
			 0x54ff048d, /// 0x52c
			 0xf3a799b1, /// 0x530
			 0xa30b72e5, /// 0x534
			 0x5822d26a, /// 0x538
			 0x3d512ed6, /// 0x53c
			 0xdf892dcd, /// 0x540
			 0xdae30a93, /// 0x544
			 0xb9478856, /// 0x548
			 0x79d9ae34, /// 0x54c
			 0x7d6707e6, /// 0x550
			 0x785959b9, /// 0x554
			 0xf38b4c7f, /// 0x558
			 0x087e0c4e, /// 0x55c
			 0xb080e90b, /// 0x560
			 0x68333869, /// 0x564
			 0x2309f4f1, /// 0x568
			 0xfddf6b63, /// 0x56c
			 0x11cda1b7, /// 0x570
			 0x4f3a1d41, /// 0x574
			 0x213f7fda, /// 0x578
			 0x9d0b9f59, /// 0x57c
			 0x2c3bd447, /// 0x580
			 0x0444728e, /// 0x584
			 0x13ab8e30, /// 0x588
			 0x51020182, /// 0x58c
			 0x6b811d2b, /// 0x590
			 0x7c5363c9, /// 0x594
			 0xc4d030c4, /// 0x598
			 0xab17a98c, /// 0x59c
			 0x909f42e6, /// 0x5a0
			 0x89d92cda, /// 0x5a4
			 0xdbe24096, /// 0x5a8
			 0x01b963fa, /// 0x5ac
			 0xfce43ef6, /// 0x5b0
			 0xe3602406, /// 0x5b4
			 0xb779e4b6, /// 0x5b8
			 0x021948c7, /// 0x5bc
			 0x8fcdef6c, /// 0x5c0
			 0xbebd020b, /// 0x5c4
			 0x0b3aa3d9, /// 0x5c8
			 0x40186f85, /// 0x5cc
			 0xbbed01c3, /// 0x5d0
			 0xca4e3d53, /// 0x5d4
			 0x4dca1c30, /// 0x5d8
			 0xfca0689b, /// 0x5dc
			 0x130c7983, /// 0x5e0
			 0x209565d7, /// 0x5e4
			 0xd43038e0, /// 0x5e8
			 0xb95c8d5d, /// 0x5ec
			 0x2c024a28, /// 0x5f0
			 0x07ad809c, /// 0x5f4
			 0xc06fe7fa, /// 0x5f8
			 0xcd8b7ba2, /// 0x5fc
			 0x7610e268, /// 0x600
			 0x369fdbbf, /// 0x604
			 0x7caadd3b, /// 0x608
			 0x6d57803f, /// 0x60c
			 0xc7c17cbf, /// 0x610
			 0x9904f493, /// 0x614
			 0x85ca2e83, /// 0x618
			 0xe035f9b0, /// 0x61c
			 0xa174c66e, /// 0x620
			 0x9650240b, /// 0x624
			 0xe91c270a, /// 0x628
			 0x6012c14f, /// 0x62c
			 0x3e7046b9, /// 0x630
			 0x17f0ca85, /// 0x634
			 0x5a326e9d, /// 0x638
			 0xf85f216c, /// 0x63c
			 0x708826af, /// 0x640
			 0xbd8e9a07, /// 0x644
			 0x46e2cfc4, /// 0x648
			 0x0bad7d4a, /// 0x64c
			 0x78f8a980, /// 0x650
			 0x818cfb16, /// 0x654
			 0xa80dc2bb, /// 0x658
			 0xc754f34a, /// 0x65c
			 0x11a4bd5a, /// 0x660
			 0x9003442b, /// 0x664
			 0xd3788084, /// 0x668
			 0x917d0a71, /// 0x66c
			 0x50addc72, /// 0x670
			 0x6ba68149, /// 0x674
			 0x4e0fad63, /// 0x678
			 0x9046ec1e, /// 0x67c
			 0x7db3de99, /// 0x680
			 0x4588ca37, /// 0x684
			 0x0050b44b, /// 0x688
			 0x5e81bab3, /// 0x68c
			 0x855ac6e2, /// 0x690
			 0x48ff7d87, /// 0x694
			 0x7ba1e26d, /// 0x698
			 0xcfc4f559, /// 0x69c
			 0x38b970c6, /// 0x6a0
			 0xb4bca3de, /// 0x6a4
			 0x33076296, /// 0x6a8
			 0xeb2c0180, /// 0x6ac
			 0xf1f90850, /// 0x6b0
			 0x501ba78e, /// 0x6b4
			 0x18d83d04, /// 0x6b8
			 0x19c13fe9, /// 0x6bc
			 0xa05afd24, /// 0x6c0
			 0xa5166980, /// 0x6c4
			 0xd1c19683, /// 0x6c8
			 0xac4d794f, /// 0x6cc
			 0x2512b585, /// 0x6d0
			 0x79eb09ab, /// 0x6d4
			 0x40b2cd50, /// 0x6d8
			 0x0ef774e7, /// 0x6dc
			 0x8ff6cfe4, /// 0x6e0
			 0x45714624, /// 0x6e4
			 0x045f6a43, /// 0x6e8
			 0x5683f985, /// 0x6ec
			 0x29b7f146, /// 0x6f0
			 0x9c2b0305, /// 0x6f4
			 0xcdbcd3dc, /// 0x6f8
			 0xa9d9e4d5, /// 0x6fc
			 0x7940fed0, /// 0x700
			 0xe97956d3, /// 0x704
			 0x83d2054d, /// 0x708
			 0xe8a4d65c, /// 0x70c
			 0xe449ef47, /// 0x710
			 0x4fdb4426, /// 0x714
			 0x7190e6b0, /// 0x718
			 0xc2e53562, /// 0x71c
			 0x43c5f6d1, /// 0x720
			 0xc3ae3aa6, /// 0x724
			 0x8e8d8fc3, /// 0x728
			 0x5a9e03b5, /// 0x72c
			 0xc5469c78, /// 0x730
			 0x3f3e5ce7, /// 0x734
			 0xd5a52b65, /// 0x738
			 0x2ee53721, /// 0x73c
			 0x5d8cfa27, /// 0x740
			 0x6f95ee4f, /// 0x744
			 0xd85100f1, /// 0x748
			 0x4cb62227, /// 0x74c
			 0x91301e41, /// 0x750
			 0xe81cb64a, /// 0x754
			 0x5e9f86b0, /// 0x758
			 0x63e546f6, /// 0x75c
			 0xe4bba59d, /// 0x760
			 0x8e0e985b, /// 0x764
			 0x876c018c, /// 0x768
			 0xf018cd42, /// 0x76c
			 0x0f525a78, /// 0x770
			 0x7689c838, /// 0x774
			 0x63a328a1, /// 0x778
			 0xdb5b3147, /// 0x77c
			 0x1a2eaba6, /// 0x780
			 0x086c9403, /// 0x784
			 0xca2460a2, /// 0x788
			 0x525e9cf7, /// 0x78c
			 0x4f25a897, /// 0x790
			 0x83cdf198, /// 0x794
			 0x3889553b, /// 0x798
			 0x028c2631, /// 0x79c
			 0xd7f8e1fb, /// 0x7a0
			 0x6c8c8132, /// 0x7a4
			 0x9c9580b7, /// 0x7a8
			 0xfe643338, /// 0x7ac
			 0xdf14230b, /// 0x7b0
			 0xe962cc3c, /// 0x7b4
			 0x99fd3a2c, /// 0x7b8
			 0x0940ec44, /// 0x7bc
			 0x6055fc15, /// 0x7c0
			 0xab45af55, /// 0x7c4
			 0xa42e605d, /// 0x7c8
			 0x0891f74f, /// 0x7cc
			 0xbae06600, /// 0x7d0
			 0x67106db5, /// 0x7d4
			 0xc6443341, /// 0x7d8
			 0x4e61b1d2, /// 0x7dc
			 0xce6cf80f, /// 0x7e0
			 0x7472ed66, /// 0x7e4
			 0xd5f8e12f, /// 0x7e8
			 0x6b44cbf6, /// 0x7ec
			 0xc32d04f0, /// 0x7f0
			 0xa069fb90, /// 0x7f4
			 0xd6a2959f, /// 0x7f8
			 0x4b96e4a4, /// 0x7fc
			 0x744e5361, /// 0x800
			 0xfe0f2c9a, /// 0x804
			 0x4d33c918, /// 0x808
			 0xa2577d60, /// 0x80c
			 0x5fc14604, /// 0x810
			 0x98bcb2d6, /// 0x814
			 0x4d4e6a4e, /// 0x818
			 0x92eac4da, /// 0x81c
			 0x0e81d43d, /// 0x820
			 0xc828ec50, /// 0x824
			 0xa44945db, /// 0x828
			 0xcbf168b1, /// 0x82c
			 0xf2163ea1, /// 0x830
			 0x235d1675, /// 0x834
			 0x530fb017, /// 0x838
			 0x309a645a, /// 0x83c
			 0x03e2e0cc, /// 0x840
			 0xb85efc83, /// 0x844
			 0x5cd01d17, /// 0x848
			 0x1cc0cdf9, /// 0x84c
			 0xea4014c8, /// 0x850
			 0x58b42ec4, /// 0x854
			 0x570ee23a, /// 0x858
			 0xfb5333ac, /// 0x85c
			 0xe38328ef, /// 0x860
			 0xc48ffc03, /// 0x864
			 0xc03fb970, /// 0x868
			 0x0c3c270f, /// 0x86c
			 0x76e8edb6, /// 0x870
			 0xef2b7b53, /// 0x874
			 0xf919b679, /// 0x878
			 0x2b77e2d1, /// 0x87c
			 0x6ce7697f, /// 0x880
			 0x7218a007, /// 0x884
			 0x9cf09061, /// 0x888
			 0xff9db974, /// 0x88c
			 0xbfd59191, /// 0x890
			 0xd228634e, /// 0x894
			 0x58f3691c, /// 0x898
			 0x440284f3, /// 0x89c
			 0x9d7375cf, /// 0x8a0
			 0x05852ea2, /// 0x8a4
			 0xc015442e, /// 0x8a8
			 0xfe27a414, /// 0x8ac
			 0xe6a52d23, /// 0x8b0
			 0x16c1f284, /// 0x8b4
			 0x15a28330, /// 0x8b8
			 0x298dcc62, /// 0x8bc
			 0xfe056521, /// 0x8c0
			 0x9dc07d99, /// 0x8c4
			 0xa7061076, /// 0x8c8
			 0xe461a88c, /// 0x8cc
			 0x631b8eb5, /// 0x8d0
			 0xf5e2285d, /// 0x8d4
			 0x4b58cd26, /// 0x8d8
			 0x2d03ce21, /// 0x8dc
			 0x921deefc, /// 0x8e0
			 0xed120a19, /// 0x8e4
			 0x5313aed6, /// 0x8e8
			 0x16efcc20, /// 0x8ec
			 0x32790d9e, /// 0x8f0
			 0xa236a970, /// 0x8f4
			 0xdc4f7855, /// 0x8f8
			 0xf4bd1b92, /// 0x8fc
			 0x98b45b9f, /// 0x900
			 0x60a89c34, /// 0x904
			 0xf500cbb6, /// 0x908
			 0xdcc09cf0, /// 0x90c
			 0xec787061, /// 0x910
			 0x1a03c487, /// 0x914
			 0xde169ac5, /// 0x918
			 0x39ee4270, /// 0x91c
			 0x8be38fd0, /// 0x920
			 0x198ac146, /// 0x924
			 0x763288cf, /// 0x928
			 0xb185bc3e, /// 0x92c
			 0x556ebc05, /// 0x930
			 0x0702ca9c, /// 0x934
			 0x77db328c, /// 0x938
			 0x3ceb45ac, /// 0x93c
			 0x8f727e84, /// 0x940
			 0xb9e8b543, /// 0x944
			 0x1b906a7f, /// 0x948
			 0xed370f07, /// 0x94c
			 0x18069ea0, /// 0x950
			 0xe1d04588, /// 0x954
			 0x5cf7e032, /// 0x958
			 0x9caf6ad7, /// 0x95c
			 0xbb6db2d3, /// 0x960
			 0xd953fa1b, /// 0x964
			 0xc05a8cbe, /// 0x968
			 0x50ab0f99, /// 0x96c
			 0x68e7b70b, /// 0x970
			 0x21410207, /// 0x974
			 0x20139728, /// 0x978
			 0xa1c7be8e, /// 0x97c
			 0xbcb742d3, /// 0x980
			 0x92608974, /// 0x984
			 0xe3a05c6d, /// 0x988
			 0x688e4343, /// 0x98c
			 0x8d152abf, /// 0x990
			 0x47a61c69, /// 0x994
			 0xc3fdf32b, /// 0x998
			 0xee417a1a, /// 0x99c
			 0x7acfad39, /// 0x9a0
			 0x92959cef, /// 0x9a4
			 0xe97b03a6, /// 0x9a8
			 0x597308d6, /// 0x9ac
			 0x32887d1a, /// 0x9b0
			 0xeac3bc57, /// 0x9b4
			 0xc04ff926, /// 0x9b8
			 0xc2e02306, /// 0x9bc
			 0xa188774b, /// 0x9c0
			 0xf91f10fb, /// 0x9c4
			 0x6784bb7a, /// 0x9c8
			 0xb289bcf7, /// 0x9cc
			 0x0cc21437, /// 0x9d0
			 0xccb76343, /// 0x9d4
			 0x49bc20cc, /// 0x9d8
			 0xccc27f98, /// 0x9dc
			 0x2ba040d7, /// 0x9e0
			 0x990216a9, /// 0x9e4
			 0x3cd97da4, /// 0x9e8
			 0x0746fe4e, /// 0x9ec
			 0xa3a9f44f, /// 0x9f0
			 0xbef51c1c, /// 0x9f4
			 0xc42cfc8c, /// 0x9f8
			 0x0b9974f3, /// 0x9fc
			 0x6936e69e, /// 0xa00
			 0x8cc6cbf7, /// 0xa04
			 0x0b560efd, /// 0xa08
			 0x2c38ba9a, /// 0xa0c
			 0xdbd26a70, /// 0xa10
			 0xc5a3cac8, /// 0xa14
			 0x90374dc2, /// 0xa18
			 0x0b995582, /// 0xa1c
			 0xae904310, /// 0xa20
			 0x7e6f8c8d, /// 0xa24
			 0x4f2c5de1, /// 0xa28
			 0xbeac078a, /// 0xa2c
			 0xc42f7764, /// 0xa30
			 0x8b0219e6, /// 0xa34
			 0x5f0ece82, /// 0xa38
			 0x8d2dab18, /// 0xa3c
			 0x60f6adaa, /// 0xa40
			 0x611873a3, /// 0xa44
			 0x8a245961, /// 0xa48
			 0x6528e4b5, /// 0xa4c
			 0x0c336eee, /// 0xa50
			 0x9d8b5752, /// 0xa54
			 0xfadf697c, /// 0xa58
			 0xf83a6847, /// 0xa5c
			 0xdf6f535e, /// 0xa60
			 0x2e435a0c, /// 0xa64
			 0x5788de57, /// 0xa68
			 0xee5c4004, /// 0xa6c
			 0xe8cbba4c, /// 0xa70
			 0x8a4a083e, /// 0xa74
			 0x22ebd49e, /// 0xa78
			 0x710b1426, /// 0xa7c
			 0x64ca1da7, /// 0xa80
			 0x807cd69c, /// 0xa84
			 0x8c0c0194, /// 0xa88
			 0xd7653cf3, /// 0xa8c
			 0x557542a9, /// 0xa90
			 0x678961be, /// 0xa94
			 0x98b1132b, /// 0xa98
			 0x6ad445ec, /// 0xa9c
			 0x64e375eb, /// 0xaa0
			 0x9687dc00, /// 0xaa4
			 0x69d3b2aa, /// 0xaa8
			 0x282adf46, /// 0xaac
			 0x3f789e45, /// 0xab0
			 0x30fd8546, /// 0xab4
			 0x3b48aad3, /// 0xab8
			 0x58829cc5, /// 0xabc
			 0x0a19e39c, /// 0xac0
			 0xa785179a, /// 0xac4
			 0x76700e5f, /// 0xac8
			 0x6d4c8683, /// 0xacc
			 0x90a04d00, /// 0xad0
			 0x434c9fdc, /// 0xad4
			 0x8eaff02a, /// 0xad8
			 0x1a35530d, /// 0xadc
			 0x53d6676f, /// 0xae0
			 0x7b3d2d3e, /// 0xae4
			 0x994845d6, /// 0xae8
			 0x3815fd24, /// 0xaec
			 0x16bfa3f6, /// 0xaf0
			 0xdc64359d, /// 0xaf4
			 0x2c81f876, /// 0xaf8
			 0x224051d2, /// 0xafc
			 0x1638063b, /// 0xb00
			 0x987a1c8b, /// 0xb04
			 0x98ef7314, /// 0xb08
			 0x0bf5fbb2, /// 0xb0c
			 0x19027d90, /// 0xb10
			 0x4a95b5a8, /// 0xb14
			 0x317d74df, /// 0xb18
			 0x47d7ebef, /// 0xb1c
			 0x8fa3fe68, /// 0xb20
			 0x52036816, /// 0xb24
			 0x63928406, /// 0xb28
			 0x56e3d788, /// 0xb2c
			 0x0603c230, /// 0xb30
			 0xd1e835e1, /// 0xb34
			 0x0f4214d0, /// 0xb38
			 0x5ebe9983, /// 0xb3c
			 0x521388eb, /// 0xb40
			 0x5048d454, /// 0xb44
			 0x4cffe479, /// 0xb48
			 0xc5fdd37f, /// 0xb4c
			 0x443dab65, /// 0xb50
			 0x1a5fb4f7, /// 0xb54
			 0x4ec5f1be, /// 0xb58
			 0xb0ec01ef, /// 0xb5c
			 0x828919e5, /// 0xb60
			 0xbd5fda0e, /// 0xb64
			 0xaaa3a7f3, /// 0xb68
			 0xdfd19f86, /// 0xb6c
			 0x82f4d4e1, /// 0xb70
			 0x2098fe0b, /// 0xb74
			 0xb034a2d4, /// 0xb78
			 0x5e4015d5, /// 0xb7c
			 0x7ad16f11, /// 0xb80
			 0x6345f3fd, /// 0xb84
			 0x300d010e, /// 0xb88
			 0xea62c1d8, /// 0xb8c
			 0xa9730a4d, /// 0xb90
			 0x00bb6b32, /// 0xb94
			 0xae3d7132, /// 0xb98
			 0x3dc18d42, /// 0xb9c
			 0x6aebc870, /// 0xba0
			 0x1f6603e2, /// 0xba4
			 0xa3fa1535, /// 0xba8
			 0x221f5e6e, /// 0xbac
			 0xc8e7ee15, /// 0xbb0
			 0xd0ad49dc, /// 0xbb4
			 0xfaf1c89a, /// 0xbb8
			 0x52299787, /// 0xbbc
			 0xae2f3656, /// 0xbc0
			 0x6cac76fb, /// 0xbc4
			 0xc9211d4a, /// 0xbc8
			 0xb8201e6d, /// 0xbcc
			 0xdd97bfa8, /// 0xbd0
			 0x5e4693bb, /// 0xbd4
			 0xae454f6e, /// 0xbd8
			 0x2534e123, /// 0xbdc
			 0xe8159ca0, /// 0xbe0
			 0xf8b302ea, /// 0xbe4
			 0x6da6ae7c, /// 0xbe8
			 0x91060fb9, /// 0xbec
			 0xc614cbc9, /// 0xbf0
			 0x83c66eed, /// 0xbf4
			 0xb59397c7, /// 0xbf8
			 0x9cb98831, /// 0xbfc
			 0xc5aea7b4, /// 0xc00
			 0x24e981f1, /// 0xc04
			 0xa6420253, /// 0xc08
			 0xd43653c8, /// 0xc0c
			 0xcf61856e, /// 0xc10
			 0x9f961c07, /// 0xc14
			 0xb4f6eb31, /// 0xc18
			 0xf9cc08e6, /// 0xc1c
			 0x14529870, /// 0xc20
			 0x65dd3770, /// 0xc24
			 0x9e2e43f0, /// 0xc28
			 0x50621fb6, /// 0xc2c
			 0xbf8a6cd2, /// 0xc30
			 0x7ed26a88, /// 0xc34
			 0x3961c453, /// 0xc38
			 0x0c5aa08d, /// 0xc3c
			 0xec2d8c6a, /// 0xc40
			 0x11651396, /// 0xc44
			 0x0a6b17a7, /// 0xc48
			 0xbb221306, /// 0xc4c
			 0x117dbc02, /// 0xc50
			 0x93fc15d0, /// 0xc54
			 0x64fcef14, /// 0xc58
			 0x4deb9db8, /// 0xc5c
			 0x8ca7923d, /// 0xc60
			 0x4cb14d48, /// 0xc64
			 0x12946ba3, /// 0xc68
			 0xe459e56b, /// 0xc6c
			 0xfe48a6dd, /// 0xc70
			 0xc3f4e08b, /// 0xc74
			 0xeab0603c, /// 0xc78
			 0xfcc0611d, /// 0xc7c
			 0x425353c4, /// 0xc80
			 0xf2241ba2, /// 0xc84
			 0x6fc55fda, /// 0xc88
			 0x49d7276b, /// 0xc8c
			 0x3e5cdb18, /// 0xc90
			 0x4adf6f3e, /// 0xc94
			 0x80a481fa, /// 0xc98
			 0xb4f2ac8d, /// 0xc9c
			 0x47027e68, /// 0xca0
			 0xd6524ce4, /// 0xca4
			 0xac8ace5c, /// 0xca8
			 0xad864ce5, /// 0xcac
			 0x1f70475e, /// 0xcb0
			 0x18a5773d, /// 0xcb4
			 0x0d461389, /// 0xcb8
			 0xe146e275, /// 0xcbc
			 0x6d357cfc, /// 0xcc0
			 0x25d3f3cc, /// 0xcc4
			 0x7f5a2b51, /// 0xcc8
			 0xddf5f1c6, /// 0xccc
			 0xe60dfcf8, /// 0xcd0
			 0xc4e180bf, /// 0xcd4
			 0x51f4d78e, /// 0xcd8
			 0x098f0dac, /// 0xcdc
			 0x472d3fe3, /// 0xce0
			 0xd08fd432, /// 0xce4
			 0x3603f7da, /// 0xce8
			 0xe1dd7476, /// 0xcec
			 0x9f172ede, /// 0xcf0
			 0x4c9169c6, /// 0xcf4
			 0x7df7f08f, /// 0xcf8
			 0x340cf9fa, /// 0xcfc
			 0x5cee4e11, /// 0xd00
			 0x0d174082, /// 0xd04
			 0xfa16d52c, /// 0xd08
			 0xef49556e, /// 0xd0c
			 0xa9b13aa5, /// 0xd10
			 0xc1956797, /// 0xd14
			 0xfb4e047b, /// 0xd18
			 0x023edc49, /// 0xd1c
			 0x73ae4d7f, /// 0xd20
			 0xa065cd33, /// 0xd24
			 0x56d3903d, /// 0xd28
			 0x8ae27dfe, /// 0xd2c
			 0x452055be, /// 0xd30
			 0x9b36821a, /// 0xd34
			 0x2e8c9da2, /// 0xd38
			 0x3e1fb45c, /// 0xd3c
			 0x5dc3489e, /// 0xd40
			 0x8e8f911e, /// 0xd44
			 0x30212276, /// 0xd48
			 0x012df8f3, /// 0xd4c
			 0x85785e86, /// 0xd50
			 0x6e97d39c, /// 0xd54
			 0xac0e4758, /// 0xd58
			 0xe9167c59, /// 0xd5c
			 0x253d1cec, /// 0xd60
			 0xbf7abe2e, /// 0xd64
			 0x2b904a95, /// 0xd68
			 0x70cd6e92, /// 0xd6c
			 0x288da727, /// 0xd70
			 0xb5c7d047, /// 0xd74
			 0x54f9af19, /// 0xd78
			 0x9d9ed09e, /// 0xd7c
			 0xcc2fbcd9, /// 0xd80
			 0xb8deb977, /// 0xd84
			 0x69353928, /// 0xd88
			 0xe16e14dd, /// 0xd8c
			 0x632e64ba, /// 0xd90
			 0x0fcecf4e, /// 0xd94
			 0xab539bc9, /// 0xd98
			 0x390acc32, /// 0xd9c
			 0xcdab714e, /// 0xda0
			 0x55ee2439, /// 0xda4
			 0x3c8226af, /// 0xda8
			 0x17ce8c99, /// 0xdac
			 0xd4488ca1, /// 0xdb0
			 0xbc64334a, /// 0xdb4
			 0x596beca7, /// 0xdb8
			 0xc85a53ce, /// 0xdbc
			 0x9ceb897e, /// 0xdc0
			 0x19b1bd89, /// 0xdc4
			 0x94e94b1e, /// 0xdc8
			 0x5ff2cad1, /// 0xdcc
			 0x680ee2fe, /// 0xdd0
			 0x014590aa, /// 0xdd4
			 0xfb2aaf80, /// 0xdd8
			 0xb7dd7d20, /// 0xddc
			 0xd58091a0, /// 0xde0
			 0xfff6b2c6, /// 0xde4
			 0x5cfb219c, /// 0xde8
			 0xb5019676, /// 0xdec
			 0x8e7cd09e, /// 0xdf0
			 0xbc1189ed, /// 0xdf4
			 0x354c4e09, /// 0xdf8
			 0x270ba4b2, /// 0xdfc
			 0x53f0e128, /// 0xe00
			 0x8a75e99d, /// 0xe04
			 0xf861983c, /// 0xe08
			 0xc5b50b35, /// 0xe0c
			 0xcf6e7a69, /// 0xe10
			 0xc725ec9f, /// 0xe14
			 0xd53a25e5, /// 0xe18
			 0x0f0db597, /// 0xe1c
			 0xa6030a17, /// 0xe20
			 0xc140941c, /// 0xe24
			 0x8251c2b2, /// 0xe28
			 0x3a3db4b6, /// 0xe2c
			 0xbb470ec0, /// 0xe30
			 0xca0c2dba, /// 0xe34
			 0x0b8586be, /// 0xe38
			 0xda6ebd1b, /// 0xe3c
			 0x2013e1ce, /// 0xe40
			 0x1bc44d34, /// 0xe44
			 0xe8d6cc8c, /// 0xe48
			 0x8ba520a6, /// 0xe4c
			 0x8994853e, /// 0xe50
			 0x7905f782, /// 0xe54
			 0xaab380f5, /// 0xe58
			 0x32671602, /// 0xe5c
			 0xf4182201, /// 0xe60
			 0x3b16f667, /// 0xe64
			 0xada8aac2, /// 0xe68
			 0x2c469192, /// 0xe6c
			 0xd14f9aae, /// 0xe70
			 0x5bd4ca7f, /// 0xe74
			 0x2a6be290, /// 0xe78
			 0x8fca3440, /// 0xe7c
			 0xeac1f351, /// 0xe80
			 0x9237b242, /// 0xe84
			 0xb4639723, /// 0xe88
			 0x45a10683, /// 0xe8c
			 0x6dd41217, /// 0xe90
			 0x93e903ee, /// 0xe94
			 0xdf4b0afc, /// 0xe98
			 0xea2a970f, /// 0xe9c
			 0xdabb00e8, /// 0xea0
			 0x28bac745, /// 0xea4
			 0x22974637, /// 0xea8
			 0x1f7e1263, /// 0xeac
			 0x35cae5f2, /// 0xeb0
			 0x8ca75300, /// 0xeb4
			 0x3fa6fdde, /// 0xeb8
			 0xb09be5d5, /// 0xebc
			 0x3b7f6aab, /// 0xec0
			 0x7874a9c3, /// 0xec4
			 0x2fb10640, /// 0xec8
			 0x2199485d, /// 0xecc
			 0x095eec65, /// 0xed0
			 0xe0cfc0f5, /// 0xed4
			 0x318e0489, /// 0xed8
			 0xc1996c48, /// 0xedc
			 0x2e50fb99, /// 0xee0
			 0xabdb462f, /// 0xee4
			 0x5aa89b33, /// 0xee8
			 0xe2d74193, /// 0xeec
			 0x51220547, /// 0xef0
			 0x142c84c9, /// 0xef4
			 0xaeba8a4d, /// 0xef8
			 0x7562949d, /// 0xefc
			 0xc16242a4, /// 0xf00
			 0x26aa92ef, /// 0xf04
			 0xb8efa845, /// 0xf08
			 0x34bfba8b, /// 0xf0c
			 0x163c20df, /// 0xf10
			 0x4fb34906, /// 0xf14
			 0xa8b59390, /// 0xf18
			 0xca8aadcc, /// 0xf1c
			 0xa1e54398, /// 0xf20
			 0x47528bd9, /// 0xf24
			 0x6b15ae0d, /// 0xf28
			 0x473edbda, /// 0xf2c
			 0x7f001256, /// 0xf30
			 0x26dfb14e, /// 0xf34
			 0xedb9f3f2, /// 0xf38
			 0xf19e8b48, /// 0xf3c
			 0x6960536a, /// 0xf40
			 0x95733964, /// 0xf44
			 0x7b0d25b7, /// 0xf48
			 0x164326e6, /// 0xf4c
			 0xb928c20d, /// 0xf50
			 0x1da9291c, /// 0xf54
			 0x60ce03db, /// 0xf58
			 0x7413e07c, /// 0xf5c
			 0x0f0abb46, /// 0xf60
			 0xe4a5ce9c, /// 0xf64
			 0x3c2279b6, /// 0xf68
			 0x28ab192c, /// 0xf6c
			 0x6f52b4bd, /// 0xf70
			 0xb8e5f3bb, /// 0xf74
			 0xa046bb51, /// 0xf78
			 0x9e29bf6f, /// 0xf7c
			 0xa08256e4, /// 0xf80
			 0xa9c050e0, /// 0xf84
			 0xd988ea20, /// 0xf88
			 0x6f031c5d, /// 0xf8c
			 0x60f3bd52, /// 0xf90
			 0x2d4db10a, /// 0xf94
			 0xfaef1e73, /// 0xf98
			 0x94aff5f0, /// 0xf9c
			 0xe94d220f, /// 0xfa0
			 0x83d5183b, /// 0xfa4
			 0xb89e1ea9, /// 0xfa8
			 0xdbfcece4, /// 0xfac
			 0xa6d12936, /// 0xfb0
			 0xe3577cb4, /// 0xfb4
			 0x15075740, /// 0xfb8
			 0x5c979273, /// 0xfbc
			 0x7f14e004, /// 0xfc0
			 0xdd7a2519, /// 0xfc4
			 0xb6b0a93d, /// 0xfc8
			 0xe36cae8d, /// 0xfcc
			 0x8d07c53f, /// 0xfd0
			 0x0127f7e5, /// 0xfd4
			 0x116c5149, /// 0xfd8
			 0x10ccdf0c, /// 0xfdc
			 0x470557c6, /// 0xfe0
			 0x0b2775f1, /// 0xfe4
			 0xa990acce, /// 0xfe8
			 0xe842c59b, /// 0xfec
			 0x657fc7a8, /// 0xff0
			 0xb7a40178, /// 0xff4
			 0xd59ab54e, /// 0xff8
			 0x6120e3a9 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x40e9f450, /// 0x0
			 0x00bae17e, /// 0x4
			 0x20869fe0, /// 0x8
			 0x4b4a8f6d, /// 0xc
			 0x7b018027, /// 0x10
			 0x6c62affe, /// 0x14
			 0xe5f22525, /// 0x18
			 0x5c4e9aeb, /// 0x1c
			 0x80a55d24, /// 0x20
			 0xafa5a47f, /// 0x24
			 0xd24666c9, /// 0x28
			 0x2ebf9b0c, /// 0x2c
			 0x235aff3d, /// 0x30
			 0x6a8e272f, /// 0x34
			 0xeb7dd332, /// 0x38
			 0xe1780857, /// 0x3c
			 0xfa352ded, /// 0x40
			 0x8018bb57, /// 0x44
			 0x50c0777f, /// 0x48
			 0x99e53223, /// 0x4c
			 0xfd80913e, /// 0x50
			 0x0666de5e, /// 0x54
			 0x1971ddef, /// 0x58
			 0x2235eaef, /// 0x5c
			 0x86b9def1, /// 0x60
			 0xc628f6e7, /// 0x64
			 0xfa9e5c60, /// 0x68
			 0x1ddcfe38, /// 0x6c
			 0x1e64cfb2, /// 0x70
			 0xecc5c62c, /// 0x74
			 0x1cbe1e34, /// 0x78
			 0x6bd944e1, /// 0x7c
			 0x9beb788b, /// 0x80
			 0x04c79f91, /// 0x84
			 0x3d4bdd48, /// 0x88
			 0xe7a54da1, /// 0x8c
			 0xcc8856fd, /// 0x90
			 0x0dcbff3b, /// 0x94
			 0x1b784d27, /// 0x98
			 0x5c8bdc25, /// 0x9c
			 0x44117528, /// 0xa0
			 0x6ac674ec, /// 0xa4
			 0xa58a63a2, /// 0xa8
			 0x4b2a5fb0, /// 0xac
			 0x5bd869a0, /// 0xb0
			 0x2172ef31, /// 0xb4
			 0xaf44152e, /// 0xb8
			 0x15b7313b, /// 0xbc
			 0x2cf7cd66, /// 0xc0
			 0xcda6f6ae, /// 0xc4
			 0x3aa28a24, /// 0xc8
			 0x620c2fbb, /// 0xcc
			 0x1a7c771d, /// 0xd0
			 0xa6a7ac43, /// 0xd4
			 0x1655d29e, /// 0xd8
			 0xf512cfc6, /// 0xdc
			 0x7630f9af, /// 0xe0
			 0xce91645e, /// 0xe4
			 0xb6f7c334, /// 0xe8
			 0xee226c1a, /// 0xec
			 0x5caf754b, /// 0xf0
			 0xd54589bc, /// 0xf4
			 0x33016edb, /// 0xf8
			 0x44a6a0d9, /// 0xfc
			 0xf17f8f09, /// 0x100
			 0x16e7f0f1, /// 0x104
			 0xb80c9b2f, /// 0x108
			 0x8a28d6f5, /// 0x10c
			 0x24cbdb76, /// 0x110
			 0x2414a6d0, /// 0x114
			 0xed0d2716, /// 0x118
			 0x2fdf60c3, /// 0x11c
			 0x609ca3a0, /// 0x120
			 0x0b50c9cc, /// 0x124
			 0x79c36489, /// 0x128
			 0x1849b426, /// 0x12c
			 0xd9a3b728, /// 0x130
			 0x281cdfd7, /// 0x134
			 0xf30bb1f2, /// 0x138
			 0x52916864, /// 0x13c
			 0xf65badbc, /// 0x140
			 0x0c1840df, /// 0x144
			 0xfdd7f585, /// 0x148
			 0x89c3b66f, /// 0x14c
			 0xc877a094, /// 0x150
			 0x9cdb8a3b, /// 0x154
			 0xd1b17497, /// 0x158
			 0xbc389a29, /// 0x15c
			 0x0650714c, /// 0x160
			 0x93c46255, /// 0x164
			 0x6d12f6aa, /// 0x168
			 0x2de15936, /// 0x16c
			 0x41c137b1, /// 0x170
			 0xcd1af259, /// 0x174
			 0x9393d88b, /// 0x178
			 0x2913b5ec, /// 0x17c
			 0x3966ba40, /// 0x180
			 0x41ae9860, /// 0x184
			 0x688e41e0, /// 0x188
			 0x2c6f4395, /// 0x18c
			 0x9c4b5d62, /// 0x190
			 0x9795d40d, /// 0x194
			 0xa93c18db, /// 0x198
			 0x7bbe4f37, /// 0x19c
			 0x9ec8be80, /// 0x1a0
			 0xaef0031e, /// 0x1a4
			 0x7bbbb3ae, /// 0x1a8
			 0x0e062356, /// 0x1ac
			 0x301c63df, /// 0x1b0
			 0x784c88b9, /// 0x1b4
			 0xe1296a08, /// 0x1b8
			 0xccfc2b1e, /// 0x1bc
			 0x590fa163, /// 0x1c0
			 0x814cd690, /// 0x1c4
			 0x78e35865, /// 0x1c8
			 0x78dd5f99, /// 0x1cc
			 0xcddd4982, /// 0x1d0
			 0xec63a797, /// 0x1d4
			 0xae1e68c7, /// 0x1d8
			 0x9498d4dd, /// 0x1dc
			 0xcdc63891, /// 0x1e0
			 0x106322d1, /// 0x1e4
			 0x41c6264c, /// 0x1e8
			 0xa6d123d5, /// 0x1ec
			 0x3aa9bfb6, /// 0x1f0
			 0xfdfef1f2, /// 0x1f4
			 0x634edc8a, /// 0x1f8
			 0x2d55c452, /// 0x1fc
			 0xa6611d93, /// 0x200
			 0xf3b003b1, /// 0x204
			 0x8be1a466, /// 0x208
			 0x0d916d99, /// 0x20c
			 0xd0daee44, /// 0x210
			 0x2441abf1, /// 0x214
			 0x68e5ca92, /// 0x218
			 0x25eb2594, /// 0x21c
			 0x55d7d965, /// 0x220
			 0xb2b738af, /// 0x224
			 0x517cf607, /// 0x228
			 0xfe8cb964, /// 0x22c
			 0xe95fd4c3, /// 0x230
			 0xef6243f7, /// 0x234
			 0x47e4f081, /// 0x238
			 0xce1d8dde, /// 0x23c
			 0x581d81ac, /// 0x240
			 0x88e1ba6c, /// 0x244
			 0x3a28716c, /// 0x248
			 0x76dd3c91, /// 0x24c
			 0x61528126, /// 0x250
			 0x90bf377f, /// 0x254
			 0x12ceec4c, /// 0x258
			 0xf71f19a7, /// 0x25c
			 0xcfe10299, /// 0x260
			 0x860f1aa4, /// 0x264
			 0x7d29565b, /// 0x268
			 0x32ae0245, /// 0x26c
			 0x25e7a33c, /// 0x270
			 0xba100bba, /// 0x274
			 0x635cc1fb, /// 0x278
			 0x4497b9d1, /// 0x27c
			 0x7887e27f, /// 0x280
			 0xb06b0c99, /// 0x284
			 0xf7af52cf, /// 0x288
			 0x050132bf, /// 0x28c
			 0x84460a3b, /// 0x290
			 0x0fdf3312, /// 0x294
			 0xc1973c0e, /// 0x298
			 0xdd45e31c, /// 0x29c
			 0x40083ba0, /// 0x2a0
			 0xd2226d35, /// 0x2a4
			 0x00d4a9b7, /// 0x2a8
			 0x645dc795, /// 0x2ac
			 0xf30afc72, /// 0x2b0
			 0x18a462c5, /// 0x2b4
			 0xfd352ba1, /// 0x2b8
			 0x7c81a6ee, /// 0x2bc
			 0xbe64df9c, /// 0x2c0
			 0x8cdde521, /// 0x2c4
			 0x6358cad8, /// 0x2c8
			 0x8e380566, /// 0x2cc
			 0xd219b8f5, /// 0x2d0
			 0xe4a89558, /// 0x2d4
			 0xc2912252, /// 0x2d8
			 0xd878384b, /// 0x2dc
			 0x1ba81e95, /// 0x2e0
			 0x116d697b, /// 0x2e4
			 0xd2385b88, /// 0x2e8
			 0xb00aacc9, /// 0x2ec
			 0x5aaf80a4, /// 0x2f0
			 0xb0b654cb, /// 0x2f4
			 0xa88f8630, /// 0x2f8
			 0x66208633, /// 0x2fc
			 0x670d7c04, /// 0x300
			 0xe460d08b, /// 0x304
			 0x786b50ec, /// 0x308
			 0xe3866656, /// 0x30c
			 0xc5820c1f, /// 0x310
			 0x968e5779, /// 0x314
			 0x48afbf1a, /// 0x318
			 0xef8388db, /// 0x31c
			 0xcad2261b, /// 0x320
			 0xa2d86c72, /// 0x324
			 0xf621e0f2, /// 0x328
			 0xbe57326f, /// 0x32c
			 0x8bb9cd45, /// 0x330
			 0xb953bb4e, /// 0x334
			 0xfe1d1d58, /// 0x338
			 0x0a0816e3, /// 0x33c
			 0xa4103394, /// 0x340
			 0x35ca8c65, /// 0x344
			 0xe7ca3b2c, /// 0x348
			 0x3c966c5b, /// 0x34c
			 0x1aeb7e90, /// 0x350
			 0xb0b425d3, /// 0x354
			 0xd215757d, /// 0x358
			 0x311457d1, /// 0x35c
			 0x10236c1f, /// 0x360
			 0x0a6d2ded, /// 0x364
			 0x6a4d71c1, /// 0x368
			 0x47625fcf, /// 0x36c
			 0x38de2337, /// 0x370
			 0x1d29cf1e, /// 0x374
			 0x0e259ea6, /// 0x378
			 0x7a6a8176, /// 0x37c
			 0xc1a8a9a5, /// 0x380
			 0x6dddfef0, /// 0x384
			 0xa44ce890, /// 0x388
			 0x7bf0aef4, /// 0x38c
			 0xbe40b947, /// 0x390
			 0x791cc2c8, /// 0x394
			 0x8ef3f9a1, /// 0x398
			 0xd7f7d3fa, /// 0x39c
			 0xc8870852, /// 0x3a0
			 0x7fcf1e27, /// 0x3a4
			 0x197b8d83, /// 0x3a8
			 0x573a898c, /// 0x3ac
			 0xecbc06a8, /// 0x3b0
			 0xfe95d372, /// 0x3b4
			 0xce072495, /// 0x3b8
			 0x0880fe29, /// 0x3bc
			 0xb42feea6, /// 0x3c0
			 0x1caa968b, /// 0x3c4
			 0x7ac95e92, /// 0x3c8
			 0x163dd8ba, /// 0x3cc
			 0xb07c8f62, /// 0x3d0
			 0x30def26b, /// 0x3d4
			 0xeb41c063, /// 0x3d8
			 0x832f5074, /// 0x3dc
			 0xcb970b8e, /// 0x3e0
			 0xa6467a52, /// 0x3e4
			 0x92c2a75b, /// 0x3e8
			 0x356bc48b, /// 0x3ec
			 0x797672cb, /// 0x3f0
			 0x0fbb72f8, /// 0x3f4
			 0x6c3b6707, /// 0x3f8
			 0x810c5428, /// 0x3fc
			 0xc436b612, /// 0x400
			 0x7cd9d3af, /// 0x404
			 0x9ab07093, /// 0x408
			 0xbb4f8a6f, /// 0x40c
			 0x620f7682, /// 0x410
			 0xfb7e8d85, /// 0x414
			 0x05065dbc, /// 0x418
			 0x7d2572ae, /// 0x41c
			 0x891f69a8, /// 0x420
			 0xd9c7c7bc, /// 0x424
			 0x6f22a1bf, /// 0x428
			 0x7fe56732, /// 0x42c
			 0x31ea0336, /// 0x430
			 0x44de16fb, /// 0x434
			 0x40d97989, /// 0x438
			 0xac879d8c, /// 0x43c
			 0x97b45a98, /// 0x440
			 0xce80fd21, /// 0x444
			 0x63fd3cd8, /// 0x448
			 0x709eefce, /// 0x44c
			 0xecf60042, /// 0x450
			 0xfd8149cb, /// 0x454
			 0x35108606, /// 0x458
			 0x0cf33971, /// 0x45c
			 0x21f11041, /// 0x460
			 0x7ad53dde, /// 0x464
			 0xcda6dd90, /// 0x468
			 0x258ad4d1, /// 0x46c
			 0xa477d346, /// 0x470
			 0xb5585fdd, /// 0x474
			 0x8634fd6c, /// 0x478
			 0xaeb37cc3, /// 0x47c
			 0xd032d50d, /// 0x480
			 0x4e2d5498, /// 0x484
			 0x29d95628, /// 0x488
			 0xf95f034a, /// 0x48c
			 0xc9436fda, /// 0x490
			 0xeb85cf29, /// 0x494
			 0x8566fbec, /// 0x498
			 0x3980223d, /// 0x49c
			 0x32a0c596, /// 0x4a0
			 0xd810c24b, /// 0x4a4
			 0x42ef9de7, /// 0x4a8
			 0x4d7bc7a5, /// 0x4ac
			 0xdbc6dc07, /// 0x4b0
			 0x2c89a988, /// 0x4b4
			 0x3f9230a7, /// 0x4b8
			 0x06e314aa, /// 0x4bc
			 0xc441f99a, /// 0x4c0
			 0x7d4aa0a1, /// 0x4c4
			 0x10e45627, /// 0x4c8
			 0x0c4a3ae2, /// 0x4cc
			 0x344ebcb3, /// 0x4d0
			 0x1220e08f, /// 0x4d4
			 0xb50f2521, /// 0x4d8
			 0x9511a6ac, /// 0x4dc
			 0xbe0ca742, /// 0x4e0
			 0x914dcdc5, /// 0x4e4
			 0x9322ea21, /// 0x4e8
			 0xa46881db, /// 0x4ec
			 0x659529c7, /// 0x4f0
			 0x963598c5, /// 0x4f4
			 0x13fa81ca, /// 0x4f8
			 0x17e4bed4, /// 0x4fc
			 0x3af9ae3f, /// 0x500
			 0xfcc81cc9, /// 0x504
			 0xa4b6a242, /// 0x508
			 0x431ae795, /// 0x50c
			 0x0d975ed6, /// 0x510
			 0xeb1a4ede, /// 0x514
			 0x17371a63, /// 0x518
			 0x73abecfc, /// 0x51c
			 0xe66e829c, /// 0x520
			 0x76799116, /// 0x524
			 0x2d78b07f, /// 0x528
			 0x9102c29d, /// 0x52c
			 0xda696c6d, /// 0x530
			 0x94966855, /// 0x534
			 0xba026c99, /// 0x538
			 0xed88f0c8, /// 0x53c
			 0x877d3adf, /// 0x540
			 0x9ce24ce5, /// 0x544
			 0xee22cdd9, /// 0x548
			 0xef632a08, /// 0x54c
			 0x70ae7fab, /// 0x550
			 0xa9ffd4c7, /// 0x554
			 0x3f163ae0, /// 0x558
			 0x5e1940fe, /// 0x55c
			 0x97cefad9, /// 0x560
			 0x52e835e1, /// 0x564
			 0x1089a534, /// 0x568
			 0xc5f4501f, /// 0x56c
			 0x8be29df9, /// 0x570
			 0xeb6ceeb5, /// 0x574
			 0x90a4358c, /// 0x578
			 0x4c4a7012, /// 0x57c
			 0x6541e8d0, /// 0x580
			 0xaac5df7f, /// 0x584
			 0xb2b189d9, /// 0x588
			 0xf4557be4, /// 0x58c
			 0x2ea86fb9, /// 0x590
			 0xed0de7ef, /// 0x594
			 0x5bf0bf8d, /// 0x598
			 0x38c86a93, /// 0x59c
			 0x1ef1fffa, /// 0x5a0
			 0x44caa8ef, /// 0x5a4
			 0xe9df14c4, /// 0x5a8
			 0xcd07cc7c, /// 0x5ac
			 0x2e8f2787, /// 0x5b0
			 0x622db895, /// 0x5b4
			 0x98d9a843, /// 0x5b8
			 0x1fe1a40b, /// 0x5bc
			 0x0c305dc3, /// 0x5c0
			 0xe18d5a31, /// 0x5c4
			 0xdee65355, /// 0x5c8
			 0x5a9b471c, /// 0x5cc
			 0x461a32a2, /// 0x5d0
			 0x76c71160, /// 0x5d4
			 0xdf7949ff, /// 0x5d8
			 0x710a370a, /// 0x5dc
			 0x0f41a2e5, /// 0x5e0
			 0x39928aa6, /// 0x5e4
			 0x265f3700, /// 0x5e8
			 0xfc013d86, /// 0x5ec
			 0x9ba01a27, /// 0x5f0
			 0x4c0a045f, /// 0x5f4
			 0x3ebfc223, /// 0x5f8
			 0x8ea853ec, /// 0x5fc
			 0xe0128808, /// 0x600
			 0x32c26469, /// 0x604
			 0x857dd3cb, /// 0x608
			 0xbd1bc622, /// 0x60c
			 0xd3964565, /// 0x610
			 0x5c732dac, /// 0x614
			 0x31ffd527, /// 0x618
			 0xb20de980, /// 0x61c
			 0x07aef8cb, /// 0x620
			 0x8bfa9b37, /// 0x624
			 0xaa05cece, /// 0x628
			 0x7cd5500a, /// 0x62c
			 0x6da90cd9, /// 0x630
			 0xf7ca14f4, /// 0x634
			 0x9aede29f, /// 0x638
			 0x12e1d9d8, /// 0x63c
			 0x1be0fa04, /// 0x640
			 0x36e7137d, /// 0x644
			 0xac0f9e00, /// 0x648
			 0x20245d1d, /// 0x64c
			 0xa227a81e, /// 0x650
			 0x4096d19c, /// 0x654
			 0x9bfc3bed, /// 0x658
			 0x8f07964a, /// 0x65c
			 0x7bd74f2f, /// 0x660
			 0x2ab24c03, /// 0x664
			 0x2e26e184, /// 0x668
			 0x9cf63d54, /// 0x66c
			 0xb74d86a9, /// 0x670
			 0x15a355d3, /// 0x674
			 0xa956ed4d, /// 0x678
			 0xde1d5eda, /// 0x67c
			 0x328a858a, /// 0x680
			 0x6bc9764e, /// 0x684
			 0xed3b291d, /// 0x688
			 0x7a9c197b, /// 0x68c
			 0x7d6e7f64, /// 0x690
			 0xc806ed03, /// 0x694
			 0x3a49c213, /// 0x698
			 0x3ad91459, /// 0x69c
			 0x33a59ffa, /// 0x6a0
			 0x1a0c42b1, /// 0x6a4
			 0x4f3953ee, /// 0x6a8
			 0x73323fda, /// 0x6ac
			 0x54f08db8, /// 0x6b0
			 0x66031e64, /// 0x6b4
			 0x75d83bbd, /// 0x6b8
			 0xd1fa776f, /// 0x6bc
			 0x4e597c7d, /// 0x6c0
			 0x353ebe21, /// 0x6c4
			 0x8c68b335, /// 0x6c8
			 0xf22a9dcb, /// 0x6cc
			 0x0629013c, /// 0x6d0
			 0x5bb38216, /// 0x6d4
			 0x1a2c99a9, /// 0x6d8
			 0x1a503a52, /// 0x6dc
			 0xb224d8b6, /// 0x6e0
			 0x43e1baf0, /// 0x6e4
			 0x7d8989b7, /// 0x6e8
			 0xd4da68e1, /// 0x6ec
			 0x9abaf093, /// 0x6f0
			 0x9b2e3da3, /// 0x6f4
			 0x07ae20df, /// 0x6f8
			 0x2954097e, /// 0x6fc
			 0xdffedbac, /// 0x700
			 0x7ad7c920, /// 0x704
			 0xb18d44e7, /// 0x708
			 0x32af2699, /// 0x70c
			 0x0e3ebf1e, /// 0x710
			 0xd1940448, /// 0x714
			 0x3fb2113e, /// 0x718
			 0xad0ab629, /// 0x71c
			 0x8cb5e3ec, /// 0x720
			 0x9c418167, /// 0x724
			 0x0127163d, /// 0x728
			 0x9e00c317, /// 0x72c
			 0x4bf13c61, /// 0x730
			 0x2e669553, /// 0x734
			 0x11e39728, /// 0x738
			 0x9e34cf65, /// 0x73c
			 0x8d12d86b, /// 0x740
			 0xa3c86ab5, /// 0x744
			 0xdc8d6509, /// 0x748
			 0x04cbfbca, /// 0x74c
			 0xbadd4f5d, /// 0x750
			 0xd554f6aa, /// 0x754
			 0x0f27873e, /// 0x758
			 0xbe43868a, /// 0x75c
			 0xdc2503a4, /// 0x760
			 0xf4aaafd5, /// 0x764
			 0x6df0b044, /// 0x768
			 0xd5c8ca39, /// 0x76c
			 0x8fa41196, /// 0x770
			 0xb0231b4f, /// 0x774
			 0x489b694d, /// 0x778
			 0xff75ab92, /// 0x77c
			 0x60a39e14, /// 0x780
			 0xa2fdec04, /// 0x784
			 0x21f4f97b, /// 0x788
			 0x21db07eb, /// 0x78c
			 0x27224b2f, /// 0x790
			 0xb3996ef6, /// 0x794
			 0xce538248, /// 0x798
			 0x60a0d0b4, /// 0x79c
			 0xb242c814, /// 0x7a0
			 0x377b7e4b, /// 0x7a4
			 0x82eb4820, /// 0x7a8
			 0x8b41d080, /// 0x7ac
			 0x510abd77, /// 0x7b0
			 0xff15cda8, /// 0x7b4
			 0xc159cedd, /// 0x7b8
			 0x24ac21e0, /// 0x7bc
			 0x95e934d9, /// 0x7c0
			 0xbc55f1cf, /// 0x7c4
			 0x4fd5d024, /// 0x7c8
			 0x6d0a4184, /// 0x7cc
			 0xab6736cb, /// 0x7d0
			 0x41327f8c, /// 0x7d4
			 0x519876fe, /// 0x7d8
			 0x353d1447, /// 0x7dc
			 0xde862f65, /// 0x7e0
			 0xc91b83c2, /// 0x7e4
			 0xa7e1fc47, /// 0x7e8
			 0x7ea2ff6f, /// 0x7ec
			 0x72bb37ad, /// 0x7f0
			 0xfd26f372, /// 0x7f4
			 0xdf62628f, /// 0x7f8
			 0x56f55644, /// 0x7fc
			 0x086f9f95, /// 0x800
			 0xf6597525, /// 0x804
			 0xa4af7321, /// 0x808
			 0x1e8b4b31, /// 0x80c
			 0x1108291c, /// 0x810
			 0x3c79d060, /// 0x814
			 0xcd358e33, /// 0x818
			 0x9f1f50b8, /// 0x81c
			 0xbda7b806, /// 0x820
			 0x93d1018a, /// 0x824
			 0x651e83db, /// 0x828
			 0x79ba377e, /// 0x82c
			 0xff2a886e, /// 0x830
			 0x56ae1221, /// 0x834
			 0x33455cad, /// 0x838
			 0x75ba49da, /// 0x83c
			 0xd99ea74f, /// 0x840
			 0x501fbeac, /// 0x844
			 0x014b2285, /// 0x848
			 0xdb3660c5, /// 0x84c
			 0x217420d0, /// 0x850
			 0x81c250cf, /// 0x854
			 0x379f72f5, /// 0x858
			 0x6ff6da4e, /// 0x85c
			 0x9a47ad6d, /// 0x860
			 0x03ca3222, /// 0x864
			 0x0b79f456, /// 0x868
			 0x0ff86efb, /// 0x86c
			 0x077bc696, /// 0x870
			 0x0c616559, /// 0x874
			 0xb2916d49, /// 0x878
			 0x84d6d3e2, /// 0x87c
			 0x69948433, /// 0x880
			 0x52cf7651, /// 0x884
			 0x373c2a52, /// 0x888
			 0xa4bbf553, /// 0x88c
			 0xe139bfc8, /// 0x890
			 0xca1a240a, /// 0x894
			 0xcb00eae5, /// 0x898
			 0x6e3e0ef8, /// 0x89c
			 0xdf9b57ce, /// 0x8a0
			 0x517ac1c3, /// 0x8a4
			 0x6cd9167a, /// 0x8a8
			 0x12f65a0a, /// 0x8ac
			 0x071ac49d, /// 0x8b0
			 0x646cc9e2, /// 0x8b4
			 0x187508c1, /// 0x8b8
			 0x2839824f, /// 0x8bc
			 0xadffd1d7, /// 0x8c0
			 0x6b2fdf99, /// 0x8c4
			 0xf0e26753, /// 0x8c8
			 0x91ef10f9, /// 0x8cc
			 0x7b49b42a, /// 0x8d0
			 0x94f518e9, /// 0x8d4
			 0xa0d832dc, /// 0x8d8
			 0xac901c4c, /// 0x8dc
			 0xceb99f11, /// 0x8e0
			 0x0faa0f2b, /// 0x8e4
			 0xfc37726a, /// 0x8e8
			 0xcc5155ad, /// 0x8ec
			 0xd0cda4ac, /// 0x8f0
			 0xb9ff3dd0, /// 0x8f4
			 0x7404bbd5, /// 0x8f8
			 0x5327380b, /// 0x8fc
			 0x05d4608f, /// 0x900
			 0x6951725f, /// 0x904
			 0x9f003cd4, /// 0x908
			 0x94548bc6, /// 0x90c
			 0x0582e3ec, /// 0x910
			 0xa6e900d4, /// 0x914
			 0x26890771, /// 0x918
			 0x3e6c7da5, /// 0x91c
			 0x9b1fe796, /// 0x920
			 0x5b42b25a, /// 0x924
			 0x39aadf10, /// 0x928
			 0x18fc76f4, /// 0x92c
			 0x4378b66e, /// 0x930
			 0x3bf1a607, /// 0x934
			 0xd1cc61e8, /// 0x938
			 0x7014bbab, /// 0x93c
			 0x448a5c34, /// 0x940
			 0xdcf2e021, /// 0x944
			 0x656c1171, /// 0x948
			 0x9e0e83f3, /// 0x94c
			 0x1dfd5794, /// 0x950
			 0x92e73857, /// 0x954
			 0x4e4fad79, /// 0x958
			 0x79c28d1b, /// 0x95c
			 0xe3f4f859, /// 0x960
			 0x4fb3da5b, /// 0x964
			 0x91fcba89, /// 0x968
			 0x5352bdae, /// 0x96c
			 0x53d71d85, /// 0x970
			 0x082cdf9d, /// 0x974
			 0xa65fc701, /// 0x978
			 0xd2e3df1f, /// 0x97c
			 0x63866c85, /// 0x980
			 0xbfbea80b, /// 0x984
			 0x0ddd29af, /// 0x988
			 0xb10aa056, /// 0x98c
			 0x778cb60a, /// 0x990
			 0x22dd9ca3, /// 0x994
			 0x2f97b2a2, /// 0x998
			 0x3e1af4a6, /// 0x99c
			 0xa31449cd, /// 0x9a0
			 0xf3ac0af2, /// 0x9a4
			 0x25de476c, /// 0x9a8
			 0x2ee659c9, /// 0x9ac
			 0xb4d0e1cd, /// 0x9b0
			 0x8a8c69fd, /// 0x9b4
			 0xb153dff1, /// 0x9b8
			 0xb00fddd3, /// 0x9bc
			 0xeb799c56, /// 0x9c0
			 0xaf27922c, /// 0x9c4
			 0xabc9d528, /// 0x9c8
			 0xcc4cdc58, /// 0x9cc
			 0x864d096a, /// 0x9d0
			 0x2c355330, /// 0x9d4
			 0x77a47f1f, /// 0x9d8
			 0x3349f759, /// 0x9dc
			 0x5923e424, /// 0x9e0
			 0x137754c1, /// 0x9e4
			 0x9052e4b6, /// 0x9e8
			 0x2cdae898, /// 0x9ec
			 0x2fa0c942, /// 0x9f0
			 0xff41a63e, /// 0x9f4
			 0x15979736, /// 0x9f8
			 0xd8afc394, /// 0x9fc
			 0x97d8e4c0, /// 0xa00
			 0x6c24b272, /// 0xa04
			 0xfb2ecfe0, /// 0xa08
			 0x5af555f3, /// 0xa0c
			 0x5e0a6a54, /// 0xa10
			 0x4aad34ce, /// 0xa14
			 0x3b64defd, /// 0xa18
			 0xf036950d, /// 0xa1c
			 0x897ae1a5, /// 0xa20
			 0x2ea1a9f2, /// 0xa24
			 0xbc1832d8, /// 0xa28
			 0xb80f63a4, /// 0xa2c
			 0x33ca83b3, /// 0xa30
			 0x5911ae79, /// 0xa34
			 0x319f5616, /// 0xa38
			 0xb10fc63d, /// 0xa3c
			 0x1ef86206, /// 0xa40
			 0x97c3037e, /// 0xa44
			 0x4b7489b0, /// 0xa48
			 0x06a22d5f, /// 0xa4c
			 0x4d22b94b, /// 0xa50
			 0xd363399b, /// 0xa54
			 0xcc0787b8, /// 0xa58
			 0xb81af04e, /// 0xa5c
			 0x98fe7b9f, /// 0xa60
			 0xb3371387, /// 0xa64
			 0xeaf8b5f2, /// 0xa68
			 0x44978476, /// 0xa6c
			 0x94100b53, /// 0xa70
			 0x88529df4, /// 0xa74
			 0x8d628ded, /// 0xa78
			 0xc2beadac, /// 0xa7c
			 0xb253c0ee, /// 0xa80
			 0xd1f84e00, /// 0xa84
			 0xdf32ef2d, /// 0xa88
			 0x05f19bbd, /// 0xa8c
			 0x15a05d98, /// 0xa90
			 0x4f312808, /// 0xa94
			 0x7147a486, /// 0xa98
			 0x36356f58, /// 0xa9c
			 0xf11875e4, /// 0xaa0
			 0x9e296687, /// 0xaa4
			 0x7916aaea, /// 0xaa8
			 0x2b065e76, /// 0xaac
			 0xe2ab206d, /// 0xab0
			 0xcb519d85, /// 0xab4
			 0x946a3370, /// 0xab8
			 0xd4f8f053, /// 0xabc
			 0x710507b7, /// 0xac0
			 0xbee112aa, /// 0xac4
			 0x1d055da7, /// 0xac8
			 0xc344e709, /// 0xacc
			 0xebc92d64, /// 0xad0
			 0x1f80e274, /// 0xad4
			 0x545fc267, /// 0xad8
			 0xb4b03a28, /// 0xadc
			 0x598e2c94, /// 0xae0
			 0xf6dd00d6, /// 0xae4
			 0x30ac5c94, /// 0xae8
			 0x39b77a3f, /// 0xaec
			 0x14e93b4c, /// 0xaf0
			 0x538ce119, /// 0xaf4
			 0x481bf78e, /// 0xaf8
			 0x9c4f3ee3, /// 0xafc
			 0x2e2d25c9, /// 0xb00
			 0xf18e441d, /// 0xb04
			 0x712ca178, /// 0xb08
			 0xd3359dce, /// 0xb0c
			 0x68d4918a, /// 0xb10
			 0x0f2284f6, /// 0xb14
			 0xc3632c32, /// 0xb18
			 0x49ebddc2, /// 0xb1c
			 0x79829dff, /// 0xb20
			 0x1a57874e, /// 0xb24
			 0xf9e980ea, /// 0xb28
			 0x86c66ce2, /// 0xb2c
			 0x98f84a7c, /// 0xb30
			 0xe3f65bc5, /// 0xb34
			 0xd2abe990, /// 0xb38
			 0xc9564da5, /// 0xb3c
			 0xf5d9c476, /// 0xb40
			 0xcdcb1ba9, /// 0xb44
			 0x6ccc183a, /// 0xb48
			 0xa643de3e, /// 0xb4c
			 0xf2b8a79c, /// 0xb50
			 0xcf4dff61, /// 0xb54
			 0xceba071c, /// 0xb58
			 0x7f795c49, /// 0xb5c
			 0xcf75d304, /// 0xb60
			 0x6359d0f0, /// 0xb64
			 0xd0369cbb, /// 0xb68
			 0x687127f2, /// 0xb6c
			 0x0da6fa57, /// 0xb70
			 0x945ae098, /// 0xb74
			 0xf230e4a1, /// 0xb78
			 0xc5ee594b, /// 0xb7c
			 0xd9b4eed3, /// 0xb80
			 0x9a2d90e8, /// 0xb84
			 0x96d5a957, /// 0xb88
			 0x40cc4d38, /// 0xb8c
			 0x436000ae, /// 0xb90
			 0x5e79b39b, /// 0xb94
			 0x9e8a0423, /// 0xb98
			 0xb345ea55, /// 0xb9c
			 0xa605eb24, /// 0xba0
			 0x8921ef1d, /// 0xba4
			 0x7947b8e0, /// 0xba8
			 0x608ae780, /// 0xbac
			 0x3e87c901, /// 0xbb0
			 0xebdbb31e, /// 0xbb4
			 0x5a9bd9cb, /// 0xbb8
			 0x762accae, /// 0xbbc
			 0x60aab8fc, /// 0xbc0
			 0xd3aea788, /// 0xbc4
			 0x07fcbe9e, /// 0xbc8
			 0x37a8aa21, /// 0xbcc
			 0x15eeeab2, /// 0xbd0
			 0x3da109ec, /// 0xbd4
			 0x72e1c3c5, /// 0xbd8
			 0x8741b0ea, /// 0xbdc
			 0xf7439b85, /// 0xbe0
			 0x7470b85a, /// 0xbe4
			 0xa72b4600, /// 0xbe8
			 0xb906b907, /// 0xbec
			 0x6a5d4b71, /// 0xbf0
			 0x3cf55bbf, /// 0xbf4
			 0x2a06eb01, /// 0xbf8
			 0x9be4fddd, /// 0xbfc
			 0x3ff853e1, /// 0xc00
			 0x5333f078, /// 0xc04
			 0x2a1ea64a, /// 0xc08
			 0x5932ef05, /// 0xc0c
			 0x120d6ddc, /// 0xc10
			 0x40541784, /// 0xc14
			 0x66f3d550, /// 0xc18
			 0xd0d10d20, /// 0xc1c
			 0x34ef2fe8, /// 0xc20
			 0xeaf7fa30, /// 0xc24
			 0x5ea1f4c2, /// 0xc28
			 0xb1544edd, /// 0xc2c
			 0x9c7862f8, /// 0xc30
			 0x185a1ff3, /// 0xc34
			 0x823356ef, /// 0xc38
			 0xa0b019d7, /// 0xc3c
			 0xbda014e1, /// 0xc40
			 0xba48ca16, /// 0xc44
			 0x85cfce01, /// 0xc48
			 0x1a39972a, /// 0xc4c
			 0xe35c1519, /// 0xc50
			 0x334442c2, /// 0xc54
			 0x724e3894, /// 0xc58
			 0x5a0a8b85, /// 0xc5c
			 0xbf56458e, /// 0xc60
			 0xb54b84df, /// 0xc64
			 0xfe7b8f29, /// 0xc68
			 0x4053026b, /// 0xc6c
			 0xc401c232, /// 0xc70
			 0x4a181e34, /// 0xc74
			 0x166cd9ed, /// 0xc78
			 0xf3e120ab, /// 0xc7c
			 0x94cf71ad, /// 0xc80
			 0xe7b27bbd, /// 0xc84
			 0x1a5df62f, /// 0xc88
			 0xfc2b7900, /// 0xc8c
			 0xb154e8a3, /// 0xc90
			 0x3b2995b9, /// 0xc94
			 0xc6acbcee, /// 0xc98
			 0x2adb366e, /// 0xc9c
			 0x2087b71f, /// 0xca0
			 0x00733fa4, /// 0xca4
			 0x060695dc, /// 0xca8
			 0x63e20a41, /// 0xcac
			 0xa109ea69, /// 0xcb0
			 0x58e8ae01, /// 0xcb4
			 0xb5f0da05, /// 0xcb8
			 0x564dbd3a, /// 0xcbc
			 0x66cf9cb1, /// 0xcc0
			 0xe5a4092c, /// 0xcc4
			 0x98932c0f, /// 0xcc8
			 0x681be9fc, /// 0xccc
			 0x60ee10b9, /// 0xcd0
			 0x9ce49158, /// 0xcd4
			 0xc9ca9f0b, /// 0xcd8
			 0x3b34c8e7, /// 0xcdc
			 0xb0f13b8e, /// 0xce0
			 0x3385b45f, /// 0xce4
			 0x8ebf5d49, /// 0xce8
			 0xb23d2f65, /// 0xcec
			 0x0f4e0672, /// 0xcf0
			 0x9d32ee9a, /// 0xcf4
			 0xff97390a, /// 0xcf8
			 0x05fb6e24, /// 0xcfc
			 0x996431a7, /// 0xd00
			 0x6d2f697a, /// 0xd04
			 0x99d906d2, /// 0xd08
			 0xeb2f3294, /// 0xd0c
			 0xcfaa32ae, /// 0xd10
			 0x1b449261, /// 0xd14
			 0x3f55fc40, /// 0xd18
			 0x3184db97, /// 0xd1c
			 0x74522cc0, /// 0xd20
			 0xc30d29ec, /// 0xd24
			 0x8616c79a, /// 0xd28
			 0x8c9fe555, /// 0xd2c
			 0xb158a277, /// 0xd30
			 0x6534e0af, /// 0xd34
			 0x50ba3390, /// 0xd38
			 0xf72240e5, /// 0xd3c
			 0x05f2da3b, /// 0xd40
			 0x75477efb, /// 0xd44
			 0x34eb8713, /// 0xd48
			 0x45bea0a4, /// 0xd4c
			 0x208ab8c2, /// 0xd50
			 0xdbc67985, /// 0xd54
			 0x2e6c0971, /// 0xd58
			 0x572d3327, /// 0xd5c
			 0xb3353af6, /// 0xd60
			 0x25c670e6, /// 0xd64
			 0x17020784, /// 0xd68
			 0xc9cfa13e, /// 0xd6c
			 0xd2c05bac, /// 0xd70
			 0x52ac180f, /// 0xd74
			 0x5486d312, /// 0xd78
			 0x40b6b68c, /// 0xd7c
			 0x7268c9c9, /// 0xd80
			 0xb6f68832, /// 0xd84
			 0xff535ed7, /// 0xd88
			 0xf440f453, /// 0xd8c
			 0xf4b4fdb0, /// 0xd90
			 0x8169c443, /// 0xd94
			 0x8815663a, /// 0xd98
			 0x4e799969, /// 0xd9c
			 0x98e2721a, /// 0xda0
			 0xfa9ce5f3, /// 0xda4
			 0xec634479, /// 0xda8
			 0xbdbd89ad, /// 0xdac
			 0x49e0b40e, /// 0xdb0
			 0xfc8c4574, /// 0xdb4
			 0x28581e85, /// 0xdb8
			 0xb21c1498, /// 0xdbc
			 0xfbb7014c, /// 0xdc0
			 0x31f53287, /// 0xdc4
			 0x1627f303, /// 0xdc8
			 0xbd424428, /// 0xdcc
			 0x0e19265e, /// 0xdd0
			 0xe7b97a23, /// 0xdd4
			 0xf3a31e52, /// 0xdd8
			 0x409164b9, /// 0xddc
			 0xae2c73ef, /// 0xde0
			 0x18a361d2, /// 0xde4
			 0x7a046488, /// 0xde8
			 0x60696ab6, /// 0xdec
			 0x00926145, /// 0xdf0
			 0x51e8c5f6, /// 0xdf4
			 0x45d585bc, /// 0xdf8
			 0xae82c90f, /// 0xdfc
			 0xfee76f46, /// 0xe00
			 0xb0f92f3e, /// 0xe04
			 0x089cb49f, /// 0xe08
			 0x4851992f, /// 0xe0c
			 0x844088fa, /// 0xe10
			 0xc71480dd, /// 0xe14
			 0x7a458297, /// 0xe18
			 0x0b0e4e8d, /// 0xe1c
			 0xcffe1d56, /// 0xe20
			 0x72a2bf66, /// 0xe24
			 0x40864d70, /// 0xe28
			 0x772087b1, /// 0xe2c
			 0xfbf60990, /// 0xe30
			 0x15802af2, /// 0xe34
			 0x32fd1ff7, /// 0xe38
			 0x8f436b86, /// 0xe3c
			 0xa68dc81d, /// 0xe40
			 0x0b9c90fd, /// 0xe44
			 0xd3ae095c, /// 0xe48
			 0xbe4188ef, /// 0xe4c
			 0x3113a0d1, /// 0xe50
			 0xf801f9d6, /// 0xe54
			 0xa79d4eb8, /// 0xe58
			 0xe32317d2, /// 0xe5c
			 0x19a0f4db, /// 0xe60
			 0x8578bc13, /// 0xe64
			 0x258748e2, /// 0xe68
			 0x3e98399a, /// 0xe6c
			 0x3c0a64a9, /// 0xe70
			 0x1554f261, /// 0xe74
			 0xd705390f, /// 0xe78
			 0x182c6239, /// 0xe7c
			 0x2262f265, /// 0xe80
			 0x6b901388, /// 0xe84
			 0x6d689ca6, /// 0xe88
			 0x035e5a2e, /// 0xe8c
			 0x51b34065, /// 0xe90
			 0x44577f7b, /// 0xe94
			 0xe45b864d, /// 0xe98
			 0xa9a6d36a, /// 0xe9c
			 0x9687b7e3, /// 0xea0
			 0xafd2923a, /// 0xea4
			 0xaa47fffd, /// 0xea8
			 0x99e2c756, /// 0xeac
			 0xec2c0a03, /// 0xeb0
			 0x40da21ad, /// 0xeb4
			 0x47d6a655, /// 0xeb8
			 0xb0c9d2c6, /// 0xebc
			 0xe80072c2, /// 0xec0
			 0x9e7593e4, /// 0xec4
			 0x4d76d73a, /// 0xec8
			 0x44c73a0a, /// 0xecc
			 0x91cd52b2, /// 0xed0
			 0xdbc6b989, /// 0xed4
			 0x96f34f25, /// 0xed8
			 0xe0f23ccf, /// 0xedc
			 0xab5fb555, /// 0xee0
			 0x7cd4e47b, /// 0xee4
			 0x8d7e6bea, /// 0xee8
			 0x5e3fc8bc, /// 0xeec
			 0xb4b9a04b, /// 0xef0
			 0xa8d568ec, /// 0xef4
			 0x3bcd982c, /// 0xef8
			 0x37a2cd61, /// 0xefc
			 0xcdd63ba2, /// 0xf00
			 0x7a113ab0, /// 0xf04
			 0x53992e41, /// 0xf08
			 0x6adaa5a9, /// 0xf0c
			 0x8ed0f5e8, /// 0xf10
			 0x7a046472, /// 0xf14
			 0x34aeb2b1, /// 0xf18
			 0x4394cfc4, /// 0xf1c
			 0x5cf140c5, /// 0xf20
			 0xbce48959, /// 0xf24
			 0x3061b96a, /// 0xf28
			 0xf1c342a3, /// 0xf2c
			 0xeac088d3, /// 0xf30
			 0x1df41825, /// 0xf34
			 0x0c4b43f9, /// 0xf38
			 0x92f409f1, /// 0xf3c
			 0x8629c8f6, /// 0xf40
			 0x75fae1f0, /// 0xf44
			 0x20043c47, /// 0xf48
			 0xe98d2062, /// 0xf4c
			 0x0cfee3cb, /// 0xf50
			 0xe238e72f, /// 0xf54
			 0xbc3f4ae0, /// 0xf58
			 0xfe3bc046, /// 0xf5c
			 0x7e9de557, /// 0xf60
			 0xe46246f7, /// 0xf64
			 0x1da1e278, /// 0xf68
			 0x7a9e9e6f, /// 0xf6c
			 0x9cca68ad, /// 0xf70
			 0xf2db24b8, /// 0xf74
			 0x67f86946, /// 0xf78
			 0xbbff659c, /// 0xf7c
			 0x917d7a85, /// 0xf80
			 0x745608f9, /// 0xf84
			 0x8370974c, /// 0xf88
			 0x8b2476c7, /// 0xf8c
			 0xdc6713de, /// 0xf90
			 0xc655f741, /// 0xf94
			 0x9b1a897f, /// 0xf98
			 0xcfa35b0f, /// 0xf9c
			 0x23b1cdd7, /// 0xfa0
			 0xfa908a77, /// 0xfa4
			 0xf03671f5, /// 0xfa8
			 0xd67e6f6d, /// 0xfac
			 0xd89424d1, /// 0xfb0
			 0xc2450c67, /// 0xfb4
			 0xd8ddb884, /// 0xfb8
			 0x641fdf1c, /// 0xfbc
			 0xed161ceb, /// 0xfc0
			 0xa342f08a, /// 0xfc4
			 0x07001844, /// 0xfc8
			 0x2b4875e0, /// 0xfcc
			 0x31a072c2, /// 0xfd0
			 0xfefbb7a7, /// 0xfd4
			 0x26b64ea2, /// 0xfd8
			 0x21c1a08d, /// 0xfdc
			 0x1bede26f, /// 0xfe0
			 0x0a4fc438, /// 0xfe4
			 0xb6325b93, /// 0xfe8
			 0xf2e9b72a, /// 0xfec
			 0x51a774a2, /// 0xff0
			 0xe22d716f, /// 0xff4
			 0xcc748075, /// 0xff8
			 0x5b72bd83 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00000
			 0x0c600000,                                                  // Leading 1s:                                 /// 00004
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00008
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0000c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00010
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00014
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x55555555,                                                  // 4 random values                             /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00034
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0003c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00040
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00048
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0004c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00050
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00054
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00058
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00060
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00068
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00074
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00078
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0007c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00080
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00088
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0008c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00090
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00094
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00098
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0009c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000ac
			 0x33333333,                                                  // 4 random values                             /// 000b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00100
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00104
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80011111,                                                  // -9.7958E-41                                 /// 0010c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00110
			 0x0c700000,                                                  // Leading 1s:                                 /// 00114
			 0xcb000000,                                                  // -8388608.0                                  /// 00118
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0011c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00120
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00128
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0012c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00130
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0013c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00140
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00148
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0014c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00154
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00158
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0015c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00164
			 0x0e000003,                                                  // Trailing 1s:                                /// 00168
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00170
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00174
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00180
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00188
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0018c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00190
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00194
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0019c
			 0x7f800000,                                                  // inf                                         /// 001a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00200
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0020c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00210
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00214
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00218
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00220
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00224
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00228
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0022c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00230
			 0x80000000,                                                  // -zero                                       /// 00234
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00238
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0023c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00240
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00244
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00248
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00250
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00254
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000000,                                                  // -zero                                       /// 0025c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00264
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00268
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0026c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00274
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x3f028f5c,                                                  // 0.51                                        /// 0027c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00284
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0028c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00290
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00294
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00298
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002c0
			 0x3f028f5c,                                                  // 0.51                                        /// 002c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x55555555,                                                  // 4 random values                             /// 002d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00304
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00308
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00310
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00314
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0031c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00324
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0032c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00330
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00334
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00338
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0033c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00344
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0034c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00350
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00354
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00358
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0035c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0036c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00370
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00374
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00378
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00380
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0038c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00390
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00398
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0039c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a4
			 0x3f028f5c,                                                  // 0.51                                        /// 003a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003bc
			 0xcb000000,                                                  // -8388608.0                                  /// 003c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0xffc00001,                                                  // -signaling NaN                              /// 003d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e0
			 0xbf028f5c,                                                  // -0.51                                       /// 003e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0xffc00001,                                                  // -signaling NaN                              /// 003f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00404
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00408
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00414
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00418
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0041c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00420
			 0x0c700000,                                                  // Leading 1s:                                 /// 00424
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00428
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0042c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00434
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e000003,                                                  // Trailing 1s:                                /// 0043c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00440
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x00011111,                                                  // 9.7958E-41                                  /// 00448
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0044c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00450
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0xbf028f5c,                                                  // -0.51                                       /// 00464
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00468
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00470
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00484
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00488
			 0x80000000,                                                  // -zero                                       /// 0048c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00498
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x55555555,                                                  // 4 random values                             /// 004b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xff800000,                                                  // -inf                                        /// 004cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00500
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00504
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0050c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0051c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00520
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00524
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0e000003,                                                  // Trailing 1s:                                /// 0052c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00534
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00538
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00544
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00548
			 0x00000000,                                                  // zero                                        /// 0054c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00558
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0055c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00560
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00564
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00568
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0056c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00570
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00574
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0057c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00580
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00584
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0058c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00590
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00594
			 0x33333333,                                                  // 4 random values                             /// 00598
			 0x0e000007,                                                  // Trailing 1s:                                /// 0059c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e8
			 0x3f028f5c,                                                  // 0.51                                        /// 005ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00600
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00604
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00000000,                                                  // zero                                        /// 0061c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00620
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00628
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0062c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00638
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0063c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00644
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0064c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00654
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00658
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00660
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0066c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00670
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00678
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00680
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00684
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0068c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00698
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0069c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006cc
			 0xcb000000,                                                  // -8388608.0                                  /// 006d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00700
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0070c
			 0x33333333,                                                  // 4 random values                             /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00720
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00724
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00728
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0072c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00730
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00734
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00738
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0073c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00744
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00748
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0074c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0c400000,                                                  // Leading 1s:                                 /// 00758
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0075c
			 0x00000000,                                                  // zero                                        /// 00760
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00764
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0076c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00770
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00774
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00780
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00788
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0078c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x0e000001,                                                  // Trailing 1s:                                /// 00798
			 0xcb000000,                                                  // -8388608.0                                  /// 0079c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0xff800000,                                                  // -inf                                        /// 007a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x33333333,                                                  // 4 random values                             /// 007c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x4b000000,                                                  // 8388608.0                                   /// 007ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f0
			 0x55555555,                                                  // 4 random values                             /// 007f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00800
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00804
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00814
			 0x0c400000,                                                  // Leading 1s:                                 /// 00818
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00824
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00828
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0082c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00830
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00834
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00838
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0083c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00844
			 0x00000000,                                                  // zero                                        /// 00848
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0084c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00850
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00854
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0085c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00860
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00864
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00868
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0086c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00870
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00878
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0087c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00880
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00884
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0088c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0089c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x4b000000,                                                  // 8388608.0                                   /// 008c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008dc
			 0x7f800000,                                                  // inf                                         /// 008e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00900
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00908
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00914
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00918
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c400000,                                                  // Leading 1s:                                 /// 00924
			 0xff800000,                                                  // -inf                                        /// 00928
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0092c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00930
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00934
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0093c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00944
			 0x0c400000,                                                  // Leading 1s:                                 /// 00948
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00950
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0095c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00960
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00968
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x33333333,                                                  // 4 random values                             /// 00974
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00978
			 0x00011111,                                                  // 9.7958E-41                                  /// 0097c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00980
			 0x0e000003,                                                  // Trailing 1s:                                /// 00984
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0098c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x7f800000,                                                  // inf                                         /// 009a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c0
			 0xff800000,                                                  // -inf                                        /// 009c4
			 0xff800000,                                                  // -inf                                        /// 009c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009dc
			 0x00000000,                                                  // zero                                        /// 009e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7fc00001,                                                  // signaling NaN                               /// 00a2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aa0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x7fc00001,                                                  // signaling NaN                               /// 00aa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ac0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ad4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00afc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b54
			 0xcb000000,                                                  // -8388608.0                                  /// 00b58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80000000,                                                  // -zero                                       /// 00b70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b84
			 0xbf028f5c,                                                  // -0.51                                       /// 00b88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bdc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00be8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bfc
			 0xcb000000,                                                  // -8388608.0                                  /// 00c00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x33333333,                                                  // 4 random values                             /// 00c1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c20
			 0x3f028f5c,                                                  // 0.51                                        /// 00c24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xff800000,                                                  // -inf                                        /// 00c78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0xff800000,                                                  // -inf                                        /// 00c90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ccc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x33333333,                                                  // 4 random values                             /// 00d14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d34
			 0x7fc00001,                                                  // signaling NaN                               /// 00d38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0xffc00001,                                                  // -signaling NaN                              /// 00d68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xcb000000,                                                  // -8388608.0                                  /// 00d84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00da0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00da4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00db8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00dc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dcc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ddc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de4
			 0x4b000000,                                                  // 8388608.0                                   /// 00de8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00df8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x4b000000,                                                  // 8388608.0                                   /// 00e2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ea4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ebc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ec4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec8
			 0x33333333,                                                  // 4 random values                             /// 00ecc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x00000000,                                                  // zero                                        /// 00ee0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ef8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f7c
			 0x00000000,                                                  // zero                                        /// 00f80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xcb000000,                                                  // -8388608.0                                  /// 00f8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fb0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fb4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fe8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff8
			 0x0c7ffe00                                                  // Leading 1s:                                 /// last
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
			 0x00000430,
			 0x000006c4,
			 0x0000066c,
			 0x00000288,
			 0x00000798,
			 0x00000430,
			 0x00000398,
			 0x00000474,

			 /// vpu register f2
			 0x41a00000,
			 0x41100000,
			 0x41d00000,
			 0x41500000,
			 0x42000000,
			 0x40a00000,
			 0x40000000,
			 0x41700000,

			 /// vpu register f3
			 0x41200000,
			 0x41900000,
			 0x41700000,
			 0x41b80000,
			 0x41f80000,
			 0x41300000,
			 0x41200000,
			 0x41800000,

			 /// vpu register f4
			 0x41400000,
			 0x41e80000,
			 0x41b00000,
			 0x41c80000,
			 0x40c00000,
			 0x40e00000,
			 0x42000000,
			 0x41100000,

			 /// vpu register f5
			 0x41000000,
			 0x41a80000,
			 0x41980000,
			 0x41880000,
			 0x41a00000,
			 0x41980000,
			 0x41700000,
			 0x41d80000,

			 /// vpu register f6
			 0x41c00000,
			 0x41300000,
			 0x41a80000,
			 0x40c00000,
			 0x41c80000,
			 0x41b80000,
			 0x41900000,
			 0x40800000,

			 /// vpu register f7
			 0x41f00000,
			 0x41d00000,
			 0x41400000,
			 0x40400000,
			 0x41800000,
			 0x41c80000,
			 0x41700000,
			 0x41400000,

			 /// vpu register f8
			 0x41000000,
			 0x41a00000,
			 0x41800000,
			 0x41f80000,
			 0x41d00000,
			 0x41700000,
			 0x41980000,
			 0x41400000,

			 /// vpu register f9
			 0x41e00000,
			 0x41500000,
			 0x40400000,
			 0x41a00000,
			 0x41900000,
			 0x41f80000,
			 0x42000000,
			 0x41c00000,

			 /// vpu register f10
			 0x41980000,
			 0x41880000,
			 0x41980000,
			 0x41b00000,
			 0x41000000,
			 0x41c80000,
			 0x41d00000,
			 0x41600000,

			 /// vpu register f11
			 0x41880000,
			 0x41a00000,
			 0x41100000,
			 0x41200000,
			 0x41200000,
			 0x41800000,
			 0x41f00000,
			 0x41500000,

			 /// vpu register f12
			 0x41a80000,
			 0x41800000,
			 0x41f80000,
			 0x41f00000,
			 0x40c00000,
			 0x41f00000,
			 0x41d00000,
			 0x40a00000,

			 /// vpu register f13
			 0x41b80000,
			 0x41980000,
			 0x40800000,
			 0x41f80000,
			 0x41980000,
			 0x41a00000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f14
			 0x41c00000,
			 0x41900000,
			 0x40400000,
			 0x41a00000,
			 0x41c80000,
			 0x41b00000,
			 0x41700000,
			 0x41880000,

			 /// vpu register f15
			 0x41900000,
			 0x41b80000,
			 0x41700000,
			 0x41a00000,
			 0x41e00000,
			 0x41a80000,
			 0x40e00000,
			 0x41880000,

			 /// vpu register f16
			 0x41a00000,
			 0x41500000,
			 0x41f00000,
			 0x41b80000,
			 0x41100000,
			 0x41a80000,
			 0x41a80000,
			 0x3f800000,

			 /// vpu register f17
			 0x41500000,
			 0x41f80000,
			 0x40000000,
			 0x41e80000,
			 0x3f800000,
			 0x41c80000,
			 0x41700000,
			 0x41f80000,

			 /// vpu register f18
			 0x41800000,
			 0x40400000,
			 0x41f80000,
			 0x40e00000,
			 0x41d00000,
			 0x41300000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f19
			 0x41b00000,
			 0x41e00000,
			 0x41600000,
			 0x40800000,
			 0x42000000,
			 0x41600000,
			 0x41a00000,
			 0x41d80000,

			 /// vpu register f20
			 0x41f00000,
			 0x41980000,
			 0x41c80000,
			 0x40400000,
			 0x41700000,
			 0x40a00000,
			 0x41e80000,
			 0x42000000,

			 /// vpu register f21
			 0x41300000,
			 0x41c80000,
			 0x41300000,
			 0x41f00000,
			 0x41d80000,
			 0x41f00000,
			 0x41200000,
			 0x40000000,

			 /// vpu register f22
			 0x41800000,
			 0x40800000,
			 0x40c00000,
			 0x41c00000,
			 0x40400000,
			 0x40800000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f23
			 0x41980000,
			 0x41c80000,
			 0x41300000,
			 0x41c00000,
			 0x41a80000,
			 0x40400000,
			 0x41f80000,
			 0x41e80000,

			 /// vpu register f24
			 0x40400000,
			 0x41880000,
			 0x41f80000,
			 0x41980000,
			 0x41300000,
			 0x40000000,
			 0x41c00000,
			 0x40000000,

			 /// vpu register f25
			 0x41300000,
			 0x41b80000,
			 0x40400000,
			 0x42000000,
			 0x40800000,
			 0x41900000,
			 0x41100000,
			 0x41b80000,

			 /// vpu register f26
			 0x41000000,
			 0x40a00000,
			 0x40400000,
			 0x40400000,
			 0x41b00000,
			 0x41d00000,
			 0x40c00000,
			 0x41f80000,

			 /// vpu register f27
			 0x42000000,
			 0x41c80000,
			 0x3f800000,
			 0x41500000,
			 0x41980000,
			 0x41300000,
			 0x41880000,
			 0x41500000,

			 /// vpu register f28
			 0x40400000,
			 0x41700000,
			 0x41980000,
			 0x41e00000,
			 0x41300000,
			 0x41d80000,
			 0x41d80000,
			 0x41c80000,

			 /// vpu register f29
			 0x41000000,
			 0x41d80000,
			 0x41a00000,
			 0x41b80000,
			 0x40400000,
			 0x41600000,
			 0x41200000,
			 0x41c00000,

			 /// vpu register f30
			 0x41100000,
			 0x41f80000,
			 0x41400000,
			 0x40e00000,
			 0x41600000,
			 0x41a80000,
			 0x41980000,
			 0x41300000,

			 /// vpu register f31
			 0x41300000,
			 0x41c80000,
			 0x41300000,
			 0x41d80000,
			 0x41200000,
			 0x41100000,
			 0x42000000,
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
		"fghg.ps f12, f1 (x11)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f19, f1 (x14)\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f29, f1 (x15)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f11, f1 (x13)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f22, f1 (x15)\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f13, f1 (x12)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f14, f1 (x13)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f2, f1 (x15)\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f8, f1 (x15)\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f26, f1 (x13)\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x14)\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f29, f1 (x12)\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f28, f1 (x11)\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x12)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f11, f1 (x13)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f19, f1 (x12)\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f13, f1 (x11)\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f29, f1 (x14)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f24, f1 (x13)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f26, f1 (x11)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x15)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f9, f1 (x14)\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x15)\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f18, f1 (x14)\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f29, f1 (x13)\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x14)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f30, f1 (x11)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f22, f1 (x15)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f27, f1 (x15)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f24, f1 (x11)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f18, f1 (x13)\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x14)\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x11)\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f8, f1 (x14)\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f16, f1 (x14)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f2, f1 (x14)\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f14, f1 (x15)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f25, f1 (x12)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f3, f1 (x14)\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f13, f1 (x12)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f13, f1 (x15)\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f30, f1 (x15)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f14, f1 (x14)\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x11)\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f30, f1 (x15)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f10, f1 (x13)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f25, f1 (x11)\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f18, f1 (x12)\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f10, f1 (x15)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f6, f1 (x14)\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f22, f1 (x13)\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f11, f1 (x15)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x13)\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f29, f1 (x11)\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f24, f1 (x15)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f29, f1 (x11)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f14, f1 (x11)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x11)\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f25, f1 (x13)\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f7, f1 (x14)\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f2, f1 (x12)\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x12)\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f28, f1 (x15)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f8, f1 (x11)\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x13)\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f30, f1 (x13)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f22, f1 (x11)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x12)\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f11, f1 (x14)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f28, f1 (x13)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f6, f1 (x11)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f7, f1 (x15)\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f27, f1 (x15)\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f6, f1 (x14)\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f24, f1 (x11)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f6, f1 (x11)\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f4, f1 (x15)\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f23, f1 (x15)\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f14, f1 (x14)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f20, f1 (x14)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f22, f1 (x15)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f14, f1 (x15)\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f4, f1 (x12)\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x12)\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x13)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x11)\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f19, f1 (x14)\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f10, f1 (x13)\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f22, f1 (x11)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x13)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x15)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f10, f1 (x11)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f12, f1 (x12)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f30, f1 (x14)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f11, f1 (x14)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f17, f1 (x14)\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x13)\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f13, f1 (x12)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghg.ps f5, f1 (x15)\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
