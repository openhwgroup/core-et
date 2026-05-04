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
			 0x80f50829, /// 0x0
			 0x95f1afe7, /// 0x4
			 0x113cf7e3, /// 0x8
			 0xfc435edd, /// 0xc
			 0x4e6137f6, /// 0x10
			 0xbcba33f6, /// 0x14
			 0xe7f7bdbd, /// 0x18
			 0xc99352b4, /// 0x1c
			 0x7d690424, /// 0x20
			 0x1ebe3035, /// 0x24
			 0xa43e5724, /// 0x28
			 0x7ae2d62d, /// 0x2c
			 0xb2de5e4b, /// 0x30
			 0xd15ae696, /// 0x34
			 0x83cb82ef, /// 0x38
			 0x9ac33184, /// 0x3c
			 0x844362db, /// 0x40
			 0x1d680741, /// 0x44
			 0x04131804, /// 0x48
			 0x9ab3b417, /// 0x4c
			 0x0ba3a640, /// 0x50
			 0x33b2b376, /// 0x54
			 0x37b51bc0, /// 0x58
			 0xe5fa8ee8, /// 0x5c
			 0xed616266, /// 0x60
			 0xec0a8c7b, /// 0x64
			 0xb5496443, /// 0x68
			 0x96ded5ae, /// 0x6c
			 0xa7eb052e, /// 0x70
			 0xdc9546c9, /// 0x74
			 0x7550aa8e, /// 0x78
			 0x0ff83670, /// 0x7c
			 0x166f3f94, /// 0x80
			 0xe9790c0d, /// 0x84
			 0x76027d82, /// 0x88
			 0x3cb84277, /// 0x8c
			 0x2e2cb54c, /// 0x90
			 0xe26924c9, /// 0x94
			 0xfa71be2b, /// 0x98
			 0xf77c482b, /// 0x9c
			 0x3220f702, /// 0xa0
			 0xf3943afb, /// 0xa4
			 0x6a49a737, /// 0xa8
			 0x9565ec37, /// 0xac
			 0xfd6e5fad, /// 0xb0
			 0x41c22c11, /// 0xb4
			 0xe1649e20, /// 0xb8
			 0x300426bc, /// 0xbc
			 0xb5fc0fed, /// 0xc0
			 0x635a55b1, /// 0xc4
			 0x515e817c, /// 0xc8
			 0xf526f734, /// 0xcc
			 0x5f4ac850, /// 0xd0
			 0x24b2cd38, /// 0xd4
			 0xdfc15e69, /// 0xd8
			 0x082efa1b, /// 0xdc
			 0x03123874, /// 0xe0
			 0x63072905, /// 0xe4
			 0xad8c42d6, /// 0xe8
			 0xa1e54920, /// 0xec
			 0x4fe7fc24, /// 0xf0
			 0xca865e8b, /// 0xf4
			 0xe45bd73c, /// 0xf8
			 0xc821f309, /// 0xfc
			 0xa8b557c7, /// 0x100
			 0x6c0dee86, /// 0x104
			 0x9bc36989, /// 0x108
			 0x2fccfe02, /// 0x10c
			 0xdb2937ff, /// 0x110
			 0xc1cb1575, /// 0x114
			 0x296d68d1, /// 0x118
			 0xe0e29ec5, /// 0x11c
			 0x856a4eef, /// 0x120
			 0x9a4f1f8e, /// 0x124
			 0x5b6ac251, /// 0x128
			 0x1b4faea4, /// 0x12c
			 0x60d2cd09, /// 0x130
			 0x1551b883, /// 0x134
			 0x79c90fc5, /// 0x138
			 0x93adf90d, /// 0x13c
			 0x0d048d5a, /// 0x140
			 0xc8b34691, /// 0x144
			 0xd20d8120, /// 0x148
			 0x39adea2b, /// 0x14c
			 0x2c6da132, /// 0x150
			 0x32778be6, /// 0x154
			 0x3f8e92e8, /// 0x158
			 0xce75f6fc, /// 0x15c
			 0x731092dc, /// 0x160
			 0xac81314f, /// 0x164
			 0xdd9b8749, /// 0x168
			 0x6c8aaeb0, /// 0x16c
			 0x0a346ce5, /// 0x170
			 0x2e04d4cd, /// 0x174
			 0xbad774ab, /// 0x178
			 0xd8a31bd7, /// 0x17c
			 0xf06a8866, /// 0x180
			 0x568dee10, /// 0x184
			 0xeb157da0, /// 0x188
			 0x6d7b1674, /// 0x18c
			 0x954fecbf, /// 0x190
			 0x63d3504a, /// 0x194
			 0x29686c39, /// 0x198
			 0xde0dd4bb, /// 0x19c
			 0x2a40bf00, /// 0x1a0
			 0xa48bdc4b, /// 0x1a4
			 0xe4758dc2, /// 0x1a8
			 0x7f5af9f5, /// 0x1ac
			 0xad083895, /// 0x1b0
			 0x61b03a60, /// 0x1b4
			 0x553bdabb, /// 0x1b8
			 0x390458ec, /// 0x1bc
			 0x7dbf82f8, /// 0x1c0
			 0x4cd276d8, /// 0x1c4
			 0xc453bce3, /// 0x1c8
			 0x2499e63e, /// 0x1cc
			 0xcb747b3b, /// 0x1d0
			 0xa6d51c37, /// 0x1d4
			 0x497ac9f2, /// 0x1d8
			 0x82bcddf9, /// 0x1dc
			 0x988bf376, /// 0x1e0
			 0x34b6894a, /// 0x1e4
			 0x430a3a1f, /// 0x1e8
			 0x36dbc01f, /// 0x1ec
			 0x82120b40, /// 0x1f0
			 0x220d62a0, /// 0x1f4
			 0x2fc93fd6, /// 0x1f8
			 0x772ed268, /// 0x1fc
			 0x0928ad48, /// 0x200
			 0xe33d2d16, /// 0x204
			 0x23ee7281, /// 0x208
			 0x1681a5e4, /// 0x20c
			 0xcdccbc8d, /// 0x210
			 0x362f0fe3, /// 0x214
			 0x279e2a1c, /// 0x218
			 0x71d43639, /// 0x21c
			 0x7ae81295, /// 0x220
			 0x21aab52c, /// 0x224
			 0x2215ec2b, /// 0x228
			 0xed9ba166, /// 0x22c
			 0xfa589fbd, /// 0x230
			 0xd73e6f5c, /// 0x234
			 0x0e78bfdb, /// 0x238
			 0xfbc35d44, /// 0x23c
			 0xea7e134a, /// 0x240
			 0xff8a77d6, /// 0x244
			 0x473d739c, /// 0x248
			 0xe6932870, /// 0x24c
			 0x67921fab, /// 0x250
			 0x51c458fc, /// 0x254
			 0x1d16228d, /// 0x258
			 0x766ed9a9, /// 0x25c
			 0x70545770, /// 0x260
			 0x6ff3133b, /// 0x264
			 0x9582dc4e, /// 0x268
			 0x6ad21878, /// 0x26c
			 0xdfb79fa9, /// 0x270
			 0xf7c1a3b1, /// 0x274
			 0x05d0576b, /// 0x278
			 0x108853c8, /// 0x27c
			 0xd4b99f59, /// 0x280
			 0x3ab935b2, /// 0x284
			 0x31166547, /// 0x288
			 0x7ca76ddf, /// 0x28c
			 0xc5b281aa, /// 0x290
			 0xa29385a3, /// 0x294
			 0xe0b1929c, /// 0x298
			 0x4b8e9d19, /// 0x29c
			 0x21c6026a, /// 0x2a0
			 0x6969da94, /// 0x2a4
			 0x9b1672ca, /// 0x2a8
			 0x02a4f621, /// 0x2ac
			 0x1dee9683, /// 0x2b0
			 0xdc80d736, /// 0x2b4
			 0xaf469cca, /// 0x2b8
			 0xf6f290c5, /// 0x2bc
			 0x237ac70a, /// 0x2c0
			 0x2064f682, /// 0x2c4
			 0x7e1d55ca, /// 0x2c8
			 0x74ed4e4f, /// 0x2cc
			 0xe6dc4f99, /// 0x2d0
			 0x4f30df05, /// 0x2d4
			 0x8560182e, /// 0x2d8
			 0x36e2fdae, /// 0x2dc
			 0x56bf2780, /// 0x2e0
			 0x1a9d3976, /// 0x2e4
			 0xd3803ec2, /// 0x2e8
			 0x16d76847, /// 0x2ec
			 0xe7b44333, /// 0x2f0
			 0x4d627715, /// 0x2f4
			 0x4dc8dd6f, /// 0x2f8
			 0x82f8c5f2, /// 0x2fc
			 0xcb263359, /// 0x300
			 0xeb079d66, /// 0x304
			 0xc15bcb40, /// 0x308
			 0xc9604018, /// 0x30c
			 0x5c386a19, /// 0x310
			 0xb9dae741, /// 0x314
			 0xfb5b38ae, /// 0x318
			 0xe3d7ba33, /// 0x31c
			 0x0fefa9ae, /// 0x320
			 0xbfc9955f, /// 0x324
			 0xd922774f, /// 0x328
			 0xa15c9d9f, /// 0x32c
			 0xa55f1b42, /// 0x330
			 0x13162f40, /// 0x334
			 0x0973d353, /// 0x338
			 0xbb2e23bd, /// 0x33c
			 0x8bc46152, /// 0x340
			 0x5a941fb3, /// 0x344
			 0x4d6550df, /// 0x348
			 0xe0182ae8, /// 0x34c
			 0x1abdd919, /// 0x350
			 0x23412bec, /// 0x354
			 0xd5951e1d, /// 0x358
			 0x72339697, /// 0x35c
			 0x97c3759b, /// 0x360
			 0xe723aa0c, /// 0x364
			 0x85c21e61, /// 0x368
			 0x4031d1c3, /// 0x36c
			 0x82b3012d, /// 0x370
			 0xa299192b, /// 0x374
			 0xc934154d, /// 0x378
			 0xcb6d0115, /// 0x37c
			 0xe175d7f1, /// 0x380
			 0x6dcc7dcc, /// 0x384
			 0xdea3572b, /// 0x388
			 0x35b5d6a1, /// 0x38c
			 0x54d606d8, /// 0x390
			 0x1a718dc5, /// 0x394
			 0xcb0f9f0a, /// 0x398
			 0xcbca5cd0, /// 0x39c
			 0xe622b495, /// 0x3a0
			 0x91545d6d, /// 0x3a4
			 0xb663941c, /// 0x3a8
			 0x9af8e8b0, /// 0x3ac
			 0x0dd9c446, /// 0x3b0
			 0x717850a8, /// 0x3b4
			 0xa158be10, /// 0x3b8
			 0x51ce62f3, /// 0x3bc
			 0xda6cea10, /// 0x3c0
			 0x132ca097, /// 0x3c4
			 0x83ecf287, /// 0x3c8
			 0x4fe221a2, /// 0x3cc
			 0xfdcc7ca3, /// 0x3d0
			 0xb3d45e26, /// 0x3d4
			 0xd63897aa, /// 0x3d8
			 0x562403ae, /// 0x3dc
			 0x29a2b4cc, /// 0x3e0
			 0xb29865e3, /// 0x3e4
			 0xea0e478a, /// 0x3e8
			 0x7cb0dc38, /// 0x3ec
			 0xcdebf64b, /// 0x3f0
			 0x2eca131c, /// 0x3f4
			 0xad905e26, /// 0x3f8
			 0xb820a0f2, /// 0x3fc
			 0x2b19d21c, /// 0x400
			 0x4b133057, /// 0x404
			 0xd1928e92, /// 0x408
			 0xe51a6fcc, /// 0x40c
			 0xfd3b488f, /// 0x410
			 0x9886329c, /// 0x414
			 0x9ac0b2b3, /// 0x418
			 0x75f3371d, /// 0x41c
			 0x45801f94, /// 0x420
			 0xe8798583, /// 0x424
			 0xcd4198a5, /// 0x428
			 0xd2ed66d3, /// 0x42c
			 0x0146ecae, /// 0x430
			 0xa03a0a35, /// 0x434
			 0x86f0fb1d, /// 0x438
			 0x84b0aa67, /// 0x43c
			 0xd776aa93, /// 0x440
			 0x0422ca0b, /// 0x444
			 0x27513d91, /// 0x448
			 0xc854903f, /// 0x44c
			 0xbc2322d8, /// 0x450
			 0x6faae3a0, /// 0x454
			 0xaf36f818, /// 0x458
			 0xe3adc408, /// 0x45c
			 0x5d4c66a8, /// 0x460
			 0x08416654, /// 0x464
			 0xba5b1eb4, /// 0x468
			 0xf995e383, /// 0x46c
			 0x0a4e8a1e, /// 0x470
			 0x2768c650, /// 0x474
			 0x797a8ff0, /// 0x478
			 0xa9c9d815, /// 0x47c
			 0x0d64103f, /// 0x480
			 0x76c3a4cf, /// 0x484
			 0xd8ffc003, /// 0x488
			 0xcd1c5378, /// 0x48c
			 0xe448a9b4, /// 0x490
			 0x4a956e5d, /// 0x494
			 0xdbe84c93, /// 0x498
			 0xe5519a2a, /// 0x49c
			 0x41868cff, /// 0x4a0
			 0x7a776c9e, /// 0x4a4
			 0x219c64dc, /// 0x4a8
			 0x173b4df8, /// 0x4ac
			 0x5910dcfb, /// 0x4b0
			 0x32f187d4, /// 0x4b4
			 0x55310332, /// 0x4b8
			 0x9baea4ec, /// 0x4bc
			 0x3dced465, /// 0x4c0
			 0xb12b65df, /// 0x4c4
			 0x06f38ac4, /// 0x4c8
			 0x385c3234, /// 0x4cc
			 0xa726edc6, /// 0x4d0
			 0x15460780, /// 0x4d4
			 0xbdb550e0, /// 0x4d8
			 0x7e5be447, /// 0x4dc
			 0x7d51018c, /// 0x4e0
			 0xa6d70aa1, /// 0x4e4
			 0x0427e47b, /// 0x4e8
			 0x7b127ae8, /// 0x4ec
			 0xb9bb791e, /// 0x4f0
			 0x7496b3e9, /// 0x4f4
			 0x024efcb0, /// 0x4f8
			 0xb6c45b2b, /// 0x4fc
			 0x225bf721, /// 0x500
			 0x052bdf15, /// 0x504
			 0x742df0c0, /// 0x508
			 0x07cc9bee, /// 0x50c
			 0x70c4a5a9, /// 0x510
			 0xfdec5813, /// 0x514
			 0xeca4e2ec, /// 0x518
			 0xdd3075f2, /// 0x51c
			 0x1b1824bb, /// 0x520
			 0x29318005, /// 0x524
			 0x5d993176, /// 0x528
			 0x30a76487, /// 0x52c
			 0x17d37762, /// 0x530
			 0x8128c63e, /// 0x534
			 0xcdbf5430, /// 0x538
			 0xea6a41c5, /// 0x53c
			 0x64d8241c, /// 0x540
			 0xe3c464fc, /// 0x544
			 0xd3e43d4f, /// 0x548
			 0x99aa4204, /// 0x54c
			 0xf05fa5a5, /// 0x550
			 0xb3a6fc35, /// 0x554
			 0x1d880a6e, /// 0x558
			 0x75bd0f8d, /// 0x55c
			 0xc9a4a677, /// 0x560
			 0x9274b452, /// 0x564
			 0xd0ef0b83, /// 0x568
			 0x411a0bb2, /// 0x56c
			 0x0fb4a8ad, /// 0x570
			 0x77417ab6, /// 0x574
			 0x09161549, /// 0x578
			 0x0fc896b3, /// 0x57c
			 0xbd56adf1, /// 0x580
			 0x1aa13396, /// 0x584
			 0x4a766d30, /// 0x588
			 0x95e9c86e, /// 0x58c
			 0xf62cbac3, /// 0x590
			 0xc8615972, /// 0x594
			 0x11069e87, /// 0x598
			 0x6ef028e5, /// 0x59c
			 0x5dd3ac33, /// 0x5a0
			 0xf6470b75, /// 0x5a4
			 0x18ac459b, /// 0x5a8
			 0x2ad9b700, /// 0x5ac
			 0x566f5de8, /// 0x5b0
			 0x9df075bd, /// 0x5b4
			 0xa3413268, /// 0x5b8
			 0xe4fdbbe4, /// 0x5bc
			 0xc9d6c573, /// 0x5c0
			 0x9384c040, /// 0x5c4
			 0xa9962eb5, /// 0x5c8
			 0x1345671d, /// 0x5cc
			 0xe6c8e9cc, /// 0x5d0
			 0x01ab059f, /// 0x5d4
			 0xdccfe035, /// 0x5d8
			 0xde5dc719, /// 0x5dc
			 0x3f4bbf30, /// 0x5e0
			 0xd3aad9c0, /// 0x5e4
			 0xb9636da0, /// 0x5e8
			 0x82b1746d, /// 0x5ec
			 0x9519bb5d, /// 0x5f0
			 0xa9d4d9b8, /// 0x5f4
			 0x0338313c, /// 0x5f8
			 0x55c07e6c, /// 0x5fc
			 0xffefa88e, /// 0x600
			 0xf0b4fe54, /// 0x604
			 0x92d320b5, /// 0x608
			 0x99ce2753, /// 0x60c
			 0xd9df7cc1, /// 0x610
			 0x2448b728, /// 0x614
			 0x869baaf4, /// 0x618
			 0xd416c012, /// 0x61c
			 0xf5465ced, /// 0x620
			 0xd2ca7391, /// 0x624
			 0x70cdc397, /// 0x628
			 0x16747964, /// 0x62c
			 0xbf808cd4, /// 0x630
			 0x27a20b50, /// 0x634
			 0x4c9319d9, /// 0x638
			 0x994f71eb, /// 0x63c
			 0x8a79e9ac, /// 0x640
			 0x94e14f59, /// 0x644
			 0xde35918e, /// 0x648
			 0x67db957a, /// 0x64c
			 0x99f2ffd2, /// 0x650
			 0x9467e9e0, /// 0x654
			 0xff93ed37, /// 0x658
			 0x9873d88d, /// 0x65c
			 0x7f7d6346, /// 0x660
			 0x3b52bd3a, /// 0x664
			 0xdc13da16, /// 0x668
			 0x14d1459c, /// 0x66c
			 0x7eaf2641, /// 0x670
			 0x530d8a37, /// 0x674
			 0x5f822343, /// 0x678
			 0xca4c5e17, /// 0x67c
			 0xffdc4a27, /// 0x680
			 0x7e0fba18, /// 0x684
			 0xf88274bd, /// 0x688
			 0xc47f49f4, /// 0x68c
			 0xb517bf33, /// 0x690
			 0x913b79a8, /// 0x694
			 0xa164450b, /// 0x698
			 0xde78ce9e, /// 0x69c
			 0xf7ce3a77, /// 0x6a0
			 0x02c1d0a9, /// 0x6a4
			 0x3a1206b4, /// 0x6a8
			 0x081067f3, /// 0x6ac
			 0xcd9f0d2c, /// 0x6b0
			 0xd20432b3, /// 0x6b4
			 0xb9ff7535, /// 0x6b8
			 0xa98e8f7b, /// 0x6bc
			 0x015aa14f, /// 0x6c0
			 0xc16a36eb, /// 0x6c4
			 0x07d2ee22, /// 0x6c8
			 0xb4ea4d47, /// 0x6cc
			 0x36a33cae, /// 0x6d0
			 0x4740c79a, /// 0x6d4
			 0xbcd46f37, /// 0x6d8
			 0x5490523f, /// 0x6dc
			 0x02f724ce, /// 0x6e0
			 0x9ee49924, /// 0x6e4
			 0x9f451cb5, /// 0x6e8
			 0x7555866b, /// 0x6ec
			 0xf072911f, /// 0x6f0
			 0xe9af3ca4, /// 0x6f4
			 0xc3a1678b, /// 0x6f8
			 0x00e45662, /// 0x6fc
			 0x77fc29e0, /// 0x700
			 0xa79b9ca2, /// 0x704
			 0xdac3d0ce, /// 0x708
			 0xb6b86b88, /// 0x70c
			 0x480aa13c, /// 0x710
			 0x32e0df54, /// 0x714
			 0x9f854dae, /// 0x718
			 0x0e997d7a, /// 0x71c
			 0xda8192ac, /// 0x720
			 0x03635963, /// 0x724
			 0x821ccfe0, /// 0x728
			 0xb7cdbb63, /// 0x72c
			 0x584b64be, /// 0x730
			 0xbb8a8bb6, /// 0x734
			 0x277b738c, /// 0x738
			 0x3c2487d0, /// 0x73c
			 0xd65bd696, /// 0x740
			 0xbdd9fb96, /// 0x744
			 0x18d6f075, /// 0x748
			 0xeb989d1e, /// 0x74c
			 0xdb269b5c, /// 0x750
			 0x35052caa, /// 0x754
			 0x412b9acf, /// 0x758
			 0x14463595, /// 0x75c
			 0x3e5ca83c, /// 0x760
			 0x35468dbb, /// 0x764
			 0x8a8a6e38, /// 0x768
			 0x0a4bdb6f, /// 0x76c
			 0x9e5ef55f, /// 0x770
			 0x25d62cd7, /// 0x774
			 0x5b5119ce, /// 0x778
			 0xd0b72bb9, /// 0x77c
			 0x2065f1b7, /// 0x780
			 0x1b598fff, /// 0x784
			 0x54f587bf, /// 0x788
			 0x170494bb, /// 0x78c
			 0x1c589b2f, /// 0x790
			 0x3f2b1b20, /// 0x794
			 0xeb943a4c, /// 0x798
			 0x922172b8, /// 0x79c
			 0xa5fa0ca3, /// 0x7a0
			 0x136fc3a4, /// 0x7a4
			 0x650ec3a4, /// 0x7a8
			 0x338cae1a, /// 0x7ac
			 0xefb95878, /// 0x7b0
			 0x3c815170, /// 0x7b4
			 0x28ed9a3a, /// 0x7b8
			 0xd302b2d2, /// 0x7bc
			 0xb26ddd54, /// 0x7c0
			 0x3a839e2a, /// 0x7c4
			 0xb6fc0338, /// 0x7c8
			 0x7c8fceb1, /// 0x7cc
			 0x9b89b710, /// 0x7d0
			 0x1a2b1fca, /// 0x7d4
			 0x7ea76946, /// 0x7d8
			 0xaa9cc7f9, /// 0x7dc
			 0xe8d6425c, /// 0x7e0
			 0x7b6952a0, /// 0x7e4
			 0x0a8af144, /// 0x7e8
			 0xb3248f44, /// 0x7ec
			 0x5fd5a740, /// 0x7f0
			 0x442338cc, /// 0x7f4
			 0x2db3c7d7, /// 0x7f8
			 0x175b6967, /// 0x7fc
			 0xf2e6a130, /// 0x800
			 0x2cc4b481, /// 0x804
			 0x03ec4029, /// 0x808
			 0xe889b307, /// 0x80c
			 0x3c018392, /// 0x810
			 0x487430e6, /// 0x814
			 0xa2b0149e, /// 0x818
			 0xd90a27b8, /// 0x81c
			 0x9e0cb794, /// 0x820
			 0x33c19046, /// 0x824
			 0x0ab77aba, /// 0x828
			 0x4428edf4, /// 0x82c
			 0x5bc4756f, /// 0x830
			 0x35f78d28, /// 0x834
			 0xa227a3ba, /// 0x838
			 0x5a071fdb, /// 0x83c
			 0xba75e187, /// 0x840
			 0x86f8f75a, /// 0x844
			 0x11ffefd0, /// 0x848
			 0x79bd682f, /// 0x84c
			 0x019a5e89, /// 0x850
			 0x70cc928c, /// 0x854
			 0x837826d6, /// 0x858
			 0x8216993c, /// 0x85c
			 0xd8115334, /// 0x860
			 0x7e70c5e0, /// 0x864
			 0x4e11fef3, /// 0x868
			 0x1127f649, /// 0x86c
			 0x5f277e15, /// 0x870
			 0x3a4587a7, /// 0x874
			 0x917c88a2, /// 0x878
			 0xc79fdad6, /// 0x87c
			 0x83d37cf9, /// 0x880
			 0x281b1695, /// 0x884
			 0xa819166d, /// 0x888
			 0x80dd890a, /// 0x88c
			 0x95b1858b, /// 0x890
			 0x49a808ca, /// 0x894
			 0x13881771, /// 0x898
			 0x44f79851, /// 0x89c
			 0x6f8a8659, /// 0x8a0
			 0x26c64d7c, /// 0x8a4
			 0x2c767201, /// 0x8a8
			 0x46718a89, /// 0x8ac
			 0x3e28e714, /// 0x8b0
			 0x80a89ecc, /// 0x8b4
			 0x746ce825, /// 0x8b8
			 0xa857ffa4, /// 0x8bc
			 0x5a8233c9, /// 0x8c0
			 0x3c207a7b, /// 0x8c4
			 0xaa923e15, /// 0x8c8
			 0x9dbad249, /// 0x8cc
			 0x45a6729a, /// 0x8d0
			 0x831df87b, /// 0x8d4
			 0x5a6bc3be, /// 0x8d8
			 0x0212103b, /// 0x8dc
			 0x1d72cd98, /// 0x8e0
			 0x848fd283, /// 0x8e4
			 0x75a75a5b, /// 0x8e8
			 0x69327501, /// 0x8ec
			 0xa95cc0cf, /// 0x8f0
			 0x4579444d, /// 0x8f4
			 0x598f3c7e, /// 0x8f8
			 0x1797fefc, /// 0x8fc
			 0x615a6a39, /// 0x900
			 0x71dedb08, /// 0x904
			 0x9cac79c4, /// 0x908
			 0xa32712a6, /// 0x90c
			 0x15325821, /// 0x910
			 0x4db4ae9f, /// 0x914
			 0x3dc4bfcd, /// 0x918
			 0x4483f5ff, /// 0x91c
			 0x72411db2, /// 0x920
			 0x766c304c, /// 0x924
			 0xdc65b45f, /// 0x928
			 0x5cb9d97f, /// 0x92c
			 0x4b378113, /// 0x930
			 0xb4cc7dc0, /// 0x934
			 0x53664de2, /// 0x938
			 0x2f7fb3ca, /// 0x93c
			 0x75766a76, /// 0x940
			 0x0aa52bd1, /// 0x944
			 0xa4a23a40, /// 0x948
			 0xc5d72994, /// 0x94c
			 0xc0fb4e5d, /// 0x950
			 0xb71e7959, /// 0x954
			 0xb9558bd4, /// 0x958
			 0x8eb12cee, /// 0x95c
			 0xcc9ff8b0, /// 0x960
			 0x438aa520, /// 0x964
			 0x1417010a, /// 0x968
			 0x4e0b7f4b, /// 0x96c
			 0x98868303, /// 0x970
			 0x6c336f3b, /// 0x974
			 0xf439612f, /// 0x978
			 0x09142360, /// 0x97c
			 0x523c46c2, /// 0x980
			 0xf1139807, /// 0x984
			 0x388620ad, /// 0x988
			 0x043da9e0, /// 0x98c
			 0x3015eb9e, /// 0x990
			 0xf5d1cad4, /// 0x994
			 0xb8c302c8, /// 0x998
			 0x639579fa, /// 0x99c
			 0xf50527e9, /// 0x9a0
			 0x084218e6, /// 0x9a4
			 0x5c0cdb84, /// 0x9a8
			 0x0ba98767, /// 0x9ac
			 0x188c2be0, /// 0x9b0
			 0xe64a3fc0, /// 0x9b4
			 0xa58d2783, /// 0x9b8
			 0xbb25c339, /// 0x9bc
			 0x6a7718f7, /// 0x9c0
			 0x4ef4d04b, /// 0x9c4
			 0xcd08ecef, /// 0x9c8
			 0x48eda1bc, /// 0x9cc
			 0x8aea802f, /// 0x9d0
			 0xfd507a37, /// 0x9d4
			 0x14e0a26d, /// 0x9d8
			 0x38d3afa5, /// 0x9dc
			 0xdfa78b61, /// 0x9e0
			 0xf073ff82, /// 0x9e4
			 0x88944bff, /// 0x9e8
			 0x268b9bc3, /// 0x9ec
			 0xba43b23d, /// 0x9f0
			 0x1da0372f, /// 0x9f4
			 0x6401cbfb, /// 0x9f8
			 0x48170d3c, /// 0x9fc
			 0x3015f54c, /// 0xa00
			 0xca89e76d, /// 0xa04
			 0xecb6a363, /// 0xa08
			 0x117bfab6, /// 0xa0c
			 0x47f00a84, /// 0xa10
			 0xfd7049b4, /// 0xa14
			 0x730a21f4, /// 0xa18
			 0x4bf15355, /// 0xa1c
			 0xc4b5f090, /// 0xa20
			 0x75c56b0e, /// 0xa24
			 0x11225825, /// 0xa28
			 0xa5acfbbc, /// 0xa2c
			 0x54a2993d, /// 0xa30
			 0xedc0f743, /// 0xa34
			 0x4185ee1f, /// 0xa38
			 0xd1719117, /// 0xa3c
			 0xd1ef6f60, /// 0xa40
			 0xa017bb83, /// 0xa44
			 0xba7166f2, /// 0xa48
			 0x4c6895fc, /// 0xa4c
			 0x5953c18e, /// 0xa50
			 0x03735803, /// 0xa54
			 0xc656ab93, /// 0xa58
			 0x18862873, /// 0xa5c
			 0x1241faa6, /// 0xa60
			 0x2637a683, /// 0xa64
			 0x567d9b8a, /// 0xa68
			 0xd62fcf97, /// 0xa6c
			 0x9183922d, /// 0xa70
			 0x2b83652f, /// 0xa74
			 0x3f795cef, /// 0xa78
			 0x785a4615, /// 0xa7c
			 0x69003f22, /// 0xa80
			 0xba5bc7e0, /// 0xa84
			 0x21357f6c, /// 0xa88
			 0x5102c125, /// 0xa8c
			 0x8f8f37d4, /// 0xa90
			 0xbd669466, /// 0xa94
			 0x48709dd0, /// 0xa98
			 0xec9071d2, /// 0xa9c
			 0x0bbfbecd, /// 0xaa0
			 0x0d418a80, /// 0xaa4
			 0x62d11cea, /// 0xaa8
			 0x3eecbf8a, /// 0xaac
			 0x16694941, /// 0xab0
			 0xab4a3550, /// 0xab4
			 0xecd037e2, /// 0xab8
			 0xb2f7fee8, /// 0xabc
			 0x1ff99d23, /// 0xac0
			 0x1a00dc02, /// 0xac4
			 0x81009001, /// 0xac8
			 0x9ff9f2ce, /// 0xacc
			 0xa44f044c, /// 0xad0
			 0x14b1f6a5, /// 0xad4
			 0x05fd0c05, /// 0xad8
			 0xd90ef194, /// 0xadc
			 0x3a5d1729, /// 0xae0
			 0xadf2db5b, /// 0xae4
			 0x830c7262, /// 0xae8
			 0xa794bd97, /// 0xaec
			 0x74c744d6, /// 0xaf0
			 0x03a07c27, /// 0xaf4
			 0x38a08f9d, /// 0xaf8
			 0xa23c5734, /// 0xafc
			 0xa42ed8dc, /// 0xb00
			 0x6c760f1b, /// 0xb04
			 0x118c0651, /// 0xb08
			 0x3411cfcc, /// 0xb0c
			 0xfb3bce9c, /// 0xb10
			 0x6dd48cf3, /// 0xb14
			 0x1fd707b5, /// 0xb18
			 0x5a320bdc, /// 0xb1c
			 0xaaa29592, /// 0xb20
			 0xf3458335, /// 0xb24
			 0x8c7f68e2, /// 0xb28
			 0x023f64a6, /// 0xb2c
			 0xda87ddbe, /// 0xb30
			 0xa5253286, /// 0xb34
			 0x0a36f987, /// 0xb38
			 0x072ad92f, /// 0xb3c
			 0x17a7ffd9, /// 0xb40
			 0xa71bc908, /// 0xb44
			 0x93d42c7c, /// 0xb48
			 0x0da2b45d, /// 0xb4c
			 0xe98aa2e6, /// 0xb50
			 0x5b7f9839, /// 0xb54
			 0xad96a64f, /// 0xb58
			 0x7e51f383, /// 0xb5c
			 0x4a9cdf42, /// 0xb60
			 0x0270b707, /// 0xb64
			 0x3ffeaa70, /// 0xb68
			 0xe6830a84, /// 0xb6c
			 0xfe95f010, /// 0xb70
			 0xfbecee35, /// 0xb74
			 0xb1cdf2a7, /// 0xb78
			 0xa2b4b44e, /// 0xb7c
			 0x9c8f0a20, /// 0xb80
			 0xf7d92a8f, /// 0xb84
			 0xe723df9c, /// 0xb88
			 0xdb9a836b, /// 0xb8c
			 0x2a15b62b, /// 0xb90
			 0xe680fc5d, /// 0xb94
			 0x1a6707f2, /// 0xb98
			 0xb9f29c3a, /// 0xb9c
			 0x11f1eabf, /// 0xba0
			 0xec517268, /// 0xba4
			 0xdc87fefa, /// 0xba8
			 0x6636a9d5, /// 0xbac
			 0x12d29d11, /// 0xbb0
			 0x4203fda7, /// 0xbb4
			 0xaf9f59cb, /// 0xbb8
			 0xea045321, /// 0xbbc
			 0x58809ec3, /// 0xbc0
			 0xc6e36519, /// 0xbc4
			 0x4a5ed5f6, /// 0xbc8
			 0xe9b540d1, /// 0xbcc
			 0x02c2fc40, /// 0xbd0
			 0x28426177, /// 0xbd4
			 0xc57eada9, /// 0xbd8
			 0x4137d8e6, /// 0xbdc
			 0x3bd50e8b, /// 0xbe0
			 0x7562267d, /// 0xbe4
			 0x8cdc59ac, /// 0xbe8
			 0x14cfcb0e, /// 0xbec
			 0xd87ce8dd, /// 0xbf0
			 0x0e9880df, /// 0xbf4
			 0x5b1ea64f, /// 0xbf8
			 0xdc37e179, /// 0xbfc
			 0xf1416001, /// 0xc00
			 0xc7bf93ba, /// 0xc04
			 0x8318ebd5, /// 0xc08
			 0x6b7cad86, /// 0xc0c
			 0xb1761228, /// 0xc10
			 0xcfd0b440, /// 0xc14
			 0x574faf88, /// 0xc18
			 0xd1d782e0, /// 0xc1c
			 0x5679c11a, /// 0xc20
			 0x5e6ebd47, /// 0xc24
			 0x7efdf45a, /// 0xc28
			 0xf56dac7c, /// 0xc2c
			 0x91e52add, /// 0xc30
			 0x9134b75d, /// 0xc34
			 0x20e324fd, /// 0xc38
			 0xd378dbb6, /// 0xc3c
			 0xc1449cf1, /// 0xc40
			 0x6695249d, /// 0xc44
			 0xcdcdcf69, /// 0xc48
			 0x755e46d5, /// 0xc4c
			 0xdad20dad, /// 0xc50
			 0x80fd82cb, /// 0xc54
			 0x9f848007, /// 0xc58
			 0xa81366d3, /// 0xc5c
			 0x3cfe63ee, /// 0xc60
			 0x1f127e2c, /// 0xc64
			 0x0c129fb1, /// 0xc68
			 0x8f05760e, /// 0xc6c
			 0xc2f598cf, /// 0xc70
			 0xe303556e, /// 0xc74
			 0xa4800c46, /// 0xc78
			 0x4a1951c2, /// 0xc7c
			 0xc7caf7ca, /// 0xc80
			 0x450c1091, /// 0xc84
			 0x00c796cc, /// 0xc88
			 0x15815ffc, /// 0xc8c
			 0x3e0c00f9, /// 0xc90
			 0x1b72a0fd, /// 0xc94
			 0x2a1bef04, /// 0xc98
			 0x5056522a, /// 0xc9c
			 0x837e3a7b, /// 0xca0
			 0x3bdfd072, /// 0xca4
			 0xcdb314ff, /// 0xca8
			 0xe1c364ff, /// 0xcac
			 0x7f74af37, /// 0xcb0
			 0x777e13bb, /// 0xcb4
			 0x14fb9a3f, /// 0xcb8
			 0x75e06973, /// 0xcbc
			 0x6f73a08d, /// 0xcc0
			 0x7199a3c4, /// 0xcc4
			 0x5898cc16, /// 0xcc8
			 0x6eb6e022, /// 0xccc
			 0x9bdea763, /// 0xcd0
			 0x8e690b85, /// 0xcd4
			 0x6b3075b0, /// 0xcd8
			 0x57100284, /// 0xcdc
			 0x3eec916f, /// 0xce0
			 0x9972bc67, /// 0xce4
			 0x4ddfc38f, /// 0xce8
			 0xa31677a0, /// 0xcec
			 0x9dc66322, /// 0xcf0
			 0x3bcc5ba7, /// 0xcf4
			 0x8245d100, /// 0xcf8
			 0x36887129, /// 0xcfc
			 0xc468de14, /// 0xd00
			 0x57491348, /// 0xd04
			 0x2c64693c, /// 0xd08
			 0x8c7eaa6e, /// 0xd0c
			 0x41901053, /// 0xd10
			 0xd833c12d, /// 0xd14
			 0xe0ab47cc, /// 0xd18
			 0x359c15a8, /// 0xd1c
			 0x90c3aff9, /// 0xd20
			 0xd4353d23, /// 0xd24
			 0xef9f2b03, /// 0xd28
			 0x9378f467, /// 0xd2c
			 0xdcbe0154, /// 0xd30
			 0xff88ea54, /// 0xd34
			 0x53e3a8db, /// 0xd38
			 0x014ccada, /// 0xd3c
			 0x03b76b41, /// 0xd40
			 0x805c69c2, /// 0xd44
			 0x61c8bfae, /// 0xd48
			 0x017f7d94, /// 0xd4c
			 0xa3bff2e3, /// 0xd50
			 0x1b12d4f0, /// 0xd54
			 0x07dca249, /// 0xd58
			 0xbc955c95, /// 0xd5c
			 0x61552ab7, /// 0xd60
			 0x5f3c3cb5, /// 0xd64
			 0x93b6ba4e, /// 0xd68
			 0x2ca41987, /// 0xd6c
			 0x3f22cc67, /// 0xd70
			 0x19b4c1e9, /// 0xd74
			 0xf76983a9, /// 0xd78
			 0xf3957ae1, /// 0xd7c
			 0x7b2fabf4, /// 0xd80
			 0xb838f57a, /// 0xd84
			 0x6c919fa9, /// 0xd88
			 0x7e06f692, /// 0xd8c
			 0x738c1b21, /// 0xd90
			 0x6bade70b, /// 0xd94
			 0x90cde9d4, /// 0xd98
			 0x77560778, /// 0xd9c
			 0x88067f53, /// 0xda0
			 0x016bfea7, /// 0xda4
			 0x7b4bd9d2, /// 0xda8
			 0xb042e986, /// 0xdac
			 0xb3194bf7, /// 0xdb0
			 0x96e21324, /// 0xdb4
			 0xe623eee0, /// 0xdb8
			 0x85ce89bf, /// 0xdbc
			 0x6292483a, /// 0xdc0
			 0xe707f46f, /// 0xdc4
			 0x50254126, /// 0xdc8
			 0x5a571691, /// 0xdcc
			 0xbf98c82b, /// 0xdd0
			 0xa9ef02f3, /// 0xdd4
			 0x20a37979, /// 0xdd8
			 0xead456da, /// 0xddc
			 0x694b4bcb, /// 0xde0
			 0x9ade4606, /// 0xde4
			 0x0ad5c02c, /// 0xde8
			 0x96e1c300, /// 0xdec
			 0x210d40dc, /// 0xdf0
			 0xeffa0b69, /// 0xdf4
			 0xceaf9c08, /// 0xdf8
			 0x979d92db, /// 0xdfc
			 0x95356d25, /// 0xe00
			 0x0b678816, /// 0xe04
			 0xeaeae71f, /// 0xe08
			 0x68a960f3, /// 0xe0c
			 0xed64b5bb, /// 0xe10
			 0xe6ec1aa1, /// 0xe14
			 0x0ade3477, /// 0xe18
			 0x0e26fed0, /// 0xe1c
			 0x0ed03381, /// 0xe20
			 0xee7b1aba, /// 0xe24
			 0xef934abb, /// 0xe28
			 0x15fb4801, /// 0xe2c
			 0x16284339, /// 0xe30
			 0x15867f1b, /// 0xe34
			 0x1c8b6ceb, /// 0xe38
			 0x0f6eaa23, /// 0xe3c
			 0xbe2cea08, /// 0xe40
			 0x8a2b63f4, /// 0xe44
			 0x67c5c971, /// 0xe48
			 0x20d99907, /// 0xe4c
			 0x3ff0a887, /// 0xe50
			 0xb0438566, /// 0xe54
			 0xaddefde6, /// 0xe58
			 0xb141e6bd, /// 0xe5c
			 0x0d600cd9, /// 0xe60
			 0x942eb6fe, /// 0xe64
			 0x84e73d61, /// 0xe68
			 0x66cf1e09, /// 0xe6c
			 0x73b883eb, /// 0xe70
			 0x0d35d939, /// 0xe74
			 0x3081de9d, /// 0xe78
			 0x41eec04a, /// 0xe7c
			 0x3280c0d7, /// 0xe80
			 0x616ec68b, /// 0xe84
			 0x298f1bd8, /// 0xe88
			 0x1b704bca, /// 0xe8c
			 0xcc54c6e2, /// 0xe90
			 0x849bfdc0, /// 0xe94
			 0xff6d65da, /// 0xe98
			 0xb78b9374, /// 0xe9c
			 0xaff22e1b, /// 0xea0
			 0xb7c86d67, /// 0xea4
			 0x2e9bd3e3, /// 0xea8
			 0x04bef8a0, /// 0xeac
			 0x371d01f6, /// 0xeb0
			 0x38b7f4c6, /// 0xeb4
			 0xb64c5194, /// 0xeb8
			 0x8adc75ff, /// 0xebc
			 0x6aff7609, /// 0xec0
			 0xe8d57e14, /// 0xec4
			 0xbd164ebb, /// 0xec8
			 0x93f1d753, /// 0xecc
			 0xdaa35725, /// 0xed0
			 0x851fda6d, /// 0xed4
			 0xaf4b49bb, /// 0xed8
			 0x941a5dd1, /// 0xedc
			 0xd1b90b69, /// 0xee0
			 0x0bab208f, /// 0xee4
			 0x18c56102, /// 0xee8
			 0xaf97c41e, /// 0xeec
			 0xa905471c, /// 0xef0
			 0x376b461b, /// 0xef4
			 0xce88d77d, /// 0xef8
			 0x04630e47, /// 0xefc
			 0x1358bad0, /// 0xf00
			 0x9a106913, /// 0xf04
			 0xb188e03d, /// 0xf08
			 0xe16e1807, /// 0xf0c
			 0x7ea20278, /// 0xf10
			 0x3a8a89fc, /// 0xf14
			 0xa6ade0e3, /// 0xf18
			 0x43046dd5, /// 0xf1c
			 0x0a34d536, /// 0xf20
			 0x944987d4, /// 0xf24
			 0x99931d7b, /// 0xf28
			 0x53f73890, /// 0xf2c
			 0x2725e378, /// 0xf30
			 0x7a5334f6, /// 0xf34
			 0x11b171ce, /// 0xf38
			 0x333d0384, /// 0xf3c
			 0x4094a271, /// 0xf40
			 0x4e07cd30, /// 0xf44
			 0x05cfca98, /// 0xf48
			 0x9a676a1c, /// 0xf4c
			 0x15926337, /// 0xf50
			 0x6941a133, /// 0xf54
			 0x493c4955, /// 0xf58
			 0x420c3e49, /// 0xf5c
			 0x77a9d4a8, /// 0xf60
			 0x8cc0cb20, /// 0xf64
			 0xb2b5a4ad, /// 0xf68
			 0xa1e2da8b, /// 0xf6c
			 0x5225d8e6, /// 0xf70
			 0xd1ca156c, /// 0xf74
			 0xe73cf561, /// 0xf78
			 0xf3016ed8, /// 0xf7c
			 0x7ea7ea13, /// 0xf80
			 0x5998c30b, /// 0xf84
			 0x21cfebd0, /// 0xf88
			 0x3831cabc, /// 0xf8c
			 0xff4ec506, /// 0xf90
			 0xef83ac53, /// 0xf94
			 0xe9b04be6, /// 0xf98
			 0xa50853e9, /// 0xf9c
			 0x203de853, /// 0xfa0
			 0x5a86c375, /// 0xfa4
			 0x4a0f1850, /// 0xfa8
			 0xb8ebfd04, /// 0xfac
			 0x822d3dec, /// 0xfb0
			 0xd40bec4f, /// 0xfb4
			 0x5e512065, /// 0xfb8
			 0xcc59cc07, /// 0xfbc
			 0x45f2e85c, /// 0xfc0
			 0x4f3b48d3, /// 0xfc4
			 0xfa1a2e36, /// 0xfc8
			 0x54686270, /// 0xfcc
			 0xbf8b92ee, /// 0xfd0
			 0x7e3e16e3, /// 0xfd4
			 0x32bfd215, /// 0xfd8
			 0x4c345cd4, /// 0xfdc
			 0x24d1a34f, /// 0xfe0
			 0xa5ae27e2, /// 0xfe4
			 0x7e6f1c38, /// 0xfe8
			 0xeab69181, /// 0xfec
			 0xf7433c8c, /// 0xff0
			 0x5a5173b1, /// 0xff4
			 0x77b9ad7e, /// 0xff8
			 0xe5022002 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00008
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0000c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00010
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0c700000,                                                  // Leading 1s:                                 /// 00018
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00020
			 0x55555555,                                                  // 4 random values                             /// 00024
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00028
			 0x33333333,                                                  // 4 random values                             /// 0002c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0003c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00044
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00050
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00058
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00060
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00064
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00068
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00070
			 0x7fc00001,                                                  // signaling NaN                               /// 00074
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00078
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00088
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xff800000,                                                  // -inf                                        /// 00090
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00094
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00098
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0009c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00108
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0010c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00110
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00114
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00118
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0011c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00120
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00124
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00134
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00138
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0013c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00140
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00148
			 0x0e000007,                                                  // Trailing 1s:                                /// 0014c
			 0x4b000000,                                                  // 8388608.0                                   /// 00150
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00154
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00158
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00170
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00174
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00178
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0017c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00180
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000000,                                                  // -zero                                       /// 00190
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0019c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0xcb000000,                                                  // -8388608.0                                  /// 00204
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00208
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0020c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00210
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0021c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00220
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00224
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00228
			 0x55555555,                                                  // 4 random values                             /// 0022c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00230
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00234
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00238
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0023c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00244
			 0x0c780000,                                                  // Leading 1s:                                 /// 00248
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0024c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00258
			 0x00011111,                                                  // 9.7958E-41                                  /// 0025c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x3f028f5c,                                                  // 0.51                                        /// 00264
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00270
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00274
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00278
			 0x33333333,                                                  // 4 random values                             /// 0027c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00280
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00284
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0028c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00290
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00294
			 0x0e000001,                                                  // Trailing 1s:                                /// 00298
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0029c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00304
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00308
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0030c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0xff800000,                                                  // -inf                                        /// 00314
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00320
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00324
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00328
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00334
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00338
			 0x3f028f5c,                                                  // 0.51                                        /// 0033c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00340
			 0x0e000007,                                                  // Trailing 1s:                                /// 00344
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00350
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00354
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00358
			 0x0c400000,                                                  // Leading 1s:                                 /// 0035c
			 0x3f028f5c,                                                  // 0.51                                        /// 00360
			 0xbf028f5c,                                                  // -0.51                                       /// 00364
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00368
			 0xffc00001,                                                  // -signaling NaN                              /// 0036c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00374
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00378
			 0xffc00001,                                                  // -signaling NaN                              /// 0037c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00380
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00388
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0038c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0e000003,                                                  // Trailing 1s:                                /// 0039c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x33333333,                                                  // 4 random values                             /// 003c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00404
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00408
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00410
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00418
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0xff800000,                                                  // -inf                                        /// 00424
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00428
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0042c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00434
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00438
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0043c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00444
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00448
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00450
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00454
			 0x33333333,                                                  // 4 random values                             /// 00458
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00460
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00464
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00468
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0046c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00470
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00478
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0047c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0048c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00011111,                                                  // 9.7958E-41                                  /// 00494
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00498
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0049c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0c700000,                                                  // Leading 1s:                                 /// 00504
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00514
			 0xbf028f5c,                                                  // -0.51                                       /// 00518
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00520
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00524
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00528
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0052c
			 0x3f028f5c,                                                  // 0.51                                        /// 00530
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00534
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00538
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00540
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00544
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00548
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00550
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00554
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00558
			 0xbf028f5c,                                                  // -0.51                                       /// 0055c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00588
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0058c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00590
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00594
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00598
			 0x0c400000,                                                  // Leading 1s:                                 /// 0059c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00600
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00604
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00608
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00610
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00614
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00618
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00620
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00624
			 0x00000000,                                                  // zero                                        /// 00628
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0062c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00638
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0063c
			 0xbf028f5c,                                                  // -0.51                                       /// 00640
			 0x0e000007,                                                  // Trailing 1s:                                /// 00644
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00648
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0064c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00650
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00654
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00658
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0065c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00664
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x0e000003,                                                  // Trailing 1s:                                /// 0066c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00670
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00678
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0067c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00684
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x3f028f5c,                                                  // 0.51                                        /// 0068c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00694
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00698
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0069c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0c780000,                                                  // Leading 1s:                                 /// 0070c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00710
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00714
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00718
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00720
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00724
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00730
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00738
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0073c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00744
			 0x00011111,                                                  // 9.7958E-41                                  /// 00748
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00754
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00758
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00760
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00764
			 0x7fc00001,                                                  // signaling NaN                               /// 00768
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00774
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x55555555,                                                  // 4 random values                             /// 0077c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00780
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00790
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00794
			 0x4b000000,                                                  // 8388608.0                                   /// 00798
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007cc
			 0x7fc00001,                                                  // signaling NaN                               /// 007d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00804
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00808
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0080c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00810
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00814
			 0x4b000000,                                                  // 8388608.0                                   /// 00818
			 0x3f028f5c,                                                  // 0.51                                        /// 0081c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00820
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00824
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00830
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00838
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x33333333,                                                  // 4 random values                             /// 00840
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0084c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00850
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00858
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0085c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00860
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00864
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0086c
			 0x00000000,                                                  // zero                                        /// 00870
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00874
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00878
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00888
			 0x7fc00001,                                                  // signaling NaN                               /// 0088c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00890
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00894
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00898
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0089c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x80000000,                                                  // -zero                                       /// 0090c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00910
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00914
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00918
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0091c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00920
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xff800000,                                                  // -inf                                        /// 00928
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00930
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00934
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00938
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00940
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00944
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00948
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00950
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00958
			 0x55555555,                                                  // 4 random values                             /// 0095c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00964
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00968
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00970
			 0x80000000,                                                  // -zero                                       /// 00974
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00980
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00984
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00990
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00994
			 0x0e000001,                                                  // Trailing 1s:                                /// 00998
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x3f028f5c,                                                  // 0.51                                        /// 009e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000000,                                                  // zero                                        /// 00a04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80000000,                                                  // -zero                                       /// 00a3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x55555555,                                                  // 4 random values                             /// 00a50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x7fc00001,                                                  // signaling NaN                               /// 00a58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a74
			 0x4b000000,                                                  // 8388608.0                                   /// 00a78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ab4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00abc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ac0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ac4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00adc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ae8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aec
			 0x55555555,                                                  // 4 random values                             /// 00af0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00af8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x7fc00001,                                                  // signaling NaN                               /// 00b6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0xbf028f5c,                                                  // -0.51                                       /// 00b98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ba0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd8
			 0x00000000,                                                  // zero                                        /// 00bdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00be0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c14
			 0x7f800000,                                                  // inf                                         /// 00c18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c30
			 0x7f800000,                                                  // inf                                         /// 00c34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c84
			 0x00000000,                                                  // zero                                        /// 00c88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ccc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cfc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d00
			 0x55555555,                                                  // 4 random values                             /// 00d04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x7f800000,                                                  // inf                                         /// 00d7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x00000000,                                                  // zero                                        /// 00d84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00da8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00db0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00db4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de0
			 0xff800000,                                                  // -inf                                        /// 00de4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xcb000000,                                                  // -8388608.0                                  /// 00e2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e54
			 0xcb000000,                                                  // -8388608.0                                  /// 00e58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x55555555,                                                  // 4 random values                             /// 00e7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ea0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ea8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ebc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ec0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ec4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ecc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ed0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ee0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ef8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00efc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f04
			 0x33333333,                                                  // 4 random values                             /// 00f08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f44
			 0xff800000,                                                  // -inf                                        /// 00f48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fb4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fbc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fcc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fe0
			 0x80000000,                                                  // -zero                                       /// 00fe4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0f7ffffc                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x54eb4288, /// 0x0
			 0x51ead01f, /// 0x4
			 0x6bd08187, /// 0x8
			 0x387bf787, /// 0xc
			 0xee903cea, /// 0x10
			 0xe3bd380e, /// 0x14
			 0x40c29d53, /// 0x18
			 0xa86d8a94, /// 0x1c
			 0xe93f4fa2, /// 0x20
			 0x6a791b2c, /// 0x24
			 0xb2100d93, /// 0x28
			 0x8b0a7b27, /// 0x2c
			 0xc9121248, /// 0x30
			 0x124139eb, /// 0x34
			 0xddfb3c5c, /// 0x38
			 0x81cc5355, /// 0x3c
			 0xde724f31, /// 0x40
			 0x78435a90, /// 0x44
			 0xe22467b7, /// 0x48
			 0xfc4ebefa, /// 0x4c
			 0x42f6c7fd, /// 0x50
			 0x87a05a4b, /// 0x54
			 0x265e60cf, /// 0x58
			 0x1663b9ea, /// 0x5c
			 0x08ca943f, /// 0x60
			 0x34dfc152, /// 0x64
			 0x50837a9c, /// 0x68
			 0x4aa51b8d, /// 0x6c
			 0xcfcc7a32, /// 0x70
			 0x5a2ec6aa, /// 0x74
			 0x8530a204, /// 0x78
			 0x92e89103, /// 0x7c
			 0x155c6666, /// 0x80
			 0x0e70a91e, /// 0x84
			 0xc9e334df, /// 0x88
			 0xf869c986, /// 0x8c
			 0x685dafbf, /// 0x90
			 0xc495b509, /// 0x94
			 0xf4ffeb77, /// 0x98
			 0x02f1d3ab, /// 0x9c
			 0x21b05fc1, /// 0xa0
			 0xcd64e2ff, /// 0xa4
			 0xf3464e9d, /// 0xa8
			 0x6dcb99b6, /// 0xac
			 0xe1334467, /// 0xb0
			 0x031f0f42, /// 0xb4
			 0xf5585051, /// 0xb8
			 0xb1ebc769, /// 0xbc
			 0xd6f9d6cc, /// 0xc0
			 0x26d64df8, /// 0xc4
			 0xdff21b08, /// 0xc8
			 0xdc7fce2f, /// 0xcc
			 0x63c4e784, /// 0xd0
			 0x544faa8a, /// 0xd4
			 0x14d73934, /// 0xd8
			 0x2a39ffa2, /// 0xdc
			 0xc85edf9f, /// 0xe0
			 0x6bbb252e, /// 0xe4
			 0xa436cc7d, /// 0xe8
			 0x3724e144, /// 0xec
			 0x406cde6b, /// 0xf0
			 0x5a9db68d, /// 0xf4
			 0x100aa153, /// 0xf8
			 0xb1ad3fa3, /// 0xfc
			 0x19ec1f59, /// 0x100
			 0x8eb7cf80, /// 0x104
			 0x416afb31, /// 0x108
			 0x437a18d2, /// 0x10c
			 0x8c3e0635, /// 0x110
			 0x55458cb0, /// 0x114
			 0xa4cf3307, /// 0x118
			 0x5c0167a2, /// 0x11c
			 0xe172150b, /// 0x120
			 0xe912f69a, /// 0x124
			 0x6bfca153, /// 0x128
			 0xf382181b, /// 0x12c
			 0x64a64c6e, /// 0x130
			 0xdef5e9fa, /// 0x134
			 0x89dcdc2d, /// 0x138
			 0x8f0804dd, /// 0x13c
			 0xf32bfb98, /// 0x140
			 0xfed287f4, /// 0x144
			 0x62dc4044, /// 0x148
			 0x9cec0b3b, /// 0x14c
			 0xb66b163e, /// 0x150
			 0x2525b1c6, /// 0x154
			 0xb4efd63b, /// 0x158
			 0x5e1e52b8, /// 0x15c
			 0x466260be, /// 0x160
			 0x9626ad36, /// 0x164
			 0xe325cfc0, /// 0x168
			 0x82596dca, /// 0x16c
			 0x97e84bd8, /// 0x170
			 0x8b8c9444, /// 0x174
			 0x2e6ec502, /// 0x178
			 0x53484161, /// 0x17c
			 0x30842ad9, /// 0x180
			 0x30b6ec1c, /// 0x184
			 0x0179e3c5, /// 0x188
			 0x630180b6, /// 0x18c
			 0xe2b7f707, /// 0x190
			 0xfaeeea22, /// 0x194
			 0x24981f0b, /// 0x198
			 0xa4100ad0, /// 0x19c
			 0xf5eb308e, /// 0x1a0
			 0xa3cf77c7, /// 0x1a4
			 0xc73a4661, /// 0x1a8
			 0x40ade557, /// 0x1ac
			 0x98e95325, /// 0x1b0
			 0xd9022e53, /// 0x1b4
			 0xb7e054d1, /// 0x1b8
			 0xb0561fdc, /// 0x1bc
			 0x0bd4ce8c, /// 0x1c0
			 0x684dae81, /// 0x1c4
			 0xd65aafec, /// 0x1c8
			 0xd6f198f3, /// 0x1cc
			 0x31191906, /// 0x1d0
			 0xb783b2bf, /// 0x1d4
			 0x956a0455, /// 0x1d8
			 0xee07c8a0, /// 0x1dc
			 0xb368097b, /// 0x1e0
			 0xe23b52b1, /// 0x1e4
			 0x0cfe6c91, /// 0x1e8
			 0xfa043f5c, /// 0x1ec
			 0xd5328a80, /// 0x1f0
			 0x711f18b7, /// 0x1f4
			 0xc3bf9110, /// 0x1f8
			 0xf62d7b0c, /// 0x1fc
			 0xfe04ae5f, /// 0x200
			 0xa1d3c09c, /// 0x204
			 0xe612711e, /// 0x208
			 0xc4f9dd62, /// 0x20c
			 0x79e66ca0, /// 0x210
			 0x5eba71ee, /// 0x214
			 0x583060fc, /// 0x218
			 0x49968355, /// 0x21c
			 0x2821d39d, /// 0x220
			 0x3d549e01, /// 0x224
			 0x75d62a91, /// 0x228
			 0x22dda874, /// 0x22c
			 0x0bd2a04f, /// 0x230
			 0x14f06d9f, /// 0x234
			 0xc71ede96, /// 0x238
			 0xfa40efa8, /// 0x23c
			 0x7ce446f7, /// 0x240
			 0x5a33d4e1, /// 0x244
			 0x1fdb8bb8, /// 0x248
			 0x0b398ca3, /// 0x24c
			 0x8f619484, /// 0x250
			 0x3880fded, /// 0x254
			 0x4921a785, /// 0x258
			 0x8f0d9b49, /// 0x25c
			 0x985c819e, /// 0x260
			 0x556c4571, /// 0x264
			 0xb0bfa27f, /// 0x268
			 0x4d85eede, /// 0x26c
			 0x3326cd7f, /// 0x270
			 0x3b91f858, /// 0x274
			 0x5da578a1, /// 0x278
			 0xc7e2db2a, /// 0x27c
			 0xa697dec4, /// 0x280
			 0x31db05ad, /// 0x284
			 0x05f6f715, /// 0x288
			 0xd68b100e, /// 0x28c
			 0xc44dd552, /// 0x290
			 0x94bed50b, /// 0x294
			 0xca458379, /// 0x298
			 0x6979e5ce, /// 0x29c
			 0xca0a0cf2, /// 0x2a0
			 0x035e06b8, /// 0x2a4
			 0x08eb0836, /// 0x2a8
			 0x8327e322, /// 0x2ac
			 0xc454988d, /// 0x2b0
			 0xa80b9ee0, /// 0x2b4
			 0xa5483883, /// 0x2b8
			 0xff93bafd, /// 0x2bc
			 0xe5d99381, /// 0x2c0
			 0x6011b445, /// 0x2c4
			 0x742e0743, /// 0x2c8
			 0xb4f01ea9, /// 0x2cc
			 0xf8c6e375, /// 0x2d0
			 0x6ba2bf7a, /// 0x2d4
			 0x190ddb28, /// 0x2d8
			 0x63dce083, /// 0x2dc
			 0x35c9e040, /// 0x2e0
			 0x4d6869a8, /// 0x2e4
			 0xa5b0143c, /// 0x2e8
			 0x524b43d0, /// 0x2ec
			 0x8dc99e23, /// 0x2f0
			 0x0ea927d3, /// 0x2f4
			 0x9dd7c3db, /// 0x2f8
			 0x496301fa, /// 0x2fc
			 0x3d3ff0d1, /// 0x300
			 0xf6081722, /// 0x304
			 0x632b0e1d, /// 0x308
			 0xf3edefbe, /// 0x30c
			 0x17a06844, /// 0x310
			 0x8aa12319, /// 0x314
			 0xfe2db232, /// 0x318
			 0x14f2c437, /// 0x31c
			 0x5a6f129d, /// 0x320
			 0x86ed6ca4, /// 0x324
			 0xb7355013, /// 0x328
			 0x1eea3b71, /// 0x32c
			 0x2b3965e4, /// 0x330
			 0x94e5190a, /// 0x334
			 0x9f683a74, /// 0x338
			 0xbf2f6514, /// 0x33c
			 0x501739ce, /// 0x340
			 0x4e72f31d, /// 0x344
			 0x28a56ab9, /// 0x348
			 0x6493ce61, /// 0x34c
			 0x848b175a, /// 0x350
			 0x34c9da21, /// 0x354
			 0x6e9dcf6c, /// 0x358
			 0x089bbd63, /// 0x35c
			 0x8a22f479, /// 0x360
			 0x5367c61e, /// 0x364
			 0xe7ba73dc, /// 0x368
			 0x5618a1e1, /// 0x36c
			 0x3fc1c0c6, /// 0x370
			 0xf8a9915c, /// 0x374
			 0xd1f8ce43, /// 0x378
			 0xa68a6156, /// 0x37c
			 0xc96b05d6, /// 0x380
			 0xb7adb19b, /// 0x384
			 0x3c3be551, /// 0x388
			 0x05341162, /// 0x38c
			 0xd7cf0041, /// 0x390
			 0x57aaa285, /// 0x394
			 0x1c9b433e, /// 0x398
			 0x817deffb, /// 0x39c
			 0xa92fb7da, /// 0x3a0
			 0xfbb84915, /// 0x3a4
			 0xbd9462eb, /// 0x3a8
			 0x909edb56, /// 0x3ac
			 0xda16f568, /// 0x3b0
			 0x0ea7fa74, /// 0x3b4
			 0x1c113156, /// 0x3b8
			 0xd3011c25, /// 0x3bc
			 0x6b382869, /// 0x3c0
			 0x8898b7d0, /// 0x3c4
			 0x579dece8, /// 0x3c8
			 0x650228f4, /// 0x3cc
			 0x07d869bf, /// 0x3d0
			 0xe9accf3c, /// 0x3d4
			 0xffb6f352, /// 0x3d8
			 0xfe3635f3, /// 0x3dc
			 0x82658c0c, /// 0x3e0
			 0xfeabcde4, /// 0x3e4
			 0x5c2ca8de, /// 0x3e8
			 0xec1a6584, /// 0x3ec
			 0xe4ea75c6, /// 0x3f0
			 0xa8057a9c, /// 0x3f4
			 0x1ecc476e, /// 0x3f8
			 0xd97896c4, /// 0x3fc
			 0x750e7431, /// 0x400
			 0xc10af4bf, /// 0x404
			 0xd8359914, /// 0x408
			 0x5238115c, /// 0x40c
			 0xa25524d5, /// 0x410
			 0x46c09071, /// 0x414
			 0x185138a3, /// 0x418
			 0x818a4c84, /// 0x41c
			 0x6db90512, /// 0x420
			 0xf85247bc, /// 0x424
			 0xf87c5bba, /// 0x428
			 0x80e2a5e7, /// 0x42c
			 0xe48e497c, /// 0x430
			 0x088e72d0, /// 0x434
			 0x46179048, /// 0x438
			 0x333bb9fe, /// 0x43c
			 0x76ec28d8, /// 0x440
			 0x7f85f611, /// 0x444
			 0xa26f0fe0, /// 0x448
			 0xd608dac1, /// 0x44c
			 0x8b1c6aee, /// 0x450
			 0x7b0ce7e8, /// 0x454
			 0x8eb4b584, /// 0x458
			 0xdf0cde61, /// 0x45c
			 0x1fc92bed, /// 0x460
			 0x2960b4cb, /// 0x464
			 0xfd04a46d, /// 0x468
			 0x2a4666ac, /// 0x46c
			 0xf9db0718, /// 0x470
			 0x06b6568e, /// 0x474
			 0x1a7edf13, /// 0x478
			 0x20be7079, /// 0x47c
			 0xf6ebe29b, /// 0x480
			 0x9c8e6462, /// 0x484
			 0x0afec3ed, /// 0x488
			 0x0dd90d76, /// 0x48c
			 0xbbc356c6, /// 0x490
			 0xa1f9b87f, /// 0x494
			 0xe42d7870, /// 0x498
			 0xd900868b, /// 0x49c
			 0x98e90223, /// 0x4a0
			 0xfa9630c9, /// 0x4a4
			 0xba41504d, /// 0x4a8
			 0xc9afc9e7, /// 0x4ac
			 0x1629c102, /// 0x4b0
			 0x7b930a6f, /// 0x4b4
			 0xfba7791c, /// 0x4b8
			 0x70da28af, /// 0x4bc
			 0x564b472a, /// 0x4c0
			 0xfdb7c96e, /// 0x4c4
			 0x60cb6333, /// 0x4c8
			 0xd1b8a522, /// 0x4cc
			 0x5be00984, /// 0x4d0
			 0xc1a3a8e5, /// 0x4d4
			 0x376d5eb5, /// 0x4d8
			 0xb2f21ee3, /// 0x4dc
			 0x2f02b3fd, /// 0x4e0
			 0xf1b70dfe, /// 0x4e4
			 0xf209138b, /// 0x4e8
			 0x53228841, /// 0x4ec
			 0x84aea384, /// 0x4f0
			 0x3c341ace, /// 0x4f4
			 0x4ec03507, /// 0x4f8
			 0x66c8bda2, /// 0x4fc
			 0x172b7fef, /// 0x500
			 0xbb270c4d, /// 0x504
			 0xd41a8722, /// 0x508
			 0x999a99f3, /// 0x50c
			 0xccd7e996, /// 0x510
			 0x6c8bdc3a, /// 0x514
			 0xf268ee0b, /// 0x518
			 0x5e603935, /// 0x51c
			 0xd8cf3bdf, /// 0x520
			 0xc69708de, /// 0x524
			 0xcb9d5c49, /// 0x528
			 0x5e50305f, /// 0x52c
			 0x055c8c14, /// 0x530
			 0x2889b9e9, /// 0x534
			 0xd0b8f963, /// 0x538
			 0xee55eca9, /// 0x53c
			 0x55ae311f, /// 0x540
			 0x8f7cf4e4, /// 0x544
			 0x690ff7be, /// 0x548
			 0x09d7b0b2, /// 0x54c
			 0x54b4702c, /// 0x550
			 0x14dd8e5e, /// 0x554
			 0x3c33a0cd, /// 0x558
			 0x285f5aed, /// 0x55c
			 0xdc1fd9ac, /// 0x560
			 0x51d54a50, /// 0x564
			 0xc19ede03, /// 0x568
			 0x0c44a884, /// 0x56c
			 0x1871d02c, /// 0x570
			 0xd3582349, /// 0x574
			 0x2b759f5f, /// 0x578
			 0xe746ccf0, /// 0x57c
			 0x97b5d6f0, /// 0x580
			 0xb5ab537e, /// 0x584
			 0xdc143d56, /// 0x588
			 0xdb30dfac, /// 0x58c
			 0xf2de2611, /// 0x590
			 0x1dbd11c4, /// 0x594
			 0xc3b19f66, /// 0x598
			 0xf3221483, /// 0x59c
			 0x5f5f12e5, /// 0x5a0
			 0x25c316b3, /// 0x5a4
			 0x58d73e09, /// 0x5a8
			 0x7d1d2618, /// 0x5ac
			 0xfebf113f, /// 0x5b0
			 0x3ac12caf, /// 0x5b4
			 0x180004db, /// 0x5b8
			 0x52b8cdaa, /// 0x5bc
			 0x9e0fa531, /// 0x5c0
			 0xa08b6505, /// 0x5c4
			 0xc736a3db, /// 0x5c8
			 0x360ca0c4, /// 0x5cc
			 0x60c94d0e, /// 0x5d0
			 0x5feab60b, /// 0x5d4
			 0xcdde3e89, /// 0x5d8
			 0x93f038a3, /// 0x5dc
			 0x30ebfd9b, /// 0x5e0
			 0x5e92a547, /// 0x5e4
			 0x3c513929, /// 0x5e8
			 0xe5176ff0, /// 0x5ec
			 0xbef60550, /// 0x5f0
			 0x37674458, /// 0x5f4
			 0xd552d3a8, /// 0x5f8
			 0xb6dc8b68, /// 0x5fc
			 0x7a8d6d29, /// 0x600
			 0xd90de39b, /// 0x604
			 0x0b07c210, /// 0x608
			 0x76c2af75, /// 0x60c
			 0xec545fbf, /// 0x610
			 0xac34a098, /// 0x614
			 0x70d19090, /// 0x618
			 0x02dc4e1f, /// 0x61c
			 0xe431aa73, /// 0x620
			 0x0a1b5123, /// 0x624
			 0xac29af89, /// 0x628
			 0x02270318, /// 0x62c
			 0xb8063b97, /// 0x630
			 0x55f89d9f, /// 0x634
			 0x8eb51358, /// 0x638
			 0x9db80b50, /// 0x63c
			 0x58394318, /// 0x640
			 0xc2a5d3e6, /// 0x644
			 0x575d17f1, /// 0x648
			 0x7cb767e0, /// 0x64c
			 0x5f7fb7e1, /// 0x650
			 0x83ce1523, /// 0x654
			 0x844c8ae4, /// 0x658
			 0x2138a46d, /// 0x65c
			 0xcded3e6e, /// 0x660
			 0xbce15051, /// 0x664
			 0xee5b4d7f, /// 0x668
			 0x0c3fb2b6, /// 0x66c
			 0xc5110eb0, /// 0x670
			 0x2e2ce014, /// 0x674
			 0xe52db1b4, /// 0x678
			 0xa709373e, /// 0x67c
			 0x2be434c1, /// 0x680
			 0xaa595028, /// 0x684
			 0x65a85f60, /// 0x688
			 0x79470bef, /// 0x68c
			 0x808ddcc3, /// 0x690
			 0x3cbca4b4, /// 0x694
			 0xb55a208a, /// 0x698
			 0xfe3d6561, /// 0x69c
			 0x8875e77f, /// 0x6a0
			 0xd19d45b0, /// 0x6a4
			 0x7f62fd01, /// 0x6a8
			 0xc0d1e33d, /// 0x6ac
			 0x5c19dda0, /// 0x6b0
			 0x81430f9e, /// 0x6b4
			 0x8278ae05, /// 0x6b8
			 0xf25ab92c, /// 0x6bc
			 0x891541d1, /// 0x6c0
			 0xde0040a1, /// 0x6c4
			 0xcff61341, /// 0x6c8
			 0xf57d067f, /// 0x6cc
			 0xd3f400d2, /// 0x6d0
			 0x4f54ada7, /// 0x6d4
			 0x0d1ad9e2, /// 0x6d8
			 0x637b3961, /// 0x6dc
			 0xc4fe8323, /// 0x6e0
			 0x38b47553, /// 0x6e4
			 0xf2f01dce, /// 0x6e8
			 0x9f3e7be0, /// 0x6ec
			 0x48054480, /// 0x6f0
			 0x891fef0c, /// 0x6f4
			 0xf666df11, /// 0x6f8
			 0xa4a6c278, /// 0x6fc
			 0x2bf86516, /// 0x700
			 0x3d79d17e, /// 0x704
			 0x666962aa, /// 0x708
			 0x8b3236de, /// 0x70c
			 0x3ce30fcc, /// 0x710
			 0x407fcaa8, /// 0x714
			 0xc2f98e3c, /// 0x718
			 0x50b50b5d, /// 0x71c
			 0xd005eb86, /// 0x720
			 0x8eba59a9, /// 0x724
			 0x280dc619, /// 0x728
			 0xb8b3eee1, /// 0x72c
			 0x84e24fa7, /// 0x730
			 0xd24d1556, /// 0x734
			 0x3f283ae5, /// 0x738
			 0xb8eca351, /// 0x73c
			 0x9aa03cd7, /// 0x740
			 0x69619ca0, /// 0x744
			 0x21979346, /// 0x748
			 0x6bb5b0f3, /// 0x74c
			 0xcde4afdf, /// 0x750
			 0x1ee639ed, /// 0x754
			 0x4c90bce3, /// 0x758
			 0x1bc84c5c, /// 0x75c
			 0x12c89c7b, /// 0x760
			 0x486de764, /// 0x764
			 0x27b4b85d, /// 0x768
			 0xe06d85f0, /// 0x76c
			 0x7b70e3f8, /// 0x770
			 0x8bb134ca, /// 0x774
			 0x9623ecd5, /// 0x778
			 0x4b2724ed, /// 0x77c
			 0x4c583754, /// 0x780
			 0xffbdd34c, /// 0x784
			 0x882fac53, /// 0x788
			 0x57621caf, /// 0x78c
			 0xab755e0f, /// 0x790
			 0x933427be, /// 0x794
			 0x599370dd, /// 0x798
			 0x1ff0eafc, /// 0x79c
			 0x32b5a61b, /// 0x7a0
			 0x85eddef6, /// 0x7a4
			 0x3b07fee4, /// 0x7a8
			 0x1900a48f, /// 0x7ac
			 0x33b9e987, /// 0x7b0
			 0x5b904901, /// 0x7b4
			 0x18600ca2, /// 0x7b8
			 0x965bf6a2, /// 0x7bc
			 0x78e8565c, /// 0x7c0
			 0x51576bc7, /// 0x7c4
			 0xd672c143, /// 0x7c8
			 0xde07c749, /// 0x7cc
			 0x96a9ca82, /// 0x7d0
			 0xb75e93cb, /// 0x7d4
			 0xa5e1c2b3, /// 0x7d8
			 0xa6988146, /// 0x7dc
			 0xe69d9906, /// 0x7e0
			 0x632f0ba8, /// 0x7e4
			 0x9e6f5e27, /// 0x7e8
			 0x40eabf85, /// 0x7ec
			 0xea6bc0dd, /// 0x7f0
			 0x0bc7f467, /// 0x7f4
			 0xc26fab59, /// 0x7f8
			 0xf8ffd4a5, /// 0x7fc
			 0x78c580e1, /// 0x800
			 0xd805ed2c, /// 0x804
			 0x11ffeba6, /// 0x808
			 0x08943333, /// 0x80c
			 0x9fc2f753, /// 0x810
			 0xdb453130, /// 0x814
			 0x63d2f933, /// 0x818
			 0xdd15ee13, /// 0x81c
			 0x4997bd01, /// 0x820
			 0xa33f5ba9, /// 0x824
			 0x83b6a90c, /// 0x828
			 0x414fd43b, /// 0x82c
			 0x3f6ebd82, /// 0x830
			 0x188fcbbb, /// 0x834
			 0x2e416210, /// 0x838
			 0x3394ac52, /// 0x83c
			 0x937d0097, /// 0x840
			 0x30a5fc24, /// 0x844
			 0x97541516, /// 0x848
			 0x5764411c, /// 0x84c
			 0xcbc2e5f0, /// 0x850
			 0x9bb7fecc, /// 0x854
			 0x89f831d9, /// 0x858
			 0x13a16d5f, /// 0x85c
			 0x8ff51351, /// 0x860
			 0xfb0ac353, /// 0x864
			 0xfcd51480, /// 0x868
			 0x040dd99c, /// 0x86c
			 0x824bd0d1, /// 0x870
			 0x0641d0ae, /// 0x874
			 0x9a90168c, /// 0x878
			 0xf6dee883, /// 0x87c
			 0x562fba54, /// 0x880
			 0x905dc3f3, /// 0x884
			 0xdd4f8e0c, /// 0x888
			 0x3467f5be, /// 0x88c
			 0xbfacea71, /// 0x890
			 0x9f4b7788, /// 0x894
			 0x736e876e, /// 0x898
			 0x9d25831b, /// 0x89c
			 0xd05e9553, /// 0x8a0
			 0x6a4befe1, /// 0x8a4
			 0xe760f4df, /// 0x8a8
			 0xe912db5c, /// 0x8ac
			 0x55ea0c2d, /// 0x8b0
			 0x8c1f1b43, /// 0x8b4
			 0x5005d61a, /// 0x8b8
			 0xa77371fa, /// 0x8bc
			 0x490fc793, /// 0x8c0
			 0xe9676d5a, /// 0x8c4
			 0xdf0e8fd7, /// 0x8c8
			 0x754fa1f2, /// 0x8cc
			 0xc9a47e85, /// 0x8d0
			 0xc4963cb5, /// 0x8d4
			 0x6dd5f69f, /// 0x8d8
			 0xcfb7e8e0, /// 0x8dc
			 0xbfe93b43, /// 0x8e0
			 0xd45f7cf8, /// 0x8e4
			 0x6347fb67, /// 0x8e8
			 0xdfe4483e, /// 0x8ec
			 0x4c068572, /// 0x8f0
			 0x925e5cb3, /// 0x8f4
			 0xda403436, /// 0x8f8
			 0x544b7e39, /// 0x8fc
			 0x7b1a6114, /// 0x900
			 0x39ab3a47, /// 0x904
			 0x610f193e, /// 0x908
			 0xdbc47154, /// 0x90c
			 0xd2e98dfe, /// 0x910
			 0xe0460eb7, /// 0x914
			 0xfe3b4083, /// 0x918
			 0xd6d1293c, /// 0x91c
			 0xbccfda38, /// 0x920
			 0x08361fd2, /// 0x924
			 0x75879ee1, /// 0x928
			 0x7363ce7d, /// 0x92c
			 0xd24bb09c, /// 0x930
			 0xae22a6d9, /// 0x934
			 0xa6688523, /// 0x938
			 0x676050ac, /// 0x93c
			 0xa7a63375, /// 0x940
			 0x2c1804c8, /// 0x944
			 0x63e3a555, /// 0x948
			 0x7a463956, /// 0x94c
			 0x43958250, /// 0x950
			 0xfebb85f9, /// 0x954
			 0xa37bfded, /// 0x958
			 0xd4d988a8, /// 0x95c
			 0xa7995c52, /// 0x960
			 0xa4ae9c6f, /// 0x964
			 0x81b795ea, /// 0x968
			 0xf3addd6b, /// 0x96c
			 0x064e5e93, /// 0x970
			 0x08de4215, /// 0x974
			 0x452e5927, /// 0x978
			 0x22a950d8, /// 0x97c
			 0x754db929, /// 0x980
			 0x606bf7db, /// 0x984
			 0xd559a905, /// 0x988
			 0xa20d87cf, /// 0x98c
			 0x9e024976, /// 0x990
			 0x5c2ddabe, /// 0x994
			 0x295202e1, /// 0x998
			 0x95432b0d, /// 0x99c
			 0x03377751, /// 0x9a0
			 0x5d24a9c4, /// 0x9a4
			 0x6c44777b, /// 0x9a8
			 0x838928a2, /// 0x9ac
			 0x9568dd6c, /// 0x9b0
			 0xf9215e11, /// 0x9b4
			 0xef2066a7, /// 0x9b8
			 0xf2983c60, /// 0x9bc
			 0x54428be2, /// 0x9c0
			 0xff7dad14, /// 0x9c4
			 0xb9cb9bcf, /// 0x9c8
			 0x9cb8be1a, /// 0x9cc
			 0x540651e5, /// 0x9d0
			 0xee6ec431, /// 0x9d4
			 0x75c9763e, /// 0x9d8
			 0x29bf6386, /// 0x9dc
			 0x0a55191f, /// 0x9e0
			 0x423d9ad9, /// 0x9e4
			 0xae32e73d, /// 0x9e8
			 0xdc9700c5, /// 0x9ec
			 0xa21b65ad, /// 0x9f0
			 0xb5c92bf0, /// 0x9f4
			 0x5ce8d38b, /// 0x9f8
			 0x2b531ac5, /// 0x9fc
			 0x9f376377, /// 0xa00
			 0xe5eeaaa4, /// 0xa04
			 0x1014f00b, /// 0xa08
			 0x6bbf577b, /// 0xa0c
			 0x1fbe9edb, /// 0xa10
			 0xc175d706, /// 0xa14
			 0xac7e6866, /// 0xa18
			 0x5940c4d3, /// 0xa1c
			 0x792b7791, /// 0xa20
			 0x5c190bcd, /// 0xa24
			 0xb639b2d8, /// 0xa28
			 0xba3f1f46, /// 0xa2c
			 0xfb342872, /// 0xa30
			 0x2d534525, /// 0xa34
			 0xf493c8f6, /// 0xa38
			 0x42b0e3d6, /// 0xa3c
			 0x044b2eb9, /// 0xa40
			 0xc4a22a72, /// 0xa44
			 0x9b96d05a, /// 0xa48
			 0x20ba2f17, /// 0xa4c
			 0x686a8504, /// 0xa50
			 0x45b7e8bc, /// 0xa54
			 0xa0efa29c, /// 0xa58
			 0xb712da75, /// 0xa5c
			 0x24ca3d4f, /// 0xa60
			 0xd7059979, /// 0xa64
			 0x753a29c0, /// 0xa68
			 0xf3f6b6eb, /// 0xa6c
			 0xe24f669d, /// 0xa70
			 0x339cc5ef, /// 0xa74
			 0xf666ae7b, /// 0xa78
			 0x3a938628, /// 0xa7c
			 0xbb254db1, /// 0xa80
			 0xf864b85e, /// 0xa84
			 0xe6110af9, /// 0xa88
			 0x0b063e70, /// 0xa8c
			 0x70dd84a9, /// 0xa90
			 0xd56814a0, /// 0xa94
			 0xa78f80d8, /// 0xa98
			 0x44364ea3, /// 0xa9c
			 0x0caac8f4, /// 0xaa0
			 0xfc241cbb, /// 0xaa4
			 0x9c63c5af, /// 0xaa8
			 0x5a569564, /// 0xaac
			 0xb6456cd5, /// 0xab0
			 0xf3e578fc, /// 0xab4
			 0xe864b320, /// 0xab8
			 0x41c7ce1c, /// 0xabc
			 0xd67973cc, /// 0xac0
			 0xeb737e77, /// 0xac4
			 0x6bb91220, /// 0xac8
			 0x0a83c7b1, /// 0xacc
			 0xc371193b, /// 0xad0
			 0xf9e9f980, /// 0xad4
			 0x85723cb3, /// 0xad8
			 0xce6d2f81, /// 0xadc
			 0xe4ce0ae9, /// 0xae0
			 0x6bb4fd8b, /// 0xae4
			 0x62e094c8, /// 0xae8
			 0xd4e49b8c, /// 0xaec
			 0x30ba1f6f, /// 0xaf0
			 0x8f5d71ca, /// 0xaf4
			 0xf9c819ac, /// 0xaf8
			 0x50df2d9f, /// 0xafc
			 0x96c9ad60, /// 0xb00
			 0xfbede1c7, /// 0xb04
			 0x272b5c88, /// 0xb08
			 0xf29f0f01, /// 0xb0c
			 0x63f63080, /// 0xb10
			 0xc4d83fa2, /// 0xb14
			 0xa274e659, /// 0xb18
			 0x60376501, /// 0xb1c
			 0x0b376259, /// 0xb20
			 0x97637284, /// 0xb24
			 0xf92be2de, /// 0xb28
			 0xeef1d764, /// 0xb2c
			 0xa0e3f472, /// 0xb30
			 0x18d5e533, /// 0xb34
			 0x13e3799d, /// 0xb38
			 0x30a677e7, /// 0xb3c
			 0xfb395ddc, /// 0xb40
			 0xed844838, /// 0xb44
			 0x00eb2aea, /// 0xb48
			 0xcd10bdc4, /// 0xb4c
			 0x2cb8cd67, /// 0xb50
			 0xe57a08de, /// 0xb54
			 0xf66b2c88, /// 0xb58
			 0x8412ec9b, /// 0xb5c
			 0x023906b5, /// 0xb60
			 0x255cfd9a, /// 0xb64
			 0x70abae09, /// 0xb68
			 0x43294400, /// 0xb6c
			 0x38312ac8, /// 0xb70
			 0x3a931eb8, /// 0xb74
			 0x1f3de47f, /// 0xb78
			 0x4b4e3b2c, /// 0xb7c
			 0xf4fad42d, /// 0xb80
			 0xcb8db4ae, /// 0xb84
			 0xa1fef02c, /// 0xb88
			 0x0d534d2c, /// 0xb8c
			 0xc4885ee8, /// 0xb90
			 0x2c05c5ca, /// 0xb94
			 0x7acce292, /// 0xb98
			 0xc90f81b6, /// 0xb9c
			 0xf6c752b6, /// 0xba0
			 0xc99ef82f, /// 0xba4
			 0x23b6dba6, /// 0xba8
			 0x456613b1, /// 0xbac
			 0x458e53ba, /// 0xbb0
			 0x5e91bca7, /// 0xbb4
			 0xea7d5cf8, /// 0xbb8
			 0x99616b4a, /// 0xbbc
			 0xabd89653, /// 0xbc0
			 0xc606459c, /// 0xbc4
			 0x27053e66, /// 0xbc8
			 0x803aa99a, /// 0xbcc
			 0x2b616696, /// 0xbd0
			 0x0a494107, /// 0xbd4
			 0x67e33ee6, /// 0xbd8
			 0xca4c5982, /// 0xbdc
			 0xa9fbc6e1, /// 0xbe0
			 0xa3c74504, /// 0xbe4
			 0x6dd98663, /// 0xbe8
			 0x03860a92, /// 0xbec
			 0x170856aa, /// 0xbf0
			 0xfe6b2684, /// 0xbf4
			 0x5fd425dd, /// 0xbf8
			 0x1fae5cab, /// 0xbfc
			 0xff84aedd, /// 0xc00
			 0x542be9f1, /// 0xc04
			 0x6c4f6116, /// 0xc08
			 0x661b8429, /// 0xc0c
			 0xdb181a4c, /// 0xc10
			 0x155046e0, /// 0xc14
			 0x699b3964, /// 0xc18
			 0xe4247e75, /// 0xc1c
			 0x989a6600, /// 0xc20
			 0x9745f7b5, /// 0xc24
			 0xb15b55e0, /// 0xc28
			 0x1ff9e5bd, /// 0xc2c
			 0x02e58fa5, /// 0xc30
			 0xc40dc0c8, /// 0xc34
			 0x50231de6, /// 0xc38
			 0xb8b34021, /// 0xc3c
			 0x1cddbd44, /// 0xc40
			 0x83252f08, /// 0xc44
			 0x4942d6c8, /// 0xc48
			 0xa0fc7c16, /// 0xc4c
			 0x8881a333, /// 0xc50
			 0xe4033218, /// 0xc54
			 0x087f4313, /// 0xc58
			 0xf3d7cb4f, /// 0xc5c
			 0x1926ca53, /// 0xc60
			 0xdbf7dbf8, /// 0xc64
			 0xe048323a, /// 0xc68
			 0xa3dfa84d, /// 0xc6c
			 0x50b44539, /// 0xc70
			 0x6df6c89b, /// 0xc74
			 0xa8b8c19d, /// 0xc78
			 0xb402f924, /// 0xc7c
			 0x7ff09da1, /// 0xc80
			 0xa80e421e, /// 0xc84
			 0x7a8a7b71, /// 0xc88
			 0xbc0ce2b7, /// 0xc8c
			 0x79b9105a, /// 0xc90
			 0xf3ea83c2, /// 0xc94
			 0x5bd981ab, /// 0xc98
			 0xbd095880, /// 0xc9c
			 0xbe3afde1, /// 0xca0
			 0xc23c41e0, /// 0xca4
			 0x37362686, /// 0xca8
			 0xf1f01ae3, /// 0xcac
			 0x4a38094e, /// 0xcb0
			 0xfebab42f, /// 0xcb4
			 0xff80b63c, /// 0xcb8
			 0xc642c04c, /// 0xcbc
			 0x931b440a, /// 0xcc0
			 0x2cc1b509, /// 0xcc4
			 0xcfa8a9eb, /// 0xcc8
			 0x0b5645fb, /// 0xccc
			 0x35d0ec08, /// 0xcd0
			 0x72292f6c, /// 0xcd4
			 0x6cb193bf, /// 0xcd8
			 0xdfe1c194, /// 0xcdc
			 0x092d5d2e, /// 0xce0
			 0xc7405b9c, /// 0xce4
			 0xf6c4d85a, /// 0xce8
			 0xcfb43cb7, /// 0xcec
			 0x041b8e57, /// 0xcf0
			 0xf6810013, /// 0xcf4
			 0x85a4dd9d, /// 0xcf8
			 0x6f65d3bd, /// 0xcfc
			 0xd7784575, /// 0xd00
			 0xcb39fdd1, /// 0xd04
			 0x44874b87, /// 0xd08
			 0xe2635a4a, /// 0xd0c
			 0x28bbbb61, /// 0xd10
			 0x1572256c, /// 0xd14
			 0x9d2aec0c, /// 0xd18
			 0x1c6e3c78, /// 0xd1c
			 0x050199fb, /// 0xd20
			 0x8d4329dc, /// 0xd24
			 0x755835f7, /// 0xd28
			 0xd2a1a5d8, /// 0xd2c
			 0x5cc208f5, /// 0xd30
			 0x22e8bfc0, /// 0xd34
			 0x26d40e49, /// 0xd38
			 0x8ed6fb8a, /// 0xd3c
			 0xf0791c88, /// 0xd40
			 0x1269a675, /// 0xd44
			 0x0f2a865c, /// 0xd48
			 0x828ba160, /// 0xd4c
			 0x7b996cd4, /// 0xd50
			 0xb56c41f7, /// 0xd54
			 0x09a63dec, /// 0xd58
			 0xdfa8e58a, /// 0xd5c
			 0x7f6425ba, /// 0xd60
			 0xb6ff6bbc, /// 0xd64
			 0x94801d4f, /// 0xd68
			 0x15f108f2, /// 0xd6c
			 0x22a51788, /// 0xd70
			 0x07654b66, /// 0xd74
			 0xe4d2e769, /// 0xd78
			 0x1d7a2d62, /// 0xd7c
			 0x1decdcdb, /// 0xd80
			 0xab5cb4f1, /// 0xd84
			 0x7da9f438, /// 0xd88
			 0xc5a44598, /// 0xd8c
			 0x1cb0a3c5, /// 0xd90
			 0x69d24184, /// 0xd94
			 0x04199ac6, /// 0xd98
			 0x06ddb6f5, /// 0xd9c
			 0x293d4d1c, /// 0xda0
			 0x8045ae3b, /// 0xda4
			 0xb52035fa, /// 0xda8
			 0x01367db3, /// 0xdac
			 0x3e2cc4be, /// 0xdb0
			 0xa568a161, /// 0xdb4
			 0x3384c5ab, /// 0xdb8
			 0x752fa522, /// 0xdbc
			 0xa7609537, /// 0xdc0
			 0xf4ce8f04, /// 0xdc4
			 0xce934556, /// 0xdc8
			 0xb22bd18b, /// 0xdcc
			 0x1ac6f620, /// 0xdd0
			 0x730a549a, /// 0xdd4
			 0xe7ffbea9, /// 0xdd8
			 0xe4ffa458, /// 0xddc
			 0xf00ea183, /// 0xde0
			 0x7ea1e00c, /// 0xde4
			 0xa22b5868, /// 0xde8
			 0x287eaec4, /// 0xdec
			 0x98cac10f, /// 0xdf0
			 0x81cf96ae, /// 0xdf4
			 0xd746717a, /// 0xdf8
			 0x8ae8225f, /// 0xdfc
			 0x24c98d47, /// 0xe00
			 0x45648bce, /// 0xe04
			 0xfce10f85, /// 0xe08
			 0xbcfd49ae, /// 0xe0c
			 0xd7c8973e, /// 0xe10
			 0x082a747c, /// 0xe14
			 0x17bd411a, /// 0xe18
			 0xbfbf2de6, /// 0xe1c
			 0xaf8cc116, /// 0xe20
			 0x23db6881, /// 0xe24
			 0xdf27804c, /// 0xe28
			 0x2f28b197, /// 0xe2c
			 0xaa2a02fe, /// 0xe30
			 0x13e8bcfd, /// 0xe34
			 0xfe2a7e0c, /// 0xe38
			 0x466cddbe, /// 0xe3c
			 0x1644d99d, /// 0xe40
			 0xf95fdc4d, /// 0xe44
			 0xa96567c5, /// 0xe48
			 0xd7747dc4, /// 0xe4c
			 0xd99f41da, /// 0xe50
			 0x44904d27, /// 0xe54
			 0x9e99bc0b, /// 0xe58
			 0xd644d39b, /// 0xe5c
			 0x5fa57aab, /// 0xe60
			 0x50e6f65f, /// 0xe64
			 0xd35b6eca, /// 0xe68
			 0x3251d610, /// 0xe6c
			 0x29be460d, /// 0xe70
			 0xa4d84d4e, /// 0xe74
			 0x63e466ec, /// 0xe78
			 0x9cb3cd9d, /// 0xe7c
			 0x68cf7deb, /// 0xe80
			 0x86c54287, /// 0xe84
			 0x89c2f9b2, /// 0xe88
			 0x0baae11d, /// 0xe8c
			 0x83fbac47, /// 0xe90
			 0xd871ae01, /// 0xe94
			 0xa3407591, /// 0xe98
			 0x852a18a9, /// 0xe9c
			 0x5e1c348b, /// 0xea0
			 0xb64dfeae, /// 0xea4
			 0xca9a0fa7, /// 0xea8
			 0xe6f28ca4, /// 0xeac
			 0x601cfad4, /// 0xeb0
			 0xd181d3a6, /// 0xeb4
			 0x5c9a283d, /// 0xeb8
			 0xc42ceb7f, /// 0xebc
			 0x5ea5c1cf, /// 0xec0
			 0x12bbec1d, /// 0xec4
			 0x1c89878e, /// 0xec8
			 0x66d09248, /// 0xecc
			 0xea0de09f, /// 0xed0
			 0xc787ca32, /// 0xed4
			 0xfb0a0b0b, /// 0xed8
			 0x1bbacc9c, /// 0xedc
			 0x417c5f3b, /// 0xee0
			 0xc5f93d02, /// 0xee4
			 0x4872bbca, /// 0xee8
			 0x80bc53b1, /// 0xeec
			 0x4f7eea10, /// 0xef0
			 0xfe4f44e8, /// 0xef4
			 0x2453949c, /// 0xef8
			 0xc1976c7e, /// 0xefc
			 0xe1d0d017, /// 0xf00
			 0xf5b8599f, /// 0xf04
			 0x6d50d3e3, /// 0xf08
			 0x4df77546, /// 0xf0c
			 0x9b47be57, /// 0xf10
			 0x5c2cda16, /// 0xf14
			 0x44b1812e, /// 0xf18
			 0x65599d85, /// 0xf1c
			 0xe8829434, /// 0xf20
			 0x41d195eb, /// 0xf24
			 0x2f3a46ec, /// 0xf28
			 0x2a95b088, /// 0xf2c
			 0x49eb8369, /// 0xf30
			 0x0dd41281, /// 0xf34
			 0x0d1d4b2d, /// 0xf38
			 0x1b3ed92a, /// 0xf3c
			 0xd2cffeb7, /// 0xf40
			 0xe4efbf90, /// 0xf44
			 0xdf283353, /// 0xf48
			 0x6876ea88, /// 0xf4c
			 0xedbccd38, /// 0xf50
			 0x356942e4, /// 0xf54
			 0x002da56a, /// 0xf58
			 0xf3e1961e, /// 0xf5c
			 0x9d35d566, /// 0xf60
			 0x161b8fb5, /// 0xf64
			 0x3c1a055d, /// 0xf68
			 0xc7113e3c, /// 0xf6c
			 0x86e35da0, /// 0xf70
			 0x4a9677e3, /// 0xf74
			 0x35ef4fcd, /// 0xf78
			 0xd7a892d4, /// 0xf7c
			 0x87f5bee8, /// 0xf80
			 0x66a5ef62, /// 0xf84
			 0x11f32a45, /// 0xf88
			 0x291c4521, /// 0xf8c
			 0x5193f1bc, /// 0xf90
			 0xcd38f17a, /// 0xf94
			 0xfbfe1b82, /// 0xf98
			 0x2f5463a3, /// 0xf9c
			 0xe3af2ea4, /// 0xfa0
			 0x0f4abda6, /// 0xfa4
			 0xb7263b9a, /// 0xfa8
			 0xff0e8921, /// 0xfac
			 0xdff2979e, /// 0xfb0
			 0xa0a3eea8, /// 0xfb4
			 0xcc369fbb, /// 0xfb8
			 0xd558e760, /// 0xfbc
			 0x7404da1c, /// 0xfc0
			 0xdc07f456, /// 0xfc4
			 0x04c22bdc, /// 0xfc8
			 0xc283d2e8, /// 0xfcc
			 0x1d09eb43, /// 0xfd0
			 0xce661512, /// 0xfd4
			 0x7a40bf10, /// 0xfd8
			 0xdb02dad9, /// 0xfdc
			 0xcbd5adf9, /// 0xfe0
			 0x60af83c4, /// 0xfe4
			 0x30cd1040, /// 0xfe8
			 0x2c679aa4, /// 0xfec
			 0x8546ba75, /// 0xff0
			 0x7c5c35e4, /// 0xff4
			 0xbe837f20, /// 0xff8
			 0x0749d05c /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000007,                                                  // Trailing 1s:                                /// 00000
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00004
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0000c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00010
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00014
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c700000,                                                  // Leading 1s:                                 /// 0001c
			 0x55555555,                                                  // 4 random values                             /// 00020
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00024
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00028
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0002c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00030
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0003c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00044
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00048
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00058
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0005c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00064
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00000000,                                                  // zero                                        /// 0006c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00070
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00074
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00078
			 0x55555555,                                                  // 4 random values                             /// 0007c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00090
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000cc
			 0x00000000,                                                  // zero                                        /// 000d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00108
			 0x33333333,                                                  // 4 random values                             /// 0010c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00118
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00124
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00011111,                                                  // 9.7958E-41                                  /// 0012c
			 0x7f800000,                                                  // inf                                         /// 00130
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00138
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x7fc00001,                                                  // signaling NaN                               /// 00144
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00150
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00154
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00160
			 0x0c700000,                                                  // Leading 1s:                                 /// 00164
			 0x0e000003,                                                  // Trailing 1s:                                /// 00168
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e000003,                                                  // Trailing 1s:                                /// 00174
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00178
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0017c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00180
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00184
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00190
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a4
			 0xff800000,                                                  // -inf                                        /// 001a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001b8
			 0xffc00001,                                                  // -signaling NaN                              /// 001bc
			 0xff800000,                                                  // -inf                                        /// 001c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00204
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0020c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00214
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00218
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00220
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00224
			 0x0c400000,                                                  // Leading 1s:                                 /// 00228
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0022c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00230
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x33333333,                                                  // 4 random values                             /// 00248
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0024c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00250
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00254
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00258
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00270
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00274
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00280
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00288
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00294
			 0x00000000,                                                  // zero                                        /// 00298
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0029c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0xbf028f5c,                                                  // -0.51                                       /// 002d8
			 0xffc00001,                                                  // -signaling NaN                              /// 002dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00300
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00304
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00308
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00324
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00328
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0032c
			 0xcb000000,                                                  // -8388608.0                                  /// 00330
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00334
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00338
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00340
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00344
			 0x80011111,                                                  // -9.7958E-41                                 /// 00348
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0035c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00360
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00364
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00368
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00370
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00374
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00378
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x55555555,                                                  // 4 random values                             /// 00384
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00388
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0038c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00390
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00394
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0xffc00001,                                                  // -signaling NaN                              /// 0039c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00400
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00408
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00410
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00418
			 0x7fc00001,                                                  // signaling NaN                               /// 0041c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00420
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00424
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00428
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0042c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00430
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00438
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00448
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0044c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00450
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00454
			 0x0c780000,                                                  // Leading 1s:                                 /// 00458
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00460
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00464
			 0x4b000000,                                                  // 8388608.0                                   /// 00468
			 0x0c700000,                                                  // Leading 1s:                                 /// 0046c
			 0xcb000000,                                                  // -8388608.0                                  /// 00470
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00478
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00480
			 0x00011111,                                                  // 9.7958E-41                                  /// 00484
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00488
			 0xff800000,                                                  // -inf                                        /// 0048c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00490
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00494
			 0x3f028f5c,                                                  // 0.51                                        /// 00498
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0049c
			 0x00011111,                                                  // 9.7958E-41                                  /// 004a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x7fc00001,                                                  // signaling NaN                               /// 004ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004fc
			 0x80000000,                                                  // -zero                                       /// 00500
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00504
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0050c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00510
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00514
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00518
			 0x4b000000,                                                  // 8388608.0                                   /// 0051c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00520
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00524
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00528
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0052c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00530
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00534
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00538
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0053c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00540
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00548
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0054c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00550
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00554
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00558
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00560
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0056c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00570
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00574
			 0x00011111,                                                  // 9.7958E-41                                  /// 00578
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0057c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00580
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00584
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0xbf028f5c,                                                  // -0.51                                       /// 00590
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00604
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0060c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x33333333,                                                  // 4 random values                             /// 00618
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00624
			 0x7fc00001,                                                  // signaling NaN                               /// 00628
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0062c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00630
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00634
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00638
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00644
			 0x3f028f5c,                                                  // 0.51                                        /// 00648
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0064c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00654
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00658
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0065c
			 0x80000000,                                                  // -zero                                       /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00668
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0066c
			 0x00000000,                                                  // zero                                        /// 00670
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00674
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0067c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00680
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00684
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00688
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0068c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d0
			 0xbf028f5c,                                                  // -0.51                                       /// 006d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00700
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00704
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00708
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0070c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00710
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00718
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0071c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c400000,                                                  // Leading 1s:                                 /// 00728
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0072c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00730
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00734
			 0x0e000001,                                                  // Trailing 1s:                                /// 00738
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0073c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00740
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00744
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00750
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00754
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0c780000,                                                  // Leading 1s:                                 /// 0075c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00760
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00770
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00774
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0077c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00784
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00788
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00798
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0079c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00808
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0080c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00810
			 0x3f028f5c,                                                  // 0.51                                        /// 00814
			 0x0c600000,                                                  // Leading 1s:                                 /// 00818
			 0xffc00001,                                                  // -signaling NaN                              /// 0081c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00820
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00824
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00828
			 0x0c700000,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0xffc00001,                                                  // -signaling NaN                              /// 00838
			 0xffc00001,                                                  // -signaling NaN                              /// 0083c
			 0x3f028f5c,                                                  // 0.51                                        /// 00840
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00844
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00848
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00850
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00854
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0085c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00864
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0086c
			 0x80000000,                                                  // -zero                                       /// 00870
			 0x0c700000,                                                  // Leading 1s:                                 /// 00874
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00880
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00884
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00898
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0xcb000000,                                                  // -8388608.0                                  /// 008d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ec
			 0x33333333,                                                  // 4 random values                             /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00908
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0090c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00910
			 0x80011111,                                                  // -9.7958E-41                                 /// 00914
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00918
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00924
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00928
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0092c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00930
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00934
			 0x00000000,                                                  // zero                                        /// 00938
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00940
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00950
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00958
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00964
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00968
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00970
			 0x3f028f5c,                                                  // 0.51                                        /// 00974
			 0x80011111,                                                  // -9.7958E-41                                 /// 00978
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0097c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00980
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00988
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0098c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x7f800000,                                                  // inf                                         /// 009ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x33333333,                                                  // 4 random values                             /// 00a04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a0c
			 0x00000000,                                                  // zero                                        /// 00a10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x00000000,                                                  // zero                                        /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00000000,                                                  // zero                                        /// 00abc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ac4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00acc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ad0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ad4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00adc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ae8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00af8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00afc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x4b000000,                                                  // 8388608.0                                   /// 00b24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b30
			 0x80000000,                                                  // -zero                                       /// 00b34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0xff800000,                                                  // -inf                                        /// 00b50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ba0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000000,                                                  // -zero                                       /// 00bb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00be0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c14
			 0x80000000,                                                  // -zero                                       /// 00c18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ca8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ccc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ce4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cec
			 0x00000000,                                                  // zero                                        /// 00cf0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cfc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d18
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0xff800000,                                                  // -inf                                        /// 00d48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d78
			 0x7f800000,                                                  // inf                                         /// 00d7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00da0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc0
			 0x55555555,                                                  // 4 random values                             /// 00dc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dc8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dcc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ddc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00de0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00de4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00de8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x80000000,                                                  // -zero                                       /// 00e20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80000000,                                                  // -zero                                       /// 00e40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e74
			 0xcb000000,                                                  // -8388608.0                                  /// 00e78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ebc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ec0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ec8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00edc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ef4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f00
			 0x7fc00001,                                                  // signaling NaN                               /// 00f04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f18
			 0x4b000000,                                                  // 8388608.0                                   /// 00f1c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f78
			 0x80000000,                                                  // -zero                                       /// 00f7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fbc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fcc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ff0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff8
			 0x0c7fffc0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xa02873ad, /// 0x0
			 0x27d2923f, /// 0x4
			 0x782547d8, /// 0x8
			 0xb71c1068, /// 0xc
			 0xe046f51a, /// 0x10
			 0x7abfcb3b, /// 0x14
			 0x4ad5ce95, /// 0x18
			 0x11b8e34a, /// 0x1c
			 0x223824ad, /// 0x20
			 0xb08df6b5, /// 0x24
			 0xbd17e793, /// 0x28
			 0xf11bf372, /// 0x2c
			 0xeddb6a7f, /// 0x30
			 0x23530b02, /// 0x34
			 0x0ee4288a, /// 0x38
			 0xbee47ff6, /// 0x3c
			 0xefb4c3ba, /// 0x40
			 0x95678cf9, /// 0x44
			 0x962745ca, /// 0x48
			 0x9bb0b452, /// 0x4c
			 0x9c9e33cf, /// 0x50
			 0x06002a08, /// 0x54
			 0xdf356402, /// 0x58
			 0x7d1a45f0, /// 0x5c
			 0xc987fff8, /// 0x60
			 0xc6ba4145, /// 0x64
			 0x4af44dd2, /// 0x68
			 0xaa3af85d, /// 0x6c
			 0xaa71bc86, /// 0x70
			 0xca1b04e6, /// 0x74
			 0x6be56fbc, /// 0x78
			 0x3e55e931, /// 0x7c
			 0x80cc387d, /// 0x80
			 0x4be86900, /// 0x84
			 0x132a7eb2, /// 0x88
			 0xe82170af, /// 0x8c
			 0xb758be26, /// 0x90
			 0x5830859e, /// 0x94
			 0xcdfb8a0e, /// 0x98
			 0x20321bc1, /// 0x9c
			 0x99e2fb12, /// 0xa0
			 0x0775b9a7, /// 0xa4
			 0x46fef1e6, /// 0xa8
			 0xfa6bc710, /// 0xac
			 0xf37ea695, /// 0xb0
			 0xbb1deaf0, /// 0xb4
			 0x3b6d1800, /// 0xb8
			 0xe2a550ad, /// 0xbc
			 0xf22a3c23, /// 0xc0
			 0x1b18f920, /// 0xc4
			 0xd762fc2f, /// 0xc8
			 0xa4451c80, /// 0xcc
			 0x2643bcd3, /// 0xd0
			 0x60c208a0, /// 0xd4
			 0x38234888, /// 0xd8
			 0xf25a2d9d, /// 0xdc
			 0x46d29893, /// 0xe0
			 0x2384e335, /// 0xe4
			 0x9a66fc84, /// 0xe8
			 0x94097416, /// 0xec
			 0x6e8c0cca, /// 0xf0
			 0x9b6b7947, /// 0xf4
			 0xcedfac85, /// 0xf8
			 0x680d25ef, /// 0xfc
			 0xef1657ea, /// 0x100
			 0xc066c0ce, /// 0x104
			 0xdac96337, /// 0x108
			 0x860207f2, /// 0x10c
			 0x1a1a7ee9, /// 0x110
			 0x40d502e5, /// 0x114
			 0x13c09f30, /// 0x118
			 0xd9e52546, /// 0x11c
			 0xcd5f9e14, /// 0x120
			 0xcd79d185, /// 0x124
			 0xc8e1b65d, /// 0x128
			 0xefe9bc1d, /// 0x12c
			 0x9df1a10a, /// 0x130
			 0x76340650, /// 0x134
			 0xa88c36e0, /// 0x138
			 0xf1e7dc1d, /// 0x13c
			 0x451a084c, /// 0x140
			 0xa9d03ee8, /// 0x144
			 0xb74c9b18, /// 0x148
			 0x0be29989, /// 0x14c
			 0x97942379, /// 0x150
			 0x508a5689, /// 0x154
			 0x5c57c171, /// 0x158
			 0xfdd9a958, /// 0x15c
			 0x272c92cb, /// 0x160
			 0x0a7b4ae6, /// 0x164
			 0xa003f2fd, /// 0x168
			 0x50b88e97, /// 0x16c
			 0x5cfb8dbd, /// 0x170
			 0x18cfaf2b, /// 0x174
			 0xe49e4572, /// 0x178
			 0x2af28f61, /// 0x17c
			 0x6a926e3d, /// 0x180
			 0xbf524e3f, /// 0x184
			 0x90229dd8, /// 0x188
			 0x7da62798, /// 0x18c
			 0x2196383f, /// 0x190
			 0xa8454272, /// 0x194
			 0x217ba81c, /// 0x198
			 0x87b61dc6, /// 0x19c
			 0x6a85743c, /// 0x1a0
			 0x6d87f7e5, /// 0x1a4
			 0x7f837f3a, /// 0x1a8
			 0x360c2c06, /// 0x1ac
			 0x2ca5bfce, /// 0x1b0
			 0x5f0cc023, /// 0x1b4
			 0x9b7cdde9, /// 0x1b8
			 0xd19c625a, /// 0x1bc
			 0xcdcf6ab7, /// 0x1c0
			 0xd595d976, /// 0x1c4
			 0x18ac9f5f, /// 0x1c8
			 0xf0e7a908, /// 0x1cc
			 0x5e9dfe7c, /// 0x1d0
			 0x0d851fc8, /// 0x1d4
			 0xe39ff229, /// 0x1d8
			 0xd60a50ab, /// 0x1dc
			 0x1c579687, /// 0x1e0
			 0x14ece5ec, /// 0x1e4
			 0x7d9d20c2, /// 0x1e8
			 0x95fbf063, /// 0x1ec
			 0xcaf5090e, /// 0x1f0
			 0xc60fe2c4, /// 0x1f4
			 0x81d612fb, /// 0x1f8
			 0xdb29c92c, /// 0x1fc
			 0x218e2f47, /// 0x200
			 0xf47b8046, /// 0x204
			 0x38d3b1a6, /// 0x208
			 0xfaf20880, /// 0x20c
			 0x6860ec38, /// 0x210
			 0x9c24a6f2, /// 0x214
			 0xca1556bc, /// 0x218
			 0x4af6f614, /// 0x21c
			 0xa1660344, /// 0x220
			 0x932e1851, /// 0x224
			 0x4931b6a6, /// 0x228
			 0xd51365c8, /// 0x22c
			 0x51e8b05b, /// 0x230
			 0x6c22b491, /// 0x234
			 0x53f1e4c4, /// 0x238
			 0x6bcf5149, /// 0x23c
			 0xdfeba691, /// 0x240
			 0xecf8b35b, /// 0x244
			 0x8c607691, /// 0x248
			 0xebfb1038, /// 0x24c
			 0xc5d02a93, /// 0x250
			 0xb10ce52e, /// 0x254
			 0xc75ffc3c, /// 0x258
			 0x227a6850, /// 0x25c
			 0xb7df88fe, /// 0x260
			 0x00ce59e0, /// 0x264
			 0x7748574a, /// 0x268
			 0xb684c760, /// 0x26c
			 0xc68ab585, /// 0x270
			 0xadc1d9d3, /// 0x274
			 0x20114bf3, /// 0x278
			 0x45e8b0ad, /// 0x27c
			 0x9efae7d2, /// 0x280
			 0x2354bf54, /// 0x284
			 0xf0b202e9, /// 0x288
			 0xf714bb9b, /// 0x28c
			 0xfc8e6162, /// 0x290
			 0xf173874a, /// 0x294
			 0xdc81b89e, /// 0x298
			 0x5a2e95e3, /// 0x29c
			 0xfa28acc1, /// 0x2a0
			 0x1844b036, /// 0x2a4
			 0x453ff0a3, /// 0x2a8
			 0xfcd58818, /// 0x2ac
			 0x0c93754a, /// 0x2b0
			 0x92b69f5c, /// 0x2b4
			 0x41d73916, /// 0x2b8
			 0x1c297462, /// 0x2bc
			 0xa1fc19b6, /// 0x2c0
			 0x742e1974, /// 0x2c4
			 0x23bcb361, /// 0x2c8
			 0x3b3f32e9, /// 0x2cc
			 0x6f7ce316, /// 0x2d0
			 0xab11c0d2, /// 0x2d4
			 0xc23e19a8, /// 0x2d8
			 0xbcab08c3, /// 0x2dc
			 0xd9adb346, /// 0x2e0
			 0x06ffef08, /// 0x2e4
			 0x01452794, /// 0x2e8
			 0xe586559b, /// 0x2ec
			 0x92a71af1, /// 0x2f0
			 0x83207317, /// 0x2f4
			 0xc3ec754d, /// 0x2f8
			 0xf3916849, /// 0x2fc
			 0x9d4fc1cd, /// 0x300
			 0xc1ecaca0, /// 0x304
			 0x7e89cd31, /// 0x308
			 0xd13ce33b, /// 0x30c
			 0xe258a758, /// 0x310
			 0xd20a1e83, /// 0x314
			 0x533b0f85, /// 0x318
			 0xd3541e92, /// 0x31c
			 0x2e5b9ef7, /// 0x320
			 0x0cdb85ec, /// 0x324
			 0xd8f72c94, /// 0x328
			 0x0458642a, /// 0x32c
			 0xee7bbeee, /// 0x330
			 0x11b58277, /// 0x334
			 0x35a2ecd6, /// 0x338
			 0x6067a6e0, /// 0x33c
			 0x57472c4a, /// 0x340
			 0xfb2aa328, /// 0x344
			 0xf4cd084a, /// 0x348
			 0x30014d38, /// 0x34c
			 0x59f80b29, /// 0x350
			 0x227a9bae, /// 0x354
			 0x1b8cf7f4, /// 0x358
			 0x8c5012af, /// 0x35c
			 0x55cedfe9, /// 0x360
			 0x587687f8, /// 0x364
			 0xb36f4022, /// 0x368
			 0xd72c432c, /// 0x36c
			 0x8cbedc3a, /// 0x370
			 0x9424b041, /// 0x374
			 0x5b02e59a, /// 0x378
			 0x0b62257e, /// 0x37c
			 0x8400c2f4, /// 0x380
			 0xf7c816be, /// 0x384
			 0xda802f2c, /// 0x388
			 0x0ecf892a, /// 0x38c
			 0x60764f32, /// 0x390
			 0xedee9279, /// 0x394
			 0x59150443, /// 0x398
			 0xcdca4b2b, /// 0x39c
			 0x3cffa669, /// 0x3a0
			 0x366a31f0, /// 0x3a4
			 0x945c215a, /// 0x3a8
			 0x17c90154, /// 0x3ac
			 0x5bfdb9ea, /// 0x3b0
			 0x9275757d, /// 0x3b4
			 0x618bb6e1, /// 0x3b8
			 0x210d8335, /// 0x3bc
			 0xdba7fbe0, /// 0x3c0
			 0x18415da8, /// 0x3c4
			 0x410e8c80, /// 0x3c8
			 0x7e843d2e, /// 0x3cc
			 0x9ddd7812, /// 0x3d0
			 0x3ad09ded, /// 0x3d4
			 0x89cd0045, /// 0x3d8
			 0x3eb69cb3, /// 0x3dc
			 0x17d43fa0, /// 0x3e0
			 0xd60cc75b, /// 0x3e4
			 0x9f71c357, /// 0x3e8
			 0x8fa22a16, /// 0x3ec
			 0x2ebdd7f6, /// 0x3f0
			 0xe3985da0, /// 0x3f4
			 0x6ca158c2, /// 0x3f8
			 0xe353d642, /// 0x3fc
			 0x9c525277, /// 0x400
			 0xe31b22f0, /// 0x404
			 0x2184ce98, /// 0x408
			 0x20b941f1, /// 0x40c
			 0x9f46492e, /// 0x410
			 0xeda284a4, /// 0x414
			 0x45adb9a1, /// 0x418
			 0x42d75892, /// 0x41c
			 0x25c7ec32, /// 0x420
			 0x3ed35a0e, /// 0x424
			 0xb2c39e9d, /// 0x428
			 0x96c32c89, /// 0x42c
			 0x5cd7ddb0, /// 0x430
			 0xc8288d77, /// 0x434
			 0x3b5be55b, /// 0x438
			 0x1b6f8727, /// 0x43c
			 0x008063ca, /// 0x440
			 0xed2c3ada, /// 0x444
			 0xf89c01ba, /// 0x448
			 0x6be0c25e, /// 0x44c
			 0x8a329298, /// 0x450
			 0x1e8a8a70, /// 0x454
			 0x2d79ac14, /// 0x458
			 0x635d63b0, /// 0x45c
			 0xa9004cbf, /// 0x460
			 0x826dd812, /// 0x464
			 0x671e44c1, /// 0x468
			 0x65de8a07, /// 0x46c
			 0x0987fd42, /// 0x470
			 0x472ea084, /// 0x474
			 0xd206d0ce, /// 0x478
			 0x7dc172a2, /// 0x47c
			 0x4506bb32, /// 0x480
			 0x93ab8de1, /// 0x484
			 0x60c2d4a5, /// 0x488
			 0x6b701163, /// 0x48c
			 0xdbcde094, /// 0x490
			 0xbf7b4483, /// 0x494
			 0xdb14d37a, /// 0x498
			 0xb0dba2fe, /// 0x49c
			 0xff547b7e, /// 0x4a0
			 0xe330c81d, /// 0x4a4
			 0x2775e5c4, /// 0x4a8
			 0xc1c1f494, /// 0x4ac
			 0x3422f3fa, /// 0x4b0
			 0xebc86376, /// 0x4b4
			 0x429cf180, /// 0x4b8
			 0x2c5b401a, /// 0x4bc
			 0xd06a9513, /// 0x4c0
			 0x53a52f7d, /// 0x4c4
			 0x23089c0b, /// 0x4c8
			 0x0625230c, /// 0x4cc
			 0x4b0bd01c, /// 0x4d0
			 0x907a7d89, /// 0x4d4
			 0x3fd7fbe5, /// 0x4d8
			 0x337fab8c, /// 0x4dc
			 0x3d31911b, /// 0x4e0
			 0xc6f117cb, /// 0x4e4
			 0x03e9b545, /// 0x4e8
			 0x6a34f293, /// 0x4ec
			 0xb6a40e51, /// 0x4f0
			 0xa373f3e0, /// 0x4f4
			 0x5dc6d2de, /// 0x4f8
			 0xda889ba2, /// 0x4fc
			 0x40ec931a, /// 0x500
			 0x67e20be7, /// 0x504
			 0xe6010ec6, /// 0x508
			 0xaebb7eb1, /// 0x50c
			 0x46708070, /// 0x510
			 0xec0e4b06, /// 0x514
			 0xe77c11a0, /// 0x518
			 0x2527fa0c, /// 0x51c
			 0x7f135683, /// 0x520
			 0x16df5921, /// 0x524
			 0xc9babc6e, /// 0x528
			 0x0c4ab3e4, /// 0x52c
			 0xc9b6a4d3, /// 0x530
			 0x6d6ca51c, /// 0x534
			 0x36a2483b, /// 0x538
			 0x1a11f4d1, /// 0x53c
			 0xc47f9a96, /// 0x540
			 0x899b91b4, /// 0x544
			 0x46d6b815, /// 0x548
			 0xa8903c75, /// 0x54c
			 0xb5ec5f77, /// 0x550
			 0xc6d2a4bd, /// 0x554
			 0xe98ff705, /// 0x558
			 0xcc51e74b, /// 0x55c
			 0x9c7035b8, /// 0x560
			 0xfd04f900, /// 0x564
			 0xeea545b9, /// 0x568
			 0x320b437c, /// 0x56c
			 0x2279061e, /// 0x570
			 0x00b31645, /// 0x574
			 0xf2697b98, /// 0x578
			 0x1e9b4012, /// 0x57c
			 0x7fac3bb3, /// 0x580
			 0x4867d83a, /// 0x584
			 0x623e7542, /// 0x588
			 0xb93edc66, /// 0x58c
			 0x77d674e2, /// 0x590
			 0x680911e4, /// 0x594
			 0x4af0fc45, /// 0x598
			 0x065c44f3, /// 0x59c
			 0xec7b835c, /// 0x5a0
			 0xa85733ad, /// 0x5a4
			 0xe70dea60, /// 0x5a8
			 0x06f2e744, /// 0x5ac
			 0x36f9e644, /// 0x5b0
			 0x476d5915, /// 0x5b4
			 0x89c43196, /// 0x5b8
			 0xbcb281d1, /// 0x5bc
			 0x0875753b, /// 0x5c0
			 0x7b9336b9, /// 0x5c4
			 0x3a048bc4, /// 0x5c8
			 0x9dc3428b, /// 0x5cc
			 0x6e807efd, /// 0x5d0
			 0xf6da7721, /// 0x5d4
			 0xa0ff7cb1, /// 0x5d8
			 0xf10b04e7, /// 0x5dc
			 0xeb0754c1, /// 0x5e0
			 0x748d2da6, /// 0x5e4
			 0x81729337, /// 0x5e8
			 0x1b7d10fd, /// 0x5ec
			 0x23dd07d2, /// 0x5f0
			 0x6c8df3b4, /// 0x5f4
			 0x77b83fbb, /// 0x5f8
			 0x089832da, /// 0x5fc
			 0x9faa0f1b, /// 0x600
			 0xef411459, /// 0x604
			 0xc2871ef4, /// 0x608
			 0x41291e90, /// 0x60c
			 0x6bc6e8a2, /// 0x610
			 0x3bf74e45, /// 0x614
			 0x0e8813e2, /// 0x618
			 0x463f9767, /// 0x61c
			 0x5a9cd0e9, /// 0x620
			 0x06a42629, /// 0x624
			 0xbd3b34f9, /// 0x628
			 0xd80a1bfa, /// 0x62c
			 0x54283a0a, /// 0x630
			 0x2a3af583, /// 0x634
			 0x2875f20d, /// 0x638
			 0x69224340, /// 0x63c
			 0x902092aa, /// 0x640
			 0x0889a807, /// 0x644
			 0xbc36dd16, /// 0x648
			 0x27f95f30, /// 0x64c
			 0xf3b2c1e2, /// 0x650
			 0x3732c4c3, /// 0x654
			 0xa2833979, /// 0x658
			 0x2bf114be, /// 0x65c
			 0xc1914dcf, /// 0x660
			 0xef2782ef, /// 0x664
			 0x87d2ef6e, /// 0x668
			 0x1b76e9c6, /// 0x66c
			 0x33855c48, /// 0x670
			 0xa2f5d33b, /// 0x674
			 0x64b5a046, /// 0x678
			 0x2580b836, /// 0x67c
			 0xd5943f4a, /// 0x680
			 0x8cb99cff, /// 0x684
			 0x28a1dde2, /// 0x688
			 0x13ec0b47, /// 0x68c
			 0x110dc540, /// 0x690
			 0x4fe7cb1a, /// 0x694
			 0x0fa31178, /// 0x698
			 0xae759b29, /// 0x69c
			 0xe0a67885, /// 0x6a0
			 0xdf457284, /// 0x6a4
			 0x9df8f4eb, /// 0x6a8
			 0xa35698a8, /// 0x6ac
			 0x999020aa, /// 0x6b0
			 0x79e64e61, /// 0x6b4
			 0x6ff8b763, /// 0x6b8
			 0x10aea3b3, /// 0x6bc
			 0x7c2db622, /// 0x6c0
			 0x63614c14, /// 0x6c4
			 0xfe066e64, /// 0x6c8
			 0x463aa7de, /// 0x6cc
			 0x4833dfdd, /// 0x6d0
			 0x065a83bf, /// 0x6d4
			 0x1beb0a84, /// 0x6d8
			 0xeb9c69c3, /// 0x6dc
			 0x08c052e8, /// 0x6e0
			 0x596a3000, /// 0x6e4
			 0xd25cb852, /// 0x6e8
			 0x3ef98120, /// 0x6ec
			 0x5bd73171, /// 0x6f0
			 0x6693dced, /// 0x6f4
			 0x09aafab7, /// 0x6f8
			 0x073ca77c, /// 0x6fc
			 0xa57c25ec, /// 0x700
			 0x6551ea71, /// 0x704
			 0x8fb4e8cd, /// 0x708
			 0xcc8e9c0d, /// 0x70c
			 0x6b4ad1cf, /// 0x710
			 0x3306b403, /// 0x714
			 0x8f056da9, /// 0x718
			 0xb048b2bb, /// 0x71c
			 0x989cec21, /// 0x720
			 0xf8453107, /// 0x724
			 0x59f1326a, /// 0x728
			 0x7a144bb6, /// 0x72c
			 0xb99fbc4c, /// 0x730
			 0x7c1da73e, /// 0x734
			 0xaa7bf98b, /// 0x738
			 0xe8de0144, /// 0x73c
			 0x63a7154c, /// 0x740
			 0xba691072, /// 0x744
			 0x5675ed0d, /// 0x748
			 0x2d21eebd, /// 0x74c
			 0x15dc3a84, /// 0x750
			 0x6ae12045, /// 0x754
			 0x40af13f8, /// 0x758
			 0x377b7c6c, /// 0x75c
			 0xdf2429aa, /// 0x760
			 0x87d222ab, /// 0x764
			 0x24d90e90, /// 0x768
			 0x82fc5d61, /// 0x76c
			 0xe8b24193, /// 0x770
			 0x7f8a6998, /// 0x774
			 0x92a02bf6, /// 0x778
			 0xadc1ea9a, /// 0x77c
			 0xc353fdbf, /// 0x780
			 0x276786c4, /// 0x784
			 0xf1d2b209, /// 0x788
			 0xb9f8949d, /// 0x78c
			 0xf97419be, /// 0x790
			 0x1840d332, /// 0x794
			 0x9235a5ff, /// 0x798
			 0xa5dde16b, /// 0x79c
			 0x7560adb5, /// 0x7a0
			 0x02242327, /// 0x7a4
			 0x89708df7, /// 0x7a8
			 0xc1e84392, /// 0x7ac
			 0xf1ff08e0, /// 0x7b0
			 0x67206d7e, /// 0x7b4
			 0x673f9cf9, /// 0x7b8
			 0x47d69c4d, /// 0x7bc
			 0x595e53ad, /// 0x7c0
			 0xc738d9bd, /// 0x7c4
			 0x91598e64, /// 0x7c8
			 0x45cff918, /// 0x7cc
			 0x9af9e326, /// 0x7d0
			 0xcaf1c729, /// 0x7d4
			 0x471e62fa, /// 0x7d8
			 0xce40c5d6, /// 0x7dc
			 0x730e43e0, /// 0x7e0
			 0x037a2f63, /// 0x7e4
			 0x1bd0d15f, /// 0x7e8
			 0x21952c73, /// 0x7ec
			 0xbb46ce4f, /// 0x7f0
			 0x91604985, /// 0x7f4
			 0x1d909f8d, /// 0x7f8
			 0x546bd8b3, /// 0x7fc
			 0xa44546fe, /// 0x800
			 0x50b82e3f, /// 0x804
			 0xf19b8844, /// 0x808
			 0x4787d4c7, /// 0x80c
			 0xd4011e38, /// 0x810
			 0x16691153, /// 0x814
			 0xc3a60a9c, /// 0x818
			 0x512a9b59, /// 0x81c
			 0xcbbafc1d, /// 0x820
			 0x1d1dfa9f, /// 0x824
			 0x482399ef, /// 0x828
			 0x37cc8e98, /// 0x82c
			 0xed8f6a73, /// 0x830
			 0xa8aacdc2, /// 0x834
			 0xae1e03c9, /// 0x838
			 0xb72bedca, /// 0x83c
			 0x6b7653f2, /// 0x840
			 0xfd8412cf, /// 0x844
			 0xc2512085, /// 0x848
			 0x362622a1, /// 0x84c
			 0x5137c671, /// 0x850
			 0x3e192072, /// 0x854
			 0x3997aeb3, /// 0x858
			 0x7880ff39, /// 0x85c
			 0x1b6d1b0c, /// 0x860
			 0x48f19554, /// 0x864
			 0x61750428, /// 0x868
			 0xc0164bbf, /// 0x86c
			 0x73fee06a, /// 0x870
			 0x135f84c9, /// 0x874
			 0xc9015ce5, /// 0x878
			 0xfac1fde1, /// 0x87c
			 0x992977fc, /// 0x880
			 0xb33dec71, /// 0x884
			 0x6c36abb5, /// 0x888
			 0x07e98e04, /// 0x88c
			 0x04e9a7a9, /// 0x890
			 0x86b8183d, /// 0x894
			 0x4866d9fc, /// 0x898
			 0x6a7f1fad, /// 0x89c
			 0x8170e75d, /// 0x8a0
			 0x52394e7a, /// 0x8a4
			 0xfd95bfa4, /// 0x8a8
			 0xa435565b, /// 0x8ac
			 0xf2db923a, /// 0x8b0
			 0xd4c36948, /// 0x8b4
			 0xc50e9e73, /// 0x8b8
			 0x11dd229f, /// 0x8bc
			 0xf5ccec08, /// 0x8c0
			 0xb572e378, /// 0x8c4
			 0x37fd2069, /// 0x8c8
			 0x6d9f3674, /// 0x8cc
			 0x0187840a, /// 0x8d0
			 0x6a21cf70, /// 0x8d4
			 0xba204685, /// 0x8d8
			 0xafaee76c, /// 0x8dc
			 0xff64df7a, /// 0x8e0
			 0x7394a50f, /// 0x8e4
			 0xeb88b4d7, /// 0x8e8
			 0x5ff7de49, /// 0x8ec
			 0x3176f55b, /// 0x8f0
			 0xd5a794fb, /// 0x8f4
			 0x6b26c98a, /// 0x8f8
			 0xcc64e3c6, /// 0x8fc
			 0xe5d437bb, /// 0x900
			 0x28bb8193, /// 0x904
			 0x1358f4d7, /// 0x908
			 0xa49eccb4, /// 0x90c
			 0x6370727b, /// 0x910
			 0x462efaf9, /// 0x914
			 0xf616d555, /// 0x918
			 0x18d31754, /// 0x91c
			 0x1ff80eef, /// 0x920
			 0xe14a4512, /// 0x924
			 0x11560df7, /// 0x928
			 0x501067b0, /// 0x92c
			 0x84f743db, /// 0x930
			 0xafab76c7, /// 0x934
			 0x33314eee, /// 0x938
			 0x6654d2fe, /// 0x93c
			 0x55edf716, /// 0x940
			 0xd884b888, /// 0x944
			 0x05ac9bfe, /// 0x948
			 0x5b9b0cb4, /// 0x94c
			 0xc81dbb66, /// 0x950
			 0x21689e7e, /// 0x954
			 0xb91ab898, /// 0x958
			 0x6edbd10a, /// 0x95c
			 0x8c812f75, /// 0x960
			 0xd524295b, /// 0x964
			 0x3c973279, /// 0x968
			 0x9df6493e, /// 0x96c
			 0x95db28ef, /// 0x970
			 0x7060d318, /// 0x974
			 0x9f624db9, /// 0x978
			 0xe71b504c, /// 0x97c
			 0xcab32766, /// 0x980
			 0x85a86f49, /// 0x984
			 0x390ba6ce, /// 0x988
			 0x303cb7f8, /// 0x98c
			 0x24a065b2, /// 0x990
			 0xed1d4803, /// 0x994
			 0xb111e09c, /// 0x998
			 0xe7b197e2, /// 0x99c
			 0xc08d1346, /// 0x9a0
			 0xd95b6fe4, /// 0x9a4
			 0x336af64f, /// 0x9a8
			 0xbf76ba9b, /// 0x9ac
			 0x59b82ef5, /// 0x9b0
			 0x369bbb7a, /// 0x9b4
			 0x40265863, /// 0x9b8
			 0x2d13f9fb, /// 0x9bc
			 0x8cfc6aca, /// 0x9c0
			 0x95f51980, /// 0x9c4
			 0xb5226da1, /// 0x9c8
			 0xc01ac338, /// 0x9cc
			 0x58f03291, /// 0x9d0
			 0x77ebccef, /// 0x9d4
			 0xb5007394, /// 0x9d8
			 0xd67ed825, /// 0x9dc
			 0x2322e58f, /// 0x9e0
			 0xf32377b8, /// 0x9e4
			 0x1d2e470d, /// 0x9e8
			 0x60dff9e6, /// 0x9ec
			 0xfa645668, /// 0x9f0
			 0x397b7e7f, /// 0x9f4
			 0x5c9ee993, /// 0x9f8
			 0x20bf089b, /// 0x9fc
			 0xe746257d, /// 0xa00
			 0x138cb940, /// 0xa04
			 0x9ebb00b2, /// 0xa08
			 0x91b942a7, /// 0xa0c
			 0x8a998a94, /// 0xa10
			 0x68dd75c1, /// 0xa14
			 0x35b19db1, /// 0xa18
			 0x4cf15519, /// 0xa1c
			 0x567e1a36, /// 0xa20
			 0x2bf07073, /// 0xa24
			 0x8b20c9cb, /// 0xa28
			 0x396abdae, /// 0xa2c
			 0x351a9a48, /// 0xa30
			 0x92dfc106, /// 0xa34
			 0x7842b8ee, /// 0xa38
			 0xe32c2a5d, /// 0xa3c
			 0xd757d820, /// 0xa40
			 0xbc76ce93, /// 0xa44
			 0x591fb788, /// 0xa48
			 0x24d388c1, /// 0xa4c
			 0xfb341fc6, /// 0xa50
			 0x58f5e6cf, /// 0xa54
			 0xd915731c, /// 0xa58
			 0x858bee06, /// 0xa5c
			 0x3d305533, /// 0xa60
			 0xc319c9b7, /// 0xa64
			 0x8e13c8ab, /// 0xa68
			 0x8f8e4056, /// 0xa6c
			 0xafd4dbf8, /// 0xa70
			 0x40016fce, /// 0xa74
			 0xbd2d2095, /// 0xa78
			 0xe6bdcd01, /// 0xa7c
			 0xb93f106f, /// 0xa80
			 0xe8cdbc38, /// 0xa84
			 0x07d4cddf, /// 0xa88
			 0xf77e3029, /// 0xa8c
			 0xa338a8f6, /// 0xa90
			 0x8b68f932, /// 0xa94
			 0x075109e4, /// 0xa98
			 0x86d4d0e7, /// 0xa9c
			 0xd62aab9a, /// 0xaa0
			 0x6fd18d1b, /// 0xaa4
			 0xdfce4da8, /// 0xaa8
			 0x753857b6, /// 0xaac
			 0x7be408fc, /// 0xab0
			 0x141d3131, /// 0xab4
			 0xa235b1a7, /// 0xab8
			 0x89fe5312, /// 0xabc
			 0x0a748f67, /// 0xac0
			 0x99cd1d38, /// 0xac4
			 0x3a9f647f, /// 0xac8
			 0xf270276f, /// 0xacc
			 0x4a3d15ef, /// 0xad0
			 0x6a1de62d, /// 0xad4
			 0xbc67ce14, /// 0xad8
			 0x13691158, /// 0xadc
			 0xc9dfdcd5, /// 0xae0
			 0x6b9371e3, /// 0xae4
			 0x2d017e2e, /// 0xae8
			 0x17c45dc6, /// 0xaec
			 0xf4aaff4f, /// 0xaf0
			 0x69dcec4a, /// 0xaf4
			 0x4c78929a, /// 0xaf8
			 0x28958f92, /// 0xafc
			 0x1f662fc3, /// 0xb00
			 0x27c70eef, /// 0xb04
			 0xbd7c6495, /// 0xb08
			 0xbb6e90a0, /// 0xb0c
			 0x0aab1399, /// 0xb10
			 0x77342ef4, /// 0xb14
			 0x63c1a8e4, /// 0xb18
			 0x72cdb14a, /// 0xb1c
			 0x6222e325, /// 0xb20
			 0xb2bcc366, /// 0xb24
			 0x9c150092, /// 0xb28
			 0x524b88e5, /// 0xb2c
			 0x0d8ebe9b, /// 0xb30
			 0x8e0daee0, /// 0xb34
			 0xf89ed2f9, /// 0xb38
			 0xf8aa2b0c, /// 0xb3c
			 0x8fe45df2, /// 0xb40
			 0xc66cb081, /// 0xb44
			 0x251686ba, /// 0xb48
			 0x356c109c, /// 0xb4c
			 0xc2d813d3, /// 0xb50
			 0xd2649909, /// 0xb54
			 0xe6a73785, /// 0xb58
			 0xb8f9f58c, /// 0xb5c
			 0xe8448cb3, /// 0xb60
			 0x99baf2e3, /// 0xb64
			 0x46dedcff, /// 0xb68
			 0xe2424ae2, /// 0xb6c
			 0xe71bd3e8, /// 0xb70
			 0x6fb15823, /// 0xb74
			 0x4f9d3b82, /// 0xb78
			 0xe94a5069, /// 0xb7c
			 0x62714f31, /// 0xb80
			 0x1556727e, /// 0xb84
			 0x018a2f73, /// 0xb88
			 0x59757bfd, /// 0xb8c
			 0x99072688, /// 0xb90
			 0x5260d404, /// 0xb94
			 0x82f3efa2, /// 0xb98
			 0xdd1febad, /// 0xb9c
			 0x2380851e, /// 0xba0
			 0xfa36d66e, /// 0xba4
			 0xc26a6ee7, /// 0xba8
			 0x3a6974c0, /// 0xbac
			 0x9fe918ca, /// 0xbb0
			 0xaa3f6af5, /// 0xbb4
			 0x070044d9, /// 0xbb8
			 0x404875be, /// 0xbbc
			 0x92434f4a, /// 0xbc0
			 0xf9c199bb, /// 0xbc4
			 0x397defdb, /// 0xbc8
			 0x6c958de1, /// 0xbcc
			 0x4bb534c1, /// 0xbd0
			 0xd903bca7, /// 0xbd4
			 0x157b55a2, /// 0xbd8
			 0x8862f809, /// 0xbdc
			 0xc763aa3e, /// 0xbe0
			 0x07513427, /// 0xbe4
			 0x99a6865e, /// 0xbe8
			 0x0cc0112e, /// 0xbec
			 0x7a7a85ab, /// 0xbf0
			 0xddd33a60, /// 0xbf4
			 0x04a9a8d8, /// 0xbf8
			 0x4e38f8a7, /// 0xbfc
			 0x0fc71e46, /// 0xc00
			 0xaf44389f, /// 0xc04
			 0xbff7324d, /// 0xc08
			 0x625a3fb2, /// 0xc0c
			 0xdd79bbf8, /// 0xc10
			 0x8a4b1dee, /// 0xc14
			 0xcd74bd04, /// 0xc18
			 0xc8df2fdd, /// 0xc1c
			 0x442638f7, /// 0xc20
			 0x35f6d5ee, /// 0xc24
			 0x90bb6f62, /// 0xc28
			 0x6ae2a85f, /// 0xc2c
			 0xa31c5c81, /// 0xc30
			 0x9edcf8dd, /// 0xc34
			 0xc75ff226, /// 0xc38
			 0xffe05d38, /// 0xc3c
			 0x7b7a0412, /// 0xc40
			 0x53508d75, /// 0xc44
			 0xd7b12196, /// 0xc48
			 0xc44a1031, /// 0xc4c
			 0x52aa503e, /// 0xc50
			 0x5f173288, /// 0xc54
			 0x1c1ea480, /// 0xc58
			 0x3d0d4423, /// 0xc5c
			 0xba73afa2, /// 0xc60
			 0x31990174, /// 0xc64
			 0xc300be7a, /// 0xc68
			 0xd4e847bf, /// 0xc6c
			 0xff6949c1, /// 0xc70
			 0xadb537c3, /// 0xc74
			 0xe69be03a, /// 0xc78
			 0x7143b845, /// 0xc7c
			 0x9b81a615, /// 0xc80
			 0x7ed4c1ea, /// 0xc84
			 0xc4d645f0, /// 0xc88
			 0x4a137688, /// 0xc8c
			 0xf63683e0, /// 0xc90
			 0x97d903f5, /// 0xc94
			 0x6a6cd6a0, /// 0xc98
			 0x047c7c13, /// 0xc9c
			 0x5ed88cc7, /// 0xca0
			 0x88ea30d1, /// 0xca4
			 0x2a710d35, /// 0xca8
			 0x3cf3d58f, /// 0xcac
			 0xb020efb7, /// 0xcb0
			 0xecabfd97, /// 0xcb4
			 0x448f1a68, /// 0xcb8
			 0x7102bada, /// 0xcbc
			 0x96ae8a7f, /// 0xcc0
			 0xa3f8d14a, /// 0xcc4
			 0x81de6d07, /// 0xcc8
			 0x53e3208d, /// 0xccc
			 0xefe0aecc, /// 0xcd0
			 0x751031fc, /// 0xcd4
			 0x38e40238, /// 0xcd8
			 0xeef97e67, /// 0xcdc
			 0x16f23e10, /// 0xce0
			 0x77f73e7b, /// 0xce4
			 0x5df03adb, /// 0xce8
			 0xdd13a4d4, /// 0xcec
			 0x170b856d, /// 0xcf0
			 0xfe073f86, /// 0xcf4
			 0x0191f2fb, /// 0xcf8
			 0x0f0af90c, /// 0xcfc
			 0xe40570bd, /// 0xd00
			 0x9c92a57b, /// 0xd04
			 0x799183f4, /// 0xd08
			 0x0d77dcef, /// 0xd0c
			 0x7f8d16c2, /// 0xd10
			 0x2a6e5e47, /// 0xd14
			 0xea253324, /// 0xd18
			 0x91ecf23a, /// 0xd1c
			 0x328a9ab0, /// 0xd20
			 0xcd4dc9cf, /// 0xd24
			 0xd87af691, /// 0xd28
			 0x9319b7ca, /// 0xd2c
			 0x640b5251, /// 0xd30
			 0xcaddf06c, /// 0xd34
			 0x99ce46fc, /// 0xd38
			 0xd084b823, /// 0xd3c
			 0x1d98e926, /// 0xd40
			 0xe742618e, /// 0xd44
			 0x8d6a50b4, /// 0xd48
			 0xa50cb25e, /// 0xd4c
			 0x235ddec8, /// 0xd50
			 0x98ec9c98, /// 0xd54
			 0xa943f658, /// 0xd58
			 0x61d72bf0, /// 0xd5c
			 0xe5cd5738, /// 0xd60
			 0x5c26ae1b, /// 0xd64
			 0xba33314a, /// 0xd68
			 0xebb62ac9, /// 0xd6c
			 0x8484fe9f, /// 0xd70
			 0x95eeaa63, /// 0xd74
			 0xbdb0f494, /// 0xd78
			 0x527cce8b, /// 0xd7c
			 0xdd2dab46, /// 0xd80
			 0x8c968391, /// 0xd84
			 0x64aae9bc, /// 0xd88
			 0x01e3c33e, /// 0xd8c
			 0x07382cc8, /// 0xd90
			 0x39db1c01, /// 0xd94
			 0x0667b70f, /// 0xd98
			 0x224a5f3c, /// 0xd9c
			 0x5693e0a5, /// 0xda0
			 0x38afb723, /// 0xda4
			 0x70c99637, /// 0xda8
			 0x04c0437b, /// 0xdac
			 0x896a0918, /// 0xdb0
			 0x4df2ed02, /// 0xdb4
			 0x37014ac5, /// 0xdb8
			 0xa1b649b8, /// 0xdbc
			 0xe0212f8f, /// 0xdc0
			 0xb7f5a571, /// 0xdc4
			 0x92361a13, /// 0xdc8
			 0xd6827f85, /// 0xdcc
			 0x30011f1d, /// 0xdd0
			 0x4949d996, /// 0xdd4
			 0x14082b81, /// 0xdd8
			 0x78d22d14, /// 0xddc
			 0xc0f58fc6, /// 0xde0
			 0xf4bbd905, /// 0xde4
			 0x7a2d9352, /// 0xde8
			 0x565efd3c, /// 0xdec
			 0x3092a017, /// 0xdf0
			 0x1a841b68, /// 0xdf4
			 0xba4a75ab, /// 0xdf8
			 0xf15996a5, /// 0xdfc
			 0xea327366, /// 0xe00
			 0xd8db7691, /// 0xe04
			 0x1314f3fd, /// 0xe08
			 0x802c3c6b, /// 0xe0c
			 0x59b01cc9, /// 0xe10
			 0xc6181e0b, /// 0xe14
			 0xe86bc928, /// 0xe18
			 0xafcce94b, /// 0xe1c
			 0xfb51c21a, /// 0xe20
			 0xf8433384, /// 0xe24
			 0xdf9d8fd4, /// 0xe28
			 0xa20de331, /// 0xe2c
			 0xea72877c, /// 0xe30
			 0x3a8fa85f, /// 0xe34
			 0x0af9df82, /// 0xe38
			 0xe1921a2e, /// 0xe3c
			 0xcb607dd4, /// 0xe40
			 0x626f981e, /// 0xe44
			 0x2e4b05de, /// 0xe48
			 0x85f1e94e, /// 0xe4c
			 0x75c86980, /// 0xe50
			 0x489b9b00, /// 0xe54
			 0x2788adee, /// 0xe58
			 0x972d976c, /// 0xe5c
			 0xf7a9bb24, /// 0xe60
			 0xe13486ab, /// 0xe64
			 0x7f3abb2e, /// 0xe68
			 0x063f822f, /// 0xe6c
			 0xb972c3b5, /// 0xe70
			 0x071157bc, /// 0xe74
			 0x8752530a, /// 0xe78
			 0x72b6d609, /// 0xe7c
			 0x81b37c08, /// 0xe80
			 0x5ff87309, /// 0xe84
			 0x3bed63a5, /// 0xe88
			 0xf7a0e298, /// 0xe8c
			 0xe917d4c8, /// 0xe90
			 0xbb864360, /// 0xe94
			 0xcd6d1697, /// 0xe98
			 0x3787dfad, /// 0xe9c
			 0xdacdfded, /// 0xea0
			 0x5559fa20, /// 0xea4
			 0x3e2c6340, /// 0xea8
			 0xa3d05b9e, /// 0xeac
			 0x2b60f185, /// 0xeb0
			 0xb3d4a2a3, /// 0xeb4
			 0x10fc5925, /// 0xeb8
			 0x57433bd9, /// 0xebc
			 0x91b4125c, /// 0xec0
			 0x4355690b, /// 0xec4
			 0xa320411f, /// 0xec8
			 0x771f6e63, /// 0xecc
			 0x38420527, /// 0xed0
			 0xa77a4e62, /// 0xed4
			 0xa7672155, /// 0xed8
			 0x9390ae13, /// 0xedc
			 0xae3391d9, /// 0xee0
			 0xb23f35ef, /// 0xee4
			 0x0310772c, /// 0xee8
			 0x8ab55e7b, /// 0xeec
			 0xc05b13fe, /// 0xef0
			 0xff07ff6e, /// 0xef4
			 0xa4eacbe3, /// 0xef8
			 0x44e6e4d1, /// 0xefc
			 0x5a0bbf00, /// 0xf00
			 0x7b5a0496, /// 0xf04
			 0x59bdb6e2, /// 0xf08
			 0x8bc60784, /// 0xf0c
			 0x1340eb36, /// 0xf10
			 0x6fb78af1, /// 0xf14
			 0x77815637, /// 0xf18
			 0x91592aff, /// 0xf1c
			 0xf611f20c, /// 0xf20
			 0xe946e306, /// 0xf24
			 0x5867bcce, /// 0xf28
			 0x97095f24, /// 0xf2c
			 0x005aa2e7, /// 0xf30
			 0x13f29def, /// 0xf34
			 0x3a236568, /// 0xf38
			 0xddae08d7, /// 0xf3c
			 0x9fbffc34, /// 0xf40
			 0x99858af1, /// 0xf44
			 0xc14d90b9, /// 0xf48
			 0xf14380c0, /// 0xf4c
			 0xaebbe9a4, /// 0xf50
			 0xbba848be, /// 0xf54
			 0x63b1a24f, /// 0xf58
			 0xa5a59a22, /// 0xf5c
			 0xa94dc596, /// 0xf60
			 0x8a8668ae, /// 0xf64
			 0x609efeaf, /// 0xf68
			 0x3057ebfd, /// 0xf6c
			 0x8f6b7140, /// 0xf70
			 0x7009f382, /// 0xf74
			 0x83af8916, /// 0xf78
			 0xe763e63c, /// 0xf7c
			 0x6899d6ff, /// 0xf80
			 0xf851c2e7, /// 0xf84
			 0xf5ce8a59, /// 0xf88
			 0x5cb6d067, /// 0xf8c
			 0x5e820be1, /// 0xf90
			 0x99c3903e, /// 0xf94
			 0x2add9768, /// 0xf98
			 0x7eba6ee2, /// 0xf9c
			 0x80fddc0a, /// 0xfa0
			 0x9bd34bcd, /// 0xfa4
			 0x23f30685, /// 0xfa8
			 0xee91dfd0, /// 0xfac
			 0x4fd3051b, /// 0xfb0
			 0x59ffceb9, /// 0xfb4
			 0x088a8b4c, /// 0xfb8
			 0x59cfd065, /// 0xfbc
			 0xf27ea0a0, /// 0xfc0
			 0x60dd28c0, /// 0xfc4
			 0x45ce428c, /// 0xfc8
			 0xc9550309, /// 0xfcc
			 0x0348a54d, /// 0xfd0
			 0xb403f381, /// 0xfd4
			 0x0e360a71, /// 0xfd8
			 0x00e87f4a, /// 0xfdc
			 0xbc0166f3, /// 0xfe0
			 0xb2350402, /// 0xfe4
			 0xabd68dd4, /// 0xfe8
			 0x0b396cde, /// 0xfec
			 0x31fb97d6, /// 0xff0
			 0x3d23fecc, /// 0xff4
			 0xefed3507, /// 0xff8
			 0xa62c01e4 /// last
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
			 0x000001e4,
			 0x00000690,
			 0x00000150,
			 0x000003d8,
			 0x00000494,
			 0x0000038c,
			 0x00000340,
			 0x000000a0,

			 /// vpu register f2
			 0x41800000,
			 0x41f80000,
			 0x41800000,
			 0x41f00000,
			 0x40400000,
			 0x41200000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f3
			 0x3f800000,
			 0x41500000,
			 0x40400000,
			 0x41500000,
			 0x41a00000,
			 0x40e00000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f4
			 0x41a00000,
			 0x41c00000,
			 0x40000000,
			 0x41a80000,
			 0x41700000,
			 0x40000000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f5
			 0x41b80000,
			 0x41f80000,
			 0x41700000,
			 0x42000000,
			 0x41880000,
			 0x41c00000,
			 0x41e80000,
			 0x41200000,

			 /// vpu register f6
			 0x41200000,
			 0x3f800000,
			 0x41500000,
			 0x41a00000,
			 0x41e00000,
			 0x40e00000,
			 0x41100000,
			 0x40800000,

			 /// vpu register f7
			 0x41e00000,
			 0x41000000,
			 0x41f00000,
			 0x41000000,
			 0x40a00000,
			 0x41f00000,
			 0x40000000,
			 0x41d00000,

			 /// vpu register f8
			 0x41f00000,
			 0x41d00000,
			 0x41f80000,
			 0x41200000,
			 0x41d00000,
			 0x41f80000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f9
			 0x40800000,
			 0x41700000,
			 0x42000000,
			 0x3f800000,
			 0x41200000,
			 0x40e00000,
			 0x40c00000,
			 0x41980000,

			 /// vpu register f10
			 0x42000000,
			 0x41b00000,
			 0x41800000,
			 0x40e00000,
			 0x41800000,
			 0x41300000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f11
			 0x41980000,
			 0x41500000,
			 0x41d80000,
			 0x40000000,
			 0x40000000,
			 0x41a00000,
			 0x41500000,
			 0x41700000,

			 /// vpu register f12
			 0x41900000,
			 0x41000000,
			 0x42000000,
			 0x41600000,
			 0x41000000,
			 0x41900000,
			 0x41c80000,
			 0x41500000,

			 /// vpu register f13
			 0x41b00000,
			 0x40c00000,
			 0x41b80000,
			 0x41200000,
			 0x3f800000,
			 0x41200000,
			 0x41500000,
			 0x41d80000,

			 /// vpu register f14
			 0x41d00000,
			 0x40c00000,
			 0x40400000,
			 0x41600000,
			 0x41900000,
			 0x41100000,
			 0x41000000,
			 0x41b80000,

			 /// vpu register f15
			 0x41800000,
			 0x41d80000,
			 0x41e80000,
			 0x3f800000,
			 0x40000000,
			 0x40000000,
			 0x41f00000,
			 0x41d00000,

			 /// vpu register f16
			 0x41980000,
			 0x41400000,
			 0x40400000,
			 0x41900000,
			 0x41e00000,
			 0x41600000,
			 0x41a00000,
			 0x41f80000,

			 /// vpu register f17
			 0x41a00000,
			 0x40a00000,
			 0x41980000,
			 0x40000000,
			 0x41100000,
			 0x41b00000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f18
			 0x41500000,
			 0x40e00000,
			 0x41e00000,
			 0x41880000,
			 0x40e00000,
			 0x41300000,
			 0x40a00000,
			 0x3f800000,

			 /// vpu register f19
			 0x41f80000,
			 0x41700000,
			 0x40400000,
			 0x40800000,
			 0x41200000,
			 0x41900000,
			 0x41a80000,
			 0x41600000,

			 /// vpu register f20
			 0x41200000,
			 0x41100000,
			 0x41c00000,
			 0x40000000,
			 0x41a00000,
			 0x41400000,
			 0x40e00000,
			 0x41400000,

			 /// vpu register f21
			 0x41d00000,
			 0x41e80000,
			 0x41a80000,
			 0x40800000,
			 0x41600000,
			 0x41100000,
			 0x41800000,
			 0x41c00000,

			 /// vpu register f22
			 0x41c80000,
			 0x42000000,
			 0x41300000,
			 0x41980000,
			 0x41600000,
			 0x40400000,
			 0x41e80000,
			 0x41f80000,

			 /// vpu register f23
			 0x41000000,
			 0x40c00000,
			 0x40800000,
			 0x41980000,
			 0x41100000,
			 0x41d80000,
			 0x40c00000,
			 0x41700000,

			 /// vpu register f24
			 0x41880000,
			 0x42000000,
			 0x41e80000,
			 0x42000000,
			 0x40000000,
			 0x41500000,
			 0x41c80000,
			 0x41c80000,

			 /// vpu register f25
			 0x41f80000,
			 0x41000000,
			 0x40800000,
			 0x41300000,
			 0x41f00000,
			 0x41e80000,
			 0x3f800000,
			 0x40e00000,

			 /// vpu register f26
			 0x41200000,
			 0x41200000,
			 0x41c00000,
			 0x41980000,
			 0x41700000,
			 0x41900000,
			 0x41b00000,
			 0x41d00000,

			 /// vpu register f27
			 0x40000000,
			 0x41f80000,
			 0x40400000,
			 0x41f00000,
			 0x41f80000,
			 0x41b00000,
			 0x41800000,
			 0x40400000,

			 /// vpu register f28
			 0x41f00000,
			 0x41900000,
			 0x40400000,
			 0x41600000,
			 0x41980000,
			 0x41600000,
			 0x41a80000,
			 0x41500000,

			 /// vpu register f29
			 0x41b80000,
			 0x41300000,
			 0x41400000,
			 0x40c00000,
			 0x41a00000,
			 0x41d00000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f30
			 0x40a00000,
			 0x40e00000,
			 0x41c80000,
			 0x40400000,
			 0x40a00000,
			 0x41a80000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f31
			 0x41e80000,
			 0x41d80000,
			 0x41880000,
			 0x41a00000,
			 0x41d80000,
			 0x41a80000,
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
		"feq.ps f18, f0, f18\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f27, f12, f21\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f26, f22\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f20, f12, f0\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f5, f21, f7\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f15, f15, f2\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f7, f15\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f29, f0, f14\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f5, f6, f0\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f25, f4, f20\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f5, f12, f28\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f17, f14, f11\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f8, f6, f8\n"                                 ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f9, f26, f20\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f0, f12\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f30, f6, f17\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f20, f0, f28\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f30, f18, f27\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f27, f10, f28\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f22, f12, f26\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f19, f3, f26\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f7, f17, f27\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f12, f20, f4\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f3, f12, f1\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f16, f18, f6\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f11, f14, f10\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f13, f2, f11\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f24, f13, f17\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f13, f8\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f5, f17, f2\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f16, f9\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f3, f7, f7\n"                                 ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f20, f18, f22\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f22, f14, f27\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f8, f21, f25\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f11, f28, f30\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f18, f21, f5\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f8, f30, f20\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f23, f10, f19\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f12, f4, f0\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f24, f9, f0\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f11, f1, f2\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f25, f11, f13\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f19, f22, f25\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f21, f27, f13\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f2, f15, f11\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f19, f18, f9\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f4, f20, f17\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f19, f26, f30\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f23, f15, f3\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f10, f10, f9\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f22, f4, f24\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f21, f26\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f19, f16, f24\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f9, f28, f11\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f24, f19, f30\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f11, f18, f19\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f19, f16, f27\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f11, f4, f2\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f29, f30, f30\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f17, f5, f19\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f3, f12, f9\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f22, f10, f12\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f26, f13, f12\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f12, f27\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f15, f18, f29\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f4, f5, f22\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f16, f17, f1\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f30, f10, f14\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f14, f27, f8\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f21, f18, f21\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f22, f20, f24\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f24, f9, f23\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f30, f25, f26\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f3, f4, f1\n"                                 ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f21, f28, f21\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f25, f4, f9\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f16, f22, f18\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f16, f0, f7\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f27, f24, f27\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f2, f28, f14\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f26, f27, f20\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f17, f15, f27\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f15, f25, f13\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f13, f16, f28\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f10, f13, f5\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f25, f4, f25\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f28, f27, f17\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f7, f4, f6\n"                                 ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f4, f2, f16\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f17, f24, f26\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f4, f10, f13\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f20, f17, f3\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f6, f29, f23\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f16, f25, f17\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f5, f7, f28\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f25, f23, f23\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f16, f12, f1\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feq.ps f20, f25, f21\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
