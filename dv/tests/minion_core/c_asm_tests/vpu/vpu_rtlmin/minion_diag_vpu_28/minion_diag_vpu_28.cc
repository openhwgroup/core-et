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
			 0x75d72059, /// 0x0
			 0x581777f0, /// 0x4
			 0x0ada1a45, /// 0x8
			 0x8eecf7f4, /// 0xc
			 0xa3ad814e, /// 0x10
			 0x1f097eeb, /// 0x14
			 0x5bfafbbc, /// 0x18
			 0x9289ebbe, /// 0x1c
			 0x43cb7e10, /// 0x20
			 0x251c7f1e, /// 0x24
			 0x727ee5c7, /// 0x28
			 0xa41ccfc0, /// 0x2c
			 0x8326bcec, /// 0x30
			 0xae565845, /// 0x34
			 0xcead11fc, /// 0x38
			 0x22d8e069, /// 0x3c
			 0x4ec3a35e, /// 0x40
			 0xec407a78, /// 0x44
			 0xb1b9fc9e, /// 0x48
			 0xcbb138f5, /// 0x4c
			 0x4ffc1fed, /// 0x50
			 0xf84ab9da, /// 0x54
			 0x4edf76ec, /// 0x58
			 0xa3239943, /// 0x5c
			 0x215bdcd6, /// 0x60
			 0x12c70573, /// 0x64
			 0x98493a6d, /// 0x68
			 0x024b1e70, /// 0x6c
			 0x676d2986, /// 0x70
			 0x36ad31f4, /// 0x74
			 0xcaad9899, /// 0x78
			 0xbd2379f3, /// 0x7c
			 0x69b7c953, /// 0x80
			 0x87950764, /// 0x84
			 0x2e1847fa, /// 0x88
			 0xc455a039, /// 0x8c
			 0xbbce8446, /// 0x90
			 0xd6755ead, /// 0x94
			 0x8eaffd9a, /// 0x98
			 0xf0c86eae, /// 0x9c
			 0x0f7c5656, /// 0xa0
			 0xe5350e49, /// 0xa4
			 0xa68a2715, /// 0xa8
			 0x0b0c0759, /// 0xac
			 0x7367aeef, /// 0xb0
			 0x14a5afc3, /// 0xb4
			 0x00142cae, /// 0xb8
			 0x2ffc5ae5, /// 0xbc
			 0x0f86d97b, /// 0xc0
			 0x849512be, /// 0xc4
			 0x17830cae, /// 0xc8
			 0x315a348e, /// 0xcc
			 0xfc6ecb7f, /// 0xd0
			 0x814bc04c, /// 0xd4
			 0xd492a92f, /// 0xd8
			 0xb104017d, /// 0xdc
			 0x6264a269, /// 0xe0
			 0xfd6e838e, /// 0xe4
			 0x4ab2bb13, /// 0xe8
			 0x54d89aef, /// 0xec
			 0x2f454628, /// 0xf0
			 0x0add2030, /// 0xf4
			 0x0650599b, /// 0xf8
			 0x56b7ccfd, /// 0xfc
			 0xbe37bed4, /// 0x100
			 0x23e400c8, /// 0x104
			 0x20690fb4, /// 0x108
			 0x9929a843, /// 0x10c
			 0xef3436f0, /// 0x110
			 0xb22c78d0, /// 0x114
			 0xc9af2ee4, /// 0x118
			 0x8282329d, /// 0x11c
			 0x95601c69, /// 0x120
			 0x7bd0cab1, /// 0x124
			 0xe623539a, /// 0x128
			 0x1c3984c9, /// 0x12c
			 0x0bc2667b, /// 0x130
			 0x5460f9ec, /// 0x134
			 0xfc76cebe, /// 0x138
			 0xd5448273, /// 0x13c
			 0x8a3cad01, /// 0x140
			 0xe49ae890, /// 0x144
			 0x1be1ca72, /// 0x148
			 0x8a48a384, /// 0x14c
			 0x513ab81d, /// 0x150
			 0x63727185, /// 0x154
			 0x3a947bbe, /// 0x158
			 0x22a3163c, /// 0x15c
			 0xa59424ba, /// 0x160
			 0x6d44cab2, /// 0x164
			 0xf8136ca3, /// 0x168
			 0xe6cace83, /// 0x16c
			 0xd825fa5d, /// 0x170
			 0xcc62583a, /// 0x174
			 0x0347fda0, /// 0x178
			 0x4f365362, /// 0x17c
			 0xfc010df4, /// 0x180
			 0x9ee98bbf, /// 0x184
			 0xa7696381, /// 0x188
			 0xa853932d, /// 0x18c
			 0xe818f99a, /// 0x190
			 0x3ee20369, /// 0x194
			 0x4c9a9b5a, /// 0x198
			 0x8a76d0f5, /// 0x19c
			 0xde7d0ad3, /// 0x1a0
			 0x57d690e7, /// 0x1a4
			 0xb03f1ab4, /// 0x1a8
			 0x8728c1ff, /// 0x1ac
			 0x0bd34301, /// 0x1b0
			 0xcb918b4b, /// 0x1b4
			 0x2c123987, /// 0x1b8
			 0xb3052460, /// 0x1bc
			 0x7862dc01, /// 0x1c0
			 0x0d494227, /// 0x1c4
			 0xd0803947, /// 0x1c8
			 0x94b3f0c8, /// 0x1cc
			 0xa70fed80, /// 0x1d0
			 0xef4131ee, /// 0x1d4
			 0xba07344d, /// 0x1d8
			 0xeab306af, /// 0x1dc
			 0x80a96ef5, /// 0x1e0
			 0x9defb7fc, /// 0x1e4
			 0x676737fd, /// 0x1e8
			 0x2c65dd92, /// 0x1ec
			 0x02d34be1, /// 0x1f0
			 0x08f219d0, /// 0x1f4
			 0x08fad80a, /// 0x1f8
			 0xc5d12f41, /// 0x1fc
			 0x360ce36b, /// 0x200
			 0x3131ca7c, /// 0x204
			 0xc29eb7f5, /// 0x208
			 0x5a4620f5, /// 0x20c
			 0x806ab816, /// 0x210
			 0xd064639a, /// 0x214
			 0x52bb5ed0, /// 0x218
			 0xeae4cc4c, /// 0x21c
			 0x70c6d2ab, /// 0x220
			 0x5f06fbf8, /// 0x224
			 0x24319ac1, /// 0x228
			 0x0f385411, /// 0x22c
			 0x81fcd2c2, /// 0x230
			 0x6b16ddbe, /// 0x234
			 0xddea2dd3, /// 0x238
			 0xafd6a323, /// 0x23c
			 0xf7520093, /// 0x240
			 0x0d009139, /// 0x244
			 0x62aee854, /// 0x248
			 0xf13be697, /// 0x24c
			 0x22d76cb7, /// 0x250
			 0x1aaa9988, /// 0x254
			 0x4a2efcd7, /// 0x258
			 0xbd750bb0, /// 0x25c
			 0x02ddadfb, /// 0x260
			 0x9bf6f927, /// 0x264
			 0x3f45250b, /// 0x268
			 0xd6fc6e0b, /// 0x26c
			 0xd6e0db0f, /// 0x270
			 0xcdcb5375, /// 0x274
			 0x79488397, /// 0x278
			 0xa4f8fcb4, /// 0x27c
			 0x1e952684, /// 0x280
			 0x1750c426, /// 0x284
			 0x07534f84, /// 0x288
			 0x47ef656d, /// 0x28c
			 0x45c8e84b, /// 0x290
			 0x9fda67b6, /// 0x294
			 0x37d157df, /// 0x298
			 0xbc676817, /// 0x29c
			 0x79bd5969, /// 0x2a0
			 0xf97e7d81, /// 0x2a4
			 0x1b5a82a8, /// 0x2a8
			 0x0783e7fe, /// 0x2ac
			 0xddbb1df8, /// 0x2b0
			 0x4ab77a3c, /// 0x2b4
			 0x01d6bb2a, /// 0x2b8
			 0xe43e1d6d, /// 0x2bc
			 0x699a7588, /// 0x2c0
			 0x76d571cc, /// 0x2c4
			 0xf3a15ebb, /// 0x2c8
			 0x5cd4ded2, /// 0x2cc
			 0xc60eae96, /// 0x2d0
			 0x19fd2d42, /// 0x2d4
			 0x434db7f2, /// 0x2d8
			 0xf12e122b, /// 0x2dc
			 0xc820072c, /// 0x2e0
			 0xc90d166d, /// 0x2e4
			 0xfe53992a, /// 0x2e8
			 0xdd65d0e2, /// 0x2ec
			 0x6416242e, /// 0x2f0
			 0xf19dbb62, /// 0x2f4
			 0x5f5993ac, /// 0x2f8
			 0x84d2c7c1, /// 0x2fc
			 0x2a4e2f7f, /// 0x300
			 0xd396ea4b, /// 0x304
			 0x69cb3f14, /// 0x308
			 0xcf654fb9, /// 0x30c
			 0xf5c5c2a3, /// 0x310
			 0x365d6094, /// 0x314
			 0x7da94cb0, /// 0x318
			 0x47a7f577, /// 0x31c
			 0x49493e81, /// 0x320
			 0xccb4437e, /// 0x324
			 0x0182fea3, /// 0x328
			 0xc2dccc3c, /// 0x32c
			 0xf0084ff3, /// 0x330
			 0xc37ace3e, /// 0x334
			 0xb98fb3aa, /// 0x338
			 0x0477ae21, /// 0x33c
			 0xd8eb8363, /// 0x340
			 0x1ceb69a7, /// 0x344
			 0xb8e68196, /// 0x348
			 0xcb6bfa7d, /// 0x34c
			 0x4488afd5, /// 0x350
			 0xc963d4b2, /// 0x354
			 0xa29274cb, /// 0x358
			 0x9ce53200, /// 0x35c
			 0x0ced5917, /// 0x360
			 0xd8dc7503, /// 0x364
			 0x4d0eb114, /// 0x368
			 0xe1a9077c, /// 0x36c
			 0x9b679d4c, /// 0x370
			 0xb931695f, /// 0x374
			 0xb3787be9, /// 0x378
			 0x5fe8e73e, /// 0x37c
			 0x78644075, /// 0x380
			 0x5f05b46e, /// 0x384
			 0x7e7a7cff, /// 0x388
			 0x5e664255, /// 0x38c
			 0x650dc3f5, /// 0x390
			 0x959217ed, /// 0x394
			 0xb2294d23, /// 0x398
			 0xc5b55c64, /// 0x39c
			 0x1db11eb5, /// 0x3a0
			 0x0fde2e25, /// 0x3a4
			 0x0d3b9c71, /// 0x3a8
			 0x7585b623, /// 0x3ac
			 0x4e4f2d20, /// 0x3b0
			 0x61a8dd6b, /// 0x3b4
			 0xc63547e5, /// 0x3b8
			 0xe2049082, /// 0x3bc
			 0x7ff697e8, /// 0x3c0
			 0xf523f7fe, /// 0x3c4
			 0xe2e0448f, /// 0x3c8
			 0x861163bc, /// 0x3cc
			 0xb28a41e7, /// 0x3d0
			 0xde27973a, /// 0x3d4
			 0xcea1623c, /// 0x3d8
			 0xd239fa68, /// 0x3dc
			 0xf4daaddf, /// 0x3e0
			 0xbe1bfa9f, /// 0x3e4
			 0x4f8bf785, /// 0x3e8
			 0x9f164125, /// 0x3ec
			 0x99d7fa9f, /// 0x3f0
			 0xb1c47a44, /// 0x3f4
			 0x77009875, /// 0x3f8
			 0xf9d6fd05, /// 0x3fc
			 0xf2fbbad4, /// 0x400
			 0xe35dc448, /// 0x404
			 0xd056aece, /// 0x408
			 0x9d965ba8, /// 0x40c
			 0xd58ce532, /// 0x410
			 0xd0211cda, /// 0x414
			 0x3c143c9a, /// 0x418
			 0x0a7550a5, /// 0x41c
			 0xcf32edc7, /// 0x420
			 0x67a0ab98, /// 0x424
			 0xb8df406b, /// 0x428
			 0xfdf2af92, /// 0x42c
			 0x7ef60b47, /// 0x430
			 0xf7b1fb4d, /// 0x434
			 0xa551f82e, /// 0x438
			 0x5a3da4a9, /// 0x43c
			 0x7f390c52, /// 0x440
			 0xba2626f7, /// 0x444
			 0xe591fd82, /// 0x448
			 0x5c8044e0, /// 0x44c
			 0x1bb30091, /// 0x450
			 0x0b73a3ab, /// 0x454
			 0xa21834e3, /// 0x458
			 0x95a8c223, /// 0x45c
			 0x00d25556, /// 0x460
			 0xe22b0ac4, /// 0x464
			 0xf8bb3090, /// 0x468
			 0x763565e5, /// 0x46c
			 0xb56413f3, /// 0x470
			 0x835e28f8, /// 0x474
			 0xcb323e4d, /// 0x478
			 0x33edaa07, /// 0x47c
			 0x19aaa606, /// 0x480
			 0x65840ac1, /// 0x484
			 0xf02730d8, /// 0x488
			 0x04344367, /// 0x48c
			 0x2e22978e, /// 0x490
			 0x955480bf, /// 0x494
			 0xaeb8d053, /// 0x498
			 0x4f9d9172, /// 0x49c
			 0x271fe3db, /// 0x4a0
			 0x4fb28426, /// 0x4a4
			 0xa6911a43, /// 0x4a8
			 0x982a412b, /// 0x4ac
			 0x40394c78, /// 0x4b0
			 0x6baa9cee, /// 0x4b4
			 0x678fce71, /// 0x4b8
			 0x2d2bdefc, /// 0x4bc
			 0x87154713, /// 0x4c0
			 0x94145528, /// 0x4c4
			 0x533d096b, /// 0x4c8
			 0x2e65bc6d, /// 0x4cc
			 0x7653dcc6, /// 0x4d0
			 0x01a94490, /// 0x4d4
			 0x4c59cac3, /// 0x4d8
			 0x07004b2f, /// 0x4dc
			 0x42375941, /// 0x4e0
			 0xe5d01833, /// 0x4e4
			 0x938fe10f, /// 0x4e8
			 0x65abd175, /// 0x4ec
			 0x47b2ce93, /// 0x4f0
			 0x8024e436, /// 0x4f4
			 0x4a8b311c, /// 0x4f8
			 0x4bc5651b, /// 0x4fc
			 0x3fc074bb, /// 0x500
			 0x1b813636, /// 0x504
			 0x14ac01c6, /// 0x508
			 0xea26d97f, /// 0x50c
			 0x5619312a, /// 0x510
			 0xa0700f80, /// 0x514
			 0xf65a9733, /// 0x518
			 0xffb27a9f, /// 0x51c
			 0xd88275c7, /// 0x520
			 0xeb08742d, /// 0x524
			 0x4d21e2a9, /// 0x528
			 0xdaafdbfa, /// 0x52c
			 0x7adcd821, /// 0x530
			 0xa2f0d6b0, /// 0x534
			 0x03432176, /// 0x538
			 0xe82548be, /// 0x53c
			 0x6bb7a119, /// 0x540
			 0x7f0c9b5e, /// 0x544
			 0xb3fd9050, /// 0x548
			 0x9321044d, /// 0x54c
			 0xd9f8b69b, /// 0x550
			 0xbdd8fb9c, /// 0x554
			 0xe01af57c, /// 0x558
			 0xe5c17451, /// 0x55c
			 0x5cef9812, /// 0x560
			 0xc174721e, /// 0x564
			 0x023b6837, /// 0x568
			 0x694a6a11, /// 0x56c
			 0x4c4da7b7, /// 0x570
			 0x8e8c7a6c, /// 0x574
			 0x1f2ce44e, /// 0x578
			 0x1f28fd7c, /// 0x57c
			 0x7638e748, /// 0x580
			 0xe71f5c48, /// 0x584
			 0x6fb772ab, /// 0x588
			 0x8758e6e0, /// 0x58c
			 0x258b1130, /// 0x590
			 0xf2ded391, /// 0x594
			 0x06d6e8dc, /// 0x598
			 0x4c0eeb47, /// 0x59c
			 0x0976851e, /// 0x5a0
			 0x3f5d1f21, /// 0x5a4
			 0x50a86b92, /// 0x5a8
			 0x5cbf0b73, /// 0x5ac
			 0x4c75c006, /// 0x5b0
			 0x4f1e0389, /// 0x5b4
			 0xab713eef, /// 0x5b8
			 0xa53ddec2, /// 0x5bc
			 0xdd993e21, /// 0x5c0
			 0x453d73d9, /// 0x5c4
			 0x9459a09c, /// 0x5c8
			 0xaae9242e, /// 0x5cc
			 0x322e2712, /// 0x5d0
			 0x86df57c8, /// 0x5d4
			 0xd834f394, /// 0x5d8
			 0x3fd1d45f, /// 0x5dc
			 0x4c8e33c9, /// 0x5e0
			 0x2834a5c2, /// 0x5e4
			 0x1630b8e9, /// 0x5e8
			 0xbd5a0c10, /// 0x5ec
			 0xcf2dcfc8, /// 0x5f0
			 0x08dca016, /// 0x5f4
			 0x88e728bf, /// 0x5f8
			 0x052826f0, /// 0x5fc
			 0xca7ac9bc, /// 0x600
			 0x47f06611, /// 0x604
			 0xdfb9ae45, /// 0x608
			 0xcca69c76, /// 0x60c
			 0xc8dc2284, /// 0x610
			 0xd8a84291, /// 0x614
			 0x5c935b82, /// 0x618
			 0x746442aa, /// 0x61c
			 0xcf6db82c, /// 0x620
			 0x2e9ce6a5, /// 0x624
			 0x37cbd8e8, /// 0x628
			 0xe2957bda, /// 0x62c
			 0x6a6eee9d, /// 0x630
			 0x5c9d11e9, /// 0x634
			 0xdc28c95d, /// 0x638
			 0x1c7cfedf, /// 0x63c
			 0x8c98e228, /// 0x640
			 0xdeab7233, /// 0x644
			 0x9c797549, /// 0x648
			 0x9133688c, /// 0x64c
			 0x87b4f811, /// 0x650
			 0x1a38444e, /// 0x654
			 0xc837abd2, /// 0x658
			 0x63c06451, /// 0x65c
			 0xd5df5619, /// 0x660
			 0xa8e2c918, /// 0x664
			 0x8df9e0fe, /// 0x668
			 0x18367ad6, /// 0x66c
			 0xb1f7c21a, /// 0x670
			 0xb776b22b, /// 0x674
			 0x99b53c3a, /// 0x678
			 0x1e863b43, /// 0x67c
			 0x43d290cb, /// 0x680
			 0x0921d0bb, /// 0x684
			 0x6cdf4788, /// 0x688
			 0x4fc187e6, /// 0x68c
			 0xd04c9441, /// 0x690
			 0x7ac66972, /// 0x694
			 0x61079dcf, /// 0x698
			 0xac61dcc9, /// 0x69c
			 0xcf63cec6, /// 0x6a0
			 0xd476c54b, /// 0x6a4
			 0x5440af86, /// 0x6a8
			 0x9219370e, /// 0x6ac
			 0x0ce9c02f, /// 0x6b0
			 0x43e7da90, /// 0x6b4
			 0x3f4d02c8, /// 0x6b8
			 0x87e76a12, /// 0x6bc
			 0xc97c28ff, /// 0x6c0
			 0x9a34fe75, /// 0x6c4
			 0x5234e14f, /// 0x6c8
			 0x02e4a958, /// 0x6cc
			 0x6997847b, /// 0x6d0
			 0xacdb0b2c, /// 0x6d4
			 0xfb25fa0f, /// 0x6d8
			 0xdd13e91b, /// 0x6dc
			 0xcc2aa45a, /// 0x6e0
			 0x89d0d1c0, /// 0x6e4
			 0xe83e2a10, /// 0x6e8
			 0x6d3b597a, /// 0x6ec
			 0x26e1fff7, /// 0x6f0
			 0x341887fb, /// 0x6f4
			 0xd6c0db59, /// 0x6f8
			 0x192fc3b3, /// 0x6fc
			 0xf2643782, /// 0x700
			 0x8288673f, /// 0x704
			 0x9f32631d, /// 0x708
			 0x59db689b, /// 0x70c
			 0x327ff0c2, /// 0x710
			 0xa1ecd865, /// 0x714
			 0x17015303, /// 0x718
			 0xb89a353b, /// 0x71c
			 0x18615f93, /// 0x720
			 0x68dc2177, /// 0x724
			 0xb7461a5d, /// 0x728
			 0x73fddf00, /// 0x72c
			 0x43ffa298, /// 0x730
			 0xa6701676, /// 0x734
			 0xde09ecf8, /// 0x738
			 0x78eacbe8, /// 0x73c
			 0x29d21d7d, /// 0x740
			 0x61624b77, /// 0x744
			 0x097b73ba, /// 0x748
			 0x4a596b56, /// 0x74c
			 0xe072e895, /// 0x750
			 0x10e963e8, /// 0x754
			 0x5cee07ad, /// 0x758
			 0x54c69b79, /// 0x75c
			 0xc8c93b36, /// 0x760
			 0x56c70de0, /// 0x764
			 0x4bbb794d, /// 0x768
			 0xcd5f9e81, /// 0x76c
			 0x5e7763fc, /// 0x770
			 0xc26cd7e3, /// 0x774
			 0x13990950, /// 0x778
			 0x5b87387a, /// 0x77c
			 0xfba3122b, /// 0x780
			 0x46245fc0, /// 0x784
			 0x24ae2a5a, /// 0x788
			 0xab9f7ffb, /// 0x78c
			 0x59094d0f, /// 0x790
			 0x936801ae, /// 0x794
			 0xd5176987, /// 0x798
			 0x5724eb18, /// 0x79c
			 0xc8412358, /// 0x7a0
			 0x5c5f251a, /// 0x7a4
			 0x77008a90, /// 0x7a8
			 0x32a7c6a2, /// 0x7ac
			 0x8d4ff8c0, /// 0x7b0
			 0x93844829, /// 0x7b4
			 0xb4ec82e2, /// 0x7b8
			 0x0759d48b, /// 0x7bc
			 0x54f28b09, /// 0x7c0
			 0x92deecf4, /// 0x7c4
			 0x6ce0a921, /// 0x7c8
			 0x144736b3, /// 0x7cc
			 0x368f00bb, /// 0x7d0
			 0x26f746c5, /// 0x7d4
			 0x0f5ba572, /// 0x7d8
			 0xc90ec38a, /// 0x7dc
			 0xea625db3, /// 0x7e0
			 0x67538094, /// 0x7e4
			 0x99a7cbd5, /// 0x7e8
			 0x70cbf33e, /// 0x7ec
			 0x8624c7d0, /// 0x7f0
			 0x56400359, /// 0x7f4
			 0xe3464875, /// 0x7f8
			 0xdd2ef1a5, /// 0x7fc
			 0x61f24069, /// 0x800
			 0x34ddf394, /// 0x804
			 0xda8718a7, /// 0x808
			 0x4d1e88ae, /// 0x80c
			 0x127a644a, /// 0x810
			 0x5a74db0a, /// 0x814
			 0x81e70460, /// 0x818
			 0xaa604653, /// 0x81c
			 0x1d71e802, /// 0x820
			 0x9f339f53, /// 0x824
			 0x50570c00, /// 0x828
			 0x69cf74c3, /// 0x82c
			 0x0b9f5f24, /// 0x830
			 0xcb5d1b23, /// 0x834
			 0x2af94d48, /// 0x838
			 0x4049743c, /// 0x83c
			 0x15d568ca, /// 0x840
			 0x39e91d2d, /// 0x844
			 0xd8554906, /// 0x848
			 0xdf27e1da, /// 0x84c
			 0xbce334a5, /// 0x850
			 0x928712c8, /// 0x854
			 0x175a4f66, /// 0x858
			 0xa7276058, /// 0x85c
			 0x4531b803, /// 0x860
			 0xe646dbd6, /// 0x864
			 0x79ebd891, /// 0x868
			 0xb44244db, /// 0x86c
			 0x64db459f, /// 0x870
			 0xc06a3a71, /// 0x874
			 0x78ced49d, /// 0x878
			 0xda696e3e, /// 0x87c
			 0xd7a16f9b, /// 0x880
			 0xa4e5b4f0, /// 0x884
			 0x84ef9f9d, /// 0x888
			 0x56de94bd, /// 0x88c
			 0x8a794a16, /// 0x890
			 0x88d43247, /// 0x894
			 0x282b8a19, /// 0x898
			 0x8945fe95, /// 0x89c
			 0xee7d086e, /// 0x8a0
			 0xc808ff0f, /// 0x8a4
			 0x2a2939f6, /// 0x8a8
			 0x44bbe707, /// 0x8ac
			 0xe7c3c44a, /// 0x8b0
			 0x3b0c3328, /// 0x8b4
			 0xff510e85, /// 0x8b8
			 0xd31802f6, /// 0x8bc
			 0xfee65b79, /// 0x8c0
			 0xbe0a8114, /// 0x8c4
			 0x1c384bd9, /// 0x8c8
			 0xf87db877, /// 0x8cc
			 0x9fa79bec, /// 0x8d0
			 0xd4beedac, /// 0x8d4
			 0x705675e5, /// 0x8d8
			 0x35e73f9f, /// 0x8dc
			 0xd7964d24, /// 0x8e0
			 0x7b3a0028, /// 0x8e4
			 0xddbe1c2b, /// 0x8e8
			 0xa483a170, /// 0x8ec
			 0x939cd87f, /// 0x8f0
			 0x075ea36c, /// 0x8f4
			 0xc2df1b5b, /// 0x8f8
			 0x7c66244e, /// 0x8fc
			 0xf6882fed, /// 0x900
			 0x8d41fe0a, /// 0x904
			 0xc02dfd7a, /// 0x908
			 0xdcdebe49, /// 0x90c
			 0x23f2400b, /// 0x910
			 0x21be8b88, /// 0x914
			 0x0c9d54e4, /// 0x918
			 0x69c73869, /// 0x91c
			 0x453e2160, /// 0x920
			 0xdd335c40, /// 0x924
			 0xfe1444f6, /// 0x928
			 0x574bae47, /// 0x92c
			 0x33c049ce, /// 0x930
			 0x2f99fb41, /// 0x934
			 0xcd5c6c68, /// 0x938
			 0x53c80006, /// 0x93c
			 0x48b1faf7, /// 0x940
			 0x31776ce0, /// 0x944
			 0xcf30102a, /// 0x948
			 0xbfb3600f, /// 0x94c
			 0x1e9dfaad, /// 0x950
			 0x5c515619, /// 0x954
			 0xf004a505, /// 0x958
			 0x749dfe0a, /// 0x95c
			 0xb5c8059d, /// 0x960
			 0x057133e6, /// 0x964
			 0xcf58bd1b, /// 0x968
			 0x9c88fcaa, /// 0x96c
			 0x31e8cd91, /// 0x970
			 0x88952c43, /// 0x974
			 0x054adba8, /// 0x978
			 0xb53c9361, /// 0x97c
			 0x07cd8f17, /// 0x980
			 0xadc744ab, /// 0x984
			 0xb544af41, /// 0x988
			 0x876d63da, /// 0x98c
			 0x1055a390, /// 0x990
			 0x3911b25d, /// 0x994
			 0x50e4319d, /// 0x998
			 0x5bf12a0e, /// 0x99c
			 0xde950a9d, /// 0x9a0
			 0x080f998a, /// 0x9a4
			 0xc65a3efe, /// 0x9a8
			 0x4b395e68, /// 0x9ac
			 0x0d4a0202, /// 0x9b0
			 0xf9196eb9, /// 0x9b4
			 0x2dea3397, /// 0x9b8
			 0x56c89a53, /// 0x9bc
			 0xe589cef5, /// 0x9c0
			 0xa14e8b1a, /// 0x9c4
			 0x5099a28e, /// 0x9c8
			 0xaef03d9d, /// 0x9cc
			 0x7a47fb4f, /// 0x9d0
			 0x97aba478, /// 0x9d4
			 0x126ca212, /// 0x9d8
			 0x55876985, /// 0x9dc
			 0xb0806d2a, /// 0x9e0
			 0x45656a4d, /// 0x9e4
			 0x38de2df9, /// 0x9e8
			 0x68d3cccb, /// 0x9ec
			 0x41394ca6, /// 0x9f0
			 0xce89a189, /// 0x9f4
			 0x6d4564c0, /// 0x9f8
			 0x2798ef17, /// 0x9fc
			 0x6ea720d6, /// 0xa00
			 0xbb21e0a8, /// 0xa04
			 0x026f8681, /// 0xa08
			 0x639345e2, /// 0xa0c
			 0x4c283aca, /// 0xa10
			 0x68de006f, /// 0xa14
			 0xdf7792ae, /// 0xa18
			 0xe5e6464c, /// 0xa1c
			 0xec7e5614, /// 0xa20
			 0x8609a53c, /// 0xa24
			 0xe36d056d, /// 0xa28
			 0x1a015cf7, /// 0xa2c
			 0x19a5dff7, /// 0xa30
			 0x291d4c85, /// 0xa34
			 0xb90df6b3, /// 0xa38
			 0x9f158767, /// 0xa3c
			 0x6ba300a5, /// 0xa40
			 0xe979af8d, /// 0xa44
			 0x0be877bc, /// 0xa48
			 0xcd2eb012, /// 0xa4c
			 0x55055e09, /// 0xa50
			 0x3aa3c438, /// 0xa54
			 0x3e7beb7f, /// 0xa58
			 0x83275a91, /// 0xa5c
			 0xd4a7d805, /// 0xa60
			 0x80c14d13, /// 0xa64
			 0x3039d9d2, /// 0xa68
			 0x2ac104c2, /// 0xa6c
			 0xa94491e4, /// 0xa70
			 0xcd09dcb4, /// 0xa74
			 0x683789b5, /// 0xa78
			 0xab75d881, /// 0xa7c
			 0xe636f9b7, /// 0xa80
			 0xd2890af1, /// 0xa84
			 0x91aef7fc, /// 0xa88
			 0x09cfb5aa, /// 0xa8c
			 0x6d860610, /// 0xa90
			 0x042cf0e6, /// 0xa94
			 0x578d1fa7, /// 0xa98
			 0x33146446, /// 0xa9c
			 0xcb5c699b, /// 0xaa0
			 0x0188f0b9, /// 0xaa4
			 0xc0890492, /// 0xaa8
			 0x420c54ed, /// 0xaac
			 0xa79263ee, /// 0xab0
			 0xe29ad970, /// 0xab4
			 0xb562d587, /// 0xab8
			 0xce033da5, /// 0xabc
			 0xe4c8efb6, /// 0xac0
			 0xa48d9424, /// 0xac4
			 0x7250200f, /// 0xac8
			 0x38d1916f, /// 0xacc
			 0xc03a8e7d, /// 0xad0
			 0xb1eb0086, /// 0xad4
			 0x77cd197e, /// 0xad8
			 0x7e9759fa, /// 0xadc
			 0xc8538c43, /// 0xae0
			 0xcf3880a8, /// 0xae4
			 0x92404dd4, /// 0xae8
			 0xb10a7430, /// 0xaec
			 0x789302d6, /// 0xaf0
			 0x07afd618, /// 0xaf4
			 0xad0f1fdf, /// 0xaf8
			 0xf2ae0df4, /// 0xafc
			 0x04726b5d, /// 0xb00
			 0xcae19b01, /// 0xb04
			 0x5868afcd, /// 0xb08
			 0x59d7dd64, /// 0xb0c
			 0xc3035ba3, /// 0xb10
			 0x06d9a789, /// 0xb14
			 0x3f16f939, /// 0xb18
			 0xff556f3d, /// 0xb1c
			 0x46b0f813, /// 0xb20
			 0x7e12dacb, /// 0xb24
			 0x11a85aa7, /// 0xb28
			 0x5482ba7e, /// 0xb2c
			 0x30fd76f4, /// 0xb30
			 0x09d837d6, /// 0xb34
			 0xad129d82, /// 0xb38
			 0xfa65a086, /// 0xb3c
			 0xf45f35d7, /// 0xb40
			 0x567683d8, /// 0xb44
			 0x09e28bac, /// 0xb48
			 0x8573aa51, /// 0xb4c
			 0xcf16037d, /// 0xb50
			 0xe4242634, /// 0xb54
			 0x1387345a, /// 0xb58
			 0x1e68b7ac, /// 0xb5c
			 0x2f59cd29, /// 0xb60
			 0xde0093bc, /// 0xb64
			 0xa84e2123, /// 0xb68
			 0x62eb0896, /// 0xb6c
			 0x3876b772, /// 0xb70
			 0x4c2ea434, /// 0xb74
			 0x28ee5a88, /// 0xb78
			 0xb1df5b5c, /// 0xb7c
			 0xd2fe71f7, /// 0xb80
			 0x652ec119, /// 0xb84
			 0x9d474191, /// 0xb88
			 0x74de2de9, /// 0xb8c
			 0x5bac5f1a, /// 0xb90
			 0xd45eb90e, /// 0xb94
			 0x6d679a65, /// 0xb98
			 0xb7b4640b, /// 0xb9c
			 0xd8a90e91, /// 0xba0
			 0x52cd81c6, /// 0xba4
			 0xf25d0fc5, /// 0xba8
			 0x9f4924ea, /// 0xbac
			 0x5c9197a8, /// 0xbb0
			 0xf555b6cd, /// 0xbb4
			 0x2476be45, /// 0xbb8
			 0xe36cda23, /// 0xbbc
			 0xb80b4d77, /// 0xbc0
			 0xa272c599, /// 0xbc4
			 0xea6c555e, /// 0xbc8
			 0x4cf73de6, /// 0xbcc
			 0x701a7103, /// 0xbd0
			 0x1960cf49, /// 0xbd4
			 0xb7aca414, /// 0xbd8
			 0x3a625f03, /// 0xbdc
			 0x9e9e9c8e, /// 0xbe0
			 0xb1ef1049, /// 0xbe4
			 0x943dac46, /// 0xbe8
			 0xf7079719, /// 0xbec
			 0x51f5527c, /// 0xbf0
			 0x3238f9e1, /// 0xbf4
			 0x91f813c4, /// 0xbf8
			 0xa4a83e8c, /// 0xbfc
			 0x3f8e6fa5, /// 0xc00
			 0x81ec9988, /// 0xc04
			 0xb3b9be0c, /// 0xc08
			 0x6930d0bd, /// 0xc0c
			 0x84449820, /// 0xc10
			 0x433960a5, /// 0xc14
			 0xea165793, /// 0xc18
			 0xb6e92e0b, /// 0xc1c
			 0x8a747107, /// 0xc20
			 0x0a2b1525, /// 0xc24
			 0x4e968220, /// 0xc28
			 0xe3b05ad2, /// 0xc2c
			 0x39163a0e, /// 0xc30
			 0xdbf807b2, /// 0xc34
			 0x5ce6b66a, /// 0xc38
			 0xc3c36ce2, /// 0xc3c
			 0xbb18c903, /// 0xc40
			 0x406d66ed, /// 0xc44
			 0x39060533, /// 0xc48
			 0x1af4fd94, /// 0xc4c
			 0xbaf4d6c2, /// 0xc50
			 0xc538bdec, /// 0xc54
			 0x5ee502d1, /// 0xc58
			 0x0fe0c96e, /// 0xc5c
			 0x648f8814, /// 0xc60
			 0xf0f99d9d, /// 0xc64
			 0xf803b97d, /// 0xc68
			 0xb6a52d2f, /// 0xc6c
			 0x870ad287, /// 0xc70
			 0x51450abd, /// 0xc74
			 0x76852362, /// 0xc78
			 0xfbf0746d, /// 0xc7c
			 0x224cf312, /// 0xc80
			 0xf2ae021a, /// 0xc84
			 0x08dba362, /// 0xc88
			 0xe1837945, /// 0xc8c
			 0x1c02192d, /// 0xc90
			 0xbca2285e, /// 0xc94
			 0x3fb19979, /// 0xc98
			 0x46424113, /// 0xc9c
			 0x4a1fdb3e, /// 0xca0
			 0xdf314186, /// 0xca4
			 0x72088f4e, /// 0xca8
			 0x312dcdde, /// 0xcac
			 0x8fd232ae, /// 0xcb0
			 0x611fb794, /// 0xcb4
			 0x26d48016, /// 0xcb8
			 0xacfd81f0, /// 0xcbc
			 0x3696ebcf, /// 0xcc0
			 0xdf66544c, /// 0xcc4
			 0x5f80e19b, /// 0xcc8
			 0x93be43ee, /// 0xccc
			 0xb4bedafe, /// 0xcd0
			 0x70be63e5, /// 0xcd4
			 0x4a3f4525, /// 0xcd8
			 0xa37ea11a, /// 0xcdc
			 0xef585766, /// 0xce0
			 0x8137631a, /// 0xce4
			 0x5d7db299, /// 0xce8
			 0xc2fe512d, /// 0xcec
			 0xbf2c9769, /// 0xcf0
			 0x9ae668b3, /// 0xcf4
			 0x272d2d04, /// 0xcf8
			 0x8471ef54, /// 0xcfc
			 0x8a07b57a, /// 0xd00
			 0xaa77898d, /// 0xd04
			 0x4e882195, /// 0xd08
			 0xbbc4e828, /// 0xd0c
			 0x0355d613, /// 0xd10
			 0x753d6bc7, /// 0xd14
			 0x336777b5, /// 0xd18
			 0x58041650, /// 0xd1c
			 0x098fd43c, /// 0xd20
			 0x18f77cdd, /// 0xd24
			 0x0ea608df, /// 0xd28
			 0xae2c22ae, /// 0xd2c
			 0xdf64876d, /// 0xd30
			 0x327ef8ff, /// 0xd34
			 0x51daca46, /// 0xd38
			 0xa358fdde, /// 0xd3c
			 0x7c314855, /// 0xd40
			 0x7c312086, /// 0xd44
			 0x7ab27e20, /// 0xd48
			 0xe5a384aa, /// 0xd4c
			 0xdc0c9361, /// 0xd50
			 0x54cfa4fb, /// 0xd54
			 0x84167d3c, /// 0xd58
			 0xfc5d7c1d, /// 0xd5c
			 0xc38f53f6, /// 0xd60
			 0xe700f32b, /// 0xd64
			 0x03557258, /// 0xd68
			 0x434a8c18, /// 0xd6c
			 0xb6090415, /// 0xd70
			 0x84335cd0, /// 0xd74
			 0x84d3978c, /// 0xd78
			 0xa1d5b301, /// 0xd7c
			 0x2fd6f7f0, /// 0xd80
			 0x210b6fc7, /// 0xd84
			 0xbfe4e01e, /// 0xd88
			 0x6a8ad3de, /// 0xd8c
			 0xb6b12ffd, /// 0xd90
			 0x7cdbb952, /// 0xd94
			 0xb07ce0de, /// 0xd98
			 0x4b567ce8, /// 0xd9c
			 0xfa492e7e, /// 0xda0
			 0xf8e63bc8, /// 0xda4
			 0x0a65b6af, /// 0xda8
			 0xe0cfbde1, /// 0xdac
			 0x3e413896, /// 0xdb0
			 0xabfbc563, /// 0xdb4
			 0x7460774b, /// 0xdb8
			 0xf8c0d2ca, /// 0xdbc
			 0x63d5cea7, /// 0xdc0
			 0xf23a97d0, /// 0xdc4
			 0x999e628f, /// 0xdc8
			 0x26786222, /// 0xdcc
			 0x3dddf35d, /// 0xdd0
			 0x43d94a62, /// 0xdd4
			 0x237663eb, /// 0xdd8
			 0x912da152, /// 0xddc
			 0x0dcf2fbe, /// 0xde0
			 0x3a108229, /// 0xde4
			 0x667bff3a, /// 0xde8
			 0x0a703143, /// 0xdec
			 0x35e897ad, /// 0xdf0
			 0x9e10f042, /// 0xdf4
			 0x048eff41, /// 0xdf8
			 0xb21a1e74, /// 0xdfc
			 0x70e365f9, /// 0xe00
			 0xc296d41e, /// 0xe04
			 0xd2673c60, /// 0xe08
			 0xce20fbd5, /// 0xe0c
			 0x236538ad, /// 0xe10
			 0xdf497f3b, /// 0xe14
			 0xf1d86459, /// 0xe18
			 0x9eab5beb, /// 0xe1c
			 0x583434b6, /// 0xe20
			 0xdc749679, /// 0xe24
			 0xba97f0da, /// 0xe28
			 0xdf458535, /// 0xe2c
			 0x64db6133, /// 0xe30
			 0xecd63612, /// 0xe34
			 0x02c66f20, /// 0xe38
			 0x6bc3e462, /// 0xe3c
			 0x4f800434, /// 0xe40
			 0x08d9e862, /// 0xe44
			 0xe0d68480, /// 0xe48
			 0x67f16348, /// 0xe4c
			 0x9f1ec194, /// 0xe50
			 0xea414f53, /// 0xe54
			 0xcb5ddb98, /// 0xe58
			 0x1ba4df71, /// 0xe5c
			 0x08f49d16, /// 0xe60
			 0xdbd46715, /// 0xe64
			 0x9b8c4395, /// 0xe68
			 0x45ef897b, /// 0xe6c
			 0xd6bf7734, /// 0xe70
			 0x70693b55, /// 0xe74
			 0xb7f96fd7, /// 0xe78
			 0x0ef556d2, /// 0xe7c
			 0xcfe8e655, /// 0xe80
			 0x7871192f, /// 0xe84
			 0x8a329ac1, /// 0xe88
			 0xc229e1e9, /// 0xe8c
			 0xcdf6c63b, /// 0xe90
			 0xc133f48f, /// 0xe94
			 0x7aba096c, /// 0xe98
			 0xcc55a83c, /// 0xe9c
			 0x91501378, /// 0xea0
			 0xd0516e68, /// 0xea4
			 0x1d93d820, /// 0xea8
			 0x2c9549cf, /// 0xeac
			 0x78b45b1c, /// 0xeb0
			 0x25e9ef9c, /// 0xeb4
			 0x66dc703f, /// 0xeb8
			 0x93fdbe37, /// 0xebc
			 0xa44e3b1b, /// 0xec0
			 0x55ee13c6, /// 0xec4
			 0x5398a170, /// 0xec8
			 0xef246240, /// 0xecc
			 0xafb772b3, /// 0xed0
			 0x159f16b5, /// 0xed4
			 0xf557e291, /// 0xed8
			 0xd788ace7, /// 0xedc
			 0x2ce93734, /// 0xee0
			 0x22f3b0bd, /// 0xee4
			 0x93230006, /// 0xee8
			 0x786f9ef1, /// 0xeec
			 0xc1c8c1e4, /// 0xef0
			 0x57bb72a8, /// 0xef4
			 0x3187ea97, /// 0xef8
			 0x2c8ef161, /// 0xefc
			 0xd7fa66eb, /// 0xf00
			 0x6290ed70, /// 0xf04
			 0x53e77069, /// 0xf08
			 0x9658efdc, /// 0xf0c
			 0x8a7446ae, /// 0xf10
			 0x3d58b985, /// 0xf14
			 0x66c8bf67, /// 0xf18
			 0x2ba03b16, /// 0xf1c
			 0x31866a5a, /// 0xf20
			 0xbb74d1e7, /// 0xf24
			 0xff37b6d2, /// 0xf28
			 0xa1336bf3, /// 0xf2c
			 0x1201a097, /// 0xf30
			 0x74c86a04, /// 0xf34
			 0x6f813bba, /// 0xf38
			 0x5442976d, /// 0xf3c
			 0x27786a56, /// 0xf40
			 0x6a4c024e, /// 0xf44
			 0xab449e7d, /// 0xf48
			 0xfa4ae7fc, /// 0xf4c
			 0x0fcff08b, /// 0xf50
			 0xb6d05787, /// 0xf54
			 0x0b89043e, /// 0xf58
			 0xc5a32792, /// 0xf5c
			 0xbbe4cc85, /// 0xf60
			 0x3af57ebc, /// 0xf64
			 0x7df3477a, /// 0xf68
			 0xb96ef1df, /// 0xf6c
			 0x3fdf5b56, /// 0xf70
			 0x9c3a56a9, /// 0xf74
			 0x147a49af, /// 0xf78
			 0x5cdec989, /// 0xf7c
			 0x1c79d2b3, /// 0xf80
			 0xed12c976, /// 0xf84
			 0x97f81048, /// 0xf88
			 0x8e1f1063, /// 0xf8c
			 0x69f5fc6a, /// 0xf90
			 0xffff1d21, /// 0xf94
			 0x9415be2e, /// 0xf98
			 0xc76dc717, /// 0xf9c
			 0x6974c318, /// 0xfa0
			 0xb7762273, /// 0xfa4
			 0x6c8449bc, /// 0xfa8
			 0xd1d85bfb, /// 0xfac
			 0xaaede198, /// 0xfb0
			 0x95f4b8de, /// 0xfb4
			 0xeb50a645, /// 0xfb8
			 0x1dbe40ea, /// 0xfbc
			 0x5b01835b, /// 0xfc0
			 0x3c273114, /// 0xfc4
			 0x15ce6828, /// 0xfc8
			 0x25a6480b, /// 0xfcc
			 0x769d8f1f, /// 0xfd0
			 0x248c8c72, /// 0xfd4
			 0x722393a1, /// 0xfd8
			 0xae42d47a, /// 0xfdc
			 0xf1fcf639, /// 0xfe0
			 0x6143e664, /// 0xfe4
			 0x8772f252, /// 0xfe8
			 0xe765e296, /// 0xfec
			 0x7333ee33, /// 0xff0
			 0xa4d66810, /// 0xff4
			 0x9317c16c, /// 0xff8
			 0xdaca6606 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00000
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c600000,                                                  // Leading 1s:                                 /// 0000c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xbf028f5c,                                                  // -0.51                                       /// 00014
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00018
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00024
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0002c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00030
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00038
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00040
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00044
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00048
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0004c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00050
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00058
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0005c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x0e000001,                                                  // Trailing 1s:                                /// 00068
			 0x0e000003,                                                  // Trailing 1s:                                /// 0006c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00070
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00074
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00078
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0007c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00080
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00084
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00088
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0008c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000000,                                                  // -zero                                       /// 000f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00104
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0010c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c600000,                                                  // Leading 1s:                                 /// 00114
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00118
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0011c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00120
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00124
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00128
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0012c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00130
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00134
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00138
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00144
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00148
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0014c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00154
			 0x0c700000,                                                  // Leading 1s:                                 /// 00158
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00160
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00164
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x00000000,                                                  // zero                                        /// 0016c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00174
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00178
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0017c
			 0xbf028f5c,                                                  // -0.51                                       /// 00180
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00184
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0018c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00194
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x4b000000,                                                  // 8388608.0                                   /// 001d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001d8
			 0x7fc00001,                                                  // signaling NaN                               /// 001dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e4
			 0xff800000,                                                  // -inf                                        /// 001e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00200
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00204
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00208
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xffc00001,                                                  // -signaling NaN                              /// 00218
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0021c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00224
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0022c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00230
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0023c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00240
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0xbf028f5c,                                                  // -0.51                                       /// 0024c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0025c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00260
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00264
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00268
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0026c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00274
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00278
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00284
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00288
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0028c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00294
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00298
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xffc00001,                                                  // -signaling NaN                              /// 002d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002fc
			 0xff800000,                                                  // -inf                                        /// 00300
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00304
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00308
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0030c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00310
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00318
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0031c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00324
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00328
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0032c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00330
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00334
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00338
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00348
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00360
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00364
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00374
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00378
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0037c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00380
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00384
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00388
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00390
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0xcb000000,                                                  // -8388608.0                                  /// 003bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00408
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0040c
			 0x33333333,                                                  // 4 random values                             /// 00410
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00418
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x7f800000,                                                  // inf                                         /// 00420
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00424
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0042c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00430
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00434
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00438
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0043c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00440
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00450
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x7fc00001,                                                  // signaling NaN                               /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00474
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x4b000000,                                                  // 8388608.0                                   /// 0047c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00480
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00484
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00488
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0048c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00498
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004dc
			 0x33333333,                                                  // 4 random values                             /// 004e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x80000000,                                                  // -zero                                       /// 004fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00518
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00520
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00524
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00530
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00538
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00544
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00548
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00550
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00554
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00558
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0055c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00560
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00564
			 0xbf028f5c,                                                  // -0.51                                       /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0056c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00570
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00574
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00578
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0057c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00584
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00588
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0058c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00590
			 0x0c400000,                                                  // Leading 1s:                                 /// 00594
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0059c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a0
			 0xff800000,                                                  // -inf                                        /// 005a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b0
			 0xffc00001,                                                  // -signaling NaN                              /// 005b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e0
			 0xff800000,                                                  // -inf                                        /// 005e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 005f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00600
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00614
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00618
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00620
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00624
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00628
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0062c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00630
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00634
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00638
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00640
			 0x0c400000,                                                  // Leading 1s:                                 /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00648
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00650
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00660
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00664
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00668
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00670
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00674
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00678
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00680
			 0x0e000003,                                                  // Trailing 1s:                                /// 00684
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00688
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0068c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00698
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006a8
			 0x55555555,                                                  // 4 random values                             /// 006ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f0
			 0x80000000,                                                  // -zero                                       /// 006f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00704
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00708
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0070c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00710
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00714
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0071c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00720
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00724
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0072c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00730
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00738
			 0x0c600000,                                                  // Leading 1s:                                 /// 0073c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00744
			 0x00000000,                                                  // zero                                        /// 00748
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00754
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0075c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00760
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00764
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00770
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00774
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x00000000,                                                  // zero                                        /// 00780
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00784
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00790
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00794
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00798
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0079c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b8
			 0xffc00001,                                                  // -signaling NaN                              /// 007bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00800
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0080c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00810
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00814
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x7f800000,                                                  // inf                                         /// 00828
			 0xffc00001,                                                  // -signaling NaN                              /// 0082c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00830
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00838
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0083c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00840
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00844
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00850
			 0x00000000,                                                  // zero                                        /// 00854
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x4b000000,                                                  // 8388608.0                                   /// 0085c
			 0xbf028f5c,                                                  // -0.51                                       /// 00860
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00864
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00868
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0086c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00878
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0087c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00884
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x55555555,                                                  // 4 random values                             /// 0088c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c4
			 0xbf028f5c,                                                  // -0.51                                       /// 008c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00900
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80011111,                                                  // -9.7958E-41                                 /// 00914
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00918
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0091c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00920
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00928
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0092c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00934
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00938
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0093c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00940
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00948
			 0x0e000001,                                                  // Trailing 1s:                                /// 0094c
			 0xffc00001,                                                  // -signaling NaN                              /// 00950
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00954
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x33333333,                                                  // 4 random values                             /// 00960
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00968
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00974
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0097c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00980
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00984
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00988
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0098c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00990
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00998
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009bc
			 0x80000000,                                                  // -zero                                       /// 009c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x3f028f5c,                                                  // 0.51                                        /// 00a68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0xbf028f5c,                                                  // -0.51                                       /// 00a74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x7f800000,                                                  // inf                                         /// 00a88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ab8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ac4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00acc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ad0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00adc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ae0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00af4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00afc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7f800000,                                                  // inf                                         /// 00b58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bdc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00be0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bfc
			 0x4b000000,                                                  // 8388608.0                                   /// 00c00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x7fc00001,                                                  // signaling NaN                               /// 00c18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x3f028f5c,                                                  // 0.51                                        /// 00c54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb4
			 0xff800000,                                                  // -inf                                        /// 00cb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ccc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ce4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ce8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cfc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d08
			 0x3f028f5c,                                                  // 0.51                                        /// 00d0c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d34
			 0xff800000,                                                  // -inf                                        /// 00d38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d5c
			 0x7f800000,                                                  // inf                                         /// 00d60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x00000000,                                                  // zero                                        /// 00d68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d84
			 0x80000000,                                                  // -zero                                       /// 00d88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d98
			 0x00000000,                                                  // zero                                        /// 00d9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00da4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00db4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0xbf028f5c,                                                  // -0.51                                       /// 00dc8
			 0x7f800000,                                                  // inf                                         /// 00dcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ddc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00df8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x7f800000,                                                  // inf                                         /// 00e1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e48
			 0x7f800000,                                                  // inf                                         /// 00e4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e50
			 0xbf028f5c,                                                  // -0.51                                       /// 00e54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e78
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x3f028f5c,                                                  // 0.51                                        /// 00e98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000000,                                                  // -zero                                       /// 00ec0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00edc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0xffc00001,                                                  // -signaling NaN                              /// 00f2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f78
			 0x33333333,                                                  // 4 random values                             /// 00f7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fe4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ff0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7ff000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0f671914, /// 0x0
			 0xea3350ec, /// 0x4
			 0xec05e3ca, /// 0x8
			 0x8a53b5cd, /// 0xc
			 0x41c71036, /// 0x10
			 0x512d2786, /// 0x14
			 0xe293fef0, /// 0x18
			 0x8fe26734, /// 0x1c
			 0x776519c9, /// 0x20
			 0x339ea69f, /// 0x24
			 0x75f01b43, /// 0x28
			 0xc8ee8fee, /// 0x2c
			 0x229c2ada, /// 0x30
			 0x52eedbd6, /// 0x34
			 0x495d7a20, /// 0x38
			 0xdc65ac97, /// 0x3c
			 0x07de5711, /// 0x40
			 0x0214765c, /// 0x44
			 0x0dbc8de1, /// 0x48
			 0x57b2f7b7, /// 0x4c
			 0xd67a775a, /// 0x50
			 0xce4e500d, /// 0x54
			 0x322f6566, /// 0x58
			 0x2b8bdda2, /// 0x5c
			 0xe0d3efad, /// 0x60
			 0x001006df, /// 0x64
			 0xcf09bbd0, /// 0x68
			 0xcd2bc9bf, /// 0x6c
			 0xf8ee47d8, /// 0x70
			 0x03c091da, /// 0x74
			 0x8a2dec65, /// 0x78
			 0xf1b7b91f, /// 0x7c
			 0x97bb5a7f, /// 0x80
			 0x9bd94e55, /// 0x84
			 0xd91e7c6c, /// 0x88
			 0x2a36fdc3, /// 0x8c
			 0xb6c88038, /// 0x90
			 0x744d8cd9, /// 0x94
			 0x409aa203, /// 0x98
			 0x406bc0df, /// 0x9c
			 0xd14293aa, /// 0xa0
			 0x0b8f4464, /// 0xa4
			 0x74502fc9, /// 0xa8
			 0x0015c2cc, /// 0xac
			 0xbdf7fcb5, /// 0xb0
			 0x09744a33, /// 0xb4
			 0xb8094b63, /// 0xb8
			 0xa38fb4f4, /// 0xbc
			 0xa610daba, /// 0xc0
			 0x76faa0a2, /// 0xc4
			 0xc03b0d7b, /// 0xc8
			 0x2b6d8d5b, /// 0xcc
			 0x364de66c, /// 0xd0
			 0x14522d9a, /// 0xd4
			 0x78334eae, /// 0xd8
			 0x6e3d4203, /// 0xdc
			 0xa66db90a, /// 0xe0
			 0x30db2bcf, /// 0xe4
			 0xc07c4300, /// 0xe8
			 0xeb853ddb, /// 0xec
			 0x9b687a34, /// 0xf0
			 0x3612ed06, /// 0xf4
			 0x8e26b1fd, /// 0xf8
			 0x5a719eb4, /// 0xfc
			 0xad945250, /// 0x100
			 0x59a3f5db, /// 0x104
			 0xc7211a15, /// 0x108
			 0x36029f45, /// 0x10c
			 0x5c366e3c, /// 0x110
			 0x1f00ef26, /// 0x114
			 0x782fc9db, /// 0x118
			 0xaa28dcfe, /// 0x11c
			 0xa47c4c45, /// 0x120
			 0x7cf954ea, /// 0x124
			 0x953e876e, /// 0x128
			 0x8540ee84, /// 0x12c
			 0x623521b1, /// 0x130
			 0x4d29ac11, /// 0x134
			 0x202219f2, /// 0x138
			 0x4d53a21a, /// 0x13c
			 0x7f2dfc39, /// 0x140
			 0x7c7f558b, /// 0x144
			 0x354bc000, /// 0x148
			 0x0f9e9fe5, /// 0x14c
			 0x1bbf5320, /// 0x150
			 0x3f1014c7, /// 0x154
			 0x4eaf618a, /// 0x158
			 0xe4d21943, /// 0x15c
			 0xd69ae37d, /// 0x160
			 0x16286977, /// 0x164
			 0x2a25ba7b, /// 0x168
			 0x3e60646a, /// 0x16c
			 0xb74c8c93, /// 0x170
			 0x02dce9dd, /// 0x174
			 0xb4ae20d3, /// 0x178
			 0xa6cd4b1e, /// 0x17c
			 0x00c88656, /// 0x180
			 0x9a3f9a98, /// 0x184
			 0x005faf8d, /// 0x188
			 0x1b92069c, /// 0x18c
			 0x98e3e133, /// 0x190
			 0xb1faadcb, /// 0x194
			 0xbddaef5c, /// 0x198
			 0xff4c5088, /// 0x19c
			 0x68501219, /// 0x1a0
			 0x29f92d81, /// 0x1a4
			 0x2fb8d862, /// 0x1a8
			 0x20c4fe34, /// 0x1ac
			 0xed494eb5, /// 0x1b0
			 0xb9bd59f7, /// 0x1b4
			 0xa0e691d6, /// 0x1b8
			 0xece309aa, /// 0x1bc
			 0xe701e791, /// 0x1c0
			 0xf0cbba40, /// 0x1c4
			 0x903c63ed, /// 0x1c8
			 0x1edeb800, /// 0x1cc
			 0x1fba1931, /// 0x1d0
			 0x1cddeeea, /// 0x1d4
			 0xb8e729e3, /// 0x1d8
			 0xc632237d, /// 0x1dc
			 0xcd60bc5e, /// 0x1e0
			 0x5f88c6bb, /// 0x1e4
			 0xb6626712, /// 0x1e8
			 0x898a1aa5, /// 0x1ec
			 0x63db7bd6, /// 0x1f0
			 0xd4286d50, /// 0x1f4
			 0x5db64bfd, /// 0x1f8
			 0xada0b710, /// 0x1fc
			 0xd9827b77, /// 0x200
			 0xac6695e1, /// 0x204
			 0xf5cf1510, /// 0x208
			 0x0818f7d6, /// 0x20c
			 0x2e3acc08, /// 0x210
			 0x07e895be, /// 0x214
			 0xabab3e44, /// 0x218
			 0x46350c54, /// 0x21c
			 0xc8943cfe, /// 0x220
			 0x6281ddf3, /// 0x224
			 0xcbdb042f, /// 0x228
			 0x68b9df12, /// 0x22c
			 0xe1cfb459, /// 0x230
			 0x4336a8fc, /// 0x234
			 0xe9f0d8b0, /// 0x238
			 0x83c9ea29, /// 0x23c
			 0xd840f02a, /// 0x240
			 0x4b9e1257, /// 0x244
			 0xa46b91a7, /// 0x248
			 0x9cb8fc56, /// 0x24c
			 0xa0720ae2, /// 0x250
			 0xc02527ad, /// 0x254
			 0x7581458f, /// 0x258
			 0x4f746397, /// 0x25c
			 0xaf602383, /// 0x260
			 0x8e00f84f, /// 0x264
			 0xb210edbe, /// 0x268
			 0x136c2e24, /// 0x26c
			 0x4f9cbe73, /// 0x270
			 0x7506c156, /// 0x274
			 0x7c85c158, /// 0x278
			 0x92b0302c, /// 0x27c
			 0xab208b87, /// 0x280
			 0x3b86e1a9, /// 0x284
			 0xa0132de0, /// 0x288
			 0x9d0b65a3, /// 0x28c
			 0x4a4b12d0, /// 0x290
			 0x05cdacae, /// 0x294
			 0xedae1e66, /// 0x298
			 0x19fa7dd5, /// 0x29c
			 0x37a83e11, /// 0x2a0
			 0xb697b6e6, /// 0x2a4
			 0x5c5e016b, /// 0x2a8
			 0xad0902bc, /// 0x2ac
			 0x7c4ff6ef, /// 0x2b0
			 0x9b453fce, /// 0x2b4
			 0x1563e7ad, /// 0x2b8
			 0x3e269a1c, /// 0x2bc
			 0xe6d10410, /// 0x2c0
			 0xfe7f917a, /// 0x2c4
			 0xe1d2d6a6, /// 0x2c8
			 0x201fdaf6, /// 0x2cc
			 0x20f51746, /// 0x2d0
			 0x82e4338b, /// 0x2d4
			 0xdabced28, /// 0x2d8
			 0x8f39ec0b, /// 0x2dc
			 0x59c9ea8b, /// 0x2e0
			 0x0514fb7b, /// 0x2e4
			 0x78bb5815, /// 0x2e8
			 0x02e9aaf8, /// 0x2ec
			 0x77fa9ebe, /// 0x2f0
			 0x270e0b66, /// 0x2f4
			 0x52a50d40, /// 0x2f8
			 0xe5d5305a, /// 0x2fc
			 0x5ad6fc25, /// 0x300
			 0x3a5bf0db, /// 0x304
			 0x472a7bfa, /// 0x308
			 0xf53eabd6, /// 0x30c
			 0x45367a77, /// 0x310
			 0xbf5ca51b, /// 0x314
			 0x494c75de, /// 0x318
			 0x549bfb38, /// 0x31c
			 0x6f269b8f, /// 0x320
			 0x17e0f378, /// 0x324
			 0x3dd05a33, /// 0x328
			 0xd50b5c9e, /// 0x32c
			 0xf42ed3ad, /// 0x330
			 0x5856d961, /// 0x334
			 0x4551c221, /// 0x338
			 0xf4f232ae, /// 0x33c
			 0x3d14efb3, /// 0x340
			 0xe36100c5, /// 0x344
			 0x74228bb8, /// 0x348
			 0xa2bbf54b, /// 0x34c
			 0xd5387d7a, /// 0x350
			 0x63c48fb3, /// 0x354
			 0x0ae4a0a2, /// 0x358
			 0x60220f41, /// 0x35c
			 0xf18a2889, /// 0x360
			 0xd9afa8ac, /// 0x364
			 0x9432ad01, /// 0x368
			 0x084fd64c, /// 0x36c
			 0xdd8fc3cf, /// 0x370
			 0x220ffa6d, /// 0x374
			 0x3cfae2ec, /// 0x378
			 0xc66a854f, /// 0x37c
			 0x7c094701, /// 0x380
			 0x9d7ffa09, /// 0x384
			 0x9a8581df, /// 0x388
			 0xd39f6e7a, /// 0x38c
			 0xbcee7310, /// 0x390
			 0xff88a79d, /// 0x394
			 0x50fefd43, /// 0x398
			 0x533e1923, /// 0x39c
			 0xae350320, /// 0x3a0
			 0xa29b7bfb, /// 0x3a4
			 0x22ca6351, /// 0x3a8
			 0x76296f4d, /// 0x3ac
			 0xbee29c19, /// 0x3b0
			 0x8bbb0d09, /// 0x3b4
			 0x66357f22, /// 0x3b8
			 0x1215410f, /// 0x3bc
			 0x723e1ccc, /// 0x3c0
			 0xf08b873c, /// 0x3c4
			 0xcfd34a78, /// 0x3c8
			 0x1c7754f0, /// 0x3cc
			 0x391e5d30, /// 0x3d0
			 0x95cb1ba8, /// 0x3d4
			 0xfb988a9e, /// 0x3d8
			 0x562066d8, /// 0x3dc
			 0xa3cc7716, /// 0x3e0
			 0x0a413ed7, /// 0x3e4
			 0xa7a56010, /// 0x3e8
			 0x2d8af140, /// 0x3ec
			 0x73d8eb12, /// 0x3f0
			 0x3db1968e, /// 0x3f4
			 0x20969639, /// 0x3f8
			 0x58dabcb5, /// 0x3fc
			 0x0f84bb8e, /// 0x400
			 0xc4adc0d9, /// 0x404
			 0x35efdf14, /// 0x408
			 0xe50c0143, /// 0x40c
			 0x6ea23900, /// 0x410
			 0x66d54dc9, /// 0x414
			 0xc2fa6eb6, /// 0x418
			 0xa2c634da, /// 0x41c
			 0x4b4caf75, /// 0x420
			 0xa123da1d, /// 0x424
			 0x8f32f7ab, /// 0x428
			 0x6c54eba7, /// 0x42c
			 0x8c34784c, /// 0x430
			 0x2c27e9fa, /// 0x434
			 0xb2719167, /// 0x438
			 0xd58da782, /// 0x43c
			 0x07199c11, /// 0x440
			 0xed54ab2f, /// 0x444
			 0x17738813, /// 0x448
			 0x975b2392, /// 0x44c
			 0x436e0f44, /// 0x450
			 0x5ef2eae5, /// 0x454
			 0x83be9477, /// 0x458
			 0x641b562c, /// 0x45c
			 0x46d0dce1, /// 0x460
			 0xeeb3016a, /// 0x464
			 0xb12225f3, /// 0x468
			 0x14a30cb7, /// 0x46c
			 0x35a58446, /// 0x470
			 0xb5ce3b9a, /// 0x474
			 0xccc75864, /// 0x478
			 0x98f2f963, /// 0x47c
			 0x442e513c, /// 0x480
			 0x02d409cc, /// 0x484
			 0x2f5f2d71, /// 0x488
			 0x484a8aae, /// 0x48c
			 0x5d474f27, /// 0x490
			 0xad23aa69, /// 0x494
			 0xb28421d7, /// 0x498
			 0x2d745377, /// 0x49c
			 0x2bed58b8, /// 0x4a0
			 0xa2228ce9, /// 0x4a4
			 0x6c3d652c, /// 0x4a8
			 0xd11ffcbe, /// 0x4ac
			 0x9c9fca47, /// 0x4b0
			 0xcad2d200, /// 0x4b4
			 0x82c9b62a, /// 0x4b8
			 0x76a2a341, /// 0x4bc
			 0x99af9705, /// 0x4c0
			 0x83d78990, /// 0x4c4
			 0x5047305b, /// 0x4c8
			 0xff631dd3, /// 0x4cc
			 0x0f1ece60, /// 0x4d0
			 0xf97ae470, /// 0x4d4
			 0x33d00dfd, /// 0x4d8
			 0x5c2e7f91, /// 0x4dc
			 0x468f0c09, /// 0x4e0
			 0x3fb7e89b, /// 0x4e4
			 0x7b272ba1, /// 0x4e8
			 0xcd25b694, /// 0x4ec
			 0x641d58c9, /// 0x4f0
			 0x20847201, /// 0x4f4
			 0x5dd82cad, /// 0x4f8
			 0xeac2e094, /// 0x4fc
			 0xe1961a36, /// 0x500
			 0x97be339e, /// 0x504
			 0x471ae2f7, /// 0x508
			 0xf69a65de, /// 0x50c
			 0x57f7a569, /// 0x510
			 0x5ae70d80, /// 0x514
			 0x019f0cb8, /// 0x518
			 0x7626e135, /// 0x51c
			 0x0323ffa3, /// 0x520
			 0x2bf64be7, /// 0x524
			 0x3b7c912b, /// 0x528
			 0xcf6bba0e, /// 0x52c
			 0x199dfee7, /// 0x530
			 0x37d17c7c, /// 0x534
			 0x2bc8e93a, /// 0x538
			 0x32e1519d, /// 0x53c
			 0x60747acf, /// 0x540
			 0xc04f7c2f, /// 0x544
			 0xd1075064, /// 0x548
			 0x427ea629, /// 0x54c
			 0xca1f457e, /// 0x550
			 0x28f19fd4, /// 0x554
			 0x41847121, /// 0x558
			 0x1d29933c, /// 0x55c
			 0x752e15f8, /// 0x560
			 0xe117080d, /// 0x564
			 0xe7fdafe7, /// 0x568
			 0x10be6cc2, /// 0x56c
			 0x9b945631, /// 0x570
			 0x909de532, /// 0x574
			 0xd111391c, /// 0x578
			 0x57202ce0, /// 0x57c
			 0xb3ca4b1e, /// 0x580
			 0x99891170, /// 0x584
			 0x678f391e, /// 0x588
			 0x566234b3, /// 0x58c
			 0x4f291040, /// 0x590
			 0xfbeb78e9, /// 0x594
			 0x50fceb6a, /// 0x598
			 0xca5b6ad0, /// 0x59c
			 0xc2e44baa, /// 0x5a0
			 0x9f651045, /// 0x5a4
			 0x2c81954e, /// 0x5a8
			 0x716bc2df, /// 0x5ac
			 0x67a82b79, /// 0x5b0
			 0x2ad2bc37, /// 0x5b4
			 0x8a4e7344, /// 0x5b8
			 0x1b13282e, /// 0x5bc
			 0x3b85ca5a, /// 0x5c0
			 0xd593bc91, /// 0x5c4
			 0x4d56c378, /// 0x5c8
			 0x21d6c252, /// 0x5cc
			 0x6faea37b, /// 0x5d0
			 0x30f88d91, /// 0x5d4
			 0xbdd159cb, /// 0x5d8
			 0x716f457e, /// 0x5dc
			 0x4c807661, /// 0x5e0
			 0xa85cae13, /// 0x5e4
			 0x825a3d78, /// 0x5e8
			 0x8d3152ed, /// 0x5ec
			 0x6f64de50, /// 0x5f0
			 0x6c2fd89c, /// 0x5f4
			 0x10592469, /// 0x5f8
			 0xba798bd2, /// 0x5fc
			 0xd1d26c8b, /// 0x600
			 0x957c3fc1, /// 0x604
			 0x5ec8236c, /// 0x608
			 0x4125f14a, /// 0x60c
			 0x4a845793, /// 0x610
			 0xe9b4bcc9, /// 0x614
			 0x88b60d8b, /// 0x618
			 0xc2915d13, /// 0x61c
			 0x832ebe52, /// 0x620
			 0x7607253f, /// 0x624
			 0x0a754244, /// 0x628
			 0xc7fca13c, /// 0x62c
			 0x7021d41f, /// 0x630
			 0x6c657030, /// 0x634
			 0xe97dec3c, /// 0x638
			 0x2adac5e9, /// 0x63c
			 0x892ebc45, /// 0x640
			 0x23d33831, /// 0x644
			 0x0ff3deeb, /// 0x648
			 0x18cd5aea, /// 0x64c
			 0xdf6f936f, /// 0x650
			 0x0b467642, /// 0x654
			 0xa1c5e8dd, /// 0x658
			 0xea7612b7, /// 0x65c
			 0x6a91cf06, /// 0x660
			 0xacb1b9cc, /// 0x664
			 0xcf33c074, /// 0x668
			 0x5de54742, /// 0x66c
			 0x7e121c33, /// 0x670
			 0xfa163f93, /// 0x674
			 0xa72aa766, /// 0x678
			 0x1e432bb5, /// 0x67c
			 0x154c8ad2, /// 0x680
			 0xd0166c50, /// 0x684
			 0x764f3386, /// 0x688
			 0x050e0a3a, /// 0x68c
			 0xf2672cb6, /// 0x690
			 0x5ab34644, /// 0x694
			 0x67ae8270, /// 0x698
			 0x85fba437, /// 0x69c
			 0xb91c8e0f, /// 0x6a0
			 0x18e06bcd, /// 0x6a4
			 0xa988c56b, /// 0x6a8
			 0xd006a8b5, /// 0x6ac
			 0x0e889cd4, /// 0x6b0
			 0xb91e06f7, /// 0x6b4
			 0x3ca667a4, /// 0x6b8
			 0xa40b64bb, /// 0x6bc
			 0x9feedc82, /// 0x6c0
			 0x093f7345, /// 0x6c4
			 0xc1a6dc70, /// 0x6c8
			 0xc0dad238, /// 0x6cc
			 0x710a6e28, /// 0x6d0
			 0xef00078d, /// 0x6d4
			 0x4175f6b4, /// 0x6d8
			 0x9b12789e, /// 0x6dc
			 0x0a6fc348, /// 0x6e0
			 0x047b82c8, /// 0x6e4
			 0xf34f2935, /// 0x6e8
			 0x2b2146c6, /// 0x6ec
			 0x3de78cba, /// 0x6f0
			 0x5d0dac59, /// 0x6f4
			 0x1901a81f, /// 0x6f8
			 0x81cfe9fc, /// 0x6fc
			 0x1060da79, /// 0x700
			 0xeb03cc65, /// 0x704
			 0x2c62783a, /// 0x708
			 0xc71cdeef, /// 0x70c
			 0x3d1c4258, /// 0x710
			 0x41622c3c, /// 0x714
			 0xfc12f119, /// 0x718
			 0x53242175, /// 0x71c
			 0x85ec3167, /// 0x720
			 0xdc12b438, /// 0x724
			 0xca2c2b39, /// 0x728
			 0xcb0a6f7e, /// 0x72c
			 0xb714817b, /// 0x730
			 0xabcecd10, /// 0x734
			 0x58c3be35, /// 0x738
			 0xc80a0757, /// 0x73c
			 0xf1e170d5, /// 0x740
			 0x76ed7739, /// 0x744
			 0xb0e36cb8, /// 0x748
			 0x6195211c, /// 0x74c
			 0xea2f6720, /// 0x750
			 0xe82ac9dc, /// 0x754
			 0xfb9fe72e, /// 0x758
			 0xcd7c8b5e, /// 0x75c
			 0x429d71a2, /// 0x760
			 0xf85a5b86, /// 0x764
			 0x75dfc899, /// 0x768
			 0x8d487b54, /// 0x76c
			 0xbf5464f4, /// 0x770
			 0x2db50093, /// 0x774
			 0xd3d75bf0, /// 0x778
			 0x355e92a7, /// 0x77c
			 0xdfbd3745, /// 0x780
			 0x23cb2393, /// 0x784
			 0x7e784794, /// 0x788
			 0xfa5a615d, /// 0x78c
			 0x2d8d8347, /// 0x790
			 0xd0285ba1, /// 0x794
			 0x0940922a, /// 0x798
			 0x72aada5c, /// 0x79c
			 0xf6f9646c, /// 0x7a0
			 0x2e22bcab, /// 0x7a4
			 0x51541b1b, /// 0x7a8
			 0x07061a5a, /// 0x7ac
			 0x30e450f9, /// 0x7b0
			 0xcfc7f6fb, /// 0x7b4
			 0x8dd206d0, /// 0x7b8
			 0x05dd576c, /// 0x7bc
			 0xab57d569, /// 0x7c0
			 0x263c2130, /// 0x7c4
			 0xdcd5a5db, /// 0x7c8
			 0x82ac6b3b, /// 0x7cc
			 0x9e4473f2, /// 0x7d0
			 0x0b997d55, /// 0x7d4
			 0x3178b83d, /// 0x7d8
			 0x520f23f6, /// 0x7dc
			 0x832434c1, /// 0x7e0
			 0x5c1eb06d, /// 0x7e4
			 0x0b97c621, /// 0x7e8
			 0x59e939ca, /// 0x7ec
			 0x58059baa, /// 0x7f0
			 0x0c6c70f0, /// 0x7f4
			 0x3d7dc4a7, /// 0x7f8
			 0x0c9c2cad, /// 0x7fc
			 0x3bc1e313, /// 0x800
			 0x4bab4c9a, /// 0x804
			 0xcc466a50, /// 0x808
			 0xaf7c5c60, /// 0x80c
			 0x3c72bdee, /// 0x810
			 0xe43efcf5, /// 0x814
			 0x6e75200b, /// 0x818
			 0x80a8cfc9, /// 0x81c
			 0x073c2835, /// 0x820
			 0xbce803da, /// 0x824
			 0xed35e256, /// 0x828
			 0xc9d0eec6, /// 0x82c
			 0x8ead2135, /// 0x830
			 0xf3c06bf1, /// 0x834
			 0x3af6163c, /// 0x838
			 0xbb311c62, /// 0x83c
			 0x3423b540, /// 0x840
			 0xdcab2de4, /// 0x844
			 0xd18c4a27, /// 0x848
			 0x9140277b, /// 0x84c
			 0x859ba314, /// 0x850
			 0x63aafb67, /// 0x854
			 0xc26b7666, /// 0x858
			 0x536a01cf, /// 0x85c
			 0x902b67c2, /// 0x860
			 0x48da0b27, /// 0x864
			 0x569ae7fc, /// 0x868
			 0xb1f00e14, /// 0x86c
			 0xcdf0a89c, /// 0x870
			 0x60e3f0fe, /// 0x874
			 0x782965fd, /// 0x878
			 0x06c65655, /// 0x87c
			 0x20cc78c7, /// 0x880
			 0x63d7ddf6, /// 0x884
			 0x35c30ad7, /// 0x888
			 0x080fe10a, /// 0x88c
			 0x7b028c1c, /// 0x890
			 0xf8fd86dc, /// 0x894
			 0xb6be8d5d, /// 0x898
			 0x175f9341, /// 0x89c
			 0xd7bcfa7e, /// 0x8a0
			 0xfe6c53c6, /// 0x8a4
			 0x3ff61c47, /// 0x8a8
			 0x60d4a25f, /// 0x8ac
			 0x43c5085c, /// 0x8b0
			 0xfbd2aec1, /// 0x8b4
			 0x4b0fcb2d, /// 0x8b8
			 0x84c95c0f, /// 0x8bc
			 0x01a3bdc6, /// 0x8c0
			 0xadc19774, /// 0x8c4
			 0xbea4426a, /// 0x8c8
			 0x7df56ff1, /// 0x8cc
			 0xf2e84002, /// 0x8d0
			 0x98a107a8, /// 0x8d4
			 0xe08afe0e, /// 0x8d8
			 0xf5874bc9, /// 0x8dc
			 0xad3e57e5, /// 0x8e0
			 0x062169d8, /// 0x8e4
			 0xedf9784c, /// 0x8e8
			 0xc9e2e3ac, /// 0x8ec
			 0x42cb059e, /// 0x8f0
			 0x2c60c8ea, /// 0x8f4
			 0xdfd6b04e, /// 0x8f8
			 0x4f7f0f42, /// 0x8fc
			 0xa7e87b73, /// 0x900
			 0xa875b349, /// 0x904
			 0x9f813019, /// 0x908
			 0x7f9b40e4, /// 0x90c
			 0x7e73cd7c, /// 0x910
			 0x279d3ae3, /// 0x914
			 0xf9b640d2, /// 0x918
			 0x84991732, /// 0x91c
			 0xa7273a3d, /// 0x920
			 0xf9ebc8b5, /// 0x924
			 0xde8788b5, /// 0x928
			 0xb93efac6, /// 0x92c
			 0x67a37769, /// 0x930
			 0xc2c822d0, /// 0x934
			 0xe40ac997, /// 0x938
			 0x03281b46, /// 0x93c
			 0x6a7d6ac2, /// 0x940
			 0x60b79249, /// 0x944
			 0x4c4c6410, /// 0x948
			 0x858027a7, /// 0x94c
			 0x33edbca8, /// 0x950
			 0x50e4bff2, /// 0x954
			 0x5beb47a3, /// 0x958
			 0x75f94a0a, /// 0x95c
			 0xbfeca90a, /// 0x960
			 0xf4af7cec, /// 0x964
			 0xba9bbba3, /// 0x968
			 0x7c2037ce, /// 0x96c
			 0xe1cde65a, /// 0x970
			 0xf57f490e, /// 0x974
			 0x9b512ec4, /// 0x978
			 0xe34a11bb, /// 0x97c
			 0x13b1ec70, /// 0x980
			 0xc09100ad, /// 0x984
			 0x79d9652d, /// 0x988
			 0x7523befc, /// 0x98c
			 0x9bde0c17, /// 0x990
			 0xfe3b558c, /// 0x994
			 0x390a65be, /// 0x998
			 0xc2c90f43, /// 0x99c
			 0xc227f293, /// 0x9a0
			 0xcae28c2d, /// 0x9a4
			 0x16593cd6, /// 0x9a8
			 0xde473c2e, /// 0x9ac
			 0xac8d6c76, /// 0x9b0
			 0xd2565573, /// 0x9b4
			 0x21bae8bf, /// 0x9b8
			 0xb99a193b, /// 0x9bc
			 0x3e53b847, /// 0x9c0
			 0x74605fc1, /// 0x9c4
			 0x6d50d154, /// 0x9c8
			 0x8949d9f3, /// 0x9cc
			 0x28742b4b, /// 0x9d0
			 0x00a768ea, /// 0x9d4
			 0x27968c38, /// 0x9d8
			 0x6b411b34, /// 0x9dc
			 0xf8fcbeaf, /// 0x9e0
			 0x85484254, /// 0x9e4
			 0x790a0feb, /// 0x9e8
			 0x2be8feca, /// 0x9ec
			 0xf3978175, /// 0x9f0
			 0x8064c9bf, /// 0x9f4
			 0xbd5ac404, /// 0x9f8
			 0xa6f4ddd1, /// 0x9fc
			 0x782ca043, /// 0xa00
			 0x8738fddb, /// 0xa04
			 0x3e0d2c73, /// 0xa08
			 0x1af61e97, /// 0xa0c
			 0xaa643e47, /// 0xa10
			 0x912d38fa, /// 0xa14
			 0xc4948bad, /// 0xa18
			 0xf8d205a5, /// 0xa1c
			 0xa81286ff, /// 0xa20
			 0x01ad045f, /// 0xa24
			 0x78058eb8, /// 0xa28
			 0x633da83e, /// 0xa2c
			 0x145b106c, /// 0xa30
			 0x6668f31c, /// 0xa34
			 0x839093b5, /// 0xa38
			 0xe97614a6, /// 0xa3c
			 0x08399332, /// 0xa40
			 0x526b20f7, /// 0xa44
			 0x837a6a09, /// 0xa48
			 0xbeae7fc6, /// 0xa4c
			 0xabbcb226, /// 0xa50
			 0x34b8d546, /// 0xa54
			 0xfa385b34, /// 0xa58
			 0xb9a6c396, /// 0xa5c
			 0x93ad8c09, /// 0xa60
			 0x52e5cf21, /// 0xa64
			 0x09107b63, /// 0xa68
			 0xeaa72c73, /// 0xa6c
			 0xdb2179d0, /// 0xa70
			 0x23742bf5, /// 0xa74
			 0xcabae0af, /// 0xa78
			 0x7b18ce83, /// 0xa7c
			 0x50e4c22d, /// 0xa80
			 0x5b8d3b28, /// 0xa84
			 0x2ac9a340, /// 0xa88
			 0xca29502a, /// 0xa8c
			 0x5609897e, /// 0xa90
			 0x2072833a, /// 0xa94
			 0x64e6934f, /// 0xa98
			 0x423a1afd, /// 0xa9c
			 0xc772748d, /// 0xaa0
			 0xc3962c5e, /// 0xaa4
			 0x23b4db33, /// 0xaa8
			 0x894a37af, /// 0xaac
			 0x6caf1e34, /// 0xab0
			 0x9f4be1f4, /// 0xab4
			 0x1e5ff095, /// 0xab8
			 0x25284002, /// 0xabc
			 0xf20a1a8c, /// 0xac0
			 0x9fbbc5d5, /// 0xac4
			 0x6d11d0fc, /// 0xac8
			 0x5e59ecda, /// 0xacc
			 0x829c8ed7, /// 0xad0
			 0x95167128, /// 0xad4
			 0x261957f4, /// 0xad8
			 0x82a17ea1, /// 0xadc
			 0xaebdca0d, /// 0xae0
			 0x35b35cf8, /// 0xae4
			 0x4d988cc5, /// 0xae8
			 0x573f84c2, /// 0xaec
			 0x54f60415, /// 0xaf0
			 0x0b259d71, /// 0xaf4
			 0x05555ad5, /// 0xaf8
			 0xe4912143, /// 0xafc
			 0x939e5901, /// 0xb00
			 0x58b38180, /// 0xb04
			 0x10d038cc, /// 0xb08
			 0x69ced473, /// 0xb0c
			 0x1cc1819c, /// 0xb10
			 0x2c920800, /// 0xb14
			 0x9f4b52e2, /// 0xb18
			 0x2e656173, /// 0xb1c
			 0x5c0f65df, /// 0xb20
			 0xc62c9e0e, /// 0xb24
			 0x52f2ac4d, /// 0xb28
			 0xd437b699, /// 0xb2c
			 0x7ce237b4, /// 0xb30
			 0x74c7deaf, /// 0xb34
			 0xa826a615, /// 0xb38
			 0x1cc209f9, /// 0xb3c
			 0xa3dcb536, /// 0xb40
			 0x791a746a, /// 0xb44
			 0xd5e5833e, /// 0xb48
			 0x2a826303, /// 0xb4c
			 0xb5b28b00, /// 0xb50
			 0x713f9a49, /// 0xb54
			 0x77729ea0, /// 0xb58
			 0x362f9936, /// 0xb5c
			 0x81c99d65, /// 0xb60
			 0x88be15b1, /// 0xb64
			 0x22f3ad61, /// 0xb68
			 0xdfac0018, /// 0xb6c
			 0x8cf03ce4, /// 0xb70
			 0x7b8ebd02, /// 0xb74
			 0x6e1455ca, /// 0xb78
			 0x0877063b, /// 0xb7c
			 0x018e895e, /// 0xb80
			 0x3bd2a698, /// 0xb84
			 0xb90d4ad1, /// 0xb88
			 0x84cf23f0, /// 0xb8c
			 0x5ef2aef4, /// 0xb90
			 0xf5a0518e, /// 0xb94
			 0x76e1d472, /// 0xb98
			 0x9776b860, /// 0xb9c
			 0xed465917, /// 0xba0
			 0x16b23186, /// 0xba4
			 0x96e6785b, /// 0xba8
			 0xf3a48a39, /// 0xbac
			 0xf41efd23, /// 0xbb0
			 0x38ff8d8e, /// 0xbb4
			 0x0efb34d4, /// 0xbb8
			 0x0f42e077, /// 0xbbc
			 0x4af6c733, /// 0xbc0
			 0x81f1eaef, /// 0xbc4
			 0x57456ed2, /// 0xbc8
			 0x354b9df0, /// 0xbcc
			 0xe152b7d0, /// 0xbd0
			 0x6cd0a8dc, /// 0xbd4
			 0xe886bc47, /// 0xbd8
			 0xe4cf1277, /// 0xbdc
			 0x01a29caf, /// 0xbe0
			 0x33cd0129, /// 0xbe4
			 0xeec4f71d, /// 0xbe8
			 0x5494753a, /// 0xbec
			 0xa725185f, /// 0xbf0
			 0x96f7a09c, /// 0xbf4
			 0x957b4396, /// 0xbf8
			 0xebc6f86a, /// 0xbfc
			 0x7b1c4aac, /// 0xc00
			 0x6494749e, /// 0xc04
			 0x4d65ead3, /// 0xc08
			 0x16892635, /// 0xc0c
			 0xb5f586b9, /// 0xc10
			 0x80146843, /// 0xc14
			 0xcb99d865, /// 0xc18
			 0x5ccec810, /// 0xc1c
			 0xb5b568d6, /// 0xc20
			 0xf154c2db, /// 0xc24
			 0x3ff319be, /// 0xc28
			 0xdca8471b, /// 0xc2c
			 0xdabd2ac7, /// 0xc30
			 0xd323faa4, /// 0xc34
			 0xc422da7d, /// 0xc38
			 0x8e73697f, /// 0xc3c
			 0x50cb4e64, /// 0xc40
			 0x573128a4, /// 0xc44
			 0xa848c50f, /// 0xc48
			 0x5e8bec4e, /// 0xc4c
			 0x4ada332c, /// 0xc50
			 0x5a8b3f67, /// 0xc54
			 0xee10cb40, /// 0xc58
			 0xe7f69513, /// 0xc5c
			 0x448b1232, /// 0xc60
			 0xa08c2ca0, /// 0xc64
			 0xb44d44ec, /// 0xc68
			 0xf2b04a34, /// 0xc6c
			 0xac481a4a, /// 0xc70
			 0x2ff87ab2, /// 0xc74
			 0x51b660e0, /// 0xc78
			 0x5f853e49, /// 0xc7c
			 0x59fcbf49, /// 0xc80
			 0x8e61685c, /// 0xc84
			 0x8ba05722, /// 0xc88
			 0x95e45c91, /// 0xc8c
			 0x21543a77, /// 0xc90
			 0x97a0d5c7, /// 0xc94
			 0x56af6295, /// 0xc98
			 0xbf2d91b4, /// 0xc9c
			 0x643dffd6, /// 0xca0
			 0x464f7d31, /// 0xca4
			 0x8814a28b, /// 0xca8
			 0x65553b10, /// 0xcac
			 0x44a4c26d, /// 0xcb0
			 0x64a2be4c, /// 0xcb4
			 0x6256cbaa, /// 0xcb8
			 0xb3e15254, /// 0xcbc
			 0xb8751d2e, /// 0xcc0
			 0x496ee14a, /// 0xcc4
			 0x2c89502c, /// 0xcc8
			 0x2dd486a8, /// 0xccc
			 0x1f0e1c5b, /// 0xcd0
			 0x6b3da2cf, /// 0xcd4
			 0x0c357493, /// 0xcd8
			 0x19aa86dc, /// 0xcdc
			 0xe2ca14d6, /// 0xce0
			 0x6aba3b8b, /// 0xce4
			 0x307a87a0, /// 0xce8
			 0xcc112895, /// 0xcec
			 0xfaa3cde8, /// 0xcf0
			 0x9e66b95a, /// 0xcf4
			 0xe7dce2b3, /// 0xcf8
			 0xb6d7a70e, /// 0xcfc
			 0xff43a5c8, /// 0xd00
			 0x0031efc8, /// 0xd04
			 0x32653114, /// 0xd08
			 0x8cf396eb, /// 0xd0c
			 0x415675d8, /// 0xd10
			 0xe80f359f, /// 0xd14
			 0xc6a4748a, /// 0xd18
			 0x79acb3f2, /// 0xd1c
			 0x17de2fd7, /// 0xd20
			 0x8a22267e, /// 0xd24
			 0xb0f3686d, /// 0xd28
			 0xcb392d7f, /// 0xd2c
			 0x95d9e791, /// 0xd30
			 0x9cbb10ff, /// 0xd34
			 0xd1909567, /// 0xd38
			 0x80136d25, /// 0xd3c
			 0x4fa8ad7f, /// 0xd40
			 0xb376490b, /// 0xd44
			 0x29e72c9a, /// 0xd48
			 0xce56e5e4, /// 0xd4c
			 0xf6d813c4, /// 0xd50
			 0x2b7515a7, /// 0xd54
			 0x4fd3f874, /// 0xd58
			 0x09fb0849, /// 0xd5c
			 0xd9dc8c3b, /// 0xd60
			 0x8151b230, /// 0xd64
			 0xd3fa0c58, /// 0xd68
			 0xf9d40f38, /// 0xd6c
			 0xb062b9ff, /// 0xd70
			 0x33fa7d67, /// 0xd74
			 0x887bdac0, /// 0xd78
			 0xb54b79e0, /// 0xd7c
			 0x31779761, /// 0xd80
			 0x34d3a4c5, /// 0xd84
			 0x5f398d22, /// 0xd88
			 0x2a92e547, /// 0xd8c
			 0x73db6831, /// 0xd90
			 0x30883033, /// 0xd94
			 0x534c9573, /// 0xd98
			 0x5257375c, /// 0xd9c
			 0x2688b37d, /// 0xda0
			 0x599557e6, /// 0xda4
			 0xfc0995c5, /// 0xda8
			 0x743fab4b, /// 0xdac
			 0xe4336538, /// 0xdb0
			 0xd10112f1, /// 0xdb4
			 0xa7dc81d6, /// 0xdb8
			 0x438b7fb1, /// 0xdbc
			 0x0bb69253, /// 0xdc0
			 0x5b78cd88, /// 0xdc4
			 0xbbb8adcb, /// 0xdc8
			 0xe59830a5, /// 0xdcc
			 0x611e53d8, /// 0xdd0
			 0x1ebbdb9b, /// 0xdd4
			 0xb39a85d2, /// 0xdd8
			 0x538c1b7d, /// 0xddc
			 0xa35638f0, /// 0xde0
			 0xc031b643, /// 0xde4
			 0x98eda1ef, /// 0xde8
			 0x40b16606, /// 0xdec
			 0x68c4bbfb, /// 0xdf0
			 0xe4e71542, /// 0xdf4
			 0xb712117a, /// 0xdf8
			 0x71513338, /// 0xdfc
			 0x64bb2c43, /// 0xe00
			 0x60b1f67c, /// 0xe04
			 0x2867b332, /// 0xe08
			 0xea9c226b, /// 0xe0c
			 0xe4d2ea4c, /// 0xe10
			 0x39ebfccb, /// 0xe14
			 0x77764473, /// 0xe18
			 0xa964ed16, /// 0xe1c
			 0xe3075bfa, /// 0xe20
			 0xb4c94cfa, /// 0xe24
			 0x1c526608, /// 0xe28
			 0x50b99e08, /// 0xe2c
			 0xe8514ffa, /// 0xe30
			 0x5b0fcee7, /// 0xe34
			 0xd851a10a, /// 0xe38
			 0xabd5152d, /// 0xe3c
			 0x829fef54, /// 0xe40
			 0xa1c5cb57, /// 0xe44
			 0xd25ba892, /// 0xe48
			 0xad0942df, /// 0xe4c
			 0xf470858c, /// 0xe50
			 0x5b661410, /// 0xe54
			 0x0684c7ee, /// 0xe58
			 0x886d80e7, /// 0xe5c
			 0xc00b7e2f, /// 0xe60
			 0xed928edf, /// 0xe64
			 0x4891da90, /// 0xe68
			 0x8c4fd958, /// 0xe6c
			 0x53d02b4f, /// 0xe70
			 0x239071bb, /// 0xe74
			 0x17d58722, /// 0xe78
			 0x1e98e2c5, /// 0xe7c
			 0xb82e0286, /// 0xe80
			 0x08cb6f7c, /// 0xe84
			 0x2f01d8cc, /// 0xe88
			 0xf2dc08a9, /// 0xe8c
			 0x0a90a329, /// 0xe90
			 0x0cdb6ab3, /// 0xe94
			 0x3658eb10, /// 0xe98
			 0xcff7f424, /// 0xe9c
			 0x2543dd68, /// 0xea0
			 0x3d16e9d5, /// 0xea4
			 0x55a4031f, /// 0xea8
			 0x4dba4aa6, /// 0xeac
			 0xee1943d4, /// 0xeb0
			 0xa5f41ee3, /// 0xeb4
			 0xf823de65, /// 0xeb8
			 0xde242fc5, /// 0xebc
			 0x542288e3, /// 0xec0
			 0x58e8665a, /// 0xec4
			 0x8fe12ea1, /// 0xec8
			 0xf916c56c, /// 0xecc
			 0x5ff3ba38, /// 0xed0
			 0x106068d6, /// 0xed4
			 0xab8c03da, /// 0xed8
			 0x9c690cae, /// 0xedc
			 0x524059bd, /// 0xee0
			 0xf54aed5a, /// 0xee4
			 0x40e046f4, /// 0xee8
			 0x605ddabe, /// 0xeec
			 0x8fb1bb3e, /// 0xef0
			 0xe42cfab6, /// 0xef4
			 0x10f1f7b4, /// 0xef8
			 0x930fa072, /// 0xefc
			 0x37547a8b, /// 0xf00
			 0x9f0464bf, /// 0xf04
			 0x425c9620, /// 0xf08
			 0xbb6c53a6, /// 0xf0c
			 0x633dcd72, /// 0xf10
			 0x2a83399c, /// 0xf14
			 0xa821202c, /// 0xf18
			 0xbbb88781, /// 0xf1c
			 0x61e8dd21, /// 0xf20
			 0x1d3699f7, /// 0xf24
			 0x1306b022, /// 0xf28
			 0xfb334eb7, /// 0xf2c
			 0x0361646c, /// 0xf30
			 0x74b9745e, /// 0xf34
			 0x0285fb0f, /// 0xf38
			 0xaa7c4ecd, /// 0xf3c
			 0x621d7400, /// 0xf40
			 0x6e0b7e54, /// 0xf44
			 0x65a800d1, /// 0xf48
			 0xdda6a074, /// 0xf4c
			 0xac8cbc33, /// 0xf50
			 0x8bd99cda, /// 0xf54
			 0xd526f22b, /// 0xf58
			 0xddd49914, /// 0xf5c
			 0x78a6fa33, /// 0xf60
			 0x1dcceefe, /// 0xf64
			 0x014466c0, /// 0xf68
			 0x44ef9224, /// 0xf6c
			 0x077665f7, /// 0xf70
			 0x5db5510d, /// 0xf74
			 0x155e98a0, /// 0xf78
			 0xda860d96, /// 0xf7c
			 0x55d8685c, /// 0xf80
			 0x95a35387, /// 0xf84
			 0x811c6c82, /// 0xf88
			 0x64f60316, /// 0xf8c
			 0xd7f2ddc6, /// 0xf90
			 0xd72ebbaf, /// 0xf94
			 0x73ad5668, /// 0xf98
			 0x2db2f4cd, /// 0xf9c
			 0x7eafc8fc, /// 0xfa0
			 0x3bf57269, /// 0xfa4
			 0xca489cf7, /// 0xfa8
			 0xc473b255, /// 0xfac
			 0xc3aff72c, /// 0xfb0
			 0xd80ae6c6, /// 0xfb4
			 0x5d108ff8, /// 0xfb8
			 0xcb5619c9, /// 0xfbc
			 0x9a148d67, /// 0xfc0
			 0x9327698c, /// 0xfc4
			 0xf01f156d, /// 0xfc8
			 0x12701ed3, /// 0xfcc
			 0x2a3f6040, /// 0xfd0
			 0xb28cef9b, /// 0xfd4
			 0x7bdf2db4, /// 0xfd8
			 0x6cff7c2a, /// 0xfdc
			 0x493f0bc4, /// 0xfe0
			 0x3693b4f0, /// 0xfe4
			 0x01cd5e92, /// 0xfe8
			 0xc6f9dcf6, /// 0xfec
			 0xbfcb189b, /// 0xff0
			 0x1d7034ae, /// 0xff4
			 0xa0067541, /// 0xff8
			 0x0e10aa4c /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00004
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00008
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0000c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00010
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00018
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0001c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00020
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00024
			 0xffc00001,                                                  // -signaling NaN                              /// 00028
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0002c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00030
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00034
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0003c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00044
			 0x00011111,                                                  // 9.7958E-41                                  /// 00048
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0004c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00050
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00054
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00058
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0005c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00060
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00064
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00070
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00074
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00078
			 0x0e000007,                                                  // Trailing 1s:                                /// 0007c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00080
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00084
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xffc00001,                                                  // -signaling NaN                              /// 0008c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00090
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00094
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0009c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000e4
			 0xff800000,                                                  // -inf                                        /// 000e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00100
			 0x0e000003,                                                  // Trailing 1s:                                /// 00104
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00108
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0010c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00110
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00128
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00130
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0013c
			 0x7fc00001,                                                  // signaling NaN                               /// 00140
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00148
			 0xffc00001,                                                  // -signaling NaN                              /// 0014c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00154
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0015c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00164
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00168
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00170
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00178
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00180
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00184
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00188
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0018c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00190
			 0x0e000003,                                                  // Trailing 1s:                                /// 00194
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00198
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0019c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001bc
			 0x55555555,                                                  // 4 random values                             /// 001c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00200
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00208
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xbf028f5c,                                                  // -0.51                                       /// 00210
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00218
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0021c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00220
			 0x33333333,                                                  // 4 random values                             /// 00224
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00228
			 0x55555555,                                                  // 4 random values                             /// 0022c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00230
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00240
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00244
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00248
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00250
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00258
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0025c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00264
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xbf028f5c,                                                  // -0.51                                       /// 00278
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0027c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00280
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0028c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00290
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00298
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0029c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b0
			 0xff800000,                                                  // -inf                                        /// 002b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00304
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00308
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00310
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00314
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0031c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00320
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00334
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00338
			 0x0c780000,                                                  // Leading 1s:                                 /// 0033c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00344
			 0xbf028f5c,                                                  // -0.51                                       /// 00348
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00350
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00354
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x55555555,                                                  // 4 random values                             /// 0035c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00368
			 0x80011111,                                                  // -9.7958E-41                                 /// 0036c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00370
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0037c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00380
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0c780000,                                                  // Leading 1s:                                 /// 00388
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0038c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00390
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00394
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0039c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b8
			 0x80000000,                                                  // -zero                                       /// 003bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00400
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00404
			 0x4b000000,                                                  // 8388608.0                                   /// 00408
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0040c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00418
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0041c
			 0xff800000,                                                  // -inf                                        /// 00420
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00424
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00428
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0042c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00430
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00440
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0044c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00450
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00454
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00458
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0045c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00460
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00464
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0046c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00470
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c600000,                                                  // Leading 1s:                                 /// 0047c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00484
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00488
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0048c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00490
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00498
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004dc
			 0xbf028f5c,                                                  // -0.51                                       /// 004e0
			 0x80000000,                                                  // -zero                                       /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004fc
			 0x33333333,                                                  // 4 random values                             /// 00500
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00504
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00508
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00514
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00518
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0051c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00520
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00524
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xbf028f5c,                                                  // -0.51                                       /// 0052c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00530
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0053c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00540
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x7fc00001,                                                  // signaling NaN                               /// 0054c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00554
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00558
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00560
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00564
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00568
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0056c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00570
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00574
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0057c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00584
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00588
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0058c
			 0xbf028f5c,                                                  // -0.51                                       /// 00590
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00594
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00598
			 0xcb000000,                                                  // -8388608.0                                  /// 0059c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a4
			 0x3f028f5c,                                                  // 0.51                                        /// 005a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005e0
			 0x33333333,                                                  // 4 random values                             /// 005e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00600
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00608
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0060c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00614
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00618
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0061c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00620
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00624
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0062c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00630
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00634
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00638
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00640
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00644
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00648
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00650
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00654
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00658
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0065c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c780000,                                                  // Leading 1s:                                 /// 00664
			 0x3f028f5c,                                                  // 0.51                                        /// 00668
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0066c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00674
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0067c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00684
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00688
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0068c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00690
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00698
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006b8
			 0x00000000,                                                  // zero                                        /// 006bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c0
			 0x3f028f5c,                                                  // 0.51                                        /// 006c4
			 0x4b000000,                                                  // 8388608.0                                   /// 006c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006f0
			 0xbf028f5c,                                                  // -0.51                                       /// 006f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00704
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00708
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00710
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x55555555,                                                  // 4 random values                             /// 00718
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00720
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00724
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00730
			 0x0e000001,                                                  // Trailing 1s:                                /// 00734
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00738
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00744
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0074c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00750
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00754
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x0c600000,                                                  // Leading 1s:                                 /// 0075c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00760
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00764
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0076c
			 0x55555555,                                                  // 4 random values                             /// 00770
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00774
			 0x80011111,                                                  // -9.7958E-41                                 /// 00778
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0077c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00780
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0c700000,                                                  // Leading 1s:                                 /// 00788
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00794
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00798
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0079c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x55555555,                                                  // 4 random values                             /// 007b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007bc
			 0xff800000,                                                  // -inf                                        /// 007c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00810
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0081c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00000000,                                                  // zero                                        /// 00824
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0082c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0x4b000000,                                                  // 8388608.0                                   /// 00838
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0083c
			 0xcb000000,                                                  // -8388608.0                                  /// 00840
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00844
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00850
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00864
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00868
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00870
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00874
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0087c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00880
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00884
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00888
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00894
			 0xff800000,                                                  // -inf                                        /// 00898
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0089c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00900
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00904
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0090c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00910
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00918
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0091c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00920
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00928
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0092c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00930
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00934
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0093c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00944
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00948
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0094c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00958
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0095c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00964
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00968
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00970
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x0c780000,                                                  // Leading 1s:                                 /// 0097c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00984
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00988
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00990
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00994
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00998
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x55555555,                                                  // 4 random values                             /// 009ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d8
			 0x80000000,                                                  // -zero                                       /// 009dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009ec
			 0x7fc00001,                                                  // signaling NaN                               /// 009f0
			 0x55555555,                                                  // 4 random values                             /// 009f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a34
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a70
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ab0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ad4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ad8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00af0
			 0xffc00001,                                                  // -signaling NaN                              /// 00af4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00af8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00afc
			 0x55555555,                                                  // 4 random values                             /// 00b00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0xbf028f5c,                                                  // -0.51                                       /// 00b28
			 0xbf028f5c,                                                  // -0.51                                       /// 00b2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b34
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x33333333,                                                  // 4 random values                             /// 00b5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b60
			 0xbf028f5c,                                                  // -0.51                                       /// 00b64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0xff800000,                                                  // -inf                                        /// 00ba0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ba4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ba8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bcc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00be0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00be4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bf0
			 0x55555555,                                                  // 4 random values                             /// 00bf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xffc00001,                                                  // -signaling NaN                              /// 00c14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80000000,                                                  // -zero                                       /// 00c30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ccc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cfc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00db0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00db8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00dc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0xff800000,                                                  // -inf                                        /// 00dd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x33333333,                                                  // 4 random values                             /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e48
			 0xbf028f5c,                                                  // -0.51                                       /// 00e4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e68
			 0xbf028f5c,                                                  // -0.51                                       /// 00e6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eac
			 0x55555555,                                                  // 4 random values                             /// 00eb0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ebc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ec0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ecc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ed8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00edc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ee8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ef4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00efc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f50
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80000000,                                                  // -zero                                       /// 00f90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x7f800000,                                                  // inf                                         /// 00fdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ff0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff8
			 0x00000000                                                  // zero                                        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xe7fbe95b, /// 0x0
			 0x7b9b9d50, /// 0x4
			 0xe908f608, /// 0x8
			 0x1d17b921, /// 0xc
			 0xc859b412, /// 0x10
			 0x5ebdca22, /// 0x14
			 0x2187d30c, /// 0x18
			 0x281bd0bd, /// 0x1c
			 0x45e8201f, /// 0x20
			 0xf673def8, /// 0x24
			 0x4ad721a0, /// 0x28
			 0x28bfe2be, /// 0x2c
			 0xf8b961c7, /// 0x30
			 0x250811e2, /// 0x34
			 0xc69c4105, /// 0x38
			 0x1d25ee8b, /// 0x3c
			 0xef255839, /// 0x40
			 0xf77ed20a, /// 0x44
			 0x7d374315, /// 0x48
			 0x19321656, /// 0x4c
			 0xc3bdc26d, /// 0x50
			 0x418231c2, /// 0x54
			 0x38d31fa5, /// 0x58
			 0x3cae50f4, /// 0x5c
			 0xdbbe79db, /// 0x60
			 0x74e94cc9, /// 0x64
			 0x736ab464, /// 0x68
			 0xfd995673, /// 0x6c
			 0x55dc2c20, /// 0x70
			 0xed5f2d6e, /// 0x74
			 0x2e8601e6, /// 0x78
			 0x7122046f, /// 0x7c
			 0xfcb7482b, /// 0x80
			 0xbbf4d265, /// 0x84
			 0x5a0007c8, /// 0x88
			 0xc7da00af, /// 0x8c
			 0xaf37fb53, /// 0x90
			 0x0ae62644, /// 0x94
			 0x944d9e77, /// 0x98
			 0x63cbab17, /// 0x9c
			 0x4333d8c5, /// 0xa0
			 0x18cf3c05, /// 0xa4
			 0x5b87aaa6, /// 0xa8
			 0x91e15498, /// 0xac
			 0x556f2a65, /// 0xb0
			 0x1a4fda8b, /// 0xb4
			 0x0c2d26f9, /// 0xb8
			 0x18fe3b7a, /// 0xbc
			 0x7b4f173b, /// 0xc0
			 0xbc6f45bb, /// 0xc4
			 0xe3224cc0, /// 0xc8
			 0x241377d2, /// 0xcc
			 0x29ee78b4, /// 0xd0
			 0xacdc4a8a, /// 0xd4
			 0xf5648362, /// 0xd8
			 0xa6ec2f85, /// 0xdc
			 0x7a3d2e5b, /// 0xe0
			 0x2dbbda88, /// 0xe4
			 0x3f3a046d, /// 0xe8
			 0x04b99af9, /// 0xec
			 0xb7537a25, /// 0xf0
			 0xa1b2634b, /// 0xf4
			 0x9816afe2, /// 0xf8
			 0x9ebf3b24, /// 0xfc
			 0x7c71ec95, /// 0x100
			 0xee5c3fb7, /// 0x104
			 0xe598221e, /// 0x108
			 0xdc68b243, /// 0x10c
			 0x4587de88, /// 0x110
			 0xd700c8a4, /// 0x114
			 0x08e76096, /// 0x118
			 0xd4cc57dc, /// 0x11c
			 0x291ab5aa, /// 0x120
			 0x0b36e489, /// 0x124
			 0xda1f512d, /// 0x128
			 0x67b89df4, /// 0x12c
			 0x7bd26c7e, /// 0x130
			 0xffaa04c5, /// 0x134
			 0xb2f4060c, /// 0x138
			 0xc04bcc1d, /// 0x13c
			 0xa8216941, /// 0x140
			 0xd0cda182, /// 0x144
			 0xa0dab7ba, /// 0x148
			 0x597870aa, /// 0x14c
			 0xb24c42a5, /// 0x150
			 0xe5fca1b1, /// 0x154
			 0x4dd85390, /// 0x158
			 0x8445d20d, /// 0x15c
			 0xaca5ecc9, /// 0x160
			 0x5c4b2c1e, /// 0x164
			 0xa2c4c654, /// 0x168
			 0xb2d788c0, /// 0x16c
			 0x3ca72e60, /// 0x170
			 0x27ce6c9c, /// 0x174
			 0xfde1d677, /// 0x178
			 0x21845fa1, /// 0x17c
			 0x0cf08aee, /// 0x180
			 0xfd9df18e, /// 0x184
			 0x28b87eb4, /// 0x188
			 0xf983debb, /// 0x18c
			 0x2428c3da, /// 0x190
			 0xc2b6ae60, /// 0x194
			 0x27bd09b3, /// 0x198
			 0xcce6c0a1, /// 0x19c
			 0xab3828b7, /// 0x1a0
			 0xd747b5c2, /// 0x1a4
			 0x38d85c01, /// 0x1a8
			 0xcd3a8fc4, /// 0x1ac
			 0xd8585a73, /// 0x1b0
			 0x984fc64a, /// 0x1b4
			 0x88b21720, /// 0x1b8
			 0x59597926, /// 0x1bc
			 0xb7d4ffea, /// 0x1c0
			 0xdf4e8c96, /// 0x1c4
			 0xad10a1a4, /// 0x1c8
			 0x8b5aa2ea, /// 0x1cc
			 0xa6ec215e, /// 0x1d0
			 0x8889f5b9, /// 0x1d4
			 0x686e735b, /// 0x1d8
			 0xf1ef1abd, /// 0x1dc
			 0xc9f1906e, /// 0x1e0
			 0x6af4dc45, /// 0x1e4
			 0xa97d946e, /// 0x1e8
			 0x23d9ee95, /// 0x1ec
			 0x3c635f7f, /// 0x1f0
			 0xcbc89362, /// 0x1f4
			 0x9ba84ccf, /// 0x1f8
			 0xde599004, /// 0x1fc
			 0xb155c2dd, /// 0x200
			 0xd7fecb9c, /// 0x204
			 0xdad0c982, /// 0x208
			 0x7287aa8b, /// 0x20c
			 0x451e2a48, /// 0x210
			 0xef89b22b, /// 0x214
			 0x693a8063, /// 0x218
			 0xd713cddb, /// 0x21c
			 0xc5bae93a, /// 0x220
			 0xea058aa5, /// 0x224
			 0xe5ba40cf, /// 0x228
			 0x21e80ed4, /// 0x22c
			 0x3e2877ba, /// 0x230
			 0xc18022e1, /// 0x234
			 0xe40cc9af, /// 0x238
			 0x82b6287d, /// 0x23c
			 0x59448057, /// 0x240
			 0x9f91f6ee, /// 0x244
			 0xc8c3f106, /// 0x248
			 0x9f59da05, /// 0x24c
			 0x934f9698, /// 0x250
			 0xe31b877c, /// 0x254
			 0x3f99d96c, /// 0x258
			 0xfcc0a428, /// 0x25c
			 0xc6ec8864, /// 0x260
			 0x706659ae, /// 0x264
			 0x71233695, /// 0x268
			 0x4a5e7613, /// 0x26c
			 0x85321a74, /// 0x270
			 0x9c2023a2, /// 0x274
			 0xba18844f, /// 0x278
			 0xd6bfcb11, /// 0x27c
			 0x5c6fe0dd, /// 0x280
			 0xb3adac1e, /// 0x284
			 0x884f2816, /// 0x288
			 0x18f8c04b, /// 0x28c
			 0xea545ba4, /// 0x290
			 0xf179a3d2, /// 0x294
			 0xc170715e, /// 0x298
			 0xb0eda6df, /// 0x29c
			 0x8215644d, /// 0x2a0
			 0x1527fbe2, /// 0x2a4
			 0x186e372f, /// 0x2a8
			 0x5873b0f1, /// 0x2ac
			 0x1f4eb1b3, /// 0x2b0
			 0x49b2dd91, /// 0x2b4
			 0x029ab9c2, /// 0x2b8
			 0xcbff6747, /// 0x2bc
			 0xc977dfa6, /// 0x2c0
			 0xf924f120, /// 0x2c4
			 0x7824b27d, /// 0x2c8
			 0xed66ab8d, /// 0x2cc
			 0x4d8d673a, /// 0x2d0
			 0xbaa67e9a, /// 0x2d4
			 0x1531f7ac, /// 0x2d8
			 0xe2bf5c42, /// 0x2dc
			 0xd45839d6, /// 0x2e0
			 0x69de5647, /// 0x2e4
			 0xae7ed661, /// 0x2e8
			 0x8f7a08ac, /// 0x2ec
			 0x30c3c103, /// 0x2f0
			 0xac3f4973, /// 0x2f4
			 0x8dd50128, /// 0x2f8
			 0x4347b5bc, /// 0x2fc
			 0x0970f808, /// 0x300
			 0xe884fdcf, /// 0x304
			 0x72ca894d, /// 0x308
			 0xcc175a97, /// 0x30c
			 0xa7b544aa, /// 0x310
			 0x7d8e9e75, /// 0x314
			 0x8dc1f0ae, /// 0x318
			 0x36bd328d, /// 0x31c
			 0x41574e77, /// 0x320
			 0x6a0a6065, /// 0x324
			 0x895ff8f2, /// 0x328
			 0xec469c74, /// 0x32c
			 0x41213f16, /// 0x330
			 0x53162370, /// 0x334
			 0x7f63c3cf, /// 0x338
			 0x76534ffc, /// 0x33c
			 0x645ddfc0, /// 0x340
			 0xaa86f1bb, /// 0x344
			 0xa331452d, /// 0x348
			 0x40b4b52e, /// 0x34c
			 0xfc291052, /// 0x350
			 0xa1463ff7, /// 0x354
			 0xf9d535e6, /// 0x358
			 0x830d4f8d, /// 0x35c
			 0x94981206, /// 0x360
			 0xda12408f, /// 0x364
			 0x4b5c896c, /// 0x368
			 0xdf2828ae, /// 0x36c
			 0x099379c3, /// 0x370
			 0x72f23a6e, /// 0x374
			 0x9ceb1f81, /// 0x378
			 0x7682af13, /// 0x37c
			 0x99415858, /// 0x380
			 0x8b90aa93, /// 0x384
			 0x092f0984, /// 0x388
			 0xeadb84b5, /// 0x38c
			 0x0cb798fe, /// 0x390
			 0xe406ef7d, /// 0x394
			 0x04657bc5, /// 0x398
			 0xbcca2bf9, /// 0x39c
			 0xc1ede24a, /// 0x3a0
			 0xdee277f6, /// 0x3a4
			 0x875d0c0a, /// 0x3a8
			 0x265bbb36, /// 0x3ac
			 0x0591c435, /// 0x3b0
			 0x2d11855d, /// 0x3b4
			 0x3242b9ea, /// 0x3b8
			 0xc50cce9f, /// 0x3bc
			 0x93f9cfbb, /// 0x3c0
			 0x0d4403bf, /// 0x3c4
			 0x23a47258, /// 0x3c8
			 0x66b482ae, /// 0x3cc
			 0xba914ff9, /// 0x3d0
			 0x4141c947, /// 0x3d4
			 0x6c1e96b5, /// 0x3d8
			 0x11259d58, /// 0x3dc
			 0xb36af1be, /// 0x3e0
			 0xfe6d8de0, /// 0x3e4
			 0xa160c171, /// 0x3e8
			 0x7e3df8cd, /// 0x3ec
			 0x0451ac49, /// 0x3f0
			 0x8be751af, /// 0x3f4
			 0xeab110b5, /// 0x3f8
			 0xb754bb65, /// 0x3fc
			 0xc0202af1, /// 0x400
			 0x0457a7a3, /// 0x404
			 0xacf9a0bd, /// 0x408
			 0xd6cbb317, /// 0x40c
			 0x97a20668, /// 0x410
			 0x5bf6e70d, /// 0x414
			 0xbe40bcec, /// 0x418
			 0x8e0ef2ca, /// 0x41c
			 0xd3af2d30, /// 0x420
			 0xc5363a07, /// 0x424
			 0xc4a3c0c6, /// 0x428
			 0xeec6ccbb, /// 0x42c
			 0xc4842e50, /// 0x430
			 0x6b5203d0, /// 0x434
			 0xf47ba7f0, /// 0x438
			 0xf79f17ba, /// 0x43c
			 0xec85e1bd, /// 0x440
			 0xb3ede34c, /// 0x444
			 0x3d8e2216, /// 0x448
			 0x79607274, /// 0x44c
			 0xb9d05002, /// 0x450
			 0x319c18a4, /// 0x454
			 0x98a07465, /// 0x458
			 0x3b8f2c28, /// 0x45c
			 0xbf68bbce, /// 0x460
			 0x773f3399, /// 0x464
			 0xc7d324fd, /// 0x468
			 0xbefd11c1, /// 0x46c
			 0x8fbeed79, /// 0x470
			 0x06ac7841, /// 0x474
			 0xf5cd322d, /// 0x478
			 0x39781536, /// 0x47c
			 0xa87226d7, /// 0x480
			 0x9df186c6, /// 0x484
			 0x7a5c7850, /// 0x488
			 0xe29ebea6, /// 0x48c
			 0x00257734, /// 0x490
			 0x45f4a6f1, /// 0x494
			 0x82a02313, /// 0x498
			 0x8bdb7506, /// 0x49c
			 0xb9a91695, /// 0x4a0
			 0x93850c5e, /// 0x4a4
			 0x352fe8b7, /// 0x4a8
			 0xfc4df4a1, /// 0x4ac
			 0xa4dd12bb, /// 0x4b0
			 0xa2de55ce, /// 0x4b4
			 0x6172b655, /// 0x4b8
			 0x7841a147, /// 0x4bc
			 0x25ee3566, /// 0x4c0
			 0xfa7b5fe9, /// 0x4c4
			 0xa5d51604, /// 0x4c8
			 0xd281aa0f, /// 0x4cc
			 0xe242b552, /// 0x4d0
			 0x4e603c14, /// 0x4d4
			 0xac03e9b2, /// 0x4d8
			 0xd9196fc3, /// 0x4dc
			 0xc4978cd3, /// 0x4e0
			 0x97184972, /// 0x4e4
			 0x7a34e31f, /// 0x4e8
			 0xce3d45bc, /// 0x4ec
			 0xd541da4a, /// 0x4f0
			 0x405bbd25, /// 0x4f4
			 0x502d0555, /// 0x4f8
			 0xc6ad7782, /// 0x4fc
			 0x5774277f, /// 0x500
			 0x1d926655, /// 0x504
			 0x765981c0, /// 0x508
			 0xbfb40d03, /// 0x50c
			 0x74ae5608, /// 0x510
			 0xe45b4bfd, /// 0x514
			 0x089ca0ef, /// 0x518
			 0xf62d4944, /// 0x51c
			 0x0a936dfb, /// 0x520
			 0x0aae9e49, /// 0x524
			 0x1836e6f9, /// 0x528
			 0x53e3a949, /// 0x52c
			 0x686a9a6e, /// 0x530
			 0x22fc40a9, /// 0x534
			 0x4556169c, /// 0x538
			 0x6cbfe997, /// 0x53c
			 0x4416ade5, /// 0x540
			 0x135a2c3f, /// 0x544
			 0x914f5673, /// 0x548
			 0xe46642e1, /// 0x54c
			 0x77783f65, /// 0x550
			 0xa4296b8d, /// 0x554
			 0xa3a0f49b, /// 0x558
			 0x0f2d021e, /// 0x55c
			 0x01e5de19, /// 0x560
			 0xf094675e, /// 0x564
			 0x38b39e59, /// 0x568
			 0x7adc638e, /// 0x56c
			 0xd65e5179, /// 0x570
			 0x5672f417, /// 0x574
			 0xd93c5165, /// 0x578
			 0x649a53fc, /// 0x57c
			 0xbe967f5d, /// 0x580
			 0x73bf5ce4, /// 0x584
			 0x81766617, /// 0x588
			 0x9b9e112e, /// 0x58c
			 0x041f32d5, /// 0x590
			 0xb61e560c, /// 0x594
			 0x177d4e46, /// 0x598
			 0xaecf6471, /// 0x59c
			 0xc0b15bd3, /// 0x5a0
			 0xa7b71663, /// 0x5a4
			 0x9578c092, /// 0x5a8
			 0x8dd5218f, /// 0x5ac
			 0x527ad139, /// 0x5b0
			 0x3035d018, /// 0x5b4
			 0xecbbfd45, /// 0x5b8
			 0x9a1fb4ef, /// 0x5bc
			 0x7bd7c2e6, /// 0x5c0
			 0x8a8136f1, /// 0x5c4
			 0x46d77b1c, /// 0x5c8
			 0x38eea615, /// 0x5cc
			 0x24cf1688, /// 0x5d0
			 0x0f8ff4c6, /// 0x5d4
			 0x5ada6378, /// 0x5d8
			 0xa9e3ad30, /// 0x5dc
			 0x99c75b4f, /// 0x5e0
			 0xa7fc1e3f, /// 0x5e4
			 0x2972f850, /// 0x5e8
			 0xe9d4c25d, /// 0x5ec
			 0x23f42a22, /// 0x5f0
			 0x846e1260, /// 0x5f4
			 0x6454e6f1, /// 0x5f8
			 0xe7f516d3, /// 0x5fc
			 0xfca0f007, /// 0x600
			 0xb7650398, /// 0x604
			 0x9308c598, /// 0x608
			 0x0d78c7c8, /// 0x60c
			 0xad0c7da0, /// 0x610
			 0x56a03100, /// 0x614
			 0x13877c12, /// 0x618
			 0xc4d00bef, /// 0x61c
			 0xee8c7abc, /// 0x620
			 0x854e7a65, /// 0x624
			 0x98ac642a, /// 0x628
			 0xed5d29e5, /// 0x62c
			 0x5ce8d157, /// 0x630
			 0xfacf4fce, /// 0x634
			 0x1fc64d0d, /// 0x638
			 0xdc85fe4b, /// 0x63c
			 0xa969d8ee, /// 0x640
			 0x6ec7f730, /// 0x644
			 0x86aafa91, /// 0x648
			 0x4c732c33, /// 0x64c
			 0x13510e0f, /// 0x650
			 0x9153bebf, /// 0x654
			 0x9ab4fed9, /// 0x658
			 0xd4e28a45, /// 0x65c
			 0xcdb67a43, /// 0x660
			 0xcc0947b8, /// 0x664
			 0x953f96c7, /// 0x668
			 0xa8b926de, /// 0x66c
			 0xd3fcc3d2, /// 0x670
			 0x2bddcfe8, /// 0x674
			 0x526bb7f9, /// 0x678
			 0x0a7685dd, /// 0x67c
			 0x901995a6, /// 0x680
			 0x5a1831c0, /// 0x684
			 0x001280c5, /// 0x688
			 0xcd90796c, /// 0x68c
			 0x5ba9ce8b, /// 0x690
			 0x4773a922, /// 0x694
			 0x64437dab, /// 0x698
			 0x14f90b93, /// 0x69c
			 0x97dc5169, /// 0x6a0
			 0x0bde39bb, /// 0x6a4
			 0xa110ff43, /// 0x6a8
			 0xb5a5bc06, /// 0x6ac
			 0xa01a2105, /// 0x6b0
			 0x74cb761f, /// 0x6b4
			 0xb5455f00, /// 0x6b8
			 0xf8ffc4d7, /// 0x6bc
			 0x519f5061, /// 0x6c0
			 0x357a6325, /// 0x6c4
			 0x5ba62806, /// 0x6c8
			 0x92b2beba, /// 0x6cc
			 0x660decf4, /// 0x6d0
			 0x9a4476d8, /// 0x6d4
			 0x8bc09ba4, /// 0x6d8
			 0x71856a29, /// 0x6dc
			 0x3ee64f27, /// 0x6e0
			 0x5321b014, /// 0x6e4
			 0x5f0363ac, /// 0x6e8
			 0x7cc22ade, /// 0x6ec
			 0x6da62a69, /// 0x6f0
			 0x78976759, /// 0x6f4
			 0xb34fe2da, /// 0x6f8
			 0xca857ac6, /// 0x6fc
			 0x8a8e70bc, /// 0x700
			 0x8e1944ee, /// 0x704
			 0x81db6e5a, /// 0x708
			 0x81637f4c, /// 0x70c
			 0xa0cc6459, /// 0x710
			 0x4377683a, /// 0x714
			 0x73c65f8d, /// 0x718
			 0xe21ab804, /// 0x71c
			 0x96e30d9b, /// 0x720
			 0x629f74ce, /// 0x724
			 0xf60705ec, /// 0x728
			 0x59b6f2f8, /// 0x72c
			 0xc020b6aa, /// 0x730
			 0x4e415577, /// 0x734
			 0x5d4b9022, /// 0x738
			 0x844aa457, /// 0x73c
			 0xcbdbeaeb, /// 0x740
			 0xcc837662, /// 0x744
			 0xa1aafee0, /// 0x748
			 0xa5489e04, /// 0x74c
			 0xbcb499aa, /// 0x750
			 0x1ba9e5c3, /// 0x754
			 0x4bc523a2, /// 0x758
			 0xf1d880f6, /// 0x75c
			 0x12ae6ae6, /// 0x760
			 0xdea5a548, /// 0x764
			 0x4ce49eb5, /// 0x768
			 0x1ed13223, /// 0x76c
			 0x68957692, /// 0x770
			 0x5dd2212f, /// 0x774
			 0x2d191e91, /// 0x778
			 0xa21d1abc, /// 0x77c
			 0x14300ef5, /// 0x780
			 0xb02800d5, /// 0x784
			 0xac3fba93, /// 0x788
			 0x10fd0986, /// 0x78c
			 0xfa992b22, /// 0x790
			 0xf7ff1864, /// 0x794
			 0xb1e446e0, /// 0x798
			 0x03ca2a4f, /// 0x79c
			 0x051fe944, /// 0x7a0
			 0xcc06e4c5, /// 0x7a4
			 0xb5617f1c, /// 0x7a8
			 0x5ea14248, /// 0x7ac
			 0x944220dc, /// 0x7b0
			 0xc9c8ad68, /// 0x7b4
			 0x10d6ab85, /// 0x7b8
			 0x78c2bf51, /// 0x7bc
			 0x84505958, /// 0x7c0
			 0xcba3c22b, /// 0x7c4
			 0x5b743291, /// 0x7c8
			 0x7c07ef15, /// 0x7cc
			 0x92fe44a1, /// 0x7d0
			 0xcdb4b383, /// 0x7d4
			 0x22a76097, /// 0x7d8
			 0x0a9b76eb, /// 0x7dc
			 0x855220e2, /// 0x7e0
			 0x79fa11e7, /// 0x7e4
			 0x191bdffa, /// 0x7e8
			 0xd52d36b1, /// 0x7ec
			 0xd8818e1c, /// 0x7f0
			 0x60d3b3c4, /// 0x7f4
			 0x1a898b2d, /// 0x7f8
			 0x1b80efbe, /// 0x7fc
			 0x53c04ddf, /// 0x800
			 0x7b5a3298, /// 0x804
			 0x21722ca6, /// 0x808
			 0xae9bba1a, /// 0x80c
			 0x23ab199c, /// 0x810
			 0x7aa08ecc, /// 0x814
			 0xd62cc570, /// 0x818
			 0xc3a55e5f, /// 0x81c
			 0xfdcee12d, /// 0x820
			 0xf2709990, /// 0x824
			 0x0edd72cb, /// 0x828
			 0x2e8fe74b, /// 0x82c
			 0xaddcc324, /// 0x830
			 0xe6094bf3, /// 0x834
			 0x81e78893, /// 0x838
			 0x07e105aa, /// 0x83c
			 0x4fbeb45b, /// 0x840
			 0x03de4e7e, /// 0x844
			 0x0faf173b, /// 0x848
			 0x9985fb29, /// 0x84c
			 0x872d6d11, /// 0x850
			 0xa49a7354, /// 0x854
			 0x98029e83, /// 0x858
			 0xd780a9e8, /// 0x85c
			 0xc61c9c8c, /// 0x860
			 0xf4d34768, /// 0x864
			 0x85de123f, /// 0x868
			 0x114386ba, /// 0x86c
			 0xb3b0b465, /// 0x870
			 0xf032b38d, /// 0x874
			 0x8f636527, /// 0x878
			 0x9e5138ca, /// 0x87c
			 0x441c92b0, /// 0x880
			 0xb952df33, /// 0x884
			 0x883509cb, /// 0x888
			 0x50822b24, /// 0x88c
			 0xc19b9ca6, /// 0x890
			 0xad8866d5, /// 0x894
			 0x0442873a, /// 0x898
			 0x7d70fc87, /// 0x89c
			 0x595e18e8, /// 0x8a0
			 0xdaaea195, /// 0x8a4
			 0xc1871c2c, /// 0x8a8
			 0x4ec951f0, /// 0x8ac
			 0x75d59d4d, /// 0x8b0
			 0x42827ea2, /// 0x8b4
			 0x5092ca63, /// 0x8b8
			 0x92ca6dbd, /// 0x8bc
			 0x86f9fe96, /// 0x8c0
			 0xde614535, /// 0x8c4
			 0x3d8f0eae, /// 0x8c8
			 0x85bd1962, /// 0x8cc
			 0x3d732ff8, /// 0x8d0
			 0xbbd52158, /// 0x8d4
			 0xd5f7deb4, /// 0x8d8
			 0xcfe86336, /// 0x8dc
			 0x56d8829f, /// 0x8e0
			 0x97d147de, /// 0x8e4
			 0x0df1586c, /// 0x8e8
			 0xbda7618d, /// 0x8ec
			 0x3077dbb9, /// 0x8f0
			 0x76d5d180, /// 0x8f4
			 0x1cb2a923, /// 0x8f8
			 0xa99c840c, /// 0x8fc
			 0x412ce181, /// 0x900
			 0x9bf13f3b, /// 0x904
			 0x000d5188, /// 0x908
			 0x79f3eee9, /// 0x90c
			 0x7f34c08c, /// 0x910
			 0x45b7f129, /// 0x914
			 0x80160ea9, /// 0x918
			 0x9b58e454, /// 0x91c
			 0x4b09e1bb, /// 0x920
			 0xf5b802a6, /// 0x924
			 0xe7e4e873, /// 0x928
			 0xa77ddfd8, /// 0x92c
			 0x64a76ab8, /// 0x930
			 0xa6005c50, /// 0x934
			 0x8a40b2c8, /// 0x938
			 0x7e1d5346, /// 0x93c
			 0x9ce60b6f, /// 0x940
			 0xc3598889, /// 0x944
			 0x575629ab, /// 0x948
			 0x8693dfc8, /// 0x94c
			 0xb42236ac, /// 0x950
			 0xb2b0f0f6, /// 0x954
			 0x9750e703, /// 0x958
			 0xc823ecb7, /// 0x95c
			 0xe63f4944, /// 0x960
			 0x5d13695c, /// 0x964
			 0xc92d3e41, /// 0x968
			 0x96b20cc1, /// 0x96c
			 0x28174c24, /// 0x970
			 0x44f63c86, /// 0x974
			 0xf9b2c3ca, /// 0x978
			 0x9ac762cb, /// 0x97c
			 0x311fb844, /// 0x980
			 0xc159163c, /// 0x984
			 0x23271d52, /// 0x988
			 0xe92c6014, /// 0x98c
			 0x1bd145b0, /// 0x990
			 0xea1e9115, /// 0x994
			 0x0dcac05d, /// 0x998
			 0xb1e15886, /// 0x99c
			 0xaec24575, /// 0x9a0
			 0xf48154d0, /// 0x9a4
			 0x5006c4a8, /// 0x9a8
			 0x09f77700, /// 0x9ac
			 0x891673d7, /// 0x9b0
			 0xc7bb93df, /// 0x9b4
			 0x1f9ac694, /// 0x9b8
			 0xe136c579, /// 0x9bc
			 0x0316d204, /// 0x9c0
			 0x26eef9d4, /// 0x9c4
			 0xb64b1c5a, /// 0x9c8
			 0x8b103b05, /// 0x9cc
			 0xce8392b6, /// 0x9d0
			 0xdd58d273, /// 0x9d4
			 0xf9e2e9a0, /// 0x9d8
			 0x67ea23be, /// 0x9dc
			 0xc3756cf5, /// 0x9e0
			 0x85cced5a, /// 0x9e4
			 0xe6fc52c5, /// 0x9e8
			 0xebe2127b, /// 0x9ec
			 0xb5e8c5e1, /// 0x9f0
			 0xf3f1730b, /// 0x9f4
			 0xcd36359f, /// 0x9f8
			 0x10902ded, /// 0x9fc
			 0xf3fb5882, /// 0xa00
			 0x94e1a545, /// 0xa04
			 0xc4ab5dac, /// 0xa08
			 0x95e020e9, /// 0xa0c
			 0xf8389195, /// 0xa10
			 0x3109f4f6, /// 0xa14
			 0xf47b6b07, /// 0xa18
			 0xb11d2033, /// 0xa1c
			 0x921f9e87, /// 0xa20
			 0xcd136503, /// 0xa24
			 0x494c6cc6, /// 0xa28
			 0x5cba9c96, /// 0xa2c
			 0x980ebad2, /// 0xa30
			 0xdf6d7f3d, /// 0xa34
			 0x58d229cb, /// 0xa38
			 0x3759964d, /// 0xa3c
			 0x2c2e6802, /// 0xa40
			 0x2d3666c1, /// 0xa44
			 0xbd2d6e52, /// 0xa48
			 0x8c166f8e, /// 0xa4c
			 0x7d6b8f37, /// 0xa50
			 0x15f6bf5a, /// 0xa54
			 0x4d60f3ba, /// 0xa58
			 0x2c578ae0, /// 0xa5c
			 0x24281ebb, /// 0xa60
			 0x12c0dc7e, /// 0xa64
			 0x460d84b3, /// 0xa68
			 0x1ca2f2d8, /// 0xa6c
			 0x50de69af, /// 0xa70
			 0x7403ee44, /// 0xa74
			 0x7bfcf55a, /// 0xa78
			 0x49ee81eb, /// 0xa7c
			 0x96d58dbe, /// 0xa80
			 0x123844ae, /// 0xa84
			 0xdfb3c444, /// 0xa88
			 0xa41388f5, /// 0xa8c
			 0x74b489c8, /// 0xa90
			 0x4c9fb716, /// 0xa94
			 0x080b6d0c, /// 0xa98
			 0x960482e1, /// 0xa9c
			 0x358cc964, /// 0xaa0
			 0xf75ab689, /// 0xaa4
			 0x175d7300, /// 0xaa8
			 0xb1a787d5, /// 0xaac
			 0x75f9bf7c, /// 0xab0
			 0x7cc2c2f4, /// 0xab4
			 0xb24853ef, /// 0xab8
			 0xac28ec5b, /// 0xabc
			 0x2e76f419, /// 0xac0
			 0x1606ede1, /// 0xac4
			 0x757d174e, /// 0xac8
			 0x188bb423, /// 0xacc
			 0x3ef6c63a, /// 0xad0
			 0x20912591, /// 0xad4
			 0x4f4c61e4, /// 0xad8
			 0x2916527c, /// 0xadc
			 0x5d238043, /// 0xae0
			 0xb27cb3a4, /// 0xae4
			 0x8c2f186a, /// 0xae8
			 0x3eb3753f, /// 0xaec
			 0x666f85f2, /// 0xaf0
			 0x126c0373, /// 0xaf4
			 0x76b69e73, /// 0xaf8
			 0xbec6c95b, /// 0xafc
			 0xfff77285, /// 0xb00
			 0x2d4682e1, /// 0xb04
			 0xfe3e81c0, /// 0xb08
			 0x30073400, /// 0xb0c
			 0xc893cc57, /// 0xb10
			 0x28015a99, /// 0xb14
			 0x83aaf08a, /// 0xb18
			 0x97d934e1, /// 0xb1c
			 0x1f128da5, /// 0xb20
			 0x2e6695ff, /// 0xb24
			 0x5474aafd, /// 0xb28
			 0x4c2e757d, /// 0xb2c
			 0x60388e49, /// 0xb30
			 0xc8416c76, /// 0xb34
			 0xc578a133, /// 0xb38
			 0x0d8709bb, /// 0xb3c
			 0xcf9940fc, /// 0xb40
			 0x1f0276d7, /// 0xb44
			 0xbd7fe26d, /// 0xb48
			 0xf2adbc6a, /// 0xb4c
			 0x035d7314, /// 0xb50
			 0x349eb99c, /// 0xb54
			 0x0da7de5b, /// 0xb58
			 0x7cbf6654, /// 0xb5c
			 0xc08ba48e, /// 0xb60
			 0x1c737690, /// 0xb64
			 0x06240961, /// 0xb68
			 0xc020fafc, /// 0xb6c
			 0x15af9a74, /// 0xb70
			 0x165e0c3b, /// 0xb74
			 0x6a0c4488, /// 0xb78
			 0xefb9d046, /// 0xb7c
			 0x7b04af8d, /// 0xb80
			 0x046c2d4c, /// 0xb84
			 0xf872c84c, /// 0xb88
			 0xb1297231, /// 0xb8c
			 0x5b3eb30f, /// 0xb90
			 0xc582b608, /// 0xb94
			 0xf21d4536, /// 0xb98
			 0x8f77bd47, /// 0xb9c
			 0x5bd57f8f, /// 0xba0
			 0x025c8c4d, /// 0xba4
			 0x49c5ccd8, /// 0xba8
			 0x2a6ce578, /// 0xbac
			 0x6b88e608, /// 0xbb0
			 0x1fb8a1a6, /// 0xbb4
			 0x3ffeba4d, /// 0xbb8
			 0xd4281742, /// 0xbbc
			 0x47e8db9b, /// 0xbc0
			 0xa5cf3bda, /// 0xbc4
			 0xb684d630, /// 0xbc8
			 0x0442cc06, /// 0xbcc
			 0x39265666, /// 0xbd0
			 0x9e7ae862, /// 0xbd4
			 0xd23c1f24, /// 0xbd8
			 0xa913f836, /// 0xbdc
			 0x3fd586bd, /// 0xbe0
			 0xab0cde2b, /// 0xbe4
			 0x8ca1fa56, /// 0xbe8
			 0xd380abaf, /// 0xbec
			 0x1fc71b48, /// 0xbf0
			 0x163349db, /// 0xbf4
			 0x9fd1e0ca, /// 0xbf8
			 0x173d45dd, /// 0xbfc
			 0x20d992a0, /// 0xc00
			 0x6b41c16a, /// 0xc04
			 0xda3e2e4f, /// 0xc08
			 0x3e3ddac8, /// 0xc0c
			 0x6d85012b, /// 0xc10
			 0xb618180e, /// 0xc14
			 0x5d95c29f, /// 0xc18
			 0x79ea769f, /// 0xc1c
			 0x56428b52, /// 0xc20
			 0x89a48ae5, /// 0xc24
			 0x58d140c5, /// 0xc28
			 0xd4056a1d, /// 0xc2c
			 0x95f160f0, /// 0xc30
			 0x20b98186, /// 0xc34
			 0x871af3ac, /// 0xc38
			 0xa87ea02f, /// 0xc3c
			 0x8d5c74ff, /// 0xc40
			 0x6946c7e9, /// 0xc44
			 0x29c44268, /// 0xc48
			 0x8ca41d6a, /// 0xc4c
			 0x81bdbf22, /// 0xc50
			 0xa11e768c, /// 0xc54
			 0xaa82f98a, /// 0xc58
			 0xd37d79e7, /// 0xc5c
			 0x542cea54, /// 0xc60
			 0x3787ba92, /// 0xc64
			 0xb92e79a6, /// 0xc68
			 0x64d0bbe7, /// 0xc6c
			 0xcee0b843, /// 0xc70
			 0x85c6db5b, /// 0xc74
			 0x1a4da7c8, /// 0xc78
			 0x4cafb5d1, /// 0xc7c
			 0xe96a8cef, /// 0xc80
			 0x0674ae63, /// 0xc84
			 0x33cf95a2, /// 0xc88
			 0x58a6d689, /// 0xc8c
			 0x7c337851, /// 0xc90
			 0x20afa79f, /// 0xc94
			 0x2e25021b, /// 0xc98
			 0x6d1a601f, /// 0xc9c
			 0xb4aba333, /// 0xca0
			 0x152e0d40, /// 0xca4
			 0x4ad5acec, /// 0xca8
			 0x05472c63, /// 0xcac
			 0x4646619b, /// 0xcb0
			 0x7844161a, /// 0xcb4
			 0x63314296, /// 0xcb8
			 0x489ae54d, /// 0xcbc
			 0x8cb9ed59, /// 0xcc0
			 0x13adaa5e, /// 0xcc4
			 0x1b8bc2a8, /// 0xcc8
			 0x4b0f6205, /// 0xccc
			 0xeb27a4f2, /// 0xcd0
			 0xdaaf2ae4, /// 0xcd4
			 0x99762a81, /// 0xcd8
			 0x1e368548, /// 0xcdc
			 0xc31ccf8e, /// 0xce0
			 0x68083df0, /// 0xce4
			 0x0657cff7, /// 0xce8
			 0xcdbbb61d, /// 0xcec
			 0x7cf3dd26, /// 0xcf0
			 0x7e650fca, /// 0xcf4
			 0x2f8890b8, /// 0xcf8
			 0x98d14d36, /// 0xcfc
			 0x13d54b63, /// 0xd00
			 0x24d7a291, /// 0xd04
			 0x85825596, /// 0xd08
			 0x8c29fc00, /// 0xd0c
			 0xf9a1ac00, /// 0xd10
			 0x8e1e4372, /// 0xd14
			 0x2094edcb, /// 0xd18
			 0x99a2481e, /// 0xd1c
			 0xf0687f9d, /// 0xd20
			 0xa030c6b8, /// 0xd24
			 0x1da1c706, /// 0xd28
			 0xf942516a, /// 0xd2c
			 0x64698255, /// 0xd30
			 0x74717064, /// 0xd34
			 0xe5442911, /// 0xd38
			 0x01dfeb50, /// 0xd3c
			 0xe157d05b, /// 0xd40
			 0x97a329e1, /// 0xd44
			 0xbb116cbd, /// 0xd48
			 0x8432c417, /// 0xd4c
			 0x384c5573, /// 0xd50
			 0x18598a5b, /// 0xd54
			 0xe443330a, /// 0xd58
			 0x1e38c0a6, /// 0xd5c
			 0xd192e001, /// 0xd60
			 0x2a57b7bd, /// 0xd64
			 0xd4327144, /// 0xd68
			 0x4f9e39ed, /// 0xd6c
			 0x99552482, /// 0xd70
			 0x0eee86dc, /// 0xd74
			 0xef1661d2, /// 0xd78
			 0x19034bc0, /// 0xd7c
			 0x37174c83, /// 0xd80
			 0x1a52f716, /// 0xd84
			 0x4226a494, /// 0xd88
			 0xb62630c1, /// 0xd8c
			 0xf5177d74, /// 0xd90
			 0xfd773a2a, /// 0xd94
			 0x42fefd02, /// 0xd98
			 0x82c185bf, /// 0xd9c
			 0x3fb06ebd, /// 0xda0
			 0x626b0c40, /// 0xda4
			 0xc7f1c411, /// 0xda8
			 0x1aaff98d, /// 0xdac
			 0xf2f9162c, /// 0xdb0
			 0xc059ae2e, /// 0xdb4
			 0xc2838950, /// 0xdb8
			 0xd6804405, /// 0xdbc
			 0x2c2bcef7, /// 0xdc0
			 0x944e31a6, /// 0xdc4
			 0xba3599a2, /// 0xdc8
			 0x67b36e0c, /// 0xdcc
			 0x58132d1a, /// 0xdd0
			 0xf5f92ec9, /// 0xdd4
			 0x17dbaf37, /// 0xdd8
			 0xafdf90ef, /// 0xddc
			 0x43885b90, /// 0xde0
			 0x2f100e1b, /// 0xde4
			 0x401b913c, /// 0xde8
			 0x343e24af, /// 0xdec
			 0x44e930d2, /// 0xdf0
			 0xd8a57d3f, /// 0xdf4
			 0x259345de, /// 0xdf8
			 0xbf2eb91a, /// 0xdfc
			 0x0c500a9b, /// 0xe00
			 0xa5a976d5, /// 0xe04
			 0xf03d88c6, /// 0xe08
			 0xe3c85f28, /// 0xe0c
			 0x1dfe9868, /// 0xe10
			 0x444cea2d, /// 0xe14
			 0xb9a3dc30, /// 0xe18
			 0xd03f43ca, /// 0xe1c
			 0x69830ad5, /// 0xe20
			 0xaadf8a61, /// 0xe24
			 0xb33eb5ac, /// 0xe28
			 0x37484a16, /// 0xe2c
			 0xd105b6ed, /// 0xe30
			 0x6ee739aa, /// 0xe34
			 0xc21b8142, /// 0xe38
			 0x94f01b5d, /// 0xe3c
			 0xf457ebb2, /// 0xe40
			 0x6e6fadc1, /// 0xe44
			 0x1641378c, /// 0xe48
			 0x74fee854, /// 0xe4c
			 0x8b8ed345, /// 0xe50
			 0xf65ac819, /// 0xe54
			 0x7e2d438b, /// 0xe58
			 0x895b7f8b, /// 0xe5c
			 0xe2f30561, /// 0xe60
			 0x9aad79b4, /// 0xe64
			 0xed7d19e3, /// 0xe68
			 0x479bd3c6, /// 0xe6c
			 0x28576126, /// 0xe70
			 0x7cc9af1b, /// 0xe74
			 0x22481e5d, /// 0xe78
			 0xc776568e, /// 0xe7c
			 0x7ed575b5, /// 0xe80
			 0xa4e62319, /// 0xe84
			 0x63966c13, /// 0xe88
			 0xc241acec, /// 0xe8c
			 0x492ffb34, /// 0xe90
			 0xd0e4b6cd, /// 0xe94
			 0x35041135, /// 0xe98
			 0x7d82b707, /// 0xe9c
			 0x7aa2a5be, /// 0xea0
			 0x609a022b, /// 0xea4
			 0x5ebe5c3a, /// 0xea8
			 0x8b908745, /// 0xeac
			 0xb33c1343, /// 0xeb0
			 0xf62626b0, /// 0xeb4
			 0x48d0bb5f, /// 0xeb8
			 0xdfd0d329, /// 0xebc
			 0x9bc7c42b, /// 0xec0
			 0xd6a8612f, /// 0xec4
			 0x2ec9637e, /// 0xec8
			 0xf9a1bd76, /// 0xecc
			 0x1f456bd3, /// 0xed0
			 0x5855a4be, /// 0xed4
			 0x7dbad21a, /// 0xed8
			 0x97847b97, /// 0xedc
			 0xfcaca9a8, /// 0xee0
			 0xdd4fda3c, /// 0xee4
			 0xcd2d448d, /// 0xee8
			 0xa493e5a4, /// 0xeec
			 0x362584c3, /// 0xef0
			 0x3a603a77, /// 0xef4
			 0x4d30b876, /// 0xef8
			 0xa94c1582, /// 0xefc
			 0xb7fd53d4, /// 0xf00
			 0x062cfe5d, /// 0xf04
			 0x93e511fa, /// 0xf08
			 0x632af673, /// 0xf0c
			 0xbefc9c0e, /// 0xf10
			 0xaa7d6aba, /// 0xf14
			 0x67e79e06, /// 0xf18
			 0x0e193852, /// 0xf1c
			 0xbf488072, /// 0xf20
			 0x0e41075b, /// 0xf24
			 0x42eee5dd, /// 0xf28
			 0x7343831c, /// 0xf2c
			 0x2de78d5f, /// 0xf30
			 0xddeba78e, /// 0xf34
			 0xcecfa0e3, /// 0xf38
			 0x16490c41, /// 0xf3c
			 0x0fb22e7b, /// 0xf40
			 0x52f840b7, /// 0xf44
			 0x68d0eb62, /// 0xf48
			 0x1f65649c, /// 0xf4c
			 0x676b928a, /// 0xf50
			 0x4f87d06f, /// 0xf54
			 0x5b4210fe, /// 0xf58
			 0x6c2f80bf, /// 0xf5c
			 0xe00bc492, /// 0xf60
			 0xaf948df6, /// 0xf64
			 0x2a0d93b5, /// 0xf68
			 0xe94d5c4d, /// 0xf6c
			 0xa64f1253, /// 0xf70
			 0x24ccaca1, /// 0xf74
			 0x0350e971, /// 0xf78
			 0xdd42c60a, /// 0xf7c
			 0xeb79e98b, /// 0xf80
			 0xf9d082e6, /// 0xf84
			 0x29134ff2, /// 0xf88
			 0x6643c725, /// 0xf8c
			 0x8e8a8df3, /// 0xf90
			 0xa1eab5b6, /// 0xf94
			 0xa1080975, /// 0xf98
			 0x0ee2178a, /// 0xf9c
			 0xa4f46d15, /// 0xfa0
			 0xfaeed055, /// 0xfa4
			 0x0dc70e10, /// 0xfa8
			 0x3d3a0319, /// 0xfac
			 0x12939c33, /// 0xfb0
			 0x4358dc76, /// 0xfb4
			 0x0179ce9e, /// 0xfb8
			 0x2d7dd495, /// 0xfbc
			 0xce034a50, /// 0xfc0
			 0x3c30b857, /// 0xfc4
			 0xd8d67625, /// 0xfc8
			 0x154524c5, /// 0xfcc
			 0xb3e9cd62, /// 0xfd0
			 0x1fc07a83, /// 0xfd4
			 0xa5995416, /// 0xfd8
			 0xdf4b717e, /// 0xfdc
			 0x0d648cf3, /// 0xfe0
			 0xf8f13f74, /// 0xfe4
			 0x6c13b8e7, /// 0xfe8
			 0x820eb565, /// 0xfec
			 0xeb6938db, /// 0xff0
			 0x0958c1c7, /// 0xff4
			 0x131430d4, /// 0xff8
			 0x7456a8e1 /// last
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
			 0x000006ac,
			 0x000001ac,
			 0x00000768,
			 0x000003e4,
			 0x0000065c,
			 0x00000550,
			 0x00000354,
			 0x00000238,

			 /// vpu register f2
			 0x41f00000,
			 0x41f00000,
			 0x41600000,
			 0x41980000,
			 0x41000000,
			 0x41b00000,
			 0x40400000,
			 0x40400000,

			 /// vpu register f3
			 0x41d00000,
			 0x41980000,
			 0x41d80000,
			 0x41b80000,
			 0x40c00000,
			 0x41e80000,
			 0x40e00000,
			 0x41f00000,

			 /// vpu register f4
			 0x41a80000,
			 0x41b00000,
			 0x41600000,
			 0x41b00000,
			 0x41e00000,
			 0x41400000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f5
			 0x41b00000,
			 0x41b80000,
			 0x40000000,
			 0x41000000,
			 0x42000000,
			 0x41900000,
			 0x40800000,
			 0x41900000,

			 /// vpu register f6
			 0x3f800000,
			 0x41400000,
			 0x41f00000,
			 0x41f00000,
			 0x41a80000,
			 0x41900000,
			 0x41880000,
			 0x40400000,

			 /// vpu register f7
			 0x42000000,
			 0x41880000,
			 0x42000000,
			 0x41100000,
			 0x41700000,
			 0x41700000,
			 0x41200000,
			 0x41b80000,

			 /// vpu register f8
			 0x41b80000,
			 0x41900000,
			 0x41880000,
			 0x41400000,
			 0x41880000,
			 0x40000000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f9
			 0x41100000,
			 0x41100000,
			 0x41f80000,
			 0x41e00000,
			 0x41300000,
			 0x40400000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f10
			 0x41b00000,
			 0x41980000,
			 0x41700000,
			 0x41b80000,
			 0x41e00000,
			 0x41100000,
			 0x41100000,
			 0x41e00000,

			 /// vpu register f11
			 0x41f80000,
			 0x3f800000,
			 0x41d80000,
			 0x41900000,
			 0x41900000,
			 0x41500000,
			 0x40a00000,
			 0x41300000,

			 /// vpu register f12
			 0x41600000,
			 0x41a00000,
			 0x41100000,
			 0x42000000,
			 0x41600000,
			 0x41f80000,
			 0x41500000,
			 0x41a80000,

			 /// vpu register f13
			 0x3f800000,
			 0x40000000,
			 0x41e80000,
			 0x40a00000,
			 0x41900000,
			 0x41a80000,
			 0x40e00000,
			 0x3f800000,

			 /// vpu register f14
			 0x40800000,
			 0x41800000,
			 0x40400000,
			 0x41900000,
			 0x41a80000,
			 0x40000000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f15
			 0x41f80000,
			 0x41d00000,
			 0x41900000,
			 0x41e80000,
			 0x41500000,
			 0x41400000,
			 0x41100000,
			 0x41000000,

			 /// vpu register f16
			 0x40c00000,
			 0x40800000,
			 0x41880000,
			 0x41f00000,
			 0x41c80000,
			 0x41b00000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f17
			 0x41c80000,
			 0x41f00000,
			 0x40a00000,
			 0x41100000,
			 0x41700000,
			 0x41a80000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f18
			 0x41400000,
			 0x41a80000,
			 0x40c00000,
			 0x40a00000,
			 0x41f80000,
			 0x40e00000,
			 0x41b00000,
			 0x41e00000,

			 /// vpu register f19
			 0x41500000,
			 0x41000000,
			 0x41e80000,
			 0x42000000,
			 0x41400000,
			 0x41f00000,
			 0x41000000,
			 0x41d00000,

			 /// vpu register f20
			 0x41a80000,
			 0x41e00000,
			 0x41200000,
			 0x42000000,
			 0x42000000,
			 0x40c00000,
			 0x41b80000,
			 0x40c00000,

			 /// vpu register f21
			 0x41300000,
			 0x41b00000,
			 0x41e80000,
			 0x41a00000,
			 0x41d80000,
			 0x41c00000,
			 0x41300000,
			 0x41d80000,

			 /// vpu register f22
			 0x41a80000,
			 0x41b00000,
			 0x41e80000,
			 0x41980000,
			 0x40a00000,
			 0x40c00000,
			 0x41980000,
			 0x41f00000,

			 /// vpu register f23
			 0x41900000,
			 0x41c00000,
			 0x41e80000,
			 0x41c00000,
			 0x41d80000,
			 0x40000000,
			 0x41b80000,
			 0x41c80000,

			 /// vpu register f24
			 0x41500000,
			 0x41600000,
			 0x41300000,
			 0x41600000,
			 0x41700000,
			 0x40800000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f25
			 0x41b00000,
			 0x40800000,
			 0x41e00000,
			 0x41400000,
			 0x41f00000,
			 0x40800000,
			 0x41e80000,
			 0x41c80000,

			 /// vpu register f26
			 0x41900000,
			 0x40800000,
			 0x40800000,
			 0x41a00000,
			 0x41980000,
			 0x41880000,
			 0x41d00000,
			 0x41200000,

			 /// vpu register f27
			 0x41f00000,
			 0x41980000,
			 0x41980000,
			 0x41700000,
			 0x41e80000,
			 0x41c80000,
			 0x40a00000,
			 0x41600000,

			 /// vpu register f28
			 0x41980000,
			 0x41b00000,
			 0x40000000,
			 0x41000000,
			 0x40c00000,
			 0x41a00000,
			 0x41400000,
			 0x40000000,

			 /// vpu register f29
			 0x41980000,
			 0x41f00000,
			 0x41000000,
			 0x41880000,
			 0x41e80000,
			 0x40000000,
			 0x40e00000,
			 0x42000000,

			 /// vpu register f30
			 0x41a00000,
			 0x41600000,
			 0x41500000,
			 0x41b80000,
			 0x41600000,
			 0x40a00000,
			 0x40000000,
			 0x41f00000,

			 /// vpu register f31
			 0x41e00000,
			 0x41a00000,
			 0x41500000,
			 0x41d80000,
			 0x40800000,
			 0x41a00000,
			 0x41600000,
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
		"fg32h.ps f20, x15 (x15)\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f26, x12 (x12)\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f30, x13 (x11)\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f14, x11 (x13)\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f27, x14 (x12)\n"                           ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x13 (x13)\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f15, x12 (x15)\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f4, x15 (x15)\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f29, x11 (x13)\n"                           ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f24, x12 (x12)\n"                           ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f26, x13 (x13)\n"                           ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f23, x12 (x11)\n"                           ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f9, x12 (x13)\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f26, x13 (x11)\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f13, x13 (x14)\n"                           ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f15, x12 (x12)\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x11 (x13)\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f30, x14 (x14)\n"                           ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f11, x14 (x12)\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f15, x13 (x15)\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f28, x14 (x13)\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f12, x12 (x14)\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f17, x12 (x13)\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f16, x13 (x15)\n"                           ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f12, x13 (x11)\n"                           ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f25, x12 (x13)\n"                           ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f27, x11 (x14)\n"                           ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f10, x12 (x15)\n"                           ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f12, x14 (x11)\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x11 (x15)\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f27, x12 (x12)\n"                           ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f2, x11 (x13)\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f26, x11 (x12)\n"                           ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f25, x13 (x13)\n"                           ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f19, x12 (x11)\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f16, x12 (x12)\n"                           ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f6, x12 (x13)\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f4, x12 (x11)\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f19, x13 (x13)\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f29, x11 (x13)\n"                           ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f3, x14 (x15)\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f29, x13 (x14)\n"                           ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x12 (x12)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f3, x14 (x14)\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f14, x12 (x15)\n"                           ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f28, x12 (x14)\n"                           ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f13, x13 (x11)\n"                           ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f3, x15 (x13)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x15 (x12)\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f13, x13 (x13)\n"                           ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f23, x14 (x11)\n"                           ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f28, x12 (x11)\n"                           ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x13 (x15)\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f25, x12 (x12)\n"                           ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x12 (x14)\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f13, x13 (x11)\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x13 (x12)\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f28, x11 (x13)\n"                           ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f16, x15 (x12)\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x13 (x15)\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f27, x12 (x14)\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f17, x12 (x11)\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x12 (x15)\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x12 (x15)\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f13, x13 (x13)\n"                           ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f9, x12 (x14)\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f6, x15 (x11)\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f4, x15 (x15)\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f5, x14 (x15)\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f21, x15 (x14)\n"                           ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f20, x14 (x11)\n"                           ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f9, x14 (x14)\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f21, x15 (x11)\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f15, x14 (x12)\n"                           ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f15, x14 (x13)\n"                           ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f6, x11 (x14)\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f15, x14 (x14)\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f28, x15 (x13)\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x11 (x12)\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f17, x12 (x13)\n"                           ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f14, x14 (x12)\n"                           ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f27, x11 (x11)\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f3, x11 (x11)\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f12, x14 (x12)\n"                           ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f24, x15 (x14)\n"                           ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f28, x15 (x11)\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f23, x12 (x14)\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f12, x11 (x11)\n"                           ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f7, x13 (x15)\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f13, x11 (x14)\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f18, x15 (x11)\n"                           ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f25, x12 (x14)\n"                           ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f21, x12 (x12)\n"                           ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f26, x13 (x12)\n"                           ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f8, x13 (x12)\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f18, x15 (x15)\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f3, x12 (x11)\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f12, x14 (x13)\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32h.ps f18, x11 (x14)\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
