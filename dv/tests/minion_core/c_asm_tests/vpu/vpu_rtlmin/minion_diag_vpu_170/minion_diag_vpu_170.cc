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
			 0xa45446b6, /// 0x0
			 0x439e9f1e, /// 0x4
			 0x18110a06, /// 0x8
			 0x363c4279, /// 0xc
			 0xf8ecdbc0, /// 0x10
			 0x3681de24, /// 0x14
			 0x2710070d, /// 0x18
			 0xc8f4c056, /// 0x1c
			 0xb0abeed7, /// 0x20
			 0x305e68ad, /// 0x24
			 0x221fadd3, /// 0x28
			 0x9525b03c, /// 0x2c
			 0xe4dd948f, /// 0x30
			 0xb12b45af, /// 0x34
			 0x1a96c5ca, /// 0x38
			 0xdf4d9c59, /// 0x3c
			 0x3cfb5604, /// 0x40
			 0x94856ed1, /// 0x44
			 0xf64142a3, /// 0x48
			 0x3bc3965f, /// 0x4c
			 0x4cec4b05, /// 0x50
			 0x587e39fa, /// 0x54
			 0x219d5dd9, /// 0x58
			 0x5233a96f, /// 0x5c
			 0xb17eda4d, /// 0x60
			 0x0ad7fdef, /// 0x64
			 0x464bf8ac, /// 0x68
			 0x48811b3e, /// 0x6c
			 0xa52cefc7, /// 0x70
			 0xd1003984, /// 0x74
			 0x9911a95b, /// 0x78
			 0xc0338082, /// 0x7c
			 0x66c5ca99, /// 0x80
			 0x01762d65, /// 0x84
			 0xe07aeba0, /// 0x88
			 0xa4d19c05, /// 0x8c
			 0xbe373ce3, /// 0x90
			 0x639414cc, /// 0x94
			 0x8c9d6480, /// 0x98
			 0x82a14d1d, /// 0x9c
			 0xa5e1d70e, /// 0xa0
			 0x60f91935, /// 0xa4
			 0x828d6972, /// 0xa8
			 0x467b8591, /// 0xac
			 0xdf745017, /// 0xb0
			 0x702d01b4, /// 0xb4
			 0x97df7677, /// 0xb8
			 0x7f92ffd4, /// 0xbc
			 0x001361ca, /// 0xc0
			 0xb94d396d, /// 0xc4
			 0x9ca6bf96, /// 0xc8
			 0x389e04b9, /// 0xcc
			 0x31b90373, /// 0xd0
			 0xa26230c9, /// 0xd4
			 0xfca5b320, /// 0xd8
			 0x000bf26d, /// 0xdc
			 0x635a4732, /// 0xe0
			 0x7a8c33bb, /// 0xe4
			 0xb1281d79, /// 0xe8
			 0xa836f2d3, /// 0xec
			 0x6102b00b, /// 0xf0
			 0xca58a565, /// 0xf4
			 0x35c1412d, /// 0xf8
			 0x02393788, /// 0xfc
			 0x9c87085c, /// 0x100
			 0x51806f80, /// 0x104
			 0x8e59a1d5, /// 0x108
			 0xf05cbc0e, /// 0x10c
			 0x8d2848b3, /// 0x110
			 0xfff978e1, /// 0x114
			 0xdd82946d, /// 0x118
			 0xc663321b, /// 0x11c
			 0x0acf0c81, /// 0x120
			 0x4d5624ab, /// 0x124
			 0xed72ad17, /// 0x128
			 0x435d945a, /// 0x12c
			 0x89eaf819, /// 0x130
			 0xc2d216da, /// 0x134
			 0x351cd551, /// 0x138
			 0xe1f3950c, /// 0x13c
			 0x779f99f7, /// 0x140
			 0x6b1ee64f, /// 0x144
			 0xf9c0dfff, /// 0x148
			 0x1a3a9997, /// 0x14c
			 0x729bf6e8, /// 0x150
			 0x7e6a45b1, /// 0x154
			 0x7f17fca8, /// 0x158
			 0xf763eed9, /// 0x15c
			 0xfef1ff90, /// 0x160
			 0x93a60928, /// 0x164
			 0x65b32e96, /// 0x168
			 0xc830b041, /// 0x16c
			 0x264c10b8, /// 0x170
			 0x8a43afb3, /// 0x174
			 0x9f4245b1, /// 0x178
			 0x821d1812, /// 0x17c
			 0x691041fd, /// 0x180
			 0xcb60445b, /// 0x184
			 0xc01f372b, /// 0x188
			 0x74f1d06e, /// 0x18c
			 0xebed92f5, /// 0x190
			 0xb9cfc478, /// 0x194
			 0xb7af9c4d, /// 0x198
			 0x33d74883, /// 0x19c
			 0xcb8e5651, /// 0x1a0
			 0x866de737, /// 0x1a4
			 0x7324d3b7, /// 0x1a8
			 0x5eb99829, /// 0x1ac
			 0x389ff27e, /// 0x1b0
			 0xee700686, /// 0x1b4
			 0x2324d733, /// 0x1b8
			 0xa7520633, /// 0x1bc
			 0x0311b5b9, /// 0x1c0
			 0xe06a8001, /// 0x1c4
			 0xedeaf64b, /// 0x1c8
			 0x1c746084, /// 0x1cc
			 0xe6caadf0, /// 0x1d0
			 0x9c8582da, /// 0x1d4
			 0x31bf9d42, /// 0x1d8
			 0x6e58eea6, /// 0x1dc
			 0x81ece3f4, /// 0x1e0
			 0xbce580f0, /// 0x1e4
			 0x95c853fb, /// 0x1e8
			 0x835617e1, /// 0x1ec
			 0x014d22b9, /// 0x1f0
			 0x00ababa7, /// 0x1f4
			 0x40aaca2e, /// 0x1f8
			 0x3595b6d2, /// 0x1fc
			 0x0ae54149, /// 0x200
			 0x3acef12b, /// 0x204
			 0xb1065e7f, /// 0x208
			 0x2cee403f, /// 0x20c
			 0x405b7d69, /// 0x210
			 0x56f2f71d, /// 0x214
			 0xb5201d00, /// 0x218
			 0xbe5614f8, /// 0x21c
			 0x923f9e22, /// 0x220
			 0xdf495adf, /// 0x224
			 0x848ea548, /// 0x228
			 0xcf8d4f1c, /// 0x22c
			 0x84c0366d, /// 0x230
			 0x88f14298, /// 0x234
			 0x83ad62c9, /// 0x238
			 0x8e17d1af, /// 0x23c
			 0xa02b71db, /// 0x240
			 0x4e1863a9, /// 0x244
			 0x75adb585, /// 0x248
			 0xa2da1822, /// 0x24c
			 0x079b3c07, /// 0x250
			 0x38fd510f, /// 0x254
			 0xac90129c, /// 0x258
			 0xd5c84c74, /// 0x25c
			 0x079c76e6, /// 0x260
			 0x15c867a4, /// 0x264
			 0xf6d12542, /// 0x268
			 0x95759324, /// 0x26c
			 0x7c8fa916, /// 0x270
			 0x5d80e2a2, /// 0x274
			 0x30d81a00, /// 0x278
			 0xa8b31c3a, /// 0x27c
			 0xc4b5e71d, /// 0x280
			 0xccd1a79d, /// 0x284
			 0x5148177a, /// 0x288
			 0xa02bd565, /// 0x28c
			 0xf11c1995, /// 0x290
			 0x5720c2ce, /// 0x294
			 0x61ab2791, /// 0x298
			 0x0300d75f, /// 0x29c
			 0xc4cd33a7, /// 0x2a0
			 0xc7380bc1, /// 0x2a4
			 0xc1b62608, /// 0x2a8
			 0x4fb3583b, /// 0x2ac
			 0x574940e7, /// 0x2b0
			 0xe279af22, /// 0x2b4
			 0xdc0bcdc9, /// 0x2b8
			 0x710db763, /// 0x2bc
			 0x6fa5ca87, /// 0x2c0
			 0x89783807, /// 0x2c4
			 0x321784c4, /// 0x2c8
			 0xbfae16e7, /// 0x2cc
			 0xf3fbb405, /// 0x2d0
			 0x9b23c60a, /// 0x2d4
			 0x69126d27, /// 0x2d8
			 0x05406688, /// 0x2dc
			 0x8159060a, /// 0x2e0
			 0x75f76ab7, /// 0x2e4
			 0xc581998c, /// 0x2e8
			 0xedd3da8d, /// 0x2ec
			 0xf3c940f9, /// 0x2f0
			 0x1a9359f8, /// 0x2f4
			 0x6adbf93a, /// 0x2f8
			 0xcb913931, /// 0x2fc
			 0x66e05174, /// 0x300
			 0x7c2c4650, /// 0x304
			 0x32efa470, /// 0x308
			 0xd4ddb893, /// 0x30c
			 0xab76165c, /// 0x310
			 0x93e018ec, /// 0x314
			 0xe2a299b3, /// 0x318
			 0x07ffebd3, /// 0x31c
			 0x0e5755f5, /// 0x320
			 0x9bab63f7, /// 0x324
			 0x7682ddd0, /// 0x328
			 0x11a27f7c, /// 0x32c
			 0xe9c2ad65, /// 0x330
			 0xb1e72771, /// 0x334
			 0x957da35a, /// 0x338
			 0x285f745f, /// 0x33c
			 0x0b2ef19b, /// 0x340
			 0x0322adc3, /// 0x344
			 0x0ea32eb5, /// 0x348
			 0x8442dca3, /// 0x34c
			 0x12b3c512, /// 0x350
			 0x0351cf7e, /// 0x354
			 0x6d170fbd, /// 0x358
			 0xc1a32f71, /// 0x35c
			 0xfbca4b01, /// 0x360
			 0x33180a6b, /// 0x364
			 0x6c31961f, /// 0x368
			 0xc924ac44, /// 0x36c
			 0xea8d3288, /// 0x370
			 0x43d8fecd, /// 0x374
			 0x0de928ac, /// 0x378
			 0xa161021b, /// 0x37c
			 0x3a6cf44b, /// 0x380
			 0xb21df58f, /// 0x384
			 0x4e5f5508, /// 0x388
			 0xffd83585, /// 0x38c
			 0x71d5d0c8, /// 0x390
			 0xbf0cf54d, /// 0x394
			 0x4f383789, /// 0x398
			 0x672313e6, /// 0x39c
			 0xc70cd5c8, /// 0x3a0
			 0x2b854180, /// 0x3a4
			 0xded279de, /// 0x3a8
			 0x7d7ecae7, /// 0x3ac
			 0xb66bde2a, /// 0x3b0
			 0x0b42c3ec, /// 0x3b4
			 0xca22ac3f, /// 0x3b8
			 0x2a3afc22, /// 0x3bc
			 0x67eee93f, /// 0x3c0
			 0x51dcd79a, /// 0x3c4
			 0x6fdfa3bc, /// 0x3c8
			 0xd4f35f77, /// 0x3cc
			 0x5ed42689, /// 0x3d0
			 0x7d2e50dc, /// 0x3d4
			 0x03821189, /// 0x3d8
			 0xe970812d, /// 0x3dc
			 0x97fb1a28, /// 0x3e0
			 0x497387a8, /// 0x3e4
			 0x445e1d7b, /// 0x3e8
			 0xc9f4232d, /// 0x3ec
			 0xfe5ceda0, /// 0x3f0
			 0x7986a9b9, /// 0x3f4
			 0x5a772bc5, /// 0x3f8
			 0xd1928c1f, /// 0x3fc
			 0xcc3faa46, /// 0x400
			 0x1b7c4ef3, /// 0x404
			 0x33695dcc, /// 0x408
			 0x5f5e2fc1, /// 0x40c
			 0xf5134e84, /// 0x410
			 0xaa4a6094, /// 0x414
			 0xcd2e1451, /// 0x418
			 0xd7589699, /// 0x41c
			 0xc540e36a, /// 0x420
			 0x1621dff9, /// 0x424
			 0x0117abe8, /// 0x428
			 0x140f183d, /// 0x42c
			 0x16da54f1, /// 0x430
			 0x7f53ff57, /// 0x434
			 0x5b0af635, /// 0x438
			 0x76b1c109, /// 0x43c
			 0x30689ec8, /// 0x440
			 0x7f181999, /// 0x444
			 0xde5b2345, /// 0x448
			 0xb2d9b778, /// 0x44c
			 0x69fa208a, /// 0x450
			 0x0428a0eb, /// 0x454
			 0xde441ea0, /// 0x458
			 0x38587457, /// 0x45c
			 0x60142d5d, /// 0x460
			 0xe2fc844e, /// 0x464
			 0xdc1c10d9, /// 0x468
			 0x68f68f40, /// 0x46c
			 0xdbaa3294, /// 0x470
			 0x2e54e191, /// 0x474
			 0x90c0b925, /// 0x478
			 0x16407ed2, /// 0x47c
			 0x98175714, /// 0x480
			 0x43501584, /// 0x484
			 0x2b1d1e95, /// 0x488
			 0xe20d922d, /// 0x48c
			 0x2becb29c, /// 0x490
			 0xc7e503bc, /// 0x494
			 0xd86d038b, /// 0x498
			 0xc1577f8b, /// 0x49c
			 0xb2105aad, /// 0x4a0
			 0x834961e2, /// 0x4a4
			 0x7ffe7aa3, /// 0x4a8
			 0x896143ea, /// 0x4ac
			 0x706bd58a, /// 0x4b0
			 0xee9de45c, /// 0x4b4
			 0xd4a3f1b7, /// 0x4b8
			 0x45ab23fd, /// 0x4bc
			 0x33795133, /// 0x4c0
			 0x2753543d, /// 0x4c4
			 0x646c5f44, /// 0x4c8
			 0xf7bd3db8, /// 0x4cc
			 0xffd6c2d6, /// 0x4d0
			 0xafbf153e, /// 0x4d4
			 0x2445fa3a, /// 0x4d8
			 0x1a979a22, /// 0x4dc
			 0x5cb1f856, /// 0x4e0
			 0x68eeb7f1, /// 0x4e4
			 0x364c76b1, /// 0x4e8
			 0x0b0f8db2, /// 0x4ec
			 0xa040b424, /// 0x4f0
			 0x08f07865, /// 0x4f4
			 0xcf7594e0, /// 0x4f8
			 0x159d0ec9, /// 0x4fc
			 0x6e8b6f59, /// 0x500
			 0x19333360, /// 0x504
			 0x6abc4ebe, /// 0x508
			 0xa7531cbb, /// 0x50c
			 0x7b0a1711, /// 0x510
			 0xf4c9507a, /// 0x514
			 0x5f7990ef, /// 0x518
			 0xca18e684, /// 0x51c
			 0x587c7212, /// 0x520
			 0x9f637ecc, /// 0x524
			 0xeaf9b7af, /// 0x528
			 0x4b524320, /// 0x52c
			 0x8f82b0b0, /// 0x530
			 0x0d0ac961, /// 0x534
			 0x9a23596b, /// 0x538
			 0xda24b88a, /// 0x53c
			 0xf813c7e6, /// 0x540
			 0xfbfc9695, /// 0x544
			 0x118dbc2d, /// 0x548
			 0x3fdaa78e, /// 0x54c
			 0x39e7f1a6, /// 0x550
			 0xec069952, /// 0x554
			 0x6e8366ed, /// 0x558
			 0xbed98938, /// 0x55c
			 0x810e2f73, /// 0x560
			 0xfad707cf, /// 0x564
			 0xf2bfc2a0, /// 0x568
			 0xa608dd32, /// 0x56c
			 0x05bfd34f, /// 0x570
			 0x600e5d16, /// 0x574
			 0xc197ee95, /// 0x578
			 0xcd1973a4, /// 0x57c
			 0xa78736a9, /// 0x580
			 0xc284b7c9, /// 0x584
			 0xdbcaa7f9, /// 0x588
			 0x72a63abe, /// 0x58c
			 0xd506df4a, /// 0x590
			 0x8a8f619f, /// 0x594
			 0xe2120bec, /// 0x598
			 0x871e0dcc, /// 0x59c
			 0x2df009a1, /// 0x5a0
			 0x26516bf5, /// 0x5a4
			 0x03a5e3ba, /// 0x5a8
			 0x77c24aeb, /// 0x5ac
			 0x0ee5731b, /// 0x5b0
			 0xb422771f, /// 0x5b4
			 0xf553b335, /// 0x5b8
			 0x40d61993, /// 0x5bc
			 0x040a2837, /// 0x5c0
			 0x162bcfc6, /// 0x5c4
			 0xd8e00d1c, /// 0x5c8
			 0x0d38a4c6, /// 0x5cc
			 0x4588e303, /// 0x5d0
			 0x82e3cdf3, /// 0x5d4
			 0x7ae816e1, /// 0x5d8
			 0xe231bbfc, /// 0x5dc
			 0x710a6257, /// 0x5e0
			 0x70012886, /// 0x5e4
			 0x53dc5351, /// 0x5e8
			 0x9b28bd09, /// 0x5ec
			 0x1031db5e, /// 0x5f0
			 0xc55a0287, /// 0x5f4
			 0x2c808e0c, /// 0x5f8
			 0x442a6360, /// 0x5fc
			 0x78ff2859, /// 0x600
			 0xe873e0ac, /// 0x604
			 0x1a61500d, /// 0x608
			 0x57be20a8, /// 0x60c
			 0x1171b8b8, /// 0x610
			 0xd44cea4e, /// 0x614
			 0xf2471a07, /// 0x618
			 0x53037acc, /// 0x61c
			 0xc99654bd, /// 0x620
			 0xccb21d8e, /// 0x624
			 0x3fcf4d01, /// 0x628
			 0x99187c4d, /// 0x62c
			 0x31c54892, /// 0x630
			 0x1165cedf, /// 0x634
			 0xdb3a4622, /// 0x638
			 0x7c9684b2, /// 0x63c
			 0x47b52782, /// 0x640
			 0xbf9229eb, /// 0x644
			 0xbfa6c1df, /// 0x648
			 0x3b53e763, /// 0x64c
			 0x4026538e, /// 0x650
			 0x58ee24f5, /// 0x654
			 0xc897c7de, /// 0x658
			 0xbd332ce7, /// 0x65c
			 0xc887cd91, /// 0x660
			 0xd91a3c12, /// 0x664
			 0x09e4d03a, /// 0x668
			 0xb3b2e63d, /// 0x66c
			 0x44057a5e, /// 0x670
			 0xb1ad3e0a, /// 0x674
			 0x10ddd31b, /// 0x678
			 0x90b9bdd6, /// 0x67c
			 0x929cfcd0, /// 0x680
			 0x1afbc6cd, /// 0x684
			 0x79208a79, /// 0x688
			 0x34634e0e, /// 0x68c
			 0x03b1b5f2, /// 0x690
			 0xfcb02578, /// 0x694
			 0xd2a76578, /// 0x698
			 0x01342411, /// 0x69c
			 0x217a2e02, /// 0x6a0
			 0xa418940e, /// 0x6a4
			 0xdfeb073d, /// 0x6a8
			 0x4e88e318, /// 0x6ac
			 0xbe1d9888, /// 0x6b0
			 0xcea10163, /// 0x6b4
			 0xe052d977, /// 0x6b8
			 0xc56ed46e, /// 0x6bc
			 0xfa06282d, /// 0x6c0
			 0x16b8c7ec, /// 0x6c4
			 0x72cba1e6, /// 0x6c8
			 0xd2c3a63f, /// 0x6cc
			 0xa44be5c4, /// 0x6d0
			 0x490fb07d, /// 0x6d4
			 0x80bdf6c3, /// 0x6d8
			 0xfcf48a54, /// 0x6dc
			 0xeed582a2, /// 0x6e0
			 0x02e242ce, /// 0x6e4
			 0x2e6ed424, /// 0x6e8
			 0x027f6043, /// 0x6ec
			 0xb89c726b, /// 0x6f0
			 0x135991a5, /// 0x6f4
			 0xfe95cd5e, /// 0x6f8
			 0x4e0ffeab, /// 0x6fc
			 0x6496a8ce, /// 0x700
			 0x82127021, /// 0x704
			 0x97670e21, /// 0x708
			 0xb0e84348, /// 0x70c
			 0x83b2f5da, /// 0x710
			 0x701a07d1, /// 0x714
			 0x4aa6c215, /// 0x718
			 0xd13e640a, /// 0x71c
			 0x0549e407, /// 0x720
			 0xa1985561, /// 0x724
			 0x26e7c739, /// 0x728
			 0x2a990325, /// 0x72c
			 0xb0106c87, /// 0x730
			 0xf4ac2d46, /// 0x734
			 0x5cab1ad0, /// 0x738
			 0xd97ef079, /// 0x73c
			 0x03754728, /// 0x740
			 0xe5fce2a8, /// 0x744
			 0x6c284a9e, /// 0x748
			 0x0a833c0b, /// 0x74c
			 0x668d9dea, /// 0x750
			 0x105d767b, /// 0x754
			 0xc2e31074, /// 0x758
			 0x27a0dbaf, /// 0x75c
			 0x1704c4c1, /// 0x760
			 0x0b58fc35, /// 0x764
			 0x0464f0e1, /// 0x768
			 0xfc6afa2a, /// 0x76c
			 0x2e8a3d79, /// 0x770
			 0x9493d9ec, /// 0x774
			 0xa81c4963, /// 0x778
			 0x81873923, /// 0x77c
			 0x8bb35ed7, /// 0x780
			 0x7dc95b5b, /// 0x784
			 0x503855a4, /// 0x788
			 0xd7386dce, /// 0x78c
			 0x3948699c, /// 0x790
			 0x9e9b6953, /// 0x794
			 0xe6ca0511, /// 0x798
			 0xf2705bba, /// 0x79c
			 0x42ddf15a, /// 0x7a0
			 0x98746262, /// 0x7a4
			 0x7b5b3c44, /// 0x7a8
			 0x180a7451, /// 0x7ac
			 0x6f47218d, /// 0x7b0
			 0xb8ca3e1a, /// 0x7b4
			 0xf278c8d9, /// 0x7b8
			 0x4ba983e5, /// 0x7bc
			 0x6ee44acb, /// 0x7c0
			 0x005c76ee, /// 0x7c4
			 0x031b1c12, /// 0x7c8
			 0x3ed5454a, /// 0x7cc
			 0x82405441, /// 0x7d0
			 0x6c393d55, /// 0x7d4
			 0x9f6be0e1, /// 0x7d8
			 0x2ff4ea84, /// 0x7dc
			 0x38527748, /// 0x7e0
			 0x39b41d43, /// 0x7e4
			 0xb532f500, /// 0x7e8
			 0x5c5528be, /// 0x7ec
			 0x6ed69da7, /// 0x7f0
			 0x1b4a280e, /// 0x7f4
			 0x9bb71c52, /// 0x7f8
			 0x545200fa, /// 0x7fc
			 0xed54cafb, /// 0x800
			 0xcae07f74, /// 0x804
			 0xd6e5a98e, /// 0x808
			 0x5eb18738, /// 0x80c
			 0x66971cfb, /// 0x810
			 0x98c479c4, /// 0x814
			 0x985a474e, /// 0x818
			 0xd353e7fe, /// 0x81c
			 0xf76efaaf, /// 0x820
			 0xa95b7f16, /// 0x824
			 0xee024d25, /// 0x828
			 0xd09b2048, /// 0x82c
			 0x21c06f29, /// 0x830
			 0x5bc8417d, /// 0x834
			 0x5db628d3, /// 0x838
			 0xc27b6982, /// 0x83c
			 0x2ac46643, /// 0x840
			 0xe286fa02, /// 0x844
			 0x86206769, /// 0x848
			 0x0234168c, /// 0x84c
			 0x384e2329, /// 0x850
			 0x4d646651, /// 0x854
			 0xdda7b89b, /// 0x858
			 0x1c85fcd0, /// 0x85c
			 0x046f6526, /// 0x860
			 0x1c4cc091, /// 0x864
			 0xed14477f, /// 0x868
			 0x78338869, /// 0x86c
			 0x1bf2247e, /// 0x870
			 0x351fbcff, /// 0x874
			 0xefa87bcf, /// 0x878
			 0xd5fb07ae, /// 0x87c
			 0x3a66e201, /// 0x880
			 0x632099b7, /// 0x884
			 0xd0a4a6ee, /// 0x888
			 0xb624d166, /// 0x88c
			 0x1bfbea82, /// 0x890
			 0xb3a70855, /// 0x894
			 0x6464c5cb, /// 0x898
			 0x5e0d1f48, /// 0x89c
			 0x16fd2e02, /// 0x8a0
			 0x1a1cb0b1, /// 0x8a4
			 0x4a74db5d, /// 0x8a8
			 0x2294819b, /// 0x8ac
			 0x6788d8ee, /// 0x8b0
			 0xf39a5a43, /// 0x8b4
			 0xac0245ed, /// 0x8b8
			 0x14bce8b9, /// 0x8bc
			 0x43a9f6d2, /// 0x8c0
			 0x41faef5a, /// 0x8c4
			 0x7111f069, /// 0x8c8
			 0xb08bda78, /// 0x8cc
			 0xb7914ede, /// 0x8d0
			 0xe32ab10c, /// 0x8d4
			 0x697997ab, /// 0x8d8
			 0xd106a272, /// 0x8dc
			 0x83facb6e, /// 0x8e0
			 0x47c06a14, /// 0x8e4
			 0xe012eeed, /// 0x8e8
			 0xd92a2fd4, /// 0x8ec
			 0xcaf61735, /// 0x8f0
			 0x840561ad, /// 0x8f4
			 0xa7a1c5ca, /// 0x8f8
			 0xa07cdf07, /// 0x8fc
			 0xae6d5676, /// 0x900
			 0x04d1a0cc, /// 0x904
			 0x83623ae9, /// 0x908
			 0x05b55b1a, /// 0x90c
			 0x68dcb961, /// 0x910
			 0xe2b34936, /// 0x914
			 0xfdbdb191, /// 0x918
			 0x87ba712b, /// 0x91c
			 0x5dd2c7bd, /// 0x920
			 0x0eea8655, /// 0x924
			 0x0127d262, /// 0x928
			 0xd696396b, /// 0x92c
			 0x10adf3a4, /// 0x930
			 0x4e4bf14a, /// 0x934
			 0x3a270f91, /// 0x938
			 0x2b68c0e7, /// 0x93c
			 0x05a14aa9, /// 0x940
			 0xaedbc41e, /// 0x944
			 0x08cfcd6a, /// 0x948
			 0x54d044eb, /// 0x94c
			 0x375ebc6f, /// 0x950
			 0x2d2480b6, /// 0x954
			 0x1c187022, /// 0x958
			 0x25ee8ca1, /// 0x95c
			 0xc115c390, /// 0x960
			 0xba457593, /// 0x964
			 0xbdedbc79, /// 0x968
			 0xd0831145, /// 0x96c
			 0x2d9ff070, /// 0x970
			 0x05d78e0f, /// 0x974
			 0xc56d4bcb, /// 0x978
			 0xc9436fc8, /// 0x97c
			 0x36096f6e, /// 0x980
			 0x7f2a8461, /// 0x984
			 0xd7929649, /// 0x988
			 0xc21255de, /// 0x98c
			 0x2b563066, /// 0x990
			 0xee7ef09e, /// 0x994
			 0x21116c97, /// 0x998
			 0xf8ddea96, /// 0x99c
			 0xf757f3e3, /// 0x9a0
			 0x2429caea, /// 0x9a4
			 0x026d47ec, /// 0x9a8
			 0x91ded6ae, /// 0x9ac
			 0x242fb23b, /// 0x9b0
			 0xd7f35fa4, /// 0x9b4
			 0xbdc2660c, /// 0x9b8
			 0x65fbf810, /// 0x9bc
			 0xeebdf49f, /// 0x9c0
			 0xe3d5a84f, /// 0x9c4
			 0x6398a8db, /// 0x9c8
			 0xcacebf9d, /// 0x9cc
			 0x28c4e57e, /// 0x9d0
			 0x33d4dc8d, /// 0x9d4
			 0x470a2158, /// 0x9d8
			 0x7f875e92, /// 0x9dc
			 0xce9fb234, /// 0x9e0
			 0x71c0c815, /// 0x9e4
			 0xcdb0f494, /// 0x9e8
			 0x99de3575, /// 0x9ec
			 0x519fb666, /// 0x9f0
			 0x021e8bfa, /// 0x9f4
			 0x1a3774cb, /// 0x9f8
			 0xf9c4765b, /// 0x9fc
			 0x41deb9ff, /// 0xa00
			 0x752dff52, /// 0xa04
			 0x1386e2a4, /// 0xa08
			 0x511edcb2, /// 0xa0c
			 0x55d3beed, /// 0xa10
			 0x8210f22b, /// 0xa14
			 0x576a61a6, /// 0xa18
			 0xe0ca6802, /// 0xa1c
			 0x991e05ef, /// 0xa20
			 0xc15e57d3, /// 0xa24
			 0xd0984a50, /// 0xa28
			 0x0cb7dac1, /// 0xa2c
			 0xa42fbc96, /// 0xa30
			 0xa4ce4b49, /// 0xa34
			 0x05da9089, /// 0xa38
			 0xebb31846, /// 0xa3c
			 0xa9d13517, /// 0xa40
			 0x9e45652d, /// 0xa44
			 0x482d6438, /// 0xa48
			 0xcac72323, /// 0xa4c
			 0x20d4cbaa, /// 0xa50
			 0xb171d40f, /// 0xa54
			 0x844624bc, /// 0xa58
			 0x1ed787ce, /// 0xa5c
			 0xd306a696, /// 0xa60
			 0x8f9dc909, /// 0xa64
			 0x65bf2213, /// 0xa68
			 0x5f0a92fb, /// 0xa6c
			 0x2c3b3589, /// 0xa70
			 0x7a1ea7c1, /// 0xa74
			 0x5ebdd391, /// 0xa78
			 0x1e8b02d0, /// 0xa7c
			 0x6c204f6b, /// 0xa80
			 0xa959a9d3, /// 0xa84
			 0x07096b9a, /// 0xa88
			 0x1dbc9062, /// 0xa8c
			 0xf6887246, /// 0xa90
			 0xddd1a54e, /// 0xa94
			 0xa3703de3, /// 0xa98
			 0x9dfd0307, /// 0xa9c
			 0xc5fa3b37, /// 0xaa0
			 0x745e392d, /// 0xaa4
			 0xf15d6d94, /// 0xaa8
			 0xca142001, /// 0xaac
			 0x7818b857, /// 0xab0
			 0xdbcede10, /// 0xab4
			 0x351a1c17, /// 0xab8
			 0x6155ebb8, /// 0xabc
			 0x4a2a68d4, /// 0xac0
			 0xbef6ae04, /// 0xac4
			 0x665487f5, /// 0xac8
			 0xd5fb6955, /// 0xacc
			 0x5f89877f, /// 0xad0
			 0x8323838b, /// 0xad4
			 0x827d25dd, /// 0xad8
			 0xe54aaf79, /// 0xadc
			 0x25d4a2f6, /// 0xae0
			 0xe4f1a22f, /// 0xae4
			 0x045d07cb, /// 0xae8
			 0xa0fc49fe, /// 0xaec
			 0xc6886a7c, /// 0xaf0
			 0x1cb9f7f4, /// 0xaf4
			 0x0e8c729e, /// 0xaf8
			 0x8a2e8f13, /// 0xafc
			 0x08edd585, /// 0xb00
			 0x6c3635f9, /// 0xb04
			 0x47fde980, /// 0xb08
			 0xde57645f, /// 0xb0c
			 0xaca78056, /// 0xb10
			 0x59d35e7f, /// 0xb14
			 0x6ed0d91e, /// 0xb18
			 0x908905a5, /// 0xb1c
			 0x6a58d325, /// 0xb20
			 0xdef0c8dd, /// 0xb24
			 0x483df3bc, /// 0xb28
			 0x2ffb182b, /// 0xb2c
			 0xe5bb031f, /// 0xb30
			 0x609a5987, /// 0xb34
			 0x651aa772, /// 0xb38
			 0xb6d6ebac, /// 0xb3c
			 0x153fc161, /// 0xb40
			 0x9fc464a1, /// 0xb44
			 0x5d1bf13c, /// 0xb48
			 0x0d54d560, /// 0xb4c
			 0xb5ca1bcf, /// 0xb50
			 0x329fd05e, /// 0xb54
			 0xea466bbb, /// 0xb58
			 0x29610d57, /// 0xb5c
			 0x802cd307, /// 0xb60
			 0x26f57455, /// 0xb64
			 0x1f10379d, /// 0xb68
			 0x0f4adba7, /// 0xb6c
			 0x5de4efbd, /// 0xb70
			 0xe808fa91, /// 0xb74
			 0x696df5ec, /// 0xb78
			 0x45d5704f, /// 0xb7c
			 0x980df905, /// 0xb80
			 0x9239607d, /// 0xb84
			 0xfb62e3fe, /// 0xb88
			 0x3a70383e, /// 0xb8c
			 0xe06df2a0, /// 0xb90
			 0x0eb5570f, /// 0xb94
			 0x2b88a82a, /// 0xb98
			 0x03a7f070, /// 0xb9c
			 0x4908b8d7, /// 0xba0
			 0x48ce8aaf, /// 0xba4
			 0x01ed4001, /// 0xba8
			 0x98d1e167, /// 0xbac
			 0x11180a4b, /// 0xbb0
			 0x9827dfcd, /// 0xbb4
			 0x6787dff2, /// 0xbb8
			 0x622e7974, /// 0xbbc
			 0x1e0f9403, /// 0xbc0
			 0xdd0ec070, /// 0xbc4
			 0x7d005275, /// 0xbc8
			 0x84eca4ec, /// 0xbcc
			 0xa131e5bb, /// 0xbd0
			 0x87f5d462, /// 0xbd4
			 0xce970016, /// 0xbd8
			 0x33d5eacc, /// 0xbdc
			 0x51d2abaa, /// 0xbe0
			 0x1d31dd8d, /// 0xbe4
			 0x34035aa0, /// 0xbe8
			 0x5ab3a45f, /// 0xbec
			 0x830942bf, /// 0xbf0
			 0xa17c10cc, /// 0xbf4
			 0x652ed139, /// 0xbf8
			 0x6a795d60, /// 0xbfc
			 0xacbdab4f, /// 0xc00
			 0x12124554, /// 0xc04
			 0xcbde9155, /// 0xc08
			 0xe4c19ad8, /// 0xc0c
			 0xf2ad319b, /// 0xc10
			 0x242782df, /// 0xc14
			 0x906d55aa, /// 0xc18
			 0x957c21f1, /// 0xc1c
			 0xe82dc7f5, /// 0xc20
			 0xcd8d1316, /// 0xc24
			 0xdc919158, /// 0xc28
			 0x315e2817, /// 0xc2c
			 0x508f2384, /// 0xc30
			 0x826cc427, /// 0xc34
			 0x9a1da513, /// 0xc38
			 0xd2ec3e2a, /// 0xc3c
			 0x947f4d83, /// 0xc40
			 0x6fc12b40, /// 0xc44
			 0xd2fcc33e, /// 0xc48
			 0x0c50864f, /// 0xc4c
			 0xe114bc96, /// 0xc50
			 0xcf9674e8, /// 0xc54
			 0x65c66ae2, /// 0xc58
			 0x8a49d3dd, /// 0xc5c
			 0x5a8b0485, /// 0xc60
			 0xd6e1aa4e, /// 0xc64
			 0xc433d2c1, /// 0xc68
			 0xbb0da8aa, /// 0xc6c
			 0x0ae9a3ad, /// 0xc70
			 0x998fc529, /// 0xc74
			 0x4691de29, /// 0xc78
			 0x47b7e66e, /// 0xc7c
			 0x28516875, /// 0xc80
			 0xa807c25c, /// 0xc84
			 0x7489bb0f, /// 0xc88
			 0xf430a1f8, /// 0xc8c
			 0xa4b1562e, /// 0xc90
			 0xc4121153, /// 0xc94
			 0x27b7cfe9, /// 0xc98
			 0xc8d36f1f, /// 0xc9c
			 0xe3497847, /// 0xca0
			 0x0489f7ee, /// 0xca4
			 0x1e6770d6, /// 0xca8
			 0x747edee2, /// 0xcac
			 0x8222b343, /// 0xcb0
			 0x632f1d8d, /// 0xcb4
			 0x07baaa20, /// 0xcb8
			 0x997cec05, /// 0xcbc
			 0x4a7be06e, /// 0xcc0
			 0x26b15949, /// 0xcc4
			 0x15b0b9be, /// 0xcc8
			 0x66b8709b, /// 0xccc
			 0xff8b98a8, /// 0xcd0
			 0x2aa7f391, /// 0xcd4
			 0x54555762, /// 0xcd8
			 0xfac2058b, /// 0xcdc
			 0x55546790, /// 0xce0
			 0xb1753774, /// 0xce4
			 0xccf4e41c, /// 0xce8
			 0xbac4b23b, /// 0xcec
			 0xeaf56fd0, /// 0xcf0
			 0x9c440584, /// 0xcf4
			 0x9cc204ad, /// 0xcf8
			 0x20c329fd, /// 0xcfc
			 0xf3d73adf, /// 0xd00
			 0xd172e0c7, /// 0xd04
			 0xb78e35b5, /// 0xd08
			 0x08cfdbe5, /// 0xd0c
			 0x10cba6fa, /// 0xd10
			 0xa446df9e, /// 0xd14
			 0x78c6479f, /// 0xd18
			 0xd12e7204, /// 0xd1c
			 0x561e54d4, /// 0xd20
			 0xab53f709, /// 0xd24
			 0x5a4edbc9, /// 0xd28
			 0x078d77a3, /// 0xd2c
			 0xc87d9b8b, /// 0xd30
			 0x31d17654, /// 0xd34
			 0x97c9b0d8, /// 0xd38
			 0xa3898d26, /// 0xd3c
			 0xc0ac7d86, /// 0xd40
			 0x78f4219b, /// 0xd44
			 0xa9cf2055, /// 0xd48
			 0xbd2ba0c8, /// 0xd4c
			 0xaf00650d, /// 0xd50
			 0x03ef757d, /// 0xd54
			 0x7440e83b, /// 0xd58
			 0x29e47008, /// 0xd5c
			 0x9ce96968, /// 0xd60
			 0x2ec8e7d8, /// 0xd64
			 0x507f5289, /// 0xd68
			 0x1a69e392, /// 0xd6c
			 0x2e0f3dc8, /// 0xd70
			 0xd8464266, /// 0xd74
			 0x49329ab0, /// 0xd78
			 0x8aac72a1, /// 0xd7c
			 0xdb17cd73, /// 0xd80
			 0xd2f61002, /// 0xd84
			 0x6143791b, /// 0xd88
			 0x3f48ccb0, /// 0xd8c
			 0x6448e26f, /// 0xd90
			 0x7c517018, /// 0xd94
			 0x9e203936, /// 0xd98
			 0x5eaa9eb5, /// 0xd9c
			 0x35989b7b, /// 0xda0
			 0xc41177bd, /// 0xda4
			 0xb8e0d7c2, /// 0xda8
			 0xa874c43c, /// 0xdac
			 0xe3626170, /// 0xdb0
			 0xec3d0d64, /// 0xdb4
			 0x3746ddf7, /// 0xdb8
			 0x3605f79a, /// 0xdbc
			 0xa3164745, /// 0xdc0
			 0x1ea1c346, /// 0xdc4
			 0x03b68172, /// 0xdc8
			 0x803b23b5, /// 0xdcc
			 0x2331aa91, /// 0xdd0
			 0xbfa9bd8d, /// 0xdd4
			 0x22910724, /// 0xdd8
			 0x12392e4f, /// 0xddc
			 0xa65c9363, /// 0xde0
			 0xd97d8999, /// 0xde4
			 0xe7f2ee4a, /// 0xde8
			 0x89e3a62a, /// 0xdec
			 0x60500aab, /// 0xdf0
			 0x0a4e5120, /// 0xdf4
			 0xd3b1baef, /// 0xdf8
			 0xe4983705, /// 0xdfc
			 0x99a24a73, /// 0xe00
			 0xb36a18a8, /// 0xe04
			 0xf32b32b0, /// 0xe08
			 0xe523ef93, /// 0xe0c
			 0xf000012a, /// 0xe10
			 0x2616cac7, /// 0xe14
			 0x065f6a4c, /// 0xe18
			 0xf7230b69, /// 0xe1c
			 0x24f7ad03, /// 0xe20
			 0x5a2a7910, /// 0xe24
			 0x9a90584c, /// 0xe28
			 0x6a797c54, /// 0xe2c
			 0xe352c00a, /// 0xe30
			 0xfc9476d6, /// 0xe34
			 0xce0014c6, /// 0xe38
			 0x37f4e625, /// 0xe3c
			 0x58f60f7b, /// 0xe40
			 0xc94b5fd7, /// 0xe44
			 0xe6c68825, /// 0xe48
			 0x53e05939, /// 0xe4c
			 0x381c7cd1, /// 0xe50
			 0xf85ce7ff, /// 0xe54
			 0xa2aef048, /// 0xe58
			 0xbd9260a6, /// 0xe5c
			 0x4be82633, /// 0xe60
			 0x7eadd9fd, /// 0xe64
			 0x57e44cc7, /// 0xe68
			 0xfeecdb00, /// 0xe6c
			 0x151e529a, /// 0xe70
			 0xc35e1387, /// 0xe74
			 0x0c8ca72d, /// 0xe78
			 0xd2400cce, /// 0xe7c
			 0x906fd748, /// 0xe80
			 0x7f4a9685, /// 0xe84
			 0xfa5f7465, /// 0xe88
			 0xd11a6ad8, /// 0xe8c
			 0x50de1829, /// 0xe90
			 0x97bcc68b, /// 0xe94
			 0x6819e3ee, /// 0xe98
			 0x62d4472f, /// 0xe9c
			 0xb6464775, /// 0xea0
			 0xe879e971, /// 0xea4
			 0xca99be07, /// 0xea8
			 0x74374c97, /// 0xeac
			 0xdc0be41d, /// 0xeb0
			 0x0731d87a, /// 0xeb4
			 0x01972aaf, /// 0xeb8
			 0x548b6a61, /// 0xebc
			 0x267d26af, /// 0xec0
			 0xaa698ca0, /// 0xec4
			 0x610ead11, /// 0xec8
			 0xeead352e, /// 0xecc
			 0x631b9751, /// 0xed0
			 0x9431f688, /// 0xed4
			 0x442bb399, /// 0xed8
			 0x5f463369, /// 0xedc
			 0xe3ac178b, /// 0xee0
			 0x0548e316, /// 0xee4
			 0xf58df37f, /// 0xee8
			 0xccc9a66f, /// 0xeec
			 0x1b7d813d, /// 0xef0
			 0x51850cad, /// 0xef4
			 0x1450664e, /// 0xef8
			 0xdc045726, /// 0xefc
			 0xfb1ede09, /// 0xf00
			 0x8676dc2a, /// 0xf04
			 0x56c0d04e, /// 0xf08
			 0x3eb9d784, /// 0xf0c
			 0x89745e5a, /// 0xf10
			 0x635614ab, /// 0xf14
			 0xfc0ade8a, /// 0xf18
			 0x1a9fd25d, /// 0xf1c
			 0xdc43a23a, /// 0xf20
			 0xf5446dd7, /// 0xf24
			 0x0a7c8a0a, /// 0xf28
			 0x57693102, /// 0xf2c
			 0xac9e0033, /// 0xf30
			 0xee5b26d4, /// 0xf34
			 0xc190d5dc, /// 0xf38
			 0x5b86b489, /// 0xf3c
			 0x547bf9ea, /// 0xf40
			 0x3c7218fd, /// 0xf44
			 0x29383e34, /// 0xf48
			 0x7e44ffb2, /// 0xf4c
			 0xaf8b5a00, /// 0xf50
			 0x720938b7, /// 0xf54
			 0xb074b368, /// 0xf58
			 0x84bf5795, /// 0xf5c
			 0xccbdee53, /// 0xf60
			 0x84d9ea14, /// 0xf64
			 0xdad02313, /// 0xf68
			 0x6c574f80, /// 0xf6c
			 0x5727886b, /// 0xf70
			 0x0665a5be, /// 0xf74
			 0x6671edf1, /// 0xf78
			 0x34810267, /// 0xf7c
			 0x650c0e8d, /// 0xf80
			 0xbff369cb, /// 0xf84
			 0x0228aca2, /// 0xf88
			 0x7e6eae59, /// 0xf8c
			 0xc2bf5d88, /// 0xf90
			 0x0d74189a, /// 0xf94
			 0x73e333ba, /// 0xf98
			 0xed85e273, /// 0xf9c
			 0x3d37b8b9, /// 0xfa0
			 0x0e251a2f, /// 0xfa4
			 0x88359e33, /// 0xfa8
			 0xfd004258, /// 0xfac
			 0x5f85f63a, /// 0xfb0
			 0xaea6c04c, /// 0xfb4
			 0xc56d4c00, /// 0xfb8
			 0x7bf89278, /// 0xfbc
			 0xed4ee3f7, /// 0xfc0
			 0xc5cff25d, /// 0xfc4
			 0xbca29c9c, /// 0xfc8
			 0x93a18905, /// 0xfcc
			 0x32408473, /// 0xfd0
			 0x8710b0cc, /// 0xfd4
			 0x9949364c, /// 0xfd8
			 0x4f4eac63, /// 0xfdc
			 0x476e5d98, /// 0xfe0
			 0xa8303eb4, /// 0xfe4
			 0xb5b6c16b, /// 0xfe8
			 0xdde2b83b, /// 0xfec
			 0x530b59fa, /// 0xff0
			 0x3505a4b2, /// 0xff4
			 0xbcaecfa9, /// 0xff8
			 0x44ac7c03 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00000
			 0x4b000000,                                                  // 8388608.0                                   /// 00004
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00018
			 0xffc00001,                                                  // -signaling NaN                              /// 0001c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00020
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00034
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00038
			 0x0c600000,                                                  // Leading 1s:                                 /// 0003c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00040
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0004c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00050
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00054
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00058
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0005c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00060
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00064
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00070
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00078
			 0x80000000,                                                  // -zero                                       /// 0007c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0008c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00094
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0xffc00001,                                                  // -signaling NaN                              /// 0009c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 000e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00108
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00110
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00114
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0c780000,                                                  // Leading 1s:                                 /// 0011c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00124
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00128
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0012c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00134
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00138
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0013c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00144
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0014c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00150
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00158
			 0x0c600000,                                                  // Leading 1s:                                 /// 0015c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00160
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0016c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00170
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00178
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0017c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00188
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0018c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00190
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00194
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00198
			 0xcb000000,                                                  // -8388608.0                                  /// 0019c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 001b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00200
			 0x0e000003,                                                  // Trailing 1s:                                /// 00204
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00218
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x33333333,                                                  // 4 random values                             /// 00224
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00228
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0022c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00230
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c700000,                                                  // Leading 1s:                                 /// 0023c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00240
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00248
			 0x0c780000,                                                  // Leading 1s:                                 /// 0024c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00250
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00254
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00258
			 0xbf028f5c,                                                  // -0.51                                       /// 0025c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00260
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00264
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00268
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00270
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00274
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00278
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0027c
			 0xbf028f5c,                                                  // -0.51                                       /// 00280
			 0xffc00001,                                                  // -signaling NaN                              /// 00284
			 0x00011111,                                                  // 9.7958E-41                                  /// 00288
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00290
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00298
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002dc
			 0xff800000,                                                  // -inf                                        /// 002e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x55555555,                                                  // 4 random values                             /// 002f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00308
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0030c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00310
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00314
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00330
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e000003,                                                  // Trailing 1s:                                /// 00338
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0033c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00340
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00344
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00348
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00350
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00354
			 0x0c600000,                                                  // Leading 1s:                                 /// 00358
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0035c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xcb000000,                                                  // -8388608.0                                  /// 00368
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00378
			 0xff800000,                                                  // -inf                                        /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00388
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0038c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00394
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x33333333,                                                  // 4 random values                             /// 003ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00400
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00408
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0040c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00410
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00414
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00420
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00424
			 0x0c780000,                                                  // Leading 1s:                                 /// 00428
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0042c
			 0xff800000,                                                  // -inf                                        /// 00430
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00434
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0044c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0045c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00460
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00464
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x80011111,                                                  // -9.7958E-41                                 /// 0046c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00470
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00474
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00478
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00488
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0049c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00000000,                                                  // zero                                        /// 004e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00500
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00504
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00508
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00510
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00518
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0051c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00528
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0052c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00530
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00534
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00538
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00544
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0054c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00554
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00558
			 0x4b000000,                                                  // 8388608.0                                   /// 0055c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00560
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00564
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00574
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0057c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00580
			 0x7f800000,                                                  // inf                                         /// 00584
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00588
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0058c
			 0xcb000000,                                                  // -8388608.0                                  /// 00590
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00598
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0059c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005ec
			 0x33333333,                                                  // 4 random values                             /// 005f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00600
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00604
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0060c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00610
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00614
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0061c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00620
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00624
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00630
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00634
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00638
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0063c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00640
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00650
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00658
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00668
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c700000,                                                  // Leading 1s:                                 /// 00674
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00678
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0067c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00680
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00684
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00688
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0068c
			 0x3f028f5c,                                                  // 0.51                                        /// 00690
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0069c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a0
			 0x55555555,                                                  // 4 random values                             /// 006a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006ac
			 0x3f028f5c,                                                  // 0.51                                        /// 006b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006bc
			 0x7f800000,                                                  // inf                                         /// 006c0
			 0x7f800000,                                                  // inf                                         /// 006c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00700
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00704
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00708
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00714
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00718
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00720
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00724
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00728
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0072c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00730
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0073c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00740
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00748
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00750
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x00000000,                                                  // zero                                        /// 00764
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00768
			 0x33333333,                                                  // 4 random values                             /// 0076c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00770
			 0x0e000003,                                                  // Trailing 1s:                                /// 00774
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00778
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0077c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00780
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00788
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0078c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00790
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00794
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00798
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007c8
			 0x7fc00001,                                                  // signaling NaN                               /// 007cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007e0
			 0xcb000000,                                                  // -8388608.0                                  /// 007e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0080c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00814
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00818
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0081c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00830
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00838
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0083c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0c780000,                                                  // Leading 1s:                                 /// 00844
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00848
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0084c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00850
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0085c
			 0x33333333,                                                  // 4 random values                             /// 00860
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00864
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0xff800000,                                                  // -inf                                        /// 0086c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00870
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00878
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0087c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00880
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00890
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0089c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00900
			 0x0c780000,                                                  // Leading 1s:                                 /// 00904
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00908
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00910
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00914
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0091c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00920
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00924
			 0x33333333,                                                  // 4 random values                             /// 00928
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0092c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00938
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0093c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00940
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00950
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00954
			 0xbf028f5c,                                                  // -0.51                                       /// 00958
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0095c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00964
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00968
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0096c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00970
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00974
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00978
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00980
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00984
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0e000007,                                                  // Trailing 1s:                                /// 0098c
			 0x00000000,                                                  // zero                                        /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a0
			 0x7fc00001,                                                  // signaling NaN                               /// 009a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d0
			 0xffc00001,                                                  // -signaling NaN                              /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e0
			 0x33333333,                                                  // 4 random values                             /// 009e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x55555555,                                                  // 4 random values                             /// 00a38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a68
			 0x80000000,                                                  // -zero                                       /// 00a6c
			 0x7f800000,                                                  // inf                                         /// 00a70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a9c
			 0x55555555,                                                  // 4 random values                             /// 00aa0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ab0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00abc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ac8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00acc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ad0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00adc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ae8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00af0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00af8
			 0xcb000000,                                                  // -8388608.0                                  /// 00afc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b00
			 0x80000000,                                                  // -zero                                       /// 00b04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0xffc00001,                                                  // -signaling NaN                              /// 00b0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b30
			 0x33333333,                                                  // 4 random values                             /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b40
			 0x3f028f5c,                                                  // 0.51                                        /// 00b44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b50
			 0x00000000,                                                  // zero                                        /// 00b54
			 0x55555555,                                                  // 4 random values                             /// 00b58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b5c
			 0x7f800000,                                                  // inf                                         /// 00b60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x7fc00001,                                                  // signaling NaN                               /// 00b7c
			 0x7f800000,                                                  // inf                                         /// 00b80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ba0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c00
			 0x00000000,                                                  // zero                                        /// 00c04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xbf028f5c,                                                  // -0.51                                       /// 00c0c
			 0x80000000,                                                  // -zero                                       /// 00c10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x33333333,                                                  // 4 random values                             /// 00c58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ccc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cdc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ce0
			 0x33333333,                                                  // 4 random values                             /// 00ce4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d04
			 0xffc00001,                                                  // -signaling NaN                              /// 00d08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d84
			 0x80000000,                                                  // -zero                                       /// 00d88
			 0x3f028f5c,                                                  // 0.51                                        /// 00d8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00da0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00da8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00db8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc4
			 0x33333333,                                                  // 4 random values                             /// 00dc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ddc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x4b000000,                                                  // 8388608.0                                   /// 00de4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00de8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00df0
			 0x4b000000,                                                  // 8388608.0                                   /// 00df4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dfc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb4
			 0xff800000,                                                  // -inf                                        /// 00eb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ebc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ec4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00edc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x80000000,                                                  // -zero                                       /// 00ee8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ef0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ef4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00efc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x55555555,                                                  // 4 random values                             /// 00f0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f30
			 0x3f028f5c,                                                  // 0.51                                        /// 00f34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f7c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fa8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fe0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ff4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff8
			 0x80000040                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00000
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00004
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00008
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0000c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00010
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00014
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00018
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00020
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00024
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0002c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00030
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00034
			 0x3f028f5c,                                                  // 0.51                                        /// 00038
			 0x0e000001,                                                  // Trailing 1s:                                /// 0003c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00044
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0004c
			 0x3f028f5c,                                                  // 0.51                                        /// 00050
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00060
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0006c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00070
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00074
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00078
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0007c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00080
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00088
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0008c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0009c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000bc
			 0x7fc00001,                                                  // signaling NaN                               /// 000c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f0
			 0x33333333,                                                  // 4 random values                             /// 000f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00104
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0010c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00110
			 0x00011111,                                                  // 9.7958E-41                                  /// 00114
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00118
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0011c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00120
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00124
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00128
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0012c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00130
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00134
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00138
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0013c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x7f800000,                                                  // inf                                         /// 00148
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00150
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00158
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0015c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00164
			 0x0c780000,                                                  // Leading 1s:                                 /// 00168
			 0x0c400000,                                                  // Leading 1s:                                 /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x0c700000,                                                  // Leading 1s:                                 /// 00178
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0017c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00180
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00184
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00188
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0018c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00190
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00198
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a0
			 0xff800000,                                                  // -inf                                        /// 001a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a8
			 0xbf028f5c,                                                  // -0.51                                       /// 001ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b0
			 0x3f028f5c,                                                  // 0.51                                        /// 001b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001c4
			 0x7f800000,                                                  // inf                                         /// 001c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00200
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00204
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0020c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0e000001,                                                  // Trailing 1s:                                /// 00214
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00218
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0021c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00224
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00228
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0022c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00230
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00238
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0023c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00240
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00248
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00258
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00260
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0026c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00270
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00278
			 0x00000000,                                                  // zero                                        /// 0027c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00280
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00284
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00288
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0028c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00290
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00294
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00298
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002f0
			 0x00000000,                                                  // zero                                        /// 002f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00300
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0030c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00314
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0031c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00324
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00328
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00330
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00334
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00340
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00344
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0034c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00350
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00358
			 0x55555555,                                                  // 4 random values                             /// 0035c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00360
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00364
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00374
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00378
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0037c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00384
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00388
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0038c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00394
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00398
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0039c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d8
			 0x3f028f5c,                                                  // 0.51                                        /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0040c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00424
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00428
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0042c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00434
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00438
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0043c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00440
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00444
			 0x0e000007,                                                  // Trailing 1s:                                /// 00448
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0044c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00450
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00454
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00460
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00464
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00470
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00474
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00484
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00488
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0048c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x00000000,                                                  // zero                                        /// 00498
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00504
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00508
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0050c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0051c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00524
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x55555555,                                                  // 4 random values                             /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00534
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0053c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x33333333,                                                  // 4 random values                             /// 00544
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0054c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00550
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0055c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00564
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00568
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00570
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00574
			 0xbf028f5c,                                                  // -0.51                                       /// 00578
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0057c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00580
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00584
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00588
			 0x0e000003,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00590
			 0x33333333,                                                  // 4 random values                             /// 00594
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00598
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c0
			 0x80000000,                                                  // -zero                                       /// 005c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xff800000,                                                  // -inf                                        /// 005d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00604
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00608
			 0x33333333,                                                  // 4 random values                             /// 0060c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00610
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00614
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00618
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00620
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0062c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00630
			 0x7f800000,                                                  // inf                                         /// 00634
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00638
			 0x0c400000,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00640
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00648
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00650
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00654
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0065c
			 0xff800000,                                                  // -inf                                        /// 00660
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00664
			 0x0e000003,                                                  // Trailing 1s:                                /// 00668
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0066c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00674
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00688
			 0x0e000003,                                                  // Trailing 1s:                                /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00690
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00698
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0069c
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00700
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00704
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00708
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0070c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00710
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00720
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00730
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00738
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x00011111,                                                  // 9.7958E-41                                  /// 00744
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0074c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00750
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00754
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0075c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00760
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00764
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00770
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00774
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00778
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x3f028f5c,                                                  // 0.51                                        /// 00780
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00788
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00794
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00798
			 0x0c400000,                                                  // Leading 1s:                                 /// 0079c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a4
			 0xff800000,                                                  // -inf                                        /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x3f028f5c,                                                  // 0.51                                        /// 007e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00800
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00814
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00818
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00820
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00824
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00830
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00834
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00838
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0083c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00840
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0084c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00854
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00858
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00860
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00864
			 0x7f800000,                                                  // inf                                         /// 00868
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0086c
			 0x80000000,                                                  // -zero                                       /// 00870
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00874
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00878
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00880
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00884
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00894
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00898
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0089c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xff800000,                                                  // -inf                                        /// 008b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x3f028f5c,                                                  // 0.51                                        /// 008d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00900
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00904
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0c400000,                                                  // Leading 1s:                                 /// 0090c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00914
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0091c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00924
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00928
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0092c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00930
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00934
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00938
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0094c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00950
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00958
			 0x0c700000,                                                  // Leading 1s:                                 /// 0095c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00968
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x80000000,                                                  // -zero                                       /// 00970
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00974
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00978
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0097c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00980
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00984
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00988
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00990
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00994
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ec
			 0x55555555,                                                  // 4 random values                             /// 009f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00aa0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ab0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ac0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00acc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00af8
			 0x4b000000,                                                  // 8388608.0                                   /// 00afc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b64
			 0xcb000000,                                                  // -8388608.0                                  /// 00b68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b74
			 0xff800000,                                                  // -inf                                        /// 00b78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc4
			 0x00000000,                                                  // zero                                        /// 00bc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x7f800000,                                                  // inf                                         /// 00bd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00be0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bf0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c98
			 0x3f028f5c,                                                  // 0.51                                        /// 00c9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ca0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ca8
			 0x80000000,                                                  // -zero                                       /// 00cac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb0
			 0x7f800000,                                                  // inf                                         /// 00cb4
			 0x00000000,                                                  // zero                                        /// 00cb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x33333333,                                                  // 4 random values                             /// 00cdc
			 0x7f800000,                                                  // inf                                         /// 00ce0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ce8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d44
			 0x80000000,                                                  // -zero                                       /// 00d48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d78
			 0x80000000,                                                  // -zero                                       /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00da4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ddc
			 0x80000000,                                                  // -zero                                       /// 00de0
			 0x00000000,                                                  // zero                                        /// 00de4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00de8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00df0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x33333333,                                                  // 4 random values                             /// 00e00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e70
			 0x33333333,                                                  // 4 random values                             /// 00e74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ea0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ecc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ed0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eec
			 0x33333333,                                                  // 4 random values                             /// 00ef0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ef8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00efc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00000000,                                                  // zero                                        /// 00f44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fa0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00fd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fe4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fec
			 0x7f800000,                                                  // inf                                         /// 00ff0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff8
			 0xcb8c4b40                                                  // -18388608.0                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x6faa5bc4, /// 0x0
			 0x0c8880c3, /// 0x4
			 0x5af281b4, /// 0x8
			 0x19330117, /// 0xc
			 0xb85898b3, /// 0x10
			 0xf188a24a, /// 0x14
			 0xe36258b5, /// 0x18
			 0x439eb11a, /// 0x1c
			 0x46430f26, /// 0x20
			 0x0f71fed3, /// 0x24
			 0x1078ca07, /// 0x28
			 0x7069bf3a, /// 0x2c
			 0x8b997fd8, /// 0x30
			 0x75720085, /// 0x34
			 0xf05ad019, /// 0x38
			 0xd0fc65a7, /// 0x3c
			 0x084b5850, /// 0x40
			 0x2de0d47a, /// 0x44
			 0x0b738325, /// 0x48
			 0xa5ea2fa4, /// 0x4c
			 0x0c393fad, /// 0x50
			 0x5aef58b2, /// 0x54
			 0xf21b7822, /// 0x58
			 0xbaabbee4, /// 0x5c
			 0x6bebd425, /// 0x60
			 0xc794eb84, /// 0x64
			 0x5ed456de, /// 0x68
			 0x5ef3cada, /// 0x6c
			 0xd30373d3, /// 0x70
			 0xae9dd730, /// 0x74
			 0xcd97d496, /// 0x78
			 0xb12e9654, /// 0x7c
			 0xfd19ac0c, /// 0x80
			 0xf199cf5f, /// 0x84
			 0x830eaad4, /// 0x88
			 0xfe1a914b, /// 0x8c
			 0x94eab7fb, /// 0x90
			 0xcd761e3f, /// 0x94
			 0x246e203d, /// 0x98
			 0xfc6cc0ec, /// 0x9c
			 0xc515a5e0, /// 0xa0
			 0xac6b9a15, /// 0xa4
			 0x77ac9493, /// 0xa8
			 0x7b898710, /// 0xac
			 0x9f8a7376, /// 0xb0
			 0x5bdef1a6, /// 0xb4
			 0x68bc5589, /// 0xb8
			 0x71d8d97f, /// 0xbc
			 0xb12a2917, /// 0xc0
			 0xcfd667f5, /// 0xc4
			 0xa7f6ac16, /// 0xc8
			 0x0f5f94ef, /// 0xcc
			 0x2c5d10dd, /// 0xd0
			 0xd15d9add, /// 0xd4
			 0xdf1708f9, /// 0xd8
			 0xdffac558, /// 0xdc
			 0x8e6855dc, /// 0xe0
			 0x6fea3e34, /// 0xe4
			 0xd9ba36e3, /// 0xe8
			 0xce3b2545, /// 0xec
			 0xd9f64963, /// 0xf0
			 0xfa237269, /// 0xf4
			 0xea59eabd, /// 0xf8
			 0x22efc5c3, /// 0xfc
			 0x053957cc, /// 0x100
			 0x3b8647aa, /// 0x104
			 0x83a59180, /// 0x108
			 0xaecab702, /// 0x10c
			 0x9a9082a4, /// 0x110
			 0x9d2ae384, /// 0x114
			 0x0d127499, /// 0x118
			 0xea4352f4, /// 0x11c
			 0x5f057f92, /// 0x120
			 0x53e66a07, /// 0x124
			 0x7a201bc2, /// 0x128
			 0xb379ad50, /// 0x12c
			 0xc44b81c7, /// 0x130
			 0x97c4da7f, /// 0x134
			 0x73801efc, /// 0x138
			 0x97ef50a7, /// 0x13c
			 0x15d27614, /// 0x140
			 0x685f5937, /// 0x144
			 0xfd8c6dcf, /// 0x148
			 0xb5aa1b75, /// 0x14c
			 0x6a10168f, /// 0x150
			 0x4a3d7bd3, /// 0x154
			 0xbac83791, /// 0x158
			 0x21538020, /// 0x15c
			 0x0dee6c09, /// 0x160
			 0x861040eb, /// 0x164
			 0x43259c80, /// 0x168
			 0xb20f3092, /// 0x16c
			 0x2e1b5d3f, /// 0x170
			 0xfbb03b87, /// 0x174
			 0x2b248cea, /// 0x178
			 0x021b4ea2, /// 0x17c
			 0x1d112e7b, /// 0x180
			 0x2e6ef53f, /// 0x184
			 0x9f912b73, /// 0x188
			 0x1c990911, /// 0x18c
			 0x65234060, /// 0x190
			 0x2bdffb52, /// 0x194
			 0x04eace2f, /// 0x198
			 0xd74332d6, /// 0x19c
			 0x9c87b071, /// 0x1a0
			 0xe3c84f5e, /// 0x1a4
			 0xf2db77d5, /// 0x1a8
			 0x1de8d140, /// 0x1ac
			 0xed40db25, /// 0x1b0
			 0xcc88225e, /// 0x1b4
			 0x559bacb3, /// 0x1b8
			 0xd81e0ec6, /// 0x1bc
			 0x2bb11896, /// 0x1c0
			 0xd5d25d99, /// 0x1c4
			 0xc4413372, /// 0x1c8
			 0xcddbdc5a, /// 0x1cc
			 0x392cbf8b, /// 0x1d0
			 0x38a6ced7, /// 0x1d4
			 0x3f9e1d26, /// 0x1d8
			 0x9711fdc3, /// 0x1dc
			 0xdb1a6f90, /// 0x1e0
			 0x12327990, /// 0x1e4
			 0x22de9ed8, /// 0x1e8
			 0xa96d0ad4, /// 0x1ec
			 0x554b7f29, /// 0x1f0
			 0x166f3cec, /// 0x1f4
			 0x4964b2f1, /// 0x1f8
			 0x25d36bf1, /// 0x1fc
			 0x4260c88e, /// 0x200
			 0x8cc27be0, /// 0x204
			 0x21066cd6, /// 0x208
			 0x363a753b, /// 0x20c
			 0xd78ebc56, /// 0x210
			 0x1483912e, /// 0x214
			 0x3bd17077, /// 0x218
			 0xa547a485, /// 0x21c
			 0x7282f48f, /// 0x220
			 0xa90ab01b, /// 0x224
			 0x50f1a1f0, /// 0x228
			 0x9daa7874, /// 0x22c
			 0x76c218d5, /// 0x230
			 0x8b9baf81, /// 0x234
			 0xc0f8ee2c, /// 0x238
			 0x5149541e, /// 0x23c
			 0xcf2fcfca, /// 0x240
			 0xf0be1ef6, /// 0x244
			 0x38493284, /// 0x248
			 0xfdb7b547, /// 0x24c
			 0x8004becf, /// 0x250
			 0x0a6d7c89, /// 0x254
			 0xe0956642, /// 0x258
			 0x290079f6, /// 0x25c
			 0x28785d3a, /// 0x260
			 0xb19562a2, /// 0x264
			 0x13f919d0, /// 0x268
			 0xfb384e88, /// 0x26c
			 0x882c7e93, /// 0x270
			 0x62157be5, /// 0x274
			 0x8e667563, /// 0x278
			 0x943f8cce, /// 0x27c
			 0x0974a206, /// 0x280
			 0xf76c1466, /// 0x284
			 0x1bebd986, /// 0x288
			 0x7e0af41a, /// 0x28c
			 0x7f198968, /// 0x290
			 0x28b205f8, /// 0x294
			 0xa90271c1, /// 0x298
			 0x1b4b9d50, /// 0x29c
			 0x7ac537c6, /// 0x2a0
			 0x4779ae84, /// 0x2a4
			 0xc20cc372, /// 0x2a8
			 0x985219bb, /// 0x2ac
			 0x2dfd186c, /// 0x2b0
			 0xa6f569f1, /// 0x2b4
			 0xa4741dba, /// 0x2b8
			 0xebdf8a80, /// 0x2bc
			 0x958140bd, /// 0x2c0
			 0x6460c4d6, /// 0x2c4
			 0xe3276df1, /// 0x2c8
			 0x2d135250, /// 0x2cc
			 0x3b336c66, /// 0x2d0
			 0x4707e0b4, /// 0x2d4
			 0x6bbb3149, /// 0x2d8
			 0x3f8d427b, /// 0x2dc
			 0x2d885197, /// 0x2e0
			 0x55a36fbe, /// 0x2e4
			 0x7491429a, /// 0x2e8
			 0x98f4da22, /// 0x2ec
			 0x065f935e, /// 0x2f0
			 0x3b3f6520, /// 0x2f4
			 0x96f93d3e, /// 0x2f8
			 0x1b18506e, /// 0x2fc
			 0x79aae836, /// 0x300
			 0x5b4abc4b, /// 0x304
			 0x013d162a, /// 0x308
			 0xfa6f7ba5, /// 0x30c
			 0xb2ef393f, /// 0x310
			 0x0295e907, /// 0x314
			 0xb20db8a3, /// 0x318
			 0xf854cd0f, /// 0x31c
			 0xa265ddc7, /// 0x320
			 0x8870712c, /// 0x324
			 0x153babe1, /// 0x328
			 0x674510a5, /// 0x32c
			 0x645ada1a, /// 0x330
			 0x0cf43455, /// 0x334
			 0xdcbf46b8, /// 0x338
			 0x6b11f054, /// 0x33c
			 0x7f66c9b6, /// 0x340
			 0x7866444b, /// 0x344
			 0x9e9bbb89, /// 0x348
			 0x36836dbf, /// 0x34c
			 0xdc7cf710, /// 0x350
			 0xa03169fa, /// 0x354
			 0x81d542e9, /// 0x358
			 0x8b25da97, /// 0x35c
			 0x0a0d61a9, /// 0x360
			 0x56e96cb6, /// 0x364
			 0xb4eb0e94, /// 0x368
			 0x3428c0ea, /// 0x36c
			 0x5c00a897, /// 0x370
			 0x6277518b, /// 0x374
			 0x85b9dae7, /// 0x378
			 0xcbde481b, /// 0x37c
			 0x289a6849, /// 0x380
			 0x3f50493a, /// 0x384
			 0x27689551, /// 0x388
			 0x3bcd90da, /// 0x38c
			 0x40c9dae9, /// 0x390
			 0xaee06e3e, /// 0x394
			 0xad584c15, /// 0x398
			 0x3e2df478, /// 0x39c
			 0x0bbf6645, /// 0x3a0
			 0xca81c81a, /// 0x3a4
			 0x6b9c280f, /// 0x3a8
			 0xb19a58eb, /// 0x3ac
			 0x6251c86a, /// 0x3b0
			 0xfa01a21c, /// 0x3b4
			 0x8aec0d64, /// 0x3b8
			 0x85149017, /// 0x3bc
			 0x7153320d, /// 0x3c0
			 0x5356931c, /// 0x3c4
			 0x606d7e52, /// 0x3c8
			 0x30d7c3bf, /// 0x3cc
			 0x07f3b54a, /// 0x3d0
			 0xed0b9727, /// 0x3d4
			 0x69ffda19, /// 0x3d8
			 0xed4069c3, /// 0x3dc
			 0x22bc6520, /// 0x3e0
			 0xfd157bb9, /// 0x3e4
			 0x370ee392, /// 0x3e8
			 0x87ae7114, /// 0x3ec
			 0x4605c907, /// 0x3f0
			 0x835b5908, /// 0x3f4
			 0x93254137, /// 0x3f8
			 0x1b2e30c7, /// 0x3fc
			 0x5776de58, /// 0x400
			 0xc5a5b89f, /// 0x404
			 0x9fd50abe, /// 0x408
			 0xe5d0f7c1, /// 0x40c
			 0xed2adb8b, /// 0x410
			 0x25243a6f, /// 0x414
			 0x393813d2, /// 0x418
			 0x3750ba4c, /// 0x41c
			 0xa635491e, /// 0x420
			 0x4694d8ae, /// 0x424
			 0x8da52ab5, /// 0x428
			 0xa5630252, /// 0x42c
			 0xae0b6bb5, /// 0x430
			 0xaef0d620, /// 0x434
			 0x62ef9998, /// 0x438
			 0xa4cda4bf, /// 0x43c
			 0xc7e63d3a, /// 0x440
			 0x1d531e58, /// 0x444
			 0x6a293ae5, /// 0x448
			 0x682ba31d, /// 0x44c
			 0x77c65b6a, /// 0x450
			 0x0f9227b5, /// 0x454
			 0x0c00cfe2, /// 0x458
			 0x87979b2e, /// 0x45c
			 0x4f17dbb6, /// 0x460
			 0x94a788f3, /// 0x464
			 0xdf3e89f2, /// 0x468
			 0xa0b82bb8, /// 0x46c
			 0xf037fbcc, /// 0x470
			 0xd949a689, /// 0x474
			 0x68c4e441, /// 0x478
			 0x3f0ec9d4, /// 0x47c
			 0xbf743c3d, /// 0x480
			 0x093b67b2, /// 0x484
			 0xe95203b1, /// 0x488
			 0x25dd67cb, /// 0x48c
			 0xc0187665, /// 0x490
			 0xf396ce10, /// 0x494
			 0x71529e9b, /// 0x498
			 0x70afb328, /// 0x49c
			 0xdaa61813, /// 0x4a0
			 0x1d1cffc9, /// 0x4a4
			 0x1bcbfd16, /// 0x4a8
			 0x5e02b0cb, /// 0x4ac
			 0x8dc9644d, /// 0x4b0
			 0xfd3d66f0, /// 0x4b4
			 0x1ae1afde, /// 0x4b8
			 0xff890c7b, /// 0x4bc
			 0x5ac4115d, /// 0x4c0
			 0x9379b393, /// 0x4c4
			 0x5c8a2f78, /// 0x4c8
			 0xa4eb12c1, /// 0x4cc
			 0xd3a751b5, /// 0x4d0
			 0x745614ca, /// 0x4d4
			 0x9e4c471f, /// 0x4d8
			 0x16e0733c, /// 0x4dc
			 0x6dce0c15, /// 0x4e0
			 0x87492231, /// 0x4e4
			 0x3b547009, /// 0x4e8
			 0x229f0214, /// 0x4ec
			 0x2a3ee45d, /// 0x4f0
			 0x210ee71d, /// 0x4f4
			 0x60bd576b, /// 0x4f8
			 0x7e89ae28, /// 0x4fc
			 0xb94aee0a, /// 0x500
			 0x5e47b25f, /// 0x504
			 0xebc5f084, /// 0x508
			 0x38dd1240, /// 0x50c
			 0x843a49b5, /// 0x510
			 0x081f8781, /// 0x514
			 0x584351bb, /// 0x518
			 0x40628a0f, /// 0x51c
			 0x5c58483e, /// 0x520
			 0x422877ec, /// 0x524
			 0x512e8dcb, /// 0x528
			 0xf2f0d5f0, /// 0x52c
			 0xcdbf1ddd, /// 0x530
			 0x11aa1a67, /// 0x534
			 0x68320d2b, /// 0x538
			 0x6207e368, /// 0x53c
			 0xc56fa87a, /// 0x540
			 0xc9253dc7, /// 0x544
			 0xda21c35f, /// 0x548
			 0x9d7c711c, /// 0x54c
			 0x4fefd289, /// 0x550
			 0xb07c41e5, /// 0x554
			 0x1c49d4eb, /// 0x558
			 0xbad51bab, /// 0x55c
			 0x903fbf07, /// 0x560
			 0x6e8d1561, /// 0x564
			 0x87584b3e, /// 0x568
			 0xa5a6d066, /// 0x56c
			 0x58cca80a, /// 0x570
			 0x6728ba51, /// 0x574
			 0xa803ce39, /// 0x578
			 0x59f1fc83, /// 0x57c
			 0x6a400ad8, /// 0x580
			 0x325dbad2, /// 0x584
			 0x00f183f4, /// 0x588
			 0x08100467, /// 0x58c
			 0xe51b19e2, /// 0x590
			 0x9da600db, /// 0x594
			 0x82afa1da, /// 0x598
			 0xf99b50e3, /// 0x59c
			 0xe204c1dd, /// 0x5a0
			 0x69123379, /// 0x5a4
			 0x778b299b, /// 0x5a8
			 0x99502b90, /// 0x5ac
			 0x80727155, /// 0x5b0
			 0xf15529ed, /// 0x5b4
			 0x5aaf6f37, /// 0x5b8
			 0x053a22cd, /// 0x5bc
			 0xdc048fe2, /// 0x5c0
			 0x67c947c9, /// 0x5c4
			 0xe9d217ea, /// 0x5c8
			 0xed70f621, /// 0x5cc
			 0xab50cf3c, /// 0x5d0
			 0x381f4d70, /// 0x5d4
			 0xd9143279, /// 0x5d8
			 0xa51b241d, /// 0x5dc
			 0x1dc99a03, /// 0x5e0
			 0x45d488dc, /// 0x5e4
			 0x677271e2, /// 0x5e8
			 0xbbcb8bb5, /// 0x5ec
			 0xf060ac35, /// 0x5f0
			 0x77ac8e98, /// 0x5f4
			 0xd148212b, /// 0x5f8
			 0xc52ac912, /// 0x5fc
			 0xb01a1552, /// 0x600
			 0x6f960841, /// 0x604
			 0x2b0daa01, /// 0x608
			 0x82ace4f9, /// 0x60c
			 0x7bc59f5e, /// 0x610
			 0x5ec96536, /// 0x614
			 0xfe361bd3, /// 0x618
			 0x08120d51, /// 0x61c
			 0x0f3473bd, /// 0x620
			 0x8438f483, /// 0x624
			 0x73b89cdd, /// 0x628
			 0x65edab49, /// 0x62c
			 0x60c0333f, /// 0x630
			 0x7a226c64, /// 0x634
			 0xbea7b08e, /// 0x638
			 0x1e043629, /// 0x63c
			 0x163badae, /// 0x640
			 0x013d12b3, /// 0x644
			 0x1329b35e, /// 0x648
			 0x7a99b220, /// 0x64c
			 0x562ce066, /// 0x650
			 0x62d3a0ee, /// 0x654
			 0xfafc03bc, /// 0x658
			 0xef7bcc88, /// 0x65c
			 0xc6746a67, /// 0x660
			 0x8217033d, /// 0x664
			 0x9d1c7978, /// 0x668
			 0xec7312d8, /// 0x66c
			 0xe3cde549, /// 0x670
			 0x748bbfe1, /// 0x674
			 0x8b271caf, /// 0x678
			 0xff564fa8, /// 0x67c
			 0x30cba1de, /// 0x680
			 0x576d33de, /// 0x684
			 0xa67ba190, /// 0x688
			 0x6a8bef21, /// 0x68c
			 0x3a912efb, /// 0x690
			 0x8d192a78, /// 0x694
			 0xf978d8b5, /// 0x698
			 0x8a1edc8c, /// 0x69c
			 0xa8f94d5f, /// 0x6a0
			 0x6daea7c6, /// 0x6a4
			 0x68c69d07, /// 0x6a8
			 0xe8a40bcb, /// 0x6ac
			 0x8f19be6f, /// 0x6b0
			 0x70037605, /// 0x6b4
			 0x41552f41, /// 0x6b8
			 0x8fa9ea52, /// 0x6bc
			 0x78a39105, /// 0x6c0
			 0x41f849b2, /// 0x6c4
			 0xd50c8d14, /// 0x6c8
			 0xb27ccc84, /// 0x6cc
			 0xb1b0dde6, /// 0x6d0
			 0x074474ac, /// 0x6d4
			 0x98fbdeb0, /// 0x6d8
			 0x5cf41f70, /// 0x6dc
			 0xa0985c4a, /// 0x6e0
			 0xbc736c3d, /// 0x6e4
			 0xa0b1ab01, /// 0x6e8
			 0x6bbc2c67, /// 0x6ec
			 0x2de5c11c, /// 0x6f0
			 0x4145ba01, /// 0x6f4
			 0xfef2a63b, /// 0x6f8
			 0xa8351873, /// 0x6fc
			 0xd974f9c7, /// 0x700
			 0xfaec8451, /// 0x704
			 0xe6676db0, /// 0x708
			 0x15c0d5af, /// 0x70c
			 0x9765660e, /// 0x710
			 0xe5444627, /// 0x714
			 0xc3183f4c, /// 0x718
			 0x3935c683, /// 0x71c
			 0x620744e2, /// 0x720
			 0xae053e9d, /// 0x724
			 0x72a3fad9, /// 0x728
			 0x37792a1a, /// 0x72c
			 0x9fff0fb9, /// 0x730
			 0x8e9e3d1c, /// 0x734
			 0x305c8b12, /// 0x738
			 0x0dec745b, /// 0x73c
			 0xed92da7b, /// 0x740
			 0x4bc5ca28, /// 0x744
			 0x2c269046, /// 0x748
			 0x0348f37b, /// 0x74c
			 0x595cc9dd, /// 0x750
			 0x339cec71, /// 0x754
			 0xacf3a93b, /// 0x758
			 0xdaf9804b, /// 0x75c
			 0x120a1503, /// 0x760
			 0xac6eb73e, /// 0x764
			 0x112c40d5, /// 0x768
			 0x9bf8b6b1, /// 0x76c
			 0xfeafdeb9, /// 0x770
			 0x86b55fe1, /// 0x774
			 0xa33f2b9e, /// 0x778
			 0xee8dd5e1, /// 0x77c
			 0x3737813e, /// 0x780
			 0x599f724f, /// 0x784
			 0xbfe59c7b, /// 0x788
			 0x3caf1f2f, /// 0x78c
			 0x1165437b, /// 0x790
			 0x8c86e58e, /// 0x794
			 0xb810b20d, /// 0x798
			 0xe2743592, /// 0x79c
			 0xcdc25f5a, /// 0x7a0
			 0x77973a3a, /// 0x7a4
			 0x2b5e42da, /// 0x7a8
			 0x2f153b93, /// 0x7ac
			 0xeab70fc7, /// 0x7b0
			 0x99ea6b74, /// 0x7b4
			 0x1d2b2160, /// 0x7b8
			 0xdaf6ba9b, /// 0x7bc
			 0xadd3c15a, /// 0x7c0
			 0xc35df0a7, /// 0x7c4
			 0x703b5988, /// 0x7c8
			 0x1983b327, /// 0x7cc
			 0xff6f53ac, /// 0x7d0
			 0xd04b60f0, /// 0x7d4
			 0xd7e7d115, /// 0x7d8
			 0x409dcf31, /// 0x7dc
			 0xda116891, /// 0x7e0
			 0x0a766b1c, /// 0x7e4
			 0x8c74da40, /// 0x7e8
			 0xcaf46423, /// 0x7ec
			 0xa339d130, /// 0x7f0
			 0x3d66cb12, /// 0x7f4
			 0x72f579c1, /// 0x7f8
			 0x520d333f, /// 0x7fc
			 0x67d66575, /// 0x800
			 0xe14d1520, /// 0x804
			 0x54541f5f, /// 0x808
			 0xa45b821e, /// 0x80c
			 0xc54d7c38, /// 0x810
			 0x92d413cb, /// 0x814
			 0x9f456af9, /// 0x818
			 0x26946ae3, /// 0x81c
			 0x566972fe, /// 0x820
			 0xd7d63976, /// 0x824
			 0x15b3041f, /// 0x828
			 0x6bb5428e, /// 0x82c
			 0x51bb6edf, /// 0x830
			 0xb90a40bf, /// 0x834
			 0xe32a54b3, /// 0x838
			 0xa97fc5c6, /// 0x83c
			 0x4f54e073, /// 0x840
			 0x89441a3b, /// 0x844
			 0x67e475a4, /// 0x848
			 0x17342d61, /// 0x84c
			 0x2f0286c0, /// 0x850
			 0xec4b9980, /// 0x854
			 0x310db457, /// 0x858
			 0xc978290e, /// 0x85c
			 0xa5545659, /// 0x860
			 0xa7d856e5, /// 0x864
			 0xd96a844a, /// 0x868
			 0xf3d1689d, /// 0x86c
			 0x15b7fad5, /// 0x870
			 0x9a386842, /// 0x874
			 0x3bed7c7b, /// 0x878
			 0x197b9da9, /// 0x87c
			 0x7ff2665a, /// 0x880
			 0xd344cd0a, /// 0x884
			 0x046ab3f9, /// 0x888
			 0x0e70112b, /// 0x88c
			 0x34b62bbd, /// 0x890
			 0x79d7eae2, /// 0x894
			 0x4af78e59, /// 0x898
			 0xde23eec4, /// 0x89c
			 0x56829114, /// 0x8a0
			 0xc352eff7, /// 0x8a4
			 0xf81e90c2, /// 0x8a8
			 0xb53fd904, /// 0x8ac
			 0x1cd1da77, /// 0x8b0
			 0x8387a4f3, /// 0x8b4
			 0x17f74c49, /// 0x8b8
			 0xff6cdfe2, /// 0x8bc
			 0xbb5f270e, /// 0x8c0
			 0x5969fd1e, /// 0x8c4
			 0x8f638b8e, /// 0x8c8
			 0x63e66bf3, /// 0x8cc
			 0x48893fbd, /// 0x8d0
			 0x6170370a, /// 0x8d4
			 0xa4e264be, /// 0x8d8
			 0xc05663a7, /// 0x8dc
			 0xc72761b6, /// 0x8e0
			 0x7c68d6d8, /// 0x8e4
			 0x02dc8882, /// 0x8e8
			 0x75577864, /// 0x8ec
			 0x92e5e682, /// 0x8f0
			 0x12c8e8c0, /// 0x8f4
			 0xfade231b, /// 0x8f8
			 0x845c9b04, /// 0x8fc
			 0x928f0e52, /// 0x900
			 0x9a54277a, /// 0x904
			 0x29aa65c3, /// 0x908
			 0xf809b1cd, /// 0x90c
			 0x99d377b6, /// 0x910
			 0xa581c905, /// 0x914
			 0x688b09aa, /// 0x918
			 0x6e652783, /// 0x91c
			 0xf18ec9d2, /// 0x920
			 0x94300b92, /// 0x924
			 0x66f1c37e, /// 0x928
			 0x79595ef8, /// 0x92c
			 0x0946ec70, /// 0x930
			 0x83ea3cc2, /// 0x934
			 0xa719c378, /// 0x938
			 0xaa244d1a, /// 0x93c
			 0xdeaee6fe, /// 0x940
			 0x2a0839c6, /// 0x944
			 0x973b1b79, /// 0x948
			 0xfbbe82e0, /// 0x94c
			 0x804b577f, /// 0x950
			 0xa3bf1d6c, /// 0x954
			 0x044ca77c, /// 0x958
			 0xdc57e08a, /// 0x95c
			 0xfffd1888, /// 0x960
			 0xc2b2f598, /// 0x964
			 0xa266c20b, /// 0x968
			 0xdb533923, /// 0x96c
			 0x0a6cd863, /// 0x970
			 0x92e68767, /// 0x974
			 0x6f8c09d2, /// 0x978
			 0xf86556cd, /// 0x97c
			 0x3d09d98d, /// 0x980
			 0x0723b551, /// 0x984
			 0x6b0c5474, /// 0x988
			 0x6448e715, /// 0x98c
			 0x4e60a2a1, /// 0x990
			 0xf88a265a, /// 0x994
			 0xb9319f28, /// 0x998
			 0x23ad3f21, /// 0x99c
			 0xe3a5ad48, /// 0x9a0
			 0xb929e60f, /// 0x9a4
			 0x209bf752, /// 0x9a8
			 0x0b784a2b, /// 0x9ac
			 0x40d8c9ab, /// 0x9b0
			 0xe405ff4d, /// 0x9b4
			 0x6abdead7, /// 0x9b8
			 0xb4a8f2be, /// 0x9bc
			 0xcb1eae4d, /// 0x9c0
			 0xc8ff5d80, /// 0x9c4
			 0x4e65de1e, /// 0x9c8
			 0xe0cfa1d3, /// 0x9cc
			 0xe9106e43, /// 0x9d0
			 0xd5ca8488, /// 0x9d4
			 0xce16c174, /// 0x9d8
			 0x2ce49288, /// 0x9dc
			 0x7b630f50, /// 0x9e0
			 0x35057999, /// 0x9e4
			 0x4d83bd7c, /// 0x9e8
			 0x0bab4608, /// 0x9ec
			 0x6360ef3e, /// 0x9f0
			 0x8e681faf, /// 0x9f4
			 0x98342f10, /// 0x9f8
			 0x5f34e5ff, /// 0x9fc
			 0xcedd5df2, /// 0xa00
			 0x19995b97, /// 0xa04
			 0xe755a69a, /// 0xa08
			 0x63a6de97, /// 0xa0c
			 0xa0632388, /// 0xa10
			 0xbb73e7dd, /// 0xa14
			 0xa731c2cc, /// 0xa18
			 0x4d718033, /// 0xa1c
			 0x61e4aab5, /// 0xa20
			 0x74a81dd9, /// 0xa24
			 0x4f028a3a, /// 0xa28
			 0x825bb6d8, /// 0xa2c
			 0x26bfad9f, /// 0xa30
			 0x6db7c847, /// 0xa34
			 0x17310be2, /// 0xa38
			 0xa99c863a, /// 0xa3c
			 0x029f567a, /// 0xa40
			 0x80f2c4dc, /// 0xa44
			 0xb46b32fc, /// 0xa48
			 0xac2384e2, /// 0xa4c
			 0x79a7f8ec, /// 0xa50
			 0x612f59e4, /// 0xa54
			 0x77335d4d, /// 0xa58
			 0xe8e16238, /// 0xa5c
			 0x3cbf62d2, /// 0xa60
			 0xe15d7786, /// 0xa64
			 0x93043afb, /// 0xa68
			 0x84676021, /// 0xa6c
			 0x1bd1f009, /// 0xa70
			 0x74851470, /// 0xa74
			 0xb669ba39, /// 0xa78
			 0x69209cf8, /// 0xa7c
			 0xf3445858, /// 0xa80
			 0x71763891, /// 0xa84
			 0x6968b532, /// 0xa88
			 0x7d33fd0f, /// 0xa8c
			 0x2b072218, /// 0xa90
			 0x580dd0c1, /// 0xa94
			 0xb4b88a61, /// 0xa98
			 0xadfa5e11, /// 0xa9c
			 0x86f65aa6, /// 0xaa0
			 0x8f3b7b7c, /// 0xaa4
			 0x7f205f8c, /// 0xaa8
			 0x9db09fce, /// 0xaac
			 0x69d40e8b, /// 0xab0
			 0x93cc584b, /// 0xab4
			 0x4cb2eb83, /// 0xab8
			 0xcc9de7c5, /// 0xabc
			 0xd5897179, /// 0xac0
			 0x6f9975c7, /// 0xac4
			 0x9daf59e7, /// 0xac8
			 0x4042d41d, /// 0xacc
			 0xb506d606, /// 0xad0
			 0x44a39418, /// 0xad4
			 0x07e31300, /// 0xad8
			 0xdf10e7a9, /// 0xadc
			 0x0c39d458, /// 0xae0
			 0x9f13af2b, /// 0xae4
			 0xf3b55bfc, /// 0xae8
			 0x413582f3, /// 0xaec
			 0x796ff7ae, /// 0xaf0
			 0x8f7dcab6, /// 0xaf4
			 0x046523b0, /// 0xaf8
			 0x0913fd51, /// 0xafc
			 0x7d996d0d, /// 0xb00
			 0x5da7f6e6, /// 0xb04
			 0xdf7705f1, /// 0xb08
			 0x5f0ec209, /// 0xb0c
			 0x20dd1892, /// 0xb10
			 0xae4937f1, /// 0xb14
			 0x96e3f533, /// 0xb18
			 0xc38bbcc8, /// 0xb1c
			 0xa8766b65, /// 0xb20
			 0x9174267a, /// 0xb24
			 0x854543d0, /// 0xb28
			 0xa0dba9c3, /// 0xb2c
			 0x8d2fadc2, /// 0xb30
			 0x2b693933, /// 0xb34
			 0x35e117a1, /// 0xb38
			 0x19e1bbf1, /// 0xb3c
			 0xab35e225, /// 0xb40
			 0x7683d53e, /// 0xb44
			 0xe9d0eb0f, /// 0xb48
			 0x78515ed7, /// 0xb4c
			 0x6020f620, /// 0xb50
			 0xf736e00d, /// 0xb54
			 0x91f5e8a4, /// 0xb58
			 0x422a3b47, /// 0xb5c
			 0xb9800673, /// 0xb60
			 0xca08211d, /// 0xb64
			 0x597de24f, /// 0xb68
			 0xa24ee44b, /// 0xb6c
			 0x447d0a71, /// 0xb70
			 0x728ec3c1, /// 0xb74
			 0x3b85e574, /// 0xb78
			 0x5d728245, /// 0xb7c
			 0x19e0f076, /// 0xb80
			 0xfaf71b39, /// 0xb84
			 0x9047cc94, /// 0xb88
			 0x32f70153, /// 0xb8c
			 0x09ff2808, /// 0xb90
			 0xc28976a1, /// 0xb94
			 0x6051cffe, /// 0xb98
			 0xf436037b, /// 0xb9c
			 0x01945a3f, /// 0xba0
			 0x2e037666, /// 0xba4
			 0xd015ba60, /// 0xba8
			 0x9b792b67, /// 0xbac
			 0xd4889fe7, /// 0xbb0
			 0x98bb08ea, /// 0xbb4
			 0x9533788c, /// 0xbb8
			 0x08d7ea2d, /// 0xbbc
			 0x21432609, /// 0xbc0
			 0x8db0b036, /// 0xbc4
			 0x2c61a7cf, /// 0xbc8
			 0x9bd94e2e, /// 0xbcc
			 0xe9ba9397, /// 0xbd0
			 0xc7f3f02a, /// 0xbd4
			 0xd75eeda0, /// 0xbd8
			 0x08cba6c0, /// 0xbdc
			 0x51b959b2, /// 0xbe0
			 0xcab1c49d, /// 0xbe4
			 0xe093a503, /// 0xbe8
			 0x68f2140d, /// 0xbec
			 0x11518c92, /// 0xbf0
			 0x9f6af361, /// 0xbf4
			 0x7793c2e9, /// 0xbf8
			 0xa8b73bdb, /// 0xbfc
			 0x4b0194d5, /// 0xc00
			 0x69bc9e65, /// 0xc04
			 0xe2069e81, /// 0xc08
			 0x1b26d249, /// 0xc0c
			 0x1418de84, /// 0xc10
			 0x591665bc, /// 0xc14
			 0xf8139770, /// 0xc18
			 0x36416012, /// 0xc1c
			 0x6564f9b0, /// 0xc20
			 0x20586723, /// 0xc24
			 0x8be6b32b, /// 0xc28
			 0xc84b5b4d, /// 0xc2c
			 0xd43e626a, /// 0xc30
			 0xe7ecab4b, /// 0xc34
			 0xe0711e63, /// 0xc38
			 0xf4d81fa5, /// 0xc3c
			 0xc1ab452a, /// 0xc40
			 0x4982f37f, /// 0xc44
			 0x5d5cd5bf, /// 0xc48
			 0x4e6d55a0, /// 0xc4c
			 0x23340e8d, /// 0xc50
			 0xa7eb92ab, /// 0xc54
			 0x1bb4a1c5, /// 0xc58
			 0x4a174998, /// 0xc5c
			 0x162b61b4, /// 0xc60
			 0x6ec51b84, /// 0xc64
			 0xd55245ef, /// 0xc68
			 0xf147bb6c, /// 0xc6c
			 0xb5002a48, /// 0xc70
			 0x59b44224, /// 0xc74
			 0x8d9167d8, /// 0xc78
			 0x325d7aee, /// 0xc7c
			 0x4e98628a, /// 0xc80
			 0x10225648, /// 0xc84
			 0xd27e4372, /// 0xc88
			 0x39b5bb5e, /// 0xc8c
			 0xe13465ff, /// 0xc90
			 0x3395a658, /// 0xc94
			 0x102b2fa1, /// 0xc98
			 0x1d6203fe, /// 0xc9c
			 0x4629eb84, /// 0xca0
			 0x8ce2208b, /// 0xca4
			 0x4e8961d5, /// 0xca8
			 0x6af63b54, /// 0xcac
			 0xedd7d829, /// 0xcb0
			 0x337cad91, /// 0xcb4
			 0xd7ca217d, /// 0xcb8
			 0xf5e70945, /// 0xcbc
			 0xba041548, /// 0xcc0
			 0x93c285c0, /// 0xcc4
			 0x7d6e39a1, /// 0xcc8
			 0x610fbfa8, /// 0xccc
			 0x36e805b7, /// 0xcd0
			 0x79a25da2, /// 0xcd4
			 0x8f36ac21, /// 0xcd8
			 0xc7c0729d, /// 0xcdc
			 0x62eb80e0, /// 0xce0
			 0x531ee584, /// 0xce4
			 0xcbf3f594, /// 0xce8
			 0xf6b3afd5, /// 0xcec
			 0xa879eb73, /// 0xcf0
			 0xa0fcd111, /// 0xcf4
			 0xb2487aa5, /// 0xcf8
			 0x673c1485, /// 0xcfc
			 0x916c8ce2, /// 0xd00
			 0x03ff4946, /// 0xd04
			 0xd0a46b5f, /// 0xd08
			 0x1a9714d1, /// 0xd0c
			 0x90305c64, /// 0xd10
			 0x4cd19b81, /// 0xd14
			 0xce5ca91d, /// 0xd18
			 0x3c29d50b, /// 0xd1c
			 0x480edb66, /// 0xd20
			 0xd35c9999, /// 0xd24
			 0x04e6d621, /// 0xd28
			 0xcb622a16, /// 0xd2c
			 0x4fe33cb7, /// 0xd30
			 0x50624d07, /// 0xd34
			 0x52857ba4, /// 0xd38
			 0x50eda216, /// 0xd3c
			 0x2f19e6e3, /// 0xd40
			 0xcd220077, /// 0xd44
			 0x09614817, /// 0xd48
			 0x876ab7c5, /// 0xd4c
			 0xe8c64315, /// 0xd50
			 0x6219a6c2, /// 0xd54
			 0x2b2180c7, /// 0xd58
			 0x22d32640, /// 0xd5c
			 0x8cd0a6f8, /// 0xd60
			 0x0c952880, /// 0xd64
			 0xd0232416, /// 0xd68
			 0x32e4f359, /// 0xd6c
			 0x89ae9442, /// 0xd70
			 0x613e2c0a, /// 0xd74
			 0x08528bf0, /// 0xd78
			 0xb595638a, /// 0xd7c
			 0x160b982c, /// 0xd80
			 0x10555454, /// 0xd84
			 0x7b90fe33, /// 0xd88
			 0x1755b907, /// 0xd8c
			 0x8b53c5cf, /// 0xd90
			 0xf45bec8c, /// 0xd94
			 0xcf5da0db, /// 0xd98
			 0x4d57b401, /// 0xd9c
			 0x9705ac33, /// 0xda0
			 0xc46af53f, /// 0xda4
			 0x2984c2dc, /// 0xda8
			 0x7ca6376d, /// 0xdac
			 0xc22bbc48, /// 0xdb0
			 0x1504d6f0, /// 0xdb4
			 0x1cd374b7, /// 0xdb8
			 0x76a26972, /// 0xdbc
			 0x7d69919b, /// 0xdc0
			 0x8dd3015e, /// 0xdc4
			 0x1d19b395, /// 0xdc8
			 0x6c09a489, /// 0xdcc
			 0x1f6c99e8, /// 0xdd0
			 0xea77a056, /// 0xdd4
			 0xcebedb71, /// 0xdd8
			 0x46e5fb18, /// 0xddc
			 0xa5df6809, /// 0xde0
			 0xd86a86e5, /// 0xde4
			 0xd77870c1, /// 0xde8
			 0x7128d5c8, /// 0xdec
			 0x02dd709b, /// 0xdf0
			 0x93768306, /// 0xdf4
			 0x638078d3, /// 0xdf8
			 0xa0ed5ab2, /// 0xdfc
			 0x353de8ff, /// 0xe00
			 0xaa2e37ba, /// 0xe04
			 0x5258ed92, /// 0xe08
			 0x1b11194c, /// 0xe0c
			 0x335c54b8, /// 0xe10
			 0x84bf4820, /// 0xe14
			 0x57ee7eb0, /// 0xe18
			 0x4a4edf50, /// 0xe1c
			 0xb98bec64, /// 0xe20
			 0x9c5599c9, /// 0xe24
			 0x5bbf2bf5, /// 0xe28
			 0xf8ebfceb, /// 0xe2c
			 0x9103ae47, /// 0xe30
			 0x6bd4be6c, /// 0xe34
			 0x2fc891c3, /// 0xe38
			 0xec1b0704, /// 0xe3c
			 0x327115b3, /// 0xe40
			 0xdae691f6, /// 0xe44
			 0xbcf5fa77, /// 0xe48
			 0x44f08365, /// 0xe4c
			 0x16b12d7b, /// 0xe50
			 0xb8975319, /// 0xe54
			 0x5c6774ef, /// 0xe58
			 0x60706c57, /// 0xe5c
			 0x29e08125, /// 0xe60
			 0xf94ad2a5, /// 0xe64
			 0x5c1e8933, /// 0xe68
			 0xc765e98b, /// 0xe6c
			 0x407449c1, /// 0xe70
			 0x87156f20, /// 0xe74
			 0x3c75fba1, /// 0xe78
			 0x3d68ae66, /// 0xe7c
			 0x8bbe2dc8, /// 0xe80
			 0xe1d76774, /// 0xe84
			 0xcca4747a, /// 0xe88
			 0xff0985bd, /// 0xe8c
			 0x18d90c09, /// 0xe90
			 0x8eca33a7, /// 0xe94
			 0xfbd47c3e, /// 0xe98
			 0x17b9a438, /// 0xe9c
			 0x8e36d5c7, /// 0xea0
			 0xd017f634, /// 0xea4
			 0xae2dfecc, /// 0xea8
			 0x387c3a14, /// 0xeac
			 0x3cac1a54, /// 0xeb0
			 0x9096334e, /// 0xeb4
			 0x48ede500, /// 0xeb8
			 0xed02e931, /// 0xebc
			 0x29f03709, /// 0xec0
			 0xaa80624c, /// 0xec4
			 0x46d684a5, /// 0xec8
			 0xa5ec6316, /// 0xecc
			 0x20d3a85b, /// 0xed0
			 0x087bc8d5, /// 0xed4
			 0x0c003be4, /// 0xed8
			 0xd15e0a45, /// 0xedc
			 0x48c16db9, /// 0xee0
			 0x1c85fa56, /// 0xee4
			 0x01c1458d, /// 0xee8
			 0xa8753a50, /// 0xeec
			 0xab91807e, /// 0xef0
			 0x838c30bf, /// 0xef4
			 0x91d46291, /// 0xef8
			 0xa7039268, /// 0xefc
			 0xd9ea4b93, /// 0xf00
			 0x720492f1, /// 0xf04
			 0xa6c8aa92, /// 0xf08
			 0x36b0e92d, /// 0xf0c
			 0x05997297, /// 0xf10
			 0xf6e1ec71, /// 0xf14
			 0xff8a6a00, /// 0xf18
			 0x88ec4c04, /// 0xf1c
			 0xc0e85f62, /// 0xf20
			 0xdbf4b55c, /// 0xf24
			 0xac133077, /// 0xf28
			 0x2c239f16, /// 0xf2c
			 0xfd2b7e8d, /// 0xf30
			 0xf43c4ae2, /// 0xf34
			 0x643cf0bb, /// 0xf38
			 0xd89f0fc3, /// 0xf3c
			 0x247ca603, /// 0xf40
			 0x2548399b, /// 0xf44
			 0x6cdaddf0, /// 0xf48
			 0x894e6920, /// 0xf4c
			 0xd7fd2f7f, /// 0xf50
			 0x99f427bb, /// 0xf54
			 0x436874fe, /// 0xf58
			 0xe6538dff, /// 0xf5c
			 0xa086c80f, /// 0xf60
			 0x2bde596e, /// 0xf64
			 0xa93e56d8, /// 0xf68
			 0x18f75ac6, /// 0xf6c
			 0x937219c5, /// 0xf70
			 0xd5ad679a, /// 0xf74
			 0xb8963dc8, /// 0xf78
			 0x4d1d57b0, /// 0xf7c
			 0x48140d0d, /// 0xf80
			 0xe273983a, /// 0xf84
			 0x34a35d1b, /// 0xf88
			 0xd2a4187d, /// 0xf8c
			 0xc2b022f2, /// 0xf90
			 0xa0838666, /// 0xf94
			 0x80313cee, /// 0xf98
			 0x62f6caa1, /// 0xf9c
			 0x3117574c, /// 0xfa0
			 0x34e94344, /// 0xfa4
			 0xc942200c, /// 0xfa8
			 0xea1cf05f, /// 0xfac
			 0xba9864a0, /// 0xfb0
			 0xb63af5f4, /// 0xfb4
			 0xb005accc, /// 0xfb8
			 0x34f67fc3, /// 0xfbc
			 0x8769cd62, /// 0xfc0
			 0x66cabe60, /// 0xfc4
			 0x66317d60, /// 0xfc8
			 0x94e09fe2, /// 0xfcc
			 0xaa364de5, /// 0xfd0
			 0x1af33eb8, /// 0xfd4
			 0x2768eda0, /// 0xfd8
			 0x2289631d, /// 0xfdc
			 0x4d93f1f0, /// 0xfe0
			 0x11f2cf8d, /// 0xfe4
			 0xbc978556, /// 0xfe8
			 0x58516dab, /// 0xfec
			 0xde8aa206, /// 0xff0
			 0xae2d79d3, /// 0xff4
			 0x808dde4b, /// 0xff8
			 0xa6f2b6c0 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00000
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00008
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00010
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00014
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00018
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00020
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00028
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00034
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00038
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0003c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00040
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00048
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0004c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00050
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0005c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00068
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00070
			 0x0c600000,                                                  // Leading 1s:                                 /// 00074
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00078
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0007c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00088
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0008c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xffc00001,                                                  // -signaling NaN                              /// 00094
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0009c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0010c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00110
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00114
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00124
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00130
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00134
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00138
			 0x0c780000,                                                  // Leading 1s:                                 /// 0013c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00140
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00144
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00148
			 0x00000000,                                                  // zero                                        /// 0014c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00154
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00158
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0015c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00164
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00178
			 0x0c600000,                                                  // Leading 1s:                                 /// 0017c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00180
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00184
			 0x0e000001,                                                  // Trailing 1s:                                /// 00188
			 0x80011111,                                                  // -9.7958E-41                                 /// 0018c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00190
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00194
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00198
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0019c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x3f028f5c,                                                  // 0.51                                        /// 001a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xff800000,                                                  // -inf                                        /// 001b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001d0
			 0xff800000,                                                  // -inf                                        /// 001d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001dc
			 0x3f028f5c,                                                  // 0.51                                        /// 001e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00200
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00208
			 0x0e000001,                                                  // Trailing 1s:                                /// 0020c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00224
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00228
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0022c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00230
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00234
			 0x0e000001,                                                  // Trailing 1s:                                /// 00238
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0023c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00244
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00248
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0024c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x33333333,                                                  // 4 random values                             /// 00254
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0x0c600000,                                                  // Leading 1s:                                 /// 00264
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00270
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00278
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00284
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00288
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0028c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00298
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0029c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00300
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00304
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00308
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0030c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00310
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00314
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0031c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00320
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00324
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0032c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00330
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0e000003,                                                  // Trailing 1s:                                /// 00338
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0033c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0034c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00350
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00358
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0035c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0036c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x7fc00001,                                                  // signaling NaN                               /// 00380
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00384
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x00011111,                                                  // 9.7958E-41                                  /// 0038c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00390
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00394
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00398
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0039c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00400
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00404
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00408
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00418
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00420
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00424
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0042c
			 0x7fc00001,                                                  // signaling NaN                               /// 00430
			 0x0e000007,                                                  // Trailing 1s:                                /// 00434
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0043c
			 0xcb000000,                                                  // -8388608.0                                  /// 00440
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00444
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00448
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00450
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00454
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00458
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0045c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00460
			 0x4b000000,                                                  // 8388608.0                                   /// 00464
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00468
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0046c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00474
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00478
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00480
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00484
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0049c
			 0x80000000,                                                  // -zero                                       /// 004a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b4
			 0x3f028f5c,                                                  // 0.51                                        /// 004b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00500
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00508
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00514
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0051c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00520
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00528
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0052c
			 0x3f028f5c,                                                  // 0.51                                        /// 00530
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00534
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00538
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0053c
			 0xbf028f5c,                                                  // -0.51                                       /// 00540
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00544
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00558
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00568
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0056c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0c700000,                                                  // Leading 1s:                                 /// 00574
			 0x0c700000,                                                  // Leading 1s:                                 /// 00578
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0057c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00580
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00584
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00588
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0058c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00590
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00598
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0059c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005ac
			 0x00000000,                                                  // zero                                        /// 005b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x7fc00001,                                                  // signaling NaN                               /// 005d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0xbf028f5c,                                                  // -0.51                                       /// 005f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00600
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00608
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0060c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00610
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00614
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0061c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00620
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00628
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00630
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00634
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00638
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0063c
			 0x7f800000,                                                  // inf                                         /// 00640
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00648
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x33333333,                                                  // 4 random values                             /// 00650
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00654
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00658
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0065c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00664
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00668
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x80000000,                                                  // -zero                                       /// 00670
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00674
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00680
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00688
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00694
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0069c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006bc
			 0x55555555,                                                  // 4 random values                             /// 006c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00700
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00710
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00714
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0071c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000000,                                                  // -zero                                       /// 00724
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00728
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0072c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00738
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0073c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00740
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00748
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00750
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00754
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00768
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0076c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00770
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00774
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0077c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00780
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00784
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00788
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00790
			 0x0c780000,                                                  // Leading 1s:                                 /// 00794
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x33333333,                                                  // 4 random values                             /// 007a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x33333333,                                                  // 4 random values                             /// 007c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00800
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00808
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00818
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0081c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00820
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00824
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0082c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0083c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00854
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00858
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0e000003,                                                  // Trailing 1s:                                /// 00868
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00870
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0xbf028f5c,                                                  // -0.51                                       /// 0087c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x80011111,                                                  // -9.7958E-41                                 /// 00884
			 0x80011111,                                                  // -9.7958E-41                                 /// 00888
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0088c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00894
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0089c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008b4
			 0xbf028f5c,                                                  // -0.51                                       /// 008b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008e4
			 0x00000000,                                                  // zero                                        /// 008e8
			 0x7fc00001,                                                  // signaling NaN                               /// 008ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008fc
			 0x00000000,                                                  // zero                                        /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00908
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0090c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00910
			 0x0e000003,                                                  // Trailing 1s:                                /// 00914
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0091c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00920
			 0x55555555,                                                  // 4 random values                             /// 00924
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00928
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0092c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00930
			 0x00000000,                                                  // zero                                        /// 00934
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80000000,                                                  // -zero                                       /// 00940
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0094c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00950
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00954
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00960
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00970
			 0x7f800000,                                                  // inf                                         /// 00974
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0097c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00980
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00984
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0098c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00990
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00994
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009b4
			 0xcb000000,                                                  // -8388608.0                                  /// 009b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e8
			 0x55555555,                                                  // 4 random values                             /// 009ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xffc00001,                                                  // -signaling NaN                              /// 009f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a18
			 0x7f800000,                                                  // inf                                         /// 00a1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a30
			 0xcb000000,                                                  // -8388608.0                                  /// 00a34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0xff800000,                                                  // -inf                                        /// 00a3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a70
			 0x55555555,                                                  // 4 random values                             /// 00a74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a8c
			 0x55555555,                                                  // 4 random values                             /// 00a90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00abc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00acc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x55555555,                                                  // 4 random values                             /// 00ad4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ae8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xcb000000,                                                  // -8388608.0                                  /// 00afc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x33333333,                                                  // 4 random values                             /// 00b5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b8c
			 0x55555555,                                                  // 4 random values                             /// 00b90
			 0x7f800000,                                                  // inf                                         /// 00b94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ba0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x7f800000,                                                  // inf                                         /// 00ba8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bec
			 0x7f800000,                                                  // inf                                         /// 00bf0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c88
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ca8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ccc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xbf028f5c,                                                  // -0.51                                       /// 00d24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0x55555555,                                                  // 4 random values                             /// 00d34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d90
			 0x33333333,                                                  // 4 random values                             /// 00d94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00db4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db8
			 0x55555555,                                                  // 4 random values                             /// 00dbc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00de4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00de8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00df0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80000000,                                                  // -zero                                       /// 00dfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x55555555,                                                  // 4 random values                             /// 00e68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x7f800000,                                                  // inf                                         /// 00e74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e80
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ea0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eb0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ecc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed0
			 0x33333333,                                                  // 4 random values                             /// 00ed4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ef0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ef8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00efc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f14
			 0x7fc00001,                                                  // signaling NaN                               /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x7f800000,                                                  // inf                                         /// 00f7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe4
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fec
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ff4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ff8
			 0x807fffff // max subnorm                                   // SP - ve numbers                             /// last
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
			 0x0000015c,
			 0x00000094,
			 0x0000079c,
			 0x000001d8,
			 0x000001f8,
			 0x00000174,
			 0x0000071c,
			 0x000003ac,

			 /// vpu register f2
			 0x41000000,
			 0x40400000,
			 0x41d80000,
			 0x41980000,
			 0x40400000,
			 0x40400000,
			 0x41400000,
			 0x40000000,

			 /// vpu register f3
			 0x40a00000,
			 0x41c00000,
			 0x41b00000,
			 0x41c00000,
			 0x41700000,
			 0x41d00000,
			 0x42000000,
			 0x40c00000,

			 /// vpu register f4
			 0x41300000,
			 0x41c00000,
			 0x40c00000,
			 0x41d00000,
			 0x40000000,
			 0x41f80000,
			 0x40c00000,
			 0x40c00000,

			 /// vpu register f5
			 0x41880000,
			 0x41100000,
			 0x41f00000,
			 0x40e00000,
			 0x41e80000,
			 0x41b80000,
			 0x41e00000,
			 0x40a00000,

			 /// vpu register f6
			 0x41980000,
			 0x40a00000,
			 0x42000000,
			 0x3f800000,
			 0x41200000,
			 0x41b00000,
			 0x41300000,
			 0x41100000,

			 /// vpu register f7
			 0x41e80000,
			 0x41880000,
			 0x41500000,
			 0x41e80000,
			 0x40c00000,
			 0x41d80000,
			 0x41200000,
			 0x41d00000,

			 /// vpu register f8
			 0x41c00000,
			 0x40800000,
			 0x41500000,
			 0x41a00000,
			 0x40000000,
			 0x41b80000,
			 0x41880000,
			 0x40000000,

			 /// vpu register f9
			 0x41880000,
			 0x41900000,
			 0x41800000,
			 0x41600000,
			 0x41980000,
			 0x41f80000,
			 0x40400000,
			 0x41e80000,

			 /// vpu register f10
			 0x41b80000,
			 0x41980000,
			 0x41800000,
			 0x42000000,
			 0x40a00000,
			 0x41300000,
			 0x3f800000,
			 0x41e80000,

			 /// vpu register f11
			 0x3f800000,
			 0x41f00000,
			 0x40400000,
			 0x41100000,
			 0x41400000,
			 0x40400000,
			 0x41c00000,
			 0x41a80000,

			 /// vpu register f12
			 0x41880000,
			 0x41d80000,
			 0x41b80000,
			 0x41f80000,
			 0x41900000,
			 0x41f00000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f13
			 0x41d00000,
			 0x41b00000,
			 0x41a00000,
			 0x40a00000,
			 0x41a00000,
			 0x41f80000,
			 0x41400000,
			 0x41f00000,

			 /// vpu register f14
			 0x40e00000,
			 0x41880000,
			 0x41500000,
			 0x41d80000,
			 0x41c80000,
			 0x42000000,
			 0x41e00000,
			 0x41800000,

			 /// vpu register f15
			 0x41b80000,
			 0x41000000,
			 0x41c80000,
			 0x41a80000,
			 0x41900000,
			 0x41c00000,
			 0x41800000,
			 0x40000000,

			 /// vpu register f16
			 0x40c00000,
			 0x41b00000,
			 0x41500000,
			 0x41700000,
			 0x40c00000,
			 0x41d00000,
			 0x41f00000,
			 0x41400000,

			 /// vpu register f17
			 0x41c00000,
			 0x40000000,
			 0x3f800000,
			 0x41300000,
			 0x41300000,
			 0x41f00000,
			 0x41000000,
			 0x41d00000,

			 /// vpu register f18
			 0x41500000,
			 0x40400000,
			 0x41c00000,
			 0x41700000,
			 0x40400000,
			 0x41400000,
			 0x41b80000,
			 0x41c80000,

			 /// vpu register f19
			 0x40000000,
			 0x41600000,
			 0x41900000,
			 0x41b00000,
			 0x40400000,
			 0x40800000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f20
			 0x41a80000,
			 0x41a80000,
			 0x41b00000,
			 0x41b80000,
			 0x40400000,
			 0x40800000,
			 0x41b00000,
			 0x41d00000,

			 /// vpu register f21
			 0x41a80000,
			 0x41b80000,
			 0x41880000,
			 0x40a00000,
			 0x41b80000,
			 0x41b80000,
			 0x40a00000,
			 0x41880000,

			 /// vpu register f22
			 0x41a80000,
			 0x41a80000,
			 0x41d80000,
			 0x41e00000,
			 0x41900000,
			 0x40800000,
			 0x3f800000,
			 0x41200000,

			 /// vpu register f23
			 0x41c00000,
			 0x41200000,
			 0x41500000,
			 0x41f80000,
			 0x41b80000,
			 0x40e00000,
			 0x41b00000,
			 0x40400000,

			 /// vpu register f24
			 0x40e00000,
			 0x41a00000,
			 0x42000000,
			 0x41300000,
			 0x41000000,
			 0x41d80000,
			 0x41e80000,
			 0x41d00000,

			 /// vpu register f25
			 0x41f00000,
			 0x41700000,
			 0x40a00000,
			 0x41d80000,
			 0x41000000,
			 0x41a00000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f26
			 0x41b00000,
			 0x41b80000,
			 0x41200000,
			 0x41c80000,
			 0x41d00000,
			 0x40000000,
			 0x42000000,
			 0x41980000,

			 /// vpu register f27
			 0x40e00000,
			 0x41980000,
			 0x41880000,
			 0x41600000,
			 0x41880000,
			 0x3f800000,
			 0x40a00000,
			 0x41c80000,

			 /// vpu register f28
			 0x41e80000,
			 0x41400000,
			 0x41e80000,
			 0x41700000,
			 0x40a00000,
			 0x41f00000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f29
			 0x41a00000,
			 0x41700000,
			 0x42000000,
			 0x40c00000,
			 0x40c00000,
			 0x41f00000,
			 0x41d00000,
			 0x41f80000,

			 /// vpu register f30
			 0x3f800000,
			 0x41700000,
			 0x40800000,
			 0x40a00000,
			 0x41c00000,
			 0x41f00000,
			 0x41200000,
			 0x41100000,

			 /// vpu register f31
			 0x40400000,
			 0x40c00000,
			 0x41000000,
			 0x40c00000,
			 0x41e80000,
			 0x41100000,
			 0x41c80000,
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
		"famominul.pi f18, f1 (x11)\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f16, f1 (x14)\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f17, f1 (x13)\n"                        ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f21, f1 (x13)\n"                        ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x11)\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f7, f1 (x12)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f19, f1 (x13)\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f27, f1 (x11)\n"                        ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f7, f1 (x14)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f10, f1 (x15)\n"                        ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f18, f1 (x12)\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f24, f1 (x12)\n"                        ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f13, f1 (x11)\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x13)\n"                        ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f22, f1 (x11)\n"                        ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f16, f1 (x14)\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f22, f1 (x15)\n"                        ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f21, f1 (x15)\n"                        ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f10, f1 (x12)\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f4, f1 (x11)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f25, f1 (x14)\n"                        ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f24, f1 (x12)\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f13, f1 (x11)\n"                        ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f23, f1 (x15)\n"                        ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f12, f1 (x15)\n"                        ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f30, f1 (x12)\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x15)\n"                        ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f21, f1 (x14)\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f24, f1 (x14)\n"                        ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f30, f1 (x11)\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f23, f1 (x11)\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f24, f1 (x15)\n"                        ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f17, f1 (x15)\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f19, f1 (x11)\n"                        ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x11)\n"                        ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f22, f1 (x15)\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f18, f1 (x15)\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x11)\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f27, f1 (x15)\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f10, f1 (x12)\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f4, f1 (x14)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x14)\n"                        ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f14, f1 (x14)\n"                        ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f17, f1 (x15)\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f13, f1 (x14)\n"                        ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f10, f1 (x15)\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f15, f1 (x13)\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x14)\n"                        ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f17, f1 (x13)\n"                        ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f20, f1 (x15)\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x13)\n"                        ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f18, f1 (x12)\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f7, f1 (x14)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f12, f1 (x15)\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f19, f1 (x15)\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f7, f1 (x12)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f21, f1 (x12)\n"                        ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f25, f1 (x14)\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f25, f1 (x11)\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f27, f1 (x14)\n"                        ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x15)\n"                        ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f8, f1 (x14)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f2, f1 (x13)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f25, f1 (x12)\n"                        ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f22, f1 (x11)\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x11)\n"                        ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x11)\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x12)\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f19, f1 (x12)\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f6, f1 (x11)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x12)\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f27, f1 (x14)\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f14, f1 (x11)\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f8, f1 (x15)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f19, f1 (x14)\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f18, f1 (x15)\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x11)\n"                        ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f4, f1 (x13)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f8, f1 (x11)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f27, f1 (x15)\n"                        ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f25, f1 (x12)\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f8, f1 (x14)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f17, f1 (x15)\n"                        ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f16, f1 (x13)\n"                        ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f4, f1 (x11)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f5, f1 (x12)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f2, f1 (x14)\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f29, f1 (x14)\n"                        ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f3, f1 (x11)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f19, f1 (x14)\n"                        ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x11)\n"                        ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f11, f1 (x14)\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x12)\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f13, f1 (x14)\n"                        ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f12, f1 (x14)\n"                        ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f2, f1 (x14)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x14)\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f21, f1 (x14)\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominul.pi f26, f1 (x13)\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
